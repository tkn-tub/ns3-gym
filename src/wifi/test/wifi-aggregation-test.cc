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
#include "ns3/simulator.h"
#include "ns3/wifi-mac-queue.h"
#include "ns3/mac-low.h"
#include "ns3/edca-txop-n.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/mac-tx-middle.h"
#include "ns3/dcf-manager.h"
#include "ns3/ampdu-tag.h"
#include "ns3/wifi-mac-trailer.h"
#include "ns3/msdu-standard-aggregator.h"
#include "ns3/mpdu-standard-aggregator.h"
#include "ns3/log.h"

using namespace ns3;

class AmpduAggregationTest : public TestCase
{
public:
  AmpduAggregationTest ();

private:
  virtual void DoRun (void);
  Ptr<MacLow> m_low;
  Ptr<YansWifiPhy> m_phy;
  Ptr<EdcaTxopN> m_edca;
  MacTxMiddle *m_txMiddle;
  Ptr<WifiRemoteStationManager> m_manager;
  ObjectFactory m_factory;
  Ptr<MpduAggregator> m_mpduAggregator;
  DcfManager *m_dcfManager;
};

AmpduAggregationTest::AmpduAggregationTest ()
  : TestCase ("Check the correctness of MPDU aggregation operations")
{
}

void
AmpduAggregationTest::DoRun (void)
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
  m_factory.Set ("DataMode", StringValue ("HtMcs7"));
  m_manager = m_factory.Create<WifiRemoteStationManager> ();
  m_manager->SetupPhy (m_phy);
  m_manager->SetHtSupported (true);

  /*
   * Create and configure mac layer.
   */
  m_low = CreateObject<MacLow> ();
  m_low->SetPhy (m_phy);
  m_low->SetWifiRemoteStationManager (m_manager);
  m_low->SetAddress (Mac48Address ("00:00:00:00:00:01"));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);
  m_dcfManager->SetupPhyListener (m_phy);
  m_dcfManager->SetSlot (MicroSeconds (9));

  m_edca = CreateObject<EdcaTxopN> ();
  m_edca->SetLow (m_low);
  m_edca->SetAccessCategory (AC_BE);
  m_edca->SetWifiRemoteStationManager (m_manager);
  m_edca->SetManager (m_dcfManager);
  
  m_txMiddle = new MacTxMiddle ();
  m_edca->SetTxMiddle (m_txMiddle);
  m_edca->CompleteConfig ();

  /*
   * Configure MPDU aggregation.
   */
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::MpduStandardAggregator");
  m_factory.Set ("MaxAmpduSize", UintegerValue (65535));
  m_mpduAggregator = m_factory.Create<MpduAggregator> ();
  m_edca->SetMpduAggregator (m_mpduAggregator);

  /*
   * Create a dummy packet of 1500 bytes and fill mac header fields.
   */
  Ptr<const Packet> pkt = Create<Packet> (1500);
  Ptr<Packet> currentAggregatedPacket = Create<Packet> ();
  WifiMacHeader hdr;
  hdr.SetAddr1 (Mac48Address ("00:00:00:00:00:02"));
  hdr.SetAddr2 (Mac48Address ("00:00:00:00:00:01"));
  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosTid (0);
  uint16_t sequence = m_txMiddle->GetNextSequenceNumberfor (&hdr);
  hdr.SetSequenceNumber (sequence);
  hdr.SetFragmentNumber (0);
  hdr.SetNoMoreFragments ();
  hdr.SetNoRetry ();

  /*
   * Establish agreement.
   */
  MgtAddBaRequestHeader reqHdr;
  reqHdr.SetImmediateBlockAck ();
  reqHdr.SetTid (0);
  reqHdr.SetBufferSize (0);
  reqHdr.SetTimeout (0);
  reqHdr.SetStartingSequence (0);
  m_edca->m_baManager->CreateAgreement (&reqHdr, hdr.GetAddr1 ());

  //-----------------------------------------------------------------------------------------------------

  /*
   * Test behavior when no other packets are in the queue
   */
  m_low->m_currentHdr = hdr;
  m_low->m_currentPacket = pkt->Copy();
  bool isAmpdu = m_low->IsAmpdu (pkt, hdr);
  NS_TEST_EXPECT_MSG_EQ (isAmpdu, false, "a single packet should not result in an A-MPDU");
  NS_TEST_EXPECT_MSG_EQ (m_low->m_aggregateQueue->GetSize (), 0, "aggregation queue is not flushed");

  //-----------------------------------------------------------------------------------------------------

  /*
   * Test behavior when 2 more packets are in the queue
   */
  Ptr<const Packet> pkt1 = Create<Packet> (1500);
  Ptr<const Packet> pkt2 = Create<Packet> (1500);
  WifiMacHeader hdr1, hdr2;

  hdr1.SetAddr1 (Mac48Address ("00:00:00:00:00:02"));
  hdr1.SetAddr2 (Mac48Address ("00:00:00:00:00:01"));
  hdr1.SetType (WIFI_MAC_QOSDATA);
  hdr1.SetQosTid (0);

  hdr2.SetAddr1 (Mac48Address ("00:00:00:00:00:02"));
  hdr2.SetAddr2 (Mac48Address ("00:00:00:00:00:01"));
  hdr2.SetType (WIFI_MAC_QOSDATA);
  hdr2.SetQosTid (0);

  m_edca->GetEdcaQueue ()->Enqueue (pkt1, hdr1);
  m_edca->GetEdcaQueue ()->Enqueue (pkt2, hdr2);

  isAmpdu = m_low->IsAmpdu (pkt, hdr);
  uint32_t aggregationQueueSize = m_low->m_aggregateQueue->GetSize ();
  NS_TEST_EXPECT_MSG_EQ (isAmpdu, true, "MPDU aggregation failed");
  NS_TEST_EXPECT_MSG_EQ (m_low->m_currentPacket->GetSize (), 4606, "A-MPDU size is not correct");
  NS_TEST_EXPECT_MSG_EQ (aggregationQueueSize, 3, "aggregation queue should not be empty");
  NS_TEST_EXPECT_MSG_EQ (m_edca->GetEdcaQueue ()->GetSize (), 0, "queue should be empty");

  Ptr <const Packet> dequeuedPacket;
  WifiMacHeader dequeuedHdr;
  uint32_t i = 0;
  for (; aggregationQueueSize > 0; aggregationQueueSize--, i++)
  {
    dequeuedPacket = m_low->m_aggregateQueue->Dequeue (&dequeuedHdr);
    NS_TEST_EXPECT_MSG_EQ (dequeuedHdr.GetSequenceNumber (), i, "wrong sequence number");
  }
  NS_TEST_EXPECT_MSG_EQ (aggregationQueueSize, 0, "aggregation queue should be empty");
  
  //-----------------------------------------------------------------------------------------------------

  /*
   * Test behavior when the 802.11n station and another non-QoS station are associated to the AP.
   * The AP sends an A-MPDU to the 802.11n station followed by the last retransmission of a non-QoS data frame to the non-QoS station.
   * This is used to reproduce bug 2224.
   */
  pkt1 = Create<Packet> (1500);
  pkt2 = Create<Packet> (1500);
  hdr1.SetAddr1 (Mac48Address ("00:00:00:00:00:02"));
  hdr1.SetAddr2 (Mac48Address ("00:00:00:00:00:01"));
  hdr1.SetType (WIFI_MAC_QOSDATA);
  hdr1.SetQosTid (0);
  hdr1.SetSequenceNumber (3);
  hdr2.SetAddr1 (Mac48Address ("00:00:00:00:00:03"));
  hdr2.SetAddr2 (Mac48Address ("00:00:00:00:00:01"));
  hdr2.SetType (WIFI_MAC_DATA);
  hdr2.SetQosTid (0);

  Ptr<const Packet> pkt3 = Create<Packet> (1500);
  WifiMacHeader hdr3;
  hdr3.SetSequenceNumber (0);
  hdr3.SetAddr1 (Mac48Address ("00:00:00:00:00:03"));
  hdr3.SetAddr2 (Mac48Address ("00:00:00:00:00:01"));
  hdr3.SetType (WIFI_MAC_DATA);
  hdr3.SetQosTid (0);

  m_edca->GetEdcaQueue ()->Enqueue (pkt3, hdr3);
  
  isAmpdu = m_low->IsAmpdu (pkt1, hdr1);
  NS_TEST_EXPECT_MSG_EQ (isAmpdu, false, "a single packet for this destination should not result in an A-MPDU");
  NS_TEST_EXPECT_MSG_EQ (m_low->m_aggregateQueue->GetSize (), 0, "aggregation queue is not flushed");
  
  m_edca->m_currentHdr = hdr2;
  m_edca->m_currentPacket = pkt2->Copy ();
  isAmpdu = m_low->IsAmpdu (pkt2, hdr2);
  NS_TEST_EXPECT_MSG_EQ (isAmpdu, false, "no MPDU aggregation should be performed if there is no agreement");
  NS_TEST_EXPECT_MSG_EQ (m_low->m_aggregateQueue->GetSize (), 0, "aggregation queue is not flushed");
  
  m_manager->SetMaxSlrc (0); //set to 0 in order to fake that the maximum number of retries has been reached
  m_edca->MissedAck();
  
  NS_TEST_EXPECT_MSG_EQ (m_edca->m_currentPacket, 0, "packet should be discarded");
  m_edca->GetEdcaQueue ()->Remove (pkt3);

  Simulator::Destroy ();
}


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
  Ptr<MsduAggregator> m_msduAggregator;
  Ptr<MpduAggregator> m_mpduAggregator;
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
  m_factory.Set ("DataMode", StringValue ("HtMcs7"));
  m_manager = m_factory.Create<WifiRemoteStationManager> ();
  m_manager->SetupPhy (m_phy);

  /*
   * Create and configure maclayer.
   */
  m_low = CreateObject<MacLow> ();
  m_low->SetPhy (m_phy);
  m_low->SetWifiRemoteStationManager (m_manager);

  m_edca = CreateObject<EdcaTxopN> ();
  m_edca->SetLow (m_low);
  m_edca->SetAccessCategory (AC_BE);
  m_edca->SetWifiRemoteStationManager (m_manager);
  m_edca->CompleteConfig ();

  /*
   * Configure aggregation.
   */
  m_msduAggregator = CreateObject<MsduStandardAggregator> ();
  m_mpduAggregator = CreateObject<MpduStandardAggregator> ();
  
  m_msduAggregator->SetMaxAmsduSize (4095);
  m_mpduAggregator->SetMaxAmpduSize (65535);

  m_edca->SetMsduAggregator (m_msduAggregator);
  m_edca->SetMpduAggregator (m_mpduAggregator);

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
  m_edca->GetEdcaQueue ()->Enqueue (pkt, hdr);
  m_edca->GetEdcaQueue ()->Enqueue (pkt, hdr);

  Ptr<const Packet> peekedPacket = m_edca->GetEdcaQueue ()->PeekByTidAndAddress (&peekedHdr, 0,
                                                                                 WifiMacHeader::ADDR1,
                                                                                 hdr.GetAddr1 (),
                                                                                 &tstamp);
  m_low->m_currentPacket = peekedPacket->Copy ();
  m_low->m_currentHdr = peekedHdr;

  Ptr<Packet> packet = m_low->PerformMsduAggregation (peekedPacket, &peekedHdr, &tstamp, currentAggregatedPacket, 0);

  bool result = (packet != 0);
  NS_TEST_EXPECT_MSG_EQ (result, true, "aggregation failed");
  NS_TEST_EXPECT_MSG_EQ (packet->GetSize (), 3030, "wrong packet size");
  NS_TEST_EXPECT_MSG_EQ (m_edca->GetEdcaQueue ()->GetSize (), 0, "aggregated packets not removed from the queue");

  //-----------------------------------------------------------------------------------------------------

  /*
   * Aggregation is refused when the maximum size is reached.
   * It checks whether MSDU aggregation has been rejected because the maximum MPDU size is set to 0 (returned packet should be equal to 0).
   * This test is needed to ensure that no packets are removed from the queue in MacLow::PerformMsduAggregation, since aggregation will no occur in MacLow::AggregateToAmpdu.
   */
  m_factory = ObjectFactory ();
  m_factory.SetTypeId ("ns3::MpduStandardAggregator");
  m_factory.Set ("MaxAmpduSize", UintegerValue (0));
  m_mpduAggregator = m_factory.Create<MpduAggregator> ();
  m_edca->SetMpduAggregator (m_mpduAggregator);

  m_edca->GetEdcaQueue ()->Enqueue (pkt, hdr);
  packet = m_low->PerformMsduAggregation (peekedPacket, &peekedHdr, &tstamp, currentAggregatedPacket, 0);

  result = (packet != 0);
  NS_TEST_EXPECT_MSG_EQ (result, false, "maximum aggregated frame size check failed");

  //-----------------------------------------------------------------------------------------------------

  /*
   * Aggregation does not occur zhen there is no more packets in the queue.
   * It checks whether MSDU aggregation has been rejected because there is no packets ready in the queue (returned packet should be equal to 0).
   * This test is needed to ensure that there is no issue when the queue is empty.
   */
  m_mpduAggregator->SetMaxAmpduSize (4095);

  m_edca->GetEdcaQueue ()->Remove (pkt);
  m_edca->GetEdcaQueue ()->Remove (pkt);
  packet = m_low->PerformMsduAggregation (peekedPacket, &peekedHdr, &tstamp, currentAggregatedPacket, 0);

  result = (packet != 0);
  NS_TEST_EXPECT_MSG_EQ (result, false, "aggregation failed to stop as queue is empty");
  Simulator::Destroy ();
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
  AddTestCase (new AmpduAggregationTest, TestCase::QUICK);
  AddTestCase (new TwoLevelAggregationTest, TestCase::QUICK);
}

static WifiAggregationTestSuite g_wifiAggregationTestSuite;
