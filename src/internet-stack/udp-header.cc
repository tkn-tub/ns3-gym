/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "udp-header.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UdpHeader);

/* The magic values below are used only for debugging.
 * They can be used to easily detect memory corruption
 * problems so you can see the patterns in memory.
 */
UdpHeader::UdpHeader ()
  : m_sourcePort (0xfffd),
    m_destinationPort (0xfffd),
    m_payloadSize (0xfffd),
    m_initialChecksum (0),
    m_checksum(0),
    m_calcChecksum(false),
    m_goodChecksum(true)
{}
UdpHeader::~UdpHeader ()
{
  m_sourcePort = 0xfffe;
  m_destinationPort = 0xfffe;
  m_payloadSize = 0xfffe;
}

void 
UdpHeader::EnableChecksums (void)
{
  m_calcChecksum = true;
}

void 
UdpHeader::SetDestinationPort (uint16_t port)
{
  m_destinationPort = port;
}
void 
UdpHeader::SetSourcePort (uint16_t port)
{
  m_sourcePort = port;
}
uint16_t 
UdpHeader::GetSourcePort (void) const
{
  return m_sourcePort;
}
uint16_t 
UdpHeader::GetDestinationPort (void) const
{
  return m_destinationPort;
}
void 
UdpHeader::SetPayloadSize (uint16_t size)
{
  m_payloadSize = size;
}
void 
UdpHeader::InitializeChecksum (Ipv4Address source, 
                              Ipv4Address destination,
                              uint8_t protocol)
{
  Buffer buf = Buffer(12);
  uint8_t tmp[4];
  Buffer::Iterator it;
  uint16_t udpLength = m_payloadSize + GetSerializedSize();

  buf.AddAtStart(12);
  it = buf.Begin();

  source.Serialize(tmp);
  it.Write(tmp, 4); /* source IP address */
  destination.Serialize(tmp);
  it.Write(tmp, 4); /* destination IP address */
  it.WriteU8(0); /* protocol */
  it.WriteU8(protocol); /* protocol */
  it.WriteU8(udpLength >> 8); /* length */
  it.WriteU8(udpLength & 0xff); /* length */

  it = buf.Begin();
  /* we don't CompleteChecksum ( ~ ) now */
  m_initialChecksum = ~(it.CalculateIpChecksum(12));
}

bool
UdpHeader::IsChecksumOk (void) const
{
  return m_goodChecksum; 
}


TypeId 
UdpHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpHeader")
    .SetParent<Header> ()
    .AddConstructor<UdpHeader> ()
    ;
  return tid;
}
TypeId 
UdpHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void 
UdpHeader::Print (std::ostream &os) const
{
  os << "length: " << m_payloadSize + GetSerializedSize ()
     << " " 
     << m_sourcePort << " > " << m_destinationPort
    ;
}

uint32_t 
UdpHeader::GetSerializedSize (void) const
{
  return 8;
}

void
UdpHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint16_t udpLength = m_payloadSize + GetSerializedSize();

  i.WriteHtonU16 (m_sourcePort);
  i.WriteHtonU16 (m_destinationPort);
  i.WriteHtonU16 (udpLength);
  i.WriteU16 (0);

  if (m_calcChecksum)
    {
      i = start;
      uint16_t checksum = i.CalculateIpChecksum(udpLength, m_initialChecksum);

      i = start;
      i.Next(6);
      i.WriteU16(checksum);
    }
}
uint32_t
UdpHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_sourcePort = i.ReadNtohU16 ();
  m_destinationPort = i.ReadNtohU16 ();
  m_payloadSize = i.ReadNtohU16 () - GetSerializedSize ();
  m_checksum = i.ReadU16();

  if(m_calcChecksum)
  {
      i = start;
      uint16_t checksum = i.CalculateIpChecksum(m_payloadSize + GetSerializedSize(), m_initialChecksum);

      m_goodChecksum = (checksum == 0);
  }

  return GetSerializedSize ();
}


}; // namespace ns3
