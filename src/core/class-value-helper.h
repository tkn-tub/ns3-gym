#ifndef CLASS_VALUE_HELPER_H
#define CLASS_VALUE_HELPER_H

#include "value.h"
#include "param-spec-helper.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

template <typename T, typename U>
class ClassValue : public U
{
public:
  ClassValue (const T &value)
    : m_value (value) {}

  void Set (const T &v) {
    m_value = v;
  }
  T Get (void) const {
    return m_value;
  }

  // inherited from Value base class
  virtual Attribute Copy (void) const {
    return Attribute::Create<ClassValue<T,U> > (*this);
  }
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const {
    std::ostringstream oss;
    oss << m_value;
    return oss.str ();
  }
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker) {
    std::istringstream iss;
    iss.str (value);
    iss >> m_value;
    return !iss.bad () && !iss.fail ();
  }

  ClassValue (Attribute value) 
    {
      ClassValue<T,U> *v = value.DynCast<ClassValue<T,U> *> ();
      if (v == 0)
	{
	  NS_FATAL_ERROR ("Unexpected type of value. Expected \""<< typeid (U).name () <<"\"");
	}
      m_value = v->Get ();
    }
  operator Attribute () const
  {
    return Attribute::Create<ClassValue<T,U> > (*this);
  }

private:
  T m_value;
};

template <typename T,typename U>
T
ClassValueHelperExtractFrom (Attribute value)
{
  ClassValue<T,U> *v = value.DynCast<ClassValue<T,U> *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("Unexpected type of value. Expected \"" << typeid (U).name () << "\"");
    }
  return v->Get ();
}

template <typename T,typename U>
Attribute
ClassValueHelperConvertTo (const T *self)
{
  return Attribute::Create<ClassValue<T,U> > (*self);
}


template <typename T, typename U, typename BASE, typename T1>
Ptr<Accessor>
MakeClassValueHelperAccessor (T1 a1)
{
  return MakeAccessorHelper<BASE,ClassValue<T,U> > (a1);
}


template <typename T, typename U, typename BASE, typename T1, typename T2>
Ptr<Accessor>
MakeClassValueHelperAccessor (T1 a1, T2 a2)
{
  return MakeAccessorHelper<BASE,ClassValue<T,U> > (a1, a2);
}

} // namespace ns3

#endif /* CLASS_VALUE_HELPER_H */
