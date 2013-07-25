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
#include "global-value.h"
#include "enum.h"
#include "string.h"
#include "object.h"
#include "config.h"
#include "log.h"
#include <cmath>
#include <sstream>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Time");

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
          return;
        }
      if (trailer == std::string ("ms"))
        {
          *this = Time::FromDouble (r, Time::MS);
          return;
        }
      if (trailer == std::string ("us"))
        {
          *this = Time::FromDouble (r, Time::US);
          return;
        }
      if (trailer == std::string ("ns"))
        {
          *this = Time::FromDouble (r, Time::NS);
          return;
        }
      if (trailer == std::string ("ps"))
        {
          *this = Time::FromDouble (r, Time::PS);
          return;
        }
      if (trailer == std::string ("fs"))
        {
          *this = Time::FromDouble (r, Time::FS);
          return;
        }
      NS_ABORT_MSG ("Can't Parse Time " << s);
    }
  // else
  // they didn't provide units, assume seconds
  std::istringstream iss;
  iss.str (s);
  double v;
  iss >> v;
  *this = Time::FromDouble (v, Time::S);
}

struct Time::Resolution
Time::GetNsResolution (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  struct Resolution resolution;
  SetResolution (Time::NS, &resolution);
  return resolution;
}
void 
Time::SetResolution (enum Unit resolution)
{
  NS_LOG_FUNCTION (resolution);
  SetResolution (resolution, PeekResolution ());
}
void 
Time::SetResolution (enum Unit unit, struct Resolution *resolution)
{
  NS_LOG_FUNCTION (unit << resolution);
  int8_t power [LAST] = { 15, 12, 9, 6, 3, 0};
  for (int i = 0; i < Time::LAST; i++)
    {
      int shift = power[i] - power[(int)unit];
      int64_t factor = (int64_t) std::pow (10, std::fabs (shift));
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
enum Time::Unit
Time::GetResolution (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return PeekResolution ()->unit;
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

