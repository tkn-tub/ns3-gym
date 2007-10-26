/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
 * Author: Mathieu Lacage, <mathieu.lacage@sophia.inria.fr>
 */
#ifndef WIFI_CHANNEL_H
#define WIFI_CHANNEL_H

#include <vector>
#include <stdint.h>
#include "ns3/packet.h"
#include "ns3/channel.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"

namespace ns3 {

class NetDevice;
class PropagationLossModel;
class PropagationDelayModel;

/**
 * \brief A 802.11 Channel
 *
 * This channel subclass can be used to connect together a set of 
 * ns3::WifiNetDevice network interfaces. A WifiChannel contains
 * a PropagationLossModel and a PropagationDelayModel which can
 * be overriden by the WifiChannel::SetPropagationLossModel
 * and the WifiChannel::SetPropagationDelayModel methods. By default,
 * The PropagationDelayModel is a ns3::ConstantSpeedPropagationModel,
 * and the PropagationLossModel is a ns3::PathLossPropagationModel.
 */
class WifiChannel : public Channel
{
public:
  /**
   * arg1: the packet to receive
   * arg2: the rx power of the packet to receive (dbm)
   * arg3: the tx mode of the packet to receive
   * arg4: the preamble of the packet to receive
   */
  typedef Callback<void,Packet,double,WifiMode,WifiPreamble> ReceiveCallback;
  WifiChannel ();
  virtual ~WifiChannel ();

  /**
   * \returns the number of network interfaces connected to 
   *          this channel.
   *
   * Overriden from the NetDevice base class.
   */
  virtual uint32_t GetNDevices (void) const;
  /**
   * \param i index of the requested network interface.
   * \returns the requested network interfaces connected to 
   *          this channel.
   *
   * Overriden from the NetDevice base class.
   * Indexes start at 0 and end at n-1.
   */
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const ;

  /**
   * \param loss the new propagation loss model.
   */
  void SetPropationLossModel (Ptr<PropagationLossModel> loss);
  /**
   * \param loss the new propagation delay model.
   */
  void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay);

  /**
   * \param device the device to add to the list of connected
   *        devices.
   * \param callback the callback to invoke when a packet must
   *        be received.
   *
   * This method should not be invoked by normal users. It is 
   * currently invoked only from WifiPhy::SetChannel.
   */
  void Add (Ptr<NetDevice> device,  ReceiveCallback callback);
  /**
   * \param sender the device from which the packet is originating.
   * \param packet the packet to send
   * \param txPowerDbm the tx power associated to the packet
   * \param wifiMode the tx mode associated to the packet
   * \param preamble the preamble associated to the packet
   *
   * This method should not be invoked by normal users. It is 
   * currently invoked only from WifiPhy::Send.
   */
  void Send (Ptr<NetDevice> sender, const Packet &packet, double txPowerDbm,
             WifiMode wifiMode, WifiPreamble preamble) const;

private:
  typedef std::vector<std::pair<Ptr<NetDevice>, ReceiveCallback> > DeviceList;
  void Receive (uint32_t i, const Packet &packet, double rxPowerDbm,
                WifiMode txMode, WifiPreamble preamble) const;

  DeviceList m_deviceList;
  Ptr<PropagationLossModel> m_loss;
  Ptr<PropagationDelayModel> m_delay;
};

} // namespace ns3


#endif /* WIFI_CHANNEL_H */
