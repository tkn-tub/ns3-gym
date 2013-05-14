/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of Washington, 2012 INRIA
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
 * Author: Alina Quereilhac <alina.quereilhac@inria.fr>
 *
 */

// +----------------------+     +-----------------------+
// |      client host     |     |      server host      |
// +----------------------+     +-----------------------+
// |     ns-3 Node 0      |     |      ns-3 Node 1      |
// |  +----------------+  |     |   +----------------+  |
// |  |    ns-3 TCP    |  |     |   |    ns-3 TCP    |  |
// |  +----------------+  |     |   +----------------+  |
// |  |    ns-3 IPv4   |  |     |   |    ns-3 IPv4   |  |
// |  +----------------+  |     |   +----------------+  |
// |  |   FdNetDevice  |  |     |   |   FdNetDevice  |  |
// |  |    10.1.1.1    |  |     |   |    10.1.1.2    |  |
// |  +----------------+  |     |   +----------------+  |
// |  |   raw socket   |  |     |   |   raw socket   |  |
// |  +----------------+  |     |   +----------------+  |
// |       | eth0 |       |     |        | eth0 |       |
// +-------+------+-------+     +--------+------+-------+
//
//         10.1.1.11                     10.1.1.12
//
//             |                            |
//             +----------------------------+
//
// This example is aimed at measuring the throughput of the FdNetDevice
// when using the EmuFdNetDeviceHelper. This is achieved by saturating
// the channel with TCP traffic. Then the throughput can be obtained from 
// the generated .pcap files.
//
// To run this example you will need two hosts (client & server).
// Steps to run the experiment:
//
// 1 - Connect the 2 computers with an Ethernet cable.
// 2 - Set the IP addresses on both Ethernet devices.
//
// client machine: $ sudo ip addr add dev eth0 10.1.1.11/24
// server machine: $ sudo ip addr add dev eth0 10.1.1.12/24
//
// 3 - Set both Ethernet devices to promiscuous mode.
//
// both machines: $ sudo ip link set eth0 promisc on
//
// 4 - Give root suid to the raw socket creator binary.
//     If the --enable-sudo option was used to configure ns-3 with waf, then the following
//     step will not be necessary.
//
// both hosts: $ sudo chown root.root build/src/fd-net-device/ns3-dev-raw-sock-creator
// both hosts: $ sudo chmod 4755 build/src/fd-net-device/ns3-dev-raw-sock-creator
//
// 5 - Run the server side:
//
// server host: $ ./waf --run="fd-emu-onoff --serverMode=1"
//
// 6 - Run the client side:
//       
// client host: $ ./waf --run="fd-emu-onoff"
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/fd-net-device-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("EmuFdNetDeviceSaturationExample");

int 
main (int argc, char *argv[])
{
  uint16_t sinkPort = 8000;
  uint32_t packetSize = 10000; // bytes
  std::string dataRate("1000Mb/s");
  bool serverMode = false;

  std::string deviceName ("eth0");
  std::string client ("10.1.1.1");
  std::string server ("10.1.1.2");
  std::string netmask ("255.255.255.0");
  std::string macClient ("00:00:00:00:00:01");
  std::string macServer ("00:00:00:00:00:02");

  CommandLine cmd;
  cmd.AddValue ("deviceName", "Device name", deviceName);
  cmd.AddValue ("client", "Local IP address (dotted decimal only please)", client);
  cmd.AddValue ("server", "Remote IP address (dotted decimal only please)", server);
  cmd.AddValue ("localmask", "Local mask address (dotted decimal only please)", netmask);
  cmd.AddValue ("serverMode", "1:true, 0:false, default client", serverMode);
  cmd.AddValue ("mac-client", "Mac Address for Server Client : 00:00:00:00:00:01", macClient);
  cmd.AddValue ("mac-server", "Mac Address for Server Default : 00:00:00:00:00:02", macServer);
  cmd.AddValue ("data-rate", "Data rate defaults to 1000Mb/s", dataRate);
  cmd.Parse (argc, argv);

  Ipv4Address remoteIp;
  Ipv4Address localIp;
  Mac48AddressValue localMac;
  
  if (serverMode)
  {
     remoteIp = Ipv4Address (client.c_str ());
     localIp = Ipv4Address (server.c_str ());
     localMac = Mac48AddressValue (macServer.c_str ());
  }
  else
  {
     remoteIp = Ipv4Address (server.c_str ());
     localIp = Ipv4Address (client.c_str ());
     localMac =  Mac48AddressValue (macClient.c_str ());
  }

  Ipv4Mask localMask (netmask.c_str ());
  
  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));

  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  NS_LOG_INFO ("Create Node");
  Ptr<Node> node = CreateObject<Node> ();

  NS_LOG_INFO ("Create Device");
  EmuFdNetDeviceHelper emu;
  emu.SetDeviceName (deviceName);
  NetDeviceContainer devices = emu.Install (node);
  Ptr<NetDevice> device = devices.Get (0);
  device->SetAttribute ("Address", localMac);

  NS_LOG_INFO ("Add Internet Stack");
  InternetStackHelper internetStackHelper;
  internetStackHelper.SetIpv4StackInstall(true);
  internetStackHelper.Install (node);

  NS_LOG_INFO ("Create IPv4 Interface");
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  uint32_t interface = ipv4->AddInterface (device);
  Ipv4InterfaceAddress address = Ipv4InterfaceAddress (localIp, localMask);
  ipv4->AddAddress (interface, address);
  ipv4->SetMetric (interface, 1);
  ipv4->SetUp (interface);

  if(serverMode)
  {
    Address sinkLocalAddress (InetSocketAddress (localIp, sinkPort));
    PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
    ApplicationContainer sinkApp = sinkHelper.Install (node);
    sinkApp.Start (Seconds (1.0));
    sinkApp.Stop (Seconds (60.0));
    
    emu.EnablePcap ("fd-server", device);
  }
  else
  {
    AddressValue remoteAddress (InetSocketAddress (remoteIp, sinkPort));
    OnOffHelper onoff ("ns3::TcpSocketFactory", Address ());
    onoff.SetAttribute ("Remote", remoteAddress);
    onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
    onoff.SetAttribute ("DataRate", DataRateValue (dataRate));
    onoff.SetAttribute ("PacketSize", UintegerValue (packetSize));

    ApplicationContainer clientApps = onoff.Install (node);
    clientApps.Start (Seconds (4.0));
    clientApps.Stop (Seconds (58.0));

    emu.EnablePcap ("fd-client", device);
  }

  Simulator::Stop (Seconds (61.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

