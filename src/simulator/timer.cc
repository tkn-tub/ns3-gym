/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
 * All rights reserved.
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
 * Author: Gustavo Carneiro  <gjc@inescporto.pt>
 */

#include "timer.h"

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class TimerTests : public Test
{
  int m_counter;
public:
  TimerTests ();
  virtual ~TimerTests ();
  virtual bool RunTests (void);

  void TimerCallback (int x);
};

TimerTests::TimerTests ()
  : Test ("Timer"), m_counter (0)
{}
TimerTests::~TimerTests ()
{}

void
TimerTests::TimerCallback (int x)
{
  m_counter += x;
}

bool TimerTests::RunTests (void)
{
  bool result = true;

  Timer<int> timer1 (MilliSeconds (1), MakeCallback (&TimerTests::TimerCallback, this));

  // Check if it works in the common case
  timer1.Schedule (1);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_counter, 1);

  // Check that it does not repeat itself
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_counter, 1);

  // Check cancellation
  timer1.Schedule (1);
  timer1.Cancel ();
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_counter, 1);

  return result;
}

static TimerTests g_timerTests;
    
};

#endif /* RUN_SELF_TESTS */
