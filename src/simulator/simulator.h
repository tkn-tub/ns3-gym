/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
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
class SchedulerFactory;

/**
 * \brief Control the scheduling of simulation events. 
 *
 * The internal simulation clock is maintained
 * as a 64-bit integer in microsecond units. This means that it is
 * not possible to specify event expiration times with anything better
 * than microsecond accuracy. Events whose expiration time is
 * the same are scheduled in FIFO order: the first event inserted in the
 * Scheduling queue is scheduled to expire first.
 * 
 * A simple example of how to use the Simulator class to schedule events
 * is shown below:
 * \include samples/main-simulator.cc
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
    static void setLinkedList (void);
    /**
     * Force the use of an event scheduler based on a binary heap.
     * This method must be invoked before any other method exported
     * by the Simulator class.
     *   - insert: O(log(n))
     *   - remove: O(log(n))
     */
    static void setBinaryHeap (void);
    /**
     * Force the use of an event scheduler based on a std::map.
     * This method must be invoked before any other method exported
     * by the Simulator class.
     *   - insert: O(log(n))
     *   - remove: O(log(n))
     */
    static void setStdMap (void);

    /**
     * Force the use of a user-provided event scheduler.
     * This method must be invoked before any other method exported
     * by the Simulator class.
     */
    static void setExternal (SchedulerFactory const*factory);

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
    static void enableLogTo (char const *filename);

    /**
     * Every event scheduled by the Simulator::insertAtDestroy method is
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
    static bool isFinished (void);
    /**
     * If Simulator::isFinished returns true, the behavior of this
     * method is undefined. Otherwise, it returns the microsecond-based
     * time of the next event expected to be scheduled.
     */
    static Time next (void);

    /**
     * Run the simulation until one of:
     *   - no events are present anymore
     *   - the user called Simulator::stop
     *   - the user called Simulator::stopAtUs and the
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
     * @param time the stop time.
     */
    static void stopAt (Time const &time);

    /**
     * Schedule an event to expire when time is reached.
	 * When the event expires, the input method will be invoked
	 * on the input object.
     *
     * @param time the expiration time of the event.
     * @param mem_ptr member method pointer to invoke
	 * @param obj the object on which to invoke the member method
     * @returns an id for the scheduled event.
     */
    template <typename T>
    static EventId schedule (Time const &time, void (T::*mem_ptr) (void), T *obj);
    /**
     * @param time the expiration time of the event.
     * @param mem_ptr member method pointer to invoke
	 * @param obj the object on which to invoke the member method
	 * @param a1 the first argument to pass to the invoked method
     * @returns an id for the scheduled event.
     */
    template <typename T, typename T1>
    static EventId schedule (Time const &time, void (T::*mem_ptr) (T1), T* obj, T1 a1);
    /**
     * @param time the expiration time of the event.
     * @param mem_ptr member method pointer to invoke
	 * @param obj the object on which to invoke the member method
	 * @param a1 the first argument to pass to the invoked method
	 * @param a2 the second argument to pass to the invoked method
     * @returns an id for the scheduled event.
     */
    template <typename T, typename T1, typename T2>
    static EventId schedule (Time const &time, void (T::*mem_ptr) (T1,T2), T* obj, T1 a1, T2 a2);
    /**
     * @param time the expiration time of the event.
     * @param mem_ptr member method pointer to invoke
	 * @param obj the object on which to invoke the member method
	 * @param a1 the first argument to pass to the invoked method
	 * @param a2 the second argument to pass to the invoked method
	 * @param a3 the third argument to pass to the invoked method
     * @returns an id for the scheduled event.
     */
    template <typename T, typename T1, typename T2, typename T3>
    static EventId schedule (Time const &time, void (T::*mem_ptr) (T1,T2,T3), T* obj, T1 a1, T2 a2, T3 a3);
    /**
     * @param time the expiration time of the event.
     * @param mem_ptr member method pointer to invoke
	 * @param obj the object on which to invoke the member method
	 * @param a1 the first argument to pass to the invoked method
	 * @param a2 the second argument to pass to the invoked method
	 * @param a3 the third argument to pass to the invoked method
	 * @param a4 the fourth argument to pass to the invoked method
     * @returns an id for the scheduled event.
     */
    template <typename T, typename T1, typename T2, typename T3, typename T4>
    static EventId schedule (Time const &time, void (T::*mem_ptr) (T1,T2,T3,T4), T* obj, T1 a1, T2 a2, T3 a3, T4 a4);
    /**
     * @param time the expiration time of the event.
     * @param mem_ptr member method pointer to invoke
	 * @param obj the object on which to invoke the member method
	 * @param a1 the first argument to pass to the invoked method
	 * @param a2 the second argument to pass to the invoked method
	 * @param a3 the third argument to pass to the invoked method
	 * @param a4 the fourth argument to pass to the invoked method
	 * @param a5 the fifth argument to pass to the invoked method
     * @returns an id for the scheduled event.
     */
    template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    static EventId schedule (Time const &time, void (T::*mem_ptr) (T1,T2,T3,T4,T5), T* obj, 
							 T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
	/**
     * @param time the expiration time of the event.
     * @param f the function to invoke
     * @returns an id for the scheduled event.
     */
    static EventId schedule (Time const &time, void (*f) (void));
    /**
     * @param time the expiration time of the event.
     * @param f the function to invoke
	 * @param a1 the first argument to pass to the function to invoke
     * @returns an id for the scheduled event.
     */
    template <typename T1>
    static EventId schedule (Time const &time, void (*f) (T1), T1 a1);
    /**
     * @param time the expiration time of the event.
     * @param f the function to invoke
	 * @param a1 the first argument to pass to the function to invoke
	 * @param a2 the second argument to pass to the function to invoke
     * @returns an id for the scheduled event.
     */
    template <typename T1, typename T2>
    static EventId schedule (Time const &time, void (*f) (T1,T2), T1 a1, T2 a2);
    /**
     * @param time the expiration time of the event.
     * @param f the function to invoke
	 * @param a1 the first argument to pass to the function to invoke
	 * @param a2 the second argument to pass to the function to invoke
	 * @param a3 the third argument to pass to the function to invoke
     * @returns an id for the scheduled event.
     */
    template <typename T1, typename T2, typename T3>
    static EventId schedule (Time const &time, void (*f) (T1,T2,T3), T1 a1, T2 a2, T3 a3);
    /**
     * @param time the expiration time of the event.
     * @param f the function to invoke
	 * @param a1 the first argument to pass to the function to invoke
	 * @param a2 the second argument to pass to the function to invoke
	 * @param a3 the third argument to pass to the function to invoke
	 * @param a4 the fourth argument to pass to the function to invoke
     * @returns an id for the scheduled event.
     */
    template <typename T1, typename T2, typename T3, typename T4>
    static EventId schedule (Time const &time, void (*f) (T1,T2,T3,T4), T1 a1, T2 a2, T3 a3, T4 a4);
    /**
     * @param time the expiration time of the event.
     * @param f the function to invoke
	 * @param a1 the first argument to pass to the function to invoke
	 * @param a2 the second argument to pass to the function to invoke
	 * @param a3 the third argument to pass to the function to invoke
	 * @param a4 the fourth argument to pass to the function to invoke
	 * @param a5 the fifth argument to pass to the function to invoke
     * @returns an id for the scheduled event.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    static EventId schedule (Time const &time, void (*f) (T1,T2,T3,T4,T5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
    /**
     * Remove an event from the event list. 
	 * This method has the same visible effect as the 
	 * ns3::Simulator::cancel method or the ns3::EventId::cancel method
	 * but its algorithmic complexity is much higher: it has often 
	 * O(log(n)) complexity, sometimes O(n), sometimes worse.
	 * Note that it is not possible to remove events which were scheduled
	 * for the "destroy" time. Doing so will result in a program error (crash).
	 *
     * @param id the event to remove from the list of scheduled events.
     */
    static void remove (EventId id);
    /**
	 * Set the cancel bit on this event: the event's associated function
	 * will not be invoked when it expires. 
	 * This method has the same visible effect as the 
	 * ns3::Simulator::remove method but its algorithmic complexity is 
	 * much lower: it has O(1) complexity.
	 * This method has the exact same semantics as ns3::EventId::cancel.
	 * Note that it is not possible to cancel events which were scheduled
	 * for the "destroy" time. Doing so will result in a program error (crash).
	 * 
	 * @param id the event to cancel
     */
    static void cancel (EventId id);
    /**
	 * This method has O(1) complexity.
	 * Note that it is not possible to test for the expiration of
	 * events which were scheduled for the "destroy" time. Doing so
	 * will result in a program error (crash).
	 *
	 * @param id the event to test for expiration
	 * @returns true if the event has expired, false otherwise.
     */
    static bool isExpired (EventId id);
    /**
     * Return the "current simulation time".
     */
    static Time now (void);
private:
    Simulator ();
    ~Simulator ();
    static SimulatorPrivate *getPriv (void);
    static EventId schedule (Time const &time, EventImpl *event);
    static SimulatorPrivate *m_priv;
    static SchedulerFactory const*m_schedFactory;
    static enum ListType {
        LINKED_LIST,
        BINARY_HEAP,
        STD_MAP,
        EXTERNAL
    } m_listType;
};

}; // namespace ns3


/********************************************************************
     Implementation of templates defined above
 ********************************************************************/

namespace ns3 {

template <typename T>
EventId Simulator::schedule (Time const &time, void (T::*mem_ptr) (void), T *obj) 
{
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
EventId Simulator::schedule (Time const &time, void (T::*mem_ptr) (T1), T* obj, T1 a1) 
{
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
	} *ev = new EventMemberImpl1 (obj, mem_ptr, a1);
	return schedule (time, ev);
}

template <typename T, typename T1, typename T2>
EventId Simulator::schedule (Time const &time, void (T::*mem_ptr) (T1,T2), T* obj, T1 a1, T2 a2) 
{
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
	} *ev = new EventMemberImpl2 (obj, mem_ptr, a1, a2);

	return schedule (time, ev);
}

template <typename T, typename T1, typename T2, typename T3>
EventId Simulator::schedule (Time const &time, void (T::*mem_ptr) (T1,T2,T3), T* obj, T1 a1, T2 a2, T3 a3) 
{
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
	} *ev = new EventMemberImpl3 (obj, mem_ptr, a1, a2, a3);
	return schedule (time, ev);
}

template <typename T, typename T1, typename T2, typename T3, typename T4>
EventId Simulator::schedule (Time const &time, void (T::*mem_ptr) (T1,T2,T3,T4), T* obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
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
	} *ev = new EventMemberImpl4 (obj, mem_ptr, a1, a2, a3, a4);
	return schedule (time, ev);
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::schedule (Time const &time, void (T::*mem_ptr) (T1,T2,T3,T4,T5), T* obj, 
							 T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
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
	} *ev = new EventMemberImpl5 (obj, mem_ptr, a1, a2, a3, a4, a5);
	return schedule (time, ev);
}

template <typename T1>
EventId Simulator::schedule (Time const &time, void (*f) (T1), T1 a1) 
{
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
EventId Simulator::schedule (Time const &time, void (*f) (T1,T2), T1 a1, T2 a2) 
{
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
EventId Simulator::schedule (Time const &time, void (*f) (T1,T2,T3), T1 a1, T2 a2, T3 a3)
{
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
EventId Simulator::schedule (Time const &time, void (*f) (T1,T2,T3,T4), T1 a1, T2 a2, T3 a3, T4 a4) 
{
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
static EventId schedule (Time const &time, void (*f) (T1,T2,T3,T4,T5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
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

}; // namespace ns3

#endif /* SIMULATOR_H */
