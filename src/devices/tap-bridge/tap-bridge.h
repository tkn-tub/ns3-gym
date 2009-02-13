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

#include <string.h>
#include "ns3/address.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ptr.h"
#include "ns3/mac48-address.h"
#include "ns3/system-thread.h"

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
 * /dev/tap device that we create for it, the write is redirected into the TapBridge
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
 * this Tap Bridge.  This node will not be able to actually do anything else in the 
 * simulation with respect to the Tap Bridge and its bridged net device.  This is 
 * because:
 *
 * - Bits sent to the Tap Bridge using its Send() method are completely ignored.  
 *   The Tap Bridge is not, itself, connected to any network.
 * - The bridged ns-3 net device is has had its receive callback disconnected from
 *   the ns-3 node and reconnected to the Tap Bridge.  All data received by a 
 *   bridged device will be sent to the Linux host and will not be received by the
 *   node.  You can send but you cannot ever receive.
 * 
 * You will be able to perform typical ns-3 operations on the ghost node if you so
 * desire.  The internet stack, for example, must be there and functional on that
 * node in order to participate in IP address assignment and global routing.
 * However, interfaces talking any Tap Bridge or associated bridged net devices 
 * will not work completely.  If you understand exactly what you are doing, you 
 * can set up other interfaces and devices on the ghost node and use them; but we 
 * generally recommend that you treat this node as a ghost of the Linux host and 
 * leave it alone.
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

  /** \brief Get the bridged net device.
   *
   * The bridged net device is the ns-3 device to which this bridge is connected,
   *
   * \returns the bridged net device.
   */
  Ptr<NetDevice> GetBridgedNetDevice (void);

  /** \brief Set the ns-3 net device to bridge.
   *
   * This method tells the bridge which ns-3 net device it should use to connect
   * the simulation side of the bridge.  
   *
   * \attention The ns-3 net device that is being set as the device must have an
   * an IP address assigned to it before the simulation is run.  This address 
   * will be used to set the hardware address of the host Linux device.
   */
  void SetBridgedNetDevice (Ptr<NetDevice> bridgedDevice);

  /**
   * \brief Set a start time for the device.
   *
   * The tap bridge consumes a non-trivial amount of time to start.  It starts
   * up in the context of a scheduled event to ensure that all configuration
   * has been completed before extracting the configuration (IP addresses, etc.)
   * In order to allow a more reasonable start-up sequence than a thundering 
   * herd of devices, the time at which each device starts is also configurable
   * bot via the Attribute system and via this call.
   *
   * \param tStart the start time
   */
  void Start (Time tStart);

  /**
   * Set a stop time for the device.
   *
   * @param tStop the stop time
   *
   * \see TapBridge::Start
   */
  void Stop (Time tStop);

  //
  // The following methods are inherited from NetDevice base class and are
  // documented there.
  //
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
  /**
   * \internal
   *
   * Call out to a separate process running as suid root in order to get our
   * tap device created.  We do this to avoid having the entire simulation 
   * running as root.  If this method returns, we'll have a socket waiting 
   * for us in m_sock that we can use to talk to the tap device.
   */
  virtual void DoDispose (void);

  void ReceiveFromBridgedDevice (Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                                 Address const &src, Address const &dst, PacketType packetType);
private:

  /**
   * \internal
   *
   * Call out to a separate process running as suid root in order to get our
   * tap device created.  We do this to avoid having the entire simulation 
   * running as root.  If this method returns, we'll have a socket waiting 
   * for us in m_sock that we can use to talk to the tap device.
   */
  void CreateTap (void);

  /**
   * \internal
   *
   * Figure out where the tap creation program lives on the system.
   *
   * \param creatorName The name of the program used to create the Tap.
   * \returns A path name to use when you want to create a Tap.
   */
  std::string FindCreator (std::string creatorName);

  /**
   * \internal
   *
   * Spin up the device
   */
  void StartTapDevice (void);

  /**
   * \internal
   *
   * Tear down the device
   */
  void StopTapDevice (void);

  /**
   * \internal
   *
   * Loop to read and process packets
   */
  void ReadThread (void);

  /*
   * \internal
   *
   * Forward a packet received from the tap device to the bridged ns-3 
   * device
   *
   * \param buf A character buffer containing the actaul packet bits that were
   *            received from the host.
   * \param buf The length of the buffer.
   */
  void ForwardToBridgedDevice (uint8_t *buf, uint32_t len);

  /**
   * \internal
   *
   * The host we are bridged to is in the evil real world.  Do some sanity
   * checking on a received packet to make sure it isn't too evil for our
   * poor naive virginal simulator to handle.
   *
   * \param packet The packet we received from the host, and which we need 
   *               to check.
   * \param src    A pointer to the data structure that will get the source
   *               MAC address of the packet (extracted from the packet Ethernet
   *               header).
   * \param dst    A pointer to the data structure that will get the destination
   *               MAC address of the packet (extracted from the packet Ethernet 
   *               header).
   * \param type   A pointer to the variable that will get the packet type from 
   *               either the Ethernet header in the case of type interpretation
   *               (DIX framing) or from the 802.2 LLC header in the case of 
   *               length interpretation (802.3 framing).
   */   
  Ptr<Packet> Filter (Ptr<Packet> packet, Address *src, Address *dst, uint16_t *type);

  /**
   * \internal
   *
   * Callback used to hook the standard packet receive callback of the TapBridge
   * ns-3 net device.  This is never called, and therefore no packets will ever
   * be received forwarded up the IP stack on the ghost node through this device.
   */
  NetDevice::ReceiveCallback m_rxCallback;

  /**
   * \internal
   *
   * Callback used to hook the promiscuous packet receive callback of the TapBridge
   * ns-3 net device.  This is never called, and therefore no packets will ever
   * be received forwarded up the IP stack on the ghost node through this device.
   *
   * Note that we intercept the similar callback in the bridged device in order to
   * do the actual bridging between the bridged ns-3 net device and the Tap device
   * on the host.
   */
  NetDevice::PromiscReceiveCallback m_promiscRxCallback;

  /**
   * \internal
   *
   * Pointer to the (ghost) Node to which we are connected.
   */
  Ptr<Node> m_node;

  /**
   * \internal
   *
   * A possible name for the (ghost) Node to which we are connected.
   */
  std::string m_name;

  /**
   * \internal
   *
   * The ns-3 interface index of this TapBridge net device.
   */
  uint32_t m_ifIndex;

  /**
   * \internal
   *
   * The common mtu to use for the net devices
   */
  uint16_t m_mtu;

  /**
   * \internal
   *
   * The socket (actually interpreted as fd) to use to talk to the Tap device on
   * the real internet host.
   */
  int32_t m_sock;

  /**
   * \internal
   *
   * The ID of the ns-3 event used to schedule the start up of the underlying
   * host Tap device and ns-3 read thread.
   */
  EventId m_startEvent;

  /**
   * \internal
   *
   * The ID of the ns-3 event used to schedule the tear down of the underlying
   * host Tap device and ns-3 read thread.
   */
  EventId m_stopEvent;

  /**
   * \internal
   *
   * Used to identify the ns-3 read thread used to do blocking reads on the 
   * socket (fd) corresponding to the host device.
   */
  Ptr<SystemThread> m_readThread;

  /**
   * \internal
   *
   * The (unused) MAC address of the TapBridge net device.  Since the TapBridge
   * is implemented as a ns-3 net device, it is required to implement certain
   * functionality.  In this case, the TapBridge is automatically assigned a
   * MAC address, but it is not used.  The MAC address assigned to the internet
   * host actually comes from the bridged (N.B. the "ed") device and not from 
   * the bridge device.
   */
  Mac48Address m_address;

  /**
   * \internal
   *
   * Time to start spinning up the device
   */
  Time m_tStart;

  /**
   * \internal
   *
   * Time to start tearing down the device
   */
  Time m_tStop;

  /**
   * \internal
   *
   * The name of the device to create on the host.  If the device name is the
   * empty string, we allow the host kernel to choose a name.
   */
  std::string m_tapDeviceName;

  /**
   * \internal
   *
   * The IP address to use as the device default gateway on the host.
   */
  Ipv4Address m_tapGateway;

  /**
   * \internal
   *
   * The IP address to use as the device IP on the host.
   */
  Ipv4Address m_tapIp;
  /**
   * \internal
   *
   * The MAC address to use as the hardware address on the host.
   */
  Mac48Address m_tapMac;

  /**
   * \internal
   *
   * The network mask to assign to the device created on the host.
   */
  Ipv4Mask m_tapNetmask;

  /**
   * \internal
   *
   * The ns-3 net device to which we are bridging.
   */
  Ptr<NetDevice> m_bridgedDevice;
};

} // namespace ns3

#endif /* TAP_BRIDGE_H */
