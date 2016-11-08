/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009-12 University of Washington
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
 * This file is based on rng-test-suite.cc.
 *
 * Modified by Mitch Watrous <watrous@u.washington.edu>
 *
 */


#include <gsl/gsl_cdf.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_sf_zeta.h>
#include <ctime>
#include <fstream>
#include <cmath>

#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/integer.h"
#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/random-variable-stream.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RandomVariableStreamGenerators");

namespace {

void
FillHistoRangeUniformly (double *array, uint32_t n, double start, double end)
{
  double increment = (end - start) / (n - 1.);
  double d = start;

  for (uint32_t i = 0; i < n; ++i)
    {
      array[i] = d;
      d += increment;
    }
}

bool seedSet = false;

// Over time, this test suite is designed to be run with varying seed
// values so that the distributions can be evaluated with chi-squared
// tests.  To enable this, normal invocation of this test suite will
// result in a seed value corresponding to the seconds since epoch
// (time (0) from ctime).  Note: this is not a recommended practice for 
// seeding normal simulations, as described in the ns-3 manual, but
// suits our purposes here.
//
// However, we also want to provide the ability to run this test suite
// with a repeatable value, such as when the seed or run number is configured
// to a specific value.  Therefore, we adopt the following policy.  When
// the test program is being run with the default global values for seed
// and run number, this function will instead pick a random, time-based
// seed for use within this test suite.  If the global values for seed or 
// run number have been configured differently from the default values,
// the global seed value will be used instead of the time-based one.
// 
// For example, this command will cause this test suite to use the 
// deterministic value of seed=3 every time:
//   NS_GLOBAL_VALUE="RngSeed=3" ./test.py -s random-variable-stream-generators
// or equivalently (to see log output):
//   NS_LOG="RandomVariableStreamGenerators" NS_GLOBAL_VALUE="RngSeed=3" ./waf --run "test-runner --suite=random-variable-stream-generators"
//  Similarly, if the value of RngRun is not set to 1, the globals will be
//  used.
//
void
SetTestSuiteSeed (void)
{
  if (seedSet == false)
    {
      uint32_t seed;
      if (RngSeedManager::GetSeed () == 1 && RngSeedManager::GetRun () == 1)
        {
          seed = static_cast<uint32_t> (time (0));
          seedSet = true;
          NS_LOG_DEBUG ("Global seed and run number are default; seeding with time of day: " << seed);
          
        }
      else
        {
          seed = RngSeedManager::GetSeed ();
          seedSet = true;
          NS_LOG_DEBUG ("Global seed and run number are not default; using the non-default values seed: " <<
                        seed << " and run: " << RngSeedManager::GetRun ());
        }
      SeedManager::SetSeed (seed);
    }
}

} // anonymous namespace

// ===========================================================================
// Test case for uniform distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamUniformTestCase : public TestCase
{
public:
  // We want the number of observations in each bin to be > 5
  // The following values should yield many more than 5 per bin
  static const uint32_t N_BINS = 100;
  static const uint32_t N_MEASUREMENTS = 1000000;

  // Number of times to wrap the Chi-Squared test and retry
  static const uint32_t N_RUNS = 2;

  RandomVariableStreamUniformTestCase ();
  virtual ~RandomVariableStreamUniformTestCase ();

  double ChiSquaredTest (Ptr<UniformRandomVariable> u);

private:
  virtual void DoRun (void);
};

RandomVariableStreamUniformTestCase::RandomVariableStreamUniformTestCase ()
  : TestCase ("Uniform Random Variable Stream Generator")
{
}

RandomVariableStreamUniformTestCase::~RandomVariableStreamUniformTestCase ()
{
}

double
RandomVariableStreamUniformTestCase::ChiSquaredTest (Ptr<UniformRandomVariable> u)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  // Note that this assumes that the range for u is [0,1], which is
  // the default range for this distribution.
  gsl_histogram_set_ranges_uniform (h, 0., 1.);

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, u->GetValue ());
    }

  double tmp[N_BINS];

  double expected = ((double)N_MEASUREMENTS / (double)N_BINS);

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected;
      tmp[i] *= tmp[i];
      tmp[i] /= expected;
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamUniformTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double confidence = 0.99;
  double maxStatistic = gsl_cdf_chisq_Pinv (confidence, (N_BINS-1));
  NS_LOG_DEBUG ("Chi square required at " << confidence << " confidence for " << N_BINS << " bins is " << maxStatistic);

  double result = maxStatistic;
  // If chi-squared test fails, re-try it up to N_RUNS times
  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<UniformRandomVariable> u = CreateObject<UniformRandomVariable> ();
      result = ChiSquaredTest (u);
      NS_LOG_DEBUG ("Chi square result is " << result);
      if (result < maxStatistic)
        {
          break;
        }
    }

  NS_TEST_ASSERT_MSG_LT (result, maxStatistic, "Chi-squared statistic out of range");

  double min = 0.0;
  double max = 10.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();

  x->SetAttribute ("Min", DoubleValue (min));
  x->SetAttribute ("Max", DoubleValue (max));
 
  // Test that values are always within the range:
  //
  //     [min, max)
  //
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      NS_TEST_ASSERT_MSG_EQ ((value >= min), true, "Value less than minimum.");
      NS_TEST_ASSERT_MSG_LT (value, max, "Value greater than or equal to maximum.");
    }

  // Boundary checking on GetInteger; should be [min,max]; from bug 1964
  static const uint32_t UNIFORM_INTEGER_MIN = 0;
  static const uint32_t UNIFORM_INTEGER_MAX = 4294967295U;
  // [0,0] should return 0
  uint32_t intValue;
  intValue = x->GetInteger (UNIFORM_INTEGER_MIN, UNIFORM_INTEGER_MIN);
  NS_TEST_ASSERT_MSG_EQ (intValue, UNIFORM_INTEGER_MIN, "Uniform RV GetInteger boundary testing");
  // [UNIFORM_INTEGER_MAX, UNIFORM_INTEGER_MAX] should return UNIFORM_INTEGER_MAX
  intValue = x->GetInteger (UNIFORM_INTEGER_MAX, UNIFORM_INTEGER_MAX);
  NS_TEST_ASSERT_MSG_EQ (intValue, UNIFORM_INTEGER_MAX, "Uniform RV GetInteger boundary testing");
  // [0,1] should return mix of 0 or 1
  intValue = 0;
  for (int i = 0; i < 20; i++)
    {
      intValue += x->GetInteger (UNIFORM_INTEGER_MIN, UNIFORM_INTEGER_MIN + 1);
    }
  NS_TEST_ASSERT_MSG_GT (intValue, 0, "Uniform RV GetInteger boundary testing");
  NS_TEST_ASSERT_MSG_LT (intValue, 20, "Uniform RV GetInteger boundary testing");
  // [MAX-1,MAX] should return mix of MAX-1 or MAX
  uint32_t count = 0;
  for (int i = 0; i < 20; i++)
    {
      intValue = x->GetInteger (UNIFORM_INTEGER_MAX - 1, UNIFORM_INTEGER_MAX);
      if (intValue == UNIFORM_INTEGER_MAX)
        {
          count++;
        }
    }
  NS_TEST_ASSERT_MSG_GT (count, 0, "Uniform RV GetInteger boundary testing");
  NS_TEST_ASSERT_MSG_LT (count, 20, "Uniform RV GetInteger boundary testing");
  // multiple [0,UNIFORM_INTEGER_MAX] should return non-zero
  intValue = x->GetInteger (UNIFORM_INTEGER_MIN, UNIFORM_INTEGER_MAX);
  uint32_t intValue2 = x->GetInteger (UNIFORM_INTEGER_MIN, UNIFORM_INTEGER_MAX);
  NS_TEST_ASSERT_MSG_GT (intValue + intValue2, 0, "Uniform RV GetInteger boundary testing");

}

// ===========================================================================
// Test case for antithetic uniform distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamUniformAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamUniformAntitheticTestCase ();
  virtual ~RandomVariableStreamUniformAntitheticTestCase ();

  double ChiSquaredTest (Ptr<UniformRandomVariable> u);

private:
  virtual void DoRun (void);
};

RandomVariableStreamUniformAntitheticTestCase::RandomVariableStreamUniformAntitheticTestCase ()
  : TestCase ("Antithetic Uniform Random Variable Stream Generator")
{
}

RandomVariableStreamUniformAntitheticTestCase::~RandomVariableStreamUniformAntitheticTestCase ()
{
}

double
RandomVariableStreamUniformAntitheticTestCase::ChiSquaredTest (Ptr<UniformRandomVariable> u)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  // Note that this assumes that the range for u is [0,1], which is
  // the default range for this distribution.
  gsl_histogram_set_ranges_uniform (h, 0., 1.);

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, u->GetValue ());
    }

  double tmp[N_BINS];

  double expected = ((double)N_MEASUREMENTS / (double)N_BINS);

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected;
      tmp[i] *= tmp[i];
      tmp[i] /= expected;
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamUniformAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<UniformRandomVariable> u = CreateObject<UniformRandomVariable> ();

      // Make this generate antithetic values.
      u->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (u);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double min = 0.0;
  double max = 10.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  x->SetAttribute ("Min", DoubleValue (min));
  x->SetAttribute ("Max", DoubleValue (max));
 
  // Test that values are always within the range:
  //
  //     [min, max)
  //
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      NS_TEST_ASSERT_MSG_EQ ((value >= min), true, "Value less than minimum.");
      NS_TEST_ASSERT_MSG_LT (value, max, "Value greater than or equal to maximum.");
    }


}

// ===========================================================================
// Test case for constant random variable stream generator
// ===========================================================================
class RandomVariableStreamConstantTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;
  static const double TOLERANCE;

  RandomVariableStreamConstantTestCase ();
  virtual ~RandomVariableStreamConstantTestCase ();

private:
  virtual void DoRun (void);
};

const double RandomVariableStreamConstantTestCase::TOLERANCE = 1e-8;

RandomVariableStreamConstantTestCase::RandomVariableStreamConstantTestCase ()
  : TestCase ("Constant Random Variable Stream Generator")
{
}

RandomVariableStreamConstantTestCase::~RandomVariableStreamConstantTestCase ()
{
}

void
RandomVariableStreamConstantTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  Ptr<ConstantRandomVariable> c = CreateObject<ConstantRandomVariable> ();

  double constant;

  // Test that the constant value can be changed using its attribute.
  constant = 10.0;
  c->SetAttribute ("Constant", DoubleValue (constant));
  NS_TEST_ASSERT_MSG_EQ_TOL (c->GetValue (), constant, TOLERANCE, "Constant value changed");
  c->SetAttribute ("Constant", DoubleValue (20.0));
  NS_TEST_ASSERT_MSG_NE (c->GetValue (), constant, "Constant value not changed");

  // Test that the constant value does not change.
  constant = c->GetValue ();
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL (c->GetValue (), constant, TOLERANCE, "Constant value changed in loop");
    }
}

// ===========================================================================
// Test case for sequential random variable stream generator
// ===========================================================================
class RandomVariableStreamSequentialTestCase : public TestCase
{
public:
  static const double TOLERANCE;

  RandomVariableStreamSequentialTestCase ();
  virtual ~RandomVariableStreamSequentialTestCase ();

private:
  virtual void DoRun (void);
};

const double RandomVariableStreamSequentialTestCase::TOLERANCE = 1e-8;

RandomVariableStreamSequentialTestCase::RandomVariableStreamSequentialTestCase ()
  : TestCase ("Sequential Random Variable Stream Generator")
{
}

RandomVariableStreamSequentialTestCase::~RandomVariableStreamSequentialTestCase ()
{
}

void
RandomVariableStreamSequentialTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  Ptr<SequentialRandomVariable> s = CreateObject<SequentialRandomVariable> ();

  // The following four attributes should give the sequence
  //
  //    4, 4, 7, 7, 10, 10
  //
  s->SetAttribute ("Min", DoubleValue (4));
  s->SetAttribute ("Max", DoubleValue (11));
  s->SetAttribute ("Increment", StringValue("ns3::UniformRandomVariable[Min=3.0|Max=3.0]"));
  s->SetAttribute ("Consecutive", IntegerValue (2));

  double value;

  // Test that the sequencet is correct.
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4, TOLERANCE, "Sequence value 1 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4, TOLERANCE, "Sequence value 2 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 7, TOLERANCE, "Sequence value 3 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 7, TOLERANCE, "Sequence value 4 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 10, TOLERANCE, "Sequence value 5 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 10, TOLERANCE, "Sequence value 6 wrong."); 

}

// ===========================================================================
// Test case for normal distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamNormalTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamNormalTestCase ();
  virtual ~RandomVariableStreamNormalTestCase ();

  double ChiSquaredTest (Ptr<NormalRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamNormalTestCase::RandomVariableStreamNormalTestCase ()
  : TestCase ("Normal Random Variable Stream Generator")
{
}

RandomVariableStreamNormalTestCase::~RandomVariableStreamNormalTestCase ()
{
}

double
RandomVariableStreamNormalTestCase::ChiSquaredTest (Ptr<NormalRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, -4., 4.);
  range[0] = -std::numeric_limits<double>::max ();
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has mean equal to zero and standard
  // deviation equal to one, which are their default values for this
  // distribution.
  double sigma = 1.;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_gaussian_P (range[i + 1], sigma) - gsl_cdf_gaussian_P (range[i], sigma);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamNormalTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<NormalRandomVariable> n = CreateObject<NormalRandomVariable> ();
      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double mean = 5.0;
  double variance = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<NormalRandomVariable> x = CreateObject<NormalRandomVariable> ();
  x->SetAttribute ("Mean", DoubleValue (mean));
  x->SetAttribute ("Variance", DoubleValue (variance));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // normally distributed random variable is equal to mean.
  double expectedMean = mean;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic normal distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamNormalAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamNormalAntitheticTestCase ();
  virtual ~RandomVariableStreamNormalAntitheticTestCase ();

  double ChiSquaredTest (Ptr<NormalRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamNormalAntitheticTestCase::RandomVariableStreamNormalAntitheticTestCase ()
  : TestCase ("Antithetic Normal Random Variable Stream Generator")
{
}

RandomVariableStreamNormalAntitheticTestCase::~RandomVariableStreamNormalAntitheticTestCase ()
{
}

double
RandomVariableStreamNormalAntitheticTestCase::ChiSquaredTest (Ptr<NormalRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, -4., 4.);
  range[0] = -std::numeric_limits<double>::max ();
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has mean equal to zero and standard
  // deviation equal to one, which are their default values for this
  // distribution.
  double sigma = 1.;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_gaussian_P (range[i + 1], sigma) - gsl_cdf_gaussian_P (range[i], sigma);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamNormalAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<NormalRandomVariable> n = CreateObject<NormalRandomVariable> ();

      // Make this generate antithetic values.
      n->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double mean = 5.0;
  double variance = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<NormalRandomVariable> x = CreateObject<NormalRandomVariable> ();
  x->SetAttribute ("Mean", DoubleValue (mean));
  x->SetAttribute ("Variance", DoubleValue (variance));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // normally distributed random variable is equal to mean.
  double expectedMean = mean;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for exponential distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamExponentialTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamExponentialTestCase ();
  virtual ~RandomVariableStreamExponentialTestCase ();

  double ChiSquaredTest (Ptr<ExponentialRandomVariable> e);

private:
  virtual void DoRun (void);
};

RandomVariableStreamExponentialTestCase::RandomVariableStreamExponentialTestCase ()
  : TestCase ("Exponential Random Variable Stream Generator")
{
}

RandomVariableStreamExponentialTestCase::~RandomVariableStreamExponentialTestCase ()
{
}

double
RandomVariableStreamExponentialTestCase::ChiSquaredTest (Ptr<ExponentialRandomVariable> e)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that e has mean equal to one, which is the
  // default value for this distribution.
  double mu = 1.;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_exponential_P (range[i + 1], mu) - gsl_cdf_exponential_P (range[i], mu);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, e->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamExponentialTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<ExponentialRandomVariable> e = CreateObject<ExponentialRandomVariable> ();
      double result = ChiSquaredTest (e);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double mean = 3.14;
  double bound = 0.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ExponentialRandomVariable> x = CreateObject<ExponentialRandomVariable> ();
  x->SetAttribute ("Mean", DoubleValue (mean));
  x->SetAttribute ("Bound", DoubleValue (bound));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // Test that values have approximately the right mean value.
  double TOLERANCE = mean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, mean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic exponential distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamExponentialAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamExponentialAntitheticTestCase ();
  virtual ~RandomVariableStreamExponentialAntitheticTestCase ();

  double ChiSquaredTest (Ptr<ExponentialRandomVariable> e);

private:
  virtual void DoRun (void);
};

RandomVariableStreamExponentialAntitheticTestCase::RandomVariableStreamExponentialAntitheticTestCase ()
  : TestCase ("Antithetic Exponential Random Variable Stream Generator")
{
}

RandomVariableStreamExponentialAntitheticTestCase::~RandomVariableStreamExponentialAntitheticTestCase ()
{
}

double
RandomVariableStreamExponentialAntitheticTestCase::ChiSquaredTest (Ptr<ExponentialRandomVariable> e)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that e has mean equal to one, which is the
  // default value for this distribution.
  double mu = 1.;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_exponential_P (range[i + 1], mu) - gsl_cdf_exponential_P (range[i], mu);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, e->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamExponentialAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<ExponentialRandomVariable> e = CreateObject<ExponentialRandomVariable> ();

      // Make this generate antithetic values.
      e->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (e);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double mean = 3.14;
  double bound = 0.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ExponentialRandomVariable> x = CreateObject<ExponentialRandomVariable> ();
  x->SetAttribute ("Mean", DoubleValue (mean));
  x->SetAttribute ("Bound", DoubleValue (bound));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // Test that values have approximately the right mean value.
  double TOLERANCE = mean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, mean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for Pareto distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamParetoTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamParetoTestCase ();
  virtual ~RandomVariableStreamParetoTestCase ();

  double ChiSquaredTest (Ptr<ParetoRandomVariable> p);

private:
  virtual void DoRun (void);
};

RandomVariableStreamParetoTestCase::RandomVariableStreamParetoTestCase ()
  : TestCase ("Pareto Random Variable Stream Generator")
{
}

RandomVariableStreamParetoTestCase::~RandomVariableStreamParetoTestCase ()
{
}

double
RandomVariableStreamParetoTestCase::ChiSquaredTest (Ptr<ParetoRandomVariable> p)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 1., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  double shape = 2.0;
  double scale = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_pareto_P (range[i + 1], shape, scale) - gsl_cdf_pareto_P (range[i], shape, scale);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, p->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamParetoTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<ParetoRandomVariable> e = CreateObject<ParetoRandomVariable> ();
      double result = ChiSquaredTest (e);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double shape = 2.0;
  double scale = 1.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ParetoRandomVariable> x = CreateObject<ParetoRandomVariable> ();
  x->SetAttribute ("Shape", DoubleValue (shape));
  x->SetAttribute ("Scale", DoubleValue (scale));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean is given by
  //
  //                   shape * scale
  //     E[value]  =  ---------------  ,
  //                     shape - 1
  // 
  // where
  // 
  //     scale  =  mean * (shape - 1.0) / shape .
  double expectedMean = (shape * scale) / (shape - 1.0);

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic Pareto distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamParetoAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamParetoAntitheticTestCase ();
  virtual ~RandomVariableStreamParetoAntitheticTestCase ();

  double ChiSquaredTest (Ptr<ParetoRandomVariable> p);

private:
  virtual void DoRun (void);
};

RandomVariableStreamParetoAntitheticTestCase::RandomVariableStreamParetoAntitheticTestCase ()
  : TestCase ("Antithetic Pareto Random Variable Stream Generator")
{
}

RandomVariableStreamParetoAntitheticTestCase::~RandomVariableStreamParetoAntitheticTestCase ()
{
}

double
RandomVariableStreamParetoAntitheticTestCase::ChiSquaredTest (Ptr<ParetoRandomVariable> p)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 1., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  double shape = 2.0;
  double scale = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_pareto_P (range[i + 1], shape, scale) - gsl_cdf_pareto_P (range[i], shape, scale);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, p->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamParetoAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<ParetoRandomVariable> e = CreateObject<ParetoRandomVariable> ();

      // Make this generate antithetic values.
      e->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (e);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double shape = 2.0;
  double scale = 1.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ParetoRandomVariable> x = CreateObject<ParetoRandomVariable> ();
  x->SetAttribute ("Shape", DoubleValue (shape));
  x->SetAttribute ("Scale", DoubleValue (scale));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean is given by
  //
  //                   shape * scale
  //     E[value]  =  ---------------  ,
  //                     shape - 1
  // 
  // where
  // 
  //     scale  =  mean * (shape - 1.0) / shape .
  //
  double expectedMean = (shape * scale) / (shape - 1.0);

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for Weibull distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamWeibullTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamWeibullTestCase ();
  virtual ~RandomVariableStreamWeibullTestCase ();

  double ChiSquaredTest (Ptr<WeibullRandomVariable> p);

private:
  virtual void DoRun (void);
};

RandomVariableStreamWeibullTestCase::RandomVariableStreamWeibullTestCase ()
  : TestCase ("Weibull Random Variable Stream Generator")
{
}

RandomVariableStreamWeibullTestCase::~RandomVariableStreamWeibullTestCase ()
{
}

double
RandomVariableStreamWeibullTestCase::ChiSquaredTest (Ptr<WeibullRandomVariable> p)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 1., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that p has shape equal to one and scale
  // equal to one, which are their default values for this
  // distribution.
  double a = 1.0;
  double b = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_weibull_P (range[i + 1], a, b) - gsl_cdf_weibull_P (range[i], a, b);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, p->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamWeibullTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<WeibullRandomVariable> e = CreateObject<WeibullRandomVariable> ();
      double result = ChiSquaredTest (e);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double scale = 5.0;
  double shape = 1.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<WeibullRandomVariable> x = CreateObject<WeibullRandomVariable> ();
  x->SetAttribute ("Scale", DoubleValue (scale));
  x->SetAttribute ("Shape", DoubleValue (shape));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // Weibull distributed random variable is
  //
  //     E[value]  =  scale * Gamma(1 + 1 / shape)  ,
  //               
  // where Gamma() is the Gamma function.  Note that 
  //               
  //     Gamma(n)  =  (n - 1)!
  //               
  // if n is a positive integer.
  //
  // For this test,
  //
  //     Gamma(1 + 1 / shape)  =  Gamma(1 + 1 / 1)
  //                           =  Gamma(2)
  //                           =  (2 - 1)!
  //                           =  1
  //
  // which means
  //
  //     E[value]  =  scale  .
  //               
  double expectedMean = scale;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic Weibull distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamWeibullAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamWeibullAntitheticTestCase ();
  virtual ~RandomVariableStreamWeibullAntitheticTestCase ();

  double ChiSquaredTest (Ptr<WeibullRandomVariable> p);

private:
  virtual void DoRun (void);
};

RandomVariableStreamWeibullAntitheticTestCase::RandomVariableStreamWeibullAntitheticTestCase ()
  : TestCase ("Antithetic Weibull Random Variable Stream Generator")
{
}

RandomVariableStreamWeibullAntitheticTestCase::~RandomVariableStreamWeibullAntitheticTestCase ()
{
}

double
RandomVariableStreamWeibullAntitheticTestCase::ChiSquaredTest (Ptr<WeibullRandomVariable> p)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 1., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that p has shape equal to one and scale
  // equal to one, which are their default values for this
  // distribution.
  double a = 1.0;
  double b = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_weibull_P (range[i + 1], a, b) - gsl_cdf_weibull_P (range[i], a, b);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, p->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamWeibullAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<WeibullRandomVariable> e = CreateObject<WeibullRandomVariable> ();

      // Make this generate antithetic values.
      e->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (e);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double scale = 5.0;
  double shape = 1.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<WeibullRandomVariable> x = CreateObject<WeibullRandomVariable> ();
  x->SetAttribute ("Scale", DoubleValue (scale));
  x->SetAttribute ("Shape", DoubleValue (shape));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // Weibull distributed random variable is
  //
  //     E[value]  =  scale * Gamma(1 + 1 / shape)  ,
  //               
  // where Gamma() is the Gamma function.  Note that 
  //               
  //     Gamma(n)  =  (n - 1)!
  //               
  // if n is a positive integer.
  //
  // For this test,
  //
  //     Gamma(1 + 1 / shape)  =  Gamma(1 + 1 / 1)
  //                           =  Gamma(2)
  //                           =  (2 - 1)!
  //                           =  1
  //
  // which means
  //
  //     E[value]  =  scale  .
  //               
  double expectedMean = scale;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for log-normal distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamLogNormalTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamLogNormalTestCase ();
  virtual ~RandomVariableStreamLogNormalTestCase ();

  double ChiSquaredTest (Ptr<LogNormalRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamLogNormalTestCase::RandomVariableStreamLogNormalTestCase ()
  : TestCase ("Log-Normal Random Variable Stream Generator")
{
}

RandomVariableStreamLogNormalTestCase::~RandomVariableStreamLogNormalTestCase ()
{
}

double
RandomVariableStreamLogNormalTestCase::ChiSquaredTest (Ptr<LogNormalRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has mu equal to zero and sigma
  // equal to one, which are their default values for this
  // distribution.
  double mu = 0.0;
  double sigma = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_lognormal_P (range[i + 1], mu, sigma) - gsl_cdf_lognormal_P (range[i], mu, sigma);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamLogNormalTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<LogNormalRandomVariable> n = CreateObject<LogNormalRandomVariable> ();
      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double mu = 5.0;
  double sigma = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<LogNormalRandomVariable> x = CreateObject<LogNormalRandomVariable> ();
  x->SetAttribute ("Mu", DoubleValue (mu));
  x->SetAttribute ("Sigma", DoubleValue (sigma));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // log-normally distributed random variable is equal to 
  //
  //                             2
  //                   mu + sigma  / 2
  //     E[value]  =  e                 .
  //
  double expectedMean = std::exp(mu + sigma * sigma / 2.0);

  // Test that values have approximately the right mean value.
  //
  /// \todo This test fails sometimes if the required tolerance is less
  /// than 3%, which may be because there is a bug in the
  /// implementation or that the mean of this distribution is more
  /// senstive to its parameters than the others are.
  double TOLERANCE = expectedMean * 3e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic log-normal distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamLogNormalAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamLogNormalAntitheticTestCase ();
  virtual ~RandomVariableStreamLogNormalAntitheticTestCase ();

  double ChiSquaredTest (Ptr<LogNormalRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamLogNormalAntitheticTestCase::RandomVariableStreamLogNormalAntitheticTestCase ()
  : TestCase ("Antithetic Log-Normal Random Variable Stream Generator")
{
}

RandomVariableStreamLogNormalAntitheticTestCase::~RandomVariableStreamLogNormalAntitheticTestCase ()
{
}

double
RandomVariableStreamLogNormalAntitheticTestCase::ChiSquaredTest (Ptr<LogNormalRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has mu equal to zero and sigma
  // equal to one, which are their default values for this
  // distribution.
  double mu = 0.0;
  double sigma = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_lognormal_P (range[i + 1], mu, sigma) - gsl_cdf_lognormal_P (range[i], mu, sigma);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamLogNormalAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<LogNormalRandomVariable> n = CreateObject<LogNormalRandomVariable> ();

      // Make this generate antithetic values.
      n->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double mu = 5.0;
  double sigma = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<LogNormalRandomVariable> x = CreateObject<LogNormalRandomVariable> ();
  x->SetAttribute ("Mu", DoubleValue (mu));
  x->SetAttribute ("Sigma", DoubleValue (sigma));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // log-normally distributed random variable is equal to 
  //
  //                             2
  //                   mu + sigma  / 2
  //     E[value]  =  e                 .
  //
  double expectedMean = std::exp(mu + sigma * sigma / 2.0);

  // Test that values have approximately the right mean value.
  //
  /// \todo This test fails sometimes if the required tolerance is less
  /// than 3%, which may be because there is a bug in the
  /// implementation or that the mean of this distribution is more
  /// senstive to its parameters than the others are.
  double TOLERANCE = expectedMean * 3e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for gamma distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamGammaTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamGammaTestCase ();
  virtual ~RandomVariableStreamGammaTestCase ();

  double ChiSquaredTest (Ptr<GammaRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamGammaTestCase::RandomVariableStreamGammaTestCase ()
  : TestCase ("Gamma Random Variable Stream Generator")
{
}

RandomVariableStreamGammaTestCase::~RandomVariableStreamGammaTestCase ()
{
}

double
RandomVariableStreamGammaTestCase::ChiSquaredTest (Ptr<GammaRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has alpha equal to one and beta
  // equal to one, which are their default values for this
  // distribution.
  double alpha = 1.0;
  double beta = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_gamma_P (range[i + 1], alpha, beta) - gsl_cdf_gamma_P (range[i], alpha, beta);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamGammaTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<GammaRandomVariable> n = CreateObject<GammaRandomVariable> ();
      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double alpha = 5.0;
  double beta = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<GammaRandomVariable> x = CreateObject<GammaRandomVariable> ();
  x->SetAttribute ("Alpha", DoubleValue (alpha));
  x->SetAttribute ("Beta", DoubleValue (beta));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // gammaly distributed random variable is equal to 
  //
  //     E[value]  =  alpha * beta  .
  //
  double expectedMean = alpha * beta;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic gamma distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamGammaAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamGammaAntitheticTestCase ();
  virtual ~RandomVariableStreamGammaAntitheticTestCase ();

  double ChiSquaredTest (Ptr<GammaRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamGammaAntitheticTestCase::RandomVariableStreamGammaAntitheticTestCase ()
  : TestCase ("Antithetic Gamma Random Variable Stream Generator")
{
}

RandomVariableStreamGammaAntitheticTestCase::~RandomVariableStreamGammaAntitheticTestCase ()
{
}

double
RandomVariableStreamGammaAntitheticTestCase::ChiSquaredTest (Ptr<GammaRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has alpha equal to one and beta
  // equal to one, which are their default values for this
  // distribution.
  double alpha = 1.0;
  double beta = 1.0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_gamma_P (range[i + 1], alpha, beta) - gsl_cdf_gamma_P (range[i], alpha, beta);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamGammaAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<GammaRandomVariable> n = CreateObject<GammaRandomVariable> ();

      // Make this generate antithetic values.
      n->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  double alpha = 5.0;
  double beta = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<GammaRandomVariable> x = CreateObject<GammaRandomVariable> ();

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  x->SetAttribute ("Alpha", DoubleValue (alpha));
  x->SetAttribute ("Beta", DoubleValue (beta));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // gammaly distributed random variable is equal to 
  //
  //     E[value]  =  alpha * beta  .
  //
  double expectedMean = alpha * beta;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for Erlang distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamErlangTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamErlangTestCase ();
  virtual ~RandomVariableStreamErlangTestCase ();

  double ChiSquaredTest (Ptr<ErlangRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamErlangTestCase::RandomVariableStreamErlangTestCase ()
  : TestCase ("Erlang Random Variable Stream Generator")
{
}

RandomVariableStreamErlangTestCase::~RandomVariableStreamErlangTestCase ()
{
}

double
RandomVariableStreamErlangTestCase::ChiSquaredTest (Ptr<ErlangRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has k equal to one and lambda
  // equal to one, which are their default values for this
  // distribution.
  uint32_t k = 1;
  double lambda = 1.0;

  // Note that Erlang distribution is equal to the gamma distribution
  // when k is an iteger, which is why the gamma distribution's cdf
  // function can be used here.
  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_gamma_P (range[i + 1], k, lambda) - gsl_cdf_gamma_P (range[i], k, lambda);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamErlangTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<ErlangRandomVariable> n = CreateObject<ErlangRandomVariable> ();
      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  uint32_t k = 5;
  double lambda = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ErlangRandomVariable> x = CreateObject<ErlangRandomVariable> ();
  x->SetAttribute ("K", IntegerValue (k));
  x->SetAttribute ("Lambda", DoubleValue (lambda));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // Erlangly distributed random variable is equal to 
  //
  //     E[value]  =  k * lambda  .
  //
  double expectedMean = k * lambda;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic Erlang distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamErlangAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_RUNS = 5;
  static const uint32_t N_BINS = 50;
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamErlangAntitheticTestCase ();
  virtual ~RandomVariableStreamErlangAntitheticTestCase ();

  double ChiSquaredTest (Ptr<ErlangRandomVariable> n);

private:
  virtual void DoRun (void);
};

RandomVariableStreamErlangAntitheticTestCase::RandomVariableStreamErlangAntitheticTestCase ()
  : TestCase ("Antithetic Erlang Random Variable Stream Generator")
{
}

RandomVariableStreamErlangAntitheticTestCase::~RandomVariableStreamErlangAntitheticTestCase ()
{
}

double
RandomVariableStreamErlangAntitheticTestCase::ChiSquaredTest (Ptr<ErlangRandomVariable> n)
{
  gsl_histogram * h = gsl_histogram_alloc (N_BINS);

  double range[N_BINS + 1];
  FillHistoRangeUniformly (range, N_BINS + 1, 0., 10.);
  range[N_BINS] = std::numeric_limits<double>::max ();

  gsl_histogram_set_ranges (h, range, N_BINS + 1);

  double expected[N_BINS];

  // Note that this assumes that n has k equal to one and lambda
  // equal to one, which are their default values for this
  // distribution.
  uint32_t k = 1;
  double lambda = 1.0;

  // Note that Erlang distribution is equal to the gamma distribution
  // when k is an iteger, which is why the gamma distribution's cdf
  // function can be used here.
  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      expected[i] = gsl_cdf_gamma_P (range[i + 1], k, lambda) - gsl_cdf_gamma_P (range[i], k, lambda);
      expected[i] *= N_MEASUREMENTS;
    }

  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      gsl_histogram_increment (h, n->GetValue ());
    }

  double tmp[N_BINS];

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      tmp[i] = gsl_histogram_get (h, i);
      tmp[i] -= expected[i];
      tmp[i] *= tmp[i];
      tmp[i] /= expected[i];
    }

  gsl_histogram_free (h);

  double chiSquared = 0;

  for (uint32_t i = 0; i < N_BINS; ++i)
    {
      chiSquared += tmp[i];
    }

  return chiSquared;
}

void
RandomVariableStreamErlangAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double sum = 0.;
  double maxStatistic = gsl_cdf_chisq_Qinv (0.05, N_BINS);

  for (uint32_t i = 0; i < N_RUNS; ++i)
    {
      Ptr<ErlangRandomVariable> n = CreateObject<ErlangRandomVariable> ();

      // Make this generate antithetic values.
      n->SetAttribute ("Antithetic", BooleanValue (true));

      double result = ChiSquaredTest (n);
      sum += result;
    }

  sum /= (double)N_RUNS;

  NS_TEST_ASSERT_MSG_LT (sum, maxStatistic, "Chi-squared statistic out of range");

  uint32_t k = 5;
  double lambda = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ErlangRandomVariable> x = CreateObject<ErlangRandomVariable> ();

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  x->SetAttribute ("K", IntegerValue (k));
  x->SetAttribute ("Lambda", DoubleValue (lambda));

  // Calculate the mean of these values.
  sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // Erlangly distributed random variable is equal to 
  //
  //     E[value]  =  k * lambda  .
  //
  double expectedMean = k * lambda;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for Zipf distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamZipfTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamZipfTestCase ();
  virtual ~RandomVariableStreamZipfTestCase ();

private:
  virtual void DoRun (void);
};

RandomVariableStreamZipfTestCase::RandomVariableStreamZipfTestCase ()
  : TestCase ("Zipf Random Variable Stream Generator")
{
}

RandomVariableStreamZipfTestCase::~RandomVariableStreamZipfTestCase ()
{
}

void
RandomVariableStreamZipfTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  uint32_t n = 1;
  double alpha = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ZipfRandomVariable> x = CreateObject<ZipfRandomVariable> ();
  x->SetAttribute ("N", IntegerValue (n));
  x->SetAttribute ("Alpha", DoubleValue (alpha));

  // Calculate the mean of these values.
  double sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // Zipfly distributed random variable is equal to 
  //
  //                   H
  //                    N, alpha - 1
  //     E[value]  =  ---------------
  //                     H
  //                      N, alpha
  //                          
  // where
  //
  //                    N   
  //                   ---    
  //                   \     -alpha
  //     H          =  /    m        .
  //      N, alpha     ---
  //                   m=1    
  //                 
  // For this test,
  //
  //                      -(alpha - 1)
  //                     1
  //     E[value]  =  ---------------
  //                      -alpha
  //                     1
  //
  //               =  1  .
  //               
  double expectedMean = 1.0;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic Zipf distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamZipfAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamZipfAntitheticTestCase ();
  virtual ~RandomVariableStreamZipfAntitheticTestCase ();

private:
  virtual void DoRun (void);
};

RandomVariableStreamZipfAntitheticTestCase::RandomVariableStreamZipfAntitheticTestCase ()
  : TestCase ("Antithetic Zipf Random Variable Stream Generator")
{
}

RandomVariableStreamZipfAntitheticTestCase::~RandomVariableStreamZipfAntitheticTestCase ()
{
}

void
RandomVariableStreamZipfAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  uint32_t n = 1;
  double alpha = 2.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ZipfRandomVariable> x = CreateObject<ZipfRandomVariable> ();
  x->SetAttribute ("N", IntegerValue (n));
  x->SetAttribute ("Alpha", DoubleValue (alpha));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  double sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // Zipfly distributed random variable is equal to 
  //
  //                   H
  //                    N, alpha - 1
  //     E[value]  =  ---------------
  //                     H
  //                      N, alpha
  //                          
  // where
  //
  //                    N   
  //                   ---    
  //                   \     -alpha
  //     H          =  /    m        .
  //      N, alpha     ---
  //                   m=1    
  //                 
  // For this test,
  //
  //                      -(alpha - 1)
  //                     1
  //     E[value]  =  ---------------
  //                      -alpha
  //                     1
  //
  //               =  1  .
  //               
  double expectedMean = 1.0;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for Zeta distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamZetaTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamZetaTestCase ();
  virtual ~RandomVariableStreamZetaTestCase ();

private:
  virtual void DoRun (void);
};

RandomVariableStreamZetaTestCase::RandomVariableStreamZetaTestCase ()
  : TestCase ("Zeta Random Variable Stream Generator")
{
}

RandomVariableStreamZetaTestCase::~RandomVariableStreamZetaTestCase ()
{
}

void
RandomVariableStreamZetaTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double alpha = 5.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ZetaRandomVariable> x = CreateObject<ZetaRandomVariable> ();
  x->SetAttribute ("Alpha", DoubleValue (alpha));

  // Calculate the mean of these values.
  double sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // zetaly distributed random variable is equal to 
  //
  //                   zeta(alpha - 1)
  //     E[value]  =  ---------------   for alpha > 2 ,
  //                     zeta(alpha)
  //                          
  // where zeta(alpha) is the Riemann zeta function.
  //                 
  // There are no simple analytic forms for the Riemann zeta function,
  // which is why the gsl library is used in this test to calculate
  // the known mean of the values.
  double expectedMean = gsl_sf_zeta_int (alpha - 1) / gsl_sf_zeta_int (alpha);

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for antithetic Zeta distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamZetaAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamZetaAntitheticTestCase ();
  virtual ~RandomVariableStreamZetaAntitheticTestCase ();

private:
  virtual void DoRun (void);
};

RandomVariableStreamZetaAntitheticTestCase::RandomVariableStreamZetaAntitheticTestCase ()
  : TestCase ("Antithetic Zeta Random Variable Stream Generator")
{
}

RandomVariableStreamZetaAntitheticTestCase::~RandomVariableStreamZetaAntitheticTestCase ()
{
}

void
RandomVariableStreamZetaAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  double alpha = 5.0;
  double value;

  // Create the RNG with the specified range.
  Ptr<ZetaRandomVariable> x = CreateObject<ZetaRandomVariable> ();
  x->SetAttribute ("Alpha", DoubleValue (alpha));

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  double sum = 0.0;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by a
  // zetaly distributed random variable is equal to 
  //
  //                   zeta(alpha - 1)
  //     E[value]  =  ---------------   for alpha > 2 ,
  //                     zeta(alpha)
  //                          
  // where zeta(alpha) is the Riemann zeta function.
  //                 
  // There are no simple analytic forms for the Riemann zeta function,
  // which is why the gsl library is used in this test to calculate
  // the known mean of the values.
  double expectedMean = gsl_sf_zeta_int (alpha - 1) / gsl_sf_zeta_int (alpha);

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

// ===========================================================================
// Test case for deterministic random variable stream generator
// ===========================================================================
class RandomVariableStreamDeterministicTestCase : public TestCase
{
public:
  static const double TOLERANCE;

  RandomVariableStreamDeterministicTestCase ();
  virtual ~RandomVariableStreamDeterministicTestCase ();

private:
  virtual void DoRun (void);
};

const double RandomVariableStreamDeterministicTestCase::TOLERANCE = 1e-8;

RandomVariableStreamDeterministicTestCase::RandomVariableStreamDeterministicTestCase ()
  : TestCase ("Deterministic Random Variable Stream Generator")
{
}

RandomVariableStreamDeterministicTestCase::~RandomVariableStreamDeterministicTestCase ()
{
}

void
RandomVariableStreamDeterministicTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  Ptr<DeterministicRandomVariable> s = CreateObject<DeterministicRandomVariable> ();

  // The following array should give the sequence
  //
  //    4, 4, 7, 7, 10, 10 .
  //
  double array1 [] = { 4, 4, 7, 7, 10, 10};
  uint64_t count1 = 6;
  s->SetValueArray (array1, count1);

  double value;

  // Test that the first sequence is correct.
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4, TOLERANCE, "Sequence 1 value 1 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4, TOLERANCE, "Sequence 1 value 2 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 7, TOLERANCE, "Sequence 1 value 3 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 7, TOLERANCE, "Sequence 1 value 4 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 10, TOLERANCE, "Sequence 1 value 5 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 10, TOLERANCE, "Sequence 1 value 6 wrong."); 

  // The following array should give the sequence
  //
  //    1000, 2000, 7, 7 .
  //
  double array2 [] = { 1000, 2000, 3000, 4000};
  uint64_t count2 = 4;
  s->SetValueArray (array2, count2);

  // Test that the second sequence is correct.
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 1000, TOLERANCE, "Sequence 2 value 1 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 2000, TOLERANCE, "Sequence 2 value 2 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 3000, TOLERANCE, "Sequence 2 value 3 wrong."); 
  value = s->GetValue (); 
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4000, TOLERANCE, "Sequence 2 value 4 wrong."); 
  value = s->GetValue (); 
}

// ===========================================================================
// Test case for empirical distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamEmpiricalTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamEmpiricalTestCase ();
  virtual ~RandomVariableStreamEmpiricalTestCase ();

private:
  virtual void DoRun (void);
};

RandomVariableStreamEmpiricalTestCase::RandomVariableStreamEmpiricalTestCase ()
  : TestCase ("Empirical Random Variable Stream Generator")
{
}

RandomVariableStreamEmpiricalTestCase::~RandomVariableStreamEmpiricalTestCase ()
{
}

void
RandomVariableStreamEmpiricalTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  // Create the RNG with a uniform distribution between 0 and 10.
  Ptr<EmpiricalRandomVariable> x = CreateObject<EmpiricalRandomVariable> ();
  x->CDF ( 0.0,  0.0);
  x->CDF ( 5.0,  0.5);
  x->CDF (10.0,  1.0);

  // Calculate the mean of these values.
  double sum = 0.0;
  double value;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by this
  // empirical distribution is the midpoint of the distribution
  //
  //     E[value]  =  5 .
  //                          
  double expectedMean = 5.0;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 

  // Bug 2082: Create the RNG with a uniform distribution between -1 and 1.
  Ptr<EmpiricalRandomVariable> y = CreateObject<EmpiricalRandomVariable> ();
  y->CDF (-1.0,  0.0);
  y->CDF (0.0,  0.5);
  y->CDF (1.0,  1.0);
  NS_TEST_ASSERT_MSG_LT (y->GetValue (), 2, "Empirical variable with negative domain");
}

// ===========================================================================
// Test case for antithetic empirical distribution random variable stream generator
// ===========================================================================
class RandomVariableStreamEmpiricalAntitheticTestCase : public TestCase
{
public:
  static const uint32_t N_MEASUREMENTS = 1000000;

  RandomVariableStreamEmpiricalAntitheticTestCase ();
  virtual ~RandomVariableStreamEmpiricalAntitheticTestCase ();

private:
  virtual void DoRun (void);
};

RandomVariableStreamEmpiricalAntitheticTestCase::RandomVariableStreamEmpiricalAntitheticTestCase ()
  : TestCase ("EmpiricalAntithetic Random Variable Stream Generator")
{
}

RandomVariableStreamEmpiricalAntitheticTestCase::~RandomVariableStreamEmpiricalAntitheticTestCase ()
{
}

void
RandomVariableStreamEmpiricalAntitheticTestCase::DoRun (void)
{
  SetTestSuiteSeed ();

  // Create the RNG with a uniform distribution between 0 and 10.
  Ptr<EmpiricalRandomVariable> x = CreateObject<EmpiricalRandomVariable> ();
  x->CDF ( 0.0,  0.0);
  x->CDF ( 5.0,  0.5);
  x->CDF (10.0,  1.0);

  // Make this generate antithetic values.
  x->SetAttribute ("Antithetic", BooleanValue (true));

  // Calculate the mean of these values.
  double sum = 0.0;
  double value;
  for (uint32_t i = 0; i < N_MEASUREMENTS; ++i)
    {
      value = x->GetValue ();
      sum += value;
    }
  double valueMean = sum / N_MEASUREMENTS;

  // The expected value for the mean of the values returned by this
  // empirical distribution is the midpoint of the distribution
  //
  //     E[value]  =  5 .
  //                          
  double expectedMean = 5.0;

  // Test that values have approximately the right mean value.
  double TOLERANCE = expectedMean * 1e-2;
  NS_TEST_ASSERT_MSG_EQ_TOL (valueMean, expectedMean, TOLERANCE, "Wrong mean value."); 
}

class RandomVariableStreamTestSuite : public TestSuite
{
public:
  RandomVariableStreamTestSuite ();
};

RandomVariableStreamTestSuite::RandomVariableStreamTestSuite ()
  : TestSuite ("random-variable-stream-generators", UNIT)
{
  AddTestCase (new RandomVariableStreamUniformTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamUniformAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamConstantTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamSequentialTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamNormalTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamNormalAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamExponentialTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamExponentialAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamParetoTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamParetoAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamWeibullTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamWeibullAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamLogNormalTestCase, TestCase::QUICK);
  /// \todo This test is currently disabled because it fails sometimes.
  /// A possible reason for the failure is that the antithetic code is
  /// not implemented properly for this log-normal case.
  /*
  AddTestCase (new RandomVariableStreamLogNormalAntitheticTestCase, TestCase::QUICK);
  */
  AddTestCase (new RandomVariableStreamGammaTestCase, TestCase::QUICK);
  /// \todo This test is currently disabled because it fails sometimes.
  /// A possible reason for the failure is that the antithetic code is
  /// not implemented properly for this gamma case.
  /*
  AddTestCase (new RandomVariableStreamGammaAntitheticTestCase, TestCase::QUICK);
  */
  AddTestCase (new RandomVariableStreamErlangTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamErlangAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamZipfTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamZipfAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamZetaTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamZetaAntitheticTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamDeterministicTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamEmpiricalTestCase, TestCase::QUICK);
  AddTestCase (new RandomVariableStreamEmpiricalAntitheticTestCase, TestCase::QUICK);
}

static RandomVariableStreamTestSuite randomVariableStreamTestSuite;
