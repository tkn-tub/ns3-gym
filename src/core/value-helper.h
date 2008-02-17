#ifndef VALUE_HELPER_H
#define VALUE_HELPER_H

#include "class-value-helper.h"

#define VALUE_HELPER_HEADER_1(type) \
  type (PValue value); \
  operator PValue () const;

#define VALUE_HELPER_HEADER_2(type)				\
  class type##Value : public Value {};				\
  class type##ParamSpec : public ParamSpec {};			\
  template <typename T1>					\
  Ptr<ParamSpec> Make##type##ParamSpec (T1 a1)			\
  {								\
    return MakeClassValueHelperParamSpec< type ,		\
      type##Value, type##ParamSpec> (a1);			\
  }								\
  template <typename T1, typename T2>				\
    Ptr<ParamSpec> Make##type##ParamSpec (T1 a1, T2 a2)	\
  {								\
    return MakeClassValueHelperParamSpec<type,			\
      type##Value,type##ParamSpec> (a1, a2);			\
  }

#define VALUE_HELPER_CPP(type)						\
  type::type (PValue value)						\
  {									\
    *this = ClassValueHelperExtractFrom<type,type##Value> (value);	\
  }									\
  type::operator PValue () const					\
  {									\
    return ClassValueHelperConvertTo<type,type##Value> (this);		\
  }

#endif /* VALUE_HELPER_H */
