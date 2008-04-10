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
#include "uinteger.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

Uinteger::Uinteger (uint64_t value)
  : m_value (value)
{}
Uinteger::Uinteger ()
{}
void 
Uinteger::Set (uint64_t value)
{
  m_value = value;
}
uint64_t 
Uinteger::Get (void) const
{
  return m_value;
}
Uinteger::operator uint64_t () const
{
  return m_value;
}
std::ostream & operator << (std::ostream &os, const Uinteger &uinteger)
{
  os << uinteger.Get ();
  return os;
}
std::istream & operator >> (std::istream &is, Uinteger &uinteger)
{
  uint64_t v;
  is >> v;
  uinteger.Set (v);
  return is;
}

ATTRIBUTE_CONVERTER_IMPLEMENT(Uinteger);
ATTRIBUTE_VALUE_IMPLEMENT(Uinteger);

namespace internal {

Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min, uint64_t max, std::string name)
{
  struct Checker : public AttributeChecker
  {
    Checker (uint64_t minValue, uint64_t maxValue, std::string name)
      : m_minValue (minValue),
        m_maxValue (maxValue),
        m_name (name) {}
    virtual bool Check (Attribute value) const {
      const UintegerValue *v = value.DynCast<const UintegerValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get ().Get () >= m_minValue && v->Get ().Get () <= m_maxValue;
    }
    virtual std::string GetType (void) const {
      return m_name;
    }
    virtual bool HasTypeConstraints (void) const {
      return true;
    }
    virtual std::string GetTypeConstraints (void) const {
      std::ostringstream oss;
      oss << m_minValue << ":" << m_maxValue;
      return oss.str ();
    }
    virtual Attribute Create (void) const {
      return Attribute (ns3::Create<UintegerValue> ());
    }
    uint64_t m_minValue;
    uint64_t m_maxValue;
    std::string m_name;
  } *checker = new Checker (min, max, name);
  return Ptr<const AttributeChecker> (checker, false);
}

} // namespace internal

} // namespace ns3
