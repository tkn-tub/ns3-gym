/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 *               2010      NICTA
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
 *         Quincy Tse <quincy.tse@nicta.com.au> (Case for Bug 991)
 */

#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/adhoc-wifi-mac.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/arf-wifi-manager.h"
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

namespace ns3 {

class WifiTest : public TestCase
{
public:
  WifiTest ();

  virtual void DoRun (void);
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
{
}

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

void
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
  m_mac.SetTypeId ("ns3::ApWifiMac");
  RunOne ();
  m_mac.SetTypeId ("ns3::StaWifiMac");
  RunOne ();


  m_propDelay.SetTypeId ("ns3::RandomPropagationDelayModel");
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  RunOne ();
  Simulator::Destroy ();
}

//-----------------------------------------------------------------------------
class QosUtilsIsOldPacketTest : public TestCase
{
public:
  QosUtilsIsOldPacketTest () : TestCase ("QosUtilsIsOldPacket")
  {
  }
  virtual void DoRun (void)
  {
    // startingSeq=0, seqNum=2047
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (0, 2047), false, "2047 is new in comparison to 0");
    // startingSeq=0, seqNum=2048
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (0, 2048), true, "2048 is old in comparison to 0");
    // startingSeq=2048, seqNum=0
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (2048, 0), true, "0 is old in comparison to 2048");
    // startingSeq=4095, seqNum=0
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (4095, 0), false, "0 is new in comparison to 4095");
    // startingSeq=0, seqNum=4095
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (0, 4095), true, "4095 is old in comparison to 0");
    // startingSeq=4095 seqNum=2047
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (4095, 2047), true, "2047 is old in comparison to 4095");
    // startingSeq=2048 seqNum=4095
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (2048, 4095), false, "4095 is new in comparison to 2048");
    // startingSeq=2049 seqNum=0
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (2049, 0), false, "0 is new in comparison to 2049");
  }
};

//-----------------------------------------------------------------------------
class InterferenceHelperSequenceTest : public TestCase
{
public:
  InterferenceHelperSequenceTest ();

  virtual void DoRun (void);
private:
  Ptr<Node> CreateOne (Vector pos, Ptr<YansWifiChannel> channel);
  void SendOnePacket (Ptr<WifiNetDevice> dev);
  void SwitchCh (Ptr<WifiNetDevice> dev);

  ObjectFactory m_manager;
  ObjectFactory m_mac;
  ObjectFactory m_propDelay;
};

InterferenceHelperSequenceTest::InterferenceHelperSequenceTest ()
  : TestCase ("InterferenceHelperSequence")
{
}

void
InterferenceHelperSequenceTest::SendOnePacket (Ptr<WifiNetDevice> dev)
{
  Ptr<Packet> p = Create<Packet> (9999);
  dev->Send (p, dev->GetBroadcast (), 1);
}

void
InterferenceHelperSequenceTest::SwitchCh (Ptr<WifiNetDevice> dev)
{
  Ptr<WifiPhy> p = dev->GetPhy ();
  p->SetChannelNumber (1);
}

Ptr<Node>
InterferenceHelperSequenceTest::CreateOne (Vector pos, Ptr<YansWifiChannel> channel)
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

  return node;
}

void
InterferenceHelperSequenceTest::DoRun (void)
{
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  m_propDelay.SetTypeId ("ns3::ConstantSpeedPropagationDelayModel");
  m_manager.SetTypeId ("ns3::ConstantRateWifiManager");

  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  Ptr<PropagationDelayModel> propDelay = m_propDelay.Create<PropagationDelayModel> ();
  Ptr<MatrixPropagationLossModel> propLoss = CreateObject<MatrixPropagationLossModel> ();
  channel->SetPropagationDelayModel (propDelay);
  channel->SetPropagationLossModel (propLoss);

  Ptr<Node> rxOnly = CreateOne (Vector (0.0, 0.0, 0.0), channel);
  Ptr<Node> senderA = CreateOne (Vector (5.0, 0.0, 0.0), channel);
  Ptr<Node> senderB = CreateOne (Vector (-5.0, 0.0, 0.0), channel);

  propLoss->SetLoss (senderB->GetObject<MobilityModel> (), rxOnly->GetObject<MobilityModel> (), 0, true);
  propLoss->SetDefaultLoss (999);

  Simulator::Schedule (Seconds (1.0),
                       &InterferenceHelperSequenceTest::SendOnePacket, this,
                       DynamicCast<WifiNetDevice> (senderB->GetDevice (0)));

  Simulator::Schedule (Seconds (1.0000001),
                       &InterferenceHelperSequenceTest::SwitchCh, this,
                       DynamicCast<WifiNetDevice> (rxOnly->GetDevice (0)));

  Simulator::Schedule (Seconds (5.0),
                       &InterferenceHelperSequenceTest::SendOnePacket, this,
                       DynamicCast<WifiNetDevice> (senderA->GetDevice (0)));

  Simulator::Schedule (Seconds (7.0),
                       &InterferenceHelperSequenceTest::SendOnePacket, this,
                       DynamicCast<WifiNetDevice> (senderB->GetDevice (0)));

  Simulator::Stop (Seconds (100.0));
  Simulator::Run ();

  Simulator::Destroy ();
}

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
  AddTestCase (new QosUtilsIsOldPacketTest);
  AddTestCase (new InterferenceHelperSequenceTest); // Bug 991
}

static WifiTestSuite g_wifiTestSuite;

} // namespace ns3
