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
MakeSimpleAttributeChecker (std::string name, std::string underlying)
{
  struct SimpleAttributeChecker : public BASE
  {
    virtual bool Check (const AttributeValue &value) const {
      return dynamic_cast<const T *> (&value) != 0;
    }
    virtual std::string GetValueTypeName (void) const {
      return m_type;
    }
    virtual bool HasUnderlyingTypeInformation (void) const {
      return true;
    }
    virtual std::string GetUnderlyingTypeInformation (void) const {
      return m_underlying;
    }
    virtual Ptr<AttributeValue> Create (void) const {
      return ns3::Create<T> ();
    }
    virtual bool Copy (const AttributeValue &source, AttributeValue &destination) const {
      const T *src = dynamic_cast<const T *> (&source);
      T *dst = dynamic_cast<T *> (&destination);
      if (src == 0 || dst == 0)
        {
          return false;
        }
      *dst = *src;
      return true;
    }
    std::string m_type;
    std::string m_underlying;
  } *checker = new SimpleAttributeChecker ();
  checker->m_type = name;
  checker->m_underlying = underlying;
  return Ptr<AttributeChecker> (checker, false);
}

}

/**
 * \ingroup core
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
 *    - \ref ATTRIBUTE_HELPER_HEADER_1,
 *    - \ref ATTRIBUTE_HELPER_HEADER_2, and,
 *    - \ref ATTRIBUTE_HELPER_CPP,
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

#define ATTRIBUTE_VALUE_DEFINE_WITH_NAME(type,name)                     \
  class name##Value : public AttributeValue				\
  {									\
  public:								\
    name##Value ();							\
    name##Value (const type &value);					\
    void Set (const type &value);					\
    type Get (void) const;						\
    virtual Ptr<AttributeValue> Copy (void) const;                      \
    virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const; \
    virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker); \
  private:								\
    type m_value;							\
  };


/**
 * \ingroup AttributeHelper
 * \param type the name of the class.
 *
 * This macro defines the class XXXValue associated to class XXX.
 * This macro is typically invoked in a class header.
 */
#define ATTRIBUTE_VALUE_DEFINE(type)					\
  ATTRIBUTE_VALUE_DEFINE_WITH_NAME (type,type)


/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro defines the conversion operators for class XXX to and
 * from instances of type Attribute.
 * Typically invoked from xxx.h.
 */
#define ATTRIBUTE_CONVERTER_DEFINE(type)

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


#define ATTRIBUTE_VALUE_IMPLEMENT_WITH_NAME(type,name)                  \
  name##Value::name##Value ()						\
    : m_value () {}							\
  name##Value::name##Value (const type &value)				\
  : m_value (value) {}							\
  void name##Value::Set (const type &v) {				\
    m_value = v;							\
  }									\
  type name##Value::Get (void) const {					\
    return m_value;							\
  }									\
  Ptr<AttributeValue>                                                   \
  name##Value::Copy (void) const {					\
    return ns3::Create<name##Value> (*this);                            \
  }                                                                     \
  std::string								\
  name##Value::SerializeToString (Ptr<const AttributeChecker> checker) const { \
    std::ostringstream oss;						\
    oss << m_value;							\
    return oss.str ();							\
  }									\
  bool									\
  name##Value::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker) { \
    std::istringstream iss;						\
    iss.str (value);							\
    iss >> m_value;							\
    return !iss.bad () && !iss.fail ();					\
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
  ATTRIBUTE_VALUE_IMPLEMENT_WITH_NAME(type,type)


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
    return MakeSimpleAttributeChecker<type##Value,type##Checker> (#type "Value", #type); \
  }									\

#define ATTRIBUTE_CHECKER_IMPLEMENT_WITH_NAME(type,name)                    \
  Ptr<const AttributeChecker> Make##type##Checker (void)		\
  {									\
    return MakeSimpleAttributeChecker<type##Value,type##Checker> (#type "Value", name); \
  }									\


/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro implements the conversion operators to and from
 * instances of type Attribute. Typically invoked from xxx.cc.
 */
#define ATTRIBUTE_CONVERTER_IMPLEMENT(type)


/**
 * \ingroup AttributeHelper
 * \param type the name of the class
 *
 * This macro should be invoked from a public section of the class
 * declaration.
 */
#define ATTRIBUTE_HELPER_HEADER_1(type)

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
