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
#ifndef STATIC_SPEED_POSITION_H
#define STATIC_SPEED_POSITION_H

#include <stdint.h>
#include "position.h"

namespace yans {

class StaticSpeedPosition : public Position {
public:
	StaticSpeedPosition ();
	virtual ~StaticSpeedPosition ();

	// m
	void set (double x, double y, double z);
	// m/s
	void set_delta (double dx, double dy, double dz);
private:
	virtual void real_get (double &x, double &y, double &z) const;
	mutable double m_x;
	mutable double m_y;
	mutable double m_z;
	double m_dx;
	double m_dy;
	double m_dz;
	mutable uint64_t m_prev_us;
};

}; // namespace yans

#endif /* STATIC_SPEED_POSITION */
