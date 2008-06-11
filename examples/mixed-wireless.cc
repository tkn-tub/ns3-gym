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
 *
 */

//
// This ns-3 example demonstrates the use of helper functions to ease 
// the construction of simulation scenarios.  
// 
// The simulation topology consists of a mixed wired and wireless
// scenario in which a hierarchical mobility model is used.
//
// The simulation layout consists of N backbone routers interconnected
// by an ad hoc wifi network.
// Each backbone router also has a local 802.11 network and is connected
// to a local LAN.  An additional set of (K-1) nodes are connected to
// this backbone.  Finally, a local LAN is connected to each router
// on the backbone, with L-1 additional hosts.  
//
// The nodes are populated with TCP/IP stacks, and OLSR unicast routing
// on the backbone.  An example UDP transfer is shown.  The simulator
// be configured to output tcpdumps or traces from different nodes.
//
//
//          +--------------------------------------------------------+
//          |                                                        |
//          |              802.11 ad hoc, ns-2 mobility              | 
//          |                                                        |
//          +--------------------------------------------------------+
//                   |       o o o (N backbone routers)       |
//               +--------+                               +--------+
//     wired LAN | mobile |                     wired LAN | mobile |
//    -----------| router |                    -----------| router |
//               ---------                                ---------
//                   |                                        |
//          +----------------+                       +----------------+
//          |     802.11     |                       |     802.11     |
//          |      net       |                       |       net      |
//          |   K-1 hosts    |                       |   K-1 hosts    |
//          +----------------+                       +----------------+
//

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"
#include "ns3/wifi-module.h"

using namespace ns3;

//
// Define logging keyword for this file
//
NS_LOG_COMPONENT_DEFINE ("MixedWireless");

//
// This function will be used below as a trace sink
// 
#ifdef ENABLE_FOR_TRACING_EXAMPLE
static void
CourseChangeCallback (std::string path, Ptr<const MobilityModel> model)
{
  Vector position = model->GetPosition ();
  std::cout << "CourseChange " << path << " x=" << position.x << ", y=" << position.y << ", z=" << position.z << std::endl;
}
#endif

int 
main (int argc, char *argv[])
{
  //
  // First, we declare and initialize a few local variables that control some 
  // simulation parameters.
  //
  uint32_t backboneNodes = 10;
  uint32_t infraNodes = 5;
  uint32_t lanNodes = 5;
  uint32_t stopTime = 10;

  //
  // Simulation defaults are typically set next, before command line
  // arguments are parsed.
  //
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", StringValue ("210"));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));

  //
  // For convenience, we add the local variables to the command line argument
  // system so that they can be overridden with flags such as 
  // "--backboneNodes=20"
  //
  CommandLine cmd;
  cmd.AddValue("backboneNodes", "number of backbone nodes", backboneNodes);
  cmd.AddValue ("infraNodes", "number of leaf nodes", infraNodes);
  cmd.AddValue("lanNodes", "number of LAN nodes", lanNodes);
  cmd.AddValue("stopTime", "simulation stop time (seconds)", stopTime);

  //
  // The system global variables and the local values added to the argument
  // system can be overridden by command line arguments by using this call.
  //
  cmd.Parse (argc, argv);

  // The metadata system (off by default) is used by ascii tracing below
  Packet::EnableMetadata ();

  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Construct the backbone                                                //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  //
  // Create a container to manage the nodes of the adhoc (backbone) network.
  // Later we'll create the rest of the nodes we'll need.
  //
  NodeContainer backbone;
  backbone.Create (backboneNodes);
  //
  // Create the backbone wifi net devices and install them into the nodes in 
  // our container
  //
  WifiHelper wifi;
  wifi.SetMac ("ns3::AdhocWifiMac");
  NetDeviceContainer backboneDevices = wifi.Install (backbone);
  //
  // Add the IPv4 protocol stack to the nodes in our container
  //
  InternetStackHelper internet;
  internet.Install (backbone);
  //
  // Assign IPv4 addresses to the device drivers (actually to the associated
  // IPv4 interfaces) we just created.
  //
  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase ("192.168.0.0", "255.255.255.0");
  ipAddrs.Assign (backboneDevices);

  //
  // The ad-hoc network nodes need a mobility model so we aggregate one to 
  // each of the nodes we just finished building.  
  //
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = 
    CreateObject<ListPositionAllocator> ();
  double x = 0.0;
  for (uint32_t i = 0; i < backboneNodes; ++i)
    {
      positionAlloc->Add (Vector (x, 0.0, 0.0));
      x += 5.0;
    }
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (0, 1000, 0, 1000)),
                             "Speed", RandomVariableValue (ConstantVariable (2000)),
                             "Pause", RandomVariableValue (ConstantVariable (0.2)));
  mobility.Install (backbone);

  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Construct the LANs                                                    //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  // Reset the address base-- all of the CSMA networks will be in
  // the "172.16 address space
  ipAddrs.SetBase ("172.16.0.0", "255.255.255.0");


  for (uint32_t i = 0; i < backboneNodes; ++i)
    {
      NS_LOG_INFO ("Configuring local area network for backbone node " << i);
      //
      // Create a container to manage the nodes of the LAN.  We need
      // two containers here; one with all of the new nodes, and one
      // with all of the nodes including new and existing nodes
      //
      NodeContainer newLanNodes;
      newLanNodes.Create (lanNodes - 1);
      // Now, create the container with all nodes on this link
      NodeContainer lan (backbone.Get (i), newLanNodes);
      //
      // Create the CSMA net devices and install them into the nodes in our 
      // collection.
      //
      CsmaHelper csma;
      csma.SetChannelParameter ("BitRate", DataRateValue (DataRate (5000000)));
      csma.SetChannelParameter ("Delay", TimeValue (MilliSeconds (2)));
      NetDeviceContainer lanDevices = csma.Install (lan);
      //
      // Add the IPv4 protocol stack to the new LAN nodes
      //
      internet.Install (newLanNodes);
      //
      // Assign IPv4 addresses to the device drivers (actually to the 
      // associated IPv4 interfaces) we just created.
      //
      ipAddrs.Assign (lanDevices);
      //
      // Assign a new network prefix for the next LAN, according to the
      // network mask initialized above
      //
      ipAddrs.NewNetwork ();
    }

  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Construct the mobile networks                                         //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  // Reset the address base-- all of the 802.11 networks will be in
  // the "10.0" address space
  ipAddrs.SetBase ("10.0.0.0", "255.255.255.0");

  for (uint32_t i = 0; i < backboneNodes; ++i)
    {
      NS_LOG_INFO ("Configuring wireless network for backbone node " << i);
      //
      // Create a container to manage the nodes of the LAN.  We need
      // two containers here; one with all of the new nodes, and one
      // with all of the nodes including new and existing nodes
      //
      NodeContainer newInfraNodes;
      newInfraNodes.Create (infraNodes - 1);
      // Now, create the container with all nodes on this link
      NodeContainer infra (backbone.Get (i), newInfraNodes);
      //
      // Create another ad hoc network and devices
      //
      WifiHelper wifiInfra;
      wifiInfra.SetMac ("ns3::AdhocWifiMac");
      NetDeviceContainer infraDevices = wifiInfra.Install (infra);

      // Add the IPv4 protocol stack to the nodes in our container
      //
      internet.Install (newInfraNodes);
      //
      // Assign IPv4 addresses to the device drivers (actually to the associated
      // IPv4 interfaces) we just created.
      //
      ipAddrs.Assign (infraDevices);
      //
      // Assign a new network prefix for each mobile network, according to 
      // the network mask initialized above
      //
      ipAddrs.NewNetwork ();
      //
      // The new wireless nodes need a mobility model so we aggregate one 
      // to each of the nodes we just finished building.
      //
      Ptr<ListPositionAllocator> subnetAlloc = 
        CreateObject<ListPositionAllocator> ();
      for (uint32_t j = 0; j < infra.GetN (); ++j)
        {
          subnetAlloc->Add (Vector (0.0, j, 0.0));
        }
      mobility.PushReferenceMobilityModel (backbone.Get (i));
      mobility.SetPositionAllocator (subnetAlloc);
      mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-25, 25, -25, 25)),
                                 "Speed", RandomVariableValue (ConstantVariable (30)),
                                 "Pause", RandomVariableValue (ConstantVariable (0.4)));
      mobility.Install (infra);
    }
  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Routing configuration                                                 //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  NS_LOG_INFO ("Enabling OLSR routing on all backbone nodes");
  OlsrHelper olsr;
  olsr.Install (backbone);

  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Application configuration                                             //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s, between two nodes
  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)

  // Let's make sure that the user does not define too few LAN nodes
  // to make this example work.  We need lanNodes >= 5
  NS_ASSERT (lanNodes >= 5);
  Ptr<Node> appSource = NodeList::GetNode (11);  
  Ptr<Node> appSink = NodeList::GetNode (13);  
  Ipv4Address remoteAddr = Ipv4Address ("172.16.0.5");

  OnOffHelper onoff ("ns3::UdpSocketFactory", 
                     Address (InetSocketAddress (remoteAddr, port)));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  ApplicationContainer apps = onoff.Install (appSource);
  apps.Start (Seconds (3.0));
  apps.Stop (Seconds (20.0));

  // Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory", 
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  apps = sink.Install (appSink);
  apps.Start (Seconds (3.0));

  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Tracing configuration                                                 //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  NS_LOG_INFO ("Configure Tracing.");
  //
  // Let's set up some ns-2-like ascii traces, using another helper class
  //
  std::ofstream ascii;
  ascii.open ("mixed-wireless.tr");
  WifiHelper::EnableAsciiAll (ascii);
  CsmaHelper::EnableAsciiAll (ascii);
  // Look at nodes 11, 13 only
  //WifiHelper::EnableAscii (ascii, 11, 0); 
  //WifiHelper::EnableAscii (ascii, 13, 0); 

  // Let's do a pcap trace on the backbone devices
  WifiHelper::EnablePcap ("mixed-wireless", backboneDevices); 
  // Let's additionally trace the application Sink, ifIndex 0
  CsmaHelper::EnablePcap ("mixed-wireless", appSink->GetId (), 0);

#ifdef ENABLE_FOR_TRACING_EXAMPLE
  Config::Connect ("/NodeList/*/$MobilityModel/CourseChange",
    MakeCallback (&CourseChangeCallback));
#endif


  /////////////////////////////////////////////////////////////////////////// 
  //                                                                       //
  // Run simulation                                                        //
  //                                                                       //
  /////////////////////////////////////////////////////////////////////////// 

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (stopTime));
  Simulator::Run ();    
  Simulator::Destroy ();
}
