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
// Network topology
//
//           6Mb/s, 500ms
//       n0-----------------n1
//
// - a 'lossy' network with long delay
// - TCP flow from n0 to n1 and from n1 to n0
// - pcap traces generated as tcp-nsc-lfn-0-0.pcap and tcp-nsc-lfn-1-0.pcap
//  Usage (e.g.): ./waf --run 'tcp-nsc-lfn --TCP_CONGESTION=hybla --runtime=30'

#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/helper-module.h"
#include "ns3/node-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/simulator-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpNscLfn");

static void 
CwndTracer (uint32_t oldval, uint32_t newval)
{
  NS_LOG_INFO ("Moving cwnd from " << oldval << " to " << newval);
}

int main (int argc, char *argv[])
{

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (4096));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("6Mbps"));

  // cubic is the default congestion algorithm in Linux 2.6.26
  std::string tcpCong = "cubic";
  // this is the default error rate of our link, that is, the the probability of a single
  // byte being 'corrupted' during transfer.
  double errRate = 0.000001;
  // how long the sender should be running, in seconds.
  unsigned int runtime = 120;
  // the name of the NSC stack library that should be used
  std::string nscStack = "liblinux2.6.26.so";

  CommandLine cmd;
  // Here, we define additional command line options.
  // This allows a user to override the defaults set above from the command line.
  cmd.AddValue("TCP_CONGESTION", "Linux 2.6.26 Tcp Congestion control algorithm to use", tcpCong);
  cmd.AddValue("error-rate", "Error rate to apply to link", errRate);
  cmd.AddValue("runtime", "How long the applications should send data (default 120 seconds)", runtime);
  cmd.AddValue("nscstack", "Set name of NSC stack (shared library) to use (default liblinux2.6.26.so)", nscStack);
  cmd.Parse (argc, argv);

  NodeContainer n;
  n.Create (2);

  PointToPointHelper p2p;
  // create point-to-point link with a bandwidth of 6MBit/s and a large delay (0.5 seconds)
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate(6 * 1000 * 1000)));
  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds(500)));

  NetDeviceContainer p2pInterfaces = p2p.Install (n);
  // The default MTU of the p2p link would be 65535, which doesn't work
  // well with our default errRate (most packets would arrive corrupted).
  p2pInterfaces.Get(0)->SetMtu(1500);
  p2pInterfaces.Get(1)->SetMtu(1500);

  InternetStackHelper internet;
  // The next statement switches the nodes to 'NSC'-Mode.
  // It disables the native ns-3 TCP model and loads the NSC library.
  internet.SetTcp ("ns3::NscTcpL4Protocol","Library",StringValue(nscStack));
  internet.Install (n);

  if (tcpCong != "cubic") // make sure we only fail if both --nscstack and --TCP_CONGESTION are used
    {
      // This uses ns-3s attribute system to set the 'net.ipv4.tcp_congestion_control' sysctl of the
      // stack.
      // The same mechanism could be used to e.g. disable TCP timestamps:
      // Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_timestamps", StringValue ("0"));
      Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_congestion_control", StringValue (tcpCong));
    }
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interfaces = ipv4.Assign (p2pInterfaces);

  DoubleValue rate(errRate);
  RandomVariableValue u01(UniformVariable (0.0, 1.0));
  Ptr<RateErrorModel> em1 = 
      CreateObject<RateErrorModel> ("RanVar", u01, "ErrorRate", rate);
  Ptr<RateErrorModel> em2 = 
      CreateObject<RateErrorModel> ("RanVar", u01, "ErrorRate", rate);

  // This enables the specified errRate on both link endpoints.
  p2pInterfaces.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue (em1));
  p2pInterfaces.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue (em2));

  GlobalRouteManager::PopulateRoutingTables ();

  uint16_t servPort = 8080;
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), servPort));
  ApplicationContainer sinkApp = sinkHelper.Install (n);
  sinkApp.Start (Seconds (0.0));
  // this makes sure that the receiver will run one minute longer than the sender applicaton.
  sinkApp.Stop (Seconds (runtime + 60.0));

  // This sets up two TCP flows, one from A -> B, one from B -> A.
  for (int i = 0, j = 1; i < 2; j--, i++)
   {
      Address remoteAddress(InetSocketAddress(ipv4Interfaces.GetAddress (i), servPort));
      OnOffHelper clientHelper ("ns3::TcpSocketFactory", remoteAddress);
      clientHelper.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
      clientHelper.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
      ApplicationContainer clientApp = clientHelper.Install(n.Get(j));
      clientApp.Start (Seconds (1.0 + i));
      clientApp.Stop (Seconds (runtime + 1.0 + i));
    }

  // Trace changes to the congestion window
  Config::ConnectWithoutContext ("/NodeList/1/$ns3::NscTcpL4Protocol/SocketList/0/CongestionWindow", 
                                 MakeCallback (&CwndTracer));

  // This tells ns-3 to generate pcap traces.
  PointToPointHelper::EnablePcapAll ("tcp-nsc-lfn");

  Simulator::Stop (Seconds(900));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
