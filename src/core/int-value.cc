#include "int-value.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

IntValue::IntValue (int64_t value)
  : m_value (value)
{}
Attribute
IntValue::Copy (void) const
{
  return Attribute::Create<IntValue> (*this);
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

IntValue::IntValue (Attribute value)
{
  const IntValue *v = value.DynCast<const IntValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("assigning non-Int value to Int value.");
    }
  m_value = v->m_value;
}
IntValue::operator Attribute () const
{
  return Attribute::Create<IntValue> (*this);
}

Ptr<AttributeChecker>
MakeIntChecker (int64_t min, int64_t max)
{
  struct IntChecker : public AttributeChecker
  {
    IntChecker (int64_t minValue, int64_t maxValue)
      : m_minValue (minValue),
      m_maxValue (maxValue) {}
    virtual bool Check (Attribute value) const {
      const IntValue *v = value.DynCast<const IntValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get () >= m_minValue && v->Get () <= m_maxValue;
    }
    int64_t m_minValue;
    int64_t m_maxValue;
  } *checker = new IntChecker (min, max);
  return Ptr<AttributeChecker> (checker, false);
}


} // namespace ns3
