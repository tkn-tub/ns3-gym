#ifndef WIFI_PHY_STATE_HELPER_H
#define WIFI_PHY_STATE_HELPER_H

#include "wifi-phy.h"
#include "ns3/traced-callback.h"
#include <vector>

namespace ns3 {

class WifiPhyStateHelper
{
public:
  WifiPhyStateHelper ();

  void SetReceiveOkCallback (WifiPhy::SyncOkCallback callback);
  void SetReceiveErrorCallback (WifiPhy::SyncErrorCallback callback);
  void RegisterListener (WifiPhyListener *listener);
  enum WifiPhy::State GetState (void);
  bool IsStateCcaBusy (void);
  bool IsStateIdle (void);
  bool IsStateBusy (void);
  bool IsStateSync (void);
  bool IsStateTx (void);
  Time GetStateDuration (void);
  Time GetDelayUntilIdle (void);
  Time GetLastRxStartTime (void) const;

  void SwitchToTx (Time txDuration, Ptr<const Packet> packet, WifiMode txMode, WifiPreamble preamble, uint8_t txPower);
  void SwitchToSync (Time syncDuration);
  void SwitchFromSyncEndOk (Ptr<Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble);
  void SwitchFromSyncEndError (Ptr<const Packet> packet, double snr);
  void SwitchMaybeToCcaBusy (Time duration);

private:
  typedef std::vector<WifiPhyListener *> Listeners;

  char const *StateToString (enum WifiPhy::State state);
  void LogPreviousIdleAndCcaBusyStates (void);

  void NotifyTxStart (Time duration);
  void NotifyWakeup (void);
  void NotifySyncStart (Time duration);
  void NotifySyncEndOk (void);
  void NotifySyncEndError (void);
  void NotifyCcaBusyStart (Time duration);
  void DoSwitchFromSync (void);

  bool m_syncing;
  Time m_endTx;
  Time m_endSync;
  Time m_endCcaBusy;
  Time m_startTx;
  Time m_startSync;
  Time m_startCcaBusy;
  Time m_previousStateChangeTime;
  TracedCallback<Time,Time,enum WifiPhy::State> m_stateLogger;
  Listeners m_listeners;
  TracedCallback<Ptr<const Packet>, double, WifiMode, enum WifiPreamble> m_rxOkTrace;
  TracedCallback<Ptr<const Packet>, double> m_rxErrorTrace;
  TracedCallback<Ptr<const Packet>,WifiMode,WifiPreamble,uint8_t> m_txTrace;
  WifiPhy::SyncOkCallback m_syncOkCallback;
  WifiPhy::SyncErrorCallback m_syncErrorCallback;
};

} // namespace ns3

#endif /* WIFI_PHY_STATE_HELPER_H */
