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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/log.h"
#include "dcf-state.h"
#include "dca-txop.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DcfState");

NS_OBJECT_ENSURE_REGISTERED (DcfState);

TypeId
DcfState::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DcfState")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
  ;
  return tid;
}

DcfState::DcfState (Ptr<DcaTxop> txop)
  : m_backoffSlots (0),
    m_backoffStart (Seconds (0.0)),
    m_cwMin (0),
    m_cwMax (0),
    m_cw (0),
    m_accessRequested (false),
    m_txop (txop)
{
  NS_LOG_FUNCTION (this);
}

DcfState::~DcfState ()
{
  NS_LOG_FUNCTION (this);
}

void
DcfState::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_txop->Dispose ();
  m_txop = 0;
}

void
DcfState::SetAifsn (uint32_t aifsn)
{
  NS_LOG_FUNCTION (this << aifsn);
  m_aifsn = aifsn;
}

void
DcfState::SetTxopLimit (Time txopLimit)
{
  NS_LOG_FUNCTION (this << txopLimit);
  NS_ASSERT_MSG ((txopLimit.GetMicroSeconds () % 32 == 0), "The TXOP limit must be expressed in multiple of 32 microseconds!");
  m_txopLimit = txopLimit;
}

void
DcfState::SetCwMin (uint32_t minCw)
{
  NS_LOG_FUNCTION (this << minCw);
  bool changed = (m_cwMin != minCw);
  m_cwMin = minCw;
  if (changed == true)
    {
      ResetCw ();
    }
}

void
DcfState::SetCwMax (uint32_t maxCw)
{
  NS_LOG_FUNCTION (this << maxCw);
  bool changed = (m_cwMax != maxCw);
  m_cwMax = maxCw;
  if (changed == true)
    {
      ResetCw ();
    }
}

uint32_t
DcfState::GetAifsn (void) const
{
  return m_aifsn;
}

Time
DcfState::GetTxopLimit (void) const
{
  return m_txopLimit;
}

uint32_t
DcfState::GetCwMin (void) const
{
  return m_cwMin;
}

uint32_t
DcfState::GetCwMax (void) const
{
  return m_cwMax;
}

void
DcfState::ResetCw (void)
{
  NS_LOG_FUNCTION (this);
  m_cw = m_cwMin;
}

void
DcfState::UpdateFailedCw (void)
{
  NS_LOG_FUNCTION (this);
  //see 802.11-2012, section 9.19.2.5
  m_cw = std::min ( 2 * (m_cw + 1) - 1, m_cwMax);
}

void
DcfState::UpdateBackoffSlotsNow (uint32_t nSlots, Time backoffUpdateBound)
{
  NS_LOG_FUNCTION (this << nSlots << backoffUpdateBound);
  m_backoffSlots -= nSlots;
  m_backoffStart = backoffUpdateBound;
  NS_LOG_DEBUG ("update slots=" << nSlots << " slots, backoff=" << m_backoffSlots);
}

void
DcfState::StartBackoffNow (uint32_t nSlots)
{
  NS_LOG_FUNCTION (this << nSlots);
  if (m_backoffSlots != 0)
    {
      NS_LOG_DEBUG ("reset backoff from " << m_backoffSlots << " to " << nSlots << " slots");
    }
  else
    {
      NS_LOG_DEBUG ("start backoff=" << nSlots << " slots");
    }
  m_backoffSlots = nSlots;
  m_backoffStart = Simulator::Now ();
}

uint32_t
DcfState::GetCw (void) const
{
  return m_cw;
}

uint32_t
DcfState::GetBackoffSlots (void) const
{
  return m_backoffSlots;
}

Time
DcfState::GetBackoffStart (void) const
{
  return m_backoffStart;
}

bool
DcfState::IsAccessRequested (void) const
{
  return m_accessRequested;
}

void
DcfState::NotifyAccessRequested (void)
{
  NS_LOG_FUNCTION (this);
  m_accessRequested = true;
}

void
DcfState::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_accessRequested);
  m_accessRequested = false;
  m_txop->NotifyAccessGranted ();
}

void
DcfState::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  m_txop->NotifyCollision ();
}

void
DcfState::NotifyInternalCollision (void)
{
  NS_LOG_FUNCTION (this);
  m_txop->NotifyInternalCollision ();
}

void
DcfState::NotifyChannelSwitching (void)
{
  NS_LOG_FUNCTION (this);
  m_txop->NotifyChannelSwitching ();
}

void
DcfState::NotifySleep (void)
{
  NS_LOG_FUNCTION (this);
  m_txop->NotifySleep ();
}

void
DcfState::NotifyWakeUp (void)
{
  NS_LOG_FUNCTION (this);
  m_txop->NotifyWakeUp ();
}

bool
DcfState::IsEdca (void) const
{
  NS_LOG_FUNCTION (this);
  return m_txop->IsEdca ();
}

} //namespace ns3
