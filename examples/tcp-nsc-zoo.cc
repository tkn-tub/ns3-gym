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


// Network topology
//
//       n0    n1   n2   n3
//       |     |    |    |
//       =================
//              LAN
//
// - Pcap traces are saved as tcp-nsc-zoo-$n-0.pcap, where $n represents the node number
// - TCP flows from n0 to n1, n2, n3, from n1 to n0, n2, n3, etc.
//  Usage (e.g.): ./waf --run 'tcp-nsc-zoo --nodes=5'

#include <iostream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/helper-module.h"
#include "ns3/node-module.h"
#include "ns3/simulator-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpNscZoo");

// Simulates a diverse network with various stacks supported by NSC.
int main(int argc, char *argv[])
{
  CsmaHelper csma;
  unsigned int MaxNodes = 4;
  unsigned int runtime = 3;

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (2048));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("8kbps"));
  CommandLine cmd;
  // this allows the user to raise the number of nodes using --nodes=X command-line argument.
  cmd.AddValue("nodes", "Number of nodes in the network (must be > 1)", MaxNodes);
  cmd.AddValue("runtime", "How long the applications should send data (default 3 seconds)", runtime);
  cmd.Parse (argc, argv);

  if (MaxNodes < 2)
    {
       std::cerr << "--nodes: must be >= 2" << std::endl;
       return 1;
    }
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate(100 * 1000 * 1000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MicroSeconds (200)));

  NodeContainer n;
  n.Create(MaxNodes);
  NetDeviceContainer ethInterfaces = csma.Install (n);

  InternetStackHelper internetStack;
  internetStack.SetTcp ("ns3::NscTcpL4Protocol","Library",StringValue("liblinux2.6.26.so"));
  // this switches nodes 0 and 1 to NSCs Linux 2.6.26 stack.
  internetStack.Install (n.Get(0));
  internetStack.Install (n.Get(1));
  // this disables TCP SACK, wscale and timestamps on node 1 (the attributes represent sysctl-values).
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_sack", StringValue ("0"));
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_timestamps", StringValue ("0"));
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_window_scaling", StringValue ("0"));

  if (MaxNodes > 2)
    {
      internetStack.Install (n.Get(2));
    }

  if (MaxNodes > 3)
    {
      // the next statement doesn't change anything for the nodes 0, 1, and 2; since they
      // already have a stack assigned.
      internetStack.SetTcp ("ns3::NscTcpL4Protocol","Library",StringValue("liblinux2.6.18.so"));
      // this switches node 3 to NSCs Linux 2.6.18 stack.
      internetStack.Install (n.Get(3));
      // and then agains disables sack/timestamps/wscale on node 3.
      Config::Set ("/NodeList/3/$ns3::Ns3NscStack<linux2.6.18>/net.ipv4.tcp_sack", StringValue ("0"));
      Config::Set ("/NodeList/3/$ns3::Ns3NscStack<linux2.6.18>/net.ipv4.tcp_timestamps", StringValue ("0"));
      Config::Set ("/NodeList/3/$ns3::Ns3NscStack<linux2.6.18>/net.ipv4.tcp_window_scaling", StringValue ("0"));
    }
  // the freebsd stack is not yet built by default, so its commented out for now.
  // internetStack.SetNscStack ("libfreebsd5.so");
  for (unsigned int i =4; i < MaxNodes; i++)
    {
      internetStack.Install (n.Get(i));
    }
  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interfaces = ipv4.Assign (ethInterfaces);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  uint16_t servPort = 8080;
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), servPort));
  // start a sink client on all nodes
  ApplicationContainer sinkApp = sinkHelper.Install (n);
  sinkApp.Start (Seconds (0));
  sinkApp.Stop (Seconds (30.0));

  // This tells every node on the network to start a flow to all other nodes on the network ...
  for (unsigned int i = 0 ; i < MaxNodes;i++)
    {
      for (unsigned int j = 0 ; j < MaxNodes;j++)
        {
          if (i == j)
            {  // ...but we don't want a node to talk to itself.
               continue;
            }
          Address remoteAddress(InetSocketAddress(ipv4Interfaces.GetAddress (j), servPort));
          OnOffHelper clientHelper ("ns3::TcpSocketFactory", remoteAddress);
          clientHelper.SetAttribute 
            ("OnTime", RandomVariableValue (ConstantVariable (1)));
          clientHelper.SetAttribute 
            ("OffTime", RandomVariableValue (ConstantVariable (0)));
          ApplicationContainer clientApp = clientHelper.Install(n.Get(i));
          clientApp.Start (Seconds (j)); /* delay startup depending on node number */
          clientApp.Stop (Seconds (j + runtime));
	}
    }

  CsmaHelper::EnablePcapAll ("tcp-nsc-zoo", false);

  Simulator::Stop (Seconds(100));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
