/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "ns3/mesh-wifi-prep-information-element.h"
#include "ns3/address-utils.h"
#include "ns3/assert.h"
namespace ns3 {
/********************************
 * WifiPrepInformationElement
 *******************************/
WifiPrepInformationElement::~WifiPrepInformationElement()
{
}

TypeId
WifiPrepInformationElement::GetTypeId()
{
  static TypeId tid = TypeId ("ns3::WifiPrepInformationElement")
                      .SetParent<Object> ();
  return tid;
}
void
WifiPrepInformationElement::Print(std::ostream &os) const
  {
    //TODO:fill this method
  }
TypeId
WifiPrepInformationElement::GetInstanceTypeId() const
  {
    return GetTypeId();
  }
WifiPrepInformationElement::WifiPrepInformationElement():
    m_flags(0),
    m_hopcount(0),
    m_ttl(0),
    m_destinationAddress(Mac48Address::GetBroadcast()),
    m_destSeqNumber(0),
    m_lifetime(0),
    m_metric(0),
    m_originatorAddress(Mac48Address::GetBroadcast()),
    m_originatorSeqNumber(0)
{
}
void
WifiPrepInformationElement::SetFlags(uint8_t flags)
{
  m_flags = flags;
}
void
WifiPrepInformationElement::SetHopcount(uint8_t hopcount)
{
  m_hopcount = hopcount;
}
void
WifiPrepInformationElement::SetTTL(uint8_t ttl)
{
  m_ttl = ttl;
}
void
WifiPrepInformationElement::SetDestinationSeqNumber(uint32_t dest_seq_number)
{
  m_destSeqNumber = dest_seq_number;
}
void
WifiPrepInformationElement::SetDestinationAddress(Mac48Address dest_address)
{
  m_destinationAddress = dest_address;
}
void
WifiPrepInformationElement::SetMetric(uint32_t metric)
{
  m_metric = metric;
}
void
WifiPrepInformationElement::SetOriginatorAddress(Mac48Address originator_address)
{
  m_originatorAddress = originator_address;
}
void
WifiPrepInformationElement::SetOriginatorSeqNumber(uint32_t originator_seq_number)
{
  m_originatorSeqNumber = originator_seq_number;
}
void
WifiPrepInformationElement::SetLifetime(uint32_t lifetime)
{
  m_lifetime = lifetime;
}
uint8_t
WifiPrepInformationElement::GetFlags() const
  {
    return m_flags;
  }
uint8_t
WifiPrepInformationElement::GetHopcount() const
  {
    return m_hopcount;
  }
uint32_t
WifiPrepInformationElement::GetTTL() const
  {
    return m_ttl;
  }
uint32_t
WifiPrepInformationElement::GetDestinationSeqNumber() const
  {
    return m_destSeqNumber;
  }
Mac48Address
WifiPrepInformationElement::GetDestinationAddress() const
  {
    return m_destinationAddress;
  }
uint32_t
WifiPrepInformationElement::GetMetric() const
  {
    return m_metric;
  }
Mac48Address
WifiPrepInformationElement::GetOriginatorAddress() const
  {
    return m_originatorAddress;
  }
uint32_t
WifiPrepInformationElement::GetOriginatorSeqNumber() const
  {
    return m_originatorSeqNumber;
  }
uint32_t
WifiPrepInformationElement::GetLifetime() const
  {
    return m_lifetime;
  }
void
WifiPrepInformationElement::DecrementTtl()
{
  m_ttl --;
}

void
WifiPrepInformationElement::IncrementMetric(uint32_t metric)
{
  m_metric +=metric;
}


void
WifiPrepInformationElement::SerializeInformation(Buffer::Iterator i) const
  {
    i.WriteU8 (m_flags);
    i.WriteU8 (m_hopcount);
    i.WriteU8 (m_ttl);
    WriteTo (i, m_destinationAddress);
    i.WriteHtonU32 (m_destSeqNumber);
    i.WriteHtonU32 (m_lifetime);
    i.WriteHtonU32 (m_metric);
    WriteTo (i, m_originatorAddress);
    i.WriteHtonU32 (m_originatorSeqNumber);
  }
uint16_t
WifiPrepInformationElement::DeserializeInformation(Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_flags = i.ReadU8();
  m_hopcount = i.ReadU8();
  m_ttl = i.ReadU8();
  ReadFrom (i, m_destinationAddress);
  m_destSeqNumber = i.ReadNtohU32();
  m_lifetime = i.ReadNtohU32();
  m_metric = i.ReadNtohU32();
  ReadFrom(i, m_originatorAddress);
  m_originatorSeqNumber = i.ReadNtohU32();
  return i.GetDistanceFrom(start);
}
uint16_t
WifiPrepInformationElement::GetInformationSize() const
  {
    uint32_t retval =
       1 //Element ID
      +1 //Length
      +1 //Flags
      +1 //Hopcount
      +1 //TTL
      +6 //Dest address
      +4 //Dest seqno
      +4 //Lifetime
      +4 //metric
      +6 //Originator address
      +4 //Originator seqno
      +1; //destination count
    return retval;

  };
uint8_t
WifiPrepInformationElement::GetLengthField() const
{
  return 32;
}
void
WifiPrepInformationElement::PrintInformation(std::ostream& os) const
{
  //TODO
}
} //namespace ns3
