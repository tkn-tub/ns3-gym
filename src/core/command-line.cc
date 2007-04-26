/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "command-line.h"
#include <unistd.h>

namespace ns3 {

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
  argc--;
  argv++;
  while (argc > 0)
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
          name = param;
          value = "";
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
              item->ParseValue (value);
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
      argc--;
      argv++;
    }
}

}//namespace ns3
