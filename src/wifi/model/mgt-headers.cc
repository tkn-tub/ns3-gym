/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
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
{
}

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
  size += m_rates.extended.GetSerializedSize ();
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
  os << "ssid=" << m_ssid << ", "
     << "rates=" << m_rates;
}
void
MgtProbeRequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i = m_ssid.Serialize (i);
  i = m_rates.Serialize (i);
  i = m_rates.extended.Serialize (i);
}
uint32_t
MgtProbeRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  i = m_rates.extended.DeserializeIfPresent (i);
  return i.GetDistanceFrom (start);
}


/***********************************************************
 *          Probe Response
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtProbeResponseHeader);

MgtProbeResponseHeader::MgtProbeResponseHeader ()
{
}
MgtProbeResponseHeader::~MgtProbeResponseHeader ()
{
}
uint64_t
MgtProbeResponseHeader::GetTimestamp ()
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
  size += m_rates.extended.GetSerializedSize ();
  // xxx
  return size;
}
void
MgtProbeResponseHeader::Print (std::ostream &os) const
{
  os << "ssid=" << m_ssid << ", "
     << "rates=" << m_rates;
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
  i.WriteHtolsbU64 (Simulator::Now ().GetMicroSeconds ());
  i.WriteHtolsbU16 (m_beaconInterval / 1024);
  i = m_capability.Serialize (i);
  i = m_ssid.Serialize (i);
  i = m_rates.Serialize (i);
  //i.WriteU8 (0, 3); // ds parameter set.
  i = m_rates.extended.Serialize (i);
}
uint32_t
MgtProbeResponseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_timestamp = i.ReadLsbtohU64 ();
  m_beaconInterval = i.ReadLsbtohU16 ();
  m_beaconInterval *= 1024;
  i = m_capability.Deserialize (i);
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  //i.Next (3); // ds parameter set
  i = m_rates.extended.DeserializeIfPresent (i);
  return i.GetDistanceFrom (start);
}

/***********************************************************
 *          Assoc Request
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAssocRequestHeader);

MgtAssocRequestHeader::MgtAssocRequestHeader ()
  : m_listenInterval (0)
{
}
MgtAssocRequestHeader::~MgtAssocRequestHeader ()
{
}

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
  size += m_rates.extended.GetSerializedSize ();
  return size;
}
void
MgtAssocRequestHeader::Print (std::ostream &os) const
{
  os << "ssid=" << m_ssid << ", "
     << "rates=" << m_rates;
}
void
MgtAssocRequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i = m_capability.Serialize (i);
  i.WriteHtolsbU16 (m_listenInterval);
  i = m_ssid.Serialize (i);
  i = m_rates.Serialize (i);
  i = m_rates.extended.Serialize (i);
}
uint32_t
MgtAssocRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i = m_capability.Deserialize (i);
  m_listenInterval = i.ReadLsbtohU16 ();
  i = m_ssid.Deserialize (i);
  i = m_rates.Deserialize (i);
  i = m_rates.extended.DeserializeIfPresent (i);
  return i.GetDistanceFrom (start);
}

/***********************************************************
 *          Assoc Response
 ***********************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAssocResponseHeader);

MgtAssocResponseHeader::MgtAssocResponseHeader ()
  : m_aid (0)
{
}
MgtAssocResponseHeader::~MgtAssocResponseHeader ()
{
}

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
  size += m_rates.extended.GetSerializedSize ();
  return size;
}

void
MgtAssocResponseHeader::Print (std::ostream &os) const
{
  os << "status code=" << m_code << ", "
     << "rates=" << m_rates;
}
void
MgtAssocResponseHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i = m_capability.Serialize (i);
  i = m_code.Serialize (i);
  i.WriteHtolsbU16 (m_aid);
  i = m_rates.Serialize (i);
  i = m_rates.extended.Serialize (i);
}
uint32_t
MgtAssocResponseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i = m_capability.Deserialize (i);
  i = m_code.Deserialize (i);
  m_aid = i.ReadLsbtohU16 ();
  i = m_rates.Deserialize (i);
  i = m_rates.extended.DeserializeIfPresent (i);
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
    case BLOCK_ACK:
      {
        m_actionValue = action.blockAck;
        break;
      }
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
    case BLOCK_ACK:
      return BLOCK_ACK;
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
    case BLOCK_ACK:
      switch (m_actionValue)
        {
        case BLOCK_ACK_ADDBA_REQUEST:
          retval.blockAck = BLOCK_ACK_ADDBA_REQUEST;
          break ;
        case BLOCK_ACK_ADDBA_RESPONSE:
          retval.blockAck = BLOCK_ACK_ADDBA_RESPONSE;
          break ;
        case BLOCK_ACK_DELBA:
          retval.blockAck = BLOCK_ACK_DELBA;
          break ;
        }
      break ;
      
    case MESH_PEERING_MGT:
      switch (m_actionValue)
        {
        case PEER_LINK_OPEN:
          retval.peerLink = PEER_LINK_OPEN;
          break ;
        case PEER_LINK_CONFIRM:
          retval.peerLink = PEER_LINK_CONFIRM;
          break ;
        case PEER_LINK_CLOSE:
          retval.peerLink = PEER_LINK_CLOSE;
          break ;
        default:
          NS_FATAL_ERROR ("Unknown mesh peering management action code");
          retval.peerLink = PEER_LINK_OPEN; /* quiet compiler */
        }
      break ;
      
    case MESH_PATH_SELECTION:
      switch (m_actionValue)
        {
        case PATH_SELECTION:
          retval.pathSelection = PATH_SELECTION;
          break ;
        default:
          NS_FATAL_ERROR ("Unknown mesh path selection action code");
          retval.peerLink = PEER_LINK_OPEN; /* quiet compiler */
        }
      break ;
      
    case MESH_LINK_METRIC:
    // not yet supported
    case MESH_INTERWORKING:
    // not yet supported
    case MESH_RESOURCE_COORDINATION:
    // not yet supported
    default:
      NS_FATAL_ERROR ("Unsupported mesh action");
      retval.peerLink = PEER_LINK_OPEN; /* quiet compiler */
    }
  return retval;
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

/***************************************************
*                 ADDBARequest
****************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAddBaRequestHeader);

MgtAddBaRequestHeader::MgtAddBaRequestHeader ()
  : m_dialogToken (1),
    m_amsduSupport (1),
    m_bufferSize (0)
{
}

TypeId
MgtAddBaRequestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MgtAddBaRequestHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtAddBaRequestHeader> ();
  return tid;
}

TypeId
MgtAddBaRequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
MgtAddBaRequestHeader::Print (std::ostream &os) const
{
}

uint32_t
MgtAddBaRequestHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += 1; //Dialog token
  size += 2; //Block ack parameter set
  size += 2; //Block ack timeout value
  size += 2; //Starting sequence control
  return size;
}

void
MgtAddBaRequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_dialogToken);
  i.WriteHtolsbU16 (GetParameterSet ());
  i.WriteHtolsbU16 (m_timeoutValue);
  i.WriteHtolsbU16 (GetStartingSequenceControl ());
}

uint32_t
MgtAddBaRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_dialogToken = i.ReadU8 ();
  SetParameterSet (i.ReadLsbtohU16 ());
  m_timeoutValue = i.ReadLsbtohU16 ();
  SetStartingSequenceControl (i.ReadLsbtohU16 ());
  return i.GetDistanceFrom (start);
}

void
MgtAddBaRequestHeader::SetDelayedBlockAck ()
{
  m_policy = 0;
}

void
MgtAddBaRequestHeader::SetImmediateBlockAck ()
{
  m_policy = 1;
}

void
MgtAddBaRequestHeader::SetTid (uint8_t tid)
{
  NS_ASSERT (tid < 16);
  m_tid = tid;
}

void
MgtAddBaRequestHeader::SetTimeout (uint16_t timeout)
{
  m_timeoutValue = timeout;
}

void
MgtAddBaRequestHeader::SetBufferSize (uint16_t size)
{
  m_bufferSize = size;
}

void
MgtAddBaRequestHeader::SetStartingSequence (uint16_t seq)
{
  m_startingSeq = seq;
}

void
MgtAddBaRequestHeader::SetAmsduSupport (bool supported)
{
  m_amsduSupport = supported;
}

uint8_t
MgtAddBaRequestHeader::GetTid (void) const
{
  return m_tid;
}

bool
MgtAddBaRequestHeader::IsImmediateBlockAck (void) const
{
  return (m_policy == 1) ? true : false;
}

uint16_t
MgtAddBaRequestHeader::GetTimeout (void) const
{
  return m_timeoutValue;
}

uint16_t
MgtAddBaRequestHeader::GetBufferSize (void) const
{
  return m_bufferSize;
}

bool
MgtAddBaRequestHeader::IsAmsduSupported (void) const
{
  return (m_amsduSupport == 1) ? true : false;
}

uint16_t
MgtAddBaRequestHeader::GetStartingSequence (void) const
{
  return m_startingSeq;
}

uint16_t
MgtAddBaRequestHeader::GetStartingSequenceControl (void) const
{
  return (m_startingSeq << 4) & 0xfff0;
}

void
MgtAddBaRequestHeader::SetStartingSequenceControl (uint16_t seqControl)
{
  m_startingSeq = (seqControl >> 4) & 0x0fff;
}

uint16_t
MgtAddBaRequestHeader::GetParameterSet (void) const
{
  uint16_t res = 0;
  res |= m_amsduSupport;
  res |= m_policy << 1;
  res |= m_tid << 2;
  res |= m_bufferSize << 6;
  return res;
}

void
MgtAddBaRequestHeader::SetParameterSet (uint16_t params)
{
  m_amsduSupport = (params) & 0x01;
  m_policy = (params >> 1) & 0x01;
  m_tid = (params >> 2) & 0x0f;
  m_bufferSize = (params >> 6) & 0x03ff;
}

/***************************************************
*                 ADDBAResponse
****************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtAddBaResponseHeader);

MgtAddBaResponseHeader::MgtAddBaResponseHeader ()
  : m_dialogToken (1),
    m_amsduSupport (1),
    m_bufferSize (0)
{
}

TypeId
MgtAddBaResponseHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::MgtAddBaResponseHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtAddBaResponseHeader> ()
  ;
  return tid;
}

TypeId
MgtAddBaResponseHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
MgtAddBaResponseHeader::Print (std::ostream &os) const
{
  os << "status code=" << m_code;
}

uint32_t
MgtAddBaResponseHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += 1; //Dialog token
  size += m_code.GetSerializedSize (); //Status code
  size += 2; //Block ack parameter set
  size += 2; //Block ack timeout value
  return size;
}

void
MgtAddBaResponseHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_dialogToken);
  i = m_code.Serialize (i);
  i.WriteHtolsbU16 (GetParameterSet ());
  i.WriteHtolsbU16 (m_timeoutValue);
}

uint32_t
MgtAddBaResponseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_dialogToken = i.ReadU8 ();
  i = m_code.Deserialize (i);
  SetParameterSet (i.ReadLsbtohU16 ());
  m_timeoutValue = i.ReadLsbtohU16 ();
  return i.GetDistanceFrom (start);
}

void
MgtAddBaResponseHeader::SetDelayedBlockAck ()
{
  m_policy = 0;
}

void
MgtAddBaResponseHeader::SetImmediateBlockAck ()
{
  m_policy = 1;
}

void
MgtAddBaResponseHeader::SetTid (uint8_t tid)
{
  NS_ASSERT (tid < 16);
  m_tid = tid;
}

void
MgtAddBaResponseHeader::SetTimeout (uint16_t timeout)
{
  m_timeoutValue = timeout;
}

void
MgtAddBaResponseHeader::SetBufferSize (uint16_t size)
{
  m_bufferSize = size;
}

void
MgtAddBaResponseHeader::SetStatusCode (StatusCode code)
{
  m_code = code;
}

void
MgtAddBaResponseHeader::SetAmsduSupport (bool supported)
{
  m_amsduSupport = supported;
}

StatusCode
MgtAddBaResponseHeader::GetStatusCode (void) const
{
  return m_code;
}

uint8_t
MgtAddBaResponseHeader::GetTid (void) const
{
  return m_tid;
}

bool
MgtAddBaResponseHeader::IsImmediateBlockAck (void) const
{
  return (m_policy == 1) ? true : false;
}

uint16_t
MgtAddBaResponseHeader::GetTimeout (void) const
{
  return m_timeoutValue;
}

uint16_t
MgtAddBaResponseHeader::GetBufferSize (void) const
{
  return m_bufferSize;
}

bool
MgtAddBaResponseHeader::IsAmsduSupported (void) const
{
  return (m_amsduSupport == 1) ? true : false;
}

uint16_t
MgtAddBaResponseHeader::GetParameterSet (void) const
{
  uint16_t res = 0;
  res |= m_amsduSupport;
  res |= m_policy << 1;
  res |= m_tid << 2;
  res |= m_bufferSize << 6;
  return res;
}

void
MgtAddBaResponseHeader::SetParameterSet (uint16_t params)
{
  m_amsduSupport = (params) & 0x01;
  m_policy = (params >> 1) & 0x01;
  m_tid = (params >> 2) & 0x0f;
  m_bufferSize = (params >> 6) & 0x03ff;
}

/***************************************************
*                     DelBa
****************************************************/

NS_OBJECT_ENSURE_REGISTERED (MgtDelBaHeader);

MgtDelBaHeader::MgtDelBaHeader ()
  : m_reasonCode (1)
{
}

TypeId
MgtDelBaHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MgtDelBaHeader")
    .SetParent<Header> ()
    .AddConstructor<MgtDelBaHeader> ()
  ;
  return tid;
}

TypeId
MgtDelBaHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
MgtDelBaHeader::Print (std::ostream &os) const
{
}

uint32_t
MgtDelBaHeader::GetSerializedSize (void) const
{
  uint32_t size = 0;
  size += 2; //DelBa parameter set
  size += 2; //Reason code
  return size;
}

void
MgtDelBaHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtolsbU16 (GetParameterSet ());
  i.WriteHtolsbU16 (m_reasonCode);
}

uint32_t
MgtDelBaHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  SetParameterSet (i.ReadLsbtohU16 ());
  m_reasonCode = i.ReadLsbtohU16 ();
  return i.GetDistanceFrom (start);
}

bool
MgtDelBaHeader::IsByOriginator (void) const
{
  return (m_initiator == 1) ? true : false;
}

uint8_t
MgtDelBaHeader::GetTid (void) const
{
  NS_ASSERT (m_tid < 16);
  uint8_t tid = static_cast<uint8_t> (m_tid);
  return tid;
}

void
MgtDelBaHeader::SetByOriginator (void)
{
  m_initiator = 1;
}

void
MgtDelBaHeader::SetByRecipient (void)
{
  m_initiator = 0;
}

void
MgtDelBaHeader::SetTid (uint8_t tid)
{
  NS_ASSERT (tid < 16);
  m_tid = static_cast<uint16_t> (tid);
}

uint16_t
MgtDelBaHeader::GetParameterSet (void) const
{
  uint16_t res = 0;
  res |= m_initiator << 11;
  res |= m_tid << 12;
  return res;
}

void
MgtDelBaHeader::SetParameterSet (uint16_t params)
{
  m_initiator = (params >> 11) & 0x01;
  m_tid = (params >> 12) & 0x0f;
}

} // namespace ns3
