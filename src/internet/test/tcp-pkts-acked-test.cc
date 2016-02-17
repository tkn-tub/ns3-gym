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

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpPktsAckedTestSuite");

/**
 * \brief Check the number of times that PktsAcked is called
 *
 * Set a custom congestion control class, which calls PktsAckedCalled
 * each time the TCP implementation calls PktsAcked.
 *
 * The checks are performed in FinalChecks: the number of bytes acked divided
 * by segment size should be the same as the number of segments passed through
 * PktsAcked in the congestion control.
 *
 * \see DummyCongControl
 * \see FinalChecks
 */
class TcpPktsAckedOpenTest : public TcpGeneralTest
{
public:
  TcpPktsAckedOpenTest (const std::string &desc);

  void PktsAckedCalled (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked,
                        const Time &rtt);

protected:
  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);
  virtual void Rx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);

  void FinalChecks ();

private:
  uint32_t m_segmentsAcked;    //! Contains the number of times PktsAcked is called
  uint32_t m_segmentsReceived; //! Contains the ack number received
};

/**
 * \brief Behaves as NewReno, except that each time PktsAcked is called,
 * a notification is sent to TcpPktsAckedOpenTest
 */
class DummyCongControl : public TcpNewReno
{
public:
  static TypeId GetTypeId (void);
  DummyCongControl ()
  {
  }
  DummyCongControl (TcpPktsAckedOpenTest *test)
  {
    m_test = test;
  }

  void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time &rtt)
  {
    m_test->PktsAckedCalled (tcb, segmentsAcked, rtt);
  }

private:
  TcpPktsAckedOpenTest *m_test;
};

TypeId
DummyCongControl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DummyCongControl")
    .SetParent<TcpNewReno> ()
    .AddConstructor<DummyCongControl> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

TcpPktsAckedOpenTest::TcpPktsAckedOpenTest (const std::string &desc)
  : TcpGeneralTest (desc, 500, 20, Seconds (0.01), Seconds (0.05), Seconds (2.0),
                    0xffffffff,1, 500),
    m_segmentsAcked (0),
    m_segmentsReceived (0)
{
}

Ptr<TcpSocketMsgBase>
TcpPktsAckedOpenTest::CreateSenderSocket (Ptr<Node> node)
{
  Ptr<TcpSocketMsgBase> s = TcpGeneralTest::CreateSenderSocket (node);
  s->SetCongestionControlAlgorithm (new DummyCongControl (this));

  return s;
}

void
TcpPktsAckedOpenTest::PktsAckedCalled (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked,
                                       const Time &rtt)
{
  m_segmentsAcked += segmentsAcked;
}

void
TcpPktsAckedOpenTest::Rx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  if (who == SENDER && (!(h.GetFlags () & TcpHeader::SYN)))
    {
      m_segmentsReceived = h.GetAckNumber ().GetValue ();
    }
}

void
TcpPktsAckedOpenTest::FinalChecks ()
{
  NS_TEST_ASSERT_MSG_EQ (m_segmentsReceived / GetSegSize (SENDER), m_segmentsAcked,
                         "Not all acked segments have been passed to PktsAcked method");
}

//-----------------------------------------------------------------------------

static class TcpPktsAckedTestSuite : public TestSuite
{
public:
  TcpPktsAckedTestSuite () : TestSuite ("tcp-pkts-acked-test", UNIT)
  {
    AddTestCase (new TcpPktsAckedOpenTest ("PktsAcked check while in OPEN state"),
                 TestCase::QUICK);
    // Add DISORDER, RECOVERY and LOSS state check
  }
} g_TcpPktsAckedTestSuite;

} // namespace ns3
