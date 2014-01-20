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
#include <ns3/double.h>
#include <ns3/cosine-antenna-model.h>
#include <ns3/simulator.h>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>


NS_LOG_COMPONENT_DEFINE ("TestCosineAntennaModel");

using namespace ns3;

enum CosineAntennaModelGainTestCondition  {
  EQUAL = 0,
  LESSTHAN = 1
};

class CosineAntennaModelTestCase : public TestCase
{
public:
  static std::string BuildNameString (Angles a, double b, double o, double g);
  CosineAntennaModelTestCase (Angles a, double b, double o, double g, double expectedGainDb, CosineAntennaModelGainTestCondition cond);


private:
  virtual void DoRun (void);

  Angles m_a;
  double m_b;
  double m_o;
  double m_g;
  double m_expectedGain;
  CosineAntennaModelGainTestCondition m_cond;
};

std::string CosineAntennaModelTestCase::BuildNameString (Angles a, double b, double o, double g)
{
  std::ostringstream oss;
  oss <<  "theta=" << a.theta << " , phi=" << a.phi 
      << ", beamdwidth=" << b << "deg"
      << ", orientation=" << o 
      << ", maxGain=" << g << " dB";
  return oss.str ();
}


CosineAntennaModelTestCase::CosineAntennaModelTestCase (Angles a, double b, double o, double g, double expectedGainDb, CosineAntennaModelGainTestCondition cond)
  : TestCase (BuildNameString (a, b, o, g)),
    m_a (a),
    m_b (b),
    m_o (o),
    m_g (g),
    m_expectedGain (expectedGainDb),
    m_cond (cond)
{
}

void
CosineAntennaModelTestCase::DoRun ()
{
  NS_LOG_FUNCTION (this << BuildNameString (m_a, m_b, m_o, m_g));

  Ptr<CosineAntennaModel> a = CreateObject<CosineAntennaModel> ();
  a->SetAttribute ("Beamwidth", DoubleValue (m_b));
  a->SetAttribute ("Orientation", DoubleValue (m_o));
  a->SetAttribute ("MaxGain", DoubleValue (m_g));
  double actualGain = a->GetGainDb (m_a);
  switch (m_cond) 
    {
    case EQUAL:
      NS_TEST_EXPECT_MSG_EQ_TOL (actualGain, m_expectedGain, 0.001, "wrong value of the radiation pattern");
      break;
    case LESSTHAN:
      NS_TEST_EXPECT_MSG_LT (actualGain, m_expectedGain, "gain higher than expected");
      break;
    default:
      break;
    }
}




class CosineAntennaModelTestSuite : public TestSuite
{
public:
  CosineAntennaModelTestSuite ();
};

CosineAntennaModelTestSuite::CosineAntennaModelTestSuite ()
  : TestSuite ("cosine-antenna-model", UNIT)
{ 
  // to calculate the azimut angle offset for a given gain in db:
  // phideg = (2*acos(10^(targetgaindb/(20*n))))*180/pi
  // e.g., with a 60 deg beamwidth, gain is -20dB at +- 74.945 degrees from boresight

  //                                                                      phi,  theta, beamwidth, orientation,  maxGain,  expectedGain,   condition
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    0),        60,           0,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),        60,           0,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),        60,           0,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (100),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (150),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (180),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-180),    0),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);

  // test positive orientation
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (60),    0),        60,          60,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),        60,          60,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),        60,          60,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (150),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (160),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (210),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (240),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-40),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-120),    0),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);

  // test negative orientation and different beamwidths
  // with a 100 deg beamwidth, gain is -20dB at +- 117.47 degrees from boresight
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),       100,        -150,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),    0),       100,        -150,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),    0),       100,        -150,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-32.531), 0),       100,        -150,        0,           -20,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (92.531),  0),       100,        -150,        0,           -20,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    0),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (60),    0),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  // with a 150 deg beamwidth, gain is -10dB at +- 124.93 degrees from boresight, and -20dB at +- 155.32 degrees from boresight
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),       150,        -150,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (135),    0),       150,        -150,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-75),    0),       150,        -150,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (85.070),  0),       150,        -150,        0,           -10,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-25.070), 0),       150,        -150,        0,           -10,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (5.3230),  0),       150,        -150,        0,           -20,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (54.677),  0),       150,        -150,        0,           -20,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),       150,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (20),    0),       150,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);

  // test maxGain
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    0),        60,           0,       10,            10,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),        60,           0,       22,            19,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),        60,           0,       -4,            -7,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    0),        60,           0,       10,           -10,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),        60,           0,      -20,           -40,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (100),    0),        60,           0,       40,            20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),       100,        -150,        2,             2,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),    0),       100,        -150,        4,             1,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),    0),       100,        -150,       -1,            -4,     EQUAL), TestCase::QUICK);


  // test elevation angle
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    2),        60,           0,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    2),        60,           0,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    2),        60,           0,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    2),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-180),    2),        60,           0,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (60),   -3),        60,          60,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),   -3),        60,          60,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),   -3),        60,          60,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-120),   -3),        60,          60,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),   -3),       100,        -150,        0,             0,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),   -3),       100,        -150,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),   -3),       100,        -150,        0,            -3,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),   -3),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),  9.5),       100,        -150,        0,           -20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),  9.5),        60,           0,       10,            10,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),  9.5),        60,           0,       22,            19,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),  9.5),        60,           0,       -4,            -7,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (100),  9.5),        60,           0,       40,            20,  LESSTHAN), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),  9.5),       100,        -150,        2,             2,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),  9.5),       100,        -150,        4,             1,     EQUAL), TestCase::QUICK);
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),  9.5),       100,        -150,       -1,            -4,     EQUAL), TestCase::QUICK);

};

static CosineAntennaModelTestSuite staticCosineAntennaModelTestSuiteInstance;
