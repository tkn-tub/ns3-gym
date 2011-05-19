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
#include <math.h>

// Set to true to enable a segmentation fault upon a test case macro test 
// failing (for debugging purposes)
bool gBreakOnFailure = false;

namespace ns3 {

//
// XML files have restrictions on certain characters that may be present in
// data.  We need to replace these characters with their alternate 
// representation on the way into the XML file.
//
std::string
ReplaceXmlSpecialCharacters (std::string xml)
{
  std::string specials = "<>&\"'";
  std::string replacements[] = { "&lt;", "&gt;", "&amp;", "&#39;", "&quot;"};
  std::string result;
  std::size_t index, length = xml.length ();

  for (size_t i = 0; i < length; ++i)
    {
      char character = xml[i];

      if ((index = specials.find (character)) == std::string::npos)
        {
          result.push_back (character);
        }
      else
        {
          result += replacements[index];
        }
    }
  return result;
}

bool
TestDoubleIsEqual (const double x1, const double x2, const double epsilon)
{
  int exponent;
  double delta, difference;

  //
  // Find exponent of largest absolute value
  //
  {
    double max = (fabs (x1) > fabs (x2)) ? x1 : x2;
    (void)frexp (max, &exponent);
  }

  //
  // Form a neighborhood of size  2 * delta
  //
  delta = ldexp (epsilon, exponent);
  difference = x1 - x2;

  if (difference > delta || difference < -delta)
    {
      return false;
    }
  return true;
} 


TestCase::TestCase (std::string name)
  : m_name (name), 
    m_verbose (false), 
    m_continueOnFailure (false), 
    m_detailsReported (false), 
    m_basedir ("invalid"), 
    m_tempdir ("invalid"), 
    m_ofs (0), 
    m_error (false)
{
}

TestCase::~TestCase ()
{
}

void
TestCase::ReportStart  (void)
{
  DoReportStart ();
}

void
TestCase::ReportCaseSuccess  (void)
{
  UpdateErrorStatus (false);
  DoReportCaseSuccess ();
}

void
TestCase::ReportCaseFailure  (void)
{
  UpdateErrorStatus (true);
  DoReportCaseFailure ();
}

void
TestCase::ReportTestFailure  (
  std::string cond, 
  std::string actual, 
  std::string limit, 
  std::string message, 
  std::string file, 
  int32_t line)
{
  UpdateErrorStatus (true);
  DoReportTestFailure (cond, actual, limit, message, file, line);
  m_detailsReported = true; 
}

void
TestCase::ReportEnd  (void)
{
  DoReportStart ();
}

void
TestCase::Run (void)
{
  //
  // We set up a flag to make sure the user plays by the rules and actually
  // does something to report the details of an error.
  //
  m_detailsReported = false; 

  DoReportStart ();
  DoSetup ();

  DoRun ();

  DoTeardown ();

  if (GetErrorStatus () == false)
    {
      DoReportCaseSuccess ();
    }
  else
    {
      //
      // It is a programming error to return an error from a test case without 
      // calling ReportTestFailure.  Typically this is done automagically when
      // using the ASSERT or EXPECT macros.  If you don't use these, you must
      // ReportTestFailure on any errors yourself, which will set 
      // m_detailsReported and make us happy.
      //
      NS_ASSERT_MSG (m_detailsReported, "The details of a failing test was not reported");

      DoReportCaseFailure ();
    }

  DoReportEnd ();

  return;
}

void 
TestCase::SetVerbose (bool verbose)
{
  m_verbose = verbose;
}

void 
TestCase::SetContinueOnFailure (bool continueOnFailure)
{
  m_continueOnFailure = continueOnFailure;
}

void 
TestCase::SetName (std::string name)
{
  m_name = name;
}

std::string 
TestCase::GetName (void)
{
  return m_name;
}

void 
TestCase::SetBaseDir (std::string basedir)
{
  //
  // C and C++ allow one to use forward slashes even on systems where the 
  // separator is actually a backslash.
  //
  if (basedir[basedir.length () - 1] != '/')
    {
      m_basedir = basedir + "/";
    }
  else
    {
      m_basedir = basedir;
    }
}

std::string 
TestCase::GetBaseDir (void)
{
  return m_basedir;
}

void 
TestCase::SetTempDir (std::string tempdir)
{
  //
  // C and C++ allow one to use forward slashes even on systems where the 
  // separator is actually a backslash.
  //
  if (tempdir[tempdir.length () - 1] != '/')
    {
      m_tempdir = tempdir + "/";
    }
  else
    {
      m_tempdir = tempdir;
    }
}

std::string 
TestCase::GetTempDir (void)
{
  return m_tempdir;
}

std::string 
TestCase::GetSourceDir (std::string file)
{
  //
  // The <file> parameter is actually going to be __FILE__ which may have 
  // backslashes in it on win32 systems.  For example,
  //
  //   ..\src\common\pcap-file-test-suite.cc  (win32)
  //
  // or
  //
  //   ../src/common/pcap-file-test-suite.cc  (grown-up systems)
  //
#ifdef WIN32
  std::string::size_type relPathBegin = file.find_first_of ("\\");
  std::string::size_type relPathEnd = file.find_last_of ("\\");
#else
  std::string::size_type relPathBegin = file.find_first_of ("/");
  std::string::size_type relPathEnd = file.find_last_of ("/");
#endif

  NS_ABORT_MSG_IF (relPathBegin == std::string::npos, "TestCase::GetSourceDir(): Internal Error");
  NS_ABORT_MSG_IF (relPathEnd == std::string::npos, "TestCase::GetSourceDir(): Internal Error");

  return GetBaseDir () + file.substr (relPathBegin, relPathEnd + 1 - relPathBegin);
}

void 
TestCase::SetStream (std::ofstream *ofs)
{
  m_ofs = ofs;
}

std::ofstream *
TestCase::GetStream (void)
{
  return m_ofs;
}

void 
TestCase::UpdateErrorStatus (bool error)
{
  m_error |= error;
}

void 
TestCase::SetErrorStatus (bool error)
{
  m_error = error;
}

bool
TestCase::GetErrorStatus (void)
{
  return m_error;
}

bool
TestCase::ContinueOnFailure (void)
{
  return m_continueOnFailure;
}

void
TestCase::DoReportStart  (void)
{
  m_msClock.Start ();

  if (m_ofs == 0)
    {
      return;
    }
  *m_ofs << "  <TestCase>" << std::endl;
  *m_ofs << "    <CaseName>" << ReplaceXmlSpecialCharacters (GetName ()) << "</CaseName>" << std::endl;
}

void
TestCase::DoReportCaseSuccess  (void)
{
  if (m_ofs == 0)
    {
      return;
    }
  *m_ofs << "    <CaseResult>PASS</CaseResult>" << std::endl;
}

void
TestCase::DoReportCaseFailure  (void)
{
  if (m_ofs == 0)
    {
      return;
    }
  *m_ofs << "    <CaseResult>FAIL</CaseResult>" << std::endl;
}

void
TestCase::DoReportTestFailure  (
  std::string cond, 
  std::string actual, 
  std::string limit, 
  std::string message, 
  std::string file, 
  int32_t line)
{
  if (m_ofs == 0)
    {
      return;
    }

  *m_ofs << "    <FailureDetails>" << std::endl;
  *m_ofs << "      <Condition>" << ReplaceXmlSpecialCharacters (cond) << "</Condition>" << std::endl;
  *m_ofs << "      <Actual>" << ReplaceXmlSpecialCharacters (actual) << "</Actual>" << std::endl;
  *m_ofs << "      <Limit>" << ReplaceXmlSpecialCharacters (limit) << "</Limit>" << std::endl;
  *m_ofs << "      <Message>" << ReplaceXmlSpecialCharacters (message) << "</Message>" << std::endl;
  *m_ofs << "      <File>" << ReplaceXmlSpecialCharacters (file) << "</File>" << std::endl;
  *m_ofs << "      <Line>" << line << "</Line>" << std::endl;
  *m_ofs << "    </FailureDetails>" << std::endl;
}

void
TestCase::DoReportEnd  (void)
{
  m_msClock.End ();

  if (m_ofs == 0)
    {
      return;
    }

  (*m_ofs).precision (3);
  *m_ofs << std::fixed;

  const double MS_PER_SEC = 1000.;

  *m_ofs << "    <CaseTime>" << "real " << m_msClock.GetElapsedReal () / MS_PER_SEC
         << " user " << m_msClock.GetElapsedUser () / MS_PER_SEC
         << " system " << m_msClock.GetElapsedSystem () / MS_PER_SEC
         << "</CaseTime>" << std::endl;

  *m_ofs << "  </TestCase>" << std::endl;
}

void 
TestCase::DoSetup (void)
{
}

void 
TestCase::DoTeardown (void)
{
}

TestSuite::TestSuite (std::string name, TestType type)
  : m_name (name), 
    m_verbose (false), 
    m_basedir ("invalid"), 
    m_tempdir ("invalid"), 
    m_ofs (0), 
    m_error (false), 
    m_type (type)
{
  TestRunner::AddTestSuite (this);
}

TestSuite::~TestSuite ()
{
  for (TestCaseVector_t::iterator i = m_tests.begin (); i != m_tests.end (); ++i)
    {
      delete *i;
      *i = 0;
    }

  m_tests.erase (m_tests.begin (), m_tests.end ());
}

void
TestSuite::ReportStart (void)
{
  DoReportStart ();
}

void
TestSuite::ReportSuccess (void)
{
  UpdateErrorStatus (false);
  DoReportSuccess ();
}

void
TestSuite::ReportFailure (void)
{
  UpdateErrorStatus (true);
  DoReportFailure ();
}

void
TestSuite::ReportEnd (void)
{
  DoReportEnd ();
}

bool
TestSuite::Run (void)
{
  DoReportStart ();

  DoSetup (); 
  DoRun ();
  DoTeardown ();

  if (GetErrorStatus () == false)
    {
      DoReportSuccess ();
    }
  else
    {
      DoReportFailure ();
    }

  DoReportEnd ();

  return GetErrorStatus ();
}

uint32_t
TestSuite::AddTestCase (TestCase *testCase)
{
  uint32_t index = m_tests.size ();
  m_tests.push_back (testCase);
  return index;
}

uint32_t 
TestSuite::GetNTestCases (void)
{
  return m_tests.size ();
}

TestCase *
TestSuite::GetTestCase (uint32_t n)
{
  return m_tests[n];
}

TestSuite::TestType 
TestSuite::GetTestType (void)
{
  return m_type;
}

void 
TestSuite::SetVerbose (bool verbose)
{
  m_verbose = verbose;
}

void 
TestSuite::SetContinueOnFailure (bool continueOnFailure)
{
  m_continueOnFailure = continueOnFailure;
}

void 
TestSuite::SetName (std::string name)
{
  m_name = name;
}

std::string 
TestSuite::GetName (void)
{
  return m_name;
}

void 
TestSuite::SetBaseDir (std::string basedir)
{
  //
  // C and C++ allow one to use forward slashes even on systems where the 
  // separator is actually a backslash.
  //
  if (basedir[basedir.length () - 1] != '/')
    {
      m_basedir = basedir + "/";
    }
  else
    {
      m_basedir = basedir;
    }
}

std::string 
TestSuite::GetBaseDir (void)
{
  return m_basedir;
}

void 
TestSuite::SetTempDir (std::string tempdir)
{
  //
  // C and C++ allow one to use forward slashes even on systems where the 
  // separator is actually a backslash.
  //
  if (tempdir[tempdir.length () - 1] != '/')
    {
      m_tempdir = tempdir + "/";
    }
  else
    {
      m_tempdir = tempdir;
    }
}

std::string 
TestSuite::GetTempDir (void)
{
  return m_tempdir;
}

void 
TestSuite::SetStream (std::ofstream *ofs)
{
  m_ofs = ofs;
}

void 
TestSuite::UpdateErrorStatus (bool error)
{
  m_error |= error;
}

void 
TestSuite::SetErrorStatus (bool error)
{
  m_error = error;
}

bool
TestSuite::GetErrorStatus (void)
{
  return m_error;
}

bool
TestSuite::ContinueOnFailure (void)
{
  return m_continueOnFailure;
}

void
TestSuite::DoReportStart (void)
{
  m_msClock.Start ();

  if (m_ofs == 0)
    {
      return;
    }
  *m_ofs << "<TestSuite>" << std::endl;
  *m_ofs << "  <SuiteName>" << ReplaceXmlSpecialCharacters (GetName ()) << "</SuiteName>" << std::endl;
}

void
TestSuite::DoReportFailure (void)
{
  if (m_ofs == 0)
    {
      return;
    }
  *m_ofs << "  <SuiteResult>FAIL</SuiteResult>" << std::endl;
}

void
TestSuite::DoReportSuccess (void)
{
  if (m_ofs == 0)
    {
      return;
    }
  *m_ofs << "  <SuiteResult>PASS</SuiteResult>" << std::endl;
}

void
TestSuite::DoReportEnd (void)
{
  m_msClock.End ();

  if (m_ofs == 0)
    {
      return;
    }

  (*m_ofs).precision (3);
  *m_ofs << std::fixed;

  const double MS_PER_SEC = 1000.;

  *m_ofs << "  <SuiteTime>" << "real " << m_msClock.GetElapsedReal () / MS_PER_SEC
         << " user " << m_msClock.GetElapsedUser () / MS_PER_SEC
         << " system " << m_msClock.GetElapsedSystem () / MS_PER_SEC
         << "</SuiteTime>" << std::endl;

  *m_ofs << "</TestSuite>" << std::endl;
}

void 
TestSuite::DoSetup (void)
{
}

void
TestSuite::DoRun (void)
{
  SetErrorStatus (false);

  for (TestCaseVector_t::iterator i = m_tests.begin (); i != m_tests.end (); ++i)
    {
      //
      // Set the current options for the test case
      //
      (*i)->SetVerbose (m_verbose);
      (*i)->SetContinueOnFailure (m_continueOnFailure);
      (*i)->SetBaseDir (m_basedir);
      (*i)->SetTempDir (m_tempdir);
      (*i)->SetStream (m_ofs);

      //
      // Run the test case
      //
      (*i)->Run ();
      UpdateErrorStatus ((*i)->GetErrorStatus ());

      //
      // Exit if we have detected an error and we are not allowing multiple failures
      //
      if (GetErrorStatus () && m_continueOnFailure == false)
        {
          return;
        }
    }

}

void 
TestSuite::DoTeardown (void)
{
}

class TestRunnerImpl
{
public:
  uint32_t AddTestSuite (TestSuite *testSuite);
  uint32_t GetNTestSuites (void);
  TestSuite *GetTestSuite (uint32_t n);
  bool RunTestSuite (uint32_t n);

  static TestRunnerImpl *Instance (void);
private:
  TestRunnerImpl ();
  ~TestRunnerImpl ();

  typedef std::vector<TestSuite *> TestSuiteVector_t;
  TestSuiteVector_t m_suites;
};

TestRunnerImpl::TestRunnerImpl ()
{
}

TestRunnerImpl::~TestRunnerImpl ()
{
}

TestRunnerImpl *
TestRunnerImpl::Instance (void)
{
  static TestRunnerImpl runner;
  return &runner;
}

uint32_t
TestRunnerImpl::AddTestSuite (TestSuite *testSuite)
{
  uint32_t index = m_suites.size ();
  m_suites.push_back (testSuite);
  return index;
}

uint32_t 
TestRunnerImpl::GetNTestSuites (void)
{
  return m_suites.size ();
}

TestSuite *
TestRunnerImpl::GetTestSuite (uint32_t n)
{
  return m_suites[n];
}

uint32_t
TestRunner::AddTestSuite (TestSuite *testSuite)
{
  return TestRunnerImpl::Instance ()->AddTestSuite (testSuite);
}

uint32_t
TestRunner::GetNTestSuites (void)
{
  return TestRunnerImpl::Instance ()->GetNTestSuites ();
}

TestSuite *
TestRunner::GetTestSuite (uint32_t n)
{
  return TestRunnerImpl::Instance ()->GetTestSuite (n);
}

} // namespace ns3
