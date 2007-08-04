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

#include "udp-header.h"
#include "ipv4-checksum.h"

namespace ns3 {

bool UdpHeader::m_calcChecksum = false;

uint32_t
UdpHeader::GetUid (void)
{
  static uint32_t uid = Header::Register<UdpHeader> ("UdpHeader.ns3");
  return uid;
}

/* The magic values below are used only for debugging.
 * They can be used to easily detect memory corruption
 * problems so you can see the patterns in memory.
 */
UdpHeader::UdpHeader ()
  : m_sourcePort (0xfffd),
    m_destinationPort (0xfffd),
    m_payloadSize (0xfffd),
    m_initialChecksum (0)
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
UdpHeader::SetDestination (uint16_t port)
{
  m_destinationPort = port;
}
void 
UdpHeader::SetSource (uint16_t port)
{
  m_sourcePort = port;
}
uint16_t 
UdpHeader::GetSource (void) const
{
  return m_sourcePort;
}
uint16_t 
UdpHeader::GetDestination (void) const
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
  uint8_t buf[12];
  source.Serialize (buf);
  destination.Serialize (buf+4);
  buf[8] = 0;
  buf[9] = protocol;
  uint16_t udpLength = m_payloadSize + GetSize ();
  buf[10] = udpLength >> 8;
  buf[11] = udpLength & 0xff;

  m_initialChecksum = Ipv4ChecksumCalculate (0, buf, 12);
}

std::string 
UdpHeader::DoGetName (void) const
{
  return "UDP";
}

void 
UdpHeader::PrintTo (std::ostream &os) const
{
  os << "(" 
     << "length: " << m_payloadSize + GetSize ()
     << ") "
     << m_sourcePort << " > " << m_destinationPort
    ;
}

uint32_t 
UdpHeader::GetSerializedSize (void) const
{
  return 8;
}

void
UdpHeader::SerializeTo (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU16 (m_sourcePort);
  i.WriteHtonU16 (m_destinationPort);
  i.WriteHtonU16 (m_payloadSize + GetSize ());
  i.WriteU16 (0);

  if (m_calcChecksum) 
    {
#if 0
      //XXXX
      uint16_t checksum = Ipv4ChecksumCalculate (m_initialChecksum, 
                                                  buffer->PeekData (), 
                                                  GetSize () + m_payloadSize);
      checksum = Ipv4ChecksumComplete (checksum);
      i = buffer->Begin ();
      i.Next (6);
      i.WriteU16 (checksum);
#endif
    }
}
uint32_t
UdpHeader::DeserializeFrom (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_sourcePort = i.ReadNtohU16 ();
  m_destinationPort = i.ReadNtohU16 ();
  m_payloadSize = i.ReadNtohU16 () - GetSize ();
  if (m_calcChecksum) 
    {
      // XXX verify checksum.
    }
  return GetSerializedSize ();
}


}; // namespace ns3
