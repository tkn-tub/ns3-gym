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
#include "integer.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

Integer::Integer (int64_t value)
  : m_value (value)
{}
Integer::Integer ()
  : m_value (0)
{}
void 
Integer::Set (int64_t value)
{
  m_value = value;
}
int64_t 
Integer::Get (void) const
{
  return m_value;
}

Integer::operator int64_t () const
{
  return m_value;
}

ATTRIBUTE_VALUE_IMPLEMENT (Integer);

std::ostream &operator << (std::ostream &os, const Integer &integer)
{
  os << integer.Get ();
  return os;
}
std::istream &operator >> (std::istream &is, Integer &integer)
{
  int64_t v;
  is >> v;
  integer.Set (v);
  return is;
}

ATTRIBUTE_CONVERTER_IMPLEMENT (Integer);


Ptr<const AttributeChecker>
MakeIntegerChecker (int64_t min, int64_t max)
{
  struct IntegerChecker : public AttributeChecker
  {
    IntegerChecker (int64_t minValue, int64_t maxValue)
      : m_minValue (minValue),
      m_maxValue (maxValue) {}
    virtual bool Check (Attribute value) const {
      const IntegerValue *v = value.DynCast<const IntegerValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get ().Get () >= m_minValue && v->Get ().Get() <= m_maxValue;
    }
    virtual Attribute Create (void) const {
      return Attribute::Create<IntegerValue> ();
    }
    int64_t m_minValue;
    int64_t m_maxValue;
  } *checker = new IntegerChecker (min, max);
  return Ptr<AttributeChecker> (checker, false);
}


} // namespace ns3
