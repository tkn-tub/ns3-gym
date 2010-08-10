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
#ifndef TIME_BASE_H
#define TIME_BASE_H

#include "high-precision.h"

namespace ns3 {

/**
 * \ingroup time
 * \brief keep track of global simulation resolution
 *
 * This class is the base class for all time-related classes. It controls
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
 * TimeBase::SetResolution. If you do need to use picoseconds, it's thus best
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
class TimeBase
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
  inline static TimeBase FromInteger (uint64_t value, enum Unit timeUnit);
  /**
   * \param value to convert into a Time object
   * \param timeUnit the unit of the value to convert
   * \return a new Time object
   *
   * \sa FromInteger, ToInteger, ToDouble
   */
  inline static TimeBase FromDouble (double value, enum Unit timeUnit);
  /**
   * \param time a Time object
   * \param timeUnit the unit of the value to return
   *
   * Convert the input time into an integer value according to the requested
   * time unit.
   */
  inline static uint64_t ToInteger (const TimeBase &time, enum Unit timeUnit);
  /**
   * \param time a Time object
   * \param timeUnit the unit of the value to return
   *
   * Convert the input time into a floating point value according to the requested
   * time unit.
   */
  inline static double ToDouble (const TimeBase &time, enum Unit timeUnit);

  inline TimeBase ();
  inline TimeBase (const TimeBase &o);
  inline TimeBase operator = (const TimeBase &o);
  inline TimeBase (const HighPrecision &data);
  /**
   * \return true if the time is zero, false otherwise.
   */
  inline bool IsZero (void) const;
  /**
   * \return true if the time is negative or zero, false otherwise.
   */
  inline bool IsNegative (void) const;
  /**
   * \return true if the time is positive or zero, false otherwise.
   */
  inline bool IsPositive (void) const;
  /**
   * \return true if the time is strictly negative, false otherwise.
   */
  inline bool IsStrictlyNegative (void) const;
  /**
   * \return true if the time is strictly positive, false otherwise.
   */
  inline bool IsStrictlyPositive (void) const;

  /**
   * This is really an internal method exported for the needs of
   * the implementation. Please, Do not try to use this method, ever.
   *
   * \return the ns3::HighPrecision object which holds the value
   *         stored in this instance of Time type.
   */
  inline HighPrecision const &GetHighPrecision (void) const;
  inline HighPrecision * PeekHighPrecision (void);

protected:
  HighPrecision m_data;
private:
  struct Information
  {
    bool toMul;
    bool fromMul;
    uint64_t factor;
    HighPrecision timeTo;
    HighPrecision timeFrom;
  };
  struct Resolution
  {
    struct Information info[LAST];
    enum TimeBase::Unit unit;
  };

  inline static struct Resolution *PeekResolution (void);
  inline static struct Information *PeekInformation (enum Unit timeUnit);
  static struct Resolution GetNsResolution (void);
  static void SetResolution (enum Unit unit, struct Resolution *resolution);
  inline static TimeBase From (HighPrecision tmp, enum Unit timeUnit);
  inline static HighPrecision To (const TimeBase &time, enum Unit timeUnit);
};

} // namespace ns3

namespace ns3 {

TimeBase::TimeBase ()
  : m_data ()
{
}
TimeBase::TimeBase (const TimeBase &o)
  : m_data (o.m_data)
{
}
TimeBase TimeBase::operator = (const TimeBase &o)
{
  m_data = o.m_data;
  return *this;
}
TimeBase::TimeBase (const HighPrecision &data)
  : m_data (data)
{
}

HighPrecision const &
TimeBase::GetHighPrecision (void) const
{
  return m_data;
}

HighPrecision *
TimeBase::PeekHighPrecision (void)
{
  return &m_data;
}

bool
TimeBase::IsZero (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) == 0;
}

bool
TimeBase::IsNegative (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) <= 0;
}

bool
TimeBase::IsPositive (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) >= 0;
}

bool
TimeBase::IsStrictlyNegative (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) < 0;
}

bool
TimeBase::IsStrictlyPositive (void) const
{
  return m_data.Compare (HighPrecision::Zero ()) > 0;
}

struct TimeBase::Resolution *
TimeBase::PeekResolution (void)
{
  static struct TimeBase::Resolution resolution = GetNsResolution ();
  return &resolution;
}
struct TimeBase::Information *
TimeBase::PeekInformation (enum Unit timeUnit)
{
  return &(PeekResolution ()->info[timeUnit]);
}
TimeBase 
TimeBase::From (HighPrecision tmp, enum Unit timeUnit)
{
  struct Information *info = PeekInformation (timeUnit);
  if (info->fromMul)
    {
      tmp.Mul (info->timeFrom);
    }
  else
    {
      tmp.MulByInvert (info->timeFrom);
    }
  return TimeBase (tmp);
}
HighPrecision
TimeBase::To (const TimeBase &time, enum Unit timeUnit)
{
  struct Information *info = PeekInformation (timeUnit);
  HighPrecision tmp = time.GetHighPrecision ();
  if (info->toMul)
    {
      tmp.Mul (info->timeTo);
    }
  else
    {
      tmp.MulByInvert (info->timeTo);
    }
  return tmp;
}

TimeBase
TimeBase::FromInteger (uint64_t value, enum Unit timeUnit)
{
  struct Information *info = PeekInformation (timeUnit);
  if (info->fromMul)
    {
      value *= info->factor;
      return TimeBase (HighPrecision (value, false));
    }
  return From (HighPrecision (value, false), timeUnit);
}
TimeBase
TimeBase::FromDouble (double value, enum Unit timeUnit)
{
  return From (HighPrecision (value), timeUnit);
}
uint64_t 
TimeBase::ToInteger (const TimeBase &time, enum Unit timeUnit)
{
  struct Information *info = PeekInformation (timeUnit);
  uint64_t v = time.m_data.GetInteger ();
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
double 
TimeBase::ToDouble (const TimeBase &time, enum Unit timeUnit)
{
  return To (time, timeUnit).GetDouble ();
}

} // namespace ns3

#endif /* TIME_BASE_H */
