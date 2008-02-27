#ifndef NS3_STRING_H
#define NS3_STRING_H

#include <string>
#include "attribute-helper.h"

namespace ns3 {

class String
{
public:
  String ();
  String (const char *value);
  String (std::string value);
  void Set (std::string value);
  void Set (const char *value);
  std::string Get (void) const;

  VALUE_HELPER_HEADER_1 (String);
private:
  std::string m_value;
};

std::ostream & operator << (std::ostream &os, const String &value);
std::istream &operator >> (std::istream &is, String &value);

VALUE_HELPER_HEADER_2 (String);

} // namespace ns3


#endif /* NS3_STRING_H */
