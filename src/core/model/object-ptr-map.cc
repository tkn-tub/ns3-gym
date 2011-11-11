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
 * Authors: Jaume Nin <jaume.nin@cttc.cat>,  Mathieu Lacage <mathieu.lacage@gmail.com>
 */
#include "object-ptr-map.h"

namespace ns3
{

ObjectPtrMapValue::ObjectPtrMapValue ()
{
}

ObjectPtrMapValue::Iterator
ObjectPtrMapValue::Begin (void) const
{
  return m_objects.begin();
}
ObjectPtrMapValue::Iterator
ObjectPtrMapValue::End (void) const
{
  return m_objects.end();
}
uint32_t
ObjectPtrMapValue::GetN (void) const
{
  return m_objects.size();
}
Ptr<Object>
ObjectPtrMapValue::Get (uint32_t i) const
{
  return m_objects.find(i)->second;
}

Ptr<AttributeValue>
ObjectPtrMapValue::Copy (void) const
{
  return ns3::Create<ObjectPtrMapValue>(*this);
}
std::string
ObjectPtrMapValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  Iterator it;
  for (it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    oss << (*it).second;
    if (it != m_objects.end())
    {
      oss << " ";
    }
  }
  return oss.str();
}

bool
ObjectPtrMapValue::DeserializeFromString (std::string value,
    Ptr<const AttributeChecker> checker)
{
  NS_FATAL_ERROR("cannot deserialize a map of object pointers.");
  return true;
}

bool
ObjectPtrMapAccessor::Set (ObjectBase * object, const AttributeValue & value) const
{
  // not allowed.
  return false;
}
bool
ObjectPtrMapAccessor::Get (const ObjectBase * object, AttributeValue &value) const
{
  ObjectPtrMapValue *v = dynamic_cast<ObjectPtrMapValue *> (&value);
  if (v == 0)
  {
    return false;
  }
  v->m_objects.clear();
  uint32_t n;
  bool ok = DoGetN(object, &n);
  if (!ok)
  {
    return false;
  }
  for (uint32_t i = 0; i < n; i++)
  {

    uint32_t k;
    Ptr < Object > element = DoGet(object, i, &k);
    v->m_objects.insert(std::pair<uint32_t, Ptr<Object> >(k, element));
  }
  return true;
}

bool
ObjectPtrMapAccessor::HasGetter (void) const
{
  return true;
}
bool
ObjectPtrMapAccessor::HasSetter (void) const
{
  return false;
}

} // name
