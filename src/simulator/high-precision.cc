/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "high-precision.h"

#include <cmath>
#include "ns3/assert.h"


namespace ns3 {

HighPrecision Abs (HighPrecision const &value)
{
  if (value.Compare (HighPrecision::Zero ()) <= 0)
    {
      HighPrecision v = HighPrecision::Zero ();
      v.Sub (value);
      return v;
    }
  else
    {
      return value;
    }
}

} /* namespace ns3 */

#include "ns3/test.h"

#define CHECK_EXPECTED(a,b) \
  NS_TEST_ASSERT_MSG_EQ (a.GetInteger (),b,"Arithmetic failure: " << (a.GetInteger ()) << "!=" << (b))

#define V(v) \
  HighPrecision (v, false)

namespace ns3 {

class HpArithmeticTestCase : public TestCase
{
public:
  HpArithmeticTestCase ();
  virtual bool DoRun (void);
};

HpArithmeticTestCase::HpArithmeticTestCase ()
  : TestCase ("Check basic arithmetic operations")
{
}
bool
HpArithmeticTestCase::DoRun (void)
{
  HighPrecision a, b;
  a = HighPrecision (1, false);
  b = HighPrecision (1, false);

  a.Sub (b);
  CHECK_EXPECTED (a, 0);

  a = V (1);
  a.Sub (V (2));
  CHECK_EXPECTED (a, -1);

  a = V (1);
  a.Sub (V (3));
  CHECK_EXPECTED (a, -2);

  a = V (1);
  a.Sub (V (-1));
  CHECK_EXPECTED (a, 2);

  a = V (1);
  a.Sub (V (-2));
  CHECK_EXPECTED (a, 3);

  a = V (-3);
  a.Sub (V (-4));
  CHECK_EXPECTED (a, 1);

  a = V (-2);
  a.Sub (V (3));
  CHECK_EXPECTED (a, -5);

  a = V (1);
  a.Add (V (2));
  CHECK_EXPECTED (a, 3);

  a = V (1);
  a.Add (V (-3));
  CHECK_EXPECTED (a, -2);

  a = V (0);
  a.Add (V (0));
  CHECK_EXPECTED (a, 0);

  a = V (0);
  a.Mul (V (0));
  CHECK_EXPECTED (a, 0);
  a = V (0);
  a.Mul (V (1));
  CHECK_EXPECTED (a, 0);
  a = V (0);
  a.Mul (V (-1));
  CHECK_EXPECTED (a, 0);
  a = V (1);
  a.Mul (V (0));
  CHECK_EXPECTED (a, 0);
  a = V (1);
  a.Mul (V (1));
  CHECK_EXPECTED (a, 1);
  a = V (1);
  a.Mul (V (-1));
  CHECK_EXPECTED (a, -1);
  a = V (-1);
  a.Mul (V (-1));
  CHECK_EXPECTED (a, 1);

  a = V (0);
  a.Mul (V (1));
  CHECK_EXPECTED (a, 0);
  a = V (0);
  a.Mul (V (-1));
  CHECK_EXPECTED (a, 0);
  a = V (1);
  a.Mul (V (1));
  CHECK_EXPECTED (a, 1);
  a = V (1);
  a.Mul (V (-1));
  CHECK_EXPECTED (a, -1);
  a = V (-1);
  a.Mul (V (1));
  CHECK_EXPECTED (a, -1);
  a = V (-1);
  a.Mul (V (-1));
  CHECK_EXPECTED (a, 1);



  a = V (2);
  a.Mul (V (3));
  a.Div (V (3));
  CHECK_EXPECTED (a, 2);

  // Below, the division loses precision because 2/3 is not
  // representable exactly in 64.64 integers. So, we got
  // something super close but the final rounding kills us.
  a = V (2);
  a.Div (V (3));
  a.Mul (V (3));
  CHECK_EXPECTED (a, 1);

  // The example below shows that we really do not lose
  // much precision internally: it is almost always the
  // final conversion which loses precision.
  a = V (2000000000);
  a.Div (V (3));
  a.Mul (V (3));
  CHECK_EXPECTED (a, 1999999999);

  return GetErrorStatus ();
}

class HpBug455TestCase : public TestCase
{
public:
  HpBug455TestCase ();
  virtual bool DoRun (void);
};

HpBug455TestCase::HpBug455TestCase ()
  : TestCase ("Test case for bug 455")
{
}
bool
HpBug455TestCase::DoRun (void)
{
  HighPrecision a = HighPrecision (0.1);
  a.Div (HighPrecision (1.25));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 0.08, "The original testcase");
  a = HighPrecision (0.5);
  a.Mul (HighPrecision (5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 2.5, "Simple test for multiplication");
  a = HighPrecision (-0.5);
  a.Mul (HighPrecision (5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -2.5, "Test sign, first operation negative");
  a = HighPrecision (-0.5);
  a.Mul (HighPrecision (-5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 2.5, "both operands negative");
  a = HighPrecision (0.5);
  a.Mul (HighPrecision (-5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -2.5, "only second operand negative");

  return GetErrorStatus ();
}


class HpBug863TestCase : public TestCase
{
public:
  HpBug863TestCase ();
  virtual bool DoRun (void);
};

HpBug863TestCase::HpBug863TestCase ()
  : TestCase ("Test case for bug 863")
{
}
bool
HpBug863TestCase::DoRun (void)
{
  HighPrecision a = HighPrecision (0.9);
  a.Div (HighPrecision (1));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 0.9, "The original testcase");
  a = HighPrecision (0.5);
  a.Div (HighPrecision (0.5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 1.0, "Simple test for division");
  a = HighPrecision (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -0.5, "Check that we actually convert doubles correctly");
  a.Div (HighPrecision (0.5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -1.0, "first argument negative");
  a = HighPrecision (0.5);
  a.Div (HighPrecision (-0.5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -1.0, "second argument negative");
  a = HighPrecision (-0.5);
  a.Div (HighPrecision (-0.5));
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 1.0, "both arguments negative");

  return GetErrorStatus ();
}

class HpCompareTestCase : public TestCase
{
public:
  HpCompareTestCase ();
  virtual bool DoRun (void);
};

HpCompareTestCase::HpCompareTestCase ()
  : TestCase ("Check basic compare operations")
{
}
bool
HpCompareTestCase::DoRun (void)
{
  HighPrecision a, b;

  a = V (-1);
  b = V (1);
  NS_TEST_ASSERT_MSG_EQ (a.Compare (b), -1, "a is smaller than b");
  a = V (-1);
  b = V (-2);
  NS_TEST_ASSERT_MSG_EQ (a.Compare (b), 1, "a is bigger than b");
  a = V (-1);
  b = V (-1);
  NS_TEST_ASSERT_MSG_EQ (a.Compare (b), 0, "a is equal to b");

  a = V (1);
  b = V (-1);
  NS_TEST_ASSERT_MSG_EQ (a.Compare (b), 1, "a is bigger than b");
  a = V (1);
  b = V (2);
  NS_TEST_ASSERT_MSG_EQ (a.Compare (b), -1, "a is smaller than b");
  a = V (1);
  b = V (1);
  NS_TEST_ASSERT_MSG_EQ (a.Compare (b), 0, "a is equal to b");

  return GetErrorStatus ();
}

class HpInvertTestCase : public TestCase
{
public:
  HpInvertTestCase ();
  virtual bool DoRun (void);
};

HpInvertTestCase::HpInvertTestCase ()
  : TestCase ("Test case for invertion")
{
}

bool
HpInvertTestCase::DoRun (void)
{
#define TEST(factor)                                                    \
  do {                                                                  \
    HighPrecision a;                                                    \
    a = HighPrecision::Invert (factor);                                 \
    HighPrecision b = V (factor);                                       \
    b.MulByInvert (a);                                                  \
    NS_TEST_ASSERT_MSG_EQ (b.GetInteger (), 1,                          \
                           "x * 1/x should be 1 for x=" << factor);     \
    HighPrecision c = V (1);                                            \
    c.MulByInvert (a);                                                  \
    NS_TEST_ASSERT_MSG_EQ (c.GetInteger (), 0,                          \
                           "1 * 1/x should be 0 for x=" << factor);     \
    HighPrecision d = V (1);                                            \
    d.Div (V(factor));                                                  \
    NS_TEST_ASSERT_MSG_EQ (d.GetDouble (), c.GetDouble (),              \
                           "1 * 1/x should be equal to 1/x for x=" << factor); \
    HighPrecision e = V (-factor);                                      \
    e.MulByInvert (a);                                                  \
    NS_TEST_ASSERT_MSG_EQ (e.GetInteger (), -1,                         \
                           "-x * 1/x should be -1 for x=" << factor);   \
  } while(false)

  TEST(2);
  TEST(3);
  TEST(4);
  TEST(5);
  TEST(6);
  TEST(10);
  TEST(99);
  TEST(100);
  TEST(1000);
  TEST(10000);
  TEST(100000);
  TEST(100000);
  TEST(1000000);
  TEST(10000000);
  TEST(100000000);
  TEST(1000000000);
  TEST(10000000000LL);
  TEST(100000000000LL);
  TEST(1000000000000LL);
  TEST(10000000000000LL);
  TEST(100000000000000LL);
  TEST(1000000000000000LL);
#undef TEST
  return GetErrorStatus ();
}


static class HighPrecisionTestSuite : public TestSuite
{
public:
  HighPrecisionTestSuite ()
    : TestSuite ("high-precision", UNIT)
  {
    AddTestCase (new HpArithmeticTestCase ());
    AddTestCase (new HpBug455TestCase ());
    AddTestCase (new HpBug863TestCase ());
    AddTestCase (new HpCompareTestCase ());
    AddTestCase (new HpInvertTestCase ());
  }
} g_highPrecisionTestSuite;

} // namespace ns3
