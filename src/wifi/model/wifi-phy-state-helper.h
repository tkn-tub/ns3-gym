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

  /**
   * Set a callback for a successful reception.
   *
   * \param callback
   */
  void SetReceiveOkCallback (WifiPhy::RxOkCallback callback);
  /**
   * Set a callback for a failed reception.
   *
   * \param callback
   */
  void SetReceiveErrorCallback (WifiPhy::RxErrorCallback callback);
  /**
   * Register WifiPhyListener to this WifiPhyStateHelper.
   *
   * \param listener
   */
  void RegisterListener (WifiPhyListener *listener);
  /**
   * Remove WifiPhyListener from this WifiPhyStateHelper.
   *
   * \param listener
   */
  void UnregisterListener (WifiPhyListener *listener);
  /**
   * Return the current state of WifiPhy.
   *
   * \return the current state of WifiPhy
   */
  enum WifiPhy::State GetState (void);
  /**
   * Check whether the current state is CCA busy.
   *
   * \return true if the current state is CCA busy, false otherwise
   */
  bool IsStateCcaBusy (void);
  /**
   * Check whether the current state is IDLE.
   *
   * \return true if the current state is IDLE, false otherwise
   */
  bool IsStateIdle (void);
  /**
   * Check whether the current state is not IDLE.
   *
   * \return true if the current state is not IDLE, false otherwise
   */
  bool IsStateBusy (void);
  /**
   * Check whether the current state is RX.
   *
   * \return true if the current state is RX, false otherwise
   */
  bool IsStateRx (void);
  /**
   * Check whether the current state is TX.
   *
   * \return true if the current state is TX, false otherwise
   */
  bool IsStateTx (void);
  /**
   * Check whether the current state is SWITCHING.
   *
   * \return true if the current state is SWITCHING, false otherwise
   */
  bool IsStateSwitching (void);
  /**
   * Check whether the current state is SLEEP.
   *
   * \return true if the current state is SLEEP, false otherwise
   */
  bool IsStateSleep (void);
  /**
   * Return the elapsed time of the current state.
   *
   * \return the elapsed time of the current state
   */
  Time GetStateDuration (void);
  /**
   * Return the time before the state is back to IDLE.
   *
   * \return the delay before the state is back to IDLE
   */
  Time GetDelayUntilIdle (void);
  /**
   * Return the time the last RX start.
   *
   * \return the time the last RX start.
   */
  Time GetLastRxStartTime (void) const;

  /**
   * Switch state to TX for the given duration.
   *
   * \param txDuration the duration of the TX
   * \param packet the packet
   * \param txPowerDbm the nominal tx power in dBm
   * \param txVector the tx vector of the packet
   * \param preamble the preamble of the packet
   */
  void SwitchToTx (Time txDuration, Ptr<const Packet> packet, double txPowerDbm, WifiTxVector txVector, WifiPreamble preamble);
  /**
   * Switch state to RX for the given duration.
   *
   * \param rxDuration the duration of the RX
   */
  void SwitchToRx (Time rxDuration);
  /**
   * Switch state to channel switching for the given duration.
   *
   * \param switchingDuration the duration of required to switch the channel
   */
  void SwitchToChannelSwitching (Time switchingDuration);
  /**
   * Switch from RX after the reception was successful.
   *
   * \param packet the successfully received packet
   * \param snr the SNR of the received packet
   * \param mode the transmission mode of the packet
   * \param preamble the preamble of the received packet
   */
  void SwitchFromRxEndOk (Ptr<Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble);
  /**
   * Switch from RX after the reception failed.
   *
   * \param packet the packet that we failed to received
   * \param snr the SNR of the received packet
   */
  void SwitchFromRxEndError (Ptr<const Packet> packet, double snr);
  /**
   * Switch to CCA busy.
   *
   * \param duration the duration of CCA busy state
   */
  void SwitchMaybeToCcaBusy (Time duration);
  /**
   * Switch to sleep mode.
   */
  void SwitchToSleep (void);
  /**
   * Switch from sleep mode.
   *
   * \param duration the duration of CCA busy state
   */
  void SwitchFromSleep (Time duration);

  /** \todo Why is this public? */
  TracedCallback<Time,Time,enum WifiPhy::State> m_stateLogger;

  /**
   * TracedCallback signature for state changes.
   *
   * \param [in] start Time when the \p state started.
   * \param [in] duration Amount of time we've been in (or will be in)
   *             the \p state.
   * \param [in] state The state.
   */
  typedef void (* StateTracedCallback)
    (const Time start, const Time duration, const WifiPhy::State state);

  /**
   * TracedCallback signature for receive end ok event.
   *
   * \param [in] packet The received packet.
   * \param [in] snr    The SNR of the received packet.
   * \param [in] mode   The transmission mode of the packet.   
   * \param [in] preamble The preamble of the packet.
   */
  typedef void (* RxOkTracedCallback)
    (const Ptr<const Packet> packet, const double snr,
     const WifiMode mode, const WifiPreamble preamble);

  /**
   * TracedCallback signature for receive end error event.
   *
   * \param [in] packet The received packet.
   * \param [in] snr    The SNR of the received packet.
   */
  typedef void (* RxEndErrorTracedCallback)
    (const Ptr<const Packet> packet, const double snr);

  /**
   * TracedCallback signature for transmit event.
   *
   * \param [in] packet The received packet.
   * \param [in] mode   The transmission mode of the packet.   
   * \param [in] preamble The preamble of the packet.
   * \param [in] power  The transmit power level.
   */
  typedef void (* TxTracedCallback)
    (const Ptr<const Packet> packet, const WifiMode mode,
     const WifiPreamble preamble, const uint8_t power);
     
                
private:
  /**
   * typedef for a list of WifiPhyListeners
   */
  typedef std::vector<WifiPhyListener *> Listeners;
  typedef std::vector<WifiPhyListener *>::iterator ListenersI;

  /**
   * Log the ideal and CCA states.
   */
  void LogPreviousIdleAndCcaBusyStates (void);

  /**
   * Notify all WifiPhyListener that the transmission has started for the given duration.
   *
   * \param duration the duration of the transmission
   * \param txPowerDbm the nominal tx power in dBm
   */
  void NotifyTxStart (Time duration, double txPowerDbm);
  /**
   * Notify all WifiPhyListener that the reception has started for the given duration.
   *
   * \param duration the duration of the reception
   */
  void NotifyRxStart (Time duration);
  /**
   * Notify all WifiPhyListener that the reception was successful.
   */
  void NotifyRxEndOk (void);
  /**
   * Notify all WifiPhyListener that the reception was not successful.
   */
  void NotifyRxEndError (void);
  /**
   * Notify all WifiPhyListener that the CCA has started for the given duration.
   *
   * \param duration the duration of the CCA state
   */
  void NotifyMaybeCcaBusyStart (Time duration);
  /**
   * Notify all WifiPhyListener that we are switching channel with the given channel
   * switching delay.
   *
   * \param duration the delay to switch the channel
   */
  void NotifySwitchingStart (Time duration);
  /**
   * Notify all WifiPhyListener that we are going to sleep
   */
  void NotifySleep (void);
  /**
   * Notify all WifiPhyListener that we woke up
   */
  void NotifyWakeup (void);
  /**
   * Switch the state from RX.
   */
  void DoSwitchFromRx (void);

  bool m_rxing;
  bool m_sleeping;
  Time m_endTx;
  Time m_endRx;
  Time m_endCcaBusy;
  Time m_endSwitching;
  Time m_startTx;
  Time m_startRx;
  Time m_startCcaBusy;
  Time m_startSwitching;
  Time m_startSleep;
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
