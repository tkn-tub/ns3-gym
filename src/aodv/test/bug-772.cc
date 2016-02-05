/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#include "bug-772.h"

#include "ns3/simulator.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/string.h"
#include "ns3/mobility-helper.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/abort.h"
#include "ns3/mobility-model.h"
#include "ns3/pcap-file.h"
#include "ns3/aodv-helper.h"
#include "ns3/v4ping-helper.h"
#include "ns3/config.h"
#include "ns3/inet-socket-address.h"
#include "ns3/data-rate.h"
#include "ns3/pcap-test.h"
#include <sstream>

using namespace ns3;

//-----------------------------------------------------------------------------
// UdpChainTest
//-----------------------------------------------------------------------------
Bug772ChainTest::Bug772ChainTest (const char * const prefix, const char * const proto, Time t, uint32_t size) : 
  TestCase ("Bug 772 UDP and TCP chain regression test"),
  m_nodes (0),
  m_prefix (prefix),
  m_proto (proto),
  m_time (t),
  m_size (size),
  m_step (120),
  m_port (9){
}

Bug772ChainTest::~Bug772ChainTest ()
{
  delete m_nodes;
}

void
Bug772ChainTest::SendData (Ptr<Socket> socket)
{
  if (Simulator::Now () < m_time)
    {
      socket->Send (Create<Packet> (1000));
      Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0.125),
                                      &Bug772ChainTest::SendData, this, socket);
    }
}

void
Bug772ChainTest::DoRun ()
{
  RngSeedManager::SetSeed (12345);
  RngSeedManager::SetRun (7);

  CreateNodes ();
  CreateDevices ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();

  CheckResults ();

  delete m_nodes, m_nodes = 0;
}

void
Bug772ChainTest::CreateNodes ()
{
  m_nodes = new NodeContainer;
  m_nodes->Create (m_size);
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (m_step),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (m_size),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (*m_nodes);
}

void
Bug772ChainTest::CreateDevices ()
{
  int64_t streamsUsed = 0;
  // 1. Setup WiFi
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  // This test suite output was originally based on YansErrorRateModel
  wifiPhy.SetErrorRateModel ("ns3::YansErrorRateModel");
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  Ptr<YansWifiChannel> chan = wifiChannel.Create ();
  wifiPhy.SetChannel (chan);
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", StringValue ("2200"));
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, *m_nodes); 

  // Assign fixed stream numbers to wifi and channel random variables
  streamsUsed += wifi.AssignStreams (devices, streamsUsed);
  // Assign 6 streams per device
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (devices.GetN () * 6), "Stream assignment mismatch");
  streamsUsed += wifiChannel.AssignStreams (chan, streamsUsed);
  // Assign 0 streams per channel for this configuration 
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (devices.GetN () * 6), "Stream assignment mismatch");

  // 2. Setup TCP/IP & AODV
  AodvHelper aodv; // Use default parameters here
  InternetStackHelper internetStack;
  internetStack.SetRoutingHelper (aodv);
  internetStack.Install (*m_nodes);
  streamsUsed += internetStack.AssignStreams (*m_nodes, streamsUsed);
  // Expect to use (3*m_size) more streams for internet stack random variables
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, ((devices.GetN () * 6) + (3*m_size)), "Stream assignment mismatch");
  streamsUsed += aodv.AssignStreams (*m_nodes, streamsUsed);
  // Expect to use m_size more streams for AODV
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, ((devices.GetN () * 6) + (3*m_size) + m_size), "Stream assignment mismatch");
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // 3. Setup UDP source and sink
  m_sendSocket = Socket::CreateSocket (m_nodes->Get (0), TypeId::LookupByName (m_proto));
  m_sendSocket->Bind ();
  m_sendSocket->Connect (InetSocketAddress (interfaces.GetAddress (m_size-1), m_port));
  m_sendSocket->SetAllowBroadcast (true);
  Simulator::ScheduleWithContext (m_sendSocket->GetNode ()->GetId (), Seconds (1.0),
                                  &Bug772ChainTest::SendData, this, m_sendSocket);

  m_recvSocket = Socket::CreateSocket (m_nodes->Get (m_size - 1), TypeId::LookupByName (m_proto));
  m_recvSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_port));
  m_recvSocket->Listen ();
  m_recvSocket->ShutdownSend ();

  // 4. write PCAP on the first and last nodes only
  wifiPhy.EnablePcap (CreateTempDirFilename (m_prefix), devices.Get (0));
  wifiPhy.EnablePcap (CreateTempDirFilename (m_prefix), devices.Get (m_size-1));
}

void
Bug772ChainTest::CheckResults ()
{
  for (uint32_t i = 0; i < m_size; i += (m_size - 1) /*first and last nodes only*/)
    {
      NS_PCAP_TEST_EXPECT_EQ(m_prefix << "-" << i << "-0.pcap");
    }
}
