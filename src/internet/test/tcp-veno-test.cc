/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 ResiliNets, ITTC, University of Kansas
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
 * Author: Truc Anh N. Nguyen <annguyen@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 */

#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/tcp-congestion-ops.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/tcp-veno.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpVenoTestSuite");

/**
 * \brief Testing the additive increase and multiplicative decrease of TcpVeno
 */
class TcpVenoTest : public TestCase
{
public:
  TcpVenoTest (uint32_t cWnd,
               uint32_t segmentSize,
               uint32_t ssThresh,
               Time rtt,
               uint32_t segmentsAcked,
               uint32_t numRtt,
               const std::string &name);

private:
  virtual void DoRun (void);
  uint32_t AdditiveIncrease (Ptr<TcpVeno> cong, uint32_t diff, UintegerValue beta);
  uint32_t MultiplicativeDecrease (uint32_t diff, UintegerValue beta);

  uint32_t m_cWnd;
  uint32_t m_segmentSize;
  uint32_t m_ssThresh;
  Time m_rtt;
  uint32_t m_segmentsAcked;
  uint32_t m_numRtt;
  bool m_inc;
  Ptr<TcpSocketState> m_state;
};

TcpVenoTest::TcpVenoTest (uint32_t cWnd,
                          uint32_t segmentSize,
                          uint32_t ssThresh,
                          Time rtt,
                          uint32_t segmentsAcked,
                          uint32_t numRtt,
                          const std::string &name)
  : TestCase (name),
    m_cWnd (cWnd),
    m_segmentSize (segmentSize),
    m_ssThresh (ssThresh),
    m_rtt (rtt),
    m_segmentsAcked (segmentsAcked),
    m_numRtt (numRtt),
    m_inc (true)
{
}

void
TcpVenoTest::DoRun ()
{
  m_state = CreateObject<TcpSocketState> ();

  m_state->m_cWnd = m_cWnd;
  m_state->m_segmentSize = m_segmentSize;
  m_state->m_ssThresh = m_ssThresh;

  Ptr<TcpVeno> cong = CreateObject <TcpVeno> ();

  // Set baseRtt to 100 ms
  cong->PktsAcked (m_state, m_segmentsAcked, MilliSeconds (100));

  // Re-set Veno to assign a new value of minRtt
  cong->CongestionStateSet (m_state, TcpSocketState::CA_OPEN);
  cong->PktsAcked (m_state, m_segmentsAcked, m_rtt);

  // Another call to PktsAcked to increment cntRtt beyond 2
  cong->PktsAcked (m_state, m_segmentsAcked, m_rtt);

  Time baseRtt = MilliSeconds (100);
  uint32_t segCwnd = m_cWnd / m_segmentSize;

  // Calculate expected throughput
  uint64_t expectedCwnd;
  expectedCwnd = (uint64_t) segCwnd * (double) baseRtt.GetMilliSeconds () * 2 / (double) m_rtt.GetMilliSeconds ();

  // Calculate the difference between actual and expected throughput
  uint32_t diff;
  diff = (segCwnd * 2) - expectedCwnd;

  // Get the beta attribute
  UintegerValue beta;
  cong->GetAttribute ("Beta", beta);

  while (m_numRtt != 0)
    {
      // Update cwnd using Veno's additive increase algorithm
      cong->IncreaseWindow (m_state, m_segmentsAcked);

      // Our calculation of cwnd
      uint32_t calculatedCwnd = AdditiveIncrease (cong, diff, beta);

      NS_TEST_ASSERT_MSG_EQ (m_state->m_cWnd.Get (), calculatedCwnd,
                             "CWnd has not updated correctly based on Veno linear increase algorithm");
      m_numRtt--;
    }

  // Update ssthresh using Veno's multiplicative decrease algorithm
  uint32_t ssThresh = cong->GetSsThresh (m_state, m_state->m_cWnd);

  // Our calculation of ssthresh
  uint32_t calculatedSsThresh = MultiplicativeDecrease (diff, beta);

  NS_TEST_ASSERT_MSG_EQ (ssThresh, calculatedSsThresh,
                         "Veno has not decremented cWnd correctly based on its multiplicative decrease algo.");
}

uint32_t
TcpVenoTest::AdditiveIncrease (Ptr<TcpVeno> cong, uint32_t diff, UintegerValue beta)
{
  if (m_cWnd < m_ssThresh)
    { // Slow start
      if (m_segmentsAcked >= 1)
        {
          m_cWnd += m_segmentSize;
          m_segmentsAcked--;
        }
    }
  else
    { // Congestion avoidance
      if (diff < beta.Get ())
        { // Increase cwnd by 1 every RTT when bandwidth is not fully utilized
          if (m_segmentsAcked > 0)
            {
              double adder = static_cast<double> (m_segmentSize * m_segmentSize) / m_cWnd;
              adder = std::max (1.0, adder);
              m_cWnd += static_cast<uint32_t> (adder);
            }
        }
      else
        { // Increase cwnd by 1 every other RTT when bandwidth is fully utilized
          if (m_inc)
            {
              if (m_segmentsAcked > 0)
                {
                  double adder = static_cast<double> (m_segmentSize * m_segmentSize) / m_cWnd;
                  adder = std::max (1.0, adder);
                  m_cWnd += static_cast<uint32_t> (adder);
                }
              m_inc = false;
            }
          else
            {
              m_inc = true;
            }
        }
    }
  return m_cWnd;
}

uint32_t
TcpVenoTest::MultiplicativeDecrease (uint32_t diff, UintegerValue beta)
{
  uint32_t calculatedSsThresh;
  if (diff < beta.Get ())
    {
      calculatedSsThresh = std::max (2 * m_segmentSize, m_cWnd * 4 / 5);
    }
  else
    {
      calculatedSsThresh = std::max (2 * m_segmentSize, m_cWnd / 2);
    }
  return calculatedSsThresh;
}


// -------------------------------------------------------------------
static class TcpVenoTestSuite : public TestSuite
{
public:
  TcpVenoTestSuite () : TestSuite ("tcp-veno-test", UNIT)
  {
    AddTestCase (new TcpVenoTest (38 * 1446, 1446, 40 * 1446, MilliSeconds (100), 1, 1,
                                  "Veno test on cWnd in slow start and non-congestive loss"),
                 TestCase::QUICK);
    AddTestCase (new TcpVenoTest (30 * 536, 536, 20 * 536, MilliSeconds (106), 1, 1,
                                  "Veno test on cWnd with diff < beta"),
                 TestCase::QUICK);
    AddTestCase (new TcpVenoTest (60 * 536, 536, 40 * 536, MilliSeconds (106), 1, 3,
                                  "Veno increment test on cWnd with diff > beta"),
                 TestCase::QUICK);
  }
} g_tcpVenoTest;

} // namespace ns3
