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
#include "ns3/node-list.h"
#include "ns3/node.h"

#include "lte-ue-rrc.h"
#include "lte-enb-rrc.h"
#include "lte-rlc.h"
#include "lte-pdcp.h"
#include "lte-pdcp-sap.h"
#include "lte-radio-bearer-info.h"
#include "lte-as-sap.h"
#include "lte-enb-net-device.h"

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





/////////////////////////////
// ue RRC methods
/////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (LteUeRrc);

LteUeRrc::LteUeRrc ()
  : m_cphySapProvider (0),
    m_cmacSapProvider (0),
    m_macSapProvider (0),
    m_asSapUser (0),
    m_cellId (0)
{
  NS_LOG_FUNCTION (this);
  m_cphySapUser = new MemberLteUeCphySapUser<LteUeRrc> (this);
  m_cmacSapUser = new UeMemberLteUeCmacSapUser (this);
  m_pdcpSapUser = new LtePdcpSpecificLtePdcpSapUser<LteUeRrc> (this);
  m_asSapProvider = new MemberLteAsSapProvider<LteUeRrc> (this);
}


LteUeRrc::~LteUeRrc ()
{
  NS_LOG_FUNCTION (this);
}

void
LteUeRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cphySapUser;
  delete m_cmacSapUser;
  delete m_pdcpSapUser;
  delete m_asSapProvider;
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
                   UintegerValue (0), // unused, read-only attribute
                   MakeUintegerAccessor (&LteUeRrc::GetCellId),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("C-RNTI",
                   "Cell Radio Network Temporary Identifier",
                   UintegerValue (0), // unused, read-only attribute
                   MakeUintegerAccessor (&LteUeRrc::GetRnti),
                   MakeUintegerChecker<uint16_t> ())
    ;
  return tid;
}


void
LteUeRrc::SetLteUeCphySapProvider (LteUeCphySapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_cphySapProvider = s;
}

LteUeCphySapUser*
LteUeRrc::GetLteUeCphySapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_cphySapUser;
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
LteUeRrc::SetAsSapUser (LteAsSapUser* s)
{
  m_asSapUser = s;
}

LteAsSapProvider* 
LteUeRrc::GetAsSapProvider ()
{
  return m_asSapProvider;
}

void 
LteUeRrc::SetImsi (uint64_t imsi)
{
  m_imsi = imsi;
}

uint64_t
LteUeRrc::GetImsi (void)
{
  return m_imsi;
}

void
LteUeRrc::SetupRadioBearer (EpsBearer bearer, TypeId rlcTypeId, uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcid);

  ObjectFactory rlcObjectFactory;
  rlcObjectFactory.SetTypeId (rlcTypeId);
  Ptr<LteRlc> rlc = rlcObjectFactory.Create ()->GetObject<LteRlc> ();
  rlc->SetLteMacSapProvider (m_macSapProvider);
  rlc->SetRnti (m_rnti);
  rlc->SetLcId (lcid);

  Ptr<LteRadioBearerInfo> rbInfo = CreateObject<LteRadioBearerInfo> ();
  rbInfo->m_rlc = rlc;

  // we need PDCP only for real RLC, i.e., RLC/UM or RLC/AM
  // if we are using RLC/SM we don't care of anything above RLC
  if (rlcTypeId != LteRlcSm::GetTypeId ())
    {
      Ptr<LtePdcp> pdcp = CreateObject<LtePdcp> ();
      pdcp->SetRnti (m_rnti);
      pdcp->SetLcId (lcid);
      pdcp->SetLtePdcpSapUser (m_pdcpSapUser);
      pdcp->SetLteRlcSapProvider (rlc->GetLteRlcSapProvider ());
      rlc->SetLteRlcSapUser (pdcp->GetLteRlcSapUser ());
      rbInfo->m_pdcp = pdcp;
    }
  
  NS_ASSERT_MSG (m_rbMap.find (lcid) == m_rbMap.end (), "bearer with same lcid already existing");
  m_rbMap.insert (std::pair<uint8_t, Ptr<LteRadioBearerInfo> > (lcid, rbInfo));
  
  m_cmacSapProvider->AddLc (lcid, rlc->GetLteMacSapUser ());
}

void
LteUeRrc::ReleaseRadioBearer (uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcid);
  std::map<uint8_t, Ptr<LteRadioBearerInfo> >::iterator it =   m_rbMap.find (lcid);
  NS_ASSERT_MSG (it != m_rbMap.end (), "could not find bearer with given lcid");
  m_rbMap.erase (it);
  NS_FATAL_ERROR ("need to remove entry from TFT classifier, but this is not implemented yet");
}


void
LteUeRrc::DoSendData (Ptr<Packet> packet, uint8_t bid)
{
  NS_LOG_FUNCTION (this << packet);

  // this is true until we implement Signaling Radio Bearers
  uint8_t lcid = bid;
  LtePdcpSapProvider::TransmitRrcPduParameters params;
  params.rrcPdu = packet;
  params.rnti = m_rnti;
  params.lcid = lcid;
  std::map<uint8_t, Ptr<LteRadioBearerInfo> >::iterator it =   m_rbMap.find (lcid);
  NS_ASSERT_MSG (it != m_rbMap.end (), "could not find bearer with lcid == " << lcid);
  
  NS_LOG_LOGIC (this << " RNTI=" << m_rnti << " sending " << packet << "on LCID " << (uint32_t) lcid << " (" << packet->GetSize () << " bytes)");
  it->second->m_pdcp->GetLtePdcpSapProvider ()->TransmitRrcPdu (params);  
}

void
LteUeRrc::DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params)
{
  NS_LOG_FUNCTION (this);
  m_asSapUser->RecvData (params.rrcPdu);
}




void
LteUeRrc::DoLcConfigCompleted ()
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("not implemented");
}


void 
LteUeRrc::DoForceCampedOnEnb (Ptr<LteEnbNetDevice> enbLteDevice, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << cellId);
  
  m_cphySapProvider->SetEarfcn (enbLteDevice->GetDlEarfcn (),
                               enbLteDevice->GetUlEarfcn ());
  
  m_cellId = cellId;
  m_cphySapProvider->SyncronizeWithEnb (enbLteDevice, cellId);

  // the bandwidth setting should be done upon receiving the MIB, but
  // MIB is not implemented for the time being. 
  m_cphySapProvider->SetBandwidth (enbLteDevice->GetUlBandwidth (),
                                   enbLteDevice->GetDlBandwidth ());

  // this is used for RRC interactions until the RRC protocol is implemented
  m_enbRrc = enbLteDevice->GetRrc ();
}

void 
LteUeRrc::DoConnect ()
{
  NS_LOG_FUNCTION (this);
  
  m_rnti = m_enbRrc->DoRecvConnectionRequest (m_imsi);
  m_cmacSapProvider->ConfigureUe (m_rnti);
  m_cphySapProvider->SetRnti (m_rnti);
}

uint16_t
LteUeRrc::GetRnti () const
{
  NS_LOG_FUNCTION (this);
  return m_rnti;
}

uint16_t
LteUeRrc::GetCellId () const
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
LteUeRrc::DoRecvConnectionSetup (LteUeConfig_t params)
{    
  m_cphySapProvider->SetTransmissionMode (params.m_transmissionMode);
  m_cphySapProvider->SetSrsConfigurationIndex (params.m_srsConfigurationIndex);


  m_enbRrc->DoRecvConnectionSetupCompleted (m_rnti);  
}

void
LteUeRrc::DoRecvConnectionReconfiguration (LteUeConfig_t params)
{
  NS_LOG_FUNCTION (this << " RNTI " << params.m_rnti << " txMode " << (uint16_t)params.m_transmissionMode);
  
  m_cphySapProvider->SetTransmissionMode (params.m_transmissionMode);
  m_cphySapProvider->SetSrsConfigurationIndex (params.m_srsConfigurationIndex);

}

void 
LteUeRrc::DoRecvConnectionReconfigurationWithMobilityControlInfo (uint16_t targetCellId, uint16_t newRnti)
{
  Ptr<LteEnbNetDevice> enbDev;
  // WILD HACK - eventually we'll get rid of all these Ptr<Device> around
  NodeList::Iterator listEnd = NodeList::End ();
  bool found = false;
  for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
    {
      Ptr<Node> node = *i;
      int nDevs = node->GetNDevices ();
      for (int j = 0; j < nDevs; j++)
        {
          enbDev = node->GetDevice (j)->GetObject <LteEnbNetDevice> ();
          if (!enbDev)
            {
              continue;
            }
          else
            {
              if (enbDev->GetCellId () == targetCellId)
                {
                  found = true;                  
                }
            }
        }
    }
  NS_ASSERT_MSG (found , " Unable to find eNB with CellId =" << targetCellId);
  m_enbRrc = enbDev->GetObject<LteEnbRrc> ();

  DoForceCampedOnEnb (enbDev, targetCellId);
  m_rnti = newRnti;
  m_cmacSapProvider->ConfigureUe (m_rnti);
  m_cphySapProvider->SetRnti (m_rnti);
  m_enbRrc->DoRecvConnectionReconfigurationCompleted (m_rnti);
  
}
  
} // namespace ns3

