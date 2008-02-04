#ifndef UINT_VALUE_H
#define UINT_VALUE_H

#include "value.h"
#include "param-spec-helper.h"
#include <stdint.h>

namespace ns3 {

class UintValue : public Value
{
public:
  UintValue (uint64_t value);
  virtual PValue Copy (void) const;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);

  void Set (uint64_t value);
  uint64_t Get (void) const;

  UintValue (PValue value);
  operator PValue () const;
private:
  uint64_t m_value;
};

template <typename U, typename T>
Ptr<const ParamSpec> MakeUintParamSpec (uint64_t initialValue,
					U T::*memberVariable);
template <typename U, typename T>
Ptr<const ParamSpec> MakePtrUintParamSpec (uint64_t initialValue,
					   uint64_t minValue,
					   uint64_t maxValue,
					   U T::*memberVariable);
template <typename U, typename T>
Ptr<const ParamSpec> MakeUintParamSpec (uint64_t initialValue,
					void (T::*setter) (U),
					U (T::*getter) (void) const);
template <typename U, typename T>
Ptr<const ParamSpec> MakeUintParamSpec (uint64_t initialValue,
					uint64_t minValue,
					uint64_t maxValue,
					void (T::*setter) (U),
					U (T::*getter) (void) const);

} // namespace ns3

namespace ns3 {

class UintValueChecker
{
public:
  UintValueChecker (uint64_t minValue, uint64_t maxValue);
  bool Check (const uint64_t &value) const;
private:
  uint64_t m_minValue;
  uint64_t m_maxValue;
};

template <typename U, typename T>
Ptr<const ParamSpec> 
MakeUintParamSpec (U T::*memberVariable,
		   uint64_t initialValue)
{
  uint64_t minValue = std::numeric_limits<U>::min ();
  uint64_t maxValue = std::numeric_limits<U>::max ();
  return MakeMemberVariableParamSpecWithChecker (memberVariable, UintValue (initialValue),
						 UintValueChecker (minValue, maxValue));
}

template <typename U, typename T>
Ptr<const ParamSpec> 
MakeUintParamSpec (U T::*memberVariable,
		   uint64_t initialValue,
		   uint64_t minValue,
		   uint64_t maxValue)
{
  return MakeMemberVariableParamSpecWithChecker (memberVariable, UintValue (initialValue),
						 UintValueChecker (minValue, maxValue));
}

template <typename U, typename T>
Ptr<const ParamSpec> 
MakeUintParamSpec (void (T::*setter) (U),
		   U (T::*getter) (void) const,
		   uint64_t initialValue)
{
  uint64_t minValue = std::numeric_limits<U>::min ();
  uint64_t maxValue = std::numeric_limits<U>::max ();
  return MakeMemberMethodParamSpecWithChecker (setter, getter, UintValue (initialValue),
					       UintValueChecker (minValue, maxValue));
}
template <typename U, typename T>
Ptr<const ParamSpec> 
MakeUintParamSpec (void (T::*setter) (U),
		   U (T::*getter) (void) const,
		   uint64_t initialValue,
		   uint64_t minValue,
		   uint64_t maxValue)
{
  return MakeMemberMethodParamSpecWithChecker (setter, getter, UintValue (initialValue),
					       UintValueChecker (minValue, maxValue));
}


} // namespace ns3

#endif /* UINT_PARAMETER_H */
