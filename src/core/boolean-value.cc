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
Ptr<Value>
BooleanValue::Copy (void) const
{
  return Create<BooleanValue> (*this);
}
std::string 
BooleanValue::SerializeToString (Ptr<const ParamSpec> spec) const
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
BooleanValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
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
BooleanValue::BooleanValue (Ptr<const Value> value)
{
  const BooleanValue *v = dynamic_cast<const BooleanValue *> (PeekPointer (value));
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Boolean value to Boolean value.");
    }
  m_value = v->m_value;
}
BooleanValue::operator Ptr<Value> () const
{
  return Create<BooleanValue> (*this);
}
BooleanValue::operator Ptr<const Value> () const
{
  return Create<BooleanValue> (*this);
}

} // namespace ns3
