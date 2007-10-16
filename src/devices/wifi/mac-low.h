/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2006 INRIA
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
#ifndef MAC_LOW_H
#define MAC_LOW_H

#include <vector>
#include <stdint.h>

#include "wifi-mac-header.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/callback-trace-source.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"

namespace ns3 {

class WifiNetDevice;
class WifiPhy;
class PacketLogger;
class MacStations;
class MacStation;
class MacParameters;

class MacLowTransmissionListener {
public:
  MacLowTransmissionListener ();
  virtual ~MacLowTransmissionListener ();

  virtual void GotCts (double snr, WifiMode txMode) = 0;
  virtual void MissedCts (void) = 0;
  /* Do not rely on the gotAck method to be
   * given valid parameters when SuperFastAck is
   * enabled.
   */
  virtual void GotAck (double snr, WifiMode txMode) = 0;
  virtual void MissedAck (void) = 0;
  virtual void StartNext (void) = 0;

  /* Invoked if this transmission was canceled 
   * one way or another. When this method is invoked,
   * you can assume that the packet has not been passed
   * down the stack to the PHY. You are responsible
   * for freeing the packet if you want to.
   */
  virtual void Cancel (void) = 0;
};


class MacLowNavListener {
public:
  MacLowNavListener ();
  virtual ~MacLowNavListener ();
  virtual void NavStart (Time now, Time duration) = 0;
  virtual void NavContinue (Time now, Time duration) = 0;
  virtual void NavReset (Time now, Time duration) = 0;
};

class MacLowTransmissionParameters {
public:
  MacLowTransmissionParameters ();
    
  /* If ACK is enabled, we wait ACKTimeout for an ACK.
   */
  void EnableAck (void);
  /* If FastAck is enabled, we:
   *   - wait PIFS after end-of-tx. If idle, report
   *     FastAckMissed.
   *   - if busy at end-of-tx+PIFS, wait end-of-rx
   *   - if Ack ok at end-of-rx, report FastAck ok.
   *   - if Ack not ok at end-of-rx, report FastAckMissed
   *     at end-of-rx+SIFS.
   * This is really complicated but it is needed for
   * proper HCCA support.
   */
  void EnableFastAck (void);
  /* If SuperFastAck is enabled, we:
   *   - if busy at end-of-tx+PIFS, report gotAck
   *   - if idle at end-of-tx+PIFS, report missedAck
   */
  void EnableSuperFastAck (void);
  /* If RTS is enabled, we wait CTSTimeout for a CTS.
   * Otherwise, no RTS is sent.
   */
  void EnableRts (void);
  /* If NextData is enabled, we add the transmission duration
   * of the nextData to the durationId and we notify the
   * transmissionListener at the end of the current
   * transmission + SIFS.
   */
  void EnableNextData (uint32_t size);
  
  /* If we enable this, we ignore all other durationId 
   * calculation and simply force the packet's durationId
   * field to this value.
   */
  void EnableOverrideDurationId (Time durationId);
  
  void DisableAck (void);
  void DisableRts (void);
  void DisableNextData (void);
  void DisableOverrideDurationId (void);

  bool MustWaitAck (void) const;
  bool MustWaitNormalAck (void) const;
  bool MustWaitFastAck (void) const;
  bool MustWaitSuperFastAck (void) const;
  bool MustSendRts (void) const;
  bool HasDurationId (void) const;
  Time GetDurationId (void) const;
  bool HasNextPacket (void) const;
  uint32_t GetNextPacketSize (void) const;

private:
  uint32_t m_nextSize;
  enum {
    ACK_NONE,
    ACK_NORMAL,
    ACK_FAST,
    ACK_SUPER_FAST
  } m_waitAck;
  bool m_sendRts;
  Time m_overrideDurationId;
};


class MacLow {
public:
  typedef Callback<void, Packet , WifiMacHeader const*> MacLowRxCallback;

  MacLow ();
  ~MacLow ();

  void SetDevice (Ptr<WifiNetDevice> device);
  void SetPhy (WifiPhy *phy);
  void SetStations (MacStations *stations);
  void SetParameters (MacParameters *parameters);
  void SetRxCallback (MacLowRxCallback callback);
  void RegisterNavListener (MacLowNavListener *listener);

  /* This transmission time includes the time required for
   * the next packet transmission if one was selected.
   */
  Time CalculateTransmissionTime (uint32_t payloadSize,
                                  Mac48Address to,
                                  MacLowTransmissionParameters const&parameters) const;

  /* start the transmission of the currently-stored data. */
  void StartTransmission (Packet packet, 
                          WifiMacHeader const*hdr, 
                          MacLowTransmissionParameters parameters,
                          MacLowTransmissionListener *listener);

  void ReceiveOk (Packet packet, double rxSnr, WifiMode txMode, WifiPreamble preamble);
  void ReceiveError (Packet packet, double rxSnr);
private:
  void CancelAllEvents (void);
  uint32_t GetAckSize (void) const;
  uint32_t GetRtsSize (void) const;
  uint32_t GetCtsSize (void) const;
  Time GetSifs (void) const;
  Time GetPifs (void) const;
  Time GetAckTimeout (void) const;
  Time GetCtsTimeout (void) const;
  uint32_t GetCurrentSize (void) const;
  Time NowUs (void) const;
  MacStation *GetStation (Mac48Address to) const;
  void ForwardDown (Packet const packet, WifiMacHeader const *hdr, 
                    WifiMode txMode);
  Time CalculateOverallTxTime (uint32_t size,
                               Mac48Address to,
                               MacLowTransmissionParameters const &params) const;
  WifiMode GetRtsTxMode (Mac48Address to) const;
  WifiMode GetDataTxMode (Mac48Address to, uint32_t size) const;
  WifiMode GetCtsTxModeForRts (Mac48Address to, WifiMode rtsTxMode) const;
  WifiMode GetAckTxModeForData (Mac48Address to, WifiMode dataTxMode) const;
  void NotifyNav (Time at, WifiMacHeader const*hdr);
  bool IsNavZero (Time at);
  void MaybeCancelPrevious (void);
  
  void NormalAckTimeout (void);
  void FastAckTimeout (void);
  void SuperFastAckTimeout (void);
  void FastAckFailedTimeout (void);
  void CtsTimeout (void);
  void SendCtsAfterRts (Mac48Address source, Time duration, WifiMode txMode, double rtsSnr);
  void SendAckAfterData (Mac48Address source, Time duration, WifiMode txMode, double rtsSnr);
  void SendDataAfterCts (Mac48Address source, Time duration, WifiMode txMode);
  void WaitSifsAfterEndTx (void);

  void SendRtsForPacket (void);
  void SendDataPacket (void);
  void SendCurrentTxPacket (void);
  void StartDataTxTimers (void);

  Ptr<WifiNetDevice> m_device;
  WifiPhy *m_phy;
  MacStations *m_stations;
  MacParameters *m_parameters;
  MacLowRxCallback m_rxCallback;
  typedef std::vector<MacLowNavListener *>::const_iterator NavListenersCI;
  typedef std::vector<MacLowNavListener *> NavListeners;
  NavListeners m_navListeners;

  EventId m_normalAckTimeoutEvent;
  EventId m_fastAckTimeoutEvent;
  EventId m_superFastAckTimeoutEvent;
  EventId m_fastAckFailedTimeoutEvent;
  EventId m_ctsTimeoutEvent;
  EventId m_sendCtsEvent;
  EventId m_sendAckEvent;
  EventId m_sendDataEvent;
  EventId m_waitSifsEvent;

  Packet m_currentPacket;
  bool m_hasCurrent;
  WifiMacHeader m_currentHdr;
  MacLowTransmissionParameters m_txParams;
  MacLowTransmissionListener *m_listener;

  Time m_lastNavStart;
  Time m_lastNavDuration;

  CallbackTraceSource<Packet> m_dropError;
};

} // namespace ns3

#endif /* MAC_LOW_H */
