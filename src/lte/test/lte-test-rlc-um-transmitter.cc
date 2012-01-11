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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/simulator.h"
#include "ns3/log.h"

#include "ns3/lte-rlc-header.h"
#include "ns3/lte-rlc-um.h"

#include "ns3/lte-test-rlc-um-transmitter.h"

NS_LOG_COMPONENT_DEFINE ("LteRlcUmTransmitterTest");

using namespace ns3;


/**
 * TestSuite 4.1.1 RLC UM: Only transmitter
 */

LteRlcUmTransmitterTestSuite::LteRlcUmTransmitterTestSuite ()
  : TestSuite ("lte-rlc-um-transmitter", SYSTEM)
{
  // LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  // LogComponentEnable ("LteRlcUmTransmitterTest", logLevel);

  NS_LOG_INFO ("Creating LteRlcUmTransmitterTestSuite");

  AddTestCase (new LteRlcUmTransmitterOneSduTestCase ("One SDU, one PDU"));
  AddTestCase (new LteRlcUmTransmitterSegmentationTestCase ("Segmentation"));
  AddTestCase (new LteRlcUmTransmitterConcatenationTestCase ("Concatenation"));
  AddTestCase (new LteRlcUmTransmitterReportBufferStatusTestCase ("ReportBufferStatus primitive"));

}

static LteRlcUmTransmitterTestSuite lteRlcUmTransmitterTestSuite;


LteRlcUmTransmitterTestCase::LteRlcUmTransmitterTestCase (std::string name)
  : TestCase (name)
{
  // NS_LOG_UNCOND ("Creating LteRlcUmTransmitterTestCase: " + name);
}

LteRlcUmTransmitterTestCase::~LteRlcUmTransmitterTestCase ()
{
}

void
LteRlcUmTransmitterTestCase::DoRun (void)
{
  // LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  // LogComponentEnable ("LteRlcUmTransmitterTest", logLevel);
  // LogComponentEnable ("LteTestEntities", logLevel);
  // LogComponentEnable ("LteRlc", logLevel);
  // LogComponentEnable ("LteRlcUm", logLevel);
  // LogComponentEnable ("LteRlcHeader", logLevel);

  uint16_t rnti = 1111;
  uint8_t lcid = 222;

  Packet::EnablePrinting ();

  // Create topology

  // Create transmission PDCP test entity
  txPdcp = CreateObject<LteTestPdcp> ();

  // Create transmission RLC entity
  txRlc = CreateObject<LteRlcUm> ();
  txRlc->SetRnti (rnti);
  txRlc->SetLcId (lcid);

  // Create transmission MAC test entity
  txMac = CreateObject<LteTestMac> ();
  txMac->SetRlcHeaderType (LteTestMac::UM_RLC_HEADER);

  // Connect SAPs: PDCP (TX) <-> RLC (Tx) <-> MAC (Tx)
  txPdcp->SetLteRlcSapProvider (txRlc->GetLteRlcSapProvider ());
  txRlc->SetLteRlcSapUser (txPdcp->GetLteRlcSapUser ());

  txRlc->SetLteMacSapProvider (txMac->GetLteMacSapProvider ());
  txMac->SetLteMacSapUser (txRlc->GetLteMacSapUser ());

}

/**
 * Test 4.1.1.1 One SDU, One PDU
 */
LteRlcUmTransmitterOneSduTestCase::LteRlcUmTransmitterOneSduTestCase (std::string name)
  : LteRlcUmTransmitterTestCase (name)
{
}

LteRlcUmTransmitterOneSduTestCase::~LteRlcUmTransmitterOneSduTestCase ()
{
}

void
LteRlcUmTransmitterOneSduTestCase::DoRun (void)
{
  // Create topology
  LteRlcUmTransmitterTestCase::DoRun ();

  // The tests...
  std::string dataToSend;
  std::string dataReceived;

  //
  // a) One SDU generates one PDU
  //

  // PDCP entity sends data
  dataToSend = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  txPdcp->SendData (Seconds (0.0), dataToSend);

  txMac->SendTxOpportunity (Seconds (0.1), 28);
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", txMac->GetDataReceived (), "SDU is not OK");

  Simulator::Destroy ();
}

/**
 * Test 4.1.1.2 Segmentation (One SDU => n PDUs)
 */
LteRlcUmTransmitterSegmentationTestCase::LteRlcUmTransmitterSegmentationTestCase (std::string name)
  : LteRlcUmTransmitterTestCase (name)
{
}

LteRlcUmTransmitterSegmentationTestCase::~LteRlcUmTransmitterSegmentationTestCase ()
{
}

void
LteRlcUmTransmitterSegmentationTestCase::DoRun (void)
{
  // Create topology
  LteRlcUmTransmitterTestCase::DoRun ();

  // The tests...
  std::string dataToSend;
  std::string dataReceived;

  //
  // b) Segmentation: one SDU generates n PDUs
  //

  // PDCP entity sends data
  dataToSend = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  txPdcp->SendData (Seconds (0.0), dataToSend);

  txMac->SendTxOpportunity (Seconds (0.1), 10);
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGH", txMac->GetDataReceived (), "Segmentation is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), 10);
  NS_TEST_ASSERT_MSG_EQ ("IJKLMNOP", txMac->GetDataReceived (), "2 Segmentation is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), 10);
  NS_TEST_ASSERT_MSG_EQ ("QRSTUVWX", txMac->GetDataReceived (), "3 Segmentation is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), 4);
  NS_TEST_ASSERT_MSG_EQ ("YZ", txMac->GetDataReceived (), "4 Segmentation is not OK");

  Simulator::Destroy ();
}

/**
 * Test 4.1.1.3 Concatenation (n SDUs => One PDU)
 */
LteRlcUmTransmitterConcatenationTestCase::LteRlcUmTransmitterConcatenationTestCase (std::string name)
  : LteRlcUmTransmitterTestCase (name)
{
}

LteRlcUmTransmitterConcatenationTestCase::~LteRlcUmTransmitterConcatenationTestCase ()
{
}

void
LteRlcUmTransmitterConcatenationTestCase::DoRun (void)
{
  // Create topology
  LteRlcUmTransmitterTestCase::DoRun ();

  // The tests...
  std::string dataToSend;
  std::string dataReceived;

  //
  // c) Concatenation: n SDUs generate one PDU
  //

  // PDCP entity sends data
  dataToSend = "ABCDEFGH";
  txPdcp->SendData (Seconds (0.0), dataToSend);

  dataToSend = "IJKLMNOPQR";
  txPdcp->SendData (Seconds (0.0), dataToSend);

  dataToSend = "STUVWXYZ";
  txPdcp->SendData (Seconds (0.0), dataToSend);

  txMac->SendTxOpportunity (Seconds (0.1), 31);
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", txMac->GetDataReceived (), "Concatenation is not OK");

  Simulator::Destroy ();
}

/**
 * Test 4.1.1.4 Report Buffer Status (test primitive parameters)
 */
LteRlcUmTransmitterReportBufferStatusTestCase::LteRlcUmTransmitterReportBufferStatusTestCase (std::string name)
  : LteRlcUmTransmitterTestCase (name)
{
}

LteRlcUmTransmitterReportBufferStatusTestCase::~LteRlcUmTransmitterReportBufferStatusTestCase ()
{
}

void
LteRlcUmTransmitterReportBufferStatusTestCase::DoRun (void)
{
  // Create topology
  LteRlcUmTransmitterTestCase::DoRun ();

  // The tests...
  std::string dataToSend;
  std::string dataReceived;

  //
  // d) Test the parameters of the ReportBufferStatus primitive
  //

  txMac->SendTxOpportunity (Seconds (0.1), (2+2) + (10+6));

  // PDCP entity sends data
  txPdcp->SendData (Seconds (1.0), "ABCDEFGHIJ"); // 10
  txPdcp->SendData (Seconds (1.0), "KLMNOPQRS");  // 9
  txPdcp->SendData (Seconds (1.0), "TUVWXYZ");    // 7

  txMac->SendTxOpportunity (Seconds (0.1), (2+2) + (10+6));
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGHIJKLMNOP",
                         txMac->GetDataReceived (), "SDU is not OK");

  txPdcp->SendData (Seconds (1.0), "ABCDEFGH");     // 8
  txPdcp->SendData (Seconds (1.0), "IJKLMNOPQRST"); // 12
  txPdcp->SendData (Seconds (1.0), "UVWXYZ");       // 6

  txMac->SendTxOpportunity (Seconds (0.1), 2 + 3);
  NS_TEST_ASSERT_MSG_EQ ("QRS",
                         txMac->GetDataReceived (), "SDU is not OK");

  txPdcp->SendData (Seconds (1.0), "ABCDEFGH");     // 8
  txPdcp->SendData (Seconds (1.0), "IJKLMNOPQRST"); // 12
  txPdcp->SendData (Seconds (1.0), "UVWXYZ");       // 6

  txPdcp->SendData (Seconds (1.0), "ABCDEFGHIJ");   // 10
  txPdcp->SendData (Seconds (1.0), "KLMNOPQRST");   // 10
  txPdcp->SendData (Seconds (1.0), "UVWXYZ");       // 6

  txMac->SendTxOpportunity (Seconds (0.1), 2 + 7);
  NS_TEST_ASSERT_MSG_EQ ("TUVWXYZ",
                         txMac->GetDataReceived (), "SDU is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), (2+2) + (8+2));
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGHIJ",
                         txMac->GetDataReceived (), "SDU is not OK");

  txPdcp->SendData (Seconds (1.0), "ABCDEFGHIJ");   // 10
  txPdcp->SendData (Seconds (1.0), "KLMNOPQRST");   // 10
  txPdcp->SendData (Seconds (1.0), "UVWXYZ");       // 6

  txMac->SendTxOpportunity (Seconds (0.1), 2 + 2);
  NS_TEST_ASSERT_MSG_EQ ("KL",
                         txMac->GetDataReceived (), "SDU is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), 2 + 3);
  NS_TEST_ASSERT_MSG_EQ ("MNO",
                         txMac->GetDataReceived (), "SDU is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), 2 + 5);
  NS_TEST_ASSERT_MSG_EQ ("PQRST",
                         txMac->GetDataReceived (), "SDU is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), (2+2+1+2+1+2+1) + (6+8+12+6+10+10+3));
  NS_TEST_ASSERT_MSG_EQ ("UVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW",
                         txMac->GetDataReceived (), "SDU is not OK");

  txMac->SendTxOpportunity (Seconds (0.1), (2+2+1+2) + (3+10+10+6));
  NS_TEST_ASSERT_MSG_EQ ("XYZABCDEFGHIJKLMNOPQRSTUVWXYZ",
                         txMac->GetDataReceived (), "SDU is not OK");

  Simulator::Destroy ();
}

