/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */


#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"
#include "ns3/wifi-module.h"

#include <iostream>

using namespace ns3;

void
DevTxTrace (std::string context, Ptr<const Packet> p, Mac48Address address)
{
  std::cout << " TX to=" << address << " p: " << *p << std::endl;
}
void
DevRxTrace (std::string context, Ptr<const Packet> p, Mac48Address address)
{
  std::cout << " RX from=" << address << " p: " << *p << std::endl;
}
void
PhyRxOkTrace (std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  std::cout << "PHYRXOK mode=" << mode << " snr=" << snr << " " << *packet << std::endl;
}
void
PhyRxErrorTrace (std::string context, Ptr<const Packet> packet, double snr)
{
  std::cout << "PHYRXERROR snr=" << snr << " " << *packet << std::endl;
}
void
PhyTxTrace (std::string context, Ptr<const Packet> packet, WifiMode mode, WifiPreamble preamble, uint8_t txPower)
{
  std::cout << "PHYTX mode=" << mode << " " << *packet << std::endl;
}
void
PhyStateTrace (std::string context, Time start, Time duration, enum WifiPhy::State state)
{
  std::cout << " state=";
  switch (state) {
  case WifiPhy::TX:
    std::cout << "tx      ";
    break;
  case WifiPhy::SYNC:
    std::cout << "sync    ";
    break;
  case WifiPhy::CCA_BUSY:
    std::cout << "cca-busy";
    break;
  case WifiPhy::IDLE:
    std::cout << "idle    ";
    break;
  }
  std::cout << " start="<<start<<" duration="<<duration<<std::endl;
}

static void
SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

static Vector
GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

static void 
AdvancePosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
  pos.x += 5.0;
  if (pos.x >= 210.0) 
    {
      return;
    }
  SetPosition (node, pos);
  //std::cout << "x="<<pos.x << std::endl;
  Simulator::Schedule (Seconds (1.0), &AdvancePosition, node);
}




int main (int argc, char *argv[])
{
  Packet::EnablePrinting ();

  // enable rts cts all the time.
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("0"));
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));

  WifiHelper wifi;
  MobilityHelper mobility;
  NodeContainer stas;
  NodeContainer ap;
  NetDeviceContainer staDevs;
  PacketSocketHelper packetSocket;

  stas.Create (2);
  ap.Create (1);

  // give packet socket powers to nodes.
  packetSocket.Install (stas);
  packetSocket.Install (ap);

  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  channel->SetPropagationLossModel (log);

  Ssid ssid = Ssid ("wifi-default");
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");
  // setup stas.
  wifi.SetMac ("ns3::NqstaWifiMac", 
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));
  staDevs = wifi.Install (stas, channel);
  // setup ap.
  wifi.SetMac ("ns3::NqapWifiMac", "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true),
               "BeaconInterval", TimeValue (Seconds (2.5)));
  wifi.Install (ap, channel);

  // mobility.
  mobility.Install (stas);
  mobility.Install (ap);

  Simulator::Schedule (Seconds (1.0), &AdvancePosition, ap.Get (0));

  PacketSocketAddress socket;
  socket.SetSingleDevice(staDevs.Get (0)->GetIfIndex ());
  socket.SetPhysicalAddress (staDevs.Get (1)->GetAddress ());
  socket.SetProtocol (1);

  OnOffHelper onoff ("ns3::PacketSocketFactory", Address (socket));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (42)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));

  ApplicationContainer apps = onoff.Install (stas.Get (0));
  apps.Start (Seconds (0.5));
  apps.Stop (Seconds (43.0));

  Simulator::Stop (Seconds (44.0));

  Config::Connect ("/NodeList/*/DeviceList/*/Tx", MakeCallback (&DevTxTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Rx", MakeCallback (&DevRxTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/State/RxOk", MakeCallback (&PhyRxOkTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/State/RxError", MakeCallback (&PhyRxErrorTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/State/Tx", MakeCallback (&PhyTxTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/State/State", MakeCallback (&PhyStateTrace));

  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
