/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include <vector>

namespace ns3 {

class MacParameters;

class DcfState
{
public:
  DcfState ();

  virtual ~DcfState ();

  void SetAifsn (uint32_t aifsn);
  void SetCwBounds (uint32_t minCw, uint32_t maxCw);
  void ResetCw (void);
  void UpdateFailedCw (void);
  void StartBackoffNow (uint32_t nSlots);

private:
  friend class DcfManager;

  uint32_t GetAifsn (void) const;
  uint32_t GetCw (void) const;
  uint32_t GetBackoffSlots (void) const;
  Time GetBackoffStart (void) const;

  void UpdateBackoffSlotsNow (uint32_t nSlots, Time backoffUpdateBound);

  virtual bool NeedsAccess (void) const = 0;
  virtual void NotifyAccessGranted (void) = 0;
  virtual void NotifyInternalCollision (void) = 0;
  virtual void NotifyCollision (void) = 0;

  uint32_t m_aifsn;
  uint32_t m_backoffSlots;
  // the backoffStart variable is used to keep track of the
  // time at which a backoff was started or the time at which
  // the backoff counter was last updated.
  Time m_backoffStart;
  uint32_t m_cwMin;
  uint32_t m_cwMax;
  uint32_t m_cw;
};

class DcfManager
{
public:
  DcfManager ();

  void SetSlotTime (Time slotTime);
  void SetSifs (Time sifs);

  // at the lowest mandatory rate.
  // used for EIFS calculation.
  void SetAckTxDuration (Time ackTxDuration);

  void Add (DcfState *dcf);

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
  void NotifyCcaBusyStartNow (Time duration);
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

private:
  void UpdateBackoff (void);
  Time MostRecent (Time a, Time b) const;
  Time MostRecent (Time a, Time b, Time c) const;
  Time MostRecent (Time a, Time b, Time c, Time d) const;
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
  Time m_ackTxTime;
  EventId m_accessTimeout;
  Time m_slotTime;
  Time m_sifs;
};

} // namespace ns3
