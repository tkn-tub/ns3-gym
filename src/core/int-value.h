#ifndef INT_VALUE_H
#define INT_VALUE_H

#include "value.h"
#include "param-spec-helper.h"
#include <stdint.h>

namespace ns3 {

class IntValue : public Value
{
public:
  IntValue (int64_t value);
  void Set (int64_t value);
  int64_t Get (void) const;

  virtual PValue Copy (void) const;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);

  IntValue (PValue value);
  operator PValue () const;
private:
  int64_t m_value;
};

template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (U T::*memberVariable,
				 int64_t initialValue);
template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (U T::*memberVariable,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue);
template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (void (T::*setter) (U),
				 U (T::*getter) (void) const,
				 int64_t initialValue);
template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (void (T::*setter) (U),
				 U (T::*getter) (void) const,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue);

} // namespace ns3

namespace ns3 {

class IntValueChecker
{
public:
  IntValueChecker (int64_t minValue, int64_t maxValue);
  bool Check (const int64_t &value) const;
private:
  int64_t m_minValue;
  int64_t m_maxValue;
};


template <typename U, typename T>
Ptr<ParamSpec> 
MakeIntParamSpec (U T::*memberVariable,
		  int64_t initialValue)
{
  int64_t minValue = std::numeric_limits<U>::min ();
  int64_t maxValue = std::numeric_limits<U>::max ();
  return MakeMemberVariableParamSpecWithChecker (memberVariable, IntValue (initialValue),
						 IntValueChecker (minValue, maxValue));
}

template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (U T::*memberVariable,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue)
{
  return MakeMemberVariableParamSpecWithChecker (memberVariable, IntValue (initialValue),
						 IntValueChecker (minValue, maxValue));
}
template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (void (T::*setter) (U),
				 U (T::*getter) (void) const,
				 int64_t initialValue)
{
  int64_t minValue = std::numeric_limits<U>::min ();
  int64_t maxValue = std::numeric_limits<U>::max ();
  return MakeMemberMethodParamSpecWithChecker (setter, getter, IntValue (initialValue),
					       IntValueChecker (minValue, maxValue));
}
template <typename U, typename T>
Ptr<ParamSpec> MakeIntParamSpec (void (T::*setter) (U),
				 U (T::*getter) (void) const,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue)
{
  return MakeMemberMethodParamSpecWithChecker (setter, getter, IntValue (initialValue),
					       IntValueChecker (minValue, maxValue));
}


} // namespace ns3

#endif /* INT_VALUE_H */
