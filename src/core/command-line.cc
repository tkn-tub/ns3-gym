/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech University, INRIA
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
 * Authors: Raj Bhattacharjea <raj.b@gatech.edu>,
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "command-line.h"
#include <unistd.h>

namespace ns3 {

CommandLine::List::~List ()
{
  for (iterator iter = begin (); iter != end (); iter++)
    {
      delete *iter;
    }
}


CommandDefaultValue CommandLine::g_help ("help",
                                         "Print Help text for all commands",
                                         MakeCallback (&CommandLine::PrintHelp));

void 
CommandLine::AddArgCommand (const std::string &name,
                            const std::string &help,
                            Callback<void> cb)
{
  DefaultValueBase *base = 
    new CommandDefaultValue (name, help, cb);
  GetUserList ()->push_back (base);
}

CommandLine::List *
CommandLine::GetUserList (void)
{
  static List list;
  return &list;
}

void 
CommandLine::PrintHelp (void)
{
  for (List::iterator i = GetUserList ()->begin ();
       i != GetUserList ()->end (); i++)
    {
      DefaultValueBase *item = *i;
      if (item->GetType () == "" &&
          item->GetDefaultValue () == "")
        {
          std::cout << "--" << item->GetName () << "\t" << item->GetHelp () << std::endl;
        }
      else
        {
          std::cout << "--" << item->GetName () << "=[" << item->GetType () << ":"
                    << item->GetDefaultValue () << "]\t" << item->GetHelp () << std::endl;
        }
    }
  for (List::iterator i = DefaultValueList::Begin ();
       i != DefaultValueList::End (); i++)
    {
      DefaultValueBase *item = *i;
      if (item->GetType () == "" &&
          item->GetDefaultValue () == "")
        {
          std::cout << "--" << item->GetName () << "\t" << item->GetHelp () << std::endl;
        }
      else
        {
          std::cout << "--" << item->GetName () << "=[" << item->GetType () << ":"
                    << item->GetDefaultValue () << "]\t" << item->GetHelp () << std::endl;
        }
    }
  // XXX on win32, do the right thing here.
  exit (0);
}

void 
CommandLine::Parse (int argc, char *argv[])
{
  for (argc--, argv++; argc > 0; argc--, argv++)
    {
      // remove "--" or "-" heading.
      std::string param = *argv;
      std::string::size_type cur = param.find ("--");
      if (cur == std::string::npos)
        {
          cur = param.find ("-");
          if (cur == std::string::npos)
            {
              // invalid argument. ignore it.
              continue;
            }
        }
      if (cur != 0)
        {
          // invalid argument. ignore it.
          continue;
        }
      param = std::string (param, 2, param.size ());
      cur = param.find ("=");
      std::string name, value;
      if (cur == std::string::npos)
        {
          if (argc == 1)
            {
              // invalid argument. ignore it.
              continue;
            }
          argv++;
          argc--;
          name = param;
          value = *argv;
        }
      else
        {
          name = std::string (param, 0, cur);
          value = std::string (param, cur + 1, std::string::npos);
        }
      // try to find this argument in the user args.
      for (List::iterator i = GetUserList ()->begin ();
           i != GetUserList ()->end (); i++)
        {
          DefaultValueBase *item = *i;
          if (item->GetName () == name)
            {
              if (!item->ParseValue (value))
                {
                  std::cerr << "Warning: failed to parse command line argument `"
                            << name << "' of type '" << item->GetType ()
                            << "' with value `" << value << "'." << std::endl;
                }
              continue;
            }
        }
           
      // try to find this argument in the default args.
      for (List::iterator i = DefaultValueList::Begin ();
           i != DefaultValueList::End (); i++)
        {
          DefaultValueBase *item = *i;
          if (item->GetName () == name)
            {
              item->ParseValue (value);
              continue;
            }
        }
    }
}

}//namespace ns3



#ifdef RUN_SELF_TESTS
#include "test.h"
#include <iostream>
#include <sstream>

namespace ns3 {


class CommandLineTest : public Test
{
public:
  CommandLineTest () : Test ("CommandLine") {}
  virtual bool RunTests (void)
  {
    bool result = true;

    // redirect stderr temporarily (else warnings appear during unit testing, which is not nice)
    std::ostringstream nullout;
    std::streambuf *origcerr = std::cerr.rdbuf (nullout.rdbuf ());
    {
      char *argv[] = {"run-tests", "--loops", "bad-value", NULL};
      int argc = sizeof (argv) / sizeof (argv[0]) - 1;
      
      uint32_t loops = 123;
      CommandLine::AddArgValue ("loops","a test of the command line", loops);
      CommandLine::Parse (argc, argv);
      
      NS_TEST_ASSERT_EQUAL (loops, 123);
    }

    {
      char *argv[] = {"run-tests", "--loops=bad-value", NULL};
      int argc = sizeof (argv) / sizeof (argv[0]) - 1;
      
      uint32_t loops = 123;
      CommandLine::AddArgValue ("loops","a test of the command line", loops);
      CommandLine::Parse (argc, argv);
      
      NS_TEST_ASSERT_EQUAL (loops, 123);
    }

    {
      char *argv[] = {"run-tests", "--loops", "456", NULL};
      int argc = sizeof (argv) / sizeof (argv[0]) - 1;
      
      uint32_t loops = 123;
      CommandLine::AddArgValue ("loops","a test of the command line", loops);
      CommandLine::Parse (argc, argv);
      
      NS_TEST_ASSERT_EQUAL (loops, 456);
    }

    {
      char *argv[] = {"run-tests", "--loops=456", NULL};
      int argc = sizeof (argv) / sizeof (argv[0]) - 1;
      
      uint32_t loops = 123;
      CommandLine::AddArgValue ("loops","a test of the command line", loops);
      CommandLine::Parse (argc, argv);
      
      NS_TEST_ASSERT_EQUAL (loops, 456);
    }

    // unredirect cerr
    std::cerr.rdbuf (origcerr);


    return result;
  }
};


static CommandLineTest g_commandLineTests;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
