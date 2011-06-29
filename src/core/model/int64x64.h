#ifndef INT64X64_H
#define INT64X64_H

#include "ns3/core-config.h"

#if defined (INT64X64_USE_DOUBLE) || defined (PYTHON_SCAN)
#include "int64x64-double.h"
#elif defined (INT64X64_USE_CAIRO)
#include "int64x64-cairo.h"
#elif defined (INT64X64_USE_128)
#include "int64x64-128.h"
#endif

#include <iostream>

namespace ns3 {

#define INT64X64_OP_ARITH_TYPE(op,type)                                 \
  inline int64x64_t operator op (const int64x64_t &lhs, const type rhs) \
  {                                                                     \
    int64x64_t tmp = lhs;                                               \
    tmp op ## = int64x64_t (rhs);                                         \
    return tmp;                                                         \
  }                                                                     \
  inline int64x64_t operator op (const type lhs, const int64x64_t &rhs) \
  {                                                                     \
    int64x64_t tmp = int64x64_t (lhs);                                  \
    tmp op ## = rhs;                                                      \
    return tmp;                                                         \
  }

#define INT64X64_OP_ARITH(op)                                           \
  inline int64x64_t operator op (const int64x64_t &lhs, const int64x64_t &rhs) \
  {                                                                     \
    int64x64_t tmp = lhs;                                               \
    tmp op ## = rhs;                                                      \
    return tmp;                                                         \
  }                                                                     \
  INT64X64_OP_ARITH_TYPE (op,double)                                     \
  INT64X64_OP_ARITH_TYPE (op,signed char)                              \
  INT64X64_OP_ARITH_TYPE (op,signed short)                             \
  INT64X64_OP_ARITH_TYPE (op,signed int)                               \
  INT64X64_OP_ARITH_TYPE (op,signed long int)                          \
  INT64X64_OP_ARITH_TYPE (op,signed long long int)                     \
  INT64X64_OP_ARITH_TYPE (op,unsigned char)                            \
  INT64X64_OP_ARITH_TYPE (op,unsigned short)                           \
  INT64X64_OP_ARITH_TYPE (op,unsigned int)                             \
  INT64X64_OP_ARITH_TYPE (op,unsigned long int)                        \
  INT64X64_OP_ARITH_TYPE (op,unsigned long long int)

#define INT64X64_OP_CMP_TYPE(op,type)                                   \
  inline bool operator op (const int64x64_t &lhs, const type &rhs)      \
  {                                                                     \
    return lhs op int64x64_t (rhs);                                     \
  }                                                                     \
  inline bool operator op (const type &lhs, const int64x64_t &rhs)      \
  {                                                                     \
    return int64x64_t (lhs) op rhs;                                     \
  }

#define INT64X64_OP_CMP(op)                                             \
  INT64X64_OP_CMP_TYPE (op,double)                                       \
  INT64X64_OP_CMP_TYPE (op,signed int)                                 \
  INT64X64_OP_CMP_TYPE (op,signed long int)                            \
  INT64X64_OP_CMP_TYPE (op,signed long long int)                       \
  INT64X64_OP_CMP_TYPE (op,unsigned int)                               \
  INT64X64_OP_CMP_TYPE (op,unsigned long int)                          \
  INT64X64_OP_CMP_TYPE (op,unsigned long long int)


INT64X64_OP_ARITH (+)
INT64X64_OP_ARITH (-)
INT64X64_OP_ARITH (*)
INT64X64_OP_ARITH (/)
INT64X64_OP_CMP (==)
INT64X64_OP_CMP (!=)
INT64X64_OP_CMP (<)
INT64X64_OP_CMP (<=)
INT64X64_OP_CMP (>)
INT64X64_OP_CMP (>=)

std::ostream &operator << (std::ostream &os, const int64x64_t &val);
std::istream &operator >> (std::istream &is, int64x64_t &val);

inline int64x64_t Abs (const int64x64_t &value)
{
  return (value < 0) ? -value : value;
}

inline int64x64_t Min (const int64x64_t &a, const int64x64_t &b)
{
  return (a < b) ? a : b;
}

inline int64x64_t Max (const int64x64_t &a, const int64x64_t &b)
{
  return (a > b) ? a : b;
}

} // namespace ns3

#endif /* INT64X64_H */
