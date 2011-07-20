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

#ifndef LTE_TEST_PATHLOSS_MODEL_H
#define LTE_TEST_PATHLOSS_MODEL_H

#include "ns3/spectrum-value.h"

#include "ns3/test.h"

#include <ns3/buildings-mobility-model.h>
#include <ns3/buildings-propagation-loss-model.h>


using namespace ns3;


/**
 * Test 1.1 pathloss calculation
 */
class LtePathlossModelTestSuite : public TestSuite
{
public:
  LtePathlossModelTestSuite ();
};


class LtePathlossModelTestCase : public TestCase
{
public:
  LtePathlossModelTestCase (double freq, Ptr<BuildingsMobilityModel> m1, Ptr<BuildingsMobilityModel> m2, BuildingsPropagationLossModel::Environment env, BuildingsPropagationLossModel::CitySize city, double refValue, std::string name);
  virtual ~LtePathlossModelTestCase ();

private:
  virtual void DoRun (void);

//   Ptr<SpectrumValue> m_sv;
//   Ptr<const SpectrumModel> m_sm;
//   Ptr<SpectrumValue> m_sinr;
  double m_freq;
  Ptr<BuildingsMobilityModel> m_node1;
  Ptr<BuildingsMobilityModel> m_node2;
  BuildingsPropagationLossModel::Environment m_env;
  BuildingsPropagationLossModel::CitySize m_city;
  double m_lossRef;
  
};


#endif /* LTE_TEST_PATHLOSS_MODEL_H */

