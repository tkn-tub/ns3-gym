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
#include "ns3/onoff-application.h"
#include "ns3/static-mobility-model.h"
#include "ns3/random-variable.h"
#include "ns3/packet-socket-address.h"
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
                 Vector position, const char *address)
{
  Ptr<Node> node = CreateObject<Node> ();  
  Ptr<AdhocWifiNetDevice> device = CreateObject<AdhocWifiNetDevice> (node, Mac48Address (address));
  device->Attach (channel);
  Ptr<MobilityModel> mobility = CreateObject<StaticMobilityModel> ();
  mobility->SetPosition (position);
  node->AddInterface (mobility);
  
  return node;
}

static void
SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->QueryInterface<MobilityModel> ();
  mobility->SetPosition (position);
}

static Vector
GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->QueryInterface<MobilityModel> ();
  return mobility->GetPosition ();
}

static void 
AdvancePosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
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
ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &address)
{
  g_bytesTotal += packet->GetSize ();
}

static Ptr<Socket>
SetupPacketReceive (Ptr<Node> node, uint16_t port)
{
  TypeId iid = TypeId::LookupByName ("Packet");
  Ptr<SocketFactory> socketFactory = node->QueryInterface<SocketFactory> (iid);
  Ptr<Socket> sink = socketFactory->CreateSocket ();
  sink->Bind ();
  sink->SetRecvCallback (MakeCallback (&ReceivePacket));
  return sink;
}

static void
RunOneExperiment (void)
{
  g_bytesTotal = 0;

  Ptr<WifiChannel> channel = CreateObject<WifiChannel> ();

  Ptr<Node> a = CreateAdhocNode (channel, 
                                 Vector (5.0,0.0,0.0),
                                 "00:00:00:00:00:01");
  Ptr<Node> b = CreateAdhocNode (channel,
                                 Vector (0.0, 0.0, 0.0),
                                 "00:00:00:00:00:02");

  PacketSocketAddress destination = PacketSocketAddress ();
  destination.SetProtocol (1);
  destination.SetSingleDevice (0);
  destination.SetPhysicalAddress (Mac48Address ("00:00:00:00:00:02"));
  Ptr<Application> app = CreateObject<OnOffApplication> (a, destination, 
                                                   "Packet", 
                                                   ConstantVariable (250),
                                                   ConstantVariable (0),
                                                   DataRate (60000000),
                                                   2000);

  app->Start (Seconds (0.5));
  app->Stop (Seconds (250.0));

  Simulator::Schedule (Seconds (1.5), &AdvancePosition, b);
  Ptr<Socket> recvSink = SetupPacketReceive (b, 10);

  Simulator::Run ();

  Simulator::Destroy ();
}

int main (int argc, char *argv[])
{
  Simulator::SetLinkedList ();

  // disable fragmentation
  DefaultValue::Bind ("WifiFragmentationThreshold", "2200");
  CommandLine::Parse (argc, argv);

  Gnuplot gnuplot = Gnuplot ("reference-rates.png");

  DefaultValue::Bind ("WifiRtsCtsThreshold", "2200");

  g_output = new GnuplotDataset ("54mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "54mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("48mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "48mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("36mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "36mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("24mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "24mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("18mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "18mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("12mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "12mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("9mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "9mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("6mb");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRateControlAlgorithm", "ConstantRate");
  DefaultValue::Bind ("WifiConstantDataRate", "6mb");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  gnuplot.GenerateOutput (std::cout);

  gnuplot = Gnuplot ("rate-control.png");

  DefaultValue::Bind ("WifiPhyStandard", "holland");

  g_output = new GnuplotDataset ("arf");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRtsCtsThreshold", "2200");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Arf");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("aarf");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRtsCtsThreshold", "2200");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Aarf");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  g_output = new GnuplotDataset ("ideal");
  g_output->SetStyle (GnuplotDataset::LINES);
  DefaultValue::Bind ("WifiRtsCtsThreshold", "2200");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Ideal");
  RunOneExperiment ();
  gnuplot.AddDataset (*g_output);
  delete g_output;

  gnuplot.GenerateOutput (std::cout);

  return 0;
}
