#include "ns3/high-precision-128.h"
#include "ns3/test.h"

namespace ns3
{

class HpFracTestCase : public TestCase
{
public:
  HpFracTestCase ();
  virtual void DoRun (void);
  void CheckFrac (int64_t hi, uint64_t lo);
};

void 
HpFracTestCase::CheckFrac (int64_t hi, uint64_t lo)
{
  HighPrecision tmp = HighPrecision (hi,lo);
  NS_TEST_EXPECT_MSG_EQ (tmp.GetHigh (), hi,
			 "High part does not match");
  NS_TEST_EXPECT_MSG_EQ (tmp.GetLow (), lo,
			 "Low part does not match");
}

HpFracTestCase::HpFracTestCase ()
  : TestCase ("Check that we can manipulate the high and low part of every number")
{
}
void
HpFracTestCase::DoRun (void)
{
  CheckFrac (1, 0);
  CheckFrac (1, 1);
  CheckFrac (-1, 0);
  CheckFrac (-1, 1);
}


class HpInputTestCase : public TestCase
{
public:
  HpInputTestCase ();
  virtual void DoRun (void);
  void CheckString (std::string str, int64_t hi, uint64_t lo);
};
HpInputTestCase::HpInputTestCase ()
  : TestCase ("Check that we parse HighPrecision numbers as strings")
{
}
void 
HpInputTestCase::CheckString (std::string str, int64_t hi, uint64_t lo)
{
  std::istringstream iss;
  iss.str (str);
  HighPrecision hp;
  iss >> hp;
  NS_TEST_EXPECT_MSG_EQ (hp.GetHigh (), hi, "High parts do not match for input string " << str);
  NS_TEST_EXPECT_MSG_EQ (hp.GetLow (), lo, "Low parts do not match for input string " << str);
}
void
HpInputTestCase::DoRun (void)
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

class HpInputOutputTestCase : public TestCase
{
public:
  HpInputOutputTestCase ();
  virtual void DoRun (void);
  void CheckString (std::string str);
};
HpInputOutputTestCase::HpInputOutputTestCase ()
  : TestCase ("Check that we can roundtrip HighPrecision numbers as strings")
{
}
void 
HpInputOutputTestCase::CheckString (std::string str)
{
  std::istringstream iss;
  iss.str (str);
  HighPrecision hp;
  iss >> hp;
  std::ostringstream oss;
  oss << hp;
  NS_TEST_EXPECT_MSG_EQ (oss.str (), str, "Converted string does not match expected string");
}
void
HpInputOutputTestCase::DoRun (void)
{
  CheckString ("+1.0");
  CheckString ("-1.0");
  CheckString ("+20.0");
  CheckString ("+1.08446744073709551615");
  CheckString ("-1.08446744073709551615");
  CheckString ("+1.18446744073709551615");
  CheckString ("-1.18446744073709551615");
}


static class HighPrecision128TestSuite : public TestSuite
{
public:
  HighPrecision128TestSuite ()
    : TestSuite ("high-precision-128", UNIT)
  {
    AddTestCase (new HpFracTestCase ());
    AddTestCase (new HpInputTestCase ());
    //AddTestCase (new HpOutputTestCase ());
    AddTestCase (new HpInputOutputTestCase ());
  }
} g_highPrecision128TestSuite;

} // namespace ns3
