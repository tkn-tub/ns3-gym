/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */



#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/test.h"
#include "ns3/epc-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-sink.h"
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/ipv4-static-routing.h>
#include "ns3/boolean.h"
#include "ns3/uinteger.h"


using namespace ns3;



struct UeTestData
{
  UeTestData (uint32_t n, uint32_t s);

  uint32_t numPkts;
  uint32_t pktSize;
 
  Ptr<PacketSink> serverApp;
  Ptr<Application> clientApp;
};

UeTestData::UeTestData (uint32_t n, uint32_t s)
  : numPkts (n),
    pktSize (s)
{
}

struct EnbTestData
{
  std::vector<UeTestData> ues;
};


class EpcS1uTestCase : public TestCase
{
public:
  EpcS1uTestCase (std::string name, std::vector<EnbTestData> v);
  virtual ~EpcS1uTestCase ();

private:
  virtual void DoRun (void);
  std::vector<EnbTestData> m_enbTestData;
};


EpcS1uTestCase::EpcS1uTestCase (std::string name, std::vector<EnbTestData> v)
  : TestCase (name),
    m_enbTestData (v)
{
}

EpcS1uTestCase::~EpcS1uTestCase ()
{
}

void 
EpcS1uTestCase::DoRun ()
{
  Ptr<EpcHelper> epcHelper = CreateObject<EpcHelper> ();
  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  
  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the internet
  PointToPointHelper p2ph;
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);  
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  ipv4h.Assign (internetDevices);
  
  // setup default gateway for the remote hosts
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());

  // hardcoded UE addresses for now
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.255.255.0"), 1);
  



  NodeContainer enbs;

  for (std::vector<EnbTestData>::iterator enbit = m_enbTestData.begin ();
       enbit < m_enbTestData.end ();
       ++enbit)
    {
      Ptr<Node> enb = CreateObject<Node> ();
      enbs.Add (enb);

      // we test EPC without LTE, hence we use:
      // 1) a CSMA network to simulate the cell
      // 2) a raw socket opened on the CSMA device to simulate the LTE socket

      NodeContainer ues;
      ues.Create (enbit->ues.size ());

      NodeContainer cell;
      cell.Add (ues);
      cell.Add (enb);

      CsmaHelper csmaCell;      
      NetDeviceContainer cellDevices = csmaCell.Install (cell);

      // the eNB's CSMA NetDevice acting as an LTE NetDevice. 
      Ptr<NetDevice> lteEnbNetDevice = cellDevices.Get (cellDevices.GetN () - 1);

      // Note that the EpcEnbApplication won't care of the actual NetDevice type
      epcHelper->AddEnb (enb, lteEnbNetDevice);      
      
      // we install the IP stack on UEs only
      InternetStackHelper internet;
      internet.Install (ues);

      // assign IP address to UEs, and install applications
      for (uint32_t u = 0; u < ues.GetN (); ++u)
        {
          Ptr<NetDevice> ueLteDevice = cellDevices.Get (u);
          Ipv4InterfaceContainer ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevice));

          Ptr<Node> ue = ues.Get (u);

          // disable IP Forwarding on the UE. This is because we use
          // CSMA broadcast MAC addresses for this test. The problem
          // won't happen with a LteUeNetDevice. 
          ue->GetObject<Ipv4> ()->SetAttribute ("IpForward", BooleanValue (false));
          
          uint16_t port = 1234;
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
          ApplicationContainer apps = packetSinkHelper.Install (ue);
          apps.Start (Seconds (1.0));
          apps.Stop (Seconds (10.0));
          enbit->ues[u].serverApp = apps.Get (0)->GetObject<PacketSink> ();
          
          Time interPacketInterval = Seconds (0.01);
          UdpClientHelper client (ueIpIface.GetAddress (0), port);
          client.SetAttribute ("MaxPackets", UintegerValue (enbit->ues[u].numPkts));
          client.SetAttribute ("Interval", TimeValue (interPacketInterval));
          client.SetAttribute ("PacketSize", UintegerValue (enbit->ues[u].pktSize));
          apps = client.Install (remoteHost);
          apps.Start (Seconds (2.0));
          apps.Stop (Seconds (10.0));   
          enbit->ues[u].clientApp = apps.Get (0);

          uint16_t rnti = u+1;
          uint16_t lcid = 1;
          epcHelper->ActivateEpsBearer (ueLteDevice, lteEnbNetDevice, LteTft::Default (), rnti, lcid);
          
        } 
            
    } 
  
  Simulator::Run ();

  for (std::vector<EnbTestData>::iterator enbit = m_enbTestData.begin ();
       enbit < m_enbTestData.end ();
       ++enbit)
    {
      for (std::vector<UeTestData>::iterator ueit = enbit->ues.begin ();
           ueit < enbit->ues.end ();
           ++ueit)
        {
          NS_TEST_ASSERT_MSG_EQ (ueit->serverApp->GetTotalRx (), (ueit->numPkts) * (ueit->pktSize), "wrong total received bytes");
        }      
    }
  
  Simulator::Destroy ();
}





/**
 * Test that the S1-U interface implementation works correctly
 */
class EpcS1uTestSuite : public TestSuite
{
public:
  EpcS1uTestSuite ();
  
} g_epcS1uTestSuiteInstance;

EpcS1uTestSuite::EpcS1uTestSuite ()
  : TestSuite ("epc-s1u-downlink", SYSTEM)
{  
  std::vector<EnbTestData> v1;  
  EnbTestData e1;
  UeTestData f1 (1, 100);
  e1.ues.push_back (f1);
  v1.push_back (e1);
  AddTestCase (new EpcS1uTestCase ("1 eNB, 1UE", v1));


  std::vector<EnbTestData> v2;  
  EnbTestData e2;
  UeTestData f2_1 (1, 100);
  e2.ues.push_back (f2_1);
  UeTestData f2_2 (2, 200);
  e2.ues.push_back (f2_2);
  v2.push_back (e2);
  AddTestCase (new EpcS1uTestCase ("1 eNB, 2UEs", v2));


  std::vector<EnbTestData> v3;  
  v3.push_back (e1);
  v3.push_back (e2);
  AddTestCase (new EpcS1uTestCase ("2 eNBs", v3));


  EnbTestData e3;
  UeTestData f3_1 (3, 50);
  e3.ues.push_back (f3_1);
  UeTestData f3_2 (5, 1472);
  e3.ues.push_back (f3_2);
  UeTestData f3_3 (1, 1);
  e3.ues.push_back (f3_2);
  std::vector<EnbTestData> v4;  
  v4.push_back (e3);
  v4.push_back (e1);
  v4.push_back (e2);
  AddTestCase (new EpcS1uTestCase ("3 eNBs", v4));

  







}

