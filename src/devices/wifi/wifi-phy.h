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

#ifndef WIFI_PHY_H
#define WIFI_PHY_H

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
#include "ns3/wifi-mode.h"


namespace ns3 {

class RandomUniform;
class RxEvent;
class TraceContainer;
class WifiNetDevice;
class WifiChannel;

class WifiPhyListener {
public:
  virtual ~WifiPhyListener ();

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



class WifiPhy
{
public:
  typedef Callback<void,Packet const , double, WifiMode, WifiMode, uint32_t> SyncOkCallback;
  typedef Callback<void,Packet const , double> SyncErrorCallback;

  WifiPhy (Ptr<WifiNetDevice> device);
  virtual ~WifiPhy ();

  void SetChannel (Ptr<WifiChannel> channel);

  void SetReceiveOkCallback (SyncOkCallback callback);
  void SetReceiveErrorCallback (SyncErrorCallback callback);

  void SendPacket (Packet const packet, WifiMode mode, WifiMode headeMode, uint8_t txPower, uint32_t stuff);

  void RegisterListener (WifiPhyListener *listener);

  bool IsStateCcaBusy (void);
  bool IsStateIdle (void);
  bool IsStateBusy (void);
  bool IsStateSync (void);
  bool IsStateTx (void);
  Time GetStateDuration (void);
  Time GetDelayUntilIdle (void);

  Time CalculateTxDuration (uint32_t size, WifiMode payloadMode) const;

  void Configure80211a (void);
  void SetEdThresholdDbm (double rxThreshold);
  void SetRxNoiseDb (double rxNoise);  
  void SetTxPowerIncrementsDbm (double txPowerBase, 
                                double txPowerEnd, 
                                int nTxPower);
  void SetRxTxGainDbm (double rxGainDbm, double txGainDbm);
  uint32_t GetNModes (void) const;
  WifiMode GetMode (uint32_t mode) const;
  uint32_t GetModeBitRate (uint8_t mode) const;
  uint32_t GetNTxpower (void) const;
  /* return snr: W/W */
  double CalculateSnr (WifiMode txMode, double ber) const;

private:
  enum WifiPhyState {
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
  typedef std::vector<WifiMode> Modes;
  typedef std::list<WifiPhyListener *> Listeners;
  typedef std::list<Ptr<RxEvent> > Events;
  typedef std::vector <NiChange> NiChanges;

private:  
  char const *StateToString (enum WifiPhyState state);
  enum WifiPhyState GetState (void);
  double GetEdThresholdW (void) const;
  double DbmToW (double dbm) const;
  double DbToRatio (double db) const;
  Time GetMaxPacketDuration (void) const;
  void CancelRx (void);
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
  double CalculateSnr (double signal, double noiseInterference, WifiMode mode) const;
  double CalculateChunkSuccessRate (double snir, Time delay, WifiMode mode) const;
  double CalculatePer (Ptr<const RxEvent> event, NiChanges *ni) const;
  void EndSync (Packet const packet, Ptr<RxEvent> event, uint32_t stuff);
  double GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const;
  /* rxPower unit is Watt */
  void ReceivePacket (Packet packet,
                      double rxPowerW,
                      WifiMode mode,
                      WifiMode headerMode,
                      uint32_t stuff);
private:
  uint64_t m_txPrepareDelayUs;
  uint64_t m_plcpPreambleDelayUs;
  uint32_t m_plcpHeaderLength;
  Time     m_maxPacketDuration;

  double   m_edThresholdW; /* unit: W */
  double   m_txGainDbm;
  double   m_rxGainDbm;
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

  Ptr<WifiChannel> m_channel;
  Ptr<WifiNetDevice> m_device;
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


#endif /* WIFI_PHY_H */
