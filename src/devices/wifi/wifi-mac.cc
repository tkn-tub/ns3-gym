/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "wifi-mac.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiMac);

Time
WifiMac::GetDefaultMaxPropagationDelay (void)
{
  // 1000m 
  return Seconds (1000.0 / 300000000.0);
}
Time
WifiMac::GetDefaultSlot (void)
{
  // 802.11-a specific
  return MicroSeconds (9);
}
Time
WifiMac::GetDefaultSifs (void)
{
  // 802.11-a specific
  return MicroSeconds (16);
}
Time
WifiMac::GetDefaultEifsNoDifs (void)
{
  return GetDefaultSifs () + GetDefaultCtsAckDelay ();
}
Time
WifiMac::GetDefaultCtsAckDelay (void)
{
  // 802.11-a specific: 6mb/s
  return MicroSeconds (44);
}
Time
WifiMac::GetDefaultCtsAckTimeout (void)
{
  /* Cts_Timeout and Ack_Timeout are specified in the Annex C 
     (Formal description of MAC operation, see details on the 
     Trsp timer setting at page 346)
  */
  Time ctsTimeout = GetDefaultSifs ();
  ctsTimeout += GetDefaultCtsAckDelay ();
  ctsTimeout += GetDefaultMaxPropagationDelay () * Scalar (2);
  ctsTimeout += GetDefaultSlot ();
  return ctsTimeout;
}

TypeId 
WifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiMac")
    .SetParent<Object> ()
    .AddAttribute ("CtsTimeout", "When this timeout expires, the RTS/CTS handshake has failed.",
                   TimeValue (GetDefaultCtsAckTimeout ()),
                   MakeTimeAccessor (&WifiMac::SetCtsTimeout,
                                     &WifiMac::GetCtsTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("AckTimeout", "When this timeout expires, the DATA/ACK handshake has failed.",
                   TimeValue (GetDefaultCtsAckTimeout ()),
                   MakeTimeAccessor (&WifiMac::GetAckTimeout,
                                     &WifiMac::SetAckTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("Sifs", "The value of the SIFS constant.",
                   TimeValue (GetDefaultSifs ()),
                   MakeTimeAccessor (&WifiMac::SetSifs,
				     &WifiMac::GetSifs),
                   MakeTimeChecker ())
    .AddAttribute ("EifsNoDifs", "The value of EIFS-DIFS",
		   TimeValue (GetDefaultEifsNoDifs ()),
		   MakeTimeAccessor (&WifiMac::SetEifsNoDifs,
				     &WifiMac::GetEifsNoDifs),
		   MakeTimeChecker ())
    .AddAttribute ("Slot", "The duration of a Slot.",
                   TimeValue (GetDefaultSlot ()),
                   MakeTimeAccessor (&WifiMac::SetSlot,
				     &WifiMac::GetSlot),
                   MakeTimeChecker ())
    .AddAttribute ("Pifs", "The value of the PIFS constant.",
                   TimeValue (GetDefaultSifs () + GetDefaultSlot ()),
                   MakeTimeAccessor (&WifiMac::SetPifs,
                                     &WifiMac::GetPifs),
                   MakeTimeChecker ())
    .AddAttribute ("MaxPropagationDelay", "The maximum propagation delay. Unused for now.",
                   TimeValue (GetDefaultMaxPropagationDelay ()),
                   MakeTimeAccessor (&WifiMac::m_maxPropagationDelay),
                   MakeTimeChecker ())
    .AddAttribute ("MaxMsduSize", "The maximum size of an MSDU accepted by the MAC layer."
                   "This value conforms to the specification.",
		   UintegerValue (2304),
		   MakeUintegerAccessor (&WifiMac::m_maxMsduSize),
		   MakeUintegerChecker<uint16_t> (1,2304))
    .AddAttribute ("Ssid", "The ssid we want to belong to.",
		   SsidValue (Ssid ("default")),
		   MakeSsidAccessor (&WifiMac::GetSsid,
				     &WifiMac::SetSsid),
		   MakeSsidChecker ())
    .AddTraceSource ("MacTx", 
                     "A packet has been received from higher layers and is being processed in preparation for "
                     "queueing for transmission.",
                     MakeTraceSourceAccessor (&WifiMac::m_macTxTrace))
    .AddTraceSource ("MacTxDrop", 
                     "A packet has been dropped in the MAC layer before being queued for transmission.",
                     MakeTraceSourceAccessor (&WifiMac::m_macTxDropTrace))
    .AddTraceSource ("MacRx", 
                     "A packet has been received by this device, has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.",
                     MakeTraceSourceAccessor (&WifiMac::m_macRxTrace))
    .AddTraceSource ("MacRxDrop", 
                     "A packet has been dropped in the MAC layer after it has been passed up from the physical "
                     "layer.",
                     MakeTraceSourceAccessor (&WifiMac::m_macRxDropTrace))
#if 0
    // Not currently implemented in this device
    .AddTraceSource ("Sniffer", 
                     "Trace source simulating a non-promiscuous packet sniffer attached to the device",
                     MakeTraceSourceAccessor (&WifiMac::m_snifferTrace))
#endif
    ;

  return tid;
}

void 
WifiMac::SetMaxPropagationDelay (Time delay)
{
  m_maxPropagationDelay = delay;
}

Time
WifiMac::GetMsduLifetime (void) const
{
  return Seconds (10);
}
Time
WifiMac::GetMaxPropagationDelay (void) const
{
  return m_maxPropagationDelay;
}

uint32_t 
WifiMac::GetMaxMsduSize (void) const
{
  return m_maxMsduSize;
}

void 
WifiMac::NotifyTx (Ptr<const Packet> packet)
{
  m_macTxTrace (packet);
}

void 
WifiMac::NotifyTxDrop (Ptr<const Packet> packet) 
{
  m_macTxDropTrace (packet);
}

void 
WifiMac::NotifyRx (Ptr<const Packet> packet) 
{
  m_macRxTrace (packet);
}

void 
WifiMac::NotifyRxDrop (Ptr<const Packet> packet) 
{
  m_macRxDropTrace (packet);
}

} // namespace ns3
