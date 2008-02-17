#include "uint-value.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

UintValue::UintValue (uint64_t value)
  : m_value (value)
{}
PValue
UintValue::Copy (void) const
{
  return PValue::Create<UintValue> (*this);
}

void 
UintValue::Set (uint64_t value)
{
  m_value = value;
}
uint64_t 
UintValue::Get (void) const
{
  return m_value;
}
std::string 
UintValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << m_value;
  return oss.str ();
}
bool 
UintValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  uint64_t v;
  std::istringstream iss;
  iss.str (value);
  iss >> v;
  bool ok = !iss.bad () && !iss.fail ();
  if (ok)
    {
      m_value = v;
    }
  return ok;
}

UintValue::UintValue (PValue value)
{
  const UintValue *v = value.DynCast<const UintValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Uint value to Uint value.");
    }
  m_value = v->m_value;
}
UintValue::operator PValue () const
{
  return PValue::Create<UintValue> (*this);
}

} // namespace ns3
