/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
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

#ifndef SCHEDULER_MAP_H
#define SCHEDULER_MAP_H

#include "scheduler.h"
#include <stdint.h>
#include <map>
#include <utility>

namespace yans {

class SchedulerMap : public Scheduler {
public:
	SchedulerMap ();
	virtual ~SchedulerMap ();

	virtual Event insert (Event event, Scheduler::EventKey key);
	virtual bool is_empty (void) const;
	virtual Event peek_next (void) const;
	virtual Scheduler::EventKey peek_next_key (void) const;
	virtual void remove_next (void);
	virtual Scheduler::EventKey remove (Event const ev);
private:
	typedef std::map<Scheduler::EventKey, Event, Scheduler::EventKeyCompare> EventMap;
	typedef std::map<Scheduler::EventKey, Event, Scheduler::EventKeyCompare>::iterator EventMapI;
	typedef std::map<Scheduler::EventKey, Event, Scheduler::EventKeyCompare>::const_iterator EventMapCI;

	void store_in_event (Event ev, EventMapI i) const;
	EventMapI get_from_event (Event const ev) const;


	EventMap m_list;
	uint32_t m_uid;
};

}; // namespace yans


#endif /* SCHEDULER_MAP_H */
