/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "default-value.h"
#include "fatal-error.h"

namespace ns3 {

namespace DefaultValue {

enum BindStatus {
  OK,
  INVALID_VALUE,
  NOT_FOUND
};


static 
enum BindStatus
BindSafe (std::string name, std::string value)
{
  for (DefaultValueList::Iterator i = DefaultValueList::Begin ();
       i != DefaultValueList::End (); i++)
    {
      DefaultValueBase *cur = *i;
      if (cur->GetName () == name)
	{
	  if (!cur->ParseValue (value))
	    {
	      return INVALID_VALUE;
	    }
	  return OK;
	}
    }
  return NOT_FOUND;
}

void
Bind (std::string name, std::string value)
{
  switch (BindSafe (name, value)) {
  case INVALID_VALUE:
    NS_FATAL_ERROR ("Invalid value: "<<name<<"="<<value);
    break;
  case NOT_FOUND:
    NS_FATAL_ERROR ("No registered DefaultValue=\"" << name << "\"");
    break;
  case OK:
    break;
  }
}

}

DefaultValueBase::DefaultValueBase (const std::string &name,
				    const std::string &help)
  : m_name (name),
    m_help (help),
    m_dirty (false)
{}
DefaultValueBase::~DefaultValueBase ()
{}
std::string 
DefaultValueBase::GetName (void) const
{
  return m_name;
}
std::string 
DefaultValueBase::GetHelp (void) const
{
  return m_help;
}
bool 
DefaultValueBase::IsDirty (void) const
{
  return m_dirty;
}
void 
DefaultValueBase::ClearDirtyFlag (void)
{
  m_dirty = false;
}
bool 
DefaultValueBase::ParseValue (const std::string &value)
{
  bool ok = DoParseValue (value);
  if (ok)
    {
      m_dirty = true;
    }
  return ok;
}
std::string 
DefaultValueBase::GetType (void) const
{
  return DoGetType ();
}
std::string 
DefaultValueBase::GetDefaultValue (void) const
{
  return DoGetDefaultValue ();
}


DefaultValueList::Iterator 
DefaultValueList::Begin (void)
{
  return GetList ()->begin ();
}
DefaultValueList::Iterator 
DefaultValueList::End (void)
{
  return GetList ()->end ();
}
void
DefaultValueList::Remove (const std::string &name)
{
  DefaultValueList::List *list = GetList ();
  for (List::iterator i = list->begin (); i != list->end (); /* nothing */)
    {
      if ((*i)->GetName () == name)
        {
          i = list->erase (i);
        }
      else
        {
          i++;
        }
    }
}
void 
DefaultValueList::Add (DefaultValueBase *defaultValue)
{
  GetList ()->push_back (defaultValue);
}

DefaultValueList::List *
DefaultValueList::GetList (void)
{
  static List list;
  return &list;
}

BooleanDefaultValue::BooleanDefaultValue (std::string name,
					  std::string help,
					  bool defaultValue)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_value (defaultValue)
{
  DefaultValueList::Add (this);
}
bool 
BooleanDefaultValue::GetValue (void) const
{
  return m_value;
}
bool 
BooleanDefaultValue::DoParseValue (const std::string &value)
{
  if (value.compare ("0") == 0 ||
      value.compare ("f") == 0 ||
      value.compare ("false") == 0 ||
      value.compare ("FALSE") == 0) 
    {
      m_value = false;
      return true;
    } 
  else if (value.compare ("1") == 0 ||
	   value.compare ("t") == 0 ||
	   value.compare ("true") == 0 ||
	   value.compare ("TRUE") == 0) 
    {
      m_value = true;
      return true;
    } 
  else 
    {
      return false;
    }
}
std::string 
BooleanDefaultValue::DoGetType (void) const
{
  return "bool";
}
std::string 
BooleanDefaultValue::DoGetDefaultValue (void) const
{
  return m_defaultValue?"true":"false";
}


StringEnumDefaultValue::StringEnumDefaultValue (const std::string &name,
                                                const std::string &help)
  : DefaultValueBase (name, help),
    m_oneDefault (false)
{
  DefaultValueList::Add (this);
}
void 
StringEnumDefaultValue::AddDefaultValue (const std::string &value)
{
  if (m_oneDefault)
    {
      NS_FATAL_ERROR ("More than one default value registered: " << value);
    }
  m_oneDefault = true;
  for (std::list<std::string>::iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (value == *i)
        {
          NS_FATAL_ERROR ("Value already exists: " << value);
        }
    }  
  m_possibleValues.push_back (value);
  m_value = value;
  m_defaultValue = value;
}
void 
StringEnumDefaultValue::AddPossibleValue (const std::string &value)
{
  for (std::list<std::string>::iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (value == *i)
        {
          NS_FATAL_ERROR ("Value already exists: " << value);
        }
    }
  m_possibleValues.push_back (value);
}
std::string 
StringEnumDefaultValue::GetValue (void) const
{
  return m_value;
}
bool 
StringEnumDefaultValue::DoParseValue (const std::string &value)
{
  for (std::list<std::string>::iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (value == *i)
        {
          m_value = value;
          return true;
        }
    }
  return false;
}
std::string 
StringEnumDefaultValue::DoGetType (void) const
{
  std::string retval;
  retval += "(";
  for (std::list<std::string>::const_iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (i != m_possibleValues.begin ())
	{
	  retval += "|";
	}
      retval += *i;
    }
  retval += ")";
  return retval;
}
std::string 
StringEnumDefaultValue::DoGetDefaultValue (void) const
{
  return m_defaultValue;
}


CommandDefaultValue::CommandDefaultValue (const std::string &name,
					  const std::string &help,
					  Callback<void> cb)
  : DefaultValueBase (name, help),
    m_cb (cb)
{
  DefaultValueList::Add (this);
}
bool 
CommandDefaultValue::DoParseValue (const std::string &value)
{
  m_cb ();
  return true;
}
std::string 
CommandDefaultValue::DoGetType (void) const
{
  return "";
}
std::string 
CommandDefaultValue::DoGetDefaultValue (void) const
{
  return "";
}


}//namespace ns3

#ifdef RUN_SELF_TESTS
#include "test.h"

namespace ns3 {

enum MyEnum {
  MY_ENUM_A,
  MY_ENUM_B,
  MY_ENUM_C,
  MY_ENUM_D,
};


class DefaultValueTest : public Test
{
public:
  DefaultValueTest ();
  virtual bool RunTests (void);
};

DefaultValueTest::DefaultValueTest ()
  : Test ("DefaultValue")
{}
bool 
DefaultValueTest::RunTests (void)
{
  bool result = true;

  BooleanDefaultValue a ("bool-a", "help a", true);
  NS_TEST_ASSERT (a.GetValue ());
  DefaultValue::Bind ("bool-a", "false");
  NS_TEST_ASSERT (!a.GetValue ());
  BooleanDefaultValue b ("bool-b", "help b", false);
  DefaultValue::Bind ("bool-b", "true");
  NS_TEST_ASSERT (b.GetValue ());
  DefaultValue::Bind ("bool-b", "0");
  NS_TEST_ASSERT (!b.GetValue ());
  DefaultValue::Bind ("bool-b", "1");
  NS_TEST_ASSERT (b.GetValue ());
  DefaultValue::Bind ("bool-b", "f");
  NS_TEST_ASSERT (!b.GetValue ());
  DefaultValue::Bind ("bool-b", "t");
  NS_TEST_ASSERT (b.GetValue ());

  DefaultValue::Bind ("bool-b", "false");
  NS_TEST_ASSERT (!b.GetValue ());
  NS_TEST_ASSERT_EQUAL (DefaultValue::BindSafe ("bool-b", "tr"), DefaultValue::INVALID_VALUE)

  NumericDefaultValue<int32_t> i ("test-i", "help-i", -1);
  NS_TEST_ASSERT_EQUAL (i.GetValue (), -1);
  DefaultValue::Bind ("test-i", "-2");
  NS_TEST_ASSERT_EQUAL (i.GetValue (), -2);
  DefaultValue::Bind ("test-i", "+2");
  NS_TEST_ASSERT_EQUAL (i.GetValue (), 2);
  NS_TEST_ASSERT_EQUAL (i.GetType (), "int32_t(-2147483648:2147483647)");
  NumericDefaultValue<uint32_t> ui32 ("test-ui32", "help-ui32", 10);
  NS_TEST_ASSERT_EQUAL (ui32.GetType (), "uint32_t(0:4294967295)");
  NumericDefaultValue<int8_t> c ("test-c", "help-c", 10);
  NS_TEST_ASSERT_EQUAL (c.GetValue (), 10);
  DefaultValue::Bind ("test-c", "257");  
  NumericDefaultValue<float> x ("test-x", "help-x", 10.0);
  NumericDefaultValue<double> y ("test-y", "help-y", 10.0);
  DefaultValue::Bind ("test-y", "-3");  

  EnumDefaultValue<enum MyEnum> e ("test-e", "help-e",
				   MY_ENUM_C, "C",
				   MY_ENUM_A, "A",
				   MY_ENUM_B, "B",
				   0, (void*)0);
  NS_TEST_ASSERT_EQUAL (e.GetValue (), MY_ENUM_C);
  DefaultValue::Bind ("test-e", "B");
  NS_TEST_ASSERT_EQUAL (e.GetValue (), MY_ENUM_B);
  NS_TEST_ASSERT_EQUAL (DefaultValue::BindSafe ("test-e", "D"), DefaultValue::INVALID_VALUE);

  class MyEnumSubclass : public EnumDefaultValue<enum MyEnum>
  {
  public:
    MyEnumSubclass () 
      : EnumDefaultValue<enum MyEnum> ("test-e1", "help-e1",
				       MY_ENUM_B, "B",
				       MY_ENUM_A, "A",
				       0, (void*)0)
    {
      AddPossibleValue (MY_ENUM_C, "C");
      AddPossibleValue (MY_ENUM_D, "D");
    }
  } e1 ;
  NS_TEST_ASSERT_EQUAL (e1.GetValue (), MY_ENUM_B);
  DefaultValue::Bind ("test-e1", "D");
  NS_TEST_ASSERT_EQUAL (e1.GetValue (), MY_ENUM_D);

  DefaultValueList::Remove ("test-e1");
  DefaultValueList::Remove ("test-e");
  DefaultValueList::Remove ("bool-b");
  DefaultValueList::Remove ("bool-a");
  DefaultValueList::Remove ("test-i");
  DefaultValueList::Remove ("test-c");
  DefaultValueList::Remove ("test-x");
  DefaultValueList::Remove ("test-y");
  DefaultValueList::Remove ("test-ui32");
  
  return result;
}

static DefaultValueTest g_default_value_tests;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
