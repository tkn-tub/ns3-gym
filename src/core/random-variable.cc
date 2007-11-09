/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//

#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <sys/time.h>			// for gettimeofday
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>       


#include "random-variable.h"
#include "rng-stream.h"
#include "fatal-error.h"

using namespace std;

namespace ns3{

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RandomVariable methods

bool          RandomVariable::initialized = false;   // True if RngStream seed set 
bool          RandomVariable::useDevRandom = false;  // True if use /dev/random
bool          RandomVariable::globalSeedSet = false; // True if GlobalSeed called
int           RandomVariable::devRandom = -1;
uint32_t      RandomVariable::globalSeed[6];
unsigned long RandomVariable::heuristic_sequence;
RngStream*    RandomVariable::m_static_generator = 0;
uint32_t      RandomVariable::runNumber = 0;

//the static object random_variable_initializer initializes the static members
//of RandomVariable
static class RandomVariableInitializer
{
  public:
  RandomVariableInitializer()
  {
//     RandomVariable::Initialize(); // sets the static package seed
//     RandomVariable::m_static_generator = new RngStream();
//     RandomVariable::m_static_generator->InitializeStream();
  }
  ~RandomVariableInitializer()
  {
    delete RandomVariable::m_static_generator;
  }
} random_variable_initializer;

RandomVariable::RandomVariable() 
  : m_generator(NULL)
{
//   m_generator = new RngStream();
//   m_generator->InitializeStream();
//   m_generator->ResetNthSubstream(RandomVariable::runNumber);
}

RandomVariable::RandomVariable(const RandomVariable& r)
  :m_generator(0)
{
  if(r.m_generator)
  {
    m_generator = new RngStream(*r.m_generator);
  }
}

RandomVariable::~RandomVariable()
{
  delete m_generator;
}

uint32_t RandomVariable::GetIntValue() 
{
  return (uint32_t)GetValue();
}

void RandomVariable::UseDevRandom(bool udr) 
{
  RandomVariable::useDevRandom = udr;
}

void RandomVariable::GetSeed(uint32_t seed[6])
{
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  m_generator->GetState(seed);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RandomVariable static methods
void RandomVariable::UseGlobalSeed(uint32_t s0, uint32_t s1, uint32_t s2, 
                                   uint32_t s3, uint32_t s4, uint32_t s5)
{
  if (RandomVariable::globalSeedSet)
    {
      cerr << "Random number generator already initialized!" << endl;
      cerr << "Call to RandomVariable::UseGlobalSeed() ignored" << endl;
      return;
    }
  RandomVariable::globalSeed[0] = s0;
  RandomVariable::globalSeed[1] = s1;
  RandomVariable::globalSeed[2] = s2;
  RandomVariable::globalSeed[3] = s3;
  RandomVariable::globalSeed[4] = s4;
  RandomVariable::globalSeed[5] = s5;
  if (!RngStream::CheckSeed(RandomVariable::globalSeed))
    NS_FATAL_ERROR("Invalid seed");
  
  RandomVariable::globalSeedSet = true;
}

void RandomVariable::Initialize()
{ 
  if (RandomVariable::initialized) return; // Already initialized and seeded
  RandomVariable::initialized = true;
  if (!RandomVariable::globalSeedSet)
    { // No global seed, try a random one
      GetRandomSeeds(globalSeed);
    }
  // Seed the RngStream package
  RngStream::SetPackageSeed(globalSeed);
}

void RandomVariable::GetRandomSeeds(uint32_t seeds[6])
{
  // Check if /dev/random exists
  if (RandomVariable::useDevRandom && RandomVariable::devRandom < 0)
    {
      RandomVariable::devRandom = open("/dev/random", O_RDONLY);
    }
  if (RandomVariable::devRandom > 0)
    { // Use /dev/random
      while(true)
        {
          for (int i = 0; i < 6; ++i)
            {
              read(RandomVariable::devRandom, &seeds[i], sizeof(seeds[i]));
            }
          if (RngStream::CheckSeed(seeds)) break; // Got a valid one
        }
    }
  else
    { // Seed from time of day (code borrowed from ns2 random seeding)
      // Thanks to John Heidemann for this technique
      while(true)
        {
          timeval tv;
          gettimeofday(&tv, 0);
          seeds[0] = (tv.tv_sec^tv.tv_usec^(++heuristic_sequence <<8))
              & 0x7fffffff;
          gettimeofday(&tv, 0);
          seeds[1] = (tv.tv_sec^tv.tv_usec^(++heuristic_sequence <<8))
              & 0x7fffffff;
          gettimeofday(&tv, 0);
          seeds[2] = (tv.tv_sec^tv.tv_usec^(++heuristic_sequence <<8))
              & 0x7fffffff;
          gettimeofday(&tv, 0);
          seeds[3] = (tv.tv_sec^tv.tv_usec^(++heuristic_sequence <<8))
              & 0x7fffffff;
          gettimeofday(&tv, 0);
          seeds[4] = (tv.tv_sec^tv.tv_usec^(++heuristic_sequence <<8))
              & 0x7fffffff;
          gettimeofday(&tv, 0);
          seeds[5] = (tv.tv_sec^tv.tv_usec^(++heuristic_sequence <<8))
              & 0x7fffffff;
          if (RngStream::CheckSeed(seeds)) break; // Got a valid one
        }
    }
}

void RandomVariable::SetRunNumber(uint32_t n)
{
  runNumber = n;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// UniformVariable
UniformVariable::UniformVariable() 
  : m_min(0), m_max(1.0) { }
  
UniformVariable::UniformVariable(double s, double l) 
  : m_min(s), m_max(l) { }

UniformVariable::UniformVariable(const UniformVariable& c) 
  : RandomVariable(c), m_min(c.m_min), m_max(c.m_max) { }

double UniformVariable::GetValue()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  return m_min + m_generator->RandU01() * (m_max - m_min);
}

RandomVariable* UniformVariable::Copy() const
{
  return new UniformVariable(*this);
}

double UniformVariable::GetSingleValue(double s, double l)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  return s + m_static_generator->RandU01() * (l - s);;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ConstantVariable methods
ConstantVariable::ConstantVariable() 
  : m_const(0) { }

ConstantVariable::ConstantVariable(double c) 
  : m_const(c) { };
  
ConstantVariable::ConstantVariable(const ConstantVariable& c) 
  : RandomVariable(c), m_const(c.m_const) { }

void ConstantVariable::NewConstant(double c) 
  { m_const = c;}
  
double ConstantVariable::GetValue()
{
  return m_const;
}

uint32_t ConstantVariable::GetIntValue()
{
  return (uint32_t)m_const;
}

RandomVariable* ConstantVariable::Copy() const
{
  return new ConstantVariable(*this);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// SequentialVariable methods
SequentialVariable::SequentialVariable(double f, double l, double i, uint32_t c)
  : m_min(f), m_max(l), m_increment(ConstantVariable(i).Copy()), m_consecutive(c),
    m_current(f), m_currentConsecutive(0)
{
}

SequentialVariable::SequentialVariable(double f, double l, const RandomVariable& i, uint32_t c)
  : m_min(f), m_max(l), m_increment(i.Copy()), m_consecutive(c),
    m_current(f), m_currentConsecutive(0)
{
}

SequentialVariable::SequentialVariable(const SequentialVariable& c)
  : RandomVariable(c), m_min(c.m_min), m_max(c.m_max),
    m_increment(c.m_increment->Copy()), m_consecutive(c.m_consecutive),
    m_current(c.m_current), m_currentConsecutive(c.m_currentConsecutive)
{
}

SequentialVariable::~SequentialVariable()
{
  delete m_increment;
}

double SequentialVariable::GetValue()
{ // Return a sequential series of values
  double r = m_current;
  if (++m_currentConsecutive == m_consecutive)
    { // Time to advance to next
      m_currentConsecutive = 0;
      m_current += m_increment->GetValue();
      if (m_current >= m_max)
        m_current = m_min + (m_current - m_max);
    }
  return r;
}

RandomVariable* SequentialVariable::Copy() const
{
  return new SequentialVariable(*this);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ExponentialVariable methods
ExponentialVariable::ExponentialVariable() 
  : m_mean(1.0), m_bound(0) { }
  
ExponentialVariable::ExponentialVariable(double m) 
  : m_mean(m), m_bound(0) { }
  
ExponentialVariable::ExponentialVariable(double m, double b) 
  : m_mean(m), m_bound(b) { }
  
ExponentialVariable::ExponentialVariable(const ExponentialVariable& c) 
  : RandomVariable(c), m_mean(c.m_mean), m_bound(c.m_bound) { }

double ExponentialVariable::GetValue()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  double r = -m_mean*log(m_generator->RandU01());
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariable* ExponentialVariable::Copy() const
{
  return new ExponentialVariable(*this);
}
double ExponentialVariable::GetSingleValue(double m, double b/*=0*/)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  double r = -m*log(m_static_generator->RandU01());
  if (b != 0 && r > b) return b;
  return r;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ParetoVariable methods
ParetoVariable::ParetoVariable() 
  : m_mean(1.0), m_shape(1.5), m_bound(0) { }

ParetoVariable::ParetoVariable(double m) 
  : m_mean(m), m_shape(1.5), m_bound(0) { }

ParetoVariable::ParetoVariable(double m, double s) 
    : m_mean(m), m_shape(s), m_bound(0) { }

ParetoVariable::ParetoVariable(double m, double s, double b) 
  : m_mean(m), m_shape(s), m_bound(b) { }

ParetoVariable::ParetoVariable(const ParetoVariable& c) 
  : RandomVariable(c), m_mean(c.m_mean), m_shape(c.m_shape), 
    m_bound(c.m_bound) { }

double ParetoVariable::GetValue()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  double scale = m_mean * ( m_shape - 1.0) / m_shape;
  double r = (scale * ( 1.0 / pow(m_generator->RandU01(), 1.0 / m_shape)));
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariable* ParetoVariable::Copy() const
{
  return new ParetoVariable(*this);
}

double ParetoVariable::GetSingleValue(double m, double s, double b/*=0*/)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  double scale = m * ( s - 1.0) / s;
  double r = (scale * ( 1.0 / pow(m_static_generator->RandU01(), 1.0 / s)));
  if (b != 0 && r > b) return b;
  return r;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// WeibullVariable methods
WeibullVariable::WeibullVariable() : m_mean(1.0), m_alpha(1), m_bound(0) { }
WeibullVariable::WeibullVariable(double m) 
  : m_mean(m), m_alpha(1), m_bound(0) { }
WeibullVariable::WeibullVariable(double m, double s) 
  : m_mean(m), m_alpha(s), m_bound(0) { }
WeibullVariable::WeibullVariable(double m, double s, double b) 
  : m_mean(m), m_alpha(s), m_bound(b) { };
WeibullVariable::WeibullVariable(const WeibullVariable& c) 
  : RandomVariable(c), m_mean(c.m_mean), m_alpha(c.m_alpha),
    m_bound(c.m_bound) { }

double WeibullVariable::GetValue()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  double exponent = 1.0 / m_alpha;
  double r = m_mean * pow( -log(m_generator->RandU01()), exponent);
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariable* WeibullVariable::Copy() const
{
  return new WeibullVariable(*this);
}

double WeibullVariable::GetSingleValue(double m, double s, double b/*=0*/)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  double exponent = 1.0 / s;
  double r = m * pow( -log(m_static_generator->RandU01()), exponent);
  if (b != 0 && r > b) return b;
  return r;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// NormalVariable methods
bool         NormalVariable::m_static_nextValid = false;
double       NormalVariable::m_static_next;
const double NormalVariable::INFINITE_VALUE = 1e307;

NormalVariable::NormalVariable() 
  : m_mean(0.0), m_variance(1.0), m_bound(INFINITE_VALUE), m_nextValid(false){}

NormalVariable::NormalVariable(double m, double v, double b/*=INFINITE_VALUE*/)
  : m_mean(m), m_variance(v), m_bound(b), m_nextValid(false) { }

NormalVariable::NormalVariable(const NormalVariable& c)
  : RandomVariable(c), m_mean(c.m_mean), m_variance(c.m_variance),
    m_bound(c.m_bound) { }

double NormalVariable::GetValue()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  if (m_nextValid)
    { // use previously generated
      m_nextValid = false;
      return m_next;
    }
  while(1)
    { // See Simulation Modeling and Analysis p. 466 (Averill Law)
      // for algorithm
      double u1 = m_generator->RandU01();
      double u2 = m_generator->RandU01();;
      double v1 = 2 * u1 - 1;
      double v2 = 2 * u2 - 1;
      double w = v1 * v1 + v2 * v2;
      if (w <= 1.0)
        { // Got good pair
          double y = sqrt((-2 * log(w))/w);
          m_next = m_mean + v2 * y * sqrt(m_variance);
          if (fabs(m_next) > m_bound) m_next = m_bound * (m_next)/fabs(m_next);
          m_nextValid = true;
          double x1 = m_mean + v1 * y * sqrt(m_variance);
          if (fabs(x1) > m_bound) x1 = m_bound * (x1)/fabs(x1);
          return x1;
        }
    }
}

RandomVariable* NormalVariable::Copy() const
{
  return new NormalVariable(*this);
}

double NormalVariable::GetSingleValue(double m, double v, double b)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  if (m_static_nextValid)
    { // use previously generated
      m_static_nextValid = false;
      return m_static_next;
    }
  while(1)
    { // See Simulation Modeling and Analysis p. 466 (Averill Law)
      // for algorithm
      double u1 = m_static_generator->RandU01();
      double u2 = m_static_generator->RandU01();;
      double v1 = 2 * u1 - 1;
      double v2 = 2 * u2 - 1;
      double w = v1 * v1 + v2 * v2;
      if (w <= 1.0)
        { // Got good pair
          double y = sqrt((-2 * log(w))/w);
          m_static_next = m + v2 * y * sqrt(v);
          if (fabs(m_static_next) > b) m_static_next = b * (m_static_next)/fabs(m_static_next);
          m_static_nextValid = true;
          double x1 = m + v1 * y * sqrt(v);
          if (fabs(x1) > b) x1 = b * (x1)/fabs(x1);
          return x1;
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ValueCDF methods
EmpiricalVariable::ValueCDF::ValueCDF() 
  : value(0.0), cdf(0.0){ }
EmpiricalVariable::ValueCDF::ValueCDF(double v, double c) 
  : value(v), cdf(c) { }
EmpiricalVariable::ValueCDF::ValueCDF(const ValueCDF& c) 
  : value(c.value), cdf(c.cdf) { }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// EmpiricalVariable methods
EmpiricalVariable::EmpiricalVariable() 
  : validated(false) { }

EmpiricalVariable::EmpiricalVariable(const EmpiricalVariable& c)
  : RandomVariable(c), validated(c.validated), emp(c.emp) { }

EmpiricalVariable::~EmpiricalVariable() { }

double EmpiricalVariable::GetValue()
{ // Return a value from the empirical distribution
  // This code based (loosely) on code by Bruce Mah (Thanks Bruce!)
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  if (emp.size() == 0) return 0.0; // HuH? No empirical data
  if (!validated) Validate();      // Insure in non-decreasing
  double r = m_generator->RandU01();
  if (r <= emp.front().cdf)return emp.front().value; // Less than first
  if (r >= emp.back().cdf) return emp.back().value;  // Greater than last
  // Binary search
  std::vector<ValueCDF>::size_type bottom = 0;
  std::vector<ValueCDF>::size_type top = emp.size() - 1;
  while(1)
    {
      std::vector<ValueCDF>::size_type c = (top + bottom) / 2;
      if (r >= emp[c].cdf && r < emp[c+1].cdf)
        { // Found it
          return Interpolate(emp[c].cdf, emp[c+1].cdf,
                             emp[c].value, emp[c+1].value,
                             r);
        }
      // Not here, adjust bounds
      if (r < emp[c].cdf) top    = c - 1;
      else                bottom = c + 1;
    }
}

RandomVariable* EmpiricalVariable::Copy() const
{
  return new EmpiricalVariable(*this);
}

void EmpiricalVariable::CDF(double v, double c)
{ // Add a new empirical datapoint to the empirical cdf
  // NOTE.   These MUST be inserted in non-decreasing order
  emp.push_back(ValueCDF(v, c));
}

void EmpiricalVariable::Validate()
{
  ValueCDF prior;
  for (std::vector<ValueCDF>::size_type i = 0; i < emp.size(); ++i)
    {
      ValueCDF& current = emp[i];
      if (current.value < prior.value || current.cdf < prior.cdf)
        { // Error
          cerr << "Empirical Dist error,"
               << " current value " << current.value
               << " prior value "   << prior.value
               << " current cdf "   << current.cdf
               << " prior cdf "     << prior.cdf << endl;
          NS_FATAL_ERROR("Empirical Dist error");
        }
      prior = current;
    }
  validated = true;
}

double EmpiricalVariable::Interpolate(double c1, double c2,
                                double v1, double v2, double r)
{ // Interpolate random value in range [v1..v2) based on [c1 .. r .. c2)
  return (v1 + ((v2 - v1) / (c2 - c1)) * (r - c1));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Integer EmpiricalVariable methods
IntEmpiricalVariable::IntEmpiricalVariable() { }

uint32_t IntEmpiricalVariable::GetIntValue()
{
  return (uint32_t)GetValue();
}

RandomVariable* IntEmpiricalVariable::Copy() const
{
  return new IntEmpiricalVariable(*this);
}

double IntEmpiricalVariable::Interpolate(double c1, double c2,
                                   double v1, double v2, double r)
{ // Interpolate random value in range [v1..v2) based on [c1 .. r .. c2)
  return ceil(v1 + ((v2 - v1) / (c2 - c1)) * (r - c1));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// DeterministicVariable
DeterministicVariable::DeterministicVariable(double* d, uint32_t c)
    : count(c), next(c), data(d)
{ // Nothing else needed
}

DeterministicVariable::~DeterministicVariable() { }
  
double DeterministicVariable::GetValue()
{
  if (next == count) next = 0;
  return data[next++];
}

RandomVariable* DeterministicVariable::Copy() const
{
  return new DeterministicVariable(*this);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// LogNormalVariable

RandomVariable* LogNormalVariable::Copy () const
{
  return new LogNormalVariable (m_mu, m_sigma);
}

LogNormalVariable::LogNormalVariable (double mu, double sigma)
    :m_mu(mu), m_sigma(sigma) 
{
}

// The code from this function was adapted from the GNU Scientific
// Library 1.8:
/* randist/lognormal.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 James Theiler, Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
/* The lognormal distribution has the form 

   p(x) dx = 1/(x * sqrt(2 pi sigma^2)) exp(-(ln(x) - zeta)^2/2 sigma^2) dx

   for x > 0. Lognormal random numbers are the exponentials of
   gaussian random numbers */
double
LogNormalVariable::GetValue ()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  double u, v, r2, normal, z;

  do
    {
      /* choose x,y in uniform square (-1,-1) to (+1,+1) */

      u = -1 + 2 * m_generator->RandU01 ();
      v = -1 + 2 * m_generator->RandU01 ();

      /* see if it is in the unit circle */
      r2 = u * u + v * v;
    }
  while (r2 > 1.0 || r2 == 0);

  normal = u * sqrt (-2.0 * log (r2) / r2);

  z =  exp (m_sigma * normal + m_mu);

  return z;
}

double LogNormalVariable::GetSingleValue (double mu, double sigma)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  double u, v, r2, normal, z;
  do
    {
      /* choose x,y in uniform square (-1,-1) to (+1,+1) */
      u = -1 + 2 * m_static_generator->RandU01 ();
      v = -1 + 2 * m_static_generator->RandU01 ();

      /* see if it is in the unit circle */
      r2 = u * u + v * v;
    }
  while (r2 > 1.0 || r2 == 0);

  normal = u * sqrt (-2.0 * log (r2) / r2);

  z =  exp (sigma * normal + mu);

  return z;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// TriangularVariable methods
TriangularVariable::TriangularVariable() 
  : m_min(0), m_max(1), m_mode(0.5) { }
  
TriangularVariable::TriangularVariable(double s, double l, double mean) 
  : m_min(s), m_max(l), m_mode(3.0*mean-s-l) { }
  
TriangularVariable::TriangularVariable(const TriangularVariable& c) 
  : RandomVariable(c), m_min(c.m_min), m_max(c.m_max), m_mode(c.m_mode) { }

double TriangularVariable::GetValue()
{
  if(!RandomVariable::initialized)
  {
    RandomVariable::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariable::runNumber);
  }
  double u = m_generator->RandU01();
  if(u <= (m_mode - m_min) / (m_max - m_min) )
    return m_min + sqrt(u * (m_max - m_min) * (m_mode - m_min) );
  else
    return m_max - sqrt( (1-u) * (m_max - m_min) * (m_max - m_mode) );
}

RandomVariable* TriangularVariable::Copy() const
{
  return new TriangularVariable(*this);
}

double TriangularVariable::GetSingleValue(double s, double l, double mean)
{
  if(!RandomVariable::m_static_generator)
  {
    RandomVariable::Initialize(); // sets the static package seed
    RandomVariable::m_static_generator = new RngStream();
    RandomVariable::m_static_generator->InitializeStream();
  }
  double mode = 3.0*mean-s-l;
  double u = m_static_generator->RandU01();
  if(u <= (mode - s) / (l - s) )
    return s + sqrt(u * (l - s) * (mode - s) );
  else
    return l - sqrt( (1-u) * (l - s) * (l - mode) );
}


}//namespace ns3


#ifdef RUN_SELF_TESTS
#include "test.h"
#include <vector>

namespace ns3 {


class RandomVariableTest : public Test
{
public:
  RandomVariableTest () : Test ("RandomVariable") {}
  virtual bool RunTests (void)
  {
    bool ok = true;
    const double desired_mean = 1.0;
    const double desired_stddev = 1.0;
    double tmp = log (1 + (desired_stddev/desired_mean)*(desired_stddev/desired_mean));
    double sigma = sqrt (tmp);
    double mu = log (desired_mean) - 0.5*tmp;

    // Test a custom lognormal instance
    {
      LogNormalVariable lognormal (mu, sigma);
      vector<double> samples;
      const int NSAMPLES = 10000;
      double sum = 0;
      for (int n = NSAMPLES; n; --n)
        {
          double value = lognormal.GetValue ();
          sum += value;
          samples.push_back (value);
        }
      double obtained_mean = sum / NSAMPLES;
      sum = 0;
      for (vector<double>::iterator iter = samples.begin (); iter != samples.end (); iter++)
        {
          double tmp = (*iter - obtained_mean);
          sum += tmp*tmp;
        }
      double obtained_stddev = sqrt (sum / (NSAMPLES - 1));

      if (not (obtained_mean/desired_mean > 0.90 and obtained_mean/desired_mean < 1.10))
        {
          ok = false;
          Failure () << "Obtained lognormal mean value " << obtained_mean << ", expected " << desired_mean << std::endl;
        }

      if (not (obtained_stddev/desired_stddev > 0.90 and obtained_stddev/desired_stddev < 1.10))
        {
          ok = false;
          Failure () << "Obtained lognormal stddev value " << obtained_stddev <<
            ", expected " << desired_stddev << std::endl;
        }
    }

    // Test GetSingleValue
    {
      vector<double> samples;
      const int NSAMPLES = 10000;
      double sum = 0;
      for (int n = NSAMPLES; n; --n)
        {
          double value = LogNormalVariable::GetSingleValue (mu, sigma);
          sum += value;
          samples.push_back (value);
        }
      double obtained_mean = sum / NSAMPLES;
      sum = 0;
      for (vector<double>::iterator iter = samples.begin (); iter != samples.end (); iter++)
        {
          double tmp = (*iter - obtained_mean);
          sum += tmp*tmp;
        }
      double obtained_stddev = sqrt (sum / (NSAMPLES - 1));

      if (not (obtained_mean/desired_mean > 0.90 and obtained_mean/desired_mean < 1.10))
        {
          ok = false;
          Failure () << "Obtained LogNormalVariable::GetSingleValue mean value " << obtained_mean
                     << ", expected " << desired_mean << std::endl;
        }

      if (not (obtained_stddev/desired_stddev > 0.90 and obtained_stddev/desired_stddev < 1.10))
        {
          ok = false;
          Failure () << "Obtained LogNormalVariable::GetSingleValue stddev value " << obtained_stddev <<
            ", expected " << desired_stddev << std::endl;
        }
    }

    return ok;
  }
};


static RandomVariableTest g_random_variable_tests;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
