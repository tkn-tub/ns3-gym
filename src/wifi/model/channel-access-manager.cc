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
#include "ns3/simulator.h"
#include "channel-access-manager.h"
#include "txop.h"
#include "wifi-phy-listener.h"
#include "wifi-phy.h"
#include "mac-low.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ChannelAccessManager");

/**
 * Listener for PHY events. Forwards to ChannelAccessManager
 */
class PhyListener : public ns3::WifiPhyListener
{
public:
  /**
   * Create a PhyListener for the given ChannelAccessManager.
   *
   * \param dcf
   */
  PhyListener (ns3::ChannelAccessManager *dcf)
    : m_dcf (dcf)
  {
  }
  virtual ~PhyListener ()
  {
  }
  void NotifyRxStart (Time duration)
  {
    m_dcf->NotifyRxStartNow (duration);
  }
  void NotifyRxEndOk (void)
  {
    m_dcf->NotifyRxEndOkNow ();
  }
  void NotifyRxEndError (void)
  {
    m_dcf->NotifyRxEndErrorNow ();
  }
  void NotifyTxStart (Time duration, double txPowerDbm)
  {
    m_dcf->NotifyTxStartNow (duration);
  }
  void NotifyMaybeCcaBusyStart (Time duration)
  {
    m_dcf->NotifyMaybeCcaBusyStartNow (duration);
  }
  void NotifySwitchingStart (Time duration)
  {
    m_dcf->NotifySwitchingStartNow (duration);
  }
  void NotifySleep (void)
  {
    m_dcf->NotifySleepNow ();
  }
  void NotifyOff (void)
  {
    m_dcf->NotifyOffNow ();
  }
  void NotifyWakeup (void)
  {
    m_dcf->NotifyWakeupNow ();
  }
  void NotifyOn (void)
  {
    m_dcf->NotifyOnNow ();
  }

private:
  ns3::ChannelAccessManager *m_dcf;  //!< ChannelAccessManager to forward events to
};


/****************************************************************
 *      Implement the DCF manager of all DCF state holders
 ****************************************************************/

ChannelAccessManager::ChannelAccessManager ()
  : m_lastAckTimeoutEnd (MicroSeconds (0)),
    m_lastCtsTimeoutEnd (MicroSeconds (0)),
    m_lastNavStart (MicroSeconds (0)),
    m_lastNavDuration (MicroSeconds (0)),
    m_lastRxStart (MicroSeconds (0)),
    m_lastRxDuration (MicroSeconds (0)),
    m_lastRxReceivedOk (true),
    m_lastRxEnd (MicroSeconds (0)),
    m_lastTxStart (MicroSeconds (0)),
    m_lastTxDuration (MicroSeconds (0)),
    m_lastBusyStart (MicroSeconds (0)),
    m_lastBusyDuration (MicroSeconds (0)),
    m_lastSwitchingStart (MicroSeconds (0)),
    m_lastSwitchingDuration (MicroSeconds (0)),
    m_rxing (false),
    m_sleeping (false),
    m_off (false),
    m_slot (Seconds (0.0)),
    m_sifs (Seconds (0.0)),
    m_phyListener (0)
{
  NS_LOG_FUNCTION (this);
}

ChannelAccessManager::~ChannelAccessManager ()
{
  NS_LOG_FUNCTION (this);
  delete m_phyListener;
  m_phyListener = 0;
}

void
ChannelAccessManager::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (Ptr<Txop> i : m_states)
    {
      i->Dispose ();
      i = 0;
    }
}

void
ChannelAccessManager::SetupPhyListener (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  NS_ASSERT (m_phyListener == 0);
  m_phyListener = new PhyListener (this);
  phy->RegisterListener (m_phyListener);
}

void
ChannelAccessManager::RemovePhyListener (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  if (m_phyListener != 0)
    {
      phy->UnregisterListener (m_phyListener);
      delete m_phyListener;
      m_phyListener = 0;
    }
}

void
ChannelAccessManager::SetupLow (Ptr<MacLow> low)
{
  NS_LOG_FUNCTION (this << low);
  low->RegisterDcf (this);
}

void
ChannelAccessManager::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_slot = slotTime;
}

void
ChannelAccessManager::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_sifs = sifs;
}

void
ChannelAccessManager::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}

Time
ChannelAccessManager::GetEifsNoDifs () const
{
  NS_LOG_FUNCTION (this);
  return m_eifsNoDifs;
}

void
ChannelAccessManager::Add (Ptr<Txop> dcf)
{
  NS_LOG_FUNCTION (this << dcf);
  m_states.push_back (dcf);
}

Time
ChannelAccessManager::MostRecent (Time a, Time b) const
{
  return Max (a, b);
}

Time
ChannelAccessManager::MostRecent (Time a, Time b, Time c, Time d, Time e, Time f) const
{
  Time g = MostRecent (a, b);
  Time h = MostRecent (c, d);
  Time i = MostRecent (e, f);
  Time k = MostRecent (g, h);
  Time retval = MostRecent (k, i);
  return retval;
}

Time
ChannelAccessManager::MostRecent (Time a, Time b, Time c, Time d, Time e, Time f, Time g) const
{
  Time h = MostRecent (a, b);
  Time i = MostRecent (c, d);
  Time j = MostRecent (e, f);
  Time k = MostRecent (h, i);
  Time l = MostRecent (j, g);
  Time retval = MostRecent (k, l);
  return retval;
}

bool
ChannelAccessManager::IsBusy (void) const
{
  NS_LOG_FUNCTION (this);
  // PHY busy
  if (m_rxing)
    {
      return true;
    }
  Time lastTxEnd = m_lastTxStart + m_lastTxDuration;
  if (lastTxEnd > Simulator::Now ())
    {
      return true;
    }
  // NAV busy
  Time lastNavEnd = m_lastNavStart + m_lastNavDuration;
  if (lastNavEnd > Simulator::Now ())
    {
      return true;
    }
  // CCA busy
  Time lastCCABusyEnd = m_lastBusyStart + m_lastBusyDuration;
  if (lastCCABusyEnd > Simulator::Now ())
    {
      return true;
    }
  return false;
}

bool
ChannelAccessManager::IsWithinAifs (Ptr<Txop> state) const
{
  NS_LOG_FUNCTION (this << state);
  Time ifsEnd = GetAccessGrantStart () + (state->GetAifsn () * m_slot);
  if (ifsEnd > Simulator::Now ())
    {
      NS_LOG_DEBUG ("IsWithinAifs () true; ifsEnd is at " << ifsEnd.GetSeconds ());
      return true;
    }
  NS_LOG_DEBUG ("IsWithinAifs () false; ifsEnd was at " << ifsEnd.GetSeconds ());
  return false;
}


void
ChannelAccessManager::RequestAccess (Ptr<Txop> state, bool isCfPeriod)
{
  NS_LOG_FUNCTION (this << state);
  //Deny access if in sleep mode or off
  if (m_sleeping || m_off)
    {
      return;
    }
  if (isCfPeriod)
    {
      state->NotifyAccessRequested ();
      Time delay = (MostRecent (GetAccessGrantStart (true), Simulator::Now ()) - Simulator::Now ());
      m_accessTimeout = Simulator::Schedule (delay, &ChannelAccessManager::DoGrantPcfAccess, this, state);
      return;
    }
  UpdateBackoff ();
  NS_ASSERT (!state->IsAccessRequested ());
  state->NotifyAccessRequested ();
  // If currently transmitting; end of transmission (ACK or no ACK) will cause
  // a later access request if needed from EndTxNoAck, GotAck, or MissedAck
  Time lastTxEnd = m_lastTxStart + m_lastTxDuration;
  if (lastTxEnd > Simulator::Now ())
    {
      NS_LOG_DEBUG ("Internal collision (currently transmitting)");
      state->NotifyInternalCollision ();
      DoRestartAccessTimeoutIfNeeded ();
      return;
    }
  /**
   * If there is a collision, generate a backoff
   * by notifying the collision to the user.
   */
  if (state->GetBackoffSlots () == 0)
    {
      if (IsBusy ())
        {
          NS_LOG_DEBUG ("medium is busy: collision");
          // someone else has accessed the medium; generate a backoff.
          state->NotifyCollision ();
          DoRestartAccessTimeoutIfNeeded ();
          return;
        }
      else if (IsWithinAifs (state))
        {
          NS_LOG_DEBUG ("busy within AIFS");
          state->NotifyCollision ();
          DoRestartAccessTimeoutIfNeeded ();
          return;
        }
    }
  DoGrantDcfAccess ();
  DoRestartAccessTimeoutIfNeeded ();
}

void
ChannelAccessManager::DoGrantPcfAccess (Ptr<Txop> state)
{
  state->NotifyAccessGranted ();
}

void
ChannelAccessManager::DoGrantDcfAccess (void)
{
  NS_LOG_FUNCTION (this);
  uint32_t k = 0;
  for (States::iterator i = m_states.begin (); i != m_states.end (); k++)
    {
      Ptr<Txop> state = *i;
      if (state->IsAccessRequested ()
          && GetBackoffEndFor (state) <= Simulator::Now () )
        {
          /**
           * This is the first dcf we find with an expired backoff and which
           * needs access to the medium. i.e., it has data to send.
           */
          NS_LOG_DEBUG ("dcf " << k << " needs access. backoff expired. access granted. slots=" << state->GetBackoffSlots ());
          i++; //go to the next item in the list.
          k++;
          std::vector<Ptr<Txop> > internalCollisionStates;
          for (States::iterator j = i; j != m_states.end (); j++, k++)
            {
              Ptr<Txop> otherState = *j;
              if (otherState->IsAccessRequested ()
                  && GetBackoffEndFor (otherState) <= Simulator::Now ())
                {
                  NS_LOG_DEBUG ("dcf " << k << " needs access. backoff expired. internal collision. slots=" <<
                                otherState->GetBackoffSlots ());
                  /**
                   * all other dcfs with a lower priority whose backoff
                   * has expired and which needed access to the medium
                   * must be notified that we did get an internal collision.
                   */
                  internalCollisionStates.push_back (otherState);
                }
            }

          /**
           * Now, we notify all of these changes in one go. It is necessary to
           * perform first the calculations of which states are colliding and then
           * only apply the changes because applying the changes through notification
           * could change the global state of the manager, and, thus, could change
           * the result of the calculations.
           */
          state->NotifyAccessGranted ();
          for (std::vector<Ptr<Txop> >::iterator l = internalCollisionStates.begin ();
               l != internalCollisionStates.end (); l++)
            {
              (*l)->NotifyInternalCollision ();
            }
          break;
        }
      i++;
    }
}

void
ChannelAccessManager::AccessTimeout (void)
{
  NS_LOG_FUNCTION (this);
  UpdateBackoff ();
  DoGrantDcfAccess ();
  DoRestartAccessTimeoutIfNeeded ();
}

Time
ChannelAccessManager::GetAccessGrantStart (bool ignoreNav) const
{
  NS_LOG_FUNCTION (this);
  Time rxAccessStart;
  if (!m_rxing)
    {
      rxAccessStart = m_lastRxEnd + m_sifs;
      if (!m_lastRxReceivedOk)
        {
          rxAccessStart += m_eifsNoDifs;
        }
    }
  else
    {
      rxAccessStart = m_lastRxStart + m_lastRxDuration + m_sifs;
    }
  Time busyAccessStart = m_lastBusyStart + m_lastBusyDuration + m_sifs;
  Time txAccessStart = m_lastTxStart + m_lastTxDuration + m_sifs;
  Time navAccessStart = m_lastNavStart + m_lastNavDuration + m_sifs;
  Time ackTimeoutAccessStart = m_lastAckTimeoutEnd + m_sifs;
  Time ctsTimeoutAccessStart = m_lastCtsTimeoutEnd + m_sifs;
  Time switchingAccessStart = m_lastSwitchingStart + m_lastSwitchingDuration + m_sifs;
  Time accessGrantedStart;
  if (ignoreNav)
    {
      accessGrantedStart = MostRecent (rxAccessStart,
                                       busyAccessStart,
                                       txAccessStart,
                                       ackTimeoutAccessStart,
                                       ctsTimeoutAccessStart,
                                       switchingAccessStart
                                       );
    }
  else
    {
      accessGrantedStart = MostRecent (rxAccessStart,
                                       busyAccessStart,
                                       txAccessStart,
                                       navAccessStart,
                                       ackTimeoutAccessStart,
                                       ctsTimeoutAccessStart,
                                       switchingAccessStart
                                       );
    }
  NS_LOG_INFO ("access grant start=" << accessGrantedStart <<
               ", rx access start=" << rxAccessStart <<
               ", busy access start=" << busyAccessStart <<
               ", tx access start=" << txAccessStart <<
               ", nav access start=" << navAccessStart);
  return accessGrantedStart;
}

Time
ChannelAccessManager::GetBackoffStartFor (Ptr<Txop> state)
{
  NS_LOG_FUNCTION (this << state);
  Time mostRecentEvent = MostRecent (state->GetBackoffStart (),
                                     GetAccessGrantStart () + (state->GetAifsn () * m_slot));

  return mostRecentEvent;
}

Time
ChannelAccessManager::GetBackoffEndFor (Ptr<Txop> state)
{
  NS_LOG_FUNCTION (this << state);
  NS_LOG_DEBUG ("Backoff start: " << GetBackoffStartFor (state).As (Time::US) <<
                " end: " << (GetBackoffStartFor (state) + state->GetBackoffSlots () * m_slot).As (Time::US));
  return GetBackoffStartFor (state) + (state->GetBackoffSlots () * m_slot);
}

void
ChannelAccessManager::UpdateBackoff (void)
{
  NS_LOG_FUNCTION (this);
  uint32_t k = 0;
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++, k++)
    {
      Ptr<Txop> state = *i;

      Time backoffStart = GetBackoffStartFor (state);
      if (backoffStart <= Simulator::Now ())
        {
          uint32_t nIntSlots = ((Simulator::Now () - backoffStart) / m_slot).GetHigh ();
          /*
           * EDCA behaves slightly different to DCA. For EDCA we
           * decrement once at the slot boundary at the end of AIFS as
           * well as once at the end of each clear slot
           * thereafter. For DCA we only decrement at the end of each
           * clear slot after DIFS. We account for the extra backoff
           * by incrementing the slot count here in the case of
           * EDCA. The if statement whose body we are in has confirmed
           * that a minimum of AIFS has elapsed since last busy
           * medium.
           */
          if (state->IsQosTxop ())
            {
              nIntSlots++;
            }
          uint32_t n = std::min (nIntSlots, state->GetBackoffSlots ());
          NS_LOG_DEBUG ("dcf " << k << " dec backoff slots=" << n);
          Time backoffUpdateBound = backoffStart + (n * m_slot);
          state->UpdateBackoffSlotsNow (n, backoffUpdateBound);
        }
    }
}

void
ChannelAccessManager::DoRestartAccessTimeoutIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  /**
   * Is there a Txop which needs to access the medium, and,
   * if there is one, how many slots for AIFS+backoff does it require ?
   */
  bool accessTimeoutNeeded = false;
  Time expectedBackoffEnd = Simulator::GetMaximumSimulationTime ();
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      Ptr<Txop> state = *i;
      if (state->IsAccessRequested ())
        {
          Time tmp = GetBackoffEndFor (state);
          if (tmp > Simulator::Now ())
            {
              accessTimeoutNeeded = true;
              expectedBackoffEnd = std::min (expectedBackoffEnd, tmp);
            }
        }
    }
  NS_LOG_DEBUG ("Access timeout needed: " << accessTimeoutNeeded);
  if (accessTimeoutNeeded)
    {
      NS_LOG_DEBUG ("expected backoff end=" << expectedBackoffEnd);
      Time expectedBackoffDelay = expectedBackoffEnd - Simulator::Now ();
      if (m_accessTimeout.IsRunning ()
          && Simulator::GetDelayLeft (m_accessTimeout) > expectedBackoffDelay)
        {
          m_accessTimeout.Cancel ();
        }
      if (m_accessTimeout.IsExpired ())
        {
          m_accessTimeout = Simulator::Schedule (expectedBackoffDelay,
                                                 &ChannelAccessManager::AccessTimeout, this);
        }
    }
}

void
ChannelAccessManager::NotifyRxStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  NS_LOG_DEBUG ("rx start for=" << duration);
  UpdateBackoff ();
  m_lastRxStart = Simulator::Now ();
  m_lastRxDuration = duration;
  m_rxing = true;
}

void
ChannelAccessManager::NotifyRxEndOkNow (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("rx end ok");
  m_lastRxEnd = Simulator::Now ();
  m_lastRxReceivedOk = true;
  m_rxing = false;
}

void
ChannelAccessManager::NotifyRxEndErrorNow (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("rx end error");
  m_lastRxEnd = Simulator::Now ();
  m_lastRxReceivedOk = false;
  m_rxing = false;
}

void
ChannelAccessManager::NotifyTxStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  if (m_rxing)
    {
      //this may be caused only if PHY has started to receive a packet
      //inside SIFS, so, we check that lastRxStart was maximum a SIFS ago
      NS_ASSERT (Simulator::Now () - m_lastRxStart <= m_sifs);
      m_lastRxEnd = Simulator::Now ();
      m_lastRxDuration = m_lastRxEnd - m_lastRxStart;
      m_lastRxReceivedOk = true;
      m_rxing = false;
    }
  NS_LOG_DEBUG ("tx start for " << duration);
  UpdateBackoff ();
  m_lastTxStart = Simulator::Now ();
  m_lastTxDuration = duration;
}

void
ChannelAccessManager::NotifyMaybeCcaBusyStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  NS_LOG_DEBUG ("busy start for " << duration);
  UpdateBackoff ();
  m_lastBusyStart = Simulator::Now ();
  m_lastBusyDuration = duration;
}

void
ChannelAccessManager::NotifySwitchingStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  Time now = Simulator::Now ();
  NS_ASSERT (m_lastTxStart + m_lastTxDuration <= now);
  NS_ASSERT (m_lastSwitchingStart + m_lastSwitchingDuration <= now);

  if (m_rxing)
    {
      //channel switching during packet reception
      m_lastRxEnd = Simulator::Now ();
      m_lastRxDuration = m_lastRxEnd - m_lastRxStart;
      m_lastRxReceivedOk = true;
      m_rxing = false;
    }
  if (m_lastNavStart + m_lastNavDuration > now)
    {
      m_lastNavDuration = now - m_lastNavStart;
    }
  if (m_lastBusyStart + m_lastBusyDuration > now)
    {
      m_lastBusyDuration = now - m_lastBusyStart;
    }
  if (m_lastAckTimeoutEnd > now)
    {
      m_lastAckTimeoutEnd = now;
    }
  if (m_lastCtsTimeoutEnd > now)
    {
      m_lastCtsTimeoutEnd = now;
    }

  //Cancel timeout
  if (m_accessTimeout.IsRunning ())
    {
      m_accessTimeout.Cancel ();
    }

  //Reset backoffs
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      Ptr<Txop> state = *i;
      uint32_t remainingSlots = state->GetBackoffSlots ();
      if (remainingSlots > 0)
        {
          state->UpdateBackoffSlotsNow (remainingSlots, now);
          NS_ASSERT (state->GetBackoffSlots () == 0);
        }
      state->ResetCw ();
      state->m_accessRequested = false;
      state->NotifyChannelSwitching ();
    }

  NS_LOG_DEBUG ("switching start for " << duration);
  m_lastSwitchingStart = Simulator::Now ();
  m_lastSwitchingDuration = duration;

}

void
ChannelAccessManager::NotifySleepNow (void)
{
  NS_LOG_FUNCTION (this);
  m_sleeping = true;
  //Cancel timeout
  if (m_accessTimeout.IsRunning ())
    {
      m_accessTimeout.Cancel ();
    }

  //Reset backoffs
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      Ptr<Txop> state = *i;
      state->NotifySleep ();
    }
}

void
ChannelAccessManager::NotifyOffNow (void)
{
  NS_LOG_FUNCTION (this);
  m_off = true;
  //Cancel timeout
  if (m_accessTimeout.IsRunning ())
    {
      m_accessTimeout.Cancel ();
    }

  //Reset backoffs
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      Ptr<Txop> state = *i;
      state->NotifyOff ();
    }
}

void
ChannelAccessManager::NotifyWakeupNow (void)
{
  NS_LOG_FUNCTION (this);
  m_sleeping = false;
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      Ptr<Txop> state = *i;
      uint32_t remainingSlots = state->GetBackoffSlots ();
      if (remainingSlots > 0)
        {
          state->UpdateBackoffSlotsNow (remainingSlots, Simulator::Now ());
          NS_ASSERT (state->GetBackoffSlots () == 0);
        }
      state->ResetCw ();
      state->m_accessRequested = false;
      state->NotifyWakeUp ();
    }
}

void
ChannelAccessManager::NotifyOnNow (void)
{
  NS_LOG_FUNCTION (this);
  m_off = false;
  for (States::iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      Ptr<Txop> state = *i;
      uint32_t remainingSlots = state->GetBackoffSlots ();
      if (remainingSlots > 0)
        {
          state->UpdateBackoffSlotsNow (remainingSlots, Simulator::Now ());
          NS_ASSERT (state->GetBackoffSlots () == 0);
        }
      state->ResetCw ();
      state->m_accessRequested = false;
      state->NotifyOn ();
    }
}

void
ChannelAccessManager::NotifyNavResetNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  NS_LOG_DEBUG ("nav reset for=" << duration);
  UpdateBackoff ();
  m_lastNavStart = Simulator::Now ();
  m_lastNavDuration = duration;
  /**
   * If the nav reset indicates an end-of-nav which is earlier
   * than the previous end-of-nav, the expected end of backoff
   * might be later than previously thought so, we might need
   * to restart a new access timeout.
   */
  DoRestartAccessTimeoutIfNeeded ();
}

void
ChannelAccessManager::NotifyNavStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  NS_ASSERT (m_lastNavStart <= Simulator::Now ());
  NS_LOG_DEBUG ("nav start for=" << duration);
  UpdateBackoff ();
  Time newNavEnd = Simulator::Now () + duration;
  Time lastNavEnd = m_lastNavStart + m_lastNavDuration;
  if (newNavEnd > lastNavEnd)
    {
      m_lastNavStart = Simulator::Now ();
      m_lastNavDuration = duration;
    }
}

void
ChannelAccessManager::NotifyAckTimeoutStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  NS_ASSERT (m_lastAckTimeoutEnd < Simulator::Now ());
  m_lastAckTimeoutEnd = Simulator::Now () + duration;
}

void
ChannelAccessManager::NotifyAckTimeoutResetNow (void)
{
  NS_LOG_FUNCTION (this);
  m_lastAckTimeoutEnd = Simulator::Now ();
  DoRestartAccessTimeoutIfNeeded ();
}

void
ChannelAccessManager::NotifyCtsTimeoutStartNow (Time duration)
{
  NS_LOG_FUNCTION (this << duration);
  m_lastCtsTimeoutEnd = Simulator::Now () + duration;
}

void
ChannelAccessManager::NotifyCtsTimeoutResetNow (void)
{
  NS_LOG_FUNCTION (this);
  m_lastCtsTimeoutEnd = Simulator::Now ();
  DoRestartAccessTimeoutIfNeeded ();
}

} //namespace ns3
