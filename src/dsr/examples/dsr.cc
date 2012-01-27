/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Yufei Cheng
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
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/dsr-module.h"

NS_LOG_COMPONENT_DEFINE ("DsrExample");

using namespace ns3;

int main (int argc, char *argv[])
{
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
#if 0
  LogComponentEnable ("Ipv4L3Protocol", LOG_LEVEL_ALL);
  LogComponentEnable ("Ipv4L4Protocol", LOG_LEVEL_ALL);
  LogComponentEnable ("UdpL4Protocol", LOG_LEVEL_ALL);
  LogComponentEnable ("NetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("Ipv4EndPointDemux", LOG_LEVEL_ALL);
#endif

#if 0
  LogComponentEnable ("DsrOptions", LOG_LEVEL_ALL);
  LogComponentEnable ("DsrRouting", LOG_LEVEL_ALL);
  LogComponentEnable ("DsrOptionHeader", LOG_LEVEL_ALL);
  LogComponentEnable ("DsrFsHeader", LOG_LEVEL_ALL);
  LogComponentEnable ("DsrGraReplyTable", LOG_LEVEL_ALL);
  LogComponentEnable ("DsrSendBuffer", LOG_LEVEL_ALL);
  LogComponentEnable ("RouteCache", LOG_LEVEL_ALL);
  LogComponentEnable ("DsrMaintainBuffer", LOG_LEVEL_ALL);
  LogComponentEnable ("RreqTable", LOG_LEVEL_ALL);
#endif

  SeedManager::SetSeed (99);
  SeedManager::SetRun (1);

  double totalTime = 1000.0;
  double dataStart = 50.0;
  uint32_t nWifis = 50;
  uint32_t nSinks = 10;
  double txp = 8.9048;
  double pauseTime = 0.0;
  double nodeSpeed = 20.0;
  uint32_t packetSize = 64;
  std::string phyModeControl ("DsssRate11Mbps");
  std::string phyModeData ("DsssRate11Mbps");
  std::string rate = "0.512kbps";
  double ppers = 1.0;
  /*
   * Define the DSR parameters
   */
  uint32_t m_maxCacheLen = 64;
  Time m_maxCacheTime = Seconds (30);
  Time m_nodeTraversalTime = MicroSeconds (2);
  Time m_passiveAckTimeout = MicroSeconds (4);
  uint32_t m_maxSendBuffLen = 64;
  Time m_sendBufferTimeout = Seconds (30);
  uint32_t m_maxMaintainLen = 50;
  Time m_maxMaintainTime = Seconds (30);
  uint32_t m_maintenanceRetries = 2;
  std::string cacheType ("PathCache");            // PathCache
  bool enableSubRoute = false;

  //Allow users to override the default parameters and set it to new ones from CommandLine.
  CommandLine cmd;
  cmd.AddValue ("MaxCacheLen", "Max route cache length.", m_maxCacheLen);
  cmd.AddValue ("RouteCacheTimeout", "Max route cache timeout.", m_maxCacheTime);
  cmd.AddValue ("NodeTraversalTime", "The time it takes to travel to neighboring nodes.", m_nodeTraversalTime);
  cmd.AddValue ("PassiveAckTimeout", "The time for ack to traversal the two neighboring nodes.", m_passiveAckTimeout);
  cmd.AddValue ("MaxSendBuffLen", "Maximum number of packets that can be stored.", m_maxSendBuffLen);
  cmd.AddValue ("MaxSendBuffTime", "Maximum time packets can be queued.", m_sendBufferTimeout);
  cmd.AddValue ("MaxMaintLen", "Maximum number of packets that can be stored.", m_maxMaintainLen);
  cmd.AddValue ("MaxMaintTime", "Maximum time packets can be queued.", m_maxMaintainTime);
  cmd.AddValue ("MaintenanceRetries", "Maximum retransmission retries for maintenance data packet.", m_maintenanceRetries);
  cmd.AddValue ("CacheType", "route cache type, Default:PathCache", cacheType);
  cmd.AddValue ("EnableSubRoute", "whether to enable the sub route mechanism, Default:false", enableSubRoute);
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (nWifis);
  NetDeviceContainer devices;

  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue (phyModeData));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));

  NS_LOG_INFO ("Create channels.");
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue (phyModeData),
                                "ControlMode", StringValue (phyModeControl));
  wifiPhy.Set ("TxPowerStart", DoubleValue (txp));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (txp));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  devices = wifi.Install (wifiPhy, wifiMac, nodes);

  InternetStackHelper internet;
  DsrMainHelper dsrMain;
  DsrHelper dsr;
  dsr.Set ("MaxCacheLen", UintegerValue (m_maxCacheLen));
  dsr.Set ("RouteCacheTimeout", TimeValue (m_maxCacheTime));
  dsr.Set ("NodeTraversalTime", TimeValue (m_nodeTraversalTime));
  dsr.Set ("PassiveAckTimeout", TimeValue (m_passiveAckTimeout));
  dsr.Set ("MaxSendBuffLen", UintegerValue (m_maxSendBuffLen));
  dsr.Set ("MaxSendBuffTime", TimeValue (m_sendBufferTimeout));
  dsr.Set ("MaxMaintLen", UintegerValue (m_maxMaintainLen));
  dsr.Set ("MaxMaintTime", TimeValue (m_maxMaintainTime));
  dsr.Set ("MaintenanceRetries", UintegerValue (m_maintenanceRetries));
  dsr.Set ("EnableSubRoute", BooleanValue (false));
  dsr.Set ("CacheType", StringValue (cacheType));
  dsr.Set ("SendBuffInterval", TimeValue (Seconds (50)));
  internet.Install (nodes);
  dsrMain.Install (dsr, nodes);

  NS_LOG_INFO ("assigning ip address");
  Ipv4AddressHelper addressAdhoc;
  addressAdhoc.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer adhocInterfaces;
  adhocInterfaces = addressAdhoc.Assign (devices);

  MobilityHelper mobility;

  ObjectFactory pos;
  pos.SetTypeId ("ns3::RandomRectanglePositionAllocator");
  pos.Set ("X", RandomVariableValue (UniformVariable (0.0, 300.0)));
  pos.Set ("Y", RandomVariableValue (UniformVariable (0.0, 1500.0)));
  Ptr<PositionAllocator> positionAlloc = pos.Create ()->GetObject<PositionAllocator> ();
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
                             "Speed", RandomVariableValue (ConstantVariable (nodeSpeed)),
                             "Pause", RandomVariableValue (ConstantVariable (pauseTime)),
                             "PositionAllocator", PointerValue (positionAlloc));
  mobility.Install (nodes);

  // many to many application
  uint16_t port = 9;
  double randomStartTime = (1 / ppers) / nSinks;     //distributed btw 1s evenly as we are sending 1pkt/s

  for (uint32_t i = 0; i < nSinks; i++)
    {
      PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
      ApplicationContainer apps_sink = sink.Install (nodes.Get (i));
      apps_sink.Start (Seconds (0.0));
      apps_sink.Stop (Seconds (totalTime));

      OnOffHelper onoff1 ("ns3::UdpSocketFactory", Address (InetSocketAddress (adhocInterfaces.GetAddress (i), port)));
      onoff1.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
      onoff1.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
      onoff1.SetAttribute ("PacketSize", UintegerValue (packetSize));
      onoff1.SetAttribute ("DataRate", DataRateValue (DataRate (rate)));

      ApplicationContainer apps1 = onoff1.Install (nodes.Get (i + nSinks));
      apps1.Start (Seconds (dataStart + i * randomStartTime));
      apps1.Stop (Seconds (totalTime));
    }

  AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream ("dsrtest.tr");
  wifiPhy.EnableAsciiAll (stream);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (totalTime));
  Simulator::Run ();
  Simulator::Destroy ();
}
