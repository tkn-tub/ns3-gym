/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 * Author: Marco Miozzo <mmiozzo@cttc.es>
 */

#ifndef LTE_UE_PHY_H
#define LTE_UE_PHY_H


#include <ns3/lte-phy.h>
#include <ns3/ff-mac-common.h>

#include <ns3/lte-control-messages.h>
#include <ns3/lte-amc.h>
#include <ns3/lte-ue-phy-sap.h>
#include <ns3/lte-ue-cphy-sap.h>
#include <ns3/ptr.h>
#include <ns3/lte-amc.h>
#include <set>
#include <ns3/lte-ue-power-control.h>


namespace ns3 {

class PacketBurst;
class LteEnbPhy;
class LteHarqPhy;


/**
 * \ingroup lte
 *
 * The LteSpectrumPhy models the physical layer of LTE
 */
class LteUePhy : public LtePhy
{

  friend class UeMemberLteUePhySapProvider;
  friend class MemberLteUeCphySapProvider<LteUePhy>;

public:
  /**
   * \brief The states of the UE PHY entity
   */
  enum State
  {
    CELL_SEARCH = 0,
    SYNCHRONIZED,
    NUM_STATES
  };

  /**
   * @warning the default constructor should not be used
   */
  LteUePhy ();

  /**
   *
   * \param dlPhy the downlink LteSpectrumPhy instance
   * \param ulPhy the uplink LteSpectrumPhy instance
   */
  LteUePhy (Ptr<LteSpectrumPhy> dlPhy, Ptr<LteSpectrumPhy> ulPhy);

  virtual ~LteUePhy ();

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

  /**
   * \brief Get the PHY SAP provider
   * \return a pointer to the SAP Provider 
   */
  LteUePhySapProvider* GetLteUePhySapProvider ();

  /**
  * \brief Set the PHY SAP User
  * \param s a pointer to the SAP user
  */
  void SetLteUePhySapUser (LteUePhySapUser* s);

  /**
   * \brief Get the CPHY SAP provider
   * \return a pointer to the SAP Provider
   */
  LteUeCphySapProvider* GetLteUeCphySapProvider ();

  /**
  * \brief Set the CPHY SAP User
  * \param s a pointer to the SAP user
  */
  void SetLteUeCphySapUser (LteUeCphySapUser* s);


  /**
   * \param pow the transmission power in dBm
   */
  void SetTxPower (double pow);

  /**
   * \return the transmission power in dBm
   */
  double GetTxPower () const;

  /**
   * \return ptr to UE Uplink Power Control entity
   */
  Ptr<LteUePowerControl> GetUplinkPowerControl () const;

  /**
   * \param nf the noise figure in dB
   */
  void SetNoiseFigure (double nf);

  /**
   * \return the noise figure in dB
   */
  double GetNoiseFigure () const;

  /**
   * \returns the TTI delay between MAC and channel
   */
  uint8_t GetMacChDelay (void) const;

  /**
   * \return a pointer to the LteSpectrumPhy instance relative to the downlink
   */
  Ptr<LteSpectrumPhy> GetDlSpectrumPhy () const;

  /**
   * \return a pointer to the LteSpectrumPhy instance relative to the uplink
   */
  Ptr<LteSpectrumPhy> GetUlSpectrumPhy () const;

  /**
   * \brief Create the PSD for the TX
   * \return the pointer to the PSD
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity ();

  /**
   * \brief Set a list of sub channels to use in TX
   * \param mask a list of sub channels
   */
  void SetSubChannelsForTransmission (std::vector <int> mask);
  /**
   * \brief Get a list of sub channels to use in RX
   * \return a list of sub channels
   */
  std::vector <int> GetSubChannelsForTransmission (void);

  /**
   * \brief Get a list of sub channels to use in RX
   * \param mask list of sub channels
   */
  void SetSubChannelsForReception (std::vector <int> mask);
  /**
   * \brief Get a list of sub channels to use in RX
   * \return a list of sub channels
   */
  std::vector <int> GetSubChannelsForReception (void);

  /**
  * \brief Create the DL CQI feedback from SINR values perceived at
  * the physical layer with the signal received from eNB
  * \param sinr SINR values vector
  * \return a DL CQI control message containing the CQI feedback
  */
  Ptr<DlCqiLteControlMessage> CreateDlCqiFeedbackMessage (const SpectrumValue& sinr);



  // inherited from LtePhy
  virtual void GenerateCtrlCqiReport (const SpectrumValue& sinr);
  virtual void GenerateDataCqiReport (const SpectrumValue& sinr);
  virtual void GenerateMixedCqiReport (const SpectrumValue& sinr);
  virtual void ReportInterference (const SpectrumValue& interf);
  virtual void ReportDataInterference (const SpectrumValue& interf);
  virtual void ReportRsReceivedPower (const SpectrumValue& power);

  // callbacks for LteSpectrumPhy
  virtual void ReceiveLteControlMessageList (std::list<Ptr<LteControlMessage> >);
  virtual void ReceivePss (uint16_t cellId, Ptr<SpectrumValue> p);



  /**
   * \brief PhySpectrum received a new PHY-PDU
   */
  void PhyPduReceived (Ptr<Packet> p);


  /**
  * \brief trigger from eNB the start from a new frame
  *
  * \param frameNo frame number
  * \param subframeNo subframe number
  */
  void SubframeIndication (uint32_t frameNo, uint32_t subframeNo);


  /**
   * \brief Send the SRS signal in the last symbols of the frame
   */
  void SendSrs ();

  /**
   * \brief PhySpectrum generated a new DL HARQ feedback
   */
  virtual void ReceiveLteDlHarqFeedback (DlInfoListElement_s mes);

  /**
   * \brief Set the HARQ PHY module
   */
  void SetHarqPhyModule (Ptr<LteHarqPhy> harq);

  /**
   * \return The current state
   */
  State GetState () const;



private:

  void SetTxMode1Gain (double gain);
  void SetTxMode2Gain (double gain);
  void SetTxMode3Gain (double gain);
  void SetTxMode4Gain (double gain);
  void SetTxMode5Gain (double gain);
  void SetTxMode6Gain (double gain);
  void SetTxMode7Gain (double gain);
  void SetTxModeGain (uint8_t txMode, double gain);

  void QueueSubChannelsForTransmission (std::vector <int> rbMap);

  /**
   * \brief Layer-1 filtering of RSRP and RSRQ measurements and reporting to
   *        the RRC entity.
   *
   * Initially executed at +0.200s, and then repeatedly executed with
   * periodicity as indicated by the *UeMeasurementsFilterPeriod* attribute.
   */
  void ReportUeMeasurements ();

  /**
   * Switch the UE PHY to the given state.
   * \param s the destination state
   */
  void SwitchToState (State s);

  // UE CPHY SAP methods
  void DoReset ();
  void DoStartCellSearch (uint16_t dlEarfcn);
  void DoSynchronizeWithEnb (uint16_t cellId);
  void DoSynchronizeWithEnb (uint16_t cellId, uint16_t dlEarfcn);
  void DoSetDlBandwidth (uint8_t ulBandwidth);
  void DoConfigureUplink (uint16_t ulEarfcn, uint8_t ulBandwidth);
  void DoConfigureReferenceSignalPower (int8_t referenceSignalPower);
  void DoSetRnti (uint16_t rnti);
  void DoSetTransmissionMode (uint8_t txMode);
  void DoSetSrsConfigurationIndex (uint16_t srcCi);

  // UE PHY SAP methods 
  virtual void DoSendMacPdu (Ptr<Packet> p);
  virtual void DoSendLteControlMessage (Ptr<LteControlMessage> msg);
  virtual void DoSendRachPreamble (uint32_t prachId, uint32_t raRnti);

  /// A list of sub channels to use in TX.
  std::vector <int> m_subChannelsForTransmission;
  /// A list of sub channels to use in RX.
  std::vector <int> m_subChannelsForReception;

  std::vector< std::vector <int> > m_subChannelsForTransmissionQueue;


  Ptr<LteAmc> m_amc;

  /**
   * The `EnableUplinkPowerControl` attribute. If true, Uplink Power Control
   * will be enabled.
   */
  bool m_enableUplinkPowerControl;
  /// Pointer to UE Uplink Power Control entity.
  Ptr<LteUePowerControl> m_powerControl;

  /// Wideband Periodic CQI. 2, 5, 10, 16, 20, 32, 40, 64, 80 or 160 ms.
  Time m_p10CqiPeriocity;
  Time m_p10CqiLast;

  /**
   * SubBand Aperiodic CQI. Activated by DCI format 0 or Random Access Response
   * Grant.
   * \note Defines a periodicity for academic studies.
   */
  Time m_a30CqiPeriocity;
  Time m_a30CqiLast;

  LteUePhySapProvider* m_uePhySapProvider;
  LteUePhySapUser* m_uePhySapUser;

  LteUeCphySapProvider* m_ueCphySapProvider;
  LteUeCphySapUser* m_ueCphySapUser;

  uint16_t  m_rnti;
 
  uint8_t m_transmissionMode;
  std::vector <double> m_txModeGain;

  uint16_t m_srsPeriodicity;
  uint16_t m_srsSubframeOffset;
  uint16_t m_srsConfigured;
  Time     m_srsStartTime;

  bool m_dlConfigured;
  bool m_ulConfigured;

  /// The current UE PHY state.
  State m_state;
  /**
   * The `StateTransition` trace source. Fired upon every UE PHY state
   * transition. Exporting the serving cell ID, RNTI, old state, and new state.
   */
  TracedCallback<uint16_t, uint16_t, State, State> m_stateTransitionTrace;

  /// \todo Can be removed.
  uint8_t m_subframeNo;

  bool m_rsReceivedPowerUpdated;
  SpectrumValue m_rsReceivedPower;

  bool m_rsInterferencePowerUpdated;
  SpectrumValue m_rsInterferencePower;

  bool m_dataInterferencePowerUpdated;
  SpectrumValue m_dataInterferencePower;

  bool m_pssReceived;
  struct PssElement
  {
    uint16_t cellId;
    double pssPsdSum;
    uint16_t nRB;
  };
  std::list <PssElement> m_pssList;

  /**
   * The `RsrqUeMeasThreshold` attribute. Receive threshold for PSS on RSRQ
   * in dB.
   */
  double m_pssReceptionThreshold;

  /// Summary results of measuring a specific cell. Used for layer-1 filtering.
  struct UeMeasurementsElement
  {
    double rsrpSum;   ///< Sum of RSRP sample values in linear unit.
    uint8_t rsrpNum;  ///< Number of RSRP samples.
    double rsrqSum;   ///< Sum of RSRQ sample values in linear unit.
    uint8_t rsrqNum;  ///< Number of RSRQ samples.
  };

  /**
   * Store measurement results during the last layer-1 filtering period.
   * Indexed by the cell ID where the measurements come from.
   */
  std::map <uint16_t, UeMeasurementsElement> m_ueMeasurementsMap;
  /**
   * The `UeMeasurementsFilterPeriod` attribute. Time period for reporting UE
   * measurements, i.e., the length of layer-1 filtering (default 200 ms).
   */
  Time m_ueMeasurementsFilterPeriod;
  /// \todo Can be removed.
  Time m_ueMeasurementsFilterLast;

  Ptr<LteHarqPhy> m_harqPhyModule;

  uint32_t m_raPreambleId;
  uint32_t m_raRnti;

  /**
   * The `ReportCurrentCellRsrpSinr` trace source. Trace information regarding
   * RSRP and average SINR (see TS 36.214). Exporting cell ID, RNTI, RSRP, and
   * SINR.
   */
  TracedCallback<uint16_t, uint16_t, double, double> m_reportCurrentCellRsrpSinrTrace;
  /**
   * The `RsrpSinrSamplePeriod` attribute. The sampling period for reporting
   * RSRP-SINR stats.
   */
  uint16_t m_rsrpSinrSamplePeriod;
  uint16_t m_rsrpSinrSampleCounter;

  /**
   * The `ReportUeMeasurements` trace source. Contains trace information
   * regarding RSRP and RSRQ measured from a specific cell (see TS 36.214).
   * Exporting RNTI, the ID of the measured cell, RSRP (in dBm), RSRQ (in dB),
   * and whether the cell is the serving cell.
   */
  TracedCallback<uint16_t, uint16_t, double, double, bool> m_reportUeMeasurements;

  EventId m_sendSrsEvent;

  /**
   * The `UlPhyTransmission` trace source. Contains trace information regarding
   * PHY stats from UL Tx perspective. Exporting a structure with type
   * PhyTransmissionStatParameters.
   */
  TracedCallback<PhyTransmissionStatParameters> m_ulPhyTransmission;

}; // end of `class LteUePhy`


}

#endif /* LTE_UE_PHY_H */
