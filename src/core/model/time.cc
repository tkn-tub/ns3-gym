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
#include "ns3/abort.h"
#include "ns3/global-value.h"
#include "ns3/enum.h"
#include "ns3/string.h"
#include "ns3/object.h"
#include "ns3/config.h"
#include <math.h>
#include <sstream>

namespace ns3 {

Time::Time (const std::string& s)
{
  std::string::size_type n = s.find_first_not_of ("0123456789.");
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
  struct Resolution resolution;
  SetResolution (Time::NS, &resolution);
  return resolution;
}
void 
Time::SetResolution (enum Unit resolution)
{
  SetResolution (resolution, PeekResolution ());
}
void 
Time::SetResolution (enum Unit unit, struct Resolution *resolution)
{
  int8_t power [LAST] = { 15, 12, 9, 6, 3, 0};
  for (int i = 0; i < Time::LAST; i++)
    {
      int shift = power[i] - power[(int)unit];
      uint64_t factor = (uint64_t) pow (10, fabs (shift));
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
ATTRIBUTE_CHECKER_IMPLEMENT (Time);

} // namespace ns3

