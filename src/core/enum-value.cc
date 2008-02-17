#include "enum-value.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {


EnumValue::EnumValue (int v)
  : m_v (v)
{}
void 
EnumValue::Set (int v)
{
  m_v = v;
}
int 
EnumValue::Get (void) const
{
  return m_v;
}

PValue
EnumValue::Copy (void) const
{
  return PValue::Create<EnumValue> (*this);
}
std::string 
EnumValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  const EnumParamSpec *p = dynamic_cast<const EnumParamSpec *> (PeekPointer (spec));
  NS_ASSERT (p != 0);
  for (EnumParamSpec::ValueSet::const_iterator i = p->m_valueSet.begin (); i != p->m_valueSet.end (); i++)
    {
      if (i->first == m_v)
	{
	  return i->second;
	}
    }
  
  NS_FATAL_ERROR ("The user has set an invalid C++ value in this EnumValue");
  // quiet compiler.
  return "";
}
bool 
EnumValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  const EnumParamSpec *p = dynamic_cast<const EnumParamSpec *> (PeekPointer (spec));
  NS_ASSERT (p != 0);
  for (EnumParamSpec::ValueSet::const_iterator i = p->m_valueSet.begin (); i != p->m_valueSet.end (); i++)
    {
      if (i->second == value)
	{
	  m_v = i->first;
	  return true;
	}
    }
  return false;
}

EnumValue::EnumValue (PValue value)
{
  const EnumValue *v = value.DynCast<const EnumValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Enum value to Enum value.");
    }
  m_v = v->m_v;
}
EnumValue::operator PValue () const
{
  return PValue::Create<EnumValue> (*this);
}



EnumParamSpec::EnumParamSpec ()
{}

void 
EnumParamSpec::AddDefault (int v, std::string name)
{
  m_valueSet.push_front (std::make_pair (v, name));
}
void 
EnumParamSpec::Add (int v, std::string name)
{
  m_valueSet.push_back (std::make_pair (v, name));
}

bool 
EnumParamSpec::Set (ObjectBase * object, PValue value) const
{
  const EnumValue *p = value.DynCast<const EnumValue *> ();
  if (p == 0)
    {
      return false;
    }
  DoSet (object, p);
  return true;
}
bool 
EnumParamSpec::Get (const ObjectBase * object, PValue value) const
{
  EnumValue *p = value.DynCast<EnumValue *> ();
  if (p == 0)
    {
      return false;
    }
  DoGet (object, p);
  return true;
}
bool 
EnumParamSpec::Check (PValue value) const
{
  const EnumValue *p = value.DynCast<const EnumValue *> ();
  if (p == 0)
    {
      return false;
    }
  for (ValueSet::const_iterator i = m_valueSet.begin (); i != m_valueSet.end (); i++)
    {
      if (i->first == p->Get ())
	{
	  return true;
	}
    }
  return false;
}

} // namespace ns3
