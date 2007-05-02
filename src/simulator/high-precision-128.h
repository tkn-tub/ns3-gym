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

/**
 * This file contains an implementation of the HighPrecision class.
 * Each instance of the Time class also contains an instance of this
 * class which is used to perform all the arithmetic operations of
 * the Time class.
 *
 * This code is a bit ugly with a lot of inline methods for speed:
 * profiling this code on anything but the simplest scenarios shows
 * that it is a big bottleneck if great care in its implementation
 * is not performed. My observations are that what dominates are 
 * Division operations (there are really really super costly)
 * and Comparison operations (because there are typically a lot of
 * these in any complex timekeeping code).
 *
 * So, the code tries really hard to perform any of these 128 bit 
 * operations by doing all arithmetic on 64 bit integers when possible
 * (i.e., when there is no fractional part. This is a very common case).
 * Hence, the following code has a m_fastValue (64 bits) and a 
 * m_slowValue (128 bits). m_fastValue is used by default and the code
 * converts it to a m_slowValue when needed.
 *
 * If you want to monitor the efficiency of this strategy, you can 
 * enable the macro HP128INC below and call the HighPrecision::PrintStats
 * method at the end of the simulation.
 */


#define noGATHER_STATISTICS 1

#ifdef GATHER_STATISTICS
#define HP128INC(x) x++
#else
#define HP128INC(x)
#endif

namespace ns3 {

class HighPrecision 
{
public:
  inline HighPrecision ();
  inline HighPrecision (int64_t value, bool dummy);
  HighPrecision (double value);

  static void PrintStats (void);
  
  inline int64_t GetInteger (void) const;
  inline double GetDouble (void) const;
  inline bool Add (HighPrecision const &o);
  inline bool Sub (HighPrecision const &o);
  inline bool Mul (HighPrecision const &o);
  bool Div (HighPrecision const &o);

  inline int Compare (HighPrecision const &o) const;
  inline static HighPrecision Zero (void);
private:
  int64_t SlowGetInteger (void) const;
  double SlowGetDouble (void) const;
  bool SlowAdd (HighPrecision const &o);
  bool SlowSub (HighPrecision const &o);
  bool SlowMul (HighPrecision const &o);
  int SlowCompare (HighPrecision const &o) const;
  inline void EnsureSlow (void);

  static const double MAX_64;
  bool m_isFast;
  int64_t m_fastValue;
  cairo_int128_t m_slowValue;

#ifdef GATHER_STATISTICS
  static int m_nfastadds;
  static int m_nfastsubs;
  static int m_nfastmuls;
  static int m_nfastcmps;
  static int m_nfastgets;
  static int m_nslowadds;
  static int m_nslowsubs;
  static int m_nslowmuls;
  static int m_nslowcmps;
  static int m_nslowgets;
  static int m_ndivs;
  static int m_nconversions;
#endif /* GATHER_STATISTICS */
};

}; // namespace ns3

namespace ns3 {

HighPrecision::HighPrecision ()
  : m_isFast (true),
    m_fastValue (0)
{}

HighPrecision::HighPrecision (int64_t value, bool dummy)
  : m_isFast (true),
    m_fastValue (value)
{}


int64_t 
HighPrecision::GetInteger (void) const
{
  if (m_isFast)
    {
      HP128INC (m_nfastgets);
      return m_fastValue;
    }
  else
    {
      HP128INC (m_nslowgets);
      return SlowGetInteger ();
    }
}
double HighPrecision::GetDouble (void) const
{
  if (m_isFast)
    {
      HP128INC (m_nfastgets);
      double retval = m_fastValue;
      return retval;
    }
  else
    {
      HP128INC (m_nslowgets);
      return SlowGetDouble ();
    }
}
bool 
HighPrecision::Add (HighPrecision const &o)
{
  if (m_isFast && o.m_isFast)
    {
      HP128INC (m_nfastadds);
      m_fastValue += o.m_fastValue;
      return false;
    }
  else
    {
      HP128INC (m_nslowadds);
      return SlowAdd (o);
    }
}
bool 
HighPrecision::Sub (HighPrecision const &o)
{
  if (m_isFast && o.m_isFast)
    {
      HP128INC (m_nfastsubs);
      m_fastValue -= o.m_fastValue;
      return false;
    }
  else
    {
      HP128INC (m_nslowsubs);
      return SlowSub (o);
    }
}
bool 
HighPrecision::Mul (HighPrecision const &o)
{
  if (m_isFast && o.m_isFast)
    {
      HP128INC (m_nfastmuls);
      m_fastValue *= o.m_fastValue;
      return false;
    }
  else
    {
      HP128INC (m_nslowmuls);
      return SlowMul (o);
    }
}

int 
HighPrecision::Compare (HighPrecision const &o) const
{
  if (m_isFast && o.m_isFast)
    {
      HP128INC (m_nfastcmps);
      if (m_fastValue < o.m_fastValue)
        {
          return -1;
        }
      else if (m_fastValue == o.m_fastValue)
        {
          return 0;
        }
      else
        {
          return +1;
        }
    }
  else
    {
      HP128INC (m_nslowcmps);
      return SlowCompare (o);
    }

}
HighPrecision 
HighPrecision::Zero (void)
{
  return HighPrecision ();
}


}; // namespace ns3

#endif /* HIGH_PRECISION_128_H */
