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

class IntParamSpec : public ParamSpec {};

template <typename T1>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1,
				 int64_t initialValue);
template <typename T1>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue);
template <typename T1, typename T2>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1, T2 a2,
				 int64_t initialValue);
template <typename T1, typename T2>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1, T2 a2,
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

  template <typename T, typename U>
  static IntValueChecker Create (U T::*) {
    return IntValueChecker (std::numeric_limits<U>::min (),
			    std::numeric_limits<U>::max ());
  }
  template <typename T, typename U>
  static IntValueChecker Create (U (T::*) (void) const) {
    return IntValueChecker (std::numeric_limits<U>::min (),
			    std::numeric_limits<U>::max ());
  }
  template <typename T, typename U>
  static IntValueChecker Create (void (T::*) (U)) {
    return IntValueChecker (std::numeric_limits<U>::min (),
			    std::numeric_limits<U>::max ());
  }
private:
  int64_t m_minValue;
  int64_t m_maxValue;
};

template <typename T1>
Ptr<ParamSpec> 
MakeIntParamSpec (T1 a1,
		  int64_t initialValue)
{
  return MakeParamSpecHelperWithChecker<IntParamSpec> (a1, IntValue (initialValue),
						       IntValueChecker::Create (a1));
}

template <typename T1>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue)
{
  return MakeParamSpecHelperWithChecker<IntParamSpec> (a1, IntValue (initialValue),
						       IntValueChecker (minValue, maxValue));
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1, T2 a2, 
				 int64_t initialValue)
{
  return MakeParamSpecHelperWithChecker<IntParamSpec> (a1, a2, IntValue (initialValue),
						       IntValueChecker::Create (a1));
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1, T2 a2,
				 int64_t initialValue,
				 int64_t minValue,
				 int64_t maxValue)
{
  return MakeParamSpecHelperWithChecker<IntParamSpec> (a1, a2, IntValue (initialValue),
						       IntValueChecker (minValue, maxValue));
}

} // namespace ns3

#endif /* INT_VALUE_H */
