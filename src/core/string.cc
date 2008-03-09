#include "string.h"

namespace ns3 {

String::String ()
  : m_value ()
{}
String::String (const char *value)
  : m_value (value)
{}
String::String (std::string value)
  : m_value (value)
{}
void 
String::Set (std::string value)
{
  m_value = value;
}
void 
String::Set (const char *value)
{
  m_value = value;
}
std::string 
String::Get (void) const
{
  return m_value;
}

std::ostream & operator << (std::ostream &os, const String &value)
{
  os << value.Get ();
  return os;
}
std::istream &operator >> (std::istream &is, String &value)
{
  std::string str;
  is >> str;
  value = String (str);
  return is;
}

ATTRIBUTE_HELPER_CPP (String);

} // namespace ns3
