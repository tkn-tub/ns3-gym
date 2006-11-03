/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <cassert>
#include "high-precision.h"

namespace ns3 {

template <int N>
class TimeUnit
{
public:
  TimeUnit ();
  TimeUnit (TimeUnit const &o);
  TimeUnit operator = (TimeUnit const &o);
  TimeUnit (HighPrecision data);
  HighPrecision GetHighPrecision (void) const;

  bool IsZero (void) const;
  bool IsNegative (void) const;
  bool IsPositive (void) const;
  bool IsStrictlyNegative (void) const;
  bool IsStrictlyPositive (void) const;

private:
  HighPrecision m_data;
};

template <int N>
TimeUnit<N>::TimeUnit ()
  : m_data ()
{}
template <int N>
TimeUnit<N>::TimeUnit (TimeUnit const &o)
  : m_data (o.m_data)
{}
template <int N>
TimeUnit<N>
TimeUnit<N>::operator = (TimeUnit const &o)
{
  m_data = o.m_data;
  return *this;
}
template <int N>
TimeUnit<N>::TimeUnit (HighPrecision data)
  : m_data (data)
{}

template <int N>
HighPrecision
TimeUnit<N>::GetHighPrecision (void) const
{
  return m_data;
}
template <int N>
bool
TimeUnit<N>::IsZero (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) == 0;
}
template <int N>
bool 
TimeUnit<N>::IsNegative (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) <= 0;
}
template <int N>
bool 
TimeUnit<N>::IsPositive (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) >= 0;
}
template <int N>
bool 
TimeUnit<N>::IsStrictlyNegative (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) < 0;
}
template <int N>
bool 
TimeUnit<N>::IsStrictlyPositive (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) > 0;
}

template <int N>
bool 
operator == (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  return lhs.GetHighPrecision ().Compare (rhs.GetHighPrecision ()) == 0;
}
template <int N>
bool 
operator <= (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  return lhs.GetHighPrecision ().Compare (rhs.GetHighPrecision ()) <= 0;
}
template <int N>
bool 
operator >= (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  return lhs.GetHighPrecision ().Compare (rhs.GetHighPrecision ()) >= 0;
}
template <int N>
bool 
operator < (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  return lhs.GetHighPrecision ().Compare (rhs.GetHighPrecision ()) < 0;
}
template <int N>
bool 
operator > (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  return lhs.GetHighPrecision ().Compare (rhs.GetHighPrecision ()) > 0;
}
template <int N>
TimeUnit<N> operator + (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  bool overflow = retval.Add (rhs.GetHighPrecision ());
  assert (!overflow);
  return TimeUnit<N> (retval);
}
template <int N>
TimeUnit<N> operator - (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  bool overflow = retval.Sub (rhs.GetHighPrecision ());
  assert (!overflow);
  return TimeUnit<N> (retval);
}
template <int N1, int N2>
TimeUnit<N1+N2> operator * (TimeUnit<N1> const &lhs, TimeUnit<N2> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  bool overflow = retval.Mul (rhs.GetHighPrecision ());
  assert (!overflow);
  return TimeUnit<N1+N2> (retval);
}
template <int N1, int N2>
TimeUnit<N1-N2> operator / (TimeUnit<N1> const &lhs, TimeUnit<N2> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  bool underflow = retval.Div (rhs.GetHighPrecision ());
  assert (!underflow);
  return TimeUnit<N1-N2> (retval);
}

template <int N>
TimeUnit<N> Abs (TimeUnit<N> const &time)
{
  return TimeUnit<N> (Abs (time.GetHighPrecision ()));
}
template <int N>
TimeUnit<N> Max (TimeUnit<N> const &ta, TimeUnit<N> const &tb)
{
  HighPrecision a = ta.GetHighPrecision ();
  HighPrecision b = tb.GetHighPrecision ();  
  return TimeUnit<N> (Max (a, b));
}
template <int N>
TimeUnit<N> Min (TimeUnit<N> const &ta, TimeUnit<N> const &tb)
{
  HighPrecision a = ta.GetHighPrecision ();
  HighPrecision b = tb.GetHighPrecision ();  
  return TimeUnit<N> (Max (a, b));
}


class Time : public TimeUnit<1>
{
public:
  Time ();
  Time (TimeUnit<1> time);

  double ApproximateToSeconds (void) const;
  int32_t ApproximateToMilliSeconds (void) const;
  int64_t ApproximateToMicroSeconds (void) const;
  int64_t ApproximateToNanoSeconds (void) const;
};


class Seconds : public TimeUnit<1>
{
public:
  Seconds ();
  Seconds (double seconds);
};

class MilliSeconds : public TimeUnit<1>
{
public:
  MilliSeconds ();
  MilliSeconds (uint32_t ms);
};
class MicroSeconds : public TimeUnit<1>
{
public:
  MicroSeconds ();
  MicroSeconds (uint32_t ms);
};
class NanoSeconds : public TimeUnit<1>
{
public:
  NanoSeconds ();
  NanoSeconds (uint32_t ms);
};

class Now : public Time
{
public:
  Now ();
};


class Scalar : public TimeUnit<0>
{
public:
  Scalar ();
  Scalar (double scalar);
  Scalar (TimeUnit<0> scalar);
  double Get (void) const;
};

typedef TimeUnit<-1> TimeInvert;
typedef TimeUnit<2> TimeSquare;


}; // namespace ns3

#endif /* TIME_H */
