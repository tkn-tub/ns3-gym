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
	bool is_destroy (void) const;
	static Time abs_s (double s);
	static Time abs_us (uint64_t us);
	static Time abs_ns (uint64_t ns);
	static Time rel_s (double s);
	static Time rel_us (uint64_t us);
	static Time rel_ns (uint64_t ns);
	static Time now (void);
	static Time destroy (void);
protected:
	Time (uint64_t ns);
	Time ();
private:
	uint64_t m_ns;
	bool m_is_destroy;
};

class AbsTimeS : public Time {
public:
	AbsTimeS (double s);
};
class AbsTimeUs : public Time {
public:
	AbsTimeUs (uint64_t us);
};
class RelTimeS : public Time {
public:
	RelTimeS (double s);
};
class RelTimeUs : public Time {
public:
	RelTimeUs (uint64_t us);
};

class NowTime : public Time {
public:
	NowTime ();
};

class DestroyTime : public Time {
public:
	DestroyTime ();
};

}; // namespace ns3

#endif /* TIME_H */
