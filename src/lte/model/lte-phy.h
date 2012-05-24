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
class IdealControlMessage;

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
   * \brief set a list of sub channel to use in the downlink.
   * A sub channel is composed by a couple of resource bloks (180KHz x 1 ms)
   * \param mask a vector of intefer values. Each elements of this vector carries information about
   * the corresponding DL sub channel. If the i-th value of mask is equal to 1 (0) it means that the corresponding sub channel is used (not used) for the downlink.
   */
  void SetDownlinkSubChannels (std::vector<int> mask );
  /**
   * \brief do some operation after the set of a list of DL sub channels
   */
  virtual void DoSetDownlinkSubChannels ();

  /**
   * \brief set a list of sub channel to use in the uplink.
   * A sub channel is composed by a couple of resource bloks (180KHz x 1 ms)
   * \param mask a vector of intefer values. Each elements of this vector carries information about
   * the corresponding UL sub channel. If the i-th value of mask is equal to 1 (0) it means that the corresponding sub channel is used (not used) for the uplink.
   */
  void SetUplinkSubChannels (std::vector<int> mask);
  /**
   * \brief do some operation after the set of a list of UL sub channels
   */
  virtual void DoSetUplinkSubChannels ();

  /**
   * \brief get a list of sub channel to use in the downlink
   * \return
   */
  std::vector<int> GetDownlinkSubChannels (void);
  /**
   * \brief get a list of sub channel to use in the downlink
   * \return
   */
  std::vector<int> GetUplinkSubChannels (void);


  /**
   * \brief Compute the TX Power Spectral Density
   * \return a pointer to a newly allocated SpectrumValue representing the TX Power Spectral Density in W/Hz for each Resource Block
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity () = 0;

  void DoDispose ();

  /**
   * \brief Receive SendIdealControlMessage (PDCCH map, CQI feedbacks) using the ideal control channel
   * \param msg the Ideal Control Message to receive
   */
  virtual void ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg) = 0;


  /**
   * \param tti transmission time interval
   */
  void SetTti (double tti);
  /**
   * \returns transmission time interval
   */
  double GetTti (void) const;

  /**
  * \param ulBandwidth the UL bandwidth in RB
  * \param dlBandwidth the DL bandwidth in RB
  */
  void DoSetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth);

  /**
   *
   * \param dlEarfcn the carrier frequency (EARFCN) in downlink
   * \param ulEarfcn the carrier frequency (EARFCN) in downlink
   */
  virtual void DoSetEarfcn (uint16_t dlEarfcn, uint16_t ulEarfcn);

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
  void SetControlMessages (Ptr<IdealControlMessage> m);

  /**
  * \returns the list of control messages to be sent
  */
  std::list<Ptr<IdealControlMessage> > GetControlMessages (void);


  /** 
   * generate a CQI report based on the given SINR
   * 
   * \param sinr the SINR vs frequency measured by the device
   */
  virtual void  GenerateCqiReport (const SpectrumValue& sinr) = 0;



protected:
  Ptr<LteNetDevice> m_netDevice;

  Ptr<LteSpectrumPhy> m_downlinkSpectrumPhy;
  Ptr<LteSpectrumPhy> m_uplinkSpectrumPhy;

  std::vector <int> m_listOfDownlinkSubchannel;
  std::vector <int> m_listOfUplinkSubchannel;

  double m_txPower;
  double m_noiseFigure;

  double m_tti;
  uint8_t m_ulBandwidth;
  uint8_t m_dlBandwidth;
  uint8_t m_rbgSize;

  uint16_t m_dlEarfcn;
  uint16_t m_ulEarfcn;

  std::vector< Ptr<PacketBurst> > m_packetBurstQueue;
  std::vector< std::list<Ptr<IdealControlMessage> > > m_controlMessagesQueue;
  uint8_t m_macChTtiDelay; // delay between MAC and channel layer in terms of TTIs

  uint16_t m_cellId;

};


}

#endif /* LTE_PHY_H */
