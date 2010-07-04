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
  inline HighPrecision (int64_t value, bool dummy);
  HighPrecision (double value);

  inline int64_t GetInteger (void) const;
  inline double GetDouble (void) const;
  inline bool Add (HighPrecision const &o);
  inline bool Sub (HighPrecision const &o);
  bool Mul (HighPrecision const &o);
  bool Div (HighPrecision const &o);

  inline int Compare (HighPrecision const &o) const;
  inline static HighPrecision Zero (void);
private:
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


double 
HighPrecision::GetDouble (void) const
{
#define HP128_MAX_64 18446744073709551615.0
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
#undef HP128_MAX_64
}

int64_t HighPrecision::GetInteger (void) const
{
  int128_t v = m_value >> 64;
  return v;
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

#endif /* HIGH_PRECISION_128_H */
