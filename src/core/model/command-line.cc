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

#include <algorithm>  // for transform
#include <cctype>     // for tolower
#include <cstdlib>    // for exit
#include <iomanip>    // for setw, boolalpha
#include <set>
#include <sstream>

#include "command-line.h"
#include "log.h"
#include "config.h"
#include "global-value.h"
#include "system-path.h"
#include "type-id.h"
#include "string.h"


NS_LOG_COMPONENT_DEFINE ("CommandLine");

namespace ns3 {

CommandLine::CommandLine ()
{
  NS_LOG_FUNCTION (this);
}
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
  NS_LOG_FUNCTION (this);
  Clear ();
}
void
CommandLine::Copy (const CommandLine &cmd)
{
  NS_LOG_FUNCTION (&cmd);

  for (Items::const_iterator i = cmd.m_items.begin (); 
       i != cmd.m_items.end (); ++i)
    {
      m_items.push_back (*i);
    }
  m_usage = cmd.m_usage;
  m_name  = cmd.m_name;
}
void
CommandLine::Clear (void)
{
  NS_LOG_FUNCTION (this);

  for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
    {
      delete *i;
    }
  m_items.clear ();
  m_usage = "";
  m_name  = "";
}

void
CommandLine::Usage (const std::string usage)
{
  m_usage = usage;
}

std::string
CommandLine::GetName () const
{
  return m_name;
}

CommandLine::Item::~Item ()
{
  NS_LOG_FUNCTION (this);
}

void
CommandLine::Parse (int iargc, char *argv[])
{
  NS_LOG_FUNCTION (this << iargc << argv);

  m_name = SystemPath::Split (argv[0]).back ();
  
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
CommandLine::PrintHelp (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);

  os << m_name << " [Program Arguments] [General Arguments]"
            << std::endl;
  
  if (m_usage.length ())
    {
      os << std::endl;
      os << m_usage << std::endl;
    }
  
  if (!m_items.empty ())
    {
      size_t width = 0;
      for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
        {
          width = std::max (width, (*i)->m_name.size ());
        }
      width += 3;

      os << std::endl;
      os << "Program Arguments:" << std::endl;
      for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
        {
          os << "    --"
                    << std::left << std::setw (width) << ( (*i)->m_name + ":")
                    << std::right
                    << (*i)->m_help;

          if ( (*i)->HasDefault ())
            {
              os << " [" << (*i)->GetDefault () << "]";
            }
          os << std::endl;
        }
    }

  os << std::endl;
  os
    << "General Arguments:\n"
    << "    --PrintGlobals:              Print the list of globals.\n"
    << "    --PrintGroups:               Print the list of groups.\n"
    << "    --PrintGroup=[group]:        Print all TypeIds of group.\n"
    << "    --PrintTypeIds:              Print all TypeIds.\n"
    << "    --PrintAttributes=[typeid]:  Print all attributes of typeid.\n"
    << "    --PrintHelp:                 Print this help message.\n"
    << std::endl;
}

void
CommandLine::PrintGlobals (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);

  os << "Global values:" << std::endl;

  // Sort output
  std::vector<std::string> globals;
  
  for (GlobalValue::Iterator i = GlobalValue::Begin ();
       i != GlobalValue::End ();
       ++i)
    {
      std::stringstream ss;
      ss << "    --" << (*i)->GetName () << "=[";
      Ptr<const AttributeChecker> checker = (*i)->GetChecker ();
      StringValue v;
      (*i)->GetValue (v);
      ss << v.Get () << "]" << std::endl;
      ss << "        " << (*i)->GetHelp () << std::endl;
      globals.push_back (ss.str ());
    }
  std::sort (globals.begin (), globals.end ());
  for (std::vector<std::string>::const_iterator it = globals.begin ();
       it < globals.end ();
       ++it)
    {
      os << *it;
    }
}

void
CommandLine::PrintAttributes (std::ostream &os, const std::string &type) const
{
  NS_LOG_FUNCTION (this);

  TypeId tid;
  if (!TypeId::LookupByNameFailSafe (type, &tid))
    {
      NS_FATAL_ERROR ("Unknown type=" << type << " in --PrintAttributes");
    }

  os << "Attributes for TypeId " << tid.GetName () << std::endl;
  
  // Sort output
  std::vector<std::string> attributes;
  
  for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
    {
      std::stringstream ss;
      ss << "    --" << tid.GetAttributeFullName (i) << "=[";
      struct TypeId::AttributeInformation info = tid.GetAttribute (i);
      ss << info.initialValue->SerializeToString (info.checker) << "]"
                << std::endl;
      ss << "        " << info.help << std::endl;
      attributes.push_back (ss.str ());
    }
  std::sort (attributes.begin (), attributes.end ());
  for (std::vector<std::string>::const_iterator it = attributes.begin ();
       it < attributes.end ();
       ++it)
    {
      os << *it;
    }
}


void
CommandLine::PrintGroup (std::ostream &os, const std::string &group) const
{
  NS_LOG_FUNCTION (this);

  os << "TypeIds in group " << group << ":" << std::endl;
  
  // Sort output
  std::vector<std::string> groupTypes;
  
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      std::stringstream ss;
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetGroupName () == group)
        {
          ss << "    " <<tid.GetName () << std::endl;
        }
      groupTypes.push_back (ss.str ());
    }
  std::sort (groupTypes.begin (), groupTypes.end ());
  for (std::vector<std::string>::const_iterator it = groupTypes.begin ();
       it < groupTypes.end ();
       ++it)
    {
      os << *it;
    }
}

void
CommandLine::PrintTypeIds (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);
  os << "Registered TypeIds:" << std::endl;

  // Sort output
  std::vector<std::string> types;
    
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      std::stringstream ss;
      TypeId tid = TypeId::GetRegistered (i);
      ss << "    " << tid.GetName () << std::endl;
      types.push_back (ss.str ());
    }
  std::sort (types.begin (), types.end ());
  for (std::vector<std::string>::const_iterator it = types.begin ();
       it < types.end ();
       ++it)
    {
      os << *it;
    }
}

void
CommandLine::PrintGroups (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);

  std::set<std::string> groups;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      groups.insert (tid.GetGroupName ());
    }
  
  os << "Registered TypeId groups:" << std::endl;
  // Sets are already sorted
  for (std::set<std::string>::const_iterator k = groups.begin ();
       k != groups.end ();
       ++k)
    {
      os << "    " << *k << std::endl;
    }
}

void
CommandLine::HandleArgument (const std::string &name, const std::string &value) const
{
  NS_LOG_FUNCTION (this << name << value);

  NS_LOG_DEBUG ("Handle arg name=" << name << " value=" << value);
  if (name == "PrintHelp" || name == "help")
    {
      // method below never returns.
      PrintHelp (std::cout);
      std::exit (0);
    } 
  else if (name == "PrintGroups")
    {
      // method below never returns.
      PrintGroups (std::cout);
      std::exit (0);
    }
  else if (name == "PrintTypeIds")
    {
      // method below never returns.
      PrintTypeIds (std::cout);
      std::exit (0);
    }
  else if (name == "PrintGlobals")
    {
      // method below never returns.
      PrintGlobals (std::cout);
      std::exit (0);
    }
  else if (name == "PrintGroup")
    {
      // method below never returns.
      PrintGroup (std::cout, value);
      std::exit (0);
    }
  else if (name == "PrintAttributes")
    {
      // method below never returns.
      PrintAttributes (std::cout, value);
      std::exit (0);
    }
  else
    {
      for (Items::const_iterator i = m_items.begin (); i != m_items.end (); ++i)
        {
          if ((*i)->m_name == name)
            {
              if (!(*i)->Parse (value))
                {
                  std::cerr << "Invalid argument value: "
                            << name << "=" << value << std::endl;
                  std::exit (1);
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
      std::cerr << "Invalid command-line arguments: --"
                << name << "=" << value << std::endl;
      PrintHelp (std::cerr);
      std::exit (1);
    }
}

bool
CommandLine::CallbackItem::Parse (const std::string value)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("CommandLine::CallbackItem::Parse \"" << value << "\"");
  return m_callback (value);
}

void
CommandLine::AddValue (const std::string &name,
                       const std::string &help,
                       Callback<bool, std::string> callback)
{
  NS_LOG_FUNCTION (this << &name << &help << &callback);
  CallbackItem *item = new CallbackItem ();
  item->m_name = name;
  item->m_help = help;
  item->m_callback = callback;
  m_items.push_back (item);
}

void
CommandLine::AddValue (const std::string &name,
                       const std::string &attributePath)
{
  NS_LOG_FUNCTION (this << name << attributePath);
  // Attribute name is last token
  size_t colon = attributePath.rfind ("::");
  const std::string typeName = attributePath.substr (0, colon);
  NS_LOG_DEBUG ("typeName: '" << typeName << "', colon: " << colon);
  
  TypeId tid;
  if (!TypeId::LookupByNameFailSafe (typeName, &tid))
    {
      NS_FATAL_ERROR ("Unknown type=" << typeName);
    }

  const std::string attrName = attributePath.substr (colon + 2);
  struct TypeId::AttributeInformation info;  
  if (!tid.LookupAttributeByName (attrName, &info))
    {
      NS_FATAL_ERROR ("Attribute not found: " << attributePath);
    }
      
  std::stringstream ss;
  ss << info.help
     << " (" << attributePath << ") ["
     << info.initialValue->SerializeToString (info.checker) << "]";
  
  AddValue (name, ss.str (),
            MakeBoundCallback (CommandLine::HandleAttribute, attributePath)) ;
}


/* static */
bool
CommandLine::HandleAttribute (const std::string name,
                              const std::string value)
{
  bool success = true;
  if (!Config::SetGlobalFailSafe (name, StringValue (value))
      && !Config::SetDefaultFailSafe (name, StringValue (value)))
    {
      success = false;
    }
  return success;
}
    

bool
CommandLine::Item::HasDefault () const
{
  return false;
}

std::string
CommandLine::Item::GetDefault () const
{
  return "";
}

template <>
std::string
CommandLineHelper::GetDefault<bool> (const bool & val)
{
  std::ostringstream oss;
  oss << std::boolalpha << val;
  return oss.str ();
}

template <>
bool
CommandLineHelper::UserItemParse<bool> (const std::string value, bool & val)
{
  std::string src = value;
  std::transform(src.begin(), src.end(), src.begin(), ::tolower);
  
  if (src.length () == 0)
    {
      val = ! val;
      return true;
    }
  else if ( (src == "true") || (src == "t") )
    {
      val = true;
      return true;
    }
  else if ( (src == "false") || (src == "f"))
    {
      val = false;
      return true;
    }
  else
    {
      std::istringstream iss;
      iss.str (src);
      iss >> val;
      return !iss.bad () && !iss.fail ();
    }
}

std::ostream &
operator << (std::ostream & os, const CommandLine & cmd)
{
  cmd.PrintHelp (os);
  return os;
}

} // namespace ns3
