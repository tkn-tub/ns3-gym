#include "boolean-value.h"

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

} // namespace ns3
