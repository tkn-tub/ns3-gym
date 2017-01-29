/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Fraunhofer FKIE
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
 * Author:
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 */

#include <ns3/log.h>
#include <ns3/core-module.h>
#include <ns3/lr-wpan-module.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/simulator.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/packet.h>
#include "ns3/rng-seed-manager.h"

#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("lr-wpan-ack-test");

/**
 * \ingroup lr-wpan
 * \defgroup lr-wpan-test LrWpan module tests
 */

/**
 * \ingroup lr-wpan-test
 * \ingroup tests
 *
 * \brief LrWpan ACK Test
 */
class LrWpanAckTestCase : public TestCase
{
public:
  LrWpanAckTestCase ();

  /**
   * \brief Function called when DataIndication is hit.
   * \param testCase The TestCase.
   * \param dev The LrWpanNetDevice.
   * \param params The MCPS params.
   * \param p the packet.
   */
  static void DataIndication (LrWpanAckTestCase *testCase, Ptr<LrWpanNetDevice> dev, McpsDataIndicationParams params, Ptr<Packet> p);
  /**
   * \brief Function called when DataConfirm is hit.
   * \param testCase The TestCase.
   * \param dev The LrWpanNetDevice.
   * \param params The MCPS params.
   */
  static void DataConfirm (LrWpanAckTestCase *testCase, Ptr<LrWpanNetDevice> dev, McpsDataConfirmParams params);

private:
  virtual void DoRun (void);

  Time m_requestTime; //!< Request time.
  Time m_requestAckTime; //!< Request ack time.
  Time m_replyTime; //!< Reply time.
  Time m_replyAckTime; //!< Reply ack time.
  Time m_replyArrivalTime; //!< Reply arrival time.
};

LrWpanAckTestCase::LrWpanAckTestCase ()
  : TestCase ("Test the 802.15.4 ACK handling")
{
  m_requestTime = Seconds (0);
  m_requestAckTime = Seconds (0);
  m_replyTime = Seconds (0);
  m_replyAckTime = Seconds (0);
  m_replyArrivalTime = Seconds (0);
}

void
LrWpanAckTestCase::DataIndication (LrWpanAckTestCase *testCase, Ptr<LrWpanNetDevice> dev, McpsDataIndicationParams params, Ptr<Packet> p)
{
  if (dev->GetAddress () == Mac16Address ("00:02"))
    {
      Ptr<Packet> p = Create<Packet> (10);  // 10 bytes of dummy data
      McpsDataRequestParams params;
      params.m_srcAddrMode = SHORT_ADDR;
      params.m_dstAddrMode = SHORT_ADDR;
      params.m_dstPanId = 0;
      params.m_dstAddr = Mac16Address ("00:01");
      params.m_msduHandle = 0;
      params.m_txOptions = TX_OPTION_NONE;

      testCase->m_replyTime = Simulator::Now ();
      dev->GetMac ()->McpsDataRequest (params, p);
    }
  else
    {
      testCase->m_replyArrivalTime = Simulator::Now ();
    }
}

void
LrWpanAckTestCase::DataConfirm (LrWpanAckTestCase *testCase, Ptr<LrWpanNetDevice> dev, McpsDataConfirmParams params)
{
  if (dev->GetAddress () == Mac16Address ("00:01"))
    {
      testCase->m_requestAckTime = Simulator::Now ();
    }
  else
    {
      testCase->m_replyAckTime = Simulator::Now ();
    }
}

void
LrWpanAckTestCase::DoRun (void)
{
  // Test setup:
  // Two nodes well in communication range.
  // Node 1 sends a request packet to node 2 with ACK request bit set. Node 2
  // immediately answers with a reply packet on reception of the request.
  // We expect the ACK of the request packet to always arrive at node 1 before
  // the reply packet sent by node 2.

  // Enable calculation of FCS in the trailers. Only necessary when interacting with real devices or wireshark.
  // GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  // Set the random seed and run number for this test
  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (6);

  // Create 2 nodes, and a NetDevice for each one
  Ptr<Node> n0 = CreateObject <Node> ();
  Ptr<Node> n1 = CreateObject <Node> ();

  Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice> ();

  // Make random variable stream assignment deterministic
  dev0->AssignStreams (0);
  dev1->AssignStreams (10);

  dev0->SetAddress (Mac16Address ("00:01"));
  dev1->SetAddress (Mac16Address ("00:02"));

  // Each device must be attached to the same channel
  Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<LogDistancePropagationLossModel> propModel = CreateObject<LogDistancePropagationLossModel> ();
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  channel->AddPropagationLossModel (propModel);
  channel->SetPropagationDelayModel (delayModel);

  dev0->SetChannel (channel);
  dev1->SetChannel (channel);

  // To complete configuration, a LrWpanNetDevice must be added to a node
  n0->AddDevice (dev0);
  n1->AddDevice (dev1);

  Ptr<ConstantPositionMobilityModel> sender0Mobility = CreateObject<ConstantPositionMobilityModel> ();
  sender0Mobility->SetPosition (Vector (0,0,0));
  dev0->GetPhy ()->SetMobility (sender0Mobility);
  Ptr<ConstantPositionMobilityModel> sender1Mobility = CreateObject<ConstantPositionMobilityModel> ();
  // Configure position 10 m distance
  sender1Mobility->SetPosition (Vector (0,10,0));
  dev1->GetPhy ()->SetMobility (sender1Mobility);

  McpsDataConfirmCallback cb0;
  cb0 = MakeBoundCallback (&LrWpanAckTestCase::DataConfirm, this, dev0);
  dev0->GetMac ()->SetMcpsDataConfirmCallback (cb0);

  McpsDataIndicationCallback cb1;
  cb1 = MakeBoundCallback (&LrWpanAckTestCase::DataIndication, this, dev0);
  dev0->GetMac ()->SetMcpsDataIndicationCallback (cb1);

  McpsDataConfirmCallback cb2;
  cb2 = MakeBoundCallback (&LrWpanAckTestCase::DataConfirm, this, dev1);
  dev1->GetMac ()->SetMcpsDataConfirmCallback (cb2);

  McpsDataIndicationCallback cb3;
  cb3 = MakeBoundCallback (&LrWpanAckTestCase::DataIndication, this, dev1);
  dev1->GetMac ()->SetMcpsDataIndicationCallback (cb3);

  Ptr<Packet> p0 = Create<Packet> (50);  // 50 bytes of dummy data
  McpsDataRequestParams params;
  params.m_srcAddrMode = SHORT_ADDR;
  params.m_dstAddrMode = SHORT_ADDR;
  params.m_dstPanId = 0;
  params.m_dstAddr = Mac16Address ("00:02");
  params.m_msduHandle = 0;
  params.m_txOptions = TX_OPTION_ACK;
  m_requestTime = Simulator::Now ();
  Simulator::ScheduleNow (&LrWpanMac::McpsDataRequest, dev0->GetMac (), params, p0);


  Simulator::Run ();

  NS_TEST_EXPECT_MSG_LT (m_requestTime, m_replyTime, "Sent the request before the reply (as expected)");
  NS_TEST_EXPECT_MSG_LT (m_requestAckTime, m_replyArrivalTime, "The request was ACKed before the reply arrived (as expected)");
  NS_TEST_EXPECT_MSG_LT (m_replyAckTime, m_replyArrivalTime, "The reply was ACKed before the reply arrived (as expected)");

  Simulator::Destroy ();
}

/**
 * \ingroup lr-wpan-test
 * \ingroup tests
 *
 * \brief LrWpan ACK TestSuite
 */
class LrWpanAckTestSuite : public TestSuite
{
public:
  LrWpanAckTestSuite ();
};

LrWpanAckTestSuite::LrWpanAckTestSuite ()
  : TestSuite ("lr-wpan-ack", UNIT)
{
  AddTestCase (new LrWpanAckTestCase, TestCase::QUICK);
}

static LrWpanAckTestSuite g_lrWpanAckTestSuite; //!< Static variable for test initialization
