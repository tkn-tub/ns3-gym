/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/mpi-interface.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"

#ifdef NS3_MPI
#include <mpi.h>
#endif

// Default Network Topology (same as third.cc from tutorial)
// Distributed simulation, split along the p2p link
// Number of wifi or csma nodes can be increased up to 250
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   ================
//                          |          LAN 10.1.2.0
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdExampleDistributed");

int 
main (int argc, char *argv[])
{
#ifdef NS3_MPI
  // Distributed simulation setup
  MpiInterface::Enable (&argc, &argv);
  GlobalValue::Bind ("SimulatorImplementationType",
                     StringValue ("ns3::DistributedSimulatorImpl"));

  uint32_t systemId = MpiInterface::GetSystemId ();
  uint32_t systemCount = MpiInterface::GetSize ();

  // Check for valid distributed parameters.
  // Must have 2 and only 2 Logical Processors (LPs)
  if (systemCount != 2)
    {
      std::cout << "This simulation requires 2 and only 2 logical processors." << std::endl;
      return 1;
    }

  bool verbose = true;
  uint32_t nCsma = 3;
  uint32_t nWifi = 3;
  bool tracing = false;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue
  if (nWifi > 250 || nCsma > 250)
    {
      std::cout << "Too many wifi or csma nodes, max 200 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  NodeContainer p2pNodes;
  Ptr<Node> p2pNode1 = CreateObject<Node> (0); // Create node with rank 0
  Ptr<Node> p2pNode2 = CreateObject<Node> (1); // Create node with rank 1
  p2pNodes.Add (p2pNode1);
  p2pNodes.Add (p2pNode2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma, 1);  // Create csma nodes with rank 1

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi, 0); // Create wifi nodes with rank 0
  NodeContainer wifiApNode = p2pNodes.Get (0);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (5.0),
                                 "GridWidth", UintegerValue (10),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-250, 250, -250, 250)));
  mobility.Install (wifiStaNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);

  // If this simulator has system id 1, then 
  // it should contain the server application, 
  // since it is on one of the csma nodes
  if (systemId == 1)
    {
      UdpEchoServerHelper echoServer (9);
      ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
      serverApps.Start (Seconds (1.0));
      serverApps.Stop (Seconds (10.0));
    }

  // If the simulator has system id 0, then 
  // it should contain the client application, 
  // since it is on one of the wifi nodes
  if (systemId == 0)
    {
      UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
      echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
      echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
      echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

      ApplicationContainer clientApps = 
        echoClient.Install (wifiStaNodes.Get (nWifi - 1));
      clientApps.Start (Seconds (2.0));
      clientApps.Stop (Seconds (10.0));
    }
 
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  if (tracing == true)
    {
      // Depending on the system Id (rank), the pcap information 
      // traced will be different.  For example, the ethernet pcap
      // will be empty for rank0, since these nodes are placed on 
      // on rank 1.  All ethernet traffic will take place on rank 1.
      // Similar differences are seen in the p2p and wirless pcaps.
      if (systemId == 0)
        {
          pointToPoint.EnablePcapAll ("third-distributed-rank0");
          phy.EnablePcap ("third-distributed-rank0", apDevices.Get (0));
          csma.EnablePcap ("third-distributed-rank0", csmaDevices.Get (0), true);
        }
      else
        {
          pointToPoint.EnablePcapAll ("third-distributed-rank1");
          phy.EnablePcap ("third-distributed-rank1", apDevices.Get (0));
          csma.EnablePcap ("third-distributed-rank1", csmaDevices.Get (0), true);
        }
    }

  Simulator::Run ();
  Simulator::Destroy ();
  // Exit the MPI execution environment
  MpiInterface::Disable ();
  return 0;

#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}
