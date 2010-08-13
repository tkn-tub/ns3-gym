#include "high-precision-128.h"
#include "ns3/abort.h"
#include "ns3/assert.h"
#include <math.h>
#ifdef COUNT_OPS
#include <iostream>
#endif

namespace ns3 {

#ifdef COUNT_OPS
uint128_t HighPrecision::g_nAdd = 0;
uint128_t HighPrecision::g_nMuli = 0;
uint128_t HighPrecision::g_nMul = 0;
uint128_t HighPrecision::g_nDiv = 0;
uint128_t HighPrecision::g_nCmp = 0;
HighPrecision::Printer  HighPrecision::g_printer;
HighPrecision::Printer::~Printer ()
{
  std::cout << "add=" << (double)g_nAdd << " mul=" << (double)g_nMul << " div=" << (double)g_nDiv
	    << " muli=" << (double)g_nMuli << " cmp=" << (double)g_nCmp;
}
#endif

#define OUTPUT_SIGN(sa,sb,ua,ub)					\
  ({bool negA, negB;							\
    negA = sa < 0;							\
    negB = sb < 0;							\
    ua = negA?-sa:sa;							\
    ub = negB?-sb:sb;							\
    (negA && !negB) || (!negA && negB);})


#define MASK_LO ((((uint128_t)1)<<64)-1)
#define MASK_HI (~MASK_LO)
HighPrecision::HighPrecision (int64_t high, uint64_t low)
{
  bool is_negative = high<0;
  m_value = is_negative?-high:high;
  m_value <<= 64;
  m_value += low;
  m_value = is_negative?-m_value:m_value;
}
int64_t
HighPrecision::GetHigh (void) const
{
  return GetInteger ();
}
uint64_t 
HighPrecision::GetLow (void) const
{
  bool negative = m_value < 0;
  int128_t v = negative?-m_value:m_value;
  int128_t low = v & MASK_LO;
  uint64_t retval = low;
  return retval;
}

void
HighPrecision::Mul (HighPrecision const &o)
{
  bool negResult;
  uint128_t a, b;
  negResult = OUTPUT_SIGN (m_value, o.m_value, a, b);
  int128_t result = Umul (a, b);
  // add the sign to the result
  result = negResult ? -result : result;
  m_value = result;
}

uint128_t
HighPrecision::Umul (uint128_t a, uint128_t b)
{
  INC_MUL;
  uint128_t aL = a & MASK_LO;
  uint128_t bL = b & MASK_LO;
  uint128_t aH = (a >> 64) & MASK_LO;
  uint128_t bH = (b >> 64) & MASK_LO;

  uint128_t result;
  uint128_t hiPart,loPart,midPart;

  // Multiplying (a.h 2^64 + a.l) x (b.h 2^64 + b.l) =
  //			2^128 a.h b.h + 2^64*(a.h b.l+b.h a.l) + a.l b.l
  // get the low part a.l b.l
  // multiply the fractional part
  loPart = aL * bL;
  // compute the middle part 2^64*(a.h b.l+b.h a.l)
  midPart = aL * bH + aH * bL;
  // truncate the low part
  result = (loPart >> 64) + (midPart & MASK_LO);
  // compute the high part 2^128 a.h b.h
  hiPart = aH * bH;
  // truncate the high part and only use the low part
  result |= ((hiPart & MASK_LO) << 64) + (midPart & MASK_HI);
  // if the high part is not zero, put a warning
  NS_ABORT_MSG_IF ((hiPart & MASK_HI) != 0,
		   "High precision 128 bits multiplication error: multiplication overflow.");
  return result;
}
void
HighPrecision::Div (HighPrecision const &o)
{
  bool negResult;
  uint128_t a, b;
  negResult = OUTPUT_SIGN (m_value, o.m_value, a, b);  
  int128_t result = Divu (a, b);
  result = negResult ? -result:result;
  m_value = result;
}

uint128_t
HighPrecision::Divu (uint128_t a, uint128_t b)
{
  INC_DIV;
  uint128_t quo = a / b;
  uint128_t rem = (a % b);
  uint128_t result = quo << 64;
  // Now, manage the remainder
  uint128_t tmp = rem >> 64;
  uint128_t div;
  if (tmp == 0)
    {
      rem = rem << 64;
      div = b;
    }
  else
    {
      rem = rem;
      div = b >> 64;
    }
  quo = rem / div;
  result = result + quo;
  return result;
}

void 
HighPrecision::MulByInvert (const HighPrecision &o)
{
  bool negResult = m_value < 0;
  uint128_t a = negResult?-m_value:m_value;
  uint128_t result = UmulByInvert (a, o.m_value);

  m_value = negResult?-result:result;
}
uint128_t
HighPrecision::UmulByInvert (uint128_t a, uint128_t b)
{
  INC_MULI;
  uint128_t result, ah, bh, al, bl;
  uint128_t hi, mid;
  ah = a >> 64;
  bh = b >> 64;
  al = a & MASK_LO;
  bl = b & MASK_LO;
  hi = ah * bh;
  mid = ah * bl + al * bh;
  mid >>= 64;
  result = ah * bh + mid;
  return result;
}
HighPrecision 
HighPrecision::Invert (uint64_t v)
{
  NS_ASSERT (v > 1);
  uint128_t a;
  a = 1;
  a <<= 64;
  HighPrecision result;
  result.m_value = Divu (a, v);
  HighPrecision tmp = HighPrecision (v, false);
  tmp.MulByInvert (result);
  if (tmp.GetInteger () != 1)
    {
      result.m_value += 1;
    }
  return result;
}

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

static uint128_t PowerOfTen (uint8_t n)
{
  uint128_t retval = 1;
  while (n > 0)
    {
      retval *= 10;
      n--;
    }
  return retval;
}

std::ostream &operator << (std::ostream &os, const HighPrecision &hp)
{
  int64_t hi = hp.GetHigh ();
  os << ((hi<0)?"-":"+") << ((hi<0)?-hi:hi) << ".";
  uint128_t low = hp.GetLow ();
  uint8_t msd = MostSignificantDigit (~((uint64_t)0));
  do
    {
      msd--;
      uint128_t pow = PowerOfTen (msd);
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

std::istream &operator >> (std::istream &is, HighPrecision &hp)
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
  hp = HighPrecision (hi, lo);
  return is;
}

} // namespace ns3

#include "ns3/test.h"

namespace ns3
{

class HpFracTestCase : public TestCase
{
public:
  HpFracTestCase ();
  virtual bool DoRun (void);
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
bool
HpFracTestCase::DoRun (void)
{
  CheckFrac (1, 0);
  CheckFrac (1, 1);
  CheckFrac (-1, 0);
  CheckFrac (-1, 1);
  return GetErrorStatus ();
}


class HpInputTestCase : public TestCase
{
public:
  HpInputTestCase ();
  virtual bool DoRun (void);
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
bool
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
  
  return GetErrorStatus ();
}

class HpInputOutputTestCase : public TestCase
{
public:
  HpInputOutputTestCase ();
  virtual bool DoRun (void);
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
bool
HpInputOutputTestCase::DoRun (void)
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
