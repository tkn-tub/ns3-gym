/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "ns3/assert.h"
#include "ns3/uinteger.h"
#include "wifi-mac-parameters.h"

namespace ns3 {

Time
WifiMacParameters::GetDefaultMaxPropagationDelay (void)
{
  // 1000m 
  return Seconds (1000.0 / 300000000.0);
}
Time
WifiMacParameters::GetDefaultSlot (void)
{
  // 802.11-a specific
  return MicroSeconds (9);
}
Time
WifiMacParameters::GetDefaultSifs (void)
{
  // 802.11-a specific
  return MicroSeconds (16);
}
Time
WifiMacParameters::GetDefaultCtsAckDelay (void)
{
  // 802.11-a specific: 6mb/s
  return MicroSeconds (44);
}
Time
WifiMacParameters::GetDefaultCtsAckTimeout (void)
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
WifiMacParameters::GetTypeId (void)
{
  static TypeId tid = TypeId ("WifiMacParameters")
    .SetParent<Object> ()
    .AddConstructor<WifiMacParameters> ()
    .AddAttribute ("CtsTimeout", "XXX",
                   GetDefaultCtsAckTimeout (),
                   MakeTimeAccessor (&WifiMacParameters::m_ctsTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("AckTimeout", "XXX",
                   GetDefaultCtsAckTimeout (),
                   MakeTimeAccessor (&WifiMacParameters::m_ackTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("Sifs", "XXX",
                   GetDefaultSifs (),
                   MakeTimeAccessor (&WifiMacParameters::m_sifs),
                   MakeTimeChecker ())
    .AddAttribute ("Slot", "XXX",
                   GetDefaultSlot (),
                   MakeTimeAccessor (&WifiMacParameters::m_slot),
                   MakeTimeChecker ())
    .AddAttribute ("Pifs", "XXX",
                   GetDefaultSifs () + GetDefaultSlot (),
                   MakeTimeAccessor (&WifiMacParameters::m_pifs),
                   MakeTimeChecker ())
    .AddAttribute ("MaxSsrc", "XXX",
                   Uinteger (7),
                   MakeUintegerAccessor (&WifiMacParameters::m_maxSsrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxSlrc", "XXX",
                   Uinteger (7),
                   MakeUintegerAccessor (&WifiMacParameters::m_maxSlrc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RtsCtsThreshold", "XXX",
                   Uinteger (1500),
                   MakeUintegerAccessor (&WifiMacParameters::m_rtsCtsThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FragmentationThreshold", "XXX",
                   Uinteger (1500),
                   MakeUintegerAccessor (&WifiMacParameters::m_fragmentationThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxPropagationDelay", "XXX",
                   GetDefaultMaxPropagationDelay (),
                   MakeTimeAccessor (&WifiMacParameters::m_maxPropagationDelay),
                   MakeTimeChecker ())
    ;
  return tid;
}

WifiMacParameters::WifiMacParameters ()
{}

void 
WifiMacParameters::SetSlotTime (Time slotTime)
{
  m_slot = slotTime;
}


Time
WifiMacParameters::GetPifs (void) const
{
  return m_pifs;
}
Time
WifiMacParameters::GetSifs (void) const
{
  return m_sifs;
}
Time
WifiMacParameters::GetSlotTime (void) const
{
  return m_slot;
}
Time
WifiMacParameters::GetCtsTimeout (void) const
{
  return m_ctsTimeout;
}
Time
WifiMacParameters::GetAckTimeout (void) const
{
  return m_ackTimeout;
}

Time
WifiMacParameters::GetBeaconInterval (void) const
{
  return Seconds (1);
}
uint32_t 
WifiMacParameters::GetMaxSsrc (void) const
{
  return m_maxSsrc;
}
uint32_t 
WifiMacParameters::GetMaxSlrc (void) const
{
  return m_maxSlrc;
}
uint32_t 
WifiMacParameters::GetRtsCtsThreshold (void) const
{
  return m_rtsCtsThreshold;
}
uint32_t 
WifiMacParameters::GetFragmentationThreshold (void) const
{
  NS_ASSERT (GetMaxMsduSize () / 16 < m_fragmentationThreshold);
  return m_fragmentationThreshold;
}
Time
WifiMacParameters::GetMsduLifetime (void) const
{
  return Seconds (10);
}
Time
WifiMacParameters::GetMaxPropagationDelay (void) const
{
  return m_maxPropagationDelay;
}

uint32_t 
WifiMacParameters::GetMaxMsduSize (void) const
{
  return 2304;
}
double 
WifiMacParameters::GetCapLimit (void) const
{
  return 0.4;
}
double 
WifiMacParameters::GetMinEdcaTrafficProportion (void) const
{
  return 0.4;
}

} // namespace ns3
