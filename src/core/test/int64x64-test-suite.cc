#include "ns3/int64x64.h"
#include "ns3/test.h"

#include <iomanip>

using namespace ns3;

/**
 * \internal
 *
 * Some of these tests are a little unusual for ns-3 in that they
 * are sensitive to implementation, specifically the double implementation.
 * To handle this, where needed we define a tolerance to use in the
 * test comparisions.  If you need to increase the tolerance,
 * please append the system and compiler version.  For example:
 *
 * \code
 *   // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
 *   tolerance = 1;
 *   // System Foo gcc 3.9
 *   tolerance = 3;
 * \endcode
 */
class Int64x64HiLoTestCase : public TestCase
{
public:
  Int64x64HiLoTestCase ();
  virtual void DoRun (void);
  void Check (const int64_t hi, const uint64_t lo,
	      const int64_t loTolerance = 0);
};

void 
Int64x64HiLoTestCase::Check (const int64_t hi,
			     const uint64_t lo,
			     const int64_t loTolerance /* = 0 */)
{
  int64x64_t tmp = int64x64_t (hi,lo);

  std::cout << GetParent ()->GetName () << " Check "
	    << std::hex  << std::setfill ('0')
	    << "hi: 0x"  << std::setw (16) << hi
	    << " lo: 0x" << std::setw (16) << lo
	    << std::dec  << std::setfill (' ')
	    << " int64x64: " << tmp
	    << std::endl;

  NS_TEST_EXPECT_MSG_EQ (tmp.GetHigh (), hi,
			 "High part does not match for hi:" << hi
			 << " lo: " << lo);
  NS_TEST_EXPECT_MSG_EQ_TOL ((int64_t)tmp.GetLow (), (int64_t)lo, loTolerance,
			     "Low part does not match for hi: " << hi
			     << " lo: " << lo);
}

Int64x64HiLoTestCase::Int64x64HiLoTestCase ()
  : TestCase ("Manipulate the high and low part of every number")
{
}
void
Int64x64HiLoTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
	    << std::endl;

  int64_t tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 1;
    }
      
  Check (1,  0);
  Check (1,  1, tolerance);
  Check (-1, 0);
  Check (-1, 1);
}


class Int64x64InputTestCase : public TestCase
{
public:
  Int64x64InputTestCase ();
  virtual void DoRun (void);
  void Check (const std::string & str,
	      const int64_t hi, const uint64_t lo,
	      const int64_t loTolerance = 0);
};
Int64x64InputTestCase::Int64x64InputTestCase ()
  : TestCase ("Parse int64x64_t numbers as strings")
{
}
void 
Int64x64InputTestCase::Check (const std::string & str,
			      const int64_t hi, const uint64_t lo,
			      const int64_t loTolerance /* = 0 */)
			      
{
  std::istringstream iss;
  iss.str (str);
  int64x64_t hp;
  iss >> hp;

  std::string input = "\"" + str + "\"";

  std::cout << GetParent ()->GetName () << " Input: "
	    << std::left << std::setw (28) << input << std::right
	    << std::hex  << std::setfill ('0')
	    << " hi: 0x" << std::setw (16) << hp.GetHigh ()
	    << " lo: 0x" << std::setw (16) << hp.GetLow ()
	    << std::dec  << std::setfill (' ')
	    << " expected: " << hi << " " << lo << " +/- " << loTolerance
	    << std::endl;

  NS_TEST_EXPECT_MSG_EQ (hp.GetHigh (), hi,
			 "High parts do not match for input string \""
			 << str << "\"");
  NS_TEST_EXPECT_MSG_EQ_TOL ((int64_t)hp.GetLow (), (int64_t)lo, loTolerance,
			     "Low parts do not match for input string \""
			     << str << "\"");
}
void
Int64x64InputTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
  Check ("-1.000000000000000000054", -1, 1, tolerance);
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
  std::istringstream iss;
  iss.str (str);
  int64x64_t value;
  iss >> value;
  std::ostringstream oss;
  oss << std::scientific << std::setprecision (21) << value;

  std::string input  = "\"" + str + "\"";
  std::string output = "\"" + oss.str () + "\"";

  std::cout << GetParent ()->GetName () << " InputOutput: "
	    << " in: "  << std::left << std::setw (28) << input  << std::right
	    << " out: " << std::left << std::setw (28) << output << std::right
	    << std::endl;

  if (tolerance == 0)
    {
      NS_TEST_EXPECT_MSG_EQ (oss.str (), str,
			     "Converted string does not match expected string");
    }
  else
    {
      // No obvious way to implement a tolerance
    }
}
void
Int64x64InputOutputTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
	      const int64x64_t value, const int64x64_t expected);
  void Check (const int        test,
	      const int64x64_t value, const int64x64_t expected,
	      const int64x64_t tolerance);
};

Int64x64ArithmeticTestCase::Int64x64ArithmeticTestCase ()
  : TestCase ("Basic arithmetic operations")
{
}
void
Int64x64ArithmeticTestCase::Check (const int test,
				   const int64x64_t value,
				   const int64x64_t expected)
{
  int64x64_t zero (0,0);
  Check (test, value, expected, zero);
}
void
Int64x64ArithmeticTestCase::Check (const int test,
				   const int64x64_t value,
				   const int64x64_t expected,
				   const int64x64_t tolerance)
{
  std::cout << GetParent ()->GetName () << " Arithmetic: "
	    << test << ": " << value << " ?= " << expected
	    << " (+/- " << tolerance << ")"
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ_TOL ( value, expected, tolerance,
			      "Arithmetic failure in test case " << test);
}

void
Int64x64ArithmeticTestCase::DoRun (void)
{
  const int64x64_t zero (0, 0);
  const int64x64_t one  (1, 0);
  const int64x64_t two  (2, 0);
  const int64x64_t thre (3, 0);

  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
  
  Check (20, (two  * thre ) / thre , two  );

  const int64x64_t frac  = int64x64_t (0, 0xc000000000000000ULL);  // 0.75
  const int64x64_t fplf2 = frac + frac * frac;  // 1.3125
  NS_TEST_ASSERT_MSG_EQ (frac,  0.75,   "Arithmetic failure fractional part");
  NS_TEST_ASSERT_MSG_EQ (fplf2, 1.3125, "Arithmetic failure fplf2");
  
  const int64x64_t zerof = zero + frac;
  NS_TEST_ASSERT_MSG_EQ (zerof, frac,   "Arithmetic failure adding fractional part");
  const int64x64_t onef  = one  + frac;
  const int64x64_t twof  = two  + frac;
  const int64x64_t thref = thre + frac;
  
  Check (21,   zerof -   zerof,   zero );
  Check (22,   zerof -   onef,   -one  );
  Check (23,   onef  -   onef,    zero );
  Check (24,   onef  -   twof,   -one  );
  Check (25,   onef  - (-onef),   twof  + frac  );
  Check (26, (-onef) - (-twof),   one  );
  Check (27, (-onef) -   twof,   -thref - frac  );
  
  Check (28,   zerof +   zerof,   zerof + frac  );
  Check (29,   zerof +   onef,    onef  + frac  );
  Check (30,   onef  +   onef,    twof  + frac  );
  Check (31,   onef  +   twof,    thref + frac  );
  Check (32,   onef  + (-onef),   zero  );
  Check (33, (-onef) + (-twof),  -thref - frac  );
  Check (34, (-onef) +   twof,    one   );
  
  Check (35,   zerof *   zerof,   frac  * frac  );
  Check (36,   zero  *   onef,    zero  );
  Check (37,   zerof *   one,     frac  );
  
  Check (38,   zerof *   onef,    fplf2 );
  Check (39,   zerof * (-onef),  -fplf2 );
  Check (40,   onef  *   onef,    onef  + fplf2 );
  Check (41,   onef  * (-onef),  -onef  - fplf2 );
  Check (42, (-onef) * (-onef),   onef  + fplf2 );
  
  
  // Multiplication followed by division is exact:
  Check (43, (two  * thre ) / thre , two  );
  Check (44, (twof * thref) / thref, twof );

  // Division followed by multiplication loses a bit or two:
  int64x64_t tolerance (0, 3);
  Check (45, (two  / thre)  * thre,  two , tolerance );
  Check (46, (twof / thref) * thref, twof, tolerance );

  // The example below shows that we really do not lose
  // much precision internally: it is almost always the
  // final conversion which loses precision.
  Check (47, (int64x64_t (2000000000) / int64x64_t (3)) * int64x64_t (3),
	      int64x64_t (1999999999, 0xfffffffffffffffeULL));

  // Check special values
  Check (48,  int64x64_t (0, 0x159fa87f8aeaad21ULL) * 10,
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
  std::cout << GetParent ()->GetName () << " Bug 455: "
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
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
  std::cout << GetParent ()->GetName () << " Bug 863: "
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
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
  oss << std::scientific << std::setprecision (21) << value;
  
  std::cout << GetParent ()->GetName () << " Bug 1786: "
	    << "    0x" << std::hex << std::setw (16) << low << std::dec
	    << " = "    << oss.str ()
	    << std::endl;

  if (tolerance == 0)
    {
      NS_TEST_EXPECT_MSG_EQ (oss.str (), str,
			     "Fraction string not correct");
    }
  else
    {
      // No obvious way to implement a tolerance
    }
}
void
Int64x64Bug1786TestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
	    << std::endl;

  int64_t tolerance = 0;
  if (int64x64_t::implementation == int64x64_t::ld_impl)
    {
      // Darwin 12.5.0 (Mac 10.8.5) g++ 4.2.1
      tolerance = 1;
    }
  
  Check (                 1ULL, "+0.000000000000000000054");
  Check (                 2ULL, "+0.000000000000000000108");
  Check (                 3ULL, "+0.000000000000000000162");
  Check (                 4ULL, "+0.000000000000000000216");
  Check (                 5ULL, "+0.000000000000000000271");
  Check (                 6ULL, "+0.000000000000000000325");
  Check (                 7ULL, "+0.000000000000000000379");
  Check (                 8ULL, "+0.000000000000000000433");
  Check (                 9ULL, "+0.000000000000000000487");
  Check (               0xAULL, "+0.000000000000000000542");
  Check (               0xFULL, "+0.000000000000000000813");
  Check (              0xF0ULL, "+0.000000000000000013010");
  Check (             0xF00ULL, "+0.000000000000000208166");
  Check (            0xF000ULL, "+0.000000000000003330669");
  Check (           0xF0000ULL, "+0.000000000000053290705");
  Check (          0xF00000ULL, "+0.000000000000852651282");
  Check (         0xF000000ULL, "+0.000000000013642420526");
  Check (        0xF0000000ULL, "+0.000000000218278728425");
  Check (       0xF00000000ULL, "+0.000000003492459654808");
  Check (      0xF000000000ULL, "+0.000000055879354476928");
  Check (     0xF0000000000ULL, "+0.000000894069671630859");
  Check (    0xF00000000000ULL, "+0.000014305114746093750");
  Check (   0xF000000000000ULL, "+0.000228881835937500000");
  Check (  0xF0000000000000ULL, "+0.003662109375000000000");
  std::cout << std::endl;
  Check (0x7FFFFFFFFFFFFFFDULL, "+0.499999999999999999837", tolerance);
  Check (0x7FFFFFFFFFFFFFFEULL, "+0.499999999999999999891", tolerance);
  Check (0x7FFFFFFFFFFFFFFFULL, "+0.499999999999999999945", tolerance);
  Check (0x8000000000000000ULL, "+0.500000000000000000000");
  Check (0x8000000000000001ULL, "+0.500000000000000000054", tolerance);
  Check (0x8000000000000002ULL, "+0.500000000000000000108", tolerance);
  Check (0x8000000000000003ULL, "+0.500000000000000000162", tolerance);
  std::cout << std::endl;
  Check (0xF000000000000000ULL, "+0.937500000000000000000");
  Check (0xFF00000000000000ULL, "+0.996093750000000000000");
  Check (0xFFF0000000000000ULL, "+0.999755859375000000000");
  Check (0xFFFF000000000000ULL, "+0.999984741210937500000");
  Check (0xFFFFF00000000000ULL, "+0.999999046325683593750");
  Check (0xFFFFFF0000000000ULL, "+0.999999940395355224609");
  Check (0xFFFFFFF000000000ULL, "+0.999999996274709701538");
  Check (0xFFFFFFFF00000000ULL, "+0.999999999767169356346");
  Check (0xFFFFFFFFF0000000ULL, "+0.999999999985448084771");
  Check (0xFFFFFFFFFF000000ULL, "+0.999999999999090505298");
  Check (0xFFFFFFFFFFF00000ULL, "+0.999999999999943156581");
  Check (0xFFFFFFFFFFFF0000ULL, "+0.999999999999996447286");
  Check (0xFFFFFFFFFFFFF000ULL, "+0.999999999999999777955");
  Check (0xFFFFFFFFFFFFFF00ULL, "+0.999999999999999986122");
  Check (0xFFFFFFFFFFFFFFF0ULL, "+0.999999999999999999132");
  Check (0xFFFFFFFFFFFFFFFFULL, "+0.999999999999999999945", tolerance);
}

class Int64x64CompareTestCase : public TestCase
{
public:
  Int64x64CompareTestCase ();
  virtual void DoRun (void);

#if defined (__GNUC__)
#if (__GNUC__ == 4 && (4 <= __GNUC_MINOR__ && __GNUC_MINOR__ <= 6))
  // gcc-4.4 is overeager to optimize away signed comparisons
  // which makes it hard to test comparison operators
  __attribute__ ((optimize ("no-strict-overflow")))
#endif
#endif
  void Check (const bool result, const bool expected,
	      const std::string & msg);
};
Int64x64CompareTestCase::Int64x64CompareTestCase ()
  : TestCase ("Basic compare operations")
{
}
void
Int64x64CompareTestCase::Check (const bool result, const bool expected,
				const std::string & msg)
{
  std::cout << GetParent ()->GetName () << " Compare: "
	    << (result == expected ? "pass:  " : "FAIL:  ") << msg
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ (result, expected, msg);
}

void
Int64x64CompareTestCase::DoRun (void)
{
  std::cout << std::endl;
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
  
  Check ( zerof == zerof, true,  "equality, zero");
  Check ( onef  == onef,  true,  "equality, positive");
  Check ( mtwof == mtwof, true,  "equality, negative");
  Check ( zero  == one,   false, "equality false, zero");
  Check ( one   == two,   false, "equality false, unsigned");
  Check ( one   == mone,  false, "equality false, signed");
  Check ( onef  == one,   false, "equality false, fraction");
  std::cout << std::endl;

  Check ( zerof != zerof, false, "inequality, zero");
  Check ( onef  != onef,  false, "inequality, positive");
  Check ( mtwof != mtwof, false, "inequality, negative");
  Check ( zero  != one,   true,  "inequality true, zero");
  Check ( one   != two,   true,  "inequality true, unsigned");
  Check ( one   != mone,  true,  "inequality true, signed");
  Check ( onef  != one,   true,  "inequality true, fraction");
  std::cout << std::endl;

  Check ( zerof <  onef,  true,  "less, zerof");
  Check ( zero  <  zerof, true,  "less, zero");
  Check ( one   <  onef,  true,  "less, positive");
  Check ( monef <  mone,  true,  "less, negative");
  Check ( onef  <  one,   false, "less, false, positive");
  Check ( mtwo  <  mtwof, false, "less, false, negative");
  std::cout << std::endl;

  Check ( zerof <= zerof, true,  "less equal, equal, zerof");
  Check ( zero  <= zerof, true,  "less equal, less, zero");
  Check ( onef  <= onef,  true,  "less equal, equal, positive");
  Check ( monef <= mone,  true,  "less equal, less, negative");
  Check ( onef  <= one,   false, "less equal, false, positive");
  Check ( mtwo  <= mtwof, false, "less equal, false, negative");
  std::cout << std::endl;

  Check ( onef  >  zerof, true,  "greater, zerof");
  Check ( zerof >  zero,  true,  "greater, zero");
  Check ( onef  >  one,   true,  "greater, positive");
  Check ( mone  >  monef, true,  "greater, negative");
  Check ( one   >  onef,  false, "greater, false, positive");
  Check ( mtwof >  mtwo,  false, "greater, false, negative");
  std::cout << std::endl;

  Check ( zerof >= zerof, true,  "greater equal, equal, zerof");
  Check ( zerof >= zero,  true,  "greater equal, greater, zero");
  Check ( onef  >= onef,  true,  "greater equal, equal, positive");
  Check ( mone  >= monef, true,  "greater equal, greater, negative");
  Check ( one   >= onef,  false, "greater equal, false, positive");
  Check ( mtwof >= mtwo,  false, "greater equal, false, negative");
  std::cout << std::endl;

  Check ( (!zero)  == one,  true,  "not zero");
  Check ( (!zerof) == zero, true,  "not zerof");
  Check ( (!one)   == zero, true,  "not one");
  Check ( (+onef) == onef, true, "unary positive");
  Check ( (-onef) == monef, true, "unary negative");
}

class Int64x64InvertTestCase : public TestCase
{
public:
  Int64x64InvertTestCase ();
  virtual void DoRun (void);
  void Check (const int64_t factor);
  void CheckCase (const uint64_t factor,
		  const int64x64_t result, const int64x64_t expected,
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
				   const int64x64_t expected,
				   const std::string & msg,
				   const double tolerance /* = 0 */)
{
  std::cout << GetParent ()->GetName () << " Invert: "
	    << factor << ": ";
  if (result == expected)
    {
      std::cout << "pass:  ";
    }
  else
    {
      std::cout << "FAIL:  "
		<< "(res: " << result
		<< " exp: " << expected
		<< " tol: " << tolerance << ")  ";
    }
  std::cout << msg 
	    << std::endl;
  
  NS_TEST_ASSERT_MSG_EQ_TOL (result, expected, int64x64_t(tolerance), msg);
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
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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
  std::cout << GetParent ()->GetName () << ": " << GetName () << ":"
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

  std::cout << std::endl;
}

static class Int64x64128TestSuite : public TestSuite
{
public:
  Int64x64128TestSuite ()
    : TestSuite ("int64x64", UNIT)
  {
    AddTestCase (new Int64x64HiLoTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64ArithmeticTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64CompareTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InputTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InputOutputTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug455TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug863TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64Bug1786TestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64InvertTestCase (), TestCase::QUICK);
    AddTestCase (new Int64x64ImplTestCase (), TestCase::QUICK);
  }
} g_int64x64TestSuite;
