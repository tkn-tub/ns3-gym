#include "uint64x64.h"
#include <stdint.h>
#include <iostream>
#include <sstream>
#include "ns3/assert.h"

namespace ns3 {

static uint8_t MostSignificantDigit (uint64_t value)
{
  uint8_t n = 0;
  do
    {
      n++;
      value /= 10;
    } while (value != 0);
  return n;
}

static uint64_t PowerOfTen (uint8_t n)
{
  uint64_t retval = 1;
  while (n > 0)
    {
      retval *= 10;
      n--;
    }
  return retval;
}

std::ostream &operator << (std::ostream &os, const uint64x64_t &value)
{
  int64_t hi = value.GetHigh ();
  os << ((hi<0)?"-":"+") << ((hi<0)?-hi:hi) << ".";
  uint64_t low = value.GetLow ();
  uint8_t msd = MostSignificantDigit (~((uint64_t)0));
  do
    {
      msd--;
      uint64_t pow = PowerOfTen (msd);
      uint8_t digit = low / pow;
      NS_ASSERT (digit < 10);
      os << (uint16_t) digit;
      low -= digit * pow;
    } while (msd > 0 && low > 0);
  return os;
}

static uint64_t ReadDigits (std::string str)
{
  const char *buf = str.c_str ();
  uint64_t retval = 0;
  while (*buf != 0)
    {
      retval *= 10;
      retval += *buf - 0x30;
      buf++;
    }
  return retval;
}

std::istream &operator >> (std::istream &is, uint64x64_t &value)
{
  std::string str;

  is >> str;
  bool negative;
  // skip heading spaces
  std::string::size_type cur;
  cur = str.find_first_not_of (" ");
  std::string::size_type next;
  // first, remove the sign.
  next = str.find ("-", cur);
  if (next != std::string::npos)
    {
      negative = true;
      next++;
    }
  else
    {
      next = str.find ("+", cur);
      if (next != std::string::npos)
	{
	  next++;
	}
      else
	{
	  next = cur;
	}
      negative = false;
    }
  cur = next;
  int64_t hi;
  uint64_t lo;
  next = str.find(".", cur);
  if (next != std::string::npos)
    {
      hi = ReadDigits (str.substr (cur, next-cur));
      lo = ReadDigits (str.substr (next+1, str.size()-(next+1)));
    }
  else
    {
      hi = ReadDigits (str.substr (cur, str.size ()-cur));
      lo = 0;
    }
  hi = negative?-hi:hi;
  value = uint64x64_t (hi, lo);
  return is;
}

} // namespace ns3

#include "ns3/test.h"

namespace ns3
{

class Uint64x64FracTestCase : public TestCase
{
public:
  Uint64x64FracTestCase ();
  virtual bool DoRun (void);
  void CheckFrac (int64_t hi, uint64_t lo);
};

void 
Uint64x64FracTestCase::CheckFrac (int64_t hi, uint64_t lo)
{
  uint64x64_t tmp = uint64x64_t (hi,lo);
  NS_TEST_EXPECT_MSG_EQ (tmp.GetHigh (), hi,
			 "High part does not match");
  NS_TEST_EXPECT_MSG_EQ (tmp.GetLow (), lo,
			 "Low part does not match");
}

Uint64x64FracTestCase::Uint64x64FracTestCase ()
  : TestCase ("Check that we can manipulate the high and low part of every number")
{
}
bool
Uint64x64FracTestCase::DoRun (void)
{
  CheckFrac (1, 0);
  CheckFrac (1, 1);
  CheckFrac (-1, 0);
  CheckFrac (-1, 1);
  return GetErrorStatus ();
}


class Uint64x64InputTestCase : public TestCase
{
public:
  Uint64x64InputTestCase ();
  virtual bool DoRun (void);
  void CheckString (std::string str, int64_t hi, uint64_t lo);
};
Uint64x64InputTestCase::Uint64x64InputTestCase ()
  : TestCase ("Check that we parse Uint64x64 numbers as strings")
{
}
void 
Uint64x64InputTestCase::CheckString (std::string str, int64_t hi, uint64_t lo)
{
  std::istringstream iss;
  iss.str (str);
  uint64x64_t hp;
  iss >> hp;
  NS_TEST_EXPECT_MSG_EQ (hp.GetHigh (), hi, "High parts do not match for input string " << str);
  NS_TEST_EXPECT_MSG_EQ (hp.GetLow (), lo, "Low parts do not match for input string " << str);
}
bool
Uint64x64InputTestCase::DoRun (void)
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
  
  return GetErrorStatus ();
}

class Uint64x64InputOutputTestCase : public TestCase
{
public:
  Uint64x64InputOutputTestCase ();
  virtual bool DoRun (void);
  void CheckString (std::string str);
};
Uint64x64InputOutputTestCase::Uint64x64InputOutputTestCase ()
  : TestCase ("Check that we can roundtrip Uint64x64 numbers as strings")
{
}
void 
Uint64x64InputOutputTestCase::CheckString (std::string str)
{
  std::istringstream iss;
  iss.str (str);
  uint64x64_t value;
  iss >> value;
  std::ostringstream oss;
  oss << value;
  NS_TEST_EXPECT_MSG_EQ (oss.str (), str, "Converted string does not match expected string");
}
bool
Uint64x64InputOutputTestCase::DoRun (void)
{
  CheckString ("+1.0");
  CheckString ("-1.0");
  CheckString ("+20.0");
  CheckString ("+1.08446744073709551615");
  CheckString ("-1.08446744073709551615");
  CheckString ("+1.18446744073709551615");
  CheckString ("-1.18446744073709551615");
  
  return GetErrorStatus ();
}

#define CHECK_EXPECTED(a,b) \
  NS_TEST_ASSERT_MSG_EQ ((a).GetHigh (),b,"Arithmetic failure: " << ((a).GetHigh ()) << "!=" << (b))

#define V(v) \
  uint64x64_t (v)

class Uint64x64ArithmeticTestCase : public TestCase
{
public:
  Uint64x64ArithmeticTestCase ();
  virtual bool DoRun (void);
};

Uint64x64ArithmeticTestCase::Uint64x64ArithmeticTestCase ()
  : TestCase ("Check basic arithmetic operations")
{
}
bool
Uint64x64ArithmeticTestCase::DoRun (void)
{
  uint64x64_t a, b;

  CHECK_EXPECTED (V(1) - V(1), 0);
  CHECK_EXPECTED (V(1) - V(2), -1);
  CHECK_EXPECTED (V(1) - V(3), -2);
  CHECK_EXPECTED (V(1) - V(-1), 2);
  CHECK_EXPECTED (V(1) - V(-2), 3);
  CHECK_EXPECTED (V(-3) - V(-4), 1);
  CHECK_EXPECTED (V(-2) - V(3), -5);
  CHECK_EXPECTED (V(1) + V(2), 3);
  CHECK_EXPECTED (V(1) + V(-3), -2);
  CHECK_EXPECTED (V(0) + V(0), 0);
  CHECK_EXPECTED (V(0) * V(0), 0);
  CHECK_EXPECTED (V(0) * V(1), 0);
  CHECK_EXPECTED (V(0) * V(-1), 0);
  CHECK_EXPECTED (V(1) * V(0), 0);
  CHECK_EXPECTED (V(1) * V(1), 1);
  CHECK_EXPECTED (V(1) * V(-1), -1);
  CHECK_EXPECTED (V(-1) * V(-1), 1);
  CHECK_EXPECTED (V(0) * V(1), 0);
  CHECK_EXPECTED (V(0) * V(-1), 0);
  CHECK_EXPECTED (V(-1) * V(1), -1);


  CHECK_EXPECTED (V (2) * V(3) / V(3), 2);

  // Below, the division loses precision because 2/3 is not
  // representable exactly in 64.64 integers. So, we got
  // something super close but the final rounding kills us.
  CHECK_EXPECTED (V(2) / V(3) * V(3), 1);

  // The example below shows that we really do not lose
  // much precision internally: it is almost always the
  // final conversion which loses precision.
  CHECK_EXPECTED (V (2000000000) / V(3) * V(3), 1999999999);

  return GetErrorStatus ();
}

class Uint64x64Bug455TestCase : public TestCase
{
public:
  Uint64x64Bug455TestCase ();
  virtual bool DoRun (void);
};

Uint64x64Bug455TestCase::Uint64x64Bug455TestCase ()
  : TestCase ("Test case for bug 455")
{
}
bool
Uint64x64Bug455TestCase::DoRun (void)
{
  uint64x64_t a = uint64x64_t (0.1);
  a /= uint64x64_t (1.25);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 0.08, "The original testcase");
  a = uint64x64_t (0.5);
  a *= uint64x64_t (5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 2.5, "Simple test for multiplication");
  a = uint64x64_t (-0.5);
  a *= uint64x64_t (5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -2.5, "Test sign, first operation negative");
  a = uint64x64_t (-0.5);
  a *=uint64x64_t (-5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 2.5, "both operands negative");
  a = uint64x64_t (0.5);
  a *= uint64x64_t (-5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -2.5, "only second operand negative");

  return GetErrorStatus ();
}

class Uint64x64Bug863TestCase : public TestCase
{
public:
  Uint64x64Bug863TestCase ();
  virtual bool DoRun (void);
};

Uint64x64Bug863TestCase::Uint64x64Bug863TestCase ()
  : TestCase ("Test case for bug 863")
{
}
bool
Uint64x64Bug863TestCase::DoRun (void)
{
  uint64x64_t a = uint64x64_t (0.9);
  a /= uint64x64_t (1);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 0.9, "The original testcase");
  a = uint64x64_t (0.5);
  a /= uint64x64_t (0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 1.0, "Simple test for division");
  a = uint64x64_t (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -0.5, "Check that we actually convert doubles correctly");
  a /= uint64x64_t (0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -1.0, "first argument negative");
  a = uint64x64_t (0.5);
  a /= uint64x64_t (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), -1.0, "second argument negative");
  a = uint64x64_t (-0.5);
  a /= uint64x64_t (-0.5);
  NS_TEST_ASSERT_MSG_EQ (a.GetDouble (), 1.0, "both arguments negative");

  return GetErrorStatus ();
}

class Uint64x64CompareTestCase : public TestCase
{
public:
  Uint64x64CompareTestCase ();
  virtual bool DoRun (void);
};
Uint64x64CompareTestCase::Uint64x64CompareTestCase ()
  : TestCase ("Check basic compare operations")
{
}
bool
Uint64x64CompareTestCase::DoRun (void)
{

  NS_TEST_ASSERT_MSG_EQ ((V(-1) < V(1)), true, "a is smaller than b");
  NS_TEST_ASSERT_MSG_EQ ((V(-1) > V(-2)), true, "a is bigger than b");
  NS_TEST_ASSERT_MSG_EQ ((V(-1) == V(-1)), true, "a is equal to b");

  NS_TEST_ASSERT_MSG_EQ ((V(1) > V(-1)), true, "a is bigger than b");
  NS_TEST_ASSERT_MSG_EQ ((V(1) < V(2)), true, "a is smaller than b");

  return GetErrorStatus ();
}

class Uint64x64InvertTestCase : public TestCase
{
public:
  Uint64x64InvertTestCase ();
  virtual bool DoRun (void);
};

Uint64x64InvertTestCase::Uint64x64InvertTestCase ()
  : TestCase ("Test case for invertion")
{
}

bool
Uint64x64InvertTestCase::DoRun (void)
{
#define TEST(factor)                                                    \
  do {                                                                  \
    uint64x64_t a;							\
    a = uint64x64_t::Invert (factor);					\
    uint64x64_t b = V (factor);						\
    b.MulByInvert (a);                                                  \
    NS_TEST_ASSERT_MSG_EQ (b.GetHigh (), 1,				\
			   "x * 1/x should be 1 for x=" << factor);     \
    uint64x64_t c = V (1);						\
    c.MulByInvert (a);                                                  \
    NS_TEST_ASSERT_MSG_EQ (c.GetHigh (), 0,				\
			   "1 * 1/x should be 0 for x=" << factor);     \
    uint64x64_t d = V (1);						\
    d /= (V(factor));                                                  \
    NS_TEST_ASSERT_MSG_EQ (d.GetDouble (), c.GetDouble (),              \
			   "1 * 1/x should be equal to 1/x for x=" << factor); \
    uint64x64_t e = V (-factor);					\
    e.MulByInvert (a);                                                  \
    NS_TEST_ASSERT_MSG_EQ (e.GetHigh (), -1,				\
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



static class Uint64x64128TestSuite : public TestSuite
{
public:
  Uint64x64128TestSuite ()
    : TestSuite ("uint64x64", UNIT)
  {
    AddTestCase (new Uint64x64FracTestCase ());
    AddTestCase (new Uint64x64InputTestCase ());
    AddTestCase (new Uint64x64InputOutputTestCase ());
    AddTestCase (new Uint64x64ArithmeticTestCase ());
    AddTestCase (new Uint64x64Bug455TestCase ());
    AddTestCase (new Uint64x64Bug863TestCase ());
    AddTestCase (new Uint64x64CompareTestCase ());
    AddTestCase (new Uint64x64InvertTestCase ());
  }
} g_uint64x64TestSuite;

} // namespace ns3
