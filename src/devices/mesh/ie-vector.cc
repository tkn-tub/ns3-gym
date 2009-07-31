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

#include "ie-vector.h"
#include "ns3/test.h"
#include "ns3/packet.h"
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
  if (element->GetSerializedSize () + GetSize () > m_maxSize)
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
WifiInformationElementVector
WifiInformationElementVector::DeserializePacket (Ptr<Packet> packet)
{
  WifiInformationElementVector retval;
  EmptyIe ie;
  while (packet->PeekHeader (ie))
    {
      Ptr<WifiInformationElement> newElement;
      switch (ie.GetElementId ())
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
        NS_FATAL_ERROR ("Information element " << (uint16_t) ie.GetElementId () << " is not implemented");
        }
      packet->RemoveHeader (*newElement);
      if (!retval.AddInformationElement (newElement))
        {
          NS_FATAL_ERROR ("Check max size for information element!");
        }
      if (packet->GetSize () == 0)
        {
          return retval;
        }
    }
  return retval;
}
Ptr<Packet>
WifiInformationElementVector::MakePacket (bool sortByElementId)
{
  if (sortByElementId)
    {
      //TODO: sort
    }
  Ptr<Packet> packet = Create<Packet> ();
  for (IE_VECTOR::const_iterator i = m_elements.begin (); i != m_elements.end (); i++)
    {
      packet->AddHeader (**i);
    }
  return packet;
}
uint32_t
WifiInformationElementVector::GetSize () const
{
  uint32_t size = 0;
  for (IE_VECTOR::const_iterator i = m_elements.begin (); i != m_elements.end (); i++)
    {
      size += (*i)->GetSerializedSize ();
    }
  return size;
}
WifiInformationElementVector::EmptyIe::~EmptyIe ()
{
}
WifiInformationElementVector::EmptyIe::EmptyIe () :
  m_elementId (0), m_length (0)
{
}
TypeId
WifiInformationElementVector::EmptyIe::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiInformationElementVector::EmptyIe")
    .SetParent<Header> ();
  return tid;
}
TypeId
WifiInformationElementVector::EmptyIe::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint8_t
WifiInformationElementVector::EmptyIe::GetLength ()
{
  return m_length;
}
uint8_t
WifiInformationElementVector::EmptyIe::GetElementId ()
{
  return m_elementId;
}
uint32_t
WifiInformationElementVector::EmptyIe::GetSerializedSize () const
{
  return 2;
}
void
WifiInformationElementVector::EmptyIe::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_elementId);
  start.WriteU8 (m_length);
}
uint32_t
WifiInformationElementVector::EmptyIe::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_elementId = i.ReadU8 ();
  m_length = i.ReadU8 ();
  return i.GetDistanceFrom (start);
}
void
WifiInformationElementVector::EmptyIe::Print (std::ostream &os) const
{
}
}
