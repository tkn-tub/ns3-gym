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

#ifndef QOS_TXOP_H
#define QOS_TXOP_H

#include "ns3/traced-value.h"
#include "block-ack-manager.h"
#include "txop.h"
#include "qos-utils.h"

class AmpduAggregationTest;

namespace ns3 {

class QosBlockedDestinations;
class MsduAggregator;
class MpduAggregator;
class MgtAddBaResponseHeader;
class MgtDelBaHeader;
class AggregationCapableTransmissionListener;

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
 * \brief Handle packet fragmentation and retransmissions for QoS data frames as well
 * as MSDU aggregation (A-MSDU) and block ack sessions, for a given access class.
 * \ingroup wifi
 *
 * This class implements the packet fragmentation and retransmission policy for
 * QoS data frames. It uses the ns3::MacLow and ns3::ChannelAccessManager helper classes
 * to respectively send packets and decide when to send them. Packets are stored
 * in a ns3::WifiMacQueue until they can be sent.
 *
 * This queue contains packets for a particular access class.
 * Possibles access classes are:
 *   - AC_VO : voice, tid = 6,7
 *   - AC_VI : video, tid = 4,5
 *   - AC_BE : best-effort, tid = 0,3
 *   - AC_BK : background, tid = 1,2
 *
 * This class also implements block ack sessions and MSDU aggregation (A-MSDU).
 * If A-MSDU is enabled for that access class, it picks several packets from the
 * queue instead of a single one and sends the aggregated packet to ns3::MacLow.
 *
 * The fragmentation policy currently implemented uses a simple
 * threshold: any packet bigger than this threshold is fragmented
 * in fragments whose size is smaller than the threshold.
 *
 * The retransmission policy is also very simple: every packet is
 * retransmitted until it is either successfully transmitted or
 * it has been retransmitted up until the ssrc or slrc thresholds.
 *
 * The rts/cts policy is similar to the fragmentation policy: when
 * a packet is bigger than a threshold, the rts/cts protocol is used.
 */

class QosTxop : public Txop
{
public:
  /// Allow test cases to access private members
  friend class ::AmpduAggregationTest;

  std::map<Mac48Address, bool> m_aMpduEnabled; //!< list containing flags whether A-MPDU is enabled for a given destination address

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  QosTxop ();
  virtual ~QosTxop ();

  /**
   * Check for QoS TXOP.
   *
   * \returns true if QoS TXOP.
   */
  bool IsQosTxop () const;

  /**
   * Set WifiRemoteStationsManager this QosTxop is associated to.
   *
   * \param remoteManager WifiRemoteStationManager.
   */
  void SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> remoteManager);
  /**
   * Set type of station with the given type.
   *
   * \param type the type of station.
   */
  void SetTypeOfStation (TypeOfStation type);
  /**
   * Return type of station.
   *
   * \return type of station.
   */
  TypeOfStation GetTypeOfStation (void) const;

  /**
   * Returns the aggregator used to construct A-MSDU subframes.
   *
   * \return the aggregator used to construct A-MSDU subframes.
   */
  Ptr<MsduAggregator> GetMsduAggregator (void) const;
  /**
   * Returns the aggregator used to construct A-MPDU subframes.
   *
   * \return the aggregator used to construct A-MPDU subframes.
   */
  Ptr<MpduAggregator> GetMpduAggregator (void) const;

  /**
   * \param address recipient address of the peer station
   * \param tid traffic ID.
   *
   * \return true if a block ack agreement exists, false otherwise.
   *
   * Checks if a block ack agreement exists with station addressed by
   * <i>recipient</i> for tid <i>tid</i>.
   */
  bool GetBaAgreementExists (Mac48Address address, uint8_t tid) const;
  /**
   * \param recipient address of peer station involved in block ack mechanism.
   * \param tid Ttraffic ID of transmitted packet.
   *
   * This function resets the status of OriginatorBlockAckAgreement after the transfer
   * of an A-MPDU with ImmediateBlockAck policy (i.e. no BAR is scheduled).
   */
  void CompleteAmpduTransfer (Mac48Address recipient, uint8_t tid);

  /* dcf notifications forwarded here */
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

  /* Event handlers */
  /**
   * Event handler when a CTS timeout has occurred.
   */
  void MissedCts (void);
  /**
   * Event handler when an ACK is received.
   */
  void GotAck (void);
  /**
   * Event handler when a Block ACK is received.
   *
   * \param blockAck block ack.
   * \param recipient address of the recipient.
   * \param rxSnr SNR of the block ack itself.
   * \param txMode wifi mode.
   * \param dataSnr reported data SNR from the peer.
   */
  void GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr);
  /**
   * Event handler when a Block ACK timeout has occurred.
   * \param nMpdus number of MPDUs sent in the A-MPDU transmission that results in a Block ACK timeout.
   */
  void MissedBlockAck (uint8_t nMpdus);
  /**
   * Event handler when an ADDBA response is received.
   *
   * \param respHdr ADDBA response header.
   * \param recipient address of the recipient.
   */
  void GotAddBaResponse (const MgtAddBaResponseHeader *respHdr, Mac48Address recipient);
  /**
   * Event handler when a DELBA frame is received.
   *
   * \param delBaHdr DELBA header.
   * \param recipient address of the recipient.
   */
  void GotDelBaFrame (const MgtDelBaHeader *delBaHdr, Mac48Address recipient);
  /**
   * Event handler when an ACK is missed.
   */
  void MissedAck (void);

  /**
   * Start transmission for the next packet if allowed by the TxopLimit.
   */
  void StartNextPacket (void);
  /**
   * Event handler when a transmission that does not require an ACK has completed.
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
   * Check if Block ACK Request should be re-transmitted.
   *
   * \return true if BAR should be re-transmitted,
   *         false otherwise.
   */
  bool NeedBarRetransmission (void);

  /**
   * Check if the current packet should be fragmented.
   *
   * \return true if the current packet should be fragmented,
   *         false otherwise.
   */
  bool NeedFragmentation (void) const;

  /**
   * Get the next fragment from the packet with
   * appropriate Wifi header for the fragment.
   *
   * \param hdr Wi-Fi header.
   *
   * \return the fragment with the current fragment number.
   */
  Ptr<Packet> GetFragmentPacket (WifiMacHeader *hdr);

  /**
   * Set the access category of this EDCAF.
   *
   * \param ac access category.
   */
  void SetAccessCategory (AcIndex ac);

  /**
   * Set the aggregator used to construct A-MSDU subframes.
   *
   * \param aggr pointer to the MSDU aggregator.
   */
  void SetMsduAggregator (const Ptr<MsduAggregator> aggr);
  /**
   * Set the aggregator used to construct A-MPDU subframes.
   *
   * \param aggr pointer to the MPDU aggregator.
   */
  void SetMpduAggregator (const Ptr<MpduAggregator> aggr);

  /**
   * \param packet packet to send.
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
   * \param threshold block ack threshold value.
   */
  void SetBlockAckThreshold (uint8_t threshold);
  /**
   * Return the current threshold for block ACK mechanism.
   *
   * \return the current threshold for block ACK mechanism.
   */
  uint8_t GetBlockAckThreshold (void) const;

  /**
   * Set the Block Ack inactivity timeout.
   *
   * \param timeout the Block Ack inactivity timeout.
   */
  void SetBlockAckInactivityTimeout (uint16_t timeout);
  /**
   * Sends DELBA frame to cancel a block ack agreement with sta
   * addressed by <i>addr</i> for tid <i>tid</i>.
   *
   * \param addr address of the recipient.
   * \param tid traffic ID.
   * \param byOriginator flag to indicate whether this is set by the originator.
   */
  void SendDelbaFrame (Mac48Address addr, uint8_t tid, bool byOriginator);
  /**
   * Stores an MPDU (part of an A-MPDU) in blockackagreement (i.e. the sender is waiting
   * for a blockack containing the sequence number of this MPDU).
   * It also calls NotifyMpdu transmission that updates the status of OriginatorBlockAckAgreement.
   *
   * \param packet received packet.
   * \param hdr received Wi-Fi header.
   * \param tstamp timestamp.
   */
  void CompleteMpduTx (Ptr<const Packet> packet, WifiMacHeader hdr, Time tstamp);
  /**
   * Return whether A-MPDU is used to transmit data to a peer station.
   *
   * \param dest address of peer station
   * \returns true if A-MPDU is used by the peer station
   */
  bool GetAmpduExist (Mac48Address dest) const;
  /**
   * Set indication whether A-MPDU is used to transmit data to a peer station.
   *
   * \param dest address of peer station.
   * \param enableAmpdu flag whether A-MPDU is used or not.
   */
  void SetAmpduExist (Mac48Address dest, bool enableAmpdu);

  /**
   * Return the next sequence number for the given header.
   *
   * \param hdr Wi-Fi header.
   *
   * \return the next sequence number.
   */
  uint16_t GetNextSequenceNumberFor (WifiMacHeader *hdr);
  /**
   * Return the next sequence number for the Traffic ID and destination, but do not pick it (i.e. the current sequence number remains unchanged).
   *
   * \param hdr Wi-Fi header.
   *
   * \return the next sequence number.
   */
  uint16_t PeekNextSequenceNumberFor (WifiMacHeader *hdr);
  /**
   * Remove a packet after you peek in the retransmit queue and get it.
   *
   * \param tid traffic ID of the packet to be removed.
   * \param recipient address of the recipient the packet was intended for.
   * \param seqnumber sequence number of the packet to be removed.
   */
  void RemoveRetransmitPacket (uint8_t tid, Mac48Address recipient, uint16_t seqnumber);
  /**
   * Peek in retransmit queue and get the next packet without removing it from the queue.
   *
   * \param header Wi-Fi header.
   * \param tid traffic ID.
   * \param timestamp the timestamp.
   * \returns the packet.
   */
  Ptr<const Packet> PeekNextRetransmitPacket (WifiMacHeader &header, uint8_t tid, Time *timestamp);
  /**
   * The packet we sent was successfully received by the receiver.
   *
   * \param hdr the header of the packet that we successfully sent.
   */
  void BaTxOk (const WifiMacHeader &hdr);
  /**
   * The packet we sent was successfully received by the receiver.
   *
   * \param hdr the header of the packet that we failed to sent.
   */
  void BaTxFailed (const WifiMacHeader &hdr);

  /**
   * This functions are used only to correctly set source address in A-MSDU subframes.
   * If aggregating sta is a STA (in an infrastructured network):
   *   SA = Address2
   * If aggregating sta is an AP
   *   SA = Address3
   *
   * \param hdr Wi-Fi header
   * \return Mac48Address
   */
  Mac48Address MapSrcAddressForAggregation (const WifiMacHeader &hdr);
  /**
   * This functions are used only to correctly set destination address in A-MSDU subframes.
   * If aggregating sta is a STA (in an infrastructured network):
   *   DA = Address3
   * If aggregating sta is an AP
   *   DA = Address1
   *
   * \param hdr Wi-Fi header
   * \return Mac48Address
   */
  Mac48Address MapDestAddressForAggregation (const WifiMacHeader &hdr);


private:
  /// allow AggregationCapableTransmissionListener class access
  friend class AggregationCapableTransmissionListener;

  /**
   * If number of packets in the queue reaches m_blockAckThreshold value, an ADDBA Request frame
   * is sent to destination in order to setup a block ack.
   *
   * \return true if we tried to set up block ACK, false otherwise.
   */
  bool SetupBlockAckIfNeeded ();
  /**
   * Sends an ADDBA Request to establish a block ack agreement with sta
   * addressed by <i>recipient</i> for tid <i>tid</i>.
   *
   * \param recipient address of the recipient.
   * \param tid traffic ID.
   * \param startSeq starting sequence.
   * \param timeout timeout value.
   * \param immediateBAck flag to indicate whether immediate block ack is used.
   */
  void SendAddBaRequest (Mac48Address recipient, uint8_t tid, uint16_t startSeq,
                         uint16_t timeout, bool immediateBAck);
  /**
   * After that all packets, for which a block ack agreement was established, have been
   * transmitted, we have to send a block ack request.
   *
   * \param bar the block ack request.
   */
  void SendBlockAckRequest (const Bar &bar);
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
   * Return the remaining duration in the current TXOP.
   *
   * \return the remaining duration in the current TXOP.
   */
  Time GetTxopRemaining (void) const;
  /**
   * Check if the station has TXOP granted for the next MPDU.
   *
   * \return true if the station has TXOP granted for the next MPDU,
   *         false otherwise.
   */
  bool HasTxop (void) const;

  /**
   * Calculate the size of the next fragment.
   *
   * \return the size of the next fragment.
   */
  uint32_t GetNextFragmentSize (void) const;
  /**
   * Calculate the size of the current fragment.
   *
   * \return the size of the current fragment.
   */
  uint32_t GetFragmentSize (void) const;
  /**
   * Calculate the offset for the current fragment.
   *
   * \return the offset for the current fragment.
   */
  uint32_t GetFragmentOffset (void) const;
  /**
   * Check if the current fragment is the last fragment.
   *
   * \return true if the current fragment is the last fragment,
   *         false otherwise.
   */
  bool IsLastFragment (void) const;

  /**
   * Check if the current packet is fragmented because of an exceeded TXOP duration.
   *
   * \return true if the current packet is fragmented because of an exceeded TXOP duration,
   *         false otherwise
   */
  bool IsTxopFragmentation () const;
  /**
   * Calculate the size of the current TXOP fragment.
   *
   * \return the size of the current TXOP fragment
   */
  uint32_t GetTxopFragmentSize () const;
  /**
   * Calculate the number of TXOP fragments needed for the transmission of the current packet.
   *
   * \return the number of TXOP fragments needed for the transmission of the current packet
   */
  uint32_t GetNTxopFragment () const;
  /**
   * Calculate the size of the next TXOP fragment.
   *
   * \param fragmentNumber number of the next fragment
   * \returns the next TXOP fragment size
   */
  uint32_t GetNextTxopFragmentSize (uint32_t fragmentNumber) const;
  /**
   * Calculate the offset for the fragment.
   *
   * \param fragmentNumber number of the fragment
   * \returns the TXOP fragment offset
   */
  uint32_t GetTxopFragmentOffset (uint32_t fragmentNumber) const;

  void DoDispose (void);
  void DoInitialize (void);

  AcIndex m_ac;                                     //!< the access category
  Ptr<MsduAggregator> m_msduAggregator;             //!< A-MSDU aggregator
  Ptr<MpduAggregator> m_mpduAggregator;             //!< A-MPDU aggregator
  TypeOfStation m_typeOfStation;                    //!< the type of station
  Ptr<QosBlockedDestinations> m_qosBlockedDestinations; //!< QOS blocked destinations
  Ptr<BlockAckManager> m_baManager;                     //!< the Block ACK manager
  uint8_t m_blockAckThreshold;                      //!< the Block ACK threshold
  BlockAckType m_blockAckType;                      //!< the Block ACK type
  Time m_currentPacketTimestamp;                    //!< the current packet timestamp
  uint16_t m_blockAckInactivityTimeout;             //!< the Block ACK inactivity timeout
  Bar m_currentBar;                                 //!< the current BAR
  Time m_startTxop;                                 //!< the start TXOP time
  bool m_isAccessRequestedForRts;                   //!< flag whether access is requested to transmit a RTS frame
  bool m_currentIsFragmented;                       //!< flag whether current packet is fragmented

  TracedValue<uint32_t> m_backoffTrace;   //!< backoff trace value
  TracedValue<uint32_t> m_cwTrace;        //!< CW trace value
  TracedCallback<Time, Time> m_txopTrace; //!< TXOP trace callback
};

} //namespace ns3

#endif /* QOS_TXOP_H */
