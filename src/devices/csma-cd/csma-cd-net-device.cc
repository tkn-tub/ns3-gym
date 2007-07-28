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
#include "csma-cd-net-device.h"
#include "csma-cd-channel.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"

NS_DEBUG_COMPONENT_DEFINE ("CsmaCdNetDevice");

namespace ns3 {

CsmaCdNetDevice::CsmaCdNetDevice (Ptr<Node> node, MacAddress addr, 
                                  CsmaCdEncapsulationMode pktType) 
  : NetDevice(node, addr), m_bps (DataRate (0xffffffff))
{
  NS_DEBUG ("CsmaCdNetDevice::CsmaCdNetDevice (" << node << ")");
  m_pktType = pktType;

  Init(true, true);
}

CsmaCdNetDevice::CsmaCdNetDevice (Ptr<Node> node, MacAddress addr, 
                                  CsmaCdEncapsulationMode pktType,
                                  bool sendEnable, bool receiveEnable) 
  : NetDevice(node, addr), m_bps (DataRate (0xffffffff))
{
  NS_DEBUG ("CsmaCdNetDevice::CsmaCdNetDevice (" << node << ")");
  m_pktType = pktType;

  Init(sendEnable, receiveEnable);
}

CsmaCdNetDevice::~CsmaCdNetDevice()
{
  NS_DEBUG ("CsmaCdNetDevice::~CsmaCdNetDevice ()");
  m_queue = 0;
}

void 
CsmaCdNetDevice::DoDispose ()
{
  m_channel = 0;
  NetDevice::DoDispose ();
}

//
// Assignment operator for CsmaCdNetDevice.
//
// This uses the non-obvious trick of taking the source net device passed by
// value instead of by reference.  This causes the copy constructor to be
// invoked (where the real work is done -- see above).  All we have to do
// here is to return the newly constructed net device.
//
/*
CsmaCdNetDevice&
CsmaCdNetDevice::operator= (const CsmaCdNetDevice nd)
{
  NS_DEBUG ("CsmaCdNetDevice::operator= (" << &nd << ")");
  return *this;
}
*/

void 
CsmaCdNetDevice::Init(bool sendEnable, bool receiveEnable)
{
  m_txMachineState = READY;
  m_tInterframeGap = Seconds(0);
  m_channel = 0; 
  m_queue = 0;

  EnableBroadcast (MacAddress ("ff:ff:ff:ff:ff:ff"));
  EnableMulticast();
  EnablePointToPoint();

  SetSendEnable (sendEnable);
  SetReceiveEnable (receiveEnable);
}

void
CsmaCdNetDevice::SetSendEnable (bool sendEnable)
{
  m_sendEnable = sendEnable;
}

void
CsmaCdNetDevice::SetReceiveEnable (bool receiveEnable)
{
  m_receiveEnable = receiveEnable;
}
bool
CsmaCdNetDevice::IsSendEnabled (void)
{
  return (m_sendEnable);
}

bool
CsmaCdNetDevice::IsReceiveEnabled (void)
{
  return (m_receiveEnable);
}

void 
CsmaCdNetDevice::SetDataRate (DataRate bps)
{
  m_bps = bps;
}

void 
CsmaCdNetDevice::SetInterframeGap (Time t)
{
  m_tInterframeGap = t;
}

void 
CsmaCdNetDevice::SetBackoffParams (Time slotTime, uint32_t minSlots, 
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
CsmaCdNetDevice::AddHeader (Packet& p, const MacAddress& dest,
                                  uint16_t protocolNumber)
{
  if ((m_pktType == ETHERNET_V1) || (m_pktType == IP_ARP))
    {
      EthernetHeader  header;
      EthernetTrailer trailer;
      
      header.SetSource(this->GetAddress());
      header.SetDestination(dest);
      switch (m_pktType)
        {
        case IP_ARP:
          header.SetLengthType(protocolNumber);
          break;
        default:
          header.SetLengthType(p.GetSize() + header.GetSize() + trailer.GetSize());
          break;
        }
      p.AddHeader(header);
      trailer.CalcFcs(p);
      p.AddTrailer(trailer);
    } 
  else if (m_pktType == LLC) 
    {
      LlcSnapHeader llc;
      llc.SetType (protocolNumber);
      p.AddHeader (llc);
    }
}
bool 
CsmaCdNetDevice::ProcessHeader(Packet& p, int& param)
{
  bool retVal = true;

  if ((m_pktType == ETHERNET_V1) || (m_pktType == IP_ARP))
    {
      EthernetHeader  header;
      EthernetTrailer trailer;
      
      p.RemoveTrailer(trailer);
      trailer.CheckFcs(p);
      p.RemoveHeader(header);
      
      param = header.GetLengthType();
      if ((header.GetDestination() != this->GetBroadcast()) &&
          (header.GetDestination() != this->GetAddress()))
        {
          retVal = false;
        }
    }
  else if (m_pktType == LLC)
    {
      LlcSnapHeader llc;
      p.RemoveHeader (llc);

      param = llc.GetType ();
    }

  return retVal;
}

bool
CsmaCdNetDevice::DoNeedsArp (void) const
{
  if ((m_pktType == IP_ARP) || (m_pktType == LLC))
    {
      return true;
    } else {
      return false;
    }
}

bool
CsmaCdNetDevice::SendTo (Packet& p, const MacAddress& dest, uint16_t protocolNumber)
{
  NS_DEBUG ("CsmaCdNetDevice::SendTo (" << &p << ")");
  NS_DEBUG ("CsmaCdNetDevice::SendTo (): UID is " << p.GetUid () << ")");

  NS_ASSERT (IsLinkUp ());

  // Only transmit if send side of net device is enabled
  if (!IsSendEnabled())
    return false;

  AddHeader(p, dest, protocolNumber);

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
CsmaCdNetDevice::TransmitStart ()
{
  NS_DEBUG ("CsmaCdNetDevice::TransmitStart (" << &m_currentPkt << ")");
  NS_DEBUG ("CsmaCdNetDevice::TransmitStart (): UID is " 
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
        } else {
          m_backoff.IncrNumRetries();
          Time backoffTime = m_backoff.GetBackoffTime();

          NS_DEBUG ("CsmaCdNetDevice::TransmitStart (): " 
                    << "Channel busy, backing off for " 
                    << backoffTime.GetSeconds () << "sec");

          Simulator::Schedule (backoffTime, 
                               &CsmaCdNetDevice::TransmitStart, 
                               this);
        }
    } else {
      // Channel is free, transmit packet
      m_txMachineState = BUSY;
      Time tEvent = Seconds (m_bps.CalculateTxTime(m_currentPkt.GetSize()));
      
      NS_DEBUG ("CsmaCdNetDevice::TransmitStart (): " <<
                "Schedule TransmitCompleteEvent in " << 
                tEvent.GetSeconds () << "sec");
      
      Simulator::Schedule (tEvent, 
                           &CsmaCdNetDevice::TransmitCompleteEvent, 
                           this);
      if (!m_channel->TransmitStart (m_currentPkt, m_deviceId))
        {
          NS_DEBUG ("CsmaCdNetDevice::TransmitStart (): " <<
                    "Channel transmit start did not work at " << 
                    tEvent.GetSeconds () << "sec");
          m_txMachineState = READY;
        } else {
          // Transmission success, reset backoff time parameters.
          m_backoff.ResetBackoffTime();
        }
    }
}


void
CsmaCdNetDevice::TransmitAbort (void)
{
  NS_DEBUG ("CsmaCdNetDevice::TransmitAbort ()");

  NS_DEBUG ("CsmaCdNetDevice::TransmitAbort (): Pkt UID is " <<
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
CsmaCdNetDevice::TransmitCompleteEvent (void)
{
  NS_DEBUG ("CsmaCdNetDevice::TransmitCompleteEvent ()");
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

  NS_DEBUG ("CsmaCdNetDevice::TransmitCompleteEvent (): Pkt UID is " << 
            m_currentPkt.GetUid () << ")");
  m_channel->TransmitEnd (); 

  NS_DEBUG (
    "CsmaCdNetDevice::TransmitCompleteEvent (): " <<
    "Schedule TransmitReadyEvent in "
    << m_tInterframeGap.GetSeconds () << "sec");

  Simulator::Schedule (m_tInterframeGap, 
                       &CsmaCdNetDevice::TransmitReadyEvent, 
                       this);
}

void
CsmaCdNetDevice::TransmitReadyEvent (void)
{
  NS_DEBUG ("CsmaCdNetDevice::TransmitReadyEvent ()");
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

TraceResolver *
CsmaCdNetDevice::DoCreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("queue", 
                 MakeCallback (&Queue::CreateTraceResolver, 
                               PeekPointer (m_queue)),
                 CsmaCdNetDevice::QUEUE);
  resolver->Add ("rx",
                 m_rxTrace,
                 CsmaCdNetDevice::RX);
  return resolver;
}

bool
CsmaCdNetDevice::Attach (Ptr<CsmaCdChannel> ch)
{
  NS_DEBUG ("CsmaCdNetDevice::Attach (" << &ch << ")");

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
CsmaCdNetDevice::AddQueue (Ptr<Queue> q)
{
  NS_DEBUG ("CsmaCdNetDevice::AddQueue (" << q << ")");

  m_queue = q;
}

void
CsmaCdNetDevice::Receive (Packet& p)
{
  NS_DEBUG ("CsmaCdNetDevice::Receive UID is (" << p.GetUid() << ")");

  // Only receive if send side of net device is enabled
  if (!IsReceiveEnabled())
    return;

  int param = 0;
  Packet packet = p;

  if (ProcessHeader(packet, param))
    {
      m_rxTrace (packet);
      ForwardUp (packet, param);
    } else {
      m_dropTrace (packet);
    }
}

Ptr<Queue>
CsmaCdNetDevice::GetQueue(void) const 
{ 
    return m_queue;
}

Ptr<Channel>
CsmaCdNetDevice::DoGetChannel(void) const 
{ 
    return m_channel;
}

} // namespace ns3
