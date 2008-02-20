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
  virtual Attribute Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  void Set (uint64_t value);
  uint64_t Get (void) const;

  UintValue (Attribute value);
  operator Attribute () const;
private:
  uint64_t m_value;
};

class UintAccessor : public Accessor {};

template <typename T1>
Ptr<Accessor> MakeUintAccessor (T1 a1);
template <typename T1, typename T2>
Ptr<Accessor> MakeUintAccessor (T1 a1, T2 a2);

template <typename T>
Ptr<AttributeChecker> MakeUintChecker (void);

Ptr<AttributeChecker> MakeUintChecker (uint64_t min, uint64_t max);

} // namespace ns3

namespace ns3 {

template <typename T1>
Ptr<Accessor> 
MakeUintAccessor (T1 a1)
{
  return MakeAccessorHelper<UintAccessor,UintValue> (a1);
}

template <typename T1, typename T2>
Ptr<Accessor> MakeUintAccessor (T1 a1, T2 a2)
{
  return MakeAccessorHelper<UintAccessor,UintValue> (a1, a2);
}

template <typename T>
Ptr<AttributeChecker> MakeUintChecker (void)
{
  return MakeUintChecker (std::numeric_limits<T>::min (),
			  std::numeric_limits<T>::max ());
}

} // namespace ns3

#endif /* UINT_PARAMETER_H */
