#ifndef FP_VALUE_H
#define FP_VALUE_H

#include "attribute.h"
#include "attribute-helper.h"
#include <stdint.h>

namespace ns3 {

class Double
{
public:
  Double ();
  Double (double value);

  void Set (double value);
  double Get (void) const;

  operator double () const;

  ATTRIBUTE_CONVERTER_DEFINE (Double);
private:
  double m_value;
};

std::ostream & operator << (std::ostream &os, const Double &value);
std::istream & operator >> (std::istream &is, Double &value);

ATTRIBUTE_VALUE_DEFINE (Double);
ATTRIBUTE_ACCESSOR_DEFINE (Double);

template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker (void);

template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker (double min);

Ptr<const AttributeChecker> MakeDoubleChecker (double min, double max);


} // namespace ns3

namespace ns3 {

template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker (void)
{
  return MakeDoubleChecker (-std::numeric_limits<T>::max (),
			    std::numeric_limits<T>::max ());
}

template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker (double min)
{
  return MakeDoubleChecker (min,
			    std::numeric_limits<T>::max ());
}

} // namespace ns3


#endif /* FP_VALUE_H */
