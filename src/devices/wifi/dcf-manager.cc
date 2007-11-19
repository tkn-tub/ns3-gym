/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <math.h>

#include "dcf-manager.h"
#include "mac-parameters.h"

NS_LOG_COMPONENT_DEFINE ("DcfManager");

#define MY_DEBUG(x) \
  NS_LOG_DEBUG (Simulator::Now () << " " << this << " " << x)

namespace ns3 {

/****************************************************************
 *      Implement the DCF state holder
 ****************************************************************/

DcfState::DcfState ()
  : m_backoffSlots (0),
    m_backoffStart (Seconds (0.0))
{}

DcfState::~DcfState ()
{}

void 
DcfState::SetAifsn (uint32_t aifsn)
{
  m_aifsn = aifsn;
}

void 
DcfState::SetCwBounds (uint32_t minCw, uint32_t maxCw)
{
  m_cwMin = minCw;
  m_cwMax = maxCw;
  ResetCw ();
}

void 
DcfState::ResetCw (void)
{
  m_cw = m_cwMin;
}
void 
DcfState::UpdateFailedCw (void)
{
  uint32_t cw = m_cw;
  cw *= 2;
  cw = std::min (m_cwMax, cw);
  m_cw = cw;
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
DcfState::GetAifsn (void) const
{
  return m_aifsn;
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
    m_sifs (Seconds (0.0))
{}

void 
DcfManager::SetSlotTime (Time slotTime)
{
  m_slotTime = slotTime;
}
void 
DcfManager::SetSifs (Time sifs)
{
  m_sifs = sifs;
}

void 
DcfManager::SetAckTxDuration (Time ackTxDuration)
{
  m_ackTxTime = ackTxDuration;
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
      if (state->NeedsAccess () &&
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
              if (otherState->NeedsAccess () &&
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
      rxAccessStart = m_lastRxEnd + m_sifs;
      if (!m_lastRxReceivedOk)
        {
          /* to handle EIFS */
          rxAccessStart += m_ackTxTime;
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
          /**
           * For each DcfState, calculate how many backoff slots elapsed since
           * the last time its backoff counter was updated. If the number of 
           * slots is smaller than its AIFSN, the backoff did not start, so,
           * we do not update it.
           */
          if (nIntSlots >= state->GetAifsn ())
            {
              uint32_t n = std::min (nIntSlots, state->GetBackoffSlots ());
              MY_DEBUG ("dcf " << k << " dec backoff slots=" << n);
              Time backoffUpdateBound = backoffStart + Scalar (n) * m_slotTime;
              state->UpdateBackoffSlotsNow (n, backoffUpdateBound);
            }
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
  Time expectedBackoffEnd = MaxSeconds ();
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      DcfState *state = *i;
      if (state->NeedsAccess ())
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
DcfManager::NotifyCcaBusyStartNow (Time duration)
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
