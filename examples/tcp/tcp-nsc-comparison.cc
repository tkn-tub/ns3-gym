/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 NICT
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
 * Author: Hajime Tazaki <tazaki@nict.go.jp>
 *
 * This code is a modified version of the code used for the the experiments in the paper
 * "DCE Cradle: Simulate Network Protocols with Real Stacks for Better Realism"
 * by Hajime Tazaki, Frederic Urbani and Thierry Turlett presented at WNS3 2013
 *
 */

#include "ns3/log.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpNscComparison");

std::string m_stack = "nsc-linux";
std::string sock_factory;
uint32_t m_seed = 1;
double startTime = 4.0;
double stopTime = 20.0;
uint32_t m_nNodes = 2;
bool enablePcap = false;

int
main (int argc, char *argv[])
{

  //ensure the ns3 TCP default values match what nsc is using
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1448));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));

  CommandLine cmd;
  cmd.AddValue ("stack", "choose network stack", m_stack);
  cmd.AddValue ("seed", "randomize seed", m_seed);
  cmd.AddValue ("nNodes", "the number of nodes in left side", m_nNodes);
  cmd.AddValue ("stopTime", "duration", stopTime);
  cmd.AddValue ("enablePcap", "pcap", enablePcap);
  cmd.Parse (argc, argv);

  SeedManager::SetSeed (m_seed);

  NodeContainer lefts, routers, rights, nodes;
  lefts.Create (m_nNodes);
  routers.Create (2);
  rights.Create (m_nNodes);
  nodes = NodeContainer (lefts, routers, rights);

  InternetStackHelper internetStack;

  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));
  if (m_stack == "ns3")
    {
      sock_factory = "ns3::TcpSocketFactory";
      internetStack.Install (nodes);
    }
  else if (m_stack == "nsc-linux")
    {
      internetStack.Install (routers);
      sock_factory = "ns3::TcpSocketFactory";
      internetStack.SetTcp ("ns3::NscTcpL4Protocol",
                            "Library", StringValue ("liblinux2.6.26.so"));
      internetStack.Install (lefts);
      internetStack.Install (rights);

      //these are not implemented in ns3 tcp so disable for comparison
      Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_sack", StringValue ("0"));
      Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_timestamps", StringValue ("0"));
      Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_window_scaling", StringValue ("0"));
    }

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ns"));

  Ipv4AddressHelper address;
  Ipv4InterfaceContainer interfaces;

  NetDeviceContainer dev0, dev1, dev2;
  for (uint32_t i = 0; i < m_nNodes; i++)
    {
      std::ostringstream oss;
      oss << "10.0." << i << ".0";
      address.SetBase (oss.str ().c_str (), "255.255.255.0");
      dev0 = pointToPoint.Install (NodeContainer (lefts.Get (i), routers.Get (0)));
      address.Assign (dev0);
    }

  // bottle neck link
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("100ms"));
  dev1 = pointToPoint.Install (NodeContainer (routers.Get (0), routers.Get (1)));

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ns"));
  // for right links
  for (uint32_t i = 0; i < m_nNodes; i++)
    {
      std::ostringstream oss;
      oss << "10.2." << i << ".0";
      address.SetBase (oss.str ().c_str (), "255.255.255.0");
      dev2 = pointToPoint.Install (NodeContainer (routers.Get (1), rights.Get (i)));
      address.Assign (dev2);
    }

  // bottle neck link
  Ptr<RateErrorModel> em1 =
    CreateObjectWithAttributes<RateErrorModel> (
      "ErrorRate", DoubleValue (0.05),
      "ErrorUnit", EnumValue (RateErrorModel::ERROR_UNIT_PACKET)
      );
  dev1.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em1));

  address.SetBase ("10.1.0.0", "255.255.255.0");
  address.Assign (dev1);
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  ApplicationContainer apps;

  OnOffHelper onoff = OnOffHelper (sock_factory,
                                   InetSocketAddress (Ipv4Address ("10.2.0.2"), 2000));
  onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  // Flow 1 - n
  for (uint32_t i = 0; i < m_nNodes; i++)
    {
      std::ostringstream oss;
      oss << "10.2." << i << ".2";
      onoff.SetAttribute ("Remote", AddressValue (InetSocketAddress (Ipv4Address (oss.str ().c_str ()), 2000)));
      onoff.SetAttribute ("PacketSize", StringValue ("1024"));
      onoff.SetAttribute ("DataRate", StringValue ("1Mbps"));
      onoff.SetAttribute ("StartTime", TimeValue (Seconds (startTime)));
      apps = onoff.Install (lefts.Get (i));
    }

  PacketSinkHelper sink = PacketSinkHelper (sock_factory,
                                            InetSocketAddress (Ipv4Address::GetAny (), 2000));
  apps = sink.Install (rights);
  apps.Start (Seconds (3.9999));

  pointToPoint.EnablePcapAll ("nsc.pcap");

  Simulator::Stop (Seconds (stopTime));
  Simulator::Run ();

  Ptr<PacketSink> pktsink;
  std::cout << "Total ";
  for (uint32_t i = 0; i < m_nNodes; i++)
    {
      pktsink = apps.Get (i)->GetObject<PacketSink> ();
      std::cout << "Rx(" << i << ") = " << pktsink->GetTotalRx () <<
      " bytes (" << pktsink->GetTotalRx () * 8 / (stopTime - startTime) << " bps), ";
    }
  std::cout << std::endl;

  Simulator::Destroy ();
  return 0;
}
