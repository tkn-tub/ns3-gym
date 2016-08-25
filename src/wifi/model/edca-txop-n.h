/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef EDCA_TXOP_N_H
#define EDCA_TXOP_N_H

#include "ns3/object.h"
#include "ns3/mac48-address.h"
#include "ns3/packet.h"
#include "ns3/traced-value.h"
#include "wifi-mode.h"
#include "wifi-mac-header.h"
#include "wifi-remote-station-manager.h"
#include "qos-utils.h"
#include "dcf.h"
#include "ctrl-headers.h"
#include "block-ack-manager.h"
#include <map>
#include <list>

class AmpduAggregationTest;

namespace ns3 {

class DcfState;
class DcfManager;
class MacLow;
class MacTxMiddle;
class WifiMac;
class WifiMacParameters;
class WifiMacQueue;
class RandomStream;
class QosBlockedDestinations;
class MsduAggregator;
class MpduAggregator;
class MgtAddBaResponseHeader;
class BlockAckManager;
class MgtDelBaHeader;

/**
 * Enumeration for type of station
 */
enum TypeOfStation
{
  STA,
  AP,
  ADHOC_STA,
  MESH,
  HT_STA,
  HT_AP,
  HT_ADHOC_STA,
  OCB
};


/**
 * \ingroup wifi
 * This queue contains packets for a particular access class.
 * possibles access classes are:
 *
 *   -AC_VO : voice, tid = 6,7         ^
 *   -AC_VI : video, tid = 4,5         |
 *   -AC_BE : best-effort, tid = 0,3   |  priority
 *   -AC_BK : background, tid = 1,2    |
 *
 * For more details see section 9.1.3.1 in 802.11 standard.
 */
class EdcaTxopN : public Dcf
{
public:
  // Allow test cases to access private members
  friend class ::AmpduAggregationTest;

  /**
   * typedef for a callback to invoke when a
   * packet transmission was completed successfully.
   */
  typedef Callback <void, const WifiMacHeader&> TxOk;
  /**
   * typedef for a callback to invoke when a
   * packet transmission was failed.
   */
  typedef Callback <void, const WifiMacHeader&> TxFailed;
  
  std::map<Mac48Address, bool> m_aMpduEnabled;

  static TypeId GetTypeId (void);
  EdcaTxopN ();
  virtual ~EdcaTxopN ();
  void DoDispose ();

  /**
   * Set MacLow associated with this EdcaTxopN.
   *
   * \param low MacLow
   */
  void SetLow (Ptr<MacLow> low);
  void SetTxMiddle (MacTxMiddle *txMiddle);
  /**
   * Set DcfManager this EdcaTxopN is associated to.
   *
   * \param manager DcfManager
   */
  void SetManager (DcfManager *manager);
  /**
   * \param callback the callback to invoke when a
   * packet transmission was completed successfully.
   */
  void SetTxOkCallback (TxOk callback);
  /**
   * \param callback the callback to invoke when a
   * packet transmission was completed unsuccessfully.
   */
  void SetTxFailedCallback (TxFailed callback);
  /**
   * Set WifiRemoteStationsManager this EdcaTxopN is associated to.
   *
   * \param remoteManager WifiRemoteStationManager
   */
  void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> remoteManager);
  /**
   * Set type of station with the given type.
   *
   * \param type
   */
  void SetTypeOfStation (enum TypeOfStation type);
  /**
   * Return type of station.
   *
   * \return type of station
   */
  enum TypeOfStation GetTypeOfStation (void) const;
  /**
   * Return the packet queue associated with this EdcaTxopN.
   *
   * \return WifiMacQueue
   */
  Ptr<WifiMacQueue > GetEdcaQueue () const;

  virtual void SetMinCw (uint32_t minCw);
  virtual void SetMaxCw (uint32_t maxCw);
  virtual void SetAifsn (uint32_t aifsn);
  virtual void SetTxopLimit (Time txopLimit);
  virtual uint32_t GetMinCw (void) const;
  virtual uint32_t GetMaxCw (void) const;
  virtual uint32_t GetAifsn (void) const;
  virtual Time GetTxopLimit (void) const;

  /**
   * Return the MacLow associated with this EdcaTxopN.
   *
   * \return MacLow
   */
  Ptr<MacLow> Low (void);

  Ptr<MsduAggregator> GetMsduAggregator (void) const;
  Ptr<MpduAggregator> GetMpduAggregator (void) const;

  /**
   * \param recipient address of the peer station
   * \param tid traffic ID.
   * \return true if a block ack agreement exists, false otherwise
   *
   * Checks if a block ack agreement exists with station addressed by
   * <i>recipient</i> for tid <i>tid</i>.
   */
  bool GetBaAgreementExists (Mac48Address address, uint8_t tid) const;
  /**
   * \param recipient address of peer station involved in block ack mechanism.
   * \param tid traffic ID.
   * \return the number of packets buffered for a specified agreement
   *
   * Returns number of packets buffered for a specified agreement.
   */
  uint32_t GetNOutstandingPacketsInBa (Mac48Address address, uint8_t tid);
  /**
   * \param recipient address of peer station involved in block ack mechanism.
   * \param tid traffic ID.
   * \return the number of packets for a specific agreement that need retransmission
   *
   * Returns number of packets for a specific agreement that need retransmission.
   */
  uint32_t GetNRetryNeededPackets (Mac48Address recipient, uint8_t tid) const;
  /**
   * \param recipient address of peer station involved in block ack mechanism.
   * \param tid Ttraffic ID of transmitted packet.
   *
   * This function resets the status of OriginatorBlockAckAgreement after the transfer
   * of an A-MPDU with ImmediateBlockAck policy (i.e. no BAR is scheduled)
   */
  void CompleteAmpduTransfer (Mac48Address recipient, uint8_t tid);

  /* dcf notifications forwarded here */
  /**
   * Check if the EDCAF requires access.
   *
   * \return true if the EDCAF requires access,
   *         false otherwise
   */
  bool NeedsAccess (void) const;
  /**
   * Notify the EDCAF that access has been granted.
   */
  void NotifyAccessGranted (void);
  /**
   * Notify the EDCAF that internal collision has occurred.
   */
  void NotifyInternalCollision (void);
  /**
   * Notify the EDCAF that collision has occurred.
   */
  void NotifyCollision (void);
  /**
   * When a channel switching occurs, enqueued packets are removed.
   */
  void NotifyChannelSwitching (void);
  /**
   * When sleep operation occurs, re-insert pending packet into front of the queue
   */
  void NotifySleep (void);
  /**
   * When wake up operation occurs, restart channel access
   */
  void NotifyWakeUp (void);

  /* Event handlers */
  /**
   * Event handler when a CTS is received.
   *
   * \param snr
   * \param txMode
   */
  void GotCts (double snr, WifiMode txMode);
  /**
   * Event handler when a CTS timeout has occurred.
   */
  void MissedCts (void);
  /**
   * Event handler when an ACK is received.
   *
   * \param snr
   * \param txMode
   */
  void GotAck (double snr, WifiMode txMode);
  /**
   * Event handler when a Block ACK is received.
   *
   * \param blockAck
   * \param recipient
   * \param rxSnr SNR of the block ack itself
   * \param txMode
   * \param dataSnr reported data SNR from the peer
   */
  void GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr);
  /**
   * Event handler when a Block ACK timeout has occurred.
   */
  void MissedBlockAck (uint32_t nMpdus);
  void GotAddBaResponse (const MgtAddBaResponseHeader *respHdr, Mac48Address recipient);
  void GotDelBaFrame (const MgtDelBaHeader *delBaHdr, Mac48Address recipient);
  /**
   * Event handler when an ACK is missed.
   */
  void MissedAck (void);
  /**
   * Start transmission for the next fragment.
   * This is called for fragment only.
   */
  void StartNextFragment (void);
  /**
   * Start transmission for the next packet if allowed by the TxopLimit.
   */
  void StartNext (void);
  /**
   * Cancel the transmission.
   */
  void Cancel (void);
  /**
   * Event handler when a transmission that
   * does not require an ACK has completed.
   */
  void EndTxNoAck (void);
  /**
   * Restart access request if needed.
   */
  void RestartAccessIfNeeded (void);
  /**
   * Request access from DCF manager if needed.
   */
  void StartAccessIfNeeded (void);
  /**
   * Check if RTS should be re-transmitted if CTS was missed.
   *
   * \param packet current packet being transmitted
   * \param hdr current header being transmitted
   * \return true if RTS should be re-transmitted,
   *         false otherwise
   */
  bool NeedRtsRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  /**
   * Check if DATA should be re-transmitted if ACK was missed.
   *
   * \param packet current packet being transmitted
   * \param hdr current header being transmitted
   * \return true if DATA should be re-transmitted,
   *         false otherwise
   */
  bool NeedDataRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  /**
   * Check if Block ACK Request should be re-transmitted.
   *
   * \return true if BAR should be re-transmitted,
   *         false otherwise
   */
  bool NeedBarRetransmission (void);
  /**
   * Check if the current packet should be fragmented.
   *
   * \return true if the current packet should be fragmented,
   *         false otherwise
   */
  bool NeedFragmentation (void) const;
  /**
   * Calculate the size of the next fragment.
   *
   * \return the size of the next fragment
   */
  uint32_t GetNextFragmentSize (void);
  /**
   * Calculate the size of the current fragment.
   *
   * \return the size of the current fragment
   */
  uint32_t GetFragmentSize (void);
  /**
   * Calculate the offset for the current fragment.
   *
   * \return the offset for the current fragment
   */
  uint32_t GetFragmentOffset (void);
  /**
   * Check if the current fragment is the last fragment.
   *
   * \return true if the current fragment is the last fragment,
   *         false otherwise
   */
  bool IsLastFragment (void) const;
  /**
   * Continue to the next fragment. This method simply
   * increments the internal variable that keep track
   * of the current fragment number.
   */
  void NextFragment (void);
  /**
   * Get the next fragment from the packet with
   * appropriate Wifi header for the fragment.
   *
   * \param hdr
   * \return the fragment with the current fragment number
   */
  Ptr<Packet> GetFragmentPacket (WifiMacHeader *hdr);

  /**
   * Set the access category of this EDCAF.
   *
   * \param ac
   */
  void SetAccessCategory (enum AcIndex ac);

  /**
   * \param packet packet to send
   * \param hdr header of packet to send.
   *
   * Store the packet in the internal queue until it
   * can be sent safely.
   */
  void Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr);

  void SetMsduAggregator (Ptr<MsduAggregator> aggr);
  void SetMpduAggregator (Ptr<MpduAggregator> aggr);

  /**
   * \param packet packet to send
   * \param hdr header of packet to send.
   *
   * Store the packet in the front of the internal queue until it
   * can be sent safely.
   */
  void PushFront (Ptr<const Packet> packet, const WifiMacHeader &hdr);

  /**
   * Complete block ACK configuration.
   */
  void CompleteConfig (void);

  /**
   * Set threshold for block ACK mechanism. If number of packets in the
   * queue reaches the threshold, block ACK mechanism is used.
   *
   * \param threshold
   */
  void SetBlockAckThreshold (uint8_t threshold);
  /**
   * Return the current threshold for block ACK mechanism.
   *
   * \return the current threshold for block ACK mechanism
   */
  uint8_t GetBlockAckThreshold (void) const;

  void SetBlockAckInactivityTimeout (uint16_t timeout);
  void SendDelbaFrame (Mac48Address addr, uint8_t tid, bool byOriginator);
  void CompleteMpduTx (Ptr<const Packet> packet, WifiMacHeader hdr, Time tstamp);
  bool GetAmpduExist (Mac48Address dest) const;
  void SetAmpduExist (Mac48Address dest, bool enableAmpdu);

  /**
   * Return the next sequence number for the given header.
   *
   * \param hdr Wi-Fi header
   *
   * \return the next sequence number
   */
  uint16_t GetNextSequenceNumberfor (WifiMacHeader *hdr);
  /**
   * Return the next sequence number for the Traffic ID and destination, but do not pick it (i.e. the current sequence number remains unchanged).
   *
   * \param hdr Wi-Fi header
   *
   * \return the next sequence number
   */
  uint16_t PeekNextSequenceNumberfor (WifiMacHeader *hdr);
  /**
   * Remove a packet after you peek in the retransmit queue and get it
   */
  void RemoveRetransmitPacket (uint8_t tid, Mac48Address recipient, uint16_t seqnumber);
  /*
   * Peek in retransmit queue and get the next packet without removing it from the queue
   */
  Ptr<const Packet> PeekNextRetransmitPacket (WifiMacHeader &header, Mac48Address recipient, uint8_t tid, Time *timestamp);
  /**
   * The packet we sent was successfully received by the receiver
   *
   * \param hdr the header of the packet that we successfully sent
   */
  void BaTxOk (const WifiMacHeader &hdr);
  /**
   * The packet we sent was successfully received by the receiver
   *
   * \param hdr the header of the packet that we failed to sent
   */
  void BaTxFailed (const WifiMacHeader &hdr);

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);


private:
  void DoInitialize ();
  /**
   * This functions are used only to correctly set addresses in a-msdu subframe.
   * If aggregating sta is a STA (in an infrastructured network):
   *   SA = Address2
   *   DA = Address3
   * If aggregating sta is an AP
   *   SA = Address3
   *   DA = Address1
   *
   * \param hdr
   * \return Mac48Address
   */
  Mac48Address MapSrcAddressForAggregation (const WifiMacHeader &hdr);
  Mac48Address MapDestAddressForAggregation (const WifiMacHeader &hdr);
  EdcaTxopN &operator = (const EdcaTxopN &);
  EdcaTxopN (const EdcaTxopN &);

  /**
   * If number of packets in the queue reaches m_blockAckThreshold value, an ADDBA Request frame
   * is sent to destination in order to setup a block ack.
   *
   * \return true if we tried to set up block ACK, false otherwise
   */
  bool SetupBlockAckIfNeeded ();
  /**
   * Sends an ADDBA Request to establish a block ack agreement with sta
   * addressed by <i>recipient</i> for tid <i>tid</i>.
   *
   * \param recipient
   * \param tid
   * \param startSeq
   * \param timeout
   * \param immediateBAck
   */
  void SendAddBaRequest (Mac48Address recipient, uint8_t tid, uint16_t startSeq,
                         uint16_t timeout, bool immediateBAck);
  /**
   * After that all packets, for which a block ack agreement was established, have been
   * transmitted, we have to send a block ack request.
   *
   * \param bar
   */
  void SendBlockAckRequest (const struct Bar &bar);
  /**
   * For now is typically invoked to complete transmission of a packets sent with ack policy
   * Block Ack: the packet is buffered and dcf is reset.
   */
  void CompleteTx (void);
  /**
   * Verifies if dequeued packet has to be transmitted with ack policy Block Ack. This happens
   * if an established block ack agreement exists with the receiver.
   */
  void VerifyBlockAck (void);
  /**
   * Get Traffic ID of the current packet.
   */
  uint8_t GetCurrentTid () const;
  /*
   * Return the remaining duration in the current TXOP.
   *
   * \return the remaining duration in the current TXOP
   */
  Time GetTxopRemaining (void);
  /*
   * Check if the station has TXOP granted for the next MPDU.
   *
   * \return true if the station has TXOP granted for the next MPDU,
   *         false otherwise
   */
  bool HasTxop (void);

  AcIndex m_ac;
  class Dcf;
  class TransmissionListener;
  class AggregationCapableTransmissionListener;
  friend class Dcf;
  friend class TransmissionListener;
  Dcf *m_dcf;
  DcfManager *m_manager;
  Ptr<WifiMacQueue> m_queue;
  TxOk m_txOkCallback;
  TxFailed m_txFailedCallback;
  Ptr<MacLow> m_low;
  MacTxMiddle *m_txMiddle;
  TransmissionListener *m_transmissionListener;
  AggregationCapableTransmissionListener *m_blockAckListener;
  RandomStream *m_rng;
  Ptr<WifiRemoteStationManager> m_stationManager;
  uint8_t m_fragmentNumber;

  /* current packet could be a simple MSDU or, if an aggregator for this queue is
     present, could be an A-MSDU.
   */
  Ptr<const Packet> m_currentPacket;

  WifiMacHeader m_currentHdr;
  Ptr<MsduAggregator> m_msduAggregator;
  Ptr<MpduAggregator> m_mpduAggregator;
  TypeOfStation m_typeOfStation;
  QosBlockedDestinations *m_qosBlockedDestinations;
  BlockAckManager *m_baManager;
  /*
   * Represents the minimum number of packets for use of block ack.
   */
  uint8_t m_blockAckThreshold;
  enum BlockAckType m_blockAckType;
  Time m_currentPacketTimestamp;
  uint16_t m_blockAckInactivityTimeout;
  struct Bar m_currentBar;
  Time m_startTxop;
  bool m_isAccessRequestedForRts;
  TracedValue<uint32_t> m_backoffTrace;
  TracedValue<uint32_t> m_cwTrace;
};

} //namespace ns3

#endif /* EDCA_TXOP_N_H */
