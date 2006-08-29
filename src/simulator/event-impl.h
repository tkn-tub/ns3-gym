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
#ifndef EVENT_IMPL_H
#define EVENT_IMPL_H

#include <stdint.h>

namespace yans {

class EventImpl {
public:
	EventImpl ();
	virtual ~EventImpl () = 0;
	void invoke (void);
	void set_tag (void *tag);
	void *get_tag (void) const;
	void cancel (void);
	bool is_running (void);
protected:
	virtual void notify (void) = 0;
private:
	friend class Event;
	void *m_id;
	uint32_t m_count;
	uint32_t m_cancel : 1;
	uint32_t m_running : 1;
};

}; // namespace yans

#endif /* EVENT_IMPL_H */
