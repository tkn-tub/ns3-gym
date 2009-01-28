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
 */

#ifndef TAP_BRIDGE_H
#define TAP_BRIDGE_H

#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"

namespace ns3 {

class Node;

/**
 * \ingroup devices
 * \defgroup tap-bridge TapBridge
 * 
 * \brief A bridge to make it appear that a host is connected to an ns-3 net device.
 *
 * The Tap Bridge lives in a kind of a gray world somewhere between a Linux host and
 * an ns-3 bridge device.  From the Linux perspective, this code appears as the user
 * mode handler for a Tap net device.  That is, when the Linux host writes to the
 * /dev/tapx device that we create for it, the write is redirected into the TapBridge
 * and from that perspective, becomes a read.  The TapBridge then redirects the data
 * written (by the Linux host) to the tap device on out the ns-3 net device to which
 * we are bridged.  When a packet comes in from the ns-3 world to the ns-3 net device
 * we are bridging, it appears via a callback from that net device.  Our job is to
 * take those bits and write them back to the host using the user mode handler for
 * /dev/tapx.  This write to the device will then appear to the Linux host as if a 
 * packet has arrived on its device.
 *
 * The upshot is that the Tap Bridge appears to bridge a tap device on a Linux host 
 * in the "real world" to an ns-3 net device in the simulation.  In order to do this
 * we need a "ghost node" in the simulation to hold the bridged ns-3 net device and 
 * this Tap Bridge.  It won't actually do anything else in the simulation.  You will
 * be able to perform typical ns-3 operations on that node, but they will have no
 * effect other than to set up, tear down and configure the net devices and bridges
 * mentioned above.
 */

/**
 * \ingroup tap-bridge
 * \brief A bridge to make it appear that a host is connected to an ns-3 net device.
 */

class TapBridge : public NetDevice
{
public:
  static TypeId GetTypeId (void);

  TapBridge ();
  virtual ~TapBridge ();

  /** \brief Set the device to bridge.
   *
   * This method tells the bridge which ns-3 net device it should use to connect
   * the simulation side of the bridge.  
   *
   * \attention The ns-3 net device that is being set as the device must not 
   * have an IP address.  This address is a property of the host Linux device.
   */
  void SetBridgedDevice (Ptr<NetDevice> bridgedDevice);

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
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom () const;
  virtual Address GetMulticast (Ipv6Address addr) const;

protected:
  virtual void DoDispose (void);

  void ReceiveFromBridgedDevice (Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                                 Address const &src, Address const &dst, PacketType packetType);

private:

  /**
   * Call out to a separate process running as suid root in order to get our
   * tap device created.  We do this to avoid having the entire simulation 
   * running as root.  If this method returns, we'll have a socket waiting 
   * for us in m_sock that we can use to talk to the tap device.
   */
  void CreateTap (void);

  /**
   * Figure out where the tap creation program lives on the system.
   */
  std::string FindCreator (void);

  NetDevice::ReceiveCallback m_rxCallback;
  NetDevice::PromiscReceiveCallback m_promiscRxCallback;

  Mac48Address m_address;
  Ptr<Node> m_node;
  std::string m_name;
  uint32_t m_ifIndex;
  uint16_t m_mtu;

  int32_t m_sock;

  std::string m_tapDeviceName;
  std::string m_tapGateway;
  std::string m_tapIp;
  std::string m_tapMac;
  std::string m_tapNetmask;

  Ptr<NetDevice> m_bridgedDevice;
};

} // namespace ns3

#endif /* TAP_BRIDGE_H */
