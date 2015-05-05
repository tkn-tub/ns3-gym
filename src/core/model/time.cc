/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * Copyright (c) 2007 Emmanuelle Laprise
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
 * TimeStep support by Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */
#include "nstime.h"
#include "abort.h"
#include "system-mutex.h"
#include "log.h"
#include <cmath>
#include <iomanip>  // showpos
#include <sstream>

/**
 * \file
 * \ingroup time
 * Implementation of classes ns3::Time and ns3::TimeWithUnit,
 * and the TimeValue implementation classes.
 */

namespace ns3 {

NS_LOG_COMPONENT_DEFINE_MASK ("Time", ns3::LOG_PREFIX_TIME);

// The set of marked times
// static
Time::MarkedTimes * Time::g_markingTimes = 0;

/**
 * \internal
 * Get mutex for critical sections around modification of Time::g_markingTimes
 *
 * \returns The static mutex to control access to Time::g_markingTimes.
 *
 * \relates Time
 */
SystemMutex &
GetMarkingMutex ()
{
  static SystemMutex g_markingMutex;
  return g_markingMutex;
}


// Function called to force static initialization
// static
bool Time::StaticInit ()
{
  static bool firstTime = true;

  CriticalSection critical (GetMarkingMutex ());

  if (firstTime)
    {
      if (! g_markingTimes)
        {
          static MarkedTimes markingTimes;
          g_markingTimes = & markingTimes;
        }
      else
        {
          NS_LOG_ERROR ("firstTime but g_markingTimes != 0");
        }

      // Schedule the cleanup.
      // We'd really like:
      //   NS_LOG_LOGIC ("scheduling ClearMarkedTimes()");
      //   Simulator::Schedule ( Seconds (0), & ClearMarkedTimes);
      //   [or even better:  Simulator::AtStart ( & ClearMarkedTimes ); ]
      // But this triggers a static initialization order error,
      // since the Simulator static initialization may not have occurred.
      // Instead, we call ClearMarkedTimes directly from Simulator::Run ()
      firstTime = false;
    }

  return firstTime;
}


Time::Time (const std::string& s)
{
  NS_LOG_FUNCTION (this << &s);
  std::string::size_type n = s.find_first_not_of ("+-0123456789.");
  if (n != std::string::npos)
    { // Found non-numeric
      std::istringstream iss;
      iss.str (s.substr (0, n));
      double r;
      iss >> r;
      std::string trailer = s.substr (n, std::string::npos);
      if (trailer == std::string ("s"))
        {
          *this = Time::FromDouble (r, Time::S);
        }
      else if (trailer == std::string ("ms"))
        {
          *this = Time::FromDouble (r, Time::MS);
        }
      else if (trailer == std::string ("us"))
        {
          *this = Time::FromDouble (r, Time::US);
        }
      else if (trailer == std::string ("ns"))
        {
          *this = Time::FromDouble (r, Time::NS);
        }
      else if (trailer == std::string ("ps"))
        {
          *this = Time::FromDouble (r, Time::PS);
        }
      else if (trailer == std::string ("fs"))
        {
          *this = Time::FromDouble (r, Time::FS);
        }
      else if (trailer == std::string ("min"))
        {
          *this = Time::FromDouble (r, Time::MIN);
        }
      else if (trailer == std::string ("h"))
        {
          *this = Time::FromDouble (r, Time::H);
        }
      else if (trailer == std::string ("d"))
        {
          *this = Time::FromDouble (r, Time::D);
        }
      else if (trailer == std::string ("y"))
        {
          *this = Time::FromDouble (r, Time::Y);
        }
      else
        {
          NS_ABORT_MSG ("Can't Parse Time " << s);
        }
    }
  else
    {
      // they didn't provide units, assume seconds
      std::istringstream iss;
      iss.str (s);
      double v;
      iss >> v;
      *this = Time::FromDouble (v, Time::S);
    }

  if (g_markingTimes)
    {
      Mark (this);
    }
}

// static
struct Time::Resolution
Time::SetDefaultNsResolution (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  struct Resolution resolution;
  SetResolution (Time::NS, &resolution, false);
  return resolution;
}

// static
void
Time::SetResolution (enum Unit resolution)
{
  NS_LOG_FUNCTION (resolution);
  SetResolution (resolution, PeekResolution ());
}


// static
void
Time::SetResolution (enum Unit unit, struct Resolution *resolution,
                     const bool convert /* = true */)
{
  NS_LOG_FUNCTION (resolution);
  if (convert)
    {
      // We have to convert existing Times with the old
      // conversion values, so do it first
      ConvertTimes (unit);
    }

  // Y, D, H, MIN, S, MS, US, NS, PS, FS
  const int8_t power [LAST] = { 17, 17, 17, 16, 15, 12, 9, 6, 3, 0 };
  const int32_t coefficient [LAST] = { 315360, 864, 36, 6, 1, 1, 1, 1, 1, 1 };
  for (int i = 0; i < Time::LAST; i++)
    {
      int shift = power[i] - power[(int)unit];
      int quotient = 1;
      if (coefficient[i] > coefficient[(int) unit])
        {
          quotient = coefficient[i] / coefficient[(int) unit];
          NS_ASSERT (quotient * coefficient[(int) unit] == coefficient[i]);
        }
      else if (coefficient[i] < coefficient[(int) unit])
        {
          quotient = coefficient[(int) unit] / coefficient[i];
          NS_ASSERT (quotient * coefficient[i] == coefficient[(int) unit]);
        }
      NS_LOG_DEBUG ("SetResolution for unit " << (int) unit << " loop iteration " << i
    		    << " has shift " << shift << " has quotient " << quotient);
      int64_t factor = static_cast<int64_t> (std::pow (10, std::fabs (shift)) * quotient);
      double realFactor = std::pow (10, (double) shift)
                        * static_cast<double> (coefficient[i]) / coefficient[(int) unit];
      NS_LOG_DEBUG ("SetResolution factor " << factor << " real factor " << realFactor);
      struct Information *info = &resolution->info[i];
      info->factor = factor;
      // here we could equivalently check for realFactor == 1.0 but it's better
      // to avoid checking equality of doubles
      if (shift == 0 && quotient == 1)
        {
          info->timeFrom = int64x64_t (1);
          info->timeTo = int64x64_t (1);
          info->toMul = true;
          info->fromMul = true;
        }
      else if (realFactor > 1)
        {
          info->timeFrom = int64x64_t (factor);
          info->timeTo = int64x64_t::Invert (factor);
          info->toMul = false;
          info->fromMul = true;
        }
      else
        {
          NS_ASSERT (realFactor < 1);
          info->timeFrom = int64x64_t::Invert (factor);
          info->timeTo = int64x64_t (factor);
          info->toMul = true;
          info->fromMul = false;
        }
    }
  resolution->unit = unit;
}


// static
void
Time::ClearMarkedTimes ()
{
  /**
   * \internal
   *
   * We're called by Simulator::Run, which knows nothing about the mutex,
   * so we need a critical section here.
   *
   * It would seem natural to use this function at the end of
   * ConvertTimes, but that function already has the mutex.
   * Our SystemMutex throws a fatal error if we try to lock it more than
   * once in the same thread (at least in the unix implementation),
   * so calling this function from ConvertTimes is a bad idea.
   *
   * Instead, we copy this body into ConvertTimes.
   */

  CriticalSection critical (GetMarkingMutex ());

  NS_LOG_FUNCTION_NOARGS ();
  if (g_markingTimes)
    {
      NS_LOG_LOGIC ("clearing MarkedTimes");
      g_markingTimes->erase (g_markingTimes->begin(), g_markingTimes->end ());
      g_markingTimes = 0;
    }
}  // Time::ClearMarkedTimes


// static
void
Time::Mark (Time * const time)
{
  CriticalSection critical (GetMarkingMutex ());

  NS_LOG_FUNCTION (time);
  NS_ASSERT (time != 0);

  // Repeat the g_markingTimes test here inside the CriticalSection,
  // since earlier test was outside and might be stale.
  if (g_markingTimes)
    {
      std::pair< MarkedTimes::iterator, bool> ret;

      ret = g_markingTimes->insert ( time);
      NS_LOG_LOGIC ("\t[" << g_markingTimes->size () << "] recording " << time);

      if (ret.second == false)
        {
          NS_LOG_WARN ("already recorded " << time << "!");
        }
   }
}  // Time::Mark ()


// static
void
Time::Clear (Time * const time)
{
  CriticalSection critical (GetMarkingMutex ());

  NS_LOG_FUNCTION (time);
  NS_ASSERT (time != 0);

  if (g_markingTimes)
    {
      NS_ASSERT_MSG (g_markingTimes->count (time) == 1,
                     "Time object " << time <<
                     " registered " << g_markingTimes->count (time) <<
                     " times (should be 1)." );

      MarkedTimes::size_type num = g_markingTimes->erase (time);
      if (num != 1)
        {
          NS_LOG_WARN ("unexpected result erasing " << time << "!");
          NS_LOG_WARN ("got " << num << ", expected 1");
        }
      else
        {
          NS_LOG_LOGIC ("\t[" << g_markingTimes->size () << "] removing  " << time);
        }
    }
}  // Time::Clear ()


// static
void
Time::ConvertTimes (const enum Unit unit)
{
  CriticalSection critical (GetMarkingMutex ());

  NS_LOG_FUNCTION_NOARGS();

  NS_ASSERT_MSG (g_markingTimes != 0,
                 "No MarkedTimes registry. "
                 "Time::SetResolution () called more than once?");

  for ( MarkedTimes::iterator it = g_markingTimes->begin();
        it != g_markingTimes->end();
        it++ )
    {
      Time * const tp = *it;
      if ( ! (    (tp->m_data == std::numeric_limits<int64_t>::min ())
               || (tp->m_data == std::numeric_limits<int64_t>::max ())
             )
         )
        {
      tp->m_data = tp->ToInteger (unit);
    }
    }

  NS_LOG_LOGIC ("logged " << g_markingTimes->size () << " Time objects.");

  // Body of ClearMarkedTimes
  // Assert above already guarantees g_markingTimes != 0
  NS_LOG_LOGIC ("clearing MarkedTimes");
  g_markingTimes->erase (g_markingTimes->begin(), g_markingTimes->end ());
  g_markingTimes = 0;

}  // Time::ConvertTimes ()


// static
enum Time::Unit
Time::GetResolution (void)
{
  // No function log b/c it interferes with operator<<
  return PeekResolution ()->unit;
}


TimeWithUnit
Time::As (const enum Unit unit) const
{
  return TimeWithUnit (*this, unit);
}
 

std::ostream &
operator << (std::ostream & os, const Time & time)
{
  os << time.As (Time::GetResolution ());
  return os;
}


std::ostream &
operator << (std::ostream & os, const TimeWithUnit & timeU)
{
  std::string unit;

  switch (timeU.m_unit)
    {
    case Time::Y:    unit = "y";    break;
    case Time::D:    unit = "d";    break;
    case Time::H:    unit = "h";    break;
    case Time::MIN:  unit = "min";  break;
    case Time::S:    unit = "s";    break;
    case Time::MS:   unit = "ms";   break;
    case Time::US:   unit = "us";   break;
    case Time::NS:   unit = "ns";   break;
    case Time::PS:   unit = "ps";   break;
    case Time::FS:   unit = "fs";   break;

    case Time::LAST:
    default:
      NS_ABORT_MSG ("can't be reached");
      unit = "unreachable";
      break;
    }

  int64x64_t v = timeU.m_time.To (timeU.m_unit);
  os << v << unit;
  
  return os;
}


std::istream &
operator >> (std::istream & is, Time & time)
{
  std::string value;
  is >> value;
  time = Time (value);
  return is;
}

ATTRIBUTE_VALUE_IMPLEMENT (Time);

Ptr<const AttributeChecker>
MakeTimeChecker (const Time min, const Time max)
{
  NS_LOG_FUNCTION (min << max);

  struct Checker : public AttributeChecker
  {
    Checker (const Time minValue, const Time maxValue)
      : m_minValue (minValue),
        m_maxValue (maxValue) {}
    virtual bool Check (const AttributeValue &value) const {
      NS_LOG_FUNCTION (&value);
      const TimeValue *v = dynamic_cast<const TimeValue *> (&value);
      if (v == 0)
        {
          return false;
        }
      return v->Get () >= m_minValue && v->Get () <= m_maxValue;
    }
    virtual std::string GetValueTypeName (void) const {
      NS_LOG_FUNCTION_NOARGS ();
      return "ns3::TimeValue";
    }
    virtual bool HasUnderlyingTypeInformation (void) const {
      NS_LOG_FUNCTION_NOARGS ();
      return true;
    }
    virtual std::string GetUnderlyingTypeInformation (void) const {
      NS_LOG_FUNCTION_NOARGS ();
      std::ostringstream oss;
      oss << "Time" << " " << m_minValue << ":" << m_maxValue;
      return oss.str ();
    }
    virtual Ptr<AttributeValue> Create (void) const {
      NS_LOG_FUNCTION_NOARGS ();
      return ns3::Create<TimeValue> ();
    }
    virtual bool Copy (const AttributeValue &source, AttributeValue &destination) const {
      NS_LOG_FUNCTION (&source << &destination);
      const TimeValue *src = dynamic_cast<const TimeValue *> (&source);
      TimeValue *dst = dynamic_cast<TimeValue *> (&destination);
      if (src == 0 || dst == 0)
        {
          return false;
        }
      *dst = *src;
      return true;
    }
    Time m_minValue;
    Time m_maxValue;
  } *checker = new Checker (min, max);
  return Ptr<const AttributeChecker> (checker, false);
}


} // namespace ns3

