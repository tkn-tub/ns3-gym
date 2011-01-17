/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hemanth Narra
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
 * Author: Hemanth Narra <hemanth@ittc.ku.com>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */
#include "ns3/test.h"
#include "ns3/mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/mobility-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/dsdv-helper.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/v4ping-helper.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/pcap-file.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/dsdv-packet.h"
#include "ns3/dsdv-rtable.h"

namespace ns3 {
class DsdvHeaderTestCase : public TestCase
{
public:
  DsdvHeaderTestCase ();
  ~DsdvHeaderTestCase ();
  virtual void
  DoRun (void);
};
DsdvHeaderTestCase::DsdvHeaderTestCase ()
  : TestCase ("Verifying the DSDV header")
{
}
DsdvHeaderTestCase::~DsdvHeaderTestCase ()
{
}
void
DsdvHeaderTestCase::DoRun ()
{
  Ptr<Packet> packet = Create<Packet> ();

  {
    dsdv::DsdvHeader hdr1;
    hdr1.SetDst (Ipv4Address ("10.1.1.2"));
    hdr1.SetDstSeqno (2);
    hdr1.SetHopCount (2);
    packet->AddHeader (hdr1);
    dsdv::DsdvHeader hdr2;
    hdr2.SetDst (Ipv4Address ("10.1.1.3"));
    hdr2.SetDstSeqno (4);
    hdr2.SetHopCount (1);
    packet->AddHeader (hdr2);
    NS_TEST_ASSERT_MSG_EQ (packet->GetSize (), 24, "XXX");
  }

  {
    dsdv::DsdvHeader hdr2;
    packet->RemoveHeader (hdr2);
    NS_TEST_ASSERT_MSG_EQ (hdr2.GetSerializedSize (),12,"XXX");
    NS_TEST_ASSERT_MSG_EQ (hdr2.GetDst (),Ipv4Address ("10.1.1.3"),"XXX");
    NS_TEST_ASSERT_MSG_EQ (hdr2.GetDstSeqno (),4,"XXX");
    NS_TEST_ASSERT_MSG_EQ (hdr2.GetHopCount (),1,"XXX");
    dsdv::DsdvHeader hdr1;
    packet->RemoveHeader (hdr1);
    NS_TEST_ASSERT_MSG_EQ (hdr1.GetSerializedSize (),12,"XXX");
    NS_TEST_ASSERT_MSG_EQ (hdr1.GetDst (),Ipv4Address ("10.1.1.2"),"XXX");
    NS_TEST_ASSERT_MSG_EQ (hdr1.GetDstSeqno (),2,"XXX");
    NS_TEST_ASSERT_MSG_EQ (hdr1.GetHopCount (),2,"XXX");
  }
}

class DsdvTableTestCase : public TestCase
{
public:
  DsdvTableTestCase ();
  ~DsdvTableTestCase ();
  virtual void
  DoRun (void);
};

DsdvTableTestCase::DsdvTableTestCase ()
  : TestCase ("Dsdv Routing Table test case")
{
}
DsdvTableTestCase::~DsdvTableTestCase ()
{
}
void
DsdvTableTestCase::DoRun ()
{
  dsdv::RoutingTable rtable;
  Ptr<NetDevice> dev;
  {
    dsdv::RoutingTableEntry rEntry1 (
      /*device=*/ dev, /*dst=*/
      Ipv4Address ("10.1.1.4"), /*seqno=*/ 2,
      /*iface=*/ Ipv4InterfaceAddress (Ipv4Address ("10.1.1.1"), Ipv4Mask ("255.255.255.0")),
      /*hops=*/ 2, /*next hop=*/
      Ipv4Address ("10.1.1.2"),
      /*lifetime=*/ Seconds (10));
    NS_TEST_EXPECT_MSG_EQ (rtable.AddRoute (rEntry1),true,"add route");
    dsdv::RoutingTableEntry rEntry2 (
      /*device=*/ dev, /*dst=*/
      Ipv4Address ("10.1.1.2"), /*seqno=*/ 4,
      /*iface=*/ Ipv4InterfaceAddress (Ipv4Address ("10.1.1.1"), Ipv4Mask ("255.255.255.0")),
      /*hops=*/ 1, /*next hop=*/
      Ipv4Address ("10.1.1.2"),
      /*lifetime=*/ Seconds (10));
    NS_TEST_EXPECT_MSG_EQ (rtable.AddRoute (rEntry2),true,"add route");
    dsdv::RoutingTableEntry rEntry3 (
      /*device=*/ dev, /*dst=*/
      Ipv4Address ("10.1.1.3"), /*seqno=*/ 4,
      /*iface=*/ Ipv4InterfaceAddress (Ipv4Address ("10.1.1.1"), Ipv4Mask ("255.255.255.0")),
      /*hops=*/ 1, /*next hop=*/
      Ipv4Address ("10.1.1.3"),
      /*lifetime=*/ Seconds (10));
    NS_TEST_EXPECT_MSG_EQ (rtable.AddRoute (rEntry3),true,"add route");
    dsdv::RoutingTableEntry rEntry4 (
      /*device=*/ dev, /*dst=*/
      Ipv4Address ("10.1.1.255"), /*seqno=*/ 0,
      /*iface=*/ Ipv4InterfaceAddress (Ipv4Address ("10.1.1.1"), Ipv4Mask ("255.255.255.0")),
      /*hops=*/ 0, /*next hop=*/
      Ipv4Address ("10.1.1.255"),
      /*lifetime=*/ Seconds (10));
    NS_TEST_EXPECT_MSG_EQ (rtable.AddRoute (rEntry4),true,"add route");
  }
  {
    dsdv::RoutingTableEntry rEntry;
    if (rtable.LookupRoute (Ipv4Address ("10.1.1.4"), rEntry))
      {
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetDestination (),Ipv4Address ("10.1.1.4"),"XXX");
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetSeqNo (),2,"XXX");
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetHop (),2,"XXX");
      }
    if (rtable.LookupRoute (Ipv4Address ("10.1.1.2"), rEntry))
      {
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetDestination (),Ipv4Address ("10.1.1.2"),"XXX");
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetSeqNo (),4,"XXX");
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetHop (),1,"XXX");
      }
    if (rtable.LookupRoute (Ipv4Address ("10.1.1.3"), rEntry))
      {
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetDestination (),Ipv4Address ("10.1.1.3"),"XXX");
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetSeqNo (),4,"XXX");
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetHop (),1,"XXX");
      }
    if (rtable.LookupRoute (Ipv4Address ("10.1.1.255"), rEntry))
      {
        NS_TEST_ASSERT_MSG_EQ (rEntry.GetDestination (),Ipv4Address ("10.1.1.255"),"XXX");
      }
    NS_TEST_ASSERT_MSG_EQ (rEntry.GetInterface ().GetLocal (),Ipv4Address ("10.1.1.1"),"XXX");
    NS_TEST_ASSERT_MSG_EQ (rEntry.GetInterface ().GetBroadcast (),Ipv4Address ("10.1.1.255"),"XXX");
    NS_TEST_ASSERT_MSG_EQ (rtable.RoutingTableSize (),4,"Rtable size incorrect");
  }
  Simulator::Destroy ();
}

class DsdvTestSuite : public TestSuite
{
public:
  DsdvTestSuite () : TestSuite ("routing-dsdv", UNIT)
  {
    AddTestCase (new DsdvHeaderTestCase ());
    AddTestCase (new DsdvTableTestCase ());
  }
} g_dsdvTestSuite;
}
