/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Craig Dowell <craigdo@ee.washington.edu>
 */

#ifndef POINT_TO_POINT_NET_DEVICE_H
#define POINT_TO_POINT_NET_DEVICE_H

#include <string.h>
#include "ns3/address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/callback-trace-source.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"

namespace ns3 {

class Queue;
class PointToPointChannel;

class PointToPointTraceType : public TraceContextElement
{
public:
  PointToPointTraceType ();
  void Print (std::ostream &os) const;
  static uint16_t GetUid (void);
  std::string GetTypeName (void) const;
};

/**
 * \class PointToPointNetDevice
 * \brief A Device for a Point to Point Network Link.
 *
 * Ns-3 takes a four-layer view of a protocol stack.  This is the same model
 * that TCP uses.  In this view, layers 5-7 of the OSI reference model are
 * grouped together into an application layer; layer four (transport / TCP) is
 * broken out; layer three (network / IP) is broken out; and layers 1-2 are
 * grouped together.  We call this grouping of layers one and two a NetDevice
 * and represent it as a class in the system.
 *
 * The NetDevice class is specialized according to the needs of the specific
 * kind of network link.  In this case, the link is a PointToPoint link.  The
 * PointToPoint link is a family of classes that includes this class, the
 * PointToPointNetDevice, a PointToPointChannel class that represents the 
 * actual medium across which bits are sent, a PointToPointIpv4Interface class
 * that provides the hook to tie a general purpose node to this specific
 * link, and finally, a PointToPointTopology object that is responsible for
 * putting all of the pieces together.
 *
 * This is the PointToPointNetDevice class that represents, essentially, the
 * PC card that is used to connect to the PointToPoint network.
 */
class PointToPointNetDevice : public NetDevice {
public:
  /**
   * Construct a PointToPointNetDevice
   *
   * This is the constructor for the PointToPointNetDevice.  It takes as a
   * parameter the Node to which this device is connected.  Ownership of the
   * Node pointer is not implied and the node must not be deleded.
   *
   * @see PointToPointTopology::AddPointToPointLink ()
   * @param node the Node to which this device is connected.
   */
  PointToPointNetDevice (Ptr<Node> node,
                         const DataRate& = g_defaultRate.GetValue());
  /**
   * Destroy a PointToPointNetDevice
   *
   * This is the destructor for the PointToPointNetDevice.
   */
  virtual ~PointToPointNetDevice();
  /**
   * Set the Data Rate used for transmission of packets.  The data rate is
   * set in the Attach () method from the corresponding field in the channel
   * to which the device is attached.  It can be overridden using this method.
   *
   * @see Attach ()
   * @param bps the data rate at which this object operates
   */
  void SetDataRate(const DataRate& bps);
  /**
   * Set the inteframe gap used to separate packets.  The interframe gap
   * defines the minimum space required between packets sent by this device.
   * It is usually set in the Attach () method based on the speed of light
   * delay of the channel to which the device is attached.  It can be 
   * overridden using this method if desired.
   *
   * @see Attach ()
   * @param t the interframe gap time
   */
  void SetInterframeGap(const Time& t);
  /**
   * Attach the device to a channel.
   *
   * The PointToPointTopology object creates a PointToPointChannel and two
   * PointtoPointNetDevices.  In order to introduce these components to each
   * other, the topology object calls Attach () on each PointToPointNetDevice.
   * Inside this method, the Net Device calls out to the PointToPointChannel
   * to introduce itself.
   *
   * @see PointToPointTopology::AddPointToPointLink ()
   * @see SetDataRate ()
   * @see SetInterframeGap ()
   * @param ch a pointer to the channel to which this object is being attached.
   */
  bool Attach(Ptr<PointToPointChannel> ch);
  /**
   * Attach a queue to the PointToPointNetDevice.
   *
   * The PointToPointNetDevice "owns" a queue.  This queue is created by the
   * PointToPointTopology object and implements a queueing method such as
   * DropTail or RED.  The PointToPointNetDevice assumes ownership of this
   * queue and must delete it when the device is destroyed.
   *
   * @see PointToPointTopology::AddPointToPointLink ()
   * @see Queue
   * @see DropTailQueue
   * @param queue a pointer to the queue for which object is assuming
   *        ownership.
   */
  void AddQueue(Ptr<Queue> queue);
  /**
   * Receive a packet from a connected PointToPointChannel.
   *
   * The PointToPointNetDevice receives packets from its connected channel
   * and forwards them up the protocol stack.  This is the public method
   * used by the channel to indicate that the last bit of a packet has 
   * arrived at the device.
   *
   * @see PointToPointChannel
   * @param p a reference to the received packet
   */
  void Receive (Packet& p);
protected:
  /**
   * Create a Trace Resolver for events in the net device.
   *
   * @see class TraceResolver
   */
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
  virtual void DoDispose (void);
  /**
   * Get a copy of the attached Queue.
   *
   * This method is provided for any derived class that may need to get
   * direct access to the underlying queue.
   *
   * @see PointToPointTopology
   * @returns a pointer to the queue.
   */
  Ptr<Queue> GetQueue(void) const; 
  /**
   * Get a copy of the attached Channel
   *
   * This method is provided for any derived class that may need to get
   * direct access to the connected channel
   *
   * @see PointToPointChannel
   * @returns a pointer to the channel
   */
  virtual Ptr<Channel> DoGetChannel(void) const;
  /**
   * Set a new default data rate
   */
  static void SetDefaultRate(const DataRate&);

  /** 
   * Get the current default rate.
   * @returns a const reference to current default
   */

  static const DataRate& GetDefaultRate();

private:
  /**
   * Adds the necessary headers and trailers to a packet of data in order to
   * respect the protocol implemented by the agent.
   */
  void AddHeader(Packet& p, uint16_t protocolNumber);
  /**
   * Removes, from a packet of data, all headers and trailers that
   * relate to the protocol implemented by the agent
   * \return Returns true if the packet should be forwarded up the
   * protocol stack.
   */
  bool ProcessHeader(Packet& p, uint16_t& param);
  /**
   * Send a Packet Down the Wire.
   *
   * The SendTo method is defined as the standard way that the level three
   * protocol uses to tell a NetDevice to send a packet.  SendTo is declared
   * as abstract in the NetDevice class and we declare it here.
   *
   * @see NetDevice
   * @param p a reference to the packet to send
   * @param dest a reference to the Address of the destination device
   * @param protocolNumber Protocol Number used to find protocol touse
   * @returns true if success, false on failure
   */
  virtual bool SendTo (const Packet& p, const Address& dest, 
                       uint16_t protocolNumber);
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
  bool TransmitStart (Packet &p);
  /**
   * Stop Sending a Packet Down the Wire and Begin the Interframe Gap.
   *
   * The TransmitComplete method is used internally to finish the process
   * of sending a packet out on the channel.
   *
   */
  void TransmitComplete(void);
  virtual bool DoNeedsArp (void) const;
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
   * @see class TraceResolver
   */
  CallbackTraceSource<const Packet &> m_rxTrace;
  /** 
   * Default data rate.  Used for all newly created p2p net devices
   */
   static DataRateDefaultValue g_defaultRate;

};

}; // namespace ns3

#endif // POINT_TO_POINT_NET_DEVICE_H

