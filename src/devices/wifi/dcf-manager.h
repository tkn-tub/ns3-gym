/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include <vector>

namespace ns3 {

class WifiPhy;
class WifiMac;
class MacLow;

/**
 * \brief keep track of the state needed for a single DCF 
 * function.
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
   * \param aifsn the number of slots which make up an AIFS for a specific DCF.
   *        a DIFS corresponds to an AIFSN = 2.
   *
   * Calling this method after DcfManager::Add has been called is not recommended.
   */
  void SetAifsn (uint32_t aifsn);
  void SetCwMin (uint32_t minCw);
  void SetCwMax (uint32_t maxCw);
  uint32_t GetAifsn (void) const;
  uint32_t GetCwMin (void) const;
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

  uint32_t GetBackoffSlots (void) const;
  Time GetBackoffStart (void) const;
  void UpdateBackoffSlotsNow (uint32_t nSlots, Time backoffUpdateBound);
  void NotifyAccessRequested (void);
  void NotifyAccessGranted (void);
  void NotifyCollision (void);
  void NotifyInternalCollision (void);


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

  uint32_t m_aifsn;
  uint32_t m_backoffSlots;
  // the backoffStart variable is used to keep track of the
  // time at which a backoff was started or the time at which
  // the backoff counter was last updated.
  Time m_backoffStart;
  uint32_t m_cwMin;
  uint32_t m_cwMax;
  uint32_t m_cw;
  bool m_accessRequested;
};

/**
 * \brief Manage a set of ns3::DcfState
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

  void SetupPhyListener (Ptr<WifiPhy> phy);
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
  void NotifyAckTimeoutStartNow (Time duration);
  void NotifyAckTimeoutResetNow ();
  void NotifyCtsTimeoutStartNow (Time duration);
  void NotifyCtsTimeoutResetNow ();
private:
  void UpdateBackoff (void);
  Time MostRecent (Time a, Time b) const;
  Time MostRecent (Time a, Time b, Time c) const;
  Time MostRecent (Time a, Time b, Time c, Time d) const;
  Time MostRecent (Time a, Time b, Time c, Time d, Time e, Time f) const;
  /**
   * Access will never be granted to the medium _before_
   * the time returned by this method.
   * 
   * \returns the absolute time at which access could start to 
   * be granted
   */
  Time GetAccessGrantStart (void) const;
  Time GetBackoffStartFor (DcfState *state);
  Time GetBackoffEndFor (DcfState *state);
  void DoRestartAccessTimeoutIfNeeded (void);
  void AccessTimeout (void);
  void DoGrantAccess (void);
  bool IsBusy (void) const;

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
  bool m_rxing;
  bool m_sleeping;
  Time m_eifsNoDifs;
  EventId m_accessTimeout;
  Time m_slotTime;
  Time m_sifs;
  class PhyListener *m_phyListener;
  class LowDcfListener *m_lowListener;
};

} // namespace ns3
