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

#include "chunk.h"
#include "ns3/assert.h"

namespace ns3 {

Chunk::Chunk ()
{}

Chunk::~Chunk ()
{}

void 
Chunk::Print (std::ostream &os) const
{
  PrintTo (os);
}
uint32_t
Chunk::GetSize (void) const
{
  return GetSerializedSize ();
}
void
Chunk::Serialize (Buffer::Iterator start) const
{
  SerializeTo (start);
}
uint32_t
Chunk::Deserialize (Buffer::Iterator start)
{
  uint32_t deserialized = DeserializeFrom (start);
  return deserialized;
}
std::ostream& operator<< (std::ostream& os, Chunk const& chunk)
{
  chunk.Print (os);
  return os;
}

}; // namespace ns3
