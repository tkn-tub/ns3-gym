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
 *         Marco Miozzo <mmiozzo@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef LTE_PHY_H
#define LTE_PHY_H


#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-signal-parameters.h>
#include <ns3/spectrum-interference.h>
#include <ns3/generic-phy.h>
#include <ns3/lte-spectrum-phy.h>

namespace ns3 {

class PacketBurst;
class LteNetDevice;
class LteControlMessage;



/**
 * \ingroup lte
 *
 * The LtePhy models the physical layer of LTE. It is composed by two
 * LteSpectrumPhy, one for the downlink and one for the uplink.
 */
class LtePhy : public Object
{

public:
  /** 
   * @warning the default constructor should not be used
   */
  LtePhy ();

  /** 
   * 
   * \param dlPhy the downlink LteSpectrumPhy instance
   * \param ulPhy the uplink LteSpectrumPhy instance
   */
  LtePhy (Ptr<LteSpectrumPhy> dlPhy, Ptr<LteSpectrumPhy> ulPhy);

  virtual ~LtePhy ();

  static TypeId GetTypeId (void);

  /**
   * \brief Set the device where the phy layer is attached
   * \param d the device
   */
  void SetDevice (Ptr<LteNetDevice> d);
  /**
   * \brief Get the device where the phy layer is attached
   * \return the pointer to the device
   */
  Ptr<LteNetDevice> GetDevice ();

  /** 
   * 
   * \return a pointer to the LteSpectrumPhy instance that manages the downlink
   */
  Ptr<LteSpectrumPhy> GetDownlinkSpectrumPhy ();


  /** 
   * 
   * \return a pointer to the LteSpectrumPhy instance that manages the uplink
   */
  Ptr<LteSpectrumPhy> GetUplinkSpectrumPhy ();

  /**
  * \brief Queue the MAC PDU to be sent (according to m_macChTtiDelay)
  * \param p the MAC PDU to sent
  */
  virtual void DoSendMacPdu (Ptr<Packet> p) = 0;

  /**
   * Set the downlink channel
   * \param c the downlink channel
   */
  void SetDownlinkChannel (Ptr<SpectrumChannel> c);

  /**
   * Set the uplink channel
   * \param c the uplink channel
   */
  void SetUplinkChannel (Ptr<SpectrumChannel> c);


  /**
   * \brief Compute the TX Power Spectral Density
   * \return a pointer to a newly allocated SpectrumValue representing the TX Power Spectral Density in W/Hz for each Resource Block
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity () = 0;

  void DoDispose ();

  /**
   * \brief Receive SendLteControlMessage (PDCCH map, CQI feedbacks) using the ideal control channel
   * \param msg the Ideal Control Message to receive
   */
//   virtual void ReceiveLteControlMessage (Ptr<LteControlMessage> msg) = 0;


  /**
   * \param tti transmission time interval
   */
  void SetTti (double tti);
  /**
   * \returns transmission time interval
   */
  double GetTti (void) const;

  /** 
   * 
   * \param cellId the Cell Identifier
   */
  void DoSetCellId (uint16_t cellId);


  /**
  * \returns the RB gruop size according to the bandwidth
  */
  uint8_t GetRbgSize (void) const;
  
  
  /**
  * \returns the SRS periodicity (see Table 8.2-1 of 36.213)
  * \param srcCi the SRS Configuration Index
  */
  uint16_t GetSrsPeriodicity (uint16_t srcCi) const;
  
  /**
  * \returns the SRS Subframe offset (see Table 8.2-1 of 36.213)
  * \param srcCi the SRS Configuration Index
  */
  uint16_t GetSrsSubframeOffset (uint16_t srcCi) const;


  /**
  * \param p queue MAC PDU to be sent
  */
  void SetMacPdu (Ptr<Packet> p);

  /**
  * \returns the packet burst to be sent
  */
  Ptr<PacketBurst> GetPacketBurst (void);

  /**
  * \param m the control message to be sent
  */
  void SetControlMessages (Ptr<LteControlMessage> m);

  /**
  * \returns the list of control messages to be sent
  */
  std::list<Ptr<LteControlMessage> > GetControlMessages (void);


  /** 
   * generate a CQI report based on the given SINR of Ctrl frame
   * 
   * \param sinr the SINR vs frequency measured by the device
   */
  virtual void  GenerateCtrlCqiReport (const SpectrumValue& sinr) = 0;
  
  /** 
  * generate a CQI report based on the given SINR of Data frame
  * (used for PUSCH CQIs)
  * 
  * \param sinr the SINR vs frequency measured by the device
  */
  virtual void  GenerateDataCqiReport (const SpectrumValue& sinr) = 0;

  /**
  * generate a report based on the linear interference and noise power
  * perceived during DATA frame
  * NOTE: used only by eNB 
  *
  * \param sinr the interference + noise power measured by the device
  */
  virtual void ReportInterference (const SpectrumValue& power) = 0;

  /**
  * generate a report based on the linear RS power perceived during CTRL 
  * frame
  * NOTE: used only by UE for evaluating RSRP
  *
  * \param sinr the RS power measured by the device
  */
  virtual void ReportRsReceivedPower (const SpectrumValue& interf) = 0;



protected:
  Ptr<LteNetDevice> m_netDevice;

  Ptr<LteSpectrumPhy> m_downlinkSpectrumPhy;
  Ptr<LteSpectrumPhy> m_uplinkSpectrumPhy;

  double m_txPower;
  double m_noiseFigure;

  double m_tti;
  uint8_t m_ulBandwidth;
  uint8_t m_dlBandwidth;
  uint8_t m_rbgSize;

  uint16_t m_dlEarfcn;
  uint16_t m_ulEarfcn;

  std::vector< Ptr<PacketBurst> > m_packetBurstQueue;
  std::vector< std::list<Ptr<LteControlMessage> > > m_controlMessagesQueue;
  uint8_t m_macChTtiDelay; // delay between MAC and channel layer in terms of TTIs

  uint16_t m_cellId;

};


}

#endif /* LTE_PHY_H */
