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
class Watchdog;

class MacHighNqsta {
public:
  typedef Callback<void, Packet > ForwardCallback;
  typedef Callback<void> AssociatedCallback;
  typedef Callback<void> DisAssociatedCallback;

  MacHighNqsta ();
  ~MacHighNqsta ();

  void SetDcaTxop (DcaTxop *dca);
  void SetInterface (WifiNetDevice *interface);
  void SetForwardCallback (ForwardCallback callback);
  void SetAssociatedCallback (AssociatedCallback callback);
  void SetDisAssociatedCallback (DisAssociatedCallback callback);
  void SetSupportedRates (SupportedRates rates);

  void SetMaxMissedBeacons (uint32_t missed);
  void SetProbeRequestTimeout (Time timeout);
  void SetAssocRequestTimeout (Time timeout);

  Mac48Address GetBssid (void) const;

  void StartActiveAssociation (void);

  void Queue (Packet packet, Mac48Address to);

  void Receive (Packet packet, WifiMacHeader const *hdr);
private:
  void SetBssid (Mac48Address bssid);
  Mac48Address GetBroadcastBssid (void);
  void SendProbeRequest (void);
  void SendAssociationRequest ();
  void TryToEnsureAssociated (void);
  void AssocRequestTimeout (void);
  void ProbeRequestTimeout (void);
  bool IsAssociated (void);
  SupportedRates GetSupportedRates (void);
  void MissedBeacons (void);
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
  WifiNetDevice *m_interface;
  ForwardCallback m_forward;
  AssociatedCallback m_associatedCallback;
  DisAssociatedCallback m_disAssociatedCallback;
  SupportedRates m_rates;
  DcaTxop *m_dca;
  Watchdog *m_beaconWatchdog;
  Mac48Address m_bssid;
  uint32_t m_maxMissedBeacons;
};

} // namespace ns3


#endif /* MAC_HIGH_NQSTA_H */
