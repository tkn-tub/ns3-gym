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

#include "assert.h"
#include "attribute.h"
#include "attribute-helper.h"
#include "int64x64.h"
#include <stdint.h>
#include <climits>
#include <cmath>
#include <ostream>

namespace ns3 {

/**
 * \ingroup core
 * \defgroup time Time
 */
/**
 * \ingroup time
 * \brief keep track of time values and allow control of global simulation resolution
 *
 * This class defines all the classic C++ arithmetic
 * operators +, -, *, /, and all the classic comparison operators:
 * ==, !=, <, >, <=, >=. It is thus easy to add, substract, or
 * multiply Time objects.
 *
 * For example:
 * \code
 * Time t1 = Seconds (10.0);
 * Time t2 = Seconds (10.0);
 * Time t3 = t1 * t2;
 * Time t4 = t1 / t2;
 * Time t5 = t3 * t1;
 * Time t6 = t1 / t5;
 * Time t7 = t3;
 * \endcode
 *
 * You can also use the following non-member functions to manipulate
 * any of these ns3::Time object:
 *  - \ref ns3-Time-Abs ns3::Abs
 *  - \ref ns3-Time-Max ns3::Max
 *  - \ref ns3-Time-Min ns3::Min
 *
 * This class also controls
 * the resolution of the underlying time value . The default resolution
 * is nanoseconds. That is, TimeStep (1).GetNanoSeconds () will return
 * 1. It is possible to either increase or decrease the resolution and the
 * code tries really hard to make this easy.
 *
 * If your resolution is X (say, nanoseconds) and if you create Time objects 
 * with a lower resolution (say, picoseconds), don't expect that this 
 * code will return 1: PicoSeconds (1).GetPicoSeconds (). It will most 
 * likely return 0 because the Time object has only 64 bits of fractional 
 * precision which means that PicoSeconds (1) is stored as a 64-bit aproximation
 * of 1/1000 in the Time object. If you later multiply it again by the exact 
 * value 1000, the result is unlikely to be 1 exactly. It will be close to
 * 1 but not exactly 1.
 * 
 * In general, it is thus a really bad idea to try to use time objects of a
 * resolution higher than the global resolution controlled through 
 * Time::SetResolution. If you do need to use picoseconds, it's thus best
 * to switch the global resolution to picoseconds to avoid nasty surprises.
 *
 * Another important issue to keep in mind is that if you increase the
 * global resolution, you also implicitely decrease the range of your simulation.
 * i.e., the global simulation time is stored in a 64 bit integer whose interpretation
 * will depend on the global resolution so, 2^64 picoseconds which is the maximum
 * duration of your simulation if the global resolution is picoseconds 
 * is smaller than 2^64 nanoseconds which is the maximum duration of your simulation
 * if the global resolution is nanoseconds.
 *
 * Finally, don't even think about ever changing the global resolution after
 * creating Time objects: all Time objects created before the call to SetResolution
 * will contain values which are not updated to the new resolution. In practice,
 * the default value for the attributes of many models is indeed calculated
 * before the main function of the main program enters. Because of this, if you
 * use one of these models (and it's likely), it's going to be hard to change
 * the global simulation resolution in a way which gives reasonable results. This
 * issue has been filed as bug 954 in the ns-3 bugzilla installation.
 */
class Time
{
public:
  /**
   * The unit to use to interpret a number representing time
   */
  enum Unit
  {
    S  = 0,
    MS = 1,
    US = 2,
    NS = 3,
    PS = 4,
    FS = 5,
    LAST = 6
  };

  inline Time &operator = (const Time &o)
  {
    m_data = o.m_data;
    return *this;
  }
  inline Time ()
    : m_data ()
  {}
  inline Time(const Time &o)
    : m_data (o.m_data)
  {}
  explicit inline Time (double v)
    : m_data (lround (v))
  {}
  explicit inline Time (int v)
    : m_data (v)
  {}
  explicit inline Time (long int v)
    : m_data (v)
  {}
  explicit inline Time (long long int v)
    : m_data (v)
  {}
  explicit inline Time (unsigned int v)
    : m_data (v)
  {}
  explicit inline Time (unsigned long int v)
    : m_data (v)
  {}
  explicit inline Time (unsigned long long int v)
    : m_data (v)
  {}

  /**
   * \brief String constructor
   * Construct Time object from common time expressions like "
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
   * \param s The string to parse into a Time
   */
  explicit Time (const std::string & s);

  /**
   * \brief Minimum representable Time
   */
  static Time MIN ()
  {
    return Time ((long long int)LLONG_MIN);
  }
  /**
   * \brief Maximum representable Time
   */
  static Time MAX ()
  {
    return Time ((long long int)LLONG_MAX);
  }
  
  /**
   * \return true if the time is zero, false otherwise.
   */
  inline bool IsZero (void) const
  {
    return m_data == 0;
  }
  /**
   * \return true if the time is negative or zero, false otherwise.
   */
  inline bool IsNegative (void) const
  {
    return m_data <= 0;
  }
  /**
   * \return true if the time is positive or zero, false otherwise.
   */
  inline bool IsPositive (void) const
  {
    return m_data >= 0;
  }
  /**
   * \return true if the time is strictly negative, false otherwise.
   */
  inline bool IsStrictlyNegative (void) const
  {
    return m_data < 0;
  }
  /**
   * \return true if the time is strictly positive, false otherwise.
   */
  inline bool IsStrictlyPositive (void) const
  {
    return m_data > 0;
  }

  inline int Compare (const Time &o) const
  {
    return (m_data < o.m_data) ? -1 : (m_data == o.m_data) ? 0 : 1;
  }

  /**
   * \returns an approximation in seconds of the time stored in this
   *          instance.
   */
  inline double GetSeconds (void) const
  {
    return ToDouble (Time::S);
  }

  /**
   * \returns an approximation in milliseconds of the time stored in this
   *          instance.
   */
  inline int64_t GetMilliSeconds (void) const
  {
    return ToInteger (Time::MS);
  }
  /**
   * \returns an approximation in microseconds of the time stored in this
   *          instance.
   */
  inline int64_t GetMicroSeconds (void) const
  {
    return ToInteger (Time::US);
  }
  /**
   * \returns an approximation in nanoseconds of the time stored in this
   *          instance.
   */
  inline int64_t GetNanoSeconds (void) const
  {
    return ToInteger (Time::NS);
  }
  /**
   * \returns an approximation in picoseconds of the time stored in this
   *          instance.
   */
  inline int64_t GetPicoSeconds (void) const
  {
    return ToInteger (Time::PS);
  }
  /**
   * \returns an approximation in femtoseconds of the time stored in this
   *          instance.
   */
  inline int64_t GetFemtoSeconds (void) const
  {
    return ToInteger (Time::FS);
  }
  /**
   * \returns an approximation of the time stored in this
   *          instance in the units specified in m_tsPrecision.
   */
  inline int64_t GetTimeStep (void) const
  {
    return m_data;
  }
  inline double GetDouble (void) const
  {
    return m_data;
  }
  inline int64_t GetInteger (void) const
  {
    return GetTimeStep ();
  }


  /**
   * \param resolution the new resolution to use
   *
   * Change the global resolution used to convert all 
   * user-provided time values in Time objects and Time objects
   * in user-expected time units.
   */
  static void SetResolution (enum Unit resolution);
  /**
   * \returns the current global resolution.
   */
  static enum Unit GetResolution (void);
  /**
   * \param value to convert into a Time object
   * \param timeUnit the unit of the value to convert
   * \return a new Time object
   *
   * This method interprets the input value according to the input
   * unit and constructs a matching Time object.
   *
   * \sa FromDouble, ToDouble, ToInteger
   */
  inline static Time FromInteger (uint64_t value, enum Unit timeUnit)
  {
    struct Information *info = PeekInformation (timeUnit);
    if (info->fromMul)
      {
        value *= info->factor;
      }
    else
      {
        value /= info->factor;
      }
    return Time (value);
  }
  /**
   * \param timeUnit the unit of the value to return
   * \return int64_t time value
   *
   * Convert the input time into an integer value according to the requested
   * time unit.
   */
  inline int64_t ToInteger (enum Unit timeUnit) const
  {
    struct Information *info = PeekInformation (timeUnit);
    int64_t v = m_data;
    if (info->toMul)
      {
        v *= info->factor;
      }
    else
      {
        v /= info->factor; 
      }
    return v;
  }
  /**
   * \param value to convert into a Time object
   * \param timeUnit the unit of the value to convert
   * \return a new Time object
   *
   * \sa FromInteger, ToInteger, ToDouble
   */
  inline static Time FromDouble (double value, enum Unit timeUnit)
  {
    return From (int64x64_t (value), timeUnit);
  }
  /**
   * \param timeUnit the unit of the value to return
   * \return double time value
   *
   * Convert the input time into a floating point value according to the requested
   * time unit.
   */
  inline double ToDouble (enum Unit timeUnit) const
  {
    return To (timeUnit).GetDouble ();
  }
  static inline Time From (const int64x64_t &from, enum Unit timeUnit)
  {
    struct Information *info = PeekInformation (timeUnit);
    // DO NOT REMOVE this temporary variable. It's here
    // to work around a compiler bug in gcc 3.4
    int64x64_t retval = from; 
    if (info->fromMul)
      {
        retval *= info->timeFrom;
      }
    else
      {
        retval.MulByInvert (info->timeFrom);
      }
    return Time (retval);
  }
  inline int64x64_t To (enum Unit timeUnit) const
  {
    struct Information *info = PeekInformation (timeUnit);
    int64x64_t retval = int64x64_t (m_data);
    if (info->toMul)
      {
        retval *= info->timeTo;
      }
    else
      {
        retval.MulByInvert (info->timeTo);
      }
    return retval;
  }
  inline operator int64x64_t () const
  {
    return int64x64_t (m_data);
  }
  explicit inline Time (const int64x64_t &value)
    : m_data (value.GetHigh ())
  {}
  inline static Time From (const int64x64_t &value)
  {
    return Time (value);
  }

private:
  struct Information
  {
    bool toMul;
    bool fromMul;
    uint64_t factor;
    int64x64_t timeTo;
    int64x64_t timeFrom;
  };
  struct Resolution
  {
    struct Information info[LAST];
    enum Time::Unit unit;
  };

  static inline struct Resolution *PeekResolution (void)
  {
    static struct Time::Resolution resolution = GetNsResolution ();
    return &resolution;
  }
  static inline struct Information *PeekInformation (enum Unit timeUnit)
  {
    return &(PeekResolution ()->info[timeUnit]);
  }

  static struct Resolution GetNsResolution (void);
  static void SetResolution (enum Unit unit, struct Resolution *resolution);

  friend bool operator == (const Time &lhs, const Time &rhs);
  friend bool operator != (const Time &lhs, const Time &rhs);
  friend bool operator <= (const Time &lhs, const Time &rhs);
  friend bool operator >= (const Time &lhs, const Time &rhs);
  friend bool operator < (const Time &lhs, const Time &rhs);
  friend bool operator > (const Time &lhs, const Time &rhs);
  friend Time operator + (const Time &lhs, const Time &rhs);
  friend Time operator - (const Time &lhs, const Time &rhs);
  friend Time &operator += (Time &lhs, const Time &rhs);
  friend Time &operator -= (Time &lhs, const Time &rhs);
  friend Time Abs (const Time &time);
  friend Time Max (const Time &ta, const Time &tb);
  friend Time Min (const Time &ta, const Time &tb);

  int64_t m_data;
};

inline bool
operator == (const Time &lhs, const Time &rhs)
{
  return lhs.m_data == rhs.m_data;
}
inline bool
operator != (const Time &lhs, const Time &rhs)
{
  return lhs.m_data != rhs.m_data;
}
inline bool
operator <= (const Time &lhs, const Time &rhs)
{
  return lhs.m_data <= rhs.m_data;
}
inline bool
operator >= (const Time &lhs, const Time &rhs)
{
  return lhs.m_data >= rhs.m_data;
}
inline bool
operator < (const Time &lhs, const Time &rhs)
{
  return lhs.m_data < rhs.m_data;
}
inline bool
operator > (const Time &lhs, const Time &rhs)
{
  return lhs.m_data > rhs.m_data;
}
inline Time operator + (const Time &lhs, const Time &rhs)
{
  return Time (lhs.m_data + rhs.m_data);
}
inline Time operator - (const Time &lhs, const Time &rhs)
{
  return Time (lhs.m_data - rhs.m_data);
}
inline Time &operator += (Time &lhs, const Time &rhs)
{
  lhs.m_data += rhs.m_data;
  return lhs;
}
inline Time &operator -= (Time &lhs, const Time &rhs)
{
  lhs.m_data -= rhs.m_data;
  return lhs;
}

/**
 * \anchor ns3-Time-Abs
 * \relates ns3::TimeUnit
 * \param time the input value
 * \returns the absolute value of the input value.
 */
inline Time Abs (const Time &time)
{
  return Time ((time.m_data < 0) ? -time.m_data : time.m_data);
}
/**
 * \anchor ns3-Time-Max
 * \relates ns3::TimeUnit
 * \param ta the first value
 * \param tb the seconds value
 * \returns the max of the two input values.
 */
inline Time Max (const Time &ta, const Time &tb)
{
  return Time ((ta.m_data < tb.m_data) ? tb : ta);
}
/**
 * \anchor ns3-Time-Min
 * \relates ns3::TimeUnit
 * \param ta the first value
 * \param tb the seconds value
 * \returns the min of the two input values.
 */
inline Time Min (const Time &ta, const Time &tb)
{
  return Time ((ta.m_data > tb.m_data) ? tb : ta);
}


std::ostream& operator<< (std::ostream& os, const Time & time);
std::istream& operator>> (std::istream& is, Time & time);

/**
 * \brief create ns3::Time instances in units of seconds.
 *
 * For example:
 * \code
 * Time t = Seconds (2.0);
 * Simulator::Schedule (Seconds (5.0), ...);
 * \endcode
 * \param seconds seconds value
 */
inline Time Seconds (double seconds)
{
  return Time::FromDouble (seconds, Time::S);
}

/**
 * \brief create ns3::Time instances in units of milliseconds.
 *
 * For example:
 * \code
 * Time t = MilliSeconds (2);
 * Simulator::Schedule (MilliSeconds (5), ...);
 * \endcode
 * \param ms milliseconds value
 */
inline Time MilliSeconds (uint64_t ms)
{
  return Time::FromInteger (ms, Time::MS);
}
/**
 * \brief create ns3::Time instances in units of microseconds.
 *
 * For example:
 * \code
 * Time t = MicroSeconds (2);
 * Simulator::Schedule (MicroSeconds (5), ...);
 * \endcode
 * \param us microseconds value
 */
inline Time MicroSeconds (uint64_t us)
{
  return Time::FromInteger (us, Time::US);
}
/**
 * \brief create ns3::Time instances in units of nanoseconds.
 *
 * For example:
 * \code
 * Time t = NanoSeconds (2);
 * Simulator::Schedule (NanoSeconds (5), ...);
 * \endcode
 * \param ns nanoseconds value
 */
inline Time NanoSeconds (uint64_t ns)
{
  return Time::FromInteger (ns, Time::NS);
}
/**
 * \brief create ns3::Time instances in units of picoseconds.
 *
 * For example:
 * \code
 * Time t = PicoSeconds (2);
 * Simulator::Schedule (PicoSeconds (5), ...);
 * \endcode
 * \param ps picoseconds value
 */
inline Time PicoSeconds (uint64_t ps)
{
  return Time::FromInteger (ps, Time::PS);
}
/**
 * \brief create ns3::Time instances in units of femtoseconds.
 *
 * For example:
 * \code
 * Time t = FemtoSeconds (2);
 * Simulator::Schedule (FemtoSeconds (5), ...);
 * \endcode
 * \param fs femtoseconds value
 */
inline Time FemtoSeconds (uint64_t fs)
{
  return Time::FromInteger (fs, Time::FS);
}


inline Time Seconds (int64x64_t seconds)
{
  return Time::From (seconds, Time::S);
}
inline Time MilliSeconds (int64x64_t ms)
{
  return Time::From (ms, Time::MS);
}
inline Time MicroSeconds (int64x64_t us)
{
  return Time::From (us, Time::US);
}
inline Time NanoSeconds (int64x64_t ns)
{
  return Time::From (ns, Time::NS);
}
inline Time PicoSeconds (int64x64_t ps)
{
  return Time::From (ps, Time::PS);
}
inline Time FemtoSeconds (int64x64_t fs)
{
  return Time::From (fs, Time::FS);
}

// internal function not publicly documented
inline Time TimeStep (uint64_t ts)
{
  return Time (ts);
}

/**
 * \class ns3::TimeValue
 * \brief hold objects of type ns3::Time
 */


ATTRIBUTE_VALUE_DEFINE (Time);
ATTRIBUTE_ACCESSOR_DEFINE (Time);

/**
 * \brief Helper to make a Time checker with bounded range.
 * Both limits are inclusive
 *
 * \return the AttributeChecker 
 */
Ptr<const AttributeChecker> MakeTimeChecker (const Time min, const Time max);

/**
 * \brief Helper to make an unbounded Time checker.
 *
 * \return the AttributeChecker
 */
inline
Ptr<const AttributeChecker> MakeTimeChecker (void)
{
  return MakeTimeChecker (Time::MIN (), Time::MAX ());
}

/**
 * \brief Helper to make a Time checker with an upper bound
 *
 * \return the AttributeChecker
 */
inline
Ptr<const AttributeChecker> MakeTimeChecker (const Time min)
{
  return MakeTimeChecker (min, Time::MAX ());
}


} // namespace ns3

#endif /* TIME_H */
