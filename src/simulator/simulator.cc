/* -*- Mode:NS3; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "simulator.h"
#include "scheduler.h"
#include "event-impl.h"

#include <math.h>
#include <cassert>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>

#define noTRACE_SIMU 1

#ifdef TRACE_SIMU
#include <iostream>
# define TRACE(x) \
std::Cout << "SIMU TRACE " << Simulator::nowS () << " " << x << std::endl;
# define TRACE_S(x) \
std::cout << "SIMU TRACE " << x << std::endl;
#else /* TRACE_SIMU */
# define TRACE(format,...)
# define TRACE_S(format,...)
#endif /* TRACE_SIMU */


namespace ns3 {


class SimulatorPrivate {
public:
    SimulatorPrivate (Scheduler *events);
    ~SimulatorPrivate ();

    void EnableLogTo (char const *filename);

    bool IsFinished (void) const;
    Time Next (void) const;
    void Stop (void);
    void StopAt (Time const &time);
    EventId Schedule (Time const &time, EventImpl *event);
    void Remove (EventId ev);
    void Cancel (EventId ev);
    bool IsExpired (EventId ev);
    void Run (void);
    Time Now (void) const;

private:
    void ProcessOneEvent (void);

    typedef std::list<std::pair<EventImpl *,uint32_t> > Events;
    Events m_destroy;
    uint64_t m_stopAt;
    bool m_stop;
    Scheduler *m_events;
    uint32_t m_uid;
    uint32_t m_currentUid;
    uint64_t m_currentNs;
    std::ofstream m_log;
    std::ifstream m_inputLog;
    bool m_logEnable;
};




SimulatorPrivate::SimulatorPrivate (Scheduler *events)
{
    m_stop = false;
    m_stopAt = 0;
    m_events = events;
    m_uid = 0;    
    m_logEnable = false;
    m_currentNs = 0;
}

SimulatorPrivate::~SimulatorPrivate ()
{
    while (!m_destroy.empty ()) 
      {
        EventImpl *ev = m_destroy.front ().first;
        m_destroy.pop_front ();
        TRACE ("handle destroy " << ev);
        ev->Invoke ();
        delete ev;
      }
    delete m_events;
    m_events = (Scheduler *)0xdeadbeaf;
}


void
SimulatorPrivate::EnableLogTo (char const *filename)
{
    m_log.open (filename);
    m_logEnable = true;
}

void
SimulatorPrivate::ProcessOneEvent (void)
{
    EventImpl *nextEv = m_events->PeekNext ();
    Scheduler::EventKey nextKey = m_events->PeekNextKey ();
    m_events->RemoveNext ();
    TRACE ("handle " << nextEv);
    m_currentNs = nextKey.m_ns;
    m_currentUid = nextKey.m_uid;
    if (m_logEnable) 
      {
        m_log << "e "<<nextKey.m_uid << " " << nextKey.m_ns << std::endl;
      }
    nextEv->Invoke ();
    delete nextEv;
}

bool 
SimulatorPrivate::IsFinished (void) const
{
    return m_events->IsEmpty ();
}
Time
SimulatorPrivate::Next (void) const
{
    assert (!m_events->IsEmpty ());
    Scheduler::EventKey nextKey = m_events->PeekNextKey ();
    return Time::AbsNs (nextKey.m_ns);
}


void
SimulatorPrivate::Run (void)
{
    while (!m_events->IsEmpty () && !m_stop && 
           (m_stopAt == 0 || m_stopAt > Next ().Ns ())) 
      {
        ProcessOneEvent ();
      }
    m_log.close ();
}


void 
SimulatorPrivate::Stop (void)
{
    m_stop = true;
}
void 
SimulatorPrivate::StopAt (Time const &at)
{
    m_stopAt = at.Ns ();
}
EventId
SimulatorPrivate::Schedule (Time const &time, EventImpl *event)
{
    if (time.IsDestroy ()) 
      {
        m_destroy.push_back (std::make_pair (event, m_uid));
        if (m_logEnable) 
          {
            m_log << "id " << m_currentUid << " " << Now ().Ns () << " "
                  << m_uid << std::endl;
          }
        m_uid++;
        //XXX
        return EventId ();
      }
    assert (time.Ns () >= Now ().Ns ());
    Scheduler::EventKey key = {time.Ns (), m_uid};
    if (m_logEnable) 
      {
        m_log << "i "<<m_currentUid<<" "<<Now ().Ns ()<<" "
              <<m_uid<<" "<<time.Ns () << std::endl;
      }
    m_uid++;
    return m_events->Insert (event, key);
}
Time
SimulatorPrivate::Now (void) const
{
    return Time::AbsNs (m_currentNs);
}

void
SimulatorPrivate::Remove (EventId ev)
{
    Scheduler::EventKey key;
    EventImpl *impl = m_events->Remove (ev, &key);
    delete impl;
    if (m_logEnable) 
      {
        m_log << "r " << m_currentUid << " " << Now ().Ns () << " "
              << key.m_uid << " " << key.m_ns << std::endl;
      }
}

void
SimulatorPrivate::Cancel (EventId id)
{
    assert (m_events->IsValid (id));
    EventImpl *ev = id.GetEventImpl ();
    ev->Cancel ();
}

bool
SimulatorPrivate::IsExpired (EventId ev)
{
    if (ev.GetEventImpl () != 0 &&
        ev.GetNs () <= Now ().Ns () &&
        ev.GetUid () < m_currentUid) 
      {
        return false;
      }
    return true;
}


}; // namespace ns3


#include "scheduler-list.h"
#include "scheduler-heap.h"
#include "scheduler-map.h"
#include "scheduler-factory.h"


namespace ns3 {

SimulatorPrivate *Simulator::m_priv = 0;
Simulator::ListType Simulator::m_listType = LINKED_LIST;
SchedulerFactory const*Simulator::m_schedFactory = 0;

void Simulator::SetLinkedList (void)
{
    m_listType = LINKED_LIST;
}
void Simulator::SetBinaryHeap (void)
{
    m_listType = BINARY_HEAP;
}
void Simulator::SetStdMap (void)
{
    m_listType = STD_MAP;
}
void 
Simulator::SetExternal (SchedulerFactory const*factory)
{
    assert (factory != 0);
    m_schedFactory = factory;
    m_listType = EXTERNAL;
}
void Simulator::EnableLogTo (char const *filename)
{
    GetPriv ()->EnableLogTo (filename);
}


SimulatorPrivate *
Simulator::GetPriv (void)
{
    if (m_priv == 0) 
      {
        Scheduler *events;
        switch (m_listType) {
        case LINKED_LIST:
            events = new SchedulerList ();
            break;
        case BINARY_HEAP:
            events = new SchedulerHeap ();
            break;
        case STD_MAP:
            events = new SchedulerMap ();
            break;
        case EXTERNAL:
            events = m_schedFactory->Create ();
        default: // not reached
            events = 0;
            assert (false); 
            break;
        }
        m_priv = new SimulatorPrivate (events);
      }
    TRACE_S ("priv " << m_priv);
    return m_priv;
}

void
Simulator::Destroy (void)
{
    delete m_priv;
    m_priv = 0;
}


bool 
Simulator::IsFinished (void)
{
    return GetPriv ()->IsFinished ();
}
Time
Simulator::Next (void)
{
    return GetPriv ()->Next ();
}


void 
Simulator::Run (void)
{
    GetPriv ()->Run ();
}
void 
Simulator::Stop (void)
{
    TRACE ("stop");
    GetPriv ()->Stop ();
}
void 
Simulator::StopAt (Time const &at)
{
    GetPriv ()->StopAt (at);
}
Time
Simulator::Now (void)
{
    return GetPriv ()->Now ();
}

EventId
Simulator::Schedule (Time const &time, EventImpl *ev)
{
    return GetPriv ()->Schedule (time, ev);
}
EventId
Simulator::Schedule (Time const &time, void (*f) (void))
{
    // zero arg version
    class EventFunctionImpl0 : public EventImpl {
    public:
    	typedef void (*F)(void);
        
    	EventFunctionImpl0 (F function) 
    		: m_function (function)
    	{}
    	virtual ~EventFunctionImpl0 () {}
    protected:
    	virtual void Notify (void) { 
    		(*m_function) (); 
        }
    private:
    	F m_function;
    } *ev = new EventFunctionImpl0 (f);
    return Schedule (time, ev);
}


void
Simulator::Remove (EventId ev)
{
    return GetPriv ()->Remove (ev);
}

void
Simulator::Cancel (EventId ev)
{
    return GetPriv ()->Cancel (ev);
}
bool 
Simulator::IsExpired (EventId id)
{
    return GetPriv ()->IsExpired (id);
}

}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class SimulatorTests : public Test {
public:
    SimulatorTests ();
    virtual ~SimulatorTests ();
    virtual bool RunTests (void);
private:
    bool RunOneTest (void);
    void A (int a);
    void B (int b);
    void C (int c);
    void D (int d);
    bool m_b;
    bool m_a;
    bool m_c;
    bool m_d;
    EventId m_idC;
};

SimulatorTests::SimulatorTests ()
    : Test ("Simulator")
{}
SimulatorTests::~SimulatorTests ()
{}
void
SimulatorTests::A (int a)
{
    m_a = false;
}
void
SimulatorTests::B (int b)
{
    if (b != 2 || Simulator::Now ().Us () != 11) 
      {
        m_b = false;
      } 
    else 
      {
        m_b = true;
      }
    Simulator::Remove (m_idC);
    Simulator::Schedule (Time::RelUs (10), &SimulatorTests::D, this, 4);
}
void
SimulatorTests::C (int c)
{
    m_c = false;
}
void
SimulatorTests::D (int d)
{
    if (d != 4 || Simulator::Now ().Us () != (11+10)) 
      {
        m_d = false;
      } 
    else 
      {
        m_d = true;
      }
}
bool
SimulatorTests::RunOneTest (void)
{
    bool ok = true;
    m_a = true;
    m_b = false;
    m_c = true;
    m_d = false;

    EventId a = Simulator::Schedule (Time::AbsUs (10), &SimulatorTests::A, this, 1);
    Simulator::Schedule (Time::AbsUs (11), &SimulatorTests::B, this, 2);
    m_idC = Simulator::Schedule (Time::AbsUs (12), &SimulatorTests::C, this, 3);

    Simulator::Cancel (a);
    Simulator::Run ();

    if (!m_a || !m_b || !m_c || !m_d) 
      {
        ok = false;
      }
    return ok;
}
bool 
SimulatorTests::RunTests (void)
{
    bool ok = true;

    Simulator::SetLinkedList ();
    if (!RunOneTest ()) 
      {
        ok = false;
      }
    Simulator::Destroy ();
    Simulator::SetBinaryHeap ();
    if (!RunOneTest ()) 
      {
        ok = false;
      }
    Simulator::Destroy ();
    Simulator::SetStdMap ();
    if (!RunOneTest ()) 
      {
        ok = false;
      }
    Simulator::Destroy ();

    return ok;
}

SimulatorTests gSimulatorTests;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */



