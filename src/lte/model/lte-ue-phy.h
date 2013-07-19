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
   * \param pw the transmission power in dBm
   */
  void SetTxPower (double pow);

  /**
   * \return the transmission power in dBm
   */
  double GetTxPower () const;
  /**
   * \param pw the noise figure in dB
   */
  void SetNoiseFigure (double pow);

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
  */
  Ptr<DlCqiLteControlMessage> CreateDlCqiFeedbackMessage (const SpectrumValue& sinr);



  // inherited from LtePhy
  virtual void GenerateCtrlCqiReport (const SpectrumValue& sinr);
  virtual void GenerateDataCqiReport (const SpectrumValue& sinr);
  virtual void ReportInterference (const SpectrumValue& interf);
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

  void ReportUeMeasurements ();

  // UE CPHY SAP methods
  void DoReset ();  
  void DoSyncronizeWithEnb (uint16_t cellId, uint16_t dlEarfcn);  
  void DoSetDlBandwidth (uint8_t ulBandwidth);
  void DoConfigureUplink (uint16_t ulEarfcn, uint8_t ulBandwidth);
  void DoSetRnti (uint16_t rnti);
  void DoSetTransmissionMode (uint8_t txMode);
  void DoSetSrsConfigurationIndex (uint16_t srcCi);

  // UE PHY SAP methods 
  virtual void DoSendMacPdu (Ptr<Packet> p);  
  virtual void DoSendLteControlMessage (Ptr<LteControlMessage> msg);
  virtual void DoSendRachPreamble (uint32_t prachId, uint32_t raRnti);
  
  std::vector <int> m_subChannelsForTransmission;
  std::vector <int> m_subChannelsForReception;
  
  std::vector< std::vector <int> > m_subChannelsForTransmissionQueue;
  
  
  Ptr<LteAmc> m_amc;

  Time m_p10CqiPeriocity; /**< Wideband Periodic CQI: 2, 5, 10, 16, 20, 32, 40, 64, 80 or 160 ms */
  Time m_p10CqiLast;

  /**< SubBand Aperiodic CQI: activated by  DCI format 0 or Random Access Response Grant */
  // NOTE defines a periodicity for academic studies
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

  uint8_t m_subframeNo;

  bool m_rsReceivedPowerUpdated;
  SpectrumValue m_rsReceivedPower;

  bool m_rsInterferencePowerUpdated;
  SpectrumValue m_rsIntereferencePower;

  bool m_pssReceived;
  struct PssElement
    {
      uint16_t cellId;
      double pssPsdSum;
      uint16_t nRB;
    };
  std::list <PssElement> m_pssList;

  double m_pssReceptionThreshold; // on RSRQ [W]

  struct UeMeasurementsElement
    {
      double rsrpSum;
      uint8_t rsrpNum;
      double rsrqSum;
      uint8_t rsrqNum;
    };

  std::map <uint16_t, UeMeasurementsElement> m_UeMeasurementsMap;
  Time m_ueMeasurementsFilterPeriod;
  Time m_ueMeasurementsFilterLast;

  Ptr<LteHarqPhy> m_harqPhyModule;

  uint32_t m_raPreambleId;
  uint32_t m_raRnti;

  /**
   * Trace information regarding RSRP and average SINR (see TS 36.214)
   * uint16_t cellId, uint16_t rnti, double rsrp, double sinr
   */
  TracedCallback<uint16_t, uint16_t, double, double> m_reportCurrentCellRsrpSinrTrace;
  uint16_t m_rsrpSinrSamplePeriod;
  uint16_t m_rsrpSinrSampleCounter;

  /**
   * Trace information regarding RSRP and RSRQ (see TS 36.214)
   * uint16_t rnti, uint16_t cellId, double rsrp, double sinr, bool servingCell
   */
  TracedCallback<uint16_t, uint16_t, double, double, bool> m_reportUeMeasurements;

  EventId m_sendSrsEvent;

  /**
   * Trace information regarding PHY stats from DL Tx perspective
   * PhyTrasmissionStatParameters  see lte-common.h
   */
  TracedCallback<PhyTransmissionStatParameters> m_ulPhyTransmission;

};


}

#endif /* LTE_UE_PHY_H */
