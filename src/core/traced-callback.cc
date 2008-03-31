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
#include "traced-callback.h"

#ifdef RUN_SELF_TESTS

#include "test.h"

namespace ns3 {

class TracedCallbackTest : public Test 
{
public:
  TracedCallbackTest ();
  virtual ~TracedCallbackTest ();
  virtual bool RunTests (void);
private:
  void CbOne (uint8_t a, double b);
  void CbTwo (uint8_t a, double b);

  bool m_one;
  bool m_two;
};

TracedCallbackTest::TracedCallbackTest ()
  : Test ("TracedCallback")
{}
TracedCallbackTest::~TracedCallbackTest ()
{}
void
TracedCallbackTest::CbOne (uint8_t a, double b)
{
  m_one = true;
}
void
TracedCallbackTest::CbTwo (uint8_t a, double b)
{
  m_two = true;
}
bool 
TracedCallbackTest::RunTests (void)
{
  bool result = true;

  TracedCallback<uint8_t,double> trace;
  trace.ConnectWithoutContext (MakeCallback (&TracedCallbackTest::CbOne, this));
  trace.ConnectWithoutContext (MakeCallback (&TracedCallbackTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (m_one);
  NS_TEST_ASSERT (m_two);

  trace.DisconnectWithoutContext (MakeCallback (&TracedCallbackTest::CbOne, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (!m_one);
  NS_TEST_ASSERT (m_two);
  trace.DisconnectWithoutContext (MakeCallback (&TracedCallbackTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (!m_one);
  NS_TEST_ASSERT (!m_two);

  trace.ConnectWithoutContext (MakeCallback (&TracedCallbackTest::CbOne, this));
  trace.ConnectWithoutContext (MakeCallback (&TracedCallbackTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  NS_TEST_ASSERT (m_one);
  NS_TEST_ASSERT (m_two);

  return result;
}

static TracedCallbackTest g_eventTraceTest;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
