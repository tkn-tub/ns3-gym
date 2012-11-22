/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
 * Copyright (c) 2011 Mathieu Lacage
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
 * Authors: Rajib Bhattacharjea<raj.b@gatech.edu>
 *          Hadi Arbabi<marbabi@cs.odu.edu>
 *          Mathieu Lacage <mathieu.lacage@gmail.com>
 *
 * Modified by Mitch Watrous <watrous@u.washington.edu>
 *
 */
#include "random-variable-stream.h"
#include "assert.h"
#include "boolean.h"
#include "double.h"
#include "integer.h"
#include "string.h"
#include "pointer.h"
#include "log.h"
#include "rng-stream.h"
#include "rng-seed-manager.h"
#include <cmath>
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("RandomVariableStream");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RandomVariableStream);

TypeId 
RandomVariableStream::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RandomVariableStream")
    .SetParent<Object> ()
    .AddAttribute("Stream",
		  "The stream number for this RNG stream. -1 means \"allocate a stream automatically\". "
		  "Note that if -1 is set, Get will return -1 so that it is not possible to know which "
		  "value was automatically allocated.",
		  IntegerValue(-1),
		  MakeIntegerAccessor(&RandomVariableStream::SetStream,
				      &RandomVariableStream::GetStream),
		  MakeIntegerChecker<int64_t>())
    .AddAttribute("Antithetic", "Set this RNG stream to generate antithetic values",
		  BooleanValue (false),
		  MakeBooleanAccessor(&RandomVariableStream::SetAntithetic,
				      &RandomVariableStream::IsAntithetic),
		  MakeBooleanChecker())
    ;
  return tid;
}

RandomVariableStream::RandomVariableStream()
  : m_rng (0)
{}
RandomVariableStream::~RandomVariableStream()
{
  delete m_rng;
}

void
RandomVariableStream::SetAntithetic(bool isAntithetic)
{
  m_isAntithetic = isAntithetic;
}
bool
RandomVariableStream::IsAntithetic(void) const
{
  return m_isAntithetic;
}
void
RandomVariableStream::SetStream (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  // negative values are not legal.
  NS_ASSERT (stream >= -1);
  delete m_rng;
  if (stream == -1)
    {
      // The first 2^63 streams are reserved for automatic stream
      // number assignment.
      uint64_t nextStream = RngSeedManager::GetNextStreamIndex ();
      NS_ASSERT(nextStream <= ((1ULL)<<63));
      m_rng = new RngStream (RngSeedManager::GetSeed (),
                             nextStream,
                             RngSeedManager::GetRun ());
    }
  else
    {
      // The last 2^63 streams are reserved for deterministic stream
      // number assignment.
      uint64_t base = ((1ULL)<<63);
      uint64_t target = base + stream;
      m_rng = new RngStream (RngSeedManager::GetSeed (),
                             target,
                             RngSeedManager::GetRun ());
    }
  m_stream = stream;
}
int64_t
RandomVariableStream::GetStream(void) const
{
  return m_stream;
}

RngStream *
RandomVariableStream::Peek(void) const
{
  return m_rng;
}

NS_OBJECT_ENSURE_REGISTERED(UniformRandomVariable);

TypeId 
UniformRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UniformRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<UniformRandomVariable> ()
    .AddAttribute("Min", "The lower bound on the values returned by this RNG stream.",
		  DoubleValue(0),
		  MakeDoubleAccessor(&UniformRandomVariable::m_min),
		  MakeDoubleChecker<double>())
    .AddAttribute("Max", "The upper bound on the values returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&UniformRandomVariable::m_max),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
UniformRandomVariable::UniformRandomVariable ()
{
  // m_min and m_max are initialized after constructor by attributes
}

double 
UniformRandomVariable::GetMin (void) const
{
  return m_min;
}
double 
UniformRandomVariable::GetMax (void) const
{
  return m_max;
}

double 
UniformRandomVariable::GetValue (double min, double max)
{
  double v = min + Peek ()->RandU01 () * (max - min);
  if (IsAntithetic ())
    {
      v = min + (max - v);
    }
  return v;
}
uint32_t 
UniformRandomVariable::GetInteger (uint32_t min, uint32_t max)
{
  NS_ASSERT (min <= max);
  return static_cast<uint32_t> ( GetValue (min, max + 1) );
}

double 
UniformRandomVariable::GetValue (void)
{
  return GetValue (m_min, m_max);
}
uint32_t 
UniformRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_min, m_max + 1);
}

NS_OBJECT_ENSURE_REGISTERED(ConstantRandomVariable);

TypeId 
ConstantRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConstantRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<ConstantRandomVariable> ()
    .AddAttribute("Constant", "The constant value returned by this RNG stream.",
		  DoubleValue(0),
		  MakeDoubleAccessor(&ConstantRandomVariable::m_constant),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
ConstantRandomVariable::ConstantRandomVariable ()
{
  // m_constant is initialized after constructor by attributes
}

double 
ConstantRandomVariable::GetConstant (void) const
{
  return m_constant;
}

double 
ConstantRandomVariable::GetValue (double constant)
{
  return constant;
}
uint32_t 
ConstantRandomVariable::GetInteger (uint32_t constant)
{
  return constant;
}

double 
ConstantRandomVariable::GetValue (void)
{
  return GetValue (m_constant);
}
uint32_t 
ConstantRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_constant);
}

NS_OBJECT_ENSURE_REGISTERED(SequentialRandomVariable);

TypeId 
SequentialRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SequentialRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<SequentialRandomVariable> ()
    .AddAttribute("Min", "The first value of the sequence.",
		  DoubleValue(0),
		  MakeDoubleAccessor(&SequentialRandomVariable::m_min),
		  MakeDoubleChecker<double>())
    .AddAttribute("Max", "One more than the last value of the sequence.",
		  DoubleValue(0),
		  MakeDoubleAccessor(&SequentialRandomVariable::m_max),
		  MakeDoubleChecker<double>())
    .AddAttribute("Increment", "The sequence random variable increment.",
                  StringValue("ns3::ConstantRandomVariable[Constant=1]"),
                  MakePointerAccessor (&SequentialRandomVariable::m_increment),
                  MakePointerChecker<RandomVariableStream> ())
    .AddAttribute("Consecutive", "The number of times each member of the sequence is repeated.",
                  IntegerValue(1),
                  MakeIntegerAccessor(&SequentialRandomVariable::m_consecutive),
                  MakeIntegerChecker<uint32_t>());
     ;
  return tid;
}
SequentialRandomVariable::SequentialRandomVariable ()
    :
    m_current            (0),
    m_currentConsecutive (0),
    m_isCurrentSet       (false)
{
  // m_min, m_max, m_increment, and m_consecutive are initialized
  // after constructor by attributes.
}

double 
SequentialRandomVariable::GetMin (void) const
{
  return m_min;
}

double 
SequentialRandomVariable::GetMax (void) const
{
  return m_max;
}

Ptr<RandomVariableStream> 
SequentialRandomVariable::GetIncrement (void) const
{
  return m_increment;
}

uint32_t 
SequentialRandomVariable::GetConsecutive (void) const
{
  return m_consecutive;
}

double 
SequentialRandomVariable::GetValue (void)
{
  // Set the current sequence value if it hasn't been set.
  if (!m_isCurrentSet)
    {
      // Start the sequence at its minimium value.
      m_current = m_min;
      m_isCurrentSet = true;
    }

 // Return a sequential series of values
  double r = m_current;
  if (++m_currentConsecutive == m_consecutive)
    { // Time to advance to next
      m_currentConsecutive = 0;
      m_current += m_increment->GetValue ();
      if (m_current >= m_max)
        {
          m_current = m_min + (m_current - m_max);
        }
    }
  return r;
}

uint32_t 
SequentialRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue ();
}

NS_OBJECT_ENSURE_REGISTERED(ExponentialRandomVariable);

TypeId 
ExponentialRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ExponentialRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<ExponentialRandomVariable> ()
    .AddAttribute("Mean", "The mean of the values returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&ExponentialRandomVariable::m_mean),
		  MakeDoubleChecker<double>())
    .AddAttribute("Bound", "The upper bound on the values returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&ExponentialRandomVariable::m_bound),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
ExponentialRandomVariable::ExponentialRandomVariable ()
{
  // m_mean and m_bound are initialized after constructor by attributes
}

double 
ExponentialRandomVariable::GetMean (void) const
{
  return m_mean;
}
double 
ExponentialRandomVariable::GetBound (void) const
{
  return m_bound;
}

double 
ExponentialRandomVariable::GetValue (double mean, double bound)
{
  while (1)
    {
      // Get a uniform random variable in [0,1].
      double v = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          v = (1 - v);
        }

      // Calculate the exponential random variable.
      double r = -mean*std::log (v);

      // Use this value if it's acceptable.
      if (bound == 0 || r <= bound)
        {
          return r;
        }
    }
}
uint32_t 
ExponentialRandomVariable::GetInteger (uint32_t mean, uint32_t bound)
{
  return static_cast<uint32_t> ( GetValue (mean, bound) );
}

double 
ExponentialRandomVariable::GetValue (void)
{
  return GetValue (m_mean, m_bound);
}
uint32_t 
ExponentialRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_mean, m_bound);
}

NS_OBJECT_ENSURE_REGISTERED(ParetoRandomVariable);

TypeId 
ParetoRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ParetoRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<ParetoRandomVariable> ()
    .AddAttribute("Mean", "The mean parameter for the Pareto distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&ParetoRandomVariable::m_mean),
		  MakeDoubleChecker<double>())
    .AddAttribute("Shape", "The shape parameter for the Pareto distribution returned by this RNG stream.",
		  DoubleValue(2.0),
		  MakeDoubleAccessor(&ParetoRandomVariable::m_shape),
		  MakeDoubleChecker<double>())
    .AddAttribute("Bound", "The upper bound on the values returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&ParetoRandomVariable::m_bound),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
ParetoRandomVariable::ParetoRandomVariable ()
{
  // m_mean, m_shape, and m_bound are initialized after constructor
  // by attributes
}

double 
ParetoRandomVariable::GetMean (void) const
{
  return m_mean;
}
double 
ParetoRandomVariable::GetShape (void) const
{
  return m_shape;
}
double 
ParetoRandomVariable::GetBound (void) const
{
  return m_bound;
}

double 
ParetoRandomVariable::GetValue (double mean, double shape, double bound)
{
  // Calculate the scale parameter.
  double scale = mean * (shape - 1.0) / shape;

  while (1)
    {
      // Get a uniform random variable in [0,1].
      double v = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          v = (1 - v);
        }

      // Calculate the Pareto random variable.
      double r = (scale * ( 1.0 / std::pow (v, 1.0 / shape)));

      // Use this value if it's acceptable.
      if (bound == 0 || r <= bound)
        {
          return r;
        }
    }
}
uint32_t 
ParetoRandomVariable::GetInteger (uint32_t mean, uint32_t shape, uint32_t bound)
{
  return static_cast<uint32_t> ( GetValue (mean, shape, bound) );
}

double 
ParetoRandomVariable::GetValue (void)
{
  return GetValue (m_mean, m_shape, m_bound);
}
uint32_t 
ParetoRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_mean, m_shape, m_bound);
}

NS_OBJECT_ENSURE_REGISTERED(WeibullRandomVariable);

TypeId 
WeibullRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WeibullRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<WeibullRandomVariable> ()
    .AddAttribute("Scale", "The scale parameter for the Weibull distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&WeibullRandomVariable::m_scale),
		  MakeDoubleChecker<double>())
    .AddAttribute("Shape", "The shape parameter for the Weibull distribution returned by this RNG stream.",
		  DoubleValue(1),
		  MakeDoubleAccessor(&WeibullRandomVariable::m_shape),
		  MakeDoubleChecker<double>())
    .AddAttribute("Bound", "The upper bound on the values returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&WeibullRandomVariable::m_bound),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
WeibullRandomVariable::WeibullRandomVariable ()
{
  // m_scale, m_shape, and m_bound are initialized after constructor
  // by attributes
}

double 
WeibullRandomVariable::GetScale (void) const
{
  return m_scale;
}
double 
WeibullRandomVariable::GetShape (void) const
{
  return m_shape;
}
double 
WeibullRandomVariable::GetBound (void) const
{
  return m_bound;
}

double 
WeibullRandomVariable::GetValue (double scale, double shape, double bound)
{
  double exponent = 1.0 / shape;
  while (1)
    {
      // Get a uniform random variable in [0,1].
      double v = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          v = (1 - v);
        }

      // Calculate the Weibull random variable.
      double r = scale * std::pow ( -std::log (v), exponent);

      // Use this value if it's acceptable.
      if (bound == 0 || r <= bound)
        {
          return r;
        }
    }
}
uint32_t 
WeibullRandomVariable::GetInteger (uint32_t scale, uint32_t shape, uint32_t bound)
{
  return static_cast<uint32_t> ( GetValue (scale, shape, bound) );
}

double 
WeibullRandomVariable::GetValue (void)
{
  return GetValue (m_scale, m_shape, m_bound);
}
uint32_t 
WeibullRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_scale, m_shape, m_bound);
}

NS_OBJECT_ENSURE_REGISTERED(NormalRandomVariable);

const double NormalRandomVariable::INFINITE_VALUE = 1e307;

TypeId 
NormalRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NormalRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<NormalRandomVariable> ()
    .AddAttribute("Mean", "The mean value for the normal distribution returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&NormalRandomVariable::m_mean),
		  MakeDoubleChecker<double>())
    .AddAttribute("Variance", "The variance value for the normal distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&NormalRandomVariable::m_variance),
		  MakeDoubleChecker<double>())
    .AddAttribute("Bound", "The bound on the values returned by this RNG stream.",
		  DoubleValue(INFINITE_VALUE),
		  MakeDoubleAccessor(&NormalRandomVariable::m_bound),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
NormalRandomVariable::NormalRandomVariable ()
  :
  m_nextValid (false)
{
  // m_mean, m_variance, and m_bound are initialized after constructor
  // by attributes
}

double 
NormalRandomVariable::GetMean (void) const
{
  return m_mean;
}
double 
NormalRandomVariable::GetVariance (void) const
{
  return m_variance;
}
double 
NormalRandomVariable::GetBound (void) const
{
  return m_bound;
}

double 
NormalRandomVariable::GetValue (double mean, double variance, double bound)
{
  if (m_nextValid)
    { // use previously generated
      m_nextValid = false;
      return m_next;
    }
  while (1)
    { // See Simulation Modeling and Analysis p. 466 (Averill Law)
      // for algorithm; basically a Box-Muller transform:
      // http://en.wikipedia.org/wiki/Box-Muller_transform
      double u1 = Peek ()->RandU01 ();
      double u2 = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          u1 = (1 - u1);
          u2 = (1 - u2);
        }
      double v1 = 2 * u1 - 1;
      double v2 = 2 * u2 - 1;
      double w = v1 * v1 + v2 * v2;
      if (w <= 1.0)
        { // Got good pair
          double y = sqrt ((-2 * log (w)) / w);
          m_next = mean + v2 * y * sqrt (variance);
          // if next is in bounds, it is valid
          m_nextValid = fabs (m_next - mean) <= bound;
          double x1 = mean + v1 * y * sqrt (variance);
          // if x1 is in bounds, return it
          if (fabs (x1 - mean) <= bound)
            {
              return x1;
            }
          // otherwise try and return m_next if it is valid
          else if (m_nextValid)
            {
              m_nextValid = false;
              return m_next;
            }
          // otherwise, just run this loop again
        }
    }
}

uint32_t 
NormalRandomVariable::GetInteger (uint32_t mean, uint32_t variance, uint32_t bound)
{
  return static_cast<uint32_t> ( GetValue (mean, variance, bound) );
}

double 
NormalRandomVariable::GetValue (void)
{
  return GetValue (m_mean, m_variance, m_bound);
}
uint32_t 
NormalRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_mean, m_variance, m_bound);
}

NS_OBJECT_ENSURE_REGISTERED(LogNormalRandomVariable);

TypeId 
LogNormalRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LogNormalRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<LogNormalRandomVariable> ()
    .AddAttribute("Mu", "The mu value for the log-normal distribution returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&LogNormalRandomVariable::m_mu),
		  MakeDoubleChecker<double>())
    .AddAttribute("Sigma", "The sigma value for the log-normal distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&LogNormalRandomVariable::m_sigma),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
LogNormalRandomVariable::LogNormalRandomVariable ()
{
  // m_mu and m_sigma are initialized after constructor by
  // attributes
}

double 
LogNormalRandomVariable::GetMu (void) const
{
  return m_mu;
}
double 
LogNormalRandomVariable::GetSigma (void) const
{
  return m_sigma;
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
LogNormalRandomVariable::GetValue (double mu, double sigma)
{
  double v1, v2, r2, normal, x;

  do
    {
      /* choose x,y in uniform square (-1,-1) to (+1,+1) */

      double u1 = Peek ()->RandU01 ();
      double u2 = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          u1 = (1 - u1);
          u2 = (1 - u2);
        }

      v1 = -1 + 2 * u1;
      v2 = -1 + 2 * u2;

      /* see if it is in the unit circle */
      r2 = v1 * v1 + v2 * v2;
    }
  while (r2 > 1.0 || r2 == 0);

  normal = v1 * std::sqrt (-2.0 * std::log (r2) / r2);

  x =  std::exp (sigma * normal + mu);

  return x;
}

uint32_t 
LogNormalRandomVariable::GetInteger (uint32_t mu, uint32_t sigma)
{
  return static_cast<uint32_t> ( GetValue (mu, sigma));
}

double 
LogNormalRandomVariable::GetValue (void)
{
  return GetValue (m_mu, m_sigma);
}
uint32_t 
LogNormalRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_mu, m_sigma);
}

NS_OBJECT_ENSURE_REGISTERED(GammaRandomVariable);

TypeId 
GammaRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GammaRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<GammaRandomVariable> ()
    .AddAttribute("Alpha", "The alpha value for the gamma distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&GammaRandomVariable::m_alpha),
		  MakeDoubleChecker<double>())
    .AddAttribute("Beta", "The beta value for the gamma distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&GammaRandomVariable::m_beta),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
GammaRandomVariable::GammaRandomVariable ()
  :
  m_nextValid (false)
{
  // m_alpha and m_beta are initialized after constructor by
  // attributes
}

double 
GammaRandomVariable::GetAlpha (void) const
{
  return m_alpha;
}
double 
GammaRandomVariable::GetBeta (void) const
{
  return m_beta;
}

/*
  The code for the following generator functions was adapted from ns-2
  tools/ranvar.cc

  Originally the algorithm was devised by Marsaglia in 2000:
  G. Marsaglia, W. W. Tsang: A simple method for gereating Gamma variables
  ACM Transactions on mathematical software, Vol. 26, No. 3, Sept. 2000

  The Gamma distribution density function has the form

                             x^(alpha-1) * exp(-x/beta)
        p(x; alpha, beta) = ----------------------------
                             beta^alpha * Gamma(alpha)

  for x > 0.
*/
double 
GammaRandomVariable::GetValue (double alpha, double beta)
{
  if (alpha < 1)
    {
      double u = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          u = (1 - u);
        }
      return GetValue (1.0 + alpha, beta) * std::pow (u, 1.0 / alpha);
    }

  double x, v, u;
  double d = alpha - 1.0 / 3.0;
  double c = (1.0 / 3.0) / sqrt (d);

  while (1)
    {
      do
        {
          // Get a value from a normal distribution that has mean
          // zero, variance 1, and no bound.
          double mean = 0.0;
          double variance = 1.0;
          double bound = NormalRandomVariable::INFINITE_VALUE;
          x = GetNormalValue (mean, variance, bound);

          v = 1.0 + c * x;
        }
      while (v <= 0);

      v = v * v * v;
      u = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          u = (1 - u);
        }
      if (u < 1 - 0.0331 * x * x * x * x)
        {
          break;
        }
      if (log (u) < 0.5 * x * x + d * (1 - v + log (v)))
        {
          break;
        }
    }

  return beta * d * v;
}

uint32_t 
GammaRandomVariable::GetInteger (uint32_t alpha, uint32_t beta)
{
  return static_cast<uint32_t> ( GetValue (alpha, beta));
}

double 
GammaRandomVariable::GetValue (void)
{
  return GetValue (m_alpha, m_beta);
}
uint32_t 
GammaRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_alpha, m_beta);
}

double 
GammaRandomVariable::GetNormalValue (double mean, double variance, double bound)
{
  if (m_nextValid)
    { // use previously generated
      m_nextValid = false;
      return m_next;
    }
  while (1)
    { // See Simulation Modeling and Analysis p. 466 (Averill Law)
      // for algorithm; basically a Box-Muller transform:
      // http://en.wikipedia.org/wiki/Box-Muller_transform
      double u1 = Peek ()->RandU01 ();
      double u2 = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          u1 = (1 - u1);
          u2 = (1 - u2);
        }
      double v1 = 2 * u1 - 1;
      double v2 = 2 * u2 - 1;
      double w = v1 * v1 + v2 * v2;
      if (w <= 1.0)
        { // Got good pair
          double y = sqrt ((-2 * log (w)) / w);
          m_next = mean + v2 * y * sqrt (variance);
          // if next is in bounds, it is valid
          m_nextValid = fabs (m_next - mean) <= bound;
          double x1 = mean + v1 * y * sqrt (variance);
          // if x1 is in bounds, return it
          if (fabs (x1 - mean) <= bound)
            {
              return x1;
            }
          // otherwise try and return m_next if it is valid
          else if (m_nextValid)
            {
              m_nextValid = false;
              return m_next;
            }
          // otherwise, just run this loop again
        }
    }
}

NS_OBJECT_ENSURE_REGISTERED(ErlangRandomVariable);

TypeId 
ErlangRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErlangRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<ErlangRandomVariable> ()
    .AddAttribute("K", "The k value for the Erlang distribution returned by this RNG stream.",
		  IntegerValue(1),
		  MakeIntegerAccessor(&ErlangRandomVariable::m_k),
		  MakeIntegerChecker<uint32_t>())
    .AddAttribute("Lambda", "The lambda value for the Erlang distribution returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&ErlangRandomVariable::m_lambda),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
ErlangRandomVariable::ErlangRandomVariable ()
{
  // m_k and m_lambda are initialized after constructor by attributes
}

uint32_t 
ErlangRandomVariable::GetK (void) const
{
  return m_k;
}
double 
ErlangRandomVariable::GetLambda (void) const
{
  return m_lambda;
}

/*
  The code for the following generator functions was adapted from ns-2
  tools/ranvar.cc

  The Erlang distribution density function has the form

                           x^(k-1) * exp(-x/lambda)
        p(x; k, lambda) = ---------------------------
                             lambda^k * (k-1)!

  for x > 0.
*/
double 
ErlangRandomVariable::GetValue (uint32_t k, double lambda)
{
  double mean = lambda;
  double bound = 0.0;

  double result = 0;
  for (unsigned int i = 0; i < k; ++i)
    {
      result += GetExponentialValue (mean, bound);

    }

  return result;
}

uint32_t 
ErlangRandomVariable::GetInteger (uint32_t k, uint32_t lambda)
{
  return static_cast<uint32_t> ( GetValue (k, lambda));
}

double 
ErlangRandomVariable::GetValue (void)
{
  return GetValue (m_k, m_lambda);
}
uint32_t 
ErlangRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_k, m_lambda);
}

double 
ErlangRandomVariable::GetExponentialValue (double mean, double bound)
{
  while (1)
    {
      // Get a uniform random variable in [0,1].
      double v = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          v = (1 - v);
        }

      // Calculate the exponential random variable.
      double r = -mean*std::log (v);

      // Use this value if it's acceptable.
      if (bound == 0 || r <= bound)
        {
          return r;
        }
    }
}

NS_OBJECT_ENSURE_REGISTERED(TriangularRandomVariable);

TypeId 
TriangularRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TriangularRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<TriangularRandomVariable> ()
    .AddAttribute("Mean", "The mean value for the triangular distribution returned by this RNG stream.",
		  DoubleValue(0.5),
		  MakeDoubleAccessor(&TriangularRandomVariable::m_mean),
		  MakeDoubleChecker<double>())
    .AddAttribute("Min", "The lower bound on the values returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&TriangularRandomVariable::m_min),
		  MakeDoubleChecker<double>())
    .AddAttribute("Max", "The upper bound on the values returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&TriangularRandomVariable::m_max),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
TriangularRandomVariable::TriangularRandomVariable ()
  :
  m_mode (0.5)
{
  // m_mean, m_min, and m_max are initialized after constructor by
  // attributes
}

double 
TriangularRandomVariable::GetMean (void) const
{
  return m_mean;
}
double 
TriangularRandomVariable::GetMin (void) const
{
  return m_min;
}
double 
TriangularRandomVariable::GetMax (void) const
{
  return m_max;
}

double 
TriangularRandomVariable::GetValue (double mean, double min, double max)
{
  // Calculate the mode.
  double mode = 3.0 * mean - min - max;

  // Get a uniform random variable in [0,1].
  double u = Peek ()->RandU01 ();
  if (IsAntithetic ())
    {
      u = (1 - u);
    }

  // Calculate the triangular random variable.
  if (u <= (mode - min) / (max - min) )
    {
      return min + sqrt (u * (max - min) * (mode - min) );
    }
  else
    {
      return max - sqrt ( (1 - u) * (max - min) * (max - mode) );
    }
}

uint32_t 
TriangularRandomVariable::GetInteger (uint32_t mean, uint32_t min, uint32_t max)
{
  return static_cast<uint32_t> ( GetValue (mean, min, max) );
}

double 
TriangularRandomVariable::GetValue (void)
{
  return GetValue (m_mean, m_min, m_max);
}
uint32_t 
TriangularRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_mean, m_min, m_max);
}

NS_OBJECT_ENSURE_REGISTERED(ZipfRandomVariable);

TypeId 
ZipfRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ZipfRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<ZipfRandomVariable> ()
    .AddAttribute("N", "The n value for the Zipf distribution returned by this RNG stream.",
		  IntegerValue(1),
		  MakeIntegerAccessor(&ZipfRandomVariable::m_n),
		  MakeIntegerChecker<uint32_t>())
    .AddAttribute("Alpha", "The alpha value for the Zipf distribution returned by this RNG stream.",
		  DoubleValue(0.0),
		  MakeDoubleAccessor(&ZipfRandomVariable::m_alpha),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
ZipfRandomVariable::ZipfRandomVariable ()
{
  // m_n and m_alpha are initialized after constructor by attributes
}

uint32_t 
ZipfRandomVariable::GetN (void) const
{
  return m_n;
}
double 
ZipfRandomVariable::GetAlpha (void) const
{
  return m_alpha;
}

double 
ZipfRandomVariable::GetValue (uint32_t n, double alpha)
{
  // Calculate the normalization constant c.
  m_c = 0.0;
  for (uint32_t i = 1; i <= n; i++)
    {
      m_c += (1.0 / std::pow ((double)i,alpha));
    }
  m_c = 1.0 / m_c;

  // Get a uniform random variable in [0,1].
  double u = Peek ()->RandU01 ();
  if (IsAntithetic ())
    {
      u = (1 - u);
    }

  double sum_prob = 0,zipf_value = 0;
  for (uint32_t i = 1; i <= m_n; i++)
    {
      sum_prob += m_c / std::pow ((double)i,m_alpha);
      if (sum_prob > u)
        {
          zipf_value = i;
          break;
        }
    }
  return zipf_value;
}

uint32_t 
ZipfRandomVariable::GetInteger (uint32_t n, uint32_t alpha)
{
  return static_cast<uint32_t> ( GetValue (n, alpha));
}

double 
ZipfRandomVariable::GetValue (void)
{
  return GetValue (m_n, m_alpha);
}
uint32_t 
ZipfRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_n, m_alpha);
}

NS_OBJECT_ENSURE_REGISTERED(ZetaRandomVariable);

TypeId 
ZetaRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ZetaRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<ZetaRandomVariable> ()
    .AddAttribute("Alpha", "The alpha value for the zeta distribution returned by this RNG stream.",
		  DoubleValue(3.14),
		  MakeDoubleAccessor(&ZetaRandomVariable::m_alpha),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
ZetaRandomVariable::ZetaRandomVariable ()
{
  // m_alpha is initialized after constructor by attributes
}

double 
ZetaRandomVariable::GetAlpha (void) const
{
  return m_alpha;
}

double 
ZetaRandomVariable::GetValue (double alpha)
{
  m_b = std::pow (2.0, alpha - 1.0);

  double u, v;
  double X, T;
  double test;

  do
    {
      // Get a uniform random variable in [0,1].
      u = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          u = (1 - u);
        }

      // Get a uniform random variable in [0,1].
      v = Peek ()->RandU01 ();
      if (IsAntithetic ())
        {
          v = (1 - v);
        }

      X = std::floor (std::pow (u, -1.0 / (m_alpha - 1.0)));
      T = std::pow (1.0 + 1.0 / X, m_alpha - 1.0);
      test = v * X * (T - 1.0) / (m_b - 1.0);
    }
  while ( test > (T / m_b) );

  return X;
}

uint32_t 
ZetaRandomVariable::GetInteger (uint32_t alpha)
{
  return static_cast<uint32_t> ( GetValue (alpha));
}

double 
ZetaRandomVariable::GetValue (void)
{
  return GetValue (m_alpha);
}
uint32_t 
ZetaRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_alpha);
}

NS_OBJECT_ENSURE_REGISTERED(DeterministicRandomVariable);

TypeId 
DeterministicRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DeterministicRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<DeterministicRandomVariable> ()
    ;
  return tid;
}
DeterministicRandomVariable::DeterministicRandomVariable ()
  :
  m_count (0),
  m_next (0),
  m_data (0)
{
}
DeterministicRandomVariable::~DeterministicRandomVariable ()
{
  // Delete any values currently set.
  if (m_data != 0)
  {
    delete[] m_data;
  }
}

void
DeterministicRandomVariable::SetValueArray (double* values, uint64_t length)
{
  // Delete any values currently set.
  if (m_data != 0)
  {
    delete[] m_data;
  }

  // Make room for the values being set.
  m_data = new double[length];
  m_count = length;
  m_next = length;

  // Copy the values.
  for (uint64_t i = 0; i < m_count; i++)
    {
      m_data[i] = values[i];
    }
}

double 
DeterministicRandomVariable::GetValue (void)
{
  // Make sure the array has been set.
  NS_ASSERT (m_count > 0);

  if (m_next == m_count)
    {
      m_next = 0;
    }
  return m_data[m_next++];
}

uint32_t 
DeterministicRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue ();
}

NS_OBJECT_ENSURE_REGISTERED(EmpiricalRandomVariable);

// ValueCDF methods
EmpiricalRandomVariable::ValueCDF::ValueCDF ()
  : value (0.0),
    cdf (0.0)
{
}
EmpiricalRandomVariable::ValueCDF::ValueCDF (double v, double c)
  : value (v),
    cdf (c)
{
}
EmpiricalRandomVariable::ValueCDF::ValueCDF (const ValueCDF& c)
  : value (c.value),
    cdf (c.cdf)
{
}

TypeId 
EmpiricalRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EmpiricalRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<EmpiricalRandomVariable> ()
    ;
  return tid;
}
EmpiricalRandomVariable::EmpiricalRandomVariable ()
  :
  validated (false)
{
}

double 
EmpiricalRandomVariable::GetValue (void)
{
  // Return a value from the empirical distribution
  // This code based (loosely) on code by Bruce Mah (Thanks Bruce!)
  if (emp.size () == 0)
    {
      return 0.0; // HuH? No empirical data
    }
  if (!validated)
    {
      Validate ();      // Insure in non-decreasing
    }

  // Get a uniform random variable in [0,1].
  double r = Peek ()->RandU01 ();
  if (IsAntithetic ())
    {
      r = (1 - r);
    }

  if (r <= emp.front ().cdf)
    {
      return emp.front ().value; // Less than first
    }
  if (r >= emp.back ().cdf)
    {
      return emp.back ().value;  // Greater than last
    }
  // Binary search
  std::vector<ValueCDF>::size_type bottom = 0;
  std::vector<ValueCDF>::size_type top = emp.size () - 1;
  while (1)
    {
      std::vector<ValueCDF>::size_type c = (top + bottom) / 2;
      if (r >= emp[c].cdf && r < emp[c + 1].cdf)
        { // Found it
          return Interpolate (emp[c].cdf, emp[c + 1].cdf,
                              emp[c].value, emp[c + 1].value,
                              r);
        }
      // Not here, adjust bounds
      if (r < emp[c].cdf)
        {
          top    = c - 1;
        }
      else
        {
          bottom = c + 1;
        }
    }
}

uint32_t 
EmpiricalRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue ();
}

void EmpiricalRandomVariable::CDF (double v, double c)
{ // Add a new empirical datapoint to the empirical cdf
  // NOTE.   These MUST be inserted in non-decreasing order
  emp.push_back (ValueCDF (v, c));
}

void EmpiricalRandomVariable::Validate ()
{
  ValueCDF prior;
  for (std::vector<ValueCDF>::size_type i = 0; i < emp.size (); ++i)
    {
      ValueCDF& current = emp[i];
      if (current.value < prior.value || current.cdf < prior.cdf)
        { // Error
          std::cerr << "Empirical Dist error,"
               << " current value " << current.value
               << " prior value "   << prior.value
               << " current cdf "   << current.cdf
                    << " prior cdf "     << prior.cdf << std::endl;
          NS_FATAL_ERROR ("Empirical Dist error");
        }
      prior = current;
    }
  validated = true;
}

double EmpiricalRandomVariable::Interpolate (double c1, double c2,
                                           double v1, double v2, double r)
{ // Interpolate random value in range [v1..v2) based on [c1 .. r .. c2)
  return (v1 + ((v2 - v1) / (c2 - c1)) * (r - c1));
}

} // namespace ns3
