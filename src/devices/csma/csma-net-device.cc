/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise
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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */

#include <iostream>
#include <cassert>
#include "ns3/debug.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/composite-trace-resolver.h"
#include "csma-net-device.h"
#include "csma-channel.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"

NS_DEBUG_COMPONENT_DEFINE ("CsmaNetDevice");

namespace ns3 {

CsmaTraceType::CsmaTraceType (enum Type type)
  : m_type (type)
{}
CsmaTraceType::CsmaTraceType ()
  : m_type (RX)
{}
void 
CsmaTraceType::Print (std::ostream &os) const
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
CsmaTraceType::GetUid (void)
{
  static uint16_t uid = AllocateUid<CsmaTraceType> ("CsmaTraceType");
  return uid;
}
std::string 
CsmaTraceType::GetTypeName (void) const
{
  return "ns3::CsmaTraceType";
}
enum CsmaTraceType::Type 
CsmaTraceType::Get (void) const
{
  return m_type;
}


CsmaNetDevice::CsmaNetDevice (Ptr<Node> node)
  : NetDevice (node, Eui48Address::Allocate ()),
    m_bps (DataRate (0xffffffff))
{
  NS_DEBUG ("CsmaNetDevice::CsmaNetDevice (" << node << ")");
  m_encapMode = IP_ARP;
  Init(true, true);
}

CsmaNetDevice::CsmaNetDevice (Ptr<Node> node, Eui48Address addr, 
                                  CsmaEncapsulationMode encapMode) 
  : NetDevice(node, addr), 
    m_bps (DataRate (0xffffffff))
{
  NS_DEBUG ("CsmaNetDevice::CsmaNetDevice (" << node << ")");
  m_encapMode = encapMode;

  Init(true, true);
}

CsmaNetDevice::CsmaNetDevice (Ptr<Node> node, Eui48Address addr, 
                                  CsmaEncapsulationMode encapMode,
                                  bool sendEnable, bool receiveEnable) 
  : NetDevice(node, addr), 
    m_bps (DataRate (0xffffffff))
{
  NS_DEBUG ("CsmaNetDevice::CsmaNetDevice (" << node << ")");
  m_encapMode = encapMode;

  Init(sendEnable, receiveEnable);
}

CsmaNetDevice::~CsmaNetDevice()
{
  NS_DEBUG ("CsmaNetDevice::~CsmaNetDevice ()");
  m_queue = 0;
}

void 
CsmaNetDevice::DoDispose ()
{
  m_channel = 0;
  NetDevice::DoDispose ();
}

//
// Assignment operator for CsmaNetDevice.
//
// This uses the non-obvious trick of taking the source net device passed by
// value instead of by reference.  This causes the copy constructor to be
// invoked (where the real work is done -- see above).  All we have to do
// here is to return the newly constructed net device.
//
/*
CsmaNetDevice&
CsmaNetDevice::operator= (const CsmaNetDevice nd)
{
  NS_DEBUG ("CsmaNetDevice::operator= (" << &nd << ")");
  return *this;
}
*/

void 
CsmaNetDevice::Init(bool sendEnable, bool receiveEnable)
{
  m_txMachineState = READY;
  m_tInterframeGap = Seconds(0);
  m_channel = 0; 
  m_queue = 0;

  EnableBroadcast (Eui48Address ("ff:ff:ff:ff:ff:ff"));
  EnableMulticast();

  SetSendEnable (sendEnable);
  SetReceiveEnable (receiveEnable);
}

void
CsmaNetDevice::SetSendEnable (bool sendEnable)
{
  m_sendEnable = sendEnable;
}

void
CsmaNetDevice::SetReceiveEnable (bool receiveEnable)
{
  m_receiveEnable = receiveEnable;
}
bool
CsmaNetDevice::IsSendEnabled (void)
{
  return (m_sendEnable);
}

bool
CsmaNetDevice::IsReceiveEnabled (void)
{
  return (m_receiveEnable);
}

void 
CsmaNetDevice::SetDataRate (DataRate bps)
{
  m_bps = bps;
}

void 
CsmaNetDevice::SetInterframeGap (Time t)
{
  m_tInterframeGap = t;
}

void 
CsmaNetDevice::SetBackoffParams (Time slotTime, uint32_t minSlots, 
                                      uint32_t maxSlots, uint32_t ceiling, 
                                      uint32_t maxRetries)
{
  m_backoff.m_slotTime = slotTime;
  m_backoff.m_minSlots = minSlots;
  m_backoff.m_maxSlots = maxSlots;
  m_backoff.m_ceiling = ceiling;
  m_backoff.m_maxRetries = maxRetries;
}
void 
CsmaNetDevice::AddHeader (Packet& p, Eui48Address dest,
                            uint16_t protocolNumber)
{
  if (m_encapMode == RAW)
    {
      return;
    }
  EthernetHeader header (false);
  EthernetTrailer trailer;
  Eui48Address source = Eui48Address::ConvertFrom (GetAddress ());
  header.SetSource(source);
  header.SetDestination(dest);

  uint16_t lengthType = 0;
  switch (m_encapMode) 
    {
    case ETHERNET_V1:
      lengthType = p.GetSize() + header.GetSerializedSize() + trailer.GetSerializedSize();
      break;
    case IP_ARP:
      lengthType = protocolNumber;
      break;
    case LLC: {
      LlcSnapHeader llc;
      llc.SetType (protocolNumber);
      p.AddHeader (llc);
    } break;
    case RAW:
      NS_ASSERT (false);
      break;
    }
  header.SetLengthType (lengthType);
  p.AddHeader(header);
  trailer.CalcFcs(p);
  p.AddTrailer(trailer);
}
bool 
CsmaNetDevice::ProcessHeader (Packet& p, uint16_t & param)
{
  if (m_encapMode == RAW)
    {
      return true;
    }
  EthernetHeader header (false);
  EthernetTrailer trailer;
      
  p.RemoveTrailer(trailer);
  trailer.CheckFcs(p);
  p.RemoveHeader(header);

  if ((header.GetDestination() != GetBroadcast ()) &&
      (header.GetDestination() != GetAddress ()))
    {
      return false;
    }

  switch (m_encapMode)
    {
    case ETHERNET_V1:
    case IP_ARP:
      param = header.GetLengthType();
      break;
    case LLC: {
      LlcSnapHeader llc;
      p.RemoveHeader (llc);
      param = llc.GetType ();
    } break;
    case RAW:
      NS_ASSERT (false);
      break;
    }
  return true;
}

bool
CsmaNetDevice::DoNeedsArp (void) const
{
  if ((m_encapMode == IP_ARP) || (m_encapMode == LLC))
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

bool
CsmaNetDevice::SendTo (
  const Packet& packet, 
  const Address& dest, 
  uint16_t protocolNumber)
{
  Packet p = packet;
  NS_DEBUG ("CsmaNetDevice::SendTo (" << &p << ")");
  NS_DEBUG ("CsmaNetDevice::SendTo (): UID is " << p.GetUid () << ")");

  NS_ASSERT (IsLinkUp ());

  // Only transmit if send side of net device is enabled
  if (!IsSendEnabled())
    return false;

  Eui48Address destination = Eui48Address::ConvertFrom (dest);
  AddHeader(p, destination, protocolNumber);

  // Place the packet to be sent on the send queue
  if (m_queue->Enqueue(p) == false )
    {
      return false;
    }
  // If the device is idle, we need to start a transmission. Otherwise,
  // the transmission will be started when the current packet finished
  // transmission (see TransmitCompleteEvent)
  if (m_txMachineState == READY) 
    {
      // Store the next packet to be transmitted
      if (m_queue->Dequeue (m_currentPkt))
        {
          TransmitStart();
        }
    }
  return true;
}

void
CsmaNetDevice::TransmitStart ()
{
  NS_DEBUG ("CsmaNetDevice::TransmitStart (" << &m_currentPkt << ")");
  NS_DEBUG ("CsmaNetDevice::TransmitStart (): UID is " 
            << m_currentPkt.GetUid () << ")");
//
// This function is called to start the process of transmitting a packet.
// We need to tell the channel that we've started wiggling the wire and
// schedule an event that will be executed when it's time to tell the 
// channel that we're done wiggling the wire.
//
  NS_ASSERT_MSG((m_txMachineState == READY) || (m_txMachineState == BACKOFF), 
                "Must be READY to transmit. Tx state is: " 
                << m_txMachineState);

  // Only transmit if send side of net device is enabled
  if (!IsSendEnabled())
    return;

  if (m_channel->GetState() != IDLE)
    { // Channel busy, backoff and rechedule TransmitStart()
      m_txMachineState = BACKOFF;
      if (m_backoff.MaxRetriesReached())
        { // Too many retries reached, abort transmission of packet
          TransmitAbort();
        } 
      else 
        {
          m_backoff.IncrNumRetries();
          Time backoffTime = m_backoff.GetBackoffTime();

          NS_DEBUG ("CsmaNetDevice::TransmitStart (): " 
                    << "Channel busy, backing off for " 
                    << backoffTime.GetSeconds () << "sec");

          Simulator::Schedule (backoffTime, 
                               &CsmaNetDevice::TransmitStart, 
                               this);
        }
    } 
  else 
    {
      // Channel is free, transmit packet
      m_txMachineState = BUSY;
      Time tEvent = Seconds (m_bps.CalculateTxTime(m_currentPkt.GetSize()));
      
      NS_DEBUG ("CsmaNetDevice::TransmitStart (): " <<
                "Schedule TransmitCompleteEvent in " << 
                tEvent.GetSeconds () << "sec");
      
      Simulator::Schedule (tEvent, 
                           &CsmaNetDevice::TransmitCompleteEvent, 
                           this);
      if (!m_channel->TransmitStart (m_currentPkt, m_deviceId))
        {
          NS_DEBUG ("CsmaNetDevice::TransmitStart (): " <<
                    "Channel transmit start did not work at " << 
                    tEvent.GetSeconds () << "sec");
          m_txMachineState = READY;
        } 
      else 
        {
          // Transmission success, reset backoff time parameters.
          m_backoff.ResetBackoffTime();
        }
    }
}


void
CsmaNetDevice::TransmitAbort (void)
{
  NS_DEBUG ("CsmaNetDevice::TransmitAbort ()");

  NS_DEBUG ("CsmaNetDevice::TransmitAbort (): Pkt UID is " <<
            m_currentPkt.GetUid () << ")");

  // Try to transmit a new packet
  bool found;
  found = m_queue->Dequeue (m_currentPkt);
  NS_ASSERT_MSG(found, "IsEmpty false but no Packet on queue?");
  m_backoff.ResetBackoffTime();
  m_txMachineState = READY;
  TransmitStart ();
}

void
CsmaNetDevice::TransmitCompleteEvent (void)
{
  NS_DEBUG ("CsmaNetDevice::TransmitCompleteEvent ()");
//
// This function is called to finish the  process of transmitting a packet.
// We need to tell the channel that we've stopped wiggling the wire and
// schedule an event that will be executed when it's time to re-enable
// the transmitter after the interframe gap.
//
  NS_ASSERT_MSG(m_txMachineState == BUSY, "Must be BUSY if transmitting");
  // Channel should be transmitting
  NS_ASSERT(m_channel->GetState() == TRANSMITTING);
  m_txMachineState = GAP;

  NS_DEBUG ("CsmaNetDevice::TransmitCompleteEvent (): Pkt UID is " << 
            m_currentPkt.GetUid () << ")");
  m_channel->TransmitEnd (); 

  NS_DEBUG (
    "CsmaNetDevice::TransmitCompleteEvent (): " <<
    "Schedule TransmitReadyEvent in "
    << m_tInterframeGap.GetSeconds () << "sec");

  Simulator::Schedule (m_tInterframeGap, 
                       &CsmaNetDevice::TransmitReadyEvent, 
                       this);
}

void
CsmaNetDevice::TransmitReadyEvent (void)
{
  NS_DEBUG ("CsmaNetDevice::TransmitReadyEvent ()");
//
// This function is called to enable the transmitter after the interframe
// gap has passed.  If there are pending transmissions, we use this opportunity
// to start the next transmit.
//
  NS_ASSERT_MSG(m_txMachineState == GAP, "Must be in interframe gap");
  m_txMachineState = READY;

  // Get the next packet from the queue for transmitting
  if (m_queue->IsEmpty())
    {
      return;
    }
  else
    {
      bool found;
      found = m_queue->Dequeue (m_currentPkt);
      NS_ASSERT_MSG(found, "IsEmpty false but no Packet on queue?");
      TransmitStart ();
    }
}

Ptr<TraceResolver>
CsmaNetDevice::GetTraceResolver (void) const
{
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddComposite ("queue", m_queue);
  resolver->AddSource ("rx",
                       TraceDoc ("receive MAC packet",
                                 "const Packet &", "packet received"),
                       m_rxTrace,
                       CsmaTraceType (CsmaTraceType::RX));
  resolver->AddSource ("drop",
                       TraceDoc ("drop MAC packet",
                                 "const Packet &", "packet dropped"),
                       m_dropTrace,
                       CsmaTraceType (CsmaTraceType::DROP));
  resolver->SetParentResolver (NetDevice::GetTraceResolver ());
  return resolver;
}

bool
CsmaNetDevice::Attach (Ptr<CsmaChannel> ch)
{
  NS_DEBUG ("CsmaNetDevice::Attach (" << &ch << ")");

  m_channel = ch;

  m_deviceId = m_channel->Attach(this);
  m_bps = m_channel->GetDataRate ();
  m_tInterframeGap = m_channel->GetDelay ();

  /* 
   * For now, this device is up whenever a channel is attached to it.
   */
  NotifyLinkUp ();
  return true;
}

void
CsmaNetDevice::AddQueue (Ptr<Queue> q)
{
  NS_DEBUG ("CsmaNetDevice::AddQueue (" << q << ")");

  m_queue = q;
}

void
CsmaNetDevice::Receive (const Packet& packet)
{
  EthernetHeader header (false);
  EthernetTrailer trailer;
  Eui48Address broadcast;
  Eui48Address destination;
  Packet p = packet;

  NS_DEBUG ("CsmaNetDevice::Receive UID is (" << p.GetUid() << ")");

  // Only receive if send side of net device is enabled
  if (!IsReceiveEnabled())
    {
      m_dropTrace (p);
      return;
    }

  if (m_encapMode == RAW)
    {
      ForwardUp (packet, 0, GetBroadcast ());
      m_dropTrace (p);
      return;
    }
  p.RemoveTrailer(trailer);
  trailer.CheckFcs(p);
  p.RemoveHeader(header);

  broadcast = Eui48Address::ConvertFrom (GetBroadcast ());
  destination = Eui48Address::ConvertFrom (GetAddress ());
  if ((header.GetDestination() != broadcast) &&
      (header.GetDestination() != destination))
    {
      // not for us.
      m_dropTrace (p);
      return;
    }

  m_rxTrace (p);
//
// protocol must be initialized to avoid a compiler warning in the RAW
// case that breaks the optimized build.
//
  uint16_t protocol = 0;

  switch (m_encapMode)
    {
    case ETHERNET_V1:
    case IP_ARP:
      protocol = header.GetLengthType();
      break;
    case LLC: {
      LlcSnapHeader llc;
      p.RemoveHeader (llc);
      protocol = llc.GetType ();
    } break;
    case RAW:
      NS_ASSERT (false);
      break;
    }
  
  ForwardUp (p, protocol, header.GetSource ());
  return;
}

Ptr<Queue>
CsmaNetDevice::GetQueue(void) const 
{ 
    return m_queue;
}

Ptr<Channel>
CsmaNetDevice::DoGetChannel(void) const 
{ 
    return m_channel;
}

} // namespace ns3
