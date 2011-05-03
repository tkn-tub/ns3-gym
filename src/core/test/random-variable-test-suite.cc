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
// Author: Hadi Arbabi<marbabi@cs.odu.edu>
//

#include <iostream>
#include <math.h>

#include "ns3/test.h"
#include "ns3/assert.h"
#include "ns3/integer.h"
#include "ns3/random-variable.h"

using namespace std;

namespace ns3 {
class BasicRandomNumberTestCase : public TestCase
{
public:
  BasicRandomNumberTestCase ();
  virtual ~BasicRandomNumberTestCase ()
  {
  }

private:
  virtual void DoRun (void);
};

BasicRandomNumberTestCase::BasicRandomNumberTestCase ()
  : TestCase ("Check basic random number operation")
{
}

void
BasicRandomNumberTestCase::DoRun (void)
{
  const double desiredMean = 1.0;
  const double desiredStdDev = 1.0;

  double tmp = log (1 + (desiredStdDev / desiredMean) * (desiredStdDev / desiredMean));
  double sigma = sqrt (tmp);
  double mu = log (desiredMean) - 0.5 * tmp;

  //
  // Test a custom lognormal instance to see if its moments have any relation
  // expected reality.
  //
  LogNormalVariable lognormal (mu, sigma);
  vector<double> samples;
  const int NSAMPLES = 10000;
  double sum = 0;

  //
  // Get and store a bunch of samples.  As we go along sum them and then find
  // the mean value of the samples.
  //
  for (int n = NSAMPLES; n; --n)
    {
      double value = lognormal.GetValue ();
      sum += value;
      samples.push_back (value);
    }
  double obtainedMean = sum / NSAMPLES;
  NS_TEST_EXPECT_MSG_EQ_TOL (obtainedMean, desiredMean, 0.1, "Got unexpected mean value from LogNormalVariable");

  //
  // Wander back through the saved stamples and find their standard deviation
  //
  sum = 0;
  for (vector<double>::iterator iter = samples.begin (); iter != samples.end (); iter++)
    {
      double tmp = (*iter - obtainedMean);
      sum += tmp * tmp;
    }
  double obtainedStdDev = sqrt (sum / (NSAMPLES - 1));
  NS_TEST_EXPECT_MSG_EQ_TOL (obtainedStdDev, desiredStdDev, 0.1, "Got unexpected standard deviation from LogNormalVariable");
}

class RandomNumberSerializationTestCase : public TestCase
{
public:
  RandomNumberSerializationTestCase ();
  virtual ~RandomNumberSerializationTestCase ()
  {
  }

private:
  virtual void DoRun (void);
};

RandomNumberSerializationTestCase::RandomNumberSerializationTestCase ()
  : TestCase ("Check basic random number operation")
{
}

void
RandomNumberSerializationTestCase::DoRun (void)
{
  RandomVariableValue val;
  val.DeserializeFromString ("Uniform:0.1:0.2", MakeRandomVariableChecker ());
  RandomVariable rng = val.Get ();
  NS_TEST_ASSERT_MSG_EQ (val.SerializeToString (MakeRandomVariableChecker ()), "Uniform:0.1:0.2",
                         "Deserialize and Serialize \"Uniform:0.1:0.2\" mismatch");

  val.DeserializeFromString ("Normal:0.1:0.2", MakeRandomVariableChecker ());
  rng = val.Get ();
  NS_TEST_ASSERT_MSG_EQ (val.SerializeToString (MakeRandomVariableChecker ()), "Normal:0.1:0.2",
                         "Deserialize and Serialize \"Normal:0.1:0.2\" mismatch");

  val.DeserializeFromString ("Normal:0.1:0.2:0.15", MakeRandomVariableChecker ());
  rng = val.Get ();
  NS_TEST_ASSERT_MSG_EQ (val.SerializeToString (MakeRandomVariableChecker ()), "Normal:0.1:0.2:0.15",
                         "Deserialize and Serialize \"Normal:0.1:0.2:0.15\" mismatch");
}

class BasicRandomNumberTestSuite : public TestSuite
{
public:
  BasicRandomNumberTestSuite ();
};

BasicRandomNumberTestSuite::BasicRandomNumberTestSuite ()
  : TestSuite ("basic-random-number", UNIT)
{
  AddTestCase (new BasicRandomNumberTestCase);
  AddTestCase (new RandomNumberSerializationTestCase);
}

static BasicRandomNumberTestSuite BasicRandomNumberTestSuite;

} // namespace ns3
