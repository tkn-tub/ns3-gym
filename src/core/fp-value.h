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


template <typename U, typename T>
Ptr<ParamSpec> MakeFpParamSpec (U T::*memberVariable,
				double initialValue);
template <typename U, typename T>
Ptr<ParamSpec> MakeFpParamSpec (U T::*memberVariable,
				double initialValue,
				double minValue,
				double maxValue);

} // namespace ns3

namespace ns3 {

class FpValueChecker
{
public:
  FpValueChecker (double min, double max);
  bool Check (double v) const;
private:
  double m_min;
  double m_max;
};

template <typename U, typename T>
Ptr<ParamSpec>
MakeFpParamSpec (U T::*memberVariable,
		 double initialValue)
{
  double minValue = -std::numeric_limits<U>::max ();
  double maxValue = std::numeric_limits<U>::max ();
  return MakeMemberVariableParamSpecWithChecker (memberVariable, 
						 FpValue (initialValue), 
						 FpValueChecker (minValue, maxValue));
}

template <typename U, typename T>
Ptr<ParamSpec>
MakeFpParamSpec (U T::*memberVariable,
		 double initialValue,
		 double minValue,
		 double maxValue)
{
  return MakeMemberVariableParamSpecWithChecker (memberVariable, 
						 FpValue (initialValue), 
						 FpValueChecker (minValue, maxValue));
}

} // namespace ns3

#endif /* FP_VALUE_H */
