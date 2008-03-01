/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 */

#ifndef WIFI_NET_DEVICE_H
#define WIFI_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/mac48-address.h"
#include "wifi-remote-station-manager.h"
#include <string>

namespace ns3 {

class WifiChannel;
class WifiPhy;
class WifiMac;

/**
 * \brief the base class for 802.11 network interfaces
 *
 */
class WifiNetDevice : public NetDevice 
{
public:
  static TypeId GetTypeId (void);

  WifiNetDevice ();
  virtual ~WifiNetDevice ();

  
  void Setup (Ptr<Node> node, Ptr<WifiMac> mac, Ptr<WifiPhy> phy,
              Ptr<WifiRemoteStationManager> manager,
              Ptr<WifiChannel> channel);
  Ptr<WifiMac> GetMac (void) const;
  Ptr<WifiPhy> GetPhy (void) const;
  Ptr<WifiRemoteStationManager> GetRemoteStationManager (void) const;


  // inherited from NetDevice base class.
  virtual void SetName(const std::string name);
  virtual std::string GetName(void) const;
  virtual void SetIfIndex(const uint32_t index);
  virtual uint32_t GetIfIndex(void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void SetLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (void) const;
  virtual Address MakeMulticastAddress (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

private:
  void ForwardUp (Ptr<Packet> packet, const Mac48Address &from);
  void LinkUp (void);
  void LinkDown (void);
  Ptr<Node> m_node;
  Ptr<WifiPhy> m_phy;
  Ptr<WifiMac> m_mac;
  Ptr<WifiRemoteStationManager> m_stationManager;
  Callback <bool,Ptr<NetDevice>,Ptr<Packet>,uint16_t,const Address &> m_forwardUp;
  TracedCallback<Ptr<const Packet>, Mac48Address> m_rxLogger;
  TracedCallback<Ptr<const Packet>, Mac48Address> m_txLogger;
  uint32_t m_ifIndex;
  std::string m_name;
  bool m_linkUp;
  Callback<void> m_linkChange;
  mutable uint16_t m_mtu;
};

} // namespace ns3

#endif /* WIFI_NET_DEVICE_H */
