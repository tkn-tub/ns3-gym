/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/command-line.h"
#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/global-value.h"
#include "ns3/type-id.h"
#include "ns3/test.h"
#include "ns3/string.h"
#include <cstdlib>
#include <cstdarg>

using namespace ns3;

/*************************************************************************//**
 * A test base class that drives Command Line parsing
 ****************************************************************************/
class CommandLineTestCaseBase : public TestCase
{
public:
  /**
   * Constructor
   *
   * \param description purpose of this TestCase
   */
  CommandLineTestCaseBase (std::string description);
  /** Destructor */
  virtual ~CommandLineTestCaseBase () {}

  /**
   * Excercise the CommandLine with the provided arguments
   *
   * \param cmd the configured CommandLine
   * \param n the number of arguments
   */
  void Parse (CommandLine &cmd, int n, ...);
};

CommandLineTestCaseBase::CommandLineTestCaseBase (std::string description)
  : TestCase (description)
{
}

void
CommandLineTestCaseBase::Parse (CommandLine &cmd, int n, ...)
{
  char **args = new char* [n+1];
  args[0] = (char *) "Test";
  va_list ap;
  va_start (ap, n);
  int i = 0;
  while (i < n)
    {
      char *arg = va_arg (ap, char *);
      args[i+1] = arg;
      i++;
    }
  va_end (ap);
  int argc = n + 1;
  cmd.Parse (argc, args);
  delete [] args;
}

/*************************************************************************//**
 * Test boolean Command Line processing
 ****************************************************************************/
class CommandLineBooleanTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineBooleanTestCase ();             /**< Constructor */
  virtual ~CommandLineBooleanTestCase () {}  /**< Destructor */

private:
  virtual void DoRun (void);                 /**< Run the test */

};

CommandLineBooleanTestCase::CommandLineBooleanTestCase ()
  : CommandLineTestCaseBase ("Check boolean arguments")
{
}

void
CommandLineBooleanTestCase::DoRun (void)
{
  CommandLine cmd;
  bool myBool = true;
  bool myDefaultFalseBool = false;

  cmd.AddValue ("my-bool", "help", myBool);
  cmd.AddValue ("my-false-bool", "help", myDefaultFalseBool);

  Parse (cmd, 1, "--my-bool=0");
  NS_TEST_ASSERT_MSG_EQ (myBool, false, "Command parser did not correctly set a boolean value to false");

  Parse (cmd, 1, "--my-bool=1");
  NS_TEST_ASSERT_MSG_EQ (myBool, true, "Command parser did not correctly set a boolean value to true, given integer argument");

  Parse (cmd, 1, "--my-bool");
  NS_TEST_ASSERT_MSG_EQ (myBool, false, "Command parser did not correctly toggle a default true boolean value to false, given no argument");

  Parse (cmd, 1, "--my-false-bool");
  NS_TEST_ASSERT_MSG_EQ (myDefaultFalseBool, true, "Command parser did not correctly toggle a default false boolean value to true, given no argument");

  Parse (cmd, 1, "--my-bool=t");
  NS_TEST_ASSERT_MSG_EQ (myBool, true, "Command parser did not correctly set a boolean value to true, given 't' argument");

  Parse (cmd, 1, "--my-bool=true");
  NS_TEST_ASSERT_MSG_EQ (myBool, true, "Command parser did not correctly set a boolean value to true, given \"true\" argument");
}

/*************************************************************************//**
 * Test int Command Line processing
 ****************************************************************************/
class CommandLineIntTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineIntTestCase ();                 /**< Constructor */ 
  virtual ~CommandLineIntTestCase () {}      /**< Destructor */  
                                                                 
private:                                                         
  virtual void DoRun (void);                 /**< Run the test */

};

CommandLineIntTestCase::CommandLineIntTestCase ()
  : CommandLineTestCaseBase ("Check int arguments")
{
}

void
CommandLineIntTestCase::DoRun (void)
{
  CommandLine cmd;
  bool myBool = true;
  int32_t myInt32 = 10;

  cmd.AddValue ("my-bool", "help", myBool);
  cmd.AddValue ("my-int32", "help", myInt32);

  Parse (cmd, 2, "--my-bool=0", "--my-int32=-3");
  NS_TEST_ASSERT_MSG_EQ (myBool, false, "Command parser did not correctly set a boolean value to false");
  NS_TEST_ASSERT_MSG_EQ (myInt32, -3, "Command parser did not correctly set an integer value to -3");

  Parse (cmd, 2, "--my-bool=1", "--my-int32=+2");
  NS_TEST_ASSERT_MSG_EQ (myBool, true, "Command parser did not correctly set a boolean value to true");
  NS_TEST_ASSERT_MSG_EQ (myInt32, +2, "Command parser did not correctly set an integer value to +2");
}

/*************************************************************************//**
 * Test unsigned int Command Line processing
 ****************************************************************************/
class CommandLineUnsignedIntTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineUnsignedIntTestCase ();              /**< Constructor */ 
  virtual ~CommandLineUnsignedIntTestCase () {}   /**< Destructor */  
                                                                      
private:                                                              
  virtual void DoRun (void);                      /**< Run the test */

};

CommandLineUnsignedIntTestCase::CommandLineUnsignedIntTestCase ()
  : CommandLineTestCaseBase ("Check unsigned int arguments")
{
}

void
CommandLineUnsignedIntTestCase::DoRun (void)
{
  CommandLine cmd;
  bool myBool = true;
  uint32_t myUint32 = 10;

  cmd.AddValue ("my-bool", "help", myBool);
  cmd.AddValue ("my-uint32", "help", myUint32);

  Parse (cmd, 2, "--my-bool=0", "--my-uint32=9");

  NS_TEST_ASSERT_MSG_EQ (myBool, false, "Command parser did not correctly set a boolean value to true");
  NS_TEST_ASSERT_MSG_EQ (myUint32, 9, "Command parser did not correctly set an unsigned integer value to 9");
}

/*************************************************************************//**
 * Test string Command Line processing
 ****************************************************************************/
class CommandLineStringTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineStringTestCase ();              /**< Constructor */ 
  virtual ~CommandLineStringTestCase () {}   /**< Destructor */  
                                                                 
private:                                                         
  virtual void DoRun (void);                 /**< Run the test */

};

CommandLineStringTestCase::CommandLineStringTestCase ()
  : CommandLineTestCaseBase ("Check unsigned int arguments")
{
}

void
CommandLineStringTestCase::DoRun (void)
{
  CommandLine cmd;
  uint32_t myUint32 = 10;
  std::string myStr = "MyStr";

  cmd.AddValue ("my-uint32", "help", myUint32);
  cmd.AddValue ("my-str", "help", myStr);

  Parse (cmd, 2, "--my-uint32=9", "--my-str=XX");

  NS_TEST_ASSERT_MSG_EQ (myUint32, 9, "Command parser did not correctly set an unsigned integer value to 9");
  NS_TEST_ASSERT_MSG_EQ (myStr, "XX", "Command parser did not correctly set an string value to \"XX\"");
}

/*************************************************************************//**
 * The Test Suite that glues all of the Test Cases together.
 ****************************************************************************/
class CommandLineTestSuite : public TestSuite
{
public:
  CommandLineTestSuite ();                   /**< Constructor */
};

CommandLineTestSuite::CommandLineTestSuite ()
  : TestSuite ("command-line", UNIT)
{
  AddTestCase (new CommandLineBooleanTestCase, TestCase::QUICK);
  AddTestCase (new CommandLineIntTestCase, TestCase::QUICK);
  AddTestCase (new CommandLineUnsignedIntTestCase, TestCase::QUICK);
  AddTestCase (new CommandLineStringTestCase, TestCase::QUICK);
}

static CommandLineTestSuite CommandLineTestSuite; /**< Test instance */
