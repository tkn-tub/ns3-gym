/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef DCA_TXOP_H
#define DCA_TXOP_H

#include "mac-low.h"
#include "wifi-mac-header.h"
#include "wifi-remote-station-manager.h"

namespace ns3 {

class DcfState;
class DcfManager;
class MacTxMiddle;
class UniformRandomVariable;
class CtrlBAckResponseHeader;

/**
 * \brief handle packet fragmentation and retransmissions.
 * \ingroup wifi
 *
 * This class implements the packet fragmentation and
 * retransmission policy. It uses the ns3::MacLow and ns3::DcfManager
 * helper classes to respectively send packets and decide when
 * to send them. Packets are stored in a ns3::WifiMacQueue until
 * they can be sent.
 *
 * The policy currently implemented uses a simple fragmentation
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

class DcaTxop : public Object
{
public:
  /// allow DcfListener class access
  friend class DcfListener;
  /// allow MacLowTransmissionListener class access
  friend class MacLowTransmissionListener;

  DcaTxop ();
  virtual ~DcaTxop ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

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
  /**
   * typedef for a callback to invoke when a
   * packet is dropped.
   */
  typedef Callback <void, Ptr<const Packet> > TxDropped;

  /**
   * Check for EDCA.
   *
   * \returns true if EDCA.
   */
  virtual bool IsEdca ();

  /**
   * Set MacLow associated with this DcaTxop.
   *
   * \param low MacLow.
   */
  void SetLow (const Ptr<MacLow> low);
  /**
   * Set DcfManager this DcaTxop is associated to.
   *
   * \param manager DcfManager.
   */
  void SetManager (const Ptr<DcfManager> manager);
  /**
   * Set WifiRemoteStationsManager this DcaTxop is associated to.
   *
   * \param remoteManager WifiRemoteStationManager.
   */
  virtual void SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> remoteManager);
  /**
   * Set MacTxMiddle this DcaTxop is associated to.
   *
   * \param txMiddle MacTxMiddle.
   */
  void SetTxMiddle (const Ptr<MacTxMiddle> txMiddle);

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
   * \param callback the callback to invoke when a
   * packet is dropped.
   */
  void SetTxDroppedCallback (TxDropped callback);

  /**
   * Return the MacLow associated with this DcaTxop.
   *
   * \return MacLow
   */
  Ptr<MacLow> GetLow (void) const;

  /**
   * Return the packet queue associated with this DcaTxop.
   *
   * \return WifiMacQueue
   */
  Ptr<WifiMacQueue > GetQueue () const;

  /**
   * Set the minimum contention window size.
   *
   * \param minCw the minimum contention window size.
   */
  void SetMinCw (uint32_t minCw);
  /**
   * Set the maximum contention window size.
   *
   * \param maxCw the maximum contention window size.
   */
  void SetMaxCw (uint32_t maxCw);
  /**
   * Set the number of slots that make up an AIFS.
   *
   * \param aifsn the number of slots that make up an AIFS.
   */
  void SetAifsn (uint32_t aifsn);
  /**
   * Set the TXOP limit.
   *
   * \param txopLimit the TXOP limit.
   * Value zero corresponds to default DCF.
   */
  void SetTxopLimit (Time txopLimit);
  /**
   * Return the minimum contention window size.
   *
   * \return the minimum contention window size.
   */
  uint32_t GetMinCw (void) const;
  /**
   * Return the maximum contention window size.
   *
   * \return the maximum contention window size.
   */
  uint32_t GetMaxCw (void) const;
  /**
   * Return the number of slots that make up an AIFS.
   *
   * \return the number of slots that make up an AIFS.
   */
  uint32_t GetAifsn (void) const;
  /**
   * Return the TXOP limit.
   *
   * \return the TXOP limit.
   */
  Time GetTxopLimit (void) const;

  /**
   * When a channel switching occurs, enqueued packets are removed.
   */
  virtual void NotifyChannelSwitching (void);
  /**
   * When sleep operation occurs, if there is a pending packet transmission,
   * it will be reinserted to the front of the queue.
   */
  virtual void NotifySleep (void);
  /**
   * When wake up operation occurs, channel access will be restarted.
   */
  virtual void NotifyWakeUp (void);

  /* Event handlers */
  /**
   * \param packet packet to send.
   * \param hdr header of packet to send.
   *
   * Store the packet in the internal queue until it
   * can be sent safely.
   */
  virtual void Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr);

  /* Event handlers */
  /**
   * Event handler when a CTS timeout has occurred.
   */
  virtual void MissedCts (void);
  /**
   * Event handler when an ACK is received.
   */
  virtual void GotAck (void);
  /**
   * Event handler when an ACK is missed.
   */
  virtual void MissedAck (void);
  /**
   * Event handler when a Block ACK is received.
   *
   * \param blockAck block ack.
   * \param recipient address of the recipient.
   * \param rxSnr SNR of the block ack itself.
   * \param txMode wifi mode.
   * \param dataSnr reported data SNR from the peer.
   */
  virtual void GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr);
  /**
   * Event handler when a Block ACK timeout has occurred.
   * \param nMpdus the number of MPDUs sent in the A-MPDU transmission that results in a Block ACK timeout.
   */
  virtual void MissedBlockAck (uint8_t nMpdus);

  /**
   * Start transmission for the next fragment.
   * This is called for fragment only.
   */
  virtual void StartNextFragment (void);
  /**
   * Cancel the transmission.
   */
  virtual void Cancel (void);
  /**
   * Start transmission for the next packet if allowed by the TxopLimit.
   */
  virtual void StartNextPacket (void);
  /**
   * Event handler when a transmission that
   * does not require an ACK has completed.
   */
  virtual void EndTxNoAck (void);

  /**
   * Check if the station has TXOP granted for the next MPDU.
   *
   * \return true if the station has TXOP granted for the next MPDU,
   *         false otherwise
   */
  virtual bool HasTxop (void) const;

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model. Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use.
   *
   * \return the number of stream indices assigned by this model.
   */
  int64_t AssignStreams (int64_t stream);


protected:
  ///< DcfState associated class
  friend class DcfState;

  virtual void DoDispose (void);
  virtual void DoInitialize (void);

  /* dcf notifications forwarded here */
  /**
   * Check if the DCF requires access.
   *
   * \return true if the DCF requires access,
   *         false otherwise
   */
  virtual bool NeedsAccess (void) const;

  /**
   * Notify the DCF that access has been granted.
   */
  virtual void NotifyAccessGranted (void);
  /**
   * Notify the DCF that internal collision has occurred.
   */
  virtual void NotifyInternalCollision (void);
  /**
   * Notify the DCF that collision has occurred.
   */
  virtual void NotifyCollision (void);

  /**
   * Restart access request if needed.
   */
  virtual void RestartAccessIfNeeded (void);
  /**
   * Request access from DCF manager if needed.
   */
  virtual void StartAccessIfNeeded (void);

  /**
   * Check if RTS should be re-transmitted if CTS was missed.
   *
   * \param packet current packet being transmitted.
   * \param hdr current header being transmitted.
   * \return true if RTS should be re-transmitted,
   *         false otherwise.
   */
  bool NeedRtsRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  /**
   * Check if DATA should be re-transmitted if ACK was missed.
   *
   * \param packet current packet being transmitted.
   * \param hdr current header being transmitted.
   * \return true if DATA should be re-transmitted,
   *         false otherwise.
   */
  bool NeedDataRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  /**
   * Check if the current packet should be fragmented.
   *
   * \return true if the current packet should be fragmented,
   *         false otherwise
   */
  virtual bool NeedFragmentation (void) const;

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
   * \param hdr Wi-Fi header.
   *
   * \return the fragment with the current fragment number.
   */
  virtual Ptr<Packet> GetFragmentPacket (WifiMacHeader *hdr);
  /**
   * Calculate the size of the next fragment.
   *
   * \return the size of the next fragment.
   */
  virtual uint32_t GetNextFragmentSize (void) const;
  /**
   * Calculate the size of the current fragment.
   *
   * \return the size of the current fragment.
   */
  virtual uint32_t GetFragmentSize (void) const;
  /**
   * Calculate the offset for the current fragment.
   *
   * \return the offset for the current fragment.
   */
  virtual uint32_t GetFragmentOffset (void) const;
  /**
   * Check if the current fragment is the last fragment.
   *
   * \return true if the current fragment is the last fragment,
   *         false otherwise.
   */
  virtual bool IsLastFragment (void) const;
  /**
   *
   * Pass the packet included in the wifi MAC queue item to the
   * packet dropped callback.
   *
   * \param item the wifi MAC queue item.
   */
  void TxDroppedPacket (Ptr<const WifiMacQueueItem> item);

  Ptr<DcfState> m_dcf; //!< the DCF state
  Ptr<DcfManager> m_manager; //!< the DCF manager
  TxOk m_txOkCallback; //!< the transmit OK callback
  TxFailed m_txFailedCallback; //!< the transmit failed callback
  TxDropped m_txDroppedCallback; //!< the packet dropped callback
  Ptr<WifiMacQueue> m_queue; //!< the wifi MAC queue
  Ptr<MacTxMiddle> m_txMiddle; //!< the MacTxMiddle
  Ptr <MacLow> m_low; //!< the MacLow
  Ptr<WifiRemoteStationManager> m_stationManager; //!< the wifi remote station manager
  Ptr<UniformRandomVariable> m_rng; //!<  the random stream

  Ptr<const Packet> m_currentPacket; //!< the current packet
  WifiMacHeader m_currentHdr; //!< the current header
  MacLowTransmissionParameters m_currentParams; ///< current transmission parameters
  uint8_t m_fragmentNumber; //!< the fragment number
};

} //namespace ns3

#endif /* DCA_TXOP_H */
