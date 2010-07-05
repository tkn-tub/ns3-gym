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

class Hp128ArithmeticTestCase : public TestCase
{
public:
  Hp128ArithmeticTestCase ();
  virtual bool DoRun (void);
};

Hp128ArithmeticTestCase::Hp128ArithmeticTestCase ()
  : TestCase ("Check basic arithmetic operations")
{
}
bool
Hp128ArithmeticTestCase::DoRun (void)
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

  return false;
}

class Hp128Bug455TestCase : public TestCase
{
public:
  Hp128Bug455TestCase ();
  virtual bool DoRun (void);
};

Hp128Bug455TestCase::Hp128Bug455TestCase ()
  : TestCase ("Test case for bug 455")
{
}
bool
Hp128Bug455TestCase::DoRun (void)
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

  return false;
}


class Hp128Bug863TestCase : public TestCase
{
public:
  Hp128Bug863TestCase ();
  virtual bool DoRun (void);
};

Hp128Bug863TestCase::Hp128Bug863TestCase ()
  : TestCase ("Test case for bug 863")
{
}
bool
Hp128Bug863TestCase::DoRun (void)
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

  return false;
}

static class HighPrecision128TestSuite : public TestSuite
{
public:
  HighPrecision128TestSuite ()
    : TestSuite ("high-precision-128", UNIT)
  {
    AddTestCase (new Hp128ArithmeticTestCase ());
    AddTestCase (new Hp128Bug455TestCase ());
    AddTestCase (new Hp128Bug863TestCase ());
  }
} g_highPrecision128TestSuite;

} // namespace ns3
