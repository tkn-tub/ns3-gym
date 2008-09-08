/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/mobility-module.h"
#include "ns3/helper-module.h"
#include "ns3/wifi-module.h"
#include "ns3/node-module.h"
#include "ns3/global-route-manager.h"
#include <vector>
#include <stdint.h>
#include <sstream>

using namespace ns3;

Ptr<WifiChannel>
CreateChannel (void)
{
  Ptr<WifiChannel> channel = CreateObject<WifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  log->SetReferenceModel (CreateObject<FriisPropagationLossModel> ());
  channel->SetPropagationLossModel (log);
  return channel;
}

int main (int argc, char *argv[])
{
  uint32_t nWifis = 2;
  uint32_t nStas = 2;
  bool sendIp = true;

  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);

  CommandLine cmd;
  cmd.AddValue ("nWifis", "Number of wifi networks", nWifis);
  cmd.AddValue ("nStas", "Number of stations per wifi network", nStas);
  cmd.AddValue ("SendIp", "Send Ipv4 or raw packets", sendIp);
  cmd.Parse (argc, argv);

  NodeContainer backboneNodes;
  NetDeviceContainer backboneDevices;
  Ipv4InterfaceContainer backboneInterfaces;
  std::vector<NodeContainer> staNodes;
  std::vector<NetDeviceContainer> staDevices;
  std::vector<NetDeviceContainer> apDevices;
  std::vector<Ipv4InterfaceContainer> staInterfaces;
  std::vector<Ipv4InterfaceContainer> apInterfaces;

  InternetStackHelper stack;
  CsmaHelper csma;
  Ipv4AddressHelper ip;
  ip.SetBase ("192.168.0.0", "255.255.255.0");

  backboneNodes.Create (nWifis);
  stack.Install (backboneNodes);

  backboneDevices = csma.Install (backboneNodes);
  backboneInterfaces = ip.Assign (backboneDevices);

  double wifiX = 0.0;
  for (uint32_t i = 0; i < nWifis; ++i)
    {
      // calculate ssid for wifi subnetwork
      std::ostringstream oss;
      oss << "wifi-default-" << i;
      Ssid ssid = Ssid (oss.str ());

      NodeContainer sta;
      NetDeviceContainer staDev;
      NetDeviceContainer apDev;
      Ipv4InterfaceContainer staInterface;
      Ipv4InterfaceContainer apInterface;
      MobilityHelper mobility;
      BridgeHelper bridge;
      WifiHelper wifi;
      Ptr<WifiChannel> channel;

      sta.Create (nStas);
      wifi.SetPhy ("ns3::WifiPhy");
      channel = CreateChannel ();
      ip.NewNetwork ();
      mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
				     "MinX", DoubleValue (wifiX),
				     "MinY", DoubleValue (0.0),
				     "DeltaX", DoubleValue (5.0),
				     "DeltaY", DoubleValue (5.0),
				     "GridWidth", UintegerValue (1),
				     "LayoutType", StringValue ("RowFirst"));


      // setup the AP.
      mobility.SetMobilityModel ("ns3::StaticMobilityModel");
      mobility.Install (backboneNodes.Get (i));
      wifi.SetMac ("ns3::NqapWifiMac",
		   "Ssid", SsidValue (ssid),
		   "BeaconGeneration", BooleanValue (true),
		   "BeaconInterval", TimeValue (Seconds (2.5)));
      apDev = wifi.Install (backboneNodes.Get (i), channel);
      apInterface = ip.Assign (apDev);
      bridge.Install (backboneNodes.Get (i), NetDeviceContainer (apDev, backboneDevices.Get (i)));

      // setup the STAs
      stack.Install (sta);
      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
				 "Mode", StringValue ("Time"),
				 "Time", StringValue ("2s"),
				 "Speed", StringValue ("Constant:1.0"),
				 "Bounds", RectangleValue (Rectangle (wifiX, wifiX+5.0,0.0, (nStas+1)*5.0)));
      mobility.Install (sta);
      wifi.SetMac ("ns3::NqstaWifiMac",
		   "Ssid", SsidValue (ssid),
		   "ActiveProbing", BooleanValue (false));
      staDev = wifi.Install (sta, channel);
      staInterface = ip.Assign (staDev);

      // save everything in containers.
      staNodes.push_back (sta);
      apDevices.push_back (apDev);
      apInterfaces.push_back (apInterface);
      staDevices.push_back (staDev);
      staInterfaces.push_back (staInterface);

      wifiX += 20.0;
    }

  GlobalRouteManager::PopulateRoutingTables ();

  Address dest;
  std::string protocol;
  if (sendIp)
    {
      dest = InetSocketAddress (staInterfaces[1].GetAddress (1), 1025);
      protocol = "ns3::UdpSocketFactory";
    }
  else
    {
      PacketSocketAddress tmp;
      tmp.SetSingleDevice (staDevices[0].Get (0)->GetIfIndex ());
      tmp.SetPhysicalAddress (staDevices[1].Get (0)->GetAddress ());
      tmp.SetProtocol (0x807);
      dest = tmp;
      protocol = "ns3::PacketSocketFactory";
    }
  OnOffHelper onoff = OnOffHelper (protocol, dest);
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  ApplicationContainer apps = onoff.Install (staNodes[0].Get (0));
  apps.Start (Seconds (0.5));
  apps.Stop (Seconds (3.0));
  

  WifiHelper::EnablePcap ("wifi-wire-bridging", staNodes[1].Get (1));
  WifiHelper::EnablePcap ("wifi-wire-bridging", staNodes[0].Get (0));
  std::ofstream os;
  os.open ("wifi-wire-bridging.mob");
  MobilityHelper::EnableAsciiAll (os);

  Simulator::Stop (Seconds (100.0));
  Simulator::Run ();
  Simulator::Destroy ();
}
