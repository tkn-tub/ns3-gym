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

#include "ns3/test.h"
#include "ns3/assert.h"
#include "ns3/abort.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

extern bool gBreakOnFailure;

using namespace ns3;

//
// Create a temporary directory for use by test programs.  This is not a 
// foolproof thing, but a reasonably good way to get a throwaway directory
// while running tests in a debugger.
//  
std::string
TempDir (void)
{
  char *path = NULL;

  path = getenv ("TMP");
  if (path == NULL)
    {
      path = getenv ("TEMP");
      if (path == NULL)
        {
          path = const_cast<char *> ("/tmp");
        }
    }

  //
  // Just in case the user wants to go back and find the output, we give
  // a hint as to which dir we created by including a time hint.
  //
  time_t now = time (NULL);
  struct tm *tm_now = localtime (&now);
  
  //
  // But we also randomize the name in case there are multiple users doing
  // this at the same time
  //
  srand (time (0));
  long int n = rand ();

  //
  // The final path to the directory is going to look something like
  // 
  //   /tmp/ns3-14.30.29.32767
  //
  // The first segment comes from one of the temporary directory env 
  // variables or /tmp if not found.  The directory name starts with an
  // identifier telling folks who is making all of the temp directories
  // and then the local time (in this case 14.30.29 -- which is 2:30 and
  // 29 seconds PM).
  //
  char dirname[1024];
  snprintf (dirname, sizeof(dirname),  "%s/ns-3.%d.%d.%d.%ld", path, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, n);

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__)
  if(mkdir(dirname) == 0)
#else
  if (mkdir (dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
#endif
    {
      return dirname;
    } 
  else
    {
      return "";
    }
}

//
// Test suites may need to figure out where their source directory is in order
// to find test vectors.  To do that they will need to know  where the base 
// directory of the distribution is (the directory in which "src" is found).
// It is painful for a user debugging test suites to always provide that dir
// so we try and find it in the current directory tree.
//
std::string
BaseDir (void)
{
  //
  // Get an absolute path to the current working directory.  Following code
  // depends on the leading '/'
  //
  char pathbuf[PATH_MAX];
  if (getcwd (pathbuf, sizeof(pathbuf)) == NULL)
    {
      NS_ABORT_MSG ("Basedir():  unable to getcwd()");
    }

  //
  // Walk up the directory tree looking for a directory that has files that
  // indicate it is the base of an ns-3 distribution.  We use VERSION and
  // LICENSE which have been there from the beginning of time.
  //
  for (;;)
    {
      bool haveVersion = false;
      bool haveLicense = false;

      //
      // Open the directory file for the current directory and loop through
      // the directory entries.
      //
      DIR *dp = opendir (pathbuf);
      if (dp != NULL)
        {
          while (struct dirent *de = readdir (dp))
            {
              if (strcmp (de->d_name, "VERSION") == 0)
                {
                  haveVersion = true;
                }

              if (strcmp (de->d_name, "LICENSE") == 0)
                {
                  haveLicense = true;
                }
            }
        }
      closedir (dp);

      //
      // If there's a file named VERSION and a file named LICENSE in this
      // directory, we assume it's our base directory.
      //
      if (haveVersion && haveLicense)
        {
          return pathbuf;
        }

      //
      // Strip off the last segment of the current directory.
      //
      char *last = strrchr (pathbuf, '/');
      NS_ASSERT_MSG (last, "No \"/\" found in absolute path ???");
      *last = '\0';

      if (strlen(pathbuf) == 0)
        {
          return "";
        }
    }

  //
  // Quiet the compiler.
  //
  return "";
}

//
// Run one of the test suites.  Returns an integer with the boolean sense of
// "an error has occurred."  That is, 0 == false -> no error; 1 == true -> an
// error occurred.
//
int 
main (int argc, char *argv[])
{
  bool doVerbose = false;
  bool doList = false;
  bool doMultiple = false;
  bool doHelp = false;
  bool doSuite = false;
  bool doKinds = false;

  gBreakOnFailure = false;

  bool haveBasedir = false;
  bool haveTempdir = false;
  bool haveType = false;

  std::string suiteName;
  std::string basedir;
  std::string tempdir;
  std::string outfileName;
  std::string typeName;


  for (int i = 1; i < argc; ++i)
    {
      std::string arg(argv[i]);

      if (arg.find ("--assert") != std::string::npos)
        {
          gBreakOnFailure = true;
        }

      if (arg.find ("--basedir=") != std::string::npos)
        {
          basedir = arg.substr (arg.find_first_of ("=") + 1, 9999);
          haveBasedir = true;
        }

      if (arg.find ("--constrain=") != std::string::npos)
        {
          typeName = arg.substr (arg.find_first_of ("=") + 1, 9999);
          haveType = true;
        }

      if (arg.compare ("--help") == 0)
        {
          doHelp = true;
        }

      if (arg.compare ("--kinds") == 0)
        {
          doKinds = true;
        }

      if (arg.compare ("--list") == 0)
        {
          doList = true;
        }

      if (arg.compare ("--multiple") == 0)
        {
          doMultiple = true;
        }

      if (arg.find ("--out=") != std::string::npos)
        {
          outfileName = arg.substr (arg.find_first_of ("=") + 1, 9999);
        }

      if (arg.find ("--suite=") != std::string::npos)
        {
          suiteName = arg.substr (arg.find_first_of ("=") + 1, 9999);
          doSuite = true;
        }

      if (arg.find ("--tempdir=") != std::string::npos)
        {
          tempdir = arg.substr (arg.find_first_of ("=") + 1, 9999);
          haveTempdir = true;
        }

      if (arg.compare ("--verbose") == 0)
        {
          doVerbose = true;
        }
    }

  //
  // A help request trumps everything else.  If we have one, just print the help
  // and leave.
  //
  if (doHelp)
    {
      std::cout << "  --assert:               Tell tests to segfault (like assert) if an error is detected" << std::endl;
      std::cout << "  --basedir=dir:          Set the base directory (where to find src) to \"dir\"" << std::endl;
      std::cout << "  --tempdir=dir:          Set the temporary directory (where to find data files) to \"dir\"" << std::endl;
      std::cout << "  --constrain=test-type:  Constrain checks to test suites of type \"test-type\"" << std::endl;
      std::cout << "  --help:                 Print this message" << std::endl;
      std::cout << "  --kinds:                List all of the available kinds of tests" << std::endl;
      std::cout << "  --list:                 List all of the test suites (optionally constrained by test-type)" << std::endl;
      std::cout << "  --multiple:             Allow test suites and cases to produce multiple failures" << std::endl;
      std::cout << "  --out=file-name:        Set the test status output file to \"file-name\"" << std::endl;
      std::cout << "  --suite=suite-name:     Run the test suite named \"suite-name\"" << std::endl;
      std::cout << "  --verbose:              Turn on messages in the run test suites" << std::endl;

      return false;
    }

  //
  // A kinds request trumps everything remaining.  If we are asked, just 
  // print the list of types and leave.
  //
  if (doKinds)
    {
      //
      // Coming up with a string to represent a test type is completely up to
      // us here.  We just define the types as being a string composed of the
      // enum defined in test.h converted to lower case.
      //
      std::cout << "  bvt:         Build Verification Tests (to see if build completed successfully)" << std::endl;
      std::cout << "  core:        Run all TestSuite-based tests (exclude examples)" << std::endl;
      std::cout << "  example:     Examples (to see if example programs run successfully)" << std::endl;
      std::cout << "  performance: Performance Tests (check to see if the system is as fast as expected)" << std::endl;
      std::cout << "  system:      System Tests (spans modules to check integration of modules)" << std::endl;
      std::cout << "  unit:        Unit Tests (within modules to check basic functionality)" << std::endl;

      return false;
    }

  //
  // A list request is the first functional request.  It trumps running the
  // actual tests.  If we get a list request, we don't run anything, we just
  // do the requested list which may or may not be qualified by a typename.
  //
  if (doList)
    {
      for (uint32_t i = 0; i < TestRunner::GetNTestSuites (); ++i)
        {
          TestSuite *suite = TestRunner::GetTestSuite (i);

          //
          // Filter the tests listed by type if requested.  The special typeName 
          // "core" means any TestSuite.
          //
          if (haveType && typeName != "core")
            {
              TestSuite::TestType type = suite->GetTestType ();
              if (typeName == "bvt" && type != TestSuite::BVT)
                {
                  continue;
                }

              if (typeName == "unit" && type != TestSuite::UNIT)
                {
                  continue;
                }

              if (typeName == "system" && type != TestSuite::SYSTEM)
                {
                  continue;
                }

              if (typeName == "example" && type != TestSuite::EXAMPLE)
                {
                  continue;
                }

              if (typeName == "performance" && type != TestSuite::PERFORMANCE)
                {
                  continue;
                }
            }

          //
          // This creates a list of test suite names that can be used by the
          // high level test manager to get a list of all tests.  It will then
          // typically launch individual tests in parallel, calling back here
          // with a specific "suite=" to run.
          //
          std::cout << suite->GetName () << std::endl;
        }

      return false;
    }

  //
  // We have a lot of options possible to provide flexibility.  It can become
  // painful, however, to provide all of the options when debugging, and it 
  // turns out that not all tests require all options.  It is really helpful
  // to try and put together some reasonable defaults if we're not provided
  // them.
  //
  if (!haveTempdir)
    {
      //
      // No temporary directory was provided.  We don't know if the selected
      // test or tests will need one, but we can cook something up.  The 
      // tmpnam function has its own set of problems, so we'll just do our 
      // own thing.
      //
      tempdir = TempDir ();
      if (tempdir.size ()) 
        {
          std::cout << "Temporary directory not provided.  Using \"" << tempdir << "\"" << std::endl;
          haveTempdir = true;
        }
      else
        {
          std::cout << "Temporary directory not provided and unable to create one." << std::endl;
          return true;
        }
    }

  if (haveBasedir == false)
    {
      //
      // No basedir was provided.  If we don't have it, we can try and find it 
      // in the current directory tree.
      //
      basedir = BaseDir ();
      if (basedir.size ()) 
        {
          std::cout << "Base directory not provided.  Using \"" << basedir << "\"" << std::endl;
          haveBasedir = true;
        }
      else
        {
          std::cout << "Base directory not provided and unable to find one." << std::endl;
          return true;
        }
    }

  //
  // If given an output file, we just append the output of each test suite 
  // we're asked to run to the end of that file.  We need to append since the
  // higher level test runner may be just running a number of tests back to 
  // back.  We leave it up to that code to decide how to deal with possible
  // parallel operation -- we just append to a file here.  If no output file
  // is specified, we don't do any output and just return the sense of error
  // given by the test.
  //
  std::ofstream *pofs = 0;
  std::ofstream ofs;

  if (!outfileName.empty ())
    {
      ofs.open (outfileName.c_str (), std::fstream::out | std::fstream::app);
      pofs = &ofs;
    }

  //
  // If we have a specified test suite to run, then we only run that suite.
  // The default case is to "run everything.  We don't expect this to be done
  // much since typically higher level code will be running suites in parallel
  // but we'll do it if asked.
  //
  bool result = false;
  bool suiteRan = false;

  for (uint32_t i = 0; i < TestRunner::GetNTestSuites (); ++i)
    {
      TestSuite *testSuite = TestRunner::GetTestSuite (i);
      if (doSuite == false || (doSuite == true && suiteName == testSuite->GetName ()))
        {
          testSuite->SetBaseDir (basedir);
          testSuite->SetTempDir (tempdir);
          testSuite->SetStream (pofs);
          testSuite->SetVerbose (doVerbose);
          testSuite->SetContinueOnFailure (doMultiple);
          result |= testSuite->Run ();
          suiteRan = true;
        }
    }

  ofs.close();

  //
  // If we couldn't figure out how to run at least one test, then return an error
  //
  if (suiteRan == false)
    {
      std::cout << "Unable to find a test to run (constraints too severe or test not found)" << std::endl;
      return true;
    }

  return result;
}
