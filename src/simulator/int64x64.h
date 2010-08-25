#ifndef INT64X64_H
#define INT64X64_H

#include "ns3/simulator-config.h"

#if defined (INT64X64_USE_DOUBLE)
#include "int64x64-double.h"
#elif defined (INT64X64_USE_CAIRO)
#include "int64x64-cairo.h"
#elif defined (INT64X64_USE_128)
#include "int64x64-128.h"
#endif

#include <iostream>

namespace ns3 {

inline int64x64_t operator + (const int64x64_t &lhs, const int64x64_t &rhs)
{
  int64x64_t tmp = lhs;
  tmp += rhs;
  return tmp;
}

inline int64x64_t operator - (const int64x64_t &lhs, const int64x64_t &rhs)
{
  int64x64_t tmp = lhs;
  tmp -= rhs;
  return tmp;
}

inline int64x64_t operator * (const int64x64_t &lhs, const int64x64_t &rhs)
{
  int64x64_t tmp = lhs;
  tmp *= rhs;
  return tmp;
}

inline int64x64_t operator / (const int64x64_t &lhs, const int64x64_t &rhs)
{
  int64x64_t tmp = lhs;
  tmp /= rhs;
  return tmp;
}

std::ostream &operator << (std::ostream &os, const int64x64_t &val);
std::istream &operator << (std::istream &is, int64x64_t &val);

} // namespace ns3

#endif /* INT64X64_H */
