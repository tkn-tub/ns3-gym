/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/opengym-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/stats-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/node-list.h"

#include "mygym.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("OpenGym");

int
main (int argc, char *argv[])
{
  // Parameters of the environment
  uint32_t simSeed = 1;
  double simulationTime = 10; //seconds
  double envStepTime = 0.1; //seconds, ns3gym env step time interval
  uint32_t openGymPort = 5555;
  uint32_t testArg = 0;

  bool eventBasedEnv = true;

  //Parameters of the scenario
  uint32_t nodeNum = 5;
  double distance = 10.0;
  bool noErrors = false;
  std::string errorModelType = "ns3::NistErrorRateModel";
  bool enableFading = true;
  uint32_t pktPerSec = 1000;
  uint32_t payloadSize = 1500;
  bool enabledMinstrel = false;

  // define datarates
  std::vector<std::string> dataRates;
  dataRates.push_back("OfdmRate1_5MbpsBW5MHz");
  dataRates.push_back("OfdmRate2_25MbpsBW5MHz");
  dataRates.push_back("OfdmRate3MbpsBW5MHz");
  dataRates.push_back("OfdmRate4_5MbpsBW5MHz");
  dataRates.push_back("OfdmRate6MbpsBW5MHz");
  dataRates.push_back("OfdmRate9MbpsBW5MHz");
  dataRates.push_back("OfdmRate12MbpsBW5MHz");
  dataRates.push_back("OfdmRate13_5MbpsBW5MHz");
  uint32_t dataRateId = 1;


  CommandLine cmd;
  // required parameters for OpenGym interface
  cmd.AddValue ("openGymPort", "Port number for OpenGym env. Default: 5555", openGymPort);
  cmd.AddValue ("simSeed", "Seed for random generator. Default: 1", simSeed);
  // optional parameters
  cmd.AddValue ("eventBasedEnv", "Whether steps should be event or time based. Default: true", eventBasedEnv);
  cmd.AddValue ("simTime", "Simulation time in seconds. Default: 10s", simulationTime);
  cmd.AddValue ("nodeNum", "Number of nodes. Default: 5", nodeNum);
  cmd.AddValue ("distance", "Inter node distance. Default: 10m", distance);
  cmd.AddValue ("testArg", "Extra simulation argument. Default: 0", testArg);
  cmd.Parse (argc, argv);

  NS_LOG_UNCOND("Ns3Env parameters:");
  NS_LOG_UNCOND("--simulationTime: " << simulationTime);
  NS_LOG_UNCOND("--openGymPort: " << openGymPort);
  NS_LOG_UNCOND("--envStepTime: " << envStepTime);
  NS_LOG_UNCOND("--seed: " << simSeed);
  NS_LOG_UNCOND("--distance: " << distance);
  NS_LOG_UNCOND("--testArg: " << testArg);

  if (noErrors){
    errorModelType = "ns3::NoErrorRateModel";
  }

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (simSeed);

  // Configuration of the scenario
  // Create Nodes
  NodeContainer nodes;
  nodes.Create (nodeNum);

  // WiFi device
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211p);

  // Channel
  SpectrumWifiPhyHelper spectrumPhy = SpectrumWifiPhyHelper ();
  Ptr<MultiModelSpectrumChannel> spectrumChannel = CreateObject<MultiModelSpectrumChannel> ();

  spectrumPhy.SetChannel (spectrumChannel);
  spectrumPhy.SetErrorRateModel (errorModelType);
  spectrumPhy.Set ("Frequency", UintegerValue (5200));
  spectrumPhy.Set ("ChannelWidth", UintegerValue (5));
  spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (false));

  Config::SetDefault ("ns3::WifiPhy::CcaMode1Threshold", DoubleValue (-82.0));
  Config::SetDefault ("ns3::WifiPhy::Frequency", UintegerValue (5200));
  Config::SetDefault ("ns3::WifiPhy::ChannelWidth", UintegerValue (5));

  // Channel
  Ptr<FriisPropagationLossModel> lossModel = CreateObject<FriisPropagationLossModel> ();
  Ptr<NakagamiPropagationLossModel> fadingModel = CreateObject<NakagamiPropagationLossModel> ();
  if (enableFading) {
    lossModel->SetNext (fadingModel);
  }
  spectrumChannel->AddPropagationLossModel (lossModel);
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  spectrumChannel->SetPropagationDelayModel (delayModel);

  // Add MAC and set DataRate
  WifiMacHelper wifiMac;

  if (enabledMinstrel) {
    wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  } else {
    std::string dataRateStr = dataRates.at(dataRateId);
    NS_LOG_UNCOND("dataRateStr: " << dataRateStr);
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode", StringValue (dataRateStr),
                                  "ControlMode", StringValue (dataRateStr));
  }

  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac",
                   "QosSupported", BooleanValue (false));

  // Install wifi device
  NetDeviceContainer devices = wifi.Install (spectrumPhy, wifiMac, nodes);

  // Mobility model
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (distance),
                                 "DeltaY", DoubleValue (distance),
                                 "GridWidth", UintegerValue (nodeNum),  // will create linear topology
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  // IP stack and routing
  InternetStackHelper internet;
  internet.Install (nodes);

  // Assign IP addresses to devices
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  //Configure static multihop routing
  for (uint32_t i = 0; i < nodes.GetN()-1; i++){
    Ptr<Node> src = nodes.Get(i);
    Ptr<Node> nextHop = nodes.Get(i+1);
    Ptr<Ipv4> destIpv4 = nextHop->GetObject<Ipv4> ();
    Ipv4InterfaceAddress dest_ipv4_int_addr = destIpv4->GetAddress (1, 0);
    Ipv4Address dest_ip_addr = dest_ipv4_int_addr.GetLocal ();

    Ptr<Ipv4StaticRouting>  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (src->GetObject<Ipv4> ()->GetRoutingProtocol ());
    staticRouting->RemoveRoute(1);
    staticRouting->SetDefaultRoute(dest_ip_addr, 1, 0);
  }

  // Traffic
  // Create a BulkSendApplication and install it on node 0
  Ptr<UniformRandomVariable> startTimeRng = CreateObject<UniformRandomVariable> ();
  startTimeRng->SetAttribute ("Min", DoubleValue (0.0));
  startTimeRng->SetAttribute ("Max", DoubleValue (1.0));

  uint16_t port = 1000;
  uint32_t srcNodeId = 0;
  uint32_t destNodeId = nodes.GetN() - 1;
  Ptr<Node> srcNode = nodes.Get(srcNodeId);
  Ptr<Node> dstNode = nodes.Get(destNodeId);

  Ptr<Ipv4> destIpv4 = dstNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress dest_ipv4_int_addr = destIpv4->GetAddress (1, 0);
  Ipv4Address dest_ip_addr = dest_ipv4_int_addr.GetLocal ();

  InetSocketAddress destAddress (dest_ip_addr, port);
  destAddress.SetTos (0x70); //AC_BE
  UdpClientHelper source (destAddress);
  source.SetAttribute ("MaxPackets", UintegerValue (pktPerSec * simulationTime));
  source.SetAttribute ("PacketSize", UintegerValue (payloadSize));
  Time interPacketInterval = Seconds (1.0/pktPerSec);
  source.SetAttribute ("Interval", TimeValue (interPacketInterval)); //packets/s

  ApplicationContainer sourceApps = source.Install (srcNode);
  sourceApps.Start (Seconds (0.0));
  sourceApps.Stop (Seconds (simulationTime));

  // Create a packet sink to receive these packets
  UdpServerHelper sink (port);
  ApplicationContainer sinkApps = sink.Install (dstNode);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (simulationTime));

  // Print node positions
  NS_LOG_UNCOND ("Node Positions:");
  for (uint32_t i = 0; i < nodes.GetN(); i++)
  {
    Ptr<Node> node = nodes.Get(i);
    Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
    NS_LOG_UNCOND ("---Node ID: " << node->GetId() << " Positions: " << mobility->GetPosition());
  }

  // OpenGym Env
  Ptr<OpenGymInterface> openGymInterface = CreateObject<OpenGymInterface> (openGymPort);
  Ptr<MyGymEnv> myGymEnv;
  if (eventBasedEnv)
  {
    myGymEnv = CreateObject<MyGymEnv> ();
  } else {
    myGymEnv = CreateObject<MyGymEnv> (Seconds(envStepTime));
  }
  myGymEnv->SetOpenGymInterface(openGymInterface);

  // connect OpenGym entity to event source
  Ptr<UdpServer> udpServer = DynamicCast<UdpServer>(sinkApps.Get(0));
  if (eventBasedEnv)
  {
    udpServer->TraceConnectWithoutContext ("Rx", MakeBoundCallback (&MyGymEnv::NotifyPktRxEvent, myGymEnv, dstNode));
  } else {
    udpServer->TraceConnectWithoutContext ("Rx", MakeBoundCallback (&MyGymEnv::CountRxPkts, myGymEnv, dstNode));
  }

  NS_LOG_UNCOND ("Simulation start");
  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run ();
  NS_LOG_UNCOND ("Simulation stop");

  myGymEnv->NotifySimulationEnd();
  Simulator::Destroy ();

}
