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
std::cout << "SIMU TRACE " << Simulator::now_s () << " " << x << std::endl;
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

	void enable_log_to (char const *filename);

	bool is_finished (void) const;
	Time next (void) const;
	void stop (void);
	void stop_at (Time time);
	EventId schedule (Time time, EventImpl *event);
	void remove (EventId ev);
	void cancel (EventId ev);
	bool is_expired (EventId ev);
	void run (void);
	Time now (void) const;

private:
	void process_one_event (void);

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
SimulatorPrivate::enable_log_to (char const *filename)
{
	m_log.open (filename);
	m_logEnable = true;
}

void
SimulatorPrivate::process_one_event (void)
{
	EventImpl *next_ev = m_events->peek_next ();
	Scheduler::EventKey next_key = m_events->peek_next_key ();
	m_events->remove_next ();
	TRACE ("handle " << next_ev);
	m_currentNs = next_key.m_ns;
	m_currentUid = next_key.m_uid;
	if (m_logEnable) {
		m_log << "e "<<next_key.m_uid << " " << next_key.m_ns << std::endl;
	}
	next_ev->invoke ();
	delete next_ev;
}

bool 
SimulatorPrivate::is_finished (void) const
{
	return m_events->is_empty ();
}
Time
SimulatorPrivate::next (void) const
{
	assert (!m_events->is_empty ());
	Scheduler::EventKey next_key = m_events->peek_next_key ();
	return Time::abs_ns (next_key.m_ns);
}


void
SimulatorPrivate::run (void)
{
	while (!m_events->is_empty () && !m_stop && 
	       (m_stopAt == 0 || m_stopAt > next ().ns ())) {
		process_one_event ();
	}
	m_log.close ();
}


void 
SimulatorPrivate::stop (void)
{
	m_stop = true;
}
void 
SimulatorPrivate::stop_at (Time at)
{
	m_stopAt = at.ns ();
}
EventId
SimulatorPrivate::schedule (Time time, EventImpl *event)
{
	if (time.is_destroy ()) {
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
	return Time::abs_ns (m_currentNs);
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
	assert (m_events->is_valid (id));
	EventImpl *ev = id.get_event_impl ();
	ev->cancel ();
}

bool
SimulatorPrivate::is_expired (EventId ev)
{
	if (ev.get_event_impl () != 0 &&
	    ev.get_ns () <= now ().ns () &&
	    ev.get_uid () < m_currentUid) {
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

void Simulator::set_linked_list (void)
{
	m_listType = LINKED_LIST;
}
void Simulator::set_binary_heap (void)
{
	m_listType = BINARY_HEAP;
}
void Simulator::set_std_map (void)
{
	m_listType = STD_MAP;
}
void 
Simulator::set_external (SchedulerFactory const*factory)
{
	assert (factory != 0);
	m_schedFactory = factory;
	m_listType = EXTERNAL;
}
void Simulator::enable_log_to (char const *filename)
{
	get_priv ()->enable_log_to (filename);
}


SimulatorPrivate *
Simulator::get_priv (void)
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
Simulator::is_finished (void)
{
	return get_priv ()->is_finished ();
}
Time
Simulator::next (void)
{
	return get_priv ()->next ();
}


void 
Simulator::run (void)
{
	get_priv ()->run ();
}
void 
Simulator::stop (void)
{
	TRACE ("stop");
	get_priv ()->stop ();
}
void 
Simulator::stop_at (Time at)
{
	get_priv ()->stop_at (at);
}
Time
Simulator::now (void)
{
	return get_priv ()->now ();
}

EventId
Simulator::schedule (Time time, EventImpl *ev)
{
	return get_priv ()->schedule (time, ev);
}

void
Simulator::remove (EventId ev)
{
	return get_priv ()->remove (ev);
}

void
Simulator::cancel (EventId ev)
{
	return get_priv ()->cancel (ev);
}
bool 
Simulator::is_expired (EventId id)
{
	return get_priv ()->is_expired (id);
}

}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class SimulatorTests : public Test {
public:
	SimulatorTests ();
	virtual ~SimulatorTests ();
	virtual bool run_tests (void);
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
	Simulator::schedule (Time::rel_us (10), &SimulatorTests::d, this, 4);
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
SimulatorTests::run_tests (void)
{
	bool ok = true;
	m_a = true;
	m_b = false;
	m_c = true;
	m_d = false;
	EventId a = Simulator::schedule (Time::abs_us (10), &SimulatorTests::a, this, 1);
	EventId b = Simulator::schedule (Time::abs_us (11), &SimulatorTests::b, this, 2);
	m_idC = Simulator::schedule (Time::abs_us (12), &SimulatorTests::c, this, 3);

	Simulator::cancel (a);
	Simulator::run ();

	if (!m_a || !m_b || !m_c || !m_d) {
		ok = false;
	}

	return ok;
}

SimulatorTests g_simulator_tests;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */



