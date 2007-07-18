/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "rectangle.h"

namespace ns3 {

Rectangle::Rectangle (double _xMin, double _xMax,
		      double _yMin, double _yMax)
  : xMin (_xMin),
    xMax (_xMax),
    yMin (_yMin),
    yMax (_yMax)
{}

Rectangle::Rectangle ()
  : xMin (0.0),
    xMax (0.0),
    yMin (0.0),
    yMax (0.0)
{}

} // namespace ns3
