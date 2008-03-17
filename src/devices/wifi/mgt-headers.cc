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
    ;
  return tid;
}
TypeId 
MgtProbeRequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MgtProbeRequestHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<MgtProbeRequestHeader> ("MgtProbeRequestHeader.ns3.inria.fr");
  return uid;
}
std::string
MgtProbeRequestHeader::GetName (void) const
{
  return "PROBEREQ";
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
    ;
  return tid;
}
TypeId 
MgtProbeResponseHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MgtProbeResponseHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<MgtProbeResponseHeader> ("MgtProbeResponseHeader.ns3.inria.fr");
  return uid;
}
std::string
MgtProbeResponseHeader::GetName (void) const
{
  return "PROBERESP";
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
  size += 3; // ds parameter set
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
  i.Next (3); // ds parameter set.
}
uint32_t
MgtProbeResponseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i.Next (8); // timestamp
  m_beaconInterval = i.ReadNtohU16 ();
  m_beaconInterval *= 1024;
  i = m_capability.Deserialize (i);
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  i.Next (3); // ds parameter set
  return i.GetDistanceFrom (start);
}

/***********************************************************
 *          Assoc Request
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAssocRequestHeader);

MgtAssocRequestHeader::MgtAssocRequestHeader ()
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
    ;
  return tid;
}
TypeId 
MgtAssocRequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MgtAssocRequestHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<MgtAssocRequestHeader> ("MgtAssocRequestHeader.ns3.inria.fr");
  return uid;
}
std::string
MgtAssocRequestHeader::GetName (void) const
{
  return "ASSOCREQ";
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
    ;
  return tid;
}
TypeId 
MgtAssocResponseHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MgtAssocResponseHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<MgtAssocResponseHeader> ("MgtAssocResponseHeader.ns3.inria.fr");
  return uid;
}
std::string
MgtAssocResponseHeader::GetName (void) const
{
  return "ASSOCRESP";
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

} // namespace ns3
