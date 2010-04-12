/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/error-model.h"
#include "ns3/pointer.h"
#include "ns3tcp-socket-writer.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Ns3TcpLossTest");

// ===========================================================================
// Tests of TCP implementation loss behavior
// ===========================================================================
//

class Ns3TcpLossTestCase1 : public TestCase
{
public:
  Ns3TcpLossTestCase1 ();
  virtual ~Ns3TcpLossTestCase1 () {}

private:
  virtual bool DoRun (void);
  bool m_writeResults;

  void SinkRx (std::string path, Ptr<const Packet> p, const Address &address);

  TestVectors<uint32_t> m_inputs;
  TestVectors<uint32_t> m_responses;
};

Ns3TcpLossTestCase1::Ns3TcpLossTestCase1 ()
  : TestCase ("Check that ns-3 TCP survives loss of first two SYNs"),
    m_writeResults (false)
{
}

void 
Ns3TcpLossTestCase1::SinkRx (std::string path, Ptr<const Packet> p, const Address &address)
{
  m_responses.Add (p->GetSize ());
}

bool
Ns3TcpLossTestCase1::DoRun (void)
{
  uint16_t sinkPort = 50000;
  double sinkStopTime = 40;  // sec; will trigger Socket::Close
  double writerStopTime = 30;  // sec; will trigger Socket::Close
  double simStopTime = 60;  // sec
  Time sinkStopTimeObj = Seconds (sinkStopTime);
  Time writerStopTimeObj = Seconds (writerStopTime);
  Time simStopTimeObj= Seconds (simStopTime);

  Ptr<Node> n0 = CreateObject<Node> ();
  Ptr<Node> n1 = CreateObject<Node> ();

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("200ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (n0, n1);

  InternetStackHelper internet;
  internet.InstallAll ();

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer ifContainer = address.Assign (devices);

  Ptr<SocketWriter> socketWriter = CreateObject<SocketWriter> ();
  Address sinkAddress (InetSocketAddress (ifContainer.GetAddress (1), sinkPort));
  socketWriter->Setup (n0, sinkAddress);
  n0->AddApplication (socketWriter);
  socketWriter->SetStartTime (Seconds (0.));
  socketWriter->SetStopTime (writerStopTimeObj);

  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer apps = sink.Install (n1);
  // Start the sink application at time zero, and stop it at sinkStopTime
  apps.Start (Seconds (0.0));
  apps.Stop (sinkStopTimeObj);

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx",
                   MakeCallback (&Ns3TcpLossTestCase1::SinkRx, this));

  Simulator::Schedule(Seconds (2), &SocketWriter::Connect, socketWriter);
  Simulator::Schedule(Seconds (10), &SocketWriter::Write, socketWriter, 500);
  m_inputs.Add (500);
  Simulator::Schedule(writerStopTimeObj, &SocketWriter::Close, socketWriter);

  std::list<uint32_t> sampleList;
  // Lose first two SYNs
  sampleList.push_back (0);
  sampleList.push_back (1);
  // This time, we'll explicitly create the error model we want
  Ptr<ListErrorModel> pem = CreateObject<ListErrorModel> ();
  pem->SetList (sampleList);
  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (pem));

  if (m_writeResults)
    {
      pointToPoint.EnablePcapAll ("tcp-loss-test-case-1");
      pointToPoint.EnableAsciiAll ("tcp-loss-test-case-1");
    }

  Simulator::Stop (simStopTimeObj);
  Simulator::Run ();
  Simulator::Destroy ();

  // Compare inputs and outputs
  NS_TEST_ASSERT_MSG_EQ (m_inputs.GetN (), m_responses.GetN (), "Incorrect number of expected receive events");
  for (uint32_t i = 0; i < m_responses.GetN (); i++)
    {
      uint32_t in = m_inputs.Get (i);
      uint32_t out = m_responses.Get (i);
      NS_TEST_ASSERT_MSG_EQ (in, out, "Mismatch:  expected " << in << " bytes, got " << out << " bytes");
    }

  return GetErrorStatus ();
}

class Ns3TcpLossTestCase2 : public TestCase
{
public:
  Ns3TcpLossTestCase2 ();
  virtual ~Ns3TcpLossTestCase2 () {}

private:
  virtual bool DoRun (void);
  bool m_writeResults;

  void SinkRx (std::string path, Ptr<const Packet> p, const Address &address);

  TestVectors<uint32_t> m_inputs;
  TestVectors<uint32_t> m_responses;
};

Ns3TcpLossTestCase2::Ns3TcpLossTestCase2 ()
  : TestCase ("Check that ns-3 TCP survives loss of first data packet"),
    m_writeResults (false)
{
}

void 
Ns3TcpLossTestCase2::SinkRx (std::string path, Ptr<const Packet> p, const Address &address)
{
  m_responses.Add (p->GetSize ());
}

bool
Ns3TcpLossTestCase2::DoRun (void)
{
  uint16_t sinkPort = 50000;
  double sinkStopTime = 40;  // sec; will trigger Socket::Close
  double writerStopTime = 12;  // sec; will trigger Socket::Close
  double simStopTime = 60;  // sec
  Time sinkStopTimeObj = Seconds (sinkStopTime);
  Time writerStopTimeObj = Seconds (writerStopTime);
  Time simStopTimeObj= Seconds (simStopTime);

  Ptr<Node> n0 = CreateObject<Node> ();
  Ptr<Node> n1 = CreateObject<Node> ();

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("200ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (n0, n1);

  InternetStackHelper internet;
  internet.InstallAll ();

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer ifContainer = address.Assign (devices);

  Ptr<SocketWriter> socketWriter = CreateObject<SocketWriter> ();
  Address sinkAddress (InetSocketAddress (ifContainer.GetAddress (1), sinkPort));
  socketWriter->Setup (n0, sinkAddress);
  n0->AddApplication (socketWriter);
  socketWriter->SetStartTime (Seconds (0.));
  socketWriter->SetStopTime (writerStopTimeObj);

  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer apps = sink.Install (n1);
  // Start the sink application at time zero, and stop it at sinkStopTime
  apps.Start (Seconds (0.0));
  apps.Stop (sinkStopTimeObj);

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx",
                   MakeCallback (&Ns3TcpLossTestCase2::SinkRx, this));

  Simulator::Schedule(Seconds (2), &SocketWriter::Connect, socketWriter);
  Simulator::Schedule(Seconds (10), &SocketWriter::Write, socketWriter, 500);
  m_inputs.Add (500);
  Simulator::Schedule(writerStopTimeObj, &SocketWriter::Close, socketWriter);

  std::list<uint32_t> sampleList;
  // Lose first data segment
  sampleList.push_back (15);
  // This time, we'll explicitly create the error model we want
  Ptr<ListErrorModel> pem = CreateObject<ListErrorModel> ();
  pem->SetList (sampleList);
  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (pem));

  if (m_writeResults)
    {
      pointToPoint.EnablePcapAll ("tcp-loss-test-case-2");
      pointToPoint.EnableAsciiAll ("tcp-loss-test-case-2");
    }

  Simulator::Stop (simStopTimeObj);
  Simulator::Run ();
  Simulator::Destroy ();

  // Compare inputs and outputs
  NS_TEST_ASSERT_MSG_EQ (m_inputs.GetN (), m_responses.GetN (), "Incorrect number of expected receive events");
  for (uint32_t i = 0; i < m_responses.GetN (); i++)
    {
      uint32_t in = m_inputs.Get (i);
      uint32_t out = m_responses.Get (i);
      NS_TEST_ASSERT_MSG_EQ (in, out, "Mismatch:  expected " << in << " bytes, got " << out << " bytes");
    }

  return GetErrorStatus ();
}

class Ns3TcpLossTestSuite : public TestSuite
{
public:
  Ns3TcpLossTestSuite ();
};

Ns3TcpLossTestSuite::Ns3TcpLossTestSuite ()
  : TestSuite ("ns3-tcp-loss", SYSTEM)
{
  AddTestCase (new Ns3TcpLossTestCase1);
  //AddTestCase (new Ns3TcpLossTestCase2);
}

Ns3TcpLossTestSuite ns3TcpLossTestSuite;
