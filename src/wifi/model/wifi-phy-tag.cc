/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include "wifi-phy-tag.h"

namespace ns3 {

TypeId
WifiPhyTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiPhyTag")
    .SetParent<Tag> ()
  ;
  return tid;
}

TypeId
WifiPhyTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
WifiPhyTag::GetSerializedSize (void) const
{
  return (sizeof (WifiTxVector) + 2 + 1);
}

void
WifiPhyTag::Serialize (TagBuffer i) const
{
  i.Write ((uint8_t *)&m_wifiTxVector, sizeof (WifiTxVector));
  i.WriteU16 (static_cast<uint16_t> (m_mpduType));
  i.WriteU8 (m_frameComplete);
}

void
WifiPhyTag::Deserialize (TagBuffer i)
{
  i.Read ((uint8_t *)&m_wifiTxVector, sizeof (WifiTxVector));
  m_mpduType = static_cast<MpduType> (i.ReadU16 ());
  m_frameComplete = i.ReadU8 ();
}

void
WifiPhyTag::Print (std::ostream &os) const
{
  os << m_wifiTxVector << " " << m_mpduType << " " << m_frameComplete;
}

WifiPhyTag::WifiPhyTag ()
{
}

WifiPhyTag::WifiPhyTag (WifiTxVector txVector, MpduType mpdutype, uint8_t frameComplete)
  : m_wifiTxVector (txVector),
    m_mpduType (mpdutype),
    m_frameComplete (frameComplete)
{
}

WifiTxVector
WifiPhyTag::GetWifiTxVector (void) const
{
  return m_wifiTxVector;
}

MpduType
WifiPhyTag::GetMpduType (void) const
{
  return m_mpduType;
}

uint8_t
WifiPhyTag::GetFrameComplete (void) const
{
  return m_frameComplete;
}

} // namespace ns3
