/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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


#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/global-routing-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mesh-module.h"
#include "ns3/mobility-module.h"
#include "ns3/dot11s-helper.h"
#include "ns3/dot11s-interface-helper.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace ns3;
using namespace dot11s;

NS_LOG_COMPONENT_DEFINE ("TestMeshScript");
class MeshTest
{
  public:
    /// Init test
    MeshTest ();
    /// Configure test from command line arguments
    void Configure (int argc, char ** argv);
    /// Run test
    int Run ();
  private:
    int       xSize;
    int       ySize;
    double    step;
    double    randomStart;
    double    totalTime;
    double    packetInterval;
    uint16_t  packetSize;
    uint32_t  nIfaces;
    bool      chan;
    bool      pcap;
    uint64_t  seed;
    /// List of network nodes
    NodeContainer nodes;
    /// List of all mesh point devices
    NetDeviceContainer meshDevices;
    //Addresses of interfaces:
    Ipv4InterfaceContainer interfaces;
    //InternetStackHelper stack;
    //Ipv4AddressHelper address;
  private:
    /// Create nodes and setup theis mobility
    void CreateNodes ();
    /// Install internet stack on nodes
    void InstallInternetStack ();
    /// Install applications
    void InstallApplication ();
    /// Print mesh devices diagnostics
    void Report ();  
};
MeshTest::MeshTest () :
  xSize (3),
  ySize (3),
  step (100.0),
  randomStart (0.1),
  totalTime (100.0),
  packetInterval (0.1),
  packetSize (1024),
  nIfaces (1),
  chan (true),
  pcap (false),
  seed (1)
{
}
void
MeshTest::Configure (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.AddValue ("x-size", "Number of nodes in a row grid. [6]", xSize);
  cmd.AddValue ("y-size", "Number of rows in a grid. [6]", ySize);
  cmd.AddValue ("step",   "Size of edge in our grid, meters. [100 m]", step);
  cmd.AddValue ("start",  "Maximum random start delay, seconds. [0.1 s]", randomStart);
  cmd.AddValue ("time",  "Simulation time, seconds [100 s]", totalTime);
  cmd.AddValue ("packet-interval",  "Interval between packets, seconds [0.001 s]", packetInterval);
  cmd.AddValue ("packet-size",  "Size of packets", packetSize);
  cmd.AddValue ("interfaces", "Number of radio interfaces used by each mesh point. [1]", nIfaces);
  cmd.AddValue ("channels",   "Use different frequency channels for different interfaces. [0]", chan);
  cmd.AddValue ("pcap",   "Enable PCAP traces on interfaces. [0]", pcap);
  cmd.AddValue ("seed",   "Seed value", seed);
  
  cmd.Parse (argc, argv);
  NS_LOG_DEBUG ("Grid:" << xSize << "*" << ySize);
  NS_LOG_DEBUG ("Simulation time: " << totalTime << " s");
  SeedManager::SetSeed(seed);
}
void
MeshTest::CreateNodes ()
{ 
  nodes.Create (ySize*xSize);
  // Setting channel
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  // Install mesh point devices & protocols
  MeshWifiHelper mesh;
  mesh.SetSpreadInterfaceChannels (chan);
  std::vector<uint32_t> roots;
  roots.push_back(xSize-1);
  roots.push_back(xSize*ySize-xSize);
  MeshInterfaceHelper interface = MeshInterfaceHelper::Default ();
  interface.SetType ("RandomStart", TimeValue (Seconds(randomStart)));

  meshDevices = mesh.Install (wifiPhy, interface, nodes, roots, nIfaces);
  // Setup mobility
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (step),
                                 "DeltaY", DoubleValue (step),
                                 "GridWidth", UintegerValue (xSize),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
  if (pcap)
    wifiPhy.EnablePcapAll (std::string ("mp-"));
}
void
MeshTest::InstallInternetStack ()
{
  InternetStackHelper stack;
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  interfaces = address.Assign (meshDevices);
}
void
MeshTest::InstallApplication ()
{
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (0));
  serverApps.Start (Seconds (0.0));
  serverApps.Stop (Seconds (totalTime));
  UdpEchoClientHelper echoClient (interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue ((uint32_t)(totalTime*(1/packetInterval))));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (packetInterval)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (xSize*ySize-1));
  clientApps.Start (Seconds (0.0));
  clientApps.Stop (Seconds (totalTime));
}
int
MeshTest::Run ()
{
  CreateNodes ();
  InstallInternetStack ();
  InstallApplication ();
  Simulator::Schedule (Seconds(totalTime), & MeshTest::Report, this);
  Simulator::Stop (Seconds (totalTime));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
void
MeshTest::Report ()
{
  NS_LOG_UNCOND("Report is here:");
  unsigned n (0);
  for (NetDeviceContainer::Iterator i = meshDevices.Begin (); i != meshDevices.End (); ++i, ++n)
  {
    std::ostringstream os;
    os << "mp-report-" << n << ".xml";
    std::cerr << "Printing mesh point device #" << n << " diagnostics to " << os.str () << "\n";
    std::ofstream of;
    of.open (os.str().c_str());
    if (! of.is_open ())
    {
      std::cerr << "Error: Can't open file " << os.str() << "\n";
      return;
    }
    MeshWifiHelper::Report (*i, of);
    of.close ();
  }
}
int
main (int argc, char *argv[])
{
  MeshTest t; 
  t.Configure (argc, argv);
  return t.Run();
}
