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

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/wifi-mode.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/dcf.h"

namespace ns3 {

class DcfState;
class DcfManager;
class WifiMacQueue;
class MacLow;
class WifiMacParameters;
class MacTxMiddle;
class RandomStream;
class MacStation;
class MacStations;

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
class DcaTxop : public Dcf
{
public:
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

  DcaTxop ();
  ~DcaTxop ();

  /**
   * Set MacLow associated with this DcaTxop.
   *
   * \param low MacLow
   */
  void SetLow (Ptr<MacLow> low);
  /**
   * Set DcfManager this DcaTxop is associated to.
   *
   * \param manager DcfManager
   */
  void SetManager (DcfManager *manager);
  /**
   * Set WifiRemoteStationsManager this DcaTxop is associated to.
   *
   * \param remoteManager WifiRemoteStationManager
   */
  void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> remoteManager);

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
   * Return the packet queue associated with this DcaTxop.
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
   * \param packet packet to send
   * \param hdr header of packet to send.
   *
   * Store the packet in the internal queue until it
   * can be sent safely.
   */
  void Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr);

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
  class TransmissionListener;
  class NavListener;
  class PhyListener;
  class Dcf;
  friend class Dcf;
  friend class TransmissionListener;

  DcaTxop &operator = (const DcaTxop &);
  DcaTxop (const DcaTxop &o);

  // Inherited from ns3::Object
  /**
   * Return the MacLow associated with this DcaTxop.
   *
   * \return MacLow
   */
  Ptr<MacLow> Low (void);
  void DoInitialize ();
  /* dcf notifications forwarded here */
  /**
   * Check if the DCF requires access.
   *
   * \return true if the DCF requires access,
   *         false otherwise
   */
  bool NeedsAccess (void) const;
  /**
   * Notify the DCF that access has been granted.
   */
  void NotifyAccessGranted (void);
  /**
   * Notify the DCF that internal collision has occurred.
   */
  void NotifyInternalCollision (void);
  /**
   * Notify the DCF that collision has occurred.
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
   * \param packet
   * \param header
   * \return true if RTS protection should be used, false otherwise
   */
  bool NeedRts (Ptr<const Packet> packet, const WifiMacHeader *header);
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
  bool NeedFragmentation (void);
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
  bool IsLastFragment (void);
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
  virtual void DoDispose (void);

  Dcf *m_dcf;
  DcfManager *m_manager;
  TxOk m_txOkCallback;
  TxFailed m_txFailedCallback;
  Ptr<WifiMacQueue> m_queue;
  MacTxMiddle *m_txMiddle;
  Ptr <MacLow> m_low;
  Ptr<WifiRemoteStationManager> m_stationManager;
  TransmissionListener *m_transmissionListener;
  RandomStream *m_rng;

  bool m_accessOngoing;
  Ptr<const Packet> m_currentPacket;
  WifiMacHeader m_currentHdr;
  uint8_t m_fragmentNumber;
};

} // namespace ns3



#endif /* DCA_TXOP_H */
