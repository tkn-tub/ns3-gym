#ifndef FP_VALUE_H
#define FP_VALUE_H

#include "value.h"
#include "param-spec-helper.h"
#include <stdint.h>

namespace ns3 {

class FpValue : public Value
{
public:
  FpValue (double value);

  virtual PValue Copy (void) const;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);

  void Set (double value);
  double Get (void) const;

  FpValue (PValue value);
  operator PValue () const;
private:
  double m_value;
};

class FpParamSpec : public ParamSpec {};

template <typename T1>
Ptr<ParamSpec> MakeFpParamSpec (T1 a1);
template <typename T1, typename T2>
Ptr<ParamSpec> MakeFpParamSpec (T1 a1, T2 a2,
				double minValue,
				double maxValue);

} // namespace ns3

namespace ns3 {

class FpValueChecker
{
public:
  FpValueChecker (double minValue, double maxValue);
  bool Check (double value) const;

  template <typename T, typename U>
  static FpValueChecker Create (U T::*) {
    return FpValueChecker (-std::numeric_limits<U>::max (),
			   std::numeric_limits<U>::max ());
  }
  template <typename T, typename U>
  static FpValueChecker Create (U (T::*) (void) const) {
    return FpValueChecker (-std::numeric_limits<U>::max (),
			   std::numeric_limits<U>::max ());
  }
  template <typename T, typename U>
  static FpValueChecker Create (void (T::*) (U)) {
    return FpValueChecker (-std::numeric_limits<U>::max (),
			   std::numeric_limits<U>::max ());
  }
private:
  double m_min;
  double m_max;
};

template <typename T1>
Ptr<ParamSpec> 
MakeFpParamSpec (T1 a1)
{
  return MakeParamSpecHelperWithChecker<FpParamSpec,FpValue> (a1,
						      FpValueChecker::Create (a1));
}

template <typename T1>
Ptr<ParamSpec> MakeFpParamSpec (T1 a1,
				double minValue,
				double maxValue)
{
  return MakeParamSpecHelperWithChecker<FpParamSpec,FpValue> (a1,
						      FpValueChecker (minValue, maxValue));
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeFpParamSpec (T1 a1, T2 a2)
{
  return MakeParamSpecHelperWithChecker<FpParamSpec,FpValue> (a1, a2,
						      FpValueChecker::Create (a1));
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeFpParamSpec (T1 a1, T2 a2,
				double minValue,
				double maxValue)
{
  return MakeParamSpecHelperWithChecker<FpParamSpec,FpValue> (a1, a2,
						      FpValueChecker (minValue, maxValue));
}

} // namespace ns3


#endif /* FP_VALUE_H */
