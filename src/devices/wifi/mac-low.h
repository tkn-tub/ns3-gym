/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2006 INRIA
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
#include <ostream>

#include "wifi-mac-header.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-remote-station-manager.h"
#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"

namespace ns3 {

class WifiPhy;
class WifiMac;

/**
 * \brief listen to events coming from ns3::MacLow.
 */
class MacLowTransmissionListener {
public:
  MacLowTransmissionListener ();
  virtual ~MacLowTransmissionListener ();

  /**
   * \param snr the snr of the cts
   * \param txMode the txMode of the cts
   *
   * ns3::MacLow received an expected CTS within
   * CtsTimeout.
   */
  virtual void GotCts (double snr, WifiMode txMode) = 0;
  /**
   * ns3::MacLow did not receive an expected CTS
   * within CtsTimeout.
   */
  virtual void MissedCts (void) = 0;
  /**
   * \param snr the snr of the ack
   * \param txMode the transmission mode of the ack
   *
   * ns3::MacLow received an expected ACL within
   * AckTimeout. The <i>snr</i> and <i>txMode</i>
   * arguments are not valid when SUPER_FAST_ACK is 
   * used.
   */
  virtual void GotAck (double snr, WifiMode txMode) = 0;
  /**
   * ns3::MacLow did not receive an expected ACK within
   * AckTimeout.
   */
  virtual void MissedAck (void) = 0;
  /**
   * Invoked when ns3::MacLow wants to start a new transmission
   * as configured by MacLowTransmissionParameters::EnableNextData.
   * The listener is expected to call again MacLow::StartTransmission
   * with the "next" data to send.
   */
  virtual void StartNext (void) = 0;

  /**
   * Invoked if this transmission was canceled 
   * one way or another. When this method is invoked,
   * you can assume that the packet has not been passed
   * down the stack to the PHY.
   */
  virtual void Cancel (void) = 0;
};


/**
 * \brief listen to NAV events
 *
 * This class is typically connected to an instance of ns3::Dcf
 * and calls to its methods are forwards to the corresponding
 * ns3::Dcf methods.
 */
class MacLowNavListener {
public:
  MacLowNavListener ();
  virtual ~MacLowNavListener ();
  /**
   * \param duration duration of NAV timer
   */
  virtual void NavStart (Time duration) = 0;
  /**
   * \param duration duration of NAV timer
   */
  virtual void NavReset (Time duration) = 0;
};

/**
 * \brief control how a packet is transmitted.
 *
 * The ns3::MacLow::StartTransmission method expects
 * an instance of this class to describe how the packet
 * should be transmitted.
 */
class MacLowTransmissionParameters {
public:
  MacLowTransmissionParameters ();
    
  /**
   * Wait ACKTimeout for an ACK. If we get an ACK
   * on time, call MacLowTransmissionListener::GotAck.
   * Call MacLowTransmissionListener::MissedAck otherwise.
   */
  void EnableAck (void);
  /**
   *   - wait PIFS after end-of-tx. If idle, call
   *     MacLowTransmissionListener::MissedAck.
   *   - if busy at end-of-tx+PIFS, wait end-of-rx
   *   - if Ack ok at end-of-rx, call 
   *     MacLowTransmissionListener::GotAck.
   *   - if Ack not ok at end-of-rx, report call 
   *     MacLowTransmissionListener::MissedAck
   *     at end-of-rx+SIFS.
   *
   * This is really complicated but it is needed for
   * proper HCCA support.
   */
  void EnableFastAck (void);
  /** 
   *  - if busy at end-of-tx+PIFS, call 
   *    MacLowTransmissionListener::GotAck
   *  - if idle at end-of-tx+PIFS, call
   *    MacLowTransmissionListener::MissedAck
   */
  void EnableSuperFastAck (void);
  /**
   * Send a RTS, and wait CTSTimeout for a CTS. If we get a 
   * CTS on time, call MacLowTransmissionListener::GotCts
   * and send data. Otherwise, call MacLowTransmissionListener::MissedCts
   * and do not send data.
   */
  void EnableRts (void);
  /**
   * \param size size of next data to send after current packet is
   *        sent.
   *
   * Add the transmission duration of the next data to the 
   * durationId of the outgoing packet and call 
   * MacLowTransmissionListener::StartNext at the end of 
   * the current transmission + SIFS.
   */
  void EnableNextData (uint32_t size);
  
  /**
   * \param durationId the value to set in the duration/Id field of
   *        the outgoing packet.
   *
   * Ignore all other durationId calculation and simply force the 
   * packet's durationId field to this value.
   */
  void EnableOverrideDurationId (Time durationId);
  
  /**
   * Do not wait for Ack after data transmission. Typically
   * used for Broadcast and multicast frames.
   */
  void DisableAck (void);
  /**
   * Do not send rts and wait for cts before sending data.
   */
  void DisableRts (void);
  /**
   * Do not attempt to send data burst after current transmission
   */
  void DisableNextData (void);
  /**
   * Do not force the duration/id field of the packet: its
   * value is automatically calculated by the MacLow before
   * calling WifiPhy::Send.
   */
  void DisableOverrideDurationId (void);

  /**
   * \returns true if must wait for ACK after data transmission,
   *          false otherwise.
   *
   * This methods returns true when any of MustWaitNormalAck,
   * MustWaitFastAck, or MustWaitSuperFastAck return true.
   */
  bool MustWaitAck (void) const;
  /**
   * \returns true if normal ACK protocol should be used, false
   *          otherwise.
   *
   * \sa EnableAck
   */
  bool MustWaitNormalAck (void) const;
  /**
   * \returns true if fast ack protocol should be used, false 
   *          otherwise.
   *
   * \sa EnableFastAck
   */
  bool MustWaitFastAck (void) const;
  /**
   * \returns true if super fast ack protocol should be used, false 
   *          otherwise.
   *
   * \sa EnableSuperFastAck
   */
  bool MustWaitSuperFastAck (void) const;
  /**
   * \returns true if RTS should be sent and CTS waited for before 
   *          sending data, false otherwise.
   */
  bool MustSendRts (void) const;
  /**
   * \returns true if a duration/id was forced with 
   *         EnableOverrideDurationId, false otherwise.
   */
  bool HasDurationId (void) const;
  /**
   * \returns the duration/id forced by EnableOverrideDurationId
   */
  Time GetDurationId (void) const;
  /**
   * \returns true if EnableNextData was called, false otherwise.
   */
  bool HasNextPacket (void) const;
  /**
   * \returns the size specified by EnableNextData.
   */
  uint32_t GetNextPacketSize (void) const;

private:
  friend std::ostream &operator << (std::ostream &os, const MacLowTransmissionParameters &params);
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

std::ostream &operator << (std::ostream &os, const MacLowTransmissionParameters &params);


/**
 * \brief handle RTS/CTS/DATA/ACK transactions.
 */
class MacLow : public Object {
public:
  typedef Callback<void, Ptr<Packet> , WifiMacHeader const*> MacLowRxCallback;

  MacLow ();
  virtual ~MacLow ();

  void SetPhy (Ptr<WifiPhy> phy);
  void SetMac (Ptr<WifiMac> mac);
  void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> manager);

  Ptr<WifiMac> GetMac (void);

  /**
   * \param callback the callback which receives every incoming packet.
   *
   * This callback typically forwards incoming packets to
   * an instance of ns3::MacRxMiddle.
   */
  void SetRxCallback (Callback<void,Ptr<Packet>,const WifiMacHeader *> callback);
  /**
   * \param listener listen to NAV events for every incoming
   *        and outgoing packet.
   */
  void RegisterNavListener (MacLowNavListener *listener);

  /**
   * \param packet to send (does not include the 802.11 MAC header and checksum)
   * \param hdr header associated to the packet to send.
   * \param parameters transmission parameters of packet.
   *
   * This transmission time includes the time required for
   * the next packet transmission if one was selected.
   */
  Time CalculateTransmissionTime (Ptr<const Packet> packet,
                                  WifiMacHeader const*hdr, 
                                  MacLowTransmissionParameters const&parameters) const;

  /**
   * \param packet packet to send
   * \param hdr 802.11 header for packet to send
   * \param parameters the transmission parameters to use for this packet.
   * \param listener listen to transmission events.
   *
   * Start the transmission of the input packet and notify the listener
   * of transmission events.
   */
  void StartTransmission (Ptr<const Packet> packet, 
                          WifiMacHeader const*hdr, 
                          MacLowTransmissionParameters parameters,
                          MacLowTransmissionListener *listener);

  /**
   * \param packet packet received
   * \param rxSnr snr of packet received
   * \param txMode transmission mode of packet received
   * \param preamble type of preamble used for the packet received
   *
   * This method is typically invoked by the lower PHY layer to notify
   * the MAC layer that a packet was successfully received.
   */
  void ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiMode txMode, WifiPreamble preamble);
  /**
   * \param packet packet received.
   * \param rxSnr snr of packet received.
   *
   * This method is typically invoked by the lower PHY layer to notify
   * the MAC layer that a packet was unsuccessfully received.
   */
  void ReceiveError (Ptr<Packet> packet, double rxSnr);
private:
  void CancelAllEvents (void);
  uint32_t GetAckSize (void) const;
  uint32_t GetRtsSize (void) const;
  uint32_t GetCtsSize (void) const;
  Time GetSifs (void) const;
  Time GetPifs (void) const;
  Time GetSlotTime (void) const;
  Time GetAckTimeout (void) const;
  Time GetCtsTimeout (void) const;
  uint32_t GetSize (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
  Time NowUs (void) const;
  WifiRemoteStation *GetStation (Mac48Address to) const;
  void ForwardDown (Ptr<const Packet> packet, WifiMacHeader const *hdr, 
                    WifiMode txMode);
  Time CalculateOverallTxTime (Ptr<const Packet> packet,
                               WifiMacHeader const*hdr, 
                               MacLowTransmissionParameters const &params) const;
  WifiMode GetRtsTxMode (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
  WifiMode GetDataTxMode (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
  WifiMode GetCtsTxModeForRts (Mac48Address to, WifiMode rtsTxMode) const;
  WifiMode GetAckTxModeForData (Mac48Address to, WifiMode dataTxMode) const;
  Time GetCtsDuration (Mac48Address to, WifiMode rtsTxMode) const;
  Time GetAckDuration (Mac48Address to, WifiMode dataTxMode) const;
  void NotifyNav (const WifiMacHeader &hdr, WifiMode txMode, WifiPreamble preamble);
  void DoNavResetNow (Time duration);
  bool DoNavStartNow (Time duration);
  bool IsNavZero (void) const;
  void MaybeCancelPrevious (void);
  
  void NavCounterResetCtsMissed (Time rtsEndRxTime);
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
  virtual void DoDispose (void);

  Ptr<WifiPhy> m_phy;
  Ptr<WifiMac> m_mac;
  Ptr<WifiRemoteStationManager> m_stationManager;
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
  EventId m_navCounterResetCtsMissed;

  Ptr<Packet> m_currentPacket;
  WifiMacHeader m_currentHdr;
  MacLowTransmissionParameters m_txParams;
  MacLowTransmissionListener *m_listener;

  Time m_lastNavStart;
  Time m_lastNavDuration;
};

} // namespace ns3

#endif /* MAC_LOW_H */
