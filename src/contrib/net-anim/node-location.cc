/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Georgia Institute of Technology
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
 * Author: George Riley<riley@ece.gatech.edu>
 */

#include "node-location.h"

namespace ns3 {

TypeId 
NodeLocation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NodeLocation")
    .SetParent<Object> ()
    .AddAttribute ("Position", "The current position of the mobility model.",
                   TypeId::ATTR_SET | TypeId::ATTR_GET,
                   VectorValue (Vector (0.0, 0.0, 0.0)),
                   MakeVectorAccessor (&NodeLocation::SetLocation,
                                        &NodeLocation::GetLocation),
                   MakeVectorChecker ())
    ;
  return tid;
}

NodeLocation::NodeLocation ()
{}

NodeLocation::~NodeLocation ()
{}

Vector NodeLocation::GetLocation (void) const
{
  return m_location;
}

void NodeLocation::SetLocation (const Vector &location)
{
  m_location = location;
}

} // namespace ns3
