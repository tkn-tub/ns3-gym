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
#include <sstream>


#include "assert.h"
#include "random-variable.h"
#include "rng-stream.h"
#include "fatal-error.h"

using namespace std;

namespace ns3{

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RandomVariableBase methods


class RandomVariableBase 
{
public:
  RandomVariableBase ();
  RandomVariableBase (const RandomVariableBase &o);
  virtual ~RandomVariableBase();
  virtual double  GetValue() = 0;
  virtual uint32_t GetIntValue();
  virtual RandomVariableBase*   Copy(void) const = 0;
  virtual void GetSeed(uint32_t seed[6]);

  static  void UseDevRandom(bool udr = true);
  static void UseGlobalSeed(uint32_t s0, uint32_t s1, uint32_t s2, 
                            uint32_t s3, uint32_t s4, uint32_t s5);
  static void SetRunNumber(uint32_t n);
private:
  static void GetRandomSeeds(uint32_t seeds[6]);
private:
  static bool useDevRandom;    // True if using /dev/random desired
  static bool globalSeedSet;   // True if global seed has been specified
  static int  devRandom;       // File handle for /dev/random
  static uint32_t globalSeed[6]; // The global seed to use
  friend class RandomVariableInitializer;
protected:
  static unsigned long heuristic_sequence;
  static RngStream* m_static_generator;
  static uint32_t runNumber;
  static void Initialize();    // Initialize  the RNG system
  static bool initialized;     // True if package seed is set 
  RngStream* m_generator;  //underlying generator being wrapped
};



bool          RandomVariableBase::initialized = false;   // True if RngStream seed set 
bool          RandomVariableBase::useDevRandom = false;  // True if use /dev/random
bool          RandomVariableBase::globalSeedSet = false; // True if GlobalSeed called
int           RandomVariableBase::devRandom = -1;
uint32_t      RandomVariableBase::globalSeed[6];
unsigned long RandomVariableBase::heuristic_sequence;
RngStream*    RandomVariableBase::m_static_generator = 0;
uint32_t      RandomVariableBase::runNumber = 0;

//the static object random_variable_initializer initializes the static members
//of RandomVariable
static class RandomVariableInitializer
{
  public:
  RandomVariableInitializer()
  {
//     RandomVariableBase::Initialize(); // sets the static package seed
//     RandomVariableBase::m_static_generator = new RngStream();
//     RandomVariableBase::m_static_generator->InitializeStream();
  }
  ~RandomVariableInitializer()
  {
    delete RandomVariableBase::m_static_generator;
  }
} random_variable_initializer;

RandomVariableBase::RandomVariableBase() 
  : m_generator(NULL)
{
//   m_generator = new RngStream();
//   m_generator->InitializeStream();
//   m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
}

RandomVariableBase::RandomVariableBase(const RandomVariableBase& r)
  :m_generator(0)
{
  if(r.m_generator)
  {
    m_generator = new RngStream(*r.m_generator);
  }
}

RandomVariableBase::~RandomVariableBase()
{
  delete m_generator;
}

uint32_t RandomVariableBase::GetIntValue() 
{
  return (uint32_t)GetValue();
}

void RandomVariableBase::UseDevRandom(bool udr) 
{
  RandomVariableBase::useDevRandom = udr;
}

void RandomVariableBase::GetSeed(uint32_t seed[6])
{
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
  }
  m_generator->GetState(seed);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// RandomVariableBase static methods
void RandomVariableBase::UseGlobalSeed(uint32_t s0, uint32_t s1, uint32_t s2, 
                                   uint32_t s3, uint32_t s4, uint32_t s5)
{
  if (RandomVariableBase::globalSeedSet)
    {
      cerr << "Random number generator already initialized!" << endl;
      cerr << "Call to RandomVariableBase::UseGlobalSeed() ignored" << endl;
      return;
    }
  RandomVariableBase::globalSeed[0] = s0;
  RandomVariableBase::globalSeed[1] = s1;
  RandomVariableBase::globalSeed[2] = s2;
  RandomVariableBase::globalSeed[3] = s3;
  RandomVariableBase::globalSeed[4] = s4;
  RandomVariableBase::globalSeed[5] = s5;
  if (!RngStream::CheckSeed(RandomVariableBase::globalSeed))
    NS_FATAL_ERROR("Invalid seed");
  
  RandomVariableBase::globalSeedSet = true;
}

void RandomVariableBase::Initialize()
{ 
  if (RandomVariableBase::initialized) return; // Already initialized and seeded
  RandomVariableBase::initialized = true;
  if (!RandomVariableBase::globalSeedSet)
    { // No global seed, try a random one
      GetRandomSeeds(globalSeed);
    }
  // Seed the RngStream package
  RngStream::SetPackageSeed(globalSeed);
}

void RandomVariableBase::GetRandomSeeds(uint32_t seeds[6])
{
  // Check if /dev/random exists
  if (RandomVariableBase::useDevRandom && RandomVariableBase::devRandom < 0)
    {
      RandomVariableBase::devRandom = open("/dev/random", O_RDONLY);
    }
  if (RandomVariableBase::devRandom > 0)
    { // Use /dev/random
      while(true)
        {
          for (int i = 0; i < 6; ++i)
            {
              read(RandomVariableBase::devRandom, &seeds[i], sizeof(seeds[i]));
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

void RandomVariableBase::SetRunNumber(uint32_t n)
{
  runNumber = n;
}



RandomVariable::RandomVariable()
  : m_variable (0)
{}
RandomVariable::RandomVariable(const RandomVariable&o)
  : m_variable (o.m_variable->Copy ())
{}
RandomVariable::RandomVariable (const RandomVariableBase &variable)
  : m_variable (variable.Copy ())
{}
RandomVariable &
RandomVariable::operator = (const RandomVariable &o)
{
  if (&o == this)
    {
      return *this;
    }
  delete m_variable;
  m_variable = o.m_variable->Copy ();
  return *this;
}
RandomVariable::~RandomVariable()
{
  delete m_variable;
}
double  
RandomVariable::GetValue (void) const
{
  return m_variable->GetValue ();
}

uint32_t 
RandomVariable::GetIntValue (void) const
{
  return m_variable->GetIntValue ();
}
void 
RandomVariable::GetSeed(uint32_t seed[6]) const
{
  return m_variable->GetSeed (seed);
}
void 
RandomVariable::UseDevRandom(bool udr)
{
  RandomVariableBase::UseDevRandom (udr);
}
void 
RandomVariable::UseGlobalSeed(uint32_t s0, uint32_t s1, uint32_t s2, 
                              uint32_t s3, uint32_t s4, uint32_t s5)
{
  RandomVariableBase::UseGlobalSeed (s0, s1, s2, s3, s4, s5);
}
void 
RandomVariable::SetRunNumber(uint32_t n)
{
  RandomVariableBase::SetRunNumber (n);
}
RandomVariableBase *
RandomVariable::Peek (void) const
{
  return m_variable;
}
RandomVariable::RandomVariable (PValue value)
  : m_variable (0)
{
  *this = ClassValueHelperExtractFrom<RandomVariable,RandomVariableValue> (value);
}
RandomVariable::operator PValue () const
{
  return ClassValueHelperConvertTo<RandomVariable,RandomVariableValue> (this);
}

Ptr<AttributeChecker> 
MakeRandomVariableChecker (void)
{
  return MakeSimpleAttributeChecker<RandomVariableValue> ();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// UniformVariableImpl

class UniformVariableImpl : public RandomVariableBase {
public:
  /**
   * Creates a uniform random number generator in the
   * range [0.0 .. 1.0).
   */
  UniformVariableImpl();

  /**
   * Creates a uniform random number generator with the specified range
   * \param s Low end of the range
   * \param l High end of the range
   */
  UniformVariableImpl(double s, double l);

  UniformVariableImpl(const UniformVariableImpl& c);

  double GetMin (void) const;
  double GetMax (void) const;
  
  /**
   * \return A value between low and high values specified by the constructor
   */
  virtual double GetValue();
  virtual RandomVariableBase*  Copy(void) const;

public:
  /**
   * \param s Low end of the range
   * \param l High end of the range
   * \return A uniformly distributed random number between s and l
   */
  static double GetSingleValue(double s, double l);
private:
  double m_min;
  double m_max;
};

UniformVariableImpl::UniformVariableImpl() 
  : m_min(0), m_max(1.0) { }
  
UniformVariableImpl::UniformVariableImpl(double s, double l) 
  : m_min(s), m_max(l) { }

UniformVariableImpl::UniformVariableImpl(const UniformVariableImpl& c) 
  : RandomVariableBase(c), m_min(c.m_min), m_max(c.m_max) { }

double 
UniformVariableImpl::GetMin (void) const
{
  return m_min;
}
double 
UniformVariableImpl::GetMax (void) const
{
  return m_max;
}


double UniformVariableImpl::GetValue()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
  }
  return m_min + m_generator->RandU01() * (m_max - m_min);
}

RandomVariableBase* UniformVariableImpl::Copy() const
{
  return new UniformVariableImpl(*this);
}

double UniformVariableImpl::GetSingleValue(double s, double l)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
  }
  return s + m_static_generator->RandU01() * (l - s);;
}

UniformVariable::UniformVariable()
  : RandomVariable (UniformVariableImpl ())
{}
UniformVariable::UniformVariable(double s, double l)
  : RandomVariable (UniformVariableImpl (s, l))
{}
double 
UniformVariable::GetSingleValue(double s, double l)
{
  return UniformVariableImpl::GetSingleValue (s, l);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ConstantVariableImpl methods

class ConstantVariableImpl : public RandomVariableBase { 

public:
  /**
   * Construct a ConstantVariableImpl RNG that returns zero every sample
   */
  ConstantVariableImpl();
  
  /**
   * Construct a ConstantVariableImpl RNG that returns the specified value
   * every sample.
   * \param c Unchanging value for this RNG.
   */
  ConstantVariableImpl(double c);


  ConstantVariableImpl(const ConstantVariableImpl& c) ;

  /**
   * \brief Specify a new constant RNG for this generator.
   * \param c New constant value for this RNG.
   */
  void    NewConstant(double c);

  /**
   * \return The constant value specified
   */
  virtual double  GetValue();
  virtual uint32_t GetIntValue();
  virtual RandomVariableBase*   Copy(void) const;
private:
  double m_const;
};

ConstantVariableImpl::ConstantVariableImpl() 
  : m_const(0) { }

ConstantVariableImpl::ConstantVariableImpl(double c) 
  : m_const(c) { };
  
ConstantVariableImpl::ConstantVariableImpl(const ConstantVariableImpl& c) 
  : RandomVariableBase(c), m_const(c.m_const) { }

void ConstantVariableImpl::NewConstant(double c) 
  { m_const = c;}
  
double ConstantVariableImpl::GetValue()
{
  return m_const;
}

uint32_t ConstantVariableImpl::GetIntValue()
{
  return (uint32_t)m_const;
}

RandomVariableBase* ConstantVariableImpl::Copy() const
{
  return new ConstantVariableImpl(*this);
}

ConstantVariable::ConstantVariable()
  : RandomVariable (ConstantVariableImpl ())
{}
ConstantVariable::ConstantVariable(double c)
  : RandomVariable (ConstantVariableImpl (c))
{}
void 
ConstantVariable::SetConstant(double c)
{
  *this = ConstantVariable (c);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// SequentialVariableImpl methods


class SequentialVariableImpl : public RandomVariableBase {

public:
  /**
   * \brief Constructor for the SequentialVariableImpl RNG.
   *
   * The four parameters define the sequence.  For example
   * SequentialVariableImpl(0,5,1,2) creates a RNG that has the sequence
   * 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 0, 0 ...
   * \param f First value of the sequence.
   * \param l One more than the last value of the sequence.
   * \param i Increment between sequence values
   * \param c Number of times each member of the sequence is repeated
   */
  SequentialVariableImpl(double f, double l, double i = 1, uint32_t c = 1);

  /**
   * \brief Constructor for the SequentialVariableImpl RNG.
   *
   * Differs from the first only in that the increment parameter is a
   * random variable
   * \param f First value of the sequence.
   * \param l One more than the last value of the sequence.
   * \param i Reference to a RandomVariableBase for the sequence increment
   * \param c Number of times each member of the sequence is repeated
   */
  SequentialVariableImpl(double f, double l, const RandomVariable& i, uint32_t c = 1);

  SequentialVariableImpl(const SequentialVariableImpl& c);
  
  ~SequentialVariableImpl();
  /**
   * \return The next value in the Sequence
   */
  virtual double GetValue();
  virtual RandomVariableBase*  Copy(void) const;
private:
  double m_min;
  double m_max;
  RandomVariable  m_increment;
  uint32_t  m_consecutive;
  double m_current;
  uint32_t  m_currentConsecutive;
};

SequentialVariableImpl::SequentialVariableImpl(double f, double l, double i, uint32_t c)
  : m_min(f), m_max(l), m_increment(ConstantVariable(i)), m_consecutive(c),
    m_current(f), m_currentConsecutive(0)
{}

SequentialVariableImpl::SequentialVariableImpl(double f, double l, const RandomVariable& i, uint32_t c)
  : m_min(f), m_max(l), m_increment(i), m_consecutive(c),
    m_current(f), m_currentConsecutive(0)
{}

SequentialVariableImpl::SequentialVariableImpl(const SequentialVariableImpl& c)
  : RandomVariableBase(c), m_min(c.m_min), m_max(c.m_max),
    m_increment(c.m_increment), m_consecutive(c.m_consecutive),
    m_current(c.m_current), m_currentConsecutive(c.m_currentConsecutive)
{}

SequentialVariableImpl::~SequentialVariableImpl()
{}

double SequentialVariableImpl::GetValue()
{ // Return a sequential series of values
  double r = m_current;
  if (++m_currentConsecutive == m_consecutive)
    { // Time to advance to next
      m_currentConsecutive = 0;
      m_current += m_increment.GetValue();
      if (m_current >= m_max)
        m_current = m_min + (m_current - m_max);
    }
  return r;
}

RandomVariableBase* SequentialVariableImpl::Copy() const
{
  return new SequentialVariableImpl(*this);
}

SequentialVariable::SequentialVariable(double f, double l, double i, uint32_t c)
  : RandomVariable (SequentialVariableImpl (f, l, i, c))
{}
SequentialVariable::SequentialVariable(double f, double l, const RandomVariable& i, uint32_t c)
  : RandomVariable (SequentialVariableImpl (f, l, i, c))
{}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ExponentialVariableImpl methods

class ExponentialVariableImpl : public RandomVariableBase { 
public:
  /**
   * Constructs an exponential random variable  with a mean
   * value of 1.0.
   */
  ExponentialVariableImpl();

  /**
   * \brief Constructs an exponential random variable with a specified mean
   * \param m Mean value for the random variable
   */
  explicit ExponentialVariableImpl(double m);

  /**
   * \brief Constructs an exponential random variable with spefified
   * \brief mean and upper limit.
   *
   * Since exponential distributions can theoretically return unbounded values,
   * it is sometimes useful to specify a fixed upper limit.  Note however when
   * the upper limit is specified, the true mean of the distribution is 
   * slightly smaller than the mean value specified.
   * \param m Mean value of the random variable
   * \param b Upper bound on returned values
   */
  ExponentialVariableImpl(double m, double b);

  ExponentialVariableImpl(const ExponentialVariableImpl& c);
  
  /**
   * \return A random value from this exponential distribution
   */
  virtual double GetValue();
  virtual RandomVariableBase* Copy(void) const;
public:
  /**
   * \param m The mean of the distribution from which the return value is drawn
   * \param b The upper bound value desired, beyond which values get clipped
   * \return A random number from an exponential distribution with mean m
   */
  static double GetSingleValue(double m, double b=0);
private:
  double m_mean;  // Mean value of RV
  double m_bound; // Upper bound on value (if non-zero)
};

ExponentialVariableImpl::ExponentialVariableImpl() 
  : m_mean(1.0), m_bound(0) { }
  
ExponentialVariableImpl::ExponentialVariableImpl(double m) 
  : m_mean(m), m_bound(0) { }
  
ExponentialVariableImpl::ExponentialVariableImpl(double m, double b) 
  : m_mean(m), m_bound(b) { }
  
ExponentialVariableImpl::ExponentialVariableImpl(const ExponentialVariableImpl& c) 
  : RandomVariableBase(c), m_mean(c.m_mean), m_bound(c.m_bound) { }

double ExponentialVariableImpl::GetValue()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
  }
  double r = -m_mean*log(m_generator->RandU01());
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariableBase* ExponentialVariableImpl::Copy() const
{
  return new ExponentialVariableImpl(*this);
}
double ExponentialVariableImpl::GetSingleValue(double m, double b/*=0*/)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
  }
  double r = -m*log(m_static_generator->RandU01());
  if (b != 0 && r > b) return b;
  return r;
}

ExponentialVariable::ExponentialVariable()
  : RandomVariable (ExponentialVariableImpl ())
{}
ExponentialVariable::ExponentialVariable(double m)
  : RandomVariable (ExponentialVariableImpl (m))
{}
ExponentialVariable::ExponentialVariable(double m, double b)
  : RandomVariable (ExponentialVariableImpl (m, b))
{}
double 
ExponentialVariable::GetSingleValue(double m, double b)
{
  return ExponentialVariableImpl::GetSingleValue (m, b);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ParetoVariableImpl methods
class ParetoVariableImpl : public RandomVariableBase {
public:
  /**
   * Constructs a pareto random variable with a mean of 1 and a shape
   * parameter of 1.5
   */
  ParetoVariableImpl();

  /**
   * Constructs a pareto random variable with specified mean and shape
   * parameter of 1.5
   * \param m Mean value of the distribution
   */
  explicit ParetoVariableImpl(double m);

  /**
   * Constructs a pareto random variable with the specified mean value and
   * shape parameter.
   * \param m Mean value of the distribution
   * \param s Shape parameter for the distribution
   */
  ParetoVariableImpl(double m, double s);

  /**
   * \brief Constructs a pareto random variable with the specified mean
   * \brief value, shape (alpha), and upper bound.
   *
   * Since pareto distributions can theoretically return unbounded values,
   * it is sometimes useful to specify a fixed upper limit.  Note however
   * when the upper limit is specified, the true mean of the distribution
   * is slightly smaller than the mean value specified.
   * \param m Mean value
   * \param s Shape parameter
   * \param b Upper limit on returned values
   */
  ParetoVariableImpl(double m, double s, double b);

  ParetoVariableImpl(const ParetoVariableImpl& c);
  
  /**
   * \return A random value from this Pareto distribution
   */
  virtual double GetValue();
  virtual RandomVariableBase* Copy() const;
public:
  /**
   * \param m The mean value of the distribution from which the return value
   * is drawn.
   * \param s The shape parameter of the distribution from which the return
   * value is drawn.
   * \param b The upper bound to which to restrict return values
   * \return A random number from a Pareto distribution with mean m and shape
   * parameter s.
   */
  static double GetSingleValue(double m, double s, double b=0);
private:
  double m_mean;  // Mean value of RV
  double m_shape; // Shape parameter
  double m_bound; // Upper bound on value (if non-zero)
};

ParetoVariableImpl::ParetoVariableImpl() 
  : m_mean(1.0), m_shape(1.5), m_bound(0) { }

ParetoVariableImpl::ParetoVariableImpl(double m) 
  : m_mean(m), m_shape(1.5), m_bound(0) { }

ParetoVariableImpl::ParetoVariableImpl(double m, double s) 
    : m_mean(m), m_shape(s), m_bound(0) { }

ParetoVariableImpl::ParetoVariableImpl(double m, double s, double b) 
  : m_mean(m), m_shape(s), m_bound(b) { }

ParetoVariableImpl::ParetoVariableImpl(const ParetoVariableImpl& c) 
  : RandomVariableBase(c), m_mean(c.m_mean), m_shape(c.m_shape), 
    m_bound(c.m_bound) { }

double ParetoVariableImpl::GetValue()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
  }
  double scale = m_mean * ( m_shape - 1.0) / m_shape;
  double r = (scale * ( 1.0 / pow(m_generator->RandU01(), 1.0 / m_shape)));
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariableBase* ParetoVariableImpl::Copy() const
{
  return new ParetoVariableImpl(*this);
}

double ParetoVariableImpl::GetSingleValue(double m, double s, double b/*=0*/)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
  }
  double scale = m * ( s - 1.0) / s;
  double r = (scale * ( 1.0 / pow(m_static_generator->RandU01(), 1.0 / s)));
  if (b != 0 && r > b) return b;
  return r;
}

ParetoVariable::ParetoVariable ()
  : RandomVariable (ParetoVariableImpl ())
{}
ParetoVariable::ParetoVariable(double m)
  : RandomVariable (ParetoVariableImpl (m))
{}
ParetoVariable::ParetoVariable(double m, double s)
  : RandomVariable (ParetoVariableImpl (m, s))
{}
ParetoVariable::ParetoVariable(double m, double s, double b)
  : RandomVariable (ParetoVariableImpl (m, s, b))
{}
double 
ParetoVariable::GetSingleValue(double m, double s, double b)
{
  return ParetoVariableImpl::GetSingleValue (m, s, b);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// WeibullVariableImpl methods

class WeibullVariableImpl : public RandomVariableBase {
public:
  /**
   * Constructs a weibull random variable  with a mean
   * value of 1.0 and a shape (alpha) parameter of 1
   */
  WeibullVariableImpl();


  /**
   * Constructs a weibull random variable with the specified mean
   * value and a shape (alpha) parameter of 1.5.
   * \param m mean value of the distribution
   */
   WeibullVariableImpl(double m) ;

  /**
   * Constructs a weibull random variable with the specified mean
   * value and a shape (alpha).
   * \param m Mean value for the distribution.
   * \param s Shape (alpha) parameter for the distribution.
   */
  WeibullVariableImpl(double m, double s);

   /**
   * \brief Constructs a weibull random variable with the specified mean
   * \brief value, shape (alpha), and upper bound.
   * Since WeibullVariableImpl distributions can theoretically return unbounded values,
   * it is sometimes usefull to specify a fixed upper limit.  Note however
   * that when the upper limit is specified, the true mean of the distribution
   * is slightly smaller than the mean value specified.
   * \param m Mean value for the distribution.
   * \param s Shape (alpha) parameter for the distribution.
   * \param b Upper limit on returned values
   */
  WeibullVariableImpl(double m, double s, double b);

  WeibullVariableImpl(const WeibullVariableImpl& c);
  
  /**
   * \return A random value from this Weibull distribution
   */
  virtual double GetValue();
  virtual RandomVariableBase* Copy(void) const;
public:
  /**
   * \param m Mean value for the distribution.
   * \param s Shape (alpha) parameter for the distribution.
   * \param b Upper limit on returned values
   * \return Random number from a distribution specified by m,s, and b
   */
  static double GetSingleValue(double m, double s, double b=0);
private:
  double m_mean;  // Mean value of RV
  double m_alpha; // Shape parameter
  double m_bound; // Upper bound on value (if non-zero)
};

WeibullVariableImpl::WeibullVariableImpl() : m_mean(1.0), m_alpha(1), m_bound(0) { }
WeibullVariableImpl::WeibullVariableImpl(double m) 
  : m_mean(m), m_alpha(1), m_bound(0) { }
WeibullVariableImpl::WeibullVariableImpl(double m, double s) 
  : m_mean(m), m_alpha(s), m_bound(0) { }
WeibullVariableImpl::WeibullVariableImpl(double m, double s, double b) 
  : m_mean(m), m_alpha(s), m_bound(b) { };
WeibullVariableImpl::WeibullVariableImpl(const WeibullVariableImpl& c) 
  : RandomVariableBase(c), m_mean(c.m_mean), m_alpha(c.m_alpha),
    m_bound(c.m_bound) { }

double WeibullVariableImpl::GetValue()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
  }
  double exponent = 1.0 / m_alpha;
  double r = m_mean * pow( -log(m_generator->RandU01()), exponent);
  if (m_bound != 0 && r > m_bound) return m_bound;
  return r;
}

RandomVariableBase* WeibullVariableImpl::Copy() const
{
  return new WeibullVariableImpl(*this);
}

double WeibullVariableImpl::GetSingleValue(double m, double s, double b/*=0*/)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
  }
  double exponent = 1.0 / s;
  double r = m * pow( -log(m_static_generator->RandU01()), exponent);
  if (b != 0 && r > b) return b;
  return r;
}

WeibullVariable::WeibullVariable()
  : RandomVariable (WeibullVariableImpl ())
{}
WeibullVariable::WeibullVariable(double m)
  : RandomVariable (WeibullVariableImpl (m))
{}
WeibullVariable::WeibullVariable(double m, double s)
  : RandomVariable (WeibullVariableImpl (m, s))
{}
WeibullVariable::WeibullVariable(double m, double s, double b)
  : RandomVariable (WeibullVariableImpl (m, s, b))
{}
double 
WeibullVariable::GetSingleValue(double m, double s, double b)
{
  return WeibullVariableImpl::GetSingleValue (m, s, b);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// NormalVariableImpl methods

class NormalVariableImpl : public RandomVariableBase { // Normally Distributed random var

public:
   static const double INFINITE_VALUE;
  /**
   * Constructs an normal random variable  with a mean
   * value of 0 and variance of 1.
   */ 
  NormalVariableImpl();

  /**
   * \brief Construct a normal random variable with specified mean and variance
   * \param m Mean value
   * \param v Variance
   * \param b Bound.  The NormalVariableImpl is bounded within +-bound.
   */ 
  NormalVariableImpl(double m, double v, double b = INFINITE_VALUE);

  NormalVariableImpl(const NormalVariableImpl& c);
  
  /**
   * \return A value from this normal distribution
   */
  virtual double GetValue();
  virtual RandomVariableBase* Copy(void) const;
public:
  /**
   * \param m Mean value
   * \param v Variance
   * \param b Bound.  The NormalVariableImpl is bounded within +-bound.
   * \return A random number from a distribution specified by m,v, and b.
   */ 
  static double GetSingleValue(double m, double v, double b = INFINITE_VALUE);
private:
  double m_mean;      // Mean value of RV
  double m_variance;  // Mean value of RV
  double m_bound;     // Bound on value (absolute value)
  bool   m_nextValid; // True if next valid
  double m_next;      // The algorithm produces two values at a time
  static bool   m_static_nextValid;
  static double m_static_next;
};

bool         NormalVariableImpl::m_static_nextValid = false;
double       NormalVariableImpl::m_static_next;
const double NormalVariableImpl::INFINITE_VALUE = 1e307;

NormalVariableImpl::NormalVariableImpl() 
  : m_mean(0.0), m_variance(1.0), m_bound(INFINITE_VALUE), m_nextValid(false){}

NormalVariableImpl::NormalVariableImpl(double m, double v, double b/*=INFINITE_VALUE*/)
  : m_mean(m), m_variance(v), m_bound(b), m_nextValid(false) { }

NormalVariableImpl::NormalVariableImpl(const NormalVariableImpl& c)
  : RandomVariableBase(c), m_mean(c.m_mean), m_variance(c.m_variance),
    m_bound(c.m_bound) { }

double NormalVariableImpl::GetValue()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
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

RandomVariableBase* NormalVariableImpl::Copy() const
{
  return new NormalVariableImpl(*this);
}

double NormalVariableImpl::GetSingleValue(double m, double v, double b)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
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

NormalVariable::NormalVariable()
  : RandomVariable (NormalVariableImpl ())
{}
NormalVariable::NormalVariable(double m, double v, double b)
  : RandomVariable (NormalVariableImpl (m, v, b))
{}
double 
NormalVariable::GetSingleValue(double m, double v, double b)
{
  return NormalVariableImpl::GetSingleValue (m, v, b);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class EmpiricalVariableImpl : public RandomVariableBase {
public:
  /**
   * Constructor for the EmpiricalVariableImpl random variables.
   */
  explicit EmpiricalVariableImpl();

  virtual ~EmpiricalVariableImpl();
  EmpiricalVariableImpl(const EmpiricalVariableImpl& c);
  /**
   * \return A value from this empirical distribution
   */
  virtual double GetValue();
  virtual RandomVariableBase* Copy(void) const;
  /**
   * \brief Specifies a point in the empirical distribution
   * \param v The function value for this point
   * \param c Probability that the function is less than or equal to v
   */
  virtual void CDF(double v, double c);  // Value, prob <= Value

private:
  class ValueCDF {
  public:
    ValueCDF();
    ValueCDF(double v, double c);
    ValueCDF(const ValueCDF& c);
    double value;
    double    cdf;
  };
  virtual void Validate();  // Insure non-decreasing emiprical values
  virtual double Interpolate(double, double, double, double, double);
  bool validated; // True if non-decreasing validated
  std::vector<ValueCDF> emp;       // Empicical CDF
};


// ValueCDF methods
EmpiricalVariableImpl::ValueCDF::ValueCDF() 
  : value(0.0), cdf(0.0){ }
EmpiricalVariableImpl::ValueCDF::ValueCDF(double v, double c) 
  : value(v), cdf(c) { }
EmpiricalVariableImpl::ValueCDF::ValueCDF(const ValueCDF& c) 
  : value(c.value), cdf(c.cdf) { }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// EmpiricalVariableImpl methods
EmpiricalVariableImpl::EmpiricalVariableImpl() 
  : validated(false) { }

EmpiricalVariableImpl::EmpiricalVariableImpl(const EmpiricalVariableImpl& c)
  : RandomVariableBase(c), validated(c.validated), emp(c.emp) { }

EmpiricalVariableImpl::~EmpiricalVariableImpl() { }

double EmpiricalVariableImpl::GetValue()
{ // Return a value from the empirical distribution
  // This code based (loosely) on code by Bruce Mah (Thanks Bruce!)
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
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

RandomVariableBase* EmpiricalVariableImpl::Copy() const
{
  return new EmpiricalVariableImpl(*this);
}

void EmpiricalVariableImpl::CDF(double v, double c)
{ // Add a new empirical datapoint to the empirical cdf
  // NOTE.   These MUST be inserted in non-decreasing order
  emp.push_back(ValueCDF(v, c));
}

void EmpiricalVariableImpl::Validate()
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

double EmpiricalVariableImpl::Interpolate(double c1, double c2,
                                double v1, double v2, double r)
{ // Interpolate random value in range [v1..v2) based on [c1 .. r .. c2)
  return (v1 + ((v2 - v1) / (c2 - c1)) * (r - c1));
}

EmpiricalVariable::EmpiricalVariable()
  : RandomVariable (EmpiricalVariableImpl ())
{}
EmpiricalVariable::EmpiricalVariable (const RandomVariableBase &variable)
  : RandomVariable (variable)
{}
void 
EmpiricalVariable::CDF(double v, double c)
{
  EmpiricalVariableImpl *impl = dynamic_cast<EmpiricalVariableImpl *> (Peek ());
  NS_ASSERT (impl);
  impl->CDF (v, c);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Integer EmpiricalVariableImpl methods
class IntEmpiricalVariableImpl : public EmpiricalVariableImpl {
public:

  IntEmpiricalVariableImpl();
  
  virtual RandomVariableBase* Copy(void) const;
  /**
   * \return An integer value from this empirical distribution
   */
  virtual uint32_t GetIntValue();
private:
  virtual double Interpolate(double, double, double, double, double);
};


IntEmpiricalVariableImpl::IntEmpiricalVariableImpl() { }

uint32_t IntEmpiricalVariableImpl::GetIntValue()
{
  return (uint32_t)GetValue();
}

RandomVariableBase* IntEmpiricalVariableImpl::Copy() const
{
  return new IntEmpiricalVariableImpl(*this);
}

double IntEmpiricalVariableImpl::Interpolate(double c1, double c2,
                                   double v1, double v2, double r)
{ // Interpolate random value in range [v1..v2) based on [c1 .. r .. c2)
  return ceil(v1 + ((v2 - v1) / (c2 - c1)) * (r - c1));
}

IntEmpiricalVariable::IntEmpiricalVariable()
  : EmpiricalVariable (IntEmpiricalVariableImpl ())
{}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// DeterministicVariableImpl
class DeterministicVariableImpl : public RandomVariableBase 
{

public:
  /**
   * \brief Constructor
   *
   * Creates a generator that returns successive elements of the d array
   * on successive calls to ::Value().  Note that the d pointer is copied
   * for use by the generator (shallow-copy), not its contents, so the 
   * contents of the array d points to have to remain unchanged for the use 
   * of DeterministicVariableImpl to be meaningful.
   * \param d Pointer to array of random values to return in sequence
   * \param c Number of values in the array
   */
  explicit DeterministicVariableImpl(double* d, uint32_t c);

  virtual ~DeterministicVariableImpl();
  /**
   * \return The next value in the deterministic sequence
   */
  virtual double GetValue();
  virtual RandomVariableBase* Copy(void) const;
private:
  uint32_t   count;
  uint32_t   next;
  double* data;
};

DeterministicVariableImpl::DeterministicVariableImpl(double* d, uint32_t c)
    : count(c), next(c), data(d)
{ // Nothing else needed
}

DeterministicVariableImpl::~DeterministicVariableImpl() { }
  
double DeterministicVariableImpl::GetValue()
{
  if (next == count) next = 0;
  return data[next++];
}

RandomVariableBase* DeterministicVariableImpl::Copy() const
{
  return new DeterministicVariableImpl(*this);
}

DeterministicVariable::DeterministicVariable(double* d, uint32_t c)
  : RandomVariable (DeterministicVariableImpl (d, c))
{}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// LogNormalVariableImpl
class LogNormalVariableImpl : public RandomVariableBase { 
public:
  /**
   * \param mu mu parameter of the lognormal distribution
   * \param sigma sigma parameter of the lognormal distribution
   */
  LogNormalVariableImpl (double mu, double sigma);

  /**
   * \return A random value from this distribution
   */
  virtual double GetValue ();
  virtual RandomVariableBase* Copy(void) const;
public:
  /**
   * \param mu mu parameter of the underlying normal distribution
   * \param sigma sigma parameter of the underlying normal distribution
   * \return A random number from the distribution specified by mu and sigma
   */
  static double GetSingleValue(double mu, double sigma);
private:
  double m_mu;
  double m_sigma;
};


RandomVariableBase* LogNormalVariableImpl::Copy () const
{
  return new LogNormalVariableImpl (m_mu, m_sigma);
}

LogNormalVariableImpl::LogNormalVariableImpl (double mu, double sigma)
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
LogNormalVariableImpl::GetValue ()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
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

double LogNormalVariableImpl::GetSingleValue (double mu, double sigma)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
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

LogNormalVariable::LogNormalVariable (double mu, double sigma)
  : RandomVariable (LogNormalVariableImpl (mu, sigma))
{}
double 
LogNormalVariable::GetSingleValue(double mu, double sigma)
{
  return LogNormalVariableImpl::GetSingleValue (mu, sigma);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// TriangularVariableImpl methods
class TriangularVariableImpl : public RandomVariableBase {
public:
  /**
   * Creates a triangle distribution random number generator in the
   * range [0.0 .. 1.0), with mean of 0.5
   */
  TriangularVariableImpl();

  /**
   * Creates a triangle distribution random number generator with the specified
   * range
   * \param s Low end of the range
   * \param l High end of the range
   * \param mean mean of the distribution
   */
  TriangularVariableImpl(double s, double l, double mean);

  TriangularVariableImpl(const TriangularVariableImpl& c);
  
  /**
   * \return A value from this distribution
   */
  virtual double GetValue();
  virtual RandomVariableBase*  Copy(void) const;
public:
  /**
   * \param s Low end of the range
   * \param l High end of the range
   * \param mean mean of the distribution
   * \return A triangularly distributed random number between s and l
   */
  static double GetSingleValue(double s, double l, double mean);
private:
  double m_min;
  double m_max;
  double m_mode;  //easier to work with the mode internally instead of the mean
                  //they are related by the simple: mean = (min+max+mode)/3
};

TriangularVariableImpl::TriangularVariableImpl() 
  : m_min(0), m_max(1), m_mode(0.5) { }
  
TriangularVariableImpl::TriangularVariableImpl(double s, double l, double mean) 
  : m_min(s), m_max(l), m_mode(3.0*mean-s-l) { }
  
TriangularVariableImpl::TriangularVariableImpl(const TriangularVariableImpl& c) 
  : RandomVariableBase(c), m_min(c.m_min), m_max(c.m_max), m_mode(c.m_mode) { }

double TriangularVariableImpl::GetValue()
{
  if(!RandomVariableBase::initialized)
  {
    RandomVariableBase::Initialize();
  }
  if(!m_generator)
  {
    m_generator = new RngStream();
    m_generator->InitializeStream();
    m_generator->ResetNthSubstream(RandomVariableBase::runNumber);
  }
  double u = m_generator->RandU01();
  if(u <= (m_mode - m_min) / (m_max - m_min) )
    return m_min + sqrt(u * (m_max - m_min) * (m_mode - m_min) );
  else
    return m_max - sqrt( (1-u) * (m_max - m_min) * (m_max - m_mode) );
}

RandomVariableBase* TriangularVariableImpl::Copy() const
{
  return new TriangularVariableImpl(*this);
}

double TriangularVariableImpl::GetSingleValue(double s, double l, double mean)
{
  if(!RandomVariableBase::m_static_generator)
  {
    RandomVariableBase::Initialize(); // sets the static package seed
    RandomVariableBase::m_static_generator = new RngStream();
    RandomVariableBase::m_static_generator->InitializeStream();
  }
  double mode = 3.0*mean-s-l;
  double u = m_static_generator->RandU01();
  if(u <= (mode - s) / (l - s) )
    return s + sqrt(u * (l - s) * (mode - s) );
  else
    return l - sqrt( (1-u) * (l - s) * (l - mode) );
}

TriangularVariable::TriangularVariable()
  : RandomVariable (TriangularVariableImpl ())
{}
TriangularVariable::TriangularVariable(double s, double l, double mean)
  : RandomVariable (TriangularVariableImpl (s,l,mean))
{}
double 
TriangularVariable::GetSingleValue(double s, double l, double mean)
{
  return TriangularVariableImpl::GetSingleValue (s,l,mean);
}


std::ostream &operator << (std::ostream &os, const RandomVariable &var)
{
  RandomVariableBase *base = var.Peek ();
  ConstantVariableImpl *constant = dynamic_cast<ConstantVariableImpl *> (base);
  if (constant != 0)
    {
      os << "Constant:" << constant->GetValue ();
      return os;
    }
  UniformVariableImpl *uniform = dynamic_cast<UniformVariableImpl *> (base);
  if (uniform != 0)
    {
      os << "Uniform:" << uniform->GetMin () << ":" << uniform->GetMax ();
      return os;
    }
  // XXX: support other distributions
  os.setstate (std::ios_base::badbit);
  return os;
}
std::istream &operator >> (std::istream &is, RandomVariable &var)
{
  std::string value;
  is >> value;
  std::string::size_type tmp;
  tmp = value.find (":");
  if (tmp == std::string::npos)
    {
      is.setstate (std::ios_base::badbit);
      return is;
    }
  std::string type = value.substr (0, tmp);
  if (value == "Constant")
    {
      // XXX parse
      var = ConstantVariable ();
    }
  else if (value == "Uniform")
    {
      // XXX parse
      var = UniformVariable ();
    }
  else
    {
      // XXX: support other distributions.
    }
  return is;
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
