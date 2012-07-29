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


#include <ns3/lte-enb-phy-sap.h>
#include <ns3/lte-phy.h>
#include <ns3/lte-ue-phy.h>

#include <map>
#include <set>



namespace ns3 {

class PacketBurst;
class LteNetDevice;

/**
 * \ingroup lte
 * LteEnbPhy models the physical layer for the eNodeB
 */
class LteEnbPhy : public LtePhy
{

  friend class EnbMemberLteEnbPhySapProvider;

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
  virtual void DoStart (void);
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
  * \brief Queue the MAC PDU to be sent
  * \param p the MAC PDU to sent
  */
  virtual void DoSendMacPdu (Ptr<Packet> p);
  
  virtual void DoSetTransmissionMode (uint16_t  rnti, uint8_t txMode);
  
  virtual void DoSetSrsConfigurationIndex (uint16_t  rnti, uint16_t srcCi);
    
  virtual uint8_t DoGetMacChTtiDelay ();

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


  void DoSendLteControlMessage (Ptr<LteControlMessage> msg);

  bool AddUePhy (uint16_t rnti);

  bool DeleteUePhy (uint16_t rnti);
  
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


private:
  std::set <uint16_t> m_ueAttached;
  
  std::vector <int> m_listOfDownlinkSubchannel;
  
  std::vector <int> m_dlDataRbMap;
  
  std::vector< std::list<UlDciLteControlMessage> > m_ulDciQueue; // for storing info on future receptions

  LteEnbPhySapProvider* m_enbPhySapProvider;
  LteEnbPhySapUser* m_enbPhySapUser;
  
  std::vector <uint16_t> m_ulRntiRxed;

  uint32_t m_nrFrames;
  uint32_t m_nrSubFrames;
  
  uint16_t m_srsPeriodicity;
  std::map <uint16_t,uint16_t> m_srsCounter;
  std::vector <uint16_t> m_srsUeOffset;
  uint16_t m_currentSrsOffset;
  
};


}

#endif /* LTE_ENB_PHY_H */
