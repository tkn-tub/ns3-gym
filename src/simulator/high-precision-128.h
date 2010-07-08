/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 INRIA
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
#ifndef HIGH_PRECISION_128_H
#define HIGH_PRECISION_128_H

#include "ns3/simulator-config.h"
#include <math.h>
#include <stdint.h>

#if defined(HAVE___UINT128_T) and !defined(HAVE_UINT128_T)
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;
#endif

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
private:
  static uint128_t UmulByInvert (uint128_t a, uint128_t b);
  static uint128_t Umul (uint128_t a, uint128_t b);
  static uint128_t Divu (uint128_t a, uint128_t b);

  int128_t m_value;
};

} // namespace ns3

namespace ns3 {

HighPrecision::HighPrecision ()
  : m_value (0)
{}
HighPrecision::HighPrecision (int64_t value, bool dummy)
  : m_value (value)
{
  m_value <<= 64;
}



int64_t HighPrecision::GetInteger (void) const
{
  int128_t v = m_value >> 64;
  return v;
}

void
HighPrecision::Add (HighPrecision const &o)
{
  m_value += o.m_value;
}
void
HighPrecision::Sub (HighPrecision const &o)
{
  m_value -= o.m_value;
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

#define HP128_MAX_64 18446744073709551615.0
double 
HighPrecision::GetDouble (void) const
{
  bool is_negative = m_value < 0;
  uint128_t value = is_negative ? -m_value:m_value;
  uint64_t hi = value >> 64;
  uint64_t lo = value;
  double flo = lo;
  flo /= HP128_MAX_64;
  double retval = hi;
  retval += flo;
  retval = is_negative ? -retval : retval;
  return retval;
}
HighPrecision::HighPrecision (double value)
{
  bool is_negative = value < 0;
  value = is_negative?-value:value;
  double hi = floor (value);
  double lo = (value - hi) * HP128_MAX_64;
  m_value = (int128_t)hi;
  m_value <<= 64;
  m_value += (int128_t)lo;
  m_value = is_negative?-m_value:m_value;
}
#undef HP128_MAX_64

} // namespace ns3

#endif /* HIGH_PRECISION_128_H */
