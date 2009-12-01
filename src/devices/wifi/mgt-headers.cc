/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "mgt-headers.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"

namespace ns3 {

/***********************************************************
 *          Probe Request
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtProbeRequestHeader);

MgtProbeRequestHeader::~MgtProbeRequestHeader ()
{}

void 
MgtProbeRequestHeader::SetSsid (Ssid ssid)
{
  m_ssid = ssid;
}
Ssid 
MgtProbeRequestHeader::GetSsid (void) const
{
  return m_ssid;
}
void 
MgtProbeRequestHeader::SetSupportedRates (SupportedRates rates)
{
  m_rates = rates;
}

SupportedRates 
MgtProbeRequestHeader::GetSupportedRates (void) const
{
  return m_rates;
}
uint32_t
MgtProbeRequestHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += m_ssid.GetSerializedSize ();
  size += m_rates.GetSerializedSize ();
  return size;
}
TypeId 
MgtProbeRequestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MgtProbeRequestHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtProbeRequestHeader> ()
    ;
  return tid;
}
TypeId 
MgtProbeRequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void 
MgtProbeRequestHeader::Print (std::ostream &os) const
{
  os << "ssid="<<m_ssid<<", "
     << "rates="<<m_rates;
}
void
MgtProbeRequestHeader::Serialize (Buffer::Iterator start) const
{  
  Buffer::Iterator i = start;
  i = m_ssid.Serialize (i);
  i = m_rates.Serialize (i);
}
uint32_t
MgtProbeRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  return i.GetDistanceFrom (start);
}


/***********************************************************
 *          Probe Response
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtProbeResponseHeader);

MgtProbeResponseHeader::MgtProbeResponseHeader ()
{}
MgtProbeResponseHeader::~MgtProbeResponseHeader ()
{}
uint64_t
MgtProbeResponseHeader::GetTimestamp()
{
  return m_timestamp;
}
Ssid 
MgtProbeResponseHeader::GetSsid (void) const
{
  return m_ssid;
}
uint64_t 
MgtProbeResponseHeader::GetBeaconIntervalUs (void) const
{
  return m_beaconInterval;
}
SupportedRates 
MgtProbeResponseHeader::GetSupportedRates (void) const
{
  return m_rates;
}

void 
MgtProbeResponseHeader::SetSsid (Ssid ssid)
{
  m_ssid = ssid;
}
void 
MgtProbeResponseHeader::SetBeaconIntervalUs (uint64_t us)
{
  m_beaconInterval = us;
}
void
MgtProbeResponseHeader::SetSupportedRates (SupportedRates rates)
{
  m_rates = rates;
}
TypeId 
MgtProbeResponseHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MgtProbeResponseHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtProbeResponseHeader> ()
    ;
  return tid;
}
TypeId 
MgtProbeResponseHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MgtProbeResponseHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += 8; // timestamp
  size += 2; // beacon interval
  size += m_capability.GetSerializedSize ();
  size += m_ssid.GetSerializedSize ();
  size += m_rates.GetSerializedSize ();
  //size += 3; // ds parameter set
  // xxx
  return size;
}
void 
MgtProbeResponseHeader::Print (std::ostream &os) const
{
  os << "ssid="<<m_ssid<<", "
     << "rates="<<m_rates;
}
void 
MgtProbeResponseHeader::Serialize (Buffer::Iterator start) const
{
  // timestamp
  // beacon interval
  // capability information
  // ssid
  // supported rates
  // fh parameter set
  // ds parameter set
  // cf parameter set
  // ibss parameter set
  //XXX
  Buffer::Iterator i = start;
  i.WriteHtonU64 (Simulator::Now ().GetMicroSeconds ());
  i.WriteHtonU16 (m_beaconInterval / 1024);
  i = m_capability.Serialize (i);
  i = m_ssid.Serialize (i);
  i = m_rates.Serialize (i);
  //i.WriteU8 (0, 3); // ds parameter set.
}
uint32_t
MgtProbeResponseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_timestamp = i.ReadNtohU64();
  m_beaconInterval = i.ReadNtohU16 ();
  m_beaconInterval *= 1024;
  i = m_capability.Deserialize (i);
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  //i.Next (3); // ds parameter set
  return i.GetDistanceFrom (start);
}

/***********************************************************
 *          Assoc Request
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAssocRequestHeader);

MgtAssocRequestHeader::MgtAssocRequestHeader ()
  : m_listenInterval (0)
{}
MgtAssocRequestHeader::~MgtAssocRequestHeader ()
{}

void 
MgtAssocRequestHeader::SetSsid (Ssid ssid)
{
  m_ssid = ssid;
}
void 
MgtAssocRequestHeader::SetSupportedRates (SupportedRates rates)
{
  m_rates = rates;
}
void
MgtAssocRequestHeader::SetListenInterval (uint16_t interval)
{
  m_listenInterval = interval;
}
Ssid 
MgtAssocRequestHeader::GetSsid (void) const
{
  return m_ssid;
}
SupportedRates 
MgtAssocRequestHeader::GetSupportedRates (void) const
{
  return m_rates;
}
uint16_t
MgtAssocRequestHeader::GetListenInterval (void) const
{
  return m_listenInterval;
}

TypeId 
MgtAssocRequestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MgtAssocRequestHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtAssocRequestHeader> ()
    ;
  return tid;
}
TypeId 
MgtAssocRequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t 
MgtAssocRequestHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += m_capability.GetSerializedSize ();
  size += 2;
  size += m_ssid.GetSerializedSize ();
  size += m_rates.GetSerializedSize ();
  return size;
}
void 
MgtAssocRequestHeader::Print (std::ostream &os) const
{
  os << "ssid="<<m_ssid<<", "
     << "rates="<<m_rates;
}
void 
MgtAssocRequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i = m_capability.Serialize (i);
  i.WriteHtonU16 (m_listenInterval);
  i = m_ssid.Serialize (i);
  i = m_rates.Serialize (i);
}
uint32_t
MgtAssocRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i = m_capability.Deserialize (i);
  m_listenInterval = i.ReadNtohU16 ();
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  return i.GetDistanceFrom (start);
}

/***********************************************************
 *          Assoc Response
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAssocResponseHeader);

MgtAssocResponseHeader::MgtAssocResponseHeader ()
  : m_aid (0)
{}
MgtAssocResponseHeader::~MgtAssocResponseHeader ()
{}

StatusCode 
MgtAssocResponseHeader::GetStatusCode (void)
{
  return m_code;
}
SupportedRates 
MgtAssocResponseHeader::GetSupportedRates (void)
{
  return m_rates;
}
void 
MgtAssocResponseHeader::SetStatusCode (StatusCode code)
{
  m_code = code;
}
void 
MgtAssocResponseHeader::SetSupportedRates (SupportedRates rates)
{
  m_rates = rates;
}

TypeId 
MgtAssocResponseHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MgtAssocResponseHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtAssocResponseHeader> ()
    ;
  return tid;
}
TypeId 
MgtAssocResponseHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MgtAssocResponseHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += m_capability.GetSerializedSize ();
  size += m_code.GetSerializedSize ();
  size += 2; // aid
  size += m_rates.GetSerializedSize ();
  return size;
}

void 
MgtAssocResponseHeader::Print (std::ostream &os) const
{
  os << "status code="<<m_code<<", "
     << "rates="<<m_rates;
}
void 
MgtAssocResponseHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i = m_capability.Serialize (i);
  i = m_code.Serialize (i);
  i.WriteHtonU16 (m_aid);
  i = m_rates.Serialize (i);
}
uint32_t
MgtAssocResponseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i = m_capability.Deserialize (i);
  i = m_code.Deserialize (i);
  m_aid = i.ReadNtohU16 ();
  i = m_rates.Deserialize (i);
  return i.GetDistanceFrom (start);
}
/**********************************************************
 *   ActionFrame
 **********************************************************/
WifiActionHeader::WifiActionHeader ()
{
}
WifiActionHeader::~WifiActionHeader ()
{
}
void
WifiActionHeader::SetAction (WifiActionHeader::CategoryValue type,
    WifiActionHeader::ActionValue action)
{
  m_category = type;

  switch (type)
    {
  case MESH_PEERING_MGT:
    {
      m_actionValue = action.peerLink;
      break;
    }
  case MESH_PATH_SELECTION:
    {
      m_actionValue = action.pathSelection;
      break;
    }
  case MESH_LINK_METRIC:
  case MESH_INTERWORKING:
  case MESH_RESOURCE_COORDINATION:
  case MESH_PROXY_FORWARDING:
    break;
    }
}
WifiActionHeader::CategoryValue
WifiActionHeader::GetCategory ()
{
  switch (m_category)
    {
  case MESH_PEERING_MGT:
    return MESH_PEERING_MGT;
  case MESH_LINK_METRIC:
    return MESH_LINK_METRIC;
  case MESH_PATH_SELECTION:
    return MESH_PATH_SELECTION;
  case MESH_INTERWORKING:
    return MESH_INTERWORKING;
  case MESH_RESOURCE_COORDINATION:
    return MESH_RESOURCE_COORDINATION;
  case MESH_PROXY_FORWARDING:
    return MESH_PROXY_FORWARDING;
  default:
    NS_FATAL_ERROR ("Unknown action value");
    return MESH_PEERING_MGT;
    }
}
WifiActionHeader::ActionValue
WifiActionHeader::GetAction ()
{
  ActionValue retval;
  retval.peerLink = PEER_LINK_OPEN; // Needs to be initialized to something to quiet valgrind in default cases
  switch (m_category)
    {
  case MESH_PEERING_MGT:
    switch (m_actionValue)
      {
    case PEER_LINK_OPEN:
      retval.peerLink = PEER_LINK_OPEN;
      return retval;
    case PEER_LINK_CONFIRM:
      retval.peerLink = PEER_LINK_CONFIRM;
      return retval;
    case PEER_LINK_CLOSE:
      retval.peerLink = PEER_LINK_CLOSE;
      return retval;
    default:
      NS_FATAL_ERROR ("Unknown mesh peering management action code");
      retval.peerLink = PEER_LINK_OPEN;  /* quiet compiler */
      return retval;
      }
  case MESH_PATH_SELECTION:
    switch (m_actionValue)
      {
    case PATH_SELECTION:
      retval.pathSelection = PATH_SELECTION;
      return retval;
    default:
      NS_FATAL_ERROR ("Unknown mesh path selection action code");
      retval.peerLink = PEER_LINK_OPEN;  /* quiet compiler */
      return retval;
      }
  case MESH_LINK_METRIC:
    // not yet supported
  case MESH_INTERWORKING:
    // not yet supported
  case MESH_RESOURCE_COORDINATION:
    // not yet supported
  default:
    NS_FATAL_ERROR ("Unsupported mesh action");
    retval.peerLink = PEER_LINK_OPEN;  /* quiet compiler */
    return retval;
    }
}
TypeId
WifiActionHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiActionHeader")
  .SetParent<Header> ()
  .AddConstructor<WifiActionHeader> ();
  return tid;
}
TypeId
WifiActionHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
void
WifiActionHeader::Print (std::ostream &os) const
{
}
uint32_t
WifiActionHeader::GetSerializedSize () const
{
  return 2;
}
void
WifiActionHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_category);
  start.WriteU8 (m_actionValue);
}
uint32_t
WifiActionHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_category = i.ReadU8 ();
  m_actionValue = i.ReadU8 ();
  return i.GetDistanceFrom (start);
}

} // namespace ns3
