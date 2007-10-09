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

#ifndef DCF_H
#define DCF_H

#include <stdint.h>
#include "ns3/event-id.h"
#include "ns3/uv-trace-source.h"
#include "ns3/callback-trace-source.h"
#include "ns3/nstime.h"

namespace ns3 {

class UniformVariable;
class MacParameters;

class DcfAccessListener {
public:
  DcfAccessListener ();
  virtual ~DcfAccessListener ();

  /* Tell the listener than it can start
   * accessing the medium right now.
   */
  virtual void AccessGrantedNow (void) = 0;
  /* ask the listener if there are candidates 
   * who need access to the medium.
   */
  virtual bool AccessNeeded (void) = 0;
  /* ask the listener if it is currently
   * performing an access which was granted 
   * earlier to him and if it will notify
   * the Dcf when the access is complete.
   */
  virtual bool AccessingAndWillNotify (void) = 0;
};

class Dcf
{
public:
  Dcf ();
  ~Dcf ();

  void ResetRng (uint32_t seed);

  void SetParameters (const MacParameters *parameters);
  void SetDifs (Time difs);
  void SetEifs (Time eifs);
  void SetCwBounds (uint32_t min, uint32_t max);
  void RegisterAccessListener (DcfAccessListener *listener);

  void RequestAccess (void);

  void ResetCw (void);
  void UpdateFailedCw (void);
  void StartBackoff (void);

  /* notification methods. */
  void NotifyRxStartNow (Time duration);
  void NotifyRxEndOkNow (void);
  void NotifyRxEndErrorNow (void);
  void NotifyTxStartNow (Time duration);
  void NotifyCcaBusyStartNow (Time duration);
  void NotifyNavReset (Time now, Time duration);
  void NotifyNavStart (Time now, Time duration);
  void NotifyNavContinue (Time now, Time duration);
private:
  void AccessTimeout (void);

  /* trivial helpers */
  Time PickBackoffDelay (void);
  Time MostRecent (Time a, Time b) const;
  Time MostRecent (Time a, Time b, Time c) const;
  Time MostRecent (Time a, Time b, Time c, Time d) const;
  Time GetDifs (void) const;
  Time GetEifs (void) const;
  uint32_t GetCwMin (void) const;
  uint32_t GetCwMax (void) const;

  /* time calculation helpers */
  bool IsPhyBusy (void);
  bool IsBackoffNotCompleted (Time now);
  Time GetDelayUntilAccessGranted (Time now);
  Time GetAccessGrantedStart (void) const;
  void UpdateBackoff (Time time);

  EventId m_accessTimerEvent;

  UniformVariable *m_random;
  const MacParameters *m_parameters;
  DcfAccessListener *m_listener;
  Time m_difs;
  Time m_eifs;
  uint32_t m_cwMin;
  uint32_t m_cwMax;

  UVTraceSource<uint32_t> m_cw;
  Time m_backoffStart;
  Time m_backoffLeft;
  Time m_lastNavStart;
  Time m_lastNavDuration;
  Time m_lastRxStart;
  Time m_lastRxDuration;
  bool m_lastRxReceivedOk;
  Time m_lastRxEnd;
  Time m_lastTxStart;
  Time m_lastTxDuration;
  Time m_lastBusyStart;
  Time m_lastBusyDuration;
  bool m_rxing;
  bool m_sleeping;
  /* "80211-dcf-backoff"
   * param1: backoff Duration
   * reports the start of a backoff
   */
  CallbackTraceSource<Time> m_backoffTrace;
};

} // namespace ns3

#endif /* DCF_H */
