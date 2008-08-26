/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Modified by Emmanuelle Laprise to remove dependance on LLC headers
 */
#ifndef NET_DEVICE_H
#define NET_DEVICE_H

#include <string>
#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "address.h"
#include "ipv4-address.h"

namespace ns3 {

class Node;
class Channel;
class Packet;

/**
 * \ingroup node
 * \defgroup netdevice NetDevice
 */
/**
 * \ingroup netdevice
 *
 * \brief Network layer to device interface
 *
 * This interface defines the API which the IP and ARP
 * layers need to access to manage an instance of a network device 
 * layer. It currently does not support MAC-level 
 * multicast but this should not be too hard to add by adding
 * extra methods to register MAC multicast addresses to
 * filter out unwanted packets before handing them to the
 * higher layers.
 *
 * In Linux, this interface is analogous to the interface
 * just above dev_queue_xmit() (i.e., IP packet is fully
 * constructed with destination MAC address already selected).
 * 
 * If you want to write a new MAC layer, you need to subclass
 * this base class and implement your own version of the
 * NetDevice::SendTo method.
 */
class NetDevice : public Object
{
public:
  static TypeId GetTypeId (void);
  virtual ~NetDevice();

  /**
   * \param name name of the device (e.g. "eth0")
   */
  virtual void SetName(const std::string name) = 0;
  /**
   * \return name name of the device (e.g. "eth0")
   */
  virtual std::string GetName(void) const = 0;
  /**
   * \param index ifIndex of the device 
   */
  virtual void SetIfIndex(const uint32_t index) = 0;
  /**
   * \return index ifIndex of the device 
   */
  virtual uint32_t GetIfIndex(void) const = 0;


  /**
   * \return the channel this NetDevice is connected to. The value
   *         returned can be zero if the NetDevice is not yet connected
   *         to any channel.
   */
  virtual Ptr<Channel> GetChannel (void) const = 0;

  /**
   * \return the current Address of this interface.
   */
  virtual Address GetAddress (void) const = 0;
  /**
   * \param mtu MTU value, in bytes, to set for the device
   * \return whether the MTU value was within legal bounds
   * 
   * Override for default MTU defined on a per-type basis.
   */
  virtual bool SetMtu (const uint16_t mtu) = 0;
  /**
   * \return the link-level MTU in bytes for this interface.
   * 
   * This value is typically used by the IP layer to perform
   * IP fragmentation when needed.
   */
  virtual uint16_t GetMtu (void) const = 0;
  /**
   * \return true if link is up; false otherwise
   */
  virtual bool IsLinkUp (void) const = 0;
  /**
   * \param callback the callback to invoke
   *
   * Register a callback invoked whenever the link 
   * status changes to UP. This callback is typically used
   * by the IP/ARP layer to flush the ARP cache 
   * whenever the link goes up.
   */
  virtual void SetLinkChangeCallback (Callback<void> callback) = 0;
  /**
   * \return true if this interface supports a broadcast address,
   *         false otherwise.
   */
  virtual bool IsBroadcast (void) const = 0;
  /**
   * \return the broadcast address supported by
   *         this netdevice.
   *
   * Calling this method is invalid if IsBroadcast returns
   * not true.
   */
  virtual Address GetBroadcast (void) const = 0;

  /**
   * \return value of m_isMulticast flag
   */
  virtual bool IsMulticast (void) const = 0;

  /**
   * \brief Return the MAC multicast base address used when mapping multicast
   * groups to MAC multicast addresses.
   *
   * Typically when one constructs a multicast MAC addresses, some bits from
   * the IP multicast group are copied into a corresponding MAC multicast 
   * group.  In EUI-48, for example, the low order 23 bits of the multicast
   * group are copied to the MAC multicast group base address.
   *
   * This method allows access to the underlying MAC multicast group base 
   * address.  It is expected that in most cases, a net device client will
   * allow the net device to perform the actual construction of the multicast
   * address.  Use of this method is discouraged unless you have a good reason
   * to perform a custom mapping.  You should prefer 
   * NetDevice::MakeMulticastAddress which will do the RFC-specified mapping
   * for the net device in question.
   *
   * \return The multicast address supported by this net device.
   *
   * \warning Calling this method is invalid if IsMulticast returns not true.
   * The method NS_ASSERTs if the device is not a multicast device.
   * \see NetDevice::MakeMulticastAddress
   */
  virtual Address GetMulticast (void) const = 0;
  
  /**
   * \brief Make and return a MAC multicast address using the provided
   *        multicast group
   *
   * RFC 1112 says that an Ipv4 host group address is mapped to an Ethernet 
   * multicast address by placing the low-order 23-bits of the IP address into 
   * the low-order 23 bits of the Ethernet multicast address 
   * 01-00-5E-00-00-00 (hex).  Similar RFCs exist for Ipv6 and Eui64 mappings.
   * This method performs the multicast address creation function appropriate
   * to the underlying MAC address of the device.  This MAC address is
   * encapsulated in an abstract Address to avoid dependencies on the exact
   * MAC address format.
   *
   * A default imlementation of MakeMulticastAddress is provided, but this
   * method simply NS_ASSERTS.  In the case of net devices that do not support
   * multicast, clients are expected to test NetDevice::IsMulticast and avoid
   * attempting to map multicast packets.  Subclasses of NetDevice that do
   * support multicasting are expected to override this method and provide an
   * implementation appropriate to the particular device.
   *
   * \param multicastGroup The IP address for the multicast group destination
   * of the packet.
   * \return The MAC multicast Address used to send packets to the provided
   * multicast group.
   *
   * \warning Calling this method is invalid if IsMulticast returns not true.
   * \see Ipv4Address
   * \see Address
   * \see NetDevice::IsMulticast
   */
  virtual Address MakeMulticastAddress (Ipv4Address multicastGroup) const = 0;
  
  /**
   * \return value of m_isPointToPoint flag
   */
  virtual bool IsPointToPoint (void) const = 0;
  /**
   * \param packet packet sent from above down to Network Device
   * \param dest mac address of the destination (already resolved)
   * \param protocolNumber identifies the type of payload contained in
   *        this packet. Used to call the right L3Protocol when the packet
   *        is received.
   * 
   *  Called from higher layer to send packet into Network Device
   *  to the specified destination Address
   * 
   * \return whether the Send operation succeeded 
   */
  virtual bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) = 0;
  /**
   * \param packet packet sent from above down to Network Device
   * \param source source mac address (so called "MAC spoofing")
   * \param dest mac address of the destination (already resolved)
   * \param protocolNumber identifies the type of payload contained in
   *        this packet. Used to call the right L3Protocol when the packet
   *        is received.
   * 
   *  Called from higher layer to send packet into Network Device
   *  with the specified source and destination Addresses.
   * 
   * \return whether the Send operation succeeded 
   */
  virtual bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber) = 0;
  /**
   * \returns the node base class which contains this network
   *          interface.
   *
   * When a subclass needs to get access to the underlying node
   * base class to print the nodeid for example, it can invoke
   * this method.
   */
  virtual Ptr<Node> GetNode (void) const = 0;

  /**
   * \param node the node associated to this netdevice.
   *
   * This method is called from ns3::Node::AddDevice.
   */
  virtual void SetNode (Ptr<Node> node) = 0;

  /**
   * \returns true if ARP is needed, false otherwise.
   *
   * Called by higher-layers to check if this NetDevice requires
   * ARP to be used.
   */
  virtual bool NeedsArp (void) const = 0;


  /** Packet types */
  enum PacketType
    {
      PACKET_HOST = 1,  /* To us                */
      PACKET_BROADCAST, /* To all               */
      PACKET_MULTICAST, /* To group             */
      PACKET_OTHERHOST, /* To someone else      */
    };

  /**
   * \param device a pointer to the net device which is calling this callback
   * \param packet the packet received
   * \param protocol the 16 bit protocol number associated with this packet.
   *        This protocol number is expected to be the same protocol number
   *        given to the Send method by the user on the sender side.
   * \param sender the address of the sender
   * \returns true if the callback could handle the packet successfully, false
   *          otherwise.
   */
  typedef Callback<bool,Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &> ReceiveCallback;

  /**
   * \param cb callback to invoke whenever a packet has been received and must
   *        be forwarded to the higher layers.
   *
   */
  virtual void SetReceiveCallback (ReceiveCallback cb) = 0;


  /**
   * \param device a pointer to the net device which is calling this callback
   * \param packet the packet received
   * \param protocol the 16 bit protocol number associated with this packet.
   *        This protocol number is expected to be the same protocol number
   *        given to the Send method by the user on the sender side.
   * \param sender the address of the sender
   * \param receiver the address of the receiver
   * \param packetType type of packet received (broadcast/multicast/unicast/otherhost)
   * \returns true if the callback could handle the packet successfully, false
   *          otherwise.
   */
  typedef Callback< bool, Ptr<NetDevice>, Ptr<const Packet>, uint16_t,
                    const Address &, const Address &, enum PacketType > PromiscReceiveCallback;

  /**
   * \param cb callback to invoke whenever a packet has been received in promiscuous mode and must
   *        be forwarded to the higher layers.
   * 
   * Enables netdevice promiscuous mode and sets the callback that
   * will handle promiscuous mode packets.  Note, promiscuous mode
   * packets means _all_ packets, including those packets that can be
   * sensed by the netdevice but which are intended to be received by
   * other hosts.
   */
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb) = 0;

  /**
   * \return true if this interface supports a promiscuous mode, false otherwise.
   */
  virtual bool SupportsPromiscuous (void) const = 0;

};

} // namespace ns3

#endif /* NET_DEVICE_H */
