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
#include "ns3/lte-pdcp-header.h"

#include "ns3/lte-test-entities.h"

NS_LOG_COMPONENT_DEFINE ("LteTestEntities");

using namespace ns3;


/////////////////////////////////////////////////////////////////////

TypeId
LteTestRrc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteTestRrc")
    .SetParent<Object> ()
    .AddConstructor<LteTestRrc> ()
    ;

  return tid;
}

LteTestRrc::LteTestRrc ()
{
  NS_LOG_FUNCTION (this);
  m_pdcpSapUser = new LtePdcpSpecificLtePdcpSapUser<LteTestRrc> (this);
  Simulator::ScheduleNow (&LteTestRrc::Start, this);
}

LteTestRrc::~LteTestRrc ()
{
  NS_LOG_FUNCTION (this);
}

void
LteTestRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_pdcpSapUser;
}

void
LteTestRrc::SetLtePdcpSapProvider (LtePdcpSapProvider* s)
{
  m_pdcpSapProvider = s;
}

LtePdcpSapUser*
LteTestRrc::GetLtePdcpSapUser (void)
{
  return m_pdcpSapUser;
}


std::string
LteTestRrc::GetDataReceived (void)
{
  NS_LOG_FUNCTION (this);

  return m_receivedData;
}


/**
 * PDCP SAP
 */

void
LteTestRrc::DoReceiveRrcPdu (Ptr<Packet> p)
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
LteTestRrc::Start ()
{
  NS_LOG_FUNCTION (this);
}

void
LteTestRrc::SendData (Time at, std::string dataToSend)
{
  NS_LOG_FUNCTION (this);

  LtePdcpSapProvider::TransmitRrcPduParameters p;
  p.rnti = 111;
  p.lcid = 222;

  NS_LOG_LOGIC ("Data(" << dataToSend.length () << ") = " << dataToSend.data ());
  p.rrcPdu = Create<Packet> ((uint8_t *) dataToSend.data (), dataToSend.length ());

  NS_LOG_LOGIC ("Packet(" << p.rrcPdu->GetSize () << ")");
  Simulator::Schedule (at, &LtePdcpSapProvider::TransmitRrcPdu, m_pdcpSapProvider, p);
  Simulator::Run ();
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
  m_macSapUser = 0;
  m_macLoopback = 0;
  m_pdcpHeaderPresent = false;
  m_txOpportunityMode = MANUAL_MODE;
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

  Simulator::Schedule (Seconds (0.1), &LteMacSapUser::NotifyTxOpportunity, m_macSapUser, bytes);
  Simulator::Run ();
}

void
LteTestMac::SetPdcpHeaderPresent (bool present)
{
  NS_LOG_FUNCTION (this);
  m_pdcpHeaderPresent = present;
}

void
LteTestMac::SetTxOpportunityMode (uint8_t mode)
{
  NS_LOG_FUNCTION (this);
  m_txOpportunityMode = mode;
}


/**
 * MAC SAP
 */

void
LteTestMac::DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params)
{
  NS_LOG_FUNCTION (this);

  if (m_macLoopback)
    {
      Simulator::Schedule (Seconds (0.1), &LteMacSapUser::ReceivePdu,
                           m_macLoopback->m_macSapUser, params.pdu);
    }
  else
    {
      LteRlcHeader rlcHeader;
      LtePdcpHeader pdcpHeader;

      // Remove RLC header
      params.pdu->RemoveHeader (rlcHeader);
      NS_LOG_LOGIC ("RLC header: " << rlcHeader);

      // Remove PDCP header, if present
      if (m_pdcpHeaderPresent)
        {
          params.pdu->RemoveHeader (pdcpHeader);
          NS_LOG_LOGIC ("PDCP header: " << pdcpHeader);
        }

      // Copy data to a string
      uint32_t dataLen = params.pdu->GetSize ();
      uint8_t *buf = new uint8_t[dataLen];
      params.pdu->CopyData (buf, dataLen);
      m_receivedData = std::string ((char *)buf, dataLen);

      NS_LOG_LOGIC ("Data (" << dataLen << ") = " << m_receivedData);
      delete [] buf;
    }
}

void
LteTestMac::DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("Queue size = " << params.txQueueSize);

  if ((m_txOpportunityMode == AUTOMATIC_MODE) && params.txQueueSize)
    {
      Simulator::Schedule (Seconds (0.1), &LteMacSapUser::NotifyTxOpportunity,
                           m_macSapUser, params.txQueueSize + 2); // TODO Including RLC header size??? Not clean
    }
}
