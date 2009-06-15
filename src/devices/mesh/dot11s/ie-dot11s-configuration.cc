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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#include "ie-dot11s-configuration.h"
#include "ns3/test.h"
#include "ns3/packet.h"
namespace ns3 {
namespace dot11s {

dot11sMeshCapability::dot11sMeshCapability ():
    acceptPeerLinks (true),
    MCCASupported (false),
    MCCAEnabled (false),
    forwarding (true),
    beaconTimingReport (true),
    TBTTAdjustment (true),
    powerSaveLevel (false)
{}
uint8_t dot11sMeshCapability::GetSerializedSize () const
{
  return 2;
}
uint16_t dot11sMeshCapability::GetUint16 () const
{
  uint16_t result = 0;
  if (acceptPeerLinks)
    result |= 1 << 0;
  if (MCCASupported)
    result |= 1 << 1;
  if (MCCAEnabled)
    result |= 1 << 2;
  if (forwarding)
    result |= 1 << 3;
  if (beaconTimingReport)
    result |= 1 << 4;
  if (TBTTAdjustment)
    result |= 1 << 5;
  if (powerSaveLevel)
    result |= 1 << 6;
  return result;
}
Buffer::Iterator dot11sMeshCapability::Serialize (Buffer::Iterator i) const
{
  i.WriteHtolsbU16 (GetUint16 ());
  return i;
}
Buffer::Iterator dot11sMeshCapability::Deserialize (Buffer::Iterator i)
{
  uint16_t  cap = i.ReadLsbtohU16 ();
  acceptPeerLinks    = Is (cap, 0);
  MCCASupported      = Is (cap, 1);
  MCCAEnabled        = Is (cap, 2);
  forwarding         = Is (cap, 3);
  beaconTimingReport = Is (cap, 4);
  TBTTAdjustment     = Is (cap, 5);
  powerSaveLevel     = Is (cap, 6);
  return i;
}
bool dot11sMeshCapability::Is (uint16_t cap, uint8_t n) const
{
  uint16_t mask = 1<<n;
  return (cap & mask) == mask;
}
IeConfiguration::IeConfiguration ():
    m_APSPId (PROTOCOL_HWMP),
    m_APSMId (METRIC_AIRTIME),
    m_CCMId (CONGESTION_NULL),
    m_SPId (SYNC_NEIGHBOUR_OFFSET),
    m_APId (AUTH_NULL),
    m_neighbors (0)
{}
uint8_t
IeConfiguration::GetInformationSize () const
{
  return 1 // Version
    + 4 // APSPId
    + 4 // APSMId
    + 4 // CCMId
    + 4 // SPId
    + 4 // APId
    + 1 // Mesh formation info (see 7.3.2.86.6 of 802.11s draft 3.0)
    + m_meshCap.GetSerializedSize ();
}
void
IeConfiguration::SerializeInformation (Buffer::Iterator i) const
{
  i.WriteU8 (1); //Version
  // Active Path Selection Protocol ID:
  i.WriteHtolsbU32 (m_APSPId);
  // Active Path Metric ID:
  i.WriteHtolsbU32 (m_APSMId);
  // Congestion Control Mode ID:
  i.WriteHtolsbU32 (m_CCMId);
  // Sync:
  i.WriteHtolsbU32 (m_SPId);
  // Auth:
  i.WriteHtolsbU32 (m_APId);
  i.WriteU8 (m_neighbors << 1);
  m_meshCap.Serialize (i);
}
uint8_t
IeConfiguration::DeserializeInformation (Buffer::Iterator i, uint8_t length)
{
  Buffer::Iterator start = i;
  uint8_t version;
  version  = i.ReadU8 ();
  // Active Path Selection Protocol ID:
  m_APSPId  = (dot11sPathSelectionProtocol)i.ReadLsbtohU32 ();
  // Active Path Metric ID:
  m_APSMId = (dot11sPathSelectionMetric)i.ReadLsbtohU32 ();
  // Congestion Control Mode ID:
  m_CCMId  = (dot11sCongestionControlMode)i.ReadLsbtohU32 ();
  m_SPId   = (dot11sSynchronizationProtocolIdentifier)i.ReadLsbtohU32 ();
  m_APId   = (dot11sAuthenticationProtocol)i.ReadLsbtohU32 ();
  m_neighbors = (i.ReadU8 () >> 1) & 0xF;
  i = m_meshCap.Deserialize (i);
  return i.GetDistanceFrom (start);
}
void
IeConfiguration::PrintInformation (std::ostream& os) const
{
  os<<"Number of neighbors:               = " << (uint16_t)m_neighbors <<
    "\nActive Path Selection Protocol ID: = " << (uint32_t)m_APSPId <<
    "\nActive Path Selection Metric ID:   = " << (uint32_t)m_APSMId <<
    "\nCongestion Control Mode ID:        = " << (uint32_t)m_CCMId <<
    "\nSynchronize protocol ID:           = " << (uint32_t)m_SPId <<
    "\nAuthentication protocol ID:        = " << (uint32_t)m_APId <<
    "\nCapabilities:                      = " << m_meshCap.GetUint16 () << "\n";
}
void
IeConfiguration::SetRouting (dot11sPathSelectionProtocol routingId)
{
  m_APSPId  =  routingId;
}
void
IeConfiguration::SetMetric (dot11sPathSelectionMetric metricId)
{
  m_APSMId =  metricId;
}
bool
IeConfiguration::IsHWMP ()
{
  return (m_APSPId == PROTOCOL_HWMP);
}
bool
IeConfiguration::IsAirtime ()
{
  return (m_APSMId  == METRIC_AIRTIME);
}
void
IeConfiguration::SetNeighborCount (uint8_t neighbors)
{
  m_neighbors = (neighbors > 31) ? 31 : neighbors;
}
uint8_t
IeConfiguration::GetNeighborCount ()
{
  return m_neighbors;
}
dot11sMeshCapability const& IeConfiguration::MeshCapability ()
{
  return m_meshCap;
}
bool operator== (const dot11sMeshCapability & a, const dot11sMeshCapability & b)
{
  return (
      (a.acceptPeerLinks == b.acceptPeerLinks) &&
      (a.MCCASupported == b.MCCASupported) &&
      (a.MCCAEnabled == b.MCCAEnabled) &&
      (a.forwarding == b.forwarding) &&
      (a.beaconTimingReport == b.beaconTimingReport) &&
      (a.TBTTAdjustment == b.TBTTAdjustment) &&
      (a.powerSaveLevel == b.powerSaveLevel)
      );
}
bool operator== (const IeConfiguration & a, const IeConfiguration & b)
{
  return (
      (a.m_APSPId == b.m_APSPId) &&
      (a.m_APSMId == b.m_APSMId) &&
      (a.m_CCMId == b.m_CCMId) &&
      (a.m_SPId == b.m_SPId) &&
      (a.m_APId == b.m_APId) &&
      (a.m_neighbors == b.m_neighbors) &&
      (a.m_meshCap == b.m_meshCap)
      );
}
#ifdef RUN_SELF_TESTS

/// Built-in self test for IePreq
struct IeConfigurationBist : public IeTest 
{
  IeConfigurationBist () : IeTest ("Mesh/802.11s/IE/Configuration") {}
  virtual bool RunTests(); 
};

/// Test instance
static IeConfigurationBist g_IeConfigurationBist;

bool IeConfigurationBist::RunTests ()
{
  bool result(true);
  IeConfiguration a;
  
  result = TestRoundtripSerialization (a);
  return result;
}
#endif
} // namespace dot11s
} //namespace ns3

