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


#include "ie-dot11s-rann.h"
#include "ns3/assert.h"
#include "ns3/address-utils.h"
#include "ns3/node.h"

namespace ns3 {
namespace dot11s {

IeRann::~IeRann ()
{
}
TypeId
IeRann::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::IeRann").SetParent<Object> ();
  return tid;
}
TypeId
IeRann::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
IeRann::Print (std::ostream &os)const
{
  // FILL
}
IeRann::IeRann ():
    m_flags (0),
    m_hopcount (0),
    m_ttl (0),
    m_originatorAddress (Mac48Address::GetBroadcast()),
    m_destSeqNumber (0),
    m_metric (0)
{
}
void
IeRann::SetFlags (uint8_t flags)
{
  m_flags = flags;
}
void
IeRann::SetHopcount (uint8_t hopcount)
{
  m_hopcount = hopcount;
}
void
IeRann::SetTTL (uint8_t ttl)
{
  m_ttl = ttl;
}
void
IeRann::SetDestSeqNumber (uint32_t dest_seq_number)
{
  m_destSeqNumber = dest_seq_number;
}
void
IeRann::SetMetric (uint32_t metric)
{
  m_metric = metric;
}
void
IeRann::SetOriginatorAddress (Mac48Address originator_address)
{
  m_originatorAddress = originator_address;
}

uint8_t
IeRann::GetFlags ()
{
  return m_flags;
}
uint8_t
IeRann::GetHopcount ()
{
  return m_hopcount;
}
uint8_t
IeRann::GetTTL ()
{
  return m_ttl;
}
uint32_t
IeRann::GetDestSeqNumber ()
{
  return m_destSeqNumber;
}
uint32_t
IeRann::GetMetric ()
{
  return m_metric;
}
Mac48Address
IeRann::GetOriginatorAddress ()
{
  return m_originatorAddress;
}
void
IeRann::SerializeInformation (Buffer::Iterator i) const
{
  i.WriteU8 (m_flags);
  i.WriteU8 (m_hopcount);
  i.WriteU8 (m_ttl);
  WriteTo (i, m_originatorAddress);
  i.WriteHtonU32 (m_destSeqNumber);
  i.WriteHtonU32 (m_metric);
}
uint8_t
IeRann::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_flags = i.ReadU8 ();
  m_hopcount = i.ReadU8 ();
  m_ttl = i.ReadU8 ();
  ReadFrom (i, m_originatorAddress);
  m_destSeqNumber = i.ReadNtohU32 ();
  m_metric = i.ReadNtohU32 ();
  return i.GetDistanceFrom (start);
}
uint8_t
IeRann::GetInformationSize () const
{
  uint8_t retval =
     1 //Flags
    +1 //Hopcount
    +1 //TTL
    +6 //OriginatorAddress
    +4 //DestSeqNumber
    +4;//Metric
  return retval;
}
  
} // namespace dot11s
} //namespace ns3


