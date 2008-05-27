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
#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "attribute-list.h"
#include "object.h"
#include "type-id.h"

namespace ns3 {

class AttributeValue;

/**
 * \brief instantiate subclasses of ns3::Object.
 *
 * This class can also hold a set of attributes to set
 * automatically during the object construction.
 */
class ObjectFactory
{
public:
  ObjectFactory ();

  /**
   * \param tid the TypeId of the object to instantiate.
   */
  void SetTypeId (TypeId tid);
  /**
   * \param tid the TypeId of the object to instantiate.
   */  
  void SetTypeId (const char *tid);
  /**
   * \param tid the TypeId of the object to instantiate.
   */  
  void SetTypeId (std::string tid);
  /**
   * \param name the name of the attribute to set during object construction
   * \param value the value of the attribute to set during object construction
   */
  void Set (std::string name, const AttributeValue &value);

  /**
   * \returns the currently-selected TypeId to use to create an object
   *          instance.
   */
  TypeId GetTypeId (void) const;

  /**
   * \returns a new object instance.
   */
  Ptr<Object> Create (void) const;
  /**
   * \returns a new object instance.
   *
   * This method performs an extra call to ns3::Object::GetObject before
   * returning a pointer of the requested type to the user. This method
   * is really syntactical sugar.
   */
  template <typename T>
  Ptr<T> Create (void) const;

private:
  friend std::ostream & operator << (std::ostream &os, const ObjectFactory &factory);
  friend std::istream & operator >> (std::istream &is, ObjectFactory &factory);

  TypeId m_tid;
  AttributeList m_parameters;
};

std::ostream & operator << (std::ostream &os, const ObjectFactory &factory);
std::istream & operator >> (std::istream &is, ObjectFactory &factory);

/**
 * \class ns3::ObjectFactoryValue
 * \brief hold objects of type ns3::ObjectFactory
 */

ATTRIBUTE_HELPER_HEADER (ObjectFactory);

} // namespace ns3

namespace ns3 {

template <typename T>
Ptr<T> 
ObjectFactory::Create (void) const
{
  Ptr<Object> object = Create ();
  return object->GetObject<T> ();
}

} // namespace ns3

#endif /* OBJECT_FACTORY_H */
