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
#ifndef HIGH_PRECISION_128_H
#define HIGH_PRECISION_128_H

#include <stdint.h>
#include "cairo-wideint-private.h"

namespace ns3 {

/**
 * This should be a high-precision 128bit integer version of
 * HighPrecision class. It should also be able to report 
 * overflow and underflow.
 */
class HighPrecision 
{
public:
  HighPrecision ();
  HighPrecision (int64_t value, bool dummy);
  HighPrecision (double value);
  
  int64_t GetInteger (void) const;
  double GetDouble (void) const;
  bool Add (HighPrecision const &o);
  bool Sub (HighPrecision const &o);
  bool Mul (HighPrecision const &o);
  bool Div (HighPrecision const &o);

  int Compare (HighPrecision const &o) const;
  static HighPrecision Zero (void);
private:
  static const double MAX_64;
  cairo_int128_t m_value;
};

}; // namespace ns3

#endif /* HIGH_PRECISION_128_H */
