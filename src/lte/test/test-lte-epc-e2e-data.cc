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
#include "ns3/lena-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-sink.h"
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/ipv4-static-routing.h>
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/abort.h"
#include "ns3/mobility-helper.h"




namespace ns3 {


NS_LOG_COMPONENT_DEFINE ("LteEpcE2eData");



struct UeTestData
{
  UeTestData (uint32_t n, uint32_t s);

  uint32_t numPkts;
  uint32_t pktSize;
  EpsBearer epsBearer;
 
  Ptr<PacketSink> serverApp;
  Ptr<Application> clientApp;
};

UeTestData::UeTestData (uint32_t n, uint32_t s)
  : numPkts (n),
    pktSize (s),
    epsBearer (EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT))
{
}

struct EnbTestData
{
  std::vector<UeTestData> ues;
};


class LteEpcE2eDataTestCase : public TestCase
{
public:
  LteEpcE2eDataTestCase (std::string name, std::vector<EnbTestData> v);
  virtual ~LteEpcE2eDataTestCase ();

private:
  virtual void DoRun (void);
  std::vector<EnbTestData> m_enbTestData;
};


LteEpcE2eDataTestCase::LteEpcE2eDataTestCase (std::string name, std::vector<EnbTestData> v)
  : TestCase (name),
    m_enbTestData (v)
{
}

LteEpcE2eDataTestCase::~LteEpcE2eDataTestCase ()
{
}

void 
LteEpcE2eDataTestCase::DoRun ()
{

  Ptr<LenaHelper> lteHelper = CreateObject<LenaHelper> ();
  Ptr<EpcHelper> epcHelper = CreateObject<EpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);

  lteHelper->SetAttribute("PathlossModel",
                          StringValue("ns3::FriisPropagationLossModel"));

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
  enbs.Create (m_enbTestData.size ());
  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (10000.0),
                                 "DeltaY", DoubleValue (10000.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  enbMobility.Install (enbs);
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbs);
  NetDeviceContainer::Iterator enbLteDevIt = enbLteDevs.Begin ();

  Vector enbPosition; 

  for (std::vector<EnbTestData>::iterator enbit = m_enbTestData.begin ();
       enbit < m_enbTestData.end ();
       ++enbit, ++enbLteDevIt)
    {
      NS_ABORT_IF (enbLteDevIt ==  enbLteDevs.End ());
      


      NodeContainer ues;
      ues.Create (enbit->ues.size ());
      Vector enbPosition = (*enbLteDevIt)->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
      MobilityHelper ueMobility;
      ueMobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",
                                        "X", DoubleValue (enbPosition.x),
                                        "Y", DoubleValue (enbPosition.y),
                                        "rho", DoubleValue (100.0));
      ueMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      ueMobility.Install (ues);
      NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ues);
      lteHelper->Attach (ueLteDevs, *enbLteDevIt);        
       
      // we install the IP stack on the UEs 
      InternetStackHelper internet;
      internet.Install (ues);
      

      // assign IP address to UEs, and install applications
      for (uint32_t u = 0; u < ues.GetN (); ++u)
        {
          Ptr<NetDevice> ueLteDevice = ueLteDevs.Get (u);
          Ipv4InterfaceContainer ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevice));

          Ptr<Node> ue = ues.Get (u);
          
          uint16_t port = 1234;
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
          ApplicationContainer apps = packetSinkHelper.Install (ue);
          apps.Start (Seconds (0.1));
          apps.Stop (Seconds (10.0));
          enbit->ues[u].serverApp = apps.Get (0)->GetObject<PacketSink> ();
          
          Time interPacketInterval = Seconds (0.01);
          UdpClientHelper client (ueIpIface.GetAddress (0), port);
          client.SetAttribute ("MaxPackets", UintegerValue (enbit->ues[u].numPkts));
          client.SetAttribute ("Interval", TimeValue (interPacketInterval));
          client.SetAttribute ("PacketSize", UintegerValue (enbit->ues[u].pktSize));
          apps = client.Install (remoteHost);
          apps.Start (Seconds (0.1));
          apps.Stop (Seconds (10.0));   
          enbit->ues[u].clientApp = apps.Get (0);
          
          lteHelper->ActivateEpsBearer (ueLteDevice, enbit->ues[u].epsBearer, LteTft::Default ());
          
        } 
            
    } 

  Simulator::Stop (Seconds (5.0));  
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
class LteEpcE2eDataTestSuite : public TestSuite
{
public:
  LteEpcE2eDataTestSuite ();
  
} g_lteEpcE2eDataTestSuite;

LteEpcE2eDataTestSuite::LteEpcE2eDataTestSuite ()
  : TestSuite ("lte-epc-e2e-data", SYSTEM)
{  
  std::vector<EnbTestData> v1;  
  EnbTestData e1;
  UeTestData f1 (1, 100);
  e1.ues.push_back (f1);
  v1.push_back (e1);
  AddTestCase (new LteEpcE2eDataTestCase ("1 eNB, 1UE", v1));


  std::vector<EnbTestData> v2;  
  EnbTestData e2;
  UeTestData f2_1 (1, 100);
  e2.ues.push_back (f2_1);
  UeTestData f2_2 (2, 200);
  e2.ues.push_back (f2_2);
  v2.push_back (e2);
  AddTestCase (new LteEpcE2eDataTestCase ("1 eNB, 2UEs", v2));


  std::vector<EnbTestData> v3;  
  v3.push_back (e1);
  v3.push_back (e2);
  AddTestCase (new LteEpcE2eDataTestCase ("2 eNBs", v3));


  EnbTestData e3;
  UeTestData f3_1 (3, 50);
  e3.ues.push_back (f3_1);
  UeTestData f3_2 (5, 1400);
  e3.ues.push_back (f3_2);
  UeTestData f3_3 (1, 1);
  e3.ues.push_back (f3_2);
  std::vector<EnbTestData> v4;  
  v4.push_back (e3);
  v4.push_back (e1);
  v4.push_back (e2);
  AddTestCase (new LteEpcE2eDataTestCase ("3 eNBs", v4));

  EnbTestData e4;
  UeTestData f4_1 (5, 1000);
  e4.ues.push_back (f4_1);
  std::vector<EnbTestData> v5;
  v5.push_back (e4);
  AddTestCase (new LteEpcE2eDataTestCase ("1 eNB, 1UE with 1000 byte packets", v5));


  EnbTestData e5;
  UeTestData f5_1 (5, 1400);
  e5.ues.push_back (f5_1);
  std::vector<EnbTestData> v6;
  v6.push_back (e5);
  AddTestCase (new LteEpcE2eDataTestCase ("1 eNB, 1UE with 1400 byte packets", v6));

}



}  // namespace ns3

