/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef FLAME_PROTOCOL_MAC_H
#define FLAME_PROTOCOL_MAC_H

#include "ns3/mesh-wifi-interface-mac.h"
#include "flame-protocol.h"

namespace ns3 {
namespace flame {
/**
 * \ingroup flame
 * 
 * \brief Interface MAC plugin FLAME routing protocol
 */
class FlameMacPlugin : public MeshWifiInterfaceMacPlugin
{
public:
  FlameMacPlugin (uint32_t, Ptr<FlameProtocol>);
  ~FlameMacPlugin ();
  ///\name Inherited from MAC plugin
  //\{
  void SetParent (Ptr<MeshWifiInterfaceMac> parent);
  bool Receive (Ptr<Packet> packet, const WifiMacHeader & header);
  bool UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to);
  /// Update beacon is empty, because HWMP does not know anything about beacons
  void UpdateBeacon (MeshWifiBeacon & beacon) const {};
  //\}
  
private:

};
} //namespace flame
} //namespace ns3
#endif /* FLAME_PROTOCOL_MAC_H */
