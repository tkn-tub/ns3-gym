/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef WIFI_PHY_H
#define WIFI_PHY_H

#include <vector>
#include <list>
#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/callback-trace-source.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"


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


/**
 * \brief 802.11 PHY layer model
 *
 * This PHY implements a model of 802.11a. The model
 * implemented here is based on the model described
 * in "Yet Another Network Simulator", 
 * (http://cutebugs.net/files/wns2-yans.pdf).
 *
 * This PHY model depends on a channel loss and delay
 * model as provided by the ns3::PropagationLossModel
 * and ns3::PropagationDelayModel classes.
 * 
 */
class WifiPhy : public Object
{
public:
  /**
   * The state of the PHY layer.
   */
  enum State {
    /**
     * The PHY layer is synchronized upon a packet.
     */
    SYNC,
    /**
     * The PHY layer is sending a packet.
     */
    TX,
    /**
     * The PHY layer has sense the medium busy through
     * the CCA mechanism
     */
    CCA_BUSY,
    /**
     * The PHY layer is IDLE.
     */
    IDLE
  };
  /**
   * arg1: packet received successfully
   * arg2: snr of packet
   * arg3: mode of packet
   * arg4: type of preamble used for packet.
   */
  typedef Callback<void,Packet, double, WifiMode, enum WifiPreamble> SyncOkCallback;
  /**
   * arg1: packet received unsuccessfully
   * arg2: snr of packet
   */
  typedef Callback<void,Packet, double> SyncErrorCallback;

  /**
   * \param device the device which contains this PHY.
   *
   * Create a new PHY layer instance initialized with values
   * coming from \valueref{WifiPhyEnergyDetectionThreshold},
   * \valueref{WifiPhyRxNoise}, \valueref{WifiPhyTxPowerBase},
   * \valueref{WifiPhyTxPowerEnd}, \valueref{WifiPhyTxPowerLevels},
   * \valueref{WifiPhyTxGain}, and, \valueref{WifiPhyRxGain}
   */
  WifiPhy (Ptr<WifiNetDevice> device);
  virtual ~WifiPhy ();

  /**
   * \param channel the channel to connect to.
   */
  void SetChannel (Ptr<WifiChannel> channel);

  /**
   * \param callback the callback to invoke
   *        upon successful packet reception.
   */
  void SetReceiveOkCallback (SyncOkCallback callback);
  /**
   * \param callback the callback to invoke
   *        upon erronous packet reception.
   */
  void SetReceiveErrorCallback (SyncErrorCallback callback);

  /**
   * \param packet the packet to send
   * \param mode the transmission mode to use to send this packet
   * \param preamble the type of preamble to use to send this packet.
   * \param txPowerLevel a power level to use to send this packet. The real
   *        transmission power is calculated as txPowerMin + txPowerLevel * (txPowerMax - txPowerMin) / nTxLevels
   */
  void SendPacket (Packet const packet, WifiMode mode, enum WifiPreamble preamble, uint8_t txPowerLevel);

  /**
   * \param listener the new listener
   *
   * Add the input listener to the list of objects to be notified of
   * PHY-level events.
   */
  void RegisterListener (WifiPhyListener *listener);

  /**
   * \returns true of the current state of the PHY layer is WifiPhy:LCCA_BUSY, false otherwise.
   */
  bool IsStateCcaBusy (void);
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::IDLE, false otherwise.
   */
  bool IsStateIdle (void);
  /**
   * \returns true of the current state of the PHY layer is not WifiPhy::IDLE, false otherwise.
   */
  bool IsStateBusy (void);
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::SYNC, false otherwise.
   */
  bool IsStateSync (void);
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::TX, false otherwise.
   */
  bool IsStateTx (void);
  /**
   * \returns the amount of time since the current state has started.
   */
  Time GetStateDuration (void);
  /**
   * \returns the predicted delay until this PHY can become WifiPhy::IDLE.
   *
   * The PHY will never become WifiPhy::IDLE _before_ the delay returned by
   * this method but it could become really idle later.
   */
  Time GetDelayUntilIdle (void);

  /**
   * \param size the number of bytes in the packet to send
   * \param payloadMode the transmission mode to use for this packet
   * \param preamble the type of preamble to use for this packet.
   * \returns the total amount of time this PHY will stay busy for
   *          the transmission of these bytes.
   */
  Time CalculateTxDuration (uint32_t size, WifiMode payloadMode, enum WifiPreamble preamble) const;

  /**
   * \returns the number of transmission modes supported by this PHY.
   */
  uint32_t GetNModes (void) const;
  /**
   * \param mode index in array of supported modes
   * \returns the mode whose index is specified.
   */
  WifiMode GetMode (uint32_t mode) const;
  /**
   * \returns the number of tx power levels available for this PHY.
   */
  uint32_t GetNTxpower (void) const;
  /* return snr: W/W */
  /**
   * \param txMode the transmission mode
   * \param ber the probability of bit error rate
   * \returns the minimum snr which is required to achieve
   *          the requested ber for the specified transmission mode.
   */
  double CalculateSnr (WifiMode txMode, double ber) const;

private:
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
  // inherited from ns3::Object.
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
  void Configure80211a (void);
  char const *StateToString (enum State state);
  enum WifiPhy::State GetState (void);
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
  void EndSync (Packet const packet, Ptr<RxEvent> event);
  double Log2 (double val) const;
  double GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const;
  double GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const;
  uint32_t Factorial (uint32_t k) const;
  double Binomial (uint32_t k, double p, uint32_t n) const;
  double CalculatePdOdd (double ber, unsigned int d) const;
  double CalculatePdEven (double ber, unsigned int d) const;
  double CalculatePd (double ber, unsigned int d) const;
  double GetFecBpskBer (double snr, double nbits, 
                        uint32_t signalSpread, uint32_t phyRate,
                        uint32_t dFree, uint32_t adFree) const;
  double GetFecQamBer (double snr, uint32_t nbits, 
                       uint32_t signalSpread,
                       uint32_t phyRate,
                       uint32_t m, uint32_t dfree,
                       uint32_t adFree, uint32_t adFreePlusOne) const;
  double GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const;
  /* rxPower unit is Watt */
  void ReceivePacket (Packet packet,
                      double rxPowerDbm,
                      WifiMode mode,
                      WifiPreamble preamble);
private:
  uint64_t m_txPrepareDelayUs;
  uint64_t m_plcpLongPreambleDelayUs;
  uint64_t m_plcpShortPreambleDelayUs;
  WifiMode m_longPlcpHeaderMode;
  WifiMode m_shortPlcpHeaderMode;
  uint32_t m_plcpHeaderLength;
  Time     m_maxPacketDuration;

  double   m_edThresholdW; /* unit: W */
  double   m_txGainDb;
  double   m_rxGainDb;
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
  CallbackTraceSource<Time,Time,enum WifiPhy::State> m_stateLogger;
};

} // namespace ns3


#endif /* WIFI_PHY_H */
