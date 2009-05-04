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


#include "ie-dot11s-prep.h"
#include "ns3/address-utils.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/test.h"
#include "ns3/packet.h"
namespace ns3 {
namespace dot11s {
/********************************
 * IePrep
 *******************************/
IePrep::~IePrep ()
{
}

TypeId
IePrep::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dot11s::IePrep")
                      .SetParent<Object> ();
  return tid;
}
TypeId
IePrep::GetInstanceTypeId () const
{
  return GetTypeId ();
}
IePrep::IePrep ():
    m_flags (0),
    m_hopcount (0),
    m_ttl (0),
    m_destinationAddress (Mac48Address::GetBroadcast()),
    m_destSeqNumber (0),
    m_lifetime (0),
    m_metric (0),
    m_originatorAddress (Mac48Address::GetBroadcast()),
    m_originatorSeqNumber (0)
{
}
void
IePrep::SetFlags (uint8_t flags)
{
  m_flags = flags;
}
void
IePrep::SetHopcount (uint8_t hopcount)
{
  m_hopcount = hopcount;
}
void
IePrep::SetTtl (uint8_t ttl)
{
  m_ttl = ttl;
}
void
IePrep::SetDestinationSeqNumber (uint32_t dest_seq_number)
{
  m_destSeqNumber = dest_seq_number;
}
void
IePrep::SetDestinationAddress (Mac48Address dest_address)
{
  m_destinationAddress = dest_address;
}
void
IePrep::SetMetric (uint32_t metric)
{
  m_metric = metric;
}
void
IePrep::SetOriginatorAddress (Mac48Address originator_address)
{
  m_originatorAddress = originator_address;
}
void
IePrep::SetOriginatorSeqNumber (uint32_t originator_seq_number)
{
  m_originatorSeqNumber = originator_seq_number;
}
void
IePrep::SetLifetime (uint32_t lifetime)
{
  m_lifetime = lifetime;
}
uint8_t
IePrep::GetFlags () const
{
  return m_flags;
}
uint8_t
IePrep::GetHopcount () const
{
  return m_hopcount;
}
uint32_t
IePrep::GetTtl () const
{
  return m_ttl;
}
uint32_t
IePrep::GetDestinationSeqNumber () const
{
  return m_destSeqNumber;
}
Mac48Address
IePrep::GetDestinationAddress () const
{
  return m_destinationAddress;
}
uint32_t
IePrep::GetMetric () const
{
  return m_metric;
}
Mac48Address
IePrep::GetOriginatorAddress () const
{
  return m_originatorAddress;
}
uint32_t
IePrep::GetOriginatorSeqNumber () const
{
  return m_originatorSeqNumber;
}
uint32_t
IePrep::GetLifetime () const
{
  return m_lifetime;
}
void
IePrep::DecrementTtl ()
{
  m_ttl --;
}

void
IePrep::IncrementMetric (uint32_t metric)
{
  m_metric +=metric;
}


void
IePrep::SerializeInformation (Buffer::Iterator i) const
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
uint8_t
IePrep::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_flags = i.ReadU8 ();
  m_hopcount = i.ReadU8 ();
  m_ttl = i.ReadU8 ();
  ReadFrom (i, m_destinationAddress);
  m_destSeqNumber = i.ReadNtohU32 ();
  m_lifetime = i.ReadNtohU32 ();
  m_metric = i.ReadNtohU32 ();
  ReadFrom (i, m_originatorAddress);
  m_originatorSeqNumber = i.ReadNtohU32 ();
  return i.GetDistanceFrom (start);
}
uint8_t
IePrep::GetInformationSize () const
{
  uint32_t retval =
     1 //Flags
    +1 //Hopcount
    +1 //Ttl
    +6 //Dest address
    +4 //Dest seqno
    +4 //Lifetime
    +4 //metric
    +6 //Originator address
    +4 //Originator seqno
    +1; //destination count
  return retval;
};
void
IePrep::PrintInformation (std::ostream& os) const
{
  //TODO
}
bool operator== (const IePrep & a, const IePrep & b)
{
  return (
      (a.m_flags == b.m_flags) &&
      (a.m_hopcount == b.m_hopcount) &&
      (a.m_ttl == b.m_ttl) &&
      (a.m_destinationAddress == b.m_destinationAddress) &&
      (a.m_destSeqNumber == b.m_destSeqNumber) && 
      (a.m_lifetime == b.m_lifetime) &&
      (a.m_metric == b.m_metric) &&
      (a.m_originatorAddress == b.m_originatorAddress) &&
      (a.m_originatorSeqNumber == b.m_originatorSeqNumber)
      );
}

#ifdef RUN_SELF_TESTS

/// Built-in self test for IePrep
struct IePrepBist : public IeTest 
{
  IePrepBist () : IeTest ("Mesh/802.11s/IE/PREP") {};
  virtual bool RunTests(); 
};

/// Test instance
static IePrepBist g_IePrepBist;

bool IePrepBist::RunTests ()
{
  bool result(true);
  
  // create test information element
  IePrep a;
  a.SetFlags (12);
  a.SetHopcount (11);
  a.SetTtl (10);
  a.SetDestinationAddress (Mac48Address("11:22:33:44:55:66"));
  a.SetDestinationSeqNumber (123);
  a.SetLifetime (5000);
  a.SetMetric (4321);
  a.SetOriginatorAddress (Mac48Address("33:00:22:00:11:00"));
  a.SetOriginatorSeqNumber (666);
  
  result = result && TestRoundtripSerialization (a);
  return result;
}

#endif // RUN_SELF_TESTS
 
} // namespace dot11s
} //namespace ns3

