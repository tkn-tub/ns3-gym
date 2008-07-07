/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Gustavo Carneiro  <gjc@inescporto.pt>
 */
#ifndef BRIDGE_NET_DEVICE_H
#define BRIDGE_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"
#include <stdint.h>
#include <string>
#include <map>

namespace ns3 {

class Node;

/**
 * \ingroup netdevice
 * 
 * \brief bridge net device for bridge things and testing
 */
class BridgeNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);
  BridgeNetDevice ();

  void AddBridgePort (Ptr<NetDevice> bridgePort);

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
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

protected:
  virtual void DoDispose (void);

  void ReceiveFromDevice (Ptr<NetDevice> device, Ptr<Packet> packet, uint16_t protocol,
                          Address const &source, Address const &destination, PacketType packetType);
  void ForwardUnicast (Ptr<NetDevice> incomingPort, Ptr<Packet> packet,
                       uint16_t protocol, Mac48Address src, Mac48Address dst);
  void ForwardBroadcast (Ptr<NetDevice> incomingPort, Ptr<Packet> packet,
                         uint16_t protocol, Mac48Address src, Mac48Address dst);
  void Learn (Mac48Address source, Ptr<NetDevice> port);
  Ptr<NetDevice> GetLearnedState (Mac48Address source);

private:
  NetDevice::ReceiveCallback m_rxCallback;

  Mac48Address m_address;
  Time m_expirationTime; // time it takes for learned MAC state to expire
  struct LearnedState
  {
    Ptr<NetDevice> associatedPort;
    Time expirationTime;
  };
  std::map<Mac48Address, LearnedState> m_learnState;
  Ptr<Node> m_node;
  std::string m_name;
  std::vector< Ptr<NetDevice> > m_ports;
  uint32_t m_ifIndex;
  uint16_t m_mtu;
  bool m_enableLearning;
};

} // namespace ns3

#endif /* BRIDGE_NET_DEVICE_H */
