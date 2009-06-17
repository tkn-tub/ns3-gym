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

#ifndef FLAME_PROTOCOL_H
#define FLAME_PROTOCOL_H

#include "ns3/mesh-l2-routing-protocol.h"
namespace ns3 {
namespace flame {
/**
 * \ingroup flame
 * 
 * \brief FLAME = Forwarding Layer for Meshing
 */

class FlameProtocol : public MeshL2RoutingProtocol
{
public:
  static TypeId GetTypeId ();
  FlameProtocol ();
  ~FlameProtocol ();
  void DoDispose ();
  
  /// Route request, inherited from MeshL2RoutingProtocol
  bool RequestRoute (uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
      Ptr<const Packet>  packet, uint16_t  protocolType, RouteReplyCallback  routeReply);
  /** 
   * \brief Install FLAME on given mesh point. 
   * 
   * Installing protocol cause installing its interface MAC plugins.
   *  
   * Also MP aggregates all installed protocols, FLAME protocol can be accessed 
   * via MeshPointDevice::GetObject<dot11s::FlameProtocol>();
   */
  bool Install (Ptr<MeshPointDevice>);
  ///\brief Statistics:
  void Report (std::ostream &) const;
  void ResetStats ();
private:
};
} //namespace flame
} //namespace ns3
#endif /* FLAME_PROTOCOL_H */
