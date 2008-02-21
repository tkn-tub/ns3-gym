#ifndef INTEGER_H
#define INTEGER_H

#include "attribute.h"
#include "attribute-helper.h"
#include <stdint.h>

namespace ns3 {

class Integer
{
public:
  Integer (int64_t value);
  Integer ();
  void Set (int64_t value);
  int64_t Get (void) const;

  operator int64_t () const;
  ATTRIBUTE_CONVERTER_DEFINE (Integer);
private:
  int64_t m_value;
};

std::ostream &operator << (std::ostream &os, const Integer &integer);
std::istream &operator >> (std::istream &is, Integer &integer);

ATTRIBUTE_VALUE_DEFINE(Integer);
ATTRIBUTE_ACCESSOR_DEFINE(Integer);

template <typename T>
Ptr<const AttributeChecker> MakeIntegerChecker (void);

Ptr<const AttributeChecker> MakeIntegerChecker (int64_t min, int64_t max);

} // namespace ns3

namespace ns3 {

template <typename T>
Ptr<const AttributeChecker>
MakeIntegerChecker (void)
{
  return MakeIntegerChecker (std::numeric_limits<T>::min (),
			     std::numeric_limits<T>::max ());
}

} // namespace ns3

#endif /* INTEGER_H */
