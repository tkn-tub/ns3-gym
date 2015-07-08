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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef MAC_LOW_H
#define MAC_LOW_H

#include <vector>
#include <stdint.h>
#include <ostream>
#include <map>

#include "wifi-mac-header.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-remote-station-manager.h"
#include "ctrl-headers.h"
#include "mgt-headers.h"
#include "block-ack-agreement.h"
#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "qos-utils.h"
#include "block-ack-cache.h"
#include "wifi-tx-vector.h"
#include "mpdu-aggregator.h"
#include "msdu-aggregator.h"

class TwoLevelAggregationTest;

namespace ns3 {

class WifiPhy;
class WifiMac;
class EdcaTxopN;
class WifiMacQueue;

/**
 * \ingroup wifi
 * \brief listen to events coming from ns3::MacLow.
 */
class MacLowTransmissionListener
{
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
   * ns3::MacLow received an expected ACK within
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
   * \param blockAck Block ack response header
   * \param source Address of block ack sender
   * \param txMode mode of block ack response
   *
   * Invoked when ns3::MacLow receives a block ack frame.
   * Block ack frame is received after a block ack request
   * and contains information about the correct reception
   * of a set of packet for which a normal ack wasn't send.
   * Default implementation for this method is empty. Every
   * queue that intends to be notified by MacLow of reception
   * of a block ack must redefine this function.
   */
  virtual void GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address source, WifiMode txMode);
  /**
   * ns3::MacLow did not receive an expected BLOCK_ACK within
   * BlockAckTimeout. This method is used only for immediate
   * block ack variant. With delayed block ack, the MissedAck method will be
   * called instead: upon receipt of a block ack request, the rx station will
   * reply with a normal ack frame. Later, when the rx station gets a txop, it
   * will send the block ack back to the tx station which will reply with a
   * normal ack to the rx station.
   */
  virtual void MissedBlockAck (void);
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
  /**
   * Invoked upon the end of the transmission of a frame that does not
   * require an ACK (e.g., broadcast and multicast frames).
   *
   */
  virtual void EndTxNoAck (void) = 0;
};


/**
 * \brief listen to NAV events
 * \ingroup wifi
 *
 * This class is typically connected to an instance of ns3::Dcf
 * and calls to its methods are forwards to the corresponding
 * ns3::Dcf methods.
 */
class MacLowDcfListener
{
public:
  MacLowDcfListener ();
  virtual ~MacLowDcfListener ();
  /**
   * Norify that NAV has started for the given duration.
   *
   * \param duration duration of NAV timer
   */
  virtual void NavStart (Time duration) = 0;
  /**
   * Notify that NAV has resetted.
   *
   * \param duration duration of NAV timer
   */
  virtual void NavReset (Time duration) = 0;
  /**
   * Notify that ACK timeout has started for a given duration.
   *
   * \param duration duration of ACK timeout
   */
  virtual void AckTimeoutStart (Time duration) = 0;
  /**
   * Notify that ACK timeout has resetted.
   */
  virtual void AckTimeoutReset () = 0;
  /**
   * Notify that CTS timeout has started for a given duration.
   *
   * \param duration duration of CTS timeout
   */
  virtual void CtsTimeoutStart (Time duration) = 0;
  /**
   * Notify that CTS timeout has resetted.
   */
  virtual void CtsTimeoutReset () = 0;
};

/**
 * \ingroup wifi
 * \brief listen for block ack events.
 */
class MacLowAggregationCapableTransmissionListener
{
public:
  MacLowAggregationCapableTransmissionListener ();
  virtual ~MacLowAggregationCapableTransmissionListener ();
  /**
   * Typically is called in order to notify EdcaTxopN that a block ack inactivity
   * timeout occurs for the block ack agreement identified by the pair <i>originator</i>, <i>tid</i>.
   *
   * Rx station maintains an inactivity timer for each block ack
   * agreement. Timer is reset when a frame with ack policy block ack
   * or a block ack request are received. When this timer reaches zero
   * this method is called and a delba frame is scheduled for transmission.
   *
   * \param originator MAC address of the data originator
   * \param tid
   */
  virtual void BlockAckInactivityTimeout (Mac48Address originator, uint8_t tid) = 0;
  /**
   * Returns the EDCA queue to check if there are packets that can be aggregated with a Block Ack
   */
  virtual Ptr<WifiMacQueue> GetQueue (void) = 0;
  /**
   * \param address address of peer station involved in block ack mechanism.
   * \param tid traffic ID of transmitted packet.
   *
   * Calls CompleteAmpduTransfer that resets the status of OriginatorBlockAckAgreement after the transfer
   * of an A-MPDU with ImmediateBlockAck policy (i.e. no BAR is scheduled)
   */
  virtual void CompleteTransfer (Mac48Address address, uint8_t tid);
  virtual void SetAmpdu (bool ampdu);
  /**
   * This function stores an MPDU (part of an A-MPDU) in blockackagreement (i.e. the sender is waiting
   * for a blockack containing the sequence number of this MPDU).
   * It also calls NotifyMpdu transmission that updates the status of OriginatorBlockAckAgreement.
   */
  virtual void CompleteMpduTx (Ptr<const Packet> packet, WifiMacHeader hdr, Time tstamp);
  /**
   * Return the next sequence number for the given header.
   *
   * \param hdr Wi-Fi header
   * \return the next sequence number
   */
  virtual uint16_t GetNextSequenceNumberfor (WifiMacHeader *hdr);
  /**
   * Return the next sequence number for the Traffic ID and destination, but do not pick it (i.e. the current sequence number remains unchanged).
   *
   * \param hdr Wi-Fi header
   * \return the next sequence number
   */
  virtual uint16_t PeekNextSequenceNumberfor (WifiMacHeader *hdr);
  /*
   * Peek in retransmit queue and get the next packet without removing it from the queue
   */
  virtual Ptr<const Packet> PeekNextPacketInBaQueue (WifiMacHeader &header, Mac48Address recipient, uint8_t tid, Time *timestamp);
  /**
   * Remove a packet after you peek in the retransmit queue and get it
   */
  virtual void RemoveFromBaQueue (uint8_t tid, Mac48Address recipient, uint16_t seqnumber);
  /**
   * \param recipient address of the peer station
   * \param tid traffic ID.
   * \return true if a block ack agreement exists, false otherwise
   *
   * Checks if a block ack agreement exists with station addressed by
   * <i>recipient</i> for tid <i>tid</i>.
   */
  virtual bool GetBlockAckAgreementExists (Mac48Address address, uint8_t tid) = 0;
  /**
   * \param recipient address of peer station involved in block ack mechanism.
   * \param tid traffic ID.
   * \return the number of packets buffered for a specified agreement
   *
   * Returns number of packets buffered for a specified agreement.
   */
  virtual uint32_t GetNOutstandingPackets (Mac48Address recipient, uint8_t tid);
  /**
   * \param recipient address of peer station involved in block ack mechanism.
   * \param tid traffic ID.
   * \return the number of packets for a specific agreement that need retransmission
   *
   * Returns number of packets for a specific agreement that need retransmission.
   */
  virtual uint32_t GetNRetryNeededPackets (Mac48Address recipient, uint8_t tid) const;
  /**
   */
  virtual Ptr<MsduAggregator> GetMsduAggregator (void) const;
  /**
   */
  virtual Mac48Address GetSrcAddressForAggregation (const WifiMacHeader &hdr);
  /**
   */
  virtual Mac48Address GetDestAddressForAggregation (const WifiMacHeader &hdr);
};

/**
 * \brief control how a packet is transmitted.
 * \ingroup wifi
 *
 * The ns3::MacLow::StartTransmission method expects
 * an instance of this class to describe how the packet
 * should be transmitted.
 */
class MacLowTransmissionParameters
{
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
   * Wait BASICBLOCKACKTimeout for a Basic Block Ack Response frame.
   */
  void EnableBasicBlockAck (void);
  /**
   * Wait COMPRESSEDBLOCKACKTimeout for a Compressed Block Ack Response frame.
   */
  void EnableCompressedBlockAck (void);
  /**
   * NOT IMPLEMENTED FOR NOW
   */
  void EnableMultiTidBlockAck (void);
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
   * \returns true if block ack mechanism is used, false otherwise.
   *
   * \sa EnableBlockAck
   */
  bool MustWaitBasicBlockAck (void) const;
  /**
   * \returns true if compressed block ack mechanism is used, false otherwise.
   *
   * \sa EnableCompressedBlockAck
   */
  bool MustWaitCompressedBlockAck (void) const;
  /**
   * \returns true if multi-tid block ack mechanism is used, false otherwise.
   *
   * \sa EnableMultiTidBlockAck
   */
  bool MustWaitMultiTidBlockAck (void) const;
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
  enum
  {
    ACK_NONE,
    ACK_NORMAL,
    ACK_FAST,
    ACK_SUPER_FAST,
    BLOCK_ACK_BASIC,
    BLOCK_ACK_COMPRESSED,
    BLOCK_ACK_MULTI_TID
  } m_waitAck;
  bool m_sendRts;
  Time m_overrideDurationId;
};

/**
 * Serialize MacLowTransmissionParameters to ostream in a human-readable form.
 *
 * \param os std::ostream
 * \param params MacLowTransmissionParameters
 * \return std::ostream
 */
std::ostream &operator << (std::ostream &os, const MacLowTransmissionParameters &params);


/**
 * \ingroup wifi
 * \brief handle RTS/CTS/DATA/ACK transactions.
 */
class MacLow : public Object
{
public:
  // Allow test cases to access private members
  friend class ::TwoLevelAggregationTest;
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
  void SetPhy (Ptr<WifiPhy> phy);
  /*
   * \return current attached PHY device
   */
  Ptr<WifiPhy> GetPhy (void) const;
  /**
   * Remove WifiPhy associated with this MacLow.
   *
   * \param phy WifiPhy associated with this MacLow
   */
  void ResetPhy (void);
  /**
   * Set up WifiRemoteStationManager associated with this MacLow.
   *
   * \param manager WifiRemoteStationManager associated with this MacLow
   */
  void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> manager);
  /**
   * Set up MpduAggregator associated with this MacLow.
   *
   * \param aggregator MpduAggregator associated with this MacLow
   */
  void SetMpduAggregator (Ptr<MpduAggregator> aggregator);
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
   * \param listener listen to NAV events for every incoming
   *        and outgoing packet.
   */
  void RegisterDcfListener (MacLowDcfListener *listener);

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
   * \param packet packet to send
   * \param hdr 802.11 header for packet to send
   * \param parameters the transmission parameters to use for this packet.
   * \param listener listen to transmission events.
   *
   * Start the transmission of the input packet and notify the listener
   * of transmission events.
   */
  virtual void StartTransmission (Ptr<const Packet> packet,
                                  const WifiMacHeader* hdr,
                                  MacLowTransmissionParameters parameters,
                                  MacLowTransmissionListener *listener);

  /**
   * \param packet packet received
   * \param rxSnr snr of packet received
   * \param txVector TXVECTOR of packet received
   * \param preamble type of preamble used for the packet received
   * \param ampduSubframe true if this MPDU is part of an A-MPDU
   *
   * This method is typically invoked by the lower PHY layer to notify
   * the MAC layer that a packet was successfully received.
   */
  void ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, bool ampduSubframe);
  /**
   * \param packet packet received.
   * \param rxSnr snr of packet received.
   *
   * This method is typically invoked by the lower PHY layer to notify
   * the MAC layer that a packet was unsuccessfully received.
   */
  void ReceiveError (Ptr<const Packet> packet, double rxSnr);
  /**
   * \param duration switching delay duration.
   *
   * This method is typically invoked by the PhyMacLowListener to notify
   * the MAC layer that a channel switching occured. When a channel switching
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
   * \param listener The listener for the queue.
   *
   * The lifetime of the registered listener is typically equal to the lifetime of the queue
   * associated to this AC.
   */
  void RegisterBlockAckListenerForAc (enum AcIndex ac, MacLowAggregationCapableTransmissionListener *listener);
  /**
   * \param packet the packet to be aggregated. If the aggregation is succesfull, it corresponds either to the first data packet that will be aggregated or to the BAR that will be piggybacked at the end of the A-MPDU.
   * \param hdr the WifiMacHeader for the packet.
   * \return the A-MPDU packet if aggregation is successfull, the input packet otherwise
   *
   * This function adds the packets that will be added to an A-MPDU to an aggregate queue
   *
   */
  Ptr<Packet> AggregateToAmpdu (Ptr<const Packet> packet, const WifiMacHeader hdr);
  /**
   * \param aggregatedPacket which is the current A-MPDU
   * \param rxSnr snr of packet received
   * \param txVector TXVECTOR of packet received
   * \param preamble type of preamble used for the packet received
   *
   * This function de-aggregates an A-MPDU and decide if each MPDU is received correctly or not
   *
   */
  void DeaggregateAmpduAndReceive (Ptr<Packet> aggregatedPacket, double rxSnr, WifiTxVector txVector, WifiPreamble preamble);
  /**
   * \param peekedPacket the packet to be aggregated
   * \param peekedHdr the WifiMacHeader for the packet.
   * \param aggregatedPacket the current A-MPDU
   * \param size the size of a piggybacked block ack request
   * \return false if the given packet can be added to an A-MPDU, true otherwise
   *
   * This function decides if a given packet can be added to an A-MPDU or not
   *
   */
  bool StopMpduAggregation (Ptr<const Packet> peekedPacket, WifiMacHeader peekedHdr, Ptr<Packet> aggregatedPacket, uint16_t size) const;
  /**
   *
   * This function is called to flush the aggregate queue, which is used for A-MPDU
   *
   */
  void FlushAggregateQueue (void);

protected:
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
private:
  /**
   * Cancel all scheduled events. Called before beginning a transmission
   * or switching channel.
   */
  void CancelAllEvents (void);
  /**
   * Return the total ACK size (including FCS trailer).
   *
   * \return the total ACK size
   */
  uint32_t GetAckSize (void) const;
  /**
   * Return the total Block ACK size (including FCS trailer).
   *
   * \param type the Block ACK type
   * \return the total Block ACK size
   */
  uint32_t GetBlockAckSize (enum BlockAckType type) const;
  /**
   * Return the total RTS size (including FCS trailer).
   *
   * \return the total RTS size
   */
  uint32_t GetRtsSize (void) const;
  /**
   * Return the total CTS size (including FCS trailer).
   *
   * \return the total CTS size
   */
  uint32_t GetCtsSize (void) const;
  /**
   * Return the total size of the packet after WifiMacHeader and FCS trailer
   * have been added.
   *
   * \param packet the packet to be encapsulated with WifiMacHeader and FCS trailer
   * \param hdr the WifiMacHeader
   * \return the total packet size
   */
  uint32_t GetSize (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
  /**
   * Forward the packet down to WifiPhy for transmission. This is called for the entire A-MPDu when MPDU aggregation is used.
   *
   * \param packet
   * \param hdr
   * \param txVector
   * \param preamble
   */
  void ForwardDown (Ptr<const Packet> packet, const WifiMacHeader *hdr,
                    WifiTxVector txVector, WifiPreamble preamble);
  /**
   * Forward the packet down to WifiPhy for transmission. This is called for each MPDU when MPDU aggregation is used.
   *
   * \param packet
   * \param hdr
   * \param txVector
   * \param preamble
   * \param packetType
   * \param mpduReferenceNumber
   */
  void SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, uint32_t mpduReferenceNumber);
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
   * Return a TXVECTOR for the CTS-to-self frame.
   * The function consults WifiRemoteStationManager, which controls the rate
   * to different destinations.
   *
   * \param packet the packet that requires CTS-to-self
   * \param hdr the Wifi header of the packet
   * \return TXVECTOR for the CTS-to-self operation
   */
  WifiTxVector GetCtsToSelfTxVector (Ptr<const Packet> packet, const WifiMacHeader *hdr) const;
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
   * \param to
   * \param dataTxVector
   * \param type the Block ACK type
   * \return the time required to transmit the Block ACK (including preamble and FCS)
   */
  Time GetBlockAckDuration (Mac48Address to, WifiTxVector blockAckReqTxVector, enum BlockAckType type) const;
  /**
   * Check if CTS-to-self mechanism should be used for the current packet.
   *
   * \return true if CTS-to-self mechanism should be used for the current packet,
   *         false otherwise
   */
  bool NeedCtsToSelf (void);

  Time CalculateOverallTxTime (Ptr<const Packet> packet,
                               const WifiMacHeader* hdr,
                               const MacLowTransmissionParameters &params) const;
  void NotifyNav (Ptr<const Packet> packet,const WifiMacHeader &hdr, WifiPreamble preamble);
  /**
   * Reset NAV with the given duration.
   *
   * \param duration
   */
  void DoNavResetNow (Time duration);
  /**
   * Start NAV with the given duration.
   *
   * \param duration
   * \return true if NAV is resetted
   */
  bool DoNavStartNow (Time duration);
  /**
   * Check if NAV is zero.
   *
   * \return true if NAV is zero,
   *         false otherwise
   */
  bool IsNavZero (void) const;
  /**
   * Notify DcfManager (via DcfListener) that
   * ACK timer should be started for the given
   * duration.
   *
   * \param duration
   */
  void NotifyAckTimeoutStartNow (Time duration);
  /**
   * Notify DcfManager (via DcfListener) that
   * ACK timer should be resetted.
   */
  void NotifyAckTimeoutResetNow ();
  /**
   * Notify DcfManager (via DcfListener) that
   * CTS timer should be started for the given
   * duration.
   *
   * \param duration
   */
  void NotifyCtsTimeoutStartNow (Time duration);
  /**
   * Notify DcfManager (via DcfListener) that
   * CTS timer should be resetted.
   */
  void NotifyCtsTimeoutResetNow ();
  /**
   * Reset NAV after CTS was missed when the NAV was
   * setted with RTS.
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
   * Event handler when fast ACK timeout occurs (idle).
   */
  void FastAckTimeout (void);
  /**
   * Event handler when super fast ACK timeout occurs.
   */
  void SuperFastAckTimeout (void);
  /**
   * Event handler when fast ACK timeout occurs (busy).
   */
  void FastAckFailedTimeout (void);
  /**
   * Event handler when block ACK timeout occurs.
   */
  void BlockAckTimeout (void);
  /**
   * Event handler when CTS timeout occurs.
   */
  void CtsTimeout (void);
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
   * \param source
   * \param duration
   */
  void SendDataAfterCts (Mac48Address source, Time duration);
  /**
   * Event handler that is usually scheduled to fired at the appropriate time
   * after completing transmissions.
   */
  void WaitSifsAfterEndTx (void);
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

  virtual void DoDispose (void);

  /**
   * \param packet packet to check
   * \param hdr 802.11 header for packet to check
   *
   * Returns Tid of different packet types
   */
  uint8_t GetTid (Ptr<const Packet> packet, const WifiMacHeader hdr) const;
  /**
   * \param originator Address of peer participating in Block Ack mechanism.
   * \param tid TID for which Block Ack was created.
   * \param seq Starting sequence
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
   * \param seq MPDU sequence number
   * \param winstart sequence number window start
   * \param winsize the size of the sequence number window (currently default is 64)
   * This method checks if the MPDU's sequence number is inside the scoreboard boundaries or not
   */
  bool IsInWindow (uint16_t seq, uint16_t winstart, uint16_t winsize);
  /**
   * This method updates the reorder buffer and the scoreboard when an MPDU is received in an HT station
   * and sotres the MPDU if needed when an MPDU is received in an non-HT Station (implements HT
   * immediate block Ack)
   */
  bool ReceiveMpdu (Ptr<Packet> packet, WifiMacHeader hdr);
  /**
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
   */
  void SendBlockAckAfterBlockAckRequest (const CtrlBAckRequestHeader reqHdr, Mac48Address originator,
                                         Time duration, WifiMode blockAckReqTxMode);
  /**
   * Invoked after an A-MPDU has been received. Looks for corresponding
   * block ack agreement and creates block ack bitmap on a received packets basis.
   */
  void SendBlockAckAfterAmpdu (uint8_t tid, Mac48Address originator,
                               Time duration, WifiTxVector blockAckReqTxVector);
  /**
   * This method creates block ack frame with header equals to <i>blockAck</i> and start its transmission.
   *
   * \param blockAck
   * \param originator
   * \param immediate
   * \param duration
   * \param blockAckReqTxMode
   */
  void SendBlockAckResponse (const CtrlBAckResponseHeader* blockAck, Mac48Address originator, bool immediate,
                             Time duration, WifiMode blockAckReqTxMode);
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
  void SetupPhyMacLowListener (Ptr<WifiPhy> phy);
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
   *
   */
  bool IsAmpdu (Ptr<const Packet> packet, const WifiMacHeader hdr);
  /**
   * Insert in a temporary queue.
   * It is only used with a RTS/CTS exchange for an A-MPDU transmission.
   */
  void InsertInTxQueue (Ptr<const Packet> packet, const WifiMacHeader &hdr, Time tStamp);
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
  Ptr<Packet> PerformMsduAggregation (Ptr<const Packet> packet, WifiMacHeader *hdr, Time *tstamp, Ptr<Packet> currentAmpduPacket, uint16_t blockAckSize);


  Ptr<WifiPhy> m_phy; //!< Pointer to WifiPhy (actually send/receives frames)
  Ptr<WifiRemoteStationManager> m_stationManager; //!< Pointer to WifiRemoteStationManager (rate control)
  MacLowRxCallback m_rxCallback; //!< Callback to pass packet up

  /**
   * A struct for packet, Wifi header, and timestamp.
   */
  typedef struct
  {
    Ptr<const Packet> packet;
    WifiMacHeader hdr;
    Time timestamp;
  } Item;

  /**
   * typedef for an iterator for a list of MacLowDcfListener.
   */
  typedef std::vector<MacLowDcfListener *>::const_iterator DcfListenersCI;
  /**
   * typedef for a list of MacLowDcfListener.
   */
  typedef std::vector<MacLowDcfListener *> DcfListeners;
  DcfListeners m_dcfListeners; //!< List of MacLowDcfListener (pass events to Dcf)

  EventId m_normalAckTimeoutEvent;      //!< Normal ACK timeout event
  EventId m_fastAckTimeoutEvent;        //!< Fast ACK timeout event
  EventId m_superFastAckTimeoutEvent;   //!< Super fast ACK timeout event
  EventId m_fastAckFailedTimeoutEvent;  //!< Fast ACK failed timeout event
  EventId m_blockAckTimeoutEvent;       //!< Block ACK timeout event
  EventId m_ctsTimeoutEvent;            //!< CTS timeout event
  EventId m_sendCtsEvent;               //!< Event to send CTS
  EventId m_sendAckEvent;               //!< Event to send ACK
  EventId m_sendDataEvent;              //!< Event to send DATA
  EventId m_waitSifsEvent;              //!< Wait for SIFS event
  EventId m_endTxNoAckEvent;            //!< Event for finishing transmission that does not require ACK
  EventId m_navCounterResetCtsMissed;   //!< Event to reset NAV when CTS is not received
  EventId m_waitRifsEvent;              //!< Wait for RIFS event

  Ptr<MpduAggregator> m_mpduAggregator; //!<

  Ptr<Packet> m_currentPacket;              //!< Current packet transmitted/to be transmitted
  WifiMacHeader m_currentHdr;               //!< Header of the current packet
  MacLowTransmissionParameters m_txParams;  //!< Transmission parameters of the current packet
  MacLowTransmissionListener *m_listener;   //!< Transmission listener for the current packet
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

  Time m_lastNavStart;     //!< The time when the latest NAV started
  Time m_lastNavDuration;  //!< The duration of the latest NAV

  bool m_promisc;  //!< Flag if the device is operating in promiscuous mode
  bool m_ampdu;    //!< Flag if the current transmission involves an A-MPDU

  class PhyMacLowListener * m_phyMacLowListener; //!< Listener needed to monitor when a channel switching occurs.

  /*
   * BlockAck data structures.
   */
  typedef std::pair<Ptr<Packet>, WifiMacHeader> BufferedPacket;
  typedef std::list<BufferedPacket>::iterator BufferedPacketI;

  typedef std::pair<Mac48Address, uint8_t> AgreementKey;
  typedef std::pair<BlockAckAgreement, std::list<BufferedPacket> > AgreementValue;

  typedef std::map<AgreementKey, AgreementValue> Agreements;
  typedef std::map<AgreementKey, AgreementValue>::iterator AgreementsI;

  typedef std::map<AgreementKey, BlockAckCache> BlockAckCaches;
  typedef std::map<AgreementKey, BlockAckCache>::iterator BlockAckCachesI;

  Agreements m_bAckAgreements;
  BlockAckCaches m_bAckCaches;

  typedef std::map<AcIndex, MacLowAggregationCapableTransmissionListener*> QueueListeners;
  QueueListeners m_edcaListeners;
  bool m_ctsToSelfSupported;          //!< Flag whether CTS-to-self is supported
  uint8_t m_sentMpdus;                //!< Number of transmitted MPDUs in an A-MPDU that have not been acknowledged yet
  Ptr<WifiMacQueue> m_aggregateQueue; //!< Queue used for MPDU aggregation
  WifiTxVector m_currentTxVector;     //!< TXVECTOR used for the current packet transmission
  bool m_receivedAtLeastOneMpdu;      //!< Flag whether an MPDU has already been successfully received while receiving an A-MPDU
  std::vector<Item> m_txPackets;      //!< Contain temporary items to be sent with the next A-MPDU transmission, once RTS/CTS exchange has succeeded. It is not used in other cases.
  uint32_t m_mpduReferenceNumber;       //!< A-MPDU reference number to identify all subframes belonging to the same A-MPDU
};

} //namespace ns3

#endif /* MAC_LOW_H */
