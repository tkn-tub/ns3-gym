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

#include "canvas-location.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CanvasLocation);

TypeId 
CanvasLocation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CanvasLocation")
    .SetParent<Object> ()
    .AddAttribute ("Location", "The current location on the canvas.",
                   TypeId::ATTR_SET | TypeId::ATTR_GET,
                   VectorValue (Vector (0.0, 0.0, 0.0)),
                   MakeVectorAccessor (&CanvasLocation::SetLocation,
                                       &CanvasLocation::GetLocation),
                   MakeVectorChecker ())
  ;
  return tid;
}

CanvasLocation::CanvasLocation ()
{
}

CanvasLocation::~CanvasLocation ()
{
}

Vector
CanvasLocation::GetLocation (void) const
{
  return m_location;
}

void
CanvasLocation::SetLocation (const Vector &location)
{
  m_location = location;
}

} // namespace ns3
