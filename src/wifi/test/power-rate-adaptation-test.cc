/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universidad de la República - Uruguay
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
 * Author: Matías Richart <mrichart@fing.edu.uy>
 */

#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/adhoc-wifi-mac.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/parf-wifi-manager.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/error-rate-model.h"
#include "ns3/yans-error-rate-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/object-factory.h"
#include "ns3/dca-txop.h"
#include "ns3/mac-rx-middle.h"
#include "ns3/pointer.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/edca-txop-n.h"
#include "ns3/config.h"
#include "ns3/boolean.h"

using namespace ns3;

class PowerRateAdaptationTest : public TestCase
{
public:
  PowerRateAdaptationTest ();

  virtual void DoRun (void);
private:
  void TestParf ();
  void TestAparf ();
  Ptr<Node> ConfigureNode ();

  ObjectFactory m_manager;
};

PowerRateAdaptationTest::PowerRateAdaptationTest ()
  : TestCase ("PowerRateAdaptation")
{
}

Ptr<Node>
PowerRateAdaptationTest::ConfigureNode ()
{
  /*
   * Create channel model. Is is necessary to configure correctly the phy layer.
   */
  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();

  /*
   * Create mac layer. We use Adhoc because association is not needed to get supported rates.
   */
  Ptr<AdhocWifiMac> mac = CreateObject<AdhocWifiMac> ();
  mac->ConfigureStandard (WIFI_PHY_STANDARD_80211a);

  /*
   * Create mobility model. Is needed by the phy layer for transmission.
   */
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel> ();

  /*
   * Create and configure phy layer.
   */
  Ptr<WifiNetDevice> dev = CreateObject<WifiNetDevice> ();
  Ptr<YansWifiPhy> phy = CreateObject<YansWifiPhy> ();
  phy->SetChannel (channel);
  phy->SetDevice (dev);
  phy->SetMobility (mobility);
  phy->ConfigureStandard (WIFI_PHY_STANDARD_80211a);

  /*
   * Configure power control parameters.
   */
  phy->SetNTxPower (18);
  phy->SetTxPowerStart (0);
  phy->SetTxPowerEnd (17);

  /*
   * Create manager.
   */
  Ptr<WifiRemoteStationManager> manager = m_manager.Create<WifiRemoteStationManager> ();

  /*
   * Create and configure node. Add mac and phy layer and the manager.
   */
  Ptr<Node> node = CreateObject<Node> ();
  mac->SetAddress (Mac48Address::Allocate ());
  dev->SetMac (mac);
  dev->SetPhy (phy);
  dev->SetRemoteStationManager (manager);
  node->AddDevice (dev);

  return node;
}

void
PowerRateAdaptationTest::TestParf ()
{
  m_manager.SetTypeId ("ns3::ParfWifiManager");
  Ptr<Node> node = ConfigureNode ();
  Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice> (node->GetDevice (0));
  Ptr<WifiRemoteStationManager> manager = dev->GetRemoteStationManager ();

  /*
   * Configure thresholds for rate and power control.
   */
  manager->SetAttribute ("AttemptThreshold",UintegerValue (15));
  manager->SetAttribute ("SuccessThreshold",UintegerValue (10));

  /*
   * Create a dummy packet to simulate transmission.
   */
  Mac48Address remoteAddress = Mac48Address::Allocate ();
  WifiMacHeader packetHeader;
  packetHeader.SetTypeData ();
  packetHeader.SetQosTid (0);
  Ptr<Packet> packet = Create<Packet> (10);
  WifiMode ackMode;

  /*
   * To initialize the manager we need to generate a transmission.
   */
  Ptr<Packet> p = Create<Packet> ();
  dev->Send (p, remoteAddress, 1);

  //-----------------------------------------------------------------------------------------------------

  /*
   * Parf initiates with maximal rate and power.
   */
  WifiTxVector txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  WifiMode mode = txVector.GetMode ();
  int power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Initial data rate wrong");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Initial power level wrong");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After 10 consecutive successful transmissions parf increase rate or decrease power.
   * As we are at maximal rate, the power should be decreased. recoveryPower=true.
   */
  for (int i = 0; i < 10; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 16, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are using recovery power, one failure make power increase.
   *
   */
  manager->ReportDataFailed (remoteAddress, &packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After 15 transmissions attempts parf increase rate or decrease power.
   * As we are at maximal rate, the power should be decreased. recoveryPower=true.
   */
  for (int i = 0; i < 7; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
      manager->ReportDataFailed (remoteAddress, &packetHeader);
    }
  manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 16, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are using recovery power, one failure make power increase. recoveryPower=false.
   */

  manager->ReportDataFailed (remoteAddress, &packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After two consecutive fails the rate is decreased or the power increased.
   * As we are at maximal power, the rate should be decreased.
   */
  manager->ReportDataFailed (remoteAddress, &packetHeader);
  manager->ReportDataFailed (remoteAddress, &packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 48000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After 10 consecutive successful transmissions parf increase rate or decrease power.
   * As we are not at maximal rate, the rate is increased again. recoveryRate=true.
   */
  for (int i = 0; i < 10; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are using recovery rate, one failure make rate decrease. recoveryRate=false.
   */

  manager->ReportDataFailed (remoteAddress, &packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 48000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After 10 consecutive successful transmissions parf increase rate or decrease power.
   * As we are not at maximal rate, the rate is increased again. recoveryRate=true.
   */
  for (int i = 0; i < 10; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After 10 consecutive successful transmissions parf increase rate or decrease power.
   * As we are at maximal rate, the power is decreased. recoveryRate=false, recoveryPower=true.
   */
  for (int i = 0; i < 10; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 16, "PARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * One successful transmissions after a power decrease make recoverPower=false.
   * So we need two consecutive failures to increase power again.
   */
  manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);

  for (int i = 0; i < 2; i++)
    {
      manager->ReportDataFailed (remoteAddress,&packetHeader);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "PARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "PARF: Incorrect value of power level");

}

void
PowerRateAdaptationTest::TestAparf ()
{
  m_manager.SetTypeId ("ns3::AparfWifiManager");
  Ptr<Node> node = ConfigureNode ();
  Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice> (node->GetDevice (0));
  Ptr<WifiRemoteStationManager> manager = dev->GetRemoteStationManager ();

  /*
   * Configure thresholds for rate and power control.
   */
  manager->SetAttribute ("SuccessThreshold1",UintegerValue (3));
  manager->SetAttribute ("SuccessThreshold2",UintegerValue (10));
  manager->SetAttribute ("FailThreshold",UintegerValue (1));
  manager->SetAttribute ("PowerThreshold",UintegerValue (10));

  /*
   * Create a dummy packet to simulate transmission.
   */
  Mac48Address remoteAddress = Mac48Address::Allocate ();
  WifiMacHeader packetHeader;
  packetHeader.SetTypeData ();
  packetHeader.SetQosTid (0);
  Ptr<Packet> packet = Create<Packet> (10);
  WifiMode ackMode;

  /*
   * To initialize the manager we need to generate a transmission.
   */
  Ptr<Packet> p = Create<Packet> ();
  dev->Send (p, remoteAddress, 1);

  //-----------------------------------------------------------------------------------------------------

  /*
   * Aparf initiates with maximal rate and power.
   */
  WifiTxVector txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  WifiMode mode = txVector.GetMode ();
  int power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Initial data rate wrong");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "APARF: Initial power level wrong");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As Aparf starts in state High, after 3 consecutive successful transmissions aparf increase rate or decrease power.
   * As we are at maximal rate, the power should be decreased.
   * Change to state Spread.
   */
  for (int i = 0; i < 3; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 16, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * One failure make the power to be increased again.
   * Change to state Low.
   */
  manager->ReportDataFailed (remoteAddress, &packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are in state Low we need 10 successful transmissions to increase rate or decrease power.
   * As we are at maximal rate, the power should be decreased.
   * Change to state Spread.
   */
  for (int i = 0; i < 10; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 16, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * One more successful transmission make to change to state High.
   * Two more successful transmissions make power decrease.
   */

  for (int i = 0; i < 3; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 15, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are in state High we need 3 successful transmissions to increase rate or decrease power.
   * After 16*3 successful transmissions power is decreased to zero.
   */
  for (int i = 0; i < 16 * 3; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 0, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After one fail the rate is decreased or the power increased.
   * As we are at minimal power, the power should be increased.
   */
  manager->ReportDataFailed (remoteAddress,&packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 1, "Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After one fail the rate is decreased or the power increased.
   * After 16 failed transmissions power is increase to 17.
   */
  for (int i = 0; i < 16; i++)
    {
      manager->ReportDataFailed (remoteAddress,&packetHeader);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * After one fail the rate is decreased or the power increased.
   * As we are at maximal power, the rate should be decreased.
   * Set critical rate to 54 Mbps.
   */
  manager->ReportDataFailed (remoteAddress,&packetHeader);

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 48000000, "Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are in state High we need 3 successful transmissions to increase rate or decrease power.
   * As rate critical is set, after 3 successful transmissions power is decreased.
   */
  for (int i = 0; i < 3; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 48000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 16, "APARF: Incorrect value of power level");

  //-----------------------------------------------------------------------------------------------------

  /*
   * As we are in state High we need 3 successful transmissions to increase rate or decrease power.
   * After 10 power changes critical rate is reseted.
   * So after 10*3 successful transmissions critical rate is set to 0.
   * And 3 successful transmissions more will make power increase to maximum and rate increase to the critical rate.
   */
  for (int i = 0; i < 9 * 3; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 48000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 7, "APARF: Incorrect value of power level");

  for (int i = 0; i < 3; i++)
    {
      manager->ReportDataOk (remoteAddress, &packetHeader, 0, ackMode, 0);
    }

  txVector = manager->GetDataTxVector (remoteAddress, &packetHeader, packet);
  mode = txVector.GetMode ();
  power = (int) txVector.GetTxPowerLevel ();

  NS_TEST_ASSERT_MSG_EQ (mode.GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1), 54000000, "APARF: Incorrect vale of data rate");
  NS_TEST_ASSERT_MSG_EQ (power, 17, "APARF: Incorrect value of power level");

}

void
PowerRateAdaptationTest::DoRun (void)
{

  TestParf ();
  TestAparf ();
}

//-----------------------------------------------------------------------------
class PowerRateAdaptationTestSuite : public TestSuite
{
public:
  PowerRateAdaptationTestSuite ();
};

PowerRateAdaptationTestSuite::PowerRateAdaptationTestSuite ()
  : TestSuite ("power-rate-adaptation-wifi", UNIT)
{
  AddTestCase (new PowerRateAdaptationTest, TestCase::QUICK);
}

static PowerRateAdaptationTestSuite g_powerRateAdaptationTestSuite;
