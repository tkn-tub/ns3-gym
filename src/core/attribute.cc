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
  : m_count (1)
{}
AttributeValue::AttributeValue (const AttributeValue &o)
  : m_count (1)
{}
AttributeValue &
AttributeValue::operator = (const AttributeValue &o)
{
  return *this;
}
AttributeValue::~AttributeValue ()
{}

/***************************************************************
 *   Big interesting warning.
 *   ------------------------
 *
 * One might wonder why we re-implement a smart pointer below 
 * in the Attribute class. This is a very good question and the answer
 * is unfortunately pretty complicated.
 *
 * 1) We could have requested the user to use Ptr<AttributeValue> and save us
 *    a lot of pain. This, however, does not work because our smart 
 *    pointer needs a special constructor which can be used to convert
 *    objects of type Ptr<T> into a PtrValue<T> to hold the pointer.
 *
 * 2) We could have made the m_value member variable below a Ptr<AttributeValue>
 *    rather than store a raw pointer. This, however, does not work
 *    because this would mean that the constructor Attribute (AttributeValue *)
 *    should be morphed into Attribute (Ptr<AttributeValue>) which, unfortunately,
 *    would conflict with the template constructor Attribute (Ptr<T>)...
 *
 * This is definitely not fun.   
 */
Attribute::Attribute ()
  : m_value (0)
{}
Attribute::Attribute (const Attribute &o)
  : m_value (o.m_value)
{
  if (m_value != 0)
    {
      m_value->m_count++;
      NS_LOG_DEBUG ("this="<<m_value<<" ++count="<<m_value->m_count);
    }
}
Attribute &
Attribute::operator = (const Attribute &o)
{
  if (&o != this)
    {
      if (m_value != 0)
	{
	  m_value->m_count--;
	  NS_LOG_DEBUG ("this="<<m_value<<" --count="<<m_value->m_count);
	  if (m_value->m_count == 0)
	    {
	      delete m_value;
	      m_value = 0;
	    }
	}
      m_value = o.m_value;
      if (m_value != 0)
	{
	  m_value->m_count++;
	  NS_LOG_DEBUG ("this="<<m_value<<" ++count="<<m_value->m_count);
	}
    }
  return *this;
}
Attribute::~Attribute ()
{
  if (m_value != 0) 
    {
      m_value->m_count--;
      NS_LOG_DEBUG ("this="<<m_value<<" --count="<<m_value->m_count);
      if (m_value->m_count == 0)
	{
	  delete m_value;
	  m_value = 0;
	}
    }
}
Attribute::Attribute (AttributeValue *value)
  : m_value (value)
{
  NS_LOG_DEBUG ("this="<<m_value<<" count="<<((m_value!=0)?m_value->m_count:666));
}

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

std::string 
PtrValueBase::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << PeekObjectBase ();
  return oss.str ();
}

bool 
PtrValueBase::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  // XXX
  return false;
}

} // namespace ns3
