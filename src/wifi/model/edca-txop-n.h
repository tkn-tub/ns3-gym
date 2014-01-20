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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef EDCA_TXOP_N_H
#define EDCA_TXOP_N_H

#include "ns3/object.h"
#include "ns3/mac48-address.h"
#include "ns3/packet.h"

#include "wifi-mode.h"
#include "wifi-mac-header.h"
#include "wifi-remote-station-manager.h"
#include "qos-utils.h"
#include "dcf.h"
#include "ctrl-headers.h"
#include "block-ack-manager.h"

#include <map>
#include <list>

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
  Ptr<WifiMacQueue > GetQueue () const;
  virtual void SetMinCw (uint32_t minCw);
  virtual void SetMaxCw (uint32_t maxCw);
  virtual void SetAifsn (uint32_t aifsn);
  virtual uint32_t GetMinCw (void) const;
  virtual uint32_t GetMaxCw (void) const;
  virtual uint32_t GetAifsn (void) const;

  /**
   * Return the MacLow associated with this EdcaTxopN.
   *
   * \return MacLow
   */
  Ptr<MacLow> Low (void);
  Ptr<MsduAggregator> GetMsduAggregator (void) const;

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
   */
  void GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient);
  /**
   * Event handler when a Block ACK timeout has occurred.
   */
  void MissedBlockAck (void);
  void GotAddBaResponse (const MgtAddBaResponseHeader *respHdr, Mac48Address recipient);
  void GotDelBaFrame (const MgtDelBaHeader *delBaHdr, Mac48Address recipient);
  /**
   * Event handler when an ACK is received.
   */
  void MissedAck (void);
  /**
   * Start transmission for the next fragment.
   * This is called for fragment only.
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
   * Check if the current packet should be sent with a RTS protection.
   *
   * \return true if RTS protection should be used, false otherwise
   */
  bool NeedRts (void);
  /**
   * Check if RTS should be re-transmitted if CTS was missed.
   *
   * \return true if RTS should be re-transmitted, false otherwise
   */
  bool NeedRtsRetransmission (void);
  /**
   * Check if DATA should be re-transmitted if ACK was missed.
   *
   * \return true if DATA should be re-transmitted, false otherwise
   */
  bool NeedDataRetransmission (void);
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
   * Check if the curren fragment is the last fragment.
   *
   * \return true if the curren fragment is the last fragment,
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
   * If number of packets in the queue reaches m_blockAckThreshold value, an ADDBARequest frame
   * is sent to destination in order to setup a block ack.
   *
   * \return true if we tried to set up block ACK, false otherwise
   */
  bool SetupBlockAckIfNeeded ();
  /**
   * Sends an ADDBARequest to establish a block ack agreement with sta
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

  AcIndex m_ac;
  class Dcf;
  class TransmissionListener;
  class BlockAckEventListener;
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
  BlockAckEventListener *m_blockAckListener;
  RandomStream *m_rng;
  Ptr<WifiRemoteStationManager> m_stationManager;
  uint8_t m_fragmentNumber;

  /* current packet could be a simple MSDU or, if an aggregator for this queue is
     present, could be an A-MSDU.
   */
  Ptr<const Packet> m_currentPacket;

  WifiMacHeader m_currentHdr;
  Ptr<MsduAggregator> m_aggregator;
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
};

}  // namespace ns3

#endif /* EDCA_TXOP_N_H */
