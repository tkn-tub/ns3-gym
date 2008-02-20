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

  virtual Attribute Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  void Set (double value);
  double Get (void) const;

  FpValue (Attribute value);
  operator Attribute () const;
private:
  double m_value;
};

class FpAccessor : public Accessor {};

template <typename T1>
Ptr<Accessor> MakeFpAccessor (T1 a1);
template <typename T1, typename T2>
Ptr<Accessor> MakeFpAccessor (T1 a1, T2 a2);

template <typename T>
Ptr<AttributeChecker> MakeFpChecker (void);

Ptr<AttributeChecker> MakeFpChecker (double min, double max);



} // namespace ns3

namespace ns3 {

template <typename T1>
Ptr<Accessor> MakeFpAccessor (T1 a1)
{
  return MakeAccessorHelper<FpAccessor,FpValue> (a1);
}
template <typename T1, typename T2>
Ptr<Accessor> MakeFpAccessor (T1 a1, T2 a2)
{
  return MakeAccessorHelper<FpAccessor,FpValue> (a1, a2);
}

template <typename T>
Ptr<AttributeChecker> MakeFpChecker (void)
{
  return MakeFpChecker (-std::numeric_limits<T>::max (),
			std::numeric_limits<T>::max ());
}

} // namespace ns3


#endif /* FP_VALUE_H */
