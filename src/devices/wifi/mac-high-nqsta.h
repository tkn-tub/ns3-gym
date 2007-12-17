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
#ifndef MAC_HIGH_NQSTA_H
#define MAC_HIGH_NQSTA_H

#include <stdint.h>

#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"

#include "supported-rates.h"

namespace ns3 {

class WifiMacHeader;
class WifiNetDevice;
class DcaTxop;
class WifiPhy;
class MacStations;

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
class MacHighNqsta {
public:
  typedef Callback<void, Ptr<Packet>, const Mac48Address &> ForwardCallback;
  typedef Callback<void> AssociatedCallback;
  typedef Callback<void> DisAssociatedCallback;

  MacHighNqsta ();
  ~MacHighNqsta ();

  void SetDcaTxop (DcaTxop *dca);
  void SetDevice (WifiNetDevice *device);
  void SetForwardCallback (ForwardCallback callback);
  void SetAssociatedCallback (AssociatedCallback callback);
  void SetDisAssociatedCallback (DisAssociatedCallback callback);
  void SetPhy (Ptr<WifiPhy> phy);
  void SetStations (MacStations *stations);

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

  Mac48Address GetBssid (void) const;

  /**
   * Start an active association sequence immediately.
   */
  void StartActiveAssociation (void);

  void Queue (Ptr<const Packet> packet, Mac48Address to);

  void Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
private:
  void SetBssid (Mac48Address bssid);
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
  enum {
    ASSOCIATED,
    WAIT_PROBE_RESP,
    WAIT_ASSOC_RESP,
    BEACON_MISSED,
    REFUSED
  } m_state;
  Time m_probeRequestTimeout;
  Time m_assocRequestTimeout;
  EventId m_probeRequestEvent;
  EventId m_assocRequestEvent;
  WifiNetDevice *m_device;
  ForwardCallback m_forward;
  AssociatedCallback m_associatedCallback;
  DisAssociatedCallback m_disAssociatedCallback;
  DcaTxop *m_dca;
  EventId m_beaconWatchdog;
  Time m_beaconWatchdogEnd;
  Mac48Address m_bssid;
  uint32_t m_maxMissedBeacons;
  Ptr<WifiPhy> m_phy;
  MacStations *m_stations;
};

} // namespace ns3


#endif /* MAC_HIGH_NQSTA_H */
