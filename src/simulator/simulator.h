/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>
#include "event-id.h"
#include "event-impl.h"
#include "time.h"

namespace ns3 {


class SimulatorPrivate;

/**
 * \brief Control the scheduling of simulation events. 
 *
 * The internal simulation clock is maintained
 * as a 64-bit integer in microsecond units. This means that it is
 * not possible to specify event expiration times with anything better
 * than microsecond accuracy. Events whose expiration time is
 * the same are scheduled in FIFO order: the first event inserted in the
 * Scheduling queue is scheduled to expire first.
 */
class Simulator {
public:
	/**
	 * Force the use of an event scheduler based on a linked-list.
	 * This method must be invoked before any other method exported
	 * by the Simulator class.
	 *   - insert: O(n)
	 *   - remove: O(1)
	 */
	static void set_linked_list (void);
	/**
	 * Force the use of an event scheduler based on a binary heap.
	 * This method must be invoked before any other method exported
	 * by the Simulator class.
	 *   - insert: O(log(n))
	 *   - remove: O(log(n))
	 */
	static void set_binary_heap (void);
	/**
	 * Force the use of an event scheduler based on a std::map.
	 * This method must be invoked before any other method exported
	 * by the Simulator class.
	 *   - insert: O(log(n))
	 *   - remove: O(log(n))
	 */
	static void set_std_map (void);

	/**
	 * Enable logging to the file identified by filename. If the file
	 * does not exist, it is created. If it exists, it is destroyed and
	 * re-created. Every scheduling event is logged to this file in a
	 * simple text format which can be read back by the event replay
	 * utility. This allows you to record the scheduling behavior of
	 * a simulation, and measure the exact overhead related to
	 * event scheduling with the event replay utility. It is also possible
	 * to compare the performance of every scheduling algorithms on this
	 * specific scheduling load.
	 * This method must be invoked before any call to Simulator::run
	 * @param filename the name of the file to log to 
	 */
	static void enable_log_to (char const *filename);

	/**
	 * Every event scheduled by the Simulator::insert_at_destroy method is
	 * invoked. Then, we ensure that any memory allocated by the 
	 * Simulator is freed.
	 * This method is typically invoked at the end of a simulation
	 * to avoid false-positive reports by a leak checker.
	 * After this method has been invoked, it is actually possible
	 * to restart a new simulation with a set of calls to Simulator::run
	 * and Simulator::insert_*.
	 */
	static void destroy (void);


	/**
	 * If there any any events lefts to be scheduled, return
	 * true. Return false otherwise.
	 */
	static bool is_finished (void);
	/**
	 * If Simulator::is_finished returns true, the behavior of this
	 * method is undefined. Otherwise, it returns the microsecond-based
	 * time of the next event expected to be scheduled.
	 */
	static Time next (void);

	/**
	 * Run the simulation until one of:
	 *   - no events are present anymore
	 *   - the user called Simulator::stop
	 *   - the user called Simulator::stop_at_us and the
	 *     expiration time of the next event to be processed
	 *     is greater than or equal to the stop time.
	 */
	static void run (void);
	/**
	 * If an event invokes this method, it will be the last
	 * event scheduled by the Simulator::run method before
	 * returning to the caller.
	 */
	static void stop (void);
	/**
	 * Force the Simulator::run method to return to the caller
	 * when the expiration time of the next event to be processed 
	 * is greater than or equal to the stop time.
	 * @param at the stop time.
	 */
	static void stop_at (Time time);

	/**
	 * Schedule an event to expire at time.
	 *
	 * @param delta the expiration time of the event.
	 * @param event the event to schedule.
	 * @returns an id for the scheduled event.
	 */
	template <typename T>
	static EventId schedule (Time time, void (T::*mem_ptr) (void), T *obj) {
		// zero argument version
		class EventMemberImpl0 : public EventImpl {
		public:
			typedef void (T::*F)(void);
			EventMemberImpl0 (T *obj, F function) 
				: m_obj (obj), 
				  m_function (function)
			{}
			virtual ~EventMemberImpl0 () {}
		private:
			virtual void notify (void) { 
				(m_obj->*m_function) (); 
			}
			T* m_obj;
			F m_function;
		} *ev = new EventMemberImpl0 (obj, mem_ptr);
		return schedule (time, ev);
	}
	template <typename T, typename T1>
	static EventId schedule (Time time, void (T::*f) (T1), T* t, T1 a1) {
		// one argument version
		class EventMemberImpl1 : public EventImpl {
		public:
			typedef void (T::*F)(T1);
			EventMemberImpl1 (T *obj, F function, T1 a1) 
				: m_obj (obj), 
				  m_function (function),
				  m_a1 (a1)
			{}
		protected:
			virtual ~EventMemberImpl1 () {}
		private:
			virtual void notify (void) { 
				(m_obj->*m_function) (m_a1);
			}
			T* m_obj;
			F m_function;
			T1 m_a1;
		} *ev = new EventMemberImpl1 (f, t, a1);
		return schedule (time, ev);
	}
	template <typename T, typename T1, typename T2>
	static EventId schedule (Time time, void (T::*f) (T1), T* t, T1 a1, T2 a2) {
		// two argument version
		class EventMemberImpl2 : public EventImpl {
		public:
			typedef void (T::*F)(T1, T2);
			
			EventMemberImpl2 (T *obj, F function, T1 a1, T2 a2) 
				: m_obj (obj), 
				  m_function (function),
				  m_a1 (a1),
				  m_a2 (a2)
			{ }
		protected:
			virtual ~EventMemberImpl2 () {}
		private:
			virtual void notify (void) { 
				(m_obj->*m_function) (m_a1, m_a2);
			}
			T* m_obj;
			F m_function;
			T1 m_a1;
			T2 m_a2;
		} *ev = new EventMemberImpl2 (t, f, a1, a2);

		return schedule (time, ev);
	}
	template <typename T, typename T1, typename T2, typename T3>
	static EventId schedule (Time time, void (T::*f) (T1), T* t, T1 a1, T2 a2, T3 a3) {
		// three argument version
		class EventMemberImpl3 : public EventImpl {
		public:
			typedef void (T::*F)(T1, T2, T3);
			
			EventMemberImpl3 (T *obj, F function, T1 a1, T2 a2, T3 a3) 
				: m_obj (obj), 
				  m_function (function),
				  m_a1 (a1),
		  m_a2 (a2),
				  m_a3 (a3)
			{ }
		protected:
			virtual ~EventMemberImpl3 () {}
		private:
			virtual void notify (void) { 
				(m_obj->*m_function) (m_a1, m_a2, m_a3);
			}
			T* m_obj;
			F m_function;
			T1 m_a1;
			T2 m_a2;
			T3 m_a3;
		} *ev = new EventMemberImpl3 (t, f, a1, a2, a3);
		return schedule (time, ev);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4>
	static EventId schedule (Time time, void (T::*f) (T1), T* t, T1 a1, T2 a2, T3 a3, T4 a4) {
		// four argument version
		class EventMemberImpl4 : public EventImpl {
		public:
			typedef void (T::*F)(T1, T2, T3, T4);
			
			EventMemberImpl4 (T *obj, F function, T1 a1, T2 a2, T3 a3, T4 a4) 
				: m_obj (obj), 
				  m_function (function),
				  m_a1 (a1),
				  m_a2 (a2),
				  m_a3 (a3),
				  m_a4 (a4)
			{ }
		protected:
			virtual ~EventMemberImpl4 () {}
		private:
			virtual void notify (void) { 
				(m_obj->*m_function) (m_a1, m_a2, m_a3, m_a4);
			}
			T* m_obj;
			F m_function;
			T1 m_a1;
			T2 m_a2;
			T3 m_a3;
			T4 m_a4;
		} *ev = new EventMemberImpl4 (t, f, a1, a2, a3, a4);
		return schedule (time, ev);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
	static EventId schedule (Time time, void (T::*f) (T1), T* t, 
				 T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
		// five argument version
		class EventMemberImpl5 : public EventImpl {
		public:
			typedef void (T::*F)(T1, T2, T3, T4, T5);
			
			EventMemberImpl5 (T *obj, F function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
				: m_obj (obj), 
				  m_function (function),
				  m_a1 (a1),
				  m_a2 (a2),
				  m_a3 (a3),
				  m_a4 (a4),
				  m_a5 (a5)
			{ }
		protected:
			virtual ~EventMemberImpl5 () {}
		private:
			virtual void notify (void) { 
				(m_obj->*m_function) (m_a1, m_a2, m_a3, m_a4, m_a5);
			}
			T* m_obj;
			F m_function;
			T1 m_a1;
			T2 m_a2;
			T3 m_a3;
			T4 m_a4;
			T5 m_a5;
		} *ev = new EventMemberImpl5 (t, f, a1, a2, a3, a4, a5);
		return schedule (time, ev);
	}
	static EventId schedule (Time time, void (*f) (void)) {
		// zero arg version
		class EventFunctionImpl0 : public EventImpl {
		public:
			typedef void (*F)(void);
			
			EventFunctionImpl0 (F function) 
				: m_function (function)
			{}
		protected:
			virtual void notify (void) { 
				(*m_function) (); 
			}
		private:
			virtual ~EventFunctionImpl0 () {}
			F m_function;
		} *ev = new EventFunctionImpl0 (f);
		return schedule (time, ev);
	}
	template <typename T1>
	static EventId schedule (Time time, void (*f) (T1), T1 a1) {
		// one arg version
		class EventFunctionImpl1 : public EventImpl {
		public:
			typedef void (*F)(T1);
			
			EventFunctionImpl1 (F function, T1 a1) 
				: m_function (function),
				  m_a1 (a1)
				{ }
		protected:
			virtual ~EventFunctionImpl1 () {}
		private:
			virtual void notify (void) { 
				(*m_function) (m_a1);
			}
			F m_function;
			T1 m_a1;
		} *ev = new EventFunctionImpl1(f, a1);
		return schedule (time, ev);
	}
	template <typename T1, typename T2>
	static EventId schedule (Time time, void (*f) (T1), T1 a1, T2 a2) {
		// two arg version
		class EventFunctionImpl2 : public EventImpl {
		public:
			typedef void (*F)(T1, T2);
			
			EventFunctionImpl2 (F function, T1 a1, T2 a2) 
				: m_function (function),
				  m_a1 (a1),
				  m_a2 (a2)
			{ }
		protected:
			virtual ~EventFunctionImpl2 () {}
		private:
			virtual void notify (void) { 
				(*m_function) (m_a1, m_a2);
			}
			F m_function;
			T1 m_a1;
			T2 m_a2;
		} *ev = new EventFunctionImpl2 (f, a1, a2);
		return schedule (time, ev);
	}
	template <typename T1, typename T2, typename T3>
	static EventId schedule (Time time, void (*f) (T1), T1 a1, T2 a2, T3 a3) {
		// three arg version
		class EventFunctionImpl3 : public EventImpl {
		public:
			typedef void (*F)(T1, T2, T3);
			
			EventFunctionImpl3 (F function, T1 a1, T2 a2, T3 a3) 
				: m_function (function),
				  m_a1 (a1),
				  m_a2 (a2),
				  m_a3 (a3)
			{ }
		protected:
			virtual ~EventFunctionImpl3 () {}
		private:
			virtual void notify (void) { 
				(*m_function) (m_a1, m_a2, m_a3);
			}
			F m_function;
			T1 m_a1;
			T2 m_a2;
			T3 m_a3;
		} *ev = new EventFunctionImpl3 (f, a1, a2, a3);
		return schedule (time, ev);
	}
	template <typename T1, typename T2, typename T3, typename T4>
	static EventId schedule (Time time, void (*f) (T1), T1 a1, T2 a2, T3 a3, T4 a4) {
		// four arg version
		class EventFunctionImpl4 : public EventImpl {
		public:
			typedef void (*F)(T1, T2, T3, T4);
			
			EventFunctionImpl4 (F function, T1 a1, T2 a2, T3 a3, T4 a4) 
				: m_function (function),
				  m_a1 (a1),
				  m_a2 (a2),
				  m_a3 (a3),
				  m_a4 (a4)
			{ }
		protected:
			virtual ~EventFunctionImpl4 () {}
		private:
			virtual void notify (void) { 
				(*m_function) (m_a1, m_a2, m_a3, m_a4);
			}
			F m_function;
			T1 m_a1;
			T2 m_a2;
			T3 m_a3;
			T4 m_a4;
		} *ev = new EventFunctionImpl4 (f, a1, a2, a3, a4);
		return schedule (time, ev);
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	static EventId schedule (Time time, void (*f) (T1), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
		// five arg version
		class EventFunctionImpl5 : public EventImpl {
		public:
			typedef void (*F)(T1, T2, T3, T4, T5);
			
			EventFunctionImpl5 (F function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
				: m_function (function),
				  m_a1 (a1),
				  m_a2 (a2),
				  m_a3 (a3),
				  m_a4 (a4),
				  m_a5 (a5)
			{ }
		protected:
			virtual ~EventFunctionImpl5 () {}
		private:
			virtual void notify (void) { 
				(*m_function) (m_a1, m_a2, m_a3, m_a4, m_a5);
			}
			F m_function;
			T1 m_a1;
			T2 m_a2;
			T3 m_a3;
			T4 m_a4;
			T5 m_a5;
		} *ev = new EventFunctionImpl5 (f, a1, a2, a3, a4, a5);
		return schedule (time, ev);
	}
	/**
	 * Unschedule the event. i.e.: the removed event never expires.
	 * @param id the event to remove from the list of scheduled events.
	 */
	static void remove (EventId id);
	/*
	  XXX
	 */
	static void cancel (EventId id);
	/*
	  XXX
	 */
	static bool is_expired (EventId id);
	/**
	 * Return the "current time".
	 */
	static Time now (void);
private:
	Simulator ();
	~Simulator ();
	static SimulatorPrivate *get_priv (void);
	static EventId schedule (Time time, EventImpl *event);
	static SimulatorPrivate *m_priv;
	static enum ListType {
		LINKED_LIST,
		BINARY_HEAP,

		STD_MAP
	} m_list_type;
};

}; // namespace ns3

#endif /* SIMULATOR_H */
