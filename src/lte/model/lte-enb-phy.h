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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */

#ifndef ENB_LTE_PHY_H
#define ENB_LTE_PHY_H


#include <ns3/lte-control-messages.h>
#include <ns3/lte-enb-phy-sap.h>
#include <ns3/lte-enb-cphy-sap.h>
#include <ns3/lte-phy.h>
#include <ns3/lte-harq-phy.h>

#include <map>
#include <set>



namespace ns3 {

class PacketBurst;
class LteNetDevice;
class LteUePhy;

/**
 * \ingroup lte
 * LteEnbPhy models the physical layer for the eNodeB
 */
class LteEnbPhy : public LtePhy
{
  friend class EnbMemberLteEnbPhySapProvider;
  friend class MemberLteEnbCphySapProvider<LteEnbPhy>;
  
public:
  /**
   * @warning the default constructor should not be used
   */
  LteEnbPhy ();

  /**
   *
   * \param dlPhy the downlink LteSpectrumPhy instance
   * \param ulPhy the uplink LteSpectrumPhy instance
   */
  LteEnbPhy (Ptr<LteSpectrumPhy> dlPhy, Ptr<LteSpectrumPhy> ulPhy);

  virtual ~LteEnbPhy ();

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoInitialize (void);
  virtual void DoDispose (void);


  /**
  * \brief Get the PHY SAP provider
  * \return a pointer to the SAP Provider of the PHY
  */
  LteEnbPhySapProvider* GetLteEnbPhySapProvider ();

  /**
  * \brief Set the PHY SAP User
  * \param s a pointer to the PHY SAP user
  */
  void SetLteEnbPhySapUser (LteEnbPhySapUser* s);

  /**
   * \brief Get the CPHY SAP provider
   * \return a pointer to the SAP Provider
   */
  LteEnbCphySapProvider* GetLteEnbCphySapProvider ();

  /**
  * \brief Set the CPHY SAP User
  * \param s a pointer to the SAP user
  */
  void SetLteEnbCphySapUser (LteEnbCphySapUser* s);

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
   * \param delay the TTI delay between MAC and channel
   */
  void SetMacChDelay (uint8_t delay);
  
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
   * \brief set the resource blocks (a.k.a. sub channels) to be used in the downlink for transmission
   * 
   * \param mask a vector of integers, if the i-th value is j it means
   * that the j-th resource block is used for transmission in the
   * downlink. If there is no i such that the value of the i-th
   * element is j, it means that RB j is not used.
   */
  void SetDownlinkSubChannels (std::vector<int> mask );


  /**
   * 
   * \return  a vector of integers, if the i-th value is j it means
   * that the j-th resource block is used for transmission in the
   * downlink. If there is no i such that the value of the i-th
   * element is j, it means that RB j is not used.
   */
  std::vector<int> GetDownlinkSubChannels (void);

  /**
   * \brief Create the PSD for TX
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity ();

  /**
   * \brief Calculate the channel quality for a given UE
   * \param sinr a list of computed SINR
   * \param ue the UE
   */
  void CalcChannelQualityForUe (std::vector <double> sinr, Ptr<LteSpectrumPhy> ue);

  /**
   * \brief Receive the control message
   * \param msg the received message
   */
  virtual void ReceiveLteControlMessage (Ptr<LteControlMessage> msg);

  /**
  * \brief Create the UL CQI feedback from SINR values perceived at
  * the physical layer with the PUSCH signal received from eNB
  * \param sinr SINR values vector
  */
  FfMacSchedSapProvider::SchedUlCqiInfoReqParameters CreatePuschCqiReport (const SpectrumValue& sinr);
  
  /**
  * \brief Create the UL CQI feedback from SINR values perceived at
  * the physical layer with the SRS signal received from eNB
  * \param sinr SINR values vector
  */
  FfMacSchedSapProvider::SchedUlCqiInfoReqParameters CreateSrsCqiReport (const SpectrumValue& sinr);

  /**
  * \brief Send the PDCCH and PCFICH in the first 3 symbols
  * \param ctrlMsgList the list of control messages of PDCCH
  */
  void SendControlChannels (std::list<Ptr<LteControlMessage> > ctrlMsgList);
  
  /**
  * \brief Send the PDSCH
  * \param pb the PacketBurst to be sent
  */
  void SendDataChannels (Ptr<PacketBurst> pb);
  
  /**
  * \param m the UL-CQI to be queued
  */
  void QueueUlDci (UlDciLteControlMessage m);
  
  /**
  * \returns the list of UL-CQI to be processed
  */
  std::list<UlDciLteControlMessage> DequeueUlDci (void);


  /**
   * \brief Start a LTE frame
   */
  void StartFrame (void);
  /**
   * \brief Start a LTE sub frame
   */
  void StartSubFrame (void);
  /**
   * \brief End a LTE sub frame
   */
  void EndSubFrame (void);
  /**
   * \brief End a LTE frame
   */
  void EndFrame (void);

  /**
   * \brief PhySpectrum received a new PHY-PDU
   */
  void PhyPduReceived (Ptr<Packet> p);
  
  /**
  * \brief PhySpectrum received a new list of LteControlMessage
  */
  virtual void ReceiveLteControlMessageList (std::list<Ptr<LteControlMessage> >);

  // inherited from LtePhy
  virtual void GenerateCtrlCqiReport (const SpectrumValue& sinr);
  virtual void GenerateDataCqiReport (const SpectrumValue& sinr);
  virtual void ReportInterference (const SpectrumValue& interf);
  virtual void ReportRsReceivedPower (const SpectrumValue& interf);



  /**
  * \brief PhySpectrum generated a new UL HARQ feedback
  */
  virtual void ReceiveLteUlHarqFeedback (UlInfoListElement_s mes);

  void SetHarqPhyModule (Ptr<LteHarqPhy> harq);


private:

  // LteEnbCphySapProvider forwarded methods
  void DoSetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth);
  void DoSetEarfcn (uint16_t dlEarfcn, uint16_t ulEarfcn);
  void DoAddUe (uint16_t rnti);  
  void DoRemoveUe (uint16_t rnti);  
  void DoSetTransmissionMode (uint16_t  rnti, uint8_t txMode);
  void DoSetSrsConfigurationIndex (uint16_t  rnti, uint16_t srcCi);  
  void DoSetMasterInformationBlock (LteRrcSap::MasterInformationBlock mib);
  void DoSetSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 sib1);

  // LteEnbPhySapProvider forwarded methods
  void DoSendMacPdu (Ptr<Packet> p);  
  void DoSendLteControlMessage (Ptr<LteControlMessage> msg);  
  uint8_t DoGetMacChTtiDelay ();  

  bool AddUePhy (uint16_t rnti);

  bool DeleteUePhy (uint16_t rnti);

  void CreateSrsReport(uint16_t rnti, double srs);


  std::set <uint16_t> m_ueAttached;
  
  std::vector <int> m_listOfDownlinkSubchannel;
  
  std::vector <int> m_dlDataRbMap;
  
  std::vector< std::list<UlDciLteControlMessage> > m_ulDciQueue; // for storing info on future receptions

  LteEnbPhySapProvider* m_enbPhySapProvider;
  LteEnbPhySapUser* m_enbPhySapUser;

  LteEnbCphySapProvider* m_enbCphySapProvider;
  LteEnbCphySapUser* m_enbCphySapUser;
  
  uint32_t m_nrFrames;
  uint32_t m_nrSubFrames;
  
  uint16_t m_srsPeriodicity;
  Time m_srsStartTime;
  std::map <uint16_t,uint16_t> m_srsCounter;
  std::vector <uint16_t> m_srsUeOffset;
  uint16_t m_currentSrsOffset;

  LteRrcSap::MasterInformationBlock m_mib;
  LteRrcSap::SystemInformationBlockType1 m_sib1;

  Ptr<LteHarqPhy> m_harqPhyModule;

  /**
   * Trace reporting the linear average of SRS SINRs 
   * uint16_t cellId, uint16_t rnti,  double sinrLinear
   */
  TracedCallback<uint16_t, uint16_t, double> m_reportUeSinr;
  uint16_t m_srsSamplePeriod;
  std::map <uint16_t,uint16_t> m_srsSampleCounterMap;

  /**
   * Trace reporting the interference per PHY RB (TS 36.214 section 5.2.2,
   *  measured on DATA) 
   * uint16_t cellId, Ptr<SpectrumValue> interference linear power per RB basis
   */
  TracedCallback<uint16_t, Ptr<SpectrumValue> > m_reportInterferenceTrace;
  uint16_t m_interferenceSamplePeriod;
  uint16_t m_interferenceSampleCounter;

  /**
   * Trace information regarding PHY stats from UL Tx perspective
   * PhyTrasmissionStatParameters see lte-common.h
   */
  TracedCallback<PhyTransmissionStatParameters> m_dlPhyTransmission;
  
};


}

#endif /* LTE_ENB_PHY_H */
