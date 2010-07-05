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
#include "ns3/fatal-error.h"
#include <math.h>
#include <iostream>

namespace ns3 {

const double HighPrecision::MAX_64 = 18446744073709551615.0;

double HighPrecision::GetDouble (void) const
{
  bool is_negative = _cairo_int128_negative (m_value);
  cairo_int128_t value = is_negative ? _cairo_int128_negate (m_value) : m_value;
  cairo_int128_t hi = _cairo_int128_rsa (value, 64);
  cairo_uint128_t lo = _cairo_int128_sub (value, _cairo_uint128_lsl (hi, 64));
  double flo = _cairo_uint128_to_uint64 (lo);
  flo /= MAX_64;
  double retval = _cairo_uint128_to_uint64 (hi);
  retval += flo;
  retval = is_negative ? -retval: retval;
  return retval;
}
void
HighPrecision::Mul (HighPrecision const &o)
{
  // use the 128 bits multiplication
  m_value = Mul128 (m_value,o.m_value);
}


HighPrecision::HighPrecision (double value)
{
  int64_t hi = (int64_t) floor (value);
  uint64_t lo = (uint64_t) ((value - floor (value)) * MAX_64);
  m_value = _cairo_int64_to_int128 (hi);
  m_value = _cairo_int128_lsl (m_value, 64);
  cairo_int128_t clo = _cairo_uint128_to_int128 (_cairo_uint64_to_uint128 (lo));
  m_value = _cairo_int128_add (m_value, clo);
}

/**
 * this function multiplies two 128 bits fractions considering
 * the high 64 bits as the integer part and the low 64 bits
 * as the fractional part. It takes into account the sign
 * of the operands to produce a signed 128 bits result.
 */
cairo_int128_t
HighPrecision::Mul128 (cairo_int128_t sa, cairo_int128_t sb ) const
{
  bool negResult, negA, negB;

  negA = _cairo_int128_negative (sa);
  negB = _cairo_int128_negative (sb);
  // the result is negative only if one of the operand is negative
  negResult = (negA && !negB) || (!negA && negB);
  // now take the absolute part to make sure that the resulting operands are positive
  cairo_uint128_t a, b;
  a = _cairo_int128_to_uint128 (sa);
  b = _cairo_int128_to_uint128 (sb);
  a = negA ? _cairo_uint128_negate (a) : a;
  b = negB ? _cairo_uint128_negate (b) : b;

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
  if (hiPart.hi != 0)
    {
      NS_FATAL_ERROR ("High precision 128 bits multiplication error: multiplication overflow.");
    }
  // add the sign to the result
  result = negResult ? _cairo_uint128_negate (result) : result;
  return _cairo_uint128_to_int128 (result);
}

void
HighPrecision::Div (HighPrecision const &o)
{
  cairo_int128_t result = Div128 (m_value, o.m_value);
  m_value = result;
}

cairo_int128_t
HighPrecision::Div128 (cairo_int128_t sa, cairo_int128_t sb) const
{
  bool negResult, negA, negB;
  // take the sign of the operands
  negA = _cairo_int128_negative (sa);
  negB = _cairo_int128_negative (sb);
  // the result is negative only if one of the operand is negative
  negResult = (negA && !negB) || (!negA && negB);
  // now take the absolute part to make sure that the resulting operands are positive
  cairo_uint128_t a, b;
  a = _cairo_int128_to_uint128 (sa);
  b = _cairo_int128_to_uint128 (sb);
  a = negA ? _cairo_uint128_negate (a) : a;
  b = negB ? _cairo_uint128_negate (b) : b;

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
  result = negResult ? _cairo_uint128_negate (result) : result;
  return _cairo_uint128_to_int128 (result);
}

} // namespace ns3

