#include "int-value.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

IntValue::IntValue (int64_t value)
  : m_value (value)
{}
PValue
IntValue::Copy (void) const
{
  return PValue::Create<IntValue> (*this);
}

void 
IntValue::Set (int64_t value)
{
  m_value = value;
}
int64_t 
IntValue::Get (void) const
{
  return m_value;
}
std::string 
IntValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << m_value;
  return oss.str ();
}
bool 
IntValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  int64_t v;
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

IntValue::IntValue (PValue value)
{
  const IntValue *v = value.DynCast<const IntValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Int value to Int value.");
    }
  m_value = v->m_value;
}
IntValue::operator PValue () const
{
  return PValue::Create<IntValue> (*this);
}


} // namespace ns3
