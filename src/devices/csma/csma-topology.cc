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
// Topology helper for Csma channels in ns3.

#include "ns3/assert.h"
#include "ns3/queue.h"

#include "csma-channel.h"
#include "csma-net-device.h"
#include "csma-topology.h"
#include "ns3/socket-factory.h"

namespace ns3 {

Ptr<CsmaChannel>
CsmaTopology::CreateCsmaChannel(
  const DataRate& bps,
  const Time& delay)
{
  Ptr<CsmaChannel> channel = CreateObject<CsmaChannel> (bps, delay);

  return channel;
}

#if 0
Ptr<CsmaNetDevice>
CsmaTopology::AddCsmaEthernetNode(
  Ptr<Node> n1,
  Ptr<CsmaChannel> ch,
  Mac48Address addr)
{
  Ptr<CsmaNetDevice> nd1 = CreateObjectWith<CsmaNetDevice> ("Node", Ptr<Node> (n1), 
                                                            "Address", addr, 
                                                            "EncapsulationMode", "EthernetV1");

  Ptr<Queue> q = Queue::CreateDefault ();
  nd1->AddQueue(q);
  nd1->Attach (ch);
  
  return nd1;
}

Ptr<PacketSocket>
CsmaTopology::ConnectPacketSocket(Ptr<PacketSocketApp> app, 
                                  Ptr<CsmaNetDevice> ndSrc,
                                  Ptr<CsmaNetDevice> ndDest)
{
  Ptr<PacketSocket> socket = CreateObject<PacketSocket> ();
  socket->Bind(ndSrc);
  socket->Connect(ndDest->GetAddress());
  app->Connect(socket);

  return socket;
}

Ptr<PacketSocket>
CsmaTopology::ConnectPacketSocket(Ptr<PacketSocketApp> app,
                                        Ptr<CsmaNetDevice> ndSrc,
                                        MacAddress macAddr)
{
  Ptr<PacketSocket> socket = CreateObject<PacketSocket> ();
  socket->Bind(ndSrc);
  socket->Connect(macAddr);
  app->Connect(socket);

  return socket;
}

Ptr<Socket>
CsmaTopology::CreatePacketSocket(Ptr<Node> n1, std::string tid_name)
{
  TypeId tid = TypeId::LookupByName (tid_name);

  Ptr<SocketFactory> socketFactory =
    n1->GetObject<SocketFactory> (tid);

  Ptr<Socket> socket = socketFactory->CreateSocket ();

  return socket;
}
#endif

} // namespace ns3
 
