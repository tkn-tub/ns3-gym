#include "high-precision-128.h"
#include "ns3/abort.h"
#include "ns3/assert.h"
#include <math.h>
#ifdef COUNT_OPS
#include <iostream>
#endif

namespace ns3 {

#ifdef COUNT_OPS
uint128_t HighPrecision::g_nAdd = 0;
uint128_t HighPrecision::g_nMuli = 0;
uint128_t HighPrecision::g_nMul = 0;
uint128_t HighPrecision::g_nDiv = 0;
uint128_t HighPrecision::g_nCmp = 0;
HighPrecision::Printer  HighPrecision::g_printer;
HighPrecision::Printer::~Printer ()
{
  std::cout << "add=" << (double)g_nAdd << " mul=" << (double)g_nMul << " div=" << (double)g_nDiv
	    << " muli=" << (double)g_nMuli << " cmp=" << (double)g_nCmp;
}
#endif

#define OUTPUT_SIGN(sa,sb,ua,ub)					\
  ({bool negA, negB;							\
    negA = sa < 0;							\
    negB = sb < 0;							\
    ua = negA?-sa:sa;							\
    ub = negB?-sb:sb;							\
    (negA && !negB) || (!negA && negB);})


#define MASK_LO ((((uint128_t)1)<<64)-1)
#define MASK_HI (~MASK_LO)
void
HighPrecision::Mul (HighPrecision const &o)
{
  bool negResult;
  uint128_t a, b;
  negResult = OUTPUT_SIGN (m_value, o.m_value, a, b);
  int128_t result = Umul (a, b);
  // add the sign to the result
  result = negResult ? -result : result;
  m_value = result;
}

uint128_t
HighPrecision::Umul (uint128_t a, uint128_t b)
{
  INC_MUL;
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
  NS_ABORT_MSG_IF ((hiPart & MASK_HI) != 0,
		   "High precision 128 bits multiplication error: multiplication overflow.");
  return result;
}
void
HighPrecision::Div (HighPrecision const &o)
{
  bool negResult;
  uint128_t a, b;
  negResult = OUTPUT_SIGN (m_value, o.m_value, a, b);  
  int128_t result = Divu (a, b);
  result = negResult ? -result:result;
  m_value = result;
}

uint128_t
HighPrecision::Divu (uint128_t a, uint128_t b)
{
  INC_DIV;
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
  return result;
}

void 
HighPrecision::MulByInvert (const HighPrecision &o)
{
  bool negResult = m_value < 0;
  uint128_t a = negResult?-m_value:m_value;
  uint128_t result = UmulByInvert (a, o.m_value);

  m_value = negResult?-result:result;
}
uint128_t
HighPrecision::UmulByInvert (uint128_t a, uint128_t b)
{
  INC_MULI;
  uint128_t result, ah, bh, al, bl;
  uint128_t hi, mid;
  ah = a >> 64;
  bh = b >> 64;
  al = a & MASK_LO;
  bl = b & MASK_LO;
  hi = ah * bh;
  mid = ah * bl + al * bh;
  mid >>= 64;
  result = ah * bh + mid;
  return result;
}
HighPrecision 
HighPrecision::Invert (uint64_t v)
{
  NS_ASSERT (v > 1);
  uint128_t a;
  a = 1;
  a <<= 64;
  HighPrecision result;
  result.m_value = Divu (a, v);
  HighPrecision tmp = HighPrecision (v, false);
  tmp.MulByInvert (result);
  if (tmp.GetInteger () != 1)
    {
      result.m_value += 1;
    }
  return result;
}

} // namespace ns3
