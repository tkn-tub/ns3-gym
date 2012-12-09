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
#include "log.h"
#include "global-value.h"
#include "enum.h"
#include "string.h"
#include "object.h"
#include "config.h"
#include "simulator.h"
#include <cmath>
#include <sstream>

NS_LOG_COMPONENT_DEFINE ("Time");

namespace ns3 {

Time::Time (const std::string& s)
{
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
  
  TimeSet (this);
}

// static
struct Time::Resolution
Time::SetDefaultNsResolution (void)
{
  NS_LOG_FUNCTION_NOARGS();
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
enum Time::Unit
Time::GetResolution (void)
{
  NS_LOG_FUNCTION_NOARGS();
  return PeekResolution ()->unit;
}

// static
void 
Time::SetResolution (enum Unit unit, struct Resolution *resolution,
                     const bool convert /* = true */)
{
  NS_LOG_FUNCTION (unit << resolution << convert);
  if (convert)
    { // We have to convert old values
      ConvertTimes (unit);
    }
  
  int8_t power [LAST] = { 15, 12, 9, 6, 3, 0};
  for (int i = 0; i < Time::LAST; i++)
    {
      int shift = power[i] - power[(int)unit];
      uint64_t factor = (uint64_t) std::pow (10, std::fabs (shift));
      struct Information *info = &resolution->info[i];
      info->factor = factor;
      if (shift == 0)
        {
          info->timeFrom = int64x64_t (1);
          info->timeTo = int64x64_t (1);
          info->toMul = true;
          info->fromMul = true;
        }
      else if (shift > 0)
        {
          info->timeFrom = int64x64_t (factor);
          info->timeTo = int64x64_t::Invert (factor);
          info->toMul = false;
          info->fromMul = true;
        }
      else
        {
          NS_ASSERT (shift < 0);
          info->timeFrom = int64x64_t::Invert (factor);
          info->timeTo = int64x64_t (factor);
          info->toMul = true;
          info->fromMul = false;
        }
    }
  resolution->unit = unit;
}


// static
Time::TimesSet *
Time::GetTimesSet ( const bool deleteMe /* = false */ )
{
  static TimesSet * times = new TimesSet;  

  if (deleteMe)
    {
      NS_LOG_LOGIC ("deleting TimesSet");
      if (times)
        {
          delete times;
        }
      times = 0;
    }

  return times;
}

// static
void
Time::DeleteTimesSet ()
{
  NS_LOG_FUNCTION_NOARGS();
  Time::GetTimesSet (true);
}

// static
void
Time::TimeSet (Time * const time)
{
  NS_ASSERT (time != 0);

  TimesSet * times = GetTimesSet();
  if (times)
    {
      std::pair< TimesSet::iterator, bool> ret;
      ret = times->insert ( time);
      NS_LOG_LOGIC ("\t[" << times->size () << "] recording " << time);
  
      if (ret.second == false)
        {
          NS_LOG_WARN ("already recorded " << time << "!");
        }
      // If this is the first Time, schedule the cleanup.
      if (times->size () == 1)
        {
          // We schedule here, after the first event has been added,
          // rather than in GetTimesSet when the set is empty.
          // Scheduling there creates another Time, which
          // finds an empty set and schedules an event . . .
          // Doing it here, the schedule creates the second Time,
          // which doesn't recurse.
          NS_LOG_LOGIC ("scheduling DeleteTimesSet()");
          Simulator::Schedule ( Seconds (0), & DeleteTimesSet);
        }
    }
}

// static
void
Time::TimeUnset (Time * const time)
{
  NS_ASSERT (time != 0);
  TimesSet * times = GetTimesSet ();
  if (times)
    {
      NS_ASSERT_MSG (times->count (time) == 1,
		     "Time object " << time << " registered "
		     << times->count (time) << " times (should be 1)." );

      TimesSet::size_type num = times->erase (time);
      if (num != 1)
        {
          NS_LOG_WARN ("unexpected result erasing " << time << "!");
          NS_LOG_WARN ("got " << num << ", expected 1");
        }
      else
        {
          NS_LOG_LOGIC ("\t[" << times->size () << "] removing  " << time);
        }
    }
}

// static
void
Time::ConvertTimes (const enum Unit unit)
{
  NS_LOG_FUNCTION_NOARGS();
  TimesSet * times = GetTimesSet ();
  NS_ASSERT_MSG (times != 0, "No Time registry. Time::SetResolution () called mare than once?");
  
  for ( TimesSet::iterator it = times->begin();
        it != times->end();
        it++ )
    {
      Time * const tp = *it;
      (*tp) = tp->ToInteger (unit);
    }

  NS_LOG_LOGIC ("logged " << GetTimesSet ()->size () << " Time objects.");
  GetTimesSet (true);
}

std::ostream&
operator<< (std::ostream& os, const Time & time)
{
  std::string unit;
  switch (Time::GetResolution ())
    {
    case Time::S:
      unit = "s";
      break;
    case Time::MS:
      unit = "ms";
      break;
    case Time::US:
      unit = "us";
      break;
    case Time::NS:
      unit = "ns";
      break;
    case Time::PS:
      unit = "ps";
      break;
    case Time::FS:
      unit = "fs";
      break;
    case Time::LAST:
      NS_ABORT_MSG ("can't be reached");
      unit = "unreachable";
      break;
    }
  int64x64_t v = time;
  os << v << unit;
  return os;
}
std::istream& operator>> (std::istream& is, Time & time)
{
  std::string value;
  is >> value;
  time = Time (value);
  return is;
}

ATTRIBUTE_VALUE_IMPLEMENT (Time);
ATTRIBUTE_CHECKER_IMPLEMENT (Time);

} // namespace ns3

