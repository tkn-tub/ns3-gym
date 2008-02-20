#include "boolean-value.h"
#include "fatal-error.h"

namespace ns3 {

BooleanValue::BooleanValue (bool value)
  : m_value (value)
{}
void 
BooleanValue::Set (bool value)
{
  m_value = value;
}
bool 
BooleanValue::Get (void) const
{
  return m_value;
}
Attribute
BooleanValue::Copy (void) const
{
  return Attribute::Create<BooleanValue> (*this);
}
std::string 
BooleanValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::string value;
  if (m_value)
    {
      value = "true";
    }
  else
    {
      value = "false";
    }
  return value;
}
bool 
BooleanValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  if (value == "true" ||
      value == "1" ||
      value == "t")
    {
      m_value = true;
      return true;
    }
  else if (value == "false" ||
	   value == "0" ||
	   value == "f")
    {
      m_value = false;
      return true;
    }
  else
    {
      return false;
    }  
}
BooleanValue::BooleanValue (Attribute value)
{
  const BooleanValue *v = value.DynCast<const BooleanValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Boolean value to Boolean value.");
    }
  m_value = v->m_value;
}
BooleanValue::operator Attribute () const
{
  return Attribute::Create<BooleanValue> (*this);
}

Ptr<AttributeChecker> MakeBooleanChecker (void)
{
  return MakeSimpleAttributeChecker<BooleanValue> ();
}

} // namespace ns3
