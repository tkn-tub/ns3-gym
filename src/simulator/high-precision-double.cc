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
#include "high-precision-double.h"

#include <cmath>
#include "ns3/assert.h"


namespace ns3 {

HighPrecision::HighPrecision ()
  : m_value (0.0)
{}

HighPrecision::HighPrecision (int64_t value, bool dummy)
  : m_value ((double)value)
{}

HighPrecision::HighPrecision (double value)
  : m_value (value)
{}

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

}; // namespace ns3
