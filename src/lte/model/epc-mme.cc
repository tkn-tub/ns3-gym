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
 */

#include <ns3/fatal-error.h>
#include <ns3/log.h>

#include "epc-s1ap-sap.h"
#include "epc-s11-sap.h"

#include "epc-mme.h"

NS_LOG_COMPONENT_DEFINE ("EpcMme");

namespace ns3 {




NS_OBJECT_ENSURE_REGISTERED (EpcMme);

EpcMme::EpcMme ()
  : m_s11SapSgw (0)
{
  NS_LOG_FUNCTION (this);
  m_s1apSapMme = new MemberEpcS1apSapMme<EpcMme> (this);
  m_s11SapMme = new MemberEpcS11SapMme<EpcMme> (this);
}


EpcMme::~EpcMme ()
{
  NS_LOG_FUNCTION (this);
}

void
EpcMme::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_s1apSapMme;
  delete m_s11SapMme;
}

TypeId
EpcMme::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcMme")
    .SetParent<Object> ()
    .AddConstructor<EpcMme> ()
    ;
  return tid;
}

EpcS1apSapMme* 
EpcMme::GetS1apSapMme ()
{
  return m_s1apSapMme;
}

void 
EpcMme::SetS11SapSgw (EpcS11SapSgw * s)
{
  m_s11SapSgw = s;
}

EpcS11SapMme* 
EpcMme::GetS11SapMme ()
{
  return m_s11SapMme;
}

void 
EpcMme::AddEnb (uint16_t gci, Ipv4Address enbS1uAddr, EpcS1apSapEnb* enbS1apSap)
{
  NS_LOG_FUNCTION (this << gci << enbS1uAddr);
  Ptr<EnbInfo> enbInfo = Create<EnbInfo> ();
  enbInfo->gci = gci;
  enbInfo->s1uAddr = enbS1uAddr;
  enbInfo->s1apSapEnb = enbS1apSap;
  m_enbInfoMap[gci] = enbInfo;
}

void 
EpcMme::AddUe (uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi);
  Ptr<UeInfo> ueInfo = Create<UeInfo> ();
  ueInfo->imsi = imsi;
  ueInfo->mmeUeS1Id = imsi;
  m_ueInfoMap[imsi] = ueInfo;
  ueInfo->bearerCounter = 0;
}

void 
EpcMme::AddBearer (uint64_t imsi, Ptr<EpcTft> tft, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this << imsi);
  std::map<uint64_t, Ptr<UeInfo> >::iterator it = m_ueInfoMap.find (imsi);
  NS_ASSERT_MSG (it != m_ueInfoMap.end (), "could not find any UE with IMSI " << imsi);
  NS_ASSERT_MSG (it->second->bearerCounter < 11, "too many bearers already! " << it->second->bearerCounter);
  BearerInfo bearerInfo;
  bearerInfo.bearerId = ++(it->second->bearerCounter);
  bearerInfo.tft = tft;
  bearerInfo.bearer = bearer;  
  it->second->bearersToBeActivated.push_back (bearerInfo);
}


// S1-AP SAP MME forwarded methods

void 
EpcMme::DoInitialUeMessage (uint64_t mmeUeS1Id, uint16_t enbUeS1Id, uint64_t imsi, uint16_t gci)
{
  NS_LOG_FUNCTION (this << mmeUeS1Id << enbUeS1Id << imsi << gci);
  std::map<uint64_t, Ptr<UeInfo> >::iterator it = m_ueInfoMap.find (imsi);
  NS_ASSERT_MSG (it != m_ueInfoMap.end (), "could not find any UE with IMSI " << imsi);
  it->second->cellId = gci;
  std::list<EpcS11SapSgw::BearerContext> bearersToBeSetup;
  for (std::list<BearerInfo>::iterator bit = it->second->bearersToBeActivated.begin ();
       bit != it->second->bearersToBeActivated.end ();
       ++bit)
    {
      EpcS11SapSgw::BearerContext bearerContext;
      bearerContext.epsBearerId =  bit->bearerId; 
      bearerContext.bearerLevelQos = bit->bearer; 
      bearerContext.tft = bit->tft;
      bearersToBeSetup.push_back (bearerContext);
    }
  EpcS11Sap::Uli uli;
  uli.gci = gci;
  m_s11SapSgw->RecvCreateSessionRequest (imsi, uli, bearersToBeSetup);
}

void 
EpcMme::DoInitialContextSetupResponse (uint64_t mmeUeS1Id, uint16_t enbUeS1Id, std::list<EpcS1apSapMme::ErabSetupItem> erabSetupList)
{
  NS_LOG_FUNCTION (this << mmeUeS1Id << enbUeS1Id);
  NS_FATAL_ERROR ("unimplemented");
}

void 
EpcMme::DoPathSwitchRequest (uint64_t enbUeS1Id, uint64_t mmeUeS1Id, uint16_t gci, std::list<EpcS1apSapMme::ErabSwitchedInDownlinkItem> erabToBeSwitchedInDownlinkList)
{
  NS_LOG_FUNCTION (this << mmeUeS1Id << enbUeS1Id << gci);
  NS_FATAL_ERROR ("unimplemented");
}


// S11 SAP MME forwarded methods

void 
EpcMme::DoRecvCreateSessionResponse (uint64_t imsi, std::list<EpcS11SapMme::BearerContext> bearerContextList)
{
  NS_LOG_FUNCTION (this << imsi);
  std::list<EpcS1apSapEnb::ErabToBeSetupItem> erabToBeSetupList;
  for (std::list<EpcS11SapMme::BearerContext>::iterator bit = bearerContextList.begin ();
       bit != bearerContextList.end ();
       ++bit)
    {
      EpcS1apSapEnb::ErabToBeSetupItem erab;
      erab.erabId = bit->epsBearerId;
      erab.erabLevelQosParameters = bit->bearerLevelQos;
      erab.transportLayerAddress = bit->sgwFteid.address;
      erab.sgwTeid = bit->sgwFteid.teid;      
      erabToBeSetupList.push_back (erab);
    }
  std::map<uint64_t, Ptr<UeInfo> >::iterator it = m_ueInfoMap.find (imsi);
  NS_ASSERT_MSG (it != m_ueInfoMap.end (), "could not find any UE with IMSI " << imsi);
  uint16_t cellId = it->second->cellId;
  uint16_t enbUeS1Id = it->second->enbUeS1Id;
  uint64_t mmeUeS1Id = it->second->mmeUeS1Id;
  std::map<uint16_t, Ptr<EnbInfo> >::iterator jt = m_enbInfoMap.find (cellId);
  NS_ASSERT_MSG (jt != m_enbInfoMap.end (), "could not find any eNB with CellId " << cellId);
  jt->second->s1apSapEnb->InitialContextSetupRequest (mmeUeS1Id, enbUeS1Id, erabToBeSetupList);
}




} // namespace ns3
