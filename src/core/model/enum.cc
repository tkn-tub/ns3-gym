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
#include "log.h"
#include <sstream>

/**
 * \file
 * \ingroup attribute_Enum
 * ns3::EnumValue attribute value implementation.
 */

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Enum");

EnumValue::EnumValue ()
  : m_value ()
{
  NS_LOG_FUNCTION (this);
}
EnumValue::EnumValue (int value)
  : m_value (value)
{
  NS_LOG_FUNCTION (this << value);
}
void
EnumValue::Set (int value)
{
  NS_LOG_FUNCTION (this << value);
  m_value = value;
}
int
EnumValue::Get (void) const
{
  NS_LOG_FUNCTION (this);
  return m_value;
}
Ptr<AttributeValue>
EnumValue::Copy (void) const
{
  NS_LOG_FUNCTION (this);
  return ns3::Create<EnumValue> (*this);
}
std::string 
EnumValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  NS_LOG_FUNCTION (this << checker);
  const EnumChecker *p = dynamic_cast<const EnumChecker *> (PeekPointer (checker));
  NS_ASSERT (p != 0);
  for (EnumChecker::ValueSet::const_iterator i = p->m_valueSet.begin (); i != p->m_valueSet.end (); i++)
    {
      if (i->first == m_value)
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
  NS_LOG_FUNCTION (this << value << checker);
  const EnumChecker *p = dynamic_cast<const EnumChecker *> (PeekPointer (checker));
  NS_ASSERT (p != 0);
  for (EnumChecker::ValueSet::const_iterator i = p->m_valueSet.begin (); i != p->m_valueSet.end (); i++)
    {
      if (i->second == value)
        {
          m_value = i->first;
          return true;
        }
    }
  return false;
}

EnumChecker::EnumChecker ()
{
  NS_LOG_FUNCTION (this);
}

void
EnumChecker::AddDefault (int value, std::string name)
{
  NS_LOG_FUNCTION (this << value << name);
  m_valueSet.push_front (std::make_pair (value, name));
}
void
EnumChecker::Add (int value, std::string name)
{
  NS_LOG_FUNCTION (this << value << name);
  m_valueSet.push_back (std::make_pair (value, name));
}
bool
EnumChecker::Check (const AttributeValue &value) const
{
  NS_LOG_FUNCTION (this << &value);
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
EnumChecker::GetValueTypeName (void) const
{
  NS_LOG_FUNCTION (this);
  return "ns3::EnumValue";
}
bool 
EnumChecker::HasUnderlyingTypeInformation (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}
std::string 
EnumChecker::GetUnderlyingTypeInformation (void) const
{
  NS_LOG_FUNCTION (this);
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
  NS_LOG_FUNCTION (this);
  return ns3::Create<EnumValue> ();
}

bool 
EnumChecker::Copy (const AttributeValue &source, AttributeValue &destination) const
{
  NS_LOG_FUNCTION (this << &source << &destination);
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
                 int v12, std::string n12,
                 int v13, std::string n13,
                 int v14, std::string n14,
                 int v15, std::string n15,
                 int v16, std::string n16,
                 int v17, std::string n17,
                 int v18, std::string n18,
                 int v19, std::string n19,
                 int v20, std::string n20,
                 int v21, std::string n21,
                 int v22, std::string n22)
{
  NS_LOG_FUNCTION (v1 << n1 << v2 << n2 << v3 << n3 << v4 << n4 << v5 << n5 <<
                   v6 << n6 << v7 << n7 << v8 << n8 << v9 << n9 << v10 << n10 <<
                   v11 << n11 << v12 << n12 << v13 << n13 << v14 << n14 <<
                   v15 << n15 << v16 << n16 << v17 << n17 << v18 << n18 <<
                   v19 << n19 << v20 << n20 << v21 << n21 << v22 << n22);
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
  if (n13 == "")
    {
      return checker;
    }
  checker->Add (v13, n13);
  if (n14 == "")
    {
      return checker;
    }
  checker->Add (v14, n14);
  if (n15 == "")
    {
      return checker;
    }
  checker->Add (v15, n15);
  if (n16 == "")
    {
      return checker;
    }
  checker->Add (v16, n16);
  if (n17 == "")
    {
      return checker;
    }
  checker->Add (v17, n17);
  if (n18 == "")
    {
      return checker;
    }
  checker->Add (v18, n18);
  if (n19 == "")
    {
      return checker;
    }
  checker->Add (v19, n19);
  if (n20 == "")
    {
      return checker;
    }
  checker->Add (v20, n20);
  if (n21 == "")
    {
      return checker;
    }
  checker->Add (v21, n21);
  if (n22 == "")
    {
      return checker;
    }
  checker->Add (v22, n22);
  return checker;
}


} // namespace ns3
