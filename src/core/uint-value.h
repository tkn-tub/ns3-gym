#ifndef UINTEGER_H
#define UINTEGER_H

#include "attribute.h"
#include "attribute-helper.h"
#include <stdint.h>

namespace ns3 {

class Uinteger
{
public:
  Uinteger (uint64_t value);
  Uinteger ();

  void Set (uint64_t value);
  uint64_t Get (void) const;

  operator uint64_t () const;

  ATTRIBUTE_CONVERTER_DEFINE (Uinteger);
private:
  uint64_t m_value;
};

std::ostream & operator << (std::ostream &os, const Uinteger &uinteger);
std::istream & operator >> (std::istream &is, Uinteger &uinteger);

ATTRIBUTE_VALUE_DEFINE (Uinteger);
ATTRIBUTE_ACCESSOR_DEFINE (Uinteger);

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (void);

Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min, uint64_t max);

} // namespace ns3

namespace ns3 {

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (void)
{
  return MakeUintegerChecker (std::numeric_limits<T>::min (),
			      std::numeric_limits<T>::max ());
}

} // namespace ns3

#endif /* UINTEGER_H */
