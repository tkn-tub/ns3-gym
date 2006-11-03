/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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
#include "high-precision.h"

#ifdef HIGH_PRECISION_I128


HighPrecision::HighPrecision ()
  : m_high (0),
    m_low (0)
{}

HighPrecision::HighPrecision (int64_t high, int64_t low)
  : m_high (high),
    m_low (low)
{}

HighPrecision::HighPrecision (double value)
  : m_high (0), // XXX
    m_low (0) // XXX
{}

int64_t
HighPrecision::GetHigh (void) const
{
  return m_high;
}
uint32_t
HighPrecision::GetLow (void) const
{
  return m_low;
}

double 
HighPrecision::GetDouble (void) const
{
  return 0.0;
}
bool 
HighPrecision::Add (HighPrecision const &o)
{
  return false;
}
bool 
HighPrecision::Sub (HighPrecision const &o)
{
  return false;
}
bool 
HighPrecision::Mul (HighPrecision const &o)
{
  return false;
}
bool 
HighPrecision::Div (HighPrecision const &o)
{
  return false;
}
int 
HighPrecision::Compare (HighPrecision const &o) const
{
  return 0;
}
HighPrecision 
HighPrecision::Zero (void)
{
  return HighPrecision (0,0);
}

#else /* HIGH_PRECISION_I128 */

#include <cmath>
#include <cassert>

const double HighPrecision::MAX_64 = 18446744073709551615.0;

HighPrecision::HighPrecision ()
  : m_value (0.0)
{}

HighPrecision::HighPrecision (int64_t high, int64_t low)
  : m_value (((double)high) + (((double)low)/MAX_64))
{}

HighPrecision::HighPrecision (double value)
  : m_value (value)
{}

int64_t
HighPrecision::GetHigh (void) const
{
  return (int64_t)floor (m_value);
}
int64_t
HighPrecision::GetLow (void) const
{
  return (int64_t)((m_value - floor (m_value)) * MAX_64);
}

double 
HighPrecision::GetDouble (void) const
{
  return m_value;
}
bool 
HighPrecision::Add (HighPrecision const &o)
{
  m_value += o.m_value;
  return false;
}
bool 
HighPrecision::Sub (HighPrecision const &o)
{
  m_value -= o.m_value;
  return false;
}
bool 
HighPrecision::Mul (HighPrecision const &o)
{
  m_value *= o.m_value;
  return false;
}
bool 
HighPrecision::Div (HighPrecision const &o)
{
  m_value /= o.m_value;
  return false;
}
int 
HighPrecision::Compare (HighPrecision const &o) const
{
  return 0;
}
HighPrecision 
HighPrecision::Zero (void)
{
  return HighPrecision (0,0);
}

#endif /* HIGH_PRECISION_I128 */
