/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "wifi-net-device.h"
#include "yans-wifi-channel.h"
#include "adhoc-wifi-mac.h"
#include "yans-wifi-phy.h"
#include "arf-wifi-manager.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "error-rate-model.h"
#include "yans-error-rate-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/object-factory.h"
#include "dca-txop.h"
#include "mac-rx-middle.h"
#include "ns3/pointer.h"

namespace ns3 {

class WifiTest : public TestCase
{
public:
  WifiTest ();

  virtual bool DoRun (void);
private:
  void RunOne (void);
  void CreateOne (Vector pos, Ptr<YansWifiChannel> channel);
  void SendOnePacket (Ptr<WifiNetDevice> dev);

  ObjectFactory m_manager;
  ObjectFactory m_mac;
  ObjectFactory m_propDelay;
};

WifiTest::WifiTest ()
  : TestCase ("Wifi")
{}

void 
WifiTest::SendOnePacket (Ptr<WifiNetDevice> dev)
{
  Ptr<Packet> p = Create<Packet> ();
  dev->Send (p, dev->GetBroadcast (), 1);
}

void 
WifiTest::CreateOne (Vector pos, Ptr<YansWifiChannel> channel)
{
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<WifiNetDevice> dev = CreateObject<WifiNetDevice> ();

  Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
  mac->ConfigureStandard (WIFI_PHY_STANDARD_80211a);
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel> ();
  Ptr<YansWifiPhy> phy = CreateObject<YansWifiPhy> ();
  Ptr<ErrorRateModel> error = CreateObject<YansErrorRateModel> ();
  phy->SetErrorRateModel (error);
  phy->SetChannel (channel);
  phy->SetDevice (dev);
  phy->SetMobility (node);
  phy->ConfigureStandard (WIFI_PHY_STANDARD_80211a);
  Ptr<WifiRemoteStationManager> manager = m_manager.Create<WifiRemoteStationManager> ();

  mobility->SetPosition (pos);
  node->AggregateObject (mobility);
  mac->SetAddress (Mac48Address::Allocate ());
  dev->SetMac (mac);
  dev->SetPhy (phy);
  dev->SetRemoteStationManager (manager);
  node->AddDevice (dev);

  Simulator::Schedule (Seconds (1.0), &WifiTest::SendOnePacket, this, dev);
}

void
WifiTest::RunOne (void)
{
  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  Ptr<PropagationDelayModel> propDelay = m_propDelay.Create<PropagationDelayModel> ();
  Ptr<PropagationLossModel> propLoss = CreateObject<RandomPropagationLossModel> ();
  channel->SetPropagationDelayModel (propDelay);
  channel->SetPropagationLossModel (propLoss);

  CreateOne (Vector (0.0, 0.0, 0.0), channel);
  CreateOne (Vector (5.0, 0.0, 0.0), channel);
  CreateOne (Vector (5.0, 0.0, 0.0), channel);

  Simulator::Run ();
  Simulator::Destroy ();

  Simulator::Stop (Seconds (10.0));
}

bool
WifiTest::DoRun (void)
{
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  m_propDelay.SetTypeId ("ns3::ConstantSpeedPropagationDelayModel");
  
  m_manager.SetTypeId ("ns3::ArfWifiManager");
  RunOne ();
  m_manager.SetTypeId ("ns3::AarfWifiManager");
  RunOne ();
  m_manager.SetTypeId ("ns3::ConstantRateWifiManager");
  RunOne ();
  m_manager.SetTypeId ("ns3::OnoeWifiManager");
  RunOne ();
  m_manager.SetTypeId ("ns3::AmrrWifiManager");
  RunOne ();
  m_manager.SetTypeId ("ns3::IdealWifiManager");
  RunOne ();

  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  RunOne ();
  m_mac.SetTypeId ("ns3::NqapWifiMac");
  RunOne ();
  m_mac.SetTypeId ("ns3::NqstaWifiMac");
  RunOne ();


  m_propDelay.SetTypeId ("ns3::RandomPropagationDelayModel");
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  RunOne ();
  Simulator::Destroy ();
  return false;
}

//-----------------------------------------------------------------------------
class MacRxMiddleTest : public TestCase
{
public:
  MacRxMiddleTest () : TestCase ("MacRxMiddle") {}
  virtual bool DoRun (void) 
  {
    MacRxMiddle middle;
    // 0 < 1
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (0 << 4, 1 << 4), true, "0 < 1");
    // 0 < 2047
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (0 << 4, 2047 << 4), true, "0 < 2047");
    // 0 > 2048
    NS_TEST_EXPECT_MSG_EQ (!middle.SequenceControlSmaller (0 << 4, 2048 << 4), true, "0 > 2048");
    // 0 > 2049
    NS_TEST_EXPECT_MSG_EQ (!middle.SequenceControlSmaller (0 << 4, 2049 << 4), true, "0 > 2049");
    // 0 > 4095
    NS_TEST_EXPECT_MSG_EQ (!middle.SequenceControlSmaller (0 << 4, 4095 << 4), true, "0 > 4095");

    // 1 > 0
    NS_TEST_EXPECT_MSG_EQ (!middle.SequenceControlSmaller (1 << 4, 0 << 4), true, "1 > 0");
    // 2047 > 0
    NS_TEST_EXPECT_MSG_EQ (!middle.SequenceControlSmaller (2047 << 4, 0 << 4), true, "2047 > 0");
    // 2048 < 0
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (2048 << 4, 0 << 4), true, "2048 < 0");
    // 2049 < 0
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (2049 << 4, 0 << 4), true, "2049 < 0");
    // 4095 < 0 
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (4095 << 4, 0 << 4), true, "4095 < 0");

    // 2048 < 2049
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (2048 << 4, 2049 << 4), true, "2048 < 2049");
    // 2048 < 4095
    NS_TEST_EXPECT_MSG_EQ (middle.SequenceControlSmaller (2048 << 4, 4095 << 4), true, "2048 < 4095");
    // 2047 > 4095
    NS_TEST_EXPECT_MSG_EQ (!middle.SequenceControlSmaller (2047 << 4, 4095 << 4), true, "2047 > 4095");

    return GetErrorStatus ();
  }
};

//-----------------------------------------------------------------------------

class WifiTestSuite : public TestSuite
{
public:
  WifiTestSuite ();
};

WifiTestSuite::WifiTestSuite ()
  : TestSuite ("devices-wifi", UNIT)
{
  AddTestCase (new WifiTest);
  AddTestCase (new MacRxMiddleTest);
}

WifiTestSuite g_wifiTestSuite;

} // namespace ns3
