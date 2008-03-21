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
#ifndef ATTRIBUTE_HELPER_H
#define ATTRIBUTE_HELPER_H

#include "attribute.h"
#include "attribute-accessor-helper.h"
#include <sstream>
#include "fatal-error.h"

namespace ns3 {

template <typename T, typename BASE>
Ptr<AttributeChecker>
MakeSimpleAttributeChecker (std::string name)
{
  struct SimpleAttributeChecker : public BASE
  {
    virtual bool Check (Attribute value) const {
      return value.DynCast<const T *> () != 0;
    }
    virtual std::string GetType (void) const {
      return m_type;
    }
    virtual bool HasTypeConstraints (void) const {
      return false;
    }
    virtual std::string GetTypeConstraints (void) const {
      return "";
    }
    virtual Attribute Create (void) const {
      return Attribute::Create<T> ();
    }
    std::string m_type;
  } *checker = new SimpleAttributeChecker ();
  checker->m_type = name;
  return Ptr<AttributeChecker> (checker, false);
}

}

/**
 * \defgroup AttributeHelper Attribute Helper
 *
 * All these macros can be used to generate automatically the code
 * for subclasses of AttributeValue, AttributeAccessor, and, AttributeChecker,
 * which can be used to give attribute powers to a normal class. i.e.,
 * the user class can then effectively be made an attribute.
 *
 * There are two kinds of helper macros:
 *  1) The simple macros.
 *  2) The more complex macros.
 *
 * The simple macros are implemented in terms of the complex
 * macros and should generally be prefered over the complex macros:
 *    - ATTRIBUTE_HELPER_HEADER_1,
 *    - ATTRIBUTE_HELPER_HEADER_2, and,
 *    - ATTRIBUTE_HELPER_CPP,
 */

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro defines and generates the code for the implementation 
 * of the MakeXXXAccessor template functions. This macro is typically
 * invoked in a class header to allow users of this class to view and
 * use the template functions defined here. This macro is implemented
 * through the helper templates functions ns3::MakeAccessorHelper<>.
 */
#define ATTRIBUTE_ACCESSOR_DEFINE(type)					\
  template <typename T1>						\
  Ptr<const AttributeAccessor> Make##type##Accessor (T1 a1)		\
  {									\
    return MakeAccessorHelper<type##Value> (a1);			\
  }									\
  template <typename T1, typename T2>					\
  Ptr<const AttributeAccessor> Make##type##Accessor (T1 a1, T2 a2)	\
  {									\
    return MakeAccessorHelper<type##Value> (a1, a2);			\
  }

/**
 * \ingroup AttributeHelper
 * \param type the name of the class.
 *
 * This macro defines the class XXXValue associated to class XXX.
 * This macro is typically invoked in a class header.
 */
#define ATTRIBUTE_VALUE_DEFINE(type)					\
  class type##Value : public AttributeValue				\
  {									\
  public:								\
    type##Value ();							\
    type##Value (const type &value);					\
    void Set (const type &value);					\
    type Get (void) const;						\
    virtual Attribute Copy (void) const;				\
    virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const; \
    virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker); \
    type##Value (Attribute value);					\
    operator Attribute () const;					\
  private:								\
    type m_value;							\
  };

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro defines the conversion operators for class XXX to and
 * from instances of type Attribute.
 * Typically invoked from xxx.h.
 */
#define ATTRIBUTE_CONVERTER_DEFINE(type)	\
  type (Attribute value);			\
  operator Attribute () const;

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro defines the XXXChecker class and the associated
 * MakeXXXChecker function.
 * Typically invoked from xxx.h.
 */
#define ATTRIBUTE_CHECKER_DEFINE(type)				\
  class type##Checker : public AttributeChecker {};		\
  Ptr<const AttributeChecker> Make##type##Checker (void);	\

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro implements the XXXValue class (without the 
 * XXXValue::SerializeToString and XXXValue::DeserializeFromString 
 * methods).
 * Typically invoked from xxx.cc.
 */
#define ATTRIBUTE_VALUE_IMPLEMENT_NO_SERIALIZE(type)			\
  type##Value::type##Value ()						\
    : m_value () {}							\
  type##Value::type##Value (const type &value)				\
  : m_value (value) {}							\
  void type##Value::Set (const type &v) {				\
    m_value = v;							\
  }									\
  type type##Value::Get (void) const {					\
    return m_value;							\
  }									\
  Attribute								\
  type##Value::Copy (void) const {					\
    return Attribute::Create<type##Value> (*this);			\
  }									\
  type##Value::type##Value (Attribute value)				\
  {									\
    type##Value *v = value.DynCast<type##Value *> ();			\
    if (v == 0)								\
      {									\
	NS_FATAL_ERROR ("Unexpected type of value. Expected \"" << #type << "Value\""); \
      }									\
    m_value = v->Get ();						\
  }									\
  type##Value::operator Attribute () const				\
  {									\
    return Attribute::Create<type##Value> (*this);			\
  }

/**
 * \ingroup AttributeHelper
 * \param type the name of the class.
 *
 * This macro implements the XXXValue class (including the 
 * XXXValue::SerializeToString and XXXValue::DeserializeFromString 
 * methods).
 * Typically invoked from xxx.cc.
 */
#define ATTRIBUTE_VALUE_IMPLEMENT(type)					\
  std::string								\
  type##Value::SerializeToString (Ptr<const AttributeChecker> checker) const { \
    std::ostringstream oss;						\
    oss << m_value;							\
    return oss.str ();							\
  }									\
  bool									\
  type##Value::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker) { \
    std::istringstream iss;						\
    iss.str (value);							\
    iss >> m_value;							\
    return !iss.bad () && !iss.fail ();					\
  }									\
  ATTRIBUTE_VALUE_IMPLEMENT_NO_SERIALIZE (type)

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro implements the MakeXXXChecker function.
 * Typically invoked from xxx.cc.
 */
#define ATTRIBUTE_CHECKER_IMPLEMENT(type)				\
  Ptr<const AttributeChecker> Make##type##Checker (void)		\
  {									\
    return MakeSimpleAttributeChecker<type##Value,type##Checker> (#type);	\
  }									\

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro implements the conversion operators to and from
 * instances of type Attribute. Typically invoked from xxx.cc.
 */
#define ATTRIBUTE_CONVERTER_IMPLEMENT(type)				\
  type::type (Attribute value)						\
  {									\
    const type##Value *v = value.DynCast<const type##Value *> ();	\
    if (v == 0)								\
      {									\
      NS_FATAL_ERROR ("Unexpected type of value. Expected \"" << #type << "Value\""); \
      }									\
    *this = v->Get ();							\
  }									\
  type::operator Attribute () const					\
  {									\
    return Attribute::Create<type##Value> (*this);			\
  }


/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro should be invoked from a public section of the class
 * declaration.
 */
#define ATTRIBUTE_HELPER_HEADER_1(type) \
  ATTRIBUTE_CONVERTER_DEFINE (type)

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro should be invoked outside of the class
 * declaration in its public header.
 */
#define ATTRIBUTE_HELPER_HEADER_2(type)					\
  ATTRIBUTE_VALUE_DEFINE (type);					\
  ATTRIBUTE_ACCESSOR_DEFINE (type);					\
  ATTRIBUTE_CHECKER_DEFINE (type);

/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro should be invoked from the class implementation file.
 */
#define ATTRIBUTE_HELPER_CPP(type)                                      \
  ATTRIBUTE_CHECKER_IMPLEMENT (type);					\
  ATTRIBUTE_CONVERTER_IMPLEMENT (type);					\
  ATTRIBUTE_VALUE_IMPLEMENT (type);


#endif /* ATTRIBUTE_HELPER_H */
