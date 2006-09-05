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
#ifndef TIME_H
#define TIME_H

#include <stdint.h>

namespace ns3 {

class Time {
public:
	Time (Time const &o);
	Time &operator = (Time const &o);
	double s (void) const;
	uint64_t us (void) const;
	uint64_t ns (void) const;
	bool isDestroy (void) const;
	static Time absS (double s);
	static Time absUs (uint64_t us);
	static Time absNs (uint64_t ns);
	static Time relS (double s);
	static Time relUs (uint64_t us);
	static Time relNs (uint64_t ns);
	static Time now (void);
	static Time destroy (void);
protected:
	Time (uint64_t ns);
	Time ();
private:
	uint64_t m_ns;
	bool m_isDestroy;
};


}; // namespace ns3

#endif /* TIME_H */
