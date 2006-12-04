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
#ifndef HIGH_PRECISION_DOUBLE_H
#define HIGH_PRECISION_DOUBLE_H

#include <stdint.h>

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
  double m_value;
};

}; // namespace ns3

#endif /* HIGH_PRECISION_DOUBLE_H  */
