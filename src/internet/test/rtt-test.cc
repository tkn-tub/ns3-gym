/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#include "ns3/test.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/rtt-estimator.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RttTestSuite");

using namespace ns3;

class RttTestCase : public TestCase
{
public:
  RttTestCase (double mean,
               double variance,
               double gain);

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  double m_mean;
  double m_variance;
  double m_gain;

};

RttTestCase::RttTestCase (double mean,
                          double variance,
                          double gain)
  : TestCase ("Rtt Estimate Test"),
    m_mean (mean),
    m_variance (variance),
    m_gain (gain)
{
}

void
RttTestCase::DoRun (void)
{
  Config::SetDefault ("ns3::RttEstimator::InitialEstimation", TimeValue (MilliSeconds (m_mean)));
  Config::SetDefault ("ns3::RttMeanDeviation::Gain", DoubleValue (m_gain));
  Config::SetDefault ("ns3::RttEstimator::MinRTO", TimeValue (Seconds (0)));

  Ptr<RttMeanDeviation> rtt = CreateObject<RttMeanDeviation> ();
  Ptr<NormalRandomVariable> nv = CreateObject<NormalRandomVariable> ();
  nv->SetAttribute ("Mean", DoubleValue (m_mean));
  nv->SetAttribute ("Variance", DoubleValue (m_variance));

  NS_TEST_EXPECT_MSG_EQ (m_mean, rtt->GetCurrentEstimate ().GetMilliSeconds (), "Initial estimate should match mean");

  double a, v, g;
  a = v = m_mean;
  g = m_gain;

  for (uint32_t i = 0; i < 10000; ++i)
    {
      int measurement = nv->GetInteger ();
      rtt->Measurement (Time::FromInteger (measurement, Time::MS));
      double err = (measurement - a);
      a = a + g * err;
      v = v + g * (std::abs (err) - v);
    }

  //5% tolerance
  double tolerance = m_mean * .05;

  NS_TEST_ASSERT_MSG_EQ_TOL (m_mean, rtt->GetCurrentEstimate ().GetMilliSeconds (), tolerance, "Unexpected estimate");

  int expectedTimeout = (int)a + 4 * (int)v;

  NS_TEST_EXPECT_MSG_EQ (rtt->RetransmitTimeout ().GetMilliSeconds (), expectedTimeout, "Timeout values do not match");
}
void
RttTestCase::DoTeardown (void)
{
}


static class RttTestSuite : public TestSuite
{
public:
  RttTestSuite ()
    : TestSuite ("rtt", UNIT)
  {
    AddTestCase (new RttTestCase (150.0, 10.0, .1), TestCase::QUICK);
    AddTestCase (new RttTestCase (5000.0, 5.0, .5), TestCase::QUICK);
    AddTestCase (new RttTestCase (200.0, 25.0, .7), TestCase::QUICK);
  }

} g_tcpTestSuite;
