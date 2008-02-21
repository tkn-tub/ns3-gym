/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "event-trace-source.h"

#ifdef RUN_SELF_TESTS

#include "test.h"

namespace ns3 {

class EventTraceSourceTest : public Test 
{
public:
  EventTraceSourceTest ();
  virtual ~EventTraceSourceTest ();
  virtual bool RunTests (void);
private:
  void CbOne (uint8_t a, double b);
  void CbTwo (uint8_t a, double b);

  bool m_one;
  bool m_two;
};

EventTraceSourceTest::EventTraceSourceTest ()
  : Test ("EventTraceSource")
{}
EventTraceSourceTest::~EventTraceSourceTest ()
{}
void
EventTraceSourceTest::CbOne (uint8_t a, double b)
{
  m_one = true;
}
void
EventTraceSourceTest::CbTwo (uint8_t a, double b)
{
  m_two = true;
}
bool 
EventTraceSourceTest::RunTests (void)
{
  bool result = true;

  EventTraceSource<uint8_t,double> trace;
  trace.Connect (MakeCallback (&EventTraceSourceTest::CbOne, this));
  trace.Connect (MakeCallback (&EventTraceSourceTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (m_one);
  NS_TEST_ASSERT (m_two);

  trace.Disconnect (MakeCallback (&EventTraceSourceTest::CbOne, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (!m_one);
  NS_TEST_ASSERT (m_two);
  trace.Disconnect (MakeCallback (&EventTraceSourceTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (!m_one);
  NS_TEST_ASSERT (!m_two);

  trace.Connect (MakeCallback (&EventTraceSourceTest::CbOne, this));
  trace.Connect (MakeCallback (&EventTraceSourceTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (m_one);
  NS_TEST_ASSERT (m_two);

  return result;
}

static EventTraceSourceTest g_eventTraceTest;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
