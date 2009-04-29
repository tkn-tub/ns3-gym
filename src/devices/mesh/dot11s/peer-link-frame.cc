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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */

#include "peer-link-frame.h"
#include "ie-dot11s-peer-management.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/test.h"
#include "ns3/packet.h"

namespace ns3 {
namespace dot11s {
NS_OBJECT_ENSURE_REGISTERED (PeerLinkFrameStart);

PeerLinkFrameStart::PeerLinkFrameStart ():
  m_subtype (255),
  m_capability (0),
  m_aid (0),
  m_rates (SupportedRates()),
  m_meshId (Ssid()),
  m_config(IeConfiguration ()),
  m_reasonCode ((uint16_t)REASON11S_RESERVED)
{
}
void
PeerLinkFrameStart::SetPlinkFrameSubtype(uint8_t subtype)
{
  m_subtype = subtype;
}
void
PeerLinkFrameStart::SetPlinkFrameStart(PeerLinkFrameStart::PlinkFrameStartFields fields)
{
  m_subtype = fields.subtype;
  m_protocol = fields.protocol;
  if(m_subtype != (uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE))
    m_capability = fields.capability;
  if(m_subtype == (uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM))
    m_aid = fields.aid;
  if(m_subtype != (uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE))
    m_rates = fields.rates;
  if(m_subtype != (uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM))
    m_meshId = fields.meshId;
  if(m_subtype != (uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE))
    m_config = fields.config;
  else
    m_reasonCode = fields.reasonCode;
}

PeerLinkFrameStart::PlinkFrameStartFields
PeerLinkFrameStart::GetFields ()
{
  PlinkFrameStartFields retval;
  //TODO: protocol version:
  retval.subtype = m_subtype;
  retval.capability = m_capability;
  retval.aid = m_aid;
  retval.rates = m_rates;
  retval.meshId = m_meshId;
  retval.config = m_config;
  retval.reasonCode = m_reasonCode;
  return retval;
}

bool
PeerLinkFrameStart::CheckPlinkFrameStart(Ptr<MeshWifiInterfaceMac> mac)
{
  bool retval;
  retval = mac->CheckSupportedRates(m_rates);
  if(! retval)
    return retval;
  retval = mac->CheckMeshId(m_meshId);
  return true;
}

TypeId
PeerLinkFrameStart::GetTypeId ()
{
  static TypeId tid =
    TypeId ("ns3::dot11s::PeerLinkFrameStart")
    .SetParent<Header> ()
    .AddConstructor<PeerLinkFrameStart> ()
    ;
  return tid;
}

TypeId
PeerLinkFrameStart::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
PeerLinkFrameStart::Print (std::ostream &os) const
{
  os << "subtype = " << (uint16_t)m_subtype
  << "\ncapability = " << m_capability
  << "\naid = " << (uint16_t)m_aid
  << "\nrates = " << m_rates
  << "\nmeshId = " << m_meshId
  << "\nconfiguration = " << m_config
  << "\nreason code = " << m_reasonCode;
}

uint32_t
PeerLinkFrameStart::GetSerializedSize () const
{
  uint32_t size = 3; //Peering protocol
  NS_ASSERT(m_subtype < 3);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    size += 2;  //capability
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM) == m_subtype)
    size += 2; //AID of remote peer
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    size += m_rates.GetSerializedSize ();
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM) != m_subtype)
    size += m_meshId.GetSerializedSize ();
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    size += m_config.GetSerializedSize ();
  else
    size += 2; //reasonCode
  return size;
}

void
PeerLinkFrameStart::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  NS_ASSERT(m_subtype < 3);
  m_protocol.Serialize (i);
  i.Next (m_protocol.GetSerializedSize ());
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    i.WriteHtonU16(m_capability);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM) == m_subtype)
    i.WriteHtonU16 (m_aid);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    i = m_rates.Serialize (i);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM) != m_subtype)
    i = m_meshId.Serialize (i);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
  {
    m_config.Serialize (i);
    i.Next(m_config.GetSerializedSize ());
  }
  else
    i.WriteHtonU16(m_reasonCode);
}

uint32_t
PeerLinkFrameStart::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  NS_ASSERT(m_subtype < 3);
  m_protocol.Deserialize (i);
  i.Next (m_protocol.GetSerializedSize ());
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    m_capability = i.ReadNtohU16();
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM) == m_subtype)
    m_aid = i.ReadNtohU16 ();
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
    i = m_rates.Deserialize (i);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM) != m_subtype)
    i = m_meshId.Deserialize (i);
  if ((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE) != m_subtype)
  {
    m_config.Deserialize (i);
    i.Next (m_config.GetSerializedSize ());
  }
  else
    m_reasonCode = i.ReadNtohU16();
  return i.GetDistanceFrom (start);
}
bool operator== (const PeerLinkFrameStart & a, const PeerLinkFrameStart & b)
{
  return (
      (a.m_subtype == b.m_subtype) &&
      (a.m_capability == b.m_capability) &&
      (a.m_aid == b.m_aid) &&
      (a.m_meshId.IsEqual(b.m_meshId)) &&
      (a.m_config == b.m_config) &&
      (a.m_reasonCode == b.m_reasonCode)
      );
}
#ifdef RUN_SELF_TESTS

/// Built-in self test for PeerLinkFrameStart
struct PeerLinkFrameStartBist : public Test 
{
  PeerLinkFrameStartBist () : Test ("Mesh/802.11s/IE/PeerLinkFrameStart") {}
  virtual bool RunTests(); 
};

/// Test instance
static PeerLinkFrameStartBist g_PeerLinkFrameStartBist;

bool PeerLinkFrameStartBist::RunTests ()
{
  bool result (true);
  {
    PeerLinkFrameStart a;
    PeerLinkFrameStart::PlinkFrameStartFields fields;
    fields.subtype = (uint8_t)(WifiMeshActionHeader::PEER_LINK_OPEN);
    fields.aid = 101;
    fields.reasonCode = 12;
    fields.meshId = Ssid("qwertyuiop");
    a.SetPlinkFrameStart(fields);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    PeerLinkFrameStart b;
    b.SetPlinkFrameSubtype((uint8_t)(WifiMeshActionHeader::PEER_LINK_OPEN));
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (a, b);
  }
  {
    PeerLinkFrameStart a;
    PeerLinkFrameStart::PlinkFrameStartFields fields;
    fields.subtype = (uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM);
    fields.aid = 1234;
    fields.reasonCode = 12;
    fields.meshId = Ssid("qwerty");
    a.SetPlinkFrameStart(fields);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    PeerLinkFrameStart b;
    b.SetPlinkFrameSubtype((uint8_t)(WifiMeshActionHeader::PEER_LINK_CONFIRM));
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (a, b);
  }
  {
    PeerLinkFrameStart a;
    PeerLinkFrameStart::PlinkFrameStartFields fields;
    fields.subtype = (uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE);
    fields.aid = 10;
    fields.meshId = Ssid("qqq");
    fields.reasonCode = 12;
    a.SetPlinkFrameStart(fields);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    PeerLinkFrameStart b;
    b.SetPlinkFrameSubtype((uint8_t)(WifiMeshActionHeader::PEER_LINK_CLOSE));
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (a, b);
  }
  return result;  
}
#endif
} // namespace dot11s
} //namespace ns3

