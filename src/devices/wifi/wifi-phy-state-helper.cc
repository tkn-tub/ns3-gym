/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "wifi-phy-state-helper.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("WifiPhyStateHelper");

namespace ns3 {

WifiPhyStateHelper::WifiPhyStateHelper ()
  : m_syncing (false),
    m_endTx (Seconds (0)),
    m_endSync (Seconds (0)),
    m_endCcaBusy (Seconds (0)),
    m_startTx (Seconds (0)),
    m_startSync (Seconds (0)),
    m_startCcaBusy (Seconds (0)),
    m_previousStateChangeTime (Seconds (0))
{
  NS_LOG_FUNCTION (this);
}

void 
WifiPhyStateHelper::SetReceiveOkCallback (WifiPhy::SyncOkCallback callback)
{
  m_syncOkCallback = callback;
}
void 
WifiPhyStateHelper::SetReceiveErrorCallback (WifiPhy::SyncErrorCallback callback)
{
  m_syncErrorCallback = callback;
}
void 
WifiPhyStateHelper::RegisterListener (WifiPhyListener *listener)
{
  m_listeners.push_back (listener);
}

bool 
WifiPhyStateHelper::IsStateCcaBusy (void)
{
  return GetState () == WifiPhy::CCA_BUSY;
}
 
bool 
WifiPhyStateHelper::IsStateIdle (void)
{
  return (GetState () == WifiPhy::IDLE)?true:false;
}
bool 
WifiPhyStateHelper::IsStateBusy (void)
{
  return (GetState () != WifiPhy::IDLE)?true:false;
}
bool 
WifiPhyStateHelper::IsStateSync (void)
{
  return (GetState () == WifiPhy::SYNC)?true:false;
}
bool 
WifiPhyStateHelper::IsStateTx (void)
{
  return (GetState () == WifiPhy::TX)?true:false;
}



Time
WifiPhyStateHelper::GetStateDuration (void)
{
  return Simulator::Now () - m_previousStateChangeTime;
}

Time
WifiPhyStateHelper::GetDelayUntilIdle (void)
{
  Time retval;

  switch (GetState ()) {
  case WifiPhy::SYNC:
    retval = m_endSync - Simulator::Now ();
    break;
  case WifiPhy::TX:
    retval = m_endTx - Simulator::Now ();
    break;
  case WifiPhy::CCA_BUSY:
    retval = m_endCcaBusy - Simulator::Now ();
    break;
  case WifiPhy::IDLE:
    retval = Seconds (0);
    break;
  default:
    NS_ASSERT (false);
    // NOTREACHED
    retval = Seconds (0);
    break;
  }
  retval = Max (retval, Seconds (0));
  return retval;
}

Time 
WifiPhyStateHelper::GetLastRxStartTime (void) const
{
  return m_startSync;
}

char const *
WifiPhyStateHelper::StateToString (enum WifiPhy::State state)
{
  switch (state) {
  case WifiPhy::TX:
    return "TX";
    break;
  case WifiPhy::CCA_BUSY:
    return "CCA_BUSY";
    break;
  case WifiPhy::IDLE:
    return "IDLE";
    break;
  case WifiPhy::SYNC:
    return "SYNC";
    break;
  default:
    NS_ASSERT (false);
    // quiet compiler
    return "INVALID";
    break;
  }
}

enum WifiPhy::State 
WifiPhyStateHelper::GetState (void)
{
  if (m_endTx > Simulator::Now ()) 
    {
      return WifiPhy::TX;
    } 
  else if (m_syncing) 
    {
      return WifiPhy::SYNC;
    } 
  else if (m_endCcaBusy > Simulator::Now ()) 
    {
      return WifiPhy::CCA_BUSY;
    } 
  else 
    {
      return WifiPhy::IDLE;
    }
}


void 
WifiPhyStateHelper::NotifyTxStart (Time duration)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyTxStart (duration);
  }
}
void 
WifiPhyStateHelper::NotifySyncStart (Time duration)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxStart (duration);
  }
}
void 
WifiPhyStateHelper::NotifySyncEndOk (void)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxEndOk ();
  }
}
void 
WifiPhyStateHelper::NotifySyncEndError (void)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxEndError ();
  }
}
void 
WifiPhyStateHelper::NotifyCcaBusyStart (Time duration)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyCcaBusyStart (duration);
  }
}

void
WifiPhyStateHelper::LogPreviousIdleAndCcaBusyStates (void)
{
  Time now = Simulator::Now ();
  Time idleStart = Max (m_endCcaBusy, m_endSync);
  idleStart = Max (idleStart, m_endTx);
  NS_ASSERT (idleStart <= now);
  if (m_endCcaBusy > m_endSync && 
      m_endCcaBusy > m_endTx) {
    Time ccaBusyStart = Max (m_endTx, m_endSync);
    ccaBusyStart = Max (ccaBusyStart, m_startCcaBusy);
    m_stateLogger (ccaBusyStart, idleStart - ccaBusyStart, WifiPhy::CCA_BUSY);
  }
  m_stateLogger (idleStart, now - idleStart, WifiPhy::IDLE);
}

void
WifiPhyStateHelper::SwitchToTx (Time txDuration, Ptr<const Packet> packet, WifiMode txMode, 
			  WifiPreamble preamble, uint8_t txPower)
{
  m_txTrace (packet, txMode, preamble, txPower);
  NotifyTxStart (txDuration);
  Time now = Simulator::Now ();
  switch (GetState ()) {
  case WifiPhy::SYNC:
    /* The packet which is being received as well
     * as its endSync event are cancelled by the caller.
     */
    m_syncing = false;
    m_stateLogger (m_startSync, now - m_startSync, WifiPhy::SYNC);
    m_endSync = now;
    break;
  case WifiPhy::CCA_BUSY: {
    Time ccaStart = Max (m_endSync, m_endTx);
    ccaStart = Max (ccaStart, m_startCcaBusy);
    m_stateLogger (ccaStart, now - ccaStart, WifiPhy::CCA_BUSY);
  } break;
  case WifiPhy::IDLE:
    LogPreviousIdleAndCcaBusyStates ();
    break;
  default:
    NS_ASSERT (false);
    break;
  }
  m_stateLogger (now, txDuration, WifiPhy::TX);
  m_previousStateChangeTime = now;
  m_endTx = now + txDuration;
  m_startTx = now;
}
void
WifiPhyStateHelper::SwitchToSync (Time rxDuration)
{
  NS_ASSERT (IsStateIdle () || IsStateCcaBusy ());
  NS_ASSERT (!m_syncing);
  NotifySyncStart (rxDuration);
  Time now = Simulator::Now ();
  switch (GetState ()) {
  case WifiPhy::IDLE:
    LogPreviousIdleAndCcaBusyStates ();
    break;
  case WifiPhy::CCA_BUSY: {
    Time ccaStart = Max (m_endSync, m_endTx);
    ccaStart = Max (ccaStart, m_startCcaBusy);
    m_stateLogger (ccaStart, now - ccaStart, WifiPhy::CCA_BUSY);
  } break;
  case WifiPhy::SYNC:
  case WifiPhy::TX:
    NS_ASSERT (false);
    break;
  }
  m_previousStateChangeTime = now;
  m_syncing = true;
  m_startSync = now;
  m_endSync = now + rxDuration;
  NS_ASSERT (IsStateSync ());
}
void 
WifiPhyStateHelper::SwitchFromSyncEndOk (Ptr<Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  m_rxOkTrace (packet, snr, mode, preamble);
  NotifySyncEndOk ();
  DoSwitchFromSync ();
  if (!m_syncOkCallback.IsNull ())
    {
      m_syncOkCallback (packet, snr, mode, preamble);
    }

}
void 
WifiPhyStateHelper::SwitchFromSyncEndError (Ptr<const Packet> packet, double snr)
{
  m_rxErrorTrace (packet, snr);
  NotifySyncEndError ();
  DoSwitchFromSync ();
  if (!m_syncErrorCallback.IsNull ())
    {
      m_syncErrorCallback (packet, snr);
    }
}

void
WifiPhyStateHelper::DoSwitchFromSync (void)
{
  NS_ASSERT (IsStateSync ());
  NS_ASSERT (m_syncing);

  Time now = Simulator::Now ();
  m_stateLogger (m_startSync, now - m_startSync, WifiPhy::SYNC);
  m_previousStateChangeTime = now;
  m_syncing = false;

  NS_ASSERT (IsStateIdle () || IsStateCcaBusy ());
}
void
WifiPhyStateHelper::SwitchMaybeToCcaBusy (Time duration)
{
  NotifyCcaBusyStart (duration);
  Time now = Simulator::Now ();
  switch (GetState ()) {
  case WifiPhy::IDLE:
    LogPreviousIdleAndCcaBusyStates ();
  break;
  case WifiPhy::CCA_BUSY:
    break;
  case WifiPhy::SYNC:
    break;
  case WifiPhy::TX:
    break;
  }
  m_startCcaBusy = now;
  m_endCcaBusy = Max (m_endCcaBusy, now + duration);
}

} // namespace ns3
