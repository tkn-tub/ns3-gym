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
#include "../ns3tcp/ns3tcp-socket-writer.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NscTcpLossTest");

// ===========================================================================
// Tests of TCP implementation loss behavior
// ===========================================================================
//

class NscTcpLossTestCase1 : public TestCase
{
public:
  NscTcpLossTestCase1 ();
  virtual ~NscTcpLossTestCase1 () {}

private:
  virtual void DoRun (void);
  bool m_writeResults;

  void SinkRx (std::string path, Ptr<const Packet> p, const Address &address);

  TestVectors<uint32_t> m_inputs;
  TestVectors<uint32_t> m_responses;
};

NscTcpLossTestCase1::NscTcpLossTestCase1 ()
  : TestCase ("Check that nsc TCP survives loss of first two SYNs"),
    m_writeResults (false)
{
}

void 
NscTcpLossTestCase1::SinkRx (std::string path, Ptr<const Packet> p, const Address &address)
{
  m_responses.Add (p->GetSize ());
}

void
NscTcpLossTestCase1::DoRun (void)
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
  internet.SetTcp ("ns3::NscTcpL4Protocol", "Library", StringValue ("liblinux2.6.26.so"));
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
                   MakeCallback (&NscTcpLossTestCase1::SinkRx, this));

  Simulator::Schedule (Seconds (2), &SocketWriter::Connect, socketWriter);
  Simulator::Schedule (Seconds (10), &SocketWriter::Write, socketWriter, 500);
  m_inputs.Add (500);
  Simulator::Schedule (writerStopTimeObj, &SocketWriter::Close, socketWriter);

  std::list<uint32_t> sampleList;
  // Lose first two SYNs
  sampleList.push_back (0);
  sampleList.push_back (1);
  // This time, we'll explicitly create the error model we want
  Ptr<ReceiveListErrorModel> pem = CreateObject<ReceiveListErrorModel> ();
  pem->SetList (sampleList);
  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (pem));

  if (m_writeResults)
    {
      pointToPoint.EnablePcapAll ("nsc-tcp-loss-test-case-1");
      pointToPoint.EnableAsciiAll ("nsc-tcp-loss-test-case-1");
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
}

class NscTcpLossTestCase2 : public TestCase
{
public:
  NscTcpLossTestCase2 ();
  virtual ~NscTcpLossTestCase2 () {}

private:
  virtual void DoRun (void);
  bool m_writeResults;

  void SinkRx (std::string path, Ptr<const Packet> p, const Address &address);

  TestVectors<uint32_t> m_inputs;
  TestVectors<uint32_t> m_responses;
};

NscTcpLossTestCase2::NscTcpLossTestCase2 ()
  : TestCase ("Check that nsc TCP survives loss of first data packet"),
    m_writeResults (false)
{
}

void 
NscTcpLossTestCase2::SinkRx (std::string path, Ptr<const Packet> p, const Address &address)
{
  m_responses.Add (p->GetSize ());
}

void
NscTcpLossTestCase2::DoRun (void)
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
  internet.SetTcp ("ns3::NscTcpL4Protocol", "Library", StringValue ("liblinux2.6.26.so"));
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
                   MakeCallback (&NscTcpLossTestCase2::SinkRx, this));

  Simulator::Schedule (Seconds (2), &SocketWriter::Connect, socketWriter);
  Simulator::Schedule (Seconds (10), &SocketWriter::Write, socketWriter, 500);
  m_inputs.Add (500);
  Simulator::Schedule (writerStopTimeObj, &SocketWriter::Close, socketWriter);

  std::list<uint32_t> sampleList;
  // Lose first data segment
  sampleList.push_back (2);
  // This time, we'll explicitly create the error model we want
  Ptr<ReceiveListErrorModel> pem = CreateObject<ReceiveListErrorModel> ();
  pem->SetList (sampleList);
  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (pem));

  if (m_writeResults)
    {
      pointToPoint.EnablePcapAll ("nsc-tcp-loss-test-case-2");
      pointToPoint.EnableAsciiAll ("nsc-tcp-loss-test-case-2");
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
}

class NscTcpLossTestSuite : public TestSuite
{
public:
  NscTcpLossTestSuite ();
};

NscTcpLossTestSuite::NscTcpLossTestSuite ()
  : TestSuite ("nsc-tcp-loss", SYSTEM)
{
  AddTestCase (new NscTcpLossTestCase1, TestCase::QUICK);
  AddTestCase (new NscTcpLossTestCase2, TestCase::QUICK);
}

static NscTcpLossTestSuite nscTcpLossTestSuite;
