/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "wifi-mac.h"
#include "wifi-mac-header.h"
#include "qos-utils.h"

#include <map>
#include <list>

namespace ns3 {

class DcfState;
class DcfManager;
class MacLow;
class MacTxMiddle;
class WifiMacParameters;
class WifiMacQueue;
class RandomStream;
class MsduAggregator;

/* This queue contains packets for a particular access class.
 * possibles access classes are:
 *   
 *   -AC_VO : voice, tid = 6,7         ^
 *   -AC_VI : video, tid = 4,5         |
 *   -AC_BE : best-effort, tid = 0,3   |  priority  
 *   -AC_BK : background, tid = 1,2    |
 * 
 * For more details see section 9.1.3.1 in 802.11 standard.
 */
enum TypeOfStation
{
  STA,
  AP,
  ADHOC_STA
};

class EdcaTxopN : public Object
{
public:

  typedef Callback <void, WifiMacHeader const&> TxOk;
  typedef Callback <void, WifiMacHeader const&> TxFailed;
  
  static TypeId GetTypeId (void);
  EdcaTxopN ();
  virtual ~EdcaTxopN ();
  void DoDispose ();
  
  void SetLow (Ptr<MacLow> low);
  void SetTxMiddle (MacTxMiddle *txMiddle);
  void SetManager (DcfManager *manager);
  void SetTxOkCallback (TxOk callback);
  void SetTxFailedCallback (TxFailed callback);
  void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> remoteManager);
  void SetTypeOfStation (enum TypeOfStation type);
  enum TypeOfStation GetTypeOfStation (void) const;

  void SetMaxQueueSize (uint32_t size);
  void SetMaxQueueDelay (Time delay);
  void SetMinCw (uint32_t minCw);
  void SetMaxCw (uint32_t maxCw);
  void SetAifsn (uint32_t aifsn);
  uint32_t GetMinCw (void) const;
  uint32_t GetMaxCw (void) const;
  uint32_t GetAifsn (void) const;

  Ptr<MacLow> Low (void);
  Ptr<MsduAggregator> GetMsduAggregator (void) const;

  /* dcf notifications forwarded here */
  bool NeedsAccess (void) const;
  void NotifyAccessGranted (void);
  void NotifyInternalCollision (void);
  void NotifyCollision (void);

  /*event handlers*/
  void GotCts (double snr, WifiMode txMode);
  void MissedCts (void);
  void GotAck (double snr, WifiMode txMode);
  void MissedAck (void);
  void StartNext (void);
  void Cancel (void);

  void RestartAccessIfNeeded (void);
  void StartAccessIfNeeded (void);
  bool NeedRts (void);
  bool NeedRtsRetransmission (void);
  bool NeedDataRetransmission (void);
  bool NeedFragmentation (void) const;
  uint32_t GetNextFragmentSize (void);
  uint32_t GetFragmentSize (void);
  uint32_t GetFragmentOffset (void);
  WifiRemoteStation *GetStation (Mac48Address to) const;
  bool IsLastFragment (void) const;
  void NextFragment (void);
  Ptr<Packet> GetFragmentPacket (WifiMacHeader *hdr);
  
  void Queue (Ptr<const Packet> packet, WifiMacHeader const &hdr);
  void SetMsduAggregator (Ptr<MsduAggregator> aggr);

private:
  /**
   * This functions are used only to correctly set addresses in a-msdu subframe.
   * If aggregating sta is a STA (in an infrastructured network):
   *   SA = Address2
   *   DA = Address3
   * If aggregating sta is an AP
   *   SA = Address3
   *   DA = Address1
   */
  Mac48Address MapSrcAddressForAggregation (WifiMacHeader const &hdr);
  Mac48Address MapDestAddressForAggregation (WifiMacHeader const &hdr);
  
  class Dcf;
  class TransmissionListener;
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
};

}  //namespace ns3

#endif /* EDCA_TXOP_N_H */
