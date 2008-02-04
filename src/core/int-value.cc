#include "int-value.h"
#include "object.h"
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
IntValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  std::ostringstream oss;
  oss << m_value;
  return oss.str ();
}
bool 
IntValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
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


IntValueChecker::IntValueChecker (int64_t minValue, int64_t maxValue)
  : m_minValue (minValue),
    m_maxValue (maxValue)
{}
bool 
IntValueChecker::Check (const int64_t &value) const
{
  return value >= m_minValue && value <= m_maxValue;
}



} // namespace ns3
