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
#include "ns3/log.h"
#include "ns3/header.h"
#include "ipv4-header.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4Header");

namespace ns3 {

NS_HEADER_ENSURE_REGISTERED (Ipv4Header);
NS_OBJECT_ENSURE_REGISTERED (Ipv4Header);

bool Ipv4Header::m_calcChecksum = false;

TypeId 
Ipv4Header::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4Header")
    .SetParent<Header> ()
    ;
  return tid;
}
TypeId 
Ipv4Header::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
Ipv4Header::GetUid (void)
{
  static uint32_t uid = AllocateUid<Ipv4Header> ("Ipv4Header.ns3");
  return uid;
}

Ipv4Header::Ipv4Header ()
  : m_payloadSize (0),
    m_identification (0),
    m_tos (0),
    m_ttl (0),
    m_protocol (0),
    m_flags (0),
    m_fragmentOffset (0),
    m_goodChecksum (true)
{}

void 
Ipv4Header::EnableChecksums (void)
{
  m_calcChecksum = true;
}

void 
Ipv4Header::SetPayloadSize (uint16_t size)
{
  m_payloadSize = size;
}
uint16_t 
Ipv4Header::GetPayloadSize (void) const
{
  return m_payloadSize;
}

uint16_t 
Ipv4Header::GetIdentification (void) const
{
  return m_identification;
}
void 
Ipv4Header::SetIdentification (uint16_t identification)
{
  m_identification = identification;
}



void 
Ipv4Header::SetTos (uint8_t tos)
{
  m_tos = tos;
}
uint8_t 
Ipv4Header::GetTos (void) const
{
  return m_tos;
}
void 
Ipv4Header::SetMoreFragments (void)
{
  m_flags |= MORE_FRAGMENTS;
}
void
Ipv4Header::SetLastFragment (void)
{
  m_flags &= ~MORE_FRAGMENTS;
}
bool 
Ipv4Header::IsLastFragment (void) const
{
  return !(m_flags & MORE_FRAGMENTS);
}

void 
Ipv4Header::SetDontFragment (void)
{
  m_flags |= DONT_FRAGMENT;
}
void 
Ipv4Header::SetMayFragment (void)
{
  m_flags &= ~DONT_FRAGMENT;
}
bool 
Ipv4Header::IsDontFragment (void) const
{
  return (m_flags & DONT_FRAGMENT);
}

void 
Ipv4Header::SetFragmentOffset (uint16_t offset)
{
  NS_ASSERT (!(offset & (~0x3fff)));
  m_fragmentOffset = offset;
}
uint16_t 
Ipv4Header::GetFragmentOffset (void) const
{
  NS_ASSERT (!(m_fragmentOffset & (~0x3fff)));
  return m_fragmentOffset;
}

void 
Ipv4Header::SetTtl (uint8_t ttl)
{
  m_ttl = ttl;
}
uint8_t 
Ipv4Header::GetTtl (void) const
{
  return m_ttl;
}
  
uint8_t 
Ipv4Header::GetProtocol (void) const
{
  return m_protocol;
}
void 
Ipv4Header::SetProtocol (uint8_t protocol)
{
  m_protocol = protocol;
}

void 
Ipv4Header::SetSource (Ipv4Address source)
{
  m_source = source;
}
Ipv4Address
Ipv4Header::GetSource (void) const
{
  return m_source;
}

void 
Ipv4Header::SetDestination (Ipv4Address dst)
{
  m_destination = dst;
}
Ipv4Address
Ipv4Header::GetDestination (void) const
{
  return m_destination;
}


bool
Ipv4Header::IsChecksumOk (void) const
{
  return m_goodChecksum;
}

std::string 
Ipv4Header::GetName (void) const
{
  return "IPV4";
}

void 
Ipv4Header::Print (std::ostream &os) const
{
  // ipv4, right ?
  std::string flags;
  if (m_flags == 0)
    {
      flags = "none";
    }
  else if (m_flags & MORE_FRAGMENTS &&
           m_flags & DONT_FRAGMENT)
    {
      flags = "MF|DF";
    }
  else if (m_flags & DONT_FRAGMENT)
    {
      flags = "DF";
    }
  else if (m_flags & MORE_FRAGMENTS)
    {
      flags = "MF";
    }
  else
    {
      flags = "XX";
    }
  os << "("
     << "tos 0x" << std::hex << m_tos << std::dec << " "
     << "ttl " << m_ttl << " "
     << "id " << m_identification << " "
     << "offset " << m_fragmentOffset << " "
     << "flags [" << flags << "] "
     << "length: " << (m_payloadSize + 5 * 4)
     << ") "
     << m_source << " > " << m_destination
    ;
}
uint32_t 
Ipv4Header::GetSerializedSize (void) const
{
  return 5 * 4;
}

void
Ipv4Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  
  uint8_t verIhl = (4 << 4) | (5);
  i.WriteU8 (verIhl);
  i.WriteU8 (m_tos);
  i.WriteHtonU16 (m_payloadSize + 5*4);
  i.WriteHtonU16 (m_identification);
  uint32_t fragmentOffset = m_fragmentOffset / 8;
  uint8_t flagsFrag = (fragmentOffset >> 8) & 0x1f;
  if (m_flags & DONT_FRAGMENT) 
    {
      flagsFrag |= (1<<6);
    }
  if (m_flags & MORE_FRAGMENTS) 
    {
      flagsFrag |= (1<<5);
    }
  i.WriteU8 (flagsFrag);
  uint8_t frag = fragmentOffset & 0xff;
  i.WriteU8 (frag);
  i.WriteU8 (m_ttl);
  i.WriteU8 (m_protocol);
  i.WriteHtonU16 (0);
  i.WriteHtonU32 (m_source.GetHostOrder ());
  i.WriteHtonU32 (m_destination.GetHostOrder ());

  if (m_calcChecksum) 
    {
#if 0
      // XXX we need to add Buffer::Iterator::PeekData method
      uint8_t *data = start.PeekData ();
      uint16_t checksum = UtilsChecksumCalculate (0, data, GetSize ());
      checksum = UtilsChecksumComplete (checksum);
      NS_LOG_LOGIC ("checksum=" <<checksum);
      i = start;
      i.Next (10);
      i.WriteU16 (checksum);
#endif
    }
}
uint32_t
Ipv4Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t verIhl = i.ReadU8 ();
  uint8_t ihl = verIhl & 0x0f; 
  uint16_t headerSize = ihl * 4;
  NS_ASSERT ((verIhl >> 4) == 4);
  m_tos = i.ReadU8 ();
  uint16_t size = i.ReadNtohU16 ();
  m_payloadSize = size - headerSize;
  m_identification = i.ReadNtohU16 ();
  uint8_t flags = i.ReadU8 ();
  m_flags = 0;
  if (flags & (1<<6)) 
    {
      m_flags |= DONT_FRAGMENT;
    }
  if (flags & (1<<5)) 
    {
      m_flags |= MORE_FRAGMENTS;
    }
  //XXXX I think we should clear some bits in fragmentOffset !
  i.Prev ();
  m_fragmentOffset = i.ReadNtohU16 ();
  m_fragmentOffset *= 8;
  m_ttl = i.ReadU8 ();
  m_protocol = i.ReadU8 ();
  i.Next (2); // checksum
  m_source.SetHostOrder (i.ReadNtohU32 ());
  m_destination.SetHostOrder (i.ReadNtohU32 ());

  if (m_calcChecksum) 
    {
#if 0
      uint8_t *data = start.PeekData ();
      uint16_t localChecksum = UtilsChecksumCalculate (0, data, headerSize);
      if (localChecksum == 0xffff) 
        {
          m_goodChecksum = true;
        } 
      else 
        {
          m_goodChecksum = false;
        }
#endif
    }
  return GetSerializedSize ();
}

}; // namespace ns3
