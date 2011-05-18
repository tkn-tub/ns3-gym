/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 */

#ifndef LTE_PHY_H
#define LTE_PHY_H


#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-type.h>
#include <ns3/spectrum-interference.h>
#include <ns3/generic-phy.h>
#include "lte-spectrum-phy.h"

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
  LtePhy ();
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
   * \brief Start a transmission. StartTx is called by the LTE device to start packet transmission.
   * \param pb the packet of packets to be transmitted
   * \return true if an error occurred and the transmission was not started, false otherwise.
   */
  virtual bool SendPacket (Ptr<PacketBurst> pb) = 0;

  /**
   * Set the LTE SpectrumPhy for the downlink
   * \param s the LTE SpectrumPhy
   */
  void SetDownlinkSpectrumPhy (Ptr<LteSpectrumPhy> s);

  /**
   * Set the LTE SpectrumPhy for the uplink
   * \param s the LTE SpectrumPhy
   */
  void SetUplinkSpectrumPhy (Ptr<LteSpectrumPhy> s);

  /**
   * Get the LTE SpectrumPhy for the downlink
   * \return a Ptr to the LTE SpectrumPhy for the downlink
   */
  Ptr<LteSpectrumPhy> GetDownlinkSpectrumPhy ();

  /**
   * Get the LTE SpectrumPhy for the uplink
   * \return a Ptr to the LTE SpectrumPhy for the
   */
  Ptr<LteSpectrumPhy> GetUplinkSpectrumPhy ();

  /**
   * Set the downlink channel
   * \param c the downlink channel
   */
  void SetDownlinkChannel (Ptr<SpectrumChannel> c);

  /**
   * Get the downlink channel
   * \return a Ptr to the downlink SpectrumChannel
   */
  Ptr<SpectrumChannel> GetDownlinkChannel ();

  /**
   * Set the uplink channel
   * \param c the uplink channel
   */
  void SetUplinkChannel (Ptr<SpectrumChannel> c);

  /**
   * Get the uplink channel
   * \return a Ptr to the uplink SpectrumChannel
   */
  Ptr<SpectrumChannel> GetUplinkChannel ();

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
   * \return a Ptr to a created SpectrumValue
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity () = 0;

  /**
   * \brief Set the power transmisison, expressed in dBm
   * \param pw the power transmission
   */
  void SetTxPower (double pw);
  /**
   * \brief Get the power transmission, expressed in dBm
   * \return the power transmission
   */
  double GetTxPower (void);

  void DoDispose ();


  /**
   * \brief Send SendIdealControlMessage (PDCCH map, CQI feedbacks) using the ideal control channel
   * \param msg the Ideal Control Message to send
   */
  virtual void SendIdealControlMessage (Ptr<IdealControlMessage> msg) = 0;
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
   * \param nrFrames the number of the current LTE frame.
   */
  void SetNrFrames (uint32_t nrFrames);
  /**
   * \returns the number of the current LTE frame.
   */
  uint32_t GetNrFrames (void) const;
  /**
   * \param nrSubFrames the number of the current LTE sub-frame.
   */
  void SetNrSubFrames (uint32_t nrSubFrames);
  /**
   * \returns the number of the current LTE sub-frame.
   */
  uint32_t GetNrSubFrames (void) const;


private:
  Ptr<LteNetDevice> m_netDevice;

  Ptr<LteSpectrumPhy> m_downlinkSpectrumPhy;
  Ptr<LteSpectrumPhy> m_uplinkSpectrumPhy;

  std::vector <int> m_listOfDownlinkSubchannel;
  std::vector <int> m_listOfUplinkSubchannel;

  double m_txPower;


  static uint32_t m_nrFrames;
  static uint32_t m_nrSubFrames;
  double m_tti;
};


}

#endif /* LTE_PHY_H */
