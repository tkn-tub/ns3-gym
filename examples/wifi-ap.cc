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


#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/onoff-application.h"
#include "ns3/mobility-helper.h"
#include "ns3/wifi-helper.h"
#include "ns3/node-container.h"
#include "ns3/random-variable.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet.h"
#include "ns3/node-list.h"
#include "ns3/ssid.h"
#include "ns3/wifi-phy.h"
#include "ns3/mobility-model.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/wifi-channel.h"
#include "ns3/boolean.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"


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
  Packet::EnableMetadata ();

  // enable rts cts all the time.
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", String ("0"));
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", String ("2200"));

  WifiHelper wifi;
  MobilityHelper mobility;
  NodeContainer stas;
  NodeContainer ap;
  NetDeviceContainer staDevs;

  stas.Create (2);
  ap.Create (1);

  Ptr<WifiChannel> channel = CreateObject<WifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  log->SetReferenceModel (CreateObject<FriisPropagationLossModel> ());
  channel->SetPropagationLossModel (log);

  Ssid ssid = Ssid ("wifi-default");
  wifi.SetPhy ("ns3::WifiPhy");
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");
  // setup ap.
  wifi.SetMac ("ns3::NqstaWifiMac", "Ssid", ssid,
               "ActiveProbing", Boolean (false));
  staDevs = wifi.Build (stas, channel);
  // setup stas.
  wifi.SetMac ("ns3::NqapWifiMac", "Ssid", ssid,
               "BeaconGeneration", Boolean (true),
               "BeaconInterval", Seconds (2.5));
  wifi.Build (ap, channel);

  // mobility.
  mobility.Layout (stas);
  mobility.Layout (ap);

  Simulator::Schedule (Seconds (1.0), &AdvancePosition, ap.Get (0));

  PacketSocketAddress destination = PacketSocketAddress ();
  destination.SetProtocol (1);
  destination.SetSingleDevice (0);
  destination.SetPhysicalAddress (staDevs.Get(1)->GetAddress ());
  Ptr<Application> app = 
    CreateObject<OnOffApplication> ("Remote", Address (destination), 
                                    "Protocol", TypeId::LookupByName ("ns3::PacketSocketFactory"),
                                    "OnTime", ConstantVariable (42),
                                    "OffTime", ConstantVariable (0));
  stas.Get (0)->AddApplication (app);
  app->Start (Seconds (0.5));
  app->Stop (Seconds (43.0));

  Simulator::StopAt (Seconds (44.0));

  Config::Connect ("/NodeList/*/DeviceList/*/Tx", MakeCallback (&DevTxTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Rx", MakeCallback (&DevRxTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/RxOk", MakeCallback (&PhyRxOkTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/RxError", MakeCallback (&PhyRxErrorTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/Tx", MakeCallback (&PhyTxTrace));
  Config::Connect ("/NodeList/*/DeviceList/*/Phy/State", MakeCallback (&PhyStateTrace));

  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
