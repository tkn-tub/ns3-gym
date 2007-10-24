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
#include "supported-rates.h"

namespace ns3 {

class WifiMacHeader;
class WifiNetDevice;
class DcaTxop;
class MacStations;
class WifiPhy;

class MacHighNqap {
public:
  typedef Callback<void, Packet, const Mac48Address &> ForwardCallback;

  MacHighNqap ();
  ~MacHighNqap ();

  void SetDcaTxop (DcaTxop *dca);
  void SetDevice (WifiNetDevice *device);
  void SetStations (MacStations *stations);
  void SetPhy (WifiPhy *phy);
  void SetForwardCallback (ForwardCallback callback);
  void SetBeaconIntervalUs (uint64_t us);

  void Queue (Packet packet, Mac48Address to);

  void Receive (Packet packet, WifiMacHeader const *hdr);
private:
  void ForwardDown (Packet packet, Mac48Address from, Mac48Address to);
  void TxOk (WifiMacHeader const &hdr);
  void TxFailed (WifiMacHeader const &hdr);
  void SendProbeResp (Mac48Address to);
  void SendAssocResp (Mac48Address to, bool success);
  SupportedRates GetSupportedRates (void) const;

  DcaTxop *m_dca;
  WifiNetDevice *m_device;
  MacStations *m_stations;
  WifiPhy *m_phy;
  ForwardCallback m_forwardUp;
  uint64_t m_beaconIntervalUs;
};

} // namespace ns3


#endif /* MAC_HIGH_NQAP_H */
