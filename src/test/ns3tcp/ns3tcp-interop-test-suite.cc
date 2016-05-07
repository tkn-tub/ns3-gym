/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include <string>

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/pcap-file.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/inet-socket-address.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Ns3TcpInteropTest");

// The below boolean constants should only be changed to 'true'       
// during test debugging (i.e. do not commit the value 'true')

// set to 'true' to have the test suite overwrite the response vectors
// stored in the test directory.  This should only be done if you are
// convinced through other means (e.g. pcap tracing or logging) that the
// revised vectors are the correct ones.  In other words, don't simply
// enable this to true to clear a failing test without looking at the
// results closely.
const bool WRITE_VECTORS = false;           // set to true to write response vectors
const bool WRITE_PCAP = false;              // set to true to write out pcap
const uint32_t PCAP_LINK_TYPE = 1187373553; // Some large random number -- we use to verify data was written by this program
const uint32_t PCAP_SNAPLEN   = 64;         // Don't bother to save much data


// ===========================================================================
// This is a simple test to demonstrate how a known good model (a reference 
// implementation) may be used to test another model in a relatively simple
// way.
//
// Node zero contains the model under test, in this case the ns-3 TCP 
// implementation.  Node one contains the reference implementation that we
// assume will generate good test vectors for us.  In this case, a Linux
// TCP implementation is used to stimulate the ns-3 TCP model with what we
// assume are perfectly good packets.  We watch the ns-3 implementation to
// see what it does in the presence of these assumed good stimuli.
//
// The test is arranged as a typical ns-3 script, but we use the trace system
// to peek into the running system and monitor the ns-3 TCP.
//
// The topology is just two nodes communicating over a point-to-point network.
// The point-to-point network is chosen because it is simple and allows us to
// easily generate pcap traces we can use to separately verify that the ns-3
// implementation is responding correctly.  Once the operation is verified, we
// capture a set of response vectors that are then checked in the test to
// ensure that the ns-3 TCP continues to respond correctly over time.
//
//         node 0                 node 1
//   +----------------+    +----------------+
//   |    ns-3 TCP    |    |    Linux TCP   |
//   +----------------+    +----------------+
//   |    10.1.1.1    |    |    10.1.1.2    |
//   +----------------+    +----------------+
//   | point-to-point |    | point-to-point |
//   +----------------+    +----------------+
//           |                     |
//           +---------------------+
//                5 Mbps, 2 ms
//
// ===========================================================================
class Ns3TcpInteroperabilityTestCase : public TestCase
{
public:
  Ns3TcpInteroperabilityTestCase ();
  virtual ~Ns3TcpInteroperabilityTestCase ();

private:
  virtual void DoSetup (void);
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  void Ipv4L3Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface);

  std::string m_pcapFilename;
  PcapFile m_pcapFile;
  bool m_writeVectors;
  bool m_writeResults;
};

Ns3TcpInteroperabilityTestCase::Ns3TcpInteroperabilityTestCase ()
  : TestCase ("Check to see that the ns-3 TCP can work with liblinux2.6.26.so"), m_writeVectors (WRITE_VECTORS), m_writeResults (WRITE_PCAP)
{
}

Ns3TcpInteroperabilityTestCase::~Ns3TcpInteroperabilityTestCase ()
{
}

void 
Ns3TcpInteroperabilityTestCase::DoSetup (void)
{
  //
  // We expect there to be a file called tcp-interop-response-vectors.pcap in
  // the data directory
  //
  m_pcapFilename = CreateDataDirFilename ("ns3tcp-interop-response-vectors.pcap");

  if (m_writeVectors)
    {
      m_pcapFile.Open (m_pcapFilename, std::ios::out|std::ios::binary);
      m_pcapFile.Init (PCAP_LINK_TYPE, PCAP_SNAPLEN);
    }
  else
    {
      m_pcapFile.Open (m_pcapFilename, std::ios::in|std::ios::binary);
      NS_ABORT_MSG_UNLESS (m_pcapFile.GetDataLinkType () == PCAP_LINK_TYPE, "Wrong response vectors in directory");
    }
}

void 
Ns3TcpInteroperabilityTestCase::DoTeardown (void)
{
  m_pcapFile.Close ();
}

void
Ns3TcpInteroperabilityTestCase::Ipv4L3Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
{
  //
  // We're not testing IP so remove and toss the header.  In order to do this,
  // though, we need to copy the packet since we have a const version.
  //
  Ptr<Packet> p = packet->Copy ();
  Ipv4Header ipHeader;
  p->RemoveHeader (ipHeader);

  //
  // What is left is the TCP header and any data that may be sent.  We aren't
  // sending any TCP data, so we expect what remains is only TCP header, which
  // is a small thing to save.
  //
  if (m_writeVectors)
    {
      //
      // Save the TCP under test response for later testing.
      //
      Time tNow = Simulator::Now ();
      int64_t tMicroSeconds = tNow.GetMicroSeconds ();

      m_pcapFile.Write (uint32_t (tMicroSeconds / 1000000), 
                        uint32_t (tMicroSeconds % 1000000), 
                        p
                        );
    }
  else
    {
      //
      // Read the TCP under test expected response from the expected vector
      // file and see if it still does the right thing.
      //
      uint8_t expected[PCAP_SNAPLEN];
      uint32_t tsSec, tsUsec, inclLen, origLen, readLen;
      m_pcapFile.Read (expected, sizeof(expected), tsSec, tsUsec, inclLen, origLen, readLen);

      uint8_t *actual = new uint8_t[readLen];
      p->CopyData (actual, readLen);

      uint32_t result = memcmp (actual, expected, readLen);

      delete [] actual;

      //
      // Avoid streams of errors -- only report the first.
      //
      if (IsStatusSuccess ())
        {
          NS_TEST_EXPECT_MSG_EQ (result, 0, "Expected data comparison error");
        }
    }
}

void
Ns3TcpInteroperabilityTestCase::DoRun (void)
{
  //
  // Just create two nodes.  One (node zero) will be the node with the TCP
  // under test which is the ns-3 TCP implementation.  The other node (node
  // one) will be the node with the reference implementation we use to drive
  // the tests.
  //
  NodeContainer nodes;
  nodes.Create (2);

  //
  // For this test we'll use a point-to-point net device.  It's not as simple
  // as a simple-net-device, but it provides nice places to hook trace events
  // so we can see what's moving between our nodes.
  //
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  //
  // Install the point-to-point devices on both nodes and connec them up.
  //
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  //
  // Install two variants of the internet stack.  The first, on node zero 
  // uses the TCP under test, which is the default ns-3 TCP implementation.
  //
  InternetStackHelper stack;
  stack.Install (nodes.Get (0));

  //
  // The other node, node one, is going to be set up to use a Linux TCP
  // implementation that we consider a known good TCP.
  //
  std::string nscStack = "liblinux2.6.26.so";
  stack.SetTcp ("ns3::NscTcpL4Protocol", "Library", StringValue ("liblinux2.6.26.so"));
  stack.Install (nodes.Get (1));

  //
  // Assign the address 10.1.1.1 to the TCP implementation under test (index
  // zero) and 10.1.1.2 to the reference implementation (index one).
  //
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  //
  // We need a place for the TCP data to go on the node with the TCP under
  // test, so just create a sink on node zero.
  //
  uint16_t sinkPort = 8080;
  Address sinkAddress (InetSocketAddress (interfaces.GetAddress (0), sinkPort));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (0));
  sinkApps.Start (Seconds (0.));

  //
  // We need something to shove data down the pipe, so we create an on-off 
  // application on the soure node with the reference TCP implementation.
  // The default behavior is to send for one second, then go quiet for one
  // second, and repeat.
  //
  OnOffHelper onOffHelper ("ns3::TcpSocketFactory", sinkAddress);
  onOffHelper.SetAttribute ("MaxBytes", UintegerValue (100000));
  ApplicationContainer sourceApps = onOffHelper.Install (nodes.Get (1));
  sourceApps.Start (Seconds (1.));
  sourceApps.Stop (Seconds (10.));

  //
  // There are currently a limited number of trace hooks in the ns-3 TCP code.
  // Rather than editing TCP to insert a bunch of trace hooks, we can just
  // intercept the packets at the IPv4 layer.  See internet-stack-helper.cc
  // for complete description of the trace hooks.  We're interested in the 
  // responses of the TCP under test, which implies we need to hook the node
  // zero Ipv4 layer three transmit trace source.  We'll then get all of the
  // responses we need
  //
  Config::Connect ("/NodeList/0/$ns3::Ipv4L3Protocol/Tx", 
                   MakeCallback (&Ns3TcpInteroperabilityTestCase::Ipv4L3Tx, this));

  //
  // The idea here is that someone will look very closely at the all of the
  // communications between the reference TCP and the TCP under test in this
  // simulation and determine that all of the responses are correct.  We expect
  // that this means generating a pcap trace file from the point-to-point link
  // and examining the packets closely using tcpdump, wireshark or some such
  // program.  So we provide the ability to generate a pcap trace of the 
  // test execution for your perusal.
  //
  // Once the validation test is determined to be running exactly as expected,
  // we allow you to generate a file that contains the response vectors that 
  // will be checked during the actual execution of the test.
  //

  if (m_writeResults)
    {
      pointToPoint.EnablePcapAll ("ns3-tcp-interop");
    }

  Simulator::Stop (Seconds (20));
  Simulator::Run ();
  Simulator::Destroy ();
}

class Ns3TcpInteroperabilityTestSuite : public TestSuite
{
public:
  Ns3TcpInteroperabilityTestSuite ();
};

Ns3TcpInteroperabilityTestSuite::Ns3TcpInteroperabilityTestSuite ()
  : TestSuite ("ns3-tcp-interoperability", SYSTEM)
{
  // We can't use NS_TEST_SOURCEDIR variable here because we use subdirectories
  SetDataDir ("src/test/ns3tcp/response-vectors");
  
  AddTestCase (new Ns3TcpInteroperabilityTestCase, TestCase::QUICK);
}

static Ns3TcpInteroperabilityTestSuite ns3TcpInteroperabilityTestSuite;
