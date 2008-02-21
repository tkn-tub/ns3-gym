#include "uinteger.h"
#include "fatal-error.h"
#include <sstream>

namespace ns3 {

Uinteger::Uinteger (uint64_t value)
  : m_value (value)
{}
Uinteger::Uinteger ()
{}
void 
Uinteger::Set (uint64_t value)
{
  m_value = value;
}
uint64_t 
Uinteger::Get (void) const
{
  return m_value;
}
Uinteger::operator uint64_t () const
{
  return m_value;
}
std::ostream & operator << (std::ostream &os, const Uinteger &uinteger)
{
  os << uinteger.Get ();
  return os;
}
std::istream & operator >> (std::istream &is, Uinteger &uinteger)
{
  uint64_t v;
  is >> v;
  uinteger.Set (v);
  return is;
}

ATTRIBUTE_CONVERTER_IMPLEMENT(Uinteger);
ATTRIBUTE_VALUE_IMPLEMENT(Uinteger);


Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min, uint64_t max)
{
  struct Checker : public AttributeChecker
  {
    Checker (uint64_t minValue, uint64_t maxValue)
      : m_minValue (minValue),
      m_maxValue (maxValue) {}
    virtual bool Check (Attribute value) const {
      const UintegerValue *v = value.DynCast<const UintegerValue *> ();
      if (v == 0)
	{
	  return false;
	}
      return v->Get ().Get () >= m_minValue && v->Get ().Get () <= m_maxValue;
    }
    uint64_t m_minValue;
    uint64_t m_maxValue;
  } *checker = new Checker (min, max);
  return Ptr<const AttributeChecker> (checker, false);
}


} // namespace ns3
