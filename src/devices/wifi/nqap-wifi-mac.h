/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#ifndef MAC_HIGH_NQAP_H
#define MAC_HIGH_NQAP_H

#include <stdint.h>
#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "supported-rates.h"
#include "wifi-remote-station-manager.h"
#include "wifi-mac.h"

namespace ns3 {

class WifiMacHeader;
class DcaTxop;
class WifiPhy;
class DcfManager;
class MacRxMiddle;
class MacLow;

/**
 * \brief non-QoS AP state machine
 *
 * Handle association, dis-association and authentication,
 * of STAs within an IBSS.
 * This class uses two output queues, each of which is server by
 * a single DCF
 *   - the highest priority DCF serves the queue which contains
 *     only beacons.
 *   - the lowest priority DCF serves the queue which contains all
 *     other frames, including user data frames.
 */
class NqapWifiMac : public WifiMac
{
public:
  static TypeId GetTypeId (void);

  NqapWifiMac ();
  ~NqapWifiMac ();

  // inherited from WifiMac.
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
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, const Mac48Address &> upCallback);
  virtual void SetLinkUpCallback (Callback<void> linkUp);
  virtual void SetLinkDownCallback (Callback<void> linkDown);
  virtual Mac48Address GetAddress (void) const;
  virtual Ssid GetSsid (void) const;
  virtual void SetAddress (Mac48Address address);
  virtual void SetSsid (Ssid ssid);


  /**
   * \param interval the interval between two beacon transmissions.
   */
  void SetBeaconInterval (Time interval);
  /**
   * \returns the interval between two beacon transmissions.
   */
  Time GetBeaconInterval (void) const;
  /**
   * Start beacon transmission immediately.
   */
  void StartBeaconing (void);

private:
  void Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
  void ForwardUp (Ptr<Packet> packet, Mac48Address from);
  void ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to);
  void TxOk (WifiMacHeader const &hdr);
  void TxFailed (WifiMacHeader const &hdr);
  void SendProbeResp (Mac48Address to);
  void SendAssocResp (Mac48Address to, bool success);
  void SendOneBeacon (void);
  SupportedRates GetSupportedRates (void) const;
  void SetBeaconGeneration (bool enable);
  bool GetBeaconGeneration (void) const;
  virtual void DoDispose (void);
  NqapWifiMac (const NqapWifiMac & ctor_arg);

  Ptr<DcaTxop> m_dca;
  Ptr<DcaTxop> m_beaconDca;
  Ptr<WifiRemoteStationManager> m_stationManager;
  Ptr<WifiPhy> m_phy;
  Callback<void, Ptr<Packet>,const Mac48Address &> m_upCallback;
  Time m_beaconInterval;

  DcfManager *m_dcfManager;
  MacRxMiddle *m_rxMiddle;
  Ptr<MacLow> m_low;
  Ssid m_ssid;
  EventId m_beaconEvent;
  Time m_eifsNoDifs;
};

} // namespace ns3


#endif /* MAC_HIGH_NQAP_H */
