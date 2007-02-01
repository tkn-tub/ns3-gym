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
#include <ostream>
#include "high-precision.h"

namespace ns3 {

/**
 * \brief keep track of time unit.
 *
 * This template class is used to keep track of the value 
 * of a specific time unit: the type TimeUnit<1> is used to 
 * keep track of seconds, the type TimeUnit<2> is used to keep
 * track of seconds squared, the type TimeUnit<-1> is used to
 * keep track of 1/seconds, etc.
 *
 * This base class defines all the functionality shared by all
 * these time unit objects: it defines all the classic arithmetic
 * operators +, -, *, /, and all the classic comparison operators:
 * ==, !=, <, >, <=, >=. It is thus easy to add, substract, or 
 * multiply multiple TimeUnit objects. The return type of any such
 * arithmetic expression is always a TimeUnit object.
 *
 * The ns3::Scalar, ns3::Time, ns3::TimeSquare, and ns3::TimeInvert classes 
 * are aliases for the TimeUnit<0>, TimeUnit<1>, TimeUnit<2> and TimeUnit<-1>
 * types respectively.
 *
 * For example:
 * \code
 * Time<1> t1 = Seconds (10.0);
 * Time<1> t2 = Seconds (10.0);
 * Time<2> t3 = t1 * t2;
 * Time<0> t4 = t1 / t2;
 * Time<3> t5 = t3 * t1;
 * Time<-2> t6 = t1 / t5;
 * TimeSquare t7 = t3;
 * Scalar s = t4;
 * \endcode
 *
 * If you try to assign the result of an expression which does not 
 * match the type of the variable it is assigned to, you will get a
 * compiler error. For example, the following will not compile:
 * \code
 * Time<1> = Seconds (10.0) * Seconds (1.5);
 * \endcode
 *
 * You can also use the following non-member functions to manipulate
 * any of these ns3::TimeUnit object:
 *  - \ref ns3-Time-Abs ns3::Abs
 *  - \ref ns3-Time-Max ns3::Max
 *  - \ref ns3-Time-Min ns3::Min
 */
template <int N>
class TimeUnit
{
public:
  TimeUnit ();
  TimeUnit (TimeUnit const &o);
  TimeUnit operator = (TimeUnit const &o);
  TimeUnit (HighPrecision data);

  /**
   * \return true if the time is zero, false otherwise.
   */
  bool IsZero (void) const;
  /**
   * \return true if the time is negative or zero, false otherwise.
   */
  bool IsNegative (void) const;
  /**
   * \return true if the time is positive or zero, false otherwise.
   */
  bool IsPositive (void) const;
  /**
   * \return true if the time is strictly negative, false otherwise.
   */
  bool IsStrictlyNegative (void) const;
  /**
   * \return true if the time is strictly positive, false otherwise.
   */
  bool IsStrictlyPositive (void) const;

  /**
   * This is really an internal method exported for the needs of
   * the implementation. Please, Do not try to use this method, ever.
   *
   * \return the ns3::HighPrecision object which holds the value 
   *         stored in this Time<N> type.
   */
  HighPrecision GetHighPrecision (void) const;
  HighPrecision *PeekHighPrecision (void);

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
HighPrecision *
TimeUnit<N>::PeekHighPrecision (void)
{
  return &m_data;
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
operator != (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  return lhs.GetHighPrecision ().Compare (rhs.GetHighPrecision ()) != 0;
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
  retval.Add (rhs.GetHighPrecision ());
  return TimeUnit<N> (retval);
}
template <int N>
TimeUnit<N> operator - (TimeUnit<N> const &lhs, TimeUnit<N> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  retval.Sub (rhs.GetHighPrecision ());
  return TimeUnit<N> (retval);
}
template <int N1, int N2>
TimeUnit<N1+N2> operator * (TimeUnit<N1> const &lhs, TimeUnit<N2> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  retval.Mul (rhs.GetHighPrecision ());
  return TimeUnit<N1+N2> (retval);
}
template <int N1, int N2>
TimeUnit<N1-N2> operator / (TimeUnit<N1> const &lhs, TimeUnit<N2> const &rhs)
{
  HighPrecision retval = lhs.GetHighPrecision ();
  retval.Div (rhs.GetHighPrecision ());
  return TimeUnit<N1-N2> (retval);
}
template <int N>
TimeUnit<N> &operator += (TimeUnit<N> &lhs, TimeUnit<N> const &rhs) {
  HighPrecision *lhsv = lhs.PeekHighPrecision ();
  lhsv->Add (rhs.GetHighPrecision ());
  return lhs;
}
template <int N>
TimeUnit<N> &operator -= (TimeUnit<N> &lhs, TimeUnit<N> const &rhs) {
  HighPrecision *lhsv = lhs.PeekHighPrecision ();
  lhsv->Sub (rhs.GetHighPrecision ());
  return lhs;
}


/**
 * \anchor ns3-Time-Abs
 * \relates ns3::TimeUnit
 * \param time the input value
 * \returns the absolute value of the input value.
 */
template <int N>
TimeUnit<N> Abs (TimeUnit<N> const &time)
{
  return TimeUnit<N> (Abs (time.GetHighPrecision ()));
}
/**
 * \anchor ns3-Time-Max
 * \relates ns3::TimeUnit
 * \param ta the first value
 * \param tb the seconds value
 * \returns the max of the two input values.
 */
template <int N>
TimeUnit<N> Max (TimeUnit<N> const &ta, TimeUnit<N> const &tb)
{
  HighPrecision a = ta.GetHighPrecision ();
  HighPrecision b = tb.GetHighPrecision ();  
  return TimeUnit<N> (Max (a, b));
}
/**
 * \anchor ns3-Time-Min
 * \relates ns3::TimeUnit
 * \param ta the first value
 * \param tb the seconds value
 * \returns the min of the two input values.
 */
template <int N>
TimeUnit<N> Min (TimeUnit<N> const &ta, TimeUnit<N> const &tb)
{
  HighPrecision a = ta.GetHighPrecision ();
  HighPrecision b = tb.GetHighPrecision ();  
  return TimeUnit<N> (Max (a, b));
}

/**
 * \brief keep track of seconds.
 *
 * This is an instance of type ns3::TimeUnit<1>: it is
 * the return value of the ns3::Simulator::Now method
 * and is needed for the Simulator::Schedule methods
 *
 * Time instances can be created through any of the following classes:
 *  - ns3::Seconds
 *  - ns3::MilliSeconds
 *  - ns3::MicroSeconds
 *  - ns3::NanoSeconds
 *  - ns3::Now
 *
 * Time instances can be added, substracted, multipled and divided using
 * the standard C++ operators (if you make sure to obey the rules
 * of the ns3::TimeUnit class template)
 * To scale a Time instance, you can multiply it with an instance of
 * the ns3::Scalar class.
 * Time instances can also be manipulated through the following non-member 
 * functions:
 *  - \ref ns3-Time-Abs ns3::Abs
 *  - \ref ns3-Time-Max ns3::Max
 *  - \ref ns3-Time-Min ns3::Min
 */
class Time : public TimeUnit<1>
{
public:
  Time ();
  Time (TimeUnit<1> time);
  Time (HighPrecision const& value);

  static Time Seconds (double seconds);

  /**
   * \returns an approximation in seconds of the time stored in this
   *          instance.
   */
  double GetSeconds (void) const;
  /**
   * \returns an approximation in milliseconds of the time stored in this
   *          instance.
   */
  int32_t GetMilliSeconds (void) const;
  /**
   * \returns an approximation in microseconds of the time stored in this
   *          instance.
   */
  int64_t GetMicroSeconds (void) const;
  /**
   * \returns an approximation in nanoseconds of the time stored in this
   *          instance.
   */
  int64_t GetNanoSeconds (void) const;
};

std::ostream& operator<< (std::ostream& os, Time const& time);

/**
 * \brief create ns3::Time instances in units of seconds.
 *
 * For example:
 * \code
 * Time t = Seconds (2.0);
 * Simulator::Schedule (NanoSeconds (5.0), ...);
 * \endcode
 */
Time Seconds (double seconds);

/**
 * \brief create ns3::Time instances in units of milliseconds.
 *
 * For example:
 * \code
 * Time t = MilliSeconds (2);
 * Simulator::Schedule (MilliSeconds (5), ...);
 * \endcode
 */
Time MilliSeconds (uint32_t ms);
/**
 * \brief create ns3::Time instances in units of microseconds.
 *
 * For example:
 * \code
 * Time t = MicroSeconds (2);
 * Simulator::Schedule (MicroSeconds (5), ...);
 * \endcode
 */
Time MicroSeconds (uint64_t us);
/**
 * \brief create ns3::Time instances in units of nanoseconds.
 *
 * For example:
 * \code
 * Time t = NanoSeconds (2);
 * Simulator::Schedule (NanoSeconds (5), ...);
 * \endcode
 */
Time NanoSeconds (uint64_t ns);

/**
 * \brief create an ns3::Time instance which contains the
 *        current simulation time.
 *
 * This is really a shortcut for the ns3::Simulator::Now method.
 * It is typically used as shown below to schedule an event
 * which expires at the absolute time "2 seconds":
 * \code
 * Simulator::Schedule (Seconds (2.0) - Now (), &my_function);
 * \endcode
 */
Time Now (void);

/**
 * \brief hold scalar values
 *
 * This class is used both to construct scalar values to multiply
 * ns3::Time instances and to hold the return value of
 * an expression which returns a scalar. For example, the 
 * following code will output on your terminal 1.5:
 * \code
 * Scalar s0 = Scalar (1.5);
 * Time t1 = Seconds (10.0) * s0;
 * Time t2 = Seconds (10.0) * Scalar (2.5);
 * Scalar s1 = Seconds (15.0) / Seconds (10.0);
 * std::cout << s1.GetDouble () << std::endl;
 * \endcode
 */
class Scalar : public TimeUnit<0>
{
public:
  Scalar ();
  Scalar (double scalar);
  Scalar (TimeUnit<0> scalar);
  double GetDouble (void) const;
};

typedef TimeUnit<-1> TimeInvert;
typedef TimeUnit<2> TimeSquare;


}; // namespace ns3

#endif /* TIME_H */
