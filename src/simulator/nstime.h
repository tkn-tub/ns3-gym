/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include "ns3/assert.h"
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include <stdint.h>
#include <math.h>
#include <ostream>
#include "high-precision.h"
#include "cairo-wideint-private.h"

namespace ns3 {

namespace TimeStepPrecision {

enum precision_t {
  S  = 0,
  MS = 3,
  US = 6,
  NS = 9,
  PS = 12,
  FS = 15
};
/**
 * \param precision the new precision to use
 *
 * This should be invoked before any Time object 
 * is created. i.e., it should be invoked at the very start
 * of every simulation. The unit specified by this method
 * is used as the unit of the internal simulation time
 * which is stored as a 64 bit integer.
 */
void Set (precision_t precision);
/**
 * \returns the currently-used time precision.
 */
precision_t Get (void);

} // namespace TimeStepPrecision


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
  HighPrecision const &GetHighPrecision (void) const;
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
HighPrecision const &
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
  //    std::cout << lhs.GetHighPrecision().GetInteger() << " * " 
  //              << rhs.GetHighPrecision().GetInteger() 
  //              << " = " << retval.GetInteger() << std::endl;
  return TimeUnit<N1+N2> (retval);
}
template <int N1, int N2>
TimeUnit<N1-N2> operator / (TimeUnit<N1> const &lhs, TimeUnit<N2> const &rhs)
{
  NS_ASSERT (rhs.GetHighPrecision ().GetDouble () != 0);
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

// Explicit instatiation of the TimeUnit template for N=1, with a few
// additional methods that should not be available for N!=1
// \class TimeUnit<1>

class TimeValue;

template <>
class TimeUnit<1>
{
  // -*- New methods -*-
public:

  /**
   * \brief String constructor
   * Construct TimeUnit<1> object from common time expressions like "
   * 1ms" or "10s".  Supported units include:
   * - s  (seconds)
   * - ms (milliseconds)
   * - us (microseconds)
   * - ns (nanoseconds)
   * - ps (picoseconds)
   * - fs (femtoseconds)
   *
   * There can be no white space between the numerical portion
   * and the units.  Any otherwise malformed string causes a fatal error to
   * occur.
   * \param s The string to parse into a TimeUnit<1>
   */
  TimeUnit<1>(const std::string& s);
  /**
   * \returns an approximation in seconds of the time stored in this
   *          instance.
   */
  double GetSeconds (void) const;

  /**
   * \returns an approximation in milliseconds of the time stored in this
   *          instance.
   */  
  int64_t GetMilliSeconds (void) const;
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
  /**
   * \returns an approximation in picoseconds of the time stored in this
   *          instance.
   */
  int64_t GetPicoSeconds (void) const;
  /**
   * \returns an approximation in femtoseconds of the time stored in this
   *          instance.
   */
  int64_t GetFemtoSeconds (void) const;
  /**
   * \returns an approximation of the time stored in this
   *          instance in the units specified in m_tsPrecision.
   */
  int64_t GetTimeStep (void) const;

  // -*- The rest is the the same as in the generic template class -*-
public:
  TimeUnit ()
    : m_data () 
  {}
  TimeUnit (TimeUnit const &o)
    : m_data (o.m_data) {}
  TimeUnit operator = (TimeUnit const &o) {
    m_data = o.m_data;
    return *this;
  }
  TimeUnit (HighPrecision data)
    : m_data (data) {}
  bool IsZero (void) const {
    return m_data.Compare (HighPrecision::Zero ()) == 0;
  }
  bool IsNegative (void) const {
    return m_data.Compare (HighPrecision::Zero ()) <= 0;
  }
  bool IsPositive (void) const {
    return m_data.Compare (HighPrecision::Zero ()) >= 0;
  }
  bool IsStrictlyNegative (void) const {
    return m_data.Compare (HighPrecision::Zero ()) < 0;
  }
  bool IsStrictlyPositive (void) const {
    return m_data.Compare (HighPrecision::Zero ()) > 0;
  }
  HighPrecision const &GetHighPrecision (void) const {
    return m_data;
  }
  HighPrecision *PeekHighPrecision (void) {
    return &m_data;
  }

  static uint64_t UnitsToTimestep (uint64_t unitValue, 
                                   uint64_t unitFactor);

private:
  HighPrecision m_data;

  /*
   * \Returns the value of time_value in units of unitPrec. time_value
   * must be specified in timestep units (which are the same as the
   * m_tsPrecision units
   */
  int64_t ConvertToUnits (int64_t timeValue, uint64_t unitFactor) const;
};

/**
 * \brief keep track of seconds.
 *
 * This is an instance of type ns3::TimeUnit<1>: it is
 * the return value of the ns3::Simulator::Now method
 * and is needed for the Simulator::Schedule methods.
 * The precision of the underlying Time unit can be
 * changed with calls to TimeStepPrecision::Set.
 *
 * Time instances can be created through any of the following functions:
 *  - ns3::Seconds
 *  - ns3::MilliSeconds
 *  - ns3::MicroSeconds
 *  - ns3::NanoSeconds
 *  - ns3::PicoSeconds
 *  - ns3::FemtoSeconds
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
 *
 * The Time class has the following additional methods not available in
 * the generic TimeUnit template:
 * 
 * \code
 * double GetSeconds (void) const;
 * \endcode
 * returns an approximation in seconds of the time stored in this
 *          instance.
 *
 * \code
 * int64_t GetMilliSeconds (void) const;
 * \endcode
 * returns an approximation in milliseconds of the time stored in this
 *          instance.
 *
 * \code
 * int64_t GetMicroSeconds (void) const;
 * \endcode
 * returns an approximation in microseconds of the time stored in this
 *          instance.
 *
 * \code
 * int64_t GetNanoSeconds (void) const;
 * \endcode
 * returns an approximation in nanoseconds of the time stored in this
 *          instance.
 *
 * \code
 * int64_t GetPicoSeconds (void) const;
 * \endcode
 * returns an approximation in picoseconds of the time stored in this
 *          instance.
 *
 * \code
 * int64_t GetFemtoSeconds (void) const;
 * \endcode
 * returns an approximation in femtoseconds of the time stored in this
 *          instance.
 */
typedef TimeUnit<1> Time;


std::ostream& operator<< (std::ostream& os, const Time & time);
std::istream& operator>> (std::istream& is, Time & time);

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
Time MilliSeconds (uint64_t ms);
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
 * \brief create ns3::Time instances in units of picoseconds.
 *
 * For example:
 * \code
 * Time t = PicoSeconds (2);
 * Simulator::Schedule (PicoSeconds (5), ...);
 * \endcode
 */
Time PicoSeconds (uint64_t ps);
/**
 * \brief create ns3::Time instances in units of femtoseconds.
 *
 * For example:
 * \code
 * Time t = FemtoSeconds (2);
 * Simulator::Schedule (FemtoSeconds (5), ...);
 * \endcode
 */
Time FemtoSeconds (uint64_t fs);

// internal function not publicly documented
Time TimeStep (uint64_t ts);

// Explicit instatiation of the TimeUnit template for N=0, with a few
// additional methods that should not be available for N != 0
template <>
class TimeUnit<0>
{
  // -*- New methods -*-
public:
  double GetDouble (void) const;
  TimeUnit<0> (double scalar);

  // -*- The rest is the the same as in the generic template class -*-
public:
  TimeUnit ()
    : m_data () 
  {};
  TimeUnit (TimeUnit const &o)
    : m_data (o.m_data) {}
  TimeUnit operator = (TimeUnit const &o) {
    m_data = o.m_data;
    return *this;
  }
  TimeUnit (HighPrecision data)
    : m_data (data) {}
  bool IsZero (void) const {
    return m_data.Compare (HighPrecision::Zero ()) == 0;
  }
  bool IsNegative (void) const {
    return m_data.Compare (HighPrecision::Zero ()) <= 0;
  }
  bool IsPositive (void) const {
    return m_data.Compare (HighPrecision::Zero ()) >= 0;
  }
  bool IsStrictlyNegative (void) const {
    return m_data.Compare (HighPrecision::Zero ()) < 0;
  }
  bool IsStrictlyPositive (void) const {
    return m_data.Compare (HighPrecision::Zero ()) > 0;
  }
  HighPrecision const &GetHighPrecision (void) const {
    return m_data;
  }
  HighPrecision *PeekHighPrecision (void) {
    return &m_data;
  }

private:
  HighPrecision m_data;
};

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
 *
 * The Scalar class has the following additional methods not available in
 * the generic TimeUnit template:
 * \code
 * double GetDouble (void) const;
 * \endcode
 * returns the C double contained in the Scalar instance
 *
 * \code
 * Scalar(double scalar);
 * \endcode
 * Constructs a Scalar instance from a C double.
 */
typedef TimeUnit<0> Scalar;

typedef TimeUnit<-1> TimeInvert;
typedef TimeUnit<2> TimeSquare;

/**
 * \class ns3::TimeValue
 * \brief hold objects of type ns3::Time
 */


ATTRIBUTE_ACCESSOR_DEFINE (Time);
ATTRIBUTE_VALUE_DEFINE (Time);
ATTRIBUTE_CHECKER_DEFINE (Time);

} // namespace ns3

#endif /* TIME_H */
