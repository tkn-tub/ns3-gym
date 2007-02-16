/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "ns3/assert.h"

#include "llc-snap-header.h"

#define noTRACE_LLC_SNAP_HEADER 1

#ifdef TRACE_LLC_SNAP_HEADER
#include <iostream>
#include "ns3/simulator.h"
# define TRACE(x) \
std::cout << "LLCSNAP HEAD TRACE " << Simulator::Now () << " " << x << std::endl;
#else /* TRACE_LLC_SNAP_HEADER */
# define TRACE(format,...)
#endif /* TRACE_LLC_SNAP_HEADER */


namespace ns3 {

LlcSnapHeader::LlcSnapHeader ()
{}

LlcSnapHeader::~LlcSnapHeader ()
{}
void 
LlcSnapHeader::SetType (uint16_t type)
{
  m_etherType = type;
}
uint16_t
LlcSnapHeader::GetType (void)
{
  return m_etherType;
}

uint32_t 
LlcSnapHeader::GetSerializedSize (void) const
{
  return 1 + 1 + 1 + 3 + 2;
}

void 
LlcSnapHeader::PrintTo (std::ostream &os) const
{
  os << "(mac)"
      << " EtherType: ";
  os.setf (std::ios::hex, std::ios::basefield);
  os << m_etherType;
  os.setf (std::ios::dec, std::ios::basefield);
}

void 
LlcSnapHeader::SerializeTo (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[] = {0xaa, 0xaa, 0x03, 0, 0, 0};
  i.Write (buf, 6);
  i.WriteHtonU16 (m_etherType);
}
void 
LlcSnapHeader::DeserializeFrom (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i.Next (5+1);
  m_etherType = i.ReadNtohU16 ();
}


}; // namespace ns3
