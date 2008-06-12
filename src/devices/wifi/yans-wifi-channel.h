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
#ifndef YANS_WIFI_CHANNEL_H
#define YANS_WIFI_CHANNEL_H

#include <vector>
#include <stdint.h>
#include "ns3/packet.h"
#include "wifi-channel.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"

namespace ns3 {

class NetDevice;
class WifiNetDevice;
class PropagationLossModel;
class PropagationDelayModel;
class YansWifiPhy;

/**
 * \brief A Yans wifi channel
 *
 * This wifi channel implements the propagation model described in
 * "Yet Another Network Simulator", (http://cutebugs.net/files/wns2-yans.pdf).
 *
 * This class is expected to be used in tandem with the ns3::YansWifiPhy 
 * class and contains a ns3::PropagationLossModel and a ns3::PropagationDelayModel.
 * By default, no propagation models are set so, it is the caller's responsability
 * to set them before using the channel.
 */
class YansWifiChannel : public WifiChannel
{
public:
  static TypeId GetTypeId (void);

  YansWifiChannel ();
  virtual ~YansWifiChannel ();

  // inherited from Channel.
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  // inherited from WifiChannel
  virtual Ptr<WifiPhy> CreatePhy (Ptr<WifiNetDevice> device,
                                  Ptr<Object> mobility,
                                  UnsafeAttributeList list);

  /**
   * \param loss the new propagation loss model.
   */
  void SetPropagationLossModel (Ptr<PropagationLossModel> loss);
  /**
   * \param delay the new propagation delay model.
   */
  void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay);

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
  void Send (Ptr<YansWifiPhy> sender, Ptr<const Packet> packet, double txPowerDbm,
             WifiMode wifiMode, WifiPreamble preamble) const;

private:
  struct Item {
    Ptr<WifiNetDevice> device;
    Ptr<YansWifiPhy> phy;
    Ptr<Object> mobility;
  };
  typedef std::vector<struct Item> DeviceList;
  void Receive (uint32_t i, Ptr<Packet> packet, double rxPowerDbm,
                WifiMode txMode, WifiPreamble preamble) const;


  DeviceList m_deviceList;
  Ptr<PropagationLossModel> m_loss;
  Ptr<PropagationDelayModel> m_delay;
};

} // namespace ns3


#endif /* YANS_WIFI_CHANNEL_H */
