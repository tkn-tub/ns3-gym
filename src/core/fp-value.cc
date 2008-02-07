#include "fp-value.h"
#include "object.h"
#include <sstream>

namespace ns3 {

FpValue::FpValue (double value)
  : m_value (value)
{}
PValue
FpValue::Copy (void) const
{
  return PValue::Create<FpValue> (*this);
}

void 
FpValue::Set (double value)
{
  m_value = value;
}
double 
FpValue::Get (void) const
{
  return m_value;
}
std::string 
FpValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  std::ostringstream oss;
  oss << m_value;
  return oss.str ();
}
bool 
FpValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  double v;
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

FpValue::FpValue (PValue value)
{
  const FpValue *v = value.DynCast<const FpValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Fp value to Fp value.");
    }
  m_value = v->m_value;
}
FpValue::operator PValue () const
{
  return PValue::Create<FpValue> (*this);
}



FpValueChecker::FpValueChecker (double min, double max)
  : m_min (min),
    m_max (max)
{}
bool 
FpValueChecker::Check (double v) const
{
  return v >= m_min && v <= m_max;
}

} // namespace ns3
