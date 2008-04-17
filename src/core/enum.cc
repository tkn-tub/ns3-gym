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
#include "enum.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

EnumValue::EnumValue ()
  : m_v ()
{}
EnumValue::EnumValue (int v)
  : m_v (v)
{}
void 
EnumValue::Set (int v)
{
  m_v = v;
}
int 
EnumValue::Get (void) const
{
  return m_v;
}

Ptr<AttributeValue>
EnumValue::Copy (void) const
{
  return ns3::Create<EnumValue> (*this);
}
std::string 
EnumValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  const EnumChecker *p = dynamic_cast<const EnumChecker *> (PeekPointer (checker));
  NS_ASSERT (p != 0);
  for (EnumChecker::ValueSet::const_iterator i = p->m_valueSet.begin (); i != p->m_valueSet.end (); i++)
    {
      if (i->first == m_v)
	{
	  return i->second;
	}
    }
  
  NS_FATAL_ERROR ("The user has set an invalid C++ value in this Enum");
  // quiet compiler.
  return "";
}
bool 
EnumValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  const EnumChecker *p = dynamic_cast<const EnumChecker *> (PeekPointer (checker));
  NS_ASSERT (p != 0);
  for (EnumChecker::ValueSet::const_iterator i = p->m_valueSet.begin (); i != p->m_valueSet.end (); i++)
    {
      if (i->second == value)
	{
	  m_v = i->first;
	  return true;
	}
    }
  return false;
}

EnumChecker::EnumChecker ()
{}

void 
EnumChecker::AddDefault (int v, std::string name)
{
  m_valueSet.push_front (std::make_pair (v, name));
}
void 
EnumChecker::Add (int v, std::string name)
{
  m_valueSet.push_back (std::make_pair (v, name));
}
bool 
EnumChecker::Check (const AttributeValue &value) const
{
  const EnumValue *p = dynamic_cast<const EnumValue *> (&value);
  if (p == 0)
    {
      return false;
    }
  for (ValueSet::const_iterator i = m_valueSet.begin (); i != m_valueSet.end (); i++)
    {
      if (i->first == p->Get ())
	{
	  return true;
	}
    }
  return false;
}
std::string 
EnumChecker::GetType (void) const
{
  return "Enum";
}
bool 
EnumChecker::HasTypeConstraints (void) const
{
  return true;
}
std::string 
EnumChecker::GetTypeConstraints (void) const
{
  std::ostringstream oss;
  for (ValueSet::const_iterator i = m_valueSet.begin (); i != m_valueSet.end ();)
    {
      oss << i->second;
      i++;
      if (i != m_valueSet.end ())
        {
          oss << "|";
        }
    }
  return oss.str ();
}
Ptr<AttributeValue>
EnumChecker::Create (void) const
{
  return ns3::Create<EnumValue> ();
}

bool 
EnumChecker::Copy (const AttributeValue &source, AttributeValue &destination) const
{
  const EnumValue *src = dynamic_cast<const EnumValue *> (&source);
  EnumValue *dst = dynamic_cast<EnumValue *> (&destination);
  if (src == 0 || dst == 0)
    {
      return false;
    }
  *dst = *src;
  return true;
}


Ptr<const AttributeChecker> 
MakeEnumChecker (int v1, std::string n1,
		 int v2, std::string n2,
		 int v3, std::string n3,
		 int v4, std::string n4,
		 int v5, std::string n5,
		 int v6, std::string n6,
		 int v7, std::string n7,
		 int v8, std::string n8,
		 int v9, std::string n9,
		 int v10, std::string n10,
		 int v11, std::string n11,
		 int v12, std::string n12)
{
  Ptr<EnumChecker> checker = Create<EnumChecker> ();
  checker->AddDefault (v1, n1);
  if (n2 == "")
    {
      return checker;
    }
  checker->Add (v2, n2);
  if (n3 == "")
    {
      return checker;
    }
  checker->Add (v3, n3);
  if (n4 == "")
    {
      return checker;
    }
  checker->Add (v4, n4);
  if (n5 == "")
    {
      return checker;
    }
  checker->Add (v5, n5);
  if (n6 == "")
    {
      return checker;
    }
  checker->Add (v6, n6);
  if (n7 == "")
    {
      return checker;
    }
  checker->Add (v7, n7);
  if (n8 == "")
    {
      return checker;
    }
  checker->Add (v8, n8);
  if (n9 == "")
    {
      return checker;
    }
  checker->Add (v9, n9);
  if (n10 == "")
    {
      return checker;
    }
  checker->Add (v10, n10);
  if (n11 == "")
    {
      return checker;
    }
  checker->Add (v11, n11);
  if (n12 == "")
    {
      return checker;
    }
  checker->Add (v12, n12);
  return checker;
}


} // namespace ns3
