/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 *         Marco Miozzo <mmiozzo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/fatal-error.h"
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/pointer.h"
#include "ns3/object-map.h"
#include "ns3/object-factory.h"

#include "ns3/lte-enb-rrc.h"
#include "ns3/lte-enb-net-device.h"
#include "ns3/lte-rlc.h"
#include "ns3/lte-pdcp.h"
#include "ns3/lte-pdcp-sap.h"
#include "ns3/lte-radio-bearer-info.h"
#include "ns3/lte-radio-bearer-tag.h"
#include "ns3/ff-mac-csched-sap.h"

// WILD HACK for UE-RRC direct communications
#include <ns3/node-list.h>
#include <ns3/node.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-ue-rrc.h>


NS_LOG_COMPONENT_DEFINE ("LteEnbRrc");

namespace ns3 {


// ///////////////////////////
// CMAC SAP forwarder
// ///////////////////////////

class EnbRrcMemberLteEnbCmacSapUser : public LteEnbCmacSapUser
{
public:
  EnbRrcMemberLteEnbCmacSapUser (LteEnbRrc* rrc);

  virtual void NotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success);
  virtual void RrcConfigurationUpdateInd (LteUeConfig_t params);

private:
  LteEnbRrc* m_rrc;
};

EnbRrcMemberLteEnbCmacSapUser::EnbRrcMemberLteEnbCmacSapUser (LteEnbRrc* rrc)
  : m_rrc (rrc)
{
}

void
EnbRrcMemberLteEnbCmacSapUser::NotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success)
{
  m_rrc->DoNotifyLcConfigResult (rnti, lcid, success);
}

void
EnbRrcMemberLteEnbCmacSapUser::RrcConfigurationUpdateInd (LteUeConfig_t params)
{
  m_rrc->DoRrcConfigurationUpdateInd (params);
}


////////////////////////////////
// PDCP SAP Forwarder
////////////////////////////////

// not needed any more if the template works

// class EnbRrcMemberLtePdcpSapUser : public LtePdcpSapUser
// {
// public:
//   MemberLtePdcpSapUser (LteEnbRrc* rrc);
//   virtual void ReceiveRrcPdu (Ptr<Packet> p);
// private:
//   LteEnbRrc* m_rrc;
// };


// EnbRrcMemberLtePdcpSapUser::EnbRrcMemberLtePdcpSapUser (LteEnbRrc* rrc)
//   : m_rrc (rrc)
// {
// }

// void EnbRrcMemberLtePdcpSapUser::ReceiveRrcPdu (Ptr<Packet> p)
// {
//   m_rrc->DoReceiveRrcPdu (p);
// }




///////////////////////////////////////////
// UeInfo 
///////////////////////////////////////////


NS_OBJECT_ENSURE_REGISTERED (UeInfo);

UeInfo::UeInfo (void)
  : m_lastAllocatedId (0)
{
  m_imsi = 0;
}

UeInfo::UeInfo (uint64_t imsi)
  : m_lastAllocatedId (0)
{
  m_imsi = imsi;
}



UeInfo::~UeInfo (void)
{
  // Nothing to do here
}

TypeId UeInfo::GetTypeId (void)
{
  static TypeId  tid = TypeId ("ns3::UeInfo")
    .SetParent<Object> ()
    .AddConstructor<UeInfo> ()
    .AddAttribute ("RadioBearerMap", "List of UE RadioBearerInfo by LCID.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&UeInfo::m_rbMap),
                   MakeObjectMapChecker<LteRadioBearerInfo> ())
  ;
  return tid;
}

uint64_t
UeInfo::GetImsi (void)
{
  return m_imsi;
}

uint8_t
UeInfo::AddRadioBearer (Ptr<LteRadioBearerInfo> rbi)
{
  NS_LOG_FUNCTION (this);
  for (uint8_t lcid = m_lastAllocatedId; lcid != m_lastAllocatedId - 1; ++lcid)
    {
      if (lcid != 0)
        {
          if (m_rbMap.find (lcid) == m_rbMap.end ())
            {
              m_rbMap.insert (std::pair<uint8_t, Ptr<LteRadioBearerInfo> > (lcid, rbi));
              m_lastAllocatedId = lcid;
              return lcid;
            }
        }
    }
  NS_LOG_WARN ("no more logical channel ids available");
  return 0;
}

Ptr<LteRadioBearerInfo>
UeInfo::GetRadioBearer (uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcid);
  NS_ASSERT (0 != lcid);
  std::map<uint8_t, Ptr<LteRadioBearerInfo> >::iterator it = m_rbMap.find (lcid);  
  NS_ABORT_IF (it == m_rbMap.end ());
  return it->second;
}


void
UeInfo::RemoveRadioBearer (uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcid);
  std::map <uint8_t, Ptr<LteRadioBearerInfo> >::iterator it = m_rbMap.find (lcid);
  NS_ASSERT_MSG (it != m_rbMap.end (), "request to remove radio bearer with unknown lcid " << lcid);
  m_rbMap.erase (it);
}





// ///////////////////////////
// eNB RRC methods
// ///////////////////////////

NS_OBJECT_ENSURE_REGISTERED (LteEnbRrc);

LteEnbRrc::LteEnbRrc ()
  : m_x2SapProvider (0),
    m_cmacSapProvider (0),
    m_ffMacSchedSapProvider (0),
    m_macSapProvider (0),
    m_configured (false),
    m_lastAllocatedRnti (0)
{
  NS_LOG_FUNCTION (this);
  m_cmacSapUser = new EnbRrcMemberLteEnbCmacSapUser (this);
  m_pdcpSapUser = new LtePdcpSpecificLtePdcpSapUser<LteEnbRrc> (this);
  m_x2SapUser = new EpcX2SpecificEpcX2SapUser<LteEnbRrc> (this);
}


LteEnbRrc::~LteEnbRrc ()
{
  NS_LOG_FUNCTION (this);
}


void
LteEnbRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cmacSapUser;
  delete m_pdcpSapUser;
  delete m_x2SapUser;
}

TypeId
LteEnbRrc::GetTypeId (void)
{
  NS_LOG_FUNCTION ("LteEnbRrc::GetTypeId");
  static TypeId tid = TypeId ("ns3::LteEnbRrc")
    .SetParent<Object> ()
    .AddConstructor<LteEnbRrc> ()
    .AddAttribute ("UeMap", "List of UE Info by C-RNTI.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&LteEnbRrc::m_ueMap),
                   MakeObjectMapChecker<UeInfo> ())
    .AddAttribute ("DefaultTransmissionMode",
                  "The default UEs' transmission mode (0: SISO)",
                  UintegerValue (0),  // default tx-mode
                  MakeUintegerAccessor (&LteEnbRrc::m_defaultTransmissionMode),
                  MakeUintegerChecker<uint8_t> ())
             
  ;
  return tid;
}

uint16_t
LteEnbRrc::GetLastAllocatedRnti () const
{
  NS_LOG_FUNCTION (this);
  return m_lastAllocatedRnti;
}
std::map<uint16_t,Ptr<UeInfo> > LteEnbRrc::GetUeMap (void) const
{
  return m_ueMap;
}

void LteEnbRrc::SetUeMap (std::map<uint16_t,Ptr<UeInfo> > ueMap)
{
  this->m_ueMap = ueMap;
}


void
LteEnbRrc::SetLastAllocatedRnti (uint16_t lastAllocatedRnti)
{
  NS_LOG_FUNCTION (this << lastAllocatedRnti);
  m_lastAllocatedRnti = lastAllocatedRnti;
}



void
LteEnbRrc::SetEpcX2SapProvider (EpcX2SapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_x2SapProvider = s;
}

EpcX2SapUser*
LteEnbRrc::GetEpcX2SapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_x2SapUser;
}


void
LteEnbRrc::SetLteEnbCmacSapProvider (LteEnbCmacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_cmacSapProvider = s;
}

LteEnbCmacSapUser*
LteEnbRrc::GetLteEnbCmacSapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_cmacSapUser;
}

void
LteEnbRrc::SetFfMacSchedSapProvider (FfMacSchedSapProvider * s)
{
  NS_LOG_FUNCTION (this);
  m_ffMacSchedSapProvider = s;
}


void
LteEnbRrc::SetLteMacSapProvider (LteMacSapProvider * s)
{
  NS_LOG_FUNCTION (this);
  m_macSapProvider = s;
}

LtePdcpSapProvider* 
LteEnbRrc::GetLtePdcpSapProvider (uint16_t rnti, uint8_t lcid)
{
  return GetUeInfo (rnti)->GetRadioBearer (lcid)->m_pdcp->GetLtePdcpSapProvider ();
}

void
LteEnbRrc::ConfigureCell (uint8_t ulBandwidth, uint8_t dlBandwidth)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (!m_configured);
  m_cmacSapProvider->ConfigureMac (ulBandwidth, dlBandwidth);
  m_configured = true;
}

uint16_t
LteEnbRrc::AddUe (uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi);
  // no Call Admission Control for now
  uint16_t rnti = CreateUeInfo (imsi); // side effect: create UeInfo for this UE
  NS_ASSERT_MSG (rnti != 0, "CreateUeInfo returned RNTI==0");
  m_cmacSapProvider->AddUe (rnti);
  return rnti;
}

void
LteEnbRrc::RemoveUe (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  RemoveUeInfo (rnti);
  NS_FATAL_ERROR ("missing RemoveUe method in CMAC SAP");
}

uint8_t
LteEnbRrc::SetupRadioBearer (uint16_t rnti, EpsBearer bearer, TypeId rlcTypeId)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  Ptr<UeInfo> ueInfo = GetUeInfo (rnti);

  // create RLC instance

  ObjectFactory rlcObjectFactory;
  rlcObjectFactory.SetTypeId (rlcTypeId);
  Ptr<LteRlc> rlc = rlcObjectFactory.Create ()->GetObject<LteRlc> ();
  rlc->SetLteMacSapProvider (m_macSapProvider);
  rlc->SetRnti (rnti);

  Ptr<LteRadioBearerInfo> rbInfo = CreateObject<LteRadioBearerInfo> ();
  rbInfo->m_rlc = rlc;
  uint8_t lcid = ueInfo->AddRadioBearer (rbInfo);
  rlc->SetLcId (lcid);

  // we need PDCP only for real RLC, i.e., RLC/UM or RLC/AM
  // if we are using RLC/SM we don't care of anything above RLC
  if (rlcTypeId != LteRlcSm::GetTypeId ())
    {
      Ptr<LtePdcp> pdcp = CreateObject<LtePdcp> ();
      pdcp->SetRnti (rnti);
      pdcp->SetLcId (lcid);
      pdcp->SetLtePdcpSapUser (m_pdcpSapUser);
      pdcp->SetLteRlcSapProvider (rlc->GetLteRlcSapProvider ());
      rlc->SetLteRlcSapUser (pdcp->GetLteRlcSapUser ());
      rbInfo->m_pdcp = pdcp;
    }
    
  LteEnbCmacSapProvider::LcInfo lcinfo;
  lcinfo.rnti = rnti;
  lcinfo.lcId = lcid;
  lcinfo.lcGroup = 0; // TBD
  lcinfo.qci = bearer.qci;
  lcinfo.isGbr = bearer.IsGbr ();
  lcinfo.mbrUl = bearer.gbrQosInfo.mbrUl;
  lcinfo.mbrDl = bearer.gbrQosInfo.mbrDl;
  lcinfo.gbrUl = bearer.gbrQosInfo.gbrUl;
  lcinfo.gbrDl = bearer.gbrQosInfo.gbrDl;
  m_cmacSapProvider->AddLc (lcinfo, rlc->GetLteMacSapUser ());
  
  // Transmission mode settings
  LteUeConfig_t ueConfig;
  ueConfig.m_rnti = rnti;
  ueConfig.m_transmissionMode = m_defaultTransmissionMode;
  DoRrcConfigurationUpdateInd (ueConfig);

  return lcid;
}

void
LteEnbRrc::ReleaseRadioBearer (uint16_t rnti, uint8_t lcId)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  Ptr<UeInfo> ueInfo = GetUeInfo (rnti);
  ueInfo->RemoveRadioBearer (lcId);
}



bool
LteEnbRrc::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);

  LteRadioBearerTag tag;
  bool found = packet->RemovePacketTag (tag);
  NS_ASSERT (found);
  
  LtePdcpSapProvider::TransmitRrcPduParameters params;
  params.rrcPdu = packet;
  params.rnti = tag.GetRnti ();
  params.lcid = tag.GetLcid ();
  LtePdcpSapProvider* pdcpSapProvider = GetLtePdcpSapProvider (tag.GetRnti (), tag.GetLcid ());
  pdcpSapProvider->TransmitRrcPdu (params);
  
  return true;
}

void 
LteEnbRrc::SetForwardUpCallback (Callback <void, Ptr<Packet> > cb)
{
  m_forwardUpCallback = cb;
}


//
// User API
//
void
LteEnbRrc::SendHandoverRequest (Ptr<Node> ueNode, Ptr<Node> sourceEnbNode, Ptr<Node> targetEnbNode)
{
  NS_LOG_FUNCTION (this << ueNode << sourceEnbNode << targetEnbNode);
  NS_LOG_LOGIC ("Request to send HANDOVER REQUEST");
  
  Ptr<LteUeRrc> ueRrc = ueNode->GetDevice (0)->GetObject<LteUeNetDevice> ()->GetRrc ();
  uint16_t rnti = ueRrc->GetRnti ();

  EpcX2SapProvider::HandoverRequestParams params;
  params.oldEnbUeX2apId = rnti;
  params.cause          = EpcX2SapProvider::HandoverDesirableForRadioReason;
  params.sourceCellId   = sourceEnbNode->GetDevice (0)->GetObject<LteEnbNetDevice> ()->GetCellId ();
  params.targetCellId   = targetEnbNode->GetDevice (0)->GetObject<LteEnbNetDevice> ()->GetCellId ();
  params.ueAggregateMaxBitRateDownlink = 200 * 1000;
  params.ueAggregateMaxBitRateUplink = 100 * 1000;
  
  std::string rrcData ("abcdefghijklmnopqrstuvwxyz");
  params.rrcContext = Create<Packet> ((uint8_t const *) rrcData.data (), rrcData.length ());

  NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
  NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);
  NS_LOG_LOGIC ("rrcContext   = " << params.rrcContext << " : " << rrcData);

  m_x2SapProvider->SendHandoverRequest (params);
}


//
// X2 User SAP
//
void
LteEnbRrc::DoRecvHandoverRequest (EpcX2SapUser::HandoverRequestParams params)
{
  NS_LOG_FUNCTION (this);

  NS_LOG_LOGIC ("Recv X2 message: HANDOVER REQUEST");

  NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
  NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);
  
  uint8_t rrcData [100];
  params.rrcContext->CopyData (rrcData, params.rrcContext->GetSize ());
  NS_LOG_LOGIC ("rrcContext   = " << rrcData);

  NS_LOG_LOGIC ("Send X2 message: HANDOVER REQUEST ACK");

  EpcX2SapProvider::HandoverRequestAckParams ackParams;
  ackParams.oldEnbUeX2apId = params.oldEnbUeX2apId;
  ackParams.newEnbUeX2apId = params.oldEnbUeX2apId + 100;
  ackParams.sourceCellId = params.sourceCellId;
  ackParams.targetCellId = params.targetCellId;

  NS_LOG_LOGIC ("oldEnbUeX2apId = " << ackParams.oldEnbUeX2apId);
  NS_LOG_LOGIC ("newEnbUeX2apId = " << ackParams.newEnbUeX2apId);
  NS_LOG_LOGIC ("sourceCellId = " << ackParams.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << ackParams.targetCellId);

  m_x2SapProvider->SendHandoverRequestAck (ackParams);
}

void
LteEnbRrc::DoRecvHandoverRequestAck (EpcX2SapUser::HandoverRequestAckParams params)
{
  NS_LOG_FUNCTION (this);
  
  NS_LOG_LOGIC ("Recv X2 message: HANDOVER REQUEST ACK");
  
  NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
  NS_LOG_LOGIC ("newEnbUeX2apId = " << params.newEnbUeX2apId);
  NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);
}


void
LteEnbRrc::DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params)
{
  NS_LOG_FUNCTION (this);
  // this tag is needed by the EpcEnbApplication to determine the S1 bearer that corresponds to this radio bearer
  LteRadioBearerTag tag;
  tag.SetRnti (params.rnti);
  tag.SetLcid (params.lcid);
  params.rrcPdu->AddPacketTag (tag);
  m_forwardUpCallback (params.rrcPdu);
}



void
LteEnbRrc::DoNotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  NS_FATAL_ERROR ("not implemented");
}



// /////////////////////////////////////////
// management of multiple UE info instances
// /////////////////////////////////////////


uint16_t
LteEnbRrc::CreateUeInfo (uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi);
  for (uint16_t rnti = m_lastAllocatedRnti; rnti != m_lastAllocatedRnti - 1; ++rnti)
    {
      if (rnti != 0)
        {
          if (m_ueMap.find (rnti) == m_ueMap.end ())
            {
              m_lastAllocatedRnti = rnti;
              m_ueMap.insert (std::pair<uint16_t, Ptr<UeInfo> > (rnti, CreateObject<UeInfo> (imsi)));
              return rnti;
            }
        }
    }
  return 0;
}

Ptr<UeInfo>
LteEnbRrc::GetUeInfo (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  NS_ASSERT (0 != rnti);
  std::map<uint16_t, Ptr<UeInfo> >::iterator it = m_ueMap.find (rnti);  
  NS_ABORT_IF (it == m_ueMap.end ());
  return it->second;
}

void
LteEnbRrc::RemoveUeInfo (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  std::map <uint16_t, Ptr<UeInfo> >::iterator it = m_ueMap.find (rnti);
  NS_ASSERT_MSG (it != m_ueMap.end (), "request to remove UE info with unknown rnti " << rnti);
  m_ueMap.erase (it);
}


void
LteEnbRrc::DoRrcConfigurationUpdateInd (LteUeConfig_t params)
{
  NS_LOG_FUNCTION (this);
  // up tp now only for TxMode change
  // update the peer UE-RRC on the change
  NodeList::Iterator listEnd = NodeList::End ();
  bool done = false;
  for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
    {
      Ptr<Node> node = *i;
      int nDevs = node->GetNDevices ();
      for (int j = 0; j < nDevs; j++)
        {
          Ptr<LteUeNetDevice> uedev = node->GetDevice (j)->GetObject <LteUeNetDevice> ();
          if (!uedev)
            {
              continue;
            }
          else
            {
              Ptr<LteUeRrc> ueRrc = uedev->GetRrc ();
              if (ueRrc->GetRnti () == params.m_rnti)
                {
                  ueRrc->DoRrcConfigurationUpdateInd (params);
                  done = true;
                }
              else
                {
                  continue;
                }
            }
        }
    }
  NS_ASSERT_MSG (done , " Unable to find peer UE-RRC, RNTI " << params.m_rnti);
  // answer to MAC (and scheduler)
  FfMacCschedSapProvider::CschedUeConfigReqParameters req;
  req.m_rnti = params.m_rnti;
  req.m_transmissionMode = params.m_transmissionMode;
  m_cmacSapProvider->RrcUpdateConfigurationReq (req);
  
}




} // namespace ns3

