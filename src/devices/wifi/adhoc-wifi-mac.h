/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef ADHOC_WIFI_MAC_H
#define ADHOC_WIFI_MAC_H

#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "wifi-mac.h"

namespace ns3 {

class DcaTxop;
class Packet;
class WifiMacHeader;
class WifiPhy;
class DcfManager;
class MacLow;
class MacRxMiddle;

/**
 * \brief the Adhoc state machine
 *
 * For now, this class is really empty but it should contain
 * the code for the distributed generation of beacons in an adhoc 
 * network.
 */
class AdhocWifiMac : public WifiMac 
{
public:
  typedef Callback<void, Ptr<Packet>, const Mac48Address &> ForwardCallback;

  static TypeId GetTypeId (void);

  AdhocWifiMac ();
  ~AdhocWifiMac ();

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


private:
  // inherited from Object base class.
  virtual void DoDispose (void);
  /* invoked by the MacLows. */
  void ForwardUp (Ptr<Packet> packet, WifiMacHeader const*hdr);
  AdhocWifiMac (const AdhocWifiMac & ctor_arg);
  AdhocWifiMac &operator = (const AdhocWifiMac &o);
  Mac48Address GetBssid (void) const;

  Ptr<DcaTxop> m_dca;
  Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> m_upCallback;
  Ptr<WifiRemoteStationManager> m_stationManager;
  Ptr<WifiPhy> m_phy;
  DcfManager *m_dcfManager;
  MacRxMiddle *m_rxMiddle;
  Ptr<MacLow> m_low;
  Ssid m_ssid;
  Time m_eifsNoDifs;
};

} // namespace ns3

#endif /* ADHOC_WIFI_MAC_H */
