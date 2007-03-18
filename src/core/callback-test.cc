/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include "test.h"
#include "callback.h"
#include <stdint.h>

#ifdef RUN_SELF_TESTS

namespace ns3 {

static bool gTest5 = false;
static bool gTest6 = false;
static bool gTest7 = false;

void Test5 (void)
{
  gTest5 = true;
}

void Test6 (int)
{
  gTest6 = true;
}

int Test7 (int a)
{
  gTest7 = true;
  return a;
}

void *Test9 (bool *a)
{
  return a;
}

void *Test10 (bool *a, int const & b)
{
  return a;
}

class CallbackTest : public ns3::Test {
private:
  bool m_test1;
  bool m_test2;
  bool m_test3;
  bool m_test4;
public:
  CallbackTest ();
  virtual bool RunTests (void);
  void Reset (void);
  bool IsWrong (void);
  void Test1 (void);
  int Test2 (void);
  void Test3 (double a);
  int Test4 (double a, int b);
  void Test8 (Callback<void, int> callback);
};

CallbackTest::CallbackTest ()
  : ns3::Test ("Callback"),
    m_test1 (false),
    m_test2 (false),
    m_test3 (false),
    m_test4 (false)
{}

void 
CallbackTest::Test1 (void)
{
  m_test1 = true;
}
int 
CallbackTest::Test2 (void)
{
  m_test2 = true;
  return 2;
}
void 
CallbackTest::Test3 (double a)
{
  m_test3 = true;
}
int 
CallbackTest::Test4 (double a, int b)
{
  m_test4 = true;
  return 4;
}
void
CallbackTest::Test8 (Callback<void,int> callback)
{
  callback (3);
}
bool
CallbackTest::IsWrong (void)
{
  if (!m_test1 ||
      !m_test2 ||
      !m_test3 ||
      !m_test4 ||
      !gTest5 ||
      !gTest6 ||
      !gTest7) 
    {
      return true;
    }
  return false;
}

void
CallbackTest::Reset (void)
{
  m_test1 = false;
  m_test2 = false;
  m_test3 = false;
  m_test4 = false;
  gTest5 = false;
  gTest6 = false;
  gTest7 = false;
}

  
bool 
CallbackTest::RunTests (void)
{
  bool ok = true;

  typedef ns3::Callback<void> A;
  typedef ns3::Callback<int> B;
  typedef ns3::Callback<void, double> C;
  typedef ns3::Callback<int, double, int> D;
  typedef ns3::Callback<void> E;
  typedef ns3::Callback<void,int> F;
  typedef ns3::Callback<int,int> G;
  
  A a0 (this, &CallbackTest::Test1);
  B b0;
  b0 = B (this, &CallbackTest::Test2);
  C c0 = C (this, &CallbackTest::Test3);
  D d0 = D (this, &CallbackTest::Test4);
  E e0 = E (&Test5, true, true);
  F f0 = F (&Test6, true, true);
  G g0 = G (&Test7, true, true);

  a0 ();
  b0 ();
  c0 (0.0);
  d0 (0.0, 1);
  e0 ();
  f0 (1);
  g0 (1);

  if (IsWrong ()) 
    {
      ok = false;
    }

  Reset ();

  A a1 = ns3::MakeCallback (&CallbackTest::Test1, this);
  B b1 = ns3::MakeCallback (&CallbackTest::Test2, this);
  C c1 = ns3::MakeCallback (&CallbackTest::Test3, this);
  D d1 = ns3::MakeCallback (&CallbackTest::Test4, this);
  E e1 = ns3::MakeCallback (&Test5);
  F f1 = ns3::MakeCallback (&Test6);
  G g1 = ns3::MakeCallback (&Test7);
  
  a1 ();
  b1 ();
  c1 (0.0);
  d1 (0.0, 1);
  e1 ();
  f1 (1);
  g1 (2);

  Test8 (f1);

  Callback<void, int64_t,int64_t> a2;

  if (IsWrong ()) 
    {
      ok = false;
    }


  MakeBoundCallback (&Test7, 0);
  bool v;
  MakeBoundCallback (&Test9, &v);
  MakeBoundCallback (&Test10, &v);

  return ok;
}

static CallbackTest gCallbackTest;

}; // namespace

#endif /* RUN_SELF_TESTS */
