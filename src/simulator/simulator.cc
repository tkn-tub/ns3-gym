/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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
std::cout << "SIMU TRACE " << Simulator::nowS () << " " << x << std::endl;
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

	void enableLogTo (char const *filename);

	bool isFinished (void) const;
	Time next (void) const;
	void stop (void);
	void stopAt (Time time);
	EventId schedule (Time time, EventImpl *event);
	void remove (EventId ev);
	void cancel (EventId ev);
	bool isExpired (EventId ev);
	void run (void);
	Time now (void) const;

private:
	void processOneEvent (void);

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
	while (!m_destroy.empty ()) {
		EventImpl *ev = m_destroy.front ().first;
		m_destroy.pop_front ();
		TRACE ("handle destroy " << ev);
		ev->invoke ();
		delete ev;
	}
	delete m_events;
	m_events = (Scheduler *)0xdeadbeaf;
}


void
SimulatorPrivate::enableLogTo (char const *filename)
{
	m_log.open (filename);
	m_logEnable = true;
}

void
SimulatorPrivate::processOneEvent (void)
{
	EventImpl *nextEv = m_events->peekNext ();
	Scheduler::EventKey nextKey = m_events->peekNextKey ();
	m_events->removeNext ();
	TRACE ("handle " << nextEv);
	m_currentNs = nextKey.m_ns;
	m_currentUid = nextKey.m_uid;
	if (m_logEnable) {
		m_log << "e "<<nextKey.m_uid << " " << nextKey.m_ns << std::endl;
	}
	nextEv->invoke ();
	delete nextEv;
}

bool 
SimulatorPrivate::isFinished (void) const
{
	return m_events->isEmpty ();
}
Time
SimulatorPrivate::next (void) const
{
	assert (!m_events->isEmpty ());
	Scheduler::EventKey nextKey = m_events->peekNextKey ();
	return Time::absNs (nextKey.m_ns);
}


void
SimulatorPrivate::run (void)
{
	while (!m_events->isEmpty () && !m_stop && 
	       (m_stopAt == 0 || m_stopAt > next ().ns ())) {
		processOneEvent ();
	}
	m_log.close ();
}


void 
SimulatorPrivate::stop (void)
{
	m_stop = true;
}
void 
SimulatorPrivate::stopAt (Time at)
{
	m_stopAt = at.ns ();
}
EventId
SimulatorPrivate::schedule (Time time, EventImpl *event)
{
	if (time.isDestroy ()) {
		m_destroy.push_back (std::make_pair (event, m_uid));
		if (m_logEnable) {
			m_log << "id " << m_currentUid << " " << now ().ns () << " "
			      << m_uid << std::endl;
		}
		m_uid++;
		//XXX
		return EventId ();
	}
	assert (time.ns () >= now ().ns ());
	Scheduler::EventKey key = {time.ns (), m_uid};
	if (m_logEnable) {
		m_log << "i "<<m_currentUid<<" "<<now ().ns ()<<" "
		      <<m_uid<<" "<<time.ns () << std::endl;
	}
	m_uid++;
	return m_events->insert (event, key);
}
Time
SimulatorPrivate::now (void) const
{
	return Time::absNs (m_currentNs);
}

void
SimulatorPrivate::remove (EventId ev)
{
	Scheduler::EventKey key;
	EventImpl *impl = m_events->remove (ev, &key);
	delete impl;
	if (m_logEnable) {
		m_log << "r " << m_currentUid << " " << now ().ns () << " "
		      << key.m_uid << " " << key.m_ns << std::endl;
	}
}

void
SimulatorPrivate::cancel (EventId id)
{
	assert (m_events->isValid (id));
	EventImpl *ev = id.getEventImpl ();
	ev->cancel ();
}

bool
SimulatorPrivate::isExpired (EventId ev)
{
	if (ev.getEventImpl () != 0 &&
	    ev.getNs () <= now ().ns () &&
	    ev.getUid () < m_currentUid) {
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

void Simulator::setLinkedList (void)
{
	m_listType = LINKED_LIST;
}
void Simulator::setBinaryHeap (void)
{
	m_listType = BINARY_HEAP;
}
void Simulator::setStdMap (void)
{
	m_listType = STD_MAP;
}
void 
Simulator::setExternal (SchedulerFactory const*factory)
{
	assert (factory != 0);
	m_schedFactory = factory;
	m_listType = EXTERNAL;
}
void Simulator::enableLogTo (char const *filename)
{
	getPriv ()->enableLogTo (filename);
}


SimulatorPrivate *
Simulator::getPriv (void)
{
	if (m_priv == 0) {
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
			events = m_schedFactory->create ();
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
Simulator::destroy (void)
{
	delete m_priv;
	m_priv = 0;
}


bool 
Simulator::isFinished (void)
{
	return getPriv ()->isFinished ();
}
Time
Simulator::next (void)
{
	return getPriv ()->next ();
}


void 
Simulator::run (void)
{
	getPriv ()->run ();
}
void 
Simulator::stop (void)
{
	TRACE ("stop");
	getPriv ()->stop ();
}
void 
Simulator::stopAt (Time at)
{
	getPriv ()->stopAt (at);
}
Time
Simulator::now (void)
{
	return getPriv ()->now ();
}

EventId
Simulator::schedule (Time time, EventImpl *ev)
{
	return getPriv ()->schedule (time, ev);
}

void
Simulator::remove (EventId ev)
{
	return getPriv ()->remove (ev);
}

void
Simulator::cancel (EventId ev)
{
	return getPriv ()->cancel (ev);
}
bool 
Simulator::isExpired (EventId id)
{
	return getPriv ()->isExpired (id);
}

}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class SimulatorTests : public Test {
public:
	SimulatorTests ();
	virtual ~SimulatorTests ();
	virtual bool runTests (void);
private:
	void a (int a);
	void b (int b);
	void c (int c);
	void d (int d);
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
SimulatorTests::a (int a)
{
	m_a = false;
}
void
SimulatorTests::b (int b)
{
	if (b != 2 || Simulator::now ().us () != 11) {
		m_b = false;
	} else {
		m_b = true;
	}
	Simulator::remove (m_idC);
	Simulator::schedule (Time::relUs (10), &SimulatorTests::d, this, 4);
}
void
SimulatorTests::c (int c)
{
	m_c = false;
}
void
SimulatorTests::d (int d)
{
	if (d != 4 || Simulator::now ().us () != (11+10)) {
		m_d = false;
	} else {
		m_d = true;
	}
}
bool 
SimulatorTests::runTests (void)
{
	bool ok = true;
	m_a = true;
	m_b = false;
	m_c = true;
	m_d = false;
	EventId a = Simulator::schedule (Time::absUs (10), &SimulatorTests::a, this, 1);
	EventId b = Simulator::schedule (Time::absUs (11), &SimulatorTests::b, this, 2);
	m_idC = Simulator::schedule (Time::absUs (12), &SimulatorTests::c, this, 3);

	Simulator::cancel (a);
	Simulator::run ();

	if (!m_a || !m_b || !m_c || !m_d) {
		ok = false;
	}

	return ok;
}

SimulatorTests gSimulatorTests;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */



