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

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/pcap-file.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/data-rate.h"
#include "ns3/inet-socket-address.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Ns3CwndTest");

// ===========================================================================
// This is a simple test to demonstrate how a known good model (a reference 
// implementation) may be used to test another model without resorting to 
// storing stimulus or response vectors.
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
// implementation is responding correctly.  Once the oopration is verified, we
// enter a list of responses that capture the response succinctly.
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
//
class SimpleSource : public Application 
{
public:

  SimpleSource ();
  virtual ~SimpleSource();

  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);

private:
  virtual void StartApplication (void);  
  virtual void StopApplication (void);

  void ScheduleTx (void);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetsSent;
};

SimpleSource::SimpleSource ()
  : m_socket (0), 
    m_peer (), 
    m_packetSize (0), 
    m_nPackets (0), 
    m_dataRate (0), 
    m_sendEvent (), 
    m_running (false), 
    m_packetsSent (0)
{
}

SimpleSource::~SimpleSource()
{
  m_socket = 0;
}

void
SimpleSource::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
}

void
SimpleSource::StartApplication (void)
{
  m_running = true;
  m_packetsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void 
SimpleSource::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
      Simulator::Cancel (m_sendEvent);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void 
SimpleSource::SendPacket (void)
{
  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  m_socket->Send (packet);

  if (++m_packetsSent < m_nPackets)
    {
      ScheduleTx ();
    }
}

void 
SimpleSource::ScheduleTx (void)
{
  if (m_running)
    {
      Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
      m_sendEvent = Simulator::Schedule (tNext, &SimpleSource::SendPacket, this);
    }
}

class Ns3TcpCwndTestCase : public TestCase
{
public:
  Ns3TcpCwndTestCase ();
  virtual ~Ns3TcpCwndTestCase ();

private:
  virtual bool DoRun (void);
  bool m_writeResults;

  class  CwndEvent {
  public:
    uint32_t m_oldCwnd;
    uint32_t m_newCwnd;
  };

  TestVectors<CwndEvent> m_responses;

  void CwndChange (uint32_t oldCwnd, uint32_t newCwnd);
};

Ns3TcpCwndTestCase::Ns3TcpCwndTestCase ()
  : TestCase ("Check to see that the ns-3 TCP congestion window works as expected against liblinux2.6.26.so"),
    m_writeResults (false)
{
}

Ns3TcpCwndTestCase::~Ns3TcpCwndTestCase ()
{
}

void
Ns3TcpCwndTestCase::CwndChange (uint32_t oldCwnd, uint32_t newCwnd)
{
  CwndEvent event;

  event.m_oldCwnd = oldCwnd;
  event.m_newCwnd = newCwnd;

  m_responses.Add (event);
}

bool
Ns3TcpCwndTestCase::DoRun (void)
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
  stack.SetTcp ("ns3::NscTcpL4Protocol", "Library", StringValue("liblinux2.6.26.so"));
  stack.Install (nodes.Get (1));

  //
  // Assign the address 10.1.1.1 to the TCP implementation under test (index
  // zero) and 10.1.1.2 to the reference implementation (index one).
  //
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  //
  // We need a place to send our TCP data on the node with the reference TCP 
  // implementation.  We aren't really concerned about what happens there, so
  // just create a sink.
  //
  uint16_t sinkPort = 8080;
  Address sinkAddress (InetSocketAddress(interfaces.GetAddress (1), sinkPort));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.));
  sinkApps.Stop (Seconds (1.1));

  //
  // We want to look at changes in the ns-3 TCP congestion window.  The 
  // congestion window is flow clontrol imposed by the sender, so we need
  // to crank up a flow from the ns-3 TCP node to the NSC TCP node and hook the
  // CongestionWindow attribute on the socket.  Normally one would use an on-off
  // application to generate a flow, but this has a couple of problems.  First,
  // the socket of the on-off application is not created until Application Start
  // time, so we wouldn't be able to hook the socket now at configuration time.  
  // Second, even if we could arrange a call after start time, the socket is not 
  // public.
  //
  // So, we can cook up a simple version of the on-off application that does what
  // we want.  On the plus side we don't need all of the complexity of the on-off
  // application.  On the minus side, we don't have a helper, so we have to get
  // a little more involved in the details, but this is trivial.
  //
  // So first, we create a socket and do the trace connect on it; then we pass this
  // socket into the constructor of our simple application which we then install
  // in the node with the ns-3 TCP.
  //
  Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (nodes.Get (0), TcpSocketFactory::GetTypeId ());
  ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeCallback (&Ns3TcpCwndTestCase::CwndChange, this));

  Ptr<SimpleSource> app = CreateObject<SimpleSource> ();
  app->Setup (ns3TcpSocket, sinkAddress, 1040, 10, DataRate ("5Mbps"));
  nodes.Get (0)->AddApplication (app);
  app->Start (Seconds (1.));
  app->Stop (Seconds (1.1));

  //
  // The idea here is that someone will look very closely at the all of the
  // communications between the reference TCP and the TCP under test in this
  // simulation and determine that all of the responses are correct.  We expect
  // that this means generating a pcap trace file from the point-to-point link
  // and examining the packets closely using tcpdump, wireshark or some such
  // program.  So we provide the ability to generate a pcap trace of the 
  // test execution for your perusal.
  //
  // Once the validation test is determined to be running exactly as exptected,
  // the set of congestion window changes is collected and hard coded into the 
  // test results which will then be checked during the actual execution of the
  // test.
  //

  if (m_writeResults)
    {
      PointToPointHelper::EnablePcapAll ("tcp-cwnd");
    }

  Simulator::Stop (Seconds(2));
  Simulator::Run ();
  Simulator::Destroy ();

  //
  // As new acks are received by the TCP under test, the congestion window 
  // should be opened up by one segment (MSS bytes) each time.  This should
  // trigger a congestion window change event which we hooked and saved above.
  // We should now be able to look through the saved response vectors and follow
  // the congestion window as it opens up when the ns-3 TCP under test 
  // transmits its bits
  //
  // From inspecting the results, we know that we should see N_EVENTS congestion
  // window change events.  The window should expand N_EVENTS - 1 times (each
  // time by MSS bytes) until it gets to its largest value.  Then the application
  // sending stops and the window should be slammed shut, with the last event 
  // reflecting the change from LARGEST_CWND back to MSS
  //
  const uint32_t MSS = 536;
  const uint32_t N_EVENTS = 21;
  const uint32_t LARGEST_CWND = MSS * N_EVENTS;

  CwndEvent event;

  NS_TEST_ASSERT_MSG_EQ (m_responses.GetN (), N_EVENTS, "Unexpectedly low number of cwnd change events");


  for (uint32_t i = 0, from = 536, to = 1072; i < N_EVENTS - 1; ++i, from += 536, to += 536)
    {
      event = m_responses.Get (i);
      NS_TEST_ASSERT_MSG_EQ (event.m_oldCwnd, from, "Wrong old cwnd value in cwnd change event " << i);
      NS_TEST_ASSERT_MSG_EQ (event.m_newCwnd, to, "Wrong new cwnd value in cwnd change event " << i);
    }

  event = m_responses.Get (N_EVENTS - 1);
  NS_TEST_ASSERT_MSG_EQ (event.m_oldCwnd, LARGEST_CWND, "Wrong old cwnd value in cwnd change event " << N_EVENTS - 1);
  NS_TEST_ASSERT_MSG_EQ (event.m_newCwnd, MSS, "Wrong new cwnd value in cwnd change event " << N_EVENTS - 1);

  return GetErrorStatus ();
}

class Ns3TcpCwndTestSuite : public TestSuite
{
public:
  Ns3TcpCwndTestSuite ();
};

Ns3TcpCwndTestSuite::Ns3TcpCwndTestSuite ()
  : TestSuite ("ns3-tcp-cwnd", SYSTEM)
{
  AddTestCase (new Ns3TcpCwndTestCase);
}

Ns3TcpCwndTestSuite ns3TcpCwndTestSuite;
