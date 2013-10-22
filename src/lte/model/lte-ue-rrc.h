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
 *         Budiarto Herman <budiarto.herman@magister.fi>
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
 * \brief Artificial delay of UE measurements procedure.
 *
 * i.e. the period between the time layer-1-filtered measurements from PHY
 * layer is received and the earliest time the actual measurement report
 * submission to the serving cell is invoked.
 *
 * This delay exists because of racing condition between several UE measurements
 * functions which happen to be scheduled at the same time. The delay ensures
 * that:
 *  - measurements (e.g., layer-3 filtering) are always performed before
 *    reporting, thus the latter always use the latest measured RSRP and RSRQ;
 *    and
 *  - time-to-trigger check is always performed before the reporting, so there
 *    would still be chance for it to cancel the reporting if necessary.
 */
static const Time UE_MEASUREMENT_REPORT_DELAY = MicroSeconds (1);


class LteRlc;
class LteMacSapProvider;
class LteUeCmacSapUser;
class LteUeCmacSapProvider;
class LteDataRadioBearerInfo;
class LteSignalingRadioBearerInfo;

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
    IDLE_START = 0,
    IDLE_CELL_SEARCH,
    IDLE_WAIT_MIB_SIB1,
    IDLE_WAIT_MIB,
    IDLE_WAIT_SIB1,
    IDLE_CAMPED_NORMALLY,
    IDLE_WAIT_SIB2,
    IDLE_RANDOM_ACCESS,
    IDLE_CONNECTING,
    CONNECTED_NORMALLY,
    CONNECTED_HANDOVER,
    CONNECTED_PHY_PROBLEM,
    CONNECTED_REESTABLISHING,
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
  uint64_t GetImsi (void) const;

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
   * \return the uplink carrier frequency (EARFCN)
   */
  uint16_t GetUlEarfcn () const;

  /**
   *
   * \return the current state
   */
  State GetState () const;

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
  void DoForceCampedOnEnb (uint16_t cellId, uint16_t dlEarfcn);
  void DoStartCellSelection (uint16_t dlEarfcn);
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

 
  // INTERNAL METHODS

  /**
   * \brief Go through the list of measurement results, choose the one with the
   *        strongest RSRP, and tell PHY to synchronize to it.
   *
   * \warning This function is a part of the *initial cell selection* procedure,
   *          hence must be only executed during IDLE mode.
   */
  void SynchronizeToStrongestCell ();

  /**
   * \brief Performs cell selection evaluation to the current serving cell.
   *
   * \warning This function is a part of the *initial cell selection* procedure,
   *          hence must be only executed during IDLE mode and specifically
   *          during the state when the UE just received the first SIB1 message
   *          from the serving cell.
   *
   * This function assumes that the required information for the evaluation
   * procedure have been readily gathered, such as *measurement results*, MIB,
   * and SIB1. Please refer to the LTE module's Design Documentation for more
   * details on the evaluation process.
   *
   * If the cell passes the evaluation, the UE will immediately camp to it.
   * Otherwise, the UE will pick another cell and restart the cell selection
   * procedure.
   */
  void EvaluateCellForSelection ();

  /**
   * \brief Update the current measurement configuration #m_varMeasConfig.
   * \param mc measurements to be performed by the UE
   *
   * Implements Section 5.5.2 "Measurement configuration" of 3GPP TS 36.331.
   * The supported subfunctions are:
   * - Measurement object removal
   * - Measurement object addition/ modification
   * - Reporting configuration removal
   * - Reporting configuration addition/ modification
   * - Quantity configuration
   * - Measurement identity removal
   * - Measurement identity addition/ modification
   *
   * The subfunctions that will be invoked are determined by the content of
   * the given measurement configuration.
   *
   * Note the existence of some chain reaction behaviours:
   * - Removal of measurement object or reporting configuration also removes any
   *   impacted measurement identities.
   * - Removal of measurement identity also removes any associated *reporting
   *   entry* from #m_varMeasReportList.
   * - Modification to measurement object or reporting configuration also
   *   removes any reporting entries of the impacted measurement identities
   *   from #m_varMeasReportList.
   * - Modification to quantity configuration also removes all existing
   *   reporting entries from #m_varMeasReportList, regardless of measurement
   *   identity.
   *
   * Some unsupported features:
   * - List of neighbouring cells
   * - List of black cells
   * - CGI reporting
   * - Periodical reporting configuration
   * - Measurement gaps
   * - s-Measure
   * - Speed-dependent scaling
   *
   * \warning There is a possibility that the input argument (of type
   *          LteRrcSap::MeasConfig) may contain information in fields related
   *          to the unsupported features. In such case, the function will raise
   *          an error.
   *
   * The measurement configuration given as an argument is typically provided by
   * the serving eNodeB. It is transmitted through the RRC protocol when the UE
   * joins the cell, e.g., by connection establishment or by incoming handover.
   * The information inside the argument can be configured from the eNodeB side,
   * which would then equally apply to all other UEs attached to the same
   * eNodeB. See the LTE module's User Documentation for more information on
   * configuring this.
   *
   * \sa LteRrcSap::MeasConfig, LteUeRrc::m_varMeasReportList
   */
  void ApplyMeasConfig (LteRrcSap::MeasConfig mc);

  /**
   * \brief Keep the given measurement result as the latest measurement figures,
   *        to be utilised by UE RRC functions.
   * \param cellId the cell ID of the measured cell
   * \param rsrp measured RSRP value to be saved (in dBm)
   * \param rsrq measured RSRQ value to be saved (in dB)
   * \param useLayer3Filtering
   * \todo Remove the useLayer3Filtering argument
   *
   * Implements Section 5.5.3.2 "Layer 3 filtering" of 3GPP TS 36.331. *Layer-3
   * filtering* is applied to the given measurement results before saved to
   * #m_storedMeasValues. The filtering is however disabled when the UE is in
   * IDLE mode, i.e., saving unfiltered values.
   *
   * Layer-3 filtering is influenced by a filter coefficient, which determines
   * the strength of the filtering. This coefficient is provided by the active
   * *quantity configuration* in #m_varMeasConfig, which is configured by the
   * LteUeRrc::ApplyMeasConfig. Details on how the coefficient works and how to
   * modify it can be found in LTE module's Design Documentation.
   *
   * \sa LteUeRrc::m_storedMeasValues
   */
  void SaveUeMeasurements (uint16_t cellId, double rsrp, double rsrq,
                           bool useLayer3Filtering);

  /**
   * \brief Evaluate the reporting criteria of a measurement identity and
   *        invoke some reporting actions based on the result.
   * \param measId the measurement identity to be evaluated
   *
   * Implements Section 5.5.4.1 "Measurement report triggering - General" of
   * 3GPP TS 36.331. This function take into use the latest measurement results
   * and evaluate them against the *entering condition* and the *leaving
   * condition* of the measurement identity's reporting criteria. The evaluation
   * also take into account other defined criteria, such as *hysteresis* and
   * *time-to-trigger*.
   *
   * The entering and leaving condition to be evaluated are determined by the
   * *event type* of the measurement identity's reporting criteria. As defined
   * in LteRrcSap::ReportConfigEutra, there 5 supported events. The gore details
   * of these events can be found in Section 5.5.4 of 3GPP TS 36.331.
   *
   * An applicable entering condition (i.e., the condition evaluates to true)
   * will insert a new *reporting entry* to #m_varMeasReportList, so
   * *measurement reports* would be produced and submitted to eNodeB. On the
   * other hand, an applicable leaving condition will remove the related
   * reporting entry from #m_varMeasReportList, so submission of related
   * measurement reports to eNodeB will be suspended.
   */
  void MeasurementReportTriggering (uint8_t measId);

  /**
   * \brief Produce a proper measurement report from the given measurement
   *        identity's reporting entry in #m_varMeasReportList and then submit
   *        it to the serving eNodeB.
   * \param measId the measurement identity which report is to be submitted.
   *
   * Implements Section 5.5.5 "Measurement reporting" of 3GPP TS 36.331.
   * Producing a *measurement report* involves several tasks such as:
   * - including the measurement results of the serving cell into the report;
   * - selecting some neighbour cells which triggered the reporting (i.e., those
   *   in *cellsTriggeredList*) to be included in the report;
   * - sorting the order of neighbour cells in the report by their RSRP or RSRQ
   *   measurement results (the highest comes first); and
   * - ensuring the number of neighbour cells in the report is under the
   *   *maxReportCells* limit defined by the measurement identity's reporting
   *   configuration.
   *
   * The RSRP and RSRQ measurement results included in the report are expressed
   * in 3GPP-specified range format. They are converted from dBm and dB units
   * using EutranMeasurementMapping::Dbm2RsrpRange and
   * EutranMeasurementMapping::Db2RsrqRange functions.
   *
   * Measurement report is submitted to the serving eNodeB through the *RRC
   * protocol*. The LteUeRrcSapUser::SendMeasurementReport method of the *UE RRC
   * SAP* facilitates this submission.
   *
   * After the submission, the function will repeat itself after a certain
   * interval. The interval length may vary from 120 ms to 60 minutes and is
   * determined by the *report interval* parameter specified by the measurement
   * identity's reporting configuration.
   */
  void SendMeasurementReport (uint8_t measId);

  void ApplyRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated rrcd);
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
  TracedCallback<uint64_t, uint16_t, uint16_t> m_randomAccessErrorTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionEstablishedTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionReconfigurationTrace;
  //             imsi      cellId    rnti      targetCellId
  TracedCallback<uint64_t, uint16_t, uint16_t, uint16_t> m_handoverStartTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_handoverEndOkTrace;
  //             imsi      cellId    rnti
  TracedCallback<uint64_t, uint16_t, uint16_t> m_handoverEndErrorTrace;

  bool m_connectionPending; /**< true if a connection request by upper layers is pending */
  bool m_hasReceivedMib; /**< true if MIB was received for the current cell  */
  bool m_hasReceivedSib1; /**< true if SIB1 was received for the current cell  */
  bool m_hasReceivedSib2; /**< true if SIB2 was received for the current cell  */

  /// Stored content of the last SIB1 received.
  LteRrcSap::SystemInformationBlockType1 m_lastSib1;

  /// List of cell ID of acceptable cells for cell selection that have been detected.
  std::set<uint16_t> m_acceptableCell;

  /// List of CSG ID which this UE entity has access to.
  uint32_t m_csgWhiteList;


  // INTERNAL DATA STRUCTURE RELATED TO UE MEASUREMENTS

  /**
   * \brief Includes the accumulated configuration of the measurements to be
   *        performed by the UE.
   *
   * Based on 3GPP TS 36.331 section 7.1. Also note that some optional variables
   * in the specification are omitted.
   */
  struct VarMeasConfig
  {
    std::map<uint8_t, LteRrcSap::MeasIdToAddMod> measIdList;
    std::map<uint8_t, LteRrcSap::MeasObjectToAddMod> measObjectList;
    std::map<uint8_t, LteRrcSap::ReportConfigToAddMod> reportConfigList;
    LteRrcSap::QuantityConfig quantityConfig; 
    double aRsrp;
    double aRsrq;
  };

  /**
   * \brief Includes the accumulated configuration of the measurements to be
   *        performed by the UE.
   *
   * Based on 3GPP TS 36.331 section 7.1.
   */
  VarMeasConfig m_varMeasConfig;

  /**
   * \brief Represents a single measurement reporting entry., which includes
   *        information about a measurement for which the triggering conditions
   *        have been met.
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

  /**
   * \brief The list of active reporting entries, indexed by the measurement
   *        identity which triggered the reporting. Includes information about
   *        measurements for which the triggering conditions have been met.
   */
  std::map<uint8_t, VarMeasReport> m_varMeasReportList;

  /**
   * \brief List of cell IDs which are responsible for a certain trigger.
   */
  typedef std::list<uint16_t> ConcernedCells_t;

  /**
   * \brief Compose a new reporting entry of the given measurement identity,
   *        insert it into #m_varMeasReportList, and set it up for submission
   *        to eNodeB.
   * \param measId the measurement identity which the new reporting entry will
   *               be based upon
   * \param enteringCells the cells which are responsible for triggering the
   *                      reporting (i.e., successfully fulfilling the entering
   *                      condition of the measurement identity) and will be
   *                      included in the measurement report.
   *
   * \note If an existing reporting entry with the same measurement identity has
   *       already existed in #m_varMeasReportList, the function will update it
   *       by adding the entering cells into the existing reporting entry.
   * \note When time-to-trigger is enabled for this measurement identity, the
   *       function will also remove the related trigger from the
   *       #m_enteringTriggerQueue.
   */
  void VarMeasReportListAdd (uint8_t measId, ConcernedCells_t enteringCells);

  /**
   * \brief Remove some cells from an existing reporting entry in
   *        #m_varMeasReportList.
   * \param measId the measurement identity to be removed from
   *               #m_varMeasReportList, must already exists there, otherwise
   *               an error would be raised
   * \param leavingCells the cells which are about to be removed
   * \param reportOnLeave when true, will make the function send one last
   *                      measurement report to eNodeB before removing it
   *
   * \note If a given cell is not found in the reporting entry, the function
   *       will quietly continue.
   * \note If the removal has removed all the cells in the reporting entry, the
   *       function will remove the reporting entry as well.
   * \note When time-to-trigger is enabled for this measurement identity, the
   *       function will also remove the related trigger from the
   *       #m_leavingTriggerQueue.
   */
  void VarMeasReportListErase (uint8_t measId, ConcernedCells_t leavingCells,
                               bool reportOnLeave);

  /**
   * \brief Remove the reporting entry of the given measurement identity from
   *        #m_varMeasReportList.
   * \param measId the measurement identity to be removed from
   *               #m_varMeasReportList, must already exists there, otherwise
   *               an error would be raised
   *
   * Any events or triggers related with this measurement identity will be
   * canceled as well.
   */
  void VarMeasReportListClear (uint8_t measId);

  /**
   * \brief Represents a measurement result from a certain cell.
   */
  struct MeasValues
  {
    double rsrp; ///< Measured RSRP in dBm.
    double rsrq; ///< Measured RSRQ in dB.
    Time timestamp; ///< Not used. \todo Should be removed.
  };

  /**
   * \brief Internal storage of the latest measurement results from all detected
   *        detected cells, indexed by the cell ID where the measurement was
   *        taken from.
   *
   * Each *measurement result* comprises of RSRP (in dBm) and RSRQ (in dB).
   *
   * In IDLE mode, the measurement results are used by the *initial cell
   * selection* procedure. While in CONNECTED mode, *layer-3 filtering* is
   * applied to the measurement results and they are used by *UE measurements*
   * function (LteUeRrc::MeasurementReportTriggering and
   * LteUeRrc::SendMeasurementReport).
   */
  std::map<uint16_t, MeasValues> m_storedMeasValues;

  /**
   * \brief Represents a single triggered event from a measurement identity
   *        which reporting criteria have been fulfilled, but delayed by
   *        time-to-trigger.
   */
  struct PendingTrigger_t
  {
    uint8_t measId; ///< The measurement identity which raised the trigger.
    ConcernedCells_t concernedCells; ///< The list of cells responsible for this trigger.
    EventId timer; ///< The pending reporting event, scheduled at the end of the time-to-trigger.
  };

  /**
   * \brief List of triggers that were raised because entering condition have
   *        been true, but are still delayed from reporting it by
   *        time-to-trigger.
   *
   * The list is indexed by the measurement identity where the trigger
   * originates from. The enclosed event will run at the end of the
   * time-to-trigger and insert a *reporting entry* to #m_varMeasReportList.
   */
  std::map<uint8_t, std::list<PendingTrigger_t> > m_enteringTriggerQueue;

  /**
   * \brief List of triggers that were raised because leaving condition have
   *        been true, but are still delayed from stopping the reporting by
   *        time-to-trigger.
   *
   * The list is indexed by the measurement identity where the trigger
   * originates from. The enclosed event will run at the end of the
   * time-to-trigger and remove the associated *reporting entry* from
   * #m_varMeasReportList.
   */
  std::map<uint8_t, std::list<PendingTrigger_t> > m_leavingTriggerQueue;

  /**
   * \brief Clear all the waiting triggers in #m_enteringTriggerQueue which are
   *        associated with the given measurement identity.
   * \param measId the measurement identity to be processed, must already exists
   *               in #m_enteringTriggerQueue, otherwise an error would be
   *               raised
   *
   * \note The function may conclude that there is nothing to be removed. In
   *       this case, the function will simply ignore quietly.
   *
   * This function is used when the entering condition of the measurement
   * identity becomes no longer true. Therefore all the waiting triggers for
   * this measurement identity in #m_enteringTriggerQueue have become invalid
   * and must be canceled.
   *
   * \sa LteUeRrc::m_enteringTriggerQueue
   */
  void CancelEnteringTrigger (uint8_t measId);

  /**
   * \brief Remove a specific cell from the waiting triggers in
   *        #m_enteringTriggerQueue which belong to the given measurement
   *        identity.
   * \param measId the measurement identity to be processed, must already exists
   *               in #m_enteringTriggerQueue, otherwise an error would be
   *               raised
   * \param cellId the cell ID to be removed from the waiting triggers
   *
   * \note The function may conclude that there is nothing to be removed. In
   *       this case, the function will simply ignore quietly.
   *
   * This function is used when a specific neighbour cell no longer fulfills
   * the entering condition of the measurement identity. Thus the cell must be
   * removed from all the waiting triggers for this measurement identity in
   * #m_enteringTriggerQueue.
   *
   * \sa LteUeRrc::m_enteringTriggerQueue
   */
  void CancelEnteringTrigger (uint8_t measId, uint16_t cellId);

  /**
   * \brief Clear all the waiting triggers in #m_leavingTriggerQueue which are
   *        associated with the given measurement identity.
   * \param measId the measurement identity to be processed, must already exists
   *               in #m_leavingTriggerQueue, otherwise an error would be
   *               raised
   *
   * \note The function may conclude that there is nothing to be removed. In
   *       this case, the function will simply ignore quietly.
   *
   * This function is used when the leaving condition of the measurement
   * identity becomes no longer true. Therefore all the waiting triggers for
   * this measurement identity in #m_leavingTriggerQueue have become invalid
   * and must be canceled.
   *
   * \sa LteUeRrc::m_leavingTriggerQueue
   */
  void CancelLeavingTrigger (uint8_t measId);

  /**
   * \brief Remove a specific cell from the waiting triggers in
   *        #m_leavingTriggerQueue which belong to the given measurement
   *        identity.
   * \param measId the measurement identity to be processed, must already exists
   *               in #m_leavingTriggerQueue, otherwise an error would be
   *               raised
   * \param cellId the cell ID to be removed from the waiting triggers
   *
   * \note The function may conclude that there is nothing to be removed. In
   *       this case, the function will simply ignore quietly.
   *
   * This function is used when a specific neighbour cell no longer fulfills
   * the leaving condition of the measurement identity. Thus the cell must be
   * removed from all the waiting triggers for this measurement identity in
   * #m_leavingTriggerQueue.
   *
   * \sa LteUeRrc::m_leavingTriggerQueue
   */
  void CancelLeavingTrigger (uint8_t measId, uint16_t cellId);

}; // end of class LteUeRrc


} // namespace ns3

#endif // LTE_UE_RRC_H
