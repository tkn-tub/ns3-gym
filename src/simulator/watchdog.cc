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
#include "watchdog.h"

namespace ns3 {

Watchdog::Watchdog ()
  : m_impl (0),
    m_event (),
    m_end (MicroSeconds (0))
{}

Watchdog::~Watchdog ()
{
  delete m_impl;
}

void 
Watchdog::Ping (Time delay)
{
  Time end = Simulator::Now () + delay;
  m_end = std::max (m_end, end);
  if (m_event.IsRunning ())
    {
      return;
    }
  m_event = Simulator::Schedule (m_end - Now (), &Watchdog::Expire, this);
}

void
Watchdog::Expire (void)
{
  if (m_end == Simulator::Now ())
    {
      m_impl->Invoke ();
    }
  else
    {
      m_event = Simulator::Schedule (m_end - Now (), &Watchdog::Expire, this);
    }
}

} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class WatchdogTests : public Test
{
public:
  WatchdogTests ();
  virtual bool RunTests (void);
private:
  void Expire (Time expected);
  bool m_error;
};

WatchdogTests::WatchdogTests ()
  : Test ("Watchdog")
{}

void
WatchdogTests::Expire (Time expected)
{
  bool result = true;
  NS_TEST_ASSERT_EQUAL (Simulator::Now (), expected);
  m_error = !result;
}

bool
WatchdogTests::RunTests (void)
{
  bool result = true;

  m_error = false;
  Watchdog watchdog;
  watchdog.SetFunction (&WatchdogTests::Expire, this);
  watchdog.SetArguments (MicroSeconds (40));
  watchdog.Ping (MicroSeconds (10));
  Simulator::Schedule (MicroSeconds (5), &Watchdog::Ping, &watchdog, MicroSeconds (20));
  Simulator::Schedule (MicroSeconds (20), &Watchdog::Ping, &watchdog, MicroSeconds (2));
  Simulator::Schedule (MicroSeconds (23), &Watchdog::Ping, &watchdog, MicroSeconds (17));
  Simulator::Run ();
  NS_TEST_ASSERT (!m_error);
  Simulator::Destroy ();

  return result;
}

static WatchdogTests g_watchdogTests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
