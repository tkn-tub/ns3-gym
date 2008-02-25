#ifndef VALUE_HELPER_H
#define VALUE_HELPER_H

#include "attribute-accessor-helper.h"
#include <sstream>
#include "fatal-error.h"

#define ATTRIBUTE_ACCESSOR_DEFINE(type)					\
  template <typename T1>						\
  Ptr<const AttributeAccessor> Make##type##Accessor (T1 a1)		\
  {									\
    return MakeAccessorHelper<type##Value> (a1);		\
  }									\
  template <typename T1, typename T2>					\
  Ptr<const AttributeAccessor> Make##type##Accessor (T1 a1, T2 a2)	\
  {									\
    return MakeAccessorHelper<type##Value> (a1, a2);	\
  }

#define ATTRIBUTE_VALUE_DEFINE(type)					\
  class type##Value : public AttributeValue				\
  {									\
  public:								\
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

#define ATTRIBUTE_CONVERTER_DEFINE(type)	\
  type (Attribute value);			\
  operator Attribute () const;

#define ATTRIBUTE_CHECKER_DEFINE(type)				\
  class type##Checker : public AttributeChecker {};		\
  Ptr<const AttributeChecker> Make##type##Checker (void);	\

#define ATTRIBUTE_VALUE_IMPLEMENT(type)					\
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

#define ATTRIBUTE_CHECKER_IMPLEMENT(type)				\
  Ptr<const AttributeChecker> Make##type##Checker (void)		\
  {									\
    return MakeSimpleAttributeChecker<type##Value,type##Checker> ();	\
  }									\

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


#define VALUE_HELPER_HEADER_1(type) \
  ATTRIBUTE_CONVERTER_DEFINE (type)

#define VALUE_HELPER_HEADER_2(type)					\
  ATTRIBUTE_VALUE_DEFINE (type);					\
  ATTRIBUTE_ACCESSOR_DEFINE (type);					\
  ATTRIBUTE_CHECKER_DEFINE (type);

#define VALUE_HELPER_CPP(type)						\
  ATTRIBUTE_CHECKER_IMPLEMENT (type);					\
  ATTRIBUTE_CONVERTER_IMPLEMENT (type);					\
  ATTRIBUTE_VALUE_IMPLEMENT (type);



#endif /* VALUE_HELPER_H */
