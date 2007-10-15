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
#ifndef MAC_HIGH_ADHOC_H
#define MAC_HIGH_ADHOC_H

#include "ns3/mac48-address.h"
#include "ns3/callback.h"
#include "ns3/packet.h"

namespace ns3 {

class DcaTxop;
class Packet;
class WifiNetDevice;
class WifiMacHeader;

class MacHighAdhoc {
public:
  typedef Callback<void, Packet, const Mac48Address &> ForwardCallback;

  MacHighAdhoc ();
  ~MacHighAdhoc ();

  void SetDevice (WifiNetDevice *device);
  void SetForwardCallback (ForwardCallback callback);
  void SetDcaTxop (DcaTxop *dca);

  Mac48Address GetBssid (void) const;

  void Enqueue (Packet packet, Mac48Address to);

  /* invoked by the MacLows. */
  void Receive (Packet packet, WifiMacHeader const*hdr);
private:
  DcaTxop *m_dca;
  WifiNetDevice *m_device;
  ForwardCallback m_callback;
};

} // namespace ns3

#endif /* MAC_HIGH_ADHOC_H */
