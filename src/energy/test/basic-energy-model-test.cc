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
 * Author: He Wu <mdzz@u.washington.edu>
 */

#include "ns3/basic-energy-source.h"
#include "ns3/wifi-radio-energy-model.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/wifi-radio-energy-model-helper.h"
#include "ns3/energy-source-container.h"
#include "ns3/device-energy-model-container.h"
#include "ns3/log.h"
#include "ns3/test.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include <math.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BasicEnergyModelTestSuite");

/**
 * Test case of update remaining energy for BasicEnergySource and
 * WifiRadioEnergyModel.
 */
class BasicEnergyUpdateTest : public TestCase
{
public:
  BasicEnergyUpdateTest ();
  virtual ~BasicEnergyUpdateTest ();

private:
  void DoRun (void);

  /**
   * \param state Radio state to switch to.
   * \return False if no error occurs.
   *
   * Runs simulation for a while, check if final state & remaining energy is
   * correctly updated.
   */
  bool StateSwitchTest (WifiPhy::State state);

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

void
BasicEnergyUpdateTest::DoRun (void)
{
  // set types
  m_energySource.SetTypeId ("ns3::BasicEnergySource");
  m_deviceEnergyModel.SetTypeId ("ns3::WifiRadioEnergyModel");

  // run state switch tests
  NS_TEST_ASSERT_MSG_EQ (StateSwitchTest (WifiPhy::IDLE), false, "Problem with state switch test (WifiPhy idle).");
  NS_TEST_ASSERT_MSG_EQ (StateSwitchTest (WifiPhy::CCA_BUSY), false, "Problem with state switch test (WifiPhy cca busy).");
  NS_TEST_ASSERT_MSG_EQ (StateSwitchTest (WifiPhy::TX), false, "Problem with state switch test (WifiPhy tx).");
  NS_TEST_ASSERT_MSG_EQ (StateSwitchTest (WifiPhy::RX), false, "Problem with state switch test (WifiPhy rx).");
  NS_TEST_ASSERT_MSG_EQ (StateSwitchTest (WifiPhy::SWITCHING), false, "Problem with state switch test (WifiPhy switching).");
}

bool
BasicEnergyUpdateTest::StateSwitchTest (WifiPhy::State state)
{
  // create node
  Ptr<Node> node = CreateObject<Node> ();

  // create energy source
  Ptr<BasicEnergySource> source = m_energySource.Create<BasicEnergySource> ();
  // aggregate energy source to node
  node->AggregateObject (source);

  // create device energy model
  Ptr<WifiRadioEnergyModel> model =
    m_deviceEnergyModel.Create<WifiRadioEnergyModel> ();
  // set energy source pointer
  model->SetEnergySource (source);
  // add device energy model to model list in energy source
  source->AppendDeviceEnergyModel (model);

  // retrieve device energy model from energy source
  DeviceEnergyModelContainer models =
    source->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel");
  // check list
  NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL (false, (models.GetN () == 0), "Model list is empty!");
  // get pointer
  Ptr<WifiRadioEnergyModel> devModel =
    DynamicCast<WifiRadioEnergyModel> (models.Get (0));
  // check pointer
  NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL (0, devModel, "NULL pointer to device model!");

  /*
   * The radio will stay IDLE for m_timeS seconds. Then it will switch into a
   * different state.
   */

  // schedule change of state
  Simulator::Schedule (Seconds (m_timeS),
                       &WifiRadioEnergyModel::ChangeState, devModel, state);

  // Calculate remaining energy at simulation stop time
  Simulator::Schedule (Seconds (m_timeS * 2), 
                       &BasicEnergySource::UpdateEnergySource, source);

  double timeDelta = 0.000000001; // 1 nanosecond
  // run simulation; stop just after last scheduled event
  Simulator::Stop (Seconds (m_timeS * 2 + timeDelta));
  Simulator::Run ();

  // energy = current * voltage * time

  // calculate idle power consumption
  double estRemainingEnergy = source->GetInitialEnergy ();
  double voltage = source->GetSupplyVoltage ();
  estRemainingEnergy -= devModel->GetIdleCurrentA () * voltage * m_timeS;

  /*
   * Manually calculate the number of periodic updates performed by the source.
   * This is to check if the periodic updates are performed correctly.
   */
  double actualTime = m_timeS;
  actualTime /= source->GetEnergyUpdateInterval ().GetSeconds ();
  actualTime = floor (actualTime); // rounding for update interval
  actualTime *= source->GetEnergyUpdateInterval ().GetSeconds ();

  // calculate new state power consumption
  double current = 0.0;
  switch (state)
    {
    case WifiPhy::IDLE:
      current = devModel->GetIdleCurrentA ();
      break;
    case WifiPhy::CCA_BUSY:
      current = devModel->GetCcaBusyCurrentA ();
      break;
    case WifiPhy::TX:
      current = devModel->GetTxCurrentA ();
      break;
    case WifiPhy::RX:
      current = devModel->GetRxCurrentA ();
      break;
    case WifiPhy::SWITCHING:
      current = devModel->GetSwitchingCurrentA ();
      break;
    default:
      NS_FATAL_ERROR ("Undefined radio state: " << state);
      break;
    }
  estRemainingEnergy -= current * voltage * m_timeS;

  // obtain remaining energy from source
  double remainingEnergy = source->GetRemainingEnergy ();
  NS_LOG_DEBUG ("Remaining energy is " << remainingEnergy);
  NS_LOG_DEBUG ("Estimated remaining energy is " << estRemainingEnergy);
  NS_LOG_DEBUG ("Difference is " << estRemainingEnergy - remainingEnergy);
  // check remaining energy
  NS_TEST_ASSERT_MSG_EQ_TOL_RETURNS_BOOL (remainingEnergy, estRemainingEnergy, m_tolerance,
                                          "Incorrect remaining energy!");

  // obtain radio state
  WifiPhy::State endState = devModel->GetCurrentState ();
  NS_LOG_DEBUG ("Radio state is " << endState);
  // check end state
  NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL (endState, state,  "Incorrect end state!");
  Simulator::Destroy ();

  return false; // no error
}

// -------------------------------------------------------------------------- //

/**
 * Test case of energy depletion handling for BasicEnergySource and
 * WifiRadioEnergyModel.
 */
class BasicEnergyDepletionTest : public TestCase
{
public:
  BasicEnergyDepletionTest ();
  virtual ~BasicEnergyDepletionTest ();

private:
  void DoRun (void);

  /**
   * Callback invoked when energy is drained from source.
   */
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
  int m_numOfNodes;         // number of nodes in simulation
  int m_callbackCount;      // counter for # of callbacks invoked
  double m_simTimeS;        // maximum simulation time, in seconds
  double m_timeStepS;       // simulation time step size, in seconds
  double m_updateIntervalS; // update interval of each device model

};

BasicEnergyDepletionTest::BasicEnergyDepletionTest ()
  : TestCase ("Basic energy model energy depletion test case")
{
  m_numOfNodes = 10;
  m_callbackCount = 0;
  m_simTimeS = 4.5;
  m_timeStepS = 0.5;
  m_updateIntervalS = 1.5;
}

BasicEnergyDepletionTest::~BasicEnergyDepletionTest ()
{
}

void
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
          NS_TEST_ASSERT_MSG_EQ (DepletionTestCase (simTimeS, updateIntervalS), false, "Depletion test case problem.");
          // reset callback count
          m_callbackCount = 0;
        }
    }
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
  NodeContainer c;
  c.Create (m_numOfNodes);

  std::string phyMode ("DsssRate1Mbps");

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold",
                      StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

  // install YansWifiPhy
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  /*
   * This is one parameter that matters when using FixedRssLossModel, set it to
   * zero; otherwise, gain will be added.
   */
  wifiPhy.Set ("RxGain", DoubleValue (0));
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper MAC, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue (phyMode),
                                "ControlMode", StringValue (phyMode));
  // Set it to ad-hoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

  /*
   * Create and install energy source and a single basic radio energy model on
   * the node using helpers.
   */
  // source helper
  BasicEnergySourceHelper basicSourceHelper;
  // set energy to 0 so that we deplete energy at the beginning of simulation
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (0.0));
  // set update interval
  basicSourceHelper.Set ("PeriodicEnergyUpdateInterval",
                         TimeValue (Seconds (updateIntervalS)));
  // install source
  EnergySourceContainer sources = basicSourceHelper.Install (c);

  // device energy model helper
  WifiRadioEnergyModelHelper radioEnergyHelper;
  // set energy depletion callback
  WifiRadioEnergyModel::WifiRadioEnergyDepletionCallback callback =
    MakeCallback (&BasicEnergyDepletionTest::DepletionHandler, this);
  radioEnergyHelper.SetDepletionCallback (callback);
  // install on node
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (devices, sources);

  // run simulation
  Simulator::Stop (Seconds (simTimeS));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_DEBUG ("Simulation time = " << simTimeS << "s");
  NS_LOG_DEBUG ("Update interval = " << updateIntervalS << "s");
  NS_LOG_DEBUG ("Expected callback count is " << m_numOfNodes);
  NS_LOG_DEBUG ("Actual callback count is " << m_callbackCount);

  // check result, call back should only be invoked once
  NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL (m_numOfNodes, m_callbackCount, "Not all callbacks are invoked!");

  return false;
}

// -------------------------------------------------------------------------- //

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
  : TestSuite ("basic-energy-model", UNIT)
{
  AddTestCase (new BasicEnergyUpdateTest);
  AddTestCase (new BasicEnergyDepletionTest);
}

// create an instance of the test suite
static BasicEnergyModelTestSuite g_energyModelTestSuite;

