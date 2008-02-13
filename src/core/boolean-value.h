#ifndef BOOLEAN_VALUE_H
#define BOOLEAN_VALUE_H

#include "value.h"
#include "param-spec-helper.h"
#include "ptr.h"

namespace ns3 {

class BooleanValue : public Value
{
public:
  BooleanValue (bool value);
  void Set (bool value);
  bool Get (void) const;

  virtual PValue Copy (void) const;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);

  BooleanValue (PValue value);
  operator PValue () const;
private:
  bool m_value;
};

class BooleanParamSpec : public ParamSpec {};


template <typename T1>
Ptr<ParamSpec> MakeBooleanParamSpec (T1 a1, bool initialValue);

template <typename T1, typename T2>
Ptr<ParamSpec> MakeBooleanParamSpec (T1 a1, T2 a2, bool initialValue);

} // namespace ns3


// Implementation of template functions below.
namespace ns3 {

template <typename T1>
Ptr<ParamSpec> MakeBooleanParamSpec (T1 a1, bool initialValue)
{
  return MakeParamSpecHelper<BooleanParamSpec> (a1, BooleanValue (initialValue));
}

template <typename T1, typename T2>
Ptr<ParamSpec> MakeBooleanParamSpec (T1 a1, T2 a2, bool initialValue)
{
  return MakeParamSpecHelper<BooleanParamSpec> (a1, a2, BooleanValue (initialValue));
}



} // namespace ns3

#endif /* BOOLEAN_PARAMETER_H */
