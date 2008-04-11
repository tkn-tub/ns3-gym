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
#include "object-factory.h"
#include <sstream>

namespace ns3 {

ObjectFactory::ObjectFactory ()
{}

void 
ObjectFactory::SetTypeId (TypeId tid)
{
  m_tid = tid;
}
void 
ObjectFactory::SetTypeId (std::string tid)
{
  m_tid = TypeId::LookupByName (tid);
}
void 
ObjectFactory::SetTypeId (const char *tid)
{
  m_tid = TypeId::LookupByName (tid);
}
void 
ObjectFactory::Set (std::string name, Attribute value)
{
  if (name == "")
    {
      return;
    }
  m_parameters.SetWithTid (m_tid, name, value);
}

TypeId 
ObjectFactory::GetTypeId (void) const
{
  return m_tid;
}

Ptr<Object> 
ObjectFactory::Create (void) const
{
  Callback<ObjectBase *> cb = m_tid.GetConstructor ();
  ObjectBase *base = cb ();
  Object *derived = dynamic_cast<Object *> (base);
  derived->SetTypeId (m_tid);
  derived->Construct (m_parameters);
  Ptr<Object> object = Ptr<Object> (derived, false);
  return object;
}

std::ostream & operator << (std::ostream &os, const ObjectFactory &factory)
{
  os << factory.m_tid.GetName () << "[" << factory.m_parameters.SerializeToString () << "]";
  return os;
}
std::istream & operator >> (std::istream &is, ObjectFactory &factory)
{
  // XXX
  return is;
}


ATTRIBUTE_HELPER_CPP (ObjectFactory);

} // namespace ns3
