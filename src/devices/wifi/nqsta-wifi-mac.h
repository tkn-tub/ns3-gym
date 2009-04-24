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
#ifndef NQSTA_WIFI_MAC_H
#define NQSTA_WIFI_MAC_H

#include <stdint.h>

#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/traced-callback.h"

#include "wifi-mac.h"
#include "supported-rates.h"

namespace ns3 {

class WifiMacHeader;
class DcaTxop;
class WifiPhy;
class DcfManager;
class MacLow;
class MacRxMiddle;

/**
 * \brief a non-QoS STA state machine
 *
 * This state machine handles association, disassociation,
 * authentication and beacon monitoring. It does not perform
 * channel scanning.
 * If the station detects a certain number of missed beacons
 * while associated, it automatically attempts a new association
 * sequence.
 */
class NqstaWifiMac : public WifiMac
{
public:
  static TypeId GetTypeId (void);

  NqstaWifiMac ();
  ~NqstaWifiMac ();

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
  virtual bool SupportsSendFrom (void) const;
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback);
  virtual void SetLinkUpCallback (Callback<void> linkUp);
  virtual void SetLinkDownCallback (Callback<void> linkDown);
  virtual Mac48Address GetAddress (void) const;
  virtual Ssid GetSsid (void) const;
  virtual void SetAddress (Mac48Address address);
  virtual void SetSsid (Ssid ssid);
  virtual Mac48Address GetBssid (void) const;

  /**
   * \param missed the number of beacons which must be missed
   * before a new association sequence is started.
   */
  void SetMaxMissedBeacons (uint32_t missed);
  /**
   * \param timeout
   *
   * If no probe response is received within the specified
   * timeout, the station sends a new probe request.
   */
  void SetProbeRequestTimeout (Time timeout);
  /**
   * \param timeout
   *
   * If no association response is received within the specified
   * timeout, the station sends a new association request.
   */
  void SetAssocRequestTimeout (Time timeout);

  /**
   * Start an active association sequence immediately.
   */
  void StartActiveAssociation (void);

private:
  enum MacState{
    ASSOCIATED,
    WAIT_PROBE_RESP,
    WAIT_ASSOC_RESP,
    BEACON_MISSED,
    REFUSED
  };
  void SetBssid (Mac48Address bssid);
  void SetActiveProbing (bool enable);
  bool GetActiveProbing (void) const;
  void ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to);
  void Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
  Mac48Address GetBroadcastBssid (void);
  void SendProbeRequest (void);
  void SendAssociationRequest (void);
  void TryToEnsureAssociated (void);
  void AssocRequestTimeout (void);
  void ProbeRequestTimeout (void);
  bool IsAssociated (void);
  void MissedBeacons (void);
  void RestartBeaconWatchdog (Time delay);
  SupportedRates GetSupportedRates (void) const;
  virtual void DoDispose (void);
  NqstaWifiMac (const NqstaWifiMac & ctor_arg);
  NqstaWifiMac &operator = (const NqstaWifiMac & ctor_arg);
  Ptr<DcaTxop> GetDcaTxop(void) const;
  void SetDcaTxop (Ptr<DcaTxop> dcaTxop);
  void SetState (enum MacState value);

  enum MacState m_state;
  Time m_probeRequestTimeout;
  Time m_assocRequestTimeout;
  EventId m_probeRequestEvent;
  EventId m_assocRequestEvent;
  Callback<void, Ptr<Packet>, Mac48Address, Mac48Address> m_forwardUp;
  Callback<void> m_linkUp;
  Callback<void> m_linkDown;
  Ptr<DcaTxop> m_dca;
  EventId m_beaconWatchdog;
  Time m_beaconWatchdogEnd;
  uint32_t m_maxMissedBeacons;

  Ptr<WifiPhy> m_phy;
  Ptr<WifiRemoteStationManager> m_stationManager;
  DcfManager *m_dcfManager;
  MacRxMiddle *m_rxMiddle;
  Ptr<MacLow> m_low;
  Ssid m_ssid;
  Time m_eifsNoDifs;

  TracedCallback<Mac48Address> m_assocLogger;
  TracedCallback<Mac48Address> m_deAssocLogger;
};

} // namespace ns3


#endif /* NQSTA_WIFI_MAC_H */
