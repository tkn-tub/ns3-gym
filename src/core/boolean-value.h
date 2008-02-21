#ifndef BOOLEAN_VALUE_H
#define BOOLEAN_VALUE_H

#include "attribute.h"
#include "attribute-helper.h"

namespace ns3 {

class Boolean
{
public:
  Boolean ();
  Boolean (bool value);
  void Set (bool value);
  bool Get (void) const;

  operator bool () const;

  ATTRIBUTE_CONVERTER_DEFINE (Boolean);
private:
  bool m_value;
};

std::ostream & operator << (std::ostream &os, const Boolean &value);
std::istream & operator >> (std::istream &is, Boolean &value);

ATTRIBUTE_VALUE_DEFINE (Boolean);
ATTRIBUTE_CHECKER_DEFINE (Boolean);
ATTRIBUTE_ACCESSOR_DEFINE (Boolean);

} // namespace ns3

#endif /* BOOLEAN_PARAMETER_H */
