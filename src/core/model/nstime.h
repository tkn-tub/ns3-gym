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
#include "unused.h"
#include <stdint.h>
#include <limits>
#include <cmath>
#include <ostream>
#include <set>

namespace ns3 {

class TimeWithUnit;
  
/**
 * \ingroup core
 * \brief Simulation virtual time values and global simulation resolution.
 *
 * This class defines all the classic C++ addition/subtraction
 * operators: +, -, +=, -=; and all the classic comparison operators:
 * ==, !=, <, >, <=, >=. It is thus easy to add, substract, or
 * compare Time objects.
 *
 * For example:
 * \code
 * Time t1 = Seconds (10.0);
 * Time t2 = Seconds (10.0);
 * Time t3 = t1;
 * t3 += t2;
 * \endcode
 *
 * You can also use the following non-member functions to manipulate
 * any of these ns3::Time object:
 *  - \ref Abs()
 *  - \ref Max()
 *  - \ref Min()
 *
 * This class also controls the resolution of the underlying time value.
 * The resolution is the smallest representable time interval.
 * The default resolution is nanoseconds.
 *
 * To change the resolution, use SetResolution().  All Time objects created
 * before the call to SetResolution() will be updated to the new resolution.
 * This can only be done once!  (Tracking each Time object uses 4 pointers.
 * For speed, once we convert the existing instances we discard the recording
 * data structure and stop tracking new instances, so we have no way
 * to do a second conversion.)
 *
 * If you increase the global resolution, you also implicitly decrease
 * the range of your simulation.  The global simulation time is stored
 * in a 64 bit integer, whose interpretation will depend on the global
 * resolution.  Therefore the maximum duration of your simulation,
 * if you use picoseconds, is 2^64 ps = 2^24 s = 7 months, whereas,
 * had you used nanoseconds, you could have run for 584 years.
 */
class Time
{
public:
  /**
   * The unit to use to interpret a number representing time
   */
  enum Unit
  {
    Y   = 0,   //!< year, 365 days
    D   = 1,   //!< day, 24 hours
    H   = 2,   //!< hour, 60 minutes
    MIN = 3,   //!< minute, 60 seconds
    S   = 4,   //!< second
    MS  = 5,   //!< millisecond
    US  = 6,   //!< microsecond
    NS  = 7,   //!< nanosecond
    PS  = 8,   //!< picosecond
    FS  = 9,   //!< femtosecond
    LAST = 10
  };

  inline Time & operator = (const Time & o)
  {
    m_data = o.m_data;
    return *this;
  }
  inline Time ()
    : m_data ()
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  inline Time(const Time & o)
    : m_data (o.m_data)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (double v)
    : m_data (lround (v))
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (int v)
    : m_data (v)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (long int v)
    : m_data (v)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (long long int v)
    : m_data (v)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (unsigned int v)
    : m_data (v)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (unsigned long int v)
    : m_data (v)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  explicit inline Time (unsigned long long int v)
    : m_data (v)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  /**
   * \brief Construct Time object from common time expressions like "1ms"
   *
   * Supported units include:
   * - `s`  (seconds)
   * - `ms` (milliseconds)
   * - `us` (microseconds)
   * - `ns` (nanoseconds)
   * - `ps` (picoseconds)
   * - `fs` (femtoseconds)
   * - `min`  (minutes)
   * - `h`  (hours)
   * - `d`  (days)
   * - `y`  (years)
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
  static Time Min ()
  {
    return Time (std::numeric_limits<int64_t>::min ());
  }
  /**
   * \brief Maximum representable Time
   */
  static Time Max ()
  {
    return Time (std::numeric_limits<int64_t>::max ());
  }

  /**
   *  Destructor
   */
  ~Time ()
  {
    if (g_markingTimes)
      {
        Clear (this);
      }
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
  /**
   *  \return -1,0,+1 if `this < o`, `this == o`, or `this > o`
   */
  inline int Compare (const Time & o) const
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
   * \returns an approximation in minutes of the time stored in this
   *          instance.
   */
  inline double GetMinutes (void) const
  {
    return ToDouble (Time::MIN);
  }
  /**
   * \returns an approximation in hours of the time stored in this
   *          instance.
   */
  inline double GetHours (void) const
  {
    return ToDouble (Time::H);
  }
  /**
   * \returns an approximation in days of the time stored in this
   *          instance.
   */
  inline double GetDays (void) const
  {
    return ToDouble (Time::D);
  }
  /**
   * \returns an approximation in years of the time stored in this
   *          instance.
   */
  inline double GetYears (void) const
  {
    return ToDouble (Time::Y);
  }

  /**
   * \returns the raw time value, in the current units
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
  static inline Time From (const int64x64_t & from, enum Unit timeUnit)
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
  explicit inline Time (const int64x64_t & value)
    : m_data (value.GetHigh ())
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  inline static Time From (const int64x64_t & value)
  {
    return Time (value);
  }

  /**
   * Attach a unit to a Time, to facilitate output in a specific unit.
   *
   * For example,
   * \code
   *   Time t (3.14e9);  // Pi seconds
   *   std::cout << t.As (Time::MS) << std::endl;
   * \endcode
   * will print ``+3140.0ms``
   *
   * \param unit [in] The unit to use.
   */
  TimeWithUnit As (const enum Unit unit) const;

private:
  /**
   * How to convert between other units and the current unit
   */
  struct Information
  {
    bool toMul;                     //!< Multiply when converting To, otherwise divide
    bool fromMul;                   //!< Multiple when converting From, otherwise divide
    int64_t factor;                 //!< Ratio of this unit / current unit
    int64x64_t timeTo;              //!< Multiplier to convert to this unit
    int64x64_t timeFrom;            //!< Multiplier to convert from this unit
  };
  /**
   * Current time unit, and conversion info.
   */
  struct Resolution
  {
    struct Information info[LAST];  //!<  Conversion info from current unit
    enum Time::Unit unit;           //!<  Current time unit
  };

  static inline struct Resolution *PeekResolution (void)
  {
    static struct Time::Resolution resolution = SetDefaultNsResolution ();
    return & resolution;
  }
  static inline struct Information *PeekInformation (enum Unit timeUnit)
  {
    return & (PeekResolution ()->info[timeUnit]);
  }

  static struct Resolution SetDefaultNsResolution (void);
  static void SetResolution (enum Unit unit, struct Resolution *resolution,
                             const bool convert = true);

  /**
   *  Record all instances of Time, so we can rescale them when
   *  the resolution changes.
   *
   *  \internal
   *
   *  We use a std::set so we can remove the record easily when
   *  ~Time() is called.
   *
   *  We don't use Ptr<Time>, because we would have to bloat every Time
   *  instance with SimpleRefCount<Time>.
   *
   *  Seems like this should be std::set< Time * const >, but
   *  [Stack Overflow](http://stackoverflow.com/questions/5526019/compile-errors-stdset-with-const-members)
   *  says otherwise, quoting the standard:
   *
   *  > & sect;23.1/3 states that std::set key types must be assignable
   *  > and copy constructable; clearly a const type will not be assignable.
   */
  typedef std::set< Time * > MarkedTimes;
  /**
   *  Record of outstanding Time objects which will need conversion
   *  when the resolution is set.
   *
   *  \internal
   *
   *  Use a classic static variable so we can check in Time ctors
   *  without a function call.
   *
   *  We'd really like to initialize this here, but we don't want to require
   *  C++0x, so we init in time.cc.  To ensure that happens before first use,
   *  we add a call to StaticInit (below) to every compilation unit which
   *  includes nstime.h.
   */
  static MarkedTimes * g_markingTimes;
public:
  /**
   *  Function to force static initialization of Time
   */
  static bool StaticInit ();
private:

  /* Friend the Simulator class so it can call the private function
     ClearMarkedTimes ()
  */
  friend class Simulator;
  /**
   *  Remove all MarkedTimes.
   *
   *  \internal
   *  Has to be visible to the Simulator class, hence the friending.
   */
  static void ClearMarkedTimes ();
  /**
   *  Record a Time instance with the MarkedTimes
   */
  static void Mark (Time * const time);
  /**
   *  Remove a Time instance from the MarkedTimes, called by ~Time()
   */
  static void Clear (Time * const time);
  /**
   *  Convert existing Times to the new unit.
   */
  static void ConvertTimes (const enum Unit unit);

  friend bool operator == (const Time & lhs, const Time & rhs);
  friend bool operator != (const Time & lhs, const Time & rhs);
  friend bool operator <= (const Time & lhs, const Time & rhs);
  friend bool operator >= (const Time & lhs, const Time & rhs);
  friend bool operator < (const Time & lhs, const Time & rhs);
  friend bool operator > (const Time & lhs, const Time & rhs);
  friend Time operator + (const Time & lhs, const Time & rhs);
  friend Time operator - (const Time & lhs, const Time & rhs);
  friend Time operator * (const Time & lhs, const int64_t & rhs);
  friend Time operator * (const int64_t & lhs, const Time & rhs);
  friend int64_t operator / (const Time & lhs, const Time & rhs);
  friend Time operator / (const Time & lhs, const int64_t & rhs);
  friend Time & operator += (Time & lhs, const Time & rhs);
  friend Time & operator -= (Time & lhs, const Time & rhs);
  friend Time Abs (const Time & time);
  friend Time Max (const Time & ta, const Time & tb);
  friend Time Min (const Time & ta, const Time & tb);


  int64_t m_data;                   //!< Virtual time value, in the current unit.

};  // class Time


/// Force static initialization of Time
static bool NS_UNUSED_GLOBAL (g_TimeStaticInit) = Time::StaticInit ();

inline bool
operator == (const Time & lhs, const Time & rhs)
{
  return lhs.m_data == rhs.m_data;
}
inline bool
operator != (const Time & lhs, const Time & rhs)
{
  return lhs.m_data != rhs.m_data;
}
inline bool
operator <= (const Time & lhs, const Time & rhs)
{
  return lhs.m_data <= rhs.m_data;
}
inline bool
operator >= (const Time & lhs, const Time & rhs)
{
  return lhs.m_data >= rhs.m_data;
}
inline bool
operator < (const Time & lhs, const Time & rhs)
{
  return lhs.m_data < rhs.m_data;
}
inline bool
operator > (const Time & lhs, const Time & rhs)
{
  return lhs.m_data > rhs.m_data;
}
inline Time operator + (const Time & lhs, const Time & rhs)
{
  return Time (lhs.m_data + rhs.m_data);
}
inline Time operator - (const Time & lhs, const Time & rhs)
{
  return Time (lhs.m_data - rhs.m_data);
}
inline Time
operator * (const Time & lhs, const int64_t & rhs)
{
  Time res = lhs;
  res.m_data *= rhs;
  return res;
}
inline Time
operator * (const int64_t & lhs, const Time & rhs)
{
  Time res = rhs;
  res.m_data *= lhs;
  return res;
}
inline int64_t
operator / (const Time & lhs, const Time & rhs)
{
  int64_t res = lhs.m_data / rhs.m_data;
  return res;
}
inline Time
operator / (const Time & lhs, const int64_t & rhs)
{
  Time res = lhs;
  res.m_data /= rhs;
  return res;
}
inline Time & operator += (Time & lhs, const Time & rhs)
{
  lhs.m_data += rhs.m_data;
  return lhs;
}
inline Time & operator -= (Time & lhs, const Time & rhs)
{
  lhs.m_data -= rhs.m_data;
  return lhs;
}

/**
 * Max function for Time.
 * \param ta the first value
 * \param tb the seconds value
 * \returns the max of the two input values.
 */
inline Time Max (const Time & ta, const Time & tb)
{
  return Time ((ta.m_data < tb.m_data) ? tb : ta);
}
/**
 * Min function for Time.
 * \param ta the first value
 * \param tb the seconds value
 * \returns the min of the two input values.
 */
inline Time Min (const Time & ta, const Time & tb)
{
  return Time ((ta.m_data > tb.m_data) ? tb : ta);
}

/**
 * Absolute value function for Time
 * \param time the input value
 * \returns the absolute value of the input value.
 */
inline Time Abs (const Time & time)
{
  return Time ((time.m_data < 0) ? -time.m_data : time.m_data);
}

/**
 * \brief Time output streamer.
 * 
 * Generates output such as "3.96ns".  Times are printed with the
 * following format flags (independent of the stream flags):
 *   - `showpos`
 *   - `fixed`
 *   - `left`
 * The stream `width` and `precision` are ignored; Time output always
 * includes ".0".
 * \relates Time
 */
std::ostream & operator<< (std::ostream & os, const Time & time);
/**
 * \brief Time input streamer
 *
 * Uses the Time::Time (const std::string &) constructor
 * \relates Time
 */
std::istream & operator>> (std::istream & is, Time & time);

/**
 * \brief create ns3::Time instances in units of seconds.
 *
 * For example:
 * \code
 * Time t = Seconds (2.0);
 * Simulator::Schedule (Seconds (5.0), ...);
 * \endcode
 * \param seconds seconds value
 * \relates Time
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
 * \relates Time
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
 * \relates Time
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
 * \relates Time
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
 * \relates Time
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
 * \relates Time
 */
inline Time FemtoSeconds (uint64_t fs)
{
  return Time::FromInteger (fs, Time::FS);
}
/**
 * \brief create ns3::Time instances in units of minutes (equal to 60 seconds).
 *
 * For example:
 * \code
 * Time t = Minutes (2.0);
 * Simulator::Schedule (Minutes (5.0), ...);
 * \endcode
 * \param minutes mintues value
 * \relates Time
 */
inline Time Minutes (double minutes)
{
  return Time::FromDouble (minutes, Time::MIN);
}
/**
 * \brief create ns3::Time instances in units of hours (equal to 60 minutes).
 *
 * For example:
 * \code
 * Time t = Hours (2.0);
 * Simulator::Schedule (Hours (5.0), ...);
 * \endcode
 * \param hours hours value
 * \relates Time
 */
inline Time Hours (double hours)
{
  return Time::FromDouble (hours, Time::H);
}
/**
 * \brief create ns3::Time instances in units of days (equal to 24 hours).
 *
 * For example:
 * \code
 * Time t = Days (2.0);
 * Simulator::Schedule (Days (5.0), ...);
 * \endcode
 * \param days days value
 * \relates Time
 */
inline Time Days (double days)
{
  return Time::FromDouble (days, Time::D);
}
/**
 * \brief create ns3::Time instances in units of years (equal to 365 days).
 *
 * For example:
 * \code
 * Time t = Years (2.0);
 * Simulator::Schedule (Years (5.0), ...);
 * \endcode
 * \param years years value
 * \relates Time
 */
inline Time Years (double years)
{
  return Time::FromDouble (years, Time::Y);
}

/**
 * \see Seconds(double)
 * \relates Time
 */
inline Time Seconds (int64x64_t seconds)
{
  return Time::From (seconds, Time::S);
}
/**
 * \see MilliSeconds(uint64_t)
 * \relates Time
 */
inline Time MilliSeconds (int64x64_t ms)
{
  return Time::From (ms, Time::MS);
}
/**
 * \see MicroSeconds(uint64_t)
 * \relates Time
 */
inline Time MicroSeconds (int64x64_t us)
{
  return Time::From (us, Time::US);
}
/**
 * \see NanoSeconds(uint64_t)
 * \relates Time
 */
inline Time NanoSeconds (int64x64_t ns)
{
  return Time::From (ns, Time::NS);
}
/**
 * \see PicoSeconds(uint64_t)
 * \relates Time
 */
inline Time PicoSeconds (int64x64_t ps)
{
  return Time::From (ps, Time::PS);
}
/**
 * \see FemtoSeconds(uint64_t)
 * \relates Time
 */
inline Time FemtoSeconds (int64x64_t fs)
{
  return Time::From (fs, Time::FS);
}
/**
 * \see Minutes(uint64_t)
 * \relates Time
 */
inline Time Minutes (int64x64_t minutes)
{
  return Time::From (minutes, Time::MIN);
}
/**
 * \see Minutes(uint64_t)
 * \relates Time
 */
inline Time Hours (int64x64_t hours)
{
  return Time::From (hours, Time::H);
}
/**
 * \see Minutes(uint64_t)
 * \relates Time
 */
inline Time Days (int64x64_t days)
{
  return Time::From (days, Time::D);
}
/**
 * \see Minutes(uint64_t)
 * \relates Time
 */
inline Time Years (int64x64_t years)
{
  return Time::From (years, Time::Y);
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
  return MakeTimeChecker (Time::Min (), Time::Max ());
}

/**
 * \brief Helper to make a Time checker with a lower bound.
 *
 * \return the AttributeChecker
 */
inline
Ptr<const AttributeChecker> MakeTimeChecker (const Time min)
{
  return MakeTimeChecker (min, Time::Max ());
}

/**
 * \ingroup time
 * \brief A Time with attached unit, to facilitate output in that unit.
 */
class TimeWithUnit
{
public:
  /**
   * Attach a unit to a Time
   *
   * \param [in] time The time.
   * \param [in] unit The unit to use for output
   */
  TimeWithUnit (const Time time, const Time::Unit unit)
    : m_time (time),
      m_unit (unit)
  { };

private:
  Time m_time;        //!< The time
  Time::Unit m_unit;  //!< The unit to use in output

  /// Output streamer
  friend std::ostream & operator << (std::ostream & os, const TimeWithUnit & time);

};  // class TimeWithUnit

} // namespace ns3

#endif /* TIME_H */
