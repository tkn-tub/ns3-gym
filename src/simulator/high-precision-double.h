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
#ifndef HIGH_PRECISION_DOUBLE_H
#define HIGH_PRECISION_DOUBLE_H

#include <math.h>

namespace ns3 {

/**
 * Obviously, this implementation of the HighPrecision class does
 * not provide the 128 bits accuracy since it uses a an IEEE754 double
 * to store the value. It also does not report overflows.
 * So, it is a nice shortcut but in no way a complete solution.
 */

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
  inline void Mul (HighPrecision const &o);
  inline void Div (HighPrecision const &o);

  inline int Compare (HighPrecision const &o) const;
  inline static HighPrecision Zero (void);
private:
  double m_value;
};

} // namespace ns3

namespace ns3 {

HighPrecision::HighPrecision ()
  : m_value (0.0)
{
}

HighPrecision::HighPrecision (int64_t value, bool dummy)
  : m_value ((double)value)
{
}

HighPrecision::HighPrecision (double value)
  : m_value (value)
{
}

int64_t
HighPrecision::GetInteger (void) const
{
  return (int64_t)floor (m_value);
}

double
HighPrecision::GetDouble (void) const
{
  return m_value;
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
void
HighPrecision::Mul (HighPrecision const &o)
{
  m_value *= o.m_value;
}
void
HighPrecision::Div (HighPrecision const &o)
{
  m_value /= o.m_value;
}
int
HighPrecision::Compare (HighPrecision const &o) const
{
  return m_value < o.m_value;
}
HighPrecision
HighPrecision::Zero (void)
{
  return HighPrecision (0,0);
}

} // namespace ns3

#endif /* HIGH_PRECISION_DOUBLE_H  */
