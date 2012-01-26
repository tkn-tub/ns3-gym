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
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>


NS_LOG_COMPONENT_DEFINE ("TestCosineAntennaModel");

namespace ns3 {

class CosineAntennaModelTestCase : public TestCase
{
public:
  static std::string BuildNameString (Angles a, double b, double o, double g);
  CosineAntennaModelTestCase (Angles a, double b, double o, double g, double expectedGainDb);


private:
  virtual void DoRun (void);

  Angles m_a;
  double m_b;
  double m_o;
  double m_g;
  double m_expectedGain;
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


CosineAntennaModelTestCase::CosineAntennaModelTestCase (Angles a, double b, double o, double g, double expectedGainDb)
  : TestCase (BuildNameString (a, b, o, g)),
    m_a (a),
    m_b (b),
    m_o (o),
    m_g (g),
    m_expectedGain (expectedGainDb)
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
  NS_TEST_EXPECT_MSG_EQ_TOL (actualGain, m_expectedGain, 0.001, "wrong value of the radiation pattern");
}




class CosineAntennaModelTestSuite : public TestSuite
{
public:
  CosineAntennaModelTestSuite ();
};

CosineAntennaModelTestSuite::CosineAntennaModelTestSuite ()
  : TestSuite ("cosine-antenna-model", UNIT)
{ 
  //                                                                                   phi,  theta, beamwidth, orientation,  maxGain,  expectedGain
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    0),        60,           0,        0,             0));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),        60,           0,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),        60,           0,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (100),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (150),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (180),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-180),    0),        60,           0,        0,     -INFINITY));

  // test positive orientation
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (60),    0),        60,          60,        0,             0));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),        60,          60,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),        60,          60,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (150),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (160),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (210),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (240),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-40),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    0),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-120),    0),        60,          60,        0,     -INFINITY));

  // test negative orientation and different beamwidth
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),       100,        -150,        0,             0));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),    0),       100,        -150,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),    0),       100,        -150,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (60),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (110),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (110),    0),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (120),    0),       100,        -150,        0,     -INFINITY));
                          
  // test maxGain
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    0),        60,           0,       10,            10));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    0),        60,           0,       22,            19));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    0),        60,           0,       -4,            -7));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    0),        60,           0,       10,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),    0),        60,           0,      -20,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (100),    0),        60,           0,       40,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),    0),       100,        -150,        2,             2));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),    0),       100,        -150,        4,             1));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),    0),       100,        -150,       -1,            -4));


  // test elevation angle
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),    2),        60,           0,        0,             0));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),    2),        60,           0,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),    2),        60,           0,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-90),    2),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-180),    2),        60,           0,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (60),   -3),        60,          60,        0,             0));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (90),   -3),        60,          60,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),   -3),        60,          60,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-120),   -3),        60,          60,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),   -3),       100,        -150,        0,             0));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),   -3),       100,        -150,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),   -3),       100,        -150,        0,            -3));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),   -3),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (120),  9.5),       100,        -150,        0,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians    (0),  9.5),        60,           0,       10,            10));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians   (30),  9.5),        60,           0,       22,            19));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (-30),  9.5),        60,           0,       -4,            -7));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians  (100),  9.5),        60,           0,       40,     -INFINITY));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-150),  9.5),       100,        -150,        2,             2));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-100),  9.5),       100,        -150,        4,             1));
  AddTestCase (new CosineAntennaModelTestCase (Angles (DegreesToRadians (-200),  9.5),       100,        -150,       -1,            -4));
                   
};

static CosineAntennaModelTestSuite staticCosineAntennaModelTestSuiteInstance;




} // namespace ns3
