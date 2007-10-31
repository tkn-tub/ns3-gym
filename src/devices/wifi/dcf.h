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

class RandomStream;
class MacParameters;

/**
 * \brief listen to DCF events
 *
 * If you want to call methods from the ns3::Dcf class,
 * you need to provide an instance of this class
 * to be notified of the DCF evens.
 */
class DcfAccessListener {
public:
  DcfAccessListener ();
  virtual ~DcfAccessListener ();

  /**
   * Tell the listener than it can start
   * accessing the medium right now.
   */
  virtual void AccessGrantedNow (void) = 0;
  /**
   * ask the listener if there are candidates 
   * who need access to the medium.
   *
   * \return true if access to the medium is 
   *         needed, false otherwise.
   */
  virtual bool AccessNeeded (void) = 0;
  /**
   * ask the listener if it is currently
   * performing an access which was granted 
   * earlier to him and if it will notify
   * the Dcf when the access is complete.
   *
   * \return true if the listener expects to call
   *         Dcf::RequestAccess later, false otherwise.
   */
  virtual bool AccessingAndWillNotify (void) = 0;
};

/**
 * \brief the Distributed Coordination Function
 *
 * This class implements the DCF as described in IEEE 802.11-1999
 * section 9.2, p72.
 *
 * This implementation is based on the technique described in
 * <i>Scalable simulation of large-scale wireless networks with
 * bounded inaccuracies.</i>, by Z. Ji, J. Zhou, M. Takai, and R. Bagrodia. 
 */
class Dcf
{
public:
  /**
   * \param minCw the minimum value for CW
   * \param maxCw the maximum value for CW
   */
  Dcf (uint32_t minCw, uint32_t maxCw);
  ~Dcf ();

  /**
   * \param parameters
   *
   * Must be invoked after construction to configure
   * a set of parameters.
   */
  void SetParameters (const MacParameters *parameters);
  /**
   * \param difs the difs
   *
   * Must be invoked after construction.
   */
  void SetDifs (Time difs);
  /**
   * \param eifs the eifs
   *
   * Must be invoked after construction.
   */
  void SetEifs (Time eifs);
  /**
   * \param minCw the minimum value for CW
   * \param maxCw the maximum value for CW
   *
   * Reset the cw bounds and CW to minCW.
   */
  void SetCwBounds (uint32_t minCw, uint32_t maxCw);
  /**
   * \param listener the listener
   *
   * This listener is notified of DCF-specific events
   * when they happen. You _must_ register a listener
   * before calling Dcf::RequestAccess.
   */
  void RegisterAccessListener (DcfAccessListener *listener);

  /**
   * Request access to the medium. This method will grant
   * access by calling the DcfAccessListener::AccessGrantedNow
   * method
   */
  void RequestAccess (void);

  /**
   * Reset the CW to CWmin
   * This method is typically invoked after a successfully
   * transmission or after the maximum number of retries has
   * been reached.
   */
  void ResetCw (void);
  /**
   * Update the CW to a new value. This method is typically
   * invoked after a failed transmission before calling
   * Dcf::StartBackoff.
   */
  void UpdateFailedCw (void);
  /**
   * Start a backoff now by picking a backoff duration
   * in the [0, cw] interval.
   */
  void StartBackoff (void);

  /**
   * \param duration expected duration of reception
   *
   * Notify the DCF that a packet reception started 
   * for the expected duration.
   */
  void NotifyRxStartNow (Time duration);
  /**
   * Notify the DCF that a packet reception was just
   * completed successfully.
   */
  void NotifyRxEndOkNow (void);
  /**
   * Notify the DCF that a packet reception was just
   * completed unsuccessfully.
   */
  void NotifyRxEndErrorNow (void);
  /**
   * \param duration expected duration of transmission
   *
   * Notify the DCF that a packet transmission was
   * just started and is expected to last for the specified
   * duration.
   */
  void NotifyTxStartNow (Time duration);
  /**
   * \param duration expected duration of cca busy period
   *
   * Notify the DCF that a CCA busy period has just started.
   */
  void NotifyCcaBusyStartNow (Time duration);
  /**
   * \param now the time at which a NAV starts
   * \param duration the value of the received NAV.
   */
  void NotifyNavReset (Time now, Time duration);
  /**
   * \param now the time at which a NAV starts
   * \param duration the value of the received NAV.
   */
  void NotifyNavStart (Time now, Time duration);
  /**
   * \param now the time at which a NAV starts
   * \param duration the value of the received NAV.
   */
  void NotifyNavContinue (Time now, Time duration);

  /**
   * \param stream a random stream
   *
   * This method is used for testing only to force a predictable
   * set of random numbers to be used.
   */
  void ResetRngForTest (RandomStream *stream);
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
  bool IsPhyBusy (void) const;
  bool IsNavBusy (void) const;
  bool IsBackoffNotCompleted (Time now);
  Time GetDelayUntilAccessGranted (Time now);
  Time GetAccessGrantedStart (void) const;
  void UpdateBackoff (Time time);

  EventId m_accessTimerEvent;

  RandomStream *m_rng;
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
