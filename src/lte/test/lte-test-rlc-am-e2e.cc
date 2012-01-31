/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/lte-simple-helper.h"
#include "ns3/error-model.h"


#include "ns3/radio-bearer-stats-calculator.h"

#include "ns3/lte-rlc-header.h"
#include "ns3/lte-rlc-um.h"

#include "ns3/lte-test-entities.h"
#include "ns3/lte-test-rlc-am-e2e.h"

#include "ns3/config-store.h"


NS_LOG_COMPONENT_DEFINE ("LteRlcAmE2eTest");

using namespace ns3;


/**
 * Test x.x.x RLC AM: End-to-end flow
 */

/**
 * TestSuite
 */

LteRlcAmE2eTestSuite::LteRlcAmE2eTestSuite ()
  : TestSuite ("lte-rlc-am-e2e", SYSTEM)
{
  // NS_LOG_INFO ("Creating LteRlcAmE2eTestSuite");

//   double losses[] = {0.0, 0.10, 0.25, 0.50, 0.75, 0.90, 1.00};
//   uint32_t seeds[] = {1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888, 9999, 10101};

  double losses[] = {0.10};
  uint32_t seeds[] = {1111};

  for ( uint32_t l = 0 ; l < (sizeof (losses) / sizeof (double)) ; l++ )
    {
      for ( uint32_t s = 0 ; s < (sizeof (seeds) / sizeof (uint32_t)) ; s++ )
        {
          std::ostringstream name;
          name << " Losses = " << losses[l] << "%. Seed = " << seeds[s];
          AddTestCase (new LteRlcAmE2eTestCase (name.str (), seeds[s], losses[l]));
        }
    }
}

static LteRlcAmE2eTestSuite lteRlcAmE2eTestSuite;


/**
 * TestCase
 */

LteRlcAmE2eTestCase::LteRlcAmE2eTestCase (std::string name, uint32_t seed, double losses)
  : TestCase (name)
{
  // NS_LOG_UNCOND ("Creating LteRlcAmTestingTestCase: " + name);

  m_seed = seed;
  m_losses = losses;

//   m_enbCount = 0;
//   m_ueCount = 0;
  m_enbDrops = 0;
  m_ueDrops = 0;
}

LteRlcAmE2eTestCase::~LteRlcAmE2eTestCase ()
{
}


// bool
// LteRlcAmE2eTestCase::EnbReceive (Ptr<NetDevice> nd, Ptr<const Packet> p, uint16_t protocol, const Address& addr)
// {
//   NS_LOG_FUNCTION (this);
//   m_enbCount++;
//   return true;
// }
// 
// bool
// LteRlcAmE2eTestCase::UeReceive (Ptr<NetDevice> nd, Ptr<const Packet> p, uint16_t protocol, const Address& addr)
// {
//   NS_LOG_FUNCTION (this);
//   m_ueCount++;
//   return true;
// }

void
LteRlcAmE2eTestCase::EnbDropEvent (Ptr<const Packet> p)
{
  // NS_LOG_FUNCTION (this);
  m_enbDrops++;
}

void
LteRlcAmE2eTestCase::UeDropEvent (Ptr<const Packet> p)
{
  // NS_LOG_FUNCTION (this);
  m_ueDrops++;
}

void
LteRlcAmE2eTestCase::DoRun (void)
{
  uint16_t numberOfNodes = 1;
  // uint16_t rnti = 1111;
  // uint8_t lcid = 222;

  Packet::EnablePrinting ();

  LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);
  LogComponentEnable ("LteRlcAmE2eTest", level);
  // LogComponentEnable ("Config", level);
  // LogComponentEnable ("NodeList", level);
  // LogComponentEnable ("ChannelList", level);
  // LogComponentEnable ("ErrorModel", level);
  LogComponentEnable ("SimpleChannel", level);
  LogComponentEnable ("SimpleNetDevice", level);
  LogComponentEnable ("LteTestEntities", level);
  LogComponentEnable ("LtePdcp", level);
  LogComponentEnable ("LteRlc", level);
  LogComponentEnable ("LteRlcUm", level);
  LogComponentEnable ("LteRlcAm", level);

  SeedManager::SetSeed (m_seed);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults();

  Ptr<LteSimpleHelper> lteSimpleHelper = CreateObject<LteSimpleHelper> ();
  // lteSimpleHelper->EnableLogComponents ();
  // lteSimpleHelper->EnableTraces ();

  // lteSimpleHelper->SetAttribute ("EpsBearerToRlcMapping", StringValue ("RlcUmAlways"));
  lteSimpleHelper->SetAttribute ("RlcEntity", StringValue ("RlcAm"));

  // eNB and UE nodes
  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (numberOfNodes);
  ueNodes.Create (numberOfNodes);

  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteSimpleHelper->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueLteDevs = lteSimpleHelper->InstallUeDevice (ueNodes);

  // Note: Just one eNB and UE is supported. Everything is done in InstallEnbDevice and InstallUeDevice
  // Attach one UE per eNodeB
  // for (uint16_t i = 0; i < numberOfNodes; i++)
  //   {
  //     lteSimpleHelper->Attach (ueLteDevs.Get(i), enbLteDevs.Get(i));
  //   }

  //   lteSimpleHelper->ActivateEpsBearer (ueLteDevs, EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT), EpcTft::Default ());


  // Create transmission PDCP test entity
//   Ptr<LteTestPdcp> txPdcp = CreateObject<LteTestPdcp> ();
//   txPdcp->SetRnti (rnti);
//   txPdcp->SetLcId (lcid);

  // Create transmission AM RLC entity
//   Ptr<LteRlcUm> txRlc = CreateObject<LteRlcUm> ();
//   txRlc->SetRnti (rnti);
//   txRlc->SetLcId (lcid);

  // Create loopback MAC test entities
//   Ptr<LteTestMac> txMac = CreateObject<LteTestMac> ();
//   Ptr<LteTestMac> rxMac = CreateObject<LteTestMac> ();
//   txMac->SetPdcpHeaderPresent (true);
//   txMac->SetTxOpportunityMode (LteTestMac::AUTOMATIC_MODE);
//   rxMac->SetPdcpHeaderPresent (true);
//   rxMac->SetTxOpportunityMode (LteTestMac::AUTOMATIC_MODE);

  // Create reception AM RLC entity
//   Ptr<LteRlcUm> rxRlc = CreateObject<LteRlcUm> ();
//   rxRlc->SetRnti (rnti);
//   rxRlc->SetLcId (lcid);

  // Create reception PDCP test entity
//   Ptr<LteTestPdcp> rxPdcp = CreateObject<LteTestPdcp> ();
//   rxPdcp->SetRnti (rnti);
//   rxPdcp->SetLcId (lcid);

  // Connect SAPs: PDCP (TX) <-> RLC (Tx) <-> MAC <--> MAC <-> RLC (Rx) <-> PDCP (Rx)
//   txPdcp->SetLteRlcSapProvider (txRlc->GetLteRlcSapProvider ());
//   txRlc->SetLteRlcSapUser (txPdcp->GetLteRlcSapUser ());

//   txRlc->SetLteMacSapProvider (txMac->GetLteMacSapProvider ());
//   txMac->SetLteMacSapUser (txRlc->GetLteMacSapUser ()); // TODO ???

//   txMac->SetLteMacLoopback (rxMac);
//   rxMac->SetLteMacLoopback (txMac);

//   rxRlc->SetLteMacSapProvider (rxMac->GetLteMacSapProvider ());
//   rxMac->SetLteMacSapUser (rxRlc->GetLteMacSapUser ()); // TODO ???

//   rxPdcp->SetLteRlcSapProvider (rxRlc->GetLteRlcSapProvider ());
//   rxRlc->SetLteRlcSapUser (rxPdcp->GetLteRlcSapUser ());

//   ConfigStore inputConfig;
//   inputConfig.ConfigureDefaults();

//   GtkConfigStore config;
//   config.ConfigureAttributes();


//           Ptr<LteEnbNetDevice> dev = CreateObject<LteEnbNetDevice> (n, phy, mac, sched, rrc);
//           phy->SetDevice (dev);
//           dlPhy->SetDevice (dev);
//           ulPhy->SetDevice (dev);

//           n->AddDevice (dev);
//           ulPhy->SetGenericPhyRxEndOkCallback (MakeCallback (&LteEnbPhy::PhyPduReceived, phy));
//
//           dev->Start ();
//           return dev;


  // Note: These callbacks are already set in the LteSimpleHelper, the receive callback is in the MAC
//   enbLteDevs.Get (0)->SetReceiveCallback (MakeCallback (&LteRlcAmE2eTestCase::EnbReceive, this));
//   ueLteDevs.Get (0)->SetReceiveCallback (MakeCallback (&LteRlcAmE2eTestCase::UeReceive, this));

  Ptr<RateErrorModel> enbEm = CreateObjectWithAttributes<RateErrorModel> ("RanVar", RandomVariableValue (UniformVariable (0.0, 1.0)));
  enbEm->SetAttribute ("ErrorRate", DoubleValue (m_losses));
  enbEm->SetAttribute ("ErrorUnit", StringValue ("EU_PKT"));

  Ptr<RateErrorModel> ueEm = CreateObjectWithAttributes<RateErrorModel> ("RanVar", RandomVariableValue (UniformVariable (0.0, 1.0)));
  ueEm->SetAttribute ("ErrorRate", DoubleValue (m_losses));
  ueEm->SetAttribute ("ErrorUnit", StringValue ("EU_PKT"));

  // The below hooks will cause drops and receptions to be counted
  enbLteDevs.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (enbEm));
  enbLteDevs.Get (0)->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&LteRlcAmE2eTestCase::EnbDropEvent, this));
  ueLteDevs.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (ueEm));
  ueLteDevs.Get (0)->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&LteRlcAmE2eTestCase::UeDropEvent, this));

  lteSimpleHelper->m_enbRrc->SetArrivalTime (Seconds (0.010));
  lteSimpleHelper->m_enbRrc->SetPduSize (100);

  lteSimpleHelper->m_enbMac->SetTxOppSize (150);
  lteSimpleHelper->m_enbMac->SetTxOppTime (Seconds (0.005));
  lteSimpleHelper->m_enbMac->SetTxOpportunityMode (LteTestMac::RANDOM_MODE);

  Simulator::Schedule (Seconds (0.100), &LteTestRrc::Start, lteSimpleHelper->m_enbRrc);
// FAIL  Simulator::Schedule (Seconds (20.100), &LteTestRrc::Stop, lteSimpleHelper->m_enbRrc);
// FAIL  Simulator::Schedule (Seconds (10.100), &LteTestRrc::Stop, lteSimpleHelper->m_enbRrc);
  Simulator::Schedule (Seconds (1.100), &LteTestRrc::Stop, lteSimpleHelper->m_enbRrc);


  Simulator::Stop (Seconds (2.000));
  Simulator::Run ();

  uint32_t txRrcPdus = lteSimpleHelper->m_enbRrc->GetTxPdus ();
//   uint32_t txMacPdus = lteSimpleHelper->m_enbMac->GetTxPdus ();
//   uint32_t txBytes = lteSimpleHelper->m_enbRrc->GetTxBytes ();
  uint32_t rxRrcPdus = lteSimpleHelper->m_ueRrc->GetRxPdus ();
//   uint32_t rxMacPdus = lteSimpleHelper->m_ueMac->GetRxPdus ();
//   uint32_t rxBytes = lteSimpleHelper->m_ueRrc->GetRxBytes ();


//   NS_LOG_INFO ("Seed = " << m_seed);
//   NS_LOG_INFO ("Losses (%) = " << uint32_t (m_losses * 100));

//   NS_LOG_INFO ("eNB dev drops = " << m_enbDrops);
//   NS_LOG_INFO ("UE dev drops = " << m_ueDrops);
//   NS_LOG_INFO ("eNB dev count = " << m_enbCount);
//   NS_LOG_INFO ("UE dev count = " << m_ueCount);

//   NS_LOG_INFO ("eNB tx RRC count = " << txRrcPdus);
//   NS_LOG_INFO ("eNB tx MAC count = " << txMacPdus);
//   NS_LOG_INFO ("UE rx RRC count = " << rxRrcPdus);
//   NS_LOG_INFO ("UE rx MAC count = " << rxMacPdus);

  NS_LOG_INFO (m_seed << "\t" << m_losses << "\t" << txRrcPdus << "\t" << rxRrcPdus << "\t" << m_ueDrops);

  NS_TEST_ASSERT_MSG_EQ (txRrcPdus, rxRrcPdus,
                         "TX PDUs (" << txRrcPdus << ") != RX PDUs (" << rxRrcPdus << ")");

  Simulator::Destroy ();
}
