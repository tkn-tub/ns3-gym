/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007, 2008 University of Washington
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

#ifndef POINT_TO_POINT_NET_DEVICE_H
#define POINT_TO_POINT_NET_DEVICE_H

#include <string.h>
#include "ns3/address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ptr.h"
#include "ns3/mac48-address.h"

namespace ns3 {

class Queue;
class PointToPointChannel;
class ErrorModel;

/**
 * \class PointToPointNetDevice
 * \brief A Device for a Point to Point Network Link.
 *
 * This PointToPointNetDevice class specializes the NetDevice abstract
 * base class.  Together with a PointToPointChannel (and a peer 
 * PointToPointNetDevice), the class models, with some level of 
 * abstraction, a generic point-to-point or serial link.  
 * Key parameters or objects that can be specified for this device 
 * include a queue, data rate, and interframe transmission gap (the 
 * propagation delay is set in the PointToPointChannel).
 */
class PointToPointNetDevice : public NetDevice 
{
public:
  static TypeId GetTypeId (void);

  /**
   * Construct a PointToPointNetDevice
   *
   * This is the constructor for the PointToPointNetDevice.  It takes as a
   * parameter a pointer to the Node to which this device is connected, 
   * as well as an optional DataRate object.
   */
  PointToPointNetDevice ();

  /**
   * Destroy a PointToPointNetDevice
   *
   * This is the destructor for the PointToPointNetDevice.
   */
  virtual ~PointToPointNetDevice ();

  /**
   * Set the Data Rate used for transmission of packets.  The data rate is
   * set in the Attach () method from the corresponding field in the channel
   * to which the device is attached.  It can be overridden using this method.
   *
   * @see Attach ()
   * @param bps the data rate at which this object operates
   */
  void SetDataRate (DataRate bps);

  /**
   * Set the inteframe gap used to separate packets.  The interframe gap
   * defines the minimum space required between packets sent by this device.
   *
   * @param t the interframe gap time
   */
  void SetInterframeGap (Time t);

  /**
   * Attach the device to a channel.
   *
   * @param ch Ptr to the channel to which this object is being attached.
   */
  bool Attach (Ptr<PointToPointChannel> ch);

  /**
   * Attach a queue to the PointToPointNetDevice.
   *
   * The PointToPointNetDevice "owns" a queue that implements a queueing 
   * method such as DropTail or RED.  
   *
   * @see Queue
   * @see DropTailQueue
   * @param queue Ptr to the new queue.
   */
  void SetQueue (Ptr<Queue> queue);

  /**
   * Attach a receive ErrorModel to the PointToPointNetDevice.
   *
   * The PointToPointNetDevice may optionally include an ErrorModel in
   * the packet receive chain.
   *
   * @see ErrorModel
   * @param em Ptr to the ErrorModel.
   */
  void SetReceiveErrorModel(Ptr<ErrorModel> em);

  /**
   * Receive a packet from a connected PointToPointChannel.
   *
   * The PointToPointNetDevice receives packets from its connected channel
   * and forwards them up the protocol stack.  This is the public method
   * used by the channel to indicate that the last bit of a packet has 
   * arrived at the device.
   *
   * @see PointToPointChannel
   * @param p Ptr to the received packet.
   */
  void Receive (Ptr<Packet> p);

  /**
   * Assign a MAC address to this device.
   *
   * @see Mac48Address
   * @param addr The new address.
   */
  void SetAddress (Mac48Address addr);

  /**
   * Set The max frame size of packets sent over this device.
   *
   * Okay, that was easy to say, but the details are a bit thorny.  We have a MAC-level MTU that is the payload that higher 
   * level protocols see.  We have a PHY-level MTU which is the maximum number of bytes we can send over the link 
   * (cf. 1500 bytes for Ethernet).  We also have a frame size which is some total number of bytes in a packet which could
   * or could not include any framing and overhead.  There can be a lot of inconsistency in definitions of these terms.  For
   * example, RFC 1042 asserts that the terms maximum transmission unit and maximum packet size are equivalent.  RFC 791, 
   * however, defines MTU as the maximum sized IP datagram that can be sent.  Packet size and frame size are sometimes
   * used interchangeably.
   *
   * So, some careful definitions are in order to avoid confusion:
   *
   * In real serial channel (HDLC, for example), the wire idles (sends all ones) until the channel begins sending a packet.
   * A frame on the wire starts with a flag character (01111110).  This is followed by what is usually called the packet: 
   * address, control, payload, and a Frame Check Sequence (FCS).  This is followed by another flag character.  If the flag
   * characters are used, then bit stuffing must be used to prevent flag characters from appearing in the packet and confusing
   * the link.  Som to be strictly and pedantically correct the frame size is then necessarily larger than the packet size on 
   * a real link.  But, this isn't a real link, it's a simulation of a device similar to a point-to-point device, and we have
   * no good reason to add framing bits and therefore to do bit-stuffing.  So, in the case of the point-to-point device, the 
   * frame size is equal to the packet size.  Since these two values are defined to be equal, there is no danger in assuming
   * they are identical.  We define packet size to be equal to frame size and this excludes the flag characters.  We define a 
   * single (MAC-level) MTU that coresponds to the payload size of the packet, which is the IP-centric view of the term as
   * seen in RFC 791.
   *
   * To make this concrete, consider PPP framing on a synchronous link.  In this framing scheme, a real serial frame on the 
   * wire starts with a flag character, address and control characters, then a 16-bit PPP protocol ID (0x21 = IP).  Then we 
   * would see the actual payload we are supposed to send, presumably an IP datagram.  At then we see the FCS and finally 
   * another flag character to end the frame.  We ignore the flag bits on this device since it they are not needed.  We 
   * aren't really using HDLC to send frames across the link, so we don't need the address and control bits either.  In fact,
   * to encapsulate using unframed PPP all we need to do is prepend the two-byte protocol ID.
   *
   * Typically the limiting factor in frame size is due to hardware limitations in the underlying HDLC controller receive 
   * FIFO buffer size.  This number can vary widely.  For example, the Motorola MC92460 has a 64 KByte maximum frame size; 
   * the Intel IXP4XX series has a 16 KByte size.  Older USARTs have a maximum frame size around 2KBytes, and typical PPP
   * links on the Internet have their MTU set to 1500 bytes since this is what will typically be used on Ethernet segments
   * and will avoid path MTU issues.  We choose to make the default MTU 1500 bytes which then fixes the maximum frame size
   * as described below.
   *
   * So, there are really two related variables at work here.  There is the maximum frame size that can be sent over the
   * link and there is the MTU.
   *
   * So, what do we do since these values must always be consistent in the driver?  We want to actually allow a user to change 
   * these variables, but we want the results (even at intermediate stages of her ultimate change) to be consistent.  We 
   * certainly don't want to require that users must understand the details of PPP encapsulation in order to set these 
   * variables.
   *
   * Consider the following situation:  A user wants to set the maximum frame size to 16 KBytes.  This user shouldn't have to
   * concern herself that the PPP encapsulation will consume six bytes.  She should not have to figure out that the MTU needs
   * to be set to 16K - 2 bytes to make things consistent.
   *
   * Similarly, a user who is interested in setting the MTU to 1500 bytes should not be forced to understand that the frame 
   * size will need to be set to 1502 bytes. 
   *
   * We could play games trying to figure out what the user wants to do, but that is typically a bad plan and programmers
   * have a long and distinguished history of guessing wrong.  We'll avoid all of that and just define a flexible behavior
   * that can be worked to get what you want.  Here it is:
   *
   * - If the user is changing the MTU, she is interested in getting that part of the system set, so the frame size
   * will be changed to make it consistent;
   *
   * - If the user is changing the frame size, he is interested in getting that part of the system set, so the MTU
   * will be changed to make it consistent;
   *
   * - You cannot define the MTU and frame size separately -- they are always tied together by the overhead of the PPP 
   * encapsulation.  This is not a restriction.  Consider what this means.  Perhaps you want to set the frame size to some 
   * large number and the MTU to some small number.  The largest packet you can send is going to be limited by the MTU, so it
   * is not possible to send a frame larger than the MTU plus overhead.  Having the ability to set a  larger frame size is not
   * useful.
   * 
   * So, if a user calls SetFrameSize, we assume that the maximum frame size is the interesting thing for that user and
   * we just adjust the MTU to a new "correct value" based on the current encapsulation mode.  If a user calls SetMtu, we 
   * assume that the MTU is the interesting property for that user, and we adjust the frame size to a new "correct value" 
   * for the current encapsulation mode.  If a user calls SetEncapsulationMode, then we take the MTU as the free variable 
   * and set its value to match the current frame size.
   *
   * \param frameSize The max frame size of packets sent over this device.
   */
  void SetFrameSize (uint16_t frameSize);

  /**
   * Get The max frame size of packets sent over this device.
   *
   * \returns The max frame size of packets sent over this device.
   */
  uint16_t GetFrameSize (void) const;

//
// Pure virtual methods inherited from NetDevice we must implement.
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

  virtual bool Send(Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);
  virtual bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);

  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);

  virtual bool NeedsArp (void) const;

  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

  virtual Address GetMulticast (Ipv6Address addr) const;

  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

private:

  virtual void DoDispose (void);

  /**
   * Get a copy of the attached Queue.
   *
   * This method is provided for any derived class that may need to get
   * direct access to the underlying queue.
   *
   * @returns Ptr to the queue.
   */
  Ptr<Queue> GetQueue(void) const; 

private:
  /**
   * Calculate the value for the MTU that would result from 
   * setting the frame size to the given value.
   */
  uint32_t MtuFromFrameSize (uint32_t frameSize);

  /**
   * Calculate the value for the frame size that would be required
   * to be able to set the MTU to the given value.
   */
  uint32_t FrameSizeFromMtu (uint32_t mtu);

  /**
   * \returns the address of the remote device connected to this device
   * through the point to point channel.
   */
  Address GetRemote (void) const;

  /**
   * Adds the necessary headers and trailers to a packet of data in order to
   * respect the protocol implemented by the agent.
   */
  void AddHeader(Ptr<Packet> p, uint16_t protocolNumber);

  /**
   * Removes, from a packet of data, all headers and trailers that
   * relate to the protocol implemented by the agent
   * \return Returns true if the packet should be forwarded up the
   * protocol stack.
   */
  bool ProcessHeader(Ptr<Packet> p, uint16_t& param);

  /**
   * Start Sending a Packet Down the Wire.
   *
   * The TransmitStart method is the method that is used internally in the
   * PointToPointNetDevice to begin the process of sending a packet out on
   * the channel.  The corresponding method is called on the channel to let
   * it know that the physical device this class represents has virually
   * started sending signals.  An event is scheduled for the time at which
   * the bits have been completely transmitted.
   *
   * @see PointToPointChannel::TransmitStart ()
   * @see TransmitCompleteEvent ()
   * @param p a reference to the packet to send
   * @returns true if success, false on failure
   */
  bool TransmitStart (Ptr<Packet> p);

  /**
   * Stop Sending a Packet Down the Wire and Begin the Interframe Gap.
   *
   * The TransmitComplete method is used internally to finish the process
   * of sending a packet out on the channel.
   */
  void TransmitComplete(void);

  void NotifyLinkUp (void);

  /**
   * Enumeration of the states of the transmit machine of the net device.
   */
  enum TxMachineState
    {
      READY, /**< The transmitter is ready to begin transmission of a packet */
      BUSY   /**< The transmitter is busy transmitting a packet */
    };
  /**
   * The state of the Net Device transmit state machine.
   * @see TxMachineState
   */
  TxMachineState m_txMachineState;

  /**
   * The data rate that the Net Device uses to simulate packet transmission
   * timing.
   * @see class DataRate
   */
  DataRate       m_bps;

  /**
   * The interframe gap that the Net Device uses to throttle packet
   * transmission
   * @see class Time
   */
  Time           m_tInterframeGap;

  /**
   * The PointToPointChannel to which this PointToPointNetDevice has been
   * attached.
   * @see class PointToPointChannel
   */
  Ptr<PointToPointChannel> m_channel;

  /**
   * The Queue which this PointToPointNetDevice uses as a packet source.
   * Management of this Queue has been delegated to the PointToPointNetDevice
   * and it has the responsibility for deletion.
   * @see class Queue
   * @see class DropTailQueue
   */
  Ptr<Queue> m_queue;

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
   * Error model for receive packet events
   */
  Ptr<ErrorModel> m_receiveErrorModel;

  Ptr<Node> m_node;
  Mac48Address m_address;
  NetDevice::ReceiveCallback m_rxCallback;
  NetDevice::PromiscReceiveCallback m_promiscCallback;
  uint32_t m_ifIndex;
  std::string m_name;
  bool m_linkUp;
  Callback<void> m_linkChangeCallback;

  static const uint16_t DEFAULT_MTU = 1500;
  static const uint16_t PPP_OVERHEAD = 2;
  static const uint16_t DEFAULT_FRAME_SIZE = DEFAULT_MTU + PPP_OVERHEAD;

  /**
   * The frame size/packet size.  This corresponds to the maximum 
   * number of bytes that can be transmitted as a packet without framing.
   * This corresponds to the 1518 byte packet size often seen on Ethernet.
   */
  uint32_t m_frameSize;

  /**
   * The Maxmimum Transmission Unit.  This corresponds to the maximum 
   * number of bytes that can be transmitted as seen from higher layers.
   * This corresponds to the 1500 byte MTU size often seen on IP over 
   * Ethernet.
   */
  uint32_t m_mtu;
};

} // namespace ns3

#endif // POINT_TO_POINT_NET_DEVICE_H

