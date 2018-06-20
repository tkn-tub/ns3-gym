/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017
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
 * Author: Sebastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/command-line.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/ssid.h"
#include "ns3/wifi-mac-header.h"

/* This is a simple example in order to show the frames exchanged in 802.11 PCF.
 * The output prints the overal throughput as well as the number of different PCF frames that have been transmitted.
 *
 * It is possible to tune some parameters using the command line:
 *   - number of connected stations
 *   - enable/disable PCF
 *   - enable PCAP output file generation in order to vizualise frame exchange.
 *   - configure UDP data traffic:
 *     -> enable/disable data generation: --withData=<0|1>
 *     -> select traffic direction: --trafficDirection=<upstream|downstream>
 *
 * For example, one can observe the benefit of PCF over DCF when the number of stations increased:
 *   ./waf --run "wifi-pcf enablePcf=0 --nWifi=10" => DCF only
 *   ./waf --run "wifi-pcf enablePcf=1 --nWifi=10" => alternance of PCF and DCF
 *
 * One can also change the value of cfpMaxDuration: a shorter valer means the granted time for PCF is shorter, and so it's benefit is reduced.
 *   ./waf --run "wifi-pcf enablePcf=1 --nWifi=10 --cfpMaxDuration=10240"
 *
 * One can also see the different types of piggybacked frames depending on the traffic direction and whether PCF is enabled or not:
 *   ./waf --run "wifi-pcf enablePcf=0 --nWifi=1" => only CF_POLL and DATA_NULL frames should be seen
 *   ./waf --run "wifi-pcf enablePcf=1 --nWifi=1 --trafficDirection=upstream" => no DATA_NULL frames should be seen
 *   ./waf --run "wifi-pcf enablePcf=1 --nWifi=1 --trafficDirection=downstream" => no CF_END_ACK frames should be seen
 */

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiPcf");

uint64_t m_countBeacon;
uint64_t m_countCfPoll;
uint64_t m_countCfPollAck;
uint64_t m_countCfPollData;
uint64_t m_countCfPollDataAck;
uint64_t m_countCfEnd;
uint64_t m_countCfEndAck;
uint64_t m_countDataNull;
uint64_t m_countData;

void TxCallback (std::string context, Ptr<const Packet> p)
{
  WifiMacHeader hdr;
  p->PeekHeader (hdr);
  if (hdr.IsBeacon ())
    {
      m_countBeacon++;
    }
  else if (hdr.IsCfPoll ())
    {
      if (hdr.IsCfAck () && hdr.HasData  ())
        {
          m_countCfPollDataAck++;
        }
      else if (!hdr.IsCfAck () && hdr.HasData ())
        {
          m_countCfPollData++;
        }
      else if (hdr.IsCfAck () && !hdr.HasData ())
        {
          m_countCfPollAck++;
        }
      else
        {
          m_countCfPoll++;
        }
    }
  else if (hdr.IsCfEnd ())
    {
      if (hdr.IsCfAck ())
        {
          m_countCfEndAck++;
        }
      else
        {
          m_countCfEnd++;
        }
    }
  else if (hdr.IsData ())
    {
      if (!hdr.HasData ())
        {
          m_countDataNull++;
        }
      else
        {
          m_countData++;
        }
    }
}

int main (int argc, char *argv[])
{
  uint32_t nWifi = 1;
  bool enablePcap = false;
  bool enablePcf = true;
  bool withData = true;
  std::string trafficDirection = "upstream";
  uint64_t cfpMaxDurationUs = 65536; //microseconds
  double simulationTime = 10; //seconds

  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("enablePcf", "Enable/disable PCF mode", enablePcf);
  cmd.AddValue ("withData", "Enable/disable UDP data packets generation", withData);
  cmd.AddValue ("trafficDirection", "Data traffic direction (if withData is set to 1): upstream (all STAs -> AP) or downstream (AP -> all STAs)", trafficDirection);
  cmd.AddValue ("cfpMaxDuration", "CFP max duration in microseconds", cfpMaxDurationUs);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("enablePcap", "Enable/disable PCAP output", enablePcap);
  cmd.Parse (argc, argv);

  m_countBeacon = 0;
  m_countCfEnd = 0;
  m_countCfEndAck = 0;
  m_countCfPoll = 0;
  m_countCfPollAck = 0;
  m_countCfPollData = 0;
  m_countCfPollDataAck = 0;
  m_countDataNull = 0;
  m_countData = 0;
  m_countDataNull = 0;
  m_countData = 0;

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  WifiMacHelper mac;

  Ssid ssid = Ssid ("wifi-pcf");
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate54Mbps"), "ControlMode", StringValue ("OfdmRate24Mbps"));

  NetDeviceContainer staDevices;
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false),
               "PcfSupported", BooleanValue (enablePcf));
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true),
               "PcfSupported", BooleanValue (enablePcf),
               "CfpMaxDuration", TimeValue (MicroSeconds (cfpMaxDurationUs)));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  for (uint32_t i = 0; i < nWifi; i++)
    {
      positionAlloc->Add (Vector (1.0, 0.0, 0.0));
    }
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNodes);

  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterface;
  StaInterface = address.Assign (staDevices);
  Ipv4InterfaceContainer ApInterface;
  ApInterface = address.Assign (apDevice);

  ApplicationContainer sourceApplications, sinkApplications;
  if (withData)
    {
      uint32_t portNumber = 9;
      for (uint32_t index = 0; index < nWifi; ++index)
        {
          auto ipv4 = (trafficDirection == "upstream") ? wifiApNode.Get (0)->GetObject<Ipv4> () : wifiStaNodes.Get (index)->GetObject<Ipv4> ();
          const auto address = ipv4->GetAddress (1, 0).GetLocal ();
          InetSocketAddress sinkSocket (address, portNumber++);
          OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
          onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
          onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
          onOffHelper.SetAttribute ("DataRate", DataRateValue (50000000 / nWifi));
          onOffHelper.SetAttribute ("PacketSize", UintegerValue (1472)); //bytes
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
          if (trafficDirection == "upstream")
            {
              sourceApplications.Add (onOffHelper.Install (wifiStaNodes.Get (index)));
              sinkApplications.Add (packetSinkHelper.Install (wifiApNode.Get (0)));
            }
          else if (trafficDirection == "downstream")
            {
              sinkApplications.Add (packetSinkHelper.Install (wifiStaNodes.Get (index)));
              sourceApplications.Add (onOffHelper.Install (wifiApNode.Get (0)));
            }
          else
            {
              NS_ASSERT_MSG (false, "Invalid trafficDirection!");
            }
        }
      sinkApplications.Start (Seconds (0.0));
      sinkApplications.Stop (Seconds (simulationTime + 1));
      sourceApplications.Start (Seconds (1.0));
      sourceApplications.Stop (Seconds (simulationTime + 1));
    }

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyTxBegin", MakeCallback (&TxCallback));

  if (enablePcap)
    {
      phy.EnablePcap ("wifi_pcf", apDevice.Get (0));
    }

  Simulator::Stop (Seconds (simulationTime + 1));
  Simulator::Run ();

  double throughput = 0;
  for (uint32_t index = 0; index < sinkApplications.GetN (); ++index)
    {
      uint64_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApplications.Get (index))->GetTotalRx ();
      throughput += ((totalPacketsThrough * 8) / (simulationTime * 1000000.0)); //Mbit/s
    }

  std::cout << "Throughput: " << throughput << " Mbit/s" << std::endl;

  std::cout << "# tx beacons: " << m_countBeacon << std::endl;
  std::cout << "# tx CF-END: " << m_countCfEnd << std::endl;
  std::cout << "# tx CF-END-ACK: " << m_countCfEndAck << std::endl;
  std::cout << "# tx CF-POLL: " << m_countCfPoll << std::endl;
  std::cout << "# tx CF-POLL-ACK: " << m_countCfPollAck << std::endl;
  std::cout << "# tx CF-POLL-DATA: " << m_countCfPollData << std::endl;
  std::cout << "# tx CF-POLL-DATA-ACK: " << m_countCfPollDataAck << std::endl;
  std::cout << "# tx DATA-NULL: " << m_countDataNull << std::endl;
  std::cout << "# tx DATA: " << m_countData << std::endl;

  Simulator::Destroy ();
  return 0;
}
