#include "callback.h"

namespace ns3 {

CallbackValue::CallbackValue ()
  : m_value ()
{}
CallbackValue::CallbackValue (const CallbackBase &base)
  : m_value (base)
{}
CallbackValue::~CallbackValue ()
{}
void 
CallbackValue::Set (CallbackBase base)
{
  m_value = base;
}
Ptr<AttributeValue> 
CallbackValue::Copy (void) const
{
  return Create<CallbackValue> (m_value);
}
std::string 
CallbackValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << PeekPointer (m_value.GetImpl ());
  return oss.str ();
}
bool 
CallbackValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  return false;
}

ATTRIBUTE_CHECKER_IMPLEMENT (Callback);

} // namespace ns3
