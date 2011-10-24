/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, Mathieu Lacage
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
 * Authors: Mathieu Lacage <mathieu.lacage@gmail.com>
 */
#include "object-ptr-vector.h"

namespace ns3 {

ObjectPtrVectorValue::ObjectPtrVectorValue ()
{
}

ObjectPtrVectorValue::Iterator
ObjectPtrVectorValue::Begin (void) const
{
  return m_objects.begin ();
}
ObjectPtrVectorValue::Iterator
ObjectPtrVectorValue::End (void) const
{
  return m_objects.end ();
}
uint32_t
ObjectPtrVectorValue::GetN (void) const
{
  return m_objects.size ();
}
Ptr<Object>
ObjectPtrVectorValue::Get (uint32_t i) const
{
  return m_objects[i];
}

Ptr<AttributeValue>
ObjectPtrVectorValue::Copy (void) const
{
  return ns3::Create<ObjectPtrVectorValue> (*this);
}
std::string 
ObjectPtrVectorValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  for (uint32_t i = 0; i < m_objects.size (); ++i)
    {
      oss << m_objects[i];
      if (i != m_objects.size () - 1)
        {
          oss << " ";
        }
    }
  return oss.str ();
}
bool 
ObjectPtrVectorValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  NS_FATAL_ERROR ("cannot deserialize a vector of object pointers.");
  return true;
}

bool 
ObjectPtrVectorAccessor::Set (ObjectBase * object, const AttributeValue & value) const
{
  // not allowed.
  return false;
}
bool 
ObjectPtrVectorAccessor::Get (const ObjectBase * object, AttributeValue &value) const
{
  ObjectPtrVectorValue *v = dynamic_cast<ObjectPtrVectorValue *> (&value);
  if (v == 0)
    {
      return false;
    }
  v->m_objects.clear ();
  uint32_t n;
  bool ok = DoGetN (object, &n);
  if (!ok)
    {
      return false;
    }
  for (uint32_t i = 0; i < n; i++)
    {
      Ptr<Object> o = DoGet (object, i);
      v->m_objects.push_back (o);
    }
  return true;
}
bool 
ObjectPtrVectorAccessor::HasGetter (void) const
{
  return true;
}
bool 
ObjectPtrVectorAccessor::HasSetter (void) const
{
  return false;
}

} // name
