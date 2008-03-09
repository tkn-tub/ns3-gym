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

namespace ns3 {

GlobalValue::GlobalValue (std::string name, std::string help,
			  Attribute initialValue,
			  Ptr<const AttributeChecker> checker)
  : m_name (name),
    m_help (help),
    m_initialValue (initialValue),
    m_checker (checker)
{
  if (m_checker == 0)
    {
      NS_FATAL_ERROR ("Checker should no be zero.");
    }
  GetVector ()->push_back (this);
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
Attribute 
GlobalValue::GetValue (void) const
{
  return m_initialValue;
}
Ptr<const AttributeChecker> 
GlobalValue::GetChecker (void) const
{
  return m_checker;
}
  
bool
GlobalValue::SetValue (Attribute value)
{
  if (m_checker->Check (value))
    {
      m_initialValue = value;
      return true;
    }
  // attempt to convert to string.
  const StringValue *str = value.DynCast<const StringValue *> ();
  if (str == 0)
    {
      return false;
    }
  // attempt to convert back to value.
  Attribute v = m_checker->Create ();
  bool ok = v.DeserializeFromString (str->Get ().Get (), m_checker);
  if (!ok)
    {
      return false;
    }
  ok = m_checker->Check (v);
  if (!ok)
    {
      return false;
    }
  m_initialValue = v;
  return true;
}

void 
GlobalValue::Bind (std::string name, Attribute value)
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
GlobalValue::BindFailSafe (std::string name, Attribute value)
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
				  Uinteger (10),
				  MakeUintegerChecker<uint32_t> ());


  NS_TEST_ASSERT_EQUAL (10, Uinteger (uint.GetValue ()).Get ());

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
