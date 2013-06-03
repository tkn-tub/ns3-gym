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

#ifndef BUILDINGS_PATHLOSS_TEST_H
#define BUILDINGS_PATHLOSS_TEST_H

#include <ns3/test.h>
// #include <ns3/buildings-mobility-model.h>
#include <ns3/hybrid-buildings-propagation-loss-model.h>


namespace ns3 {

/**
* Test 1.1 pathloss calculation
*/
class BuildingsPathlossTestSuite : public TestSuite
{
public:
  BuildingsPathlossTestSuite ();
};


class BuildingsPathlossTestCase : public TestCase
{
public:
  BuildingsPathlossTestCase (double freq, uint16_t m1, uint16_t m2, EnvironmentType env, CitySize city, double refValue, std::string name);
  virtual ~BuildingsPathlossTestCase ();

private:
  virtual void DoRun (void);
  Ptr<MobilityModel> CreateMobilityModel (uint16_t index);

  double m_freq;
  uint16_t m_mobilityModelIndex1;
  uint16_t m_mobilityModelIndex2;
  EnvironmentType m_env;
  CitySize m_city;
  double m_lossRef;

};


#endif /* BUILDING_PATHLOSS_TEST_H */


} // namespace ns3
