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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */

#include "ns3/assert.h"
#include "ns3/debug.h"
#include "ns3/header.h"
#include "ethernet-header.h"
#include "address-utils.h"

NS_DEBUG_COMPONENT_DEFINE ("EthernetHeader");

NS_HEADER_ENSURE_REGISTERED (ns3::EthernetHeader);

namespace ns3 {

uint32_t
EthernetHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<EthernetHeader> ("EthernetHeader.ns3");
  return uid;
}

EthernetHeader::EthernetHeader (bool hasPreamble)
  : m_enPreambleSfd (hasPreamble),
    m_lengthType (0)
{}

EthernetHeader::EthernetHeader ()
  : m_enPreambleSfd (false),
    m_lengthType (0)
{}

void 
EthernetHeader::SetLengthType (uint16_t lengthType)
{
  m_lengthType = lengthType;
}
uint16_t 
EthernetHeader::GetLengthType (void) const
{
  return m_lengthType;
}

void 
EthernetHeader::SetPreambleSfd (uint64_t preambleSfd)
{
  m_preambleSfd = preambleSfd;
}
uint64_t 
EthernetHeader::GetPreambleSfd (void) const
{
  return m_preambleSfd;
}

void 
EthernetHeader::SetSource (Eui48Address source)
{
  m_source = source;
}
Eui48Address
EthernetHeader::GetSource (void) const
{
  return m_source;
}

void 
EthernetHeader::SetDestination (Eui48Address dst)
{
  m_destination = dst;
}
Eui48Address
EthernetHeader::GetDestination (void) const
{
  return m_destination;
}

ethernet_header_t 
EthernetHeader::GetPacketType (void) const
{
  return LENGTH;
}

uint32_t 
EthernetHeader::GetHeaderSize (void) const
{
  return GetSerializedSize();
}

std::string
EthernetHeader::GetName (void) const
{
  return "ETHERNET";
}

void 
EthernetHeader::Print (std::ostream &os) const
{
  // ethernet, right ?
  if (m_enPreambleSfd)
    {
      os << " preamble/sfd=" << m_preambleSfd << ",";
    }
  os << " length/type=" << m_lengthType
     << ", source=" << m_source
     << ", destination=" << m_destination;
}
uint32_t 
EthernetHeader::GetSerializedSize (void) const
{
  if (m_enPreambleSfd)
    {
      return PREAMBLE_SIZE + LENGTH_SIZE + 2*MAC_ADDR_SIZE;
    } 
  else 
    {
      return LENGTH_SIZE + 2*MAC_ADDR_SIZE;
    }
}

void
EthernetHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  
  if (m_enPreambleSfd)
    {
      i.WriteU64(m_preambleSfd);
    }
  WriteTo (i, m_destination);
  WriteTo (i, m_source);
  i.WriteU16 (m_lengthType);
}
uint32_t
EthernetHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  if (m_enPreambleSfd)
    {
      m_enPreambleSfd = i.ReadU64 ();
    }

  ReadFrom (i, m_destination);
  ReadFrom (i, m_source);
  m_lengthType = i.ReadU16 ();

  return GetSerializedSize ();
}

} // namespace ns3
