/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef TEST_H
#define TEST_H

#include <list>
#include <string>
#include <utility>
#include <ostream>

#ifdef RUN_SELF_TESTS

namespace ns3 {

class TestManager;

/**
 * \ingroup core
 * \defgroup test Test
 */
/**
 * \ingroup test
 *
 * \brief base class for new regressions tests
 *
 * To add a new regression test, you need to:
 *    - create subclass of this abstract base class
 *    - instantiate once this subclass through a static
 *      variable.
 *
 * The following sample code shows you how to do this:
 * \include samples/main-test.cc
 */
class Test {
public:
  /**
   * \param name the name of the test
   */
  Test (char const *name);
  virtual ~Test ();

  /**
   * \returns true if the test was successful, false otherwise.
   */
  virtual bool RunTests (void) = 0;

protected:
  /**
   * \returns an output stream which base classes can write to
   *          to return extra information on test errors.
   */
  std::ostream &Failure (void);
};

/**
 * \ingroup test
 *
 * \brief gather and run all regression tests
 */
class TestManager {
public:
  /**
   * Enable verbose output. If you do not enable verbose output,
   * nothing is printed on screen during the test runs.
   */
  static void EnableVerbose (void);
  /**
   * \returns true if all tests passed, false otherwise.
   *
   * run all registered regression tests
   */
  static bool RunTests (void);

  static bool RunTest (std::string name);

  static void PrintTestNames (std::ostream &os);

private:
  friend class Test;
  static void Add (Test *test, char const *name);
  static std::ostream &Failure (void);
  static TestManager *Get (void);
  bool RealRunTests (void);
  bool RealRunTest (std::string name);

  TestManager ();
  ~TestManager ();

  typedef std::list<std::pair<Test *,std::string *> > Tests;
  typedef std::list<std::pair<Test *,std::string *> >::iterator TestsI;
  typedef std::list<std::pair<Test *,std::string *> >::const_iterator TestsCI;

  Tests m_tests;
  bool m_verbose;
};
}; // namespace ns3 

#define NS_TEST_ASSERT_EQUAL_FILELINE(got, expected, file, line)    \
  do {                                                              \
    if (! ((got) == (expected)))                                        \
      {                                                             \
        Failure () << file << ":" <<line                            \
                   << ": expected " << (expected)                   \
                   << ", got " << (got) << std::endl;               \
        result = false;                                             \
      }                                                             \
  } while (false)

#define NS_TEST_ASSERT_UNEQUAL_FILELINE(got, expected,file,line)    \
  do {                                                              \
    if ((got) == (expected))                                        \
      {                                                             \
        Failure () << file << ":" <<line                            \
                   << ": did not want " << (expected)               \
                   << ", got " << (got) << std::endl;               \
        result = false;                                             \
      }                                                             \
  } while (false)


#define NS_TEST_ASSERT_FILELINE(assertion, file, line)  \
  do {                                                  \
    if (!(assertion))                                   \
      {                                                 \
        Failure () << file << ":" <<line                \
                   << ": assertion `" << #assertion     \
                   << "' failed." << std::endl;         \
        result = false;                                 \
      }                                                 \
  } while (false)



/**
 * Convenience macro to check that a value returned by a test is what
 * is expected.  Note: this macro assumes a 'bool result = true'
 * declaration exists in the test function body, and that the function
 * returns that value.
 *
 * \param got value obtained from the test
 * \param expected value that the test is expected to return
 */
#define NS_TEST_ASSERT_EQUAL(got, expected)             \
  NS_TEST_ASSERT_EQUAL_FILELINE(got,expected,__FILE__,__LINE__)

/**
 * Convenience macro to check that a value returned by a test is what
 * is expected.  Note: this macro assumes a 'bool result = true'
 * declaration exists in the test function body, and that the function
 * returns that value.
 *
 * \param got value obtained from the test
 * \param expected value that the test is expected to return
 */
#define NS_TEST_ASSERT_UNEQUAL(got, expected)           \
  NS_TEST_ASSERT_UNEQUAL_FILELINE(got,expected,__FILE__,__LINE__)

/**
 * Convenience macro to check an assertion is held during an unit
 * test.  Note: this macro assumes a 'bool result = true' declaration
 * exists in the test function body, and that the function returns
 * that value.
 *
 * \param assertion expression that must be true if the test did not fail
 */
#define NS_TEST_ASSERT(assertion)                       \
  NS_TEST_ASSERT_FILELINE (assertion, __FILE__,__LINE__)


#endif /* RUN_SELF_TESTS */

#endif /* TEST_H */
