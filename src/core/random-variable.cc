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
// Seed methods

Seed::~Seed()
{
}

RandomSeed::RandomSeed()
{
}

RandomSeed::~RandomSeed()
{
}

bool RandomSeed::IsRandom() const 
{
  return true;
}

ConstantSeed::~ConstantSeed()
{
}

bool ConstantSeed::IsRandom() const 
{
  return false;
}

ConstantSeed::ConstantSeed(uint32_t s)
{
  seeds[0] = s;
  seeds[1] = s;
  seeds[2] = s;
  seeds[3] = s;
  seeds[4] = s;
  seeds[5] = s;
}

ConstantSeed::ConstantSeed(uint32_t s0, uint32_t s1, uint32_t s2,
                           uint32_t s3, uint32_t s4, uint32_t s5)
{
  seeds[0] = s0;
  seeds[1] = s1;
  seeds[2] = s2;
  seeds[3] = s3;
  seeds[4] = s4;
  seeds[5] = s5;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RandomVariable methods

uint32_t      RandomVariable::runNumber = 0;
bool          RandomVariable::initialized = false;   // True if RngStream seed set 
bool          RandomVariable::useDevRandom = false;  // True if use /dev/random desired
bool          RandomVariable::globalSeedSet = false; // True if GlobalSeed called
int           RandomVariable::devRandom = -1;
uint32_t        RandomVariable::globalSeed[6];
unsigned long RandomVariable::heuristic_sequence;

RandomVariable::RandomVariable() 
{
  m_generator = new RngStream();
  RandomVariable::Initialize(); // sets the seed for the static object
  m_generator->InitializeStream();
  m_generator->ResetNthSubstream(RandomVariable::runNumber);
}

RandomVariable::RandomVariable(const RandomVariable& r)
{
  m_generator = new RngStream(*r.m_generator);
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
  m_generator->GetState(seed);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RandomVariable static methods
void RandomVariable::UseGlobalSeed(const Seed& s)
{
  if (RandomVariable::globalSeedSet)
    {
      cout << "Random number generator already initialized!" << endl;
      cout << "Call to RandomVariable::UseGlobalSeed() ignored" << endl;
      return;
    }
  if (s.IsRandom()) return; // Random seed is the default
  const ConstantSeed& cs = (ConstantSeed&)s;
  RandomVariable::globalSeed[0] = cs.seeds[0];
  RandomVariable::globalSeed[1] = cs.seeds[1];
  RandomVariable::globalSeed[2] = cs.seeds[2];
  RandomVariable::globalSeed[3] = cs.seeds[3];
  RandomVariable::globalSeed[4] = cs.seeds[4];
  RandomVariable::globalSeed[5] = cs.seeds[5];
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
// UniformVariable methods
UniformVariable::UniformVariable() 
  : m_min(0), m_max(1.0) { }
  
UniformVariable::UniformVariable(double s, double l) 
  : m_min(s), m_max(l) { }

UniformVariable::UniformVariable(const UniformVariable& c) 
  : RandomVariable(c), m_min(c.m_min), m_max(c.m_max) { }

double UniformVariable::GetValue()
{
  return m_min + m_generator->RandU01() * (m_max - m_min);
}

RandomVariable* UniformVariable::Copy() const
{
  return new UniformVariable(*this);
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
  double r = -m_mean*log(m_generator->RandU01());
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariable* ExponentialVariable::Copy() const
{
  return new ExponentialVariable(*this);
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
  double scale = m_mean * ( m_shape - 1.0) / m_shape;
  double r = (scale * ( 1.0 / pow(m_generator->RandU01(), 1.0 / m_shape)));
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariable* ParetoVariable::Copy() const
{
  return new ParetoVariable(*this);
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
  double exponent = 1.0 / m_alpha;
  double r = m_mean * pow( -log(m_generator->RandU01()), exponent);
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariable* WeibullVariable::Copy() const
{
  return new WeibullVariable(*this);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// NormalVariable methods
NormalVariable::NormalVariable() 
  : m_mean(0.0), m_variance(1.0), m_bound(INFINITE_VALUE), m_nextValid(false){}

NormalVariable::NormalVariable(double m, double v, double b)
  : m_mean(m), m_variance(v), m_bound(b), m_nextValid(false) { }

NormalVariable::NormalVariable(const NormalVariable& c)
  : RandomVariable(c), m_mean(c.m_mean), m_variance(c.m_variance),
    m_bound(c.m_bound) { }

double NormalVariable::GetValue()
{
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ValueCDF methods
ValueCDF::ValueCDF() 
  : value(0.0), cdf(0.0){ }
ValueCDF::ValueCDF(double v, double c) 
  : value(v), cdf(c) { }
ValueCDF::ValueCDF(const ValueCDF& c) 
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
          cout << "Empirical Dist error,"
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

}//namespace ns3

