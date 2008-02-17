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
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

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
template <typename T1, typename T2>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1, T2 a2);

template <typename T>
Ptr<AttributeChecker> MakeUintChecker (void);
template <typename T>
Ptr<AttributeChecker> MakeUintChecker (T min, T max);

} // namespace ns3

namespace ns3 {

template <typename T1>
Ptr<ParamSpec> 
MakeUintParamSpec (T1 a1)
{
  return MakeParamSpecHelper<UintParamSpec,UintValue> (a1);
}

template <typename T1, typename T2>
Ptr<ParamSpec> MakeUintParamSpec (T1 a1, T2 a2)
{
  return MakeParamSpecHelper<UintParamSpec,UintValue> (a1, a2);
}

template <typename T>
Ptr<AttributeChecker> MakeUintChecker (void)
{
  return MakeUintChecker (std::numeric_limits<T>::min (),
			  std::numeric_limits<T>::max ());
}
template <typename T>
Ptr<AttributeChecker> MakeUintChecker (T min, T max)
{
  struct Checker : public AttributeChecker
  {
    Checker (uint64_t minValue, uint64_t maxValue)
      : m_minValue (minValue),
      m_maxValue (maxValue) {}
    virtual bool Check (PValue value) const {
      const UintValue *v = value.DynCast<const UintValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get () >= m_minValue && v->Get () <= m_maxValue;
    }
    uint64_t m_minValue;
    uint64_t m_maxValue;
  } *checker = new Checker (min, max);
  return Ptr<AttributeChecker> (checker, false);
}

} // namespace ns3

#endif /* UINT_PARAMETER_H */
