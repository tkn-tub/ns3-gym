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

#include "ns3/wifi-net-device.h"
#include "ns3/wifi-channel.h"
#include "ns3/wifi-phy.h"

#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/onoff-application.h"
#include "ns3/static-mobility-model.h"
#include "ns3/random-variable.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet.h"
#include "ns3/node-list.h"


#include <iostream>

using namespace ns3;

void
WifiNetDeviceTrace (const TraceContext &context, Ptr<const Packet> p, Mac48Address address)
{
  std::cout << context << " ad=" << address << " p: " << p << std::endl;
}
void
WifiPhyStateTrace (const TraceContext &context, Time start, Time duration, enum WifiPhy::State state)
{
  std::cout << context << " state=";
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

static Ptr<Node>
CreateApNode (Ptr<WifiChannel> channel,
              Vector position, 
              const char *macAddress,
              Ssid ssid, 
              Time at)
{
  Ptr<Node> node = CreateObject<Node> ();  
  Ptr<NqapWifiNetDevice> device = CreateObject<NqapWifiNetDevice> (node, Mac48Address (macAddress));
  device->SetSsid (ssid);
  Simulator::Schedule (at, &NqapWifiNetDevice::StartBeaconing, device);
  device->Attach (channel);
  Ptr<MobilityModel> mobility = CreateObject<StaticMobilityModel> ();
  mobility->SetPosition (position);
  node->AggregateObject (mobility);
  return node;
}

static Ptr<Node>
CreateStaNode (Ptr<WifiChannel> channel,
               Vector position, 
               const char *macAddress,
               Ssid ssid)
{
  Ptr<Node> node = CreateObject<Node> ();  
  Ptr<NqstaWifiNetDevice> device = CreateObject<NqstaWifiNetDevice> (node, Mac48Address (macAddress));
  Simulator::ScheduleNow (&NqstaWifiNetDevice::StartActiveAssociation, device, 
                          ssid);
  device->Attach (channel);
  Ptr<MobilityModel> mobility = CreateObject<StaticMobilityModel> ();
  mobility->SetPosition (position);
  node->AggregateObject (mobility);
  return node;
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
  Simulator::SetLinkedList ();

  Packet::EnableMetadata ();

  //Simulator::EnableLogTo ("80211.log");


  // enable rts cts all the time.
  DefaultValue::Bind ("WifiRtsCtsThreshold", "0");
  // disable fragmentation
  DefaultValue::Bind ("WifiFragmentationThreshold", "2200");
  DefaultValue::Bind ("WifiRateControlAlgorithm", "Aarf");
  //DefaultValue::Bind ("WifiRateControlAlgorithm", "Arf");

  Ptr<WifiChannel> channel = CreateObject<WifiChannel> ();
  Ssid ssid = Ssid ("mathieu");

  Ptr<Node> a = CreateApNode (channel, 
                              Vector (5.0,0.0,0.0),
                              "00:00:00:00:00:01",
                              ssid, 
                              Seconds (0.1));
  Simulator::Schedule (Seconds (1.0), &AdvancePosition, a);

  Ptr<Node> b = CreateStaNode (channel,
                               Vector (0.0, 0.0, 0.0),
                               "00:00:00:00:00:02",
                               ssid);

  Ptr<Node> c = CreateStaNode (channel,
                               Vector (0.0, 0.0, 0.0),
                               "00:00:00:00:00:03",
                               ssid);

  PacketSocketAddress destination = PacketSocketAddress ();
  destination.SetProtocol (1);
  destination.SetSingleDevice (0);
  destination.SetPhysicalAddress (Mac48Address ("00:00:00:00:00:03"));
  Ptr<Application> app = CreateObject<OnOffApplication> (b, destination, 
                                                   "Packet", 
                                                   ConstantVariable (42),
                                                   ConstantVariable (0));
  app->Start (Seconds (0.5));
  app->Stop (Seconds (43.0));

  Simulator::StopAt (Seconds (44.0));

  //NodeList::Connect ("/nodes/*/devices/*/*", MakeCallback (&WifiNetDeviceTrace));
  //NodeList::Connect ("/nodes/*/devices/*/phy/state", MakeCallback (&WifiPhyStateTrace));

  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
