/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/watchdog.h"
#include "ns3/test.h"

using namespace ns3;

class WatchdogTestCase : public TestCase
{
public:
  WatchdogTestCase ();
  virtual void DoRun (void);
  void Expire (Time expected);
  bool m_expired;
  Time m_expiredTime;
  Time m_expiredArgument;
};

WatchdogTestCase::WatchdogTestCase()
  : TestCase ("Check that we can keepalive a watchdog")
{
}

void
WatchdogTestCase::Expire (Time expected)
{
  m_expired = true;
  m_expiredTime = Simulator::Now ();
  m_expiredArgument = expected;
}

void
WatchdogTestCase::DoRun (void)
{
  m_expired = false;
  m_expiredArgument = Seconds (0);
  m_expiredTime = Seconds (0);
  Watchdog watchdog;
  watchdog.SetFunction (&WatchdogTestCase::Expire, this);
  watchdog.SetArguments (MicroSeconds (40));
  watchdog.Ping (MicroSeconds (10));
  Simulator::Schedule (MicroSeconds (5), &Watchdog::Ping, &watchdog, MicroSeconds (20));
  Simulator::Schedule (MicroSeconds (20), &Watchdog::Ping, &watchdog, MicroSeconds (2));
  Simulator::Schedule (MicroSeconds (23), &Watchdog::Ping, &watchdog, MicroSeconds (17));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_TEST_ASSERT_MSG_EQ (m_expired, true, "The timer did not expire ??");
  NS_TEST_ASSERT_MSG_EQ (m_expiredTime, MicroSeconds (40), "The timer did not expire at the expected time ?");
  NS_TEST_ASSERT_MSG_EQ (m_expiredArgument, MicroSeconds (40), "We did not get the right argument");
}



static class WatchdogTestSuite : public TestSuite
{
public:
  WatchdogTestSuite()
    : TestSuite ("watchdog", UNIT)
  {
    AddTestCase (new WatchdogTestCase (), TestCase::QUICK);
  }
} g_watchdogTestSuite;
