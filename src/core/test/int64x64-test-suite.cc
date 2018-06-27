/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 INRIA
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
 */

#include "ns3/int64x64.h"
#include "ns3/test.h"
#include "ns3/valgrind.h"  // Bug 1882

#include <cmath>    // fabs
#include <iomanip>
#include <limits>   // numeric_limits<>::epsilon ()

using namespace ns3;

namespace ns3 {

  namespace int64x64 {

    namespace test {
  
/**
 * \class ns3::int64x64::Int64x64TestSuite
 * \internal
 *
 * Some of these tests are a little unusual for ns-3 in that they
 * are sensitive to implementation, specifically the resolution
 * of the double and long double implementations.
 * 
 * To handle this, where needed we define a tolerance to use in the
 * test comparisons.  If you need to increase the tolerance,
 * please append the system and compiler version.  For example:
 *
 * \code
 *   // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
 *   tolerance = 1;
 *   // System Foo gcc 3.9
 *   tolerance = 3;
 * \endcode
 */


/**
 * Pretty printer for test cases.
 */
class Printer
{
public:
  /**
   * Construct from high and low words of Q64.64 representation.
   *
   * \param [in] high The integer portion.
   * \param [in] low The fractional portion.
   */
  Printer (const int64_t high, const uint64_t low)
    : m_haveInt (false),
      m_value (0),
      m_high (high),
      m_low (low)
  { }

  /**
   * Construct from an \c int64x64_t Q64.64 value.
   *
   * \param [in] value The value.
   */
  Printer (const int64x64_t value)
    : m_haveInt (true),
      m_value (value),
      m_high (value.GetHigh ()),
      m_low (value.GetLow ())
  { }

private:
  /**
   * Output streamer, the main reason for this class.
   *
   * \param [in] os The stream.
   * \param [in] p The value to print.
   * \returns The stream.
   */
  friend std::ostream & operator << (std::ostream & os, const Printer & p);

  bool       m_haveInt;  /**< Do we have a full int64x64_t value? */
  int64x64_t m_value;    /**< The int64x64_t value. */
  int64_t    m_high;     /**< The high (integer) word. */
  uint64_t   m_low;      /**< The low (fractional) word. */
};

std::ostream & operator << (std::ostream & os, const Printer & p)
{
  if (p.m_haveInt)
    {
      os << std::fixed << std::setprecision (22)
	 << p.m_value;

    }
  
  os << std::hex  << std::setfill ('0')
     << " (0x" << std::setw (16) << p.m_high
     << " 0x"  << std::setw (16) << p.m_low  << ")"
     << std::dec  << std::setfill (' ');
  return os;
}


class Int64x64HiLoTestCase : public TestCase
{
public:
  Int64x64HiLoTestCase ();
  virtual void DoRun (void);
  void Check (const int64_t hi, const uint64_t lo);
};

Int64x64HiLoTestCase::Int64x64HiLoTestCase ()
  : TestCase ("Manipulate the high and low part of every number")
{
}

void 
Int64x64HiLoTestCase::Check (const int64_t hi, const uint64_t lo)
{
  uint64_t tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 1;
    }

  int64x64_t value = int64x64_t (hi,lo);
  uint64_t vLow = value.GetLow ();
  bool pass = ( (value.GetHigh () == hi)
		&& ( (Max (vLow, lo) - Min (vLow, lo)) <= tolerance)
		);

  std::cout << GetParent ()->GetName () << " Check: "
	    << (pass ? "pass " : "FAIL ")
	    << Printer (value) << " from" << Printer (hi, lo)
	    << std::endl;

  NS_TEST_EXPECT_MSG_EQ (value.GetHigh (), hi,
			 "High part does not match for hi:" << hi
			 << " lo: " << lo);
  NS_TEST_EXPECT_MSG_EQ_TOL ((int64_t)vLow, (int64_t)lo, (int64_t)tolerance,
			     "Low part does not match for hi: " << hi
			     << " lo: " << lo);
}

void
Int64x64HiLoTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Check: " << GetName ()
	    << std::endl;

  uint64_t low = 1;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      low = static_cast<uint64_t> (HP_MAX_64 * std::numeric_limits<long double>::epsilon ());
    }

  Check ( 0, 0);
  Check ( 0, low);
  Check ( 0, 0xffffffffffffffffULL - low);
  
  Check ( 1, 0);
  Check ( 1, low);
  Check ( 1, 0xffffffffffffffffULL - low);
  
  Check (-1, 0);
  Check (-1, low);
  Check (-1, 0xffffffffffffffffULL - low);
}


class Int64x64InputTestCase : public TestCase
{
public:
  Int64x64InputTestCase ();
  virtual void DoRun (void);
  void Check (const std::string & str,
	      const int64_t hi, const uint64_t lo,
	      const int64_t tolerance = 0);
};
Int64x64InputTestCase::Int64x64InputTestCase ()
  : TestCase ("Parse int64x64_t numbers as strings")
{
}
void 
Int64x64InputTestCase::Check (const std::string & str,
			      const int64_t hi, const uint64_t lo,
			      const int64_t tolerance /* = 0 */)
			      
{
  std::istringstream iss;
  iss.str (str);
  int64x64_t value;
  iss >> value;

  std::string input = "\"" + str + "\"";
  uint64_t vLow = value.GetLow ();
  bool pass = ( (value.GetHigh () == hi) &&
		( Max (vLow, lo) - Min (vLow, lo) <= tolerance)
		);

  std::cout << GetParent ()->GetName () << " Input: "
	    << (pass ? "pass " : "FAIL ")
	    << std::left << std::setw (28) << input << std::right
	    << Printer (value) 
	    << " expected: " << Printer (hi, lo) << " +/- " << tolerance
	    << std::endl;

  NS_TEST_EXPECT_MSG_EQ (value.GetHigh (), hi,
			 "High parts do not match for input string \""
			 << str << "\"");
  NS_TEST_EXPECT_MSG_EQ_TOL ((int64_t)value.GetLow (), (int64_t)lo, tolerance,
			     "Low parts do not match for input string \""
			     << str << "\"");
}
void
Int64x64InputTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Input: " << GetName ()
	    << std::endl;

  int64_t tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 2;
    }
  
  Check ("1", 1, 0);
  Check ("+1", 1, 0);
  Check ("-1", -1, 0);
  Check ("1.0", 1, 0);
  Check ("+1.0", 1, 0);
  Check ("001.0", 1, 0);
  Check ("+001.0", 1, 0);
  Check ("020.0", 20, 0);
  Check ("+020.0", 20, 0);
  Check ("1.0000000", 1, 0);
  Check ("-1.0", -1, 0, tolerance);
  Check ("-1.0000", -1, 0, tolerance);
  Check (" 1.000000000000000000054",  1, 1, tolerance);
  Check ("-1.000000000000000000054", (int64_t)-2, (uint64_t)-1, tolerance);
}


class Int64x64InputOutputTestCase : public TestCase
{
public:
  Int64x64InputOutputTestCase ();
  virtual void DoRun (void);
  void Check (const std::string & str,
	      const int64_t tolerance = 0);
};
Int64x64InputOutputTestCase::Int64x64InputOutputTestCase ()
  : TestCase ("Roundtrip int64x64_t numbers as strings")
{
}
void 
Int64x64InputOutputTestCase::Check (const std::string & str,
				    const int64_t tolerance /* = 0 */)
{
  std::stringstream iss (str);
  int64x64_t expect;
  iss >> expect;

  std::stringstream oss;
  oss << std::scientific << std::setprecision (21) << expect;
  int64x64_t value;
  oss >> value;

  bool pass = Abs (value - expect) <= int64x64_t (0, tolerance + 1);

  std::string input  = "\"" + str + "\"";
  std::string output = "\"" + oss.str () + "\"";

  if (pass)
    {
      std::cout << GetParent ()->GetName () << " InputOutput: "
		<< (pass ? "pass " : "FAIL ")
		<< " in:  " << std::left << std::setw (28) << input
		<< " out: " << std::left << std::setw (28) << output
		<< std::right
		<< std::endl;
    }
  else
    {
      std::cout << GetParent ()->GetName () << " InputOutput: "
		<< (pass ? "pass " : "FAIL ")
		<< " in:  " << std::left << std::setw (28) << input
		<< std::right << Printer (expect)
		<< std::endl;
      std::cout << GetParent ()->GetName ()
		<< std::setw (19) << " "
		<< " out: " << std::left << std::setw (28) << output
		<< std::right << Printer (value)
		<< std::endl;
    }      

  NS_TEST_EXPECT_MSG_EQ_TOL (value, expect, int64x64_t (0, tolerance),
			     "Converted string does not match expected string");
}

void
Int64x64InputOutputTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " InputOutput: " << GetName ()
	    << std::endl;

  int64_t tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 1;
    }
  
  Check ("+1.000000000000000000000");
  Check ("+20.000000000000000000000");
  Check ("+0.000000000000000000000", tolerance);
  Check ("-1.000000000000000000000", tolerance);
  Check ("+1.084467440737095516158", tolerance);
  Check ("-2.084467440737095516158", tolerance);
  Check ("+3.184467440737095516179", tolerance);
  Check ("-4.184467440737095516179", tolerance);
}


class Int64x64ArithmeticTestCase : public TestCase
{
public:
  Int64x64ArithmeticTestCase ();
  virtual void DoRun (void);
  void Check (const int        test,
	      const int64x64_t value, const int64x64_t expect);
  void Check (const int        test,
	      const int64x64_t value, const int64x64_t expect,
	      const int64x64_t tolerance);
};

Int64x64ArithmeticTestCase::Int64x64ArithmeticTestCase ()
  : TestCase ("Basic arithmetic operations")
{
}
void
Int64x64ArithmeticTestCase::Check (const int test,
				   const int64x64_t value,
				   const int64x64_t expect)
{
  int64x64_t zero (0,0);
  Check (test, value, expect, zero);
}
void
Int64x64ArithmeticTestCase::Check (const int test,
				   const int64x64_t value,
				   const int64x64_t expect,
				   const int64x64_t tolerance)
{
  bool pass = Abs (value - expect) <= tolerance;
  
  std::cout << GetParent ()->GetName () << " Arithmetic: "
	    << (pass ? "pass " : "FAIL ")
	    << test << ": " << value << " == " << expect
	    << " (+/- " << tolerance << ")"
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ_TOL ( value, expect, tolerance,
			      "Arithmetic failure in test case " << test);
}

void
Int64x64ArithmeticTestCase::DoRun (void)
{
  const int64x64_t tol1 (0, 1);
  const int64x64_t zero (0, 0);
  const int64x64_t one  (1, 0);
  const int64x64_t two  (2, 0);
  const int64x64_t thre (3, 0);

  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Arithmetic: " << GetName ()
	    << std::endl;
  
  Check ( 0,   zero  -   zero ,   zero  );
  Check ( 1,   zero  -   one ,   -one   );
  Check ( 2,   one   -   one ,    zero  );
  Check ( 3,   one   -   two ,   -one   );
  Check ( 4,   one   - (-one ),   two   );
  Check ( 5, (-one ) - (-two ),   one   );
  Check ( 6, (-one ) -   two ,   -thre  );
  
  Check ( 7,   zero  +   zero ,   zero  );
  Check ( 8,   zero  +   one ,    one   );
  Check ( 9,   one   +   one ,    two   );
  Check (10,   one   +   two ,    thre  );
  Check (11,   one   + (-one ),   zero  );
  Check (12, (-one ) + (-two ),  -thre  );
  Check (13, (-one ) +   two ,    one   );
  
  Check (14,   zero  *   zero ,   zero  );
  Check (15,   zero  *   one ,    zero  );
  Check (16,   zero  * (-one ),   zero  );
  Check (17,   one   *   one ,    one   );
  Check (18,   one   * (-one ),  -one   );
  Check (19, (-one ) * (-one ),   one   );
  
  Check (20,  (two  * thre ) / thre , two  );

  const int64x64_t frac  = int64x64_t (0, 0xc000000000000000ULL);  // 0.75
  const int64x64_t fplf2 = frac + frac * frac;  // 1.3125
  
  Check (21,   frac,              0.75);
  Check (22,   fplf2,             1.3125);
  
  const int64x64_t zerof = zero + frac;
  const int64x64_t onef  = one  + frac;
  const int64x64_t twof  = two  + frac;
  const int64x64_t thref = thre + frac;

  Check (23,   zerof,             frac);
  
  
  Check (24,   zerof -   zerof,   zero );
  Check (25,   zerof -   onef,   -one  );
  Check (26,   onef  -   onef,    zero );
  Check (27,   onef  -   twof,   -one  );
  Check (28,   onef  - (-onef),   twof  + frac  );
  Check (29 , (-onef) - (-twof),   one  );
  Check (30 , (-onef) -   twof,   -thref - frac  );
  
  Check (31,   zerof +   zerof,   zerof + frac  );
  Check (32,   zerof +   onef,    onef  + frac  );
  Check (33,   onef  +   onef,    twof  + frac  );
  Check (34,   onef  +   twof,    thref + frac  );
  Check (35,   onef  + (-onef),   zero  );
  Check (36 , (-onef) + (-twof),  -thref - frac  );
  Check (37, (-onef) +   twof,    one   );
  
  Check (38,   zerof *   zerof,   frac  * frac  );
  Check (39 ,   zero  *   onef,    zero  );
  Check (40 ,   zerof *   one,     frac  );
  
  Check (41,   zerof *   onef,    fplf2 );
  Check (42,   zerof * (-onef),  -fplf2 );
  Check (43,   onef  *   onef,    onef  + fplf2 );
  Check (44,   onef  * (-onef),  -onef  - fplf2 );
  Check (45, (-onef) * (-onef),   onef  + fplf2 );
  
  
  // Multiplication followed by division is exact:
  Check (46, (two  * thre ) / thre , two  );
  Check (47, (twof * thref) / thref, twof );

  // Division followed by multiplication loses a bit or two:
  Check (48, (two  / thre)  * thre,  two , 2 * tol1 );
  Check (49, (twof / thref) * thref, twof, 3 * tol1 );

  // The example below shows that we really do not lose
  // much precision internally: it is almost always the
  // final conversion which loses precision.
  Check (50, (int64x64_t (2000000000) / int64x64_t (3)) * int64x64_t (3),
	      int64x64_t (1999999999, 0xfffffffffffffffeULL));

  // Check special values
  Check (51,  int64x64_t (0, 0x159fa87f8aeaad21ULL) * 10,
	           int64x64_t (0, 0xd83c94fb6d2ac34aULL));
  
}



/**
 * See \bugid{455}
 */
class Int64x64Bug455TestCase : public TestCase
{
public:
  Int64x64Bug455TestCase ();
  virtual void DoRun (void);
  void Check (const double result, const double expect,
	      const std::string & msg);
};

Int64x64Bug455TestCase::Int64x64Bug455TestCase ()
  : TestCase ("Test case for bug 455")
{
}
void
Int64x64Bug455TestCase::Check (const double result, const double expect,
			       const std::string & msg)
{
  bool pass = result == expect;
  
  std::cout << GetParent ()->GetName () << " Bug 455: "
	    << (pass ? "pass " : "FAIL ")
	    << "res: "  << result
	    << " exp: " << expect
	    << ": " << msg
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ (result, expect, msg);
}

void
Int64x64Bug455TestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Bug 455: " << GetName ()
	    << std::endl;
  
  int64x64_t a = int64x64_t (0.1);
  a /= int64x64_t (1.25);
  Check (a.GetDouble (), 0.08, "The original testcase");
  
  a = int64x64_t (0.5);
  a *= int64x64_t (5);
  Check (a.GetDouble (), 2.5, "Simple test for multiplication");
  
  a = int64x64_t (-0.5);
  a *= int64x64_t (5);
  Check (a.GetDouble (), -2.5, "Test sign, first operation negative");
  
  a = int64x64_t (-0.5);
  a *=int64x64_t (-5);
  Check (a.GetDouble (), 2.5, "both operands negative");
  
  a = int64x64_t (0.5);
  a *= int64x64_t (-5);
  Check (a.GetDouble (), -2.5, "only second operand negative");
}



/**
 * See \bugid{863}
 */
class Int64x64Bug863TestCase : public TestCase
{
public:
  Int64x64Bug863TestCase ();
  virtual void DoRun (void);
  void Check (const double result, const double expect,
	      const std::string & msg);
};

Int64x64Bug863TestCase::Int64x64Bug863TestCase ()
  : TestCase ("Test case for bug 863")
{
}
void
Int64x64Bug863TestCase::Check (const double result, const double expect,
			       const std::string & msg)
{
  bool pass = result == expect;
  
  std::cout << GetParent ()->GetName () << " Bug 863: "
	    << (pass ? "pass " : "FAIL ")
	    << "res: "  << result
	    << " exp: " << expect
	    << ": " << msg
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ (result, expect, msg);
}

void
Int64x64Bug863TestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Bug 863: " << GetName ()
	    << std::endl;
  
  int64x64_t a = int64x64_t (0.9);
  a /= int64x64_t (1);
  Check (a.GetDouble (), 0.9, "The original testcase");
  
  a = int64x64_t (0.5);
  a /= int64x64_t (0.5);
  Check (a.GetDouble (), 1.0, "Simple test for division");
  
  a = int64x64_t (-0.5);
  Check (a.GetDouble (), -0.5, "Check that we actually convert doubles correctly");
  
  a /= int64x64_t (0.5);
  Check (a.GetDouble (), -1.0, "first argument negative");
  
  a = int64x64_t (0.5);
  a /= int64x64_t (-0.5);
  Check (a.GetDouble (), -1.0, "second argument negative");
  
  a = int64x64_t (-0.5);
  a /= int64x64_t (-0.5);
  Check (a.GetDouble (), 1.0, "both arguments negative");
}


/**
 * See \bugid{1786}
 */
class Int64x64Bug1786TestCase : public TestCase
{
public:
  Int64x64Bug1786TestCase ();
  virtual void DoRun (void);
  void Check (const uint64_t low, const std::string & value,
	      const int64_t tolerance = 0);
};

Int64x64Bug1786TestCase::Int64x64Bug1786TestCase ()
  : TestCase ("Test case for bug 1786")
{
}
void
Int64x64Bug1786TestCase::Check (const uint64_t low,
				const std::string & str,
				const int64_t tolerance /* = 0 */)
{
  int64x64_t value (0, low);
  std::ostringstream oss;
  oss << std::scientific << std::setprecision (22) << value;

  if (tolerance == 0)
    {
      bool pass = oss.str () == str;
  
      std::cout << GetParent ()->GetName () << " Bug 1786: "
		<< (pass ? "pass " : "FAIL ")
		<< "    0x" << std::hex << std::setw (16) << low << std::dec
		<< " = "    << oss.str ();
      if (!pass)
	{
	  std::cout << ", expected " << str;
	}
      std::cout << std::endl;

      NS_TEST_EXPECT_MSG_EQ (oss.str (), str,
			     "Fraction string not correct");
    }
  else
    {
      // No obvious way to implement a tolerance on the strings
      
      std::cout << GetParent ()->GetName () << " Bug 1786: "
		<< "skip "
		<< "    0x" << std::hex << std::setw (16) << low << std::dec
		<< " = "    << oss.str ()
		<< ", expected " << str
		<< std::endl;

    }
}
void
Int64x64Bug1786TestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " But 1786: " << GetName ()
	    << std::endl;

  int64_t tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 1;
    }

  // Some of these values differ from the DoubleTestCase
  // by one count in the last place
  // because operator<< truncates the last output digit,
  // instead of rounding.
  Check (                 1ULL, "+0.0000000000000000000542");
  Check (                 2ULL, "+0.0000000000000000001084");
  Check (                 3ULL, "+0.0000000000000000001626");
  Check (                 4ULL, "+0.0000000000000000002168");
  Check (                 5ULL, "+0.0000000000000000002710");
  Check (                 6ULL, "+0.0000000000000000003253");
  Check (                 7ULL, "+0.0000000000000000003795");
  Check (                 8ULL, "+0.0000000000000000004337");
  Check (                 9ULL, "+0.0000000000000000004879");
  Check (               0xAULL, "+0.0000000000000000005421");
  Check (               0xFULL, "+0.0000000000000000008132");
  Check (              0xF0ULL, "+0.0000000000000000130104");
  Check (             0xF00ULL, "+0.0000000000000002081668");
  Check (            0xF000ULL, "+0.0000000000000033306691");
  Check (           0xF0000ULL, "+0.0000000000000532907052");
  Check (          0xF00000ULL, "+0.0000000000008526512829");
  Check (         0xF000000ULL, "+0.0000000000136424205266");
  Check (        0xF0000000ULL, "+0.0000000002182787284255");
  Check (       0xF00000000ULL, "+0.0000000034924596548080");
  Check (      0xF000000000ULL, "+0.0000000558793544769287");
  Check (     0xF0000000000ULL, "+0.0000008940696716308594");
  Check (    0xF00000000000ULL, "+0.0000143051147460937500");
  Check (   0xF000000000000ULL, "+0.0002288818359375000000");
  Check (  0xF0000000000000ULL, "+0.0036621093750000000000");
  Check ( 0xF00000000000000ULL, "+0.0585937500000000000000");
  std::cout << std::endl;
  Check (0x7FFFFFFFFFFFFFFDULL, "+0.4999999999999999998374", tolerance);
  Check (0x7FFFFFFFFFFFFFFEULL, "+0.4999999999999999998916", tolerance);
  Check (0x7FFFFFFFFFFFFFFFULL, "+0.4999999999999999999458", tolerance);
  Check (0x8000000000000000ULL, "+0.5000000000000000000000");
  Check (0x8000000000000001ULL, "+0.5000000000000000000542", tolerance);
  Check (0x8000000000000002ULL, "+0.5000000000000000001084", tolerance);
  Check (0x8000000000000003ULL, "+0.5000000000000000001626", tolerance);
  std::cout << std::endl;
  Check (0xF000000000000000ULL, "+0.9375000000000000000000");
  Check (0xFF00000000000000ULL, "+0.9960937500000000000000");
  Check (0xFFF0000000000000ULL, "+0.9997558593750000000000");
  Check (0xFFFF000000000000ULL, "+0.9999847412109375000000");
  Check (0xFFFFF00000000000ULL, "+0.9999990463256835937500");
  Check (0xFFFFFF0000000000ULL, "+0.9999999403953552246094");
  Check (0xFFFFFFF000000000ULL, "+0.9999999962747097015381");
  Check (0xFFFFFFFF00000000ULL, "+0.9999999997671693563461");
  Check (0xFFFFFFFFF0000000ULL, "+0.9999999999854480847716");
  Check (0xFFFFFFFFFF000000ULL, "+0.9999999999990905052982");
  Check (0xFFFFFFFFFFF00000ULL, "+0.9999999999999431565811");
  Check (0xFFFFFFFFFFFF0000ULL, "+0.9999999999999964472863");
  Check (0xFFFFFFFFFFFFF000ULL, "+0.9999999999999997779554");
  Check (0xFFFFFFFFFFFFFF00ULL, "+0.9999999999999999861222");
  Check (0xFFFFFFFFFFFFFFF0ULL, "+0.9999999999999999991326");
  Check (0xFFFFFFFFFFFFFFF5ULL, "+0.9999999999999999994037", tolerance);
  Check (0xFFFFFFFFFFFFFFF6ULL, "+0.9999999999999999994579", tolerance);
  Check (0xFFFFFFFFFFFFFFF7ULL, "+0.9999999999999999995121", tolerance);
  Check (0xFFFFFFFFFFFFFFF8ULL, "+0.9999999999999999995663", tolerance);
  Check (0xFFFFFFFFFFFFFFF9ULL, "+0.9999999999999999996205", tolerance);
  Check (0xFFFFFFFFFFFFFFFAULL, "+0.9999999999999999996747", tolerance);
  Check (0xFFFFFFFFFFFFFFFBULL, "+0.9999999999999999997289", tolerance);
  Check (0xFFFFFFFFFFFFFFFCULL, "+0.9999999999999999997832", tolerance);
  Check (0xFFFFFFFFFFFFFFFDULL, "+0.9999999999999999998374", tolerance);
  Check (0xFFFFFFFFFFFFFFFEULL, "+0.9999999999999999998916", tolerance);
  Check (0xFFFFFFFFFFFFFFFFULL, "+0.9999999999999999999458", tolerance);
}


class Int64x64CompareTestCase : public TestCase
{
public:
  Int64x64CompareTestCase ();
  virtual void DoRun (void);

  void Check (const bool result, const bool expect,
	      const std::string & msg);
};
Int64x64CompareTestCase::Int64x64CompareTestCase ()
  : TestCase ("Basic compare operations")
{
}
void
Int64x64CompareTestCase::Check (const bool result, const bool expect,
				const std::string & msg)
{
  bool pass = result == expect;
  
  std::cout << GetParent ()->GetName () << " Compare: "
	    << (pass ? "pass " : "FAIL ")
	    << msg
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ (result, expect, msg);
}

void
Int64x64CompareTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Compare: " << GetName ()
	    << std::endl;

  const int64x64_t zero ( 0, 0);
  const int64x64_t one  ( 1, 0);
  const int64x64_t two  ( 2, 0);
  const int64x64_t mone (-1, 0);
  const int64x64_t mtwo (-2, 0);
  const int64x64_t frac  = int64x64_t (0, 0xc000000000000000ULL);  // 0.75
  const int64x64_t zerof = zero + frac;
  const int64x64_t onef  = one  + frac;
  const int64x64_t monef = mone - frac;
  const int64x64_t mtwof = mtwo - frac;
  
  Check ( zerof    == zerof, true,  "equality, zero");
  Check ( onef     == onef,  true,  "equality, positive");
  Check ( mtwof    == mtwof, true,  "equality, negative");
  Check ( zero     == one,   false, "equality false, zero");
  Check ( one      == two,   false, "equality false, unsigned");
  Check ( one      == mone,  false, "equality false, signed");
  Check ( onef     == one,   false, "equality false, fraction");
  std::cout << std::endl;

  Check ( zerof    != zerof, false, "inequality, zero");
  Check ( onef     != onef,  false, "inequality, positive");
  Check ( mtwof    != mtwof, false, "inequality, negative");
  Check ( zero     != one,   true,  "inequality true, zero");
  Check ( one      != two,   true,  "inequality true, unsigned");
  Check ( one      != mone,  true,  "inequality true, signed");
  Check ( onef     != one,   true,  "inequality true, fraction");
  std::cout << std::endl;

  Check ( zerof    <  onef,  true,  "less, zerof");
  Check ( zero     <  zerof, true,  "less, zero");
  Check ( one      <  onef,  true,  "less, positive");
  Check ( monef    <  mone,  true,  "less, negative");
  Check ( onef     <  one,   false, "less, false, positive");
  Check ( mtwo     <  mtwof, false, "less, false, negative");
  std::cout << std::endl;

  Check ( zerof    <= zerof, true,  "less equal, equal, zerof");
  Check ( zero     <= zerof, true,  "less equal, less, zero");
  Check ( onef     <= onef,  true,  "less equal, equal, positive");
  Check ( monef    <= mone,  true,  "less equal, less, negative");
  Check ( onef     <= one,   false, "less equal, false, positive");
  Check ( mtwo     <= mtwof, false, "less equal, false, negative");
  std::cout << std::endl;

  Check ( onef     >  zerof, true,  "greater, zerof");
  Check ( zerof    >  zero,  true,  "greater, zero");
  Check ( onef     >  one,   true,  "greater, positive");
  Check ( mone     >  monef, true,  "greater, negative");
  Check ( one      >  onef,  false, "greater, false, positive");
  Check ( mtwof    >  mtwo,  false, "greater, false, negative");
  std::cout << std::endl;

  Check ( zerof    >= zerof, true,  "greater equal, equal, zerof");
  Check ( zerof    >= zero,  true,  "greater equal, greater, zero");
  Check ( onef     >= onef,  true,  "greater equal, equal, positive");
  Check ( mone     >= monef, true,  "greater equal, greater, negative");
  Check ( one      >= onef,  false, "greater equal, false, positive");
  Check ( mtwof    >= mtwo,  false, "greater equal, false, negative");
  std::cout << std::endl;

  Check ( zero     == false, true,  "zero   == false");
  Check ( one      == true,  true,  "one    == true");
  Check ( zerof    != false, true,  "zerof  != false");
  Check ( (!zero)  == true,  true,  "!zero  == true");
  Check ( (!zerof) == false, true,  "!zerof == false");
  Check ( (!one)   == false, true,  "!one   == false");
  Check ( (+onef)  == onef,  true, "unary positive");
  Check ( (-onef)  == monef, true, "unary negative");
}


class Int64x64InvertTestCase : public TestCase
{
public:
  Int64x64InvertTestCase ();
  virtual void DoRun (void);
  void Check (const int64_t factor);
  void CheckCase (const uint64_t factor,
		  const int64x64_t result, const int64x64_t expect,
		  const std::string & msg,
		  const double tolerance = 0);
};

Int64x64InvertTestCase::Int64x64InvertTestCase ()
  : TestCase ("Invert and MulByInvert")
{
}
void
Int64x64InvertTestCase::CheckCase (const uint64_t factor,
				   const int64x64_t result,
				   const int64x64_t expect,
				   const std::string & msg,
				   const double tolerance /* = 0 */)
{
  bool pass = Abs (result - expect) <= tolerance;
  
  std::cout << GetParent ()->GetName () << " Invert: ";

  if (pass)
    {
      std::cout << "pass:  " << factor << ": ";

    }
  else
    {
      std::cout << "FAIL:  " << factor << ": "
		<< "(res: " << result
		<< " exp: " << expect
		<< " tol: " << tolerance << ")  ";
    }
  std::cout << msg 
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ_TOL (result, expect, int64x64_t(tolerance), msg);
}

void
Int64x64InvertTestCase::Check (const int64_t factor)
{
  const int64x64_t one (1, 0);
  const int64x64_t factorI = one / int64x64_t (factor);
  
  const int64x64_t a = int64x64_t::Invert (factor);
  int64x64_t b = int64x64_t (factor);

  double tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 0.000000000000000001L;
    }
  
  b.MulByInvert (a);
  CheckCase (factor, b, one, "x * x^-1 == 1", tolerance);
  
  int64x64_t c = int64x64_t (1);
  c.MulByInvert (a);
  CheckCase (factor, c, factorI, "1 * x^-1 == 1 / x");
  
  int64x64_t d = int64x64_t (1);
  d /= (int64x64_t (factor));
  CheckCase (factor, d, c, "1/x == x^-1");
  
  int64x64_t e = int64x64_t (-factor);
  e.MulByInvert (a);
  CheckCase (factor, e, -one, "-x * x^-1 == -1", tolerance);
}

void
Int64x64InvertTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Invert: " << GetName ()
	    << std::endl;
  
  Check (2);
  Check (3);
  Check (4);
  Check (5);
  Check (6);
  Check (10);
  Check (99);
  Check (100);
  Check (1000);
  Check (10000);
  Check (100000);
  Check (100000);
  Check (1000000);
  Check (10000000);
  Check (100000000);
  Check (1000000000);
  Check (10000000000LL);
  Check (100000000000LL);
  Check (1000000000000LL);
  Check (10000000000000LL);
  Check (100000000000000LL);
  Check (1000000000000000LL);
}


class Int64x64DoubleTestCase : public TestCase
{
public:
  Int64x64DoubleTestCase ();
  virtual void DoRun (void);
  void Check (const int64_t intPart);
  void Check (const long double value,
	      const int64_t intPart,
	      const uint64_t lo);
private:
  long double m_last;
  int64x64_t  m_deltaMax;
  int         m_deltaCount;
};

Int64x64DoubleTestCase::Int64x64DoubleTestCase ()
  : TestCase ("Construct from floating point.")
{
}

void
Int64x64DoubleTestCase::Check (const long double value,
			       const int64_t intPart,
			       const uint64_t lo)
{
  // Construct the expected value
  int64x64_t expect = int64x64_t (0, lo);
  expect += intPart;

  // Actual value of conversion from double
  const int64x64_t result = int64x64_t (value);

  // Make tolerance depend on magnitude of value
  long double epsilon = std::numeric_limits<long double>::epsilon ();
  long double margin = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      margin = 1.0;
    }
  if (RUNNING_ON_VALGRIND)
    {
      // Valgrind uses 64-bit doubles for long doubles
      // See ns-3 bug 1882
      // Need non-zero margin to ensure final tolerance is non-zero
      margin = 1.0;
      epsilon = std::numeric_limits<double>::epsilon ();
    }

  const int64x64_t tolerance = (margin + std::fabs (value)) * epsilon;
  
  const int64x64_t delta = Abs (result - expect);
  const bool skip = value == m_last;
  const bool pass = delta <= tolerance;
  
  // Save stream format flags
  std::ios_base::fmtflags ff = std::cout.flags ();
  std::cout << std::fixed << std::setprecision (22);
  
  std::cout << GetParent ()->GetName () << " Double: "
	    << (skip ? "skip " : (pass ? "pass " : "FAIL "))
	    << std::showpos << value << " == " 
	    << Printer (result)
	    << std::endl;
  
  // Log non-zero delta
  if ( delta > int64x64_t() )
    {
      std::cout << GetParent ()->GetName ()
		<< std::left  << std::setw (43) << "         expected"
		<< std::right << Printer (expect)
		<< std::endl;

      if (delta == tolerance)
	{
	  std::cout << GetParent ()->GetName ()
		    << std::left  << std::setw (43) << "         delta = tolerance"
		    << std::right << Printer (delta)
		    << std::endl;
	}
      else
	{
	  std::cout << GetParent ()->GetName ()
		    << std::left  << std::setw (43) << "         delta"
		    << std::right << Printer (delta)
		    << std::endl;
	  std::cout << GetParent ()->GetName ()
		    << std::left  << std::setw (43) << "         +/-"
		    << std::right << Printer (tolerance)
		    << std::endl;
	}
      
      ++m_deltaCount;
      
      if ( delta > m_deltaMax )
	{
	  m_deltaMax = delta;
	}
    }
  
  NS_TEST_ASSERT_MSG_EQ_TOL (result, expect, tolerance,
			     "int64x64_t (long double) failed");
  m_last = value;
  std::cout.flags (ff);

}

void
Int64x64DoubleTestCase::Check (const int64_t intPart)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Double: "
	    << "integer: " << intPart
	    << std::endl;
  m_last = static_cast<long double> (intPart);
  m_deltaCount = 0;

  // Nudging the integer part eliminates deltas around 0
  long double v = static_cast<long double> (intPart);
  
  Check (v +0.0000000000000000000542L, intPart,                0x1ULL);
  Check (v +0.0000000000000000001084L, intPart,                0x2ULL);
  Check (v +0.0000000000000000001626L, intPart,                0x3ULL);
  Check (v +0.0000000000000000002168L, intPart,                0x4ULL);
  Check (v +0.0000000000000000002710L, intPart,                0x5ULL);
  Check (v +0.0000000000000000003253L, intPart,                0x6ULL);
  Check (v +0.0000000000000000003795L, intPart,                0x7ULL);
  Check (v +0.0000000000000000004337L, intPart,                0x8ULL);
  Check (v +0.0000000000000000004879L, intPart,                0x9ULL);
  Check (v +0.0000000000000000005421L, intPart,                0xAULL);
  Check (v +0.0000000000000000008132L, intPart,                0xFULL);
  Check (v +0.0000000000000000130104L, intPart,               0xF0ULL);
  Check (v +0.0000000000000002081668L, intPart,              0xF00ULL);
  Check (v +0.0000000000000033306691L, intPart,             0xF000ULL);
  Check (v +0.0000000000000532907052L, intPart,            0xF0000ULL);
  Check (v +0.0000000000008526512829L, intPart,           0xF00000ULL);
  Check (v +0.0000000000136424205266L, intPart,          0xF000000ULL);
  Check (v +0.0000000002182787284255L, intPart,         0xF0000000ULL);
  Check (v +0.0000000034924596548080L, intPart,        0xF00000000ULL);
  Check (v +0.0000000558793544769287L, intPart,       0xF000000000ULL);
  Check (v +0.0000008940696716308594L, intPart,      0xF0000000000ULL);
  Check (v +0.0000143051147460937500L, intPart,     0xF00000000000ULL);
  Check (v +0.0002288818359375000000L, intPart,    0xF000000000000ULL);
  Check (v +0.0036621093750000000000L, intPart,   0xF0000000000000ULL);
  Check (v +0.0585937500000000000000L, intPart,  0xF00000000000000ULL);
  std::cout << std::endl;
  Check (v +0.4999999999999999998374L, intPart, 0x7FFFFFFFFFFFFFFDULL);
  Check (v +0.4999999999999999998916L, intPart, 0x7FFFFFFFFFFFFFFEULL);
  Check (v +0.4999999999999999999458L, intPart, 0x7FFFFFFFFFFFFFFFULL);
  Check (v +0.5000000000000000000000L, intPart, 0x8000000000000000ULL);
  Check (v +0.5000000000000000000542L, intPart, 0x8000000000000001ULL);
  Check (v +0.5000000000000000001084L, intPart, 0x8000000000000002ULL);
  Check (v +0.5000000000000000001626L, intPart, 0x8000000000000003ULL);
  std::cout << std::endl;
  Check (v +0.9375000000000000000000L, intPart, 0xF000000000000000ULL);
  Check (v +0.9960937500000000000000L, intPart, 0xFF00000000000000ULL);
  Check (v +0.9997558593750000000000L, intPart, 0xFFF0000000000000ULL);
  Check (v +0.9999847412109375000000L, intPart, 0xFFFF000000000000ULL);
  Check (v +0.9999990463256835937500L, intPart, 0xFFFFF00000000000ULL);
  Check (v +0.9999999403953552246094L, intPart, 0xFFFFFF0000000000ULL);
  Check (v +0.9999999962747097015381L, intPart, 0xFFFFFFF000000000ULL);
  Check (v +0.9999999997671693563461L, intPart, 0xFFFFFFFF00000000ULL);
  Check (v +0.9999999999854480847716L, intPart, 0xFFFFFFFFF0000000ULL);
  Check (v +0.9999999999990905052982L, intPart, 0xFFFFFFFFFF000000ULL);
  Check (v +0.9999999999999431565811L, intPart, 0xFFFFFFFFFFF00000ULL);
  Check (v +0.9999999999999964472863L, intPart, 0xFFFFFFFFFFFF0000ULL);
  Check (v +0.9999999999999997779554L, intPart, 0xFFFFFFFFFFFFF000ULL);
  Check (v +0.9999999999999999861222L, intPart, 0xFFFFFFFFFFFFFF00ULL);
  Check (v +0.9999999999999999991326L, intPart, 0xFFFFFFFFFFFFFFF0ULL);
  Check (v +0.9999999999999999994037L, intPart, 0xFFFFFFFFFFFFFFF5ULL);
  Check (v +0.9999999999999999994579L, intPart, 0xFFFFFFFFFFFFFFF6ULL);
  Check (v +0.9999999999999999995121L, intPart, 0xFFFFFFFFFFFFFFF7ULL);
  Check (v +0.9999999999999999995663L, intPart, 0xFFFFFFFFFFFFFFF8ULL);
  Check (v +0.9999999999999999996205L, intPart, 0xFFFFFFFFFFFFFFF9ULL);
  Check (v +0.9999999999999999996747L, intPart, 0xFFFFFFFFFFFFFFFAULL);
  Check (v +0.9999999999999999997289L, intPart, 0xFFFFFFFFFFFFFFFBULL);
  Check (v +0.9999999999999999997832L, intPart, 0xFFFFFFFFFFFFFFFCULL);
  Check (v +0.9999999999999999998374L, intPart, 0xFFFFFFFFFFFFFFFDULL);
  Check (v +0.9999999999999999998916L, intPart, 0xFFFFFFFFFFFFFFFEULL);
  Check (v +0.9999999999999999999458L, intPart, 0xFFFFFFFFFFFFFFFFULL);

  std::cout << GetParent ()->GetName () << " Double: "
	    << "integer: " << intPart
	    << ": delta count: " << m_deltaCount
	    << ", max: " << Printer (m_deltaMax)
	    << std::endl;  
}

void
Int64x64DoubleTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Double: " << GetName ()
	    << std::endl;

  // Save stream format flags
  std::ios_base::fmtflags ff = std::cout.flags ();
  std::cout << std::scientific << std::setprecision (21);

  m_deltaMax = int64x64_t ();

  std::cout << GetParent ()->GetName () << " Double: "
	    << std::endl;
    
  Check (-2);
  Check (-1);
  Check ( 0);
  Check ( 1);
  Check ( 2);

  std::cout << GetParent ()->GetName () << " Double: "
	    << "max delta: " << Printer (m_deltaMax)
	    << std::endl;

  std::cout.flags (ff);
}


class Int64x64ImplTestCase : public TestCase
{
public:
  Int64x64ImplTestCase ();
  virtual void DoRun (void);
};

Int64x64ImplTestCase::Int64x64ImplTestCase ()
  : TestCase ("Print the implementation")
{
}

void
Int64x64ImplTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << " Impl: " << GetName ()
	    << std::endl;

  
  std::cout << "int64x64_t::implementation: ";
  switch (int64x64_t::implementation)
    {
    case (int64x64_t::int128_impl) : std::cout << "int128_impl"; break;
    case (int64x64_t::cairo_impl)  : std::cout << "cairo_impl";  break;
    case (int64x64_t::ld_impl)     : std::cout << "ld_impl";     break;
    default :                        std::cout << "unknown!";    
    }
  std::cout << std::endl;

#if defined (INT64X64_USE_CAIRO) && !defined (PYTHON_SCAN)
  std::cout << "cairo_impl64:  " << cairo_impl64 << std::endl;
  std::cout << "cairo_impl128: " << cairo_impl128 << std::endl;
#endif

  if (RUNNING_ON_VALGRIND != 0)
    {
      std::cout << "Running with valgrind" << std::endl;
    }

}

static class Int64x64TestSuite : public TestSuite
{
public:
  Int64x64TestSuite ()
    : TestSuite ("int64x64", UNIT)
  {
    AddTestCase (new Int64x64ImplTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64HiLoTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64ArithmeticTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64CompareTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InputTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InputOutputTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug455TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug863TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug1786TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InvertTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64DoubleTestCase (), TestCase::QUICK);
  }
}  g_int64x64TestSuite;

}  // namespace test

}  // namespace int64x64

}  // namespace ns3

