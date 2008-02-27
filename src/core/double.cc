#include "double.h"
#include "object.h"
#include <sstream>

namespace ns3 {

Double::Double ()
{}
Double::Double (double value)
  : m_value (value)
{}
void 
Double::Set (double value)
{
  m_value = value;
}
double 
Double::Get (void) const
{
  return m_value;
}
Double::operator double () const
{
  return m_value;
}
std::ostream & operator << (std::ostream &os, const Double &value)
{
  os << value.Get ();
  return os;
}
std::istream & operator >> (std::istream &is, Double &value)
{
  double v;
  is >> v;
  value.Set (v);
  return is;
}

ATTRIBUTE_VALUE_IMPLEMENT (Double);
  ATTRIBUTE_CONVERTER_IMPLEMENT (Double);

Ptr<const AttributeChecker> MakeDoubleChecker (double min, double max)
{
  struct Checker : public AttributeChecker
  {
    Checker (double minValue, double maxValue)
      : m_minValue (minValue),
      m_maxValue (maxValue) {}
    virtual bool Check (Attribute value) const {
      const DoubleValue *v = value.DynCast<const DoubleValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get () >= m_minValue && v->Get () <= m_maxValue;
    }
    double m_minValue;
    double m_maxValue;
  } *checker = new Checker (min, max);
  return Ptr<const AttributeChecker> (checker, false);
}


} // namespace ns3
