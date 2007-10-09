/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include <cassert>

#include "mac-parameters.h"
#include "wifi-phy.h"
#include "wifi-mac-header.h"

namespace ns3 {

MacParameters::MacParameters ()
{
  m_rtsCtsThreshold = 1000;
  m_fragmentationThreshold = 2000;
  m_maxSsrc = 7;
  m_maxSlrc = 7;
}
void 
MacParameters::Initialize80211a (WifiPhy const*phy)
{
  m_sifs = MicroSeconds (16);
  m_slot = MicroSeconds (9);
  /* see section 9.2.10 ieee 802.11-1999 */
  m_pifs = m_sifs + m_slot;
  // 1000m 
  m_maxPropagationDelay = Seconds (1000.0 / 300000000.0);
  /* Cts_Timeout and Ack_Timeout are specified in the Annex C 
     (Formal description of MAC operation, see details on the 
     Trsp timer setting at page 346)
  */
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_CTL_CTS);
  m_ctsTimeout = m_sifs;
  m_ctsTimeout += phy->CalculateTxDuration (hdr.GetSize (), phy->GetMode (0), phy->GetMode (0));
  m_ctsTimeout += m_maxPropagationDelay * Scalar (2);
  m_ctsTimeout += m_slot;

  hdr.SetType (WIFI_MAC_CTL_ACK);
  m_ackTimeout = m_sifs;
  m_ackTimeout += phy->CalculateTxDuration (hdr.GetSize (), phy->GetMode (0), phy->GetMode (0));
  m_ackTimeout += m_maxPropagationDelay * Scalar (2);
  m_ackTimeout += m_slot;
}

void 
MacParameters::SetSlotTime (Time slotTime)
{
  m_slot = slotTime;
}

void 
MacParameters::SetMaxSsrc (uint32_t ssrc)
{
  m_maxSsrc = ssrc;
}
void 
MacParameters::SetMaxSlrc (uint32_t slrc)
{
  m_maxSlrc = slrc;
}
void 
MacParameters::SetRtsCtsThreshold (uint32_t threshold)
{
  m_rtsCtsThreshold = threshold;
}
void 
MacParameters::SetFragmentationThreshold (uint32_t threshold)
{
  m_fragmentationThreshold = threshold;
}

Time
MacParameters::GetPifs (void) const
{
  return m_pifs;
}
Time
MacParameters::GetSifs (void) const
{
  return m_sifs;
}
Time
MacParameters::GetSlotTime (void) const
{
  return m_slot;
}
Time
MacParameters::GetCtsTimeout (void) const
{
  return m_ctsTimeout;
}
Time
MacParameters::GetAckTimeout (void) const
{
  return m_ackTimeout;
}

Time
MacParameters::GetBeaconInterval (void) const
{
  return Seconds (1);
}
uint32_t 
MacParameters::GetMaxSsrc (void) const
{
  return m_maxSsrc;
}
uint32_t 
MacParameters::GetMaxSlrc (void) const
{
  return m_maxSlrc;
}
uint32_t 
MacParameters::GetRtsCtsThreshold (void) const
{
  return m_rtsCtsThreshold;
}
uint32_t 
MacParameters::GetFragmentationThreshold (void) const
{
  assert (GetMaxMsduSize () / 16 < m_fragmentationThreshold);
  return m_fragmentationThreshold;
}
Time
MacParameters::GetMsduLifetime (void) const
{
  return Seconds (10);
}
uint32_t 
MacParameters::GetMaxQueueSize (void) const
{
  return 400;
}
Time
MacParameters::GetMaxPropagationDelay (void) const
{
  return m_maxPropagationDelay;
}

uint32_t 
MacParameters::GetMaxMsduSize (void) const
{
  return 2304;
}
double 
MacParameters::GetCapLimit (void) const
{
  return 0.4;
}
double 
MacParameters::GetMinEdcaTrafficProportion (void) const
{
  return 0.4;
}

}; // namespace ns3
