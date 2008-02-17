#ifndef ENUM_VALUE_H
#define ENUM_VALUE_H

#include "value.h"
#include "param-spec-helper.h"
#include <list>

namespace ns3 {

class EnumValue : public Value
{
public:
  EnumValue (int v);
  void Set (int v);
  int Get (void) const;

  virtual PValue Copy (void) const;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);

  EnumValue (PValue value);
  operator PValue () const;
private:
  int m_v;
};

class EnumParamSpec : public ParamSpec
{
public:
  EnumParamSpec ();

  void AddDefault (int v, std::string name);
  void Add (int v, std::string name);

  virtual bool Set (ObjectBase* object, PValue value) const;
  virtual bool Get (const ObjectBase* object, PValue value) const;
  virtual bool Check (PValue value) const;

private:
  virtual bool DoSet (ObjectBase *object, const EnumValue *value) const = 0;
  virtual bool DoGet (const ObjectBase *object, EnumValue *value) const = 0;
  friend class EnumValue;
  typedef std::list<std::pair<int,std::string> > ValueSet;
  ValueSet m_valueSet;
};

template <typename T, typename U>
Ptr<ParamSpec> MakeEnumParamSpec (U T::*v,
				  int v1, std::string n1,
				  int v2 = 0, std::string n2 = "",
				  int v3 = 0, std::string n3 = "",
				  int v4 = 0, std::string n4 = "",
				  int v5 = 0, std::string n5 = "",
				  int v6 = 0, std::string n6 = "",
				  int v7 = 0, std::string n7 = "",
				  int v8 = 0, std::string n8 = "",
				  int v9 = 0, std::string n9 = "",
				  int v10 = 0, std::string n10 = "",
				  int v11 = 0, std::string n11 = "",
				  int v12 = 0, std::string n12 = "");


} // namespace ns3

namespace ns3 {

template <typename T, typename U>
Ptr<ParamSpec> MakeEnumParamSpec (U T::*v,
				  int v1, std::string n1,
				  int v2 = 0, std::string n2 = "",
				  int v3 = 0, std::string n3 = "",
				  int v4 = 0, std::string n4 = "",
				  int v5 = 0, std::string n5 = "",
				  int v6 = 0, std::string n6 = "",
				  int v7 = 0, std::string n7 = "",
				  int v8 = 0, std::string n8 = "",
				  int v9 = 0, std::string n9 = "",
				  int v10 = 0, std::string n10 = "",
				  int v11 = 0, std::string n11 = "",
				  int v12 = 0, std::string n12 = "")
{
  class MemberVariable : public EnumParamSpec
  {
  private:
    U T::*m_memberVariable;
  public:
    MemberVariable (U T::*memberVariable)
      : m_memberVariable (memberVariable) {}
    virtual bool DoSet (ObjectBase *object, const EnumValue *value) const {
      T *o = dynamic_cast<T *> (object);
      if (o == 0)
	{
	  return false;
	}
      (o->*m_memberVariable) = U (value->Get ());
      return true;
    }
    virtual bool DoGet (const ObjectBase *object, EnumValue *value) const {
      const T *o = dynamic_cast<const T *> (object);
      if (o == 0)
	{
	  return false;
	}
      value->Set (o->*m_memberVariable);
      return true;
    }
  };
  Ptr<EnumParamSpec> spec = Ptr<EnumParamSpec> (new MemberVariable (v), false);
  spec->AddDefault (v1, n1);
  if (n2 == "")
    {
      return spec;
    }
  spec->Add (v2, n2);
  if (n3 == "")
    {
      return spec;
    }
  spec->Add (v3, n3);
  if (n4 == "")
    {
      return spec;
    }
  spec->Add (v4, n4);
  if (n5 == "")
    {
      return spec;
    }
  spec->Add (v5, n5);
  if (n6 == "")
    {
      return spec;
    }
  spec->Add (v6, n6);
  if (n7 == "")
    {
      return spec;
    }
  spec->Add (v7, n7);
  if (n8 == "")
    {
      return spec;
    }
  spec->Add (v8, n8);
  if (n9 == "")
    {
      return spec;
    }
  spec->Add (v9, n9);
  if (n10 == "")
    {
      return spec;
    }
  spec->Add (v10, n10);
  if (n11 == "")
    {
      return spec;
    }
  spec->Add (v11, n11);
  if (n12 == "")
    {
      return spec;
    }
  spec->Add (v12, n12);
  return spec;
}

} // namespace ns3

#endif /* ENUM_VALUE_H */
