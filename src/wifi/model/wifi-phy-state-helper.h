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
#ifndef WIFI_PHY_STATE_HELPER_H
#define WIFI_PHY_STATE_HELPER_H

#include "wifi-phy.h"
#include "ns3/traced-callback.h"
#include "ns3/object.h"
#include <vector>

namespace ns3 {

/**
 * \ingroup wifi
 *
 * This objects implements the PHY state machine of the Wifi device.
 */
class WifiPhyStateHelper : public Object
{
public:
  static TypeId GetTypeId (void);

  WifiPhyStateHelper ();

  void SetReceiveOkCallback (WifiPhy::RxOkCallback callback);
  void SetReceiveErrorCallback (WifiPhy::RxErrorCallback callback);
  void RegisterListener (WifiPhyListener *listener);
  enum WifiPhy::State GetState (void);
  bool IsStateCcaBusy (void);
  bool IsStateIdle (void);
  bool IsStateBusy (void);
  bool IsStateRx (void);
  bool IsStateTx (void);
  bool IsStateSwitching (void);
  Time GetStateDuration (void);
  Time GetDelayUntilIdle (void);
  Time GetLastRxStartTime (void) const;

  void SwitchToTx (Time txDuration, Ptr<const Packet> packet, WifiMode txMode, WifiPreamble preamble, uint8_t txPower);
  void SwitchToRx (Time rxDuration);
  void SwitchToChannelSwitching (Time switchingDuration);
  void SwitchFromRxEndOk (Ptr<Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble);
  void SwitchFromRxEndError (Ptr<const Packet> packet, double snr);
  void SwitchMaybeToCcaBusy (Time duration);

  TracedCallback<Time,Time,enum WifiPhy::State> m_stateLogger;
private:
  typedef std::vector<WifiPhyListener *> Listeners;

  void LogPreviousIdleAndCcaBusyStates (void);

  void NotifyTxStart (Time duration);
  void NotifyWakeup (void);
  void NotifyRxStart (Time duration);
  void NotifyRxEndOk (void);
  void NotifyRxEndError (void);
  void NotifyMaybeCcaBusyStart (Time duration);
  void NotifySwitchingStart (Time duration);
  void DoSwitchFromRx (void);

  bool m_rxing;
  Time m_endTx;
  Time m_endRx;
  Time m_endCcaBusy;
  Time m_endSwitching;
  Time m_startTx;
  Time m_startRx;
  Time m_startCcaBusy;
  Time m_startSwitching;
  Time m_previousStateChangeTime;

  Listeners m_listeners;
  TracedCallback<Ptr<const Packet>, double, WifiMode, enum WifiPreamble> m_rxOkTrace;
  TracedCallback<Ptr<const Packet>, double> m_rxErrorTrace;
  TracedCallback<Ptr<const Packet>,WifiMode,WifiPreamble,uint8_t> m_txTrace;
  WifiPhy::RxOkCallback m_rxOkCallback;
  WifiPhy::RxErrorCallback m_rxErrorCallback;
};

} // namespace ns3

#endif /* WIFI_PHY_STATE_HELPER_H */
