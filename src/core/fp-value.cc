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
FpValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << m_value;
  return oss.str ();
}
bool 
FpValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
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

Ptr<AttributeChecker> MakeFpChecker (double min, double max)
{
  struct Checker : public AttributeChecker
  {
    Checker (double minValue, double maxValue)
      : m_minValue (minValue),
      m_maxValue (maxValue) {}
    virtual bool Check (PValue value) const {
      const FpValue *v = value.DynCast<const FpValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get () >= m_minValue && v->Get () <= m_maxValue;
    }
    double m_minValue;
    double m_maxValue;
  } *checker = new Checker (min, max);
  return Ptr<AttributeChecker> (checker, false);
}


} // namespace ns3
