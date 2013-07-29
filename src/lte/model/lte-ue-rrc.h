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

#ifndef LTE_UE_RRC_H
#define LTE_UE_RRC_H

#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/lte-ue-cmac-sap.h>
#include <ns3/lte-pdcp-sap.h>
#include <ns3/lte-as-sap.h>
#include <ns3/lte-ue-cphy-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/traced-callback.h>

#include <map>
#include <set>

namespace ns3 {


/**
 * \brief Artificial delay of UE measurements procedure
 *
 * i.e. the period between the time layer-1-filtered measurements from PHY
 * layer is received and the earliest time the actual measurement report
 * submission to the serving cell is invoked.
 *
 * This delay exists because of racing condition between several UE measurements
 * functions which happen to be scheduled at the same time. The delay ensures
 * that:
 *  - measurements (e.g. layer-3-filtering) is always performed before
 *    reporting, thus the latter always uses the newest layer-1-filtered
 *    measurements from PHY; and
 *  - time-to-trigger is always performed just before the reporting, so there
 *    would still be chance to cancel the reporting if necessary.
 */
static const Time UE_MEASUREMENTS_DELAY = MicroSeconds (1);


class LteRlc;
class LteMacSapProvider;
class LteUeCmacSapUser;
class LteUeCmacSapProvider;
class LteDataRadioBearerInfo;
class LteSignalingRadioBearerInfo;
class LteEnbRrc;

/**
 *
 *
 */
class LteUeRrc : public Object
{

  friend class UeMemberLteUeCmacSapUser;
  friend class UeRrcMemberLteEnbCmacSapUser;
  friend class LtePdcpSpecificLtePdcpSapUser<LteUeRrc>;
  friend class MemberLteAsSapProvider<LteUeRrc>;
  friend class MemberLteUeCphySapUser<LteUeRrc>;
  friend class MemberLteUeRrcSapProvider<LteUeRrc>;

public:

  /**
   * The states of the UE RRC entity
   * 
   */
  enum State
    {
      IDLE_CELL_SELECTION = 0,
      IDLE_WAIT_SYSTEM_INFO,
      IDLE_CAMPED_NORMALLY,
      IDLE_RANDOM_ACCESS,
      IDLE_CONNECTING,
      CONNECTED_NORMALLY,
      CONNECTED_REESTABLISHING,
      CONNECTED_HANDOVER,
      NUM_STATES
    };


  /**
   * create an RRC instance for use within an ue
   *
   */
  LteUeRrc ();


  /**
   * Destructor
   */
  virtual ~LteUeRrc ();


  // inherited from Object
private:
  virtual void DoInitialize (void);
  virtual void DoDispose (void);
public:
  static TypeId GetTypeId (void);


  /**
   * set the CPHY SAP this RRC should use to interact with the PHY
   *
   * \param s the CPHY SAP Provider
   */
  void SetLteUeCphySapProvider (LteUeCphySapProvider * s);

  /**
   *
   *
   * \return s the CPHY SAP User interface offered to the PHY by this RRC
   */
  LteUeCphySapUser* GetLteUeCphySapUser ();

  /**
   * set the CMAC SAP this RRC should interact with
   *
   * \param s the CMAC SAP Provider to be used by this RRC
   */
  void SetLteUeCmacSapProvider (LteUeCmacSapProvider * s);

  /**
   *
   *
   * \return s the CMAC SAP User interface offered to the MAC by this RRC
   */
  LteUeCmacSapUser* GetLteUeCmacSapUser ();


  /**
   * set the RRC SAP this RRC should interact with
   *
   * \param s the RRC SAP User to be used by this RRC
   */
  void SetLteUeRrcSapUser (LteUeRrcSapUser * s);

  /**
   *
   *
   * \return s the RRC SAP Provider interface offered to the MAC by this RRC
   */
  LteUeRrcSapProvider* GetLteUeRrcSapProvider ();

  /**
   * set the MAC SAP provider. The ue RRC does not use this
   * directly, but it needs to provide it to newly created RLC instances.
   *
   * \param s the MAC SAP provider that will be used by all
   * newly created RLC instances
   */
  void SetLteMacSapProvider (LteMacSapProvider* s);

  /** 
   * Set the AS SAP user to interact with the NAS entity
   * 
   * \param s the AS SAP user
   */
  void SetAsSapUser (LteAsSapUser* s);

  /** 
   * 
   * 
   * \return the AS SAP provider exported by this RRC
   */
  LteAsSapProvider* GetAsSapProvider ();

  /** 
   * 
   * \param imsi the unique UE identifier
   */
  void SetImsi (uint64_t imsi);

  /**
   *
   * \return imsi the unique UE identifier
   */
  uint64_t GetImsi (void);


  /**
   *
   * \return the C-RNTI of the user
   */
  uint16_t GetRnti () const;


  /**
   *
   * \return the CellId of the attached Enb
   */
  uint16_t GetCellId () const;


  /** 
   * \return the uplink bandwidth in RBs
   */
  uint8_t GetUlBandwidth () const;

  /** 
   * \return the downlink bandwidth in RBs
   */
  uint8_t GetDlBandwidth () const;

  /**
   * \return the downlink carrier frequency (EARFCN)
   */
  uint16_t GetDlEarfcn () const;

  /**
   * \param bw the downlink carrier frequency (EARFCN)
   */
  void SetDlEarfcn (uint16_t earfcn);

  /** 
   * \return the uplink carrier frequency (EARFCN)
   */
  uint16_t GetUlEarfcn () const;

  /**
   * \param bw the uplink carrier frequency (EARFCN)
   */
  void SetUlEarfcn (uint16_t earfcn);

  /**
   *
   * \return the current state
   */
  State GetState ();

  /** 
   * 
   * 
   * \param val true if RLC SM is to be used, false if RLC UM/AM are to be used
   */
  void SetUseRlcSm (bool val);


private:


  // PDCP SAP methods
  void DoReceivePdcpSdu (LtePdcpSapUser::ReceivePdcpSduParameters params);

  // CMAC SAP methods
  void DoSetTemporaryCellRnti (uint16_t rnti);
  void DoNotifyRandomAccessSuccessful ();
  void DoNotifyRandomAccessFailed ();
 
  // LTE AS SAP methods
  void DoSetCsgWhiteList (uint32_t csgId);
  void DoForceCampedOnEnb (uint16_t cellId, uint16_t earfcn);
  void DoConnect ();
  void DoSendData (Ptr<Packet> packet, uint8_t bid);
  void DoDisconnect ();

  // CPHY SAP methods
  void DoRecvMasterInformationBlock (uint16_t cellId,
                                     LteRrcSap::MasterInformationBlock msg);
  void DoRecvSystemInformationBlockType1 (uint16_t cellId,
                                          LteRrcSap::SystemInformationBlockType1 msg);
  void DoReportUeMeasurements (LteUeCphySapUser::UeMeasurementsParameters params);

  // RRC SAP methods
  void DoCompleteSetup (LteUeRrcSapProvider::CompleteSetupParameters params);
  void DoRecvSystemInformation (LteRrcSap::SystemInformation msg);
  void DoRecvRrcConnectionSetup (LteRrcSap::RrcConnectionSetup msg);
  void DoRecvRrcConnectionReconfiguration (LteRrcSap::RrcConnectionReconfiguration msg);
  void DoRecvRrcConnectionReestablishment (LteRrcSap::RrcConnectionReestablishment msg);
  void DoRecvRrcConnectionReestablishmentReject (LteRrcSap::RrcConnectionReestablishmentReject msg);
  void DoRecvRrcConnectionRelease (LteRrcSap::RrcConnectionRelease msg);
  void DoRecvRrcConnectionReject (LteRrcSap::RrcConnectionReject msg);

 
  // internal methods
  void SaveUeMeasurements (uint16_t cellId, double rsrp, double rsrq,
                           bool useLayer3Filtering);
  void SynchronizeToStrongestCell ();
  void EvaluateCellForSelection ();
  void ApplyRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated rrcd);
  /// 3GPP TS 36.331 section 5.5.2 Measurement configuration
  void ApplyMeasConfig (LteRrcSap::MeasConfig mc);
  ///  3GPP TS 36.331 section 5.5.3.2 Layer 3 filtering
  void Layer3Filtering (uint16_t cellId, double rsrp, double rsrq);
  /// 3GPP TS 36.331 section 5.5.4.1 Measurement report triggering - General
  void MeasurementReportTriggering (uint8_t measId);
  /// 3GPP TS 36.331 section 5.5.5 Measurement reporting
  void SendMeasurementReport (uint8_t measId);
  void StartConnection ();
  void LeaveConnectedMode ();
  void DisposeOldSrb1 ();
  uint8_t Bid2Drbid (uint8_t bid);
  void SwitchToState (State s);

  std::map<uint8_t, uint8_t> m_bid2DrbidMap;

  LteUeCphySapUser* m_cphySapUser;
  LteUeCphySapProvider* m_cphySapProvider;

  LteUeCmacSapUser* m_cmacSapUser;
  LteUeCmacSapProvider* m_cmacSapProvider;

  LteUeRrcSapUser* m_rrcSapUser;
  LteUeRrcSapProvider* m_rrcSapProvider;

  LteMacSapProvider* m_macSapProvider;
  LtePdcpSapUser* m_drbPdcpSapUser;

  LteAsSapProvider* m_asSapProvider;
  LteAsSapUser* m_asSapUser;

  State m_state;

  uint64_t m_imsi;
  uint16_t m_rnti;
  uint16_t m_cellId;

  Ptr<LteSignalingRadioBearerInfo> m_srb0;
  Ptr<LteSignalingRadioBearerInfo> m_srb1;
  Ptr<LteSignalingRadioBearerInfo> m_srb1Old;
  std::map <uint8_t, Ptr<LteDataRadioBearerInfo> > m_drbMap;

  bool m_useRlcSm;

  uint8_t m_lastRrcTransactionIdentifier;

  uint8_t m_dlBandwidth; /**< downlink bandwidth in RBs */
  uint8_t m_ulBandwidth; /**< uplink bandwidth in RBs */

  uint16_t m_dlEarfcn;  /**< downlink carrier frequency */
  uint16_t m_ulEarfcn;  /**< uplink carrier frequency */

  //             imsi      cellId    rnti,     sourceCellId
  TracedCallback<uint64_t, uint16_t, uint16_t, uint16_t> m_mibReceivedTrace;
  //             imsi      cellId    rnti,     sourceCellId
  TracedCallback<uint64_t, uint16_t, uint16_t, uint16_t> m_sib1ReceivedTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_sib2ReceivedTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t, State, State> m_stateTransitionTrace;
  //             imsi      cellId
  TracedCallback<uint64_t, uint16_t> m_initialCellSelectionEndOkTrace;
  //             imsi      cellId
  TracedCallback<uint64_t, uint16_t> m_initialCellSelectionEndErrorTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_randomAccessSuccessfulTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionEstablishedTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionReconfigurationTrace;
  //             imsi      cellId    rnti      targetCellId
  TracedCallback<uint64_t, uint16_t, uint16_t, uint16_t> m_handoverStartTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_handoverEndOkTrace;

  bool m_connectionPending; /**< true if a connection request by upper layers is pending */
  bool m_hasReceivedMib; /**< true if MIB was received for the current cell  */
  bool m_hasReceivedSib1; /**< true if SIB1 was received for the current cell  */
  bool m_hasReceivedSib2; /**< true if SIB2 was received for the current cell  */

  /// Stored content of the last SIB1 received.
  LteRrcSap::SystemInformationBlockType1 m_lastSib1;

  /// List of cell ID of acceptable cells for cell selection that have been detected.
  std::set<uint16_t> m_acceptableCell;

  uint32_t m_csgWhiteList;

  /**
   * \brief Includes the accumulated configuration of the measurements to be
   *        performed by the UE.
   *
   * Based on 3GPP TS 36.331 section 7.1. Also note that some optional variables
   * in the specification are omitted.
   */
  struct VarMeasConfig
  {
    std::map<uint8_t, LteRrcSap::MeasIdToAddMod>   measIdList;
    std::map<uint8_t, LteRrcSap::MeasObjectToAddMod> measObjectList;
    std::map<uint8_t, LteRrcSap::ReportConfigToAddMod> reportConfigList;
    LteRrcSap::QuantityConfig quantityConfig; 
    double aRsrp;
    double aRsrq;
  };

  VarMeasConfig m_varMeasConfig;

  /**
   * \brief Includes information about a measurement for which the triggering
   *        conditions have been met.
   *
   * Based on 3GPP TS 36.331 section 7.1.
   */
  struct VarMeasReport
  {
    uint8_t measId;
    std::set<uint16_t> cellsTriggeredList; // note: only E-UTRA is supported.
    uint32_t numberOfReportsSent;
    EventId periodicReportTimer;
  };

  //       measId
  std::map<uint8_t, VarMeasReport> m_varMeasReportList;

  typedef std::list<uint16_t> ConcernedCells_t;

  void VarMeasReportListAdd (uint8_t measId, ConcernedCells_t enteringCells);
  void VarMeasReportListErase (uint8_t measId, ConcernedCells_t leavingCells,
                               bool reportOnLeave);

  struct MeasValues
  {
    double rsrp;
    double rsrq;
    Time timestamp;
  };

  /////////cellId
  std::map<uint16_t, MeasValues> m_storedMeasValues;

  struct PendingTrigger_t
  {
    uint8_t measId;
    ConcernedCells_t concernedCells;
    EventId timer;
  };

  //       measId
  std::map<uint8_t, std::list<PendingTrigger_t> > m_enteringTriggerQueue;
  //       measId
  std::map<uint8_t, std::list<PendingTrigger_t> > m_leavingTriggerQueue;

  void CancelEnteringTrigger (uint8_t measId);
  void CancelEnteringTrigger (uint8_t measId, uint16_t cellId);
  void CancelLeavingTrigger (uint8_t measId);
  void CancelLeavingTrigger (uint8_t measId, uint16_t cellId);

}; // end of class LteUeRrc


} // namespace ns3

#endif // LTE_UE_RRC_H
