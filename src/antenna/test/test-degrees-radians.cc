/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/antenna-model.h>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

using namespace ns3;

class DegreesToRadiansTestCase : public TestCase
{
public:
  static std::string BuildNameString (double a);
  DegreesToRadiansTestCase (double a, double b);


private:
  virtual void DoRun (void);

  double m_a;
  double m_b;
};

std::string DegreesToRadiansTestCase::BuildNameString (double a)
{
  std::ostringstream oss;
  oss <<  "angle = " << a << " degrees";
  return oss.str ();
}


DegreesToRadiansTestCase::DegreesToRadiansTestCase (double a, double b)
  : TestCase (BuildNameString (a)),
    m_a (a),
    m_b (b)
{
}

void
DegreesToRadiansTestCase::DoRun ()
{

  NS_TEST_EXPECT_MSG_EQ_TOL (DegreesToRadians (m_a), m_b, 1e-10, "wrong conversion");
}



class RadiansToDegreesTestCase : public TestCase
{
public:
  static std::string BuildNameString (double a);
  RadiansToDegreesTestCase (double a, double b);


private:
  virtual void DoRun (void);

  double m_a;
  double m_b;
};

std::string RadiansToDegreesTestCase::BuildNameString (double a)
{
  std::ostringstream oss;
  oss <<  "angle = " << a << " degrees";
  return oss.str ();
}


RadiansToDegreesTestCase::RadiansToDegreesTestCase (double a, double b)
  : TestCase (BuildNameString (a)),
    m_a (a),
    m_b (b)
{
}

void
RadiansToDegreesTestCase::DoRun ()
{

  NS_TEST_EXPECT_MSG_EQ_TOL (RadiansToDegrees (m_a), m_b, 1e-10, "wrong conversion");
}



class DegreesRadiansTestSuite : public TestSuite
{
public:
  DegreesRadiansTestSuite ();
};

DegreesRadiansTestSuite::DegreesRadiansTestSuite ()
  : TestSuite ("degrees-radians", UNIT)
{
  AddTestCase (new DegreesToRadiansTestCase (0, 0), TestCase::QUICK);
  AddTestCase (new DegreesToRadiansTestCase (90, M_PI_2), TestCase::QUICK);
  AddTestCase (new DegreesToRadiansTestCase (180, M_PI), TestCase::QUICK);
  AddTestCase (new DegreesToRadiansTestCase (270, M_PI + M_PI_2), TestCase::QUICK);
  AddTestCase (new DegreesToRadiansTestCase (360, M_PI + M_PI), TestCase::QUICK);
  AddTestCase (new DegreesToRadiansTestCase (-90, -M_PI_2), TestCase::QUICK);
  AddTestCase (new DegreesToRadiansTestCase (810, 4.5*M_PI), TestCase::QUICK);

  AddTestCase (new RadiansToDegreesTestCase (0, 0), TestCase::QUICK);
  AddTestCase (new RadiansToDegreesTestCase (M_PI_2, 90), TestCase::QUICK);
  AddTestCase (new RadiansToDegreesTestCase (M_PI, 180), TestCase::QUICK);
  AddTestCase (new RadiansToDegreesTestCase (M_PI + M_PI_2, 270), TestCase::QUICK);
  AddTestCase (new RadiansToDegreesTestCase (M_PI + M_PI, 360), TestCase::QUICK);
  AddTestCase (new RadiansToDegreesTestCase (-M_PI_2, -90), TestCase::QUICK);
  AddTestCase (new RadiansToDegreesTestCase (4.5*M_PI, 810), TestCase::QUICK);

};

static DegreesRadiansTestSuite staticDegreesRadiansTestSuiteInstance;
