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

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/mac48-address.h"
#include "ns3/llc-snap-header.h"
#include "ns3/error-model.h"
#include "point-to-point-net-device.h"
#include "point-to-point-channel.h"

NS_LOG_COMPONENT_DEFINE ("PointToPointNetDevice");

namespace ns3 {

DataRateDefaultValue PointToPointNetDevice::g_defaultRate(
           "PointToPointLinkDataRate", 
           "The default data rate for point to point links",
           DataRate ("10Mb/s"));

PointToPointTraceType::PointToPointTraceType (enum Type type)
  : m_type (type)
{
  NS_LOG_FUNCTION;
}
PointToPointTraceType::PointToPointTraceType ()
  : m_type (RX)
{
  NS_LOG_FUNCTION;
}

void 
PointToPointTraceType::Print (std::ostream &os) const
{
  switch (m_type) {
  case RX:
    os << "dev-rx";
    break;
  case DROP:
    os << "dev-drop";
    break;
  }
}

uint16_t 
PointToPointTraceType::GetUid (void)
{
  NS_LOG_FUNCTION;
  static uint16_t uid = AllocateUid<PointToPointTraceType> ("PointToPointTraceType");
  return uid;
}

std::string 
PointToPointTraceType::GetTypeName (void) const
{
  NS_LOG_FUNCTION;
  return "ns3::PointToPointTraceType";
}

enum PointToPointTraceType::Type
PointToPointTraceType::Get (void) const
{
  NS_LOG_FUNCTION;
  return m_type;
}

PointToPointNetDevice::PointToPointNetDevice (Ptr<Node> node,
                                              const DataRate& rate) 
: 
  NetDevice(node, Mac48Address::Allocate ()), 
  m_txMachineState (READY),
  m_bps (rate),
  m_tInterframeGap (Seconds(0)),
  m_channel (0), 
  m_queue (0),
  m_rxTrace (),
  m_dropTrace (),
  m_receiveErrorModel (0)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << node << ")");
//
// XXX BUGBUG
//
// You _must_ support broadcast to get any sort of packet from the ARP layer.
//
  EnableBroadcast (Mac48Address ("ff:ff:ff:ff:ff:ff"));
//
// We want to allow multicast packets to flow across this link
//
  EnableMulticast (Mac48Address ("01:00:5e:00:00:00"));
  EnablePointToPoint();
}

PointToPointNetDevice::~PointToPointNetDevice()
{
  NS_LOG_FUNCTION;
  m_queue = 0;
  m_receiveErrorModel = 0;
}

void 
PointToPointNetDevice::AddHeader(Packet& p, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION;
  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  p.AddHeader (llc);
}

bool 
PointToPointNetDevice::ProcessHeader(Packet& p, uint16_t& param)
{
  NS_LOG_FUNCTION;
  LlcSnapHeader llc;
  p.RemoveHeader (llc);

  param = llc.GetType ();

  return true;
}

void PointToPointNetDevice::DoDispose()
{
  NS_LOG_FUNCTION;
  m_channel = 0;
  NetDevice::DoDispose ();
}

void PointToPointNetDevice::SetDataRate(const DataRate& bps)
{
  NS_LOG_FUNCTION;
  if (!m_channel || bps <= m_channel->GetDataRate ())
    {
      m_bps = bps;
    }
}

void PointToPointNetDevice::SetInterframeGap(const Time& t)
{
  NS_LOG_FUNCTION;
  m_tInterframeGap = t;
}

bool PointToPointNetDevice::SendTo (const Packet& packet, const Address& dest, 
                                    uint16_t protocolNumber)
{
  NS_LOG_FUNCTION;
  Packet p = packet;
  NS_LOG_LOGIC ("p=" << &p << ", dest=" << &dest);
  NS_LOG_LOGIC ("UID is " << p.GetUid ());

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
// transmission; otherwise we send it now.
  if (m_txMachineState == READY) 
    {
// We still enqueue and dequeue it to hit the tracing hooks
      m_queue->Enqueue (p);
      m_queue->Dequeue (p);
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
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << &p << ")");
  NS_LOG_LOGIC ("UID is " << p.GetUid () << ")");
//
// This function is called to start the process of transmitting a packet.
// We need to tell the channel that we've started wiggling the wire and
// schedule an event that will be executed when the transmission is complete.
//
  NS_ASSERT_MSG(m_txMachineState == READY, "Must be READY to transmit");
  m_txMachineState = BUSY;
  Time txTime = Seconds (m_bps.CalculateTxTime(p.GetSize()));
  Time txCompleteTime = txTime + m_tInterframeGap;

  NS_LOG_LOGIC ("Schedule TransmitCompleteEvent in " << 
    txCompleteTime.GetSeconds () << "sec");
  // Schedule the tx complete event
  Simulator::Schedule (txCompleteTime, 
                       &PointToPointNetDevice::TransmitComplete, 
                       this);
  return m_channel->TransmitStart(p, this, txTime); 
}

void PointToPointNetDevice::TransmitComplete (void)
{
  NS_LOG_FUNCTION;
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

Ptr<TraceResolver> 
PointToPointNetDevice::GetTraceResolver (void) const
{
  NS_LOG_FUNCTION;
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddComposite ("queue", m_queue);
  resolver->AddSource ("rx",
                       TraceDoc ("receive MAC packet",
                                 "const Packet &", "packet received"),
                       m_rxTrace,
                       PointToPointTraceType (PointToPointTraceType::RX));
  resolver->AddSource ("drop",
                       TraceDoc ("drop MAC packet",
                                 "const Packet &", "packet dropped"),
                       m_dropTrace,
                       PointToPointTraceType (PointToPointTraceType::DROP));
  resolver->SetParentResolver (NetDevice::GetTraceResolver ());
  return resolver;
}

bool 
PointToPointNetDevice::Attach (Ptr<PointToPointChannel> ch)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << &ch << ")");

  m_channel = ch;

  m_channel->Attach(this);
  m_bps = m_channel->GetDataRate ();
  // GFR Comment.  Below is definitely wrong.  Interframe gap
  // is unrelated to channel delay.
  // -- unlesss you want to introduce a default gap which is there to avoid
  // parts of multiple packets flowing on the "wire" at the same time.
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
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << q << ")");

  m_queue = q;
}

void PointToPointNetDevice::AddReceiveErrorModel (Ptr<ErrorModel> em)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << em << ")");

  m_receiveErrorModel = em;
  AddInterface (em);
}

void PointToPointNetDevice::Receive (Packet& p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << &p << ")");
  uint16_t protocol = 0;
  Packet packet = p;

  if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt (p) ) 
    {
      m_dropTrace (packet);
      // Do not forward up; let this packet go
    }
  else 
    {
      m_rxTrace (packet);
      ProcessHeader(packet, protocol);
      ForwardUp (packet, protocol, GetBroadcast ());
    }
}

Ptr<Queue> PointToPointNetDevice::GetQueue(void) const 
{ 
  NS_LOG_FUNCTION;
  return m_queue;
}

Ptr<Channel> PointToPointNetDevice::DoGetChannel(void) const 
{ 
  NS_LOG_FUNCTION;
  return m_channel;
}

bool PointToPointNetDevice::DoNeedsArp (void) const
{
  NS_LOG_FUNCTION;
  return false;
}

} // namespace ns3
