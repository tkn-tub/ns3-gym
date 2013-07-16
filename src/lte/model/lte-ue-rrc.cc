/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
#include <ns3/object-map.h>
#include <ns3/object-factory.h>
#include <ns3/node-list.h>
#include <ns3/node.h>
#include <ns3/simulator.h>

#include "lte-ue-rrc.h"
#include "lte-enb-rrc.h"
#include "lte-rlc.h"
#include "lte-rlc-tm.h"
#include "lte-rlc-um.h"
#include "lte-rlc-am.h"
#include "lte-pdcp.h"
#include "lte-pdcp-sap.h"
#include "lte-radio-bearer-info.h"
#include "lte-as-sap.h"
#include "lte-enb-net-device.h"

#include <cmath>

NS_LOG_COMPONENT_DEFINE ("LteUeRrc");

namespace ns3 {



/////////////////////////////
// CMAC SAP forwarder
/////////////////////////////

class UeMemberLteUeCmacSapUser : public LteUeCmacSapUser
{
public:
  UeMemberLteUeCmacSapUser (LteUeRrc* rrc);

  virtual void SetTemporaryCellRnti (uint16_t rnti);
  virtual void NotifyRandomAccessSuccessful ();
  virtual void NotifyRandomAccessFailed ();
  
private:
  LteUeRrc* m_rrc;
};

UeMemberLteUeCmacSapUser::UeMemberLteUeCmacSapUser (LteUeRrc* rrc)
  : m_rrc (rrc)
{
}

void
UeMemberLteUeCmacSapUser::SetTemporaryCellRnti (uint16_t rnti)
{
  m_rrc->DoSetTemporaryCellRnti (rnti);
}


void
UeMemberLteUeCmacSapUser::NotifyRandomAccessSuccessful ()
{
  m_rrc->DoNotifyRandomAccessSuccessful ();
}

void
UeMemberLteUeCmacSapUser::NotifyRandomAccessFailed ()
{
  m_rrc->DoNotifyRandomAccessFailed ();
}







const char* g_ueRrcStateName[LteUeRrc::NUM_STATES] = 
  {
    "IDLE_CELL_SELECTION",
    "IDLE_WAIT_SYSTEM_INFO",
    "IDLE_CAMPED_NORMALLY",
    "IDLE_RANDOM_ACCESS",
    "IDLE_CONNECTING",
    "CONNECTED_NORMALLY",
    "CONNECTED_REESTABLISHING",
    "CONNECTED_HANDOVER"
  };

std::string ToString (LteUeRrc::State s)
{
  return std::string (g_ueRrcStateName[s]);
}


/////////////////////////////
// ue RRC methods
/////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (LteUeRrc);


LteUeRrc::LteUeRrc ()
  : m_cphySapProvider (0),
    m_cmacSapProvider (0),
    m_rrcSapUser (0),
    m_macSapProvider (0),
    m_asSapUser (0),
    m_state (IDLE_CELL_SELECTION),
    m_imsi (0),
    m_rnti (0),
    m_cellId (0),
    m_useRlcSm (true),
    m_connectionPending (0),
    m_receivedMib (0),
    m_receivedSib2 (0)

{
  NS_LOG_FUNCTION (this);
  m_cphySapUser = new MemberLteUeCphySapUser<LteUeRrc> (this);
  m_cmacSapUser = new UeMemberLteUeCmacSapUser (this);
  m_rrcSapProvider = new MemberLteUeRrcSapProvider<LteUeRrc> (this);
  m_drbPdcpSapUser = new LtePdcpSpecificLtePdcpSapUser<LteUeRrc> (this);
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
  delete m_rrcSapProvider;
  delete m_drbPdcpSapUser;
  delete m_asSapProvider;
  m_drbMap.clear ();
}

TypeId
LteUeRrc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteUeRrc")
    .SetParent<Object> ()
    .AddConstructor<LteUeRrc> ()
    .AddAttribute ("DataRadioBearerMap", "List of UE RadioBearerInfo for Data Radio Bearers by LCID.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&LteUeRrc::m_drbMap),
                   MakeObjectMapChecker<LteDataRadioBearerInfo> ())
    .AddAttribute ("Srb0", "SignalingRadioBearerInfo for SRB0",
                   PointerValue (),
                   MakePointerAccessor (&LteUeRrc::m_srb0),
                   MakePointerChecker<LteSignalingRadioBearerInfo> ())
    .AddAttribute ("Srb1", "SignalingRadioBearerInfo for SRB1",
                   PointerValue (),
                   MakePointerAccessor (&LteUeRrc::m_srb1),
                   MakePointerChecker<LteSignalingRadioBearerInfo> ())
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
    .AddTraceSource ("StateTransition",
                     "trace fired upon every UE RRC state transition",
                     MakeTraceSourceAccessor (&LteUeRrc::m_stateTransitionTrace))
    .AddTraceSource ("RandomAccessSuccessful",
                     "trace fired upon successful completion of the random access procedure",
                     MakeTraceSourceAccessor (&LteUeRrc::m_randomAccessSuccessfulTrace))
    .AddTraceSource ("ConnectionEstablished",
                     "trace fired upon successful RRC connection establishment",
                     MakeTraceSourceAccessor (&LteUeRrc::m_connectionEstablishedTrace))
    .AddTraceSource ("ConnectionReconfiguration",
                     "trace fired upon RRC connection reconfiguration",
                     MakeTraceSourceAccessor (&LteUeRrc::m_connectionReconfigurationTrace))
    .AddTraceSource ("HandoverStart",
                     "trace fired upon start of a handover procedure",
                     MakeTraceSourceAccessor (&LteUeRrc::m_handoverStartTrace))
    .AddTraceSource ("HandoverEndOk",
                     "trace fired upon successful termination of a handover procedure",
                     MakeTraceSourceAccessor (&LteUeRrc::m_handoverEndOkTrace))
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
LteUeRrc::SetLteUeRrcSapUser (LteUeRrcSapUser * s)
{
  NS_LOG_FUNCTION (this << s);
  m_rrcSapUser = s;
}

LteUeRrcSapProvider*
LteUeRrc::GetLteUeRrcSapProvider ()
{
  NS_LOG_FUNCTION (this);
  return m_rrcSapProvider;
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
  NS_LOG_FUNCTION (this << imsi);
  m_imsi = imsi;
}

uint64_t
LteUeRrc::GetImsi (void)
{
  return m_imsi;
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


uint8_t 
LteUeRrc::GetUlBandwidth () const
{
  NS_LOG_FUNCTION (this);
  return m_ulBandwidth;
}

uint8_t 
LteUeRrc::GetDlBandwidth () const
{
  NS_LOG_FUNCTION (this);
  return m_dlBandwidth;
}

uint16_t 
LteUeRrc::GetDlEarfcn () const
{
  return m_dlEarfcn;
}

uint16_t 
LteUeRrc::GetUlEarfcn () const
{
  NS_LOG_FUNCTION (this);
  return m_ulEarfcn;
}


LteUeRrc::State
LteUeRrc::GetState (void)
{
  NS_LOG_FUNCTION (this);
  return m_state;
}

void
LteUeRrc::SetUseRlcSm (bool val) 
{
  NS_LOG_FUNCTION (this);
  m_useRlcSm = val;
}


void
LteUeRrc::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);

  // setup the UE side of SRB0
  uint8_t lcid = 0;

  Ptr<LteRlc> rlc = CreateObject<LteRlcTm> ()->GetObject<LteRlc> ();
  rlc->SetLteMacSapProvider (m_macSapProvider);
  rlc->SetRnti (m_rnti);
  rlc->SetLcId (lcid);

  m_srb0 = CreateObject<LteSignalingRadioBearerInfo> ();  
  m_srb0->m_rlc = rlc;
  m_srb0->m_srbIdentity = 0;
  LteUeRrcSapUser::SetupParameters ueParams;
  ueParams.srb0SapProvider = m_srb0->m_rlc->GetLteRlcSapProvider ();
  ueParams.srb1SapProvider = 0;
  m_rrcSapUser->Setup (ueParams);

  // CCCH (LCID 0) is pre-configured, here is the hardcoded configuration:
  LteUeCmacSapProvider::LogicalChannelConfig lcConfig;
  lcConfig.priority = 0; // highest priority
  lcConfig.prioritizedBitRateKbps = 65535; // maximum
  lcConfig.bucketSizeDurationMs = 65535; // maximum
  lcConfig.logicalChannelGroup = 0; // all SRBs mapped to LCG 0

  m_cmacSapProvider->AddLc (lcid, lcConfig, rlc->GetLteMacSapUser ());
    
}


void
LteUeRrc::DoSendData (Ptr<Packet> packet, uint8_t bid)
{
  NS_LOG_FUNCTION (this << packet);


  uint8_t drbid = Bid2Drbid (bid);

  std::map<uint8_t, Ptr<LteDataRadioBearerInfo> >::iterator it =   m_drbMap.find (drbid);
  NS_ASSERT_MSG (it != m_drbMap.end (), "could not find bearer with drbid == " << drbid);

  LtePdcpSapProvider::TransmitPdcpSduParameters params;
  params.pdcpSdu = packet;
  params.rnti = m_rnti;
  params.lcid = it->second->m_logicalChannelIdentity;
  
  NS_LOG_LOGIC (this << " RNTI=" << m_rnti << " sending " << packet << "on DRBID " << (uint32_t) drbid << " (LCID" << params.lcid << ")" << " (" << packet->GetSize () << " bytes)");
  it->second->m_pdcp->GetLtePdcpSapProvider ()->TransmitPdcpSdu (params);  
}

void
LteUeRrc::DoDisconnect ()
{
  NS_LOG_FUNCTION (this);

  switch (m_state)
    {      
    case IDLE_CELL_SELECTION:
    case IDLE_CAMPED_NORMALLY:
      NS_LOG_INFO ("already disconnected");
      break;

    case IDLE_CONNECTING:
      NS_LOG_INFO ("aborting connection setup procedure");
      SwitchToState (IDLE_CAMPED_NORMALLY);
      break;
      
    case CONNECTED_NORMALLY:
    case CONNECTED_REESTABLISHING:
    case CONNECTED_HANDOVER:
      LeaveConnectedMode ();
      break;
      
    default:
      NS_FATAL_ERROR ("method unexpected in state " << ToString (m_state));
      break;
    }
}

void
LteUeRrc::DoReceivePdcpSdu (LtePdcpSapUser::ReceivePdcpSduParameters params)
{
  NS_LOG_FUNCTION (this);
  m_asSapUser->RecvData (params.pdcpSdu);
}


void
LteUeRrc::DoSetTemporaryCellRnti (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << rnti);
  m_rnti = rnti;
  m_srb0->m_rlc->SetRnti (m_rnti);
  m_cphySapProvider->SetRnti (m_rnti);
}

void
LteUeRrc::DoNotifyRandomAccessSuccessful ()
{
  NS_LOG_FUNCTION (this << m_imsi << ToString (m_state));
  m_randomAccessSuccessfulTrace (m_imsi, m_cellId, m_rnti);  
  switch (m_state)
    {
    case IDLE_RANDOM_ACCESS:     
      {
        // we just received a RAR with a T-C-RNTI and an UL grant
        // send RRC connection request as message 3 of the random access procedure 
        SwitchToState (IDLE_CONNECTING);
        LteRrcSap::RrcConnectionRequest msg;
        msg.ueIdentity = m_imsi;
        m_rrcSapUser->SendRrcConnectionRequest (msg); 
      }
      break;
      
    case CONNECTED_HANDOVER:     
      {
        LteRrcSap::RrcConnectionReconfigurationCompleted msg;
        msg.rrcTransactionIdentifier = m_lastRrcTransactionIdentifier;
        m_rrcSapUser->SendRrcConnectionReconfigurationCompleted (msg);
        SwitchToState (CONNECTED_NORMALLY);
        m_handoverEndOkTrace (m_imsi, m_cellId, m_rnti);
      }
      break;
          
    default:
      NS_FATAL_ERROR ("unexpected event in state " << ToString (m_state));
      break; 
    } 
}

void
LteUeRrc::DoNotifyRandomAccessFailed ()
{
  NS_LOG_FUNCTION (this);
}


void 
LteUeRrc::DoForceCampedOnEnb (uint16_t cellId, uint16_t earfcn)
{
  NS_LOG_FUNCTION (this << cellId << earfcn);
    
  m_cellId = cellId;
  m_dlEarfcn = earfcn;
  m_cphySapProvider->SyncronizeWithEnb (m_cellId, m_dlEarfcn); 
  SwitchToState (IDLE_WAIT_SYSTEM_INFO);
}

void 
LteUeRrc::DoConnect ()
{
  NS_LOG_FUNCTION (this);
  switch (m_state)
    {
    case IDLE_CELL_SELECTION:
    case IDLE_WAIT_SYSTEM_INFO:
      m_connectionPending = true;
      break;

    case IDLE_CAMPED_NORMALLY:     
      StartConnection ();
      break;

    case IDLE_RANDOM_ACCESS:
    case IDLE_CONNECTING:
      NS_LOG_WARN ("already connecting (state " << ToString (m_state) << ")");      
      break;

    case CONNECTED_NORMALLY:
    case CONNECTED_REESTABLISHING:
    case CONNECTED_HANDOVER:
      NS_LOG_WARN ("already connected (state " << ToString (m_state) << ")");      
      break;  
      
    default:
      NS_FATAL_ERROR ("cannot connect while in state " << ToString (m_state));
      break;
    }


}



// CPHY SAP methods

void 
LteUeRrc::DoRecvMasterInformationBlock (LteRrcSap::MasterInformationBlock msg)  
{ 
  NS_LOG_FUNCTION (this);
  m_dlBandwidth = msg.dlBandwidth;
  m_cphySapProvider->SetDlBandwidth (msg.dlBandwidth);
  m_receivedMib = true;
  if (m_state == IDLE_WAIT_SYSTEM_INFO && m_receivedMib && m_receivedSib2)
    {
      SwitchToState (IDLE_CAMPED_NORMALLY);
    }
}

void
LteUeRrc::DoReportUeMeasurements (LteUeCphySapUser::UeMeasurementsParameters params)
{
  NS_LOG_FUNCTION (this);

  for (std::vector <LteUeCphySapUser::UeMeasurementsElement>::iterator newMeasIt = params.m_ueMeasurementsList.begin (); newMeasIt != params.m_ueMeasurementsList.end (); ++newMeasIt)
    {      
      NS_LOG_LOGIC (this << " CellId " << newMeasIt->m_cellId << " RSRP " << newMeasIt->m_rsrp << " RSRQ " << newMeasIt->m_rsrq);    
      
      //  3GPP TS 36.331 section 5.5.3.2 Layer 3 filtering
      std::map<uint16_t, MeasValues>::iterator storedMeasIt = m_storedMeasValues.find (newMeasIt->m_cellId);
      if (storedMeasIt == m_storedMeasValues.end ())
        {
          // first value is unfiltered
          MeasValues v;
          v.rsrp = newMeasIt->m_rsrp;
          v.rsrq = newMeasIt->m_rsrq;
          std::pair<uint16_t, MeasValues> val (newMeasIt->m_cellId, v);
          std::pair<std::map<uint16_t, MeasValues>::iterator, bool> 
            ret = m_storedMeasValues.insert (val);
          NS_ASSERT_MSG (ret.second == true, "element already existed");
          storedMeasIt = ret.first;
        }
      else
        {
          // F_n = (1-a) F_{n-1} + a M_n
          storedMeasIt->second.rsrp = (1 - m_varMeasConfig.aRsrp) * storedMeasIt->second.rsrp 
            + m_varMeasConfig.aRsrp * newMeasIt->m_rsrp;          
          storedMeasIt->second.rsrq = (1 - m_varMeasConfig.aRsrq) * storedMeasIt->second.rsrq 
            + m_varMeasConfig.aRsrq * newMeasIt->m_rsrq;          
        }  
      storedMeasIt->second.timestamp = Simulator::Now ();
    }

                  
  //  3GPP TS 36.331 section 5.5.4.1 Measurement report triggering - General
  for (std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator measIdIt 
         = m_varMeasConfig.measIdList.begin ();
       measIdIt != m_varMeasConfig.measIdList.end ();
       ++measIdIt)               
    {
      NS_ASSERT (measIdIt->first == measIdIt->second.measId);
      uint8_t measId = measIdIt->first;
      NS_LOG_LOGIC (this << " considering measId " << (uint32_t) measId);

      std::map<uint8_t, LteRrcSap::ReportConfigToAddMod>::iterator 
        reportConfigIt = m_varMeasConfig.reportConfigList.find (measIdIt->second.reportConfigId);
      NS_ASSERT (reportConfigIt != m_varMeasConfig.reportConfigList.end ());
      LteRrcSap::ReportConfigEutra& reportConfigEutra = reportConfigIt->second.reportConfigEutra;

      std::map<uint8_t, LteRrcSap::MeasObjectToAddMod>::iterator 
        measObjectIt = m_varMeasConfig.measObjectList.find (measIdIt->second.measObjectId);
      NS_ASSERT (measObjectIt != m_varMeasConfig.measObjectList.end ());
      LteRrcSap::MeasObjectEutra& measObjectEutra = measObjectIt->second.measObjectEutra;

      std::map<uint8_t, VarMeasReport>::iterator 
        measReportIt = m_varMeasReportList.find (measId);
          
      // we don't check the purpose field, as it is only included for
      // triggerType == periodical, which is not supported  
      NS_ASSERT_MSG (reportConfigEutra.triggerType 
                     == LteRrcSap::ReportConfigEutra::EVENT,
                     "only triggerType == event is supported");
      // only EUTRA is supported, no need to check for it

      bool eventEntryCondApplicable = false;
      bool eventLeavingCondApplicable = false;

      std::list<uint16_t> concernedCellsEntry;
      std::list<uint16_t> concernedCellsLeaving;
              
      switch (reportConfigEutra.eventId)
        {
        case LteRrcSap::ReportConfigEutra::EVENT_A2:
          {
            double ms; // Ms, the measurement for serving cell
            double thresh; // Tresh, the threshold parameter for this event
            double hys = (double) reportConfigEutra.hysteresis * 0.5; // Hys, the hysteresis parameter for this event. See 36.331 section 6.3.5 for the conversion.
            switch (reportConfigEutra.triggerQuantity)
              {
              case LteRrcSap::ReportConfigEutra::RSRP:
                ms = m_storedMeasValues[m_cellId].rsrp;
                //ms = EutranMeasurementMapping::QuantizeRsrp (m_storedMeasValues[m_cellId].rsrp);
                NS_ASSERT (reportConfigEutra.threshold1.choice 
                           == LteRrcSap::ThresholdEutra::THRESHOLD_RSRP);
                thresh = EutranMeasurementMapping::RsrpRange2Dbm (reportConfigEutra.threshold1.range);
                break;
              case LteRrcSap::ReportConfigEutra::RSRQ:
                ms = m_storedMeasValues[m_cellId].rsrq;
                //ms = EutranMeasurementMapping::QuantizeRsrq (m_storedMeasValues[m_cellId].rsrq);
                NS_ASSERT (reportConfigEutra.threshold1.choice 
                           == LteRrcSap::ThresholdEutra::THRESHOLD_RSRQ);                
                thresh = EutranMeasurementMapping::RsrqRange2Db (reportConfigEutra.threshold1.range);
                break;
              default:
                NS_FATAL_ERROR ("unsupported triggerQuantity");
                break;
              }            
            // Inequality A2-1 (Entering condition) :  Ms + Hys < Thresh
            bool entryCond = ms + hys < thresh;
            if (entryCond == true 
                && (measReportIt == m_varMeasReportList.end ()
                    || (measReportIt != m_varMeasReportList.end () 
                        && (measReportIt->second.cellsTriggeredList.find (m_cellId) 
                            == measReportIt->second.cellsTriggeredList.end ()))))
              {
                concernedCellsEntry.push_back (m_cellId);
                eventEntryCondApplicable = true;
              }
            // Inequality A2-2 (Leaving condition) : Ms − Hys > Thresh
            bool leavingCond = ms - hys > thresh;
            if (leavingCond         
                && measReportIt != m_varMeasReportList.end ()
                && (measReportIt->second.cellsTriggeredList.find (m_cellId) 
                    != measReportIt->second.cellsTriggeredList.end ()))
              {
                concernedCellsLeaving.push_back (m_cellId);
                eventLeavingCondApplicable = true;
              }
            NS_LOG_LOGIC ("event A2: serving cell " << m_cellId << " ms=" << ms << " thresh=" << thresh << " entryCond=" << entryCond << " leavingCond=" << leavingCond);
          }                  
          break;

        case LteRrcSap::ReportConfigEutra::EVENT_A4:
          {
            for (std::map<uint16_t, MeasValues>::iterator storedMeasIt = m_storedMeasValues.begin ();
                 storedMeasIt != m_storedMeasValues.end ();
                 ++storedMeasIt)
              {
                uint16_t cellId = storedMeasIt->first;
                if (cellId != m_cellId) 
                  {
                    double mn; // Mn, the measurement for neighboring cell
                    double thresh; // Tresh, the threshold parameter for this event
                    double hys = (double) reportConfigEutra.hysteresis * 0.5; // Hys, the hysteresis parameter for this event. See 36.331 section 6.3.5 for the conversion.
                    switch (reportConfigEutra.triggerQuantity)
                      {
                      case LteRrcSap::ReportConfigEutra::RSRP:
                        mn = storedMeasIt->second.rsrp;
                        //mn = EutranMeasurementMapping::QuantizeRsrp (storedMeasIt->second.rsrp);
                        NS_ASSERT (reportConfigEutra.threshold1.choice 
                                   == LteRrcSap::ThresholdEutra::THRESHOLD_RSRP);    
                        thresh = EutranMeasurementMapping::RsrpRange2Dbm (reportConfigEutra.threshold1.range);            
                        break;
                      case LteRrcSap::ReportConfigEutra::RSRQ:
                        mn = storedMeasIt->second.rsrq;
                        //mn = EutranMeasurementMapping::QuantizeRsrq (storedMeasIt->second.rsrq);
                        NS_ASSERT (reportConfigEutra.threshold1.choice 
                                   == LteRrcSap::ThresholdEutra::THRESHOLD_RSRQ);                
                        thresh = EutranMeasurementMapping::RsrqRange2Db (reportConfigEutra.threshold1.range);
                        break;
                      default:
                        NS_FATAL_ERROR ("unsupported triggerQuantity");
                        break;
                      }            
                    // Inequality A4-1 (Entering condition) :  Mn + Ofn + Ocn − Hys > Thresh
                    bool entryCond = mn + measObjectEutra.offsetFreq + 0.0 - hys > thresh;
                    if (entryCond == true 
                        && (measReportIt == m_varMeasReportList.end ()
                            || (measReportIt != m_varMeasReportList.end () 
                                && (measReportIt->second.cellsTriggeredList.find (cellId) 
                                    == measReportIt->second.cellsTriggeredList.end ()))))
                      {
                        concernedCellsEntry.push_back (cellId);
                        eventEntryCondApplicable = true;
                      }
                    // Inequality A4-2 (Leaving condition) : Mn + Ofn + Ocn + Hys < Thresh
                    bool leavingCond = mn + measObjectEutra.offsetFreq + 0.0 + hys < thresh;
                    if (leavingCond         
                        && measReportIt != m_varMeasReportList.end ()
                        && (measReportIt->second.cellsTriggeredList.find (cellId) 
                            != measReportIt->second.cellsTriggeredList.end ()))
                      {
                        concernedCellsLeaving.push_back (cellId);
                        eventLeavingCondApplicable = true;
                      }
                   
                    NS_LOG_LOGIC ("event A4: neighbor cell " << cellId << " mn=" << mn << " thresh=" << thresh << " entryCond=" << entryCond << " leavingCond=" << leavingCond);
                  }
              }
          }
          break;

        default:
          NS_FATAL_ERROR ("unsupported eventId " << reportConfigEutra.eventId);
          break;
          
          } // switch (event type)

      NS_LOG_LOGIC ("eventEntryCondApplicable=" << eventEntryCondApplicable
                    << " eventLeavingCondApplicable=" << eventLeavingCondApplicable);

      bool initiateUeMeasurementReportingProcedure = false;

      if (eventEntryCondApplicable)
        {          
          if (measReportIt == m_varMeasReportList.end ())
            {
              VarMeasReport r;
              r.measId = measId;              
              std::pair<uint8_t, VarMeasReport> val (measId, r);
              std::pair<std::map<uint8_t, VarMeasReport>::iterator, bool> 
                ret = m_varMeasReportList.insert (val);
              NS_ASSERT_MSG (ret.second == true, "element already existed");
              measReportIt = ret.first;
            }
          for (std::list<uint16_t>::iterator it = concernedCellsEntry.begin ();
               it != concernedCellsEntry.end ();
               ++it)
            {
              measReportIt->second.cellsTriggeredList.insert (*it);
            }
          measReportIt->second.numberOfReportsSent = 0;
          initiateUeMeasurementReportingProcedure = true;
        }
      
      if (eventLeavingCondApplicable)
        {                  
          NS_ASSERT (measReportIt != m_varMeasReportList.end ());
          for (std::list<uint16_t>::iterator it = concernedCellsLeaving.begin ();
               it != concernedCellsLeaving.end ();
               ++it)
            {
              measReportIt->second.cellsTriggeredList.erase (*it);
            }

          // reportOnLeave will only be set when eventId = eventA3
          if(reportConfigEutra.eventId == LteRrcSap::ReportConfigEutra::EVENT_A3)
            {
              if (reportConfigEutra.reportOnLeave)
                {
                  initiateUeMeasurementReportingProcedure = true;
                }
            }    
        }

      if (initiateUeMeasurementReportingProcedure)
        {
          SendMeasurementReport (measId);
        }    
      
      if ((measReportIt != m_varMeasReportList.end ())
          && (measReportIt->second.cellsTriggeredList.empty ()))
        {
          measReportIt->second.periodicReportTimer.Cancel ();
          m_varMeasReportList.erase (measReportIt);
        }
      
    } // for each measId in m_varMeasConfig.measIdList   
}



// RRC SAP methods

void 
LteUeRrc::DoCompleteSetup (LteUeRrcSapProvider::CompleteSetupParameters params)
{
  NS_LOG_FUNCTION (this);
  m_srb0->m_rlc->SetLteRlcSapUser (params.srb0SapUser);
  if (m_srb1)
    {
      m_srb1->m_pdcp->SetLtePdcpSapUser (params.srb1SapUser);
    }
}


void 
LteUeRrc::DoRecvSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 msg)
{
  NS_LOG_FUNCTION (this);
  // to be implemented
}

 
void 
LteUeRrc::DoRecvSystemInformation (LteRrcSap::SystemInformation msg)
{
  NS_LOG_FUNCTION (this);
  if (msg.haveSib2)
    {
      m_receivedSib2 = true;
      m_ulBandwidth = msg.sib2.freqInfo.ulBandwidth;
      m_ulEarfcn = msg.sib2.freqInfo.ulCarrierFreq;
      LteUeCmacSapProvider::RachConfig rc;
      rc.numberOfRaPreambles = msg.sib2.radioResourceConfigCommon.rachConfigCommon.preambleInfo.numberOfRaPreambles;
      rc.preambleTransMax = msg.sib2.radioResourceConfigCommon.rachConfigCommon.raSupervisionInfo.preambleTransMax;
      rc.raResponseWindowSize = msg.sib2.radioResourceConfigCommon.rachConfigCommon.raSupervisionInfo.raResponseWindowSize;
      m_cmacSapProvider->ConfigureRach (rc);
      m_cphySapProvider->ConfigureUplink (m_ulEarfcn, m_ulBandwidth);
    }
  if (m_state == IDLE_WAIT_SYSTEM_INFO && m_receivedMib && m_receivedSib2)
    {
       SwitchToState (IDLE_CAMPED_NORMALLY);
    }
}


void 
LteUeRrc::DoRecvRrcConnectionSetup (LteRrcSap::RrcConnectionSetup msg)
{    
  NS_LOG_FUNCTION (this);
  switch (m_state)
    {
    case IDLE_CONNECTING:
      {
        ApplyRadioResourceConfigDedicated (msg.radioResourceConfigDedicated);
        SwitchToState (CONNECTED_NORMALLY);
        LteRrcSap::RrcConnectionSetupCompleted msg2;
        msg2.rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
        m_rrcSapUser->SendRrcConnectionSetupCompleted (msg2);
        m_asSapUser->NotifyConnectionSuccessful ();
        m_connectionEstablishedTrace (m_imsi, m_cellId, m_rnti);
      }
      break;
      
    default:
      NS_FATAL_ERROR ("method unexpected in state " << ToString (m_state));
      break;
    }
}

void
LteUeRrc::DoRecvRrcConnectionReconfiguration (LteRrcSap::RrcConnectionReconfiguration msg)
{
  NS_LOG_FUNCTION (this << " RNTI " << m_rnti);
  switch (m_state)
    {
    case CONNECTED_NORMALLY:
      if (msg.haveMobilityControlInfo)
        {
          NS_LOG_INFO ("haveMobilityControlInfo == true");
          SwitchToState (CONNECTED_HANDOVER);
          const LteRrcSap::MobilityControlInfo& mci = msg.mobilityControlInfo;
          m_handoverStartTrace (m_imsi, m_cellId, m_rnti, mci.targetPhysCellId);
          m_cmacSapProvider->Reset ();
          m_cphySapProvider->Reset ();
          m_cellId = mci.targetPhysCellId;
          NS_ASSERT (mci.haveCarrierFreq);
          NS_ASSERT (mci.haveCarrierBandwidth);
          m_cphySapProvider->SyncronizeWithEnb (m_cellId, mci.carrierFreq.dlCarrierFreq); 
          m_cphySapProvider->SetDlBandwidth ( mci.carrierBandwidth.dlBandwidth); 
          m_cphySapProvider->ConfigureUplink (mci.carrierFreq.ulCarrierFreq, mci.carrierBandwidth.ulBandwidth); 
          m_rnti = msg.mobilityControlInfo.newUeIdentity;
          m_srb0->m_rlc->SetRnti (m_rnti);
          NS_ASSERT_MSG (mci.haveRachConfigDedicated, "handover is only supported with non-contention-based random access procedure");          
          m_cmacSapProvider->StartNonContentionBasedRandomAccessProcedure (m_rnti, mci.rachConfigDedicated.raPreambleIndex, mci.rachConfigDedicated.raPrachMaskIndex);
          m_cphySapProvider->SetRnti (m_rnti);
          m_lastRrcTransactionIdentifier = msg.rrcTransactionIdentifier;
          NS_ASSERT (msg.haveRadioResourceConfigDedicated);

          // we re-establish SRB1 by creating a new entity
          // note that we can't dispose the old entity now, because
          // it's in the current stack, so we would corrupt the stack
          // if we did so. Hence we schedule it for later disposal
          m_srb1Old = m_srb1;
          Simulator::ScheduleNow (&LteUeRrc::DisposeOldSrb1, this);          
          m_srb1 = 0; // new instance will be be created within ApplyRadioResourceConfigDedicated

          m_drbMap.clear (); // dispose all DRBs
          ApplyRadioResourceConfigDedicated (msg.radioResourceConfigDedicated);    

          if (msg.haveMeasConfig)
            {
              ApplyMeasConfig (msg.measConfig);
            }
          // RRC connection reconfiguration completed will be sent
          // after handover is complete
        }
      else
        {
          NS_LOG_INFO ("haveMobilityControlInfo == false");
          if (msg.haveRadioResourceConfigDedicated)
            {
              ApplyRadioResourceConfigDedicated (msg.radioResourceConfigDedicated);
            } 
          if (msg.haveMeasConfig)
            {
              ApplyMeasConfig (msg.measConfig);
            }          
          LteRrcSap::RrcConnectionReconfigurationCompleted msg2;
          msg2.rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
          m_rrcSapUser->SendRrcConnectionReconfigurationCompleted (msg2);
          m_connectionReconfigurationTrace (m_imsi, m_cellId, m_rnti);
        }
      break;

    default:
      NS_FATAL_ERROR ("method unexpected in state " << ToString (m_state));
      break;
    }
}

void 
LteUeRrc::DoRecvRrcConnectionReestablishment (LteRrcSap::RrcConnectionReestablishment msg)
{
  switch (m_state)
    {
    case CONNECTED_REESTABLISHING:
      {
      }
      break;
      
    default:
      NS_FATAL_ERROR ("method unexpected in state " << ToString (m_state));
      break;
    }
}

void 
LteUeRrc::DoRecvRrcConnectionReestablishmentReject (LteRrcSap::RrcConnectionReestablishmentReject msg)
{
  NS_LOG_FUNCTION (this);
  switch (m_state)
    {
    case CONNECTED_REESTABLISHING:
      {
        LeaveConnectedMode ();
      }
      break;
      
    default:
      NS_FATAL_ERROR ("method unexpected in state " << ToString (m_state));
      break;
    }
}

void 
LteUeRrc::DoRecvRrcConnectionRelease (LteRrcSap::RrcConnectionRelease msg)
{
  NS_LOG_FUNCTION (this);
}

void 
LteUeRrc::DoRecvRrcConnectionReject (LteRrcSap::RrcConnectionReject msg)
{
  NS_LOG_FUNCTION (this);
  m_cmacSapProvider->Reset ();
  SwitchToState (IDLE_CAMPED_NORMALLY);
}



void 
LteUeRrc::ApplyRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated rrcd)
{
  NS_LOG_FUNCTION (this);
  const struct LteRrcSap::PhysicalConfigDedicated& pcd = rrcd.physicalConfigDedicated;
  
  if (pcd.haveAntennaInfoDedicated)
    {
      m_cphySapProvider->SetTransmissionMode (pcd.antennaInfo.transmissionMode);
    }
  if (pcd.haveSoundingRsUlConfigDedicated)
    {
      m_cphySapProvider->SetSrsConfigurationIndex (pcd.soundingRsUlConfigDedicated.srsConfigIndex);
    }

  std::list<LteRrcSap::SrbToAddMod>::const_iterator stamIt = rrcd.srbToAddModList.begin ();
  if (stamIt != rrcd.srbToAddModList.end ())
    {
      if (m_srb1 == 0)
        {
          // SRB1 not setup yet        
          NS_ASSERT_MSG ((m_state == IDLE_CONNECTING) || (m_state == CONNECTED_HANDOVER), 
                         "unexpected state " << ToString (m_state));
          NS_ASSERT_MSG (stamIt->srbIdentity == 1, "only SRB1 supported");

          const uint8_t lcid = 1; // fixed LCID for SRB1

          Ptr<LteRlc> rlc = CreateObject<LteRlcAm> ();
          rlc->SetLteMacSapProvider (m_macSapProvider);
          rlc->SetRnti (m_rnti);
          rlc->SetLcId (lcid);      

          Ptr<LtePdcp> pdcp = CreateObject<LtePdcp> ();
          pdcp->SetRnti (m_rnti);
          pdcp->SetLcId (lcid);
          pdcp->SetLtePdcpSapUser (m_drbPdcpSapUser);
          pdcp->SetLteRlcSapProvider (rlc->GetLteRlcSapProvider ());
          rlc->SetLteRlcSapUser (pdcp->GetLteRlcSapUser ());

          m_srb1 = CreateObject<LteSignalingRadioBearerInfo> ();
          m_srb1->m_rlc = rlc;
          m_srb1->m_pdcp = pdcp;
          m_srb1->m_srbIdentity = 1;
          
          m_srb1->m_logicalChannelConfig.priority = stamIt->logicalChannelConfig.priority;
          m_srb1->m_logicalChannelConfig.prioritizedBitRateKbps = stamIt->logicalChannelConfig.prioritizedBitRateKbps;
          m_srb1->m_logicalChannelConfig.bucketSizeDurationMs = stamIt->logicalChannelConfig.bucketSizeDurationMs;
          m_srb1->m_logicalChannelConfig.logicalChannelGroup = stamIt->logicalChannelConfig.logicalChannelGroup;

          struct LteUeCmacSapProvider::LogicalChannelConfig lcConfig;
          lcConfig.priority = stamIt->logicalChannelConfig.priority;
          lcConfig.prioritizedBitRateKbps = stamIt->logicalChannelConfig.prioritizedBitRateKbps;
          lcConfig.bucketSizeDurationMs = stamIt->logicalChannelConfig.bucketSizeDurationMs;
          lcConfig.logicalChannelGroup = stamIt->logicalChannelConfig.logicalChannelGroup;
      
          m_cmacSapProvider->AddLc (lcid, lcConfig, rlc->GetLteMacSapUser ());
      
          ++stamIt;
          NS_ASSERT_MSG (stamIt == rrcd.srbToAddModList.end (), "at most one SrbToAdd supported");     
          
          LteUeRrcSapUser::SetupParameters ueParams;
          ueParams.srb0SapProvider = m_srb0->m_rlc->GetLteRlcSapProvider ();
          ueParams.srb1SapProvider = m_srb1->m_pdcp->GetLtePdcpSapProvider ();
          m_rrcSapUser->Setup (ueParams);
        }
      else
        {
          NS_LOG_INFO ("request to modify SRB1 (skipping as currently not implemented)");
          // would need to modify m_srb1, and then propagate changes to the MAC
        }
    }


  std::list<LteRrcSap::DrbToAddMod>::const_iterator dtamIt;
  for (dtamIt = rrcd.drbToAddModList.begin ();
       dtamIt != rrcd.drbToAddModList.end ();
       ++dtamIt)
    {
      NS_LOG_INFO (this << " IMSI " << m_imsi << " adding/modifying DRBID " << (uint32_t) dtamIt->drbIdentity << " LC " << (uint32_t) dtamIt->logicalChannelIdentity);
      NS_ASSERT_MSG (dtamIt->logicalChannelIdentity > 2, "LCID value " << dtamIt->logicalChannelIdentity << " is reserved for SRBs");

      std::map<uint8_t, Ptr<LteDataRadioBearerInfo> >::iterator drbMapIt = m_drbMap.find (dtamIt->drbIdentity);
      if (drbMapIt == m_drbMap.end ())
        {
          NS_LOG_INFO ("New Data Radio Bearer");
        
          TypeId rlcTypeId;
          if (m_useRlcSm)
            {
              rlcTypeId = LteRlcSm::GetTypeId ();
            }
          else
            {
              switch (dtamIt->rlcConfig.choice)
                {
                case LteRrcSap::RlcConfig::AM: 
                  rlcTypeId = LteRlcAm::GetTypeId ();
                  break;
          
                case LteRrcSap::RlcConfig::UM_BI_DIRECTIONAL: 
                  rlcTypeId = LteRlcUm::GetTypeId ();
                  break;
          
                default:
                  NS_FATAL_ERROR ("unsupported RLC configuration");
                  break;                
                }
            }
  
          ObjectFactory rlcObjectFactory;
          rlcObjectFactory.SetTypeId (rlcTypeId);
          Ptr<LteRlc> rlc = rlcObjectFactory.Create ()->GetObject<LteRlc> ();
          rlc->SetLteMacSapProvider (m_macSapProvider);
          rlc->SetRnti (m_rnti);
          rlc->SetLcId (dtamIt->logicalChannelIdentity);

          Ptr<LteDataRadioBearerInfo> drbInfo = CreateObject<LteDataRadioBearerInfo> ();
          drbInfo->m_rlc = rlc;
          drbInfo->m_epsBearerIdentity = dtamIt->epsBearerIdentity;
          drbInfo->m_logicalChannelIdentity = dtamIt->logicalChannelIdentity;
          drbInfo->m_drbIdentity = dtamIt->drbIdentity;
 
          // we need PDCP only for real RLC, i.e., RLC/UM or RLC/AM
          // if we are using RLC/SM we don't care of anything above RLC
          if (rlcTypeId != LteRlcSm::GetTypeId ())
            {
              Ptr<LtePdcp> pdcp = CreateObject<LtePdcp> ();
              pdcp->SetRnti (m_rnti);
              pdcp->SetLcId (dtamIt->logicalChannelIdentity);
              pdcp->SetLtePdcpSapUser (m_drbPdcpSapUser);
              pdcp->SetLteRlcSapProvider (rlc->GetLteRlcSapProvider ());
              rlc->SetLteRlcSapUser (pdcp->GetLteRlcSapUser ());
              drbInfo->m_pdcp = pdcp;
            }

          m_bid2DrbidMap[dtamIt->epsBearerIdentity] = dtamIt->drbIdentity;
  
          m_drbMap.insert (std::pair<uint8_t, Ptr<LteDataRadioBearerInfo> > (dtamIt->drbIdentity, drbInfo));
  

          struct LteUeCmacSapProvider::LogicalChannelConfig lcConfig;
          lcConfig.priority = dtamIt->logicalChannelConfig.priority;
          lcConfig.prioritizedBitRateKbps = dtamIt->logicalChannelConfig.prioritizedBitRateKbps;
          lcConfig.bucketSizeDurationMs = dtamIt->logicalChannelConfig.bucketSizeDurationMs;
          lcConfig.logicalChannelGroup = dtamIt->logicalChannelConfig.logicalChannelGroup;      

          m_cmacSapProvider->AddLc (dtamIt->logicalChannelIdentity,
                                    lcConfig,
                                    rlc->GetLteMacSapUser ());
          rlc->Initialize ();
        }
      else
        {
          NS_LOG_INFO ("request to modify existing DRBID");
          Ptr<LteDataRadioBearerInfo> drbInfo = drbMapIt->second;
          /// \todo currently not implemented. Would need to modify drbInfo, and then propagate changes to the MAC
        }
    }
  
  std::list<uint8_t>::iterator dtdmIt;
  for (dtdmIt = rrcd.drbToReleaseList.begin ();
       dtdmIt != rrcd.drbToReleaseList.end ();
       ++dtdmIt)
    {
      uint8_t drbid = *dtdmIt;
      NS_LOG_INFO (this << " IMSI " << m_imsi << " releasing DRB " << (uint32_t) drbid << drbid);
      std::map<uint8_t, Ptr<LteDataRadioBearerInfo> >::iterator it =   m_drbMap.find (drbid);
      NS_ASSERT_MSG (it != m_drbMap.end (), "could not find bearer with given lcid");
      m_drbMap.erase (it);      
      m_bid2DrbidMap.erase (drbid);
    }
}


void 
LteUeRrc::ApplyMeasConfig (LteRrcSap::MeasConfig mc)
{
  NS_LOG_FUNCTION (this);

  // perform the actions specified in 3GPP TS 36.331 section 5.5.2.1 

  // 3GPP TS 36.331 section 5.5.2.4 Measurement object removal
  for (std::list<uint8_t>::iterator it = mc.measObjectToRemoveList.begin ();
       it !=  mc.measObjectToRemoveList.end ();
       ++it)
    {
      uint8_t measObjectId = *it;
      NS_LOG_LOGIC (this << " deleting measObjectId " << (uint32_t)  measObjectId);
      m_varMeasConfig.measObjectList.erase (measObjectId);
      std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator measIdIt = m_varMeasConfig.measIdList.begin ();
      while (measIdIt != m_varMeasConfig.measIdList.end ())
        {
          if (measIdIt->second.measObjectId == measObjectId)
            {
              uint8_t measId = measIdIt->second.measId;
              NS_ASSERT (measId == measIdIt->first);
              NS_LOG_LOGIC (this << " deleting measId " << (uint32_t) measId << " because referring to measObjectId " << (uint32_t)  measObjectId);
              // note: postfix operator preserves iterator validity
              m_varMeasConfig.measIdList.erase (measIdIt++);
              std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
              if (measReportIt != m_varMeasReportList.end ())
                {                  
                  NS_LOG_LOGIC (this << " deleting existing report for measId " << (uint32_t) measId << " because referring to measObjectId " << (uint32_t)  measObjectId);
                  measReportIt->second.periodicReportTimer.Cancel ();
                  m_varMeasReportList.erase (measReportIt);
                }
            }
          else
            {
              ++measIdIt;
            }
        }    
      
    }     

  // 3GPP TS 36.331 section 5.5.2.5  Measurement object addition/ modification  
  for (std::list<LteRrcSap::MeasObjectToAddMod>::iterator it = mc.measObjectToAddModList.begin ();
       it !=  mc.measObjectToAddModList.end ();
       ++it)
    {
      // simplifying assumptions
      NS_ASSERT_MSG (it->measObjectEutra.cellsToRemoveList.empty (), "cellsToRemoveList not supported");
      NS_ASSERT_MSG (it->measObjectEutra.cellsToAddModList.empty (), "cellsToAddModList not supported");
      NS_ASSERT_MSG (it->measObjectEutra.cellsToRemoveList.empty (), "blackCellsToRemoveList not supported");
      NS_ASSERT_MSG (it->measObjectEutra.blackCellsToAddModList.empty (), "blackCellsToAddModList not supported");
      NS_ASSERT_MSG (it->measObjectEutra.haveCellForWhichToReportCGI == false , "cellForWhichToReportCGI is not supported");
      
                     
      uint8_t measObjectId = it->measObjectId;         
      std::map<uint8_t, LteRrcSap::MeasObjectToAddMod>::iterator measObjectIt = m_varMeasConfig.measObjectList.find (measObjectId);      
      if (measObjectIt != m_varMeasConfig.measObjectList.end ())
        {
          NS_LOG_LOGIC ("measObjectId " << (uint32_t) measObjectId << " exists, updating entry");
          measObjectIt->second = *it;          
          for (std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator measIdIt 
                 = m_varMeasConfig.measIdList.begin ();
               measIdIt != m_varMeasConfig.measIdList.end ();
               ++measIdIt)               
            {
              if (measIdIt->second.measObjectId == measObjectId)
                {
                  uint8_t measId = measIdIt->second.measId;
                  NS_LOG_LOGIC (this << " found measId " << (uint32_t) measId << " referring to measObjectId " << (uint32_t)  measObjectId);
                  std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
                  if (measReportIt != m_varMeasReportList.end ())
                    {
                      NS_LOG_LOGIC (this << " deleting existing report for measId " << (uint32_t) measId << " because referring to measObjectId " << (uint32_t)  measObjectId);
                      measReportIt->second.periodicReportTimer.Cancel ();
                      m_varMeasReportList.erase (measReportIt);
                    }
                }
            }
        }
      else
        {
          NS_LOG_LOGIC ("measObjectId " << (uint32_t) measObjectId << " is new, adding entry");     
          m_varMeasConfig.measObjectList[measObjectId] = *it;
        }            
    }

  // 3GPP TS 36.331 section 5.5.2.6 Reporting configuration removal
  for (std::list<uint8_t>::iterator it = mc.reportConfigToRemoveList.begin ();
       it !=  mc.reportConfigToRemoveList.end ();
       ++it)
    {
      uint8_t reportConfigId = *it;
      NS_LOG_LOGIC (this << " deleting reportConfigId " << (uint32_t)  reportConfigId);
      m_varMeasConfig.reportConfigList.erase (reportConfigId);
      std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator measIdIt = m_varMeasConfig.measIdList.begin ();
      while (measIdIt != m_varMeasConfig.measIdList.end ())
        {
          if (measIdIt->second.reportConfigId == reportConfigId)
            {
              uint8_t measId = measIdIt->second.measId;
              NS_ASSERT (measId == measIdIt->first);
              NS_LOG_LOGIC (this << " deleting measId " << (uint32_t) measId << " because referring to reportConfigId " << (uint32_t)  reportConfigId);
              // note: postfix operator preserves iterator validity
              m_varMeasConfig.measIdList.erase (measIdIt++);
              std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
              if (measReportIt != m_varMeasReportList.end ())
                {                  
                  NS_LOG_LOGIC (this << " deleting existing report for measId" << (uint32_t) measId << " because referring to reportConfigId " << (uint32_t)  reportConfigId);
                  measReportIt->second.periodicReportTimer.Cancel ();
                  m_varMeasReportList.erase (measReportIt);
                }
            }
          else
            {
              ++measIdIt;
            }
        }    

    }
  
  // 3GPP TS 36.331 section 5.5.2.7 Reporting configuration addition/ modification
  for (std::list<LteRrcSap::ReportConfigToAddMod>::iterator it = mc.reportConfigToAddModList.begin ();
       it !=  mc.reportConfigToAddModList.end ();
       ++it)
    {
      // simplifying assumptions
      NS_ASSERT_MSG (it->reportConfigEutra.triggerType == LteRrcSap::ReportConfigEutra::EVENT,
                     "only trigger type EVENT is supported");
      NS_ASSERT_MSG (it->reportConfigEutra.eventId == LteRrcSap::ReportConfigEutra::EVENT_A2
                     || it->reportConfigEutra.eventId == LteRrcSap::ReportConfigEutra::EVENT_A4,
                     "only events A2 and A4 are supported");
      NS_ASSERT_MSG (it->reportConfigEutra.timeToTrigger == 0, "timeToTrigger > 0 is not supported");
      
      uint8_t reportConfigId = it->reportConfigId;         
      std::map<uint8_t, LteRrcSap::ReportConfigToAddMod>::iterator reportConfigIt = m_varMeasConfig.reportConfigList.find (reportConfigId);      
      if (reportConfigIt != m_varMeasConfig.reportConfigList.end ())
        {
          NS_LOG_LOGIC ("reportConfigId " << (uint32_t) reportConfigId << " exists, updating entry");
          m_varMeasConfig.reportConfigList[reportConfigId] = *it;          
          for (std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator measIdIt 
                 = m_varMeasConfig.measIdList.begin ();
               measIdIt != m_varMeasConfig.measIdList.end ();
               ++measIdIt)               
            {
              if (measIdIt->second.reportConfigId == reportConfigId)
                {
                  uint8_t measId = measIdIt->second.measId;
                  NS_LOG_LOGIC (this << " found measId " << (uint32_t) measId << " referring to reportConfigId " << (uint32_t)  reportConfigId);
                  std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
                  if (measReportIt != m_varMeasReportList.end ())
                    {
                      NS_LOG_LOGIC (this << " deleting existing report for measId " << (uint32_t) measId << " because referring to reportConfigId " << (uint32_t)  reportConfigId);
                      measReportIt->second.periodicReportTimer.Cancel ();
                      m_varMeasReportList.erase (measReportIt);
                    }
                }
            }
        }
      else
        {
          NS_LOG_LOGIC ("reportConfigId " << (uint32_t) reportConfigId << " is new, adding entry");     
          m_varMeasConfig.reportConfigList[reportConfigId] = *it;
        }            
    }


  // 3GPP TS 36.331 section 5.5.2.8 Quantity configuration
  if (mc.haveQuantityConfig)
    {
      NS_LOG_LOGIC (this << " setting quantityConfig");
      m_varMeasConfig.quantityConfig = mc.quantityConfig;
      std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator measIdIt = m_varMeasConfig.measIdList.begin ();
      while (measIdIt != m_varMeasConfig.measIdList.end ())
        {
          uint8_t measId = measIdIt->second.measId;
          NS_ASSERT (measId == measIdIt->first);
          NS_LOG_LOGIC (this << " deleting measId " << (uint32_t) measId);
          // note: postfix operator preserves iterator validity
          m_varMeasConfig.measIdList.erase (measIdIt++);
          std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
          if (measReportIt != m_varMeasReportList.end ())
            {                  
              NS_LOG_LOGIC (this << " deleting existing report for measId" << (uint32_t) measId);
              measReportIt->second.periodicReportTimer.Cancel ();
              m_varMeasReportList.erase (measReportIt);
            }
        }
      // we calculate here the coefficient a used for Layer 3 filtering, see 3GPP TS 36.331 section 5.5.3.2
      m_varMeasConfig.aRsrp = std::pow (0.5,  mc.quantityConfig.filterCoefficientRSRP/4.0);
      m_varMeasConfig.aRsrq = std::pow (0.5,  mc.quantityConfig.filterCoefficientRSRQ/4.0);
      NS_LOG_LOGIC (this << " new filter coefficients: aRsrp=" << m_varMeasConfig.aRsrp << ", aRsrq=" << m_varMeasConfig.aRsrq);


    }


  // 3GPP TS 36.331 section 5.5.2.2 Measurement identity removal  
  for (std::list<uint8_t>::iterator it = mc.measIdToRemoveList.begin ();
       it !=  mc.measIdToRemoveList.end ();
       ++it)
    {
      uint8_t measId = *it;
      NS_LOG_LOGIC (this << " deleting measId " << (uint32_t) measId);
      m_varMeasConfig.measIdList.erase (measId);
      std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
      if (measReportIt != m_varMeasReportList.end ())
        {
          NS_LOG_LOGIC (this << " deleting existing report for measId" << (uint32_t) measId);
          measReportIt->second.periodicReportTimer.Cancel ();
          m_varMeasReportList.erase (measReportIt);
        }
    }

  // 3GPP TS 36.331 section 5.5.2.3 Measurement identity addition/ modification
  for (std::list<LteRrcSap::MeasIdToAddMod>::iterator it = mc.measIdToAddModList.begin ();
       it !=  mc.measIdToAddModList.end ();
       ++it)
    {
      NS_LOG_LOGIC (this << " measId " << (uint32_t) it->measId << " (measObjectId=" 
                    << (uint32_t) it->measObjectId << ", reportConfigId=" << (uint32_t) it->reportConfigId << ")");
      NS_ASSERT (m_varMeasConfig.measObjectList.find (it->measObjectId)
                 != m_varMeasConfig.measObjectList.end ());
      NS_ASSERT (m_varMeasConfig.reportConfigList.find (it->reportConfigId)
                 != m_varMeasConfig.reportConfigList.end ());      
      m_varMeasConfig.measIdList[it->measId] = *it; // side effect: create new entry if not exists
      std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (it->measId);
      if (measReportIt != m_varMeasReportList.end ())
        {
          measReportIt->second.periodicReportTimer.Cancel ();
          m_varMeasReportList.erase (measReportIt);
        }
      NS_ASSERT (m_varMeasConfig.reportConfigList.find (it->reportConfigId)
                 ->second.reportConfigEutra.triggerType != LteRrcSap::ReportConfigEutra::PERIODICAL);

    }

  if (mc.haveMeasGapConfig)
    {
      NS_FATAL_ERROR ("measurement gaps are currently not supported");
    }

  if (mc.haveSmeasure)
    {
      NS_FATAL_ERROR ("s-measure is currently not supported");
    }

  if (mc.haveSpeedStatePars)
    {
      NS_FATAL_ERROR ("SpeedStatePars are currently not supported");
    }
}

void 
LteUeRrc::SendMeasurementReport (uint8_t measId)
{
  NS_LOG_FUNCTION (this << (uint32_t) measId);
  //  3GPP TS 36.331 section 5.5.5 Measurement reporting

  std::map<uint8_t, LteRrcSap::MeasIdToAddMod>::iterator 
    measIdIt = m_varMeasConfig.measIdList.find (measId);
  NS_ASSERT (measIdIt != m_varMeasConfig.measIdList.end ());
  
  std::map<uint8_t, LteRrcSap::ReportConfigToAddMod>::iterator 
    reportConfigIt = m_varMeasConfig.reportConfigList.find (measIdIt->second.reportConfigId);
  NS_ASSERT (reportConfigIt != m_varMeasConfig.reportConfigList.end ());
  LteRrcSap::ReportConfigEutra& reportConfigEutra = reportConfigIt->second.reportConfigEutra;

  LteRrcSap::MeasurementReport measurementReport;
  LteRrcSap::MeasResults& measResults = measurementReport.measResults;
  measResults.measId = measId;
  
  std::map<uint16_t, MeasValues>::iterator servingMeasIt = m_storedMeasValues.find (m_cellId);
  NS_ASSERT (servingMeasIt != m_storedMeasValues.end ());
  measResults.rsrpResult = EutranMeasurementMapping::Dbm2RsrpRange (servingMeasIt->second.rsrp);
  measResults.rsrqResult = EutranMeasurementMapping::Db2RsrqRange (servingMeasIt->second.rsrq);
  NS_LOG_INFO (this << " reporting serving cell "
               "RSRP " << (uint32_t) measResults.rsrpResult << " (" << servingMeasIt->second.rsrp << " dBm) "
               "RSRQ " << (uint32_t) measResults.rsrqResult << " (" << servingMeasIt->second.rsrq << " dB)");
  measResults.haveMeasResultNeighCells = false;
  std::map<uint8_t, VarMeasReport>::iterator measReportIt = m_varMeasReportList.find (measId);
  if (measReportIt == m_varMeasReportList.end ())
    {
      NS_LOG_ERROR ("no entry found in m_varMeasReportList for measId " << (uint32_t) measId);
    }
  else
    {
      if (!(measReportIt->second.cellsTriggeredList.empty ()))
        {
          measResults.haveMeasResultNeighCells = true;
          
          std::multimap<double, uint16_t> sortedNeighCells;         
          for (std::set<uint16_t>::iterator cellsTriggeredIt = measReportIt->second.cellsTriggeredList.begin ();
               cellsTriggeredIt != measReportIt->second.cellsTriggeredList.end ();
               ++cellsTriggeredIt)
            {
              uint16_t cellId = *cellsTriggeredIt;
              if (cellId != m_cellId)
                {
                  std::map<uint16_t, MeasValues>::iterator neighborMeasIt = m_storedMeasValues.find (cellId);
                  double triggerValue;             
                  switch (reportConfigEutra.triggerQuantity)
                    {
                    case LteRrcSap::ReportConfigEutra::RSRP:
                      triggerValue = neighborMeasIt->second.rsrp;
                      break;
                    case LteRrcSap::ReportConfigEutra::RSRQ:
                      triggerValue = neighborMeasIt->second.rsrq;
                      break;
                    default:
                      NS_FATAL_ERROR ("unsupported triggerQuantity");
                      break;
                    }  
                  sortedNeighCells.insert (std::pair<double, uint16_t> (triggerValue, cellId));
                }
            }
          
          std::multimap<double, uint16_t>::reverse_iterator sortedNeighCellsIt;
          uint32_t count;
          for (sortedNeighCellsIt = sortedNeighCells.rbegin (), count = 0;
               sortedNeighCellsIt != sortedNeighCells.rend () && count < reportConfigEutra.maxReportCells;
               ++sortedNeighCellsIt, ++count)
            {
              uint16_t cellId = sortedNeighCellsIt->second;
              std::map<uint16_t, MeasValues>::iterator neighborMeasIt = m_storedMeasValues.find (cellId);
              NS_ASSERT (neighborMeasIt != m_storedMeasValues.end ());            
              LteRrcSap::MeasResultEutra measResultEutra;
              measResultEutra.physCellId = cellId;
              measResultEutra.haveCgiInfo = false;
              measResultEutra.haveRsrpResult = true;
              measResultEutra.rsrpResult = EutranMeasurementMapping::Dbm2RsrpRange (neighborMeasIt->second.rsrp);
              measResultEutra.haveRsrqResult = true;
              measResultEutra.rsrqResult = EutranMeasurementMapping::Db2RsrqRange (neighborMeasIt->second.rsrq);           
              NS_LOG_INFO (this << " reporting neighbor cell " << (uint32_t) measResultEutra.physCellId 
                           << " RSRP " << (uint32_t) measResultEutra.rsrpResult 
                           << " (" << neighborMeasIt->second.rsrp << " dBm) "
                           << " RSRQ " << (uint32_t) measResultEutra.rsrqResult 
                           << " (" << neighborMeasIt->second.rsrq << " dB)"); 
              measResults.measResultListEutra.push_back (measResultEutra);
            }
        }
      else
        {
          NS_LOG_WARN (this << " cellsTriggeredList is empty");
        }
      measReportIt->second.numberOfReportsSent++;
      measReportIt->second.periodicReportTimer.Cancel ();
            
      // the current LteRrcSap implementation is broken in that it does not allow for infinite values
      // which is probably the most reasonable setting. So we just assume infinite reportAmount
      // if (measReportIt->numberOfReportsSent < reportConfigEutra.reportAmount)
      uint32_t intervalMs;
      switch (reportConfigEutra.reportInterval)
        {
        case LteRrcSap::ReportConfigEutra::MS480:
          intervalMs = 480;
          break;
          
        default:
          NS_FATAL_ERROR ("unsupported reportInterval");
          break;          
        }
      measReportIt->second.periodicReportTimer 
        = Simulator::Schedule (MilliSeconds (intervalMs), 
                               &LteUeRrc::SendMeasurementReport,
                               this,
                               measId);

      m_rrcSapUser->SendMeasurementReport (measurementReport);
    } 
}

void 
LteUeRrc::StartConnection ()
{
  NS_LOG_FUNCTION (this);
  m_connectionPending = false;
  SwitchToState (IDLE_RANDOM_ACCESS);        
  m_cmacSapProvider->StartContentionBasedRandomAccessProcedure ();
}

void 
LteUeRrc::LeaveConnectedMode ()
{
  NS_LOG_FUNCTION (this << m_imsi);
  m_asSapUser->NotifyConnectionReleased ();
  m_cmacSapProvider->RemoveLc (1);
  std::map<uint8_t, Ptr<LteDataRadioBearerInfo> >::iterator it;
  for (it = m_drbMap.begin (); it != m_drbMap.end (); ++it)
    {
      m_cmacSapProvider->RemoveLc (it->second->m_logicalChannelIdentity);
    }
  m_drbMap.clear ();
  m_bid2DrbidMap.clear ();
  m_srb1 = 0;
  SwitchToState (IDLE_CAMPED_NORMALLY);
}

void
LteUeRrc::DisposeOldSrb1 ()
{
  NS_LOG_FUNCTION (this);
  m_srb1Old = 0;
}

uint8_t 
LteUeRrc::Bid2Drbid (uint8_t bid)
{
  std::map<uint8_t, uint8_t>::iterator it = m_bid2DrbidMap.find (bid);
  NS_ASSERT_MSG (it != m_bid2DrbidMap.end (), "could not find BID " << bid);
  return it->second;
}


void 
LteUeRrc::SwitchToState (State newState)
{
  NS_LOG_FUNCTION (this << newState);
  State oldState = m_state;
  m_state = newState;
  NS_LOG_INFO ("IMSI " << m_imsi << " RNTI " << m_rnti << " UeRrc " << ToString (oldState) << " --> " << ToString (newState));
  m_stateTransitionTrace (m_imsi, m_cellId, m_rnti, oldState, newState);

  switch (newState)
    {
    case IDLE_CELL_SELECTION:
      break;

    case IDLE_WAIT_SYSTEM_INFO:
      break;

    case IDLE_CAMPED_NORMALLY:
      if (m_connectionPending)
        {
          StartConnection ();
        }
      break;

    case IDLE_RANDOM_ACCESS:
      break;

    case IDLE_CONNECTING:
      break;
 
    case CONNECTED_NORMALLY:
      break;

    case CONNECTED_REESTABLISHING:
      break;

    case CONNECTED_HANDOVER:
      break;
 
    default:
      break;
    }
}
  

    
  
} // namespace ns3

