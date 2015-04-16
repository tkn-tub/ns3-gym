/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *         Junling Bu <linlinjavaer@gmail.com>
 */
#ifndef WAVE_MAC_LOW_H
#define WAVE_MAC_LOW_H

#include "ns3/mac-low.h"
#include "wave-net-device.h"

namespace ns3 {
class WaveNetDevice;

/**
 * \ingroup wave
 * This class is the subclass of MacLow to provide support for MAC extension
 * (1) allows higher layer control data rate and tx power level.
 *     If higher layer does not set, they will be determined by WifiRemoteStationManager
 *          of MAC layer;
 *     If higher layer sets tx parameters in non-adaptable mode, the data rate
 *        and tx power level will be used for transmission;.
 *    If higher layer sets tx parameters in adaptable mode, the data rate
 *        will be lower bound for the actual data rate, and the power level
 *        will be upper bound for the actual transmit power.
 * (2) implements the feature described in Annex C : avoid transmission at scheduled guard intervals
 *      However, the feature is extended further here that the required transmit time is determined
 *      by MAC layer itself rather than PHY layer, which contains RTS/CTS, DATA and ACK time.
 */
class WaveMacLow : public MacLow
{
public:
  static TypeId GetTypeId (void);
  WaveMacLow ();
  virtual ~WaveMacLow ();

  /**
   * \param device WaveNetDevice associated with WaveMacLow
   */
  void SetWaveNetDevice (Ptr<WaveNetDevice> device);

  /**
   * \param packet packet to send
   * \param hdr 802.11 header for packet to send
   * \param parameters the transmission parameters to use for this packet.
   * \param listener listen to transmission events.
   *
   * Start the transmission of the input packet and notify the listener
   * of transmission events.
   */
  virtual void StartTransmission (Ptr<const Packet> packet,
                                  const WifiMacHeader* hdr,
                                  MacLowTransmissionParameters parameters,
                                  MacLowTransmissionListener *listener);
private:
  /**
   * Return a TXVECTOR for the DATA frame given the destination.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param packet the packet being asked for TXVECTOR
   * \param hdr the WifiMacHeader
   * \return TXVECTOR for the given packet
   */
  virtual WifiTxVector GetDataTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;

  Ptr<ChannelScheduler> m_scheduler;
  Ptr<ChannelCoordinator> m_coordinator;
};

} // namespace ns3

#endif /* WAVE_MAC_LOW_H*/
