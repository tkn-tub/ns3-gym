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
#include "static-speed-position.h"
#include "ns3/simulator.h"

namespace ns3 {

StaticSpeedPosition::StaticSpeedPosition ()
	: m_x (0.0),
	  m_y (0.0),
	  m_z (0.0),
	  m_dx (0.0),
	  m_dy (0.0),
	  m_dz (0.0),
	  m_prev_us (0)
{}
StaticSpeedPosition::~StaticSpeedPosition ()
{}

void 
StaticSpeedPosition::set (double x, double y, double z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void 
StaticSpeedPosition::set_delta (double dx, double dy, double dz)
{
	m_dx = dx / 1000000;
	m_dy = dy / 1000000;
	m_dz = dz / 1000000;
}

void 
StaticSpeedPosition::real_get (double &x, double &y, double &z) const
{
	uint64_t now_us = Simulator::now ().us ();
	uint64_t delta_us = now_us - m_prev_us;
	m_x += m_dx * delta_us;
	m_y += m_dy * delta_us;
	m_z += m_dz * delta_us;
	m_prev_us = now_us;
	x = m_x;
	y = m_y;
	z = m_z;
}

}; // namespace ns3
