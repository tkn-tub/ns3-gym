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

#include "test.h"
#include "assert.h"
#include "abort.h"
#include "system-path.h"
#include "log.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <list>
#include <map>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Test");

bool
TestDoubleIsEqual (const double x1, const double x2, const double epsilon)
{
  NS_LOG_FUNCTION (x1 << x2 << epsilon);
  int exponent;
  double delta, difference;

  //
  // Find exponent of largest absolute value
  //
  {
    double max = (std::fabs (x1) > std::fabs (x2)) ? x1 : x2;
    (void)std::frexp (max, &exponent);
  }

  //
  // Form a neighborhood of size  2 * delta
  //
  delta = std::ldexp (epsilon, exponent);
  difference = x1 - x2;

  if (difference > delta || difference < -delta)
    {
      return false;
    }
  return true;
} 

struct TestCaseFailure
{
  TestCaseFailure (std::string _cond, std::string _actual, 
                   std::string _limit, std::string _message, 
                   std::string _file, int32_t _line);
  std::string cond;
  std::string actual;
  std::string limit;
  std::string message; 
  std::string file;
  int32_t line;
};
std::ostream & operator << (std::ostream & os, const TestCaseFailure & failure)
{
  os << "    test=\""  << failure.cond
     << "\" actual=\"" << failure.actual
     << "\" limit=\""  << failure.limit
     << "\" in=\""     << failure.file
     << ":"            << failure.line 
     << "\" "          << failure.message;

  return os;
}

struct TestCase::Result
{
  Result ();
  SystemWallClockMs clock;
  std::vector<TestCaseFailure> failure;
  bool childrenFailed;
};

class TestRunnerImpl
{
public:
  void AddTestSuite (TestSuite *testSuite);
  void StartTestCase (std::string name);
  void EndTestCase (void);
  void ReportTestFailure (std::string cond, std::string actual, 
                      std::string limit, std::string message, 
                      std::string file, int32_t line);
  bool MustAssertOnFailure (void) const;
  bool MustContinueOnFailure (void) const;
  bool MustUpdateData (void) const;
  std::string GetTopLevelSourceDir (void) const;
  std::string GetTempDir (void) const;

  int Run (int argc, char *argv[]);

  static TestRunnerImpl *Instance (void);

private:
  TestRunnerImpl ();
  ~TestRunnerImpl ();

  bool IsTopLevelSourceDir (std::string path) const;
  std::string ReplaceXmlSpecialCharacters (std::string xml) const;
  void PrintReport (TestCase *test, std::ostream *os, bool xml, int level);
  void PrintTestNameList (std::list<TestCase *>::const_iterator begin, 
                          std::list<TestCase *>::const_iterator end,
                          bool printTestType) const;
  void PrintTestTypeList (void) const;
  void PrintHelp (const char *programName) const;
  std::list<TestCase *> FilterTests (std::string testName,
                                     enum TestSuite::Type testType,
                                     enum TestCase::TestDuration maximumTestDuration);


  typedef std::vector<TestSuite *> TestSuiteVector;

  TestSuiteVector m_suites;
  std::string m_tempDir;
  bool m_verbose;
  bool m_assertOnFailure;
  bool m_continueOnFailure;
  bool m_updateData;
};



TestCaseFailure::TestCaseFailure (std::string _cond, std::string _actual, 
                                  std::string _limit, std::string _message, 
                                  std::string _file, int32_t _line)
  : cond (_cond), actual (_actual), limit (_limit),
    message (_message), file (_file), line (_line)
{
  NS_LOG_FUNCTION (this << _cond << _actual << _limit << _message << _file << _line);
}
TestCase::Result::Result ()
  : childrenFailed (false)
{
  NS_LOG_FUNCTION (this);
}



TestCase::TestCase (std::string name)
  : m_parent (0),
    m_dataDir (""),
    m_runner (0),
    m_result (0),
    m_name (name),
    m_duration (TestCase::QUICK)
{
  NS_LOG_FUNCTION (this << name);
}

TestCase::~TestCase ()
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_runner == 0);
  m_parent = 0;
  delete m_result;
  for (std::vector<TestCase *>::const_iterator i = m_children.begin (); i != m_children.end (); ++i)
    {
      delete *i;
    }
  m_children.clear ();
}

void
TestCase::AddTestCase (TestCase *testCase)
{
  AddTestCase (testCase, TestCase::QUICK);
}

void
TestCase::AddTestCase (TestCase *testCase, enum TestCase::TestDuration duration)
{
  // Record this for use later when all test cases are run.
  testCase->m_duration = duration;

  NS_LOG_FUNCTION (&testCase);
  m_children.push_back (testCase);
  testCase->m_parent = this;

  std::string::size_type slash, antislash;
  slash = testCase->m_name.find ("/");
  antislash = testCase->m_name.find ("\\");
  if (slash != std::string::npos || antislash != std::string::npos)
    {
      std::string fullname = testCase->m_name;
      TestCase *current = testCase->m_parent;
      while (current != 0)
        {
          fullname = current->m_name + "/" + fullname;
          current = current->m_parent;
        }
      if (slash != std::string::npos)
        {
          NS_FATAL_ERROR ("Invalid test name: cannot contain slashes: \"" << fullname << "\"");
        }
      if (antislash != std::string::npos)
        {
          NS_FATAL_ERROR ("Invalid test name: cannot contain antislashes: \"" << fullname << "\"");
        }
    }
}

bool
TestCase::IsFailed (void) const
{
  NS_LOG_FUNCTION (this);
  return m_result->childrenFailed || !m_result->failure.empty ();
}

void 
TestCase::Run (TestRunnerImpl *runner)
{
  NS_LOG_FUNCTION (this << runner);
  m_result = new Result ();
  m_runner = runner;
  DoSetup ();
  m_result->clock.Start ();
  for (std::vector<TestCase *>::const_iterator i = m_children.begin (); i != m_children.end (); ++i)
    {
      TestCase *test = *i;
      test->Run (runner);
      if (IsFailed ())
        {
          goto out;
        }
    }
  DoRun ();
 out:
  m_result->clock.End ();
  DoTeardown ();
  m_runner = 0;
}
std::string 
TestCase::GetName (void) const
{
  NS_LOG_FUNCTION (this);
  return m_name;
}
TestCase *
TestCase::GetParent () const
{
  return m_parent;
}

void
TestCase::ReportTestFailure (std::string cond, std::string actual, 
                             std::string limit, std::string message, 
                             std::string file, int32_t line)
{
  NS_LOG_FUNCTION (this << cond << actual << limit << message << file << line);
  m_result->failure.push_back (TestCaseFailure (cond, actual, limit,
                                                message, file, line));
  // set childrenFailed flag on parents.
  TestCase *current = m_parent;
  while (current != 0)
    {
      current->m_result->childrenFailed = true;
      current = current->m_parent;
    }

}
bool 
TestCase::MustAssertOnFailure (void) const
{
  NS_LOG_FUNCTION (this);
  return m_runner->MustAssertOnFailure ();
}
bool 
TestCase::MustContinueOnFailure (void) const
{
  NS_LOG_FUNCTION (this);
  return m_runner->MustContinueOnFailure ();
}

std::string 
TestCase::CreateDataDirFilename (std::string filename)
{
  NS_LOG_FUNCTION (this << filename);
  const TestCase *current = this;
  while (current != 0 && current->m_dataDir == "")
    {
      current = current->m_parent;
    }
  if (current == 0)
    {
      NS_FATAL_ERROR ("No one called SetDataDir prior to calling this function");
    }

  std::string a = SystemPath::Append (m_runner->GetTopLevelSourceDir (), current->m_dataDir);
  std::string b = SystemPath::Append (a, filename);
  return b;
}
std::string 
TestCase::CreateTempDirFilename (std::string filename)
{
  NS_LOG_FUNCTION (this << filename);
  if (m_runner->MustUpdateData ())
    {
      return CreateDataDirFilename (filename);
    }
  else
    {
      std::list<std::string> names;
      const TestCase *current = this;
      while (current != 0)
        {
          names.push_front (current->m_name);
          current = current->m_parent;
        }
      std::string tempDir = SystemPath::Append (m_runner->GetTempDir (), SystemPath::Join (names.begin (), names.end ()));
      SystemPath::MakeDirectories (tempDir);
      return SystemPath::Append (tempDir, filename);
    }
}
bool 
TestCase::GetErrorStatus (void) const
{
  NS_LOG_FUNCTION (this);
  return IsStatusFailure ();
}
bool 
TestCase::IsStatusFailure (void) const
{
  NS_LOG_FUNCTION (this);
  return !IsStatusSuccess ();
}
bool 
TestCase::IsStatusSuccess (void) const
{
  NS_LOG_FUNCTION (this);
  return m_result->failure.empty ();
}

void 
TestCase::SetDataDir (std::string directory)
{
  NS_LOG_FUNCTION (this << directory);
  m_dataDir = directory;
}

void 
TestCase::DoSetup (void)
{
  NS_LOG_FUNCTION (this);
}
void 
TestCase::DoTeardown (void)
{
  NS_LOG_FUNCTION (this);
}


TestSuite::TestSuite (std::string name, TestSuite::Type type)
  : TestCase (name), 
    m_type (type)
{
  NS_LOG_FUNCTION (this << name << type);
  TestRunnerImpl::Instance ()->AddTestSuite (this);
}

TestSuite::Type 
TestSuite::GetTestType (void)
{
  NS_LOG_FUNCTION (this);
  return m_type;
}

void 
TestSuite::DoRun (void)
{
  NS_LOG_FUNCTION (this);
}

TestRunnerImpl::TestRunnerImpl ()
 : m_tempDir (""),
   m_assertOnFailure (false),
   m_continueOnFailure (true),
   m_updateData (false)
{
  NS_LOG_FUNCTION (this);
}

TestRunnerImpl::~TestRunnerImpl ()
{
  NS_LOG_FUNCTION (this);
}



TestRunnerImpl *
TestRunnerImpl::Instance (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static TestRunnerImpl runner;
  return &runner;
}

void
TestRunnerImpl::AddTestSuite (TestSuite *testSuite)
{
  NS_LOG_FUNCTION (this << testSuite);
  m_suites.push_back (testSuite);
}


bool 
TestRunnerImpl::MustAssertOnFailure (void) const
{
  NS_LOG_FUNCTION (this);
  return m_assertOnFailure;
}
bool 
TestRunnerImpl::MustContinueOnFailure (void) const
{
  NS_LOG_FUNCTION (this);
  return m_continueOnFailure;
}

bool 
TestRunnerImpl::MustUpdateData (void) const
{
  NS_LOG_FUNCTION (this);
  return m_updateData;
}
std::string
TestRunnerImpl::GetTempDir (void) const
{
  NS_LOG_FUNCTION (this);
  return m_tempDir;
}
bool
TestRunnerImpl::IsTopLevelSourceDir (std::string path) const
{
  NS_LOG_FUNCTION (this << path);
  bool haveVersion = false;
  bool haveLicense = false;
  
  //
  // If there's a file named VERSION and a file named LICENSE in this
  // directory, we assume it's our top level source directory.
  //

  std::list<std::string> files = SystemPath::ReadFiles (path);
  for (std::list<std::string>::const_iterator i = files.begin (); i != files.end (); ++i)
    {
      if (*i == "VERSION")
        {
          haveVersion = true;
        }
      else if (*i == "LICENSE")
        {
          haveLicense = true;
        }
    }
  
  return haveVersion && haveLicense;
}

std::string 
TestRunnerImpl::GetTopLevelSourceDir (void) const
{
  NS_LOG_FUNCTION (this);
  std::string self = SystemPath::FindSelfDirectory ();
  std::list<std::string> elements = SystemPath::Split (self);
  while (!elements.empty ())
    {
      std::string path = SystemPath::Join (elements.begin (), elements.end ());
      if (IsTopLevelSourceDir (path))
        {
          return path;
        }
      elements.pop_back ();
    }
  NS_FATAL_ERROR ("Could not find source directory from self=" << self);
}

//
// XML files have restrictions on certain characters that may be present in
// data.  We need to replace these characters with their alternate 
// representation on the way into the XML file.
//
std::string
TestRunnerImpl::ReplaceXmlSpecialCharacters (std::string xml) const
{
  NS_LOG_FUNCTION (this << xml);
  typedef std::map <char, std::string> specials_map;
  specials_map specials;
  specials['<'] = "&lt;";
  specials['>'] = "&gt;";
  specials['&'] = "&amp;";
  specials['"'] = "&#39;";
  specials['\''] = "&quot;";

  std::string result;
  std::size_t length = xml.length ();

  for (size_t i = 0; i < length; ++i)
    {
      char character = xml[i];

      specials_map::const_iterator it = specials.find (character);

      if (it == specials.end ())
        {
          result.push_back (character);
        }
      else
        {
          result += it->second;
        }
    }
  return result;
}

struct Indent
{
  Indent (int level);
  int level;
};
Indent::Indent (int _level)
  : level (_level)
{
  NS_LOG_FUNCTION (this << _level);
}
std::ostream &operator << (std::ostream &os, const Indent &val)
{
  for (int i = 0; i < val.level; i++)
    {
      os << "  ";
    }
  return os;
}

void
TestRunnerImpl::PrintReport (TestCase *test, std::ostream *os, bool xml, int level)
{
  NS_LOG_FUNCTION (this << test << os << xml << level);
  if (test->m_result == 0)
    {
      // Do not print reports for tests that were not run.
      return;
    }
  // Report times in seconds, from ms timer
  const double MS_PER_SEC = 1000.;
  double real = test->m_result->clock.GetElapsedReal () / MS_PER_SEC;
  double user = test->m_result->clock.GetElapsedUser () / MS_PER_SEC;
  double system = test->m_result->clock.GetElapsedSystem () / MS_PER_SEC;

  std::streamsize oldPrecision = (*os).precision (3);
  *os << std::fixed;

  std::string statusString = test->IsFailed ()?"FAIL":"PASS";
  if (xml)
    {
      *os << Indent (level) << "<Test>" << std::endl;
      *os << Indent (level+1) << "<Name>" << ReplaceXmlSpecialCharacters (test->m_name)
          << "</Name>" << std::endl;
      *os << Indent (level+1) << "<Result>" << statusString << "</Result>" << std::endl;
      *os << Indent (level+1) << "<Time real=\"" << real << "\" user=\"" << user 
          << "\" system=\"" << system << "\"/>" << std::endl;
      for (uint32_t i = 0; i < test->m_result->failure.size (); i++)
        {
          TestCaseFailure failure = test->m_result->failure[i];
          *os << Indent (level+2) << "<FailureDetails>" << std::endl
              << Indent (level+3) << "<Condition>" 
              << ReplaceXmlSpecialCharacters (failure.cond) << "</Condition>" << std::endl
              << Indent (level+3) << "<Actual>" 
              << ReplaceXmlSpecialCharacters (failure.actual) << "</Actual>" << std::endl
              << Indent (level+3) << "<Limit>" 
              << ReplaceXmlSpecialCharacters (failure.limit) << "</Limit>" << std::endl
              << Indent (level+3) << "<Message>" 
              << ReplaceXmlSpecialCharacters (failure.message) << "</Message>" << std::endl
              << Indent (level+3) << "<File>" 
              << ReplaceXmlSpecialCharacters (failure.file) << "</File>" << std::endl
              << Indent (level+3) << "<Line>" << failure.line << "</Line>" << std::endl
              << Indent (level+2) << "</FailureDetails>" << std::endl;
        }
      for (uint32_t i = 0; i < test->m_children.size (); i++)
        {
          TestCase *child = test->m_children[i];
          PrintReport (child, os, xml, level + 1);
        }
      *os << Indent (level) << "</Test>" << std::endl;
    }
  else
    {
      *os << Indent (level) << statusString << " " << test->GetName () 
          << " " << real << " s" << std::endl;
      if (m_verbose)
        {
          for (uint32_t i = 0; i < test->m_result->failure.size (); i++)
            {
              *os << Indent (level) << test->m_result->failure[i] << std::endl;
            }
          for (uint32_t i = 0; i < test->m_children.size (); i++)
            {
              TestCase *child = test->m_children[i];
              PrintReport (child, os, xml, level + 1);
            }
        }
    }

  (*os).unsetf(std::ios_base::floatfield);
  (*os).precision (oldPrecision);
}
  
void
TestRunnerImpl::PrintHelp (const char *program_name) const
{
  NS_LOG_FUNCTION (this << program_name);
  std::cout << "Usage: " << program_name << " [OPTIONS]" << std::endl
            << std::endl
            << "Options: " << std::endl
            << "  --help                 : print these options" << std::endl
            << "  --print-test-name-list : print the list of names of tests available" << std::endl
            << "  --list                 : an alias for --print-test-name-list" << std::endl
            << "  --print-test-types     : print the type of tests along with their names" << std::endl
            << "  --print-test-type-list : print the list of types of tests available" << std::endl
            << "  --print-temp-dir       : print name of temporary directory before running " << std::endl
            << "                           the tests" << std::endl
            << "  --test-type=TYPE       : process only tests of type TYPE" << std::endl
            << "  --test-name=NAME       : process only test whose name matches NAME" << std::endl
            << "  --suite=NAME           : an alias (here for compatibility reasons only) " << std::endl
            << "                           for --test-name=NAME" << std::endl
            << "  --assert-on-failure    : when a test fails, crash immediately (useful" << std::endl
            << "                           when running under a debugger" << std::endl
            << "  --stop-on-failure      : when a test fails, stop immediately" << std::endl
            << "  --fullness=FULLNESS    : choose the duration of tests to run: QUICK, " << std::endl
            << "                           EXTENSIVE, or TAKES_FOREVER, where EXTENSIVE " << std::endl
            << "                           includes QUICK and TAKES_FOREVER includes " << std::endl
            << "                           QUICK and EXTENSIVE (only QUICK tests are " << std::endl
            << "                           run by default)" << std::endl
            << "  --verbose              : print details of test execution" << std::endl
            << "  --xml                  : format test run output as xml" << std::endl
            << "  --tempdir=DIR          : set temp dir for tests to store output files" << std::endl
            << "  --datadir=DIR          : set data dir for tests to read reference files" << std::endl
            << "  --out=FILE             : send test result to FILE instead of standard "
            << "output" << std::endl
            << "  --append=FILE          : append test result to FILE instead of standard "
            << "output" << std::endl
    ;  
}

void
TestRunnerImpl::PrintTestNameList (std::list<TestCase *>::const_iterator begin, 
                                   std::list<TestCase *>::const_iterator end,
                                   bool printTestType) const
{
  NS_LOG_FUNCTION (this << &begin << &end << printTestType);
  std::map<TestSuite::Type, std::string> label;

  label[TestSuite::ALL]         = "all          ";
  label[TestSuite::BVT]         = "bvt          ";
  label[TestSuite::UNIT]        = "unit         ";
  label[TestSuite::SYSTEM]      = "system       ";
  label[TestSuite::EXAMPLE]     = "example      ";
  label[TestSuite::PERFORMANCE] = "performance  ";

  for (std::list<TestCase *>::const_iterator i = begin; i != end; ++i)
    {
      TestSuite * test= dynamic_cast<TestSuite *>(*i);
      NS_ASSERT (test != 0);
      if (printTestType)
        {
          std::cout << label[test->GetTestType ()];
        }
      std::cout << test->GetName () << std::endl;
    }
}

void
TestRunnerImpl::PrintTestTypeList (void) const
{
  NS_LOG_FUNCTION (this);
  std::cout << "  bvt:         Build Verification Tests (to see if build completed successfully)" << std::endl;
  std::cout << "  core:        Run all TestSuite-based tests (exclude examples)" << std::endl;
  std::cout << "  example:     Examples (to see if example programs run successfully)" << std::endl;
  std::cout << "  performance: Performance Tests (check to see if the system is as fast as expected)" << std::endl;
  std::cout << "  system:      System Tests (spans modules to check integration of modules)" << std::endl;
  std::cout << "  unit:        Unit Tests (within modules to check basic functionality)" << std::endl;
}


std::list<TestCase *>
TestRunnerImpl::FilterTests (std::string testName,
                             enum TestSuite::Type testType,
                             enum TestCase::TestDuration maximumTestDuration)
{
  NS_LOG_FUNCTION (this << testName << testType);
  std::list<TestCase *> tests;
  for (uint32_t i = 0; i < m_suites.size (); ++i)
    {
      TestSuite *test = m_suites[i];
      if (testType != TestSuite::ALL && test->GetTestType () != testType)
        {
          // skip test
          continue;
        }
      if (testName != "" && test->GetName () != testName)
        {
          // skip test
          continue;
        }

      // Remove any test cases that should be skipped.
      std::vector<TestCase *>::iterator j;
      for (j = test->m_children.begin (); j != test->m_children.end ();)
        {
          TestCase *testCase = *j;

          // If this test case takes longer than the maximum test
          // duration that should be run, then don't run it.
          if (testCase->m_duration > maximumTestDuration)
            {
              // Free this test case's memory.
              delete *j;

              // Remove this test case from the test suite.
              j = test->m_children.erase (j);
            }
          else
            {
              // Only advance through the vector elements if this test
              // case wasn't deleted.
              ++j;
            }
        }

      // Add this test suite.
      tests.push_back (test);
    }
  return tests;
}


int 
TestRunnerImpl::Run (int argc, char *argv[])
{
  NS_LOG_FUNCTION (this << argc << argv);
  std::string testName = "";
  std::string testTypeString = "";
  std::string out = "";
  std::string fullness = "";
  bool xml = false;
  bool append = false;
  bool printTempDir = false;
  bool printTestTypeList = false;
  bool printTestNameList = false;
  bool printTestTypeAndName = false;
  enum TestCase::TestDuration maximumTestDuration = TestCase::QUICK;
  char *progname = argv[0];

  argv++;

  while (*argv != 0)
    {
      char *arg = *argv;

      if (strcmp(arg, "--assert-on-failure") == 0)
        {
          m_assertOnFailure = true;
        }
      else if (strcmp (arg, "--stop-on-failure") == 0)
        {
          m_continueOnFailure = false;
        }
      else if (strcmp (arg, "--verbose") == 0)
        {
          m_verbose = true;
        }
      else if (strcmp (arg, "--print-temp-dir") == 0)
        {
          printTempDir = true;
        }
      else if (strcmp (arg, "--update-data") == 0)
        {
          m_updateData = true;
        }
      else if (strcmp (arg, "--help") == 0)
        {
          PrintHelp (progname);
          return 0;
        }
      else if (strcmp (arg, "--print-test-name-list") == 0 ||
               strcmp(arg, "--list") == 0)
        {
          printTestNameList = true;
        }
      else if (strcmp (arg, "--print-test-types") == 0)
        {
          printTestTypeAndName = true;
        }
      else if (strcmp (arg, "--print-test-type-list") == 0)
        {
          printTestTypeList = true;
        }
     else if (strcmp(arg, "--append") == 0)
        {
          append = true;
        }
      else if (strcmp(arg, "--xml") == 0)
        {
          xml = true;
        }
      else if (strncmp(arg, "--test-type=", strlen("--test-type=")) == 0)
        {
          testTypeString = arg + strlen("--test-type=");
        }
      else if (strncmp(arg, "--test-name=", strlen("--test-name=")) == 0)
        {
          testName = arg + strlen("--test-name=");
        }
      else if (strncmp(arg, "--suite=", strlen("--suite=")) == 0)
        {
          testName = arg + strlen("--suite=");
        }
      else if (strncmp(arg, "--tempdir=", strlen("--tempdir=")) == 0)
        {
          m_tempDir = arg + strlen("--tempdir=");
        }
      else if (strncmp(arg, "--out=", strlen("--out=")) == 0)
        {
          out = arg + strlen("--out=");
        }
      else if (strncmp(arg, "--fullness=", strlen("--fullness=")) == 0)
        {
          fullness = arg + strlen("--fullness=");

          // Set the maximum test length allowed.
          if (fullness == "EXTENSIVE")
            {
              maximumTestDuration = TestCase::EXTENSIVE;
            }
          else if (fullness == "TAKES_FOREVER")
            {
              maximumTestDuration = TestCase::TAKES_FOREVER;
            }
          else
            {
              maximumTestDuration = TestCase::QUICK;
            }
        }
      else
        {
          // un-recognized command-line argument
          PrintHelp (progname);
          return 0;
        }
      argv++;
    }
  enum TestSuite::Type testType;
  if (testTypeString == "")
    {
      testType = TestSuite::ALL;
    }
  else if (testTypeString == "bvt")
    {
      testType = TestSuite::BVT;
    }
  else if (testTypeString == "core")
    {
      testType = TestSuite::ALL;
    }
  else if (testTypeString == "example")
    {
      testType = TestSuite::EXAMPLE;
    }
  else if (testTypeString == "unit")
    {
      testType = TestSuite::UNIT;
    }
  else if (testTypeString == "system")
    {
      testType = TestSuite::SYSTEM;
    }
  else if (testTypeString == "performance")
    {
      testType = TestSuite::PERFORMANCE;
    }
  else
    {
      std::cout << "Invalid test type specified: " << testTypeString << std::endl;
      PrintTestTypeList ();
      return 1;
    }

  std::list<TestCase *> tests = FilterTests (testName, testType, maximumTestDuration);

  if (m_tempDir == "")
    {
      m_tempDir = SystemPath::MakeTemporaryDirectoryName ();
    }
  if (printTempDir)
    {
      std::cout << m_tempDir << std::endl;
    }
  if (printTestNameList)
    {
      PrintTestNameList (tests.begin (), tests.end (), printTestTypeAndName);
      return 0;
    }
  if (printTestTypeList)
    {
      PrintTestTypeList ();
      return 0;
    }
  

  std::ostream *os;
  if (out != "")
    {
      std::ofstream *ofs;
      ofs = new std::ofstream();
      std::ios_base::openmode mode = std::ios_base::out;
      if (append)
        {
          mode |= std::ios_base::app;
        }
      else
        {
          mode |= std::ios_base::trunc;
        }
      ofs->open (out.c_str (), mode);
      os = ofs;
    }
  else
    {
      os = &std::cout;
    }

  // let's run our tests now.
  bool failed = false;
  for (std::list<TestCase *>::const_iterator i = tests.begin (); i != tests.end (); ++i)
    {
      TestCase *test = *i;

      test->Run (this);
      PrintReport (test, os, xml, 0);
      if (test->IsFailed ())
        {
          failed = true;
          if (!m_continueOnFailure)
            {
              return 1;
            }
        }
    }

  if (out != "")
    {
      delete os;
    }

  return failed?1:0;
}

int 
TestRunner::Run (int argc, char *argv[])
{
  NS_LOG_FUNCTION (argc << argv);
  return TestRunnerImpl::Instance ()->Run (argc, argv);
}

} // namespace ns3
