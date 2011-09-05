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
#include "ns3/string.h"

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
  LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlcUmTransmitterTest", logLevel);

  NS_LOG_INFO ("Creating LteRlcUmTransmitterTestSuite");

  AddTestCase (new LteRlcUmTransmitterOneSduTestCase ("One SDU, one PDU"));
  AddTestCase (new LteRlcUmTransmitterSegmentationTestCase ("Segmentation"));
  AddTestCase (new LteRlcUmTransmitterConcatenationTestCase ("Concatenation"));

}

static LteRlcUmTransmitterTestSuite lteRlcUmTransmitterTestSuite;


LteRlcUmTransmitterTestCase::LteRlcUmTransmitterTestCase (std::string name)
  : TestCase (name)
{
  NS_LOG_UNCOND ("Creating LteRlcUmTransmitterTestCase: " + name);
}

LteRlcUmTransmitterTestCase::~LteRlcUmTransmitterTestCase ()
{
}

void
LteRlcUmTransmitterTestCase::DoRun (void)
{
  LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlcUmTransmitterTest", logLevel);
  LogComponentEnable ("LteRlc", logLevel);
  LogComponentEnable ("LteRlcUm", logLevel);
  LogComponentEnable ("LteRlcHeader", logLevel);

  uint16_t rnti = 1111;
  uint16_t lcid = 2222;

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

  txMac->SendTxOpportunity (28);
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

  txMac->SendTxOpportunity (10);
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGH", txMac->GetDataReceived (), "Segmentation is not OK");

  txMac->SendTxOpportunity (10);
  NS_TEST_ASSERT_MSG_EQ ("IJKLMNOP", txMac->GetDataReceived (), "2 Segmentation is not OK");

  txMac->SendTxOpportunity (10);
  NS_TEST_ASSERT_MSG_EQ ("QRSTUVWX", txMac->GetDataReceived (), "3 Segmentation is not OK");

  txMac->SendTxOpportunity (4);
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

  txMac->SendTxOpportunity (31);
  NS_TEST_ASSERT_MSG_EQ ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", txMac->GetDataReceived (), "Concatenation is not OK");

  Simulator::Destroy ();
}

/////////////////////////////////////////////////////////////////////

TypeId
LteTestMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteTestMac")
    .SetParent<Object> ()
    .AddConstructor<LteTestMac> ()
    ;

  return tid;
}

LteTestMac::LteTestMac ()
{
  NS_LOG_FUNCTION (this);
  m_macSapProvider = new EnbMacMemberLteMacSapProvider<LteTestMac> (this);
//   m_cmacSapProvider = new EnbMacMemberLteEnbCmacSapProvider (this);
//   m_schedSapUser = new EnbMacMemberFfMacSchedSapUser (this);
//   m_cschedSapUser = new EnbMacMemberFfMacCschedSapUser (this);
//   m_enbPhySapUser = new EnbMacMemberLteEnbPhySapUser (this);
}

LteTestMac::~LteTestMac ()
{
  NS_LOG_FUNCTION (this);
}

void
LteTestMac::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_macSapProvider;
//   delete m_cmacSapProvider;
//   delete m_schedSapUser;
//   delete m_cschedSapUser;
//   delete m_enbPhySapUser;
}

void
LteTestMac::SetLteMacSapUser (LteMacSapUser* s)
{
  m_macSapUser = s;
}

LteMacSapProvider*
LteTestMac::GetLteMacSapProvider (void)
{
  return m_macSapProvider;
}

void
LteTestMac::SetLteMacLoopback (Ptr<LteTestMac> s)
{
  m_macLoopback = s;
}

std::string
LteTestMac::GetDataReceived (void)
{
  NS_LOG_FUNCTION (this);

  return m_receivedData;
}

void
LteTestMac::SendTxOpportunity (uint32_t bytes)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("Bytes = " << bytes);

  Simulator::Schedule (Seconds (0.0), &LteMacSapUser::NotifyTxOpportunity, m_macSapUser, bytes);
  Simulator::Run ();
}


/**
 * MAC SAP
 */

void
LteTestMac::DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params)
{
  NS_LOG_FUNCTION (this);

  LteRlcHeader rlcHeader;

  // Remove RLC header
  params.pdu->RemoveHeader (rlcHeader);
  NS_LOG_LOGIC ("RLC header: " << rlcHeader);

  // Copy data to a string
  uint32_t dataLen = params.pdu->GetSize ();
  uint8_t *buf = new uint8_t[dataLen];
  params.pdu->CopyData (buf, dataLen);
  m_receivedData = std::string ((char *)buf, dataLen);

  NS_LOG_LOGIC ("Data (" << dataLen << ") = " << m_receivedData);
  delete [] buf;
}

void
LteTestMac::DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params)
{
  NS_LOG_FUNCTION (this);
}

/////////////////////////////////////////////////////////////////////

TypeId
LteTestPdcp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteTestPdcp")
    .SetParent<Object> ()
    .AddConstructor<LteTestPdcp> ()
    ;

  return tid;
}

LteTestPdcp::LteTestPdcp ()
{
  NS_LOG_FUNCTION (this);
  m_rlcSapUser = new LteRlcSpecificLteRlcSapUser<LteTestPdcp> (this);
  Simulator::ScheduleNow (&LteTestPdcp::Start, this);
}

LteTestPdcp::~LteTestPdcp ()
{
  NS_LOG_FUNCTION (this);
}

void
LteTestPdcp::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rlcSapUser;
}

void
LteTestPdcp::SetLteRlcSapProvider (LteRlcSapProvider* s)
{
  m_rlcSapProvider = s;
}

LteRlcSapUser*
LteTestPdcp::GetLteRlcSapUser (void)
{
  return m_rlcSapUser;
}


std::string
LteTestPdcp::GetDataReceived (void)
{
  NS_LOG_FUNCTION (this);

  return m_receivedData;
}


/**
 * RLC SAP
 */

void
LteTestPdcp::DoReceivePdcpPdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("PDU received = " << (*p));

  uint32_t dataLen = p->GetSize ();
  uint8_t *buf = new uint8_t[dataLen];
  p->CopyData (buf, dataLen);
  m_receivedData = std::string ((char *)buf, dataLen);

  NS_LOG_LOGIC (m_receivedData);

  delete [] buf;
}

/**
 * START
 */

void
LteTestPdcp::Start ()
{
  NS_LOG_FUNCTION (this);
}

void
LteTestPdcp::SendData (Time at, std::string dataToSend)
{
  NS_LOG_FUNCTION (this);

  LteRlcSapProvider::TransmitPdcpPduParameters p;
  p.rnti = 111;
  p.lcid = 222;

  NS_LOG_LOGIC ("Data(" << dataToSend.length () << ") = " << dataToSend.data ());
  p.pdcpPdu = Create<Packet> ((uint8_t *) dataToSend.data (), dataToSend.length ());

  NS_LOG_LOGIC ("Packet(" << p.pdcpPdu->GetSize () << ")");
  Simulator::Schedule (at, &LteRlcSapProvider::TransmitPdcpPdu, m_rlcSapProvider, p);
  Simulator::Run ();
}

