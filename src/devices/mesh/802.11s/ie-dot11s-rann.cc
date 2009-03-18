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


#include "ns3/ie-dot11s-rann.h"
#include "ns3/assert.h"
#include "ns3/address-utils.h"

namespace ns3 {

IeDot11sRann::~IeDot11sRann()
{
}
TypeId
IeDot11sRann::GetTypeId()
{
  static TypeId tid = TypeId ("ns3::IeDot11sRann").SetParent<Object> ();
  return tid;
}
TypeId
IeDot11sRann::GetInstanceTypeId() const
  {
    return GetTypeId();
  }
void
IeDot11sRann::Print(std::ostream &os)const
  {
    // FILL
  }
IeDot11sRann::IeDot11sRann():
    m_flags(0),
    m_hopcount(0),
    m_ttl(0),
    m_originatorAddress(Mac48Address::GetBroadcast()),
    m_destSeqNumber(0),
    m_metric(0)
{
}
void
IeDot11sRann::SetFlags(uint8_t flags)
{
  m_flags = flags;
}
void
IeDot11sRann::SetHopcount(uint8_t hopcount)
{
  m_hopcount = hopcount;
}
void
IeDot11sRann::SetTTL(uint8_t ttl)
{
  m_ttl = ttl;
}
void
IeDot11sRann::SetDestSeqNumber(uint32_t dest_seq_number)
{
  m_destSeqNumber = dest_seq_number;
}
void
IeDot11sRann::SetMetric(uint32_t metric)
{
  m_metric = metric;
}
void
IeDot11sRann::SetOriginatorAddress(Mac48Address originator_address)
{
  m_originatorAddress = originator_address;
}

uint8_t
IeDot11sRann::GetFlags()
{
  return m_flags;
}
uint8_t
IeDot11sRann::GetHopcount()
{
  return m_hopcount;
}
uint8_t
IeDot11sRann::GetTTL()
{
  return m_ttl;
}
uint32_t
IeDot11sRann::GetDestSeqNumber()
{
  return m_destSeqNumber;
}
uint32_t
IeDot11sRann::GetMetric()
{
  return m_metric;
}
Mac48Address
IeDot11sRann::GetOriginatorAddress()
{
  return m_originatorAddress;
}
void
IeDot11sRann::SerializeInformation(Buffer::Iterator i) const
  {
    i.WriteU8 (m_flags);
    i.WriteU8 (m_hopcount);
    i.WriteU8 (m_ttl);
    WriteTo (i, m_originatorAddress);
    i.WriteHtonU32 (m_destSeqNumber);
    i.WriteHtonU32 (m_metric);
  }
uint16_t
IeDot11sRann::DeserializeInformation(Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_flags = i.ReadU8();
  m_hopcount = i.ReadU8();
  m_ttl = i.ReadU8();
  ReadFrom(i, m_originatorAddress);
  m_destSeqNumber = i.ReadNtohU32();
  m_metric = i.ReadNtohU32();
  return i.GetDistanceFrom(start);
}
uint16_t
IeDot11sRann::GetInformationSize() const
  {
    uint16_t retval =
      1//Flags
      +1//Hopcount
      +1//TTL
      +6//OriginatorAddress
      +4//DestSeqNumber
      +4;//Metric
    return retval;
  }

}

