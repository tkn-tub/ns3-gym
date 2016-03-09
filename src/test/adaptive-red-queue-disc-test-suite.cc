/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 NITK Surathkal
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
 * Author: Mohit P. Tahiliani (tahiliani@nitk.edu.in)
 *
 */

#include "ns3/test.h"
#include "ns3/red-queue-disc.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-queue-disc-item.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

// Tests to verify the working of *automatically set* parameters in ARED
class AutoRedQueueDiscTestCase : public TestCase
{
public:
  AutoRedQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  void Enqueue (Ptr<RedQueueDisc> queue, uint32_t size, uint32_t nPkt);
  void RunAutoRedDiscTest (StringValue mode);
};

AutoRedQueueDiscTestCase::AutoRedQueueDiscTestCase ()
  : TestCase ("Sanity check on automatically set parameters of ARED")
{
}

void
AutoRedQueueDiscTestCase::RunAutoRedDiscTest (StringValue mode)
{
  uint32_t pktSize = 0;
  uint32_t modeSize = 1;                // 1 for packets; pktSize for bytes
  Ptr<RedQueueDisc> queue = CreateObject<RedQueueDisc> ();

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");

  Ipv4Header ipHeader;
  Address dest;

  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      pktSize = 500;
      modeSize = pktSize + ipHeader.GetSerializedSize ();
    }

  double minTh = 70 * modeSize;
  double maxTh = 150 * modeSize;
  uint32_t qSize = 300 * modeSize;

  // test 1: Verify automatic setting of QW. [QW = 0.0 with default LinkBandwidth]
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.0)), true,
                         "Verify that we can actually set the attribute QW");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  RedQueueDisc::Stats st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should be zero dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should be zero dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 2: Verify automatic setting of QW. [QW = 0.0 with lesser LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.0)), true,
                         "Verify that we can actually set the attribute QW");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("LinkBandwidth", DataRateValue (DataRate ("0.015Mbps"))), true,
                         "Verify that we can actually set the attribute LinkBandwidth");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 44, "There should be 44 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 45, "There should be 45 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 3: Verify automatic setting of QW. [QW = -1.0 with default LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (-1.0)), true,
                         "Verify that we can actually set the attribute QW");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should be zero dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should be zero dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 4: Verify automatic setting of QW. [QW = -1.0 with lesser LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (-1.0)), true,
                         "Verify that we can actually set the attribute QW");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("LinkBandwidth", DataRateValue (DataRate ("0.015Mbps"))), true,
                         "Verify that we can actually set the attribute LinkBandwidth");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 32, "There should be 32 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 33, "There should be 33 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 5: Verify automatic setting of QW. [QW = -2.0 with default LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (-2.0)), true,
                         "Verify that we can actually set the attribute QW");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 29, "There should be 29 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 30, "There should be 30 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 6: Verify automatic setting of QW. [QW = -2.0 with lesser LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (-2.0)), true,
                         "Verify that we can actually set the attribute QW");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("LinkBandwidth", DataRateValue (DataRate ("0.015Mbps"))), true,
                         "Verify that we can actually set the attribute LinkBandwidth");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 44, "There should be 44 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 46, "There should be 46 dropped packets  due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 7: Verify automatic setting of minTh and maxTh. [minTh = maxTh = 0.0, with default LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (0.0)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (0.0)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (modeSize)), true,
                             "Verify that we can actually set the attribute MeanPktSize");
    }
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 20, "There should be 20 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 113, "There should be 113 dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 21, "There should be 21 dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 113, "There should be 113 dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }


  // test 8: Verify automatic setting of minTh and maxTh. [minTh = maxTh = 0.0, with higher LinkBandwidth]
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (0.0)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (0.0)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("LinkBandwidth", DataRateValue (DataRate ("150Mbps"))), true,
                         "Verify that we can actually set the attribute LinkBandwidth");
  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (modeSize)), true,
                             "Verify that we can actually set the attribute MeanPktSize");
    }
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should be zero dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should be zero dropped packets due to probability mark");
      NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero dropped packets due to hard mark");
      NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should be zero dropped packets due to queue full");
    }
}

void
AutoRedQueueDiscTestCase::Enqueue (Ptr<RedQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Ipv4Header ipHeader;
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<Ipv4QueueDiscItem> (Create<Packet> (size), dest, 0, ipHeader));
    }
}

void
AutoRedQueueDiscTestCase::DoRun (void)
{
  RunAutoRedDiscTest (StringValue ("QUEUE_MODE_PACKETS"));
  RunAutoRedDiscTest (StringValue ("QUEUE_MODE_BYTES"));
  Simulator::Destroy ();
}


// Tests to verify the working of *adaptive* parameter in ARED
class AdaptiveRedQueueDiscTestCase : public TestCase
{
public:
  AdaptiveRedQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  void RunAdaptiveRedDiscTest (StringValue mode);
};

AdaptiveRedQueueDiscTestCase::AdaptiveRedQueueDiscTestCase ()
  : TestCase ("Sanity check on adaptive parameter of ARED")
{
}

void
AdaptiveRedQueueDiscTestCase::RunAdaptiveRedDiscTest (StringValue mode)
{
  uint32_t    pktSize = 500;
  uint32_t    modeSize = 1;             // 1 for packets; pktSize for bytes
  uint32_t    nLeaf = 3;

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (pktSize));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("10Mbps"));

  Ptr<RedQueueDisc> queue = CreateObject<RedQueueDisc> ();

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");

  Ipv4Header ipHeader;

  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      modeSize = pktSize + ipHeader.GetSerializedSize ();
    }

  uint32_t    qSize = 100 * modeSize;

  Config::SetDefault ("ns3::RedQueueDisc::ARED", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::LInterm", DoubleValue (10.0));
  Config::SetDefault ("ns3::RedQueueDisc::QueueLimit", UintegerValue (qSize));
  Config::SetDefault ("ns3::RedQueueDisc::MeanPktSize", UintegerValue (pktSize + ipHeader.GetSerializedSize ()));

  // Create the point-to-point link helpers
  PointToPointHelper bottleNeckLink;
  bottleNeckLink.SetDeviceAttribute  ("DataRate", StringValue ("1.5Mbps"));
  bottleNeckLink.SetChannelAttribute ("Delay", StringValue ("20ms"));

  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("10Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("1ms"));

  PointToPointDumbbellHelper d (nLeaf, pointToPointLeaf,
                                nLeaf, pointToPointLeaf,
                                bottleNeckLink);

  // Install Stack
  InternetStackHelper stack;
  for (uint32_t i = 0; i < d.LeftCount (); ++i)
    {
      stack.Install (d.GetLeft (i));
    }
  for (uint32_t i = 0; i < d.RightCount (); ++i)
    {
      stack.Install (d.GetRight (i));
    }

  stack.Install (d.GetLeft ());
  stack.Install (d.GetRight ());
  TrafficControlHelper tchBottleneck;
  tchBottleneck.SetRootQueueDisc ("ns3::RedQueueDisc");
  tchBottleneck.Install (d.GetLeft ()->GetDevice (0));
  tchBottleneck.Install (d.GetRight ()->GetDevice (0));

  // Assign IP Addresses
  d.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.3.1.0", "255.255.255.0"));

  // Install on/off app on all right side nodes
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
  clientHelper.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.,Max=1.]"));
  clientHelper.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.,Max=1.]"));
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), 5001));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApps;
  for (uint32_t i = 0; i < d.LeftCount (); ++i)
    {
      sinkApps.Add (packetSinkHelper.Install (d.GetLeft (i)));
    }
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (30.0));

  ApplicationContainer clientApps;
  for (uint32_t i = 0; i < d.RightCount (); ++i)
    {
      // Create an on/off app sending packets to the left side
      AddressValue remoteAddress (InetSocketAddress (d.GetLeftIpv4Address (i), 5001));
      clientHelper.SetAttribute ("Remote", remoteAddress);
      clientApps.Add (clientHelper.Install (d.GetRight (i)));
    }
  clientApps.Start (Seconds (1.0)); // Start 1 second after sink
  clientApps.Stop (Seconds (15.0)); // Stop before the sink

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Run ();

  uint32_t totalRxBytesCounter = 0;
  for (uint32_t i = 0; i < sinkApps.GetN (); i++)
    {
      Ptr <Application> app = sinkApps.Get (i);
      Ptr <PacketSink> pktSink = DynamicCast <PacketSink> (app);
      totalRxBytesCounter += pktSink->GetTotalRx ();
    }

  if (queue->GetMode () == Queue::QUEUE_MODE_PACKETS)
    {
      NS_TEST_EXPECT_MSG_EQ (totalRxBytesCounter, 2605000, "Total received bytes should be 2605000");
    }
  else if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      NS_TEST_EXPECT_MSG_EQ (totalRxBytesCounter, 2557000, "Total received bytes should be 2557000");
    }

  Simulator::Destroy ();
}

void
AdaptiveRedQueueDiscTestCase::DoRun (void)
{
  RunAdaptiveRedDiscTest (StringValue ("QUEUE_MODE_PACKETS"));
  RunAdaptiveRedDiscTest (StringValue ("QUEUE_MODE_BYTES"));
  Simulator::Destroy ();
}

static class AredQueueDiscTestSuite : public TestSuite
{
public:
  AredQueueDiscTestSuite ()
    : TestSuite ("adaptive-red-queue-disc", UNIT)
  {
    AddTestCase (new AutoRedQueueDiscTestCase (), TestCase::QUICK);         // Tests for automatically set parameters of ARED
    AddTestCase (new AdaptiveRedQueueDiscTestCase (), TestCase::QUICK);     // Tests for adaptive parameter of ARED
  }
} g_aredQueueDiscTestSuite;
