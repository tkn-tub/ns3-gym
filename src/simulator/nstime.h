/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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

/**
 * \brief simulation time
 *
 * This class is used by the user to specify when a scheduled event
 * is expected to expire (see ns3::Simulator::schedule).
 */
class Time {
public:
    Time (Time const &o);
    Time &operator = (Time const &o);
	/**
	 * \returns the time stored in this
	 *          instance as seconds.
	 */
    double s (void) const;
	/**
	 * \returns the time stored in this
	 *          instance as microseconds.
	 */
    uint64_t us (void) const;
	/**
	 * \returns the time stored in this
	 *          instance as nanoseconds.
	 */
    uint64_t ns (void) const;
	/**
	 * \returns true if this instance represents
	 *          the "destroy" time.
	 */
    bool isDestroy (void) const;
	/**
	 * \param s absolute time in seconds
	 * \returns a constructed Time object
	 */
    static Time absS (double s);
	/**
	 * \param us absolute time in microseconds
	 * \returns a constructed Time object
	 */
    static Time absUs (uint64_t us);
	/**
	 * \param ns absolute time in nanoseconds
	 * \returns a constructed Time object
	 */
    static Time absNs (uint64_t ns);
	/**
	 * \param s relative time in seconds
	 * \returns a constructed Time object
	 */
    static Time relS (double s);
	/**
	 * \param us relative time in microseconds
	 * \returns a constructed Time object
	 */
    static Time relUs (uint64_t us);
	/**
	 * \param ns relative time in nanoseconds
	 * \returns a constructed Time object
	 */
    static Time relNs (uint64_t ns);
	/**
	 * \returns a constructed Time object which represents
	 *          the current simulation time
	 */
    static Time now (void);
	/**
	 * \returns a constructed Time object which represents
	 *          the current "destroy" simulation time, that
	 *          is, the time when Simulator::destroy is
	 *          invoked by the user.
	 */
    static Time destroy (void);
private:
    Time (uint64_t ns);
    Time ();
    uint64_t m_ns;
    bool m_isDestroy;
};


}; // namespace ns3

#endif /* TIME_H */
