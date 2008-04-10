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
#include "double.h"
#include "object.h"
#include <sstream>

namespace ns3 {

Double::Double ()
{}
Double::Double (double value)
  : m_value (value)
{}
void 
Double::Set (double value)
{
  m_value = value;
}
double 
Double::Get (void) const
{
  return m_value;
}
Double::operator double () const
{
  return m_value;
}
std::ostream & operator << (std::ostream &os, const Double &value)
{
  os << value.Get ();
  return os;
}
std::istream & operator >> (std::istream &is, Double &value)
{
  double v;
  is >> v;
  value.Set (v);
  return is;
}

ATTRIBUTE_VALUE_IMPLEMENT (Double);
ATTRIBUTE_CONVERTER_IMPLEMENT (Double);

namespace internal {

Ptr<const AttributeChecker> MakeDoubleChecker (double min, double max, std::string name)
{
  struct Checker : public AttributeChecker
  {
    Checker (double minValue, double maxValue, std::string name)
      : m_minValue (minValue),
        m_maxValue (maxValue),
        m_name (name) {}
    virtual bool Check (Attribute value) const {
      const DoubleValue *v = value.DynCast<const DoubleValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get () >= m_minValue && v->Get () <= m_maxValue;
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
      return Attribute (ns3::Create<DoubleValue> ());
    }
    double m_minValue;
    double m_maxValue;
    std::string m_name;
  } *checker = new Checker (min, max, name);
  return Ptr<const AttributeChecker> (checker, false);
}

} // namespace internal

} // namespace ns3
