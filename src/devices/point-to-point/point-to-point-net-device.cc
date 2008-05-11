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
 * Author:  Craig Dowell <craigdo@ee.washington.edu>
 * Revised: George Riley <riley@ece.gatech.edu>
 */

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"
#include "ns3/llc-snap-header.h"
#include "ns3/error-model.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "point-to-point-net-device.h"
#include "point-to-point-channel.h"

NS_LOG_COMPONENT_DEFINE ("PointToPointNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PointToPointNetDevice);

TypeId 
PointToPointNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PointToPointNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<PointToPointNetDevice> ()
    .AddAttribute ("Address", "The address of this device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&PointToPointNetDevice::m_address),
                   MakeMac48AddressChecker ())
    .AddAttribute ("DataRate", "The default data rate for point to point links",
                   DataRateValue (DataRate ("10Mb/s")),
                   MakeDataRateAccessor (&PointToPointNetDevice::m_bps),
                   MakeDataRateChecker ())
    .AddAttribute ("ReceiveErrorModel", "XXX",
                   PointerValue (),
                   MakePointerAccessor (&PointToPointNetDevice::m_receiveErrorModel),
                   MakePointerChecker<ErrorModel> ())
    .AddAttribute ("TxQueue", "XXX",
                   PointerValue (),
                   MakePointerAccessor (&PointToPointNetDevice::m_queue),
                   MakePointerChecker<Queue> ())
    .AddAttribute ("InterframeGap", "XXX",
                   TimeValue (Seconds (0.0)),
                   MakeTimeAccessor (&PointToPointNetDevice::m_tInterframeGap),
                   MakeTimeChecker ())
    .AddTraceSource ("Rx", "Receive MAC packet.",
                     MakeTraceSourceAccessor (&PointToPointNetDevice::m_rxTrace))
    .AddTraceSource ("Drop", "Drop MAC packet.",
                     MakeTraceSourceAccessor (&PointToPointNetDevice::m_dropTrace))

    ;
  return tid;
}


PointToPointNetDevice::PointToPointNetDevice () 
: 
  m_txMachineState (READY),
  m_channel (0), 
  m_name (""),
  m_linkUp (false),
  m_mtu (0xffff)
{
  NS_LOG_FUNCTION (this);
}

PointToPointNetDevice::~PointToPointNetDevice ()
{}

void 
PointToPointNetDevice::SetAddress (Mac48Address self)
{
  m_address = self;
}

void 
PointToPointNetDevice::AddHeader(Ptr<Packet> p, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION_NOARGS ();
  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  p->AddHeader (llc);
}

bool 
PointToPointNetDevice::ProcessHeader(Ptr<Packet> p, uint16_t& param)
{
  NS_LOG_FUNCTION_NOARGS ();
  LlcSnapHeader llc;
  p->RemoveHeader (llc);

  param = llc.GetType ();

  return true;
}

void PointToPointNetDevice::DoDispose()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_channel = 0;
  m_receiveErrorModel = 0;
  NetDevice::DoDispose ();
}

void PointToPointNetDevice::SetDataRate(const DataRate& bps)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_channel || bps <= m_channel->GetDataRate ())
    {
      m_bps = bps;
    }
}

void PointToPointNetDevice::SetInterframeGap(const Time& t)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_tInterframeGap = t;
}

bool
PointToPointNetDevice::TransmitStart (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  NS_LOG_LOGIC ("UID is " << p->GetUid () << ")");
//
// This function is called to start the process of transmitting a packet.
// We need to tell the channel that we've started wiggling the wire and
// schedule an event that will be executed when the transmission is complete.
//
  NS_ASSERT_MSG(m_txMachineState == READY, "Must be READY to transmit");
  m_txMachineState = BUSY;
  Time txTime = Seconds (m_bps.CalculateTxTime(p->GetSize()));
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
  NS_LOG_FUNCTION_NOARGS ();
//
// This function is called to finish the  process of transmitting a packet.
// We need to tell the channel that we've stopped wiggling the wire and
// get the next packet from the queue.  If the queue is empty, we are
// done, otherwise transmit the next packet.
//
  NS_ASSERT_MSG(m_txMachineState == BUSY, "Must be BUSY if transmitting");
  m_txMachineState = READY;
  Ptr<Packet> p = m_queue->Dequeue ();
  if (p == 0)
    {
      return; // Nothing to do at this point
    }
  TransmitStart(p);
}

bool 
PointToPointNetDevice::Attach (Ptr<PointToPointChannel> ch)
{
  NS_LOG_FUNCTION (this << &ch);

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

void PointToPointNetDevice::SetQueue (Ptr<Queue> q)
{
  NS_LOG_FUNCTION (this << q);

  m_queue = q;
}

void PointToPointNetDevice::SetReceiveErrorModel (Ptr<ErrorModel> em)
{
  NS_LOG_FUNCTION ("(" << em << ")");

  m_receiveErrorModel = em;
}

void PointToPointNetDevice::Receive (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  uint16_t protocol = 0;

  if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt (packet) ) 
    {
      m_dropTrace (packet);
      // Do not forward up; let this packet go
    }
  else 
    {
      m_rxTrace (packet);
      ProcessHeader(packet, protocol);
      m_rxCallback (this, packet, protocol, GetBroadcast ());
    }
}

Ptr<Queue> PointToPointNetDevice::GetQueue(void) const 
{ 
  NS_LOG_FUNCTION_NOARGS ();
  return m_queue;
}

void
PointToPointNetDevice::NotifyLinkUp (void)
{
  m_linkUp = true;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

void 
PointToPointNetDevice::SetName(const std::string name)
{
  m_name = name;
}
std::string 
PointToPointNetDevice::GetName(void) const
{
  return m_name;
}
void 
PointToPointNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
PointToPointNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
PointToPointNetDevice::GetChannel (void) const
{
  return m_channel;
}
Address 
PointToPointNetDevice::GetAddress (void) const
{
  return m_address;
}
bool 
PointToPointNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}
uint16_t 
PointToPointNetDevice::GetMtu (void) const
{
  return m_mtu;
}
bool 
PointToPointNetDevice::IsLinkUp (void) const
{
  return m_linkUp;
}
void 
PointToPointNetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  m_linkChangeCallback = callback;
}
bool 
PointToPointNetDevice::IsBroadcast (void) const
{
  return true;
}
Address
PointToPointNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
PointToPointNetDevice::IsMulticast (void) const
{
  return false;
}
Address 
PointToPointNetDevice::GetMulticast (void) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
Address 
PointToPointNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
bool 
PointToPointNetDevice::IsPointToPoint (void) const
{
  return true;
}
bool 
PointToPointNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC ("p=" << packet << ", dest=" << &dest);
  NS_LOG_LOGIC ("UID is " << packet->GetUid ());

  // GFR Comment. Why is this an assertion? Can't a link legitimately
  // "go down" during the simulation?  Shouldn't we just wait for it
  // to come back up?
  NS_ASSERT (IsLinkUp ());
  AddHeader(packet, protocolNumber);

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
      m_queue->Enqueue (packet);
      packet = m_queue->Dequeue ();
      return TransmitStart (packet);
    }
  else
    {
      return m_queue->Enqueue(packet);
    }
}
Ptr<Node> 
PointToPointNetDevice::GetNode (void) const
{
  return m_node;
}
void 
PointToPointNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}
bool 
PointToPointNetDevice::NeedsArp (void) const
{
  return false;
}
void 
PointToPointNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}


} // namespace ns3
