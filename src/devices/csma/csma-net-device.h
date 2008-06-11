/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise
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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca
 */

#ifndef CSMA_NET_DEVICE_H
#define CSMA_NET_DEVICE_H

#include <string.h>
#include "ns3/node.h"
#include "ns3/backoff.h"
#include "ns3/address.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "ns3/mac48-address.h"

namespace ns3 {

class Queue;
class CsmaChannel;
class ErrorModel;

/**
 * \class CsmaNetDevice
 * \brief A Device for a Csma Network Link.
 *
 * The Csma net device class is analogous to layer 1 and 2 of the
 * TCP stack. The NetDevice takes a raw packet of bytes and creates a
 * protocol specific packet from them. 
 *
 * The Csma net device class takes a packet and adds (or removes) the 
 * headers/trailers that are associated with EthernetV1, EthernetV2, RAW
 * or LLC protocols. The EthernetV1 packet type adds and removes Ethernet
 * destination and source addresses. The LLC packet type adds and
 * removes LLC snap headers. The raw packet type does not add or
 * remove any headers.  
 *
 * Each Csma net device will receive all packets written to the Csma link. 
 * The ProcessHeader function can be used to filter out the packets such that
 * higher level layers only receive packets that are addressed to their
 * associated net devices
 */
class CsmaNetDevice : public NetDevice 
{
public:
  static TypeId GetTypeId (void);
  /**
   * Enumeration of the types of packets supported in the class.
   *
   */
  enum CsmaEncapsulationMode {
    ETHERNET_V1, /**< Version one ethernet packet, length field */
    IP_ARP,      /**< Ethernet packet encapsulates IP/ARP packet */
    RAW,         /**< Packet that contains no headers */
    LLC,         /**< LLC packet encapsulation */  
  };

  /**
   * Construct a CsmaNetDevice
   *
   * This is the default constructor for a CsmaNetDevice.
   */
  CsmaNetDevice ();

  /**
   * Destroy a CsmaNetDevice
   *
   * This is the destructor for a CsmaNetDevice.
   */
  virtual ~CsmaNetDevice ();

  /**
   * Set the inteframe gap used to separate packets.  The interframe gap
   * defines the minimum space required between packets sent by this device.
   * As in Ethernet, it defaults to 96 bit times.
   *
   * \param t the interframe gap time
   */
  void SetInterframeGap (Time t);

  /**
   * Set the backoff parameters used to determine the wait to retry
   * transmitting a packet when the channel is busy.
   *
   * @see Attach ()
   * \param slotTime Length of a packet slot (or average packet time)
   * \param minSlots Minimum number of slots to wait
   * \param maxSlots Maximum number of slots to wait
   * \param maxRetries Maximum number of retries before packet is discard
   * \param ceiling Cap on the exponential function when calculating max slots
   */
  void SetBackoffParams (Time slotTime, uint32_t minSlots, uint32_t maxSlots, 
    uint32_t maxRetries, uint32_t ceiling);

  /**
   * Attach the device to a channel.
   *
   * The function Attach is used to add a CsmaNetDevice to a CsmaChannel.
   *
   * @see SetDataRate ()
   * @see SetInterframeGap ()
   * \param ch a pointer to the channel to which this object is being attached.
   */
  bool Attach (Ptr<CsmaChannel> ch);

  /**
   * Attach a queue to the CsmaNetDevice.
   *
   * The CsmaNetDevice "owns" a queue.  This queue may be set by higher
   * level topology objects to implement a particular queueing method such as
   * DropTail or RED.  
   *
   * @see Queue
   * @see DropTailQueue
   * \param queue a Ptr to the queue for being assigned to the device.
   */
  void SetQueue (Ptr<Queue> queue);

  /**
   * Attach a receive ErrorModel to the CsmaNetDevice.
   *
   * The CsmaNetDevice may optionally include an ErrorModel in
   * the packet receive chain to simulate data errors in during transmission.
   *
   * @see ErrorModel
   * @param em a pointer to the ErrorModel 
   */
  void SetReceiveErrorModel (Ptr<ErrorModel> em);

  /**
   * Receive a packet from a connected CsmaChannel.
   *
   * The CsmaNetDevice receives packets from its connected channel
   * and forwards them up the protocol stack.  This is the public method
   * used by the channel to indicate that the last bit of a packet has 
   * arrived at the device.
   *
   * @see CsmaChannel
   * \param p a reference to the received packet
   */
  void Receive (Ptr<Packet> p);

  /**
   * Is the send side of the network device enabled?
   *
   * \returns True if the send side is enabled, otherwise false.
   */
  bool IsSendEnabled (void);

  /**
   * Enable or disable the send side of the network device.
   *
   * \param enable Enable the send side if true, otherwise disable.
   */
  void SetSendEnable (bool enable);

  /**
   * Is the receive side of the network device enabled?
   *
   * \returns True if the receiver side is enabled, otherwise false.
   */
  bool IsReceiveEnabled (void);

  /**
   * Enable or disable the receive side of the network device.
   *
   * \param enable Enable the receive side if true, otherwise disable.
   */
  void SetReceiveEnable (bool enable);

  /**
   * Set the MAC address of the the network device.
   *
   * \param addr The Mac48Address to use as the address of the device.
   */
  void SetAddress (Mac48Address addr);

//
// The following methods are inherited from NetDevice base class.
//
  virtual void SetName (const std::string name);
  virtual std::string GetName (void) const;
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
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

  /**
   * @brief Make and return a MAC multicast address using the provided
   *        multicast group
   *
   * RFC 1112 says that an Ipv4 host group address is mapped to an Ethernet 
   * multicast address by placing the low-order 23-bits of the IP address into 
   * the low-order 23 bits of the Ethernet multicast address 
   * 01-00-5E-00-00-00 (hex).
   *
   * This method performs the multicast address creation function appropriate
   * to an EUI-48-based CSMA device.  This MAC address is encapsulated in an
   *  abstract Address to avoid dependencies on the exact address format.
   *
   * @param multicastGroup The IP address for the multicast group destination
   * of the packet.
   * @return The MAC multicast Address used to send packets to the provided
   * multicast group.
   *
   * @see Ipv4Address
   * @see Mac48Address
   * @see Address
   */
  virtual Address MakeMulticastAddress (Ipv4Address multicastGroup) const;

  /**
   * Is this a point to point link?
   * \returns false.
   */
  virtual bool IsPointToPoint (void) const;

  /**
   * Start sending a packet down the channel.
   */
  virtual bool Send (Ptr<Packet> packet, const Address& dest, 
    uint16_t protocolNumber);

  /**
   * Get the node to which this device is attached.
   *
   * \returns Ptr to the Node to which the device is attached.
   */
  virtual Ptr<Node> GetNode (void) const;

  /**
   * Set the node to which this device is being attached.
   *
   * \param node Ptr to the Node to which the device is being attached.
   */
  virtual void SetNode (Ptr<Node> node);

  /**
   * Does this device need to use the address resolution protocol?
   *
   * \returns True if the encapsulation mode is set to a value that requires
   * ARP (IP_ARP or LLC).
   */
  virtual bool NeedsArp (void) const;

  /**
   * Set the callback to be used to notify higher layers when a packet has been
   * received.
   *
   * \param cb The callback.
   */
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

protected:
  /**
   * Perform any object release functionality required to break reference 
   * cycles in reference counted objects held by the device.
   */
  virtual void DoDispose (void);

  /**
   * Get a copy of the attached Queue.
   *
   * This method is provided for any derived class that may need to get
   * direct access to the underlying queue.
   *
   * \return a pointer to the queue.
   */
  Ptr<Queue> GetQueue (void) const; 

  /**
   * Adds the necessary headers and trailers to a packet of data in order to
   * respect the packet type
   *
   * \param p Packet to which header should be added
   * \param dest MAC destination address to which packet should be sent
   * \param protocolNumber In some protocols, identifies the type of
   * payload contained in this packet.
   */
  void AddHeader (Ptr<Packet> p, Mac48Address dest, uint16_t protocolNumber);

  /**
   * Removes, from a packet of data, all headers and trailers that
   * relate to the packet type
   *
   * \param p Packet whose headers need to be processed
   * \param param An integer parameter that can be set by the function
   * to return information gathered in the header
   * \return Returns true if the packet should be forwarded up the
   * protocol stack.
   */
  bool ProcessHeader (Ptr<Packet> p, uint16_t & param);

private:
  /**
   * Operator = is declared but not implemented.  This disables the assigment
   * operator for CsmaNetDevice objects.

   */
  CsmaNetDevice &operator = (const CsmaNetDevice &o);

  /**
   * Copy constructor is declared but not implemented.  This disables the
   * copy constructor for CsmaNetDevice objects.
   */
  CsmaNetDevice (const CsmaNetDevice &o);

  /**
   * Initialization function used during object construction.
   */
  void Init (bool sendEnable, bool receiveEnable);

  /**
   * Start Sending a Packet Down the Wire.
   *
   * The TransmitStart method is the method that is used internally in
   * the CsmaNetDevice to begin the process of sending a packet
   * out on the channel.  A corresponding method is called on the
   * channel to let it know that the physical device this class
   * represents has actually started sending signals, this causes the
   * channel to enter the BUSY state.  An event is scheduled for the time at
   * which the bits have been completely transmitted. 
   *
   * If the channel is found to be BUSY, this method reschedules itself for
   * execution at a later time (within the backoff period).
   *
   * @see CsmaChannel::TransmitStart ()
   * @see TransmitCompleteEvent ()
   */
  void TransmitStart ();

  /**
   * Stop Sending a Packet Down the Wire and Begin the Interframe Gap.
   *
   * The TransmitCompleteEvent method is used internally to finish the process
   * of sending a packet out on the channel.  During execution of this method
   * the TransmitEnd method is called on the channel to let it know that the
   * physical device this class represents has finished sending simulated
   * signals.  The channel uses this event to begin its speed of light delay
   * timer after which it notifies the Net Device(s) at the other end of the 
   * link that new bits have arrived (it delivers the Packet).  During this 
   * method, the net device also schedules the TransmitReadyEvent at which
   * time the transmitter becomes ready to send the next packet.
   *
   * @see CsmaChannel::TransmitEnd ()
   * @see TransmitReadyEvent ()
   */
  void TransmitCompleteEvent (void);

  /**
   * Cause the Transmitter to Become Ready to Send Another Packet.
   *
   * The TransmitReadyEvent method is used internally to re-enable the 
   * transmit machine of the net device.  It is scheduled after a suitable
   * interframe gap after the completion of the previous transmission.
   * The queue is checked at this time, and if there is a packet waiting on
   * the queue, the transmission process is begun.
   *
   * If a packet is in the queue, it is extracted for the queue as the
   * next packet to be transmitted by the net device.
   *
   * @see TransmitStart ()
   */
  void TransmitReadyEvent (void);

  /**
   * Aborts the transmission of the current packet
   *
   * If the net device has tried to transmit a packet for more times
   * than the maximum allowed number of retries (channel always busy)
   * then the packet is dropped.
   */
  void TransmitAbort (void);

  /**
   * Notify any interested parties that the link has come up.
   */
  void NotifyLinkUp (void);

  /** 
   * Device ID returned by the attached functions. It is used by the
   * mp-channel to identify each net device to make sure that only
   * active net devices are writing to the channel
   */
  uint32_t m_deviceId; 

  /**
   * Enable net device to send packets. True by default
   */
  bool m_sendEnable;

  /**
   * Enable net device to receive packets. True by default
   */
  bool m_receiveEnable;

  /**
   * Enumeration of the states of the transmit machine of the net device.
   */
  enum TxMachineState
    {
      READY, /**< The transmitter is ready to begin transmission of a packet */
      BUSY,  /**< The transmitter is busy transmitting a packet */
      GAP,    /**< The transmitter is in the interframe gap time */
      BACKOFF    /**< The transmitter is waiting for the channel to be free */
    };

  /**
   * The state of the Net Device transmit state machine.
   * @see TxMachineState
   */
  TxMachineState m_txMachineState;
  
  /**
   * The type of packet that should be created by the AddHeader
   * function and that should be processed by the ProcessHeader
   * function.
   */
  CsmaEncapsulationMode m_encapMode;

  /**
   * The data rate that the Net Device uses to simulate packet transmission
   * timing.
   * @see class DataRate
   */
  DataRate m_bps;

  /**
   * The interframe gap that the Net Device uses insert time between packet
   * transmission
   * @see class Time
   */
  Time m_tInterframeGap;

  /**
   * Holds the backoff parameters and is used to calculate the next
   * backoff time to use when the channel is busy and the net device
   * is ready to transmit
   */
  Backoff m_backoff;

  /**
   * Next packet that will be transmitted (if transmitter is not
   * currently transmitting) or packet that is currently being
   * transmitted.
   */
  Ptr<Packet> m_currentPkt;

  /**
   * The CsmaChannel to which this CsmaNetDevice has been
   * attached.
   * @see class CsmaChannel
   */
  Ptr<CsmaChannel> m_channel;

  /**
   * The Queue which this CsmaNetDevice uses as a packet source.
   * Management of this Queue has been delegated to the CsmaNetDevice
   * and it has the responsibility for deletion.
   * @see class Queue
   * @see class DropTailQueue
   */
  Ptr<Queue> m_queue;

  /**
   * Error model for receive packet events
   */
  Ptr<ErrorModel> m_receiveErrorModel;

  /**
   * The trace source for the packet reception events that the device can
   * fire.
   *
   * @see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_rxTrace;

  /**
   * The trace source for the packet drop events that the device can
   * fire.
   *
   * @see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_dropTrace;

  /**
   * The Node to which this device is attached.
   */
  Ptr<Node> m_node;

  /**
   * The MAC address which has been assigned to this device.
   */
  Mac48Address m_address;

  /**
   * The callback used to notify higher layers that a packet has been received.
   */
  NetDevice::ReceiveCallback m_rxCallback;

  /**
   * The interface index (really net evice index) that has been assigned to 
   * this network device.
   */
  uint32_t m_ifIndex;

  /**
   * The human readable name of this device.
   */
  std::string m_name;

  /**
   * Flag indicating whether or not the link is up.  In this case,
   * whether or not the device is connected to a channel.
   */
  bool m_linkUp;

  /**
   * Callback to fire if the link changes state (up or down).
   */
  Callback<void> m_linkChangeCallback;

  /**
   * The maximum transmission unit (biggest packet) allowed to be sent or 
   * received by this network device.
   */
  uint16_t m_mtu;
};

}; // namespace ns3

#endif // CSMA_NET_DEVICE_H
