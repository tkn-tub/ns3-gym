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
#include "ns3/arf-wifi-manager.h"
#include "ns3/adhoc-wifi-mac.h"
#include "ns3/wifi-phy.h"
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
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include "ns3/wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/log.h"


#include <iostream>

NS_LOG_COMPONENT_DEFINE ("Main");

using namespace ns3;

class Experiment
{
public:
  Experiment ();
  Experiment (std::string name);
  GnuplotDataset Run (const WifiHelper &wifi);
private:
  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &address);
  void SetPosition (Ptr<Node> node, Vector position);
  Vector GetPosition (Ptr<Node> node);
  void AdvancePosition (Ptr<Node> node);
  Ptr<Socket> SetupPacketReceive (Ptr<Node> node);

  uint32_t m_bytesTotal;
  GnuplotDataset m_output;
};

Experiment::Experiment ()
  : m_output ()
{}

Experiment::Experiment (std::string name)
  : m_output (name)
{
  m_output.SetStyle (GnuplotDataset::LINES);
}

void
Experiment::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
Experiment::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void 
Experiment::AdvancePosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add (pos.x, mbs);
  pos.x += 1.0;
  if (pos.x >= 210.0) 
    {
      return;
    }
  SetPosition (node, pos);
  //std::cout << "x="<<pos.x << std::endl;
  Simulator::Schedule (Seconds (1.0), &Experiment::AdvancePosition, this, node);
}

void
Experiment::ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &address)
{
  m_bytesTotal += packet->GetSize ();
}

Ptr<Socket>
Experiment::SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("Packet");
  Ptr<SocketFactory> socketFactory = node->GetObject<SocketFactory> (tid);
  Ptr<Socket> sink = socketFactory->CreateSocket ();
  sink->Bind ();
  sink->SetRecvCallback (MakeCallback (&Experiment::ReceivePacket, this));
  return sink;
}

GnuplotDataset
Experiment::Run (const WifiHelper &wifi)
{
  m_bytesTotal = 0;

  NodeContainer c;
  c.Create (2);

  NetDeviceContainer devices = wifi.Build (c);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObjectWith<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("StaticMobilityModel");

  mobility.Layout (c.Begin (), c.End ());

  PacketSocketAddress destination = PacketSocketAddress ();
  destination.SetProtocol (1);
  destination.SetSingleDevice (0);
  destination.SetPhysicalAddress (devices.Get (1)->GetAddress ());
  Ptr<Application> app = 
    CreateObjectWith<OnOffApplication> ("Node", c.Get (0), 
                                        "Remote", Address (destination),
                                        "Protocol", TypeId::LookupByName ("Packet"),
                                        "OnTime", ConstantVariable (250),
                                        "OffTime", ConstantVariable (0),
                                        "DataRate", DataRate (60000000),
                                        "PacketSize", Uinteger (2000));
  c.Get (0)->AddApplication (app);

  app->Start (Seconds (0.5));
  app->Stop (Seconds (250.0));

  Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (1));
  Ptr<Socket> recvSink = SetupPacketReceive (c.Get (1));

  Simulator::Run ();

  Simulator::Destroy ();

  return m_output;
}

int main (int argc, char *argv[])
{
  Simulator::SetLinkedList ();

  // disable fragmentation
  Config::SetDefault ("WifiRemoteStationManager::FragmentationThreshold", String ("2200"));
  Config::SetDefault ("WifiRemoteStationManager::RtsCtsThreshold", String ("2200"));

  CommandLine::Parse (argc, argv);

  Gnuplot gnuplot = Gnuplot ("reference-rates.png");

  Experiment experiment;
  WifiHelper wifi;
  GnuplotDataset dataset;

  wifi.SetMac ("AdhocWifiMac");
  wifi.SetPhy ("WifiPhy");

  NS_LOG_DEBUG ("54");
  experiment = Experiment ("54mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-54mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("48");
  experiment = Experiment ("48mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-48mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("36");
  experiment = Experiment ("36mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-36mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("24");
  experiment = Experiment ("24mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-24mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("18");
  experiment = Experiment ("18mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-18mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("12");
  experiment = Experiment ("12mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-12mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("9");
  experiment = Experiment ("9mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-9mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("6");
  experiment = Experiment ("6mb");
  wifi.SetRemoteStationManager ("ConstantRateWifiManager",
                                "DataMode", String ("wifia-6mbs"));
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);



  gnuplot = Gnuplot ("rate-control.png");
  Config::SetDefault ("WifiPhy::Standard", String ("holland"));


  NS_LOG_DEBUG ("arf");
  experiment = Experiment ("arf");
  wifi.SetRemoteStationManager ("ArfWifiManager");
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("aarf");
  experiment = Experiment ("aarf");
  wifi.SetRemoteStationManager ("AarfWifiManager");
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("ideal");
  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("IdealWifiManager");
  dataset = experiment.Run (wifi);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);

  return 0;
}
