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


template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (bool T::*v, bool initialValue);

template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (bool (T::*getter) (void) const, 
				     bool initialValue);

template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (bool (T::*getter) (void) const,
				     void (T::*setter) (bool),
				     bool initialValue);
template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (void (T::*setter) (bool),
				     bool initialValue);

} // namespace ns3


// Implementation of template functions below.
namespace ns3 {

template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (bool T::*v, bool initialValue)
{
  return MakeMemberVariableParamSpec (v, BooleanValue (initialValue));
}

template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (void (T::*setter) (bool),
				     bool (T::*getter) (void) const,
				     bool initialValue)
{
  return MakeMemberMethodParamSpec (setter, getter, BooleanValue (initialValue));
}

template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (bool (T::*getter) (void) const,
				     bool initialValue)
{
  return MakeMemberMethodGetterParamSpec (getter, BooleanValue (initialValue));
}

template <typename T>
Ptr<ParamSpec> MakeBooleanParamSpec (void (T::*setter) (bool),
				     bool initialValue)
{
  return MakeMemberMethodSetterParamSpec (setter, BooleanValue (initialValue));
}

} // namespace ns3

#endif /* BOOLEAN_PARAMETER_H */
