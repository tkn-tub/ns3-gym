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
#include "high-precision-128.h"

namespace ns3 {

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
int64_t
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


}; // namespace ns3
