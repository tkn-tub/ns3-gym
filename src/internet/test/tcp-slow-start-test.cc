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
  m_lastChange (Time (0.0)),
  m_leftSegments (1),
  m_ackedSegments (0),
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

  if (m_initial)
    {
      m_initial = false;
      NS_LOG_INFO ("Ignored update to " << newValue <<
                   " with a segsize of " << segSize);
      return;
    }

  uint32_t segsBefore = oldValue / segSize;
  uint32_t segsAfter  = newValue / segSize;
  uint32_t segsCumAcked = segsAfter - segsBefore;

  NS_TEST_ASSERT_MSG_EQ (newValue % segSize, 0, "SSTh increased more than segsize");

  oldValue += segSize;

  NS_TEST_ASSERT_MSG_LT_OR_EQ (newValue, GetInitialSsThresh (), "cWnd increased over ssth");
  NS_TEST_ASSERT_MSG_EQ (oldValue, newValue, "Incremented cWnd more than segsize");

  if (m_ackedSegments == 0)
    {
      m_lastChange = Simulator::Now ();
    }

  m_ackedSegments += segsCumAcked;
  m_leftSegments -= segsCumAcked;

  NS_LOG_INFO ("Incremented cWnd by " << segSize << " byte in Slow Start " <<
               "achieving a value of " << newValue << ". Acked segments: " <<
               m_ackedSegments << " left to be acked: " << m_leftSegments);

  if (m_leftSegments == 0)
    {
      m_leftSegments = m_ackedSegments * 2;
      m_ackedSegments = 0;

      NS_LOG_INFO ("Updated test value. Left to be acked: " << m_leftSegments);
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
