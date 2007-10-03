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
#include "ns3/wifi-mode.h"

namespace ns3 {

class NetDevice;
class PropagationLossModel;
class PropagationDelayModel;

class WifiChannel : public Channel
{
public:
  typedef Callback<void,Packet,double,WifiMode,WifiMode,uint32_t> ReceiveCallback;
  WifiChannel ();
  virtual ~WifiChannel ();

  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const ;

  void SetPropationLossModel (Ptr<PropagationLossModel> loss);
  void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay);

  void Add (Ptr<NetDevice> device,  ReceiveCallback callback);
  void Send (Ptr<NetDevice> sender, const Packet &packet, double txPowerDbm,
             WifiMode wifiMode, WifiMode headerMode, uint32_t extra) const;

private:
  typedef std::vector<std::pair<Ptr<NetDevice>, ReceiveCallback> > DeviceList;
  struct ReceiveData {
    ReceiveData (const Packet &packet, double rxPowerDbm,
                 WifiMode txMode, WifiMode headerMode, uint32_t extra);
    Packet m_packet;
    double m_rxPowerDbm;
    WifiMode m_wifiMode;
    WifiMode m_headerMode;
    uint32_t m_extra;
  };
  void Receive (uint32_t i, 
                const struct ReceiveData &data) const;

  DeviceList m_deviceList;
  Ptr<PropagationLossModel> m_loss;
  Ptr<PropagationDelayModel> m_delay;
};

} // namespace ns3


#endif /* WIFI_CHANNEL_H */
