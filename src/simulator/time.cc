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
#include "ns3/fatal-error.h"
#include "ns3/default-value.h"
#include <math.h>

namespace ns3 {

namespace TimeStepPrecision {

static const uint64_t MS_FACTOR = (uint64_t)pow(10,3);
static const uint64_t US_FACTOR = (uint64_t)pow(10,6);
static const uint64_t NS_FACTOR = (uint64_t)pow(10,9);
static const uint64_t PS_FACTOR = (uint64_t)pow(10,12);
static const uint64_t FS_FACTOR = (uint64_t)pow(10,15);
static uint64_t g_tsPrecFactor = NS_FACTOR;

static EnumDefaultValue<enum precision_t> g_precisionDefaultValue ("TimeStepPrecision", 
                                                                   "The time unit of the internal 64 bit integer time.",
                                                                   NS, "NS",
                                                                   S, "S",
                                                                   MS, "MS",
                                                                   US, "US",
                                                                   PS, "PS",
                                                                   FS, "FS",
                                                                   0, (void *)0);

precision_t
Get (void)
{
  return g_precisionDefaultValue.GetValue ();
}

void 
Set (precision_t precision)
{
  g_precisionDefaultValue.SetValue (precision);
  g_tsPrecFactor = (uint64_t)pow(10, precision);
}

} // namespace TimeStepPrecision

TimeUnit<1>::TimeUnit(const std::string& s)
{
  std::string::size_type n = s.find_first_not_of("0123456789.");
  if (n != std::string::npos)
  { // Found non-numeric
    double r = atof(s.substr(0, n).c_str());
    std::string trailer = s.substr(n, std::string::npos);
    if (trailer == std::string("s"))
    {
      m_data = HighPrecision (r * TimeStepPrecision::g_tsPrecFactor);
      return;
    }
    if (trailer == std::string("ms"))
    {
      m_data = HighPrecision ((int64_t)(r * (TimeStepPrecision::g_tsPrecFactor/pow(10,3))), 
                              false);
      return;
    }
    if (trailer == std::string("us"))
    {
      m_data = HighPrecision ((int64_t)(r * (TimeStepPrecision::g_tsPrecFactor/pow(10,6))), 
                              false);
      return;
    }
    if (trailer == std::string("ns"))
    {
      m_data = HighPrecision ((int64_t)(r * (TimeStepPrecision::g_tsPrecFactor/pow(10,9))), 
                              false);
      return;
    }
    if (trailer == std::string("ps"))
    {
      m_data = HighPrecision ((int64_t)(r * (TimeStepPrecision::g_tsPrecFactor/pow(10,12))), 
                              false);
      return;
    }
    if (trailer == std::string("fs"))
    {
      m_data = HighPrecision ((int64_t)(r * (TimeStepPrecision::g_tsPrecFactor/pow(10,15))), 
                              false);
      return;
    }
    NS_FATAL_ERROR("Can't Parse Time "<<s);
  }
  //else
  //they didn't provide units, assume seconds
  m_data = HighPrecision (atof(s.c_str()) * TimeStepPrecision::g_tsPrecFactor);
}

double 
TimeUnit<1>::GetSeconds (void) const
{
  double timeValue = GetHighPrecision ().GetDouble ();
  return timeValue/TimeStepPrecision::g_tsPrecFactor;
}

int64_t
TimeUnit<1>::ConvertToUnits (int64_t timeValue, uint64_t unitFactor) const
{
  uint64_t precFactor;
  // In order to avoid conversion to double, precFactor can't be less 1
  if (TimeStepPrecision::g_tsPrecFactor < unitFactor)
    {
      precFactor = unitFactor / TimeStepPrecision::g_tsPrecFactor;
      timeValue = timeValue * precFactor;
    }
  else
    {
      precFactor = TimeStepPrecision::g_tsPrecFactor / unitFactor;
      timeValue = timeValue / precFactor;
    }
  return timeValue;
}


int64_t 
TimeUnit<1>::GetMilliSeconds (void) const
{
  int64_t ts = GetTimeStep();
  int64_t ms = ConvertToUnits(ts, TimeStepPrecision::MS_FACTOR);

  return ms;
}
int64_t 
TimeUnit<1>::GetMicroSeconds (void) const
{
  int64_t ts = GetTimeStep();
  int64_t us = ConvertToUnits(ts, TimeStepPrecision::US_FACTOR);

  return us;
}
int64_t 
TimeUnit<1>::GetNanoSeconds (void) const
{
  int64_t ts = GetTimeStep();
  int64_t ns = ConvertToUnits(ts, TimeStepPrecision::NS_FACTOR);

  return ns;
}
int64_t 
TimeUnit<1>::GetPicoSeconds (void) const
{
  int64_t ts = GetTimeStep();
  int64_t ps = ConvertToUnits(ts, TimeStepPrecision::PS_FACTOR);

  return ps;
}
int64_t 
TimeUnit<1>::GetFemtoSeconds (void) const
{
  int64_t ts = GetTimeStep();
  int64_t fs = ConvertToUnits(ts, TimeStepPrecision::FS_FACTOR);

  return fs;
}

/**
 * This returns the value with the precision returned by TimeStepPrecision::Get
 */
int64_t
TimeUnit<1>::GetTimeStep (void) const
{
  int64_t timeValue = GetHighPrecision ().GetInteger ();
  return timeValue;
}


std::ostream& 
operator<< (std::ostream& os, Time const& time)
{
  std::string unit;
  switch (TimeStepPrecision::Get ()) {
  case TimeStepPrecision::S:
    unit = "s";
    break;
  case TimeStepPrecision::MS:
    unit = "ms";
    break;
  case TimeStepPrecision::US:
    unit = "us";
    break;
  case TimeStepPrecision::NS:
    unit = "ns";
    break;
  case TimeStepPrecision::PS:
    unit = "ps";
    break;
  case TimeStepPrecision::FS:
    unit = "fs";
    break;
  }
  os << time.GetTimeStep () << unit;
  return os;
}

Time Seconds (double seconds)
{
  double d_sec = seconds * TimeStepPrecision::g_tsPrecFactor;
  return Time (HighPrecision (d_sec));
  //  return Time (HighPrecision ((int64_t)d_sec, false));
}

uint64_t
TimeUnit<1>::UnitsToTimestep (uint64_t unitValue, 
                              uint64_t unitFactor)
{
  uint64_t precFactor;
  // In order to avoid conversion to double, precFactor can't be less 1
  if (TimeStepPrecision::g_tsPrecFactor < unitFactor)
    {
      precFactor = unitFactor / TimeStepPrecision::g_tsPrecFactor;
      unitValue = unitValue / precFactor;
    }
  else
    {
      precFactor = TimeStepPrecision::g_tsPrecFactor / unitFactor;
      unitValue = unitValue * precFactor;
    }
  return unitValue;
}

Time MilliSeconds (uint64_t ms)
{
  uint64_t ts = TimeUnit<1>::UnitsToTimestep(ms, TimeStepPrecision::MS_FACTOR);
  return TimeStep(ts);
}

Time MicroSeconds (uint64_t us)
{
  uint64_t ts = TimeUnit<1>::UnitsToTimestep(us, TimeStepPrecision::US_FACTOR);
  return TimeStep(ts);
}

Time NanoSeconds (uint64_t ns)
{
  uint64_t ts = TimeUnit<1>::UnitsToTimestep(ns, TimeStepPrecision::NS_FACTOR);
  return TimeStep(ts);
}
Time PicoSeconds (uint64_t ps)
{
  uint64_t ts = TimeUnit<1>::UnitsToTimestep(ps, TimeStepPrecision::PS_FACTOR);
  return TimeStep(ts);
}
Time FemtoSeconds (uint64_t fs)
{
  uint64_t ts = TimeUnit<1>::UnitsToTimestep(fs, TimeStepPrecision::FS_FACTOR);
  return TimeStep(ts);
}
Time MaxSeconds (void)
{
  // XXX: I am fairly certain other compilers use other non-standard
  // post-fixes to indicate 64 bit constants.
  return TimeStep (0x7fffffffffffffffLL);
}

/*
 * The timestep value passed to this function must be of the precision
 * of TimeStepPrecision::Get
 */
Time TimeStep (uint64_t ts)
{
  return Time (HighPrecision (ts, false));
}

TimeUnit<0>::TimeUnit (double scalar)
  : m_data (HighPrecision (scalar))
{}

double
TimeUnit<0>::GetDouble (void) const
{
  return GetHighPrecision ().GetDouble ();
}

}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
//#include <iostream>

namespace ns3 {

class TimeTests : public Test
{
public:
  TimeTests ();
  virtual ~TimeTests ();
  virtual bool RunTests (void);

  /*
   * Verifies that a calculated time value is as expected using
   * doubles since GetSeconds() returns a double
   */ 
  void CheckTimeSec(std::string test_id, double actual, double expected, 
                    bool *flag, double precMultFactor = 1, 
                    bool verbose = false);

  /*
   * Verifies that a calculated time value is as expected.
   */ 
  void CheckTime(std::string test_id, int64_t actual, int64_t expected, 
                 bool *flag, double precMultFactor = 1, 
                 bool verbose = false);

  /*
   * Verifies the +, -, * and / operations for the TimeUnit<1> or Time class
   */
  void CheckOperations(Time t0, Time t1, bool *ok, bool verbose = false);

  /*
   * Verifies that the TimeUnit class stores values with the precision
   * set in the variable TimeStepPrecision::Get
   * Checks that overflow and underflow occur at expected numbers
   */
  void CheckPrecision(TimeStepPrecision::precision_t prec, uint64_t val, bool *ok,
                      bool verbose = false);

  /*
   * Verifies that the conversion between units in the class
   * TimeUnit<1> or Time is done correctly. This is verified both when
   * setting and retrieving a Time value
   */
  void CheckConversions(uint64_t tval, bool *ok, bool verbose = false);

  /*
   * These are the old tests that used to be run
   */
  void CheckOld(bool *ok);
};

TimeTests::TimeTests ()
  : Test ("Time")
{}
TimeTests::~TimeTests ()
{}

bool TimeTests::RunTests (void)
{
  bool result = true;

  Time t0, t1;

  CheckOld(&result);

  t0 = MilliSeconds ((uint64_t)10.0);
  t1 = MilliSeconds ((uint64_t)11.0);

  CheckOperations(t0, t1, &result);

  //  t0 = Seconds ((uint64_t)10.0);
  //  t1 = Seconds ((uint64_t)11.0);

  //  CheckOperations(t0, t1, &result);

  CheckConversions((uint64_t)5, &result);
  CheckConversions((uint64_t)0, &result);
  CheckConversions((uint64_t)783, &result);
  CheckConversions((uint64_t)1132, &result);
  //  CheckConversions((uint64_t)3341039, &result);

  // Now vary the precision and check the conversions
  if (TimeStepPrecision::Get () != TimeStepPrecision::NS) {
    result = false;
  }

  CheckPrecision(TimeStepPrecision::US, 7, &result);

  CheckConversions((uint64_t)7, &result);
  CheckConversions((uint64_t)546, &result);
  CheckConversions((uint64_t)6231, &result);
  //  CheckConversions((uint64_t)1234639, &result);

  CheckPrecision(TimeStepPrecision::MS, 3, &result);

  CheckConversions((uint64_t)3, &result);
  CheckConversions((uint64_t)134, &result);
  CheckConversions((uint64_t)2341, &result);
  //  CheckConversions((uint64_t)8956239, &result);

  CheckPrecision(TimeStepPrecision::PS, 21, &result);

  CheckConversions((uint64_t)4, &result);
  CheckConversions((uint64_t)342, &result);
  CheckConversions((uint64_t)1327, &result);
  //  CheckConversions((uint64_t)5439627, &result);

  CheckPrecision(TimeStepPrecision::NS, 12, &result);
  CheckConversions((uint64_t)12, &result);

  CheckPrecision(TimeStepPrecision::S, 7, &result);
  CheckConversions((uint64_t)7, &result);

  CheckPrecision(TimeStepPrecision::FS, 5, &result);
  CheckConversions((uint64_t)5, &result);

  TimeStepPrecision::Set (TimeStepPrecision::NS);

  DefaultValue::Bind ("TimeStepPrecision", "S");
  DefaultValue::Bind ("TimeStepPrecision", "MS");
  DefaultValue::Bind ("TimeStepPrecision", "US");
  DefaultValue::Bind ("TimeStepPrecision", "NS");
  DefaultValue::Bind ("TimeStepPrecision", "PS");
  DefaultValue::Bind ("TimeStepPrecision", "FS");


  Time tooBig = TimeStep (0x8000000000000000LL);
  NS_TEST_ASSERT (tooBig.IsNegative ());
  tooBig = TimeStep (0xffffffffffffffffLL);
  NS_TEST_ASSERT (tooBig.IsNegative ());
  tooBig = TimeStep (0x7fffffffffffffffLL);
  NS_TEST_ASSERT (tooBig.IsPositive ());
  tooBig += TimeStep (1);
  NS_TEST_ASSERT (tooBig.IsNegative ());

  return result;
}

void TimeTests::CheckOld (bool *ok)
{
  double dt0, dt1, dt2;
  int64_t it0, it1;

  Time t0 = Seconds (10.0);
  CheckTimeSec("old 1", t0.GetSeconds(), 10.0, ok);

  Time t1 = Seconds (11.0);
  CheckTimeSec("old 2", t1.GetSeconds(), 11.0, ok);

  t0 = Seconds (1.5);
  CheckTimeSec("old 3", t0.GetSeconds(), 1.5, ok);

  t0 = Seconds (-1.5);
  CheckTimeSec("old 4", t0.GetSeconds(), -1.5, ok);

  t0 = MilliSeconds ((uint64_t)10.0);
  dt0 = t0.GetSeconds();
  CheckTimeSec("old 5", dt0, 0.01, ok);

  t1 = MilliSeconds ((uint64_t)11.0);
  dt1 = t1.GetSeconds();
  CheckTimeSec("old 6", dt1, 0.011, ok);

  Time t2, t3;

  t2 = t1 - t0;
  if (!t2.IsStrictlyPositive ())
    {
      ok = false;
    }
  dt2 = t2.GetSeconds();
  CheckTimeSec("old 7", dt2, dt1-dt0, ok);

  t2 = t1 - t1;
  if (!t2.IsZero ())
    {
      ok = false;
    }
  dt2 = t2.GetSeconds();
  CheckTimeSec("old 8", dt2, dt1-dt1, ok);

  t2 = t0 - t1;
  if (!t2.IsStrictlyNegative ())
    {
      ok = false;
    }
  dt2 = t2.GetSeconds();
  CheckTimeSec("old 9", dt2, dt0-dt1, ok);

  t1 = NanoSeconds(15);
  it0 = t0.GetNanoSeconds();
  it1 = t1.GetNanoSeconds();
  TimeUnit<-2> tu4 = t0 / (t1 * t1 * t1);
  CheckTime("old 10", tu4.GetHighPrecision().GetInteger(), it0 / (it1*it1*it1), 
            ok, 1e9);

  Time tmp = MilliSeconds (0);
  if ((tmp != NanoSeconds (0)) ||
      (tmp > NanoSeconds (0)) ||
      (tmp < NanoSeconds (0)))
    {
      ok = false;
    }

  Time t4;
  t4 = Seconds (10.0) * Scalar (1.5);
  CheckTimeSec("old 11", t4.GetSeconds(), 10, ok);

  Time t5;
  t5 = NanoSeconds (10) * Scalar (1.5);
  CheckTime("old 12", t5.GetNanoSeconds(), 10, ok);

  t4 = Seconds (10.0) * Scalar (15) / Scalar (10);
  CheckTimeSec("old 13", t4.GetSeconds(), 15, ok);

  t5 = NanoSeconds (10) * Scalar (15) / Scalar (10);
  CheckTime("old 14", t5.GetNanoSeconds(), 15, ok);


  double foo = (t1 + t2).GetSeconds ();
  dt1 = t1.GetSeconds();
  dt2 = t2.GetSeconds();
  CheckTimeSec("old 15", foo, dt1+dt2, ok);

  foo += (t4 == t5)? 1 : 0;
  CheckTimeSec("old 16", foo, dt1+dt2, ok);

  foo = (t1/t2).GetDouble ();
  CheckTimeSec("old 17", foo, dt1/dt2, ok);
}


void TimeTests::CheckOperations(Time t0, Time t1, bool *ok, bool verbose) 
{

  if (verbose) 
    std::cout << std::endl << "Check operations: " 
              << t0 << " " << t1 << std::endl;

  Time t2, t3;
  double it0, it1, it2, it3, itu2, itu3;
  int64_t iti0;

  it0 = t0.GetSeconds();
  it1 = t1.GetSeconds();

  t2 = t0 - t1;
  it2 = t2.GetSeconds();
  CheckTimeSec("ops 1", it2, it0-it1, ok);

  t3 = t2 * t0 / t0;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 2a", it3, it2*it0/it0, ok);

  t3 = t2 * t0 / t1;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 2", it3, it2*it0/it1, ok);

  t3 = t0 * t2 / t1;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 3", it3, it0*it2/it1, ok);

  t3 = t0 * t1 / t2;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 4", it3, it0*it1/it2, ok);

  t3 = t0 * (t1 / t2);
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 5", it3, it0*(it1/it2), ok);

  t3 = (t0 * t1) / t2;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 6", it3, (it0*it1)/it2, ok);

  t3 = t0 / t1 * t2;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 7", it3, it0/it1*it2, ok);

  t3 = (t0 / t1) * t2;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 8", it3, (it0/it1)*it2, ok);

  t3 = t0 * Scalar (10.0);
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 9", it3, it0*10, ok);

  t3 = Scalar (10.0) * t0;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 10", it3, 10 * it0, ok);

  t3 = Scalar (10.0) * t0 / t2 * t1;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 11", it3, 10 * it0 / it2 * it1, ok);

  t3 = (Scalar (10.0) * t0 ) / t2 * t1;
  it3 = t3.GetSeconds();
  CheckTimeSec("ops 12", it3, (10 * it0) / it2 * it1, ok);

  TimeInvert ti0;
  ti0 = t0 / (t1 * t2);
  iti0 = ti0.GetHighPrecision().GetInteger();
  // This check is not quite working yet.
  //  CheckTime("ops 13", iti0, (int64_t)(it0/(it1*it2)), ok);

  Scalar s0 = t0 / t1;
  CheckTimeSec("ops 14", s0.GetDouble(), it0/it1, ok);

  Scalar s1;
  s1 = t0 * t1 / (t2 * t0);
  CheckTimeSec("ops 15", s1.GetDouble(), it0*it1/(it2*it0), ok);

  TimeUnit<0> tu0;
  tu0 = s0;
  CheckTimeSec("ops 16", tu0.GetDouble(), s0.GetDouble(), ok);

  TimeUnit<1> tu1;
  tu1 = t0;
  CheckTimeSec("ops 17", tu1.GetSeconds(), it0, ok);

  TimeUnit<2> tu2;
  tu2 = t0 * t1;
  CheckTimeSec("ops 18", tu2.GetHighPrecision().GetInteger()/(1e18), 
               it0 * it1, ok);
  itu2 = tu2.GetHighPrecision().GetInteger()/(1e18);
  
  TimeUnit<3> tu3;
  tu3 = t0 / Scalar(10e6) * tu2;
  CheckTimeSec("ops 19", tu3.GetHighPrecision().GetInteger()/(1e27), 
               it0 / 1000000 * itu2, ok);
  itu3 = tu3.GetHighPrecision().GetInteger()/(1e27);
}

void TimeTests::CheckConversions(uint64_t tval, bool *ok, bool verbose) {
  Time t_sec, t_ms, t_us, t_ns, t_ps, t_fs;

  if (verbose) 
    std::cout << std::endl << "Check conversions: " << tval << std::endl;

  // First check the seconds
  t_sec = Seconds((double)tval);
  CheckTimeSec("conv sec sec", t_sec.GetSeconds(), (double)tval, ok);
  CheckTime("conv sec ms", t_sec.GetMilliSeconds(), (int64_t)(tval*1e3), ok, 1e3);
  CheckTime("conv sec us", t_sec.GetMicroSeconds(), (int64_t)(tval*1e6), ok, 1e6);
  CheckTime("conv sec ns", t_sec.GetNanoSeconds(), (int64_t)(tval*1e9), ok, 1e9);
  CheckTime("conv sec ps", t_sec.GetPicoSeconds(), 
            (int64_t)(tval*1e12), ok, 1e12);
  CheckTime("conv sec fs", t_sec.GetFemtoSeconds(), 
            (int64_t)(tval*1e15), ok, 1e15);

  // Then check the milliseconds
  t_ms = MilliSeconds(tval);
  CheckTimeSec("conv ms sec", t_ms.GetSeconds(), (double)tval/1e3, ok);
  CheckTime("conv ms ms", t_ms.GetMilliSeconds(), (int64_t)(tval), ok, 1e3);
  CheckTime("conv ms us", t_ms.GetMicroSeconds(), (int64_t)(tval*1e3), ok, 1e6);
  CheckTime("conv ms ns", t_ms.GetNanoSeconds(), (int64_t)(tval*1e6), ok, 1e9);
  CheckTime("conv ms ps", t_ms.GetPicoSeconds(), (int64_t)(tval*1e9), ok, 1e12);
  CheckTime("conv ms fs", t_ms.GetFemtoSeconds(), (int64_t)(tval*1e12), ok, 1e15);

  // Then check the microseconds
  t_us = MicroSeconds(tval);
  CheckTimeSec("conv us sec", t_us.GetSeconds(), (double)tval/1e6, ok);
  CheckTime("conv us ms", t_us.GetMilliSeconds(), (int64_t)(tval/1e3), ok, 1e3);
  CheckTime("conv us us", t_us.GetMicroSeconds(), (int64_t)(tval), ok, 1e6);
  CheckTime("conv us ns", t_us.GetNanoSeconds(), (int64_t)(tval*1e3), ok, 1e9);
  CheckTime("conv us ps", t_us.GetPicoSeconds(), (int64_t)(tval*1e6), ok, 1e12);
  CheckTime("conv us fs", t_us.GetFemtoSeconds(), (int64_t)(tval*1e9), ok, 1e15);
  
  // Then check the nanoseconds
  t_ns = NanoSeconds(tval);
  CheckTimeSec("conv ns sec", t_ns.GetSeconds(), (double)tval/1e9, ok);
  CheckTime("conv ns ms", t_ns.GetMilliSeconds(), (int64_t)(tval/1e6), ok, 1e3);
  CheckTime("conv ns us", t_ns.GetMicroSeconds(), (int64_t)(tval/1e3), ok, 1e6);
  CheckTime("conv ns ns", t_ns.GetNanoSeconds(), (int64_t)(tval), ok, 1e9);
  CheckTime("conv ns ps", t_ns.GetPicoSeconds(), (int64_t)(tval*1e3), ok, 1e12);
  CheckTime("conv ns fs", t_ns.GetFemtoSeconds(), (int64_t)(tval*1e6), ok, 1e15);
  
  // Then check the picoseconds
  t_ps = PicoSeconds(tval);
  CheckTimeSec("conv ps sec", t_ps.GetSeconds(), (double)tval/1e12, ok);
  CheckTime("conv ps ms", t_ps.GetMilliSeconds(), (int64_t)(tval/1e9), ok, 1e3);
  CheckTime("conv ps us", t_ps.GetMicroSeconds(), (int64_t)(tval/1e6), ok, 1e6);
  CheckTime("conv ps ns", t_ps.GetNanoSeconds(), (int64_t)(tval/1e3), ok, 1e9);
  CheckTime("conv ps ps", t_ps.GetPicoSeconds(), (int64_t)(tval), ok, 1e12);
  CheckTime("conv ps fs", t_ps.GetFemtoSeconds(), (int64_t)(tval*1e3), ok, 1e15);
  
  // Then check the femtoseconds
  t_fs = FemtoSeconds(tval);
  CheckTimeSec("conv fs sec", t_fs.GetSeconds(), (double)tval/1e15, ok);
  CheckTime("conv fs ms", t_fs.GetMilliSeconds(), (int64_t)(tval/1e12), ok, 1e3);
  CheckTime("conv fs us", t_fs.GetMicroSeconds(), (int64_t)(tval/1e9), ok, 1e6);
  CheckTime("conv fs ns", t_fs.GetNanoSeconds(), (int64_t)(tval/1e6), ok, 1e9);
  CheckTime("conv fs ps", t_fs.GetPicoSeconds(), (int64_t)(tval/1e3), ok, 1e12);
  CheckTime("conv fs fs", t_fs.GetFemtoSeconds(), (int64_t)(tval), ok, 1e15);

  
}

void TimeTests::CheckPrecision(TimeStepPrecision::precision_t prec, uint64_t val, bool *ok, 
                               bool verbose) {
  if (verbose) {
    std::cout << "check precision 10^-" << prec << std::endl;
  }

  TimeStepPrecision::Set (prec);
  if (TimeStepPrecision::Get () != prec) {
    ok = false;
  }

  /* These still need to be fixed.
  // The smallest value that can be stored is 1x10^(-prec)
  Time smallest = Seconds(pow(10,-prec));
  CheckTimeSec("Prec small:     ", smallest.GetSeconds(), pow(10,-prec), ok, 0.1, 
               true);
  
  double d_ts = pow(10,-prec) - pow(10, -(prec+3));
  Time too_small = Seconds(d_ts);
  CheckTimeSec("Prec too small: ", too_small.GetSeconds(), 0, ok, 0.1, true);

  double d_la = 0xFFFFFFFF*pow(10,-prec);
  Time largest = Seconds(d_la);
  CheckTimeSec("Prec large:     ", largest.GetSeconds(), d_la, ok, 0.1, true);

  double d_tl = (0xFFFFFFFF*pow(10,-prec)) + 1;
  Time too_large = Seconds(d_tl);
  if ((largest.GetSeconds() + 1) == too_large.GetSeconds())
    std::cout << "Overflow did not occur." << std::endl;

  NS_ASSERT(d_la+1 == d_tl);
  */  
}

void TimeTests::CheckTimeSec (std::string test_id, double actual, 
                              double expected, bool *flag, double precMultFactor,
                              bool verbose)
{
  double prec = pow(10,-((double)(ns3::TimeStepPrecision::Get ()))) * precMultFactor;
  if ((actual < (expected-prec)) || (actual > (expected+prec))) {
    std::cout << "FAIL " << test_id 
              << " Expected:" << expected 
              << " Actual: " << actual
              << " Precision: " << prec << std::endl;
    *flag = false;
  } else {
    if (verbose) {
      std::cout << "PASS " << test_id 
                << " Expected:" << expected 
                << " Actual: " << actual
                << " Precision: " << prec << std::endl;
    }
  }
}

void TimeTests::CheckTime (std::string test_id, int64_t actual, 
                           int64_t expected, bool *flag, double precMultFactor,
                           bool verbose)
{
  double prec = pow(10,-((double)(ns3::TimeStepPrecision::Get ()))) * precMultFactor;
  if ((actual < (expected-prec)) || (actual > (expected+prec))) {
    std::cout << "FAIL " << test_id 
              << " Expected:" << expected 
              << " Actual: " << actual
              << " Precision: " << prec << std::endl;
    *flag = false;
  } else {
    if (verbose) {
      std::cout << "PASS " << test_id 
                << " Expected:" << expected 
                << " Actual: " << actual 
                << " Precision: " << prec << std::endl;
    }
  }
}


static TimeTests g_time_tests;
  
};

#endif /* RUN_SELF_TESTS */
