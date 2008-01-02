/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise
 * All rights reserved.
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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */
#include "packet-socket-factory.h"
#include "node.h"
#include "packet-socket.h"

namespace ns3 {

const InterfaceId PacketSocketFactory::iid = MakeInterfaceId ("Packet", 
                                                              SocketFactory::iid);

PacketSocketFactory::PacketSocketFactory ()
{}

Ptr<Socket> PacketSocketFactory::CreateSocket (void)
{
  Ptr<Node> node = QueryInterface<Node> (Node::iid);
  Ptr<PacketSocket> socket = CreateObject<PacketSocket> (node);
  return socket;
} 
} // namespace ns3
