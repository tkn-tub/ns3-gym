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

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/csma-module.h"
#include "ns3/global-route-manager.h"

// Network topology (default)
//
//            n2     +          +     n3          .
//             | ... |\        /| ... |           .
//             ======= \      / =======           .
//              CSMA    \    /   CSMA             .
//                       \  /                     .
//            n1     +--- n0 ---+     n4          .
//             | ... |   /  \   | ... |           .
//             =======  /    \  =======           .
//              CSMA   /      \  CSMA             .
//                    /        \                  .
//            n6     +          +     n5          .
//             | ... |          | ... |           .
//             =======          =======           .
//              CSMA             CSMA             .
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaStar");

int 
main (int argc, char *argv[])
{
  //
  // Make the random number generators generate reproducible results.
  //
  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);

  //
  // Set up some default values for the simulation.
  //
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (137));

  // ??? try and stick 15kb/s into the data rate
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("14kb/s"));

  //
  // Default number of nodes in the star.  Overridable by command line argument.
  //
  uint32_t nNodes = 7;

  CommandLine cmd;
  cmd.AddValue("nNodes", "Number of nodes to place in the star", nNodes);
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");
  NodeContainer hubNode;
  NodeContainer spokeNodes;
  hubNode.Create (1);
  Ptr<Node> hub = hubNode.Get (0);
  spokeNodes.Create (nNodes - 1);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("1ms"));

  NS_LOG_INFO ("Build star topology.");
  NetDeviceContainer hubDevices, spokeDevices;
  csma.InstallStar (hubNode.Get (0), spokeNodes, hubDevices, spokeDevices);

  NodeContainer fillNodes;

  //
  // Just to be nasy, hang some more nodes off of the CSMA channel for each
  // spoke, so that there are a total of 16 nodes on each channel.  Stash
  // all of these new devices into a container.
  //
  NetDeviceContainer fillDevices;

  uint32_t nFill = 14;
  for (uint32_t i = 0; i < spokeDevices.GetN (); ++i)
    {
      Ptr<Channel> channel = spokeDevices.Get (i)->GetChannel ();
      Ptr<CsmaChannel> csmaChannel = channel->GetObject<CsmaChannel> ();
      NodeContainer newNodes;
      NetDeviceContainer newDevices;
      newNodes.Create (nFill);
      fillNodes.Add (newNodes);
      fillDevices.Add (csma.Install (newNodes, csmaChannel));
    }

  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet;
  internet.Install (NodeContainer (hubNode, spokeNodes, fillNodes));

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper address;

  //
  // Assign IPv4 interfaces and IP addresses to the devices we previously
  // created.  Keep track of the resulting addresses, one for the addresses
  // of the hub node, and one for addresses on the spoke nodes.  Despite the
  // name of the class (Ipv4InterfaceContainer), what is visible to clients 
  // is really the address not the interface.
  //
  Ipv4InterfaceContainer hubAddresses;
  Ipv4InterfaceContainer spokeAddresses;

  for(uint32_t i = 0; i < spokeNodes.GetN (); ++i)
  {
    std::ostringstream subnet;
    subnet << "10.1." << i << ".0";
    NS_LOG_INFO ("Assign IP Addresses for CSMA subnet " << subnet.str ());
    address.SetBase (subnet.str ().c_str (), "255.255.255.0");
    hubAddresses.Add (address.Assign (hubDevices.Get (i)));
    spokeAddresses.Add (address.Assign (spokeDevices.Get (i)));
    //
    // We assigned addresses to the logical hub and the first "drop" of the 
    // CSMA network that acts as the spoke, but we also have a number of fill
    // devices (nFill) also hanging off the CSMA network.  We have got to 
    // assign addresses to them as well.  We put all of the fill devices into
    // a single device container, so the first nFill devices are associated
    // with the channel connected to spokeDevices.Get (0), the second nFill
    // devices afe associated with the channel connected to spokeDevices.Get (1)
    // etc.
    //
    for (uint32_t j = 0; j < nFill; ++j)
      {
        address.Assign (fillDevices.Get (i * nFill + j));
      }
  }

  NS_LOG_INFO ("Create applications.");
  //
  // Create a packet sink on the star "hub" to receive packets.  
  // 
  uint16_t port = 50000;
  Address hubLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", hubLocalAddress);
  ApplicationContainer hubApp = packetSinkHelper.Install (hubNode);
  hubApp.Start (Seconds (1.0));
  hubApp.Stop (Seconds (10.0));

  //
  // Create OnOff applications to send TCP to the hub, one on each spoke node.
  //
  OnOffHelper onOffHelper ("ns3::TcpSocketFactory", Address ());
  onOffHelper.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onOffHelper.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));

  ApplicationContainer spokeApps;

  for (uint32_t i = 0; i < spokeNodes.GetN (); ++i)
    {
      AddressValue remoteAddress (InetSocketAddress (hubAddresses.GetAddress (i), port));
      onOffHelper.SetAttribute ("Remote", remoteAddress);
      spokeApps.Add (onOffHelper.Install (spokeNodes.Get (i)));
    }

  spokeApps.Start (Seconds (1.0));
  spokeApps.Stop (Seconds (10.0));

  //
  // Because we are evil, we also add OnOff applications to send TCP to the hub 
  // from the fill devices on each CSMA link.  The first nFill nodes in the 
  // fillNodes container are on the CSMA network talking to the zeroth device
  // on the hub node.  The next nFill nodes are on the CSMA network talking to
  // the first device on the hub node, etc.  So the ith fillNode is associated
  // with the hub address found on the (i / nFill)th device on the hub node.
  //
  ApplicationContainer fillApps;

  for (uint32_t i = 0; i < fillNodes.GetN (); ++i)
    {
      AddressValue remoteAddress (InetSocketAddress (hubAddresses.GetAddress (i / nFill), port));
      onOffHelper.SetAttribute ("Remote", remoteAddress);
      fillApps.Add (onOffHelper.Install (fillNodes.Get (i)));
    }

  fillApps.Start (Seconds (1.0));
  fillApps.Stop (Seconds (10.0));

  NS_LOG_INFO ("Enable static global routing.");
  //
  // Turn on global static routing so we can actually be routed across the star.
  //
  GlobalRouteManager::PopulateRoutingTables ();

  NS_LOG_INFO ("Enable pcap tracing.");
  //
  // Do pcap tracing on all devices on all nodes.
  //
  CsmaHelper::EnablePcapAll ("csma-star");

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
