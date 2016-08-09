/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 NITK Surathkal
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
 * Authors: Shravya Ks <shravya.ks0@gmail.com>
 *          Smriti Murali <m.smriti.95@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

class PieQueueDiscTestItem : public QueueDiscItem
{
public:
  PieQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol);
  virtual ~PieQueueDiscTestItem ();
  virtual void AddHeader (void);

private:
  PieQueueDiscTestItem ();
  PieQueueDiscTestItem (const PieQueueDiscTestItem &);
  PieQueueDiscTestItem &operator = (const PieQueueDiscTestItem &);
};

PieQueueDiscTestItem::PieQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol)
  : QueueDiscItem (p, addr, protocol)
{
}

PieQueueDiscTestItem::~PieQueueDiscTestItem ()
{
}

void
PieQueueDiscTestItem::AddHeader (void)
{
}

class PieQueueDiscTestCase : public TestCase
{
public:
  PieQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  void Enqueue (Ptr<PieQueueDisc> queue, uint32_t size, uint32_t nPkt);
  void RunPieTest (StringValue mode);
};

PieQueueDiscTestCase::PieQueueDiscTestCase ()
  : TestCase ("Sanity check on the pie queue implementation")
{
}

void
PieQueueDiscTestCase::RunPieTest (StringValue mode)
{
  uint32_t pktSize = 0;
  // 1 for packets; pktSize for bytes
  uint32_t modeSize = 1;
  uint32_t qSize = 8;
  Ptr<PieQueueDisc> queue = CreateObject<PieQueueDisc> ();

  // test 1: simple enqueue/dequeue with no drops
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Tupdate", TimeValue (Seconds (0.03))), true,
                         "Verify that we can actually set the attribute Tupdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Supdate", TimeValue (Seconds (0.0))), true,
                         "Verify that we can actually set the attribute Supdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("DequeueThreshold", UintegerValue (10000)), true,
                         "Verify that we can actually set the attribute DequeueThreshold");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueDelayReference", TimeValue (Seconds (0.02))), true,
                         "Verify that we can actually set the attribute QueueDelayReference");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBurstAllowance", TimeValue (Seconds (0.1))), true,
                         "Verify that we can actually set the attribute MaxBurstAllowance");

  Address dest;

  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      pktSize = 1000;
      modeSize = pktSize;
      queue->SetQueueLimit (qSize * modeSize);
    }

  Ptr<Packet> p1, p2, p3, p4, p5, p6, p7, p8;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);
  p4 = Create<Packet> (pktSize);
  p5 = Create<Packet> (pktSize);
  p6 = Create<Packet> (pktSize);
  p7 = Create<Packet> (pktSize);
  p8 = Create<Packet> (pktSize);

  queue->Initialize ();
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 0 * modeSize, "There should be no packets in there");
  queue->Enqueue (Create<PieQueueDiscTestItem> (p1, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 1 * modeSize, "There should be one packet in there");
  queue->Enqueue (Create<PieQueueDiscTestItem> (p2, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 2 * modeSize, "There should be two packets in there");
  queue->Enqueue (Create<PieQueueDiscTestItem> (p3, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p4, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p5, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p6, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p7, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p8, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 8 * modeSize, "There should be eight packets in there");

  Ptr<QueueDiscItem> item;

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the first packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 7 * modeSize, "There should be seven packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p1->GetUid (), "was this the first packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the second packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 6 * modeSize, "There should be six packet in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p2->GetUid (), "Was this the second packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the third packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 5 * modeSize, "There should be five packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p3->GetUid (), "Was this the third packet ?");

  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item == 0), true, "There are really no packets in there");

  // test 2: test in a simple dumbbell topology
  pktSize = 1000;
  std::string pieLinkDataRate = "1.5Mbps";
  std::string pieLinkDelay = "20ms";

  double global_start_time = 0.0;
  double global_stop_time = 7.0;
  double sink_start_time = global_start_time;
  double sink_stop_time = global_stop_time + 3.0;
  double client_start_time = global_start_time + 1.5;
  double client_stop_time = global_stop_time - 2.0;

  NodeContainer n0n2;
  NodeContainer n1n2;
  NodeContainer n2n3;
  NodeContainer n3n4;
  NodeContainer n3n5;

  Ipv4InterfaceContainer i0i2;
  Ipv4InterfaceContainer i1i2;
  Ipv4InterfaceContainer i2i3;
  Ipv4InterfaceContainer i3i4;
  Ipv4InterfaceContainer i3i5;

  NodeContainer c;
  c.Create (6);
  n0n2 = NodeContainer (c.Get (0), c.Get (2));
  n1n2 = NodeContainer (c.Get (1), c.Get (2));
  n2n3 = NodeContainer (c.Get (2), c.Get (3));
  n3n4 = NodeContainer (c.Get (3), c.Get (4));
  n3n5 = NodeContainer (c.Get (3), c.Get (5));

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
  // 42 = headers size
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000 - 42));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (false));

  uint32_t meanPktSize = 1000;

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");

  Ipv4Header ipHeader;

  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      modeSize = pktSize + ipHeader.GetSerializedSize ();
    }

  qSize = 100 * modeSize;

  // PIE params
  Config::SetDefault ("ns3::PieQueueDisc::Mode", StringValue (mode));
  Config::SetDefault ("ns3::PieQueueDisc::MeanPktSize", UintegerValue (meanPktSize + ipHeader.GetSerializedSize ()));
  Config::SetDefault ("ns3::PieQueueDisc::DequeueThreshold", UintegerValue (10000));
  Config::SetDefault ("ns3::PieQueueDisc::QueueDelayReference", TimeValue (Seconds (0.02)));
  Config::SetDefault ("ns3::PieQueueDisc::MaxBurstAllowance", TimeValue (Seconds (0.1)));
  Config::SetDefault ("ns3::PieQueueDisc::QueueLimit", UintegerValue (qSize));

  InternetStackHelper internet;
  internet.Install (c);

  TrafficControlHelper tchPfifo;
  uint16_t handle = tchPfifo.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  tchPfifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", UintegerValue (1000));

  TrafficControlHelper tchPie;
  tchPie.SetRootQueueDisc ("ns3::PieQueueDisc");

  PointToPointHelper p2p;

  NetDeviceContainer devn0n2;
  NetDeviceContainer devn1n2;
  NetDeviceContainer devn2n3;
  NetDeviceContainer devn3n4;
  NetDeviceContainer devn3n5;

  QueueDiscContainer queueDiscs;

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  devn0n2 = p2p.Install (n0n2);
  tchPfifo.Install (devn0n2);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("3ms"));
  devn1n2 = p2p.Install (n1n2);
  tchPfifo.Install (devn1n2);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue (pieLinkDataRate));
  p2p.SetChannelAttribute ("Delay", StringValue (pieLinkDelay));
  devn2n3 = p2p.Install (n2n3);
  // only backbone link has PIE queue disc
  queueDiscs = tchPie.Install (devn2n3);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("4ms"));
  devn3n4 = p2p.Install (n3n4);
  tchPfifo.Install (devn3n4);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("5ms"));
  devn3n5 = p2p.Install (n3n5);
  tchPfifo.Install (devn3n5);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  i0i2 = ipv4.Assign (devn0n2);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  i1i2 = ipv4.Assign (devn1n2);
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  i2i3 = ipv4.Assign (devn2n3);
  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  i3i4 = ipv4.Assign (devn3n4);
  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  i3i5 = ipv4.Assign (devn3n5);

  // Set up the routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // SINK is in the right side
  uint16_t port = 50000;
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApp = sinkHelper.Install (n3n4.Get (1));
  sinkApp.Start (Seconds (sink_start_time));
  sinkApp.Stop (Seconds (sink_stop_time));

  // Connection one
  // Clients are in left side
  /*
   * Create the OnOff applications to send TCP to the server
   * onoffhelper is a client that send data to TCP destination
  */
  OnOffHelper clientHelper1 ("ns3::TcpSocketFactory", Address ());
  clientHelper1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  clientHelper1.SetAttribute ("PacketSize", UintegerValue (pktSize));
  clientHelper1.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));

  // Connection two
  OnOffHelper clientHelper2 ("ns3::TcpSocketFactory", Address ());
  clientHelper2.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper2.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  clientHelper2.SetAttribute ("PacketSize", UintegerValue (pktSize));
  clientHelper2.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));

  ApplicationContainer clientApps1;
  AddressValue remoteAddress (InetSocketAddress (i3i4.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n0n2.Get (0)));
  clientApps1.Start (Seconds (client_start_time));
  clientApps1.Stop (Seconds (client_stop_time));

  ApplicationContainer clientApps2;
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (n1n2.Get (0)));
  clientApps2.Start (Seconds (client_start_time));
  clientApps2.Stop (Seconds (client_stop_time));

  Simulator::Stop (Seconds (sink_stop_time));
  Simulator::Run ();

  PieQueueDisc::Stats st = StaticCast<PieQueueDisc> (queueDiscs.Get (0))->GetStats ();

  NS_TEST_EXPECT_MSG_NE (st.unforcedDrop, 0, "There should be some packets dropped due to prob mark");
  NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should be zero packets dropped due to queue limit");

  Simulator::Destroy ();
}

void
PieQueueDiscTestCase::DoRun (void)
{
  RunPieTest (StringValue ("QUEUE_MODE_PACKETS"));
  RunPieTest (StringValue ("QUEUE_MODE_BYTES"));
  Simulator::Destroy ();
}

static class PieQueueDiscTestSuite : public TestSuite
{
public:
  PieQueueDiscTestSuite ()
    : TestSuite ("pie-queue-disc", UNIT)
  {
    AddTestCase (new PieQueueDiscTestCase (), TestCase::QUICK);
  }
} g_pieQueueTestSuite;
