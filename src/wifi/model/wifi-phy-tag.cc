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
#include <ns3/wifi-phy-tag.h>

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
  return (4 + (6 * 1) + 4 + 2);
}

void
WifiPhyTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_wifiTxVector.GetMode ().GetUid ());
  i.WriteU8 (m_wifiTxVector.GetTxPowerLevel ());
  i.WriteU8 (m_wifiTxVector.GetRetries ());
  i.WriteU8 (m_wifiTxVector.IsShortGuardInterval ());
  i.WriteU8 (m_wifiTxVector.GetNss ());
  i.WriteU8 (m_wifiTxVector.GetNess ());
  i.WriteU8 (m_wifiTxVector.IsStbc ());
  i.WriteU32 (m_wifiPreamble);
  i.WriteU16 (m_mpduType);
}

void
WifiPhyTag::Deserialize (TagBuffer i)
{
  m_wifiTxVector.SetMode (WifiMode (i.ReadU32 ()));
  m_wifiTxVector.SetTxPowerLevel (i.ReadU8 ());
  m_wifiTxVector.SetRetries (i.ReadU8 ());
  m_wifiTxVector.SetShortGuardInterval (i.ReadU8 ());
  m_wifiTxVector.SetNss (i.ReadU8 ());
  m_wifiTxVector.SetNess (i.ReadU8 ());
  m_wifiTxVector.SetStbc (i.ReadU8 ());
  m_wifiPreamble = i.ReadU32 ();
  m_mpduType = static_cast<enum mpduType> (i.ReadU16 ());
}
void
WifiPhyTag::Print (std::ostream &os) const
{
  os << m_wifiTxVector << " " << (m_wifiPreamble ? "SP " : "LP ") << m_mpduType;
}

WifiPhyTag::WifiPhyTag ()
{
}

WifiPhyTag::WifiPhyTag (WifiTxVector txVector, WifiPreamble preamble, enum mpduType mpdutype)
  : m_wifiTxVector (txVector),
    m_wifiPreamble (preamble),
    m_mpduType (mpdutype)
{
}

WifiTxVector
WifiPhyTag::GetWifiTxVector (void) const
{
  return m_wifiTxVector;
}

WifiPreamble
WifiPhyTag::GetWifiPreamble (void) const
{
  return ((enum WifiPreamble) m_wifiPreamble);
}

enum mpduType
WifiPhyTag::GetMpduType (void) const
{
  return m_mpduType;
}

} // namespace ns3
