#include "ns3/int64x64.h"
#include "ns3/test.h"

using namespace ns3;

class Int64x64FracTestCase : public TestCase
{
public:
  Int64x64FracTestCase ();
  virtual void DoRun (void);
  void CheckFrac (int64_t hi, uint64_t lo);
};

void 
Int64x64FracTestCase::CheckFrac (int64_t hi, uint64_t lo)
{
  int64x64_t tmp = int64x64_t (hi,lo);
  NS_TEST_EXPECT_MSG_EQ (tmp.GetHigh (), hi,
                         "High part does not match");
  NS_TEST_EXPECT_MSG_EQ (tmp.GetLow (), lo,
                         "Low part does not match");
}

Int64x64FracTestCase::Int64x64FracTestCase ()
  : TestCase ("Check that we can manipulate the high and low part of every number")
{
}
void
Int64x64FracTestCase::DoRun (void)
{
  CheckFrac (1, 0);
  CheckFrac (1, 1);
  CheckFrac (-1, 0);
  CheckFrac (-1, 1);
}


class Int64x64InputTestCase : public TestCase
{
public:
  Int64x64InputTestCase ();
  virtual void DoRun (void);
  void CheckString (std::string str, int64_t hi, uint64_t lo);
};
Int64x64InputTestCase::Int64x64InputTestCase ()
  : TestCase ("Check that we parse Int64x64 numbers as strings")
{
}
void 
Int64x64InputTestCase::CheckString (std::string str, int64_t hi, uint64_t lo)
{
  std::istringstream iss;
  iss.str (str);
  int64x64_t hp;
  iss >> hp;
  NS_TEST_EXPECT_MSG_EQ (hp.GetHigh (), hi, "High parts do not match for input string " << str);
  NS_TEST_EXPECT_MSG_EQ (hp.GetLow (), lo, "Low parts do not match for input string " << str);
}
void
Int64x64InputTestCase::DoRun (void)
{
  CheckString ("1", 1, 0);
  CheckString ("+1", 1, 0);
  CheckString ("-1", -1, 0);
  CheckString ("1.0", 1, 0);
  CheckString ("+1.0", 1, 0);
  CheckString ("001.0", 1, 0);
  CheckString ("+001.0", 1, 0);
  CheckString ("020.0", 20, 0);
  CheckString ("+020.0", 20, 0);
  CheckString ("-1.0", -1, 0);
  CheckString ("-1.0000", -1, 0);
  CheckString ("1.0000000", 1, 0);
  CheckString ("1.08446744073709551615", 1, 8446744073709551615LL);
  CheckString ("-1.08446744073709551615", -1, 8446744073709551615LL);
}

class Int64x64InputOutputTestCase : public TestCase
{
public:
  Int64x64InputOutputTestCase ();
  virtual void DoRun (void);
  void CheckString (std::string str);
};
Int64x64InputOutputTestCase::Int64x64InputOutputTestCase ()
  : TestCase ("Check that we can roundtrip Int64x64 numbers as strings")
{
}
void 
Int64x64InputOutputTestCase::CheckString (std::string str)
{
  std::istringstream iss;
  iss.str (str);
  int64x64_t value;
  iss >> value;
  std::ostringstream oss;
  oss << value;
  NS_TEST_EXPECT_MSG_EQ (oss.str (), str, "Converted string does not match expected string");
}
void
Int64x64InputOutputTestCase::DoRun (void)
{
  CheckString ("+1.0");
  CheckString ("-1.0");
  CheckString ("+20.0");
  CheckString ("+1.08446744073709551615");
  CheckString ("-1.08446744073709551615");
  CheckString ("+1.18446744073709551615");
  CheckString ("-1.18446744073709551615");
}

#define CHECK_EXPECTED(a,b) \
  NS_TEST_ASSERT_MSG_EQ ((a).GetHigh (),b,"Arithmetic failure: " << ((a).GetHigh ()) << "!=" << (b))

#define V(v) \
  int64x64_t (v)

class Int64x64ArithmeticTestCase : public TestCase
{
public:
  Int64x64ArithmeticTestCase ();
  virtual void DoRun (void);
};

Int64x64ArithmeticTestCase::Int64x64ArithmeticTestCase ()
  : TestCase ("Check basic arithmetic operations")
{
}
void
Int64x64ArithmeticTestCase::DoRun (void)
{
  int64x64_t a, b;

  CHECK_EXPECTED (V (1) - V (1), 0);
  CHECK_EXPECTED (V (1) - V (2), -1);
  CHECK_EXPECTED (V (1) - V (3), -2);
  CHECK_EXPECTED (V (1) - V (-1), 2);
  CHECK_EXPECTED (V (1) - V (-2), 3);
  CHECK_EXPECTED (V (-3) - V (-4), 1);
  CHECK_EXPECTED (V (-2) - V (3), -5);
  CHECK_EXPECTED (V (1) + V (2), 3);
  CHECK_EXPECTED (V (1) + V (-3), -2);
  CHECK_EXPECTED (V (0) + V (0), 0);
  CHECK_EXPECTED (V (0) * V (0), 0);
  CHECK_EXPECTED (V (0) * V (1), 0);
  CHECK_EXPECTED (V (0) * V (-1), 0);
  CHECK_EXPECTED (V (1) * V (0), 0);
  CHECK_EXPECTED (V (1) * V (1), 1);
  CHECK_EXPECTED (V (1) * V (-1), -1);
  CHECK_EXPECTED (V (-1) * V (-1), 1);
  CHECK_EXPECTED (V (0) * V (1), 0);
  CHECK_EXPECTED (V (0) * V (-1), 0);
  CHECK_EXPECTED (V (-1) * V (1), -1);


  CHECK_EXPECTED (V (2) * V (3) / V (3), 2);

  // Below, the division loses precision because 2/3 is not
  // representable exactly in 64.64 integers. So, we got
  // something super close but the final rounding kills us.
  a = V (2);
  b = V (3);
  a /= b;
  a *= b;
  CHECK_EXPECTED (V (2) / V (3) * V (3), 1);

  // The example below shows that we really do not lose
  // much precision internally: it is almost always the
  // final conversion which loses precision.
  CHECK_EXPECTED (V (2000000000) / V (3) * V (3), 1999999999);
}

/**
 * See \bugid{455}
 */
class Int64x64Bug455TestCase : public TestCase
{
public:
  Int64x64Bug455TestCase ();
  virtual void DoRun (void);
};

Int64x64Bug455TestCase::Int64x64Bug455TestCase ()
  : TestCase ("Test case for bug 455")
{
}
void
Int64x64Bug455TestCase::DoRun (void)
{
  int64x64_t a = int64x64_t (0.1);
  a /= int64x64_t (1.25);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 0.08, "The original testcase");
  a = int64x64_t (0.5);
  a *= int64x64_t (5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 2.5, "Simple test for multiplication");
  a = int64x64_t (-0.5);
  a *= int64x64_t (5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -2.5, "Test sign, first operation negative");
  a = int64x64_t (-0.5);
  a *=int64x64_t (-5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 2.5, "both operands negative");
  a = int64x64_t (0.5);
  a *= int64x64_t (-5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -2.5, "only second operand negative");
}

/**
 * See \bugid{863}
 */
class Int64x64Bug863TestCase : public TestCase
{
public:
  Int64x64Bug863TestCase ();
  virtual void DoRun (void);
};

Int64x64Bug863TestCase::Int64x64Bug863TestCase ()
  : TestCase ("Test case for bug 863")
{
}
void
Int64x64Bug863TestCase::DoRun (void)
{
  int64x64_t a = int64x64_t (0.9);
  a /= int64x64_t (1);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 0.9, "The original testcase");
  a = int64x64_t (0.5);
  a /= int64x64_t (0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 1.0, "Simple test for division");
  a = int64x64_t (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -0.5, "Check that we actually convert doubles correctly");
  a /= int64x64_t (0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -1.0, "first argument negative");
  a = int64x64_t (0.5);
  a /= int64x64_t (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -1.0, "second argument negative");
  a = int64x64_t (-0.5);
  a /= int64x64_t (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 1.0, "both arguments negative");
}

class Int64x64CompareTestCase : public TestCase
{
public:
  Int64x64CompareTestCase ();
  virtual void DoRun (void);
};
Int64x64CompareTestCase::Int64x64CompareTestCase ()
  : TestCase ("Check basic compare operations")
{
}
void
Int64x64CompareTestCase::DoRun (void)
{

  NS_TEST_ASSERT_MSG_EQ ((V (-1) < V (1)), true, "a is smaller than b");
  NS_TEST_ASSERT_MSG_EQ ((V (-1) > V (-2)), true, "a is bigger than b");
  NS_TEST_ASSERT_MSG_EQ ((V (-1) == V (-1)), true, "a is equal to b");

  NS_TEST_ASSERT_MSG_EQ ((V (1) > V (-1)), true, "a is bigger than b");
  NS_TEST_ASSERT_MSG_EQ ((V (1) < V (2)), true, "a is smaller than b");
}

class Int64x64InvertTestCase : public TestCase
{
public:
  Int64x64InvertTestCase ();
  virtual void DoRun (void);
};

Int64x64InvertTestCase::Int64x64InvertTestCase ()
  : TestCase ("Test case for invertion")
{
}

void
Int64x64InvertTestCase::DoRun (void)
{
#define TEST(factor)                                                    \
  do {                                                                  \
      int64x64_t a;                                                       \
      a = int64x64_t::Invert (factor);                                    \
      int64x64_t b = V (factor);                                          \
      b.MulByInvert (a);                                                  \
      NS_TEST_ASSERT_MSG_EQ (b.GetHigh (), 1,                             \
                             "x * 1/x should be 1 for x=" << factor);     \
      int64x64_t c = V (1);                                               \
      c.MulByInvert (a);                                                  \
      NS_TEST_ASSERT_MSG_EQ (c.GetHigh (), 0,                             \
                             "1 * 1/x should be 0 for x=" << factor);     \
      int64x64_t d = V (1);                                               \
      d /= (V (factor));                                                  \
      NS_TEST_ASSERT_MSG_EQ (d.GetDouble (), c.GetDouble (),              \
                             "1 * 1/x should be equal to 1/x for x=" << factor); \
      int64x64_t e = V (-factor);                                 \
      e.MulByInvert (a);                                                  \
      NS_TEST_ASSERT_MSG_EQ (e.GetHigh (), -1,                            \
                             "-x * 1/x should be -1 for x=" << factor);   \
    } \
  while(false)
  TEST (2);
  TEST (3);
  TEST (4);
  TEST (5);
  TEST (6);
  TEST (10);
  TEST (99);
  TEST (100);
  TEST (1000);
  TEST (10000);
  TEST (100000);
  TEST (100000);
  TEST (1000000);
  TEST (10000000);
  TEST (100000000);
  TEST (1000000000);
  TEST (10000000000LL);
  TEST (100000000000LL);
  TEST (1000000000000LL);
  TEST (10000000000000LL);
  TEST (100000000000000LL);
  TEST (1000000000000000LL);
#undef TEST
}



static class Int64x64128TestSuite : public TestSuite
{
public:
  Int64x64128TestSuite ()
    : TestSuite ("int64x64", UNIT)
  {
    AddTestCase (new Int64x64FracTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InputTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InputOutputTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64ArithmeticTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug455TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug863TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64CompareTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InvertTestCase (), TestCase::QUICK);
  }
} g_int64x64TestSuite;
