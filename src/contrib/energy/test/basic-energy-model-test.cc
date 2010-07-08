/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Network Security Lab, University of Washington, Seattle.
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
 * Authors: He Wu <mdzz@u.washington.edu>
 */

#include "ns3/basic-energy-source.h"
#include "ns3/basic-radio-energy-model.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/basic-radio-energy-model-helper.h"
#include "ns3/log.h"
#include "ns3/test.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include <math.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BasicEnergyModelTestSuite");

/**
 * Test case of update remaining energy for BasicEnergySource and
 * BasicRadioEnergyModel.
 */
class BasicEnergyUpdateTest : public TestCase
{
public:
  BasicEnergyUpdateTest ();
  virtual ~BasicEnergyUpdateTest ();

private:
  bool DoRun (void);

  /**
   * \param state Radio state to switch to.
   * \return False if no error occurs.
   *
   * Runs simulation for a while, check if final state & remaining energy is
   * correctly updated.
   */
  bool StateSwitchTest (RadioEnergyModel::RadioState state);

private:
  double m_timeS;     // in seconds
  double m_tolerance; // tolerance for power estimation

  ObjectFactory m_energySource;
  ObjectFactory m_deviceEnergyModel;
};

BasicEnergyUpdateTest::BasicEnergyUpdateTest ()
  : TestCase ("Basic energy model update remaining energy test case")
{
  m_timeS = 15.5; // idle for 15 seconds before changing state
  m_tolerance = 1.0e-13;  //
}

BasicEnergyUpdateTest::~BasicEnergyUpdateTest ()
{
}

bool
BasicEnergyUpdateTest::DoRun (void)
{
  // set types
  m_energySource.SetTypeId ("ns3::BasicEnergySource");
  m_deviceEnergyModel.SetTypeId ("ns3::BasicRadioEnergyModel");

  // run state switch tests
  if (StateSwitchTest (RadioEnergyModel::TX))
    {
      return true;
    }
  if (StateSwitchTest (RadioEnergyModel::RX))
    {
      return true;
    }
  if (StateSwitchTest (RadioEnergyModel::IDLE))
    {
      return true;
    }
  if (StateSwitchTest (RadioEnergyModel::SLEEP))
    {
      return true;
    }

  // error free
  return false;
}

bool
BasicEnergyUpdateTest::StateSwitchTest (
  RadioEnergyModel::RadioState state)
{
  // create node
  Ptr<Node> node = CreateObject<Node> ();

  // create energy source
  Ptr<EnergySource> source = m_energySource.Create<EnergySource> ();
  // aggregate energy source to node
  node->AggregateObject (source);

  // create device energy model
  Ptr<BasicRadioEnergyModel> model =
    m_deviceEnergyModel.Create<BasicRadioEnergyModel> ();
  // set energy source pointer
  model->SetEnergySource (source);
  // add device energy model to model list in energy source
  source->AppendDeviceEnergyModel (model);

  // retrieve device energy model from energy source
  EnergySource::DeviceEnergyModelList modelList =
    source->FindDeviceEnergyModels ("ns3::BasicRadioEnergyModel");
  // check list
  NS_TEST_ASSERT_MSG_EQ (false, modelList.empty (), "Model list is empty!");
  // get pointer
  Ptr<BasicRadioEnergyModel> devModel =
    DynamicCast<BasicRadioEnergyModel> (modelList[0]);
  // check pointer
  NS_TEST_ASSERT_MSG_NE (0, devModel, "NULL pointer to device model!");

  // schedule change of state
  Simulator::Schedule (Seconds (m_timeS),
                       &BasicRadioEnergyModel::UpdateRemainingEnergy, devModel, state);

  // run simulation
  Simulator::Stop (Seconds (m_timeS * 2));  // run twice as long
  Simulator::Run ();
  Simulator::Destroy ();

  // calculate estimated remaining energy
  double estRemainingEnergy = source->GetInitialEnergy ();
  estRemainingEnergy -= devModel->GetIdlePowerW () * m_timeS;
  /*
   * Energy is updated periodically, hence we have to take into account of the
   * update interval.
   */
  double actualTime = m_timeS;
  actualTime /= devModel->GetEnergyUpdateInterval ().GetSeconds ();
  actualTime = floor (actualTime); // rounding for update interval
  actualTime *= devModel->GetEnergyUpdateInterval ().GetSeconds ();
  switch (state)
    {
    case RadioEnergyModel::TX:
      estRemainingEnergy -= devModel->GetTxPowerW () * actualTime;
      break;
    case RadioEnergyModel::RX:
      estRemainingEnergy -= devModel->GetRxPowerW () * actualTime;
      break;
    case RadioEnergyModel::IDLE:
      estRemainingEnergy -= devModel->GetIdlePowerW () * actualTime;
      break;
    case RadioEnergyModel::SLEEP:
      estRemainingEnergy -= devModel->GetSleepPowerW () * actualTime;
      break;
    default:
      break;
    }
  // obtain remaining energy
  double remainingEnergy = source->GetRemainingEnergy ();
  NS_LOG_UNCOND ("Remaining energy is " << remainingEnergy << "\n"
                                        << "Estimated remaining energy is " << estRemainingEnergy << "\n"
                                        << "Difference is " << estRemainingEnergy - remainingEnergy);
  // check remaining energy
  NS_TEST_ASSERT_MSG_EQ_TOL (remainingEnergy, estRemainingEnergy, m_tolerance,
                             "Incorrect remaining energy!");


  // obtain radio state
  RadioEnergyModel::RadioState endState = devModel->GetCurrentState ();
  NS_LOG_UNCOND ("Radio state is " << endState);
  // check end state
  NS_TEST_ASSERT_MSG_EQ (endState, state,  "Incorrect end state!");

  return false; // no error
}

// ----------------------------------------------------------------------------//

/**
 * Test case of energy depletion handling for BasicEnergySource and
 * BasicRadioEnergyModel.
 */
class BasicEnergyDepletionTest : public TestCase
{
public:
  BasicEnergyDepletionTest ();
  virtual ~BasicEnergyDepletionTest ();

private:
  bool DoRun (void);

  // / Callback invoked when energy is drained from source.
  void DepletionHandler (void);

  /**
   * \param simTimeS Simulation time, in seconds.
   * \param updateIntervalS Device model update interval, in seconds.
   * \return False if all is good.
   *
   * Runs simulation with specified simulation time and update interval.
   */
  bool DepletionTestCase (double simTimeS, double updateIntervalS);

private:
  int m_numOfModels;        // number of BasicRadioEnergyModel to install
  int m_callbackCount;      // counter for # of callbacks invoked
  double m_simTimeS;        // maximum simulation time, in seconds
  double m_timeStepS;       // simulation time step size, in seconds
  double m_updateIntervalS; // update interval of each device model

};

BasicEnergyDepletionTest::BasicEnergyDepletionTest ()
  : TestCase ("Basic energy model energy depletion test case")
{
  m_numOfModels = 10;
  m_callbackCount = 0;
  m_simTimeS = 4.5;
  m_timeStepS = 0.5;
  m_updateIntervalS = 1.5;
}

BasicEnergyDepletionTest::~BasicEnergyDepletionTest ()
{
}

bool
BasicEnergyDepletionTest::DoRun (void)
{
  /*
   * Run simulation with different simulation time and update interval.
   */
  for (double simTimeS = 0.0; simTimeS <= m_simTimeS; simTimeS += m_timeStepS)
    {
      for (double updateIntervalS = 0.5; updateIntervalS <= m_updateIntervalS;
           updateIntervalS += m_timeStepS)
        {
          if (DepletionTestCase (simTimeS, updateIntervalS))
            {
              return true;
            }
          // reset callback count
          m_callbackCount = 0;
        }
    }
  // error free
  return false;
}

void
BasicEnergyDepletionTest::DepletionHandler (void)
{
  m_callbackCount++;
}

bool
BasicEnergyDepletionTest::DepletionTestCase (double simTimeS,
                                             double updateIntervalS)
{
  // create node
  Ptr<Node> node = CreateObject<Node> ();

  /*
   * Create and install energy source and a single basic radio energy model on
   * the node using helpers.
   */
  // source helper
  BasicEnergySourceHelper basicSourceHelper;
  // set energy to 0 so that we deplete energy at the beginning of simulation
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (0.0));
  // device energy model helper
  BasicRadioEnergyModelHelper radioEnergyHelper;
  // set update interval
  Time updateInterval = Seconds (1.0);
  radioEnergyHelper.Set ("PeriodicEnergyUpdateInterval",
                         TimeValue (Seconds (updateIntervalS)));
  // set energy depletion callback
  BasicRadioEnergyModel::BasicEnergyDepletionCallback callback =
    MakeCallback (&BasicEnergyDepletionTest::DepletionHandler, this);
  radioEnergyHelper.SetDepletionCallback (callback);
  // energy model helper
  EnergyModelHelper energyHelper;
  // install on node
  energyHelper.Install (basicSourceHelper, radioEnergyHelper, node);

  // Install more basic radio energy models onto the same node, using helper
  for (int i = 1; i < m_numOfModels; i++)
    {
      radioEnergyHelper.Install (node);
    }

  // run simulation
  Simulator::Stop (Seconds (simTimeS));
  Simulator::Run ();
  Simulator::Destroy ();

  /*
   * Calculate total number of callbacks invoked. Taking the ceiling instead of
   * the floor here because initial update is at time = 0.
   */
  double tmp = ceil (simTimeS / updateIntervalS);
  int numOfUpdates = (tmp == 0) ? 1 : static_cast<int> (tmp);
  /*
   * Every update will trigger *all* DeviceEnergyModels to react, therefore the
   * total count should be numOfUpdates * m_numOfModels ^ 2
   */
  int totalCallbackCount = numOfUpdates * m_numOfModels * m_numOfModels;

  NS_LOG_UNCOND ("Simulation time = " << simTimeS << "s\n"
                                      << "Update interval = " << updateIntervalS << "s\n"
                                      << "Calculated callback count is " << totalCallbackCount << "\n"
                                      << "Actual callback count is " << m_callbackCount);

  // check result
  NS_TEST_ASSERT_MSG_EQ (totalCallbackCount, m_callbackCount,
                         "Not all callbacks are invoked!");

  return false;
}

// ----------------------------------------------------------------------------//

/**
 * Unit test suite for energy model. Although the test suite involves 2 modules
 * it is still considered a unit test. Because a DeviceEnergyModel cannot live
 * without an EnergySource.
 */
class BasicEnergyModelTestSuite : public TestSuite
{
public:
  BasicEnergyModelTestSuite ();
};

BasicEnergyModelTestSuite::BasicEnergyModelTestSuite ()
  : TestSuite ("devices-basic-energy-model", UNIT)
{
  AddTestCase (new BasicEnergyUpdateTest);
  AddTestCase (new BasicEnergyDepletionTest);
}

// create an instance of the test suite
BasicEnergyModelTestSuite g_energyModelTestSuite;
