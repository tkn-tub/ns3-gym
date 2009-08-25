/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 */

#include "wifi-information-element-vector.h"
#include "ns3/test.h"
#include "ns3/packet.h"
#include <algorithm>
// All information elements:
#include "dot11s/ie-dot11s-beacon-timing.h"
#include "dot11s/ie-dot11s-configuration.h"
#include "dot11s/ie-dot11s-id.h"
#include "dot11s/ie-dot11s-metric-report.h"
#include "dot11s/ie-dot11s-peer-management.h"
#include "dot11s/ie-dot11s-peering-protocol.h"
#include "dot11s/ie-dot11s-perr.h"
#include "dot11s/ie-dot11s-prep.h"
#include "dot11s/ie-dot11s-preq.h"
#include "dot11s/ie-dot11s-rann.h"

namespace ns3 {
WifiInformationElementVector::WifiInformationElementVector () :
  m_maxSize (1500)
{
}
WifiInformationElementVector::~WifiInformationElementVector ()
{
}
TypeId
WifiInformationElementVector::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiInformationElementVector")
                      .SetParent<Header> ();
  return tid;
}
TypeId
WifiInformationElementVector::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint32_t
WifiInformationElementVector::GetSerializedSize () const
{
  return GetSize ();
}
void
WifiInformationElementVector::Serialize (Buffer::Iterator start) const
{
  for(std::vector<Ptr<WifiInformationElement> >::const_iterator i = m_elements.begin (); i != m_elements.end (); i ++)
    {
      start.WriteU8((*i)->ElementId ());
      start.WriteU8 ((*i)->GetInformationSize ());
      (*i)->SerializeInformation (start);
      start.Next ((*i)->GetInformationSize ());
    }
}
uint32_t
WifiInformationElementVector::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint32_t size = start.GetSize();
  while (size > 0)
    {
      uint32_t deserialized = DeserializeSingleIe(i);
      i.Next (deserialized);
      size -= deserialized;
    }
  return i.GetDistanceFrom(start);
}
uint32_t
WifiInformationElementVector::DeserializeSingleIe(Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t id = i.ReadU8 ();
  uint8_t length = i.ReadU8 ();
  Ptr<WifiInformationElement> newElement;
  switch (id)
    {
  case IE11S_MESH_CONFIGURATION:
    newElement = Create<dot11s::IeConfiguration> ();
    break;
  case IE11S_MESH_ID:
    newElement = Create<dot11s::IeMeshId> ();
    break;
  case IE11S_LINK_METRIC_REPORT:
    newElement = Create<dot11s::IeLinkMetricReport> ();
    break;
  case IE11S_PEERING_MANAGEMENT:
    newElement = Create<dot11s::IePeerManagement> ();
    break;
  case IE11S_BEACON_TIMING:
    newElement = Create<dot11s::IeBeaconTiming> ();
    break;
  case IE11S_RANN:
    newElement = Create<dot11s::IeRann> ();
    break;
  case IE11S_PREQ:
    newElement = Create<dot11s::IePreq> ();
    break;
  case IE11S_PREP:
    newElement = Create<dot11s::IePrep> ();
    break;
  case IE11S_PERR:
    newElement = Create<dot11s::IePerr> ();
    break;
  case IE11S_MESH_PEERING_PROTOCOL_VERSION:
    newElement = Create<dot11s::IePeeringProtocol> ();
    break;
  default:
    NS_FATAL_ERROR ("Information element " << (uint16_t) id << " is not implemented");
    return 0;
    }
  if (GetSize () + length > m_maxSize)
    {
      NS_FATAL_ERROR ("Check max size for information element!");
    }
  newElement->DeserializeInformation (i, length);
  i.Next (length);
  m_elements.push_back (newElement);
  return i.GetDistanceFrom(start);
}
void
WifiInformationElementVector::Print(std::ostream & os) const
{
  //TODO
}
void
WifiInformationElementVector::SetMaxSize (uint16_t size)
{
  m_maxSize = size;
}
WifiInformationElementVector::Iterator
WifiInformationElementVector::Begin ()
{
  return m_elements.begin ();
}
WifiInformationElementVector::Iterator
WifiInformationElementVector::End ()
{
  return m_elements.end ();
}
bool
WifiInformationElementVector::AddInformationElement (Ptr<WifiInformationElement> element)
{
  if (element->GetInformationSize () + 2 + GetSize () > m_maxSize)
    {
      return false;
    }
  m_elements.push_back (element);
  return true;
}
Ptr<WifiInformationElement>
WifiInformationElementVector::FindFirst (enum WifiElementId id) const
{
  for (IE_VECTOR::const_iterator i = m_elements.begin (); i != m_elements.end (); i++)
    {
      if ((*i)->ElementId () == id)
        {
          return (*i);
        }
    }
  return 0;
}
namespace {
struct PIEComparator
{
  bool
  operator () (Ptr<WifiInformationElement> a, Ptr<WifiInformationElement> b) const
  {
    return ((*PeekPointer (a)) < (*PeekPointer (b)));
  }
};
}
uint32_t
WifiInformationElementVector::GetSize () const
{
  uint32_t size = 0;
  for (IE_VECTOR::const_iterator i = m_elements.begin (); i != m_elements.end (); i++)
    {
      size += ((*i)->GetInformationSize () + 2);
    }
  return size;
}
bool
operator== (const WifiInformationElementVector & a, const WifiInformationElementVector & b)
{
  if (a.m_elements.size () != b.m_elements.size ())
    {
      NS_ASSERT(false);
      return false;
    }
  WifiInformationElementVector::IE_VECTOR::const_iterator j = b.m_elements.begin ();
  for (WifiInformationElementVector::IE_VECTOR::const_iterator i = a.m_elements.begin (); i
      != a.m_elements.end (); i++, j++)
    {     
      if ((*i)->ElementId () != (*j)->ElementId ())
        {
          return false;
        }
      if ((*i)->GetInformationSize () != (*j)->GetInformationSize ())
        {
          return false;
        }
    }
  return true;
}
#ifdef RUN_SELF_TESTS

/// Built-in self test for WifiInformationElementVector
struct WifiInformationElementVectorBist : public IeTest
{
  WifiInformationElementVectorBist () :
    IeTest ("Mesh/WifiInformationElementVector")
  {
  };
  virtual bool
  RunTests ();
};

/// Test instance
static WifiInformationElementVectorBist g_IePrepBist;

bool
WifiInformationElementVectorBist::RunTests ()
{
  bool result = true;
  WifiInformationElementVector vector;
  vector.AddInformationElement (Create<dot11s::IeMeshId> ());
  vector.AddInformationElement (Create<dot11s::IeConfiguration> ());
  vector.AddInformationElement (Create<dot11s::IeLinkMetricReport> ());
  vector.AddInformationElement (Create<dot11s::IePeerManagement> ());
  vector.AddInformationElement (Create<dot11s::IeBeaconTiming> ());
  vector.AddInformationElement (Create<dot11s::IeRann> ());
  vector.AddInformationElement (Create<dot11s::IePreq> ());
  vector.AddInformationElement (Create<dot11s::IePrep> ());
  vector.AddInformationElement (Create<dot11s::IePerr> ());
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (vector);
  WifiInformationElementVector resultVector;
  packet->RemoveHeader (resultVector);
  NS_TEST_ASSERT (vector == resultVector);
  
  return result;
}

#endif // RUN_SELF_TESTS
} //namespace ns3
