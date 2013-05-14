/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
#ifndef WIFI_REMOTE_STATION_MANAGER_H
#define WIFI_REMOTE_STATION_MANAGER_H

#include <vector>
#include <utility>
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "wifi-mode.h"

namespace ns3 {

struct WifiRemoteStation;
struct WifiRemoteStationState;
class WifiPhy;
class WifiMacHeader;

/**
 * \brief Tid independent remote station statistics
 *
 * Structure is similar to struct sta_info in Linux kernel (see
 * net/mac80211/sta_info.h)
 */
class WifiRemoteStationInfo
{
public:
  WifiRemoteStationInfo ();
  /**
   * \brief Updates average frame error rate when data or RTS
   * was transmitted successfully.
   * \param retryCounter is slrc or ssrc value at the moment of
   * success transmission.
   */
  void NotifyTxSuccess (uint32_t retryCounter);
  /// Updates average frame error rate when final data or RTS has failed.
  void NotifyTxFailed ();
  /// Returns frame error rate (probability that frame is corrupted due to transmission error).
  double GetFrameErrorRate () const;
private:
  /**
   * \brief Calculate averaging coefficient for frame error rate. Depends on time of the last update.
   * \attention Calling this method twice gives different results,
   * because it resets time of last update.
   */
  double CalculateAveragingCoefficient ();
  /// averaging coefficient depends on the memory time
  Time m_memoryTime;
  /// when last update has occured
  Time m_lastUpdate;
  /// moving percentage of failed frames
  double m_failAvg;
};

/**
 * \ingroup wifi
 * \brief hold a list of per-remote-station state.
 *
 * \sa ns3::WifiRemoteStation.
 */
class WifiRemoteStationManager : public Object
{
public:
  static TypeId GetTypeId (void);

  WifiRemoteStationManager ();
  virtual ~WifiRemoteStationManager ();

  virtual void SetupPhy (Ptr<WifiPhy> phy);

  uint32_t GetMaxSsrc (void) const;
  uint32_t GetMaxSlrc (void) const;
  uint32_t GetRtsCtsThreshold (void) const;
  uint32_t GetFragmentationThreshold (void) const;
  void SetMaxSsrc (uint32_t maxSsrc);
  void SetMaxSlrc (uint32_t maxSlrc);
  void SetRtsCtsThreshold (uint32_t threshold);
  void SetFragmentationThreshold (uint32_t threshold);

  // Invoked in a STA upon dis-association
  // or in an AP upon reboot
  void Reset (void);
  // Invoked in a STA upon association to store
  // the set of rates which belong to the
  // BSSBasicRateSet of the associated AP
  // and which are supported locally.
  // Invoked in an AP to configure the BSSBasicRateSet
  void AddBasicMode (WifiMode mode);

  WifiMode GetDefaultMode (void) const;
  uint32_t GetNBasicModes (void) const;
  WifiMode GetBasicMode (uint32_t i) const;

  WifiMode GetNonUnicastMode (void) const;


  /**
   * Invoked in an AP upon disassociation of a
   * specific STA.
   */
  void Reset (Mac48Address address);
  /**
   * Invoked in a STA or AP to store the set of
   * modes supported by a destination which is
   * also supported locally.
   * The set of supported modes includes
   * the BSSBasicRateSet.
   */
  void AddSupportedMode (Mac48Address address, WifiMode mode);

  bool IsBrandNew (Mac48Address address) const;
  bool IsAssociated (Mac48Address address) const;
  bool IsWaitAssocTxOk (Mac48Address address) const;
  void RecordWaitAssocTxOk (Mac48Address address);
  void RecordGotAssocTxOk (Mac48Address address);
  void RecordGotAssocTxFailed (Mac48Address address);
  void RecordDisassociated (Mac48Address address);

  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to queue
   * \param fullPacketSize the size of the packet after its 802.11 MAC header has been added.
   *
   * This method is typically invoked just before queuing a packet for transmission.
   * It is a no-op unless the IsLowLatency attribute of the attached ns3::WifiRemoteStationManager
   * is set to false, in which case, the tx parameters of the packet are calculated and stored in
   * the packet as a tag. These tx parameters are later retrieved from GetDadaMode and GetRtsMode.
   */
  void PrepareForQueue (Mac48Address address, const WifiMacHeader *header,
                        Ptr<const Packet> packet, uint32_t fullPacketSize);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \param fullPacketSize the size of the packet after its 802.11 MAC header has been added.
   * \returns the transmission mode to use to send this packet
   */
  WifiMode GetDataMode (Mac48Address address, const WifiMacHeader *header,
                        Ptr<const Packet> packet, uint32_t fullPacketSize);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \returns the transmission mode to use to send the RTS prior to the
   *          transmission of the data packet itself.
   */
  WifiMode GetRtsMode (Mac48Address address, const WifiMacHeader *header,
                       Ptr<const Packet> packet);
  /**
   * Should be invoked whenever the RtsTimeout associated to a transmission
   * attempt expires.
   */
  void ReportRtsFailed (Mac48Address address, const WifiMacHeader *header);
  /**
   * Should be invoked whenever the AckTimeout associated to a transmission
   * attempt expires.
   */
  void ReportDataFailed (Mac48Address address, const WifiMacHeader *header);
  /**
   * Should be invoked whenever we receive the Cts associated to an RTS
   * we just sent.
   */
  void ReportRtsOk (Mac48Address address, const WifiMacHeader *header,
                    double ctsSnr, WifiMode ctsMode, double rtsSnr);
  /**
   * Should be invoked whenever we receive the Ack associated to a data packet
   * we just sent.
   */
  void ReportDataOk (Mac48Address address, const WifiMacHeader *header,
                     double ackSnr, WifiMode ackMode, double dataSnr);
  /**
   * Should be invoked after calling ReportRtsFailed if
   * NeedRtsRetransmission returns false
   */
  void ReportFinalRtsFailed (Mac48Address address, const WifiMacHeader *header);
  /**
   * Should be invoked after calling ReportDataFailed if
   * NeedDataRetransmission returns false
   */
  void ReportFinalDataFailed (Mac48Address address, const WifiMacHeader *header);

  /**
   * \param address remote address
   * \param header MAC header
   * \param rxSnr the snr of the packet received
   * \param txMode the transmission mode used for the packet received.
   *
   * Should be invoked whenever a packet is successfully received.
   */
  void ReportRxOk (Mac48Address address, const WifiMacHeader *header,
                   double rxSnr, WifiMode txMode);

  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \returns true if we want to use an RTS/CTS handshake for this
   *          packet before sending it, false otherwise.
   */
  bool NeedRts (Mac48Address address, const WifiMacHeader *header,
                Ptr<const Packet> packet);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \returns true if we want to restart a failed RTS/CTS
   *          handshake, false otherwise.
   */
  bool NeedRtsRetransmission (Mac48Address address, const WifiMacHeader *header,
                              Ptr<const Packet> packet);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \returns true if we want to resend a packet
   *          after a failed transmission attempt, false otherwise.
   */
  bool NeedDataRetransmission (Mac48Address address, const WifiMacHeader *header,
                               Ptr<const Packet> packet);

  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \returns true if this packet should be fragmented, false otherwise.
   */
  bool NeedFragmentation (Mac48Address address, const WifiMacHeader *header,
                          Ptr<const Packet> packet);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \param fragmentNumber the fragment index of the next fragment to send (starts at zero).
   * \returns the size of the corresponding fragment.
   */
  uint32_t GetFragmentSize (Mac48Address address, const WifiMacHeader *header,
                            Ptr<const Packet> packet, uint32_t fragmentNumber);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \param fragmentNumber the fragment index of the next fragment to send (starts at zero).
   * \returns the offset within the original packet where this fragment starts.
   */
  uint32_t GetFragmentOffset (Mac48Address address, const WifiMacHeader *header,
                              Ptr<const Packet> packet, uint32_t fragmentNumber);
  /**
   * \param address remote address
   * \param header MAC header
   * \param packet the packet to send
   * \param fragmentNumber the fragment index of the next fragment to send (starts at zero).
   * \returns true if this is the last fragment, false otherwise.
   */
  bool IsLastFragment (Mac48Address address, const WifiMacHeader *header,
                       Ptr<const Packet> packet, uint32_t fragmentNumber);

  /**
   * \param address remote address
   * \param rtsMode the transmission mode used to send an RTS we just received
   * \returns the transmission mode to use for the CTS to complete the RTS/CTS
   *          handshake.
   */
  WifiMode GetCtsMode (Mac48Address address, WifiMode rtsMode);
  /**
   * \param address
   * \param dataMode the transmission mode used to send an ACK we just received
   * \returns the transmission mode to use for the ACK to complete the data/ACK
   *          handshake.
   */
  WifiMode GetAckMode (Mac48Address address, WifiMode dataMode);

  WifiRemoteStationInfo GetInfo (Mac48Address address);
protected:
  virtual void DoDispose (void);
  // for convenience
  WifiMode GetSupported (const WifiRemoteStation *station, uint32_t i) const;
  uint32_t GetNSupported (const WifiRemoteStation *station) const;
private:
  /**
   * \param station the station with which we need to communicate
   * \param packet the packet to send
   * \param normally indicates whether the normal 802.11 rts enable mechanism would
   *        request that the rts is sent or not.
   * \returns true if we want to use an RTS/CTS handshake for this
   *          packet before sending it, false otherwise.
   *
   * Note: This method is called before a unicast packet is sent on the medium.
   */
  virtual bool DoNeedRts (WifiRemoteStation *station,
                          Ptr<const Packet> packet, bool normally);
  /**
   * \param station the station with which we need to communicate
   * \param packet the packet to send
   * \param normally indicates whether the normal 802.11 rts enable mechanism would
   *        request that the rts is retransmitted or not.
   * \returns true if we want to restart a failed RTS/CTS
   *          handshake, false otherwise.
   *
   * Note: This method is called after an rts/cts handshake has been attempted
   *       and has failed.
   */
  virtual bool DoNeedRtsRetransmission (WifiRemoteStation *station,
                                        Ptr<const Packet> packet, bool normally);
  /**
   * \param station the station with which we need to communicate
   * \param packet the packet to send
   * \param normally indicates whether the normal 802.11 data retransmission mechanism
   *        would request that the data is retransmitted or not.
   * \returns true if we want to resend a packet
   *          after a failed transmission attempt, false otherwise.
   *
   * Note: This method is called after a unicast packet transmission has been attempted
   *       and has failed.
   */
  virtual bool DoNeedDataRetransmission (WifiRemoteStation *station,
                                         Ptr<const Packet> packet, bool normally);

  /**
   * \param station the station with which we need to communicate
   * \param packet the packet to send
   * \param normally indicates whether the normal 802.11 data fragmentation mechanism
   *        would request that the data packet is fragmented or not.
   * \returns true if this packet should be fragmented, false otherwise.
   *
   * Note: This method is called before sending a unicast packet.
   */
  virtual bool DoNeedFragmentation (WifiRemoteStation *station,
                                    Ptr<const Packet> packet, bool normally);
  /**
   * \returns whether this manager is a manager designed to work in low-latency
   *          environments.
   *
   * Note: In this context, low vs high latency is defined in <i>IEEE 802.11 Rate Adaptation:
   * A Practical Approach</i>, by M. Lacage, M.H. Manshaei, and T. Turletti.
   */
  virtual bool IsLowLatency (void) const = 0;
  /**
   * \return a new station data structure
   */
  virtual WifiRemoteStation* DoCreateStation (void) const = 0;
  /**
   * \param station the station with which we need to communicate
   * \param size size of the packet or fragment we want to send
   * \returns the transmission mode to use to send a packet to the station
   *
   * Note: This method is called before sending a unicast packet or a fragment
   *       of a unicast packet to decide which transmission mode to use.
   */
  virtual WifiMode DoGetDataMode (WifiRemoteStation *station,
                                  uint32_t size) = 0;
  /**
   * \param station the station with which we need to communicate
   * \returns the transmission mode to use to send an rts to the station
   *
   * Note: This method is called before sending an rts to a station
   *       to decide which transmission mode to use for the rts.
   */
  virtual WifiMode DoGetRtsMode (WifiRemoteStation *station) = 0;
  virtual void DoReportRtsFailed (WifiRemoteStation *station) = 0;
  virtual void DoReportDataFailed (WifiRemoteStation *station) = 0;
  virtual void DoReportRtsOk (WifiRemoteStation *station,
                              double ctsSnr, WifiMode ctsMode, double rtsSnr) = 0;
  virtual void DoReportDataOk (WifiRemoteStation *station,
                               double ackSnr, WifiMode ackMode, double dataSnr) = 0;
  virtual void DoReportFinalRtsFailed (WifiRemoteStation *station) = 0;
  virtual void DoReportFinalDataFailed (WifiRemoteStation *station) = 0;
  virtual void DoReportRxOk (WifiRemoteStation *station,
                             double rxSnr, WifiMode txMode) = 0;

  WifiRemoteStationState* LookupState (Mac48Address address) const;
  WifiRemoteStation* Lookup (Mac48Address address, uint8_t tid) const;
  /// Find a remote station by its remote address and TID taken from MAC header
  WifiRemoteStation* Lookup (Mac48Address address, const WifiMacHeader *header) const;
  WifiMode GetControlAnswerMode (Mac48Address address, WifiMode reqMode);

  void DoSetFragmentationThreshold (uint32_t threshold);
  uint32_t DoGetFragmentationThreshold (void) const;
  uint32_t GetNFragments (const WifiMacHeader *header, Ptr<const Packet> packet);

  typedef std::vector <WifiRemoteStation *> Stations;
  typedef std::vector <WifiRemoteStationState *> StationStates;

  StationStates m_states;
  Stations m_stations;
  /**
   * This is a pointer to the WifiPhy associated with this
   * WifiRemoteStationManager that is set on call to
   * WifiRemoteStationManager::SetupPhy(). Through this pointer the
   * station manager can determine PHY characteristics, such as the
   * set of all transmission rates that may be supported (the
   * "DeviceRateSet").
   */
  Ptr<WifiPhy> m_wifiPhy;
  WifiMode m_defaultTxMode;

  /**
   * This member is the list of WifiMode objects that comprise the
   * BSSBasicRateSet parameter. This list is constructed through calls
   * to WifiRemoteStationManager::AddBasicMode(), and an API that
   * allows external access to it is available through
   * WifiRemoteStationManager::GetNBasicModes() and
   * WifiRemoteStationManager::GetBasicMode().
   */
  WifiModeList m_bssBasicRateSet;

  bool m_isLowLatency;
  uint32_t m_maxSsrc;
  uint32_t m_maxSlrc;
  uint32_t m_rtsCtsThreshold;
  uint32_t m_fragmentationThreshold;
  WifiMode m_nonUnicastMode;
  double m_avgSlrcCoefficient;
  /**
   * The trace source fired when the transmission of a single RTS has failed
   */
  TracedCallback<Mac48Address> m_macTxRtsFailed;
  /**
   * The trace source fired when the transmission of a single data packet has failed
   */
  TracedCallback<Mac48Address> m_macTxDataFailed;
  /**
   * The trace source fired when the transmission of a RTS has
   * exceeded the maximum number of attempts
   */
  TracedCallback<Mac48Address> m_macTxFinalRtsFailed;
  /**
   * The trace source fired when the transmission of a data packet has
   * exceeded the maximum number of attempts
   */
  TracedCallback<Mac48Address> m_macTxFinalDataFailed;

};

struct WifiRemoteStationState
{
  enum
  {
    BRAND_NEW,
    DISASSOC,
    WAIT_ASSOC_TX_OK,
    GOT_ASSOC_TX_OK
  } m_state;

  /**
   * This member is the list of WifiMode objects that comprise the
   * OperationalRateSet parameter for this remote station. This list
   * is constructed through calls to
   * WifiRemoteStationManager::AddSupportedMode(), and an API that
   * allows external access to it is available through
   * WifiRemoteStationManager::GetNSupported() and
   * WifiRemoteStationManager::GetSupported().
   */
  WifiModeList m_operationalRateSet;

  Mac48Address m_address;
  WifiRemoteStationInfo m_info;
};

/**
 * \brief hold per-remote-station state.
 *
 * The state in this class is used to keep track
 * of association status if we are in an infrastructure
 * network and to perform the selection of tx parameters
 * on a per-packet basis.
 */
struct WifiRemoteStation
{
  WifiRemoteStationState *m_state;
  uint32_t m_ssrc;
  uint32_t m_slrc;
  uint8_t m_tid;
};


} // namespace ns3

#endif /* WIFI_REMOTE_STATION_MANAGER_H */
