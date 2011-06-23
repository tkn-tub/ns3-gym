/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Dean Armstrong
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
 * Author: Dean Armstrong <deanarm@gmail.com>
 */
#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/double.h"

#include "ns3/ssid.h"
#include "ns3/data-rate.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-sink.h"

#include "ns3/wifi-helper.h"
#include "ns3/qos-wifi-mac-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"

NS_LOG_COMPONENT_DEFINE ("WifiMsduAggregatorThroughputTest");

using namespace ns3;

class WifiMsduAggregatorThroughputTest : public TestCase
{
public:
  WifiMsduAggregatorThroughputTest ();
  virtual void DoRun (void);

private:
  bool m_writeResults;
};

WifiMsduAggregatorThroughputTest::WifiMsduAggregatorThroughputTest ()
  : TestCase ("MsduAggregator throughput test"),
    m_writeResults (false)
{
}

void
WifiMsduAggregatorThroughputTest::DoRun (void)
{
  WifiHelper wifi = WifiHelper::Default ();

  QosWifiMacHelper wifiMac = QosWifiMacHelper::Default ();
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  Ssid ssid = Ssid ("wifi-amsdu-throughput");
  // It may seem a little farcical running an 802.11n aggregation
  // scenario with 802.11b rates (transmit rate fixed to 1 Mbps, no
  // less), but this approach tests the bit we need to without unduly
  // increasing the complexity of the simulation.
  std::string phyMode ("DsssRate1Mbps");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue (phyMode),
                                "ControlMode", StringValue (phyMode));

  // Setup the AP, which will be the source of traffic for this test
  // and thus has an aggregator on AC_BE.
  NodeContainer ap;
  ap.Create (1);
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid),
                   "BeaconGeneration", BooleanValue (true),
                   "BeaconInterval", TimeValue (MicroSeconds (102400)));
  wifiMac.SetMsduAggregatorForAc (AC_BE, "ns3::MsduStandardAggregator",
                                  "MaxAmsduSize", UintegerValue (4000));
  NetDeviceContainer apDev = wifi.Install (wifiPhy, wifiMac, ap);

  // Setup one STA, which will be the sink for traffic in this test.
  NodeContainer sta;
  sta.Create (1);
  wifiMac.SetType ("ns3::StaWifiMac",
                   "Ssid", SsidValue (ssid),
                   "ActiveProbing", BooleanValue (false));
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

  // The applications for this test will see a unidirectional UDP
  // stream from the AP to the STA. The following UDP port will be
  // used (arbitrary choice).
  uint16_t udpPort = 50000;

  // The packet sink application is on the STA device, and is running
  // right from the start. The traffic source will turn on at 1 second
  // and then off at 9 seconds, so we turn the sink off at 9 seconds
  // too in order to measure throughput in a fixed window.
  PacketSinkHelper packetSink ("ns3::UdpSocketFactory",
                               InetSocketAddress (Ipv4Address::GetAny (),
                                                  udpPort));
  ApplicationContainer sinkApp = packetSink.Install (sta.Get (0));
  sinkApp.Start (Seconds (0));
  sinkApp.Stop (Seconds (9.0));

  // The packet source is an on-off application on the AP
  // device. Given that we have fixed the transmit rate at 1 Mbps
  // above, a 1 Mbps stream at the transport layer should be sufficent
  // to determine whether aggregation is working or not.
  //
  // We configure this traffic stream to operate between 1 and 9 seconds.
  OnOffHelper onoff ("ns3::UdpSocketFactory",
                     InetSocketAddress (staNodeInterface.GetAddress (0),
                                        udpPort));
  onoff.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  onoff.SetAttribute ("PacketSize", UintegerValue (100));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  ApplicationContainer sourceApp = onoff.Install (ap.Get (0));
  sourceApp.Start (Seconds (1.0));
  sourceApp.Stop (Seconds (9.0));

  // Enable tracing at the AP
  if (m_writeResults)
    {
      wifiPhy.EnablePcap ("wifi-amsdu-throughput", sta.Get (0)->GetId (), 0);
    }

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  // Now the simulation is complete we note the total number of octets
  // receive at the packet sink so that we can shortly test that this
  // is plausible.
  uint32_t totalOctetsThrough =
    DynamicCast<PacketSink>(sinkApp.Get (0))->GetTotalRx ();

  // Check that throughput was acceptable. This threshold is set based
  // on inspection of a trace where things are working. Basically, we
  // there get 26 UDP packets (of size 100, as specified above)
  // aggregated per A-MSDU, for which the complete frame exchange
  // (including RTS/CTS and plus medium access) takes around 32
  // ms. Over the eight seconds of the test this means we expect about
  // 650 kilobytes, so a pass threshold of 600000 seems to provide a
  // fair amount of margin to account for reduced utilisation around
  // stream startup, and contention around AP beacon transmission.
  //
  // If aggregation is turned off, then we get about 350 kilobytes in
  // the same test, so we'll definitely catch the major failures.
  NS_TEST_ASSERT_MSG_GT (totalOctetsThrough, 600000,
                         "A-MSDU test fails for low throughput of "
                         << totalOctetsThrough << " octets");
}


// For now the MSDU Aggregator Test Suite contains only the one test
// that is defined in this file, so it's class definition and
// instantiation can live here.
class WifiMsduAggregatorTestSuite : public TestSuite
{
public:
  WifiMsduAggregatorTestSuite ();
};

WifiMsduAggregatorTestSuite::WifiMsduAggregatorTestSuite ()
  : TestSuite ("ns3-wifi-msdu-aggregator", SYSTEM)
{
  AddTestCase (new WifiMsduAggregatorThroughputTest);
}

static WifiMsduAggregatorTestSuite wifiMsduAggregatorTestSuite;
