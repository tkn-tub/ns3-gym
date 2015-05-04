/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/string.h"
#include "ns3/test.h"
#include "ns3/object-factory.h"
#include "ns3/wifi-mac-queue.h"
#include "ns3/mac-low.h"
#include "ns3/edca-txop-n.h"
#include "ns3/yans-wifi-phy.h"

using namespace ns3;

class TwoLevelAggregationTest : public TestCase
{
public:
  TwoLevelAggregationTest ();

private:
  virtual void DoRun (void);
  Ptr<MacLow> m_low;
  Ptr<YansWifiPhy> m_phy;
  Ptr<EdcaTxopN> m_edca;
  Ptr<WifiRemoteStationManager> m_manager;
  ObjectFactory m_factory;
  Ptr<MpduAggregator> m_mpduAggregator;
  Ptr<MsduAggregator> m_msduAggregator;
};

TwoLevelAggregationTest::TwoLevelAggregationTest ()
  : TestCase ("Check the correctness of two-level aggregation operations")
{
}

void
TwoLevelAggregationTest::DoRun (void)
{
  /*
   * Create and configure phy layer.
   */
  m_phy = CreateObject<YansWifiPhy> ();
  m_phy->ConfigureStandard (WIFI_PHY_STANDARD_80211n_5GHZ);

  /*
   * Create and configure manager.
   */
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::ConstantRateWifiManager");
  m_factory.Set ("DataMode", StringValue ("OfdmRate65MbpsBW20MHz"));
  m_manager = m_factory.Create<WifiRemoteStationManager> ();
  m_manager->SetupPhy(m_phy);

  /*
   * Create and configure maclayer.
   */
  m_low = CreateObject<MacLow> ();
  m_low->SetPhy(m_phy);
  m_low->SetWifiRemoteStationManager (m_manager);

  m_edca = CreateObject<EdcaTxopN> ();
  m_edca->SetLow (m_low);
  m_edca->SetAccessCategory (AC_BE);
  m_edca->SetWifiRemoteStationManager (m_manager);
  m_edca->CompleteConfig ();

  /*
   * Configure aggregation.
   */
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::MsduStandardAggregator");
  m_factory.Set ("MaxAmsduSize", UintegerValue(4095));
  m_msduAggregator = m_factory.Create<MsduAggregator> ();
  m_edca->SetMsduAggregator (m_msduAggregator);
  
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::MpduStandardAggregator");
  m_factory.Set ("MaxAmpduSize", UintegerValue(65535));
  m_mpduAggregator = m_factory.Create<MpduAggregator> ();
  m_low->SetMpduAggregator (m_mpduAggregator);
  
  /*
   * Create dummy packets of 1500 bytes and fill mac header fields that will be used for the tests.
   */
  Ptr<const Packet> pkt = Create<Packet> (1500);
  Ptr<Packet> currentAggregatedPacket = Create<Packet> ();
  WifiMacHeader hdr, peekedHdr;
  hdr.SetAddr1 (Mac48Address ("00:00:00:00:00:01"));
  hdr.SetAddr2 (Mac48Address ("00:00:00:00:00:02"));
  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosTid (0);
  Time tstamp;

  //-----------------------------------------------------------------------------------------------------
  
  /*
   * Test MSDU aggregation of two packets using MacLow::PerformMsduAggregation.
   * It checks whether aggregation succeeded: 
   *      - returned packet should be different from 0;
   *      - A-MSDU frame size should be 3030 bytes (= 2 packets + headers + padding);
   *      - one packet should be removed from the queue (the other packet is removed later in MacLow::AggregateToAmpdu) .
   */
  m_edca->GetEdcaQueue()->Enqueue(pkt, hdr);
  m_edca->GetEdcaQueue()->Enqueue(pkt, hdr);
    
  Ptr<const Packet> peekedPacket = m_edca->GetEdcaQueue()->PeekByTidAndAddress (&peekedHdr, 0,
                                                                                WifiMacHeader::ADDR1,
                                                                                hdr.GetAddr1 (),
                                                                                &tstamp);
  m_low->m_currentPacket = peekedPacket->Copy ();
  m_low->m_currentHdr = peekedHdr;
  
  Ptr<Packet> packet = m_low->PerformMsduAggregation(peekedPacket, &peekedHdr, &tstamp, currentAggregatedPacket, 0);
    
  bool result = (packet != 0);
  NS_TEST_EXPECT_MSG_EQ (result, true, "aggregation failed");
  NS_TEST_EXPECT_MSG_EQ (packet->GetSize(), 3030, "wrong packet size");
  NS_TEST_EXPECT_MSG_EQ (m_edca->GetEdcaQueue()->GetSize(), 1, "removing packet from EDCA queue failed");

  //-----------------------------------------------------------------------------------------------------
    
  /*
   * Aggregation is refused when the maximum size is reached.
   * It checks whether MSDU aggregation has been rejected because the maximum MPDU size is set to 0 (returned packet should be equal to 0).
   * This test is needed to ensure that no packets are removed from the queue in MacLow::PerformMsduAggregation, since aggregation will no occur in MacLow::AggregateToAmpdu.
   */
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::MpduStandardAggregator");
  m_factory.Set ("MaxAmpduSize", UintegerValue(0));
  m_mpduAggregator = m_factory.Create<MpduAggregator> ();
  m_low->SetMpduAggregator (m_mpduAggregator);
  
  m_edca->GetEdcaQueue()->Enqueue(pkt, hdr);
  packet = m_low->PerformMsduAggregation(peekedPacket, &peekedHdr, &tstamp, currentAggregatedPacket, 0);
  
  result = (packet != 0);
  NS_TEST_EXPECT_MSG_EQ (result, false, "maximum aggregated frame size check failed");

  //-----------------------------------------------------------------------------------------------------
  
  /*
   * Aggregation does not occur zhen there is no more packets in the queue.
   * It checks whether MSDU aggregation has been rejected because there is no packets ready in the queue (returned packet should be equal to 0).
   * This test is needed to ensure that there is no issue when the queue is empty.
   */
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::MpduStandardAggregator");
  m_factory.Set ("MaxAmpduSize", UintegerValue(4095));
  m_mpduAggregator = m_factory.Create<MpduAggregator> ();
  m_low->SetMpduAggregator (m_mpduAggregator);
    
  m_edca->GetEdcaQueue()->Remove(pkt);
  m_edca->GetEdcaQueue()->Remove(pkt);
  packet = m_low->PerformMsduAggregation(peekedPacket, &peekedHdr, &tstamp, currentAggregatedPacket, 0);
  
  result = (packet != 0);
  NS_TEST_EXPECT_MSG_EQ (result, false, "aggregation failed to stop as queue is empty");
}

//-----------------------------------------------------------------------------
class WifiAggregationTestSuite : public TestSuite
{
public:
  WifiAggregationTestSuite ();
};

WifiAggregationTestSuite::WifiAggregationTestSuite ()
  : TestSuite ("aggregation-wifi", UNIT)
{
  AddTestCase (new TwoLevelAggregationTest, TestCase::QUICK);
}

static WifiAggregationTestSuite g_wifiAggregationTestSuite;
