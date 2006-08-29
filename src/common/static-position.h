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
#ifndef STATIC_POSITION_H
#define STATIC_POSITION_H

#include "position.h"

namespace ns3 {

class StaticPosition : public Position {
public:
	StaticPosition ();
	virtual ~StaticPosition ();

	void set (double x, double y, double z);
private:
	virtual void real_get (double &x, double &y, double &z) const;
	double m_x;
	double m_y;
	double m_z;
};

}; // namespace ns3

#endif /* STATIC_POSITION_H */
