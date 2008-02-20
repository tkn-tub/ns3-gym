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

  virtual Attribute Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  IntValue (Attribute value);
  operator Attribute () const;
private:
  int64_t m_value;
};

class IntParamSpec : public ParamSpec {};

template <typename T1>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1);
template <typename T1, typename T2>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1, T2 a2);

template <typename T>
Ptr<AttributeChecker> MakeIntChecker (void);

Ptr<AttributeChecker> MakeIntChecker (int64_t min, int64_t max);

} // namespace ns3

namespace ns3 {

template <typename T1>
Ptr<ParamSpec> 
MakeIntParamSpec (T1 a1)
{
  return MakeParamSpecHelper<IntParamSpec,IntValue> (a1);
}
template <typename T1, typename T2>
Ptr<ParamSpec> MakeIntParamSpec (T1 a1, T2 a2)
{
  return MakeParamSpecHelper<IntParamSpec,IntValue> (a1, a2);
}

template <typename T>
Ptr<AttributeChecker>
MakeIntChecker (void)
{
  return MakeIntChecker (std::numeric_limits<T>::min (),
			 std::numeric_limits<T>::max ());
}

} // namespace ns3

#endif /* INT_VALUE_H */
