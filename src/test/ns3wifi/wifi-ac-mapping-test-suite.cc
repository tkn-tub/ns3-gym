/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
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
 * Author: Stefano Avallone <stavallo@unina.it>
 */

#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/pointer.h"
#include "ns3/ssid.h"
#include "ns3/data-rate.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-sink.h"
#include "ns3/wifi-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-mac-queue.h"
#include "ns3/edca-txop-n.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiAcMappingTest");

class WifiAcMappingTest : public TestCase
{
public:
  WifiAcMappingTest (uint8_t tos, uint8_t expectedQueue);
  virtual void DoRun (void);

private:
  uint8_t m_tos;
  uint8_t m_expectedQueue;
};

WifiAcMappingTest::WifiAcMappingTest (uint8_t tos, uint8_t expectedQueue)
  : TestCase ("User priority to Access Category mapping test"),
    m_tos (tos),
    m_expectedQueue (expectedQueue)
{
}

void
WifiAcMappingTest::DoRun (void)
{
  WifiHelper wifi;
  WifiMacHelper wifiMac;
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  Ssid ssid = Ssid ("wifi-ac-mapping");
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");

  // Setup the AP, which will be the source of traffic for this test
  NodeContainer ap;
  ap.Create (1);
  wifiMac.SetType ("ns3::ApWifiMac",
                   "QosSupported", BooleanValue (true),
                   "Ssid", SsidValue (ssid));

  NetDeviceContainer apDev = wifi.Install (wifiPhy, wifiMac, ap);

  // Setup one STA, which will be the sink for traffic in this test.
  NodeContainer sta;
  sta.Create (1);
  wifiMac.SetType ("ns3::StaWifiMac",
                   "QosSupported", BooleanValue (true),
                   "Ssid", SsidValue (ssid));
  NetDeviceContainer staDev = wifi.Install (wifiPhy, wifiMac, sta);

  // Our devices will have fixed positions
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (2),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.Install (sta);
  mobility.Install (ap);

  // Now we install internet stacks on our devices
  InternetStackHelper stack;
  stack.Install (ap);
  stack.Install (sta);

  Ipv4AddressHelper address;
  address.SetBase ("192.168.0.0", "255.255.255.0");
  Ipv4InterfaceContainer staNodeInterface, apNodeInterface;
  staNodeInterface = address.Assign (staDev);
  apNodeInterface = address.Assign (apDev);

  uint16_t udpPort = 50000;

  PacketSinkHelper packetSink ("ns3::UdpSocketFactory",
                               InetSocketAddress (Ipv4Address::GetAny (), udpPort));
  ApplicationContainer sinkApp = packetSink.Install (sta.Get (0));
  sinkApp.Start (Seconds (0));
  sinkApp.Stop (Seconds (3.0));

  // The packet source is an on-off application on the AP device
  InetSocketAddress dest (staNodeInterface.GetAddress (0), udpPort);
  dest.SetTos (m_tos);
  OnOffHelper onoff ("ns3::UdpSocketFactory", dest);
  onoff.SetConstantRate (DataRate ("5kbps"), 500);
  ApplicationContainer sourceApp = onoff.Install (ap.Get (0));
  sourceApp.Start (Seconds (1.0));
  sourceApp.Stop (Seconds (3.0));

  // The first packet will be transmitted at time 1+(500*8)/5000 = 1.8s.
  // Let this packet be transmitted successfully, so that the AP can resolve
  // the IP address of the station and get its MAC address.
  // The second packet will be transmitted at time 1.8+(500*8)/5000 = 2.6s.
  // Put the AP in sleep mode at time 2.0s, so that the second packet remains
  // in a wifi mac queue and we can discover the queue it has been sent to.
  Ptr<WifiPhy> apPhy = DynamicCast<WifiNetDevice> (apDev.Get (0))->GetPhy ();
  Simulator::ScheduleWithContext (ap.Get (0)->GetId (), Seconds (2.0),
                                  &WifiPhy::SetSleepMode, apPhy);

  Simulator::Stop (Seconds (4.0));

  Ptr<WifiMac> apMac = DynamicCast<WifiNetDevice> (apDev.Get (0))->GetMac ();
  PointerValue ptr;
  Ptr<WifiMacQueue> queues[4];
  // Get the four wifi mac queues and set their MaxDelay attribute to a large
  // value, so that packets are not removed by the Cleanup method
  apMac->GetAttribute ("BE_EdcaTxopN", ptr);
  queues[0] = ptr.Get<EdcaTxopN> ()->GetEdcaQueue ();
  queues[0]->SetAttribute ("MaxDelay", TimeValue (Seconds (10.0)));

  apMac->GetAttribute ("BK_EdcaTxopN", ptr);
  queues[1] = ptr.Get<EdcaTxopN> ()->GetEdcaQueue ();
  queues[1]->SetAttribute ("MaxDelay", TimeValue (Seconds (10.0)));

  apMac->GetAttribute ("VI_EdcaTxopN", ptr);
  queues[2] = ptr.Get<EdcaTxopN> ()->GetEdcaQueue ();
  queues[2]->SetAttribute ("MaxDelay", TimeValue (Seconds (10.0)));

  apMac->GetAttribute ("VO_EdcaTxopN", ptr);
  queues[3] = ptr.Get<EdcaTxopN> ()->GetEdcaQueue ();
  queues[3]->SetAttribute ("MaxDelay", TimeValue (Seconds (10.0)));


  Simulator::Run ();

  for (uint32_t i = 0; i < 4; i++)
    {
      if (i == m_expectedQueue)
        {
          NS_TEST_ASSERT_MSG_EQ (queues[i]->GetSize (), 1, "There is no packet in the expected queue " << i);
        }
      else
        {
          NS_TEST_ASSERT_MSG_EQ (queues[i]->GetSize (), 0, "Unexpectedly, there is a packet in queue " << i);
        }
    }

  uint32_t totalOctetsThrough =
    DynamicCast<PacketSink>(sinkApp.Get (0))->GetTotalRx ();

  // Check that the first packet has been received
  NS_TEST_ASSERT_MSG_EQ (totalOctetsThrough, 500, "A single packet should have been received");

  Simulator::Destroy ();
}


class WifiAcMappingTestSuite : public TestSuite
{
public:
  WifiAcMappingTestSuite ();
};

WifiAcMappingTestSuite::WifiAcMappingTestSuite ()
  : TestSuite ("ns3-wifi-ac-mapping", SYSTEM)
{
  AddTestCase (new WifiAcMappingTest (0xb8, 2), TestCase::QUICK); // EF in AC_VI
  AddTestCase (new WifiAcMappingTest (0x28, 1), TestCase::QUICK); // AF11 in AC_BK
  AddTestCase (new WifiAcMappingTest (0x70, 0), TestCase::QUICK); // AF32 in AC_BE
  AddTestCase (new WifiAcMappingTest (0xc0, 3), TestCase::QUICK); // CS7 in AC_VO
}

static WifiAcMappingTestSuite wifiAcMappingTestSuite;
