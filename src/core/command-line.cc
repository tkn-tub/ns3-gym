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
#include "command-line.h"
#include "log.h"
#include "config.h"
#include "global-value.h"
#include "type-id.h"
#include "test.h"
#include "string.h"
#include <stdlib.h>
#include <stdarg.h>

NS_LOG_COMPONENT_DEFINE ("CommandLine");

namespace ns3 {

CommandLine::CommandLine ()
{}
CommandLine::CommandLine (const CommandLine &cmd)
{
  Copy (cmd);
}
CommandLine &
CommandLine::operator = (const CommandLine &cmd)
{
  Clear ();
  Copy (cmd);
  return *this;
}
CommandLine::~CommandLine ()
{
  Clear ();
}
void
CommandLine::Copy (const CommandLine &cmd)
{
  for (Items::const_iterator i = cmd.m_items.begin (); 
       i != cmd.m_items.end (); ++i)
    {
      m_items.push_back (*i);
    }
}
void
CommandLine::Clear (void)
{
  for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
    {
      delete *i;
    }
  m_items.clear ();
}

CommandLine::Item::~Item ()
{}

void 
CommandLine::Parse (int iargc, char *argv[]) const
{
  int argc = iargc;
  for (argc--, argv++; argc > 0; argc--, argv++)
    {
      // remove "--" or "-" heading.
      std::string param = *argv;
      std::string::size_type cur = param.find ("--");
      if (cur == 0)
	{
	  param = param.substr (2, param.size () - 2);
	}
      else
	{
	  cur = param.find ("-");
	  if (cur == 0)
	    {
	      param = param.substr (1, param.size () - 1);
	    }
	  else
	    {
	      // invalid argument. ignore.
	      continue;
	    }
	}
      cur = param.find ("=");
      std::string name, value;
      if (cur == std::string::npos)
        {
	  name = param;
	  value = "";
        }
      else
        {
          name = param.substr (0, cur);
          value = param.substr (cur + 1, param.size () - (cur+1));
        }
      HandleArgument (name, value);
    }
}

void
CommandLine::PrintHelp (void) const
{
  std::cout << "--PrintHelp: Print this help message." << std::endl;
  std::cout << "--PrintGroups: Print the list of groups." << std::endl;
  std::cout << "--PrintTypeIds: Print all TypeIds." << std::endl;  
  std::cout << "--PrintGroup=[group]: Print all TypeIds of group." << std::endl;
  std::cout << "--PrintAttributes=[typeid]: Print all attributes of typeid." << std::endl;
  std::cout << "--PrintGlobals: Print the list of globals." << std::endl;
  if (!m_items.empty ())
    {
      std::cout << "User Arguments:" << std::endl;
      for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
	{
	  std::cout << "    --" << (*i)->m_name << ": " << (*i)->m_help << std::endl;
	}
    }
}

void
CommandLine::PrintGlobals (void) const
{
  for (GlobalValue::Iterator i = GlobalValue::Begin (); i != GlobalValue::End (); ++i)
    {
      std::cout << "    --" << (*i)->GetName () << "=[";
      Ptr<const AttributeChecker> checker = (*i)->GetChecker ();
      StringValue v;
      (*i)->GetValue (v);
      std::cout << v.Get () << "]:  "
		<< (*i)->GetHelp () << std::endl;      
    }
}

void
CommandLine::PrintAttributes (std::string type) const
{
  TypeId tid;
  if (!TypeId::LookupByNameFailSafe (type, &tid))
    {
      NS_FATAL_ERROR ("Unknown type="<<type<<" in --PrintAttributes");
    }
  for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
    {
      std::cout << "    --"<<tid.GetAttributeFullName (i)<<"=[";
      Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (i);
      Ptr<const AttributeValue> initial = tid.GetAttributeInitialValue (i);
      std::cout << initial->SerializeToString (checker) << "]:  "
		<< tid.GetAttributeHelp (i) << std::endl;
    }
}


void
CommandLine::PrintGroup (std::string group) const
{
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetGroupName () == group)
	{
	  std::cout << "    --PrintAttributes=" <<tid.GetName ()<<std::endl;
	}
    }
}

void
CommandLine::PrintTypeIds (void) const
{
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      std::cout << "    --PrintAttributes=" <<tid.GetName ()<<std::endl;
    }
}

void
CommandLine::PrintGroups (void) const
{
  std::list<std::string> groups;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      std::string group = tid.GetGroupName ();
      if (group == "")
	{
	  continue;
	}
      bool found = false;
      for (std::list<std::string>::const_iterator j = groups.begin (); j != groups.end (); ++j)
	{
	  if (*j == group)
	    {
	      found = true;
	      break;
	    }
	}
      if (!found)
	{
	  groups.push_back (group);
	}
    }
  for (std::list<std::string>::const_iterator k = groups.begin (); k != groups.end (); ++k)
    {
      std::cout << "    --PrintGroup="<<*k<<std::endl;
    }
}

void
CommandLine::HandleArgument (std::string name, std::string value) const
{
  NS_LOG_DEBUG ("Handle arg name="<<name<<" value="<<value);
  if (name == "PrintHelp")
    {
      // method below never returns.
      PrintHelp ();
      exit (0);
    } 
  else if (name == "PrintGroups")
    {
      // method below never returns.
      PrintGroups ();
      exit (0);
    }
  else if (name == "PrintTypeIds")
    {
      // method below never returns.
      PrintTypeIds ();
      exit (0);
    }
  else if (name == "PrintGlobals")
    {
      // method below never returns.
      PrintGlobals ();
      exit (0);
    }
  else if (name == "PrintGroup")
    {
      // method below never returns.
      PrintGroup (value);
      exit (0);
    }
  else if (name == "PrintAttributes")
    {
      // method below never returns.
      PrintAttributes (value);
      exit (0);
    }
  else
    {
      for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
        {
          if ((*i)->m_name == name)
            {
              if (!(*i)->Parse (value))
                {
                  std::cerr << "Invalid argument value: "<<name<<"="<<value << std::endl;
                  exit (1);
                }
              else
                {
                  return;
                }
            }
        }
    }
  if (!Config::SetGlobalFailSafe (name, StringValue (value))
      && !Config::SetDefaultFailSafe (name, StringValue (value)))
    {
      std::cerr << "Invalid command-line arguments: --"<<name<<"="<<value<<std::endl;
      PrintHelp ();
      exit (1);
    }
}

bool
CommandLine::CallbackItem::Parse (std::string value)
{
  NS_LOG_DEBUG ("CommandLine::CallbackItem::Parse \"" << value << "\"");
  return m_callback (value);
}

void
CommandLine::AddValue (const std::string &name,
                       const std::string &help,
                       Callback<bool, std::string> callback)
{
  NS_LOG_FUNCTION (this << name << help << "callback");
  CallbackItem *item = new CallbackItem ();
  item->m_name = name;
  item->m_help = help;
  item->m_callback = callback;
  m_items.push_back (item);
}

// ===========================================================================
// A test base class that drives Command Line parsing
// ===========================================================================
class CommandLineTestCaseBase : public TestCase
{
public:
  CommandLineTestCaseBase (std::string description);
  virtual ~CommandLineTestCaseBase () {}

  void Parse (const CommandLine &cmd, int n, ...);
};

CommandLineTestCaseBase::CommandLineTestCaseBase (std::string description)
  : TestCase (description)
{
}

void
CommandLineTestCaseBase::Parse (const CommandLine &cmd, int n, ...)
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
  int argc = n + 1;
  cmd.Parse (argc, args);
  delete [] args;
}

// ===========================================================================
// Test boolean Command Line processing
// ===========================================================================
class CommandLineBooleanTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineBooleanTestCase ();
  virtual ~CommandLineBooleanTestCase () {}

private:
  virtual bool DoRun (void);

};

CommandLineBooleanTestCase::CommandLineBooleanTestCase ()
  : CommandLineTestCaseBase ("Check boolean arguments")
{
}

bool
CommandLineBooleanTestCase::DoRun (void)
{
  CommandLine cmd;
  bool myBool = true;

  cmd.AddValue ("my-bool", "help", myBool);

  Parse (cmd, 1, "--my-bool=0");
  NS_TEST_ASSERT_MSG_EQ (myBool, false, "Command parser did not correctly set a boolean value to false");

  Parse (cmd, 1, "--my-bool=1");
  NS_TEST_ASSERT_MSG_EQ (myBool, true, "Command parser did not correctly set a boolean value to true");

  return GetErrorStatus ();
}

// ===========================================================================
// Test int Command Line processing
// ===========================================================================
class CommandLineIntTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineIntTestCase ();
  virtual ~CommandLineIntTestCase () {}

private:
  virtual bool DoRun (void);

};

CommandLineIntTestCase::CommandLineIntTestCase ()
  : CommandLineTestCaseBase ("Check int arguments")
{
}

bool
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

  return GetErrorStatus ();
}

// ===========================================================================
// Test unsigned int Command Line processing
// ===========================================================================
class CommandLineUnsignedIntTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineUnsignedIntTestCase ();
  virtual ~CommandLineUnsignedIntTestCase () {}

private:
  virtual bool DoRun (void);

};

CommandLineUnsignedIntTestCase::CommandLineUnsignedIntTestCase ()
  : CommandLineTestCaseBase ("Check unsigned int arguments")
{
}

bool
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

  return GetErrorStatus ();
}

// ===========================================================================
// Test string Command Line processing
// ===========================================================================
class CommandLineStringTestCase : public CommandLineTestCaseBase
{
public:
  CommandLineStringTestCase ();
  virtual ~CommandLineStringTestCase () {}

private:
  virtual bool DoRun (void);

};

CommandLineStringTestCase::CommandLineStringTestCase ()
  : CommandLineTestCaseBase ("Check unsigned int arguments")
{
}

bool
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

  return GetErrorStatus ();
}

// ===========================================================================
// The Test Suite that glues all of the Test Cases together.
// ===========================================================================
class CommandLineTestSuite : public TestSuite
{
public:
  CommandLineTestSuite ();
};

CommandLineTestSuite::CommandLineTestSuite ()
  : TestSuite ("command-line", BVT)
{
  AddTestCase (new CommandLineBooleanTestCase);
  AddTestCase (new CommandLineIntTestCase);
  AddTestCase (new CommandLineUnsignedIntTestCase);
  AddTestCase (new CommandLineStringTestCase);
}

CommandLineTestSuite CommandLineTestSuite;

} // namespace ns3
