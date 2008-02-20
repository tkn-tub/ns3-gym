#include "boolean-value.h"
#include "fatal-error.h"

namespace ns3 {

Boolean::Boolean ()
  : m_value (false)
{}
Boolean::Boolean (bool value)
  : m_value (value)
{}
void 
Boolean::Set (bool value)
{
  m_value = value;
}
bool 
Boolean::Get (void) const
{
  return m_value;
}
Boolean::operator bool () const
{
  return m_value;
}

std::ostream & operator << (std::ostream &os, const Boolean &value)
{
  if (value.Get ())
    {
      os << "true";
    }
  else
    {
      os << "false";
    }
  return os;
}
std::istream & operator >> (std::istream &is, Boolean &value)
{
  std::string v;
  is >> v;
  if (v == "true" ||
      v == "1" ||
      v == "t")
    {
      value.Set (true);
    }
  else if (v == "false" ||
	   v == "0" ||
	   v == "f")
    {
      value.Set (false);
    }
  else
    {
      is.setstate (std::ios_base::badbit);
    }  
  return is;
}

ATTRIBUTE_CONVERTER_IMPLEMENT (Boolean);
ATTRIBUTE_VALUE_IMPLEMENT (Boolean);
ATTRIBUTE_CHECKER_IMPLEMENT (Boolean);

} // namespace ns3
