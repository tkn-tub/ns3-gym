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

#ifndef DCF_STATE_H
#define DCF_STATE_H

#include "ns3/nstime.h"
#include "ns3/simulator.h"

namespace ns3 {

class DcaTxop;

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
class DcfState : public Object
{
public:

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * Constructor
   *
   * \param txop the DCA txop
   */
  DcfState (Ptr<DcaTxop> txop);
  virtual ~DcfState ();

  /**
   * \return whether this DCF state is an EDCA state
   */
  bool IsEdca (void) const;

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
   * Set the TXOP limit.
   *
   * \param txopLimit the TXOP limit
   */
  void SetTxopLimit (Time txopLimit);
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
   * Return the TXOP limit.
   *
   * \return the TXOP limit
   */
  Time GetTxopLimit (void) const;

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


protected:
  /// DcfManager associated class
  friend class DcfManager;

  // Inherited from ns3::Object
  void DoDispose (void);

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

  uint32_t m_aifsn;        //!< the AIFSN
  uint32_t m_backoffSlots; //!< the backoff slots
  /**
   * the backoffStart variable is used to keep track of the
   * time at which a backoff was started or the time at which
   * the backoff counter was last updated.
   */
  Time m_backoffStart;
  uint32_t m_cwMin;       //!< the CW minimum
  uint32_t m_cwMax;       //!< the CW maximum
  uint32_t m_cw;          //!< the current CW
  Time m_txopLimit;       //!< the txop limit time
  bool m_accessRequested; //!< flag whether channel access is already requested
  Ptr<DcaTxop> m_txop;    //!< the DCA TXOP
};

} //namespace ns3

#endif /* DCF_STATE_H */
