/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <math.h>

#include "dcf-manager.h"
#include "mac-parameters.h"

NS_LOG_COMPONENT_DEFINE ("DcfManager");

namespace ns3 {

/****************************************************************
 *      Implement the DCF state holder
 ****************************************************************/

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
DcfState::UpdateBackoffSlotsNow (uint32_t nSlots)
{
  uint32_t n = std::min (nSlots, m_backoffSlots);
  m_backoffSlots -= n;
}

void 
DcfState::StartBackoffNow (uint32_t nSlots)
{
  NS_ASSERT (m_backoffSlots == 0);
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

void 
DcfManager::SetParameters (const MacParameters *parameters)
{
  m_parameters = parameters;
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
  if (m_accessTimeout.IsRunning ())
    {
      /* we don't need to do anything because we have an access
       * timer which will expire soon.
       */
      NS_LOG_DEBUG ("access timer running. will be notified");
      return;
    }
  /**
   * Since no access timeout is running, and if we have no
   * backoff running for this DcfState, start a new backoff
   * if needed.
   */ 
  if (state->GetBackoffSlots () == 0 && 
      IsBusy ())
    {
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
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); )
    {
      DcfState *state = *i;
      if (state->GetBackoffSlots () == 0 && state->NeedsAccess ())
        {
          /**
           * This is the first dcf we find with an expired backoff and which
           * needs access to the medium. i.e., it has data to send.
           */
          state->NotifyAccessGranted ();
          i++; // go to the next item in the list.
          for (States::const_iterator j = i; j != m_states.end (); j++)
            {
              DcfState *state = *j;
              if (state->GetBackoffSlots () == 0 && state->NeedsAccess ())
                {
                  /**
                   * all other dcfs with a lower priority whose backoff
                   * has expired and which needed access to the medium
                   * must be notified that we did get an internal collision.
                   */
                  state->NotifyInternalCollision ();
                }
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
      rxAccessStart = m_lastRxEnd + m_parameters->GetSifs ();
      if (!m_lastRxReceivedOk) 
        {
          rxAccessStart += m_ackTxTime;
        } 
    } 
  else 
    {
      rxAccessStart = m_lastRxStart + m_lastRxDuration + m_parameters->GetSifs ();
    }
  Time busyAccessStart = m_lastBusyStart + m_lastBusyDuration + m_parameters->GetSifs ();
  Time txAccessStart = m_lastTxStart + m_lastTxDuration + m_parameters->GetSifs ();
  Time navAccessStart = m_lastNavStart + m_lastNavDuration + m_parameters->GetSifs ();
  Time accessGrantedStart = MostRecent (rxAccessStart, 
                                        busyAccessStart,
                                        txAccessStart, 
                                        navAccessStart);
  NS_LOG_DEBUG ("access grant start=" << accessGrantedStart);
  return accessGrantedStart;
}

void
DcfManager::UpdateBackoff (void)
{
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      DcfState *state = *i;

      Time mostRecentEvent = MostRecent (state->GetBackoffStart (),
                                         GetAccessGrantStart ());
      if (mostRecentEvent < Simulator::Now ())
        {
          Scalar nSlots = (Simulator::Now () - mostRecentEvent) / m_parameters->GetSlotTime ();
          uint32_t nIntSlots = lrint (nSlots.GetDouble ());
          /**
           * For each DcfState, calculate how many backoff slots elapsed since
           * the last time its backoff counter was updated. If the number of 
           * slots is smaller than its AIFSN, the backoff did not start, so,
           * we do not update it.
           */
          if (nIntSlots > state->GetAifsn ())
            {
              state->UpdateBackoffSlotsNow (nIntSlots - state->GetAifsn ());
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
  uint32_t minNSlots = 0xffffffff;
  Time backoffStart;
  for (States::const_iterator i = m_states.begin (); i != m_states.end (); i++)
    {
      DcfState *state = *i;
      if (state->NeedsAccess ())
        {
          accessTimeoutNeeded = true;
          minNSlots = std::min (state->GetAifsn () + state->GetBackoffSlots (), minNSlots);
        }
    }
  if (accessTimeoutNeeded)
    {
      /**
       * If one of the DcfState needs access to the medium, calculate when its
       * backoff is expected to end.
       */
      Time expectedBackoffEnd = GetAccessGrantStart () + Scalar (minNSlots) * m_parameters->GetSlotTime ();
      /**
       * It is not possible that the backoff was expected to end before now
       * because if it were possible, this would mean that we have missed
       * a backoff expiration ! And that would be a bug.
       */
      NS_ASSERT (expectedBackoffEnd >= Simulator::Now ());
      if (expectedBackoffEnd > Simulator::Now ())
        {
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
}

void 
DcfManager::NotifyRxStartNow (Time duration)
{
  Time now = Simulator::Now ();
  NS_LOG_DEBUG ("rx start at="<<now<<", for="<<duration);
  UpdateBackoff ();
  m_lastRxStart = now;
  m_lastRxDuration = duration;
  m_rxing = true;
}
void 
DcfManager::NotifyRxEndOkNow (void)
{
  Time now = Simulator::Now ();
  NS_LOG_DEBUG ("rx end ok at="<<now);
  m_lastRxEnd = now;
  m_lastRxReceivedOk = true;
  m_rxing = false;
}
void 
DcfManager::NotifyRxEndErrorNow (void)
{
  Time now = Simulator::Now ();
  NS_LOG_DEBUG ("rx end error at=");
  m_lastRxEnd = now;
  m_lastRxReceivedOk = false;
  m_rxing = false;
}
void 
DcfManager::NotifyTxStartNow (Time duration)
{
  Time now = Simulator::Now ();
  NS_LOG_DEBUG ("tx start at="<<now<<" for "<<duration);
  UpdateBackoff ();
  m_lastTxStart = now;
  m_lastTxDuration = duration;
}
void 
DcfManager::NotifyCcaBusyStartNow (Time duration)
{
  Time now = Simulator::Now ();
  NS_LOG_DEBUG ("busy start at="<<now<<" for "<<duration);
  UpdateBackoff ();
  m_lastBusyStart = now;
  m_lastBusyDuration = duration;
}
void 
DcfManager::NotifyNavResetNow (Time duration)
{
  Time now = Simulator::Now ();
  NS_LOG_DEBUG ("nav reset at="<<now<<", for="<<duration);
  UpdateBackoff ();
  m_lastNavStart = now;
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
  Time now = Simulator::Now ();
  NS_ASSERT (m_lastNavStart < now);
  NS_LOG_DEBUG ("nav start at="<<now<<", for="<<duration);
  UpdateBackoff ();
  // XXX handle 
  m_lastNavStart = now;
  m_lastNavDuration = duration;
}

} // namespace ns3
