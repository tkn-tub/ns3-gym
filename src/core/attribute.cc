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
#include "attribute.h"
#include "log.h"
#include "fatal-error.h"
#include <sstream>

NS_LOG_COMPONENT_DEFINE ("AttributeValue");

namespace ns3 {

AttributeValue::AttributeValue ()
{}
AttributeValue::~AttributeValue ()
{}

Attribute::Attribute ()
  : m_value ()
{}
Attribute::Attribute (Ptr<AttributeValue> value)
  : m_value (value)
{}

Attribute 
Attribute::Copy (void) const
{
  return m_value->Copy ();
}
std::string 
Attribute::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  return m_value->SerializeToString (checker);
}
bool 
Attribute::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  return m_value->DeserializeFromString (value, checker);
}

AttributeAccessor::AttributeAccessor ()
  : m_count (1)
{}
void 
AttributeAccessor::Ref (void) const
{
  m_count++;
}
void 
AttributeAccessor::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}
AttributeAccessor::~AttributeAccessor ()
{}

AttributeChecker::AttributeChecker ()
  : m_count (1)
{}
void 
AttributeChecker::Ref (void) const
{
  m_count++;
}
void 
AttributeChecker::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}
AttributeChecker::~AttributeChecker ()
{}

} // namespace ns3
