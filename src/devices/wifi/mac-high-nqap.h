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

namespace ns3 {

class WifiMacHeader;
class WifiNetDevice;
class DcaTxop;
class MacStations;
class WifiPhy;

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
class MacHighNqap {
public:
  typedef Callback<void, Ptr<Packet>, const Mac48Address &> ForwardCallback;

  MacHighNqap ();
  ~MacHighNqap ();

  void SetDcaTxop (Ptr<DcaTxop> dca);
  void SetBeaconDcaTxop (Ptr<DcaTxop> dca);
  void SetDevice (WifiNetDevice *device);
  void SetStations (MacStations *stations);
  void SetPhy (Ptr<WifiPhy> phy);
  void SetForwardCallback (ForwardCallback callback);
  void SetBeaconInterval (Time interval);

  void Queue (Ptr<const Packet> packet, Mac48Address to);

  /**
   * Start beacon transmission immediately.
   */
  void StartBeaconing (void);

  void Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
private:
  void ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to);
  void TxOk (WifiMacHeader const &hdr);
  void TxFailed (WifiMacHeader const &hdr);
  void SendProbeResp (Mac48Address to);
  void SendAssocResp (Mac48Address to, bool success);
  void SendOneBeacon (void);
  SupportedRates GetSupportedRates (void) const;

  Ptr<DcaTxop> m_dca;
  Ptr<DcaTxop> m_beaconDca;
  WifiNetDevice *m_device;
  MacStations *m_stations;
  Ptr<WifiPhy> m_phy;
  ForwardCallback m_forwardUp;
  Time m_beaconInterval;
};

} // namespace ns3


#endif /* MAC_HIGH_NQAP_H */
