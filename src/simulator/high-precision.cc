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

#include <cmath>
#include <cassert>


namespace ns3 {

HighPrecision Abs (HighPrecision const &value)
{
  if (value.Compare (HighPrecision::Zero ()) <= 0)
    {
      HighPrecision v = HighPrecision::Zero ();
      v.Sub (value);
      return v;
    }
  else
    {
      return value;
    }
}
HighPrecision Max (HighPrecision const &a, HighPrecision const &b)
{
  if (a.Compare (b) >= 0)
    {
      return a;
    }
  else
    {
      return b;
    }
}
HighPrecision Min (HighPrecision const &a, HighPrecision const &b)
{
  if (a.Compare (b) <= 0)
    {
      return a;
    }
  else
    {
      return b;
    }
}

}; /* namespace ns3 */
