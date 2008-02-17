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

class UintParamSpec : public ParamSpec {};

template <typename T1>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1);
template <typename T1>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1,
				 uint64_t minValue,
				 uint64_t maxValue);
template <typename T1, typename T2>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1, T2 a2);
template <typename T1, typename T2>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1, T2 a2,
				 uint64_t minValue,
				 uint64_t maxValue);

} // namespace ns3

namespace ns3 {

class UintValueChecker
{
public:
  UintValueChecker (uint64_t minValue, uint64_t maxValue);
  bool Check (const uint64_t &value) const;

  template <typename T, typename U>
  static UintValueChecker Create (U T::*) {
    return UintValueChecker (std::numeric_limits<U>::min (),
			    std::numeric_limits<U>::max ());
  }
  template <typename T, typename U>
  static UintValueChecker Create (U (T::*) (void) const) {
    return UintValueChecker (std::numeric_limits<U>::min (),
			    std::numeric_limits<U>::max ());
  }
  template <typename T, typename U>
  static UintValueChecker Create (void (T::*) (U)) {
    return UintValueChecker (std::numeric_limits<U>::min (),
			    std::numeric_limits<U>::max ());
  }
private:
  uint64_t m_minValue;
  uint64_t m_maxValue;
};

template <typename T1>
Ptr<ParamSpec> 
MakeUintParamSpec (T1 a1)
{
  return MakeParamSpecHelperWithChecker<UintParamSpec,UintValue> (a1,
							UintValueChecker::Create (a1));
}

template <typename T1>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1,
				 uint64_t minValue,
				 uint64_t maxValue)
{
  return MakeParamSpecHelperWithChecker<UintParamSpec,UintValue> (a1,
							UintValueChecker (minValue, maxValue));
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1, T2 a2)
{
  return MakeParamSpecHelperWithChecker<UintParamSpec,UintValue> (a1, a2,
							UintValueChecker::Create (a1));
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1, T2 a2,
				 uint64_t minValue,
				 uint64_t maxValue)
{
  return MakeParamSpecHelperWithChecker<UintParamSpec,UintValue> (a1, a2,
							UintValueChecker (minValue, maxValue));
}

} // namespace ns3

#endif /* UINT_PARAMETER_H */
