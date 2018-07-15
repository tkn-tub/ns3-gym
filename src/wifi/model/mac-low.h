/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2006 INRIA
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

#ifndef MAC_LOW_H
#define MAC_LOW_H

#include <map>
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "channel-access-manager.h"
#include "block-ack-cache.h"
#include "mac-low-transmission-parameters.h"
#include "qos-utils.h"
#include "wifi-mac-header.h"
#include "wifi-tx-vector.h"
#include "block-ack-type.h"
#include "wifi-mpdu-type.h"

class TwoLevelAggregationTest;
class AmpduAggregationTest;

namespace ns3 {

class WifiPhy;
class Txop;
class QosTxop;
class WifiMacQueueItem;
class WifiMacQueue;
class BlockAckAgreement;
class MgtAddBaResponseHeader;
class WifiRemoteStationManager;
class CtrlBAckRequestHeader;
class CtrlBAckResponseHeader;

/**
 * \ingroup wifi
 * \brief handle RTS/CTS/DATA/ACK transactions.
 */
class MacLow : public Object
{
public:
  /// Allow test cases to access private members
  friend class ::TwoLevelAggregationTest;
  /// Allow test cases to access private members
  friend class ::AmpduAggregationTest;
  /**
   * typedef for a callback for MacLowRx
   */
  typedef Callback<void, Ptr<Packet>, const WifiMacHeader*> MacLowRxCallback;

  MacLow ();
  virtual ~MacLow ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * Set up WifiPhy associated with this MacLow.
   *
   * \param phy WifiPhy associated with this MacLow
   */
  void SetPhy (const Ptr<WifiPhy> phy);
  /**
   * \return current attached PHY device
   */
  Ptr<WifiPhy> GetPhy (void) const;
  /**
   * Remove WifiPhy associated with this MacLow.
   */
  void ResetPhy (void);
  /**
   * Set up WifiRemoteStationManager associated with this MacLow.
   *
   * \param manager WifiRemoteStationManager associated with this MacLow
   */
  void SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> manager);
  /**
   * Set MAC address of this MacLow.
   *
   * \param ad Mac48Address of this MacLow
   */
  void SetAddress (Mac48Address ad);
  /**
   * Set ACK timeout of this MacLow.
   *
   * \param ackTimeout ACK timeout of this MacLow
   */
  void SetAckTimeout (Time ackTimeout);
  /**
   * Set Basic Block ACK timeout of this MacLow.
   *
   * \param blockAckTimeout Basic Block ACK timeout of this MacLow
   */
  void SetBasicBlockAckTimeout (Time blockAckTimeout);
  /**
   * Set Compressed Block ACK timeout of this MacLow.
   *
   * \param blockAckTimeout Compressed Block ACK timeout of this MacLow
   */
  void SetCompressedBlockAckTimeout (Time blockAckTimeout);
  /**
   * Enable or disable CTS-to-self capability.
   *
   * \param enable Enable or disable CTS-to-self capability
   */
  void SetCtsToSelfSupported (bool enable);
  /**
   * Set CTS timeout of this MacLow.
   *
   * \param ctsTimeout CTS timeout of this MacLow
   */
  void SetCtsTimeout (Time ctsTimeout);
  /**
   * Set Short Interframe Space (SIFS) of this MacLow.
   *
   * \param sifs SIFS of this MacLow
   */
  void SetSifs (Time sifs);
  /**
   * Set Reduced Interframe Space (RIFS) of this MacLow.
   *
   * \param rifs RIFS of this MacLow
   */
  void SetRifs (Time rifs);
  /**
   * Set slot duration of this MacLow.
   *
   * \param slotTime slot duration of this MacLow
   */
  void SetSlotTime (Time slotTime);
  /**
   * Set PCF Interframe Space (PIFS) of this MacLow.
   *
   * \param pifs PIFS of this MacLow
   */
  void SetPifs (Time pifs);
  /**
   * \param interval the expected interval between two beacon transmissions.
   */
  void SetBeaconInterval (Time interval);
  /**
   * \param duration the maximum duration for the CF period.
   */
  void SetCfpMaxDuration (Time duration);
  /**
   * Set the Basic Service Set Identification.
   *
   * \param ad the BSSID
   */
  void SetBssid (Mac48Address ad);
  /**
   * Enable promiscuous mode.
   */
  void SetPromisc (void);
  /**
   * Return whether CTS-to-self capability is supported.
   *
   * \return true if CTS-to-self is supported, false otherwise
   */
  bool GetCtsToSelfSupported () const;
  /**
   * Return the MAC address of this MacLow.
   *
   * \return Mac48Address of this MacLow
   */
  Mac48Address GetAddress (void) const;
  /**
   * Return ACK timeout of this MacLow.
   *
   * \return ACK timeout
   */
  Time GetAckTimeout (void) const;
  /**
   * Return Basic Block ACK timeout of this MacLow.
   *
   * \return Basic Block ACK timeout
   */
  Time GetBasicBlockAckTimeout () const;
  /**
   * Return Compressed Block ACK timeout of this MacLow.
   *
   * \return Compressed Block ACK timeout
   */
  Time GetCompressedBlockAckTimeout () const;
  /**
   * Return CTS timeout of this MacLow.
   *
   * \return CTS timeout
   */
  Time GetCtsTimeout (void) const;
  /**
   * Return Short Interframe Space (SIFS) of this MacLow.
   *
   * \return SIFS
   */
  Time GetSifs (void) const;
  /**
   * Return slot duration of this MacLow.
   *
   * \return slot duration
   */
  Time GetSlotTime (void) const;
  /**
   * Return PCF Interframe Space (PIFS) of this MacLow.
   *
   * \return PIFS
   */
  Time GetPifs (void) const;
  /**
   * Return Reduced Interframe Space (RIFS) of this MacLow.
   *
   * \return RIFS
   */
  Time GetRifs (void) const;
  /**
   * \return the expected interval between two beacon transmissions.
   */
  Time GetBeaconInterval (void) const;
  /**
   * \return the maximum duration for the CF period.
   */
  Time GetCfpMaxDuration (void) const;
  /**
   * \return the remaining duration for the CF period.
   */
  Time GetRemainingCfpDuration (void) const;
  /**
   * Return the Basic Service Set Identification.
   *
   * \return BSSID
   */
  Mac48Address GetBssid (void) const;
  /**
   * Check if MacLow is operating in promiscuous mode.
   *
   * \return true if MacLow is operating in promiscuous mode,
   *         false otherwise
   */
  bool IsPromisc (void) const;

  /**
   * \param callback the callback which receives every incoming packet.
   *
   * This callback typically forwards incoming packets to
   * an instance of ns3::MacRxMiddle.
   */
  void SetRxCallback (Callback<void,Ptr<Packet>,const WifiMacHeader *> callback);
  /**
   * \param dcf listen to NAV events for every incoming and outgoing packet.
   */
  void RegisterDcf (Ptr<ChannelAccessManager> dcf);

  /**
   * \param packet to send (does not include the 802.11 MAC header and checksum)
   * \param hdr header associated to the packet to send.
   * \param parameters transmission parameters of packet.
   * \return the transmission time that includes the time for the next packet transmission
   *
   * This transmission time includes the time required for
   * the next packet transmission if one was selected.
   */
  Time CalculateTransmissionTime (Ptr<const Packet> packet,
                                  const WifiMacHeader* hdr,
                                  const MacLowTransmissionParameters& parameters) const;

  /**
   * \param packet to send (does not include the 802.11 MAC header and checksum)
   * \param hdr header associated to the packet to send.
   * \param params transmission parameters of packet.
   * \param fragmentSize the packet fragment size (if fragmentation is used)
   * \return the transmission time that includes the time for the next packet transmission
   *
   * This transmission time includes the time required for
   * the next packet transmission if one was selected.
   */
  Time CalculateOverallTxTime (Ptr<const Packet> packet,
                               const WifiMacHeader* hdr,
                               const MacLowTransmissionParameters& params,
                               uint32_t fragmentSize = 0) const;

  /**
   * \param packet packet to send
   * \param hdr 802.11 header for packet to send
   * \param parameters the transmission parameters to use for this packet.
   * \param txop pointer to the calling Txop.
   *
   * Start the transmission of the input packet and notify the listener
   * of transmission events.
   */
  virtual void StartTransmission (Ptr<const Packet> packet,
                                  const WifiMacHeader* hdr,
                                  MacLowTransmissionParameters parameters,
                                  Ptr<Txop> txop);

  /**
   * \param packet packet received
   * \param rxSnr snr of packet received
   * \param txVector TXVECTOR of packet received
   * \param ampduSubframe true if this MPDU is part of an A-MPDU
   *
   * This method is typically invoked by the lower PHY layer to notify
   * the MAC layer that a packet was successfully received.
   */
  void ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiTxVector txVector, bool ampduSubframe);
  /**
   * \param packet packet received.
   * \param rxSnr snr of packet received.
   *
   * This method is typically invoked by the lower PHY layer to notify
   * the MAC layer that a packet was unsuccessfully received.
   */
  void ReceiveError (Ptr<Packet> packet, double rxSnr);
  /**
   * \param duration switching delay duration.
   *
   * This method is typically invoked by the PhyMacLowListener to notify
   * the MAC layer that a channel switching occurred. When a channel switching
   * occurs, pending MAC transmissions (RTS, CTS, DATA and ACK) are cancelled.
   */
  void NotifySwitchingStartNow (Time duration);
  /**
   * This method is typically invoked by the PhyMacLowListener to notify
   * the MAC layer that the device has been put into sleep mode. When the device is put
   * into sleep mode, pending MAC transmissions (RTS, CTS, DATA and ACK) are cancelled.
   */
  void NotifySleepNow (void);
  /**
   * This method is typically invoked by the PhyMacLowListener to notify
   * the MAC layer that the device has been put into off mode. When the device is put
   * into off mode, pending MAC transmissions (RTS, CTS, DATA and ACK) are cancelled.
   */
  void NotifyOffNow (void);
  /**
   * \param respHdr Add block ack response from originator (action
   * frame).
   * \param originator Address of peer station involved in block ack
   * mechanism.
   * \param startingSeq Sequence number of the first MPDU of all
   * packets for which block ack was negotiated.
   *
   * This function is typically invoked only by ns3::RegularWifiMac
   * when the STA (which may be non-AP in ESS, or in an IBSS) has
   * received an ADDBA Request frame and is transmitting an ADDBA
   * Response frame. At this point MacLow must allocate buffers to
   * collect all correctly received packets belonging to the category
   * for which Block Ack was negotiated.
   */
  void CreateBlockAckAgreement (const MgtAddBaResponseHeader *respHdr,
                                Mac48Address originator,
                                uint16_t startingSeq);
  /**
   * \param originator Address of peer participating in Block Ack mechanism.
   * \param tid TID for which Block Ack was created.
   *
   * Checks if exists an established block ack agreement with <i>originator</i>
   * for tid <i>tid</i>. If the agreement exists, tears down it. This function is typically
   * invoked when a DELBA frame is received from <i>originator</i>.
   */
  void DestroyBlockAckAgreement (Mac48Address originator, uint8_t tid);
  /**
   * \param ac Access class managed by the queue.
   * \param edca the QosTxop for the queue.
   *
   * The lifetime of the registered QosTxop is typically equal to the lifetime of the queue
   * associated to this AC.
   */
  void RegisterEdcaForAc (AcIndex ac, Ptr<QosTxop> edca);
  /**
   * \param packet the packet to be aggregated. If the aggregation is successful, it corresponds either to the first data packet that will be aggregated or to the BAR that will be piggybacked at the end of the A-MPDU.
   * \param hdr the WifiMacHeader for the packet.
   * \return the A-MPDU packet if aggregation is successful, the input packet otherwise
   *
   * This function adds the packets that will be added to an A-MPDU to an aggregate queue
   *
   */
  Ptr<Packet> AggregateToAmpdu (Ptr<const Packet> packet, const WifiMacHeader hdr);
  /**
   * \param aggregatedPacket which is the current A-MPDU
   * \param rxSnr snr of packet received
   * \param txVector TXVECTOR of packet received
   *
   * This function de-aggregates an A-MPDU and decide if each MPDU is received correctly or not
   *
   */
  void DeaggregateAmpduAndReceive (Ptr<Packet> aggregatedPacket, double rxSnr, WifiTxVector txVector);
  /**
   * \param peekedPacket the packet to be aggregated
   * \param peekedHdr the WifiMacHeader for the packet.
   * \param aggregatedPacket the current A-MPDU
   * \param blockAckSize the size of a piggybacked block ack request
   * \return false if the given packet can be added to an A-MPDU, true otherwise
   *
   * This function decides if a given packet can be added to an A-MPDU or not
   *
   */
  bool StopMpduAggregation (Ptr<const Packet> peekedPacket, WifiMacHeader peekedHdr, Ptr<Packet> aggregatedPacket, uint8_t blockAckSize) const;
  /**
   *
   * This function is called to flush the aggregate queue, which is used for A-MPDU
   * \param tid the Traffic ID
   *
   */
  void FlushAggregateQueue (uint8_t tid);

  /**
   * Return a TXVECTOR for the DATA frame given the destination.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param packet the packet being asked for TXVECTOR
   * \param hdr the WifiMacHeader
   * \return TXVECTOR for the given packet
   */
  virtual WifiTxVector GetDataTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
  /**
   * Start NAV with the given duration.
   *
   * \param duration the duration
   * \return true if NAV is reset
   */
  bool DoNavStartNow (Time duration);
  /**
   * This function indicates whether it is the CF period.
   */
  virtual bool IsCfPeriod (void) const;
  /**
   * This function decides if a CF frame can be transmitted in the current CFP.
   */
  bool CanTransmitNextCfFrame (void) const;


private:
  /**
   * Cancel all scheduled events. Called before beginning a transmission
   * or switching channel.
   */
  void CancelAllEvents (void);
  /**
   * Return the total CF-END size (including FCS trailer).
   *
   * \return the total CF-END size
   */
  uint32_t GetCfEndSize (void) const;
  /**
   * Forward the packet down to WifiPhy for transmission. This is called for the entire A-MPDu when MPDU aggregation is used.
   *
   * \param packet the packet
   * \param hdr the header
   * \param txVector the transmit vector
   */
  void ForwardDown (Ptr<const Packet> packet, const WifiMacHeader *hdr, WifiTxVector txVector);
  /**
   * Forward the MPDU down to WifiPhy for transmission. This is called for each MPDU when MPDU aggregation is used.
   *
   * \param packet the packet
   * \param txVector the transmit vector
   * \param mpdutype the MPDU type
   */
  void SendMpdu (Ptr<const Packet> packet, WifiTxVector txVector, MpduType mpdutype);
  /**
   * Return a TXVECTOR for the RTS frame given the destination.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param packet the packet being asked for RTS TXVECTOR
   * \param hdr the WifiMacHeader
   * \return TXVECTOR for the RTS of the given packet
   */
  WifiTxVector GetRtsTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
  /**
   * Return a TXVECTOR for the CTS frame given the destination and the mode of the RTS
   * used by the sender.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param to the MAC address of the CTS receiver
   * \param rtsTxMode the mode of the RTS used by the sender
   * \return TXVECTOR for the CTS
   */
  WifiTxVector GetCtsTxVector (Mac48Address to, WifiMode rtsTxMode) const;
  /**
   * Return a TXVECTOR for the ACK frame given the destination and the mode of the DATA
   * used by the sender.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param to the MAC address of the ACK receiver
   * \param dataTxMode the mode of the DATA used by the sender
   * \return TXVECTOR for the ACK
   */
  WifiTxVector GetAckTxVector (Mac48Address to, WifiMode dataTxMode) const;
  /**
   * Return a TXVECTOR for the Block ACK frame given the destination and the mode of the DATA
   * used by the sender.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param to the MAC address of the Block ACK receiver
   * \param dataTxMode the mode of the DATA used by the sender
   * \return TXVECTOR for the Block ACK
   */
  WifiTxVector GetBlockAckTxVector (Mac48Address to, WifiMode dataTxMode) const;
  /**
   * Return a TXVECTOR for the CTS frame given the destination and the mode of the RTS
   * used by the sender.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param to the MAC address of the CTS receiver
   * \param rtsTxMode the mode of the RTS used by the sender
   * \return TXVECTOR for the CTS
   */
  WifiTxVector GetCtsTxVectorForRts (Mac48Address to, WifiMode rtsTxMode) const;
  /**
   * Return a TXVECTOR for the Block ACK frame given the destination and the mode of the DATA
   * used by the sender.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param to the MAC address of the Block ACK receiver
   * \param dataTxMode the mode of the DATA used by the sender
   * \return TXVECTOR for the Block ACK
   */
  WifiTxVector GetAckTxVectorForData (Mac48Address to, WifiMode dataTxMode) const;
  /**
   * Return the time required to transmit the CTS (including preamble and FCS).
   *
   * \param ctsTxVector
   * \return the time required to transmit the CTS (including preamble and FCS)
   */
  Time GetCtsDuration (WifiTxVector ctsTxVector) const;
  /**
   * Return the time required to transmit the CTS to the specified address
   * given the TXVECTOR of the RTS (including preamble and FCS).
   *
   * \param to
   * \param rtsTxVector
   * \return the time required to transmit the CTS (including preamble and FCS)
   */
  Time GetCtsDuration (Mac48Address to, WifiTxVector rtsTxVector) const;
  /**
   * Return the time required to transmit the ACK (including preamble and FCS).
   *
   * \param ackTxVector
   * \return the time required to transmit the ACK (including preamble and FCS)
   */
  Time GetAckDuration (WifiTxVector ackTxVector) const;
  /**
   * Return the time required to transmit the ACK to the specified address
   * given the TXVECTOR of the DATA (including preamble and FCS).
   *
   * \param to
   * \param dataTxVector
   * \return the time required to transmit the ACK (including preamble and FCS)
   */
  Time GetAckDuration (Mac48Address to, WifiTxVector dataTxVector) const;
  /**
   * Return the time required to transmit the Block ACK to the specified address
   * given the TXVECTOR of the BAR (including preamble and FCS).
   *
   * \param blockAckReqTxVector
   * \param type the Block ACK type
   * \return the time required to transmit the Block ACK (including preamble and FCS)
   */
  Time GetBlockAckDuration (WifiTxVector blockAckReqTxVector, BlockAckType type) const;
  /**
   * Check if the current packet should be sent with a RTS protection.
   *
   * \return true if RTS protection should be used,
   *         false otherwise
   */
  bool NeedRts (void) const;
  /**
   * Check if CTS-to-self mechanism should be used for the current packet.
   *
   * \return true if CTS-to-self mechanism should be used for the current packet,
   *         false otherwise
   */
  bool NeedCtsToSelf (void) const;

  /**
   * Notify NAV function.
   *
   * \param packet the packet
   * \param hdr the header
   */
  void NotifyNav (Ptr<const Packet> packet,const WifiMacHeader &hdr);
  /**
   * Reset NAV with the given duration.
   *
   * \param duration
   */
  void DoNavResetNow (Time duration);
  /**
   * Check if NAV is zero.
   *
   * \return true if NAV is zero,
   *         false otherwise
   */
  bool IsNavZero (void) const;
  /**
   * Notify ChannelAccessManager that ACK timer should be started for the given duration.
   *
   * \param duration the duration
   */
  void NotifyAckTimeoutStartNow (Time duration);
  /**
   * Notify ChannelAccessManager that ACK timer should be reset.
   */
  void NotifyAckTimeoutResetNow ();
  /**
   * Notify ChannelAccessManager that CTS timer should be started for the given duration.
   *
   * \param duration
   */
  void NotifyCtsTimeoutStartNow (Time duration);
  /**
   * Notify ChannelAccessManager that CTS timer should be reset.
   */
  void NotifyCtsTimeoutResetNow ();
  /**
   * Reset NAV after CTS was missed when the NAV was
   * set with RTS.
   *
   * \param rtsEndRxTime
   */
  void NavCounterResetCtsMissed (Time rtsEndRxTime);
  /* Event handlers */
  /**
   * Event handler when normal ACK timeout occurs.
   */
  void NormalAckTimeout (void);
  /**
   * Event handler when block ACK timeout occurs.
   */
  void BlockAckTimeout (void);
  /**
   * Event handler when CTS timeout occurs.
   */
  void CtsTimeout (void);
  /**
   * Event handler when CF-POLL timeout occurs.
   */
  void CfPollTimeout (void);
  /**
   * Send CTS for a CTS-to-self mechanism.
   */
  void SendCtsToSelf (void);
  /**
   * Send CTS after receiving RTS.
   *
   * \param source
   * \param duration
   * \param rtsTxVector
   * \param rtsSnr
   */
  void SendCtsAfterRts (Mac48Address source, Time duration, WifiTxVector rtsTxVector, double rtsSnr);
  /**
   * Send ACK after receiving DATA.
   *
   * \param source
   * \param duration
   * \param dataTxMode
   * \param dataSnr
   */
  void SendAckAfterData (Mac48Address source, Time duration, WifiMode dataTxMode, double dataSnr);
  /**
   * Send DATA after receiving CTS.
   *
   * \param duration
   */
  void SendDataAfterCts (Time duration);

  /**
   * Event handler that is usually scheduled to fired at the appropriate time
   * after completing transmissions.
   */
  void WaitIfsAfterEndTxFragment (void);
  /**
   * Event handler that is usually scheduled to fired at the appropriate time
   * after sending a packet.
   */
  void WaitIfsAfterEndTxPacket (void);

  /**
   * A transmission that does not require an ACK has completed.
   */
  void EndTxNoAck (void);
  /**
   * Send RTS to begin RTS-CTS-DATA-ACK transaction.
   */
  void SendRtsForPacket (void);
  /**
   * Send DATA packet, which can be DATA-ACK or
   * RTS-CTS-DATA-ACK transaction.
   */
  void SendDataPacket (void);
  /**
   * Start a DATA timer by scheduling appropriate
   * ACK timeout.
   *
   * \param dataTxVector
   */
  void StartDataTxTimers (WifiTxVector dataTxVector);

  void DoDispose (void);

  /**
   * \param originator Address of peer participating in Block Ack mechanism.
   * \param tid TID for which Block Ack was created.
   * \param seq Starting sequence control
   *
   * This function forward up all completed "old" packets with sequence number
   * smaller than <i>seq</i>. All comparison are performed circularly mod 4096.
   */
  void RxCompleteBufferedPacketsWithSmallerSequence (uint16_t seq, Mac48Address originator, uint8_t tid);
  /**
   * \param originator Address of peer participating in Block Ack mechanism.
   * \param tid TID for which Block Ack was created.
   *
   * This method is typically invoked when a MPDU with ack policy
   * subfield set to Normal Ack is received and a block ack agreement
   * for that packet exists.
   * This happens when the originator of block ack has only few MPDUs to send.
   * All completed MSDUs starting with starting sequence number of block ack
   * agreement are forward up to WifiMac until there is an incomplete or missing MSDU.
   * See section 9.10.4 in IEEE 802.11 standard for more details.
   */
  void RxCompleteBufferedPacketsUntilFirstLost (Mac48Address originator, uint8_t tid);
  /**
   * \param packet the packet
   * \param hdr the header
   * \returns true if MPDU received
   *
   * This method updates the reorder buffer and the scoreboard when an MPDU is received in an HT station
   * and stores the MPDU if needed when an MPDU is received in an non-HT Station (implements HT
   * immediate block Ack)
   */
  bool ReceiveMpdu (Ptr<Packet> packet, WifiMacHeader hdr);
  /**
   * \param packet the packet
   * \param hdr the header
   * \returns true if the MPDU stored
   *
   * This method checks if exists a valid established block ack agreement.
   * If there is, store the packet without pass it up to WifiMac. The packet is buffered
   * in order of increasing sequence control field. All comparison are performed
   * circularly modulo 2^12.
   */
  bool StoreMpduIfNeeded (Ptr<Packet> packet, WifiMacHeader hdr);
  /**
   * Invoked after that a block ack request has been received. Looks for corresponding
   * block ack agreement and creates block ack bitmap on a received packets basis.
   *
   * \param reqHdr
   * \param originator
   * \param duration
   * \param blockAckReqTxMode
   * \param rxSnr
   */
  void SendBlockAckAfterBlockAckRequest (const CtrlBAckRequestHeader reqHdr, Mac48Address originator,
                                         Time duration, WifiMode blockAckReqTxMode, double rxSnr);
  /**
   * Invoked after an A-MPDU has been received. Looks for corresponding
   * block ack agreement and creates block ack bitmap on a received packets basis.
   *
   * \param tid the Traffic ID
   * \param originator the originator MAC address
   * \param duration the remaining NAV duration
   * \param blockAckReqTxVector the transmit vector
   * \param rxSnr the receive SNR
   */
  void SendBlockAckAfterAmpdu (uint8_t tid, Mac48Address originator,
                               Time duration, WifiTxVector blockAckReqTxVector, double rxSnr);
  /**
   * This method creates block ack frame with header equals to <i>blockAck</i> and start its transmission.
   *
   * \param blockAck
   * \param originator
   * \param immediate
   * \param duration
   * \param blockAckReqTxMode
   * \param rxSnr
   */
  void SendBlockAckResponse (const CtrlBAckResponseHeader* blockAck, Mac48Address originator, bool immediate,
                             Time duration, WifiMode blockAckReqTxMode, double rxSnr);
  /**
   * Every time that a block ack request or a packet with ack policy equals to <i>block ack</i>
   * are received, if a relative block ack agreement exists and the value of inactivity timeout
   * is not 0, the timer is reset.
   * see section 11.5.3 in IEEE 802.11e for more details.
   *
   * \param agreement
   */
  void ResetBlockAckInactivityTimerIfNeeded (BlockAckAgreement &agreement);

  /**
   * Set up WifiPhy listener for this MacLow.
   *
   * \param phy the WifiPhy this MacLow is connected to
   */
  void SetupPhyMacLowListener (const Ptr<WifiPhy> phy);
  /**
   * Remove current WifiPhy listener for this MacLow.
   *
   * \param phy the WifiPhy this MacLow is connected to
   */
  void RemovePhyMacLowListener (Ptr<WifiPhy> phy);
  /**
   * Checks if the given packet will be aggregated to an A-MPDU or not
   *
   * \param packet packet to check whether it can be aggregated in an A-MPDU
   * \param hdr 802.11 header for packet to check whether it can be aggregated in an A-MPDU
   * \returns true if is A-MPDU
   */
  bool IsAmpdu (Ptr<const Packet> packet, const WifiMacHeader hdr);
  /**
   * Insert in a temporary queue.
   * It is only used with a RTS/CTS exchange for an A-MPDU transmission.
   *
   * \param packet packet to be inserted in the A-MPDU tx queue
   * \param hdr 802.11 header for the packet to be inserted in the A-MPDU tx queue
   * \param tStamp timestamp of the packet to be inserted in the A-MPDU tx queue
   * \param tid the Traffic ID of the packet to be inserted in the A-MPDU tx queue
   */
  void InsertInTxQueue (Ptr<const Packet> packet, const WifiMacHeader &hdr, Time tStamp, uint8_t tid);
  /**
   * Perform MSDU aggregation for a given MPDU in an A-MPDU
   *
   * \param packet packet picked for aggregation
   * \param hdr 802.11 header for packet picked for aggregation
   * \param tstamp timestamp
   * \param currentAmpduPacket current A-MPDU packet
   * \param blockAckSize size of the piggybacked block ack request
   *
   * \return the aggregate if MSDU aggregation succeeded, 0 otherwise
   */
  Ptr<Packet> PerformMsduAggregation (Ptr<const Packet> packet, WifiMacHeader *hdr, Time *tstamp, Ptr<Packet> currentAmpduPacket, uint8_t blockAckSize);

  Ptr<WifiPhy> m_phy; //!< Pointer to WifiPhy (actually send/receives frames)
  Ptr<WifiRemoteStationManager> m_stationManager; //!< Pointer to WifiRemoteStationManager (rate control)
  MacLowRxCallback m_rxCallback; //!< Callback to pass packet up

  /**
   * A struct for packet, Wifi header, and timestamp.
   */
  struct Item
  {
    Ptr<const Packet> packet; //!< the packet
    WifiMacHeader hdr; //!< the header
    Time timestamp; //!< the timestamp
  }; //!< item structure

  /**
   * A struct that holds information about ACK piggybacking (CF-ACK).
   */
  struct CfAckInfo
  {
    bool appendCfAck; //!< Flag used for PCF to indicate whether a CF-ACK should be appended
    bool expectCfAck; //!< Flag used for PCF to indicate whether a CF-ACK should be expected
    Mac48Address address; //!< Address of the station to be acknowledged
  };

  /**
   * typedef for an iterator for a list of ChannelAccessManager.
   */
  typedef std::vector<Ptr<ChannelAccessManager> >::const_iterator ChannelAccessManagersCI;
  /**
   * typedef for a list of ChannelAccessManager.
   */
  typedef std::vector<Ptr<ChannelAccessManager> > ChannelAccessManagers;
  ChannelAccessManagers m_channelAccessManagers; //!< List of ChannelAccessManager

  EventId m_normalAckTimeoutEvent;      //!< Normal ACK timeout event
  EventId m_blockAckTimeoutEvent;       //!< Block ACK timeout event
  EventId m_ctsTimeoutEvent;            //!< CTS timeout event
  EventId m_sendCtsEvent;               //!< Event to send CTS
  EventId m_sendAckEvent;               //!< Event to send ACK
  EventId m_sendDataEvent;              //!< Event to send DATA
  EventId m_waitIfsEvent;               //!< Wait for IFS event
  EventId m_endTxNoAckEvent;            //!< Event for finishing transmission that does not require ACK
  EventId m_navCounterResetCtsMissed;   //!< Event to reset NAV when CTS is not received

  Ptr<Packet> m_currentPacket;              //!< Current packet transmitted/to be transmitted
  WifiMacHeader m_currentHdr;               //!< Header of the current transmitted packet
  Ptr<Txop> m_currentTxop;                  //!< Current TXOP
  MacLowTransmissionParameters m_txParams;  //!< Transmission parameters of the current packet
  Mac48Address m_self;                      //!< Address of this MacLow (Mac48Address)
  Mac48Address m_bssid;                     //!< BSSID address (Mac48Address)
  Time m_ackTimeout;                        //!< ACK timeout duration
  Time m_basicBlockAckTimeout;              //!< Basic block ACK timeout duration
  Time m_compressedBlockAckTimeout;         //!< Compressed block ACK timeout duration
  Time m_ctsTimeout;                        //!< CTS timeout duration
  Time m_sifs;                              //!< Short Interframe Space (SIFS) duration
  Time m_slotTime;                          //!< Slot duration
  Time m_pifs;                              //!< PCF Interframe Space (PIFS) duration
  Time m_rifs;                              //!< Reduced Interframe Space (RIFS) duration

  Time m_beaconInterval;   //!< Expected interval between two beacon transmissions
  Time m_cfpMaxDuration;   //!< CFP max duration

  Time m_lastNavStart;     //!< The time when the latest NAV started
  Time m_lastNavDuration;  //!< The duration of the latest NAV

  Time m_cfpStart;          //!< The time when the latest CF period started
  Time m_lastBeacon;        //!< The time when the last beacon frame transmission started
  Time m_cfpForeshortening; //!< The delay the current CF period should be foreshortened

  bool m_promisc;  //!< Flag if the device is operating in promiscuous mode
  bool m_ampdu;    //!< Flag if the current transmission involves an A-MPDU

  class PhyMacLowListener * m_phyMacLowListener; //!< Listener needed to monitor when a channel switching occurs.

  /*
   * BlockAck data structures.
   */
  typedef std::pair<Ptr<Packet>, WifiMacHeader> BufferedPacket; //!< buffered packet typedef
  typedef std::list<BufferedPacket>::iterator BufferedPacketI; //!< buffered packet iterator typedef

  typedef std::pair<Mac48Address, uint8_t> AgreementKey; //!< agreement key typedef
  typedef std::pair<BlockAckAgreement, std::list<BufferedPacket> > AgreementValue; //!< agreement value typedef

  typedef std::map<AgreementKey, AgreementValue> Agreements; //!< agreements
  typedef std::map<AgreementKey, AgreementValue>::iterator AgreementsI; //!< agreements iterator

  typedef std::map<AgreementKey, BlockAckCache> BlockAckCaches; //!< block ack caches typedef
  typedef std::map<AgreementKey, BlockAckCache>::iterator BlockAckCachesI; //!< block ack caches iterator typedef

  Agreements m_bAckAgreements; //!< block ack agreements
  BlockAckCaches m_bAckCaches; //!< block ack caches

  typedef std::map<AcIndex, Ptr<QosTxop> > QueueEdcas; //!< EDCA queues typedef
  QueueEdcas m_edca; //!< EDCA queues

  bool m_ctsToSelfSupported;             //!< Flag whether CTS-to-self is supported
  Ptr<WifiMacQueue> m_aggregateQueue[8]; //!< Queues per TID used for MPDU aggregation
  std::vector<Item> m_txPackets[8];      //!< Contain temporary items to be sent with the next A-MPDU transmission for a given TID, once RTS/CTS exchange has succeeded.
  WifiTxVector m_currentTxVector;        //!< TXVECTOR used for the current packet transmission

  CfAckInfo m_cfAckInfo; //!< Info about piggyback ACKs used in PCF
};

} //namespace ns3

#endif /* MAC_LOW_H */
