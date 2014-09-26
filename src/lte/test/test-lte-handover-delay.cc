/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions
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
 * Author: Budiarto Herman <budiarto.herman@magister.fi>
 */

#include <ns3/test.h>

#include <ns3/log.h>
#include <ns3/nstime.h>
#include <ns3/callback.h>
#include <ns3/config.h>
#include <ns3/boolean.h>
#include <ns3/simulator.h>

#include <ns3/node-container.h>
#include <ns3/net-device-container.h>
#include <ns3/ipv4-interface-container.h>

#include <ns3/lte-helper.h>
#include <ns3/point-to-point-epc-helper.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/mobility-helper.h>

#include <ns3/data-rate.h>
#include <ns3/ipv4-static-routing.h>
#include <ns3/position-allocator.h>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("LteHandoverDelayTest");

/*
 * HANDOVER DELAY TEST CASE
 */

class LteHandoverDelayTestCase : public TestCase
{
public:
  LteHandoverDelayTestCase (bool useIdealRrc, Time handoverTime,
      Time delayThreshold, Time simulationDuration)
      : TestCase ("Verifying that the time needed for handover is under a specified threshold"),
        m_useIdealRrc (useIdealRrc),
        m_handoverTime (handoverTime),
        m_delayThreshold (delayThreshold),
        m_simulationDuration (simulationDuration),
        m_ueHandoverStart (Seconds (0)),
        m_enbHandoverStart (Seconds (0))
  {
  }
private:
  virtual void DoRun (void);

  void UeHandoverStartCallback (std::string context, uint64_t imsi,
      uint16_t cellid, uint16_t rnti, uint16_t targetCellId);
  void UeHandoverEndOkCallback (std::string context, uint64_t imsi,
      uint16_t cellid, uint16_t rnti);
  void EnbHandoverStartCallback (std::string context, uint64_t imsi,
      uint16_t cellid, uint16_t rnti, uint16_t targetCellId);
  void EnbHandoverEndOkCallback (std::string context, uint64_t imsi,
      uint16_t cellid, uint16_t rnti);

  bool m_useIdealRrc;
  Time m_handoverTime;
  Time m_delayThreshold;
  Time m_simulationDuration;

  Time m_ueHandoverStart;
  Time m_enbHandoverStart;
};


void
LteHandoverDelayTestCase::DoRun ()
{
  NS_LOG_INFO ("-----test case: ideal RRC = " << m_useIdealRrc
      << " handover time = " << m_handoverTime.GetSeconds () << "-----");

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->SetAttribute ("UseIdealRrc", BooleanValue(m_useIdealRrc));

  // SETUP A REMOTE HOST

  NodeContainer remoteHosts;
  remoteHosts.Create (1);
  InternetStackHelper inetStackHelper;
  inetStackHelper.Install (remoteHosts);

  // SETUP POINT-TO-POINT CONNECTION BETWEEN REMOTE HOST AND EPC

  PointToPointHelper p2pHelper;
  p2pHelper.SetDeviceAttribute ("DataRate",
      DataRateValue (DataRate ("100Gb/s")));
  p2pHelper.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2pHelper.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));

  NetDeviceContainer inetDevs = p2pHelper.Install (epcHelper->GetPgwNode (),
      remoteHosts.Get (0));

  Ipv4AddressHelper addrHelper;
  addrHelper.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer inetIfs;
  inetIfs = addrHelper.Assign (inetDevs);

  // SETUP ROUTING

  Ipv4StaticRoutingHelper ipRoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostRouting =
      ipRoutingHelper.GetStaticRouting (remoteHosts.Get (0)->GetObject<Ipv4> ());
  remoteHostRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"),
      Ipv4Mask ("255.0.0.0"), 1);

  // CREATE NODES

  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (2);
  ueNodes.Create (1);

  /**
   * eNodeB 0                    UE                      eNodeB 1
   *
   *    x ----------------------- x ----------------------- x
   *              500 m                      500 m
   */

  // SETUP MOBILITY

  Ptr<ListPositionAllocator> posAlloc = CreateObject<ListPositionAllocator> ();
  posAlloc->Add (Vector (0, 0, 0));
  posAlloc->Add (Vector (1000, 0, 0));
  posAlloc->Add (Vector (500, 0, 0));

  MobilityHelper mobilityHelper;
  mobilityHelper.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityHelper.SetPositionAllocator (posAlloc);
  mobilityHelper.Install (enbNodes);
  mobilityHelper.Install (ueNodes);

  // SETUP LTE DEVICES

  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // SETUP INTERNET IN UE

  inetStackHelper.Install(ueNodes);
  Ipv4InterfaceContainer ueIfs;
  ueIfs = epcHelper->AssignUeIpv4Address (ueDevs);

  // SETUP DEFAULT GATEWAY FOR UE

  Ptr<Ipv4StaticRouting> ueStaticRouting =
      ipRoutingHelper.GetStaticRouting (ueNodes.Get (0)->GetObject<Ipv4> ());
  ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (),
      1);

  // INSTALLING TRACES

  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
      MakeCallback (&LteHandoverDelayTestCase::UeHandoverStartCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
      MakeCallback (&LteHandoverDelayTestCase::UeHandoverEndOkCallback, this));

  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
      MakeCallback (&LteHandoverDelayTestCase::EnbHandoverStartCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
      MakeCallback (&LteHandoverDelayTestCase::EnbHandoverEndOkCallback, this));

  // SIMULATION

  lteHelper->AddX2Interface (enbNodes);
  lteHelper->Attach (ueDevs.Get(0), enbDevs.Get(0));
  lteHelper->HandoverRequest (m_handoverTime, ueDevs.Get (0), enbDevs.Get (0),
      enbDevs.Get (1));
  // disable error model in dl ctrl channel
  //Config::Set ("/NodeList/*/DeviceList/*/LteUePhy/DlSpectrumPhy/CtrlErrorModelEnabled",
  //    BooleanValue (false));
  Simulator::Stop (m_simulationDuration);
  Simulator::Run ();
  Simulator::Destroy ();

} // end of void LteHandoverDelayTestCase::DoRun ()


void
LteHandoverDelayTestCase::UeHandoverStartCallback (std::string context,
    uint64_t imsi, uint16_t cellid, uint16_t rnti, uint16_t targetCellId)
{
  NS_LOG_FUNCTION (this << context);
  m_ueHandoverStart = Simulator::Now ();
}

void
LteHandoverDelayTestCase::UeHandoverEndOkCallback (std::string context,
    uint64_t imsi, uint16_t cellid, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context);
  NS_ASSERT (m_ueHandoverStart > Seconds (0));
  Time delay = Simulator::Now () - m_ueHandoverStart;
  NS_LOG_DEBUG (this << " UE delay = " << delay.GetSeconds ());
  NS_TEST_ASSERT_MSG_LT (delay.GetSeconds (), m_delayThreshold.GetSeconds (),
      "UE handover delay is higher than the allowed threshold "
      << "(ideal RRC = " << m_useIdealRrc
      << " handover time = " << m_handoverTime.GetSeconds () << ")");
}


void
LteHandoverDelayTestCase::EnbHandoverStartCallback (std::string context,
    uint64_t imsi, uint16_t cellid, uint16_t rnti, uint16_t targetCellId)
{
  NS_LOG_FUNCTION (this << context);
  m_enbHandoverStart = Simulator::Now ();
}

void
LteHandoverDelayTestCase::EnbHandoverEndOkCallback (std::string context,
    uint64_t imsi, uint16_t cellid, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context);
  NS_ASSERT (m_enbHandoverStart > Seconds (0));
  Time delay = Simulator::Now () - m_enbHandoverStart;
  NS_LOG_DEBUG (this << " eNodeB delay = " << delay.GetSeconds ());
  NS_TEST_ASSERT_MSG_LT (delay.GetSeconds (), m_delayThreshold.GetSeconds (),
      "eNodeB handover delay is higher than the allowed threshold "
      << "(ideal RRC = " << m_useIdealRrc
      << " handover time = " << m_handoverTime.GetSeconds () << ")");
}



/*
 * TEST SUITE
 */

static class LteHandoverDelayTestSuite : public TestSuite
{
public:
  LteHandoverDelayTestSuite ()
      : TestSuite ("lte-handover-delay", TestSuite::SYSTEM)
  {
    //LogComponentEnable ("LteHandoverDelayTest", LOG_PREFIX_TIME);
    //LogComponentEnable ("LteHandoverDelayTest", LOG_DEBUG);
    //LogComponentEnable ("LteHandoverDelayTest", LOG_INFO);

    // HANDOVER DELAY TEST CASES WITH IDEAL RRC (THRESHOLD = 0.005 sec)

    for (Time handoverTime = Seconds (0.100); handoverTime < Seconds (0.110);
        handoverTime += Seconds (0.001))
      {
        // arguments: useIdealRrc, handoverTime, delayThreshold, simulationDuration
        AddTestCase (
            new LteHandoverDelayTestCase (true, handoverTime, Seconds (0.005),
                Seconds (0.200)), TestCase::QUICK);
      }

    // HANDOVER DELAY TEST CASES WITH REAL RRC (THRESHOLD = 0.020 sec)

    for (Time handoverTime = Seconds (0.100); handoverTime < Seconds (0.110);
        handoverTime += Seconds (0.001))
      {
        // arguments: useIdealRrc, handoverTime, delayThreshold, simulationDuration
        AddTestCase (
            new LteHandoverDelayTestCase (false, handoverTime, Seconds (0.020),
                Seconds (0.200)), TestCase::QUICK);
      }
  }
} g_lteHandoverDelayTestSuite;
