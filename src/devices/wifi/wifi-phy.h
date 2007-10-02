/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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

#ifndef PHY_80211_H
#define PHY_80211_H

#include <vector>
#include <list>
#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/callback-trace-source.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"


namespace ns3 {

class TransmissionMode;
class PropagationModel;
class RandomUniform;
class RxEvent;
class TraceContainer;

class Phy80211Listener {
public:
  virtual ~Phy80211Listener ();

  /* we have received the first bit of a packet. We decided
   * that we could synchronize on this packet. It does not mean
   * we will be able to successfully receive completely the
   * whole packet. It means we will report a BUSY status.
   * r.end will be invoked later to report whether or not
   * the packet was successfully received.
   */
  virtual void NotifyRxStart (Time duration) = 0;
  /* we have received the last bit of a packet for which
   * rxStart was invoked first. 
   */
  virtual void NotifyRxEndOk (void) = 0;
  virtual void NotifyRxEndError (void) = 0;
  /* we start the transmission of a packet.
   */
  virtual void NotifyTxStart (Time duration) = 0;
  virtual void NotifyCcaBusyStart (Time duration) = 0;
};



class Phy80211
{
public:
  typedef Callback<void,Packet const , double, uint8_t, uint8_t> SyncOkCallback;
  typedef Callback<void,Packet const , double> SyncErrorCallback;

  Phy80211 ();
  virtual ~Phy80211 ();

  void SetPropagationModel (PropagationModel *propagation);
  void SetReceiveOkCallback (SyncOkCallback callback);
  void SetReceiveErrorCallback (SyncErrorCallback callback);

  /* rxPower unit is Watt */
  void ReceivePacket (Packet const packet,
                      double rxPowerW,
                      uint8_t txMode,
                      uint8_t stuff);
  void SendPacket (Packet const packet, uint8_t txMode, uint8_t txPower, uint8_t stuff);

  void RegisterListener (Phy80211Listener *listener);

  bool IsStateCcaBusy (void);
  bool IsStateIdle (void);
  bool IsStateBusy (void);
  bool IsStateSync (void);
  bool IsStateTx (void);
  Time GetStateDuration (void);
  Time GetDelayUntilIdle (void);

  Time CalculateTxDuration (uint32_t size, uint8_t payloadMode) const;

  void Configure80211a (void);
  void SetEdThresholdDbm (double rxThreshold);
  void SetRxNoiseDb (double rxNoise);  
  void SetTxPowerIncrementsDbm (double txPowerBase, 
              double txPowerEnd, 
              int nTxPower);
  uint32_t GetNModes (void) const;
  uint32_t GetModeBitRate (uint8_t mode) const;
  uint32_t GetNTxpower (void) const;
  /* return snr: W/W */
  double CalculateSnr (uint8_t txMode, double ber) const;

private:
  enum Phy80211State {
    SYNC,
    TX,
    CCA_BUSY,
    IDLE
  };
  class NiChange {
  public:
    NiChange (Time time, double delta);
    Time GetTime (void) const;
    double GetDelta (void) const;
    bool operator < (NiChange const &o) const;
  private:
    Time m_time;
    double m_delta;
  };
  typedef std::vector<TransmissionMode *> Modes;
  typedef std::vector<TransmissionMode *>::const_iterator ModesCI;
  typedef std::list<Phy80211Listener *> Listeners;
  typedef std::list<Phy80211Listener *>::const_iterator ListenersCI;
  typedef std::list<Ptr<RxEvent> > Events;
  typedef std::vector <NiChange> NiChanges;
  typedef std::vector <NiChange>::iterator NiChangesI;

private:  
  char const *StateToString (enum Phy80211State state);
  enum Phy80211State GetState (void);
  double GetEdThresholdW (void) const;
  double DbmToW (double dbm) const;
  double DbToRatio (double db) const;
  Time GetMaxPacketDuration (void) const;
  void AddTxRxMode (TransmissionMode *mode);
  void CancelRx (void);
  TransmissionMode *GetMode (uint8_t txMode) const;
  double GetPowerDbm (uint8_t power) const;
  void NotifyTxStart (Time duration);
  void NotifyWakeup (void);
  void NotifySyncStart (Time duration);
  void NotifySyncEndOk (void);
  void NotifySyncEndError (void);
  void NotifyCcaBusyStart (Time duration);
  void LogPreviousIdleAndCcaBusyStates (void);
  void SwitchToTx (Time txDuration);
  void SwitchToSync (Time syncDuration);
  void SwitchFromSync (void);
  void SwitchMaybeToCcaBusy (Time duration);
  void AppendEvent (Ptr<RxEvent> event);
  double CalculateNoiseInterferenceW (Ptr<RxEvent> event, NiChanges *ni) const;
  double CalculateSnr (double signal, double noiseInterference, TransmissionMode *mode) const;
  double CalculateChunkSuccessRate (double snir, Time delay, TransmissionMode *mode) const;
  double CalculatePer (Ptr<const RxEvent> event, NiChanges *ni) const;
  void EndSync (Packet const packet, Ptr<RxEvent> event, uint8_t stuff);
  double GetSnrForBer (TransmissionMode *mode, double ber) const;
private:
  uint64_t m_txPrepareDelayUs;
  uint64_t m_plcpPreambleDelayUs;
  uint32_t m_plcpHeaderLength;
  Time     m_maxPacketDuration;

  double   m_edThresholdW; /* unit: W */
  double   m_rxNoiseRatio;
  double   m_txPowerBaseDbm;
  double   m_txPowerEndDbm;
  uint32_t m_nTxPower;

  
  bool m_syncing;
  Time m_endTx;
  Time m_endSync;
  Time m_endCcaBusy;
  Time m_startTx;
  Time m_startSync;
  Time m_startCcaBusy;
  Time m_previousStateChangeTime;

  PropagationModel *m_propagation;
  SyncOkCallback m_syncOkCallback;
  SyncErrorCallback m_syncErrorCallback;
  Modes m_modes;
  Listeners m_listeners;
  EventId m_endSyncEvent;
  Events m_events;
  UniformVariable m_random;
  /* param1: - true: sync completed ok
   *   - false: sync completed with failure
   * Invoked when the last bit of a Signal (which was
   * synchronized upon) is received.
   * Reports whether or not the signal was received
   * successfully.
   */
  CallbackTraceSource<bool> m_endSyncLogger;
  /* param1: Duration 
   * param2: signal Energy (w)
   * Invoked whenever the first bit of a signal is received.
   */
  CallbackTraceSource<Time,double> m_startRxLogger;
  /* param1: Duration
   * param2: signal Energy (w)
   * Invoked whenever the first bit of a signal is 
   * synchronized upon.
   */
  CallbackTraceSource<Time,double> m_startSyncLogger;
  /* param1: Duration 
   * param2: tx Mode (bit rate: bit/s)
   * param3: tx Power (dbm)
   * Invoked whenever we send the first bit of a signal.
   */
  CallbackTraceSource<Time, uint32_t, double> m_startTxLogger;
  /* 80211-phy-state
   * param1: Start
   * param2: Duration
   * param3: state: 0 -> TX, 1 -> SYNC, 2 -> CCA, 3 -> IDLE
   */
  CallbackTraceSource<Time,Time,uint8_t> m_stateLogger;
};

}; // namespace ns3


#endif /* PHY_80211_H */
