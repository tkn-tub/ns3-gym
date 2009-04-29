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
#ifndef QADHOC_WIFI_MAC_H
#define QADHOC_WIFI_MAC_H

#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/packet.h"

#include "wifi-mac.h"
#include "qos-utils.h"
#include "amsdu-subframe-header.h"

namespace ns3 {

class EdcaTxopN;
class WifiMacHeader;
class WifiPhy;
class DcfManager;
class MacLow;
class MacRxMiddle;

class QadhocWifiMac : public WifiMac
{
public:
  static TypeId GetTypeId (void);

  QadhocWifiMac ();
  ~QadhocWifiMac ();

  // all inherited from WifiMac base class.
  virtual void SetSlot (Time slotTime);
  virtual void SetSifs (Time sifs);
  virtual void SetEifsNoDifs (Time eifsNoDifs);
  virtual void SetAckTimeout (Time ackTimeout);
  virtual void SetCtsTimeout (Time ctsTimeout);
  virtual void SetPifs (Time pifs);
  virtual Time GetSlot (void) const;
  virtual Time GetSifs (void) const;
  virtual Time GetEifsNoDifs (void) const;
  virtual Time GetAckTimeout (void) const;
  virtual Time GetCtsTimeout (void) const;
  virtual Time GetPifs (void) const;
  virtual void SetWifiPhy (Ptr<WifiPhy> phy);
  virtual void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager);
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from);
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to);
  virtual bool SupportsSendFrom (void) const;
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback);
  virtual void SetLinkUpCallback (Callback<void> linkUp);
  virtual void SetLinkDownCallback (Callback<void> linkDown);
  virtual Mac48Address GetAddress (void) const;
  virtual Ssid GetSsid (void) const;
  virtual void SetAddress (Mac48Address address);
  virtual void SetSsid (Ssid ssid);
  virtual Mac48Address GetBssid (void) const;

private:
  Callback<void, Ptr<Packet>, Mac48Address, Mac48Address> m_forwardUp;
  virtual void DoDispose (void);
  void Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
  void ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to);
  QadhocWifiMac &operator = (const QadhocWifiMac &);
  QadhocWifiMac (const QadhocWifiMac &);

  /**
  * When an A-MSDU is received, is deaggregated by this method and all extracted packets are
  * forwarded up.
  */
  void DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket, WifiMacHeader const *hdr);

  typedef std::map<AccessClass, Ptr<EdcaTxopN> > Queues;
  typedef std::list<std::pair<Ptr<Packet>, AmsduSubframeHeader> > DeaggregatedMsdus;
  typedef std::list<std::pair<Ptr<Packet>, AmsduSubframeHeader> >::const_iterator DeaggregatedMsdusCI;

  Ptr<EdcaTxopN> GetVOQueue (void) const;
  Ptr<EdcaTxopN> GetVIQueue (void) const;
  Ptr<EdcaTxopN> GetBEQueue (void) const;
  Ptr<EdcaTxopN> GetBKQueue (void) const;

  void SetVOQueue (Ptr<EdcaTxopN> voQueue);
  void SetVIQueue (Ptr<EdcaTxopN> viQueue);
  void SetBEQueue (Ptr<EdcaTxopN> beQueue);
  void SetBKQueue (Ptr<EdcaTxopN> bkQueue);

  Queues m_queues;
  Ptr<EdcaTxopN> m_voEdca;
  Ptr<EdcaTxopN> m_viEdca;
  Ptr<EdcaTxopN> m_beEdca;
  Ptr<EdcaTxopN> m_bkEdca;
  Ptr<MacLow> m_low;
  Ptr<WifiPhy> m_phy;
  Ptr<WifiRemoteStationManager> m_stationManager;
  MacRxMiddle *m_rxMiddle;
  MacTxMiddle *m_txMiddle;
  DcfManager *m_dcfManager;
  Ssid m_ssid;
  Time m_eifsNoDifs;
};

} //namespace ns3

#endif /* QADHOC_WIFI_MAC_H */
