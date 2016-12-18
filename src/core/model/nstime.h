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

/**
 * \file
 * \ingroup time
 * Declaration of classes ns3::Time and ns3::TimeWithUnit,
 * and the TimeValue implementation classes.
 */

namespace ns3 {

class TimeWithUnit;
  
/**
 * \ingroup core
 * \defgroup time Virtual Time
 * \brief Management of virtual time in real world units.
 *
 * The underlying simulator is unit agnostic, just dealing with
 * dimensionless virtual time.  Models usually need to handle
 * time in real world units, such as seconds, and conversions/scaling
 * between different units, between minutes and seconds, for example.
 *
 * The convenience constructors in the \ref timecivil "Standard Units" module
 * make it easy to create Times in specific units.
 *
 * The Time::SetResolution() function allows a one-time change of the
 * base resolution, before Simulator::Run().
 */
/**
 * \ingroup time
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
 *  - Abs(Time)
 *  - Max(Time,Time)
 *  - Min(Time,Time)
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

  /**
   *  Assignment operator
   * \param [in] o Time to assign.
   * \return The Time.
   */      
  inline Time & operator = (const Time & o)
  {
    m_data = o.m_data;
    return *this;
  }
  /** Default constructor, with value 0. */
  inline Time ()
    : m_data ()
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  /**
   *  Copy constructor
   *
   * \param [in] o Time to copy
   */      
  inline Time(const Time & o)
    : m_data (o.m_data)
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  /**
   * \name Numeric constructors.
   *  Construct from a numeric value.
   * @{
   */
  /**
   *  Construct from a numeric value.
   *  The current time resolution will be assumed as the unit.
   *  \param [in] v The value.
   */
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
  explicit inline Time (const int64x64_t & v)
    : m_data (v.GetHigh ())
  {
    if (g_markingTimes)
      {
	Mark (this);
      }
  }
  /**@}*/
  
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
   * \param [in] s The string to parse into a Time
   */
  explicit Time (const std::string & s);

  /** Minimum representable Time */
  static Time Min ()
  {
    return Time (std::numeric_limits<int64_t>::min ());
  }
  /** Maximum representable Time */
  static Time Max ()
  {
    return Time (std::numeric_limits<int64_t>::max ());
  }

  /** Destructor */
  ~Time ()
  {
    if (g_markingTimes)
      {
        Clear (this);
      }
  }

  /** \return \c true if the time is zero, \c false otherwise. */
  inline bool IsZero (void) const
  {
    return m_data == 0;
  }
  /** \return \c true if the time is negative or zero, \c false otherwise. */
  inline bool IsNegative (void) const
  {
    return m_data <= 0;
  }
  /** \return \c true if the time is positive or zero, \c false otherwise. */
  inline bool IsPositive (void) const
  {
    return m_data >= 0;
  }
  /** \return \c true if the time is strictly negative, \c false otherwise. */
  inline bool IsStrictlyNegative (void) const
  {
    return m_data < 0;
  }
  /** \return \c true if the time is strictly positive, \c false otherwise. */
  inline bool IsStrictlyPositive (void) const
  {
    return m_data > 0;
  }
  /**
   *  Compare \p this to another Time
   *
   *  \param [in] o The other Time
   *  \return -1,0,+1 if `this < o`, `this == o`, or `this > o`
   */
  inline int Compare (const Time & o) const
  {
    return (m_data < o.m_data) ? -1 : (m_data == o.m_data) ? 0 : 1;
  }

  /**
   * \name Convert to Number in a Unit.
   * Convert a Time to number, in indicated units.
   *
   * Conversions to seconds and larger will return doubles, with
   * possible loss of precision.  Conversions to units smaller than
   * seconds will by rounded.
   *
   * @{
   */
  /**
   * Get an approximation of the time stored in this instance
   * in the indicated unit.
   *
   * \return An approximate value in the indicated unit.
   */
  inline double GetYears (void) const
  {
    return ToDouble (Time::Y);
  }
  inline double GetDays (void) const
  {
    return ToDouble (Time::D);
  }
  inline double GetHours (void) const
  {
    return ToDouble (Time::H);
  }
  inline double GetMinutes (void) const
  {
    return ToDouble (Time::MIN);
  }
  inline double GetSeconds (void) const
  {
    return ToDouble (Time::S);
  }
  inline int64_t GetMilliSeconds (void) const
  {
    return ToInteger (Time::MS);
  }
  inline int64_t GetMicroSeconds (void) const
  {
    return ToInteger (Time::US);
  }
  inline int64_t GetNanoSeconds (void) const
  {
    return ToInteger (Time::NS);
  }
  inline int64_t GetPicoSeconds (void) const
  {
    return ToInteger (Time::PS);
  }
  inline int64_t GetFemtoSeconds (void) const
  {
    return ToInteger (Time::FS);
  }
  /**@}*/

  /**
   * \name Convert to Raw Value.
   * Convert a Time to a number in the current resolution units.
   *
   * @{
   */
  /**
   * Get the raw time value, in the current resolution unit.
   * \returns The raw time value
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
  /**@}*/


  /**
   * \param [in] resolution The new resolution to use
   *
   * Change the global resolution used to convert all
   * user-provided time values in Time objects and Time objects
   * in user-expected time units.
   */
  static void SetResolution (enum Unit resolution);
  /**
   * \returns The current global resolution.
   */
  static enum Unit GetResolution (void);

  
  /**
   *  Create a Time in the current unit.
   *
   *  \param [in] value The value of the new Time.
   *  \return A Time with \p value in the current time unit.
   */
  inline static Time From (const int64x64_t & value)
  {
    return Time (value);
  }
  /**
   * \name Create Times from Values and Units.
   * Create Times from values given in the indicated units.
   *
   * @{
   */
  /**
   *  Create a Time equal to \p value  in unit \c unit
   *
   *  \param [in] value The new Time value, expressed in \c unit
   *  \param [in] unit The unit of \p value
   *  \return The Time representing \p value in \c unit
   */
  inline static Time FromInteger (uint64_t value, enum Unit unit)
  {
    struct Information *info = PeekInformation (unit);
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
  inline static Time FromDouble (double value, enum Unit unit)
  {
    return From (int64x64_t (value), unit);
  }
  inline static Time From (const int64x64_t & value, enum Unit unit)
  {
    struct Information *info = PeekInformation (unit);
    // DO NOT REMOVE this temporary variable. It's here
    // to work around a compiler bug in gcc 3.4
    int64x64_t retval = value;
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
  /**@}*/


  /**
   * \name Get Times as Numbers in Specified Units
   * Get the Time as integers or doubles in the indicated unit.
   *
   * @{
   */
  /**
   *  Get the Time value expressed in a particular unit.
   *
   *  \param [in] unit The desired unit
   *  \return The Time expressed in \p unit
   */
  inline int64_t ToInteger (enum Unit unit) const
  {
    struct Information *info = PeekInformation (unit);
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
  inline double ToDouble (enum Unit unit) const
  {
    return To (unit).GetDouble ();
  }
  inline int64x64_t To (enum Unit unit) const
  {
    struct Information *info = PeekInformation (unit);
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
  /**@}*/

  
  /** Cast to int64x64_t */
  inline operator int64x64_t () const
  {
    return int64x64_t (m_data);
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
   * \param [in] unit The unit to use.
   * \return The Time with embedded unit.
   */
  TimeWithUnit As (const enum Unit unit) const;

private:
  /** How to convert between other units and the current unit. */
  struct Information
  {
    bool toMul;                     //!< Multiply when converting To, otherwise divide
    bool fromMul;                   //!< Multiple when converting From, otherwise divide
    int64_t factor;                 //!< Ratio of this unit / current unit
    int64x64_t timeTo;              //!< Multiplier to convert to this unit
    int64x64_t timeFrom;            //!< Multiplier to convert from this unit
  };
  /** Current time unit, and conversion info. */
  struct Resolution
  {
    struct Information info[LAST];  //!<  Conversion info from current unit
    enum Time::Unit unit;           //!<  Current time unit
  };

  /**
   *  Get the current Resolution
   *
   * \return A pointer to the current Resolution
   */
  static inline struct Resolution *PeekResolution (void)
  {
    static struct Time::Resolution resolution = SetDefaultNsResolution ();
    return & resolution;
  }
  /**
   *  Get the Information record for \p timeUnit for the current Resolution
   *
   *  \param [in] timeUnit The Unit to get Information for
   *  \return The Information for \p timeUnit
   */
  static inline struct Information *PeekInformation (enum Unit timeUnit)
  {
    return & (PeekResolution ()->info[timeUnit]);
  }

  /**
   *  Set the default resolution
   *
   * \return The Resolution object for the default resolution.
   */
  static struct Resolution SetDefaultNsResolution (void);
  /**
   *  Set the current Resolution.
   *
   *  \param [in] unit The unit to use as the new resolution.
   *  \param [in,out] resolution The Resolution record to update.
   *  \param [in] convert Whether to convert existing Time objects to the new resolution.
   */
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
   *  Function to force static initialization of Time.
   *
   * \return \c true on the first call
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
   *  Record a Time instance with the MarkedTimes.
   *  \param [in] time The Time instance to record.
   */
  static void Mark (Time * const time);
  /**
   *  Remove a Time instance from the MarkedTimes, called by ~Time().
   *  \param [in] time The Time instance to remove.
   */
  static void Clear (Time * const time);
  /**
   *  Convert existing Times to the new unit.
   *  \param [in] unit The Unit to convert existing Times to.
   */
  static void ConvertTimes (const enum Unit unit);

  /*
   * \name Arithmetic Operators
   * Arithmetic operators between Times, and scaling by scalars.
   */
  /**
   * @{
   *  Arithmetic operator.
   *  \param [in] lhs Left hand argument
   *  \param [in] rhs Right hand argument
   *  \return The result of the operator.
   */
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
  /** @} */

  /**
   *  Absolute value function for Time
   *  \param [in] time The input value
   *  \returns The absolute value of the input value.
   */
  friend Time Abs (const Time & time);
  /**
   *  Max function for Time.
   *  \param [in] ta The first value
   *  \param [in] tb The seconds value
   *  \returns The max of the two input values.
   */
  friend Time Max (const Time & ta, const Time & tb);
  /**
   *  Min function for Time.
   *  \param [in] ta The first value
   *  \param [in] tb The seconds value
   *  \returns The min of the two input values.
   */
  friend Time Min (const Time & ta, const Time & tb);

  int64_t m_data;  //!< Virtual time value, in the current unit.

};  // class Time

namespace TracedValueCallback {
  
  /**
   * TracedValue callback signature for Time
   *
   * \param [in] oldValue Original value of the traced variable
   * \param [in] newValue New value of the traced variable
   */
  typedef void (* Time)(Time oldValue, Time newValue);
  
}  // namespace TracedValueCallback

/// Force static initialization of Time
static bool NS_UNUSED_GLOBAL (g_TimeStaticInit) = Time::StaticInit ();

/**
 * \ingroup time
 * \brief Equality operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns true if the two input values are equal.
 */
inline bool
operator == (const Time & lhs, const Time & rhs)
{
  return lhs.m_data == rhs.m_data;
}
/**
 * \ingroup time
 * \brief Inequality operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns true if the two input values not are equal.
 */
inline bool
operator != (const Time & lhs, const Time & rhs)
{
  return lhs.m_data != rhs.m_data;
}
/**
 * \ingroup time
 * \brief Less than or equal operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns true if the first input value is less than or equal to the second input value.
 */
inline bool
operator <= (const Time & lhs, const Time & rhs)
{
  return lhs.m_data <= rhs.m_data;
}
/**
 * \ingroup time
 * \brief Greater than or equal operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns true if the first input value is greater than or equal to the second input value.
 */
inline bool
operator >= (const Time & lhs, const Time & rhs)
{
  return lhs.m_data >= rhs.m_data;
}
/**
 * \ingroup time
 * \brief Less than operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns true if the first input value is less than the second input value.
 */
inline bool
operator < (const Time & lhs, const Time & rhs)
{
  return lhs.m_data < rhs.m_data;
}
/**
 * \ingroup time
 * \brief Greater than operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns true if the first input value is greater than the second input value.
 */
inline bool
operator > (const Time & lhs, const Time & rhs)
{
  return lhs.m_data > rhs.m_data;
}
/**
 * \ingroup time
 * \brief Addition operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the sum of the two input values.
 */
inline Time operator + (const Time & lhs, const Time & rhs)
{
  return Time (lhs.m_data + rhs.m_data);
}
/**
 * \ingroup time
 * \brief Difference operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The seconds value
 * \returns the difference of the two input values.
 */
inline Time operator - (const Time & lhs, const Time & rhs)
{
  return Time (lhs.m_data - rhs.m_data);
}
/**
 * \ingroup time
 * \brief Multiplication operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the product of the two input values.
 */
inline Time
operator * (const Time & lhs, const int64_t & rhs)
{
  Time res = lhs;
  res.m_data *= rhs;
  return res;
}
/**
 * \ingroup time
 * \brief Multiplication operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the product of the two input values.
 */
inline Time
operator * (const int64_t & lhs, const Time & rhs)
{
  Time res = rhs;
  res.m_data *= lhs;
  return res;
}
/**
 * \ingroup time
 * \brief Division operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the resultof the first input value divided by the second input value.
 */
inline int64_t
operator / (const Time & lhs, const Time & rhs)
{
  int64_t res = lhs.m_data / rhs.m_data;
  return res;
}
/**
 * \ingroup time
 * \brief Division operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the resultof the first input value divided by the second input value.
 */
inline Time
operator / (const Time & lhs, const int64_t & rhs)
{
  Time res = lhs;
  res.m_data /= rhs;
  return res;
}
/**
 * \ingroup time
 * \brief Addition operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the result of the first input value plus the second input value.
 */
inline Time & operator += (Time & lhs, const Time & rhs)
{
  lhs.m_data += rhs.m_data;
  return lhs;
}
/**
 * \ingroup time
 * \brief Subtraction operator for Time.
 * \param [in] lhs The first value
 * \param [in] rhs The second value
 * \returns the result of the first input value minus the second input value.
 */
inline Time & operator -= (Time & lhs, const Time & rhs)
{
  lhs.m_data -= rhs.m_data;
  return lhs;
}
/**@}*/
  
inline Time Abs (const Time & time)
{
  return Time ((time.m_data < 0) ? -time.m_data : time.m_data);
}
inline Time Max (const Time & ta, const Time & tb)
{
  return Time ((ta.m_data < tb.m_data) ? tb : ta);
}
inline Time Min (const Time & ta, const Time & tb)
{
  return Time ((ta.m_data > tb.m_data) ? tb : ta);
}

/**
 * \ingroup time
 * \brief Time output streamer.
 * 
 * Generates output such as "3.96ns".  Times are printed with the
 * following format flags (independent of the stream flags):
 *   - `showpos`
 *   - `fixed`
 *   - `left`
 * The stream `width` and `precision` are ignored; Time output always
 * includes ".0".
 *
 * \param [in,out] os The output stream.
 * \param [in] time The Time to put on the stream.
 * \return The stream.
 */
std::ostream & operator << (std::ostream & os, const Time & time);
/**
 * \ingroup time
 * \brief Time input streamer
 *
 * Uses the Time::Time (const std::string &) constructor
 *
 * \param [in,out] is The input stream.
 * \param [out] time The Time variable to set from the stream data.
 * \return The stream.
 */
std::istream & operator >> (std::istream & is, Time & time);

/**
 * \ingroup time
 * \defgroup timecivil Standard Time Units.
 * \brief Convenience constructors in standard units.
 *
 * For example:
 * \code
 *   Time t = Seconds (2.0);
 *   Simulator::Schedule (Seconds (5.0), ...);
 * \endcode
 */
/**
 * \ingroup timecivil
 * Construct a Time in the indicated unit.
 * \param [in] value The value
 * \return The Time
 * @{
 */
inline Time Years (double value)
{
  return Time::FromDouble (value, Time::Y);
}
inline Time Years (int64x64_t value)
{
  return Time::From (value, Time::Y);
}
inline Time Days (double value)
{
  return Time::FromDouble (value, Time::D);
}
inline Time Days (int64x64_t value)
{
  return Time::From (value, Time::D);
}
inline Time Hours (double value)
{
  return Time::FromDouble (value, Time::H);
}
inline Time Hours (int64x64_t value)
{
  return Time::From (value, Time::H);
}
inline Time Minutes (double value)
{
  return Time::FromDouble (value, Time::MIN);
}
inline Time Minutes (int64x64_t value)
{
  return Time::From (value, Time::MIN);
}
inline Time Seconds (double value)
{
  return Time::FromDouble (value, Time::S);
}
inline Time Seconds (int64x64_t value)
{
  return Time::From (value, Time::S);
}
inline Time MilliSeconds (uint64_t value)
{
  return Time::FromInteger (value, Time::MS);
}
inline Time MilliSeconds (int64x64_t value)
{
  return Time::From (value, Time::MS);
}
inline Time MicroSeconds (uint64_t value)
{
  return Time::FromInteger (value, Time::US);
}
inline Time MicroSeconds (int64x64_t value)
{
  return Time::From (value, Time::US);
}
inline Time NanoSeconds (uint64_t value)
{
  return Time::FromInteger (value, Time::NS);
}
inline Time NanoSeconds (int64x64_t value)
{
  return Time::From (value, Time::NS);
}
inline Time PicoSeconds (uint64_t value)
{
  return Time::FromInteger (value, Time::PS);
}
inline Time PicoSeconds (int64x64_t value)
{
  return Time::From (value, Time::PS);
}
inline Time FemtoSeconds (uint64_t value)
{
  return Time::FromInteger (value, Time::FS);
}
inline Time FemtoSeconds (int64x64_t value)
{
  return Time::From (value, Time::FS);
}
/**@}*/
  

/**
 *  \ingroup time
 *  \internal Scheduler interface
 *  \param [in] ts The time value, in the current unit.
 *  \return A Time.
 */
inline Time TimeStep (uint64_t ts)
{
  return Time (ts);
}

ATTRIBUTE_VALUE_DEFINE (Time);
ATTRIBUTE_ACCESSOR_DEFINE (Time);

/**
 *  \ingroup time
 *  \brief Helper to make a Time checker with bounded range.
 *  Both limits are inclusive
 *
 *  \param [in] min Minimum allowed value.
 *  \param [in] max Maximum allowed value.
 *  \return The AttributeChecker
 */
Ptr<const AttributeChecker> MakeTimeChecker (const Time min, const Time max);

/**
 * \ingroup time
 * \brief Helper to make an unbounded Time checker.
 *
 * \return The AttributeChecker
 */
inline
Ptr<const AttributeChecker> MakeTimeChecker (void)
{
  return MakeTimeChecker (Time::Min (), Time::Max ());
}

/**
 * \ingroup time
 * \brief Helper to make a Time checker with a lower bound.
 *
 *  \param [in] min Minimum allowed value.
 * \return The AttributeChecker
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

  /**
   *  Output streamer
   *  \param [in,out] os The stream.
   *  \param [in] timeU The Time with desired unit
   *  \returns The stream.
   */
  friend std::ostream & operator << (std::ostream & os, const TimeWithUnit & timeU);

};  // class TimeWithUnit

} // namespace ns3

#endif /* TIME_H */
