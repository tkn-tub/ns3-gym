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

/**
 * \file
 * \ingroup object
 * ns3::ObjectFactory class declaration.
 */

namespace ns3 {

class AttributeValue;

/**
 * \ingroup object
 *
 * \brief Instantiate subclasses of ns3::Object.
 *
 * This class can also hold a set of attributes to set
 * automatically during the object construction.
 *
 * \see attribute_ObjectFactory
 */
class ObjectFactory
{
public:
  /**
   * Default constructor.
   *
   * This factory is not capable of constructing a real Object
   * until it has at least a TypeId.
   */
  ObjectFactory ();
  /**
   * Construct a factory for a specific TypeId by name.
   *
   * \param [in] typeId The name of the TypeId this factory should create.
   */
  ObjectFactory (std::string typeId);

  /**@{*/
  /**
   * Set the TypeId of the Objects to be created by this factory.
   *
   * \param [in] tid The TypeId of the object to instantiate.
   */
  void SetTypeId (TypeId tid);
  void SetTypeId (const char *tid);
  void SetTypeId (std::string tid);
  /**@}*/
  
  /**
   * Set an attribute to be set during construction.
   *
   * \param [in] name The name of the attribute to set.
   * \param [in] value The value of the attribute to set.
   */
  void Set (std::string name, const AttributeValue &value);

  /**
   * Get the TypeId which will be created by this ObjectFactory.
   * \returns The currently-selected TypeId.
   */
  TypeId GetTypeId (void) const;

  /**
   * Create an Object instance of the configured TypeId.
   *
   * \returns A new object instance.
   */
  Ptr<Object> Create (void) const;
  /**
   * Create an Object instance of the requested type.
   *
   * This method performs an extra call to ns3::Object::GetObject before
   * returning a pointer of the requested type to the user. This method
   * is really syntactical sugar.
   *
   * \tparam T \explicit The requested Object type.
   * \returns A new object instance.
   */
  template <typename T>
  Ptr<T> Create (void) const;

private:
  /**
   * Print the factory configuration on an output stream.
   *
   * The configuration will be printed as a string with the form
   * "<TypeId-name>[<attribute-name>=<attribute-value>|...]"
   *
   * \param [in,out] os The stream.
   * \param [in] factory The ObjectFactory.
   * \returns The stream.
   */
  friend std::ostream & operator << (std::ostream &os, const ObjectFactory &factory);
  /**
   * Read a factory configuration from an input stream.
   *
   * The configuration should be in the form
   * "<TypeId-name>[<attribute-name>=<attribute-value>|...]"
   *
   * \param [in,out] is The input stream.
   * \param [out] factory The factory to configure as described by the stream.
   * \return The stream.
   */
  friend std::istream & operator >> (std::istream &is, ObjectFactory &factory);

  /** The TypeId this factory will create. */
  TypeId m_tid;
  /**
   * The list of attributes and values to be used in constructing
   * objects by this factory.
   */
  AttributeConstructionList m_parameters;  
};

std::ostream & operator << (std::ostream &os, const ObjectFactory &factory);
std::istream & operator >> (std::istream &is, ObjectFactory &factory);


/**
 * \ingroup object
 * Allocate an Object on the heap and initialize with a set of attributes.
 *
 * \tparam T \explicit The requested Object type.
 * \param [in] n1 Name of attribute
 * \param [in] v1 Value of attribute
 * \param [in] n2 Name of attribute
 * \param [in] v2 Value of attribute
 * \param [in] n3 Name of attribute
 * \param [in] v3 Value of attribute
 * \param [in] n4 Name of attribute
 * \param [in] v4 Value of attribute
 * \param [in] n5 Name of attribute
 * \param [in] v5 Value of attribute
 * \param [in] n6 Name of attribute
 * \param [in] v6 Value of attribute
 * \param [in] n7 Name of attribute
 * \param [in] v7 Value of attribute
 * \param [in] n8 Name of attribute
 * \param [in] v8 Value of attribute
 * \param [in] n9 Name of attribute
 * \param [in] v9 Value of attribute
 * \returns A pointer to a newly allocated object.
 */
template <typename T>
Ptr<T> 
CreateObjectWithAttributes
  (std::string n1 = "", const AttributeValue & v1 = EmptyAttributeValue (),
   std::string n2 = "", const AttributeValue & v2 = EmptyAttributeValue (),
   std::string n3 = "", const AttributeValue & v3 = EmptyAttributeValue (),
   std::string n4 = "", const AttributeValue & v4 = EmptyAttributeValue (),
   std::string n5 = "", const AttributeValue & v5 = EmptyAttributeValue (),
   std::string n6 = "", const AttributeValue & v6 = EmptyAttributeValue (),
   std::string n7 = "", const AttributeValue & v7 = EmptyAttributeValue (),
   std::string n8 = "", const AttributeValue & v8 = EmptyAttributeValue (),
   std::string n9 = "", const AttributeValue & v9 = EmptyAttributeValue ()
   );


ATTRIBUTE_HELPER_HEADER (ObjectFactory);

} // namespace ns3


/***************************************************************
 *  Implementation of the templates declared above.
 ***************************************************************/

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
