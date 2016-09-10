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
 * Authors: Pasquale Imputato <p.imputato@gmail.com>
 *          Stefano Avallone <stefano.avallone@unina.it>
*/

#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/fq-codel-queue-disc.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-packet-filter.h"
#include "ns3/ipv4-queue-disc-item.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-header.h"
#include "ns3/ipv6-packet-filter.h"
#include "ns3/ipv6-queue-disc-item.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"

using namespace ns3;

/**
 * This class tests packets for which there is no suitable filter
 */
class FqCoDelQueueDiscNoSuitableFilter : public TestCase
{
public:
  FqCoDelQueueDiscNoSuitableFilter ();
  virtual ~FqCoDelQueueDiscNoSuitableFilter ();

private:
  virtual void DoRun (void);
};

FqCoDelQueueDiscNoSuitableFilter::FqCoDelQueueDiscNoSuitableFilter ()
  : TestCase ("Test packets that are not classified by any filter")
{
}

FqCoDelQueueDiscNoSuitableFilter::~FqCoDelQueueDiscNoSuitableFilter ()
{
}

void
FqCoDelQueueDiscNoSuitableFilter::DoRun (void)
{
  // Packets that cannot be classified by the available filters should be dropped
  Ptr<FqCoDelQueueDisc> queueDisc = CreateObjectWithAttributes<FqCoDelQueueDisc> ("PacketLimit", UintegerValue (4));
  Ptr<FqCoDelIpv4PacketFilter> filter = CreateObject<FqCoDelIpv4PacketFilter> ();
  queueDisc->AddPacketFilter (filter);

  queueDisc->SetQuantum (1500);
  queueDisc->Initialize ();

  Ptr<Packet> p;
  p = Create<Packet> ();
  Ptr<Ipv6QueueDiscItem> item;
  Ipv6Header ipv6Header;
  Address dest;
  item = Create<Ipv6QueueDiscItem> (p, dest, 0, ipv6Header);
  queueDisc->Enqueue (item);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetNQueueDiscClasses (), 0, "no flow queue should have been created");

  p = Create<Packet> (reinterpret_cast<const uint8_t*> ("hello, world"), 12);
  item = Create<Ipv6QueueDiscItem> (p, dest, 0, ipv6Header);
  queueDisc->Enqueue (item);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetNQueueDiscClasses (), 0, "no flow queue should have been created");

  Simulator::Destroy ();
}

/**
 * This class tests the IP flows separation and the packet limit
 */
class FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit : public TestCase
{
public:
  FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit ();
  virtual ~FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit ();

private:
  virtual void DoRun (void);
  void AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header hdr);
};

FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit::FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit ()
  : TestCase ("Test IP flows separation and packet limit")
{
}

FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit::~FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit ()
{
}

void
FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit::AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header hdr)
{
  Ptr<Packet> p = Create<Packet> (100);
  Address dest;
  Ptr<Ipv4QueueDiscItem> item = Create<Ipv4QueueDiscItem> (p, dest, 0, hdr);
  queue->Enqueue (item);
}

void
FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit::DoRun (void)
{
  Ptr<FqCoDelQueueDisc> queueDisc = CreateObjectWithAttributes<FqCoDelQueueDisc> ("PacketLimit", UintegerValue (4));
  Ptr<FqCoDelIpv6PacketFilter> ipv6Filter = CreateObject<FqCoDelIpv6PacketFilter> ();
  Ptr<FqCoDelIpv4PacketFilter> ipv4Filter = CreateObject<FqCoDelIpv4PacketFilter> ();
  queueDisc->AddPacketFilter (ipv6Filter);
  queueDisc->AddPacketFilter (ipv4Filter);

  queueDisc->SetQuantum (1500);
  queueDisc->Initialize ();

  Ipv4Header hdr;
  hdr.SetPayloadSize (100);
  hdr.SetSource (Ipv4Address ("10.10.1.1"));
  hdr.SetDestination (Ipv4Address ("10.10.1.2"));
  hdr.SetProtocol (7);

  // Add three packets from the first flow
  AddPacket (queueDisc, hdr);
  AddPacket (queueDisc, hdr);
  AddPacket (queueDisc, hdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 3, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the flow queue");

  // Add two packets from the second flow
  hdr.SetDestination (Ipv4Address ("10.10.1.7"));
  // Add the first packet
  AddPacket (queueDisc, hdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 4, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the flow queue");
  // Add the second packet that causes two packets to be dropped from the fat flow (max backlog = 300, threshold = 150)
  AddPacket (queueDisc, hdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 3, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the flow queue");

  Simulator::Destroy ();
}

/**
 * This class tests the deficit per flow
 */
class FqCoDelQueueDiscDeficit : public TestCase
{
public:
  FqCoDelQueueDiscDeficit ();
  virtual ~FqCoDelQueueDiscDeficit ();

private:
  virtual void DoRun (void);
  void AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header hdr);
};

FqCoDelQueueDiscDeficit::FqCoDelQueueDiscDeficit ()
  : TestCase ("Test credits and flows status")
{
}

FqCoDelQueueDiscDeficit::~FqCoDelQueueDiscDeficit ()
{
}

void
FqCoDelQueueDiscDeficit::AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header hdr)
{
  Ptr<Packet> p = Create<Packet> (100);
  Address dest;
  Ptr<Ipv4QueueDiscItem> item = Create<Ipv4QueueDiscItem> (p, dest, 0, hdr);
  queue->Enqueue (item);
}

void
FqCoDelQueueDiscDeficit::DoRun (void)
{
  Ptr<FqCoDelQueueDisc> queueDisc = CreateObjectWithAttributes<FqCoDelQueueDisc> ();
  Ptr<FqCoDelIpv6PacketFilter> ipv6Filter = CreateObject<FqCoDelIpv6PacketFilter> ();
  Ptr<FqCoDelIpv4PacketFilter> ipv4Filter = CreateObject<FqCoDelIpv4PacketFilter> ();
  queueDisc->AddPacketFilter (ipv6Filter);
  queueDisc->AddPacketFilter (ipv4Filter);

  queueDisc->SetQuantum (90);
  queueDisc->Initialize ();

  Ipv4Header hdr;
  hdr.SetPayloadSize (100);
  hdr.SetSource (Ipv4Address ("10.10.1.1"));
  hdr.SetDestination (Ipv4Address ("10.10.1.2"));
  hdr.SetProtocol (7);

  // Add a packet from the first flow
  AddPacket (queueDisc, hdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 1, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the first flow queue");
  Ptr<FqCoDelFlow> flow1 = StaticCast<FqCoDelFlow> (queueDisc->GetQueueDiscClass (0));
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), static_cast<int32_t> (queueDisc->GetQuantum ()), "the deficit of the first flow must equal the quantum");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::NEW_FLOW, "the first flow must be in the list of new queues");
  // Dequeue a packet
  queueDisc->Dequeue ();
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 0, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 0, "unexpected number of packets in the first flow queue");
  // the deficit for the first flow becomes 90 - (100+20) = -30
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), -30, "unexpected deficit for the first flow");

  // Add two packets from the first flow
  AddPacket (queueDisc, hdr);
  AddPacket (queueDisc, hdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 2, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::NEW_FLOW, "the first flow must still be in the list of new queues");

  // Add two packets from the second flow
  hdr.SetDestination (Ipv4Address ("10.10.1.10"));
  AddPacket (queueDisc, hdr);
  AddPacket (queueDisc, hdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 4, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the second flow queue");
  Ptr<FqCoDelFlow> flow2 = StaticCast<FqCoDelFlow> (queueDisc->GetQueueDiscClass (1));
  NS_TEST_ASSERT_MSG_EQ (flow2->GetDeficit (), static_cast<int32_t> (queueDisc->GetQuantum ()), "the deficit of the second flow must equal the quantum");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetStatus (), FqCoDelFlow::NEW_FLOW, "the second flow must be in the list of new queues");

  // Dequeue a packet (from the second flow, as the first flow has a negative deficit)
  queueDisc->Dequeue ();
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 3, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");
  // the first flow got a quantum of deficit (-30+90=60) and has been moved to the end of the list of old queues
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), 60, "unexpected deficit for the first flow");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::OLD_FLOW, "the first flow must be in the list of old queues");
  // the second flow has a negative deficit (-30) and is still in the list of new queues
  NS_TEST_ASSERT_MSG_EQ (flow2->GetDeficit (), -30, "unexpected deficit for the second flow");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetStatus (), FqCoDelFlow::NEW_FLOW, "the second flow must be in the list of new queues");

  // Dequeue a packet (from the first flow, as the second flow has a negative deficit)
  queueDisc->Dequeue ();
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 2, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");
  // the first flow has a negative deficit (60-(100+20)= -60) and stays in the list of old queues
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), -60, "unexpected deficit for the first flow");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::OLD_FLOW, "the first flow must be in the list of old queues");
  // the second flow got a quantum of deficit (-30+90=60) and has been moved to the end of the list of old queues
  NS_TEST_ASSERT_MSG_EQ (flow2->GetDeficit (), 60, "unexpected deficit for the second flow");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetStatus (), FqCoDelFlow::OLD_FLOW, "the second flow must be in the list of new queues");

  // Dequeue a packet (from the second flow, as the first flow has a negative deficit)
  queueDisc->Dequeue ();
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 1, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 0, "unexpected number of packets in the second flow queue");
  // the first flow got a quantum of deficit (-60+90=30) and has been moved to the end of the list of old queues
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), 30, "unexpected deficit for the first flow");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::OLD_FLOW, "the first flow must be in the list of old queues");
  // the second flow has a negative deficit (60-(100+20)= -60)
  NS_TEST_ASSERT_MSG_EQ (flow2->GetDeficit (), -60, "unexpected deficit for the second flow");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetStatus (), FqCoDelFlow::OLD_FLOW, "the second flow must be in the list of new queues");

  // Dequeue a packet (from the first flow, as the second flow has a negative deficit)
  queueDisc->Dequeue ();
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 0, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 0, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 0, "unexpected number of packets in the second flow queue");
  // the first flow has a negative deficit (30-(100+20)= -90)
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), -90, "unexpected deficit for the first flow");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::OLD_FLOW, "the first flow must be in the list of old queues");
  // the second flow got a quantum of deficit (-60+90=30) and has been moved to the end of the list of old queues
  NS_TEST_ASSERT_MSG_EQ (flow2->GetDeficit (), 30, "unexpected deficit for the second flow");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetStatus (), FqCoDelFlow::OLD_FLOW, "the second flow must be in the list of new queues");

  // Dequeue a packet
  queueDisc->Dequeue ();
  // the first flow is at the head of the list of old queues but has a negative deficit, thus it gets a quantun
  // of deficit (-90+90=0) and is moved to the end of the list of old queues. Then, the second flow (which has a
  // positive deficit) is selected, but the second flow is empty and thus it is set to inactive. The first flow is
  // reconsidered, but it has a null deficit, hence it gets another quantum of deficit (0+90=90). Then, the first
  // flow is reconsidered again, now it has a positive deficit and hence it is selected. But, it is empty and
  // therefore is set to inactive, too.
  NS_TEST_ASSERT_MSG_EQ (flow1->GetDeficit (), 90, "unexpected deficit for the first flow");
  NS_TEST_ASSERT_MSG_EQ (flow1->GetStatus (), FqCoDelFlow::INACTIVE, "the first flow must be inactive");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetDeficit (), 30, "unexpected deficit for the second flow");
  NS_TEST_ASSERT_MSG_EQ (flow2->GetStatus (), FqCoDelFlow::INACTIVE, "the second flow must be inactive");

  Simulator::Destroy ();
}

/**
 * This class tests the TCP flows separation
 */
class FqCoDelQueueDiscTCPFlowsSeparation : public TestCase
{
public:
  FqCoDelQueueDiscTCPFlowsSeparation ();
  virtual ~FqCoDelQueueDiscTCPFlowsSeparation ();

private:
  virtual void DoRun (void);
  void AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header ipHdr, TcpHeader tcpHdr);
};

FqCoDelQueueDiscTCPFlowsSeparation::FqCoDelQueueDiscTCPFlowsSeparation ()
  : TestCase ("Test TCP flows separation")
{
}

FqCoDelQueueDiscTCPFlowsSeparation::~FqCoDelQueueDiscTCPFlowsSeparation ()
{
}

void
FqCoDelQueueDiscTCPFlowsSeparation::AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header ipHdr, TcpHeader tcpHdr)
{
  Ptr<Packet> p = Create<Packet> (100);
  p->AddHeader (tcpHdr);
  Address dest;
  Ptr<Ipv4QueueDiscItem> item = Create<Ipv4QueueDiscItem> (p, dest, 0, ipHdr);
  queue->Enqueue (item);
}

void
FqCoDelQueueDiscTCPFlowsSeparation::DoRun (void)
{
  Ptr<FqCoDelQueueDisc> queueDisc = CreateObjectWithAttributes<FqCoDelQueueDisc> ("PacketLimit", UintegerValue (10));
  Ptr<FqCoDelIpv6PacketFilter> ipv6Filter = CreateObject<FqCoDelIpv6PacketFilter> ();
  Ptr<FqCoDelIpv4PacketFilter> ipv4Filter = CreateObject<FqCoDelIpv4PacketFilter> ();
  queueDisc->AddPacketFilter (ipv6Filter);
  queueDisc->AddPacketFilter (ipv4Filter);

  queueDisc->SetQuantum (1500);
  queueDisc->Initialize ();

  Ipv4Header hdr;
  hdr.SetPayloadSize (100);
  hdr.SetSource (Ipv4Address ("10.10.1.1"));
  hdr.SetDestination (Ipv4Address ("10.10.1.2"));
  hdr.SetProtocol (6);

  TcpHeader tcpHdr;
  tcpHdr.SetSourcePort (7);
  tcpHdr.SetDestinationPort (27);

  // Add three packets from the first flow
  AddPacket (queueDisc, hdr, tcpHdr);
  AddPacket (queueDisc, hdr, tcpHdr);
  AddPacket (queueDisc, hdr, tcpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 3, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");

  // Add a packet from the second flow
  tcpHdr.SetSourcePort (8);
  AddPacket (queueDisc, hdr, tcpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 4, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");

  // Add a packet from the third flow
  tcpHdr.SetDestinationPort (28);
  AddPacket (queueDisc, hdr, tcpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 5, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (2)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the third flow queue");

  // Add two packets from the fourth flow
  tcpHdr.SetSourcePort (7);
  AddPacket (queueDisc, hdr, tcpHdr);
  AddPacket (queueDisc, hdr, tcpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 7, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (2)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the third flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (3)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the third flow queue");

  Simulator::Destroy ();
}

/**
 * This class tests the UDP flows separation
 */
class FqCoDelQueueDiscUDPFlowsSeparation : public TestCase
{
public:
  FqCoDelQueueDiscUDPFlowsSeparation ();
  virtual ~FqCoDelQueueDiscUDPFlowsSeparation ();

private:
  virtual void DoRun (void);
  void AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header ipHdr, UdpHeader udpHdr);
};

FqCoDelQueueDiscUDPFlowsSeparation::FqCoDelQueueDiscUDPFlowsSeparation ()
  : TestCase ("Test UDP flows separation")
{
}

FqCoDelQueueDiscUDPFlowsSeparation::~FqCoDelQueueDiscUDPFlowsSeparation ()
{
}

void
FqCoDelQueueDiscUDPFlowsSeparation::AddPacket (Ptr<FqCoDelQueueDisc> queue, Ipv4Header ipHdr, UdpHeader udpHdr)
{
  Ptr<Packet> p = Create<Packet> (100);
  p->AddHeader (udpHdr);
  Address dest;
  Ptr<Ipv4QueueDiscItem> item = Create<Ipv4QueueDiscItem> (p, dest, 0, ipHdr);
  queue->Enqueue (item);
}

void
FqCoDelQueueDiscUDPFlowsSeparation::DoRun (void)
{
  Ptr<FqCoDelQueueDisc> queueDisc = CreateObjectWithAttributes<FqCoDelQueueDisc> ("PacketLimit", UintegerValue (10));
  Ptr<FqCoDelIpv6PacketFilter> ipv6Filter = CreateObject<FqCoDelIpv6PacketFilter> ();
  Ptr<FqCoDelIpv4PacketFilter> ipv4Filter = CreateObject<FqCoDelIpv4PacketFilter> ();
  queueDisc->AddPacketFilter (ipv6Filter);
  queueDisc->AddPacketFilter (ipv4Filter);

  queueDisc->SetQuantum (1500);
  queueDisc->Initialize ();

  Ipv4Header hdr;
  hdr.SetPayloadSize (100);
  hdr.SetSource (Ipv4Address ("10.10.1.1"));
  hdr.SetDestination (Ipv4Address ("10.10.1.2"));
  hdr.SetProtocol (17);

  UdpHeader udpHdr;
  udpHdr.SetSourcePort (7);
  udpHdr.SetDestinationPort (27);

  // Add three packets from the first flow
  AddPacket (queueDisc, hdr, udpHdr);
  AddPacket (queueDisc, hdr, udpHdr);
  AddPacket (queueDisc, hdr, udpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 3, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");

  // Add a packet from the second flow
  udpHdr.SetSourcePort (8);
  AddPacket (queueDisc, hdr, udpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 4, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");

  // Add a packet from the third flow
  udpHdr.SetDestinationPort (28);
  AddPacket (queueDisc, hdr, udpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 5, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (2)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the third flow queue");

  // Add two packets from the fourth flow
  udpHdr.SetSourcePort (7);
  AddPacket (queueDisc, hdr, udpHdr);
  AddPacket (queueDisc, hdr, udpHdr);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->QueueDisc::GetNPackets (), 7, "unexpected number of packets in the queue disc");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (0)->GetQueueDisc ()->GetNPackets (), 3, "unexpected number of packets in the first flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (1)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the second flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (2)->GetQueueDisc ()->GetNPackets (), 1, "unexpected number of packets in the third flow queue");
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetQueueDiscClass (3)->GetQueueDisc ()->GetNPackets (), 2, "unexpected number of packets in the third flow queue");

  Simulator::Destroy ();
}

class FqCoDelQueueDiscTestSuite : public TestSuite
{
public:
  FqCoDelQueueDiscTestSuite ();
};

FqCoDelQueueDiscTestSuite::FqCoDelQueueDiscTestSuite ()
  : TestSuite ("fq-codel-queue-disc", UNIT)
{
  AddTestCase (new FqCoDelQueueDiscNoSuitableFilter, TestCase::QUICK);
  AddTestCase (new FqCoDelQueueDiscIPFlowsSeparationAndPacketLimit, TestCase::QUICK);
  AddTestCase (new FqCoDelQueueDiscDeficit, TestCase::QUICK);
  AddTestCase (new FqCoDelQueueDiscTCPFlowsSeparation, TestCase::QUICK);
  AddTestCase (new FqCoDelQueueDiscUDPFlowsSeparation, TestCase::QUICK);
}

static FqCoDelQueueDiscTestSuite fqCoDelQueueDiscTestSuite;
