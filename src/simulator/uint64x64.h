#ifndef UINT64X64_H
#define UINT64X64_H

#include "ns3/simulator-config.h"

#if defined (UINT64x64_USE_DOUBLE)
#include "uint64x64-double.h"
#elif defined (UINT64x64_USE_CAIRO)
#include "uint64x64-cairo.h"
#elif defined (UINT64x64_USE_128)
#include "uint64x64-128.h"
#endif

#include <iostream>

namespace ns3 {

inline uint64x64_t operator + (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  uint64x64_t tmp = lhs;
  tmp += rhs;
  return tmp;
}

inline uint64x64_t operator - (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  uint64x64_t tmp = lhs;
  tmp -= rhs;
  return tmp;
}

inline uint64x64_t operator * (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  uint64x64_t tmp = lhs;
  tmp *= rhs;
  return tmp;
}

inline uint64x64_t operator / (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  uint64x64_t tmp = lhs;
  tmp /= rhs;
  return tmp;
}

std::ostream &operator << (std::ostream &os, const uint64x64_t &val);
std::istream &operator << (std::istream &is, uint64x64_t &val);

} // namespace ns3

#endif /* UINT64X64_H */
