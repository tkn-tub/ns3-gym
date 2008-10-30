/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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

#ifndef EMU_NET_DEVICE_H
#define EMU_NET_DEVICE_H

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

class Queue;

/**
 * \class EmuNetDevice
 * \brief A Device for an Emu Network Link.
 */
class EmuNetDevice : public NetDevice 
{
public:
  static TypeId GetTypeId (void);

  /**
   * Construct a EmuNetDevice
   *
   * This is the constructor for the EmuNetDevice.  It takes as a
   */
  EmuNetDevice ();

  /**
   * Destroy a EmuNetDevice
   *
   * This is the destructor for the EmuNetDevice.
   */
  virtual ~EmuNetDevice ();

  /**
   * Set the Data Rate used for transmission of packets.  
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
   * Set a start time for the device.
   *
   * @param tStart the start time
   */
  void Start (Time tStart);

  /**
   * Set a stop time for the device.
   *
   * @param tStop the stop time
   */
  void Stop (Time tStop);

  /**
   * Attach a queue to the EmuNetDevice.
   *
   * The EmuNetDevice "owns" a queue that implements a queueing 
   * method such as DropTail or RED.  
   *
   * @see Queue
   * @see DropTailQueue
   * @param queue Ptr to the new queue.
   */
  void SetQueue (Ptr<Queue> queue);

  /**
   * Receive a packet.
   *
   * The EmuNetDevice receives packets from its socket reader
   * and forwards them up the protocol stack.  This is the public method
   * used by the reader to indicate that a packet has arrived at the device.
   *
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
  virtual Address GetMulticast (void) const;
  virtual Address MakeMulticastAddress (Ipv4Address multicastGroup) const;

  virtual bool IsPointToPoint (void) const;

  virtual bool Send(Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);

  virtual bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);

  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);

  virtual bool NeedsArp (void) const;

  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);

  virtual bool SupportsSendFrom (void) const;

private:

  virtual void DoDispose (void);

  /**
   * Call out to a separate process running as suid root in order to get a raw 
   * socket.  We do this to avoid having the entire simulation running as root.
   * If this method returns, we'll have a raw socket waiting for us in m_sock.
   */
  void CreateSocket (void);

  /**
   * Figure out where the raw socket creation process lives on the system.
   */
  std::string FindCreator (void);

  /**
   * Get a copy of the attached Queue.
   *
   * This method is provided for any derived class that may need to get
   * direct access to the underlying queue.
   *
   * @returns Ptr to the queue.
   */
  Ptr<Queue> GetQueue(void) const; 

  /**
   * Spin up the device
   */
  void StartDevice (void);

  /**
   * Tear down the device
   */
  void StopDevice (void);

  /**
   * Loop to read and process packets
   */
  void ReadThread (void);

  /**
   * Method to handle received packets.  Synchronized with simulator via ScheduleNow from ReadThread.
   */
  void ForwardUp (uint8_t *buf, uint32_t len);

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
   * @returns true if success, false on failure
   */
  bool TransmitStart (Ptr<Packet> p);

  void NotifyLinkUp (void);

  /**
   * The Queue which this EmuNetDevice uses as a packet source.
   * Management of this Queue has been delegated to the EmuNetDevice
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
   * Time to start spinning up the device
   */
  Time m_tStart;

  /**
   * Time to start tearing down the device
   */
  Time m_tStop;

  EventId m_startEvent;
  EventId m_stopEvent;

  int32_t m_sock;

  Ptr<SystemThread> m_readThread;

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
   * The callback used to notify higher layers that a packet has been received in promiscuous mode.
   */
  NetDevice::PromiscReceiveCallback m_promiscRxCallback;

  /**
   * The ns-3 interface index (in the sense of net device index) that has been assigned to this network device.
   */
  uint32_t m_ifIndex;

  /**
   * The Unix interface index that we got from the system and which corresponds to the interface (e.g., "eth1")
   * we are using to talk to the network.  Valid when m_sock is valid.
   */
  int32_t m_sll_ifindex;

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
   * The unix/linux name of the underlying device (e.g., eth0)
   */
  std::string m_deviceName;
};

} // namespace ns3

#endif // EMU_NET_DEVICE_H

