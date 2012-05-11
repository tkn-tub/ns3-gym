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
#include "ns3/object-map.h"
#include "ns3/object-factory.h"

#include "lte-ue-rrc.h"
#include "lte-rlc.h"
#include "lte-pdcp.h"
#include "lte-pdcp-sap.h"
#include "lte-radio-bearer-info.h"


NS_LOG_COMPONENT_DEFINE ("LteUeRrc");

namespace ns3 {



/////////////////////////////
// CMAC SAP forwarder
/////////////////////////////

class UeMemberLteUeCmacSapUser : public LteUeCmacSapUser
{
public:
  UeMemberLteUeCmacSapUser (LteUeRrc* rrc);

  virtual void LcConfigCompleted ();

private:
  LteUeRrc* m_rrc;
};

UeMemberLteUeCmacSapUser::UeMemberLteUeCmacSapUser (LteUeRrc* rrc)
  : m_rrc (rrc)
{
}

void
UeMemberLteUeCmacSapUser::LcConfigCompleted ()
{
  m_rrc->DoLcConfigCompleted ();
}


////////////////////////////////
// PDCP SAP Forwarder
////////////////////////////////

// class UeRrcMemberLtePdcpSapUser : public LtePdcpSapUser
// {
// public:
//   MemberLtePdcpSapUser (LteUeRrc* rrc);
//   virtual void ReceiveRrcPdu (Ptr<Packet> p);
// private:
//   LteUeRrc* m_rrc;EnbRrc
// };


// UeRrcMemberLtePdcpSapUser::UeRrcMemberLtePdcpSapUser (LteUeRrc* rrc)
//   : m_rrc (rrc)
// {
// }

// void UeRrcMemberLtePdcpSapUser::ReceiveRrcPdu (Ptr<Packet> p)
// {
//   m_rrc->DoReceiveRrcPdu (p);
// }




/////////////////////////////
// ue RRC methods
/////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (LteUeRrc);

LteUeRrc::LteUeRrc ()
  : m_cmacSapProvider (0),
    m_macSapProvider (0),
    m_cellId (0)
{
  NS_LOG_FUNCTION (this);
  m_cmacSapUser = new UeMemberLteUeCmacSapUser (this);
  m_pdcpSapUser = new LtePdcpSpecificLtePdcpSapUser<LteUeRrc> (this);
}


LteUeRrc::~LteUeRrc ()
{
  NS_LOG_FUNCTION (this);
}

void
LteUeRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cmacSapUser;
  delete m_pdcpSapUser;
  m_rbMap.clear ();
}

TypeId
LteUeRrc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteUeRrc")
    .SetParent<Object> ()
    .AddConstructor<LteUeRrc> ()
    .AddAttribute ("RadioBearerMap", "List of UE RadioBearerInfo by LCID.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&LteUeRrc::m_rbMap),
                   MakeObjectMapChecker<LteRadioBearerInfo> ())
    .AddAttribute ("CellId",
                   "Serving cell identifier",
                   UintegerValue (1),
                   MakeUintegerAccessor (&LteUeRrc::m_cellId),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("C-RNTI",
                   "Cell Radio Network Temporary Identifier",
                   UintegerValue (1),
                   MakeUintegerAccessor (&LteUeRrc::m_rnti),
                   MakeUintegerChecker<uint16_t> ())
  ;
  return tid;
}

void
LteUeRrc::SetLteUeCmacSapProvider (LteUeCmacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_cmacSapProvider = s;
}

LteUeCmacSapUser*
LteUeRrc::GetLteUeCmacSapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_cmacSapUser;
}

void
LteUeRrc::SetLteMacSapProvider (LteMacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_macSapProvider = s;
}



void
LteUeRrc::ConfigureUe (uint16_t rnti, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  m_rnti = rnti;
  m_cellId = cellId;
  m_cmacSapProvider->ConfigureUe (rnti);
}

void
LteUeRrc::SetupRadioBearer (uint16_t rnti, EpsBearer bearer, TypeId rlcTypeId, uint8_t lcid, Ptr<EpcTft> tft)
{
  NS_LOG_FUNCTION (this << (uint32_t)  rnti << (uint32_t) lcid);

  ObjectFactory rlcObjectFactory;
  rlcObjectFactory.SetTypeId (rlcTypeId);
  Ptr<LteRlc> rlc = rlcObjectFactory.Create ()->GetObject<LteRlc> ();
  rlc->SetLteMacSapProvider (m_macSapProvider);
  rlc->SetRnti (rnti);
  rlc->SetLcId (lcid);

  Ptr<LteRadioBearerInfo> rbInfo = CreateObject<LteRadioBearerInfo> ();
  rbInfo->m_rlc = rlc;

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
  
  NS_ASSERT_MSG (m_rbMap.find (lcid) == m_rbMap.end (), "bearer with same lcid already existing");
  m_rbMap.insert (std::pair<uint8_t, Ptr<LteRadioBearerInfo> > (lcid, rbInfo));


  m_tftClassifier.Add (tft, lcid);
  
  m_cmacSapProvider->AddLc (lcid, rlc->GetLteMacSapUser ());
}

void
LteUeRrc::ReleaseRadioBearer (uint16_t rnti, uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t)  rnti << (uint32_t) lcid);
  std::map<uint8_t, Ptr<LteRadioBearerInfo> >::iterator it =   m_rbMap.find (lcid);
  NS_ASSERT_MSG (it != m_rbMap.end (), "could not find bearer with given lcid");
  m_rbMap.erase (it);
  NS_FATAL_ERROR ("need to remove entry from TFT classifier, but this is not implemented yet");
}


bool
LteUeRrc::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  uint8_t lcid = m_tftClassifier.Classify (packet, EpcTft::UPLINK);
  LtePdcpSapProvider::TransmitRrcPduParameters params;
  params.rrcPdu = packet;
  params.rnti = m_rnti;
  params.lcid = lcid;
  std::map<uint8_t, Ptr<LteRadioBearerInfo> >::iterator it =   m_rbMap.find (lcid);
  if (it == m_rbMap.end ())
    {
      NS_LOG_WARN ("could not find bearer with lcid == " << lcid);
      return false;
    }
  else
    {
      NS_LOG_LOGIC (this << " RNTI=" << m_rnti << " sending " << packet << "on LCID " << (uint32_t) lcid << " (" << packet->GetSize () << " bytes)");
      it->second->m_pdcp->GetLtePdcpSapProvider ()->TransmitRrcPdu (params);
      return true;
    }
}


void 
LteUeRrc::SetForwardUpCallback (Callback <void, Ptr<Packet> > cb)
{
  m_forwardUpCallback = cb;
}


void
LteUeRrc::DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params)
{
  NS_LOG_FUNCTION (this);
  m_forwardUpCallback (params.rrcPdu);
}




void
LteUeRrc::DoLcConfigCompleted ()
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("not implemented");
}

uint16_t
LteUeRrc::GetRnti ()
{
  NS_LOG_FUNCTION (this);
  return m_rnti;
}

uint16_t
LteUeRrc::GetCellId ()
{
  NS_LOG_FUNCTION (this);
  return m_cellId;
}

std::vector<uint8_t>
LteUeRrc::GetLcIdVector ()
{
  std::vector<uint8_t> v;
  for (std::map<uint8_t, Ptr<LteRadioBearerInfo> >::iterator it = m_rbMap.begin (); it != m_rbMap.end (); ++it)
    {
      v.push_back (it->first);
    }
  return v;
}

void
LteUeRrc::DoRrcConfigurationUpdateInd (LteUeConfig_t params)
{
  NS_LOG_FUNCTION (this << " RNTI " << params.m_rnti << " txMode " << (uint16_t)params.m_transmissionMode);
  
  // propagate the information to MAC layer
  m_cmacSapProvider->RrcUpdateConfigurationReq (params);
}


} // namespace ns3

