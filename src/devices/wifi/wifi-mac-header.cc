/* -*-  Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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
#include "ns3/assert.h"
#include "ns3/address-utils.h"
#include "wifi-mac-header.h"

#define MAC80211HEADER_DEBUG 1

#ifdef MAC80211HEADER_DEBUG
#include <iostream>
#  define TRACE(x) \
std::Cout << "MAC80211HEADER " << x << std::Endl;
#else
#  define TRACE(x)
#endif


namespace ns3 {

enum {
  TYPE_MGT = 0,
  TYPE_CTL  = 1,
  TYPE_DATA = 2
};

enum {
  SUBTYPE_CTL_BACKREQ = 8,
  SUBTYPE_CTL_BACKRESP = 9,
  SUBTYPE_CTL_RTS = 11,
  SUBTYPE_CTL_CTS = 12,
  SUBTYPE_CTL_ACK = 13
};

WifiMacHeader::WifiMacHeader ()
{}
WifiMacHeader::~WifiMacHeader ()
{}

void 
WifiMacHeader::SetDsFrom (void)
{
  m_ctrlFromDs = 1;
}
void 
WifiMacHeader::SetDsNotFrom (void)
{
  m_ctrlFromDs = 0;
}
void 
WifiMacHeader::SetDsTo (void)
{
  m_ctrlToDs = 1;
}
void 
WifiMacHeader::SetDsNotTo (void)
{
  m_ctrlToDs = 0;
}

void 
WifiMacHeader::SetAddr1 (Mac48Address address)
{
  m_addr1 = address;
}
void 
WifiMacHeader::SetAddr2 (Mac48Address address)
{
  m_addr2 = address;
}
void 
WifiMacHeader::SetAddr3 (Mac48Address address)
{
  m_addr3 = address;
}
void 
WifiMacHeader::SetAddr4 (Mac48Address address)
{
  m_addr4 = address;
}
void 
WifiMacHeader::SetAssocReq (void)
{
  m_ctrlType = TYPE_MGT;
  m_ctrlSubtype = 0;
}
void 
WifiMacHeader::SetAssocResp (void)
{
  m_ctrlType = TYPE_MGT;
  m_ctrlSubtype = 1;
}
void 
WifiMacHeader::SetProbeReq (void)
{
  m_ctrlType = TYPE_MGT;
  m_ctrlSubtype = 4;
}
void 
WifiMacHeader::SetProbeResp (void)
{
  m_ctrlType = TYPE_MGT;
  m_ctrlSubtype = 5;
}
void 
WifiMacHeader::SetTypeData (void)
{
  m_ctrlType = TYPE_DATA;
  m_ctrlSubtype = 0;
}
void 
WifiMacHeader::SetType (enum WifiMacType_e type)
{
  switch (type) {
  case WIFI_MAC_CTL_BACKREQ:
    m_ctrlType = TYPE_CTL;
    m_ctrlSubtype = SUBTYPE_CTL_BACKREQ;
    break;
  case WIFI_MAC_CTL_BACKRESP:
    m_ctrlType = TYPE_CTL;
    m_ctrlSubtype = SUBTYPE_CTL_BACKRESP;
    break;
  case WIFI_MAC_CTL_RTS:
    m_ctrlType = TYPE_CTL;
    m_ctrlSubtype = SUBTYPE_CTL_RTS;
    break;
  case WIFI_MAC_CTL_CTS:
    m_ctrlType = TYPE_CTL;
    m_ctrlSubtype = SUBTYPE_CTL_CTS;
    break;
  case WIFI_MAC_CTL_ACK:
    m_ctrlType = TYPE_CTL;
    m_ctrlSubtype = SUBTYPE_CTL_ACK;
    break;
  case WIFI_MAC_MGT_ASSOCIATION_REQUEST:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 0;
    break;
  case WIFI_MAC_MGT_ASSOCIATION_RESPONSE:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 1;
    break;
  case WIFI_MAC_MGT_REASSOCIATION_REQUEST:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 2;
    break;
  case WIFI_MAC_MGT_REASSOCIATION_RESPONSE:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 3;
    break;
  case WIFI_MAC_MGT_PROBE_REQUEST:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 4;
    break;
  case WIFI_MAC_MGT_PROBE_RESPONSE:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 5;
    break;
  case WIFI_MAC_MGT_BEACON:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 8;
    break;
  case WIFI_MAC_MGT_DISASSOCIATION:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 10;
    break;
  case WIFI_MAC_MGT_AUTHENTICATION:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 11;
    break;
  case WIFI_MAC_MGT_DEAUTHENTICATION:
    m_ctrlType = TYPE_MGT;
    m_ctrlSubtype = 12;
    break;

  case WIFI_MAC_DATA:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 0;
    break;
  case WIFI_MAC_DATA_CFACK:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 1;
    break;
  case WIFI_MAC_DATA_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 2;
    break;
  case WIFI_MAC_DATA_CFACK_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 3;
    break;
  case WIFI_MAC_DATA_NULL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 4;
    break;
  case WIFI_MAC_DATA_NULL_CFACK:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 5;
    break;
  case WIFI_MAC_DATA_NULL_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 6;
    break;
  case WIFI_MAC_DATA_NULL_CFACK_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 7;
    break;
  case WIFI_MAC_QOSDATA:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 8;
    break;
  case WIFI_MAC_QOSDATA_CFACK:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 9;
    break;
  case WIFI_MAC_QOSDATA_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 10;
    break;
  case WIFI_MAC_QOSDATA_CFACK_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 11;
    break;
  case WIFI_MAC_QOSDATA_NULL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 12;
    break;
  case WIFI_MAC_QOSDATA_NULL_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 14;
    break;
  case WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL:
    m_ctrlType = TYPE_DATA;
    m_ctrlSubtype = 15;
    break;
  }
  m_ctrlToDs = 0;
  m_ctrlFromDs = 0;
}
void 
WifiMacHeader::SetDuration (uint16_t duration)
{
  m_duration = duration;
}
void 
WifiMacHeader::SetDurationUs (uint64_t duration_us)
{
  NS_ASSERT (duration_us <= (1<<16) - 1);
  m_duration = static_cast<uint16_t> (duration_us);
}

void WifiMacHeader::SetId (uint16_t id)
{
  m_duration = id;
}
void WifiMacHeader::SetSequenceNumber (uint16_t seq)
{
  m_seqSeq = seq;
}
void WifiMacHeader::SetFragmentNumber (uint8_t frag)
{
  m_seqFrag = frag;
}
void WifiMacHeader::SetNoMoreFragments (void)
{
  m_ctrlMoreFrag = 0;
}
void WifiMacHeader::SetMoreFragments (void)
{
  m_ctrlMoreFrag = 1;
}
void WifiMacHeader::SetRetry (void)
{
  m_ctrlRetry = 1;
}
void WifiMacHeader::SetNoRetry (void)
{
  m_ctrlRetry = 0;
}
void WifiMacHeader::SetQosTid (uint8_t tid)
{
  m_qosTid = tid;
}
void WifiMacHeader::SetQosTxopLimit (uint8_t txop)
{
  m_qosStuff = txop;
}

Mac48Address 
WifiMacHeader::GetAddr1 (void) const
{
  return m_addr1;
}
Mac48Address 
WifiMacHeader::GetAddr2 (void) const
{
  return m_addr2;
}
Mac48Address 
WifiMacHeader::GetAddr3 (void) const
{
  return m_addr3;
}
Mac48Address 
WifiMacHeader::GetAddr4 (void) const
{
  return m_addr4;
}
enum WifiMacType_e 
WifiMacHeader::GetType (void) const
{
  switch (m_ctrlType) {
  case TYPE_MGT:
    switch (m_ctrlSubtype) {
    case 0:
      return WIFI_MAC_MGT_ASSOCIATION_REQUEST;
      break;
    case 1:
      return WIFI_MAC_MGT_ASSOCIATION_RESPONSE;
      break;
    case 2:
      return WIFI_MAC_MGT_REASSOCIATION_REQUEST;
      break;
    case 3:
      return WIFI_MAC_MGT_REASSOCIATION_RESPONSE;
      break;
    case 4:
      return WIFI_MAC_MGT_PROBE_REQUEST;
      break;
    case 5:
      return WIFI_MAC_MGT_PROBE_RESPONSE;
      break;
    case 8:
      return WIFI_MAC_MGT_BEACON;
      break;
    case 10:
      return WIFI_MAC_MGT_DISASSOCIATION;
      break;
    case 11:
      return WIFI_MAC_MGT_AUTHENTICATION;
      break;
    case 12:
      return WIFI_MAC_MGT_DEAUTHENTICATION;
      break;

    }
    break;
  case TYPE_CTL:
    switch (m_ctrlSubtype) {
    case SUBTYPE_CTL_BACKREQ:
      return WIFI_MAC_CTL_BACKREQ;
      break;
    case SUBTYPE_CTL_BACKRESP:
      return WIFI_MAC_CTL_BACKRESP;
      break;
    case SUBTYPE_CTL_RTS:
      return WIFI_MAC_CTL_RTS;
      break;
    case SUBTYPE_CTL_CTS:
      return WIFI_MAC_CTL_CTS;
      break;
    case SUBTYPE_CTL_ACK:
      return WIFI_MAC_CTL_ACK;
      break;
    }
    break;
  case TYPE_DATA:
    switch (m_ctrlSubtype) {
    case 0:
      return WIFI_MAC_DATA;
      break;
    case 1:
      return WIFI_MAC_DATA_CFACK;
      break;
    case 2:
      return WIFI_MAC_DATA_CFPOLL;
      break;
    case 3:
      return WIFI_MAC_DATA_CFACK_CFPOLL;
      break;
    case 4:
      return WIFI_MAC_DATA_NULL;
      break;
    case 5:
      return WIFI_MAC_DATA_NULL_CFACK;
      break;
    case 6:
      return WIFI_MAC_DATA_NULL_CFPOLL;
      break;
    case 7:
      return WIFI_MAC_DATA_NULL_CFACK_CFPOLL;
      break;
    case 8:
      return WIFI_MAC_QOSDATA;
      break;
    case 9:
      return WIFI_MAC_QOSDATA_CFACK;
      break;
    case 10:
      return WIFI_MAC_QOSDATA_CFPOLL;
      break;
    case 11:
      return WIFI_MAC_QOSDATA_CFACK_CFPOLL;
      break;
    case 12:
      return WIFI_MAC_QOSDATA_NULL;
      break;
    case 14:
      return WIFI_MAC_QOSDATA_NULL_CFPOLL;
      break;
    case 15:
      return WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL;
      break;

    }
    break;
  }
  // NOTREACHED
  NS_ASSERT (false);
  return (enum WifiMacType_e)-1;
}
bool 
WifiMacHeader::IsFromDs (void) const
{
  return m_ctrlFromDs == 1;
}
bool 
WifiMacHeader::IsToDs (void) const
{
  return m_ctrlToDs == 1;
}

bool 
WifiMacHeader::IsData (void) const
{
  return (m_ctrlType == TYPE_DATA)?true:false;
    
}
bool 
WifiMacHeader::IsQosData (void) const
{
  return (m_ctrlType == TYPE_DATA && (m_ctrlSubtype & 0x08))?true:false;
}
bool 
WifiMacHeader::IsCtl (void) const
{
  return (m_ctrlType == TYPE_CTL)?true:false;
}
bool 
WifiMacHeader::IsMgt (void) const
{
  return (m_ctrlType == TYPE_MGT)?true:false;
}
bool 
WifiMacHeader::IsCfpoll (void) const
{
  switch (GetType ()) {
  case WIFI_MAC_DATA_CFPOLL:
  case WIFI_MAC_DATA_CFACK_CFPOLL:
  case WIFI_MAC_DATA_NULL_CFPOLL:
  case WIFI_MAC_DATA_NULL_CFACK_CFPOLL:
  case WIFI_MAC_QOSDATA_CFPOLL:
  case WIFI_MAC_QOSDATA_CFACK_CFPOLL:
  case WIFI_MAC_QOSDATA_NULL_CFPOLL:
  case WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL:
    return true;
    break;
  default:
    return false;
    break;
  }
}
bool 
WifiMacHeader::IsRts (void) const
{
  return (GetType () == WIFI_MAC_CTL_RTS)?true:false;
}
bool 
WifiMacHeader::IsCts (void) const
{
  return (GetType () == WIFI_MAC_CTL_CTS)?true:false;
}
bool 
WifiMacHeader::IsAck (void) const
{
  return (GetType () == WIFI_MAC_CTL_ACK)?true:false;
}
bool 
WifiMacHeader::IsAssocReq (void) const
{
  return (GetType () == WIFI_MAC_MGT_ASSOCIATION_REQUEST)?true:false;
}
bool 
WifiMacHeader::IsAssocResp (void) const
{
  return (GetType () == WIFI_MAC_MGT_ASSOCIATION_RESPONSE)?true:false;
}
bool 
WifiMacHeader::IsReassocReq (void) const
{
  return (GetType () == WIFI_MAC_MGT_REASSOCIATION_REQUEST)?true:false;
}
bool 
WifiMacHeader::IsReassocResp (void) const
{
  return (GetType () == WIFI_MAC_MGT_REASSOCIATION_RESPONSE)?true:false;
}
bool 
WifiMacHeader::IsProbeReq (void) const
{
  return (GetType () == WIFI_MAC_MGT_PROBE_REQUEST)?true:false;
}
bool 
WifiMacHeader::IsProbeResp (void) const
{
  return (GetType () == WIFI_MAC_MGT_PROBE_RESPONSE)?true:false;
}
bool 
WifiMacHeader::IsBeacon (void) const
{
  return (GetType () == WIFI_MAC_MGT_BEACON)?true:false;
}
bool 
WifiMacHeader::IsDisassociation (void) const
{
  return (GetType () == WIFI_MAC_MGT_DISASSOCIATION)?true:false;
}
bool 
WifiMacHeader::IsAuthentication (void) const
{
  return (GetType () == WIFI_MAC_MGT_AUTHENTICATION)?true:false;
}
bool 
WifiMacHeader::IsDeauthentication (void) const
{
  return (GetType () == WIFI_MAC_MGT_DEAUTHENTICATION)?true:false;
}


uint16_t 
WifiMacHeader::GetDuration (void) const
{
  return m_duration;
}
uint64_t 
WifiMacHeader::GetDurationUs (void) const
{
  return m_duration;
}
uint16_t 
WifiMacHeader::GetSequenceControl (void) const
{
  return (m_seqSeq << 4) | m_seqFrag;
}
uint16_t
WifiMacHeader::GetSequenceNumber (void) const
{
  return m_seqSeq;
}
uint16_t
WifiMacHeader::GetFragmentNumber (void) const
{
  return m_seqFrag;
}
bool 
WifiMacHeader::IsRetry (void) const
{
  return (m_ctrlRetry == 1)?true:false;
}
bool 
WifiMacHeader::IsMoreFragments (void) const
{
  return (m_ctrlMoreFrag == 1)?true:false;
}
bool 
WifiMacHeader::IsQosBlockAck (void) const
{
  NS_ASSERT (IsQosData ());
  return (m_qosAckPolicy == 3)?true:false;
}
bool 
WifiMacHeader::IsQosNoAck (void) const
{
  NS_ASSERT (IsQosData ());
  return (m_qosAckPolicy == 1)?true:false;
}
bool 
WifiMacHeader::IsQosAck (void) const
{
  NS_ASSERT (IsQosData ());
  return (m_qosAckPolicy == 0)?true:false;
}
uint8_t
WifiMacHeader::GetQosTid (void) const
{
  NS_ASSERT (IsQosData ());
  return m_qosTid;
}
uint8_t 
WifiMacHeader::GetQosTxopLimit (void) const
{
  NS_ASSERT (IsQosData ());
  return m_qosStuff;
}

uint16_t
WifiMacHeader::GetFrameControl (void) const
{
  uint16_t val = 0;
  val |= (m_ctrlType << 2) & (0x3<<2);
  val |= (m_ctrlSubtype << 4) & (0xf<<4);
  val |= (m_ctrlToDs << 8) & (0x1<<8);
  val |= (m_ctrlFromDs << 9) & (0x1<<9);
  val |= (m_ctrlMoreFrag << 10) & (0x1<<10);
  val |= (m_ctrlRetry << 11) & (0x1<<11);
  val |= (m_ctrlMoreData << 13) & (0x1<<13);
  val |= (m_ctrlWep << 14) & (0x1<<14);
  val |= (m_ctrlOrder << 15) & (0x1<<15);
  return val;
}

uint16_t
WifiMacHeader::GetQosControl (void) const
{
  uint16_t val = 0;
  val |= m_qosTid;
  val |= m_qosEosp << 4;
  val |= m_qosAckPolicy << 5;
  val |= m_qosStuff << 8;
  return val;
}

void
WifiMacHeader::SetFrameControl (uint16_t ctrl)
{
  m_ctrlType = (ctrl >> 2) & 0x03;
  m_ctrlSubtype = (ctrl >> 4) & 0x0f;
  m_ctrlToDs = (ctrl >> 8) & 0x01;
  m_ctrlFromDs = (ctrl >> 9) & 0x01;
  m_ctrlMoreFrag = (ctrl >> 10) & 0x01;
  m_ctrlRetry = (ctrl >> 11) & 0x01;
  m_ctrlMoreData = (ctrl >> 13) & 0x01;
  m_ctrlWep = (ctrl >> 14) & 0x01;
  m_ctrlOrder = (ctrl >> 15) & 0x01;
}
void 
WifiMacHeader::SetSequenceControl (uint16_t seq)
{
  m_seqFrag = seq & 0x0f;
  m_seqSeq = (seq >> 4) & 0x0fff;
}
void
WifiMacHeader::SetQosControl (uint16_t qos)
{
  m_qosTid = qos & 0x000f;
  m_qosEosp = (qos >> 4) & 0x0001;
  m_qosAckPolicy = (qos >> 5) & 0x0003;
  m_qosStuff = (qos >> 8) & 0x00ff;
}


void 
WifiMacHeader::SetDurationS (double duration)
{
  uint16_t us = (uint16_t)(duration * 1000000);
  us &= 0x7fff;
  SetDuration (us);
}

uint32_t 
WifiMacHeader::GetSize (void) const
{
  uint32_t size = 0;
  switch (m_ctrlType) {
  case TYPE_MGT:
    size = 2+2+6+6+6+2;
    break;
  case TYPE_CTL:
    switch (m_ctrlSubtype) {
    case SUBTYPE_CTL_RTS:
      size = 2+2+6+6;
      break;
    case SUBTYPE_CTL_CTS:
    case SUBTYPE_CTL_ACK:
      size = 2+2+6;
      break;
    case SUBTYPE_CTL_BACKREQ:
    case SUBTYPE_CTL_BACKRESP:
      // NOT IMPLEMENTED
      NS_ASSERT (false);
      break;
    }
    break;
  case TYPE_DATA:
    size = 2+2+6+6+6+2;
    if (m_ctrlToDs && m_ctrlFromDs) {
      size += 6;
    }
    if (m_ctrlSubtype & 0x08) {
      size += 2;
    }
    break;
  }
  return size;
}
char const *
WifiMacHeader::GetTypeString (void) const
{
#define FOO(x) \
case WIFI_MAC_ ## x: \
  return #x; \
  break;

  switch (GetType ()) {
    FOO (CTL_RTS);
    FOO (CTL_CTS);
    FOO (CTL_ACK);
    FOO (CTL_BACKREQ);
    FOO (CTL_BACKRESP);

    FOO (MGT_BEACON);
    FOO (MGT_ASSOCIATION_REQUEST);
    FOO (MGT_ASSOCIATION_RESPONSE);
    FOO (MGT_DISASSOCIATION);
    FOO (MGT_REASSOCIATION_REQUEST);
    FOO (MGT_REASSOCIATION_RESPONSE);
    FOO (MGT_PROBE_REQUEST);
    FOO (MGT_PROBE_RESPONSE);
    FOO (MGT_AUTHENTICATION);
    FOO (MGT_DEAUTHENTICATION);
    
    FOO (DATA);
    FOO (DATA_CFACK);
    FOO (DATA_CFPOLL);
    FOO (DATA_CFACK_CFPOLL);
    FOO (DATA_NULL);
    FOO (DATA_NULL_CFACK);
    FOO (DATA_NULL_CFPOLL);
    FOO (DATA_NULL_CFACK_CFPOLL);
    FOO (QOSDATA);
    FOO (QOSDATA_CFACK);
    FOO (QOSDATA_CFPOLL);
    FOO (QOSDATA_CFACK_CFPOLL);
    FOO (QOSDATA_NULL);
    FOO (QOSDATA_NULL_CFPOLL);
    FOO (QOSDATA_NULL_CFACK_CFPOLL);
  default:
    return "ERROR";
  }
#undef FOO
}

uint32_t
WifiMacHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<WifiMacHeader> ("WifiMacHeader.ns3.inria.fr");
  return uid;
}

std::string
WifiMacHeader::GetName (void) const
{
  return "802.11";
}

void 
WifiMacHeader::PrintFrameControl (std::ostream &os) const
{
  os << "("
     << "ToDS=" << m_ctrlToDs << ", FromDS=" << m_ctrlFromDs 
     << ", MoreFrag=" << m_ctrlMoreFrag << ", Retry=" << m_ctrlRetry 
     << ", MoreData=" << m_ctrlMoreData 
     << ")";
}

void 
WifiMacHeader::Print (std::ostream &os) const
{
  os << GetTypeString () << " ";
  switch (GetType ())
    {
    case WIFI_MAC_CTL_RTS:
      os << "Duration/ID=" << m_duration << "us"
         << ", RA=" << m_addr1 << ", TA=" << m_addr2;
      break;
    case WIFI_MAC_CTL_CTS:
    case WIFI_MAC_CTL_ACK:
      os << "Duration/ID=" << m_duration << "us"
         << ", RA=" << m_addr1;
      break;
    case WIFI_MAC_CTL_BACKREQ:
      break;
    case WIFI_MAC_CTL_BACKRESP:
      break;

    case WIFI_MAC_MGT_BEACON:
    case WIFI_MAC_MGT_ASSOCIATION_REQUEST:
    case WIFI_MAC_MGT_ASSOCIATION_RESPONSE:
    case WIFI_MAC_MGT_DISASSOCIATION:
    case WIFI_MAC_MGT_REASSOCIATION_REQUEST:
    case WIFI_MAC_MGT_REASSOCIATION_RESPONSE:
    case WIFI_MAC_MGT_PROBE_REQUEST:
    case WIFI_MAC_MGT_PROBE_RESPONSE:
    case WIFI_MAC_MGT_AUTHENTICATION:
    case WIFI_MAC_MGT_DEAUTHENTICATION:
      PrintFrameControl (os);
      os << " Duration/ID=" << m_duration << "us"
         << ", DA=" << m_addr1 << ", SA=" << m_addr2
         << ", BSSID=" << m_addr3 << ", FragNumber=" << m_seqFrag
         << ", SeqNumber=" << m_seqSeq;
      break;
    case WIFI_MAC_DATA:
      PrintFrameControl (os);
      os << " Duration/ID=" << m_duration << "us";
      if (!m_ctrlToDs && !m_ctrlFromDs)
        {
          os << "DA=" << m_addr1 << ", SA=" << m_addr2 << ", BSSID=" << m_addr3;
        }
      else if (!m_ctrlToDs && m_ctrlFromDs)
        {
          os << "DA=" << m_addr1 << ", SA=" << m_addr3 << ", BSSID=" << m_addr2;
        }
      else if (m_ctrlToDs && !m_ctrlFromDs)
        {
          os << "DA=" << m_addr3 << ", SA=" << m_addr2 << ", BSSID=" << m_addr1;
        }
      else if (m_ctrlToDs && m_ctrlFromDs)
        {
          os << "DA=" << m_addr3 << ", SA=" << m_addr4 << ", RA=" << m_addr1 << ", TA=" << m_addr2;
        }
      else
        {
          NS_ASSERT (false);
        }
      os << ", FragNumber=" << m_seqFrag
         << ", SeqNumber=" << m_seqSeq;
      break;
    case WIFI_MAC_DATA_CFACK:
    case WIFI_MAC_DATA_CFPOLL:
    case WIFI_MAC_DATA_CFACK_CFPOLL:
    case WIFI_MAC_DATA_NULL:
    case WIFI_MAC_DATA_NULL_CFACK:
    case WIFI_MAC_DATA_NULL_CFPOLL:
    case WIFI_MAC_DATA_NULL_CFACK_CFPOLL:
    case WIFI_MAC_QOSDATA:
    case WIFI_MAC_QOSDATA_CFACK:
    case WIFI_MAC_QOSDATA_CFPOLL:
    case WIFI_MAC_QOSDATA_CFACK_CFPOLL:
    case WIFI_MAC_QOSDATA_NULL:
    case WIFI_MAC_QOSDATA_NULL_CFPOLL:
    case WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL:
      break;
    }
}
uint32_t 
WifiMacHeader::GetSerializedSize (void) const
{
  return GetSize ();
}
void 
WifiMacHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteHtonU16 (GetFrameControl ());
  i.WriteHtonU16 (m_duration);
  WriteTo (i, m_addr1);
  switch (m_ctrlType) {
  case TYPE_MGT:
    WriteTo (i, m_addr2);
    WriteTo (i, m_addr3);
    i.WriteHtonU16 (GetSequenceControl ());
    break;
  case TYPE_CTL:
    switch (m_ctrlSubtype) {
    case SUBTYPE_CTL_RTS:
      WriteTo (i, m_addr2);
      break;
    case SUBTYPE_CTL_CTS:
    case SUBTYPE_CTL_ACK:
      break;
    case SUBTYPE_CTL_BACKREQ:
    case SUBTYPE_CTL_BACKRESP:
      // NOT IMPLEMENTED
      NS_ASSERT (false);
      break;
    default:
      //NOTREACHED
      NS_ASSERT (false);
      break;
    }
    break;
  case TYPE_DATA: {
    WriteTo (i, m_addr2);
    WriteTo (i, m_addr3);
    i.WriteHtonU16 (GetSequenceControl ());
    if (m_ctrlToDs && m_ctrlFromDs) {
      WriteTo (i, m_addr4);
    }
    if (m_ctrlSubtype & 0x08) {
      i.WriteHtonU16 (GetQosControl ());
    }
    } break;
  default:
    //NOTREACHED
    NS_ASSERT (false);
    break;
  }
}
uint32_t
WifiMacHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint16_t frame_control = i.ReadNtohU16 ();
  SetFrameControl (frame_control);
  m_duration = i.ReadNtohU16 ();
  ReadFrom (i, m_addr1);
  switch (m_ctrlType) {
  case TYPE_MGT:
    ReadFrom (i, m_addr2);
    ReadFrom (i, m_addr3);
    SetSequenceControl (i.ReadNtohU16 ());
    break;
  case TYPE_CTL:
    switch (m_ctrlSubtype) {
    case SUBTYPE_CTL_RTS:
      ReadFrom (i, m_addr2);
      break;
    case SUBTYPE_CTL_CTS:
    case SUBTYPE_CTL_ACK:
      break;
    case SUBTYPE_CTL_BACKREQ:
    case SUBTYPE_CTL_BACKRESP:
      // NOT IMPLEMENTED
      NS_ASSERT (false);
      break;
    }
    break;
  case TYPE_DATA:
    ReadFrom (i, m_addr2);
    ReadFrom (i, m_addr3);
    SetSequenceControl (i.ReadNtohU16 ());
    if (m_ctrlToDs && m_ctrlFromDs) {
      ReadFrom (i, m_addr4);
    }
    if (m_ctrlSubtype & 0x08) {
      SetQosControl (i.ReadNtohU16 ());
    }
    break;
  }
  return i.GetDistanceFrom (start);
}

}; // namespace ns3
