/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
* Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
* Author: Marco Miozzo <marco.miozzo@cttc.es>
*/

#ifndef BUILDINGS_SHADOWING_TEST_H
#define BUILDINGS_SHADOWING_TEST_H

#include "ns3/test.h"



using namespace ns3;

/**
 * Test shadowing calculation
 */
class BuildingsShadowingTestSuite : public TestSuite
{
public:
  BuildingsShadowingTestSuite ();
};


class BuildingsShadowingTestCase : public TestCase
{
public:
  BuildingsShadowingTestCase (uint16_t m1, uint16_t m2, double refValue, double sigmaRef, std::string name);
  virtual ~BuildingsShadowingTestCase ();

private:
  virtual void DoRun (void);
  Ptr<MobilityModel> CreateMobilityModel (uint16_t index);

  uint16_t m_mobilityModelIndex1;
  uint16_t m_mobilityModelIndex2;
  double m_lossRef;     // pathloss value (without shadowing)
  double m_sigmaRef;

};

#endif /*BUILDINGS_SHADOWING_TEST_H*/
