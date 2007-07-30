/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2007 Emmanuelle Laprise
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
//

//
// Topology helper for CsmaCd channels in ns3.

#include "ns3/assert.h"
#include "ns3/debug.h"
#include "ns3/queue.h"

#include "csma-cd-channel.h"
#include "csma-cd-net-device.h"
#include "csma-cd-topology.h"
#include "ns3/socket-factory.h"

namespace ns3 {

Ptr<CsmaCdChannel>
CsmaCdTopology::CreateCsmaCdChannel(
  const DataRate& bps,
  const Time& delay)
{
  Ptr<CsmaCdChannel> channel = Create<CsmaCdChannel> (bps, delay);

  return channel;
}

#if 0
Ptr<CsmaCdNetDevice>
CsmaCdTopology::AddCsmaCdEthernetNode(
  Ptr<Node> n1,
  Ptr<CsmaCdChannel> ch,
  MacAddress addr)
{
  Ptr<CsmaCdNetDevice> nd1 = Create<CsmaCdNetDevice> (n1, addr, 
                                                      ns3::CsmaCdNetDevice::ETHERNET_V1);

  Ptr<Queue> q = Queue::CreateDefault ();
  nd1->AddQueue(q);
  nd1->Attach (ch);
  
  return nd1;
}

Ptr<PacketSocket>
CsmaCdTopology::ConnectPacketSocket(Ptr<PacketSocketApp> app, 
                                        Ptr<CsmaCdNetDevice> ndSrc,
                                        Ptr<CsmaCdNetDevice> ndDest)
{
  Ptr<PacketSocket> socket = Create<PacketSocket> ();
  socket->Bind(ndSrc);
  socket->Connect(ndDest->GetAddress());
  app->Connect(socket);

  return socket;
}

Ptr<PacketSocket>
CsmaCdTopology::ConnectPacketSocket(Ptr<PacketSocketApp> app,
                                        Ptr<CsmaCdNetDevice> ndSrc,
                                        MacAddress macAddr)
{
  Ptr<PacketSocket> socket = Create<PacketSocket> ();
  socket->Bind(ndSrc);
  socket->Connect(macAddr);
  app->Connect(socket);

  return socket;
}

Ptr<Socket>
CsmaCdTopology::CreatePacketSocket(Ptr<Node> n1, std::string iid_name)
{
  InterfaceId iid = InterfaceId::LookupByName (iid_name);

  Ptr<SocketFactory> socketFactory =
    n1->QueryInterface<SocketFactory> (iid);

  Ptr<Socket> socket = socketFactory->CreateSocket ();

  return socket;
}
#endif

} // namespace ns3
 
