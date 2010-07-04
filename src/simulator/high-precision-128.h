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

//#define HP_INLINE inline
#define HP_INLINE

namespace ns3 {

class HighPrecision
{
public:
  HP_INLINE HighPrecision ();
  HP_INLINE HighPrecision (int64_t value, bool dummy);
  HP_INLINE HighPrecision (double value);

  HP_INLINE int64_t GetInteger (void) const;
  HP_INLINE double GetDouble (void) const;
  HP_INLINE bool Add (HighPrecision const &o);
  HP_INLINE bool Sub (HighPrecision const &o);
  HP_INLINE bool Mul (HighPrecision const &o);
  HP_INLINE bool Div (HighPrecision const &o);

  HP_INLINE int Compare (HighPrecision const &o) const;
  HP_INLINE static HighPrecision Zero (void);
private:
  int128_t m_value;
};

} // namespace ns3

#endif /* HIGH_PRECISION_128_H */
