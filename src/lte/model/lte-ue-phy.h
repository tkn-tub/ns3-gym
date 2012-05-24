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

#include <ns3/ideal-control-messages.h>
#include <ns3/lte-amc.h>
#include <ns3/lte-ue-phy-sap.h>
#include <ns3/ptr.h>
#include <ns3/lte-amc.h>


namespace ns3 {

class PacketBurst;
class LteNetDevice;
class LteEnbPhy;

/**
 * \ingroup lte
 *
 * The LteSpectrumPhy models the physical layer of LTE
 */
class LteUePhy : public LtePhy
{

  friend class UeMemberLteUePhySapProvider;

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
  virtual void DoStart (void);
  virtual void DoDispose (void);

  /**
   * \brief Get the PHY SAP provider
   * \return a pointer to the SAP Provider of the PHY
   */
  LteUePhySapProvider* GetLteUePhySapProvider ();

  /**
  * \brief Set the PHY SAP User
  * \param s a pointer to the PHY SAP user
  */
  void SetLteUePhySapUser (LteUePhySapUser* s);


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
   * \brief Queue the MAC PDU to be sent
   * \param p the MAC PDU to sent
   */
  virtual void DoSendMacPdu (Ptr<Packet> p);

  /**
   * \brief Create the PSD for the TX
   * \return the pointer to the PSD
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity ();

  /**
   * \brief Update available channel for TX
   */
  virtual void DoSetUplinkSubChannels ();

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
  Ptr<DlCqiIdealControlMessage> CreateDlCqiFeedbackMessage (const SpectrumValue& sinr);



  // inherited from LtePhy
  virtual void GenerateCqiReport (const SpectrumValue& sinr);

  virtual void DoSendIdealControlMessage (Ptr<IdealControlMessage> msg);
  virtual void ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg);
  
  virtual void DoSetTransmissionMode (uint8_t txMode);
  
  



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
  * \param rnti the rnti assigned to the UE
  */
  void SetRnti (uint16_t rnti);


  /**
   * set the cellId of the eNb this PHY is synchronized with
   *
   * \param cellId the cell identifier of the eNB
   */
  void SetEnbCellId (uint16_t cellId);
  


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

  uint16_t  m_rnti;

  uint16_t m_enbCellId;
  
  uint8_t m_transmissionMode;
  std::vector <double> m_txModeGain;

};


}

#endif /* LTE_UE_PHY_H */
