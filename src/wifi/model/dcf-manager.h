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

#ifndef DCF_MANAGER_H
#define DCF_MANAGER_H

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include <vector>

namespace ns3 {

class WifiPhy;
class WifiMac;
class MacLow;
class PhyListener;
class LowDcfListener;

/**
 * \brief keep track of the state needed for a single DCF
 * function.
 * \ingroup wifi
 *
 * Multiple instances of a DcfState can be registered in a single
 * DcfManager to implement 802.11e-style relative QoS.
 * DcfState::SetAifsn and DcfState::SetCwBounds allow the user to
 * control the relative QoS differentiation.
 */
class DcfState
{
public:
  DcfState ();
  virtual ~DcfState ();

  /**
   * \return whether this DCF state is an EDCA state
   *
   * This method, which must be overridden in derived classes,
   * indicates whether DCF or EDCAF rules should be used for this
   * channel access function. This affects the behavior of DcfManager
   * when dealing with this instance. 
   */
  virtual bool IsEdca (void) const = 0;

  /**
   * \param aifsn the number of slots which make up an AIFS for a specific DCF.
   *        a DIFS corresponds to an AIFSN = 2.
   *
   * Calling this method after DcfManager::Add has been called is not recommended.
   */
  void SetAifsn (uint32_t aifsn);
  /**
   * Set the minimum congestion window size.
   *
   * \param minCw the minimum congestion window size
   */
  void SetCwMin (uint32_t minCw);
  /**
   * Set the maximum congestion window size.
   *
   * \param maxCw the maximum congestion window size
   */
  void SetCwMax (uint32_t maxCw);
  /**
   * Return the number of slots that make up an AIFS.
   *
   * \return the number of slots that make up an AIFS
   */
  uint32_t GetAifsn (void) const;
  /**
   * Return the minimum congestion window size.
   *
   * \return the minimum congestion window size
   */
  uint32_t GetCwMin (void) const;
  /**
   * Return the maximum congestion window size.
   *
   * \return the maximum congestion window size
   */
  uint32_t GetCwMax (void) const;
  /**
   * Update the value of the CW variable to take into account
   * a transmission success or a transmission abort (stop transmission
   * of a packet after the maximum number of retransmissions has been
   * reached). By default, this resets the CW variable to minCW.
   */
  void ResetCw (void);
  /**
   * Update the value of the CW variable to take into account
   * a transmission failure. By default, this triggers a doubling
   * of CW (capped by maxCW).
   */
  void UpdateFailedCw (void);
  /**
   * \param nSlots the number of slots of the backoff.
   *
   * Start a backoff by initializing the backoff counter to the number of
   * slots specified.
   */
  void StartBackoffNow (uint32_t nSlots);
  /**
   * \returns the current value of the CW variable. The initial value is
   * minCW.
   */
  uint32_t GetCw (void) const;
  /**
   * \returns true if access has been requested for this DcfState and
   *          has not been granted already, false otherwise.
   */
  bool IsAccessRequested (void) const;


private:
  friend class DcfManager;

  /**
   * Return the current number of backoff slots.
   *
   * \return the current number of backoff slots
   */
  uint32_t GetBackoffSlots (void) const;
  /**
   * Return the time when the backoff procedure started.
   *
   * \return the time when the backoff procedure started
   */
  Time GetBackoffStart (void) const;
  /**
   * Update backoff slots that nSlots has passed.
   *
   * \param nSlots
   * \param backoffUpdateBound
   */
  void UpdateBackoffSlotsNow (uint32_t nSlots, Time backoffUpdateBound);
  /**
   * Notify that access request has been received.
   */
  void NotifyAccessRequested (void);
  /**
   * Notify that access has been granted.
   */
  void NotifyAccessGranted (void);
  /**
   * Notify that collision has occurred.
   */
  void NotifyCollision (void);
  /**
   * Notify that internal collision has occurred.
   */
  void NotifyInternalCollision (void);
  /**
   * Notify that the device is switching channel.
   */
  void NotifyChannelSwitching (void);
  /**
   * Notify that the device has started to sleep.
   */
  void NotifySleep (void);
  /**
   * Notify that the device has started to wake up
   */
  void NotifyWakeUp (void);

  /**
   * Called by DcfManager to notify a DcfState subclass
   * that access to the medium is granted and can
   * start immediately.
   */
  virtual void DoNotifyAccessGranted (void) = 0;
  /**
   * Called by DcfManager to notify a DcfState subclass
   * that an 'internal' collision occured, that is, that
   * the backoff timer of a higher priority DcfState expired
   * at the same time and that access was granted to this
   * higher priority DcfState.
   *
   * The subclass is expected to start a new backoff by
   * calling DcfState::StartBackoffNow and DcfManager::RequestAccess
   * is access is still needed.
   */
  virtual void DoNotifyInternalCollision (void) = 0;
  /**
   * Called by DcfManager to notify a DcfState subclass
   * that a normal collision occured, that is, that
   * the medium was busy when access was requested.
   *
   * The subclass is expected to start a new backoff by
   * calling DcfState::StartBackoffNow and DcfManager::RequestAccess
   * is access is still needed.
   */
  virtual void DoNotifyCollision (void) = 0;
  /**
  * Called by DcfManager to notify a DcfState subclass
  * that a channel switching occured.
  *
  * The subclass is expected to flush the queue of packets.
  */
  virtual void DoNotifyChannelSwitching (void) = 0;
  /**
  * Called by DcfManager to notify a DcfState subclass that the device has
  * begun to sleep.
  *
  * The subclass is expected to re-insert the pending packet into the queue
  */
  virtual void DoNotifySleep (void) = 0;
  /**
  * Called by DcfManager to notify a DcfState subclass that the device
  * has begun to wake up.
  *
  * The subclass is expected to restart a new backoff by
  * calling DcfState::StartBackoffNow and DcfManager::RequestAccess
  * is access is still needed.
  */
  virtual void DoNotifyWakeUp (void) = 0;

  uint32_t m_aifsn;
  uint32_t m_backoffSlots;
  //the backoffStart variable is used to keep track of the
  //time at which a backoff was started or the time at which
  //the backoff counter was last updated.
  Time m_backoffStart;
  uint32_t m_cwMin;
  uint32_t m_cwMax;
  uint32_t m_cw;
  bool m_accessRequested;
};


/**
 * \brief Manage a set of ns3::DcfState
 * \ingroup wifi
 *
 * Handle a set of independent ns3::DcfState, each of which represents
 * a single DCF within a MAC stack. Each ns3::DcfState has a priority
 * implicitely associated with it (the priority is determined when the
 * ns3::DcfState is added to the DcfManager: the first DcfState to be
 * added gets the highest priority, the second, the second highest
 * priority, and so on.) which is used to handle "internal" collisions.
 * i.e., when two local DcfState are expected to get access to the
 * medium at the same time, the highest priority local DcfState wins
 * access to the medium and the other DcfState suffers a "internal"
 * collision.
 */
class DcfManager
{
public:
  DcfManager ();
  ~DcfManager ();

  /**
   * Set up listener for Phy events.
   *
   * \param phy
   */
  void SetupPhyListener (Ptr<WifiPhy> phy);
  /**
   * Remove current registered listener for Phy events.
   *
   * \param phy
   */
  void RemovePhyListener (Ptr<WifiPhy> phy);
  /**
   * Set up listener for MacLow events.
   *
   * \param low
   */
  void SetupLowListener (Ptr<MacLow> low);

  /**
   * \param slotTime the duration of a slot.
   *
   * It is a bad idea to call this method after RequestAccess or
   * one of the Notify methods has been invoked.
   */
  void SetSlot (Time slotTime);
  /**
   * \param sifs the duration of a SIFS.
   *
   * It is a bad idea to call this method after RequestAccess or
   * one of the Notify methods has been invoked.
   */
  void SetSifs (Time sifs);
  /**
   * \param eifsNoDifs the duration of a EIFS minus the duration of DIFS.
   *
   * It is a bad idea to call this method after RequestAccess or
   * one of the Notify methods has been invoked.
   */
  void SetEifsNoDifs (Time eifsNoDifs);

  /**
   * \return value set previously using SetEifsNoDifs.
   */
  Time GetEifsNoDifs () const;

  /**
   * \param dcf a new DcfState.
   *
   * The DcfManager does not take ownership of this pointer so, the callee
   * must make sure that the DcfState pointer will stay valid as long
   * as the DcfManager is valid. Note that the order in which DcfState
   * objects are added to a DcfManager matters: the first DcfState added
   * has the highest priority, the second DcfState added, has the second
   * highest priority, etc.
   */
  void Add (DcfState *dcf);

  /**
   * \param state a DcfState
   *
   * Notify the DcfManager that a specific DcfState needs access to the
   * medium. The DcfManager is then responsible for starting an access
   * timer and, invoking DcfState::DoNotifyAccessGranted when the access
   * is granted if it ever gets granted.
   */
  void RequestAccess (DcfState *state);

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
  void NotifyMaybeCcaBusyStartNow (Time duration);
  /**
   * \param duration expected duration of channel switching period
   *
   * Notify the DCF that a channel switching period has just started.
   * During switching state, new packets can be enqueued in DcaTxop/EdcaTxop
   * but they won't access to the medium until the end of the channel switching.
   */
  void NotifySwitchingStartNow (Time duration);
  /**
   * Notify the DCF that the device has been put in sleep mode.
   */
  void NotifySleepNow (void);
  /**
   * Notify the DCF that the device has been resumed from sleep mode.
   */
  void NotifyWakeupNow (void);
  /**
   * \param duration the value of the received NAV.
   *
   * Called at end of rx
   */
  void NotifyNavResetNow (Time duration);
  /**
   * \param duration the value of the received NAV.
   *
   * Called at end of rx
   */
  void NotifyNavStartNow (Time duration);
  /**
   * Notify that ACK timer has started for the given duration.
   *
   * \param duration
   */
  void NotifyAckTimeoutStartNow (Time duration);
  /**
   * Notify that ACK timer has resetted.
   */
  void NotifyAckTimeoutResetNow ();
  /**
   * Notify that CTS timer has started for the given duration.
   *
   * \param duration
   */
  void NotifyCtsTimeoutStartNow (Time duration);
  /**
   * Notify that CTS timer has resetted.
   */
  void NotifyCtsTimeoutResetNow ();


private:
  /**
   * Update backoff slots for all DcfStates.
   */
  void UpdateBackoff (void);
  /**
   * Return the most recent time.
   *
   * \param a
   * \param b
   *
   * \return the most recent time
   */
  Time MostRecent (Time a, Time b) const;
  /**
   * Return the most recent time.
   *
   * \param a
   * \param b
   * \param c
   *
   * \return the most recent time
   */
  Time MostRecent (Time a, Time b, Time c) const;
  /**
   * Return the most recent time.
   *
   * \param a
   * \param b
   * \param c
   * \param d
   *
   * \return the most recent time
   */
  Time MostRecent (Time a, Time b, Time c, Time d) const;
  /**
   * Return the most recent time.
   *
   * \param a
   * \param b
   * \param c
   * \param d
   * \param e
   * \param f
   *
   * \return the most recent time
   */
  Time MostRecent (Time a, Time b, Time c, Time d, Time e, Time f) const;
  /**
   * Return the most recent time.
   *
   * \param a
   * \param b
   * \param c
   * \param d
   * \param e
   * \param f
   * \param g
   *
   * \return the most recent time
   */
  Time MostRecent (Time a, Time b, Time c, Time d, Time e, Time f, Time g) const;
  /**
   * Access will never be granted to the medium _before_
   * the time returned by this method.
   *
   * \returns the absolute time at which access could start to be granted
   */
  Time GetAccessGrantStart (void) const;
  /**
   * Return the time when the backoff procedure
   * started for the given DcfState.
   *
   * \param state
   *
   * \return the time when the backoff procedure started
   */
  Time GetBackoffStartFor (DcfState *state);
  /**
   * Return the time when the backoff procedure
   * ended (or will ended) for the given DcfState.
   *
   * \param state
   *
   * \return the time when the backoff procedure ended (or will ended)
   */
  Time GetBackoffEndFor (DcfState *state);

  void DoRestartAccessTimeoutIfNeeded (void);

  /**
   * Called when access timeout should occur
   * (e.g. backoff procedure expired).
   */
  void AccessTimeout (void);
  /**
   * Grant access to DCF
   */
  void DoGrantAccess (void);
  /**
   * Check if the device is busy sending or receiving,
   * or NAV busy.
   *
   * \return true if the device is busy,
   *         false otherwise
   */
  bool IsBusy (void) const;

  /**
   * typedef for a vector of DcfStates
   */
  typedef std::vector<DcfState *> States;

  States m_states;
  Time m_lastAckTimeoutEnd;
  Time m_lastCtsTimeoutEnd;
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
  Time m_lastSwitchingStart;
  Time m_lastSwitchingDuration;
  bool m_rxing;
  bool m_sleeping;
  Time m_eifsNoDifs;
  EventId m_accessTimeout;
  uint32_t m_slotTimeUs;
  Time m_sifs;
  PhyListener* m_phyListener;
  LowDcfListener* m_lowListener;
};

} //namespace ns3

#endif /* DCF_MANAGER_H */
