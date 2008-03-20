/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */

#include <stdint.h>
#include <iostream>
#include "tcp-socket.h"
#include "tcp-header.h"
#include "ns3/buffer.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpHeader);

bool TcpHeader::m_calcChecksum = false;

TcpHeader::TcpHeader ()
  : m_sourcePort (0),
    m_destinationPort (0),
    m_sequenceNumber (0),
    m_ackNumber (0),
    m_length (5),
    m_flags (0),
    m_windowSize (0xffff),
    m_checksum (0),
    m_urgentPointer (0)
{}

TcpHeader::~TcpHeader ()
{}

void
TcpHeader::EnableChecksums (void)
{
  m_calcChecksum = true;
}

void TcpHeader::SetSourcePort (uint16_t port)
{
  m_sourcePort = port;
}
void TcpHeader::SetDestinationPort (uint16_t port)
{
  m_destinationPort = port;
}
void TcpHeader::SetSequenceNumber (SequenceNumber sequenceNumber)
{
  m_sequenceNumber = sequenceNumber;
}
void TcpHeader::SetAckNumber (SequenceNumber ackNumber)
{
  m_ackNumber = ackNumber;
}
void TcpHeader::SetLength (uint8_t length)
{
  m_length = length;
}
void TcpHeader::SetFlags (uint8_t flags)
{
  m_flags = flags;
}
void TcpHeader::SetWindowSize (uint16_t windowSize)
{
  m_windowSize = windowSize;
}
void TcpHeader::SetChecksum (uint16_t checksum)
{
  m_checksum = checksum;
}
void TcpHeader::SetUrgentPointer (uint16_t urgentPointer)
{
  m_urgentPointer = urgentPointer;
}

uint16_t TcpHeader::GetSourcePort () const
{
  return m_sourcePort;
}
uint16_t TcpHeader::GetDestinationPort () const
{
  return m_destinationPort;
}
SequenceNumber TcpHeader::GetSequenceNumber () const
{
  return m_sequenceNumber;
}
SequenceNumber TcpHeader::GetAckNumber () const
{
  return m_ackNumber;
}
uint8_t  TcpHeader::GetLength () const
{
  return m_length;
}
uint8_t  TcpHeader::GetFlags () const
{
  return m_flags;
}
uint16_t TcpHeader::GetWindowSize () const
{
  return m_windowSize;
}
uint16_t TcpHeader::GetChecksum () const
{
  return m_checksum;
}
uint16_t TcpHeader::GetUrgentPointer () const
{
  return m_urgentPointer;
}

void 
TcpHeader::InitializeChecksum (Ipv4Address source, 
                                   Ipv4Address destination,
                                   uint8_t protocol)
{
  m_checksum = 0;
//XXX requires peeking into IP to get length of the TCP segment
}

TypeId 
TcpHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpHeader")
    .SetParent<Header> ()
    .AddConstructor<TcpHeader> ()
    ;
  return tid;
}
TypeId 
TcpHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void TcpHeader::Print (std::ostream &os)  const
{
  //XXX
}
uint32_t TcpHeader::GetSerializedSize (void)  const
{
  return 20;  //tcp headers are 20 bytes
}
void TcpHeader::Serialize (Buffer::Iterator start)  const
{
  start.WriteHtonU16 (m_sourcePort);
  start.WriteHtonU16 (m_destinationPort);
  start.WriteHtonU32 (m_sequenceNumber);
  start.WriteHtonU32 (m_ackNumber);
  start.WriteHtonU16 (m_length << 12 | m_flags); //reserved bits are all zero
  start.WriteHtonU16 (m_windowSize);
  //XXX calculate checksum here
  start.WriteHtonU16 (m_checksum);
  start.WriteHtonU16 (m_urgentPointer);
}
uint32_t TcpHeader::Deserialize (Buffer::Iterator start)
{
  m_sourcePort = start.ReadNtohU16 ();
  m_destinationPort = start.ReadNtohU16 ();
  m_sequenceNumber = start.ReadNtohU32 ();
  m_ackNumber = start.ReadNtohU32 ();
  uint16_t field = start.ReadNtohU16 ();
  m_flags = field & 0x3F;
  m_length = field>>12;
  m_windowSize = start.ReadNtohU16 ();
  m_checksum = start.ReadNtohU16 ();
  m_urgentPointer = start.ReadNtohU16 ();
  return GetSerializedSize ();
}


}; // namespace ns3
