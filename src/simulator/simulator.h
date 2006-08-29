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
#include "event.h"

namespace yans {

class SimulatorPrivate;
class ParallelSimulatorQueuePrivate;

class ParallelSimulatorQueue {
public:
	virtual ~ParallelSimulatorQueue () = 0;
	void schedule_abs_us (uint64_t at, Event ev);
private:
	friend class Simulator;
	friend class ParallelSimulatorQueuePrivate;

	void set_priv (ParallelSimulatorQueuePrivate *priv);
	virtual void send_null_message (void) = 0;
	ParallelSimulatorQueuePrivate *m_priv;
protected:
	ParallelSimulatorQueue ();
};

/**
 * \brief Control the scheduling of simulation events. 
 *
 * The internal simulation clock is maintained
 * as a 64-bit integer in microsecond units. This means that it is
 * not possible to specify event expiration times with anything better
 * than microsecond accuracy. Events which whose expiration time is
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
	 * Register a new source of events from a remote simulation engine.
	 * This new source of events is used by the parallel scheduler
	 * to synchronize with the remote simulation engine by sending and
	 * receiving null messages. The synchronization algorithm used
	 * here is the classic Chandy/Misra/Bryant null-message algorithm.
	 * This method must be invoked bfore any call to Simulator::run.
	 * @param queue the queue to add to the list of event sources.
	 */
	static void add_parallel_queue (ParallelSimulatorQueue *queue);

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
	static uint64_t next_us (void);

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
	static void stop_at_us (uint64_t at);

	/**
	 * Schedule an event to expire at delta, relative to the
	 * current time.
	 * @param delta the expiration time relative to the current
	 *        time. Expressed in microsecond units.
	 * @param event the event to schedule.
	 */
	static Event schedule_rel_us (uint64_t delta, Event event);
	/**
	 * Schedule an event to expire at delta, relative to the
	 * current time.
	 * @param delta the expiration time, relative to the current
	 *        time. Expressed in second units.
	 * @param event the event to schedule.
	 */
	static Event schedule_rel_s (double delta, Event event);
	/**
	 * Schedule an event to expire at an absolute time.
	 * @param time the expiration time. Expressed in 
	 *             microsecond units.
	 * @param event the event to schedule.
	 */
	static Event schedule_abs_us (uint64_t time, Event event);
	/**
	 * Schedule an event to expire at an absolute time.
	 * @param time the expiration time. Expressed in 
	 *             second units.
	 * @param event the event to schedule.
	 */
	static Event schedule_abs_s (double time, Event event);
	/**
	 * Unschedule the event. i.e.: the removed event never expires.
	 * @param id the event to remove from the list of scheduled events.
	 */
	static Event remove (Event const id);
	/**
	 * Return the "current time" in microsecond units.
	 */
	static uint64_t now_us (void);
	/**
	 * Return the "current time" in second units.
	 */
	static double now_s (void);
	/**
	 * Schedule an event to expire right now. i.e., it will
	 * expire after the currently-executing event is executed.
	 * If multiple events are scheduled with this method, 
	 * they are executed in FIFO order: the events scheduled first
	 * are executed first.
	 * @param event the event to schedule now.
	 */
	static void schedule_now (Event event);
	/**
	 * Schedule an event to expire when the Simulator::destroy method
	 * is invoked. Events are executed in FIFO order: the events
	 * scheduled first are executed first.
	 * @param event the event to schedule.
	 */
	static void schedule_destroy (Event event);
private:
	Simulator ();
	~Simulator ();
	static SimulatorPrivate *get_priv (void);
	static SimulatorPrivate *m_priv;
	static enum ListType {
		LINKED_LIST,
		BINARY_HEAP,
		STD_MAP
	} m_list_type;
};

}; // namespace yans

#endif /* SIMULATOR_H */
