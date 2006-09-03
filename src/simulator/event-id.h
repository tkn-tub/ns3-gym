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
#ifndef EVENT_ID_H
#define EVENT_ID_H

#include <stdint.h>

namespace ns3 {

class EventImpl;

class EventId {
public:
        EventId ();
        EventId (EventImpl *impl, uint64_t time, uint32_t uid);
	void cancel (void);
	bool is_expired (void);
public:
	/* The following methods are semi-private
	 * they are supposed to be invoked only by
	 * subclasses of the Scheduler base class.
	 */
	EventImpl *get_event_impl (void) const;
	uint64_t get_time (void) const;
	uint32_t get_uid (void) const;
private:
	EventImpl *m_event_impl;
	uint64_t m_time;
	uint32_t m_uid;
};

}; // namespace ns3

#endif /* EVENT_ID_H */
