#include "high-precision-128.h"
#include "ns3/fatal-error.h"
#include <math.h>

#define MAX_64 18446744073709551615.0

namespace ns3 {

HighPrecision::HighPrecision ()
  : m_value (0)
{}
HighPrecision::HighPrecision (int64_t value, bool dummy)
  : m_value (value)
{
  m_value <<= 64;
}
HighPrecision::HighPrecision (double value)
{
  bool is_negative = value < 0;
  value = is_negative?-value:value;
  double hi = floor (value);
  double lo = (value - hi) * MAX_64;
  m_value = hi;
  m_value <<= 64;
  m_value += lo;
  m_value = is_negative?-m_value:m_value;
}

int64_t HighPrecision::GetInteger (void) const
{
  int128_t v = m_value >> 64;
  return v;
}
double 
HighPrecision::GetDouble (void) const
{
  bool is_negative = m_value < 0;
  uint128_t value = is_negative ? -m_value:m_value;
  uint64_t hi = value >> 64;
  uint64_t lo = value;
  double flo = lo;
  flo /= MAX_64;
  double retval = hi;
  retval += flo;
  retval = is_negative ? -retval : retval;
  return retval;
}
bool 
HighPrecision::Add (HighPrecision const &o)
{
  m_value += o.m_value;
  return true;
}
bool 
HighPrecision::Sub (HighPrecision const &o)
{
  m_value -= o.m_value;
  return true;
}
#define MASK_LO ((((uint128_t)1)<<64)-1)
#define MASK_HI (~MASK_LO)
bool 
HighPrecision::Mul (HighPrecision const &o)
{
  bool negResult, negA, negB;
    // take the sign of the operands
  negA = m_value < 0;
  negB = o.m_value < 0;
  // the result is negative only if one of the operand is negative
  negResult = (negA && !negB) || (!negA && negB);
  // now take the absolute part to make sure that the resulting operands are positive
  uint128_t a, b;
  a = negA?-m_value:m_value;
  b = negB?-o.m_value:o.m_value;
  uint128_t aL = a & MASK_LO;
  uint128_t bL = b & MASK_LO;
  uint128_t aH = (a >> 64) & MASK_LO;
  uint128_t bH = (b >> 64) & MASK_LO;


  uint128_t result;
  uint128_t hiPart,loPart,midPart;

  // Multiplying (a.h 2^64 + a.l) x (b.h 2^64 + b.l) =
  //			2^128 a.h b.h + 2^64*(a.h b.l+b.h a.l) + a.l b.l
  // get the low part a.l b.l
  // multiply the fractional part
  loPart = aL * bL;
  // compute the middle part 2^64*(a.h b.l+b.h a.l)
  midPart = aL * bH + aH * bL;
  // truncate the low part
  result = (loPart >> 64) + (midPart & MASK_LO);
  // compute the high part 2^128 a.h b.h
  hiPart = aH * bH;
  // truncate the high part and only use the low part
  result |= ((hiPart & MASK_LO) << 64) + (midPart & MASK_HI);
  // if the high part is not zero, put a warning
  if ((hiPart & MASK_HI) != 0)
    {
      NS_FATAL_ERROR ("High precision 128 bits multiplication error: multiplication overflow.");
    }
  // add the sign to the result
  result = negResult ? -result:result;
  m_value = result;
  return true;
}
bool 
HighPrecision::Div (HighPrecision const &o)
{
  bool negResult, negA, negB;
    // take the sign of the operands
  negA = m_value < 0;
  negB = o.m_value < 0;
  // the result is negative only if one of the operand is negative
  negResult = (negA && !negB) || (!negA && negB);
  // now take the absolute part to make sure that the resulting operands are positive
  uint128_t a, b;
  a = negA?-m_value:m_value;
  b = negB?-o.m_value:o.m_value;

  uint128_t quo = a / b;
  uint128_t rem = (a % b);
  uint128_t result = quo << 64;
  // Now, manage the remainder
  uint128_t tmp = rem >> 64;
  uint128_t div;
  if (tmp == 0)
    {
      rem = rem << 64;
      div = b;
    }
  else
    {
      rem = rem;
      div = b >> 64;
    }
  quo = rem / div;
  result = result + quo;
  result = negResult ? -result:result;
  m_value = result;
  return true;
}

int 
HighPrecision::Compare (HighPrecision const &o) const
{
  return (m_value < o.m_value)?-1:(m_value == o.m_value)?0:1;
}

HighPrecision
HighPrecision::Zero (void)
{
  return HighPrecision ();
}

} // namespace ns3
