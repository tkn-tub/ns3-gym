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
#include <ns3/isotropic-antenna-model.h>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

namespace ns3 {

class IsotropicAntennaModelTestCase : public TestCase
{
public:
  static std::string BuildNameString (Angles a);
  IsotropicAntennaModelTestCase (Angles a, double expectedGainDb);


private:
  virtual void DoRun (void);

  Angles m_a;
  double m_expectedGain;
};

std::string IsotropicAntennaModelTestCase::BuildNameString (Angles a)
{
  std::ostringstream oss;
  oss <<  "theta=" << a.theta << " , phi=" << a.phi;
  return oss.str ();
}


IsotropicAntennaModelTestCase::IsotropicAntennaModelTestCase (Angles a, double expectedGainDb)
  : TestCase (BuildNameString (a)),
    m_a (a),
    m_expectedGain (expectedGainDb)
{
}

void
IsotropicAntennaModelTestCase::DoRun ()
{
  Ptr<IsotropicAntennaModel> a = Create<IsotropicAntennaModel> ();
  double actualGain = a->GetGainDb (m_a);
  NS_TEST_EXPECT_MSG_EQ_TOL (actualGain, m_expectedGain, 0.01, "wrong value of the radiation pattern");
}




class IsotropicAntennaModelTestSuite : public TestSuite
{
public:
  IsotropicAntennaModelTestSuite ();
};

IsotropicAntennaModelTestSuite::IsotropicAntennaModelTestSuite ()
  : TestSuite ("isotropic-antenna-model", UNIT)
{
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (0, 0),           0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (0, M_PI),        0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (0, M_PI_2),      0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (M_PI, 0),        0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (M_PI, M_PI),     0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (M_PI, M_PI_2),   0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (M_PI_2, 0),      0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (M_PI_2, M_PI),   0.0), TestCase::QUICK);
  AddTestCase (new IsotropicAntennaModelTestCase (Angles (M_PI_2, M_PI_2), 0.0), TestCase::QUICK);

};

static IsotropicAntennaModelTestSuite staticIsotropicAntennaModelTestSuiteInstance;




} // namespace ns3
