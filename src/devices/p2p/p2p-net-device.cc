/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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

#include <iostream>
#include <cassert>
#include "ns3/debug.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/composite-trace-resolver.h"
#include "p2p-net-device.h"
#include "p2p-channel.h"

NS_DEBUG_COMPONENT_DEFINE ("PointToPointNetDevice");

namespace ns3 {

PointToPointNetDevice::PointToPointNetDevice (Node* node) 
: 
  NetDevice(node, MacAddress ("00:00:00:00:00:00")), 
  m_txMachineState (READY),
  m_bps (DataRate (0xffffffff)),
  m_tInterframeGap (Seconds(0)),
  m_channel (0), 
  m_queue (0),
  m_rxTrace ()
{
  NS_DEBUG ("PointToPointNetDevice::PointToPointNetDevice (" << node << ")");

  // BUGBUG FIXME
  //
  // You _must_ support broadcast to get any sort of packet from the ARP layer.
  EnableBroadcast (MacAddress ("ff:ff:ff:ff:ff:ff"));
  EnableMulticast();
  EnablePointToPoint();
}

PointToPointNetDevice::~PointToPointNetDevice()
{
  NS_DEBUG ("PointToPointNetDevice::~PointToPointNetDevice ()");

  delete m_queue;
  m_queue = 0;
}

//
// Copy constructor for PointToPointNetDevice.
//
// We use the underlying NetDevice copy constructor to get the base class
// copied.  These just remain as is (e.g. you get the same name, the same
// MAC address).  If you need to fix them up, YOU, the copier need to do 
// that.
// 
// The things we need to be careful of are the channel, the queue and the
// trace callback.  If the channel pointer is non-zero, we copy the pointer 
// and add a reference.  If the queue is non-zero, we copy it using the queue
// assignment operator.  We don't mess with the trace -- we just reset it.
// We're assuming that the tracing will be set up after the topology creation
// phase and this won't actually matter.
//
PointToPointNetDevice::PointToPointNetDevice (const PointToPointNetDevice& nd)
: 
  NetDevice(nd), 
  m_txMachineState(READY),
  m_bps (nd.m_bps),
  m_tInterframeGap (nd.m_tInterframeGap),
  m_channel(0), 
  m_queue(0),
  m_rxTrace ()
{
  NS_DEBUG ("PointToPointNetDevice::PointToPointNetDevice (" << &nd << ")");

  if (nd.m_channel)
    {
      m_channel = nd.m_channel;
      m_channel->Ref ();
    }

  if (nd.m_queue)
    {
      m_queue = nd.m_queue;
    }
    
}

void PointToPointNetDevice::DoDispose()
{
 if (m_channel != 0)
 {
   m_channel->Unref ();
   m_channel = 0;
 }
 NetDevice::DoDispose ();
}

//
// Assignment operator for PointToPointNetDevice.
//
// This uses the non-obvious trick of taking the source net device passed by
// value instead of by reference.  This causes the copy constructor to be
// invoked (where the real work is done -- see above).  All we have to do
// here is to return the newly constructed net device.
//
  PointToPointNetDevice&
PointToPointNetDevice::operator= (const PointToPointNetDevice nd)
{
  NS_DEBUG ("PointToPointNetDevice::operator= (" << &nd << ")");
  return *this;
}

  void 
PointToPointNetDevice::SetDataRate(DataRate bps)
{
  m_bps = bps;
}

  void 
PointToPointNetDevice::SetInterframeGap(Time t)
{
  m_tInterframeGap = t;
}

  bool
PointToPointNetDevice::SendTo (Packet& p, const MacAddress& dest)
{
  NS_DEBUG ("PointToPointNetDevice::SendTo (" << &p << ", " << &dest << ")");
  NS_DEBUG ("PointToPointNetDevice::SendTo (): UID is " << p.GetUid () << ")");

  NS_ASSERT (IsLinkUp ());

#ifdef NOTYET
    struct NetDevicePacketDestAddress tag;
    tag.address = address;
    p.AddTag (tag);
#endif

//
// This class simulates a point to point device.  In the case of a serial
// link, this means that we're simulating something like a UART.  This is
// not a requirement for a point-to-point link, but it's a typical model for
// the device.  
//
// Generally, a real device will have a list of pending packets to transmit.  
// An on-device CPU frees the main CPU(s) of the details of what is happening
// in the device and feeds the USART.  The main CPU basically just sees the 
// list of packets -- it puts packets into the list, and the device frees the
// packets when they are transmitted.
//
// In the case of our virtual device here, the queue pointed to by m_queue
// corresponds to this list.  The main CPU adds packets to the list by 
// calling this method and when the device completes a send, the packets are
// freed in an "interrupt" service routine.
//
// We're going to do the same thing here.  So first of all, the incoming packet
// goes onto our queue if possible.  If the queue can't handle it, there's
// nothing to be done.
//
    if (m_queue->Enqueue(p) == false )
      {
        return false;
      }
//
// If there's a transmission in progress, the "interrupt" will keep the
// transmission process going.  If the device is idle, we need to start a
// transmission.
//
// In the real world, the USART runs until it finishes sending bits, and then
// pulls on the device's transmit complete interrupt wire.  At the same time,
// the electrons from the last wiggle of the wire are busy propagating down
// the wire.  In the case of a long speed-of-light delay in the wire, we could
// conceivably start transmitting the next packet before the end of the 
// previously sent data has even reached the end of the wire.  This situation
// is usually avoided (like the plague) and an "interframe gap" is introduced.
// This is usually the round-trip delay on the channel plus some hard-to-
// quantify receiver turn-around time (the time required for the receiver
// to process the last frame and prepare for reception of the next).
//
// So, if the transmit machine is ready, we need to schedule a transmit 
// complete event (at which time we tell the channel we're no longer sending
// bits).  A separate transmit ready event (at which time the transmitter 
// becomes ready to start sending bits again is scheduled there).  Finally, 
// we tell the channel (via TransmitStart ()) that we've started wiggling the 
// wire and bits are coming out.
//
// If the transmit machine is not ready, we just leave and the transmit ready
// event we know is coming will kick-start the transmit process.
//
    if (m_txMachineState == READY) 
      {
        return TransmitStart (p);
      }
    return true;
}

  bool
PointToPointNetDevice::TransmitStart (Packet &p)
{
  NS_DEBUG ("PointToPointNetDevice::TransmitStart (" << &p << ")");
  NS_DEBUG (
    "PointToPointNetDevice::TransmitStart (): UID is " << p.GetUid () << ")");
//
// This function is called to start the process of transmitting a packet.
// We need to tell the channel that we've started wiggling the wire and
// schedule an event that will be executed when it's time to tell the 
// channel that we're done wiggling the wire.
//
  NS_ASSERT_MSG(m_txMachineState == READY, "Must be READY to transmit");
  m_txMachineState = BUSY;
  Time tEvent = Seconds (m_bps.CalculateTxTime(p.GetSize()));

  NS_DEBUG ("PointToPointNetDevice::TransmitStart (): " <<
    "Schedule TransmitCompleteEvent in " << 
    tEvent.GetSeconds () << "sec");

  Simulator::Schedule (tEvent, 
                       &PointToPointNetDevice::TransmitCompleteEvent, 
                       this);
  return m_channel->TransmitStart (p, this); 
}

  void
PointToPointNetDevice::TransmitCompleteEvent (void)
{
  NS_DEBUG ("PointToPointNetDevice::TransmitCompleteEvent ()");
//
// This function is called to finish the  process of transmitting a packet.
// We need to tell the channel that we've stopped wiggling the wire and
// schedule an event that will be executed when it's time to re-enable
// the transmitter after the interframe gap.
//
  NS_ASSERT_MSG(m_txMachineState == BUSY, "Must be BUSY if transmitting");
  m_txMachineState = GAP;
  Packet p;
  bool found;
  found = m_queue->Dequeue (p);
  NS_ASSERT_MSG(found, "Packet must be on queue if transmitted");
  NS_DEBUG ("PointToPointNetDevice::TransmitCompleteEvent (): Pkt UID is " << 
            p.GetUid () << ")");
  m_channel->TransmitEnd (p, this); 

  NS_DEBUG (
    "PointToPointNetDevice::TransmitCompleteEvent (): " <<
    "Schedule TransmitReadyEvent in "
    << m_tInterframeGap.GetSeconds () << "sec");

  Simulator::Schedule (m_tInterframeGap, 
                       &PointToPointNetDevice::TransmitReadyEvent, 
                       this);
}

  void
PointToPointNetDevice::TransmitReadyEvent (void)
{
  NS_DEBUG ("PointToPointNetDevice::TransmitReadyEvent ()");
//
// This function is called to enable the transmitter after the interframe
// gap has passed.  If there are pending transmissions, we use this opportunity
// to start the next transmit.
//
  NS_ASSERT_MSG(m_txMachineState == GAP, "Must be in interframe gap");
  m_txMachineState = READY;

  if (m_queue->IsEmpty())
    {
      return;
    }
  else
    {
      Packet p;
      bool found;
      found = m_queue->Peek (p);
      NS_ASSERT_MSG(found, "IsEmpty false but no Packet on queue?");
      TransmitStart (p);
    }
}

TraceResolver *
PointToPointNetDevice::DoCreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("queue", 
                 MakeCallback (&Queue::CreateTraceResolver, m_queue),
                 PointToPointNetDevice::QUEUE);
  resolver->Add ("rx",
                 m_rxTrace,
                 PointToPointNetDevice::RX);
  return resolver;
}

bool
PointToPointNetDevice::Attach (PointToPointChannel* ch)
{
  NS_DEBUG ("PointToPointNetDevice::Attach (" << &ch << ")");

  if (m_channel)
    {
      m_channel->Unref ();
      m_channel = 0;
    }

  m_channel = ch;
  m_channel->Ref ();

  m_channel->Attach(this);
  m_bps = m_channel->GetDataRate ();
  m_tInterframeGap = m_channel->GetDelay ();

  /* 
   * For now, this device is up whenever a channel is attached to it.
   * In fact, it should become up only when the second device
   * is attached to the channel. So, there should be a way for
   * a PointToPointChannel to notify both of its attached devices
   * that the channel is 'complete', hence that the devices are
   * up, hence that they can call NotifyLinkUp. 
   */
  NotifyLinkUp ();
  return true;
}

void
PointToPointNetDevice::AddQueue (Queue* q)
{
  NS_DEBUG ("PointToPointNetDevice::AddQueue (" << q << ")");

  m_queue = q;
}

void
PointToPointNetDevice::Receive (Packet& p)
{
  // ignore return value for now.
  NS_DEBUG ("PointToPointNetDevice::Receive (" << &p << ")");

  m_rxTrace (p);
  ForwardUp (p);
}

Queue* 
PointToPointNetDevice::GetQueue(void) const 
{ 
    return m_queue;
}

Channel* 
PointToPointNetDevice::DoGetChannel(void) const 
{ 
    m_channel->Ref();
    return m_channel;
}

bool 
PointToPointNetDevice::DoNeedsArp (void) const
{
  return false;
}

} // namespace ns3
