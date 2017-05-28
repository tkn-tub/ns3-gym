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
#include "ns3/traffic-control-helper.h"
#include "ns3/traffic-control-layer.h"
#include "ns3/llc-snap-header.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiAcMappingTest");

class WifiAcMappingTest : public TestCase
{
public:
  WifiAcMappingTest (uint8_t tos, uint8_t expectedQueue);
  virtual void DoRun (void);

private:
  static void PacketEnqueuedInQueueDisc (uint8_t tos, uint8_t* count, Ptr<const QueueDiscItem> item);
  static void PacketEnqueuedInWifiMacQueue (uint8_t tos, uint8_t* count, Ptr<const WifiMacQueueItem> item);
  uint8_t m_tos;
  uint8_t m_expectedQueue;
  uint8_t m_QueueDiscCount[4];
  uint8_t m_WifiMacQueueCount[4];
};

WifiAcMappingTest::WifiAcMappingTest (uint8_t tos, uint8_t expectedQueue)
  : TestCase ("User priority to Access Category mapping test. Checks that packets are"
              "enqueued in the correct child queue disc of the mq root queue disc and"
              "in the correct wifi MAC queue"),
    m_tos (tos),
    m_expectedQueue (expectedQueue)
{
  for (uint8_t i = 0; i < 4; i++)
    {
      m_QueueDiscCount[i] = 0;
      m_WifiMacQueueCount[i] = 0;
    }
}

void
WifiAcMappingTest::PacketEnqueuedInQueueDisc (uint8_t tos, uint8_t* count, Ptr<const QueueDiscItem> item)
{
  uint8_t val;
  if (item->GetUint8Value (QueueItem::IP_DSFIELD, val) && val == tos)
    {
      (*count)++;
    }
}

void
WifiAcMappingTest::PacketEnqueuedInWifiMacQueue (uint8_t tos, uint8_t* count, Ptr<const WifiMacQueueItem> item)
{
  LlcSnapHeader llc;
  Ptr<Packet> packet = item->GetPacket ()->Copy ();
  packet->RemoveHeader (llc);

  if (llc.GetType () == Ipv4L3Protocol::PROT_NUMBER)
    {
      Ipv4Header iph;
      packet->PeekHeader (iph);
      if (iph.GetTos () == tos)
        {
          (*count)++;
        }
    }
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

  TrafficControlHelper tch;
  uint16_t handle = tch.SetRootQueueDisc ("ns3::MqQueueDisc");
  TrafficControlHelper::ClassIdList cls = tch.AddQueueDiscClasses (handle, 4, "ns3::QueueDiscClass");
  TrafficControlHelper::HandleList hdl = tch.AddChildQueueDiscs (handle, cls, "ns3::FqCoDelQueueDisc");
  for (auto h : hdl)
    {
      tch.AddPacketFilter (h, "ns3::FqCoDelIpv4PacketFilter");
    }
  tch.Install (apDev);
  tch.Install (staDev);

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
  sinkApp.Stop (Seconds (4.0));

  // The packet source is an on-off application on the AP device
  InetSocketAddress dest (staNodeInterface.GetAddress (0), udpPort);
  dest.SetTos (m_tos);
  OnOffHelper onoff ("ns3::UdpSocketFactory", dest);
  onoff.SetConstantRate (DataRate ("5kbps"), 500);
  ApplicationContainer sourceApp = onoff.Install (ap.Get (0));
  sourceApp.Start (Seconds (1.0));
  sourceApp.Stop (Seconds (4.0));

  // The first packet will be transmitted at time 1+(500*8)/5000 = 1.8s.
  // The second packet will be transmitted at time 1.8+(500*8)/5000 = 2.6s.
  // The third packet will be transmitted at time 2.6+(500*8)/5000 = 3.4s.

  Simulator::Stop (Seconds (5.0));

  Ptr<QueueDisc> root = ap.Get (0)->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (apDev.Get (0));
  NS_TEST_ASSERT_MSG_EQ (root->GetNQueueDiscClasses (), 4, "The root queue disc should have 4 classes");
  // Get the four child queue discs and connect their Enqueue trace to the PacketEnqueuedInQueueDisc
  // method, which counts how many packets with the given ToS value have been enqueued
  root->GetQueueDiscClass (0)->GetQueueDisc ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInQueueDisc, m_tos, m_QueueDiscCount));

  root->GetQueueDiscClass (1)->GetQueueDisc ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInQueueDisc, m_tos, m_QueueDiscCount+1));

  root->GetQueueDiscClass (2)->GetQueueDisc ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInQueueDisc, m_tos, m_QueueDiscCount+2));

  root->GetQueueDiscClass (3)->GetQueueDisc ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInQueueDisc, m_tos, m_QueueDiscCount+3));

  Ptr<WifiMac> apMac = DynamicCast<WifiNetDevice> (apDev.Get (0))->GetMac ();
  PointerValue ptr;
  // Get the four wifi mac queues and connect their Enqueue trace to the PacketEnqueuedInWifiMacQueue
  // method, which counts how many packets with the given ToS value have been enqueued
  apMac->GetAttribute ("BE_EdcaTxopN", ptr);
  ptr.Get<EdcaTxopN> ()->GetQueue ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInWifiMacQueue, m_tos, m_WifiMacQueueCount));

  apMac->GetAttribute ("BK_EdcaTxopN", ptr);
  ptr.Get<EdcaTxopN> ()->GetQueue ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInWifiMacQueue, m_tos, m_WifiMacQueueCount+1));

  apMac->GetAttribute ("VI_EdcaTxopN", ptr);
  ptr.Get<EdcaTxopN> ()->GetQueue ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInWifiMacQueue, m_tos, m_WifiMacQueueCount+2));

  apMac->GetAttribute ("VO_EdcaTxopN", ptr);
  ptr.Get<EdcaTxopN> ()->GetQueue ()->TraceConnectWithoutContext ("Enqueue",
                        MakeBoundCallback (&WifiAcMappingTest::PacketEnqueuedInWifiMacQueue, m_tos, m_WifiMacQueueCount+3));

  Simulator::Run ();

  for (uint32_t i = 0; i < 4; i++)
    {
      if (i == m_expectedQueue)
        {
          NS_TEST_ASSERT_MSG_GT_OR_EQ (m_QueueDiscCount[i], 1, "There is no packet in the expected queue disc " << i);
          NS_TEST_ASSERT_MSG_GT_OR_EQ (m_WifiMacQueueCount[i], 1, "There is no packet in the expected Wifi MAC queue " << i);
        }
      else
        {
          NS_TEST_ASSERT_MSG_EQ (m_QueueDiscCount[i], 0, "Unexpectedly, there is a packet in queue disc " << i);
          NS_TEST_ASSERT_MSG_EQ (m_WifiMacQueueCount[i], 0, "Unexpectedly, there is a packet in Wifi MAC queue " << i);
        }
    }

  uint32_t totalOctetsThrough =
    DynamicCast<PacketSink> (sinkApp.Get (0))->GetTotalRx ();

  // Check that the three packets have been received
  NS_TEST_ASSERT_MSG_EQ (totalOctetsThrough, 1500, "Three packets should have been received");

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
