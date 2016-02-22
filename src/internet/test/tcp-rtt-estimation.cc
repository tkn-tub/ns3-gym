/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Natale Patriciello <natale.patriciello@gmail.com>
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
 */

#include "tcp-general-test.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "tcp-error-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpRttEstimationTestSuite");

/**
 * \brief Check Rtt calculations
 *
 * First check is that, for each ACK, we have a valid estimation of the RTT.
 * The second check is that, when updating RTT history, we should consider
 * retransmission only segments which sequence number is lower than the highest
 * already transmitted.
 */
class TcpRttEstimationTest : public TcpGeneralTest
{
public:
  TcpRttEstimationTest (const std::string &desc, bool enableTs, uint32_t dataPkt);

protected:
  virtual Ptr<TcpSocketMsgBase> CreateReceiverSocket (Ptr<Node> node);
  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);

  virtual void Rx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void UpdatedRttHistory (const SequenceNumber32 & seq, uint32_t sz,
                                  bool isRetransmission, SocketWho who);
  virtual void RttTrace (Time oldTime, Time newTime);
  void FinalChecks ();

private:
  bool m_enableTs;
  bool m_rttChanged;
  SequenceNumber32 m_highestTxSeq;
};

TcpRttEstimationTest::TcpRttEstimationTest (const std::string &desc, bool enableTs, uint32_t dataPkt)
  : TcpGeneralTest (desc, 500, dataPkt, Seconds (0.01), Seconds (0.05), Seconds (2.0),
                    0xffffffff,1, 500),
    m_enableTs (enableTs),
    m_rttChanged (false),
    m_highestTxSeq (0)
{
}

Ptr<TcpSocketMsgBase>
TcpRttEstimationTest::CreateReceiverSocket (Ptr<Node> node)
{
  Ptr<TcpSocketMsgBase> s = TcpGeneralTest::CreateReceiverSocket (node);
  if (!m_enableTs)
    {
      s->SetAttribute ("Timestamp", BooleanValue (false));
    }

  return s;
}

Ptr<TcpSocketMsgBase>
TcpRttEstimationTest::CreateSenderSocket (Ptr<Node> node)
{
  Ptr<TcpSocketMsgBase> s = TcpGeneralTest::CreateSenderSocket (node);
  if (!m_enableTs)
    {
      s->SetAttribute ("Timestamp", BooleanValue (false));
    }

  return s;
}

void
TcpRttEstimationTest::Tx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  if (who == SENDER && h.GetFlags () != TcpHeader::SYN)
    {
      if (m_highestTxSeq < h.GetSequenceNumber ())
        {
          m_highestTxSeq = h.GetSequenceNumber ();
        }

      Ptr<RttEstimator> rttEstimator = GetRttEstimator (SENDER);
      NS_ASSERT (rttEstimator != 0);
      NS_LOG_DEBUG ("S Rx: seq=" << h.GetSequenceNumber () << " ack=" << h.GetAckNumber ());
      NS_TEST_ASSERT_MSG_NE (rttEstimator->GetEstimate (), Seconds (1),
                             "Default Estimate for the RTT");
    }
}

void
TcpRttEstimationTest::Rx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  if (who == RECEIVER)
    {
      NS_LOG_DEBUG ("R Rx: seq=" << h.GetSequenceNumber () << " ack=" << h.GetAckNumber ());
    }
}

void
TcpRttEstimationTest::UpdatedRttHistory (const SequenceNumber32 & seq, uint32_t sz,
                                         bool isRetransmission, SocketWho who)
{
  if (seq < m_highestTxSeq)
    {
      NS_TEST_ASSERT_MSG_EQ (isRetransmission, true,
                             "A retransmission is not flagged as such");
    }
  else
    {
      NS_TEST_ASSERT_MSG_EQ (isRetransmission, false,
                             "Incorrectly flagging seq as retransmission");
    }

}

void
TcpRttEstimationTest::RttTrace (Time oldTime, Time newTime)
{
  NS_LOG_DEBUG ("Rtt changed to " << newTime);
  m_rttChanged = true;
}

void
TcpRttEstimationTest::FinalChecks ()
{
  NS_TEST_ASSERT_MSG_EQ (m_rttChanged, true, "Rtt was not updated");
}

//-----------------------------------------------------------------------------

static class TcpRttEstimationTestSuite : public TestSuite
{
public:
  TcpRttEstimationTestSuite () : TestSuite ("tcp-rtt-estimation-test", UNIT)
  {
    AddTestCase (new TcpRttEstimationTest ("RTT estimation, ts, no data", true, 0),
                 TestCase::QUICK);
    AddTestCase (new TcpRttEstimationTest ("RTT estimation, no ts, no data", false, 0),
                 TestCase::QUICK);
    AddTestCase (new TcpRttEstimationTest ("RTT estimation, ts, some data", true, 10),
                 TestCase::QUICK);
    AddTestCase (new TcpRttEstimationTest ("RTT estimation, no ts, some data", false, 10),
                 TestCase::QUICK);
  }
} g_tcpRttEstimationTestSuite;

} // namespace ns3
