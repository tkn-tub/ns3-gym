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
#include "global-value.h"
#include "fatal-error.h"
#include "attribute.h"
#include "string.h"
#include "ns3/core-config.h"
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

namespace ns3 {

GlobalValue::GlobalValue (std::string name, std::string help,
			  const AttributeValue &initialValue,
			  Ptr<const AttributeChecker> checker)
  : m_name (name),
    m_help (help),
    m_initialValue (initialValue.Copy ()),
    m_checker (checker)
{
  if (m_checker == 0)
    {
      NS_FATAL_ERROR ("Checker should not be zero.");
    }
  GetVector ()->push_back (this);
  InitializeFromEnv ();
}

void
GlobalValue::InitializeFromEnv (void)
{
#ifdef HAVE_GETENV
  char *envVar = getenv ("NS_GLOBAL_VALUE");
  if (envVar == 0)
    {
      return;
    }
  std::string env = std::string (envVar);
  std::string::size_type cur = 0;
  std::string::size_type next = 0;
  while (next != std::string::npos)
    {
      next = env.find (";", cur);
      std::string tmp = std::string (env, cur, next-cur);
      std::string::size_type equal = tmp.find ("=");
      if (equal != std::string::npos)
        {
          std::string name = tmp.substr (0, equal);
          std::string value = tmp.substr (equal+1, tmp.size () - equal - 1);
          if (name == m_name)
            {
              SetValue (StringValue (value));
            }
        }
      cur = next + 1;
    }
#endif /* HAVE_GETENV */
}

std::string 
GlobalValue::GetName (void) const
{
  return m_name;
}
std::string 
GlobalValue::GetHelp (void) const
{
  return m_help;
}
void
GlobalValue::GetValue (AttributeValue &value) const
{
  bool ok = m_checker->Copy (*m_initialValue, value);
  if (ok)
    {
      return;
    }
  StringValue *str = dynamic_cast<StringValue *> (&value);
  if (str == 0)
    {
      NS_FATAL_ERROR ("GlobalValue name="<<m_name<<": input value is not a string");
    }
  str->Set (m_initialValue->SerializeToString (m_checker));
}
Ptr<const AttributeChecker> 
GlobalValue::GetChecker (void) const
{
  return m_checker;
}
  
bool
GlobalValue::SetValue (const AttributeValue &value)
{
  if (m_checker->Check (value))
    {
      m_initialValue = value.Copy ();
      return true;
    }
  // attempt to convert to string.
  const StringValue *str = dynamic_cast<const StringValue *> (&value);
  if (str == 0)
    {
      return false;
    }
  // attempt to convert back to value.
  Ptr<AttributeValue> v = m_checker->Create ();
  bool ok = v->DeserializeFromString (str->Get (), m_checker);
  if (!ok)
    {
      return false;
    }
  ok = m_checker->Check (*v);
  if (!ok)
    {
      return false;
    }
  m_checker->Copy (*v, *PeekPointer (m_initialValue));
  return true;
}

void 
GlobalValue::Bind (std::string name, const AttributeValue &value)
{
  for (Iterator i = Begin (); i != End (); i++)
    {
      if ((*i)->GetName () == name)
	{
	  if (!(*i)->SetValue (value))
	    {
	      NS_FATAL_ERROR ("Invalid new value for global value: "<<name);
	    }
	  return;
	}
    }
  NS_FATAL_ERROR ("Non-existant global value: "<<name);
}
bool 
GlobalValue::BindFailSafe (std::string name, const AttributeValue &value)
{
  for (Iterator i = Begin (); i != End (); i++)
    {
      if ((*i)->GetName () == name)
	{
	  return (*i)->SetValue (value);
	}
    }
  return false;
}
GlobalValue::Iterator 
GlobalValue::Begin (void)
{
  return GetVector ()->begin ();
}
GlobalValue::Iterator 
GlobalValue::End (void)
{
  return GetVector ()->end ();
}

bool
GlobalValue::GetValueByNameFailSafe (std::string name, AttributeValue &value)
{
  for (GlobalValue::Iterator gvit = GlobalValue::Begin (); gvit != GlobalValue::End (); ++gvit)
    {
      if ((*gvit)->GetName () == name)
         {
           (*gvit)->GetValue (value);  
           return true;
         }
    } 
  return false; // not found
}

void
GlobalValue::GetValueByName (std::string name, AttributeValue &value)
{
  if (! GetValueByNameFailSafe (name, value))
    {
      NS_FATAL_ERROR ("Could not find GlobalValue named \"" << name << "\"");
    }
}

GlobalValue::Vector *
GlobalValue::GetVector (void)
{
  static Vector vector;
  return &vector;
}

} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "uinteger.h"

namespace ns3 {

class GlobalValueTests : public Test
{
public:
  GlobalValueTests ();
  virtual bool RunTests (void);
private:
};

GlobalValueTests::GlobalValueTests ()
  : Test ("GlobalValue")
{}
bool 
GlobalValueTests::RunTests (void)
{
  bool result = true;
  GlobalValue uint = GlobalValue ("TestUint", "help text",
				  UintegerValue (10),
				  MakeUintegerChecker<uint32_t> ());


  UintegerValue v;
  uint.GetValue (v);
  NS_TEST_ASSERT_EQUAL (10, v.Get ());

  GlobalValue::Vector *vector = GlobalValue::GetVector ();
  for (GlobalValue::Vector::iterator i = vector->begin (); i != vector->end (); ++i)
    {
      if ((*i) == &uint)
	{
	  vector->erase (i);
	  break;
	}
    }

  return result;
}

static GlobalValueTests g_initialValueTests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
