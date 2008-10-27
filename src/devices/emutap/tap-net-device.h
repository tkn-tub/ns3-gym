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
 */
#ifndef TAP_NET_DEVICE_H
#define TAP_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include <stdint.h>
#include <string>

namespace ns3 {

class Node;
class SystemThread;
class TapChannel;

/**
 * \ingroup netdevice
 * 
 * \brief a NetDevice to get packets to and from a host tap device.
 */
class TapNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);
  TapNetDevice ();

  void SetAddress (Mac48Address address);

  void SetChannel (Ptr<TapChannel> channel);

  void SetupHost (Ipv4Address ad, Ipv4Mask mask, Ipv4Address gateway);

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
  virtual bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

protected:
  virtual void DoDispose (void);
private:
  void Receive (Ptr<Packet> packet, uint16_t protocol, 
                Mac48Address to, Mac48Address from);
  void ForwardUp (uint8_t *buffer, uint32_t size);
  void ReadThread (void);

  NetDevice::ReceiveCallback m_rxCallback;
  NetDevice::PromiscReceiveCallback m_promiscCallback;
  Ptr<Node> m_node;
  uint16_t m_mtu;
  std::string m_name;
  uint32_t m_ifIndex;
  Mac48Address m_address;
  int m_tap;
  Ptr<SystemThread> m_thread;
  TracedCallback<Ptr<const Packet>,Mac48Address,Mac48Address> m_rxTrace;
  TracedCallback<Ptr<const Packet>,Mac48Address,Mac48Address> m_txTrace;
  TracedCallback<Ptr<const Packet>,Mac48Address,Mac48Address> m_dropTrace;
  Ptr<TapChannel> m_channel;
};

} // namespace ns3

#endif /* TAP_NET_DEVICE_H */
