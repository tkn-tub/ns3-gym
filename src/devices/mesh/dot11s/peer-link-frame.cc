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

PeerLinkFrameStart::PeerLinkFrameStart () :
  m_subtype (255), m_capability (0), m_aid (0), m_rates (SupportedRates ()), m_meshId (),
      m_config (IeConfiguration ()), m_reasonCode ((uint16_t) REASON11S_RESERVED)
{
}
void
PeerLinkFrameStart::SetPlinkFrameSubtype (uint8_t subtype)
{
  m_subtype = subtype;
}
void
PeerLinkFrameStart::SetPlinkFrameStart (PeerLinkFrameStart::PlinkFrameStartFields fields)
{
  m_subtype = fields.subtype;
  m_protocol = fields.protocol;
  if (m_subtype != (uint8_t) (WifiActionHeader::PEER_LINK_CLOSE))
    {
      m_capability = fields.capability;
    }
  if (m_subtype == (uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM))
    {
      m_aid = fields.aid;
    }
  if (m_subtype != (uint8_t) (WifiActionHeader::PEER_LINK_CLOSE))
    {
      m_rates = fields.rates;
    }
  if (m_subtype != (uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM))
    {
      m_meshId = fields.meshId;
    }
  if (m_subtype != (uint8_t) (WifiActionHeader::PEER_LINK_CLOSE))
    {
      m_config = fields.config;
    }
  else
    {
      m_reasonCode = fields.reasonCode;
    }
}
PeerLinkFrameStart::PlinkFrameStartFields
PeerLinkFrameStart::GetFields () const
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
TypeId
PeerLinkFrameStart::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dot11s::PeerLinkFrameStart") .SetParent<Header> () .AddConstructor<
      PeerLinkFrameStart> ();
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
  os << "subtype = " << (uint16_t) m_subtype << std::endl << "capability = " << m_capability << std::endl << "laid = "
      << (uint16_t) m_aid << std::endl << "rates = " << m_rates << std::endl << "meshId = ";
  m_meshId.Print (os);
  os << std::endl << "configuration = ";
  m_config.Print (os);
  os << std::endl << "reason code = " << m_reasonCode;
}
uint32_t
PeerLinkFrameStart::GetSerializedSize () const
{
  uint32_t size = 3; //Peering protocol
  NS_ASSERT (m_subtype < 3);
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      size += 2; //capability
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM) == m_subtype)
    {
      size += 2; //AID of remote peer
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      size += m_rates.GetSerializedSize ();
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM) != m_subtype)
    {
      size += m_meshId.GetInformationSize () + 2;
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      size += m_config.GetInformationSize () + 2;
    }
  else
    {
      size += 2; //reasonCode
    }
  return size;
}
void
PeerLinkFrameStart::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  NS_ASSERT (m_subtype < 3);
  i.WriteU8 (IE11S_MESH_PEERING_PROTOCOL_VERSION);
  i.WriteU8 (m_protocol.GetInformationSize ());
  m_protocol.SerializeInformation (i);
  i.Next (m_protocol.GetInformationSize ());
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      i.WriteHtolsbU16 (m_capability);
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM) == m_subtype)
    {
      i.WriteHtolsbU16 (m_aid);
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      i = m_rates.Serialize (i);
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM) != m_subtype)
    {
      i.WriteU8 (IE11S_MESH_ID);
      i.WriteU8 (m_meshId.GetInformationSize ());
      m_meshId.SerializeInformation (i);
      i.Next (m_meshId.GetInformationSize ());
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      i.WriteU8 (IE11S_MESH_CONFIGURATION);
      i.WriteU8 (m_config.GetInformationSize ());
      m_config.SerializeInformation (i);
      i.Next (m_config.GetInformationSize ());
    }
  else
    {
      i.WriteHtolsbU16 (m_reasonCode);
    }
}
uint32_t
PeerLinkFrameStart::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  NS_ASSERT (m_subtype < 3);
  {
      uint8_t id = i.ReadU8 ();
      uint8_t length = i.ReadU8 ();
      m_protocol.DeserializeInformation (i, length);
      if ((m_protocol.ElementId () != (WifiElementId) id) || (m_protocol.GetInformationSize () != length))
        {
          NS_FATAL_ERROR ("Broken frame: Element ID does not match IE itself!");
        }
      i.Next (m_protocol.GetInformationSize ());
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      m_capability = i.ReadLsbtohU16 ();
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM) == m_subtype)
    {
      m_aid = i.ReadLsbtohU16 ();
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      i = m_rates.Deserialize (i);
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CONFIRM) != m_subtype)
    {
      uint8_t id = i.ReadU8 ();
      uint8_t length = i.ReadU8 ();
      m_meshId.DeserializeInformation (i, length);
      if ((m_meshId.ElementId () != (WifiElementId) id) || (m_meshId.GetInformationSize () != length))
        {
          NS_FATAL_ERROR ("Broken frame: Element ID does not match IE itself!");
        }
      i.Next (m_meshId.GetInformationSize ());
    }
  if ((uint8_t) (WifiActionHeader::PEER_LINK_CLOSE) != m_subtype)
    {
      uint8_t id = i.ReadU8 ();
      uint8_t length = i.ReadU8 ();
      m_config. DeserializeInformation (i, length);
      if ((m_config.ElementId () != (WifiElementId) id) || (m_config.GetInformationSize () != length))
        {
          NS_FATAL_ERROR ("Broken frame: Element ID does not match IE itself!");
        }
      i.Next (m_config.GetInformationSize ());
    }
  else
    {
      m_reasonCode = i.ReadLsbtohU16 ();
    }
  return i.GetDistanceFrom (start);
}
bool
operator== (const PeerLinkFrameStart & a, const PeerLinkFrameStart & b)
{
  return ((a.m_subtype == b.m_subtype) && (a.m_capability == b.m_capability) && (a.m_aid == b.m_aid)
      && (a.m_meshId.IsEqual (b.m_meshId)) && (a.m_config == b.m_config)
      && (a.m_reasonCode == b.m_reasonCode));
}
} // namespace dot11s
} //namespace ns3

