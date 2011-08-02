/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UAN_CHANNEL_H
#define UAN_CHANNEL_H

#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/uan-prop-model.h"
#include "ns3/uan-noise-model.h"

#include <list>
#include <vector>

namespace ns3 {

class UanNetDevice;
class UanPhy;
class UanTransducer;
class UanTxMode;

/**
 * \class UanChannel
 * \brief Channel class used by UAN devices
 */
class UanChannel : public Channel
{
public:
  /**
   * \brief UanDeviceList is a standard template vector of pairs (UanNetDevice, UanTransducer)
   */
  typedef std::vector<std::pair<Ptr<UanNetDevice>, Ptr<UanTransducer> > > UanDeviceList;


  UanChannel ();
  virtual ~UanChannel ();

  static TypeId GetTypeId ();

  // Methods inherrited from base class
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  /**
   * \param src Transducer transmitting packet
   * \param packet Packet to be transmitted
   * \param txPowerDb Transmission power in dB
   * \param txmode UanTxMode defining modulation of transmitted packet
   * Send a packet out on the channel
   */
  void TxPacket  (Ptr<UanTransducer> src, Ptr<Packet> packet, double txPowerDb,
                  UanTxMode txmode);

  /**
   * \param dev Net Device of node
   * \param trans Transducer of net device attached to this channel
   *
   * Adds device to receiver list for this channel
   */
  void AddDevice (Ptr<UanNetDevice> dev, Ptr<UanTransducer> trans);

  /**
   * \param prop Propagation model this channel will use for path loss/propagation delay
   */
  void SetPropagationModel (Ptr<UanPropModel> prop);

  /**
   * \param noise Noise model this channel will use to determine ambient channel noise.
   *
   */
  void SetNoiseModel  (Ptr<UanNoiseModel> noise);

  /**
   * \param fKhz Frequency in kHz
   * \returns Ambient noise in dB/Hz on channel at a frequency
   */
  double GetNoiseDbHz (double fKhz);

  /**
   * Clears all pointer references
   */
  void Clear (void);

private:
  UanDeviceList m_devList;
  Ptr<UanPropModel> m_prop;
  Ptr<UanNoiseModel> m_noise;
  bool m_cleared;

  void SendUp (uint32_t i, Ptr<Packet> packet, double rxPowerDb, UanTxMode txMode, UanPdp pdp);
protected:
  virtual void DoDispose ();
};

}

#endif /* UAN_CHANNEL_H */
