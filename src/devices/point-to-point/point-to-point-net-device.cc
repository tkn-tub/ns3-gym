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
 * Author:  Craig Dowell <craigdo@ee.washington.edu>
 * Revised: George Riley <riley@ece.gatech.edu>
 */

#include <iostream>
#include <cassert>
#include "ns3/debug.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/eui48-address.h"
#include "ns3/llc-snap-header.h"
#include "point-to-point-net-device.h"
#include "point-to-point-channel.h"

NS_DEBUG_COMPONENT_DEFINE ("PointToPointNetDevice");

namespace ns3 {

DataRateDefaultValue PointToPointNetDevice::g_defaultRate(
           "PointToPointLinkDataRate", 
           "The default data rate for point to point links",
           DataRate ("10Mb/s"));

PointToPointNetDevice::PointToPointNetDevice (Ptr<Node> node,
                                              const DataRate& rate) 
: 
  NetDevice(node, Eui48Address::Allocate ().ConvertTo ()), 
  m_txMachineState (READY),
  m_bps (rate),
  m_tInterframeGap (Seconds(0)),
  m_channel (0), 
  m_queue (0),
  m_rxTrace ()
{
  NS_DEBUG ("PointToPointNetDevice::PointToPointNetDevice (" << node << ")");

  // BUGBUG FIXME
  //
  // You _must_ support broadcast to get any sort of packet from the ARP layer.
  EnableBroadcast (Eui48Address ("ff:ff:ff:ff:ff:ff").ConvertTo ());
  EnableMulticast();
  EnablePointToPoint();
}

PointToPointNetDevice::~PointToPointNetDevice()
{
  NS_DEBUG ("PointToPointNetDevice::~PointToPointNetDevice ()");
  m_queue = 0;
}

void 
PointToPointNetDevice::AddHeader(Packet& p, uint16_t protocolNumber)
{
  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  p.AddHeader (llc);
}

bool 
PointToPointNetDevice::ProcessHeader(Packet& p, uint16_t& param)
{
  LlcSnapHeader llc;
  p.RemoveHeader (llc);

  param = llc.GetType ();

  return true;
}

void PointToPointNetDevice::DoDispose()
{
  m_channel = 0;
  NetDevice::DoDispose ();
}

void PointToPointNetDevice::SetDataRate(const DataRate& bps)
{
  m_bps = bps;
}

void PointToPointNetDevice::SetInterframeGap(const Time& t)
{
  m_tInterframeGap = t;
}

bool PointToPointNetDevice::SendTo (const Packet& packet, const Address& dest, 
                                    uint16_t protocolNumber)
{
  Packet p = packet;
  NS_DEBUG ("PointToPointNetDevice::SendTo (" << &p << ", " << &dest << ")");
  NS_DEBUG ("PointToPointNetDevice::SendTo (): UID is " << p.GetUid () << ")");

  // GFR Comment. Why is this an assertion? Can't a link legitimately
  // "go down" during the simulation?  Shouldn't we just wait for it
  // to come back up?
  NS_ASSERT (IsLinkUp ());
  AddHeader(p, protocolNumber);

//
// This class simulates a point to point device.  In the case of a serial
// link, this means that we're simulating something like a UART.
//
//
// If there's a transmission in progress, we enque the packet for later
// trnsmission; otherwise we send it now.
  if (m_txMachineState == READY) 
    {
      return TransmitStart (p);
    }
  else
    {
      return m_queue->Enqueue(p);
    }
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
// schedule an event that will be executed when the transmission is complete.
//
  NS_ASSERT_MSG(m_txMachineState == READY, "Must be READY to transmit");
  m_txMachineState = BUSY;
  Time txTime = Seconds (m_bps.CalculateTxTime(p.GetSize()));
  Time txCompleteTime = txTime + m_tInterframeGap;

  NS_DEBUG ("PointToPointNetDevice::TransmitStart (): " <<
    "Schedule TransmitCompleteEvent in " << 
    txCompleteTime.GetSeconds () << "sec");
  // Schedule the tx complete event
  Simulator::Schedule (txCompleteTime, 
                       &PointToPointNetDevice::TransmitComplete, 
                       this);
  return m_channel->TransmitStart(p, this, txTime); 
}

void PointToPointNetDevice::TransmitComplete (void)
{
  NS_DEBUG ("PointToPointNetDevice::TransmitCompleteEvent ()");
//
// This function is called to finish the  process of transmitting a packet.
// We need to tell the channel that we've stopped wiggling the wire and
// get the next packet from the queue.  If the queue is empty, we are
// done, otherwise transmit the next packet.
//
  NS_ASSERT_MSG(m_txMachineState == BUSY, "Must be BUSY if transmitting");
  m_txMachineState = READY;
  Packet p;
  if (!m_queue->Dequeue(p)) return; // Nothing to do at this point
  TransmitStart(p);
}

TraceResolver* PointToPointNetDevice::DoCreateTraceResolver (
                                      TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("queue", 
                 MakeCallback (&Queue::CreateTraceResolver, PeekPointer (m_queue)),
                 PointToPointNetDevice::QUEUE);
  resolver->Add ("rx",
                 m_rxTrace,
                 PointToPointNetDevice::RX);
  return resolver;
}

bool PointToPointNetDevice::Attach (Ptr<PointToPointChannel> ch)
{
  NS_DEBUG ("PointToPointNetDevice::Attach (" << &ch << ")");

  m_channel = ch;

  m_channel->Attach(this);
  m_bps = m_channel->GetDataRate ();
  // GFR Comment.  Below is definitely wrong.  Interframe gap
  // is unrelated to channel delay.
  //m_tInterframeGap = m_channel->GetDelay ();

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

void PointToPointNetDevice::AddQueue (Ptr<Queue> q)
{
  NS_DEBUG ("PointToPointNetDevice::AddQueue (" << q << ")");

  m_queue = q;
}

void PointToPointNetDevice::Receive (Packet& p)
{
  NS_DEBUG ("PointToPointNetDevice::Receive (" << &p << ")");
  uint16_t protocol = 0;
  Packet packet = p;

  ProcessHeader(packet, protocol);
  m_rxTrace (packet);
  ForwardUp (packet, protocol, GetBroadcast ());
}

Ptr<Queue> PointToPointNetDevice::GetQueue(void) const 
{ 
    return m_queue;
}

Ptr<Channel> PointToPointNetDevice::DoGetChannel(void) const 
{ 
    return m_channel;
}

bool PointToPointNetDevice::DoNeedsArp (void) const
{
  return false;
}

} // namespace ns3
