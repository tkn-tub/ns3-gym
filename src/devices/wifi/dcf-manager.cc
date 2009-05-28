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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <math.h>

#include "dcf-manager.h"
#include "wifi-phy.h"
#include "wifi-mac.h"
#include "mac-low.h"

NS_LOG_COMPONENT_DEFINE ("DcfManager");

#define MY_DEBUG(x) \
  NS_LOG_DEBUG (Simulator::Now () << " " << this << " " << x)

namespace ns3 {

/****************************************************************
 *      Implement the DCF state holder
 ****************************************************************/

DcfState::DcfState ()
  : m_backoffSlots (0),
    m_backoffStart (Seconds (0.0)),
    m_cwMin (0),
    m_cwMax (0),
    m_cw (0),
    m_accessRequested (false)
{}

DcfState::~DcfState ()
{}

void 
DcfState::SetAifsn (uint32_t aifsn)
{
  m_aifsn = aifsn;
}
void 
DcfState::SetCwMin (uint32_t minCw)
{
  m_cwMin = minCw;
  ResetCw ();
}
void 
DcfState::SetCwMax (uint32_t maxCw)
{
  m_cwMax = maxCw;
  ResetCw ();
}
uint32_t 
DcfState::GetAifsn (void) const
{
  return m_aifsn;
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
  m_cw = m_cwMin;
}
void 
DcfState::UpdateFailedCw (void)
{
  // see 802.11-2007, section 9.9.1.5
  m_cw = std::min ( 2 * (m_cw + 1) - 1, m_cwMax);
}
void 
DcfState::UpdateBackoffSlotsNow (uint32_t nSlots, Time backoffUpdateBound)
{
  m_backoffSlots -= nSlots;
  m_backoffStart = backoffUpdateBound;
  MY_DEBUG ("update slots="<<nSlots<<" slots, backoff="<<m_backoffSlots);
}

void 
DcfState::StartBackoffNow (uint32_t nSlots)
{
  NS_ASSERT (m_backoffSlots == 0);
  MY_DEBUG ("start backoff="<<nSlots<<" slots");
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
  m_accessRequested = true;
}
void 
DcfState::NotifyAccessGranted (void)
{
  NS_ASSERT (m_accessRequested);
  m_accessRequested = false;
  DoNotifyAccessGranted ();
}
void 
DcfState::NotifyCollision (void)
{
  DoNotifyCollision ();
}
void 
DcfState::NotifyInternalCollision (void)
{
  DoNotifyInternalCollision ();
}


/***************************************************************
 *         Listener for Nav events. Forwards to DcfManager
 ***************************************************************/

class LowNavListener : public ns3::MacLowNavListener {
public:
  LowNavListener (ns3::DcfManager *dcf)
    : m_dcf (dcf) {}
  virtual ~LowNavListener () {}
  virtual void NavStart (Time duration) {
    m_dcf->NotifyNavStartNow (duration);
  }
  virtual void NavReset (Time duration) {
    m_dcf->NotifyNavResetNow (duration);
  }
private:
  ns3::DcfManager *m_dcf;
};

/***************************************************************
 *         Listener for PHY events. Forwards to DcfManager
 ***************************************************************/

class PhyListener : public ns3::WifiPhyListener {
public:
  PhyListener (ns3::DcfManager *dcf)
    : m_dcf (dcf) {}
  virtual ~PhyListener () {}
  virtual void NotifyRxStart (Time duration) {
    m_dcf->NotifyRxStartNow (duration);
  }
  virtual void NotifyRxEndOk (void) {
    m_dcf->NotifyRxEndOkNow ();
  }
  virtual void NotifyRxEndError (void) {
    m_dcf->NotifyRxEndErrorNow ();
  }
  virtual void NotifyTxStart (Time duration) {
    m_dcf->NotifyTxStartNow (duration);
  }
  virtual void NotifyMaybeCcaBusyStart (Time duration) {
    m_dcf->NotifyMaybeCcaBusyStartNow (duration);
  }
private:
  ns3::DcfManager *m_dcf;
};

/****************************************************************
 *      Implement the DCF manager of all DCF state holders
 ****************************************************************/

DcfManager::DcfManager ()
  : m_lastNavStart (MicroSeconds (0)),
    m_lastNavDuration (MicroSeconds (0)),
    m_lastRxStart (MicroSeconds (0)),
    m_lastRxDuration (MicroSeconds (0)),
    m_lastRxReceivedOk (true),
    m_lastRxEnd (MicroSeconds (0)),
    m_lastTxStart (MicroSeconds (0)),
    m_lastTxDuration (MicroSeconds (0)),
    m_lastBusyStart (MicroSeconds (0)),
    m_lastBusyDuration (MicroSeconds (0)),
    m_rxing (false),
    m_slotTime (Seconds (0.0)),
    m_sifs (Seconds (0.0)),
    m_phyListener (0),
    m_lowListener (0)
{}

DcfManager::~DcfManager ()
{
  delete m_phyListener;
  delete m_lowListener;
  m_phyListener = 0;
  m_lowListener = 0;
}

void 
DcfManager::SetupPhyListener (Ptr<WifiPhy> phy)
{
  m_phyListener = new PhyListener (this);
  phy->RegisterListener (m_phyListener);
}
void 
DcfManager::SetupLowListener (Ptr<MacLow> low)
{
  m_lowListener = new LowNavListener (this);
  low->RegisterNavListener (m_lowListener);
}

void 
DcfManager::SetSlot (Time slotTime)
{
  m_slotTime = slotTime;
}
void 
DcfManager::SetSifs (Time sifs)
{
  m_sifs = sifs;
}
void 
DcfManager::SetEifsNoDifs (Time eifsNoDifs)
{
  m_eifsNoDifs = eifsNoDifs;
}
Time
DcfManager::GetEifsNoDifs () const
{
  return m_eifsNoDifs;
}

void 
DcfManager::Add (DcfState *dcf)
{
  m_states.push_back (dcf);
}

Time
DcfManager::MostRecent (Time a, Time b) const
{
  return Max (a, b);
}
Time
DcfManager::MostRecent (Time a, Time b, Time c) const
{
  Time retval;
  retval = Max (a, b);
  retval = Max (retval, c);
  return retval;
}
Time
DcfManager::MostRecent (Time a, Time b, Time c, Time d) const
{
  Time e = Max (a, b);
  Time f = Max (c, d);
  Time retval = Max (e, f);
  return retval;
}

bool 
DcfManager::IsBusy (void) const
{
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
  return false;
}


void 
DcfManager::RequestAccess (DcfState *state)
{
  UpdateBackoff ();
  NS_ASSERT (!state->IsAccessRequested ());
  state->NotifyAccessRequested ();
  /**
   * If there is a collision, generate a backoff
   * by notifying the collision to the user.
   */
  if (state->GetBackoffSlots () == 0 && 
      IsBusy ())
    {
      MY_DEBUG ("medium is busy: collision");
      /* someone else has accessed the medium.
       * generate a backoff.
       */
      state->NotifyCollision ();
    }
  DoGrantAccess ();
  DoRestartAccessTimeoutIfNeeded ();  
}

void
DcfManager::DoGrantAccess (void)
{
  Time accessGrantStart = GetAccessGrantStart ();

  uint32_t k = 0;
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); k++)
    {
      DcfState *state = *i;
      if (state->IsAccessRequested () && 
          GetBackoffEndFor (state) <= Simulator::Now ())
        {
          /**
           * This is the first dcf we find with an expired backoff and which
           * needs access to the medium. i.e., it has data to send.
           */
          MY_DEBUG ("dcf " << k << " needs access. backoff expired. access granted. slots="<<state->GetBackoffSlots ());
          i++; // go to the next item in the list.
          k++;
          std::vector<DcfState *> internalCollisionStates;
          for (States::const_iterator j = i; j != m_states.end (); j++, k++)
            {
              DcfState *otherState = *j;
              if (otherState->IsAccessRequested () &&
                  GetBackoffEndFor (otherState) <= Simulator::Now ())
                {
                  MY_DEBUG ("dcf " << k << " needs access. backoff expired. internal collision. slots=" << 
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
          for (std::vector<DcfState *>::const_iterator k = internalCollisionStates.begin ();
               k != internalCollisionStates.end (); k++)
            {
              (*k)->NotifyInternalCollision ();
            }
          break;
        }
      i++;
    }
}

void
DcfManager::AccessTimeout (void)
{
  UpdateBackoff ();
  DoGrantAccess ();
  DoRestartAccessTimeoutIfNeeded ();
}

Time
DcfManager::GetAccessGrantStart (void) const
{
  Time rxAccessStart;
  if (m_lastRxEnd >= m_lastRxStart) 
    {
      rxAccessStart = m_lastRxEnd;
      if (!m_lastRxReceivedOk)
        {
          rxAccessStart += m_eifsNoDifs;
        }
      else
        {
          rxAccessStart += m_sifs;
        }
    } 
  else 
    {
      rxAccessStart = m_lastRxStart + m_lastRxDuration + m_sifs;
    }
  Time busyAccessStart = m_lastBusyStart + m_lastBusyDuration + m_sifs;
  Time txAccessStart = m_lastTxStart + m_lastTxDuration + m_sifs;
  Time navAccessStart = m_lastNavStart + m_lastNavDuration + m_sifs;
  Time accessGrantedStart = MostRecent (rxAccessStart, 
                                        busyAccessStart,
                                        txAccessStart, 
                                        navAccessStart);
  NS_LOG_INFO ("access grant start=" << accessGrantedStart <<
               ", rx access start=" << rxAccessStart <<
               ", busy access start=" << busyAccessStart <<
               ", tx access start=" << txAccessStart <<
               ", nav access start=" << navAccessStart);
  return accessGrantedStart;
}

Time
DcfManager::GetBackoffStartFor (DcfState *state)
{
  Time mostRecentEvent = MostRecent (state->GetBackoffStart (),
                                     GetAccessGrantStart () + Scalar (state->GetAifsn ()) * m_slotTime);
  
  return mostRecentEvent;
}

Time
DcfManager::GetBackoffEndFor (DcfState *state)
{
  return GetBackoffStartFor (state) + Scalar (state->GetBackoffSlots ()) * m_slotTime;
}

void
DcfManager::UpdateBackoff (void)
{
  uint32_t k = 0;
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); i++, k++)
    {
      DcfState *state = *i;

      Time backoffStart = GetBackoffStartFor (state);
      if (backoffStart <= Simulator::Now ())
        {
          Scalar nSlots = (Simulator::Now () - backoffStart) / m_slotTime;
          uint32_t nIntSlots = lrint (nSlots.GetDouble ());          
          uint32_t n = std::min (nIntSlots, state->GetBackoffSlots ());
          MY_DEBUG ("dcf " << k << " dec backoff slots=" << n);
          Time backoffUpdateBound = backoffStart + Scalar (n) * m_slotTime;
          state->UpdateBackoffSlotsNow (n, backoffUpdateBound);
        }
    }
}

void
DcfManager::DoRestartAccessTimeoutIfNeeded (void)
{
  /**
   * Is there a DcfState which needs to access the medium, and, 
   * if there is one, how many slots for AIFS+backoff does it require ?
   */
  bool accessTimeoutNeeded = false;
  Time expectedBackoffEnd = Simulator::GetMaximumSimulationTime ();
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      DcfState *state = *i;
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
  if (accessTimeoutNeeded)
    {
      MY_DEBUG ("expected backoff end="<<expectedBackoffEnd);
      Time expectedBackoffDelay = expectedBackoffEnd - Simulator::Now ();
      if (m_accessTimeout.IsRunning () &&
          Simulator::GetDelayLeft (m_accessTimeout) > expectedBackoffDelay)
        {
          m_accessTimeout.Cancel ();
        }
      if (m_accessTimeout.IsExpired ())
        {
          m_accessTimeout = Simulator::Schedule (expectedBackoffDelay,
                                                 &DcfManager::AccessTimeout, this);
        }
    }
}

void 
DcfManager::NotifyRxStartNow (Time duration)
{
  MY_DEBUG ("rx start for="<<duration);
  UpdateBackoff ();
  m_lastRxStart = Simulator::Now ();
  m_lastRxDuration = duration;
  m_rxing = true;
}
void 
DcfManager::NotifyRxEndOkNow (void)
{
  MY_DEBUG ("rx end ok");
  m_lastRxEnd = Simulator::Now ();
  m_lastRxReceivedOk = true;
  m_rxing = false;
}
void 
DcfManager::NotifyRxEndErrorNow (void)
{
  MY_DEBUG ("rx end error");
  m_lastRxEnd = Simulator::Now ();
  m_lastRxReceivedOk = false;
  m_rxing = false;
}
void 
DcfManager::NotifyTxStartNow (Time duration)
{
  MY_DEBUG ("tx start for "<<duration);
  UpdateBackoff ();
  m_lastTxStart = Simulator::Now ();
  m_lastTxDuration = duration;
}
void 
DcfManager::NotifyMaybeCcaBusyStartNow (Time duration)
{
  MY_DEBUG ("busy start for "<<duration);
  UpdateBackoff ();
  m_lastBusyStart = Simulator::Now ();
  m_lastBusyDuration = duration;
}
void 
DcfManager::NotifyNavResetNow (Time duration)
{
  MY_DEBUG ("nav reset for="<<duration);
  UpdateBackoff ();
  m_lastNavStart = Simulator::Now ();
  m_lastNavDuration = duration;
  UpdateBackoff ();
  /**
   * If the nav reset indicates an end-of-nav which is earlier
   * than the previous end-of-nav, the expected end of backoff
   * might be later than previously thought so, we might need
   * to restart a new access timeout.
   */
  DoRestartAccessTimeoutIfNeeded ();
}
void 
DcfManager::NotifyNavStartNow (Time duration)
{
  NS_ASSERT (m_lastNavStart < Simulator::Now ());
  MY_DEBUG ("nav start for="<<duration);
  UpdateBackoff ();
  Time newNavEnd = Simulator::Now () + duration;
  Time lastNavEnd = m_lastNavStart + m_lastNavDuration;
  if (newNavEnd > lastNavEnd)
    {
      m_lastNavStart = Simulator::Now ();
      m_lastNavDuration = duration;
    }
}

} // namespace ns3
