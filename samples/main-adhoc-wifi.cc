/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#include "ns3/wifi-net-device.h"
#include "ns3/wifi-net-device-factory.h"
#include "ns3/wifi-channel.h"
#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/internet-node.h"
#include "ns3/onoff-application.h"
#include "ns3/static-mobility-model.h"
#include "ns3/ipv4.h"
#include "ns3/random-variable.h"
#include "ns3/inet-socket-address.h"


#include <iostream>

using namespace ns3;

static Ptr<Node>
CreateAdhocNode (Ptr<WifiNetDeviceFactory> factory, Ptr<WifiChannel> channel,
                 Position position, const char *address)
{
  Ptr<Node> node = Create<InternetNode> ();  
  Ptr<WifiAdhocNetDevice> device = factory->CreateAdhoc (node);
  device->ConnectTo (channel);
  Ptr<MobilityModel> mobility = Create<StaticMobilityModel> ();
  mobility->Set (position);
  node->AddInterface (mobility);
  
  Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
  uint32_t index = ipv4->AddInterface (device);
  ipv4->SetAddress (index, Ipv4Address (address));
  return node;
}

static void
SetPosition (Ptr<Node> node, Position position)
{
  Ptr<MobilityModel> mobility = node->QueryInterface<MobilityModel> (MobilityModel::iid);
  mobility->Set (position);
}

static Position
GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->QueryInterface<MobilityModel> (MobilityModel::iid);
  return mobility->Get ();
}

static void 
AdvancePosition (Ptr<Node> node) 
{
  Position pos = GetPosition (node);
  pos.x += 5.0;
  if (pos.x >= 210.0) {
    return;
  }
  SetPosition (node, pos);
  //std::cout << "x="<<x << std::endl;
  Simulator::Schedule (Seconds (1.0), &AdvancePosition, node);
}




int main (int argc, char *argv[])
{
  Simulator::SetLinkedList ();

  //Simulator::EnableLogTo ("80211.log");


  Ptr<WifiNetDeviceFactory> factory = Create<WifiNetDeviceFactory> ();
  // force rts/cts on all the time.
  factory->SetMacRtsCtsThreshold (2200);
  factory->SetMacFragmentationThreshold (2200);
  //factory->SetCr (5, 5);
  //factory->SetIdeal (1e-5);
  factory->SetAarf ();
  //factory->SetArf ();
  Ptr<WifiChannel> channel = Create<WifiChannel> ();

  Ptr<Node> a = CreateAdhocNode (factory, channel, 
                                 Position (5.0,0.0,0.0),
                                 "192.168.0.1");
  Simulator::Schedule (Seconds (1.0), &AdvancePosition, a);

  Ptr<Node> b = CreateAdhocNode (factory, channel,
                                 Position (0.0, 0.0, 0.0),
                                 "192.168.0.2");

  Ptr<Application> app = Create<OnOffApplication> (a, InetSocketAddress ("192.168.0.2", 10), 
                                                   "Udp", 
                                                   ConstantVariable (42),
                                                   ConstantVariable (0));
  app->Start (Seconds (0.5));
  app->Stop (Seconds (43.0));

#if 0
  TraceContainer container = TraceContainer ();
  wifiServer->RegisterTraces (&container);
  container.SetCallback ("80211-packet-rx", 
                         MakeCallback (&ThroughputPrinter::Receive, printer));
#endif

  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
