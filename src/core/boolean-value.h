#ifndef BOOLEAN_VALUE_H
#define BOOLEAN_VALUE_H

#include "attribute.h"
#include "param-spec-helper.h"
#include "ptr.h"

namespace ns3 {

class BooleanValue : public AttributeValue
{
public:
  BooleanValue (bool value);
  void Set (bool value);
  bool Get (void) const;

  virtual Attribute Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  BooleanValue (Attribute value);
  operator Attribute () const;
private:
  bool m_value;
};

class BooleanAccessor : public AttributeAccessor {};


template <typename T1>
Ptr<const AttributeAccessor> MakeBooleanAccessor (T1 a1);

template <typename T1, typename T2>
Ptr<const AttributeAccessor> MakeBooleanAccessor (T1 a1, T2 a2);

Ptr<const AttributeChecker> MakeBooleanChecker (void);

} // namespace ns3


// Implementation of template functions below.
namespace ns3 {

template <typename T1>
Ptr<const AttributeAccessor> MakeBooleanAccessor (T1 a1)
{
  return MakeAccessorHelper<BooleanAccessor,BooleanValue> (a1);
}

template <typename T1, typename T2>
Ptr<const AttributeAccessor> MakeBooleanAccessor (T1 a1, T2 a2)
{
  return MakeAccessorHelper<BooleanAccessor,BooleanValue> (a1, a2);
}

} // namespace ns3

#endif /* BOOLEAN_PARAMETER_H */
