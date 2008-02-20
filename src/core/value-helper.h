#ifndef VALUE_HELPER_H
#define VALUE_HELPER_H

#include "class-value-helper.h"

#define VALUE_HELPER_HEADER_1(type) \
  type (Attribute value); \
  operator Attribute () const;

#define VALUE_HELPER_HEADER_2(type)					\
  class type##Value : public AttributeValue {};				\
  class type##Accessor : public AttributeAccessor {};			\
  Ptr<const AttributeChecker> Make##type##Checker (void);		\
  template <typename T1>						\
  Ptr<const AttributeAccessor> Make##type##Accessor (T1 a1)		\
  {									\
    return MakeClassValueHelperAccessor< type ,				\
      type##Value, type##Accessor> (a1);				\
  }									\
  template <typename T1, typename T2>					\
  Ptr<const AttributeAccessor> Make##type##Accessor (T1 a1, T2 a2)	\
  {									\
    return MakeClassValueHelperAccessor<type,				\
      type##Value,type##Accessor> (a1, a2);				\
  }

#define VALUE_HELPER_CPP(type)						\
  Ptr<const AttributeChecker> Make##type##Checker (void)		\
  {									\
    return MakeSimpleAttributeChecker<type> ();				\
  }									\
  type::type (Attribute value)						\
  {									\
    *this = ClassValueHelperExtractFrom<type,type##Value> (value);	\
  }									\
  type::operator Attribute () const					\
  {									\
    return ClassValueHelperConvertTo<type,type##Value> (this);		\
  }

#endif /* VALUE_HELPER_H */
