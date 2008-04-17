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
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <stdint.h>
#include "ptr.h"
#include "ref-count-base.h"

namespace ns3 {

class AttributeAccessor;
class AttributeChecker;
class Attribute;
class ObjectBase;

/**
 * \brief Hold a value for an Attribute.
 *
 * Instances of this class should always be wrapped into an Attribute object.
 * Most subclasses of this base class are implemented by the 
 * ATTRIBUTE_HELPER_* macros.
 */
class AttributeValue : public RefCountBase
{
public:
  AttributeValue ();
  virtual ~AttributeValue ();

  /**
   * \returns a deep copy of this class, wrapped into an Attribute object.
   */
  virtual Ptr<AttributeValue> Copy (void) const = 0;
  /**
   * \param checker the checker associated to the attribute
   * \returns a string representation of this value.
   *
   * In most cases, this method will not make any use of the checker argument.
   * However, in a very limited set of cases, the checker argument is needed to
   * perform proper serialization. A nice example of code which needs it is
   * the EnumValue::SerializeToString code.
   */
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const = 0;
  /**
   * \param value a string representation of the value
   * \param checker a pointer to the checker associated to the attribute.
   * \returns true if the input string was correctly-formatted and could be
   *          successfully deserialized, false otherwise.
   *
   * Upon return of this function, this AttributeValue instance contains
   * the deserialized value.
   * In most cases, this method will not make any use of the checker argument.
   * However, in a very limited set of cases, the checker argument is needed to
   * perform proper serialization. A nice example of code which needs it is
   * the EnumValue::SerializeToString code.
   */
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker) = 0;
};

/**
 * \brief allow setting and getting the value of an attribute.
 *
 * The goal of this class is to hide from the user how an attribute
 * is actually set or get to or from a class instance. Implementations
 * of this base class are usually provided through the MakeAccessorHelper
 * template functions, hidden behind an ATTRIBUTE_HELPER_* macro.
 */
class AttributeAccessor : public RefCountBase
{
public:
  AttributeAccessor ();
  virtual ~AttributeAccessor ();

  /**
   * \param object the object instance to set the value in
   * \param value the value to set
   * \returns true if the value could be set successfully, false otherwise.
   *
   * This method expects that the caller has checked that the input value is
   * valid with AttributeChecker::Check.
   */
  virtual bool Set (ObjectBase * object, const AttributeValue &value) const = 0;
  /**
   * \param object the object instance to get the value from
   * \param attribute a pointer to where the value should be set.
   * \returns true if the value could be read successfully, and
   *          stored in the input value, false otherwise.
   *
   * This method expects that the caller has checked that the input value is
   * valid with AttributeChecker::Check.
   */
  virtual bool Get (const ObjectBase * object, AttributeValue &attribute) const = 0;

  /**
   * \return true if this accessor supports the Get operation, false
   *         otherwise.
   */
  virtual bool HasGetter (void) const = 0;
  /**
   * \return true if this accessor supports the Set operation, false
   *         otherwise.
   */
  virtual bool HasSetter (void) const = 0;
};

/**
 * \brief Represent the type of an attribute
 *
 * Each type of attribute has an associated unique AttributeChecker
 * subclass. The type of the subclass can be safely used by users
 * to infer the type of the associated attribute. i.e., we expect
 * binding authors to use the checker associated to an attribute
 * to detect the type of the associated attribute.
 *
 * Most subclasses of this base class are implemented by the 
 * ATTRIBUTE_HELPER_* macros.
 */
class AttributeChecker : public RefCountBase
{
public:
  AttributeChecker ();
  virtual ~AttributeChecker ();
  /**
   * \param value a pointer to the value to check
   * \returns true if the input value is both of the right type
   *          and if its value is within the requested range. Returns
   *          false otherwise.
   */
  virtual bool Check (const AttributeValue &value) const = 0;
  virtual std::string GetType (void) const = 0;
  virtual bool HasTypeConstraints (void) const = 0;
  virtual std::string GetTypeConstraints (void) const = 0;
  /**
   * \returns a new instance of an AttributeValue (wrapper in an Attribute 
   *          instance) which matches the type of the underlying attribute.
   *
   * This method is typically used to create a temporary variable prior
   * to calling Attribute::DeserializeFromString.
   */
  virtual Ptr<AttributeValue> Create (void) const = 0;

  virtual bool Copy (const AttributeValue &source, AttributeValue &destination) const = 0;

};

class EmptyAttributeValue : public AttributeValue
{
public:
  EmptyAttributeValue ();
private:
  virtual Ptr<AttributeValue> Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);
};

} // namespace ns3

#endif /* ATTRIBUTE_H */
