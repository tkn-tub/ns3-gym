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
#include "ns3/log.h"
#include <math.h>
#include <sstream>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Time");

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
  NS_LOG_FUNCTION (unit << resolution);
  int8_t power [LAST] = {15, 12, 9, 6, 3, 0};
  for (int i = 0; i < Time::LAST; i++)
    {
      int shift = power[i] - power[(int)unit];
      uint64_t factor = (uint64_t) pow (10, fabs (shift));
      struct Information *info = &resolution->info[i];
      info->factor = factor;
      if (shift == 0)
	{
	  info->timeFrom = HighPrecision (1, false);
	  info->timeTo = HighPrecision (1, false);
	  info->toMul = true;
	  info->fromMul = true;
	}
      else if (shift > 0)
	{
	  info->timeFrom = HighPrecision (factor, false);
	  info->timeTo = HighPrecision::Invert (factor);
	  info->toMul = false;
	  info->fromMul = true;
	}
      else
	{
	  NS_ASSERT (shift < 0);
	  info->timeFrom = HighPrecision::Invert (factor);
	  info->timeTo = HighPrecision (factor, false);
	  info->toMul = true;
	  info->fromMul = false;
	}
      NS_LOG_DEBUG ("i=" << i << 
                    " shift=" << shift << 
                    " from=" << info->timeFrom <<
                    " to=" << info->timeTo);
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
  uint64_t v = Time::ToInteger (time, Time::GetResolution ());
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

#include "ns3/test.h"

namespace ns3 {

class Bug863TestCase : public TestCase
{
public:
  Bug863TestCase ();
  virtual bool DoRun (void);
};

Bug863TestCase::Bug863TestCase ()
  : TestCase ("Bug 863")
{
}

bool Bug863TestCase::DoRun (void)
{
  Scalar result = Scalar (0.9) / Scalar (1.0);
  NS_TEST_ASSERT_MSG_EQ ((result == Scalar (0.9)), true, "Invalid arithmetic result");
  return false;
}

class TimeSimpleTestCase : public TestCase
{
public:
  TimeSimpleTestCase (enum Time::Unit resolution);
private:
  virtual bool DoRun (void);
  virtual void DoTearDown (void);
  enum Time::Unit m_originalResolution;
  enum Time::Unit m_resolution;
};

TimeSimpleTestCase::TimeSimpleTestCase (enum Time::Unit resolution)
  : TestCase ("Sanity check of common time operations"),
    m_resolution (resolution)
{}
bool
TimeSimpleTestCase::DoRun (void)
{
  m_originalResolution = Time::GetResolution ();
  Time::SetResolution (m_resolution);
  NS_TEST_ASSERT_MSG_EQ_TOL (Seconds (1.0).GetSeconds (), 1.0, TimeStep (1).GetSeconds (), 
                             "is 1 really 1 ?");
  NS_TEST_ASSERT_MSG_EQ_TOL (Seconds (10.0).GetSeconds (), 10.0, TimeStep (1).GetSeconds (), 
                             "is 10 really 10 ?");
  NS_TEST_ASSERT_MSG_EQ (MilliSeconds (1).GetMilliSeconds (), 1, 
                         "is 1ms really 1ms ?");
  NS_TEST_ASSERT_MSG_EQ (MicroSeconds (1).GetMicroSeconds (), 1, 
                         "is 1us really 1us ?");
#if 0
  Time ns = NanoSeconds (1);
  ns.GetNanoSeconds ();
  NS_TEST_ASSERT_MSG_EQ (NanoSeconds (1).GetNanoSeconds (), 1, 
                         "is 1ns really 1ns ?");
  NS_TEST_ASSERT_MSG_EQ (PicoSeconds (1).GetPicoSeconds (), 1, 
                         "is 1ps really 1ps ?");
  NS_TEST_ASSERT_MSG_EQ (FemtoSeconds (1).GetFemtoSeconds (), 1, 
                         "is 1fs really 1fs ?");
#endif
  return false;
}

void 
TimeSimpleTestCase::DoTearDown (void)
{
  Time::SetResolution (m_originalResolution);
}

class ArithTestCase : public TestCase
{
public:
  ArithTestCase ();
private:
  virtual bool DoRun (void);
};

ArithTestCase::ArithTestCase ()
  : TestCase ("check arithmetic operators")
{
}
bool 
ArithTestCase::DoRun (void)
{
  Time a, b, c;
  c = a + b;
  c = a * b;
  c = a / Seconds (1.0);
  c = a - b;
  c += a;
  c -= a;
  c /= Seconds (1.0);
  c *= a;
  bool x;
  x = a < b;
  x = a > b;
  x = a <= b;
  x = a >= b;
  x = a == b;
  x = a != b;
  //a = 1.0;
  //a = 1;
  return false;
}



static class TimeTestSuite : public TestSuite
{
public:
  TimeTestSuite ()
    : TestSuite ("time", UNIT)
  {
    AddTestCase (new Bug863TestCase ());
    AddTestCase (new TimeSimpleTestCase (Time::US));
    AddTestCase (new ArithTestCase ());
  }
} g_timeTestSuite;

} // namespace ns3
