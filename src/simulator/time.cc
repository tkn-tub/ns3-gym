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
#include "ns3/global-value.h"
#include "ns3/enum.h"
#include "ns3/string.h"
#include "ns3/object.h"
#include "ns3/config.h"
#include <math.h>

namespace ns3 {

namespace TimeStepPrecision {

static const uint64_t MS_FACTOR = (uint64_t)1000;
static const uint64_t US_FACTOR = (uint64_t)1000000;
static const uint64_t NS_FACTOR = (uint64_t)1000000 * (uint64_t)1000;
static const uint64_t PS_FACTOR = (uint64_t)1000000 * (uint64_t)1000000;
static const uint64_t FS_FACTOR = (uint64_t)1000000 * (uint64_t)1000000 * (uint64_t)1000;
static uint64_t g_tsPrecFactor = NS_FACTOR;

static GlobalValue g_precisionDefaultValue ("TimeStepPrecision", 
                                            "The time unit of the internal 64 bit integer time.",
                                            EnumValue (NS),
                                            MakeEnumChecker (NS, "NS",
                                                             S, "S",
                                                             MS, "MS",
                                                             US, "US",
                                                             PS, "PS",
                                                             FS, "FS")
                                            );

precision_t
Get (void)
{
  EnumValue v;
  g_precisionDefaultValue.GetValue (v);
  return (precision_t) v.Get ();
}

void 
Set (precision_t precision)
{
  g_precisionDefaultValue.SetValue (EnumValue (precision));
  g_tsPrecFactor = (uint64_t)pow(10, precision);
}

} // namespace TimeStepPrecision

TimeUnit<1>::TimeUnit(const std::string& s)
{
  std::string::size_type n = s.find_first_not_of("0123456789.");
  if (n != std::string::npos)
  { // Found non-numeric
    std::istringstream iss;
    iss.str (s.substr(0, n));
    double r;
    iss >> r;
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
  std::istringstream iss;
  iss. str (s);
  double v;
  iss >> v;
  m_data = HighPrecision (v * TimeStepPrecision::g_tsPrecFactor);
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
operator<< (std::ostream& os, const Time & time)
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
std::istream& operator>> (std::istream& is, Time & time)
{
  std::string value;
  is >> value;
  std::string::size_type n = value.find_first_not_of("0123456789.");
  if (n == std::string::npos)
    {
      is.setstate (std::ios_base::failbit);
      return is;
    }
  std::string trailer = value.substr(n, value.size ()-n);
  std::istringstream iss;
  iss.str (value.substr(0, n));

  if (trailer == std::string("s"))
    {
      double v;
      iss >> v;
      time = Seconds (v);
      return is;
    }
  uint64_t integer;
  iss >> integer;
  if (is.bad () || is.fail ())
    {
      is.setstate (std::ios_base::failbit);
    }
  else if (trailer == std::string("ms"))
    {
      time = MilliSeconds (integer);
    }
  else if (trailer == std::string("us"))
    {
      time = MicroSeconds (integer);
    }
  else if (trailer == std::string("ns"))
    {
      time = NanoSeconds (integer);
    }
  else if (trailer == std::string("ps"))
    {
      time = PicoSeconds (integer);
    }
  else if (trailer == std::string("fs"))
    {
      time = FemtoSeconds (integer);
    }
  else
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
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

ATTRIBUTE_VALUE_IMPLEMENT (Time);
ATTRIBUTE_CHECKER_IMPLEMENT (Time);

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

} // namespace ns3

#include "ns3/test.h"

namespace ns3 {

#define PRECISION(mult) (pow(10,-((double)(ns3::TimeStepPrecision::Get ())))*mult)
#define ASSERT_MSG_EQ(a,b,mult,msg)                                     \
  NS_TEST_ASSERT_MSG_EQ(((a)<((b)-PRECISION(mult)) || (a)>((b)+PRECISION(mult))),false, \
                        msg << " Values are not equal within requested precision range: " << \
                        (a) << "!=" << (b) << " ~ " << PRECISION(mult))
#define ASSERT_MSG_EQ_INT(a,b,mult,msg) \
  ASSERT_MSG_EQ(((int64_t)(a)),((int64_t)(b)),mult,msg)
#define ASSERT_EQ(a,b)                          \
  ASSERT_MSG_EQ(a,b,1,"")

class OldTimeTestCase : public TestCase
{
public:
  OldTimeTestCase();
  virtual bool DoRun (void);
};

OldTimeTestCase::OldTimeTestCase()
  : TestCase("Sanity check of common time operations")
{}
bool 
OldTimeTestCase::DoRun (void)
{  
  NS_TEST_ASSERT_MSG_EQ(TimeStepPrecision::Get(), 
                        TimeStepPrecision::NS,
                        "Invalid precision mode");

  Time t0 = Seconds (10.0);
  ASSERT_EQ(t0.GetSeconds(), 10.0);

  Time t1 = Seconds (11.0);
  ASSERT_EQ(t1.GetSeconds(), 11.0);

  t0 = Seconds (1.5);
  ASSERT_EQ(t0.GetSeconds(), 1.5);

  t0 = Seconds (-1.5);
  ASSERT_EQ(t0.GetSeconds(), -1.5);

  t0 = MilliSeconds ((uint64_t)10.0);
  ASSERT_EQ(t0.GetSeconds(), 0.01);

  t1 = MilliSeconds ((uint64_t)11.0);
  ASSERT_EQ(t1.GetSeconds(), 0.011);


  Time t2, t3;

  t2 = t1 - t0;
  NS_TEST_ASSERT_MSG_EQ(t2.IsStrictlyPositive (),true, "Variable should be positive");
  ASSERT_EQ(t2.GetSeconds(), t1.GetSeconds()-t0.GetSeconds());

  t2 = t1 - t1;
  NS_TEST_ASSERT_MSG_EQ(t2.IsZero (),true, "Variable should be zero");
  ASSERT_EQ(t2.GetSeconds(), t1.GetSeconds()-t1.GetSeconds());

  t2 = t0 - t1;
  NS_TEST_ASSERT_MSG_EQ(t2.IsStrictlyNegative (),true, "Variable should be negative");
  ASSERT_EQ(t2.GetSeconds(), t0.GetSeconds()-t1.GetSeconds());

  Time tmp = MilliSeconds (0);
  NS_TEST_ASSERT_MSG_EQ((MilliSeconds (0) == NanoSeconds(0)), true, "Zero is not Zero ?");
  NS_TEST_ASSERT_MSG_EQ((MilliSeconds (0) > NanoSeconds(0)), false, "Zero is bigger than Zero ?");
  NS_TEST_ASSERT_MSG_EQ((MilliSeconds (0) < NanoSeconds(0)), false, "Zero is smaller than Zero ?");

  Time t4 = Seconds (10.0) * Scalar (1.5);
  ASSERT_EQ(t4.GetSeconds(), 15.0);

  Time t5 = NanoSeconds (10) * Scalar (1.5);
  ASSERT_EQ(t5.GetNanoSeconds(), 15.0);

  Time t6 = Seconds (10.0) * Scalar (15) / Scalar (10);
  ASSERT_EQ(t6.GetSeconds (), 15.0);

  Time t7 = NanoSeconds (10) * Scalar (15) / Scalar (10);
  ASSERT_EQ(t7.GetNanoSeconds (), 15.0);

  ASSERT_EQ((t1 + t2).GetSeconds (), t1.GetSeconds()+t2.GetSeconds());

  ASSERT_EQ((t1 / t2).GetDouble (), t1.GetSeconds()/t2.GetSeconds());

  // XXX
  return false;
}

class OperationsTimeTestCase : public TestCase
{
public:
  OperationsTimeTestCase();
  virtual bool DoRun(void);
};

OperationsTimeTestCase::OperationsTimeTestCase()
  : TestCase ("Check the +, -, * and / operators for the TimeUnit<1>")
{}

bool
OperationsTimeTestCase::DoRun(void)
{
  // What happens if you set these values ?
  //  t0 = Seconds ((uint64_t)10.0);
  //  t1 = Seconds ((uint64_t)11.0);

  Time t0 = MilliSeconds(10);
  Time t1 = MilliSeconds(11);

  ASSERT_EQ((t0-t1).GetSeconds(),
            (t0.GetSeconds()-t1.GetSeconds()));
  ASSERT_EQ(((t0-t1) * t0 / t0).GetSeconds(),
            ((t0.GetSeconds()-t1.GetSeconds()) * t0.GetSeconds () / t0.GetSeconds ()));
  ASSERT_EQ(((t0-t1) * t0 / t1).GetSeconds(),
            ((t0.GetSeconds()-t1.GetSeconds()) * t0.GetSeconds () / t1.GetSeconds ()));
  ASSERT_EQ((t0 * (t0-t1) / t1).GetSeconds(),
            (t0.GetSeconds () * (t0.GetSeconds()-t1.GetSeconds()) / t1.GetSeconds ()));
  ASSERT_EQ((t0 * t1 / (t0-t1)).GetSeconds(),
            (t0.GetSeconds () * t1.GetSeconds() / (t0.GetSeconds()-t1.GetSeconds())));
  ASSERT_EQ((t0 * (t1 / (t0-t1))).GetSeconds(),
            (t0.GetSeconds () * (t1.GetSeconds() / (t0.GetSeconds()-t1.GetSeconds()))));
  ASSERT_EQ(((t0 * t1) / (t0-t1)).GetSeconds(),
            ((t0.GetSeconds () * t1.GetSeconds()) / (t0.GetSeconds()-t1.GetSeconds())));
  ASSERT_EQ((t0 / t1 * (t0-t1)).GetSeconds(),
            (t0.GetSeconds () / t1.GetSeconds() * (t0.GetSeconds()-t1.GetSeconds())));
  ASSERT_EQ(((t0 / t1) * (t0-t1)).GetSeconds(),
              (t0.GetSeconds () / t1.GetSeconds()) * (t0.GetSeconds()-t1.GetSeconds()));
  ASSERT_EQ((t0 * Scalar(10.0)).GetSeconds (), (t0.GetSeconds () * 10.0));
  ASSERT_EQ((Scalar(10.0) * t0).GetSeconds (), (10.0 * t0.GetSeconds ()));

  // Note: we need to multiply by 1e9 because GetSeconds is multiplying
  ASSERT_EQ(((t0/(t1*(t0-t1))).GetHighPrecision().GetDouble() * 1e9),
             (t0.GetSeconds()/(t1.GetSeconds()*(t0.GetSeconds()-t1.GetSeconds()))));

  ASSERT_EQ((t0/t1).GetDouble(),(t0.GetSeconds()/t1.GetSeconds()));


  ASSERT_EQ((t0 * t1 / ((t0-t1) * t0)).GetDouble (),
            (t0.GetSeconds () * t1.GetSeconds () / ((t0.GetSeconds () - t1.GetSeconds()) * t0.GetSeconds ())));
  // XXX
  return false;
}

class TimeStepTestCase : public TestCase
{
public:
  TimeStepTestCase ();
  virtual bool DoRun (void);
};

TimeStepTestCase::TimeStepTestCase ()
  : TestCase("Check boundaries of TimeStep")
{}
bool
TimeStepTestCase::DoRun (void)
{
  Time tooBig = TimeStep (0x8000000000000000LL);
  NS_TEST_ASSERT_MSG_EQ (tooBig.IsNegative (), true, "Is not negative ?");
  tooBig = TimeStep (0xffffffffffffffffLL);
  NS_TEST_ASSERT_MSG_EQ (tooBig.IsNegative (), true, "Is not negative ?");
  tooBig = TimeStep (0x7fffffffffffffffLL);
  NS_TEST_ASSERT_MSG_EQ (tooBig.IsPositive (), true, "Is not negative ?");
  tooBig += TimeStep (1);
  NS_TEST_ASSERT_MSG_EQ (tooBig.IsNegative (), true, "Is not negative ?");
  // XXX
  return false;
}

class GlobalPrecisionTestCase : public TestCase
{
public:
  GlobalPrecisionTestCase ();
  virtual bool DoRun (void);
  virtual void DoTeardown (void);
};

GlobalPrecisionTestCase::GlobalPrecisionTestCase ()
  : TestCase ("Check that global value actually changes the underlying precision")
{}
#define CHECK_PRECISION(prec) \
  Config::SetGlobal ("TimeStepPrecision", StringValue (#prec)); \
  NS_TEST_ASSERT_MSG_EQ(TimeStepPrecision::Get(), TimeStepPrecision::prec, "Could not set precision " << #prec)
bool 
GlobalPrecisionTestCase::DoRun (void)
{
  CHECK_PRECISION(S);
  CHECK_PRECISION(MS);
  CHECK_PRECISION(US);
  CHECK_PRECISION(NS);
  CHECK_PRECISION(PS);
  CHECK_PRECISION(FS);
  // XXX
  return false;
}

void 
GlobalPrecisionTestCase::DoTeardown (void)
{
  TimeStepPrecision::Set (TimeStepPrecision::NS);
}

class ConversionTestCase : public TestCase
{
public:
  ConversionTestCase ();
  virtual bool DoRun (void);
  virtual void DoTeardown (void);
};

ConversionTestCase::ConversionTestCase ()
  : TestCase ("Check crazy time conversions")
{}

void ConversionTestCase::DoTeardown (void)
{
  TimeStepPrecision::Set (TimeStepPrecision::NS);
}

#define CHECK_CONVERSIONS(tmp)                                          \
  do {                                                                  \
    double val = tmp;                                                   \
    Time t_sec = Seconds(val);                                          \
    ASSERT_MSG_EQ(t_sec.GetSeconds(), val*1e0, 1e0, "conv sec s");          \
    ASSERT_MSG_EQ_INT(t_sec.GetMilliSeconds(), val*1e3, 1e3, "conv sec ms"); \
    ASSERT_MSG_EQ_INT(t_sec.GetMicroSeconds(), val*1e6, 1e6, "conv sec us"); \
    ASSERT_MSG_EQ_INT(t_sec.GetNanoSeconds(), val*1e9, 1e9, "conv sec ns"); \
    ASSERT_MSG_EQ_INT(t_sec.GetPicoSeconds(), val*1e12, 1e12, "conv sec ps"); \
    ASSERT_MSG_EQ_INT(t_sec.GetFemtoSeconds(), val*1e15, 1e15, "conv sec fs"); \
    Time t_ms = MilliSeconds(val);                                     \
    ASSERT_MSG_EQ(t_ms.GetSeconds(), val*1e-3, 1e0, "conv ms s");       \
    ASSERT_MSG_EQ_INT(t_ms.GetMilliSeconds(), val*1e0, 1e3, "conv ms ms");     \
    ASSERT_MSG_EQ_INT(t_ms.GetMicroSeconds(), val*1e3, 1e6, "conv ms us"); \
    ASSERT_MSG_EQ_INT(t_ms.GetNanoSeconds(), val*1e6, 1e9, "conv ms ns");  \
    ASSERT_MSG_EQ_INT(t_ms.GetPicoSeconds(), val*1e9, 1e12, "conv ms fs"); \
    ASSERT_MSG_EQ_INT(t_ms.GetFemtoSeconds(), val*1e12, 1e15, "conv ms ps"); \
    Time t_us = MicroSeconds(val);                                     \
    ASSERT_MSG_EQ(t_us.GetSeconds(), val*1e-6, 1e0, "conv us s");       \
    ASSERT_MSG_EQ_INT(t_us.GetMilliSeconds(), val*1e-3, 1e3, "conv us ms"); \
    ASSERT_MSG_EQ_INT(t_us.GetMicroSeconds(), val*1e0, 1e6, "conv us us");     \
    ASSERT_MSG_EQ_INT(t_us.GetNanoSeconds(), val*1e3, 1e9, "conv us ns");  \
    ASSERT_MSG_EQ_INT(t_us.GetPicoSeconds(), val*1e6, 1e12, "conv us ps"); \
    ASSERT_MSG_EQ_INT(t_us.GetFemtoSeconds(), val*1e9, 1e15, "conv us fs"); \
    Time t_ns = NanoSeconds(val);                                      \
    ASSERT_MSG_EQ(t_ns.GetSeconds(), val*1e-9, 1e0, "conv ns s");       \
    ASSERT_MSG_EQ_INT(t_ns.GetMilliSeconds(), val*1e-6, 1e3, "conv ns ms"); \
    ASSERT_MSG_EQ_INT(t_ns.GetMicroSeconds(), val*1e-3, 1e6, "conv ns us"); \
    ASSERT_MSG_EQ_INT(t_ns.GetNanoSeconds(), val*1e0, 1e9, "conv ns ns");      \
    ASSERT_MSG_EQ_INT(t_ns.GetPicoSeconds(), val*1e3, 1e12, "conv ns ps"); \
    ASSERT_MSG_EQ_INT(t_ns.GetFemtoSeconds(), val*1e6, 1e15, "conv ns fs"); \
    Time t_ps = PicoSeconds(val);                                      \
    ASSERT_MSG_EQ(t_ps.GetSeconds(), val*1e-12, 1e0, "conv ps s");      \
    ASSERT_MSG_EQ_INT(t_ps.GetMilliSeconds(), val*1e-9, 1e3, "conv ps ms"); \
    ASSERT_MSG_EQ_INT(t_ps.GetMicroSeconds(), val*1e-6, 1e6, "conv ps us"); \
    ASSERT_MSG_EQ_INT(t_ps.GetNanoSeconds(), val*1e-3, 1e9, "conv ps ns");  \
    ASSERT_MSG_EQ_INT(t_ps.GetPicoSeconds(), val*1e0, 1e12, "conv ps ps");     \
    ASSERT_MSG_EQ_INT(t_ps.GetFemtoSeconds(), val*1e3, 1e15, "conv ps fs"); \
    Time t_fs = FemtoSeconds(val);                                     \
    ASSERT_MSG_EQ(t_fs.GetSeconds(), val*1e-15, 1e0, "conv fs sec");    \
    ASSERT_MSG_EQ_INT(t_fs.GetMilliSeconds(), val*1e-12, 1e3, "conv fs ms"); \
    ASSERT_MSG_EQ_INT(t_fs.GetMicroSeconds(), val*1e-9, 1e6, "conv fs us"); \
    ASSERT_MSG_EQ_INT(t_fs.GetNanoSeconds(), val*1e-6, 1e9, "conv fs ns");  \
    ASSERT_MSG_EQ_INT(t_fs.GetPicoSeconds(), val*1e-3, 1e12, "conv fs ps"); \
    ASSERT_MSG_EQ_INT(t_fs.GetFemtoSeconds(), val*1e0, 1e15, "conv fs fs");    \
  } while (false)

bool
ConversionTestCase::DoRun (void)
{
  CHECK_CONVERSIONS(5);
  CHECK_CONVERSIONS(0);
  CHECK_CONVERSIONS(783);
  CHECK_CONVERSIONS(1132);
  // triggers overflow
  // XXX
  // CHECK_CONVERSIONS(3341039);

  TimeStepPrecision::Set (TimeStepPrecision::US);
  CHECK_CONVERSIONS(7);
  CHECK_CONVERSIONS(546);
  CHECK_CONVERSIONS(6231);
  // triggers overflow
  // XXX
  // CHECK_CONVERSIONS(1234639);

  TimeStepPrecision::Set (TimeStepPrecision::MS);
  CHECK_CONVERSIONS(3);
  CHECK_CONVERSIONS(134);
  CHECK_CONVERSIONS(2341);
  // triggers overflow
  // XXX
  // CHECK_CONVERSIONS(8956239);

  TimeStepPrecision::Set (TimeStepPrecision::NS);
  CHECK_CONVERSIONS(4);
  CHECK_CONVERSIONS(342);
  CHECK_CONVERSIONS(1327);
  // triggers overflow
  // XXX
  // CHECK_CONVERSIONS(5439627);

  TimeStepPrecision::Set (TimeStepPrecision::PS);
  CHECK_CONVERSIONS(4);
  CHECK_CONVERSIONS(342);
  CHECK_CONVERSIONS(1327);
  // triggers overflow
  // XXX
  // CHECK_CONVERSIONS(5439627);

  TimeStepPrecision::Set (TimeStepPrecision::NS);
  CHECK_CONVERSIONS(12);

  TimeStepPrecision::Set (TimeStepPrecision::S);
  CHECK_CONVERSIONS(7);

  TimeStepPrecision::Set (TimeStepPrecision::FS);
  CHECK_CONVERSIONS(5);

  return false;
}

static class TimeTestSuite : public TestSuite
{
public:
  TimeTestSuite()
    : TestSuite("time", UNIT)
  {
    AddTestCase(new OldTimeTestCase());
    AddTestCase(new OperationsTimeTestCase());
    AddTestCase(new TimeStepTestCase());
    AddTestCase(new GlobalPrecisionTestCase());
    AddTestCase(new ConversionTestCase());
  }
} g_timeTestSuite;

} // namespace ns3
