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

const bool WRITE_VECTORS = false;           // set to true to write response vectors
const uint32_t PCAP_LINK_TYPE = 1187373553; // Some large random number -- we use to verify data was written by this program
const uint32_t PCAP_SNAPLEN   = 64;         // Don't bother to save much data

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
  virtual void DoSetup (void);
  virtual bool DoRun (void);
  virtual void DoTeardown (void);

  std::string m_pcapFilename;
  PcapFile m_pcapFile;
  bool m_writeVectors;
  bool m_writeResults;

  void Ipv4L3Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface);

};

Ns3TcpLossTestCase1::Ns3TcpLossTestCase1 ()
  : TestCase ("Check that ns-3 TCP survives loss of first two SYNs"),
    m_writeVectors (WRITE_VECTORS),
    m_writeResults (false)
{
}

void
Ns3TcpLossTestCase1::DoSetup (void)
{
    //
    // We expect there to be a file called ns3tcp-loss1-response-vectors.pcap" in
    // the source directory of this file.
    //
    m_pcapFilename = NS_TEST_SOURCEDIR + "ns3tcp-loss1-response-vectors.pcap";

    if (m_writeVectors)
      {
        m_pcapFile.Open (m_pcapFilename, std::ios::out|std::ios::binary);
        m_pcapFile.Init(PCAP_LINK_TYPE, PCAP_SNAPLEN);
      }
    else
      {
        m_pcapFile.Open (m_pcapFilename, std::ios::in|std::ios::binary);
        NS_ABORT_MSG_UNLESS (m_pcapFile.GetDataLinkType () == PCAP_LINK_TYPE, "Wrong response vectors in directory");
      }
}

void
Ns3TcpLossTestCase1::DoTeardown (void)
{
    m_pcapFile.Close ();
}

void
Ns3TcpLossTestCase1::Ipv4L3Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
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
      
      uint32_t size = p->GetSize ();
      uint8_t *buf = new uint8_t[size];
      p->CopyData (buf, size);

      m_pcapFile.Write (uint32_t (tMicroSeconds / 1000000), 
                        uint32_t (tMicroSeconds % 1000000), 
                        buf, 
                        size);
      delete [] buf;
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

      uint32_t result = memcmp(actual, expected, readLen);

      delete [] actual;

      //
      // Avoid streams of errors -- only report the first.
      //
      if (GetErrorStatus () == false)
        {
          NS_TEST_EXPECT_MSG_EQ (result, 0, "Expected data comparison error");
        }
    }
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

  Config::Connect ("/NodeList/0/$ns3::Ipv4L3Protocol/Tx",
                   MakeCallback (&Ns3TcpLossTestCase1::Ipv4L3Tx, this));

  Simulator::Schedule(Seconds (2), &SocketWriter::Connect, socketWriter);
  Simulator::Schedule(Seconds (10), &SocketWriter::Write, socketWriter, 500);
  Simulator::Schedule(writerStopTimeObj, &SocketWriter::Close, socketWriter);

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
      pointToPoint.EnablePcapAll ("tcp-loss-test-case-1");
      pointToPoint.EnableAsciiAll ("tcp-loss-test-case-1");
    }

  Simulator::Stop (simStopTimeObj);
  Simulator::Run ();
  Simulator::Destroy ();

  return GetErrorStatus ();
}

class Ns3TcpLossTestCase2 : public TestCase
{
public:
  Ns3TcpLossTestCase2 ();
  virtual ~Ns3TcpLossTestCase2 () {}

private:
  virtual void DoSetup (void);
  virtual bool DoRun (void);
  virtual void DoTeardown (void);

  std::string m_pcapFilename;
  PcapFile m_pcapFile;
  bool m_writeVectors;
  bool m_writeResults;

  void Ipv4L3Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface);
};

Ns3TcpLossTestCase2::Ns3TcpLossTestCase2 ()
  : TestCase ("Check that ns-3 TCP survives loss of first data packet"),
    m_writeVectors (WRITE_VECTORS),
    m_writeResults (false)
{
}

void
Ns3TcpLossTestCase2::DoSetup (void)
{
    //
    // We expect there to be a file called ns3tcp-loss1-response-vectors.pcap" in
    // the source directory of this file.
    //
    m_pcapFilename = NS_TEST_SOURCEDIR + "ns3tcp-loss2-response-vectors.pcap";

    if (m_writeVectors)
      {
        m_pcapFile.Open (m_pcapFilename, std::ios::out|std::ios::binary);
        m_pcapFile.Init(PCAP_LINK_TYPE, PCAP_SNAPLEN);
      }
    else
      {
        m_pcapFile.Open (m_pcapFilename, std::ios::in|std::ios::binary);
        NS_ABORT_MSG_UNLESS (m_pcapFile.GetDataLinkType () == PCAP_LINK_TYPE, "Wrong response vectors in directory");
      }
}

void
Ns3TcpLossTestCase2::DoTeardown (void)
{
    m_pcapFile.Close ();
}

void
Ns3TcpLossTestCase2::Ipv4L3Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
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
      uint32_t size = p->GetSize ();
      uint8_t *buf = new uint8_t[size];
      p->CopyData (buf, size);

      m_pcapFile.Write (uint32_t (tMicroSeconds / 1000000), 
                        uint32_t (tMicroSeconds % 1000000), 
                        buf, 
                        size);
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

      uint32_t result = memcmp(actual, expected, readLen);

      delete [] actual;

      //
      // Avoid streams of errors -- only report the first.
      //
      if (GetErrorStatus () == false)
        {
          NS_TEST_EXPECT_MSG_EQ (result, 0, "Expected data comparison error");
        }
    }
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

  Config::Connect ("/NodeList/0/$ns3::Ipv4L3Protocol/Tx",
                   MakeCallback (&Ns3TcpLossTestCase2::Ipv4L3Tx, this));

  Simulator::Schedule(Seconds (2), &SocketWriter::Connect, socketWriter);
  Simulator::Schedule(Seconds (10), &SocketWriter::Write, socketWriter, 500);
  Simulator::Schedule(writerStopTimeObj, &SocketWriter::Close, socketWriter);

  std::list<uint32_t> sampleList;
  // Lose first data segment
  sampleList.push_back (2);
  // This time, we'll explicitly create the error model we want
  Ptr<ReceiveListErrorModel> pem = CreateObject<ReceiveListErrorModel> ();
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
  Packet::EnablePrinting ();  // Enable packet metadata for all test cases
  AddTestCase (new Ns3TcpLossTestCase1);
  AddTestCase (new Ns3TcpLossTestCase2);
}

Ns3TcpLossTestSuite ns3TcpLossTestSuite;
