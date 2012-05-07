/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of Washington
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
 * Author: Mitch Watrous (watrous@u.washington.edu)
 */

#include <math.h>

#include "ns3/test.h"
#include "ns3/average.h"

using namespace ns3;

const double TOLERANCE = 1e-14;

// ===========================================================================
// Test case for a single integer.
// ===========================================================================

class OneIntegerAverageTestCase : public TestCase
{
public:
  OneIntegerAverageTestCase ();
  virtual ~OneIntegerAverageTestCase ();

private:
  virtual void DoRun (void);
};

OneIntegerAverageTestCase::OneIntegerAverageTestCase ()
  : TestCase ("Average Object Test using One Integer")

{
}

OneIntegerAverageTestCase::~OneIntegerAverageTestCase ()
{
}

void
OneIntegerAverageTestCase::DoRun (void)
{
  Average<int> calculator;

  long count = 1;

  double sum = 0;
  double sqrSum = 0;
  double min;
  double max;
  double mean;
  double stddev;
  double variance;

  // Put all of the values into the calculator.
  int multiple = 5;
  int value;
  for (long i = 0; i < count; i++)
    {
      value = multiple * (i + 1);

      calculator.Update (value);

      sum    += value;
      sqrSum += value * value;
    }

  // Calculate the expected values for the statistical functions.
  min = multiple;
  max = multiple * count;
  mean = sum / count;
  if (count == 1)
    {
      variance = 0;
    }
  else
    {
      variance = (count * sqrSum - sum * sum) / (count * (count - 1));
    }
  stddev = sqrt (variance);

  // Test the calculator.
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Count (),    count,    TOLERANCE, "Count value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Min (),      min,      TOLERANCE, "Min value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Max (),      max,      TOLERANCE, "Max value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Mean (),     mean,     TOLERANCE, "Mean value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Stddev (),   stddev,   TOLERANCE, "Stddev value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Var (), variance, TOLERANCE, "Variance value wrong");
}


// ===========================================================================
// Test case for five integers.
// ===========================================================================

class FiveIntegersAverageTestCase : public TestCase
{
public:
  FiveIntegersAverageTestCase ();
  virtual ~FiveIntegersAverageTestCase ();

private:
  virtual void DoRun (void);
};

FiveIntegersAverageTestCase::FiveIntegersAverageTestCase ()
  : TestCase ("Average Object Test using Five Integers")

{
}

FiveIntegersAverageTestCase::~FiveIntegersAverageTestCase ()
{
}

void
FiveIntegersAverageTestCase::DoRun (void)
{
  Average<int> calculator;

  long count = 5;

  double sum = 0;
  double sqrSum = 0;
  double min;
  double max;
  double mean;
  double stddev;
  double variance;

  // Put all of the values into the calculator.
  int multiple = 5;
  int value;
  for (long i = 0; i < count; i++)
    {
      value = multiple * (i + 1);

      calculator.Update (value);

      sum    += value;
      sqrSum += value * value;
    }

  // Calculate the expected values for the statistical functions.
  min = multiple;
  max = multiple * count;
  mean = sum / count;
  if (count == 1)
    {
      variance = 0;
    }
  else
    {
      variance = (count * sqrSum - sum * sum) / (count * (count - 1));
    }
  stddev = sqrt (variance);

  // Test the calculator.
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Count (),    count,    TOLERANCE, "Count value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Min (),      min,      TOLERANCE, "Min value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Max (),      max,      TOLERANCE, "Max value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Mean (),     mean,     TOLERANCE, "Mean value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Stddev (),   stddev,   TOLERANCE, "Stddev value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Var (), variance, TOLERANCE, "Variance value wrong");
}


// ===========================================================================
// Test case for five double values.
// ===========================================================================

class FiveDoublesAverageTestCase : public TestCase
{
public:
  FiveDoublesAverageTestCase ();
  virtual ~FiveDoublesAverageTestCase ();

private:
  virtual void DoRun (void);
};

FiveDoublesAverageTestCase::FiveDoublesAverageTestCase ()
  : TestCase ("Average Object Test using Five Double Values")

{
}

FiveDoublesAverageTestCase::~FiveDoublesAverageTestCase ()
{
}

void
FiveDoublesAverageTestCase::DoRun (void)
{
  Average<double> calculator;

  long count = 5;

  double sum = 0;
  double sqrSum = 0;
  double min;
  double max;
  double mean;
  double stddev;
  double variance;

  // Put all of the values into the calculator.
  double multiple = 3.14;
  double value;
  for (long i = 0; i < count; i++)
    {
      value = multiple * (i + 1);

      calculator.Update (value);

      sum    += value;
      sqrSum += value * value;
    }

  // Calculate the expected values for the statistical functions.
  min = multiple;
  max = multiple * count;
  mean = sum / count;
  if (count == 1)
    {
      variance = 0;
    }
  else
    {
      variance = (count * sqrSum - sum * sum) / (count * (count - 1));
    }
  stddev = sqrt (variance);

  // Test the calculator.
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Count (),    count,    TOLERANCE, "Count value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Min (),      min,      TOLERANCE, "Min value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Max (),      max,      TOLERANCE, "Max value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Mean (),     mean,     TOLERANCE, "Mean value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Stddev (),   stddev,   TOLERANCE, "Stddev value wrong");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculator.Var (), variance, TOLERANCE, "Variance value wrong");
}


class AverageTestSuite : public TestSuite
{
public:
  AverageTestSuite ();
};

AverageTestSuite::AverageTestSuite ()
  : TestSuite ("average", UNIT)
{
  AddTestCase (new OneIntegerAverageTestCase);
  AddTestCase (new FiveIntegersAverageTestCase);
  AddTestCase (new FiveDoublesAverageTestCase);
}

static AverageTestSuite averageTestSuite;
