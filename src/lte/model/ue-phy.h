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
 */

#ifndef UE_PHY_H
#define UE_PHY_H


#include "lte-phy.h"

namespace ns3 {

class PacketBurst;
class LteNetDevice;

/**
 * \ingroup lte
 *
 * The LteSpectrumPhy models the physical layer of LTE
 */
class UeLtePhy : public LtePhy
{

public:
  UeLtePhy ();

  /**
   * \brief Create the physical layer
   * \param d the device where the physical layer is attached
   */
  UeLtePhy (Ptr<LteNetDevice> d);

  virtual ~UeLtePhy ();
  static TypeId GetTypeId (void);

  /**
   * \brief Send the packet to the channel
   * \param pb the burst of packet to send
   * \return true if 
   */
  virtual bool SendPacket (Ptr<PacketBurst> pb);

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
   * \brief Create CQI feedbacks from SINR values. SINR values are
   * computed at the physical layer when is received a signal from the eNB
   * \param sinr list of SINR values
   */
  void CreateCqiFeedbacks (std::vector<double> sinr);

  virtual void SendIdealControlMessage (Ptr<IdealControlMessage> msg);
  virtual void ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg);

private:
  std::vector <int> m_subChannelsForTransmission;
  std::vector <int> m_subChannelsForReception;
};


}

#endif /* UE_PHY_H */
