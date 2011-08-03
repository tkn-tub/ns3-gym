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

#include "attribute-construction-list.h"
#include "object.h"
#include "type-id.h"

namespace ns3 {

class AttributeValue;

/**
 * \ingroup object
 *
 * \brief instantiate subclasses of ns3::Object.
 *
 * This class can also hold a set of attributes to set
 * automatically during the object construction.
 */
class ObjectFactory
{
public:
  ObjectFactory ();
  ObjectFactory (std::string typeId);

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
  AttributeConstructionList m_parameters;
};

std::ostream & operator << (std::ostream &os, const ObjectFactory &factory);
std::istream & operator >> (std::istream &is, ObjectFactory &factory);


/**
 * \param n1 name of attribute
 * \param v1 value of attribute
 * \param n2 name of attribute
 * \param v2 value of attribute
 * \param n3 name of attribute
 * \param v3 value of attribute
 * \param n4 name of attribute
 * \param v4 value of attribute
 * \param n5 name of attribute
 * \param v5 value of attribute
 * \param n6 name of attribute
 * \param v6 value of attribute
 * \param n7 name of attribute
 * \param v7 value of attribute
 * \param n8 name of attribute
 * \param v8 value of attribute
 * \param n9 name of attribute
 * \param v9 value of attribute
 * \returns a pointer to a newly allocated object.
 *
 * This allocates an object on the heap and initializes
 * it with a set of attributes.
 */
template <typename T>
Ptr<T> 
CreateObjectWithAttributes (std::string n1 = "", const AttributeValue & v1 = EmptyAttributeValue (),
                            std::string n2 = "", const AttributeValue & v2 = EmptyAttributeValue (),
                            std::string n3 = "", const AttributeValue & v3 = EmptyAttributeValue (),
                            std::string n4 = "", const AttributeValue & v4 = EmptyAttributeValue (),
                            std::string n5 = "", const AttributeValue & v5 = EmptyAttributeValue (),
                            std::string n6 = "", const AttributeValue & v6 = EmptyAttributeValue (),
                            std::string n7 = "", const AttributeValue & v7 = EmptyAttributeValue (),
                            std::string n8 = "", const AttributeValue & v8 = EmptyAttributeValue (),
                            std::string n9 = "", const AttributeValue & v9 = EmptyAttributeValue ());




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

template <typename T>
Ptr<T> 
CreateObjectWithAttributes (std::string n1, const AttributeValue & v1,
                            std::string n2, const AttributeValue & v2,
                            std::string n3, const AttributeValue & v3,
                            std::string n4, const AttributeValue & v4,
                            std::string n5, const AttributeValue & v5,
                            std::string n6, const AttributeValue & v6,
                            std::string n7, const AttributeValue & v7,
                            std::string n8, const AttributeValue & v8,
                            std::string n9, const AttributeValue & v9)
{
  ObjectFactory factory;
  factory.SetTypeId (T::GetTypeId ());
  factory.Set(n1, v1);
  factory.Set(n2, v2);
  factory.Set(n3, v3);
  factory.Set(n4, v4);
  factory.Set(n5, v5);
  factory.Set(n6, v6);
  factory.Set(n7, v7);
  factory.Set(n8, v8);
  factory.Set(n9, v9);
  return factory.Create<T> ();
}


} // namespace ns3

#endif /* OBJECT_FACTORY_H */
