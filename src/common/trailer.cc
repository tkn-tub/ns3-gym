/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "trailer.h"
#include <cassert>

namespace ns3 {

Trailer::Trailer ()
  : m_isDeserialized (false) 
{}

Trailer::~Trailer ()
{}

void 
Trailer::Print (std::ostream &os) const
{
  PrintTo (os);
}
uint32_t
Trailer::GetSize (void) const
{
  return GetSerializedSize ();
}
void 
Trailer::Serialize (Buffer::Iterator start) const
{
  SerializeTo (start);
}
void 
Trailer::Deserialize (Buffer::Iterator start)
{
  DeserializeFrom (start);
  m_isDeserialized = true;
}
bool 
Trailer::IsDeserialized (void) const
{
  return m_isDeserialized;
}

std::ostream& operator<< (std::ostream& os, Trailer const& trailer)
{
  trailer.Print (os);
  return os;
}

}; // namespace ns3
