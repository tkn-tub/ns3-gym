/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#ifndef NS3_TEST_H
#define NS3_TEST_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <stdint.h>

#include "ns3/system-wall-clock-ms.h"

extern bool gBreakOnFailure;

// 
// Note on below macros:
//
// When multiple statements are used in a macro, they should be bound together 
// in a loop syntactically, so the macro can appear safely inside if clauses 
// or other places that expect a single statement or a statement block.  The
// "strange" do while construct is a generally expected best practice for
// defining a robust macro.
//

/**
 * \brief Convenience macro to extract the source directory of the current 
 * source file.
 *
 * \see TestCase::GetSourceDir
 */
#define NS_TEST_SOURCEDIR \
  TestCase::GetSourceDir (__FILE__)

// ===========================================================================
// Test for equality (generic version)
// ===========================================================================

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_EQ_INTERNAL(actual, limit, msg, file, line)                                  \
  do {                                                                                                  \
      if (!((actual) == (limit)))                                                                         \
        {                                                                                                 \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                           \
          std::ostringstream msgStream;                                                                   \
          msgStream << msg;                                                                               \
          std::ostringstream actualStream;                                                                \
          actualStream << actual;                                                                         \
          std::ostringstream limitStream;                                                                 \
          limitStream << limit;                                                                           \
          ReportTestFailure (std::string (# actual) + " (actual) == " + std::string (# limit) + " (limit)", \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);          \
          if (!ContinueOnFailure ())                                                                      \
            {                                                                                             \
              return;                                                                                \
            }                                                                                             \
        }                                                                                                 \
    } while (false)

/**
 * \brief Test that an actual and expected (limit) value are equal and report
 * and abort if not.
 *
 * Check to see if the expected (limit) value is equal to the actual value found
 * in a test case.  If the two values are equal nothing happens, but if the 
 * comparison fails, an error is reported in a consistent way and the execution
 * of the current test case is aborted.
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_ASSERT_MSG_EQ (result, true, 
 *      "cannot open file " << filename << " in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * \warning Do not use this macro if you are comparing floating point numbers
 * (float or double) as it is unlikely to do what you expect.  Use 
 * NS_TEST_ASSERT_MSG_EQ_TOL instead.
 */
#define NS_TEST_ASSERT_MSG_EQ(actual, limit, msg) \
  NS_TEST_ASSERT_MSG_EQ_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL_INTERNAL(actual, limit, msg, file, line)                                  \
  do {                                                                                                  \
      if (!((actual) == (limit)))                                                                         \
        {                                                                                                 \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                           \
          std::ostringstream msgStream;                                                                   \
          msgStream << msg;                                                                               \
          std::ostringstream actualStream;                                                                \
          actualStream << actual;                                                                         \
          std::ostringstream limitStream;                                                                 \
          limitStream << limit;                                                                           \
          ReportTestFailure (std::string (# actual) + " (actual) == " + std::string (# limit) + " (limit)", \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);          \
          if (!ContinueOnFailure ())                                                                      \
            {                                                                                             \
              return true;                                                                                \
            }                                                                                             \
        }                                                                                                 \
    } while (false)

/**
 * \brief Test that an actual and expected (limit) value are equal and report
 * and abort if not.
 *
 * Check to see if the expected (limit) value is equal to the actual value found
 * in a test case.  If the two values are equal nothing happens, but if the 
 * comparison fails, an error is reported in a consistent way and the execution
 * of the current test case is aborted.
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL (result, true, 
 *      "cannot open file " << filename << " in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * \warning Do not use this macro if you are comparing floating point numbers
 * (float or double) as it is unlikely to do what you expect.  Use 
 * NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL_TOL instead.
 *
 * This function returns a boolean value.
 *
 */
#define NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL(actual, limit, msg) \
  NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

/**
 * \internal
 * 
 * Required to avoid use of return statement which allows use in methods 
 * (esp. callbacks) returning void.
 */
#define NS_TEST_EXPECT_MSG_EQ_INTERNAL(actual, limit, msg, file, line)                                  \
  do {                                                                                                  \
      if (!((actual) == (limit)))                                                                         \
        {                                                                                                 \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                           \
          std::ostringstream msgStream;                                                                   \
          msgStream << msg;                                                                               \
          std::ostringstream actualStream;                                                                \
          actualStream << actual;                                                                         \
          std::ostringstream limitStream;                                                                 \
          limitStream << limit;                                                                           \
          ReportTestFailure (std::string (# actual) + " (actual) == " + std::string (# limit) + " (limit)", \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);          \
        }                                                                                                 \
    } while (false)

/**
 * \brief Test that an actual and expected (limit) value are equal and report
 * if not.
 *
 * Check to see if the expected (lmit) value is equal to the actual value found
 * in a test case.  If the two values are equal nothing happens, but if the 
 * comparison fails, an error is reported in a consistent way.  EXPECT* macros
 * do not return if an error is detected.
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_EXPECT_MSG_EQUAL (result, true, 
 *      "cannot open file " << filename << " in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * \warning Do not use this macro if you are comparing floating point numbers
 * (float or double) as it is unlikely to do what you expect.  Use 
 * NS_TEST_EXPECT_MSG_EQ_TOL instead.
 */
#define NS_TEST_EXPECT_MSG_EQ(actual, limit, msg) \
  NS_TEST_EXPECT_MSG_EQ_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

// ===========================================================================
// Test for equality with a provided tolerance (use for floating point 
// comparisons -- both float and double)
// ===========================================================================

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_EQ_TOL_INTERNAL(actual, limit, tol, msg, file, line)                                       \
  do {                                                                                                                \
      if ((actual) > (limit) + (tol) || (actual) < (limit) - (tol))                                                     \
        {                                                                                                               \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                                         \
          std::ostringstream msgStream;                                                                                 \
          msgStream << msg;                                                                                             \
          std::ostringstream actualStream;                                                                              \
          actualStream << actual;                                                                                       \
          std::ostringstream limitStream;                                                                               \
          limitStream << limit << " +- " << tol;                                                                        \
          std::ostringstream condStream;                                                                                \
          condStream << # actual << " (actual) < " << # limit << " (limit) + " << # tol << " (tol) && " <<                 \
          # actual << " (actual) > " << # limit << " (limit) - " << # tol << " (tol)";                       \
          ReportTestFailure (condStream.str (), actualStream.str (), limitStream.str (), msgStream.str (), file, line); \
          if (!ContinueOnFailure ())                                                                                    \
            {                                                                                                           \
              return;                                                                                              \
            }                                                                                                           \
        }                                                                                                               \
    } while (false)

/**
 * \brief Test that actual and expected (limit) values are equal to plus or minus
 * some tolerance and report and abort if not.
 *
 * Check to see if the expected (limit) value is equal to the actual value found
 * in a test case to some tolerance.  This is not the same thing as asking if
 * two floating point are equal to within some epsilon, but is useful for that
 * case.  This assertion is geared toward more of a measurement problem.  Consider
 * measuring a physical rod of some kind that you have ordered.  You need to 
 * determine if it is "good."  You won't measure the rod to an arbitrary precision
 * of sixteen significant figures, you will measure the rod to determine if its 
 * length is within the tolerances you provided.  For example, 12.00 inches plus
 * or minus .005 inch may be just fine.
 * 
 * In ns-3, you might want to measure a signal to noise ratio and check to see
 * if the answer is what you expect.  If you naively measure (double)1128.93 and
 * compare this number with a constant 1128.93 you are almost certainly going to
 * have your test fail because of floating point rounding errors.  We provide a
 * floating point comparison function ns3::TestDoubleIsEqual() but you will 
 * probably quickly find that is not what you want either.  It may turn out to
 * be the case that when you measured an snr that printed as 1128.93, what was 
 * actually measured was something more like 1128.9287653857625442 for example.
 * Given that the double epsilon is on the order of 0.0000000000000009, you would
 * need to provide sixteen significant figures of expected value for this kind of
 * test to pass even with a typical test for floating point "approximate equality."
 * That is clearly not required or desired.  You really want to be able to provide 
 * 1128.93 along with a tolerance just like you provided 12 inches +- 0.005 inch 
 * above.
 *
 * This assertion is designed for real measurements by taking into account
 * measurement tolerances.  By doing so it also automatically compensates for 
 * floating point rounding errors.    If you really want to check floating point
 * equality down to the numeric_limits<double>::epsilon () range, consider using 
 * ns3::TestDoubleIsEqual().
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_ASSERT_MSG_EQ_TOL (snr, 1128.93, 0.005, "wrong snr (" << snr << ") in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param tol Tolerance of the test.
 * \param msg Message that is output if the test does not pass.
 */
#define NS_TEST_ASSERT_MSG_EQ_TOL(actual, limit, tol, msg)                 \
  NS_TEST_ASSERT_MSG_EQ_TOL_INTERNAL (actual, limit, tol, msg, __FILE__, __LINE__)

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_EQ_TOL_RETURNS_BOOL_INTERNAL(actual, limit, tol, msg, file, line)                                       \
  do {                                                                                                                \
      if ((actual) > (limit) + (tol) || (actual) < (limit) - (tol))                                                     \
        {                                                                                                               \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                                         \
          std::ostringstream msgStream;                                                                                 \
          msgStream << msg;                                                                                             \
          std::ostringstream actualStream;                                                                              \
          actualStream << actual;                                                                                       \
          std::ostringstream limitStream;                                                                               \
          limitStream << limit << " +- " << tol;                                                                        \
          std::ostringstream condStream;                                                                                \
          condStream << # actual << " (actual) < " << # limit << " (limit) + " << # tol << " (tol) && " <<                 \
          # actual << " (actual) > " << # limit << " (limit) - " << # tol << " (tol)";                       \
          ReportTestFailure (condStream.str (), actualStream.str (), limitStream.str (), msgStream.str (), file, line); \
          if (!ContinueOnFailure ())                                                                                    \
            {                                                                                                           \
              return true;                                                                                              \
            }                                                                                                           \
        }                                                                                                               \
    } while (false)

/**
 * \brief Test that actual and expected (limit) values are equal to plus or minus
 * some tolerance and report and abort if not.
 *
 * Check to see if the expected (limit) value is equal to the actual value found
 * in a test case to some tolerance.  This is not the same thing as asking if
 * two floating point are equal to within some epsilon, but is useful for that
 * case.  This assertion is geared toward more of a measurement problem.  Consider
 * measuring a physical rod of some kind that you have ordered.  You need to 
 * determine if it is "good."  You won't measure the rod to an arbitrary precision
 * of sixteen significant figures, you will measure the rod to determine if its 
 * length is within the tolerances you provided.  For example, 12.00 inches plus
 * or minus .005 inch may be just fine.
 * 
 * In ns-3, you might want to measure a signal to noise ratio and check to see
 * if the answer is what you expect.  If you naively measure (double)1128.93 and
 * compare this number with a constant 1128.93 you are almost certainly going to
 * have your test fail because of floating point rounding errors.  We provide a
 * floating point comparison function ns3::TestDoubleIsEqual() but you will 
 * probably quickly find that is not what you want either.  It may turn out to
 * be the case that when you measured an snr that printed as 1128.93, what was 
 * actually measured was something more like 1128.9287653857625442 for example.
 * Given that the double epsilon is on the order of 0.0000000000000009, you would
 * need to provide sixteen significant figures of expected value for this kind of
 * test to pass even with a typical test for floating point "approximate equality."
 * That is clearly not required or desired.  You really want to be able to provide 
 * 1128.93 along with a tolerance just like you provided 12 inches +- 0.005 inch 
 * above.
 *
 * This assertion is designed for real measurements by taking into account
 * measurement tolerances.  By doing so it also automatically compensates for 
 * floating point rounding errors.    If you really want to check floating point
 * equality down to the numeric_limits<double>::epsilon () range, consider using 
 * ns3::TestDoubleIsEqual().
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_ASSERT_MSG_EQ_TOL_RETURNS_BOOL (snr, 1128.93, 0.005, "wrong snr (" << snr << ") in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param tol Tolerance of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * This function returns a boolean value.
 *
 */
#define NS_TEST_ASSERT_MSG_EQ_TOL_RETURNS_BOOL(actual, limit, tol, msg)                 \
  NS_TEST_ASSERT_MSG_EQ_TOL_RETURNS_BOOL_INTERNAL (actual, limit, tol, msg, __FILE__, __LINE__)

/**
 * \internal
 * 
 * Required to avoid use of return statement which allows use in methods 
 * (esp. callbacks) returning void.
 */
#define NS_TEST_EXPECT_MSG_EQ_TOL_INTERNAL(actual, limit, tol, msg, file, line)                                       \
  do {                                                                                                                \
      if ((actual) > (limit) + (tol) || (actual) < (limit) - (tol))                                                     \
        {                                                                                                               \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                                         \
          std::ostringstream msgStream;                                                                                 \
          msgStream << msg;                                                                                             \
          std::ostringstream actualStream;                                                                              \
          actualStream << actual;                                                                                       \
          std::ostringstream limitStream;                                                                               \
          limitStream << limit << " +- " << tol;                                                                        \
          std::ostringstream condStream;                                                                                \
          condStream << # actual << " (actual) < " << # limit << " (limit) + " << # tol << " (tol) && " <<                 \
          # actual << " (actual) > " << # limit << " (limit) - " << # tol << " (tol)";                       \
          ReportTestFailure (condStream.str (), actualStream.str (), limitStream.str (), msgStream.str (), file, line); \
        }                                                                                                               \
    } while (false)

/**
 * \brief Test that actual and expected (limit) values are equal to plus or minus
 * some tolerance and report if not.
 *
 * Check to see if the expected (limit) value is equal to the actual value found
 * in a test case to some tolerance.  This is not the same thing as asking if
 * two floating point are equal to within some epsilon, but is useful for that
 * case.  This assertion is geared toward more of a measurement problem.  Consider
 * measuring a physical rod of some kind that you have ordered.  You need to 
 * determine if it is "good."  You won't measure the rod to an arbitrary precision
 * of sixteen significant figures, you will measure the rod to determine if its 
 * length is within the tolerances you provided.  For example, 12.00 inches plus
 * or minus .005 inch may be just fine.
 * 
 * In ns-3, you might want to measure a signal to noise ratio and check to see
 * if the answer is what you expect.  If you naively measure (double)1128.93 and
 * compare this number with a constant 1128.93 you are almost certainly going to
 * have your test fail because of floating point rounding errors.  We provide a
 * floating point comparison function ns3::TestDoubleIsEqual() but you will 
 * probably quickly find that is not what you want either.  It may turn out to
 * be the case that when you measured an snr that printed as 1128.93, what was 
 * actually measured was something more like 1128.9287653857625442 for example.
 * Given that the double epsilon is on the order of 0.0000000000000009, you would
 * need to provide sixteen significant figures of expected value for this kind of
 * test to pass even with a typical test for floating point "approximate equality."
 * That is clearly not required or desired.  You really want to be able to provide 
 * 1128.93 along with a tolerance just like you provided 12 inches +- 0.005 inch 
 * above.
 *
 * This assertion is designed for real measurements by taking into account
 * measurement tolerances.  By doing so it also automatically compensates for 
 * floating point rounding errors.    If you really want to check floating point
 * equality down to the numeric_limits<double>::epsilon () range, consider using 
 * ns3::TestDoubleIsEqual().
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_EXPECT_MSG_EQ_TOL (snr, 1128.93, 0.005, "wrong snr (" << snr << ") in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param tol Tolerance of the test.
 * \param msg Message that is output if the test does not pass.
 */
#define NS_TEST_EXPECT_MSG_EQ_TOL(actual, limit, tol, msg) \
  NS_TEST_EXPECT_MSG_EQ_TOL_INTERNAL (actual, limit, tol, msg, __FILE__, __LINE__)

// ===========================================================================
// Test for inequality
// ===========================================================================

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_NE_INTERNAL(actual, limit, msg, file, line)                                  \
  do {                                                                                                  \
      if (!((actual) != (limit)))                                                                         \
        {                                                                                                 \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                           \
          std::ostringstream msgStream;                                                                   \
          msgStream << msg;                                                                               \
          std::ostringstream actualStream;                                                                \
          actualStream << actual;                                                                         \
          std::ostringstream limitStream;                                                                 \
          limitStream << limit;                                                                           \
          ReportTestFailure (std::string (# actual) + " (actual) != " + std::string (# limit) + " (limit)", \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);          \
          if (!ContinueOnFailure ())                                                                      \
            {                                                                                             \
              return;                                                                                \
            }                                                                                             \
        }                                                                                                 \
    } while (false)

/**
 * \brief Test that an actual and expected (limit) value are equal and report
 * and abort if not.
 *
 * Check to see if the expected (limit) value is not equal to the actual value
 * found in a test case.  If the two values are equal nothing happens, but if 
 * the comparison fails, an error is reported in a consistent way and the 
 * execution of the current test case is aborted.
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_ASSERT_MSG_NE (result, false, 
 *      "cannot open file " << filename << " in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * \warning Do not use this macro if you are comparing floating point numbers
 * (float or double).  Use NS_TEST_ASSERT_MSG_FLNE instead.
 */
#define NS_TEST_ASSERT_MSG_NE(actual, limit, msg) \
  NS_TEST_ASSERT_MSG_NE_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL_INTERNAL(actual, limit, msg, file, line)                                  \
  do {                                                                                                  \
      if (!((actual) != (limit)))                                                                         \
        {                                                                                                 \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                           \
          std::ostringstream msgStream;                                                                   \
          msgStream << msg;                                                                               \
          std::ostringstream actualStream;                                                                \
          actualStream << actual;                                                                         \
          std::ostringstream limitStream;                                                                 \
          limitStream << limit;                                                                           \
          ReportTestFailure (std::string (# actual) + " (actual) != " + std::string (# limit) + " (limit)", \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);          \
          if (!ContinueOnFailure ())                                                                      \
            {                                                                                             \
              return true;                                                                                \
            }                                                                                             \
        }                                                                                                 \
    } while (false)

/**
 * \brief Test that an actual and expected (limit) value are equal and report
 * and abort if not.
 *
 * Check to see if the expected (limit) value is not equal to the actual value
 * found in a test case.  If the two values are equal nothing happens, but if 
 * the comparison fails, an error is reported in a consistent way and the 
 * execution of the current test case is aborted.
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL (result, false, 
 *      "cannot open file " << filename << " in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * \warning Do not use this macro if you are comparing floating point numbers
 * (float or double).  Use NS_TEST_ASSERT_MSG_FLNE instead.
 *
 * This function returns a boolean value.
 *
 */
#define NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL(actual, limit, msg) \
  NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

/**
 * \internal
 * 
 * Required to avoid use of return statement which allows use in methods 
 * (callbacks) returning void.
 */
#define NS_TEST_EXPECT_MSG_NE_INTERNAL(actual, limit, msg, file, line)                                  \
  do {                                                                                                  \
      if (!((actual) != (limit)))                                                                         \
        {                                                                                                 \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                           \
          std::ostringstream msgStream;                                                                   \
          msgStream << msg;                                                                               \
          std::ostringstream actualStream;                                                                \
          actualStream << actual;                                                                         \
          std::ostringstream limitStream;                                                                 \
          limitStream << limit;                                                                           \
          ReportTestFailure (std::string (# actual) + " (actual) != " + std::string (# limit) + " (limit)", \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);          \
        }                                                                                                 \
    } while (false)

/**
 * \brief Test that an actual and expected (limit) value are equal and report
 * if not.
 *
 * Check to see if the expected (limit) value is equal to the actual value 
 * found in a test case.  If the two values are equal nothing happens, but if 
 * the comparison fails, an error is reported in a consistent way.  EXPECT* 
 * macros do not return if an error is detected.
 *
 * The message is interpreted as a stream, for example:
 *
 * \code
 * NS_TEST_EXPECT_MSG_EQUAL (result, true, 
 *      "cannot open file " << filename << " in test");
 * \endcode
 *
 * is legal.
 * 
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the expected value of the test.
 * \param msg Message that is output if the test does not pass.
 *
 * \warning Do not use this macro if you are comparing floating point numbers
 * (float or double).  Use NS_TEST_EXPECT_MSG_FLNE instead.
 */
#define NS_TEST_EXPECT_MSG_NE(actual, limit, msg) \
  NS_TEST_EXPECT_MSG_NE_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

// ===========================================================================
// Test for less than relation
// ===========================================================================

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_LT_INTERNAL(actual, limit, msg, file, line)                                   \
  do {                                                                                                   \
      if (!((actual) < (limit)))                                                                           \
        {                                                                                                  \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                            \
          std::ostringstream msgStream;                                                                    \
          msgStream << msg;                                                                                \
          std::ostringstream actualStream;                                                                 \
          actualStream << actual;                                                                          \
          std::ostringstream limitStream;                                                                  \
          limitStream << limit;                                                                            \
          ReportTestFailure (std::string (# actual) + " (actual) < " + std::string (# limit) + " (limit)",   \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);           \
          if (!ContinueOnFailure ())                                                                       \
            {                                                                                              \
              return;                                                                                 \
            }                                                                                              \
        }                                                                                                  \
    } while (false)

/**
 * \brief Test that an actual value is less than a limit and report and abort
 * if not.
 *
 * Check to see if the actual value found in a test case is less than the 
 * limit value.  If the actual value is lesser nothing happens, but if the 
 * check fails, an error is reported in a consistent way and the execution
 * of the current test case is aborted.
 *
 * The message is interpreted as a stream.
 *
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the limit value of the test.
 * \param msg Message that is output if the test does not pass.
 */
#define NS_TEST_ASSERT_MSG_LT(actual, limit, msg) \
  NS_TEST_ASSERT_MSG_LT_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

/**
 * \internal
 * 
 * Required to avoid use of return statement which allows use in methods 
 * (callbacks) returning void.
 */
#define NS_TEST_EXPECT_MSG_LT_INTERNAL(actual, limit, msg, file, line)                                   \
  do {                                                                                                   \
      if (!((actual) < (limit)))                                                                           \
        {                                                                                                  \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                            \
          std::ostringstream msgStream;                                                                    \
          msgStream << msg;                                                                                \
          std::ostringstream actualStream;                                                                 \
          actualStream << actual;                                                                          \
          std::ostringstream limitStream;                                                                  \
          limitStream << limit;                                                                            \
          ReportTestFailure (std::string (# actual) + " (actual) < " + std::string (# limit) + " (limit)",   \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);           \
        }                                                                                                  \
    } while (false)

/**
 * \brief Test that an actual value is less than a limit and report if not.
 *
 * Check to see if the actual value found in a test case is less than the 
 * limit value.  If the actual value is lesser nothing happens, but if the 
 * check fails, an error is reported in a consistent way.  EXPECT* macros do 
 * not return if an error is detected.
 *
 * The message is interpreted as a stream.
 *
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the limit value of the test.
 * \param msg Message that is output if the test does not pass.
 */
#define NS_TEST_EXPECT_MSG_LT(actual, limit, msg) \
  NS_TEST_EXPECT_MSG_LT_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

// ===========================================================================
// Test for greater than relation
// ===========================================================================

/**
 * \internal
 */
#define NS_TEST_ASSERT_MSG_GT_INTERNAL(actual, limit, msg, file, line)                                   \
  do {                                                                                                   \
      if (!((actual) > (limit)))                                                                           \
        {                                                                                                  \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                            \
          std::ostringstream msgStream;                                                                    \
          msgStream << msg;                                                                                \
          std::ostringstream actualStream;                                                                 \
          actualStream << actual;                                                                          \
          std::ostringstream limitStream;                                                                  \
          limitStream << limit;                                                                            \
          ReportTestFailure (std::string (# actual) + " (actual) > " + std::string (# limit) + " (limit)",   \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);           \
          if (!ContinueOnFailure ())                                                                       \
            {                                                                                              \
              return;                                                                                 \
            }                                                                                              \
        }                                                                                                  \
    } while (false)

/**
 * \brief Test that an actual value is greater than a limit and report and abort
 * if not.
 *
 * Check to see if the actual value found in a test case is greater than the 
 * limit value.  If the actual value is greater nothing happens, but if the 
 * check fails, an error is reported in a consistent way and the execution
 * of the current test case is aborted.
 *
 * The message is interpreted as a stream.
 *
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the limit value of the test.
 * \param msg Message that is output if the test does not pass.
 */
#define NS_TEST_ASSERT_MSG_GT(actual, limit, msg) \
  NS_TEST_ASSERT_MSG_GT_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

/**
 * \internal
 * 
 * Required to avoid use of return statement which allows use in methods 
 * (callbacks) returning void.
 */
#define NS_TEST_EXPECT_MSG_GT_INTERNAL(actual, limit, msg, file, line)                                   \
  do {                                                                                                   \
      if (!((actual) > (limit)))                                                                           \
        {                                                                                                  \
          if (gBreakOnFailure) { *(int *)0 = 0; }                                                            \
          std::ostringstream msgStream;                                                                    \
          msgStream << msg;                                                                                \
          std::ostringstream actualStream;                                                                 \
          actualStream << actual;                                                                          \
          std::ostringstream limitStream;                                                                  \
          limitStream << limit;                                                                            \
          ReporTesttFailure (std::string (# actual) + " (actual) > " + std::string (# limit) + " (limit)",   \
                             actualStream.str (), limitStream.str (), msgStream.str (), file, line);           \
        }                                                                                                  \
    } while (false)

/**
 * \brief Test that an actual value is greater than a limit and report if not.
 *
 * Check to see if the actual value found in a test case is greater than the 
 * limit value.  If the actual value is greater nothing happens, but if the 
 * check fails, an error is reported in a consistent way.  EXPECT* macros do 
 * not return if an error is detected.
 *
 * The message is interpreted as a stream.
 *
 * \param actual Expression for the actual value found during the test.
 * \param limit Expression for the limit value of the test.
 * \param msg Message that is output if the test does not pass.
 */
#define NS_TEST_EXPECT_MSG_GT(actual, limit, msg) \
  NS_TEST_EXPECT_MSG_GT_INTERNAL (actual, limit, msg, __FILE__, __LINE__)

namespace ns3 {

/**
 * \brief Compare two double precision floating point numbers and declare them
 * equal if they are within some epsilon of each other.
 *
 * Approximate comparison of floating point numbers near equality is trickier
 * than one may expect and is well-discussed in the literature.  Basic 
 * strategies revolve around a suggestion by Knuth to compare the floating 
 * point numbers as binary integers, supplying a maximum difference between
 * them .  This max difference is specified in Units in the Last Place (ulps)
 * or a floating point epsilon.
 *
 * This routine is based on the GNU Scientific Library function gsl_fcmp.
 * 
 * \param a The first of double precision floating point numbers to compare
 * \param b The second of double precision floating point numbers to compare
 * \param epsilon The second of double precision floating point numberss to compare
 * \returns Returns true if the doubles are equal to a precision defined by epsilon
 */
bool TestDoubleIsEqual (const double a, const double b, const double epsilon = std::numeric_limits<double>::epsilon ());

/**
 * \brief A single test case.
 */
class TestCase
{
public:
  TestCase (std::string name);
  virtual ~TestCase ();

  /**
   * \brief Run this test case.
   */
  void Run (void);

  /**
   * \brief Set the verbosity of this test case.
   * \param verbose Whether or not to turn on any output the
   *                test case may provide.
   */
  void SetVerbose (bool verbose);

  /**
   * \brief Tell the test case whether or not to continue testing if an error is
   * detected.
   *
   * Typically, test cases depend on some number of individual tests.  Often, 
   * these tests build on functionality that has been previously verified.  In 
   * this case, subsequent test failures may simply be alternate manifestations
   * of previously detected errors.  Some developers may only be interested in 
   * seeing the first failure.  Other developers may want to see all the 
   * information they can get, and want to see all failures.  This is a matter
   * of individual preference, so we allow this behavior to be configured.
   *
   * \param continueOnFailure If true, run tests after a failure has been 
   *                          detected, otherwise stop on the first error.
   */
  void SetContinueOnFailure (bool continueOnFailure);

  /**
   * \brief Set the name of this test case.
   */
  void SetName (std::string name);

  /**
   * \brief Get the name of this test case.
   */
  std::string GetName (void);

  /**
   * \brief Set the base directory of the ns-3 distribution.
   */
  void SetBaseDir (std::string dir);

  /**
   * \brief Get the base directory of the ns-3 distribution.
   */
  std::string GetBaseDir (void);

  /**
   * \brief Set the temporary file directory (where to write temporary files).
   */
  void SetTempDir (std::string dir);

  /**
   * \brief Get the temporary file directory .
   */
  std::string GetTempDir (void);

/**
 * \brief Get the source directory of the current source file.
 *
 * One of the basic models of the test environment is that dedicated test- 
 * and response vectors live in the same directory as the source file.  So it 
 * is a common operation to figure out what directory a given source file lives
 * in.
 *
 * __FILE__ provides almost all of what we need, but in the gnu toolchain it 
 * comes out as something like "../src/core/pcap-file-test-suite.cc".
 * 
 * We really don't want to have any dependency on the directory out of which a
 * test is run, so we ask that any test-runner give us the base directory of the 
 * distribution, which is set via TestCase::SetBaseDir().  That string will look 
 * something like "/home/user/repos/ns-3-allinone/ns-3-dev".
 *
 * This function stitches the two pieces together and removes the file name to 
 * return something like "/home/user/repos/ns-3-allinone/ns-3-dev/src/core/".
 *
 * \param file The current source file name relative to the base directory.
 * \returns The current source directory.
 *
 * \warning Always call this function as GetSourceDir (__FILE__) or use the 
 * convenience macro NS_TEST_SOURCEDIR.
 */
  std::string GetSourceDir (std::string file);

  /**
   * \brief Set the stream to which status and result messages will be written.
   *
   * We really don't want to have to pass an ofstream around to every function
   * and we especially don't want to have to make our clients plumb an ofstream
   * around so we need to save it.  Since file streams are not designed to be
   * copied or assigned (what does it mean to have duplicate streams to a file) 
   * we have to stash a pointer to the stream.
   *
   * \param ofs output file stream
   */
  void SetStream (std::ofstream *ofs);

  /**
   * \brief Get the stream to which status and result messages will be written.
   */
  std::ofstream *GetStream (void);

  /**
   * \brief Manually update the error status of this test case.
   *
   * This does a logical OR of the error argument with the current error status.
   * If the argument is false, it does nothing.  If the argument is true, it 
   * sets the error status to "an error has occurred."
   *
   * \param error The status to use to update the test case error status
   */
  void UpdateErrorStatus (bool error);

  /**
   * \brief Manually set the error status of this test case.
   *
   * This sets the current error status to the argument provided.  Can be used
   * to reset any previous errors if the argument is false.
   *
   * \param error The status to use to set the test case error status
   */
  void SetErrorStatus (bool error);

  /**
   * \brief Get the error status of this test case.
   */
  bool GetErrorStatus (void);

  /**
   * \brief Should test cases continue running in the presence of errors?
   * \returns True if the test case should continue, false otherwise.
   */
  bool ContinueOnFailure (void);

  /**
   * \brief Issue a test report than the test suite has started running.
   */
  void ReportStart (void);

  /**
   * \brief Issue a test report than the test case has declared success.
   */
  void ReportCaseSuccess (void);

  /**
   * \brief Issue a test report than the test case has declared failure.
   */
  void ReportCaseFailure (void);

  /**
   * \brief Issue a test report than the test case has found an error and
   * report the details.
   */
  void ReportTestFailure (std::string cond, std::string actual, std::string limit, std::string message, 
                          std::string file, int32_t line);

  /**
   * \brief Issue a test report than the test case has completed its run.
   */
  void ReportEnd (void);

protected:
  /**
   * \internal
   * \brief Implementation of reporting method for the start of the test case.
   */
  virtual void DoReportStart (void);

  /**
   * \internal
   * \brief Implementation of reporting method for success of the test case.
   */
  virtual void DoReportCaseSuccess (void);

  /**
   * \internal
   * \brief Implementation of reporting method for failure of the test case.
   */
  virtual void DoReportCaseFailure (void);

  /**
   * \internal
   * \brief Implementation of reporting method for failure of the test case.
   */
  virtual void DoReportTestFailure (std::string cond, std::string actual, std::string limit, std::string message, 
                                    std::string file, int32_t line);

  /**
   * \internal
   * \brief Implementation of reporting method for the end of the test case.
   */
  virtual void DoReportEnd (void);

  /**
   * \internal
   * \brief Implementation to do any local setup required for this test case.
   */
  virtual void DoSetup (void);

  /**
   * \internal
   * \brief Implementation to actually run this test case.
   */
  virtual void DoRun (void) = 0;

  /**
   * \internal
   * \brief Implementation to do any local setup required for this test case.
   */
  virtual void DoTeardown (void);

private:
  TestCase (TestCase& tc);
  TestCase& operator= (TestCase& tc);

  SystemWallClockMs m_msClock;
  std::string m_name;
  bool m_verbose;
  bool m_continueOnFailure;
  bool m_detailsReported;
  std::string m_basedir;
  std::string m_tempdir;
  std::ofstream *m_ofs;
  bool m_error;
};

/**
 * \brief A suite of tests to run.
 */
class TestSuite
{
public:
  /**
   * \enum TestType
   * \brief Type of test.
   */
  enum TestType {
    BVT = 1,    /**< This test suite implements a Build Verification Test */
    UNIT,       /**< This test suite implements a Unit Test */
    SYSTEM,     /**< This test suite implements a System Test */
    EXAMPLE,    /**< This test suite implements an Example Test */
    PERFORMANCE /**< This test suite implements a Performance Test */
  };

  /**
   * \brief Constuct a new test suite.
   *
   * \param name The name of the test suite.
   * \param type The TestType of the test suite (defaults to UNIT test).
   */
  TestSuite (std::string name, TestType type = UNIT);

  /**
   * \brief Destroy a test suite.
   */
  virtual ~TestSuite ();

  /**
   * \brief Run this test suite.
   *
   * \returns Boolean sense of "an error has occurred."
   */
  bool Run (void);

  /**
   * \brief Add an individual test case to this test suite.
   *
   * \param testCase Pointer to the test case object to be added.
   * \returns Integer assigned as identifer of the provided test case.
   */
  uint32_t AddTestCase (TestCase *testCase);

  /**
   * \brief Get the number of test cases that have been added to this test suite.
   *
   * \returns Number of test cases in the suite.
   */
  uint32_t GetNTestCases (void);

  /**
   * \brief Get the test case at index i.
   */
  TestCase *GetTestCase (uint32_t i);

  /**
   * \brief get the kind of test this test suite implements
   *
   * \returns the TestType of the suite.
   */
  TestType GetTestType (void);

  /**
   * \brief Set the verbosity of this test suite.
   * \param verbose Whether or not to turn on any output the
   *                test case may provide.
   */
  void SetVerbose (bool verbose);

  /**
   * \brief Tell the test suite and its test cases whether or not to continue
   * testing if an error is detected.
   *
   * Typically, test suites depend on some number of test cases, which in turn
   * depend on some number of individual tests.  Often, these tests build on 
   * functionality that has been previously verified.  In this case, subsequent
   * test failures may simply be alternate manifestations of previously detected
   * errors.  Some developers may only be interested in seeing the first failure.
   * Other developers may want to see all the information they can get, and want
   * to see all failures.  This is a matter of individual preference, so we allow
   * this behavior to be configured.
   *
   * \param continueOnFailure If true, continue running test cases after a 
   *                          failure has been detected, otherwise stop on the 
   *                          first error.
   */
  void SetContinueOnFailure (bool continueOnFailure);

  /**
   * \brief Set the name of this test suite.
   */
  void SetName (std::string name);

  /**
   * \brief Get the name of this test suite.
   */
  std::string GetName (void);

  /**
   * \brief Set the base directory of the ns-3 distribution.
   */
  void SetBaseDir (std::string basedir);

  /**
   * \brief Get the base directory of the ns-3 distribution.
   */
  std::string GetBaseDir (void);

  /**
   * \brief Set the temporary file directory (where to write temporary files).
   */
  void SetTempDir (std::string dir);

  /**
   * \brief Get the temporary file directory.
   */
  std::string GetTempDir (void);

  /**
   * \brief Set the stream to which status and result messages will be written.
   *
   * We really don't want to have to pass an ofstream around to every function
   * and we especially don't want to have to make our clients plumb an ofstream
   * around so we need to save it.  Since file streams are not designed to be
   * copied or assigned (what does it mean to have duplicate streams to a file) 
   * we have to stash a pointer to the stream.
   * \param ofs output file stream
   */
  void SetStream (std::ofstream *ofs);

  /**
   * \brief Manually update the error status of this test suite.
   *
   * This does a logical OR of the error argument with the current error status.
   * If the argument is false, it does nothing.  If the argument is true, it 
   * sets the error status to "an error has occurred."
   *
   * \param error The status to use to update the test suite error status
   */
  void UpdateErrorStatus (bool error);

  /**
   * \brief Manually set the error status of this test suite.
   *
   * This sets the current error status to the argument provided.  Can be used
   * to reset any previous errors if the argument is false.
   *
   * \param error The status to use to set the test suite error status
   */
  void SetErrorStatus (bool error);

  /**
   * \brief Get the error status of this test suite.
   */
  bool GetErrorStatus (void);

  /**
   * \brief Should test suite continue running cases in the presence of errors?
   * \returns True if the test suite should continue, false otherwise.
   */
  bool ContinueOnFailure (void);

  /**
   * \brief Issue a test report than the test suite has started running.
   */
  void ReportStart (void);

  /**
   * \brief Issue a test report than the test suite has declared success.
   */
  void ReportSuccess (void);

  /**
   * \brief Issue a test report than the test suite has found an error.
   */
  void ReportFailure (void);

  /**
   * \brief Issue a test report than the test suite has completed its run.
   */
  void ReportEnd (void);

protected:
  /**
   * \internal
   * \brief Implementation of reporting method for the start of the test suite.
   */
  virtual void DoReportStart (void);

  /**
   * \internal
   * \brief Implementation of reporting method for success of the test suite.
   */
  virtual void DoReportSuccess (void);

  /**
   * \internal
   * \brief Implementation of reporting method for failure of the test suite.
   */
  virtual void DoReportFailure (void);

  /**
   * \internal
   * \brief Implementation of reporting method for the end of the test suite.
   */
  virtual void DoReportEnd (void);

  /**
   * \internal
   * \brief Implementation to do any local setup required for this test suite.
   */
  virtual void DoSetup (void);

  /**
   * \internal
   * \brief Implementation to actually run this test suite.
   */
  virtual void DoRun (void);

  /**
   * \internal
   * \brief Implementation to do any local setup required for this test suite.
   */
  virtual void DoTeardown (void);

private:
  TestSuite (TestSuite& ts);
  TestSuite& operator= (TestSuite& ts);

  SystemWallClockMs m_msClock;
  std::string m_name;
  bool m_verbose;
  bool m_continueOnFailure;
  std::string m_basedir;
  std::string m_tempdir;
  std::ofstream *m_ofs;
  bool m_error;
  TestType m_type;

  typedef std::vector<TestCase *> TestCaseVector_t;
  TestCaseVector_t m_tests;
};

/**
 * \brief A runner to execute tests.
 */
class TestRunner
{
public:
  static uint32_t AddTestSuite (TestSuite *testSuite);
  static uint32_t GetNTestSuites (void);
  static TestSuite *GetTestSuite (uint32_t n);
};

/**
 * \brief A simple way to store test vectors (for stimulus or from responses)
 */
template <typename T>
class TestVectors
{
public:
  TestVectors ();
  virtual ~TestVectors ();

  void Reserve (uint32_t reserve);

  uint32_t Add (T vector);

  uint32_t GetN (void) const;
  T Get (uint32_t i) const;

private:
  TestVectors (const TestVectors& tv);
  TestVectors& operator= (const TestVectors& tv);
  bool operator== (const TestVectors& tv) const;

  typedef std::vector<T> TestVector_t;
  TestVector_t m_vectors;
};

template <typename T>
TestVectors<T>::TestVectors ()
  : m_vectors ()
{
}

template <typename T>
void
TestVectors<T>::Reserve (uint32_t reserve)
{
  m_vectors.reserve (reserve);
}

template <typename T>
TestVectors<T>::~TestVectors ()
{
}

template <typename T>
uint32_t
TestVectors<T>::Add (T vector)
{
  uint32_t index = m_vectors.size ();
  m_vectors.push_back (vector);
  return index;
}

template <typename T>
uint32_t 
TestVectors<T>::GetN (void) const
{
  return m_vectors.size ();
}

template <typename T>
T
TestVectors<T>::Get (uint32_t i) const
{
  NS_ABORT_MSG_UNLESS (m_vectors.size () > i, "TestVectors::Get(): Bad index");
  return m_vectors[i];
}

} // namespace ns3 

#endif /* NS3_TEST_H */
