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
#ifndef HIGH_PRECISION_CAIRO_H
#define HIGH_PRECISION_CAIRO_H

#include <stdint.h>
#include <math.h>
#include <iostream>
#include "cairo-wideint-private.h"

/**
 * This file contains an implementation of the HighPrecision class.
 * Each instance of the Time class also contains an instance of this
 * class which is used to perform all the arithmetic operations of
 * the Time class.
 *
 * This code is a bit ugly with a lot of inline methods for speed:
 * profiling this code on anything but the simplest scenarios shows
 * that it is a big bottleneck if great care in its implementation
 * is not performed. My observations are that what dominates are
 * Division operations (there are really really super costly)
 * and Comparison operations (because there are typically a lot of
 * these in any complex timekeeping code).
 */

namespace ns3 {

class HighPrecision
{
public:
  inline HighPrecision ();
  explicit inline HighPrecision (int64_t value, bool dummy);
  explicit inline HighPrecision (double value);

  inline int64_t GetInteger (void) const;
  inline double GetDouble (void) const;
  inline void Add (HighPrecision const &o);
  inline void Sub (HighPrecision const &o);
  void Mul (HighPrecision const &o);
  void Div (HighPrecision const &o);
  void MulByInvert (const HighPrecision &o);
  static HighPrecision Invert (uint64_t v);

  inline int Compare (HighPrecision const &o) const;
  inline static HighPrecision Zero (void);
  int64_t GetHigh (void) const;
  uint64_t GetLow (void) const;
private:
  static cairo_uint128_t  Umul (cairo_uint128_t a, cairo_uint128_t b);
  static cairo_uint128_t Udiv (cairo_uint128_t a, cairo_uint128_t b);
  static cairo_uint128_t UmulByInvert (cairo_uint128_t a, cairo_uint128_t b);
  inline bool IsNegative (void) const;

  cairo_int128_t m_value;
};

std::ostream &operator << (std::ostream &os, const HighPrecision &hp);
std::istream &operator >> (std::istream &is, HighPrecision &hp);


} // namespace ns3

namespace ns3 {

HighPrecision::HighPrecision ()
{
  m_value.hi = 0;
  m_value.lo = 0;
}

HighPrecision::HighPrecision (int64_t value, bool dummy)
{
  m_value.hi = value;
  m_value.lo = 0;
}

bool 
HighPrecision::IsNegative (void) const
{
  int64_t hi = m_value.hi;
  return hi < 0;
}

int64_t
HighPrecision::GetInteger (void) const
{
  return m_value.hi;
}
void
HighPrecision::Add (HighPrecision const &o)
{
  m_value.hi += o.m_value.hi;
  m_value.lo += o.m_value.lo;
  if (m_value.lo < o.m_value.lo)
    {
      m_value.hi++;
    }
}
void
HighPrecision::Sub (HighPrecision const &o)
{
  m_value.hi -= o.m_value.hi;
  m_value.lo -= o.m_value.lo;
  if (m_value.lo > o.m_value.lo)
    {
      m_value.hi--;
    }
}
int
HighPrecision::Compare (HighPrecision const &o) const
{
  HighPrecision tmp = *this;
  tmp.Sub (o);
  return tmp.IsNegative ()?-1:(tmp.m_value.hi == 0 && tmp.m_value.lo == 0)?0:1;
}
HighPrecision
HighPrecision::Zero (void)
{
  return HighPrecision ();
}


#define HPCAIRO_MAX_64 18446744073709551615.0
double HighPrecision::GetDouble (void) const
{
  bool is_negative = IsNegative ();
  cairo_int128_t value = is_negative ? _cairo_int128_negate (m_value) : m_value;
  double flo = value.lo;
  flo /= HPCAIRO_MAX_64;
  double retval = value.hi;
  retval += flo;
  retval = is_negative ? -retval: retval;
  return retval;
}
#undef HPCAIRO_MAX_64

#define HPCAIRO_MAX_64 18446744073709551615.0
HighPrecision::HighPrecision (double value)
{
  double fhi = floor (value);
  int64_t hi = static_cast<int64_t> (fhi);
  uint64_t lo = (uint64_t) ((value - fhi) * HPCAIRO_MAX_64);
  m_value.hi = hi;
  m_value.lo = lo;
}
#undef HPCAIRO_MAX_64

} // namespace ns3

#endif /* HIGH_PRECISION_CAIRO_H */
