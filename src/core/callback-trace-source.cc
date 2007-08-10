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
#include "callback-trace-source.h"
#include "test.h"

namespace ns3 {

class CallbackTraceSourceTest : public Test 
{
public:
  CallbackTraceSourceTest ();
  virtual ~CallbackTraceSourceTest ();
  virtual bool RunTests (void);
private:
  void CbOne (TraceContext const &context, uint8_t a, double b);
  void CbTwo (TraceContext const &context, uint8_t a, double b);

  bool m_one;
  bool m_two;
};

CallbackTraceSourceTest::CallbackTraceSourceTest ()
  : Test ("CallbackTraceSource")
{}
CallbackTraceSourceTest::~CallbackTraceSourceTest ()
{}
void
CallbackTraceSourceTest::CbOne (TraceContext const &context, uint8_t a, double b)
{
  m_one = true;
}
void
CallbackTraceSourceTest::CbTwo (TraceContext const &context, uint8_t a, double b)
{
  m_two = true;
}
bool 
CallbackTraceSourceTest::RunTests (void)
{
  bool ok = true;
  TraceContext ctx;

  CallbackTraceSource<uint8_t,double> trace;
  trace.AddCallback (MakeCallback (&CallbackTraceSourceTest::CbOne, this), ctx);
  trace.AddCallback (MakeCallback (&CallbackTraceSourceTest::CbTwo, this), ctx);
  m_one = false;
  m_two = false;
  trace (1, 2);
  if (!m_one || !m_two)
    {
      ok = false;
    }
  trace.RemoveCallback (MakeCallback (&CallbackTraceSourceTest::CbOne, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  if (m_one || !m_two)
    {
      ok = false;
    }
  trace.RemoveCallback (MakeCallback (&CallbackTraceSourceTest::CbTwo, this));
  m_one = false;
  m_two = false;
  trace (1, 2);
  if (m_one || m_two)
    {
      ok = false;
    }

  return ok;
}

CallbackTraceSourceTest g_callbackTraceTest;



}//namespace ns3
