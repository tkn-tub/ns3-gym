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
 * Modified by Emmanuelle Laprise to remove dependence on LLC headers
 * Modified by Stefano Avallone to add NetDeviceQueue and NetDeviceQueueInterface
 */
#ifndef NET_DEVICE_H
#define NET_DEVICE_H

#include <string>
#include <vector>
#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"

namespace ns3 {

class Node;
class Channel;
class Packet;
class QueueLimits;

/**
 * \ingroup network
 * \defgroup netdevice Network Device
 */

/**
 * \ingroup netdevice
 * \brief Base class to represent items of packet Queues
 *
 * An item stored in an ns-3 packet Queue contains a packet and possibly other
 * information. An item of the base class only contains a packet. Subclasses
 * can be derived from this base class to allow items to contain additional
 * information.
 */
class QueueItem : public SimpleRefCount<QueueItem>
{
public:
  /**
   * \brief Create a queue item containing a packet.
   * \param p the packet included in the created item.
   */
  QueueItem (Ptr<Packet> p);

  virtual ~QueueItem ();

  /**
   * \return the packet included in this item.
   */
  Ptr<Packet> GetPacket (void) const;

  /**
   * \brief Use this method (instead of GetPacket ()->GetSize ()) to get the packet size
   *
   * Subclasses may keep header and payload separate to allow manipulating the header,
   * so using this method ensures that the correct packet size is returned.
   *
   * \return the size of the packet included in this item.
   */
  virtual uint32_t GetPacketSize (void) const;

  /**
   * \enum Uint8Values
   * \brief 1-byte fields of the packet whose value can be retrieved, if present
   */
  enum Uint8Values
    {
      IP_DSFIELD
    };

  /**
   * \brief Retrieve the value of a given field from the packet, if present
   * \param field the field whose value has to be retrieved
   * \param value the output parameter to store the retrieved value
   *
   * \return true if the requested field is present in the packet, false otherwise.
   */
  virtual bool GetUint8Value (Uint8Values field, uint8_t &value) const;

  /**
   * \brief Print the item contents.
   * \param os output stream in which the data should be printed.
   */
  virtual void Print (std::ostream &os) const;

  /**
   * TracedCallback signature for Ptr<QueueItem>
   *
   * \param [in] item The queue item.
   */
  typedef void (* TracedCallback) (Ptr<const QueueItem> item);

private:
  /**
   * \brief Default constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueItem ();
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueItem (const QueueItem &);
  /**
   * \brief Assignment operator
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  QueueItem &operator = (const QueueItem &);

  /**
   * The packet contained in the queue item.
   */
  Ptr<Packet> m_packet;
};

/**
 * \brief Stream insertion operator.
 *
 * \param os the stream
 * \param item the item
 * \returns a reference to the stream
 */
std::ostream& operator<< (std::ostream& os, const QueueItem &item);

/**
 * \ingroup netdevice
 *
 * \brief Network device transmission queue
 *
 * This class stores information about a single transmission queue
 * of a network device that is exposed to queue discs. Such information
 * includes the state of the transmission queue (whether it has been
 * stopped or not) and data used by techniques such as Byte Queue Limits.
 *
 * This class roughly models the struct netdev_queue of Linux.
 */
class NetDeviceQueue : public SimpleRefCount<NetDeviceQueue>
{
public:
  NetDeviceQueue ();
  virtual ~NetDeviceQueue();

  /**
   * Called by the device to start this device transmission queue.
   * This is the analogous to the netif_tx_start_queue function of the Linux kernel.
   */
  virtual void Start (void);

  /**
   * Called by the device to stop this device transmission queue.
   * This is the analogous to the netif_tx_stop_queue function of the Linux kernel.
   */
  virtual void Stop (void);

  /**
   * Called by the device to wake the queue disc associated with this
   * device transmission queue. This is done by invoking the wake callback.
   * This is the analogous to the netif_tx_wake_queue function of the Linux kernel.
   */
  virtual void Wake (void);

  /**
   * \brief Get the status of the device transmission queue.
   * \return true if the device transmission queue is stopped.
   *
   * Called by queue discs to enquire about the status of a given transmission queue.
   * This is the analogous to the netif_tx_queue_stopped function of the Linux kernel.
   */
  bool IsStopped (void) const;

  /// Callback invoked by netdevices to wake upper layers
  typedef Callback< void > WakeCallback;

  /**
   * \brief Set the wake callback
   * \param cb the callback to set
   *
   * Called by the traffic control layer to set the wake callback. The wake callback
   * is invoked by the device whenever it is needed to "wake" the upper layers (i.e.,
   * solicitate the queue disc associated with this transmission queue (in case of
   * multi-queue aware queue discs) or to the network device (otherwise) to send
   * packets down to the device).
   */
  virtual void SetWakeCallback (WakeCallback cb);

  /**
   * \brief Called by the netdevice to report the number of bytes queued to the device queue
   * \param bytes number of bytes queued to the device queue
   */
  void NotifyQueuedBytes (uint32_t bytes);

  /**
   * \brief Called by the netdevice to report the number of bytes it is going to transmit
   * \param bytes number of bytes the device is going to transmit
   */
  void NotifyTransmittedBytes (uint32_t bytes);

  /**
   * \brief Reset queue limits state
   */
  void ResetQueueLimits ();

  /**
   * \brief Set queue limits to this queue
   * \param ql the queue limits associated to this queue
   */
  void SetQueueLimits (Ptr<QueueLimits> ql);

  /**
   * \brief Get queue limits to this queue
   * \return the queue limits associated to this queue
   */
  Ptr<QueueLimits> GetQueueLimits ();

private:
  bool m_stoppedByDevice;         //!< True if the queue has been stopped by the device
  bool m_stoppedByQueueLimits;    //!< True if the queue has been stopped by a queue limits object
  Ptr<QueueLimits> m_queueLimits; //!< Queue limits object
  WakeCallback m_wakeCallback;    //!< Wake callback
};


/**
 * \ingroup netdevice
 *
 * \brief Network device transmission queue interface
 *
 * This interface is used by the traffic control layer and by the aggregated
 * device to access the transmission queues of the device. Additionally, through
 * this interface, traffic control aware netdevices can:
 * - set the number of transmission queues
 * - set the method used (by upper layers) to determine the transmission queue
 *   in which the netdevice would enqueue a given packet
 * This interface is created and aggregated to a device by the traffic control
 * layer when an Ipv{4,6}Interface is added to the device or a queue disc is
 * installed on the device.
 */
class NetDeviceQueueInterface : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   *
   * Creates one NetDeviceQueue by default
   */
  NetDeviceQueueInterface ();
  virtual ~NetDeviceQueueInterface ();

  /**
   * \brief Get the i-th transmission queue of the device.
   *
   * \param i the index of the requested queue.
   * \return the i-th transmission queue of the device.
   *
   * The index of the first transmission queue is zero.
   */
  Ptr<NetDeviceQueue> GetTxQueue (uint8_t i) const;

  /**
   * \brief Get the number of device transmission queues.
   * \return the number of device transmission queues.
   */
  uint8_t GetNTxQueues (void) const;

  /**
   * \brief Set the number of device transmission queues to create.
   * \param numTxQueues number of device transmission queues to create.
   *
   * A multi-queue netdevice must call this method from within its
   * NotifyNewAggregate method to set the number of device transmission queues
   * to create.
   */
  void SetTxQueuesN (uint8_t numTxQueues);

  /**
   * \brief Create the device transmission queues.
   *
   * Called by the traffic control layer just after aggregating this netdevice
   * queue interface to the netdevice.
   */
  void CreateTxQueues (void);

  /// Callback invoked to determine the tx queue selected for a given packet
  typedef Callback< uint8_t, Ptr<QueueItem> > SelectQueueCallback;

  /**
   * \brief Set the select queue callback.
   * \param cb the callback to set.
   *
   * A multi-queue netdevice must call this method from within its
   * NotifyNewAggregate method to set the select queue callback, i.e., the
   * method used to select a device transmission queue for a given packet.
   */
  void SetSelectQueueCallback (SelectQueueCallback cb);

  /**
   * \brief Get the select queue callback.
   * \return the select queue callback.
   *
   * Called by the traffic control layer to get the select queue callback set
   * by a multi-queue device.
   */
  SelectQueueCallback GetSelectQueueCallback (void) const;

protected:
  /**
   * \brief Dispose of the object
   */
  virtual void DoDispose (void);

private:
  std::vector< Ptr<NetDeviceQueue> > m_txQueuesVector;   //!< Device transmission queues
  SelectQueueCallback m_selectQueueCallback;   //!< Select queue callback
  uint8_t m_numTxQueues;   //!< Number of transmission queues to create
};


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
 * pure virtual methods in this class.
 *
 * This class was designed to hide as many MAC-level details as 
 * possible from the perspective of layer 3 to allow a single layer 3
 * to work with any kind of MAC layer. Specifically, this class 
 * encapsulates the specific format of MAC addresses used by a
 * device such that the layer 3 does not need any modification
 * to handle new address formats. This means obviously that the
 * NetDevice class must know about the address format of all potential 
 * layer 3 protocols through its GetMulticast methods: the current
 * API has been optimized to make it easy to add new MAC protocols,
 * not to add new layer 3 protocols.
 *
 * Devices aiming to be Traffic Control aware must implement a NotifyNewAggregate
 * method to perform the following operations:
 *   - cache the pointer to the netdevice queue interface aggregated to the device
 *   - set the number of device transmission queues through the netdevice queue
 *     interface, if the device is multi-queue
 *   - set the select queue callback through the netdevice queue interface, if
 *     the device is multi-queue
 * In order to support flow control, a Traffic Control aware device must:
 *   - stop a device queue when there is no room for another packet. This check
 *     is typically performed after successfully enqueuing a packet in the device
 *     queue. Failing to enqueue a packet because there is no room for the packet
 *     in the queue should be avoided. Should such a situation occur, the device
 *     queue should be immediately stopped
 *   - wake up the queue disc when the device queue is empty. This check is
 *     typically performed after a dequeue operation fails because the device
 *     queue is empty.
 *   - start a device queue when the queue is stopped and there is room for
 *     another packet. This check is typically performed after successfully
 *     dequeuing a packet from the device queue
 * In order to support BQL, a Traffic Control aware device must:
 *   - call NotifyQueuedBytes after successfully enqueuing a packet in the
 *     device queue
 *   - call NotifyTransmittedBytes after successfully dequeuing a packet from
 *     the device queue
 */
class NetDevice : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual ~NetDevice();

  /**
   * \param index ifIndex of the device 
   */
  virtual void SetIfIndex (const uint32_t index) = 0;
  /**
   * \return index ifIndex of the device 
   */
  virtual uint32_t GetIfIndex (void) const = 0;


  /**
   * \return the channel this NetDevice is connected to. The value
   *         returned can be zero if the NetDevice is not yet connected
   *         to any channel or if the underlying NetDevice has no
   *         concept of a channel. i.e., callers _must_ check for zero
   *         and be ready to handle it.
   */
  virtual Ptr<Channel> GetChannel (void) const = 0;

  /**
   * Set the address of this interface
   * \param address address to set
   */
  virtual void SetAddress (Address address) = 0;

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
   * TracedCallback signature for link changed event.
   */
  typedef void (* LinkChangeTracedCallback) (void);
  /**
   * \param callback the callback to invoke
   *
   * Add a callback invoked whenever the link 
   * status changes to UP. This callback is typically used
   * by the IP/ARP layer to flush the ARP cache and by IPv6 stack
   * to flush NDISC cache whenever the link goes up.
   */
  virtual void AddLinkChangeCallback (Callback<void> callback) = 0;
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
   * \brief Make and return a MAC multicast address using the provided
   *        multicast group
   *
   * \RFC{1112} says that an Ipv4 host group address is mapped to an Ethernet 
   * multicast address by placing the low-order 23-bits of the IP address into 
   * the low-order 23 bits of the Ethernet multicast address 
   * 01-00-5E-00-00-00 (hex).  Similar RFCs exist for Ipv6 and Eui64 mappings.
   * This method performs the multicast address creation function appropriate
   * to the underlying MAC address of the device.  This MAC address is
   * encapsulated in an abstract Address to avoid dependencies on the exact
   * MAC address format.
   *
   * In the case of net devices that do not support
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
   * \see IsMulticast()
   */
  virtual Address GetMulticast (Ipv4Address multicastGroup) const = 0;

  /**
   * \brief Get the MAC multicast address corresponding
   * to the IPv6 address provided.
   * \param addr IPv6 address
   * \return the MAC multicast address
   * \warning Calling this method is invalid if IsMulticast returns not true.
   */
  virtual Address GetMulticast (Ipv6Address addr) const = 0;

  /**
   * \brief Return true if the net device is acting as a bridge.
   *
   * \return value of m_isBridge flag
   */
  virtual bool IsBridge (void) const = 0;

  /**
   * \brief Return true if the net device is on a point-to-point link.
   *
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
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) = 0;
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
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber) = 0;
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


  /** 
   * Packet types are used as they are in Linux.  GCC name resolution on 
   * typedef enum {} PacketType is broken for the foreseeable future, so
   * if you need to use ns-3 PacketType in a driver that also uses the 
   * Linux packet types you're hosed unless we define a shadow type, 
   * which we do here.
   */
  enum PacketType
  {
    PACKET_HOST = 1,   /**< Packet addressed oo us */
    NS3_PACKET_HOST = PACKET_HOST,
    PACKET_BROADCAST,   /**< Packet addressed to all */
    NS3_PACKET_BROADCAST = PACKET_BROADCAST,
    PACKET_MULTICAST,   /**< Packet addressed to multicast group */
    NS3_PACKET_MULTICAST = PACKET_MULTICAST,
    PACKET_OTHERHOST,   /**< Packet addressed to someone else */
    NS3_PACKET_OTHERHOST = PACKET_OTHERHOST,
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
  typedef Callback< bool, Ptr<NetDevice>, Ptr<const Packet>, uint16_t, const Address & > ReceiveCallback;

  /**
   * \param cb callback to invoke whenever a packet has been received and must
   *        be forwarded to the higher layers.
   *
   * Set the callback to be used to notify higher layers when a packet has been
   * received.
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
   * \return true if this interface supports a bridging mode, false otherwise.
   */
  virtual bool SupportsSendFrom (void) const = 0;

};

} // namespace ns3

#endif /* NET_DEVICE_H */
