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
 * Modified by: Marco Miozzo <mmiozzo@cttc.es> 
 *                add transmission Mode and SRS related functionalities
 */

#include <ns3/fatal-error.h>
#include <ns3/log.h>
#include <ns3/abort.h>
#include "ns3/pointer.h"
#include "ns3/object-map.h"
#include "ns3/object-factory.h"

#include "lte-enb-rrc.h"
#include "lte-rlc.h"
#include "lte-pdcp.h"
#include "lte-pdcp-sap.h"
#include "lte-radio-bearer-info.h"
#include "lte-radio-bearer-tag.h"
#include "ns3/object-map.h"
#include <ns3/ff-mac-csched-sap.h>

#include <ns3/simulator.h>

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
  m_srsConfigurationIndex = 0;
  m_transmissionMode = 0;
}

UeInfo::UeInfo (uint64_t imsi)
  : m_lastAllocatedId (0)
{
  m_imsi = imsi;
  m_srsConfigurationIndex = 0;
  m_transmissionMode = 0;
}

UeInfo::UeInfo (uint64_t imsi, uint16_t srsConfIndex)
: m_lastAllocatedId (0)
{
  m_imsi = imsi;
  m_srsConfigurationIndex = srsConfIndex;
  m_transmissionMode = 0;
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

uint16_t
UeInfo::GetSrsConfigurationIndex (void)
{
  return m_srsConfigurationIndex;
}

uint8_t
UeInfo::GetTransmissionMode (void)
{
  return m_transmissionMode;
}

void
UeInfo::SetSrsConfigurationIndex (uint16_t srsConfIndex)
{
  m_srsConfigurationIndex = srsConfIndex;
}

void
UeInfo::SetTransmissionMode (uint8_t txMode)
{
  m_transmissionMode = txMode;
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
  : m_cmacSapProvider (0),
    m_ffMacSchedSapProvider (0),
    m_macSapProvider (0),
    m_configured (false),
    m_lastAllocatedRnti (0),
    m_srsCurrentPeriodicityId (0),
    m_lastAllocatedConfigurationIndex (0),
    m_reconfigureUes (false)
{
  NS_LOG_FUNCTION (this);
  m_cmacSapUser = new EnbRrcMemberLteEnbCmacSapUser (this);
  m_pdcpSapUser = new LtePdcpSpecificLtePdcpSapUser<LteEnbRrc> (this);
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

// from 3GPP TS 36.213 table 8.2-1 UE Specific SRS Periodicity
#define SRS_ENTRIES 9
uint16_t g_srsPeriodicity[SRS_ENTRIES] = {0, 2, 5, 10, 20, 40, 80, 160, 320};
uint16_t g_srsCiLow[SRS_ENTRIES] = {0, 0, 2, 7, 17, 37, 77, 157, 317};
uint16_t g_srsCiHigh[SRS_ENTRIES] = {0, 1, 6, 16, 36, 76, 156, 316, 636};

void
LteEnbRrc::SetCellId (uint16_t cellId)
{
  m_cellId = cellId;
}

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
              Ptr<UeInfo> ueInfo = CreateObject<UeInfo> (imsi, GetNewSrsConfigurationIndex ());
              m_ueMap.insert (std::pair<uint16_t, Ptr<UeInfo> > (rnti, ueInfo));
              NS_LOG_DEBUG (this << " New UE RNTI " << rnti << " cellId " << m_cellId << " srs CI " << ueInfo->GetSrsConfigurationIndex ());
              return rnti;
            }
        }
    }
    
  return 0;
}

uint16_t
LteEnbRrc::GetNewSrsConfigurationIndex ()
{
  NS_LOG_FUNCTION (this << m_ueSrsConfigurationIndexSet.size ());
  // SRS
  if (m_srsCurrentPeriodicityId==0)
    {
      // no UEs -> init
      m_ueSrsConfigurationIndexSet.insert (0);
      m_lastAllocatedConfigurationIndex = 0;
      m_srsCurrentPeriodicityId++;
      
      return 0;
    }
  NS_ASSERT (m_srsCurrentPeriodicityId < SRS_ENTRIES);
  NS_LOG_DEBUG (this << " SRS p " << g_srsPeriodicity[m_srsCurrentPeriodicityId] << " set " << m_ueSrsConfigurationIndexSet.size ());
  if (m_ueSrsConfigurationIndexSet.size () == g_srsPeriodicity[m_srsCurrentPeriodicityId])
    {
//       NS_LOG_DEBUG (this << " SRS reconfigure CIs " << g_srsPeriodicity[m_srsCurrentPeriodicityId] << " to " << g_srsPeriodicity[m_srsCurrentPeriodicityId+1] << " at " << Simulator::Now ());
      // increase the current periocity for having enough CIs
      m_ueSrsConfigurationIndexSet.clear ();
      m_srsCurrentPeriodicityId++;
      NS_ASSERT (m_srsCurrentPeriodicityId < SRS_ENTRIES);
      // update all the UE's CI
      uint16_t srcCi = g_srsCiLow[m_srsCurrentPeriodicityId];
      std::map<uint16_t, Ptr<UeInfo> >::iterator it;
      for (it = m_ueMap.begin (); it != m_ueMap.end (); it++)
        {
          (*it).second->SetSrsConfigurationIndex (srcCi);
          m_ueSrsConfigurationIndexSet.insert (srcCi);
          m_lastAllocatedConfigurationIndex = srcCi;
          srcCi++;
          // send update to peer RRC
          LteUeConfig_t ueConfig;
          ueConfig.m_rnti = (*it).first;
          ueConfig.m_transmissionMode = (*it).second->GetTransmissionMode ();
          ueConfig.m_srsConfigurationIndex = (*it).second->GetSrsConfigurationIndex ();
          ueConfig.m_reconfigureFlag = false;
          NS_LOG_DEBUG (this << "\t rnti "<<ueConfig.m_rnti<< " CI " << ueConfig.m_srsConfigurationIndex);
          SendUeConfigurationUpdate (ueConfig);
        }
      m_ueSrsConfigurationIndexSet.insert (m_lastAllocatedConfigurationIndex + 1);
      m_lastAllocatedConfigurationIndex++;
    }
  else
    {
      // find a CI from the available ones
      std::set<uint16_t>::reverse_iterator rit = m_ueSrsConfigurationIndexSet.rbegin ();
      NS_LOG_DEBUG (this << " lower bound " << (*rit) << " of " << g_srsCiHigh[m_srsCurrentPeriodicityId]);
      if ((*rit) <= g_srsCiHigh[m_srsCurrentPeriodicityId])
        {
          // got it from the upper bound
          m_lastAllocatedConfigurationIndex = (*rit) + 1;
          m_ueSrsConfigurationIndexSet.insert (m_lastAllocatedConfigurationIndex);
        }
      else
        {
          // look for released ones
          for (uint16_t srcCi = g_srsCiLow[m_srsCurrentPeriodicityId]; srcCi < g_srsCiHigh[m_srsCurrentPeriodicityId]; srcCi++) 
            {
              std::set<uint16_t>::iterator it = m_ueSrsConfigurationIndexSet.find (srcCi);
              if (it==m_ueSrsConfigurationIndexSet.end ())
                {
                  m_lastAllocatedConfigurationIndex = srcCi;
                  m_ueSrsConfigurationIndexSet.insert (srcCi);
                  break;
                }
            }
        } 
    }
  return m_lastAllocatedConfigurationIndex;
  
}


void
LteEnbRrc::RemoveSrsConfigurationIndex (uint16_t srcCi)
{
  NS_LOG_FUNCTION (this << srcCi);
  std::set<uint16_t>::iterator it = m_ueSrsConfigurationIndexSet.find (srcCi);
  NS_ASSERT_MSG (it != m_ueSrsConfigurationIndexSet.end (), "request to remove unkwown SRS CI " << srcCi);
  m_ueSrsConfigurationIndexSet.erase (it);
  NS_ASSERT (m_srsCurrentPeriodicityId > 1);
  if (m_ueSrsConfigurationIndexSet.size () < g_srsPeriodicity[m_srsCurrentPeriodicityId - 1])
    {
      // reduce the periodicity
      m_ueSrsConfigurationIndexSet.clear ();
      m_srsCurrentPeriodicityId--;
      if (m_srsCurrentPeriodicityId==0)
        {
          // no active users : renitialize structures
          m_lastAllocatedConfigurationIndex = 0;
        }
      else
        {
          // update all the UE's CI
          uint16_t srcCi = g_srsCiLow[m_srsCurrentPeriodicityId];
          std::map<uint16_t, Ptr<UeInfo> >::iterator it;
          for (it = m_ueMap.begin (); it != m_ueMap.end (); it++)
            {
              (*it).second->SetSrsConfigurationIndex (srcCi);
              m_ueSrsConfigurationIndexSet.insert (srcCi);
              m_lastAllocatedConfigurationIndex = srcCi;
              srcCi++;
              // send update to peer RRC
              LteUeConfig_t ueConfig;
              ueConfig.m_rnti = (*it).first;
              ueConfig.m_transmissionMode = (*it).second->GetTransmissionMode ();
              ueConfig.m_srsConfigurationIndex = (*it).second->GetSrsConfigurationIndex ();
              ueConfig.m_reconfigureFlag = false;
              NS_LOG_DEBUG (this << "\t rnti "<<ueConfig.m_rnti<< " CI " << ueConfig.m_srsConfigurationIndex);
              if (Simulator::Now ()!=Seconds(0))
                {
                  // avoid multiple reconfiguration during initialization
                  SendUeConfigurationUpdate (ueConfig);
                }
            }
        }
    }
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
  RemoveSrsConfigurationIndex ((*it).second->GetSrsConfigurationIndex ());
  m_ueMap.erase (it);
  // remove SRS configuration index
}


void
LteEnbRrc::DoRrcConfigurationUpdateInd (LteUeConfig_t params)
{
  NS_LOG_FUNCTION (this);
  // at this stage used only by the scheduler for updating txMode
  // retrieve UE info
  std::map<uint16_t, Ptr<UeInfo> >::iterator it;
  it = m_ueMap.find (params.m_rnti);
  NS_ASSERT_MSG (it!=m_ueMap.end (), "Unable to find UeInfo");
  params.m_srsConfigurationIndex = (*it).second->GetSrsConfigurationIndex ();
  // update Tx Mode info
  (*it).second->SetTransmissionMode (params.m_transmissionMode);
  params.m_reconfigureFlag = true;
  // answer to MAC (and scheduler) and forward info to UEs
  SendUeConfigurationUpdate (params);  
}

void
LteEnbRrc::ConfigureNewUe (uint16_t rnti)
{
  NS_LOG_FUNCTION (this);
  // retrieve UE info
  std::map<uint16_t, Ptr<UeInfo> >::iterator it;
  it = m_ueMap.find (rnti);
  NS_ASSERT_MSG (it!=m_ueMap.end (), "Unable to find UeInfo");
  LteUeConfig_t params;
  params.m_rnti = rnti;
  params.m_srsConfigurationIndex = (*it).second->GetSrsConfigurationIndex ();
  params.m_transmissionMode = (*it).second->GetTransmissionMode ();
  params.m_reconfigureFlag = false;
  // answer to MAC (and scheduler) and forward info to UEs
  SendUeConfigurationUpdate (params);  
}

void
LteEnbRrc::SendUeConfigurationUpdate (LteUeConfig_t params)
{
  NS_LOG_FUNCTION (this);
  // send LteUeConfig_t to MAC layer and peer RRCs
  m_cmacSapProvider->UeUpdateConfigurationReq (params);
  NodeList::Iterator listEnd = NodeList::End ();
  bool done = false;
  params.m_reconfigureFlag = false;
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
              if ((ueRrc->GetRnti () == params.m_rnti)&&(ueRrc->GetCellId () == m_cellId))
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
}




} // namespace ns3

