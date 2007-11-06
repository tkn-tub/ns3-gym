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
#include "ns3/global-route-manager.h"
#include "ns3/packet.h"
#include "ns3/socket.h"
#include "ns3/socket-factory.h"
#include "ns3/command-line.h"
#include "ns3/gnuplot.h"


#include <iostream>

using namespace ns3;
static uint32_t g_bytesTotal = 0;
static GnuplotDataset *g_output = 0;

static Ptr<Node>
CreateAdhocNode (Ptr<WifiChannel> channel,
                 Position position, const char *address)
{
  Ptr<Node> node = Create<InternetNode> ();  
  Ptr<AdhocWifiNetDevice> device = Create<AdhocWifiNetDevice> (node);
  device->ConnectTo (channel);
  Ptr<MobilityModel> mobility = Create<StaticMobilityModel> ();
  mobility->Set (position);
  node->AddInterface (mobility);
  
  Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
  uint32_t index = ipv4->AddInterface (device);
  ipv4->SetAddress (index, Ipv4Address (address));
  ipv4->SetNetworkMask (index, Ipv4Mask ("255.255.0.0"));
  ipv4->SetUp (index);
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
  double mbs = ((g_bytesTotal * 8.0) / 1000000);
  g_bytesTotal = 0;
  g_output->Add (pos.x, mbs);
  pos.x += 1.0;
  if (pos.x >= 210.0) 
    {
      return;
    }
  SetPosition (node, pos);
  //std::cout << "x="<<pos.x << std::endl;
  Simulator::Schedule (Seconds (1.0), &AdvancePosition, node);
}

static void
ReceivePacket (Ptr<Socket> socket, const Packet &packet, const Address &address)
{
  g_bytesTotal += packet.GetSize ();
}

static Ptr<Socket>
SetupUdpReceive (Ptr<Node> node, uint16_t port)
{
  InterfaceId iid = InterfaceId::LookupByName ("Udp");
  Ptr<SocketFactory> socketFactory = node->QueryInterface<SocketFactory> (iid);

  Ptr<Socket> sink = socketFactory->CreateSocket ();
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), port);
  sink->Bind (local);
  sink->SetRecvCallback (MakeCallback (&ReceivePacket));
  return sink;
}

static void
RunOneExperiment (void)
{
  Ptr<WifiChannel> channel = Create<WifiChannel> ();

  Ptr<Node> a = CreateAdhocNode (channel, 
                                 Position (5.0,0.0,0.0),
                                 "192.168.0.1");
  Ptr<Node> b = CreateAdhocNode (channel,
                                 Position (0.0, 0.0, 0.0),
                                 "192.168.0.2");

  Ptr<Application> app = Create<OnOffApplication> (a, InetSocketAddress ("192.168.0.2", 10), 
                                                   "Udp", 
                                                   ConstantVariable (250),
                                                   ConstantVariable (0),
                                                   DataRate (60000000),
                                                   2000);

  app->Start (Seconds (0.5));
  app->Stop (Seconds (250.0));

  Simulator::Schedule (Seconds (1.5), &AdvancePosition, b);
  Ptr<Socket> recvSink = SetupUdpReceive (b, 10);

  GlobalRouteManager::PopulateRoutingTables ();

  Simulator::Run ();

  Simulator::Destroy ();
}

int main (int argc, char *argv[])
{
  Simulator::SetLinkedList ();

  // disable fragmentation
  DefaultValue::Bind ("WifiFragmentationThreshold", "2200");
  CommandLine::Parse (argc, argv);

  Gnuplot gnuplot = Gnuplot ("rate-control.png");

  g_output = new GnuplotDataset ("aarf");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRtsCtsThreshold", "2200");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Aarf");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("arf");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRtsCtsThreshold", "2200");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Arf");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("ideal");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRtsCtsThreshold", "0");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Ideal");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  gnuplot.GenerateOutput (std::cout);

  return 0;
}
