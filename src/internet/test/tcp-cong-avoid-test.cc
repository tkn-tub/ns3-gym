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
#include "ns3/log.h"
#include "ns3/simple-channel.h"
#include "ns3/internet-module.h"
#include "ns3/config.h"
#include "tcp-cong-avoid-test.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpNewRenoCongAvoidTest");

TcpNewRenoCongAvoidNormalTest::TcpNewRenoCongAvoidNormalTest (uint32_t segmentSize,
                                                              uint32_t packetSize,
                                                              uint32_t packets,
                                                              TypeId &typeId,
                                                              const std::string &desc)
  : TcpGeneralTest (desc, packetSize, packets, Seconds (0.01), Seconds (0.5),
                    Seconds (10), 0, 1, segmentSize, typeId, 1500),
    m_increment (0),
    m_initial (true)
{
}

/**
 * \brief Check the increment of the congestion window in the congestion avoidance
 *
 * The method trace the size change of the cWnd, saving each increment.
 *
 * \see Check
 */
void
TcpNewRenoCongAvoidNormalTest::CWndTrace (uint32_t oldValue, uint32_t newValue)
{
  if (m_initial)
    {
      m_initial = false;
      return;
    }

  if (!m_event.IsRunning ())
    {
      m_event = Simulator::Schedule (Seconds (1.0),
                                     &TcpNewRenoCongAvoidNormalTest::Check, this);
    }

  m_increment += newValue - oldValue;
}

void
TcpNewRenoCongAvoidNormalTest::QueueDrop (SocketWho who)
{
  NS_FATAL_ERROR ("Drop on the queue; cannot validate congestion avoidance");
}

void
TcpNewRenoCongAvoidNormalTest::PhyDrop (SocketWho who)
{
  NS_FATAL_ERROR ("Drop on the phy: cannot validate congestion avoidance");
}

/**
 * \brief Called each RTT (1.0 sec in the testing environment) and check
 * that the overall increment in this RTT is less or equal than 1 MSS
 */
void
TcpNewRenoCongAvoidNormalTest::Check ()
{
  uint32_t segSize = GetSegSize (TcpGeneralTest::SENDER);

  if (m_increment != 0)
    {
      NS_TEST_ASSERT_MSG_LT_OR_EQ (m_increment, segSize,
                                   "Increment exceeded segment size in one RTT");
    }

  m_increment = 0;

  m_event = Simulator::Schedule (Seconds (1.0),
                                 &TcpNewRenoCongAvoidNormalTest::Check, this);

}

void
TcpNewRenoCongAvoidNormalTest::NormalClose (SocketWho who)
{
  if (who == SENDER)
    {
      m_event.Cancel ();
    }
}

//-----------------------------------------------------------------------------

static class TcpRenoCongAvoidTestSuite : public TestSuite
{
public:
  TcpRenoCongAvoidTestSuite () : TestSuite ("tcp-cong-avoid-test", UNIT)
  {
    std::list<TypeId> types;
    types.insert (types.begin (), TcpWestwood::GetTypeId ());
    types.insert (types.begin (), TcpNewReno::GetTypeId ());

    for (std::list<TypeId>::iterator it = types.begin (); it != types.end (); ++it)
      {
        for (uint32_t i = 10; i <= 50; i += 10)
          {
            AddTestCase (new TcpNewRenoCongAvoidNormalTest (500, 500, i, (*it),
                                                            "cong avoid MSS=500, pkt_size=500," + (*it).GetName ()),
                         TestCase::QUICK);
            AddTestCase (new TcpNewRenoCongAvoidNormalTest (500, 1000, i, (*it),
                                                            "cong avoid MSS=500, pkt_size=1000," + (*it).GetName ()),
                         TestCase::QUICK);
          }
      }
  }
} g_tcpCongAvoidNormalTest;

} // namespace ns3
