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
#include "object-ptr-container.h"

namespace ns3 {

ObjectPtrContainerValue::ObjectPtrContainerValue ()
{
}

ObjectPtrContainerValue::Iterator
ObjectPtrContainerValue::Begin (void) const
{
  return m_objects.begin ();
}
ObjectPtrContainerValue::Iterator
ObjectPtrContainerValue::End (void) const
{
  return m_objects.end ();
}
uint32_t
ObjectPtrContainerValue::GetN (void) const
{
  return m_objects.size ();
}
Ptr<Object>
ObjectPtrContainerValue::Get (uint32_t i) const
{
  Iterator it = m_objects.find (i); 
  Ptr<Object> value = 0;
  if ( it != m_objects.end () )
  {
    value = m_objects.find (i)->second;
  }
  return value;
}

Ptr<AttributeValue>
ObjectPtrContainerValue::Copy (void) const
{
  return ns3::Create<ObjectPtrContainerValue> (*this);
}
std::string 
ObjectPtrContainerValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  Iterator it;
  for (it = Begin (); it != End (); ++it)
    {
      oss << (*it).second;
      if (it != End ())
        {
          oss << " ";
        }
    }
  return oss.str ();
}
bool 
ObjectPtrContainerValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  NS_FATAL_ERROR ("cannot deserialize a set of object pointers.");
  return true;
}

bool 
ObjectPtrContainerAccessor::Set (ObjectBase * object, const AttributeValue & value) const
{
  // not allowed.
  return false;
}
bool 
ObjectPtrContainerAccessor::Get (const ObjectBase * object, AttributeValue &value) const
{
  ObjectPtrContainerValue *v = dynamic_cast<ObjectPtrContainerValue *> (&value);
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
      uint32_t index;
      Ptr<Object> o = DoGet (object, i, &index);
      v->m_objects.insert (std::pair <uint32_t, Ptr<Object> > (index, o));
    }
  return true;
}
bool 
ObjectPtrContainerAccessor::HasGetter (void) const
{
  return true;
}
bool 
ObjectPtrContainerAccessor::HasSetter (void) const
{
  return false;
}

} // name
