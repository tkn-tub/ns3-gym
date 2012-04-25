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
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>
 *         Nicola Baldo  <nbaldo@cttc.es>
 */

#ifndef LTE_ENB_MAC_H
#define LTE_ENB_MAC_H


#include <map>
#include <vector>
#include <ns3/lte-common.h>
#include <ns3/lte-mac-sap.h>
#include <ns3/lte-enb-cmac-sap.h>
#include <ns3/ff-mac-csched-sap.h>
#include <ns3/ff-mac-sched-sap.h>
#include <ns3/lte-enb-phy-sap.h>
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

class DlCqiIdealControlMessage;
class UlCqiIdealControlMessage;
class PdcchMapIdealControlMessage;



/**
 * This class implements the MAC layer of the eNodeB device
 */
class LteEnbMac :   public Object
{
  friend class EnbMacMemberLteEnbCmacSapProvider;
  friend class EnbMacMemberLteMacSapProvider<LteEnbMac>;
  friend class EnbMacMemberFfMacSchedSapUser;
  friend class EnbMacMemberFfMacCschedSapUser;
  friend class EnbMacMemberLteEnbPhySapUser;

public:
  static TypeId GetTypeId (void);

  LteEnbMac (void);
  virtual ~LteEnbMac (void);
  virtual void DoDispose (void);


  /**
   * \brief Set the scheduler SAP provider
   * \param s a pointer SAP provider of the FF packet scheduler
   */
  void SetFfMacSchedSapProvider (FfMacSchedSapProvider* s);
  /**
   * \brief Get the scheduler SAP user
   * \return a pointer to the SAP user of the scheduler
   */
  FfMacSchedSapUser* GetFfMacSchedSapUser (void);
  /**
   * \brief Set the control scheduler SAP provider
   * \param s a pointer to the control scheduler SAP provider
   */
  void SetFfMacCschedSapProvider (FfMacCschedSapProvider* s);
  /**
   * \brief Get the control scheduler SAP user
   * \return a pointer to the control scheduler SAP user
   */
  FfMacCschedSapUser* GetFfMacCschedSapUser (void);



  /**
   * \brief Set the MAC SAP user
   * \param s a pointer to the MAC SAP user
   */
  void SetLteMacSapUser (LteMacSapUser* s);
  /**
   * \brief Get the MAC SAP provider
   * \return a pointer to the SAP provider of the MAC
   */
  LteMacSapProvider* GetLteMacSapProvider (void);
  /**
   * \brief Set the control MAC SAP user
   * \param s a pointer to the control MAC SAP user
   */
  void SetLteEnbCmacSapUser (LteEnbCmacSapUser* s);
  /**
   * \brief Get the control MAC SAP provider
   * \return a pointer to the control MAC SAP provider
   */
  LteEnbCmacSapProvider* GetLteEnbCmacSapProvider (void);


  /**
  * \brief Get the eNB-PHY SAP User
  * \return a pointer to the SAP User of the PHY
  */
  LteEnbPhySapUser* GetLteEnbPhySapUser ();

  /**
  * \brief Set the PHY SAP Provider
  * \param s a pointer to the PHY SAP provider
  */
  void SetLteEnbPhySapProvider (LteEnbPhySapProvider* s);


  /**
  * \brief Receive a DL CQI ideal control message
  * \param msg the DL CQI message
  */
  void ReceiveDlCqiIdealControlMessage  (Ptr<DlCqiIdealControlMessage> msg);

  void DoReceiveIdealControlMessage (Ptr<IdealControlMessage> msg);

  /**
  * \brief Receive a CE element containing the buffer status report
  * \param msg the BSR message
  */
  void ReceiveBsrMessage  (MacCeListElement_s bsr);

  void DoUlCqiReport (UlCqi_s ulcqi);



private:
  // forwarded from LteEnbCmacSapProvider
  void DoConfigureMac (uint8_t ulBandwidth, uint8_t dlBandwidth);
  void DoAddUe (uint16_t rnti);
  void DoAddLc (LteEnbCmacSapProvider::LcInfo lcinfo, LteMacSapUser* msu);
  void DoReconfigureLc (LteEnbCmacSapProvider::LcInfo lcinfo);
  void DoReleaseLc (uint16_t  rnti, uint8_t lcid);

  // forwarded from LteMacSapProvider
  void DoTransmitPdu (LteMacSapProvider::TransmitPduParameters);
  void DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters);


  // forwarded from FfMacCchedSapUser
  void DoCschedCellConfigCnf (FfMacCschedSapUser::CschedCellConfigCnfParameters params);
  void DoCschedUeConfigCnf (FfMacCschedSapUser::CschedUeConfigCnfParameters params);
  void DoCschedLcConfigCnf (FfMacCschedSapUser::CschedLcConfigCnfParameters params);
  void DoCschedLcReleaseCnf (FfMacCschedSapUser::CschedLcReleaseCnfParameters params);
  void DoCschedUeReleaseCnf (FfMacCschedSapUser::CschedUeReleaseCnfParameters params);
  void DoCschedUeConfigUpdateInd (FfMacCschedSapUser::CschedUeConfigUpdateIndParameters params);
  void DoCschedCellConfigUpdateInd (FfMacCschedSapUser::CschedCellConfigUpdateIndParameters params);

  // forwarded from FfMacSchedSapUser
  void DoSchedDlConfigInd (FfMacSchedSapUser::SchedDlConfigIndParameters ind);
  void DoSchedUlConfigInd (FfMacSchedSapUser::SchedUlConfigIndParameters params);
  
  void DoRrcUpdateConfigurationReq (FfMacCschedSapProvider::CschedUeConfigReqParameters params);

  /**
  * \brief Forwarded from LteEnbPhySapUser: trigger the start from a new frame
  *
  * \param frameNo frame number
  * \param subframeNo subframe number
  */
  void DoSubframeIndication (uint32_t frameNo, uint32_t subframeNo);

  /**
  * \brief Forwarded from LtePhySapUser: receive a PHY-PDU
  *
  * \param p PHY-PDU received
  */
public:
  // legacy public for use the Phy callback
  void DoReceivePhyPdu (Ptr<Packet> p);

private:
private:
  // std::map <uint16_t, std::map <uint8_t,Ptr<LteMacSapUser> > > m_rlcAttached;
  std::map <LteFlowId_t, LteMacSapUser*> m_rlcAttached;

  std::vector <CqiListElement_s> m_dlCqiReceived; // DL-CQI received
  std::vector <UlCqi_s> m_ulCqiReceived; // UL-CQI received
  std::vector <MacCeListElement_s> m_ulCeReceived; // CE received (BSR up to now)


  /*
  * Map of UE's info element (see 4.3.12 of FF MAC Scheduler API)
  */
  std::map <uint16_t,UlInfoListElement_s> m_ulInfoListElements; 



  LteMacSapProvider* m_macSapProvider;
  LteEnbCmacSapProvider* m_cmacSapProvider;
  LteMacSapUser* m_macSapUser;
  LteEnbCmacSapUser* m_cmacSapUser;


  FfMacSchedSapProvider* m_schedSapProvider;
  FfMacCschedSapProvider* m_cschedSapProvider;
  FfMacSchedSapUser* m_schedSapUser;
  FfMacCschedSapUser* m_cschedSapUser;

  // PHY-SAP
  LteEnbPhySapProvider* m_enbPhySapProvider;
  LteEnbPhySapUser* m_enbPhySapUser;

  uint32_t m_frameNo;
  uint32_t m_subframeNo;
  /**
   * Trace information regarding DL scheduling
   * Frame number, Subframe number, RNTI, MCS of TB1, size of TB1,
   * MCS of TB2 (0 if not present), size of TB2 (0 if not present)
   */
  TracedCallback<uint32_t, uint32_t, uint16_t, uint8_t, uint16_t, uint8_t, uint16_t> m_dlScheduling;
  /**
   * Trace information regarding UL scheduling
   * Frame number, Subframe number, RNTI, MCS of TB, size of TB
   */
  TracedCallback<uint32_t, uint32_t, uint16_t, uint8_t, uint16_t> m_ulScheduling;
  
  uint8_t m_macChTtiDelay; // delay of MAC, PHY and channel in terms of TTIs
  

};

} // end namespace ns3

#endif /* LTE_ENB_MAC_ENTITY_H */
