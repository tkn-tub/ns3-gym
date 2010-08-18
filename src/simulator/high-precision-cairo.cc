/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "high-precision-cairo.h"
#include "ns3/test.h"
#include "ns3/abort.h"
#include "ns3/assert.h"
#include <math.h>
#include <iostream>

namespace ns3 {


#define OUTPUT_SIGN(sa,sb,ua,ub)                                        \
  ({bool negA, negB;                                                    \
  negA = _cairo_int128_negative (sa);                                   \
  negB = _cairo_int128_negative (sb);                                   \
  ua = _cairo_int128_to_uint128 (sa);                                   \
  ub = _cairo_int128_to_uint128 (sb);                                   \
  ua = negA ? _cairo_uint128_negate (ua) : ua;                          \
  ub = negB ? _cairo_uint128_negate (ub) : ub;                          \
  (negA && !negB) || (!negA && negB);})

void
HighPrecision::Mul (HighPrecision const &o)
{
  cairo_uint128_t a, b, result;
  bool sign = OUTPUT_SIGN (m_value, o.m_value, a, b);
  result = Umul (a, b);
  m_value = sign ? _cairo_uint128_negate (result) : result;
}


/**
 * this function multiplies two 128 bits fractions considering
 * the high 64 bits as the integer part and the low 64 bits
 * as the fractional part. It takes into account the sign
 * of the operands to produce a signed 128 bits result.
 */
cairo_uint128_t
HighPrecision::Umul (cairo_uint128_t a, cairo_uint128_t b)
{
  cairo_uint128_t result;
  cairo_uint128_t hiPart,loPart,midPart;

  // Multiplying (a.h 2^64 + a.l) x (b.h 2^64 + b.l) =
  //			2^128 a.h b.h + 2^64*(a.h b.l+b.h a.l) + a.l b.l
  // get the low part a.l b.l
  // multiply the fractional part
  loPart = _cairo_uint64x64_128_mul (a.lo, b.lo);
  // compute the middle part 2^64*(a.h b.l+b.h a.l)
  midPart = _cairo_uint128_add (_cairo_uint64x64_128_mul (a.lo, b.hi),
                                _cairo_uint64x64_128_mul (a.hi, b.lo));
  // truncate the low part
  result.lo = _cairo_uint64_add (loPart.hi,midPart.lo);
  // compute the high part 2^128 a.h b.h
  hiPart = _cairo_uint64x64_128_mul (a.hi, b.hi);
  // truncate the high part and only use the low part
  result.hi = _cairo_uint64_add (hiPart.lo,midPart.hi);
  // if the high part is not zero, put a warning
  NS_ABORT_MSG_IF (hiPart.hi != 0,
                   "High precision 128 bits multiplication error: multiplication overflow.");
  return result;
}

void
HighPrecision::Div (HighPrecision const &o)
{
  cairo_uint128_t a, b, result;
  bool sign = OUTPUT_SIGN (m_value, o.m_value, a, b);
  result = Udiv (a, b);
  m_value = sign ? _cairo_uint128_negate (result) : result;
}

cairo_uint128_t
HighPrecision::Udiv (cairo_uint128_t a, cairo_uint128_t b)
{
  cairo_uquorem128_t qr = _cairo_uint128_divrem (a, b);
  cairo_uint128_t result = _cairo_uint128_lsl (qr.quo, 64);
  // Now, manage the remainder
  cairo_uint128_t tmp = _cairo_uint128_rsl (qr.rem, 64);
  cairo_uint128_t zero = _cairo_uint64_to_uint128 (0);
  cairo_uint128_t rem, div;
  if (_cairo_uint128_eq (tmp, zero))
    {
      rem = _cairo_uint128_lsl (qr.rem, 64);
      div = b;
    }
  else
    {
      rem = qr.rem;
      div = _cairo_uint128_rsl (b, 64);
    }
  qr = _cairo_uint128_divrem (rem, div);
  result = _cairo_uint128_add (result, qr.quo);
  return result;
}

void 
HighPrecision::MulByInvert (const HighPrecision &o)
{
  bool negResult = _cairo_int128_negative (m_value);
  cairo_uint128_t a = negResult?_cairo_int128_negate(m_value):m_value;
  cairo_uint128_t result = UmulByInvert (a, o.m_value);

  m_value = negResult?_cairo_int128_negate(result):result;
}
cairo_uint128_t
HighPrecision::UmulByInvert (cairo_uint128_t a, cairo_uint128_t b)
{
  cairo_uint128_t result;
  cairo_uint128_t hi, mid;
  hi = _cairo_uint64x64_128_mul (a.hi, b.hi);
  mid = _cairo_uint128_add (_cairo_uint64x64_128_mul (a.hi, b.lo),
                           _cairo_uint64x64_128_mul (a.lo, b.hi));
  mid.lo = mid.hi;
  mid.hi = 0;
  result = _cairo_uint128_add (hi,mid);
  return result;
}
HighPrecision 
HighPrecision::Invert (uint64_t v)
{
  NS_ASSERT (v > 1);
  cairo_uint128_t a, factor;
  a.hi = 1;
  a.lo = 0;
  factor.hi = 0;
  factor.lo = v;
  HighPrecision result;
  result.m_value = Udiv (a, factor);
  HighPrecision tmp = HighPrecision (v, false);
  tmp.MulByInvert (result);
  if (tmp.GetInteger () != 1)
    {
      cairo_uint128_t one = {1, 0};
      result.m_value = _cairo_uint128_add (result.m_value, one);
    }
  return result;
}

int64_t
HighPrecision::GetHigh (void) const
{
  NS_FATAL_ERROR ("XXX this function unavailable for high-precision-as-cairo; patch requested");
  return 0;
}

uint64_t
HighPrecision::GetLow (void) const
{
  NS_FATAL_ERROR ("XXX this function unavailable for high-precision-as-cairo; patch requested");
  return 0;
}

std::ostream &operator << (std::ostream &os, const HighPrecision &hp)
{
  return os;
}
std::istream &operator >> (std::istream &is, HighPrecision &hp)
{
  return is;
}


} // namespace ns3

// include directly to allow optimizations within the compilation unit.
extern "C" {
#include "cairo-wideint.c"
}
