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
 *
 * So, the code tries really hard to perform any of these 128 bit
 * operations by doing all arithmetic on 64 bit integers when possible
 * (i.e., when there is no fractional part. This is a very common case).
 * Hence, the following code has a m_fastValue (64 bits) and a
 * m_slowValue (128 bits). m_fastValue is used by default and the code
 * converts it to a m_slowValue when needed.
 *
 * If you want to monitor the efficiency of this strategy, you can
 * enable the macro HP128INC below and call the HighPrecision::PrintStats
 * method at the end of the simulation.
 *
 *  Explanation of Slow and Fast values:
 *
 * HighPrecision class create a fastValue and a slowValue depending on the
 * input number. If the input is an integer with 0 fractional part, it will
 * use the fastValue which will contain the integer in a 64 bits format. If
 * it has a fractional part, the slowValue will be used. It is represented
 * simply as a high part slowValue.hi which will contain the integer part
 * and the fractional part slowValue.lo which will contain the factional
 * part as an integer (obtained by multiplying the fractional part by 2^64).
 *
 * Explanation of Slow and Fast operations:
 *
 * If both operands are fastValues, we will perform fast operations, i-e
 * simply using integer operations. If we have though one of the value is
 * slowValue we need to convert the fastValue into a slow one. It is simply
 * obtained by putting the slowValue.lo = 0 and slowValue.hi = fastValue.
 * After that we apply the slow operation which will be a 128 bits operation
 * with two 128 bits operands.
 *
 */

namespace ns3 {

class HighPrecision
{
public:
  inline HighPrecision ();
  inline HighPrecision (int64_t value, bool dummy);
  inline HighPrecision (double value);

  inline int64_t GetInteger (void) const;
  inline double GetDouble (void) const;
  inline void Add (HighPrecision const &o);
  inline void Sub (HighPrecision const &o);
  void Mul (HighPrecision const &o);
  void Div (HighPrecision const &o);

  inline int Compare (HighPrecision const &o) const;
  inline static HighPrecision Zero (void);
private:
  cairo_uint128_t  Mul128 (cairo_uint128_t, cairo_uint128_t ) const;
  cairo_int128_t Div128 (cairo_int128_t sa, cairo_int128_t sb) const;
  inline bool IsNegative (void) const;

  cairo_int128_t m_value;
};

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
  int64_t hi = fhi;
  uint64_t lo = (uint64_t) ((value - fhi) * HPCAIRO_MAX_64);
  m_value.hi = hi;
  m_value.lo = lo;
}
#undef HPCAIRO_MAX_64

} // namespace ns3

#endif /* HIGH_PRECISION_CAIRO_H */
