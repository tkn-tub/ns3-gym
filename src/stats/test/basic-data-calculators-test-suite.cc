/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of Washington
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
#include "ns3/basic-data-calculators.h"

using namespace ns3;


// ===========================================================================
// Test case for a single integer.
// ===========================================================================

class OneIntegerTestCase : public TestCase
{
public:
  OneIntegerTestCase ();
  virtual ~OneIntegerTestCase ();

private:
  virtual void DoRun (void);
};

OneIntegerTestCase::OneIntegerTestCase ()
  : TestCase ("Basic Statistical Functions using One Integer")

{
}

OneIntegerTestCase::~OneIntegerTestCase ()
{
}

void
OneIntegerTestCase::DoRun (void)
{
  MinMaxAvgTotalCalculator<int> calculator;

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
  for (uint32_t i = 0; i < count; i++)
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
  NS_TEST_ASSERT_MSG_EQ (calculator.getCount(),    count,    "Count value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getSum(),      sum,      "Sum value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMin(),      min,      "Min value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMax(),      max,      "Max value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMean(),     mean,     "Mean value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getStddev(),   stddev,   "Stddev value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getVariance(), variance, "Variance value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getSqrSum(),   sqrSum,   "SqrSum value wrong");
}


// ===========================================================================
// Test case for five integers.
// ===========================================================================

class FiveIntegersTestCase : public TestCase
{
public:
  FiveIntegersTestCase ();
  virtual ~FiveIntegersTestCase ();

private:
  virtual void DoRun (void);
};

FiveIntegersTestCase::FiveIntegersTestCase ()
  : TestCase ("Basic Statistical Functions using Five Integers")

{
}

FiveIntegersTestCase::~FiveIntegersTestCase ()
{
}

void
FiveIntegersTestCase::DoRun (void)
{
  MinMaxAvgTotalCalculator<int> calculator;

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
  for (uint32_t i = 0; i < count; i++)
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
  NS_TEST_ASSERT_MSG_EQ (calculator.getCount(),    count,    "Count value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getSum(),      sum,      "Sum value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMin(),      min,      "Min value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMax(),      max,      "Max value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMean(),     mean,     "Mean value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getStddev(),   stddev,   "Stddev value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getVariance(), variance, "Variance value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getSqrSum(),   sqrSum,   "SqrSum value wrong");
}


// ===========================================================================
// Test case for five double values.
// ===========================================================================

class FiveDoublesTestCase : public TestCase
{
public:
  FiveDoublesTestCase ();
  virtual ~FiveDoublesTestCase ();

private:
  virtual void DoRun (void);
};

FiveDoublesTestCase::FiveDoublesTestCase ()
  : TestCase ("Basic Statistical Functions using Five Double Values")

{
}

FiveDoublesTestCase::~FiveDoublesTestCase ()
{
}

void
FiveDoublesTestCase::DoRun (void)
{
  MinMaxAvgTotalCalculator<double> calculator;

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
  for (uint32_t i = 0; i < count; i++)
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
  NS_TEST_ASSERT_MSG_EQ (calculator.getCount(),    count,    "Count value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getSum(),      sum,      "Sum value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMin(),      min,      "Min value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMax(),      max,      "Max value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getMean(),     mean,     "Mean value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getStddev(),   stddev,   "Stddev value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getVariance(), variance, "Variance value wrong");
  NS_TEST_ASSERT_MSG_EQ (calculator.getSqrSum(),   sqrSum,   "SqrSum value wrong");
}


class BasicDataCalculatorsTestSuite : public TestSuite
{
public:
  BasicDataCalculatorsTestSuite ();
};

BasicDataCalculatorsTestSuite::BasicDataCalculatorsTestSuite ()
  : TestSuite ("basic-data-calculators", UNIT)
{
  AddTestCase (new OneIntegerTestCase);
  AddTestCase (new FiveIntegersTestCase);
  AddTestCase (new FiveDoublesTestCase);
}

static BasicDataCalculatorsTestSuite basicDataCalculatorsTestSuite;
