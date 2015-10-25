/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
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
#include "tcp-slow-start-test.h"
#include "ns3/log.h"
#include "ns3/simple-channel.h"
#include "ns3/internet-module.h"
#include "ns3/config.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpSlowStartTest");

// TcpSlowStartNormalTest

TcpSlowStartNormalTest::TcpSlowStartNormalTest (uint32_t segmentSize,
                                                uint32_t packetSize,
                                                uint32_t initSsTh,
                                                uint32_t packets,
                                                TypeId &typeId,
                                                const std::string &desc)
  : TcpGeneralTest (desc, packetSize, packets, Seconds (0.01), Seconds (0.5),
                    Seconds (10), initSsTh, 1, segmentSize, typeId, 1500),
  m_ackedBytes (0),
  m_sentBytes (0),
  m_totalAckedBytes (0),
  m_allowedIncrease (0),
  m_initial (true)
{
}

/**
 * \brief Trace the cWnd over the slow start
 *
 * This method is called each time the cWnd changes. It should be updated only
 * by MSS bytes at time. Since the size doubles each RTT, a timing test is also
 * performed: the doubling should be made in 0.5s from the first (0.5s is
 * the delay of the SimpleChannel which connect the two socket).
 *
 * \param oldValue old value of cWnd
 * \param newValue new value of cWnd
 */
void
TcpSlowStartNormalTest::CWndTrace (uint32_t oldValue, uint32_t newValue)
{
  uint32_t segSize = GetSegSize (TcpGeneralTest::SENDER);
  uint32_t increase = newValue - oldValue;

  if (m_initial)
    {
      m_initial = false;
      NS_LOG_INFO ("Ignored update to " << newValue <<
                   " with a segsize of " << segSize);
      return;
    }

  // The increase in RFC should be <= of segSize. In ns-3 we force = segSize
  NS_TEST_ASSERT_MSG_EQ (increase, segSize, "Increase different than segsize");
  NS_TEST_ASSERT_MSG_LT_OR_EQ (newValue, GetInitialSsThresh (), "cWnd increased over ssth");

  NS_LOG_INFO ("Incremented cWnd by " << segSize << " bytes in Slow Start " <<
               "achieving a value of " << newValue);

  NS_TEST_ASSERT_MSG_GT_OR_EQ (m_allowedIncrease, 1, "Increase not allowed");
  m_allowedIncrease--;
}

void
TcpSlowStartNormalTest::Tx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  NS_LOG_FUNCTION (this << p << h << who);

  if (who == SENDER && Simulator::Now ().GetSeconds () > 5.0)
    {
      m_sentBytes += GetSegSize (TcpGeneralTest::SENDER);
    }
}

void
TcpSlowStartNormalTest::Rx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  NS_LOG_FUNCTION (this << p << h << who);

  if (who == SENDER && Simulator::Now ().GetSeconds () > 5.0)
    {
      uint32_t acked = h.GetAckNumber().GetValue() - m_totalAckedBytes - 1;
      m_totalAckedBytes += acked;
      m_ackedBytes += acked;

      NS_LOG_INFO ("Ack of " << acked << " bytes, acked this round=" << m_ackedBytes);

      if (m_ackedBytes >= GetSegSize (SENDER))
        {
          NS_LOG_INFO ("FULL ACK achieved, bytes=" << m_ackedBytes);
          m_allowedIncrease += 1;
          m_ackedBytes -= GetSegSize (SENDER);
        }

      while (m_ackedBytes >= GetSegSize (SENDER))
        {
          m_ackedBytes -= GetSegSize (SENDER);
        }
    }
}

// TcpSlowStartAttackerTest
TcpSlowStartAttackerTest::TcpSlowStartAttackerTest (uint32_t segmentSize,
                                                    uint32_t packetSize,
                                                    uint32_t initSsTh,
                                                    uint32_t packets,
                                                    TypeId &typeId, const std::string &msg)
  : TcpSlowStartNormalTest (segmentSize, packetSize, initSsTh, packets, typeId, msg)
{

}

Ptr<TcpSocketMsgBase>
TcpSlowStartAttackerTest::CreateReceiverSocket (Ptr<Node> node)
{
  Ptr<TcpSocketSmallAcks> socket = DynamicCast<TcpSocketSmallAcks> (
      CreateSocket (node,
                    TcpSocketSmallAcks::GetTypeId (),
                    m_congControlTypeId));
  socket->SetBytesToAck (125);

  return socket;
}


//-----------------------------------------------------------------------------

static class TcpSlowStartTestSuite : public TestSuite
{
public:
  TcpSlowStartTestSuite () : TestSuite ("tcp-slow-start-test", UNIT)
  {
    // This test have less packets to transmit than SsTh
    std::list<TypeId> types;
    types.insert (types.begin (), TcpNewReno::GetTypeId ());
    types.insert (types.begin (), TcpWestwood::GetTypeId ());

    for (std::list<TypeId>::iterator it = types.begin (); it != types.end (); ++it)
      {
        AddTestCase (new TcpSlowStartNormalTest (500, 500, 10000, 10, (*it),
                                                 "slow start 500 byte, " + (*it).GetName ()),
                     TestCase::QUICK);
        AddTestCase (new TcpSlowStartNormalTest (1000, 1000, 10000, 9, (*it),
                                                 "slow start 1000 byte, " + (*it).GetName ()),
                     TestCase::QUICK);
        AddTestCase (new TcpSlowStartNormalTest (500, 250, 10000, 10, (*it),
                                                 "slow start small packets, " + (*it).GetName ()),
                     TestCase::QUICK);
        AddTestCase (new TcpSlowStartAttackerTest (500, 500, 10000, 10, (*it),
                                                   "slow start ack attacker, 500 byte, " + (*it).GetName ()),
                     TestCase::QUICK);
        AddTestCase (new TcpSlowStartAttackerTest (1000, 1000, 10000, 9, (*it),
                                                   "slow start ack attacker, 1000 byte, " + (*it).GetName ()),
                     TestCase::QUICK);
      }
  }
} g_tcpSlowStartTestSuite;

} // namespace ns3
