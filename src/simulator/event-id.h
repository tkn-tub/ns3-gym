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
#ifndef EVENT_ID_H
#define EVENT_ID_H

#include <stdint.h>

namespace ns3 {

class EventImpl;

class EventId {
public:
    EventId ();
    EventId (EventImpl *impl, uint64_t ns, uint32_t uid);
    void cancel (void);
    bool isExpired (void);
public:
    /* The following methods are semi-private
     * they are supposed to be invoked only by
     * subclasses of the Scheduler base class.
     */
    EventImpl *getEventImpl (void) const;
    uint64_t getNs (void) const;
    uint32_t getUid (void) const;
private:
    EventImpl *m_eventImpl;
    uint64_t m_ns;
    uint32_t m_uid;
};

}; // namespace ns3

#endif /* EVENT_ID_H */
