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

#include "ns3/peer-link-frame.h"
#include "ns3/mesh-wifi-interface-mac.h"
namespace ns3 {
namespace dot11s {
NS_OBJECT_ENSURE_REGISTERED (PeerLinkFrameStart);

PeerLinkFrameStart::PeerLinkFrameStart ()
{
  m_fields.subtype = IeDot11sPeerManagement::PEER_OPEN;
  m_fields.aid = 0;
  m_fields.rates = SupportedRates();
  m_fields.meshId = Ssid();
}
void
PeerLinkFrameStart::SetPlinkFrameStart(PeerLinkFrameStart::PlinkFrameStartFields fields)
{
  m_fields = fields;
}

PeerLinkFrameStart::PlinkFrameStartFields
PeerLinkFrameStart::GetFields ()
{
  return m_fields;
}

bool
PeerLinkFrameStart::CheckPlinkFrameStart(Ptr<MeshWifiInterfaceMac> mac)
{
  bool retval;
  retval = mac->CheckSupportedRates(m_fields.rates);
  if(! retval)
    return retval;
  retval = mac->CheckMeshId(m_fields.meshId);
  return true;
}

TypeId
PeerLinkFrameStart::GetTypeId ()
{
  static TypeId tid =
    TypeId ("ns3::PeerLinkFrameStart")
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
  //TODO:fill this method
}

uint32_t
PeerLinkFrameStart::GetSerializedSize () const
{
  uint32_t size = 1; //Subtype
  if (IeDot11sPeerManagement::PEER_CONFIRM == m_fields.subtype)
    size += 2; //AID of remote peer
  if (IeDot11sPeerManagement::PEER_CLOSE != m_fields.subtype)
    {
      size += m_fields.rates.GetSerializedSize ();
      size += 2;
      size += m_fields.meshId.GetSerializedSize ();
    }
  return size;
}

void
PeerLinkFrameStart::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_fields.subtype); //Like a Category in Standart
  if (IeDot11sPeerManagement::PEER_CONFIRM == m_fields.subtype)
    i.WriteHtonU16 (m_fields.aid);
  if (IeDot11sPeerManagement::PEER_CLOSE != m_fields.subtype)
    {
      i = m_fields.rates.Serialize (i);
      i.Next(2); //QoS
      i = m_fields.meshId.Serialize (i);
    }
}

uint32_t
PeerLinkFrameStart::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_fields.subtype = (IeDot11sPeerManagement::Subtype)i.ReadU8 ();
  if (IeDot11sPeerManagement::PEER_CONFIRM == m_fields.subtype)
    m_fields.aid = i.ReadNtohU16 ();
  if (IeDot11sPeerManagement::PEER_CLOSE != m_fields.subtype)
    {
      i = m_fields.rates.Deserialize (i);
      i.Next(2);  //QoS
      i = m_fields.meshId.Deserialize (i);
    }
  return i.GetDistanceFrom (start);
}
  
} // namespace dot11s
} //namespace ns3

