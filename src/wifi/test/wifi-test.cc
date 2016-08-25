/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Quincy Tse <quincy.tse@nicta.com.au>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/adhoc-wifi-mac.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/yans-error-rate-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/test.h"
#include "ns3/pointer.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/config.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet-socket-server.h"
#include "ns3/packet-socket-client.h"
#include "ns3/packet-socket-helper.h"

using namespace ns3;

//Helper function to assign streams to random variables, to control
//randomness in the tests
static void
AssignWifiRandomStreams (Ptr<WifiMac> mac, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<RegularWifiMac> rmac = DynamicCast<RegularWifiMac> (mac);
  if (rmac)
    {
      PointerValue ptr;
      rmac->GetAttribute ("DcaTxop", ptr);
      Ptr<DcaTxop> dcaTxop = ptr.Get<DcaTxop> ();
      currentStream += dcaTxop->AssignStreams (currentStream);

      rmac->GetAttribute ("VO_EdcaTxopN", ptr);
      Ptr<EdcaTxopN> vo_edcaTxopN = ptr.Get<EdcaTxopN> ();
      currentStream += vo_edcaTxopN->AssignStreams (currentStream);

      rmac->GetAttribute ("VI_EdcaTxopN", ptr);
      Ptr<EdcaTxopN> vi_edcaTxopN = ptr.Get<EdcaTxopN> ();
      currentStream += vi_edcaTxopN->AssignStreams (currentStream);

      rmac->GetAttribute ("BE_EdcaTxopN", ptr);
      Ptr<EdcaTxopN> be_edcaTxopN = ptr.Get<EdcaTxopN> ();
      currentStream += be_edcaTxopN->AssignStreams (currentStream);

      rmac->GetAttribute ("BK_EdcaTxopN", ptr);
      Ptr<EdcaTxopN> bk_edcaTxopN = ptr.Get<EdcaTxopN> ();
      currentStream += bk_edcaTxopN->AssignStreams (currentStream);
    }
}


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

  Simulator::Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
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
    //startingSeq=0, seqNum=2047
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (0, 2047), false, "2047 is new in comparison to 0");
    //startingSeq=0, seqNum=2048
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (0, 2048), true, "2048 is old in comparison to 0");
    //startingSeq=2048, seqNum=0
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (2048, 0), true, "0 is old in comparison to 2048");
    //startingSeq=4095, seqNum=0
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (4095, 0), false, "0 is new in comparison to 4095");
    //startingSeq=0, seqNum=4095
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (0, 4095), true, "4095 is old in comparison to 0");
    //startingSeq=4095 seqNum=2047
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (4095, 2047), true, "2047 is old in comparison to 4095");
    //startingSeq=2048 seqNum=4095
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (2048, 4095), false, "4095 is new in comparison to 2048");
    //startingSeq=2049 seqNum=0
    NS_TEST_EXPECT_MSG_EQ (QosUtilsIsOldPacket (2049, 0), false, "0 is new in comparison to 2049");
  }
};


//-----------------------------------------------------------------------------
/**
 * See \bugid{991}
 */
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
  phy->SetMobility (mobility);
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
/**
 * Make sure that when multiple broadcast packets are queued on the same
 * device in a short succession no virtual collision occurs
 *
 * The observed behavior is that the first frame will be sent immediately,
 * and the frames are spaced by (backoff + SIFS + AIFS) time intervals
 * (where backoff is a random number of slot sizes up to maximum CW)
 *
 * The following test case should _not_ generate virtual collision for the second frame.
 * The seed and run numbers were pick such that the second frame gets backoff = 0.
 *
 *                      frame 1, frame 2
 *                      arrive                SIFS + AIFS
 *                      V                    <----------->
 * time  |--------------|-------------------|-------------|----------->
 *       0              1s                  1.001408s     1.001442s
 *                      ^                   ^             ^
 *                      start TX            finish TX     start TX
 *                      frame 1             frame 1       frame 2
 *                      ^
 *                      frame 2
 *                      backoff = 0
 *
 * The buggy behavior was that frame 2 experiences a virtual collision and re-selects the
 * backoff again. As a result, the _actual_ backoff experience by frame 2 is less likely to be 0
 * since that would require two successions of 0 backoff (one that generates the virtual collision and
 * one after the virtual collision).
 *
 * See \bugid{555}
 */

class Bug555TestCase : public TestCase
{
public:
  Bug555TestCase ();

  virtual void DoRun (void);


private:
  void SendOnePacket (Ptr<WifiNetDevice> dev);

  ObjectFactory m_manager;
  ObjectFactory m_mac;
  ObjectFactory m_propDelay;

  Time m_firstTransmissionTime;
  Time m_secondTransmissionTime;
  unsigned int m_numSentPackets;

  void NotifyPhyTxBegin (Ptr<const Packet> p);
};

Bug555TestCase::Bug555TestCase ()
  : TestCase ("Test case for Bug 555")
{
}

void
Bug555TestCase::NotifyPhyTxBegin (Ptr<const Packet> p)
{
  if (m_numSentPackets == 0)
    {
      NS_ASSERT_MSG (Simulator::Now () == Time (Seconds (1)), "Packet 0 not transmitted at 1 second");
      m_numSentPackets++;
      m_firstTransmissionTime = Simulator::Now ();
    }
  else if (m_numSentPackets == 1)
    {
      m_secondTransmissionTime = Simulator::Now ();
    }
}

void
Bug555TestCase::SendOnePacket (Ptr<WifiNetDevice> dev)
{
  Ptr<Packet> p = Create<Packet> (1000);
  dev->Send (p, dev->GetBroadcast (), 1);
}

void
Bug555TestCase::DoRun (void)
{
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  m_propDelay.SetTypeId ("ns3::ConstantSpeedPropagationDelayModel");
  m_manager.SetTypeId ("ns3::ConstantRateWifiManager");

  //Assign a seed and run number, and later fix the assignment of streams to
  //WiFi random variables, so that the first backoff used is zero slots
  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (17);

  //Disable the initial jitter of AP beacons (test case was written before
  //beacon jitter was added)
  Config::SetDefault ("ns3::ApWifiMac::EnableBeaconJitter", BooleanValue (false));

  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  Ptr<PropagationDelayModel> propDelay = m_propDelay.Create<PropagationDelayModel> ();
  Ptr<PropagationLossModel> propLoss = CreateObject<RandomPropagationLossModel> ();
  channel->SetPropagationDelayModel (propDelay);
  channel->SetPropagationLossModel (propLoss);

  Ptr<Node> txNode = CreateObject<Node> ();
  Ptr<WifiNetDevice> txDev = CreateObject<WifiNetDevice> ();
  Ptr<WifiMac> txMac = m_mac.Create<WifiMac> ();
  txMac->ConfigureStandard (WIFI_PHY_STANDARD_80211a);
  //Fix the stream assignment to the Dcf Txop objects (backoffs)
  //The below stream assignment will result in the DcaTxop object
  //using a backoff value of zero for this test when the
  //DcaTxop::EndTxNoAck() calls to StartBackoffNow()
  AssignWifiRandomStreams (txMac, 23);

  Ptr<ConstantPositionMobilityModel> txMobility = CreateObject<ConstantPositionMobilityModel> ();
  Ptr<YansWifiPhy> txPhy = CreateObject<YansWifiPhy> ();
  Ptr<ErrorRateModel> txError = CreateObject<YansErrorRateModel> ();
  txPhy->SetErrorRateModel (txError);
  txPhy->SetChannel (channel);
  txPhy->SetDevice (txDev);
  txPhy->SetMobility (txMobility);
  txPhy->ConfigureStandard (WIFI_PHY_STANDARD_80211a);

  txPhy->TraceConnectWithoutContext ("PhyTxBegin", MakeCallback (&Bug555TestCase::NotifyPhyTxBegin, this));

  txMobility->SetPosition (Vector (0.0, 0.0, 0.0));
  txNode->AggregateObject (txMobility);
  txMac->SetAddress (Mac48Address::Allocate ());
  txDev->SetMac (txMac);
  txDev->SetPhy (txPhy);
  txDev->SetRemoteStationManager (m_manager.Create<WifiRemoteStationManager> ());
  txNode->AddDevice (txDev);

  m_firstTransmissionTime = Seconds (0.0);
  m_secondTransmissionTime = Seconds (0.0);
  m_numSentPackets = 0;

  Simulator::Schedule (Seconds (1.0), &Bug555TestCase::SendOnePacket, this, txDev);
  Simulator::Schedule (Seconds (1.0), &Bug555TestCase::SendOnePacket, this, txDev);

  Simulator::Stop (Seconds (2.0));
  Simulator::Run ();
  Simulator::Destroy ();

  //First packet has 1408 us of transmit time.   Slot time is 9 us.
  //Backoff is 0 slots.  SIFS is 16 us.  AIFS is 2 slots = 18 us.
  //Should send next packet at 1408 us + (0 * 9 us) + 16 us + 18 us
  //1442 us after the first one.
  uint32_t expectedWait1 = 1408 + (0 * 9) + 16 + 18;
  Time expectedSecondTransmissionTime = MicroSeconds (expectedWait1) + Seconds (1.0);

  NS_TEST_ASSERT_MSG_EQ (m_secondTransmissionTime, expectedSecondTransmissionTime, "The second transmission time not correct!");
}


//-----------------------------------------------------------------------------
/**
 * Make sure that when changing the fragmentation threshold during the simulation,
 * the TCP transmission does not unexpectedly stop.
 *
 * The scenario considers a TCP transmission between a 802.11b station and a 802.11b
 * access point. After the simulation has begun, the fragmentation threshold is set at
 * a value lower than the packet size. It then checks whether the TCP transmission
 * continues after the fragmentation threshold modification.
 *
 * See \bugid{730}
 */

class Bug730TestCase : public TestCase
{
public:
  Bug730TestCase ();
  virtual ~Bug730TestCase ();

  virtual void DoRun (void);


private:
  uint32_t m_received;

  void Receive (std::string context, Ptr<const Packet> p, const Address &adr);

};

Bug730TestCase::Bug730TestCase ()
  : TestCase ("Test case for Bug 730"),
    m_received (0)
{
}

Bug730TestCase::~Bug730TestCase ()
{
}

void
Bug730TestCase::Receive (std::string context, Ptr<const Packet> p, const Address &adr)
{
  if ((p->GetSize () == 1460) && (Simulator::Now () > Seconds (20)))
    {
      m_received++;
    }
}


void
Bug730TestCase::DoRun (void)
{
  m_received = 0;

  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2304"));

  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);

  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("DsssRate1Mbps"),
                                "ControlMode", StringValue ("DsssRate1Mbps"));

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  Ptr<WifiNetDevice> ap_device = DynamicCast<WifiNetDevice> (apDevices.Get (0));
  Ptr<WifiNetDevice> sta_device = DynamicCast<WifiNetDevice> (staDevices.Get (0));

  PacketSocketAddress socket;
  socket.SetSingleDevice (sta_device->GetIfIndex ());
  socket.SetPhysicalAddress (ap_device->GetAddress ());
  socket.SetProtocol (1);

  // give packet socket powers to nodes.
  PacketSocketHelper packetSocket;
  packetSocket.Install (wifiStaNode);
  packetSocket.Install (wifiApNode);

  Ptr<PacketSocketClient> client = CreateObject<PacketSocketClient> ();
  client->SetAttribute ("PacketSize", UintegerValue (1460));
  client->SetRemote (socket);
  wifiStaNode.Get(0)->AddApplication (client);
  client->SetStartTime (Seconds (1));
  client->SetStopTime (Seconds (51.0));

  Ptr<PacketSocketServer> server = CreateObject<PacketSocketServer> ();
  server->SetLocal (socket);
  wifiApNode.Get(0)->AddApplication (server);
  server->SetStartTime (Seconds (0.0));
  server->SetStopTime (Seconds (52.0));

  Config::Connect ("/NodeList/*/ApplicationList/0/$ns3::PacketSocketServer/Rx", MakeCallback (&Bug730TestCase::Receive, this));

  Simulator::Schedule (Seconds (10.0), Config::Set, "/NodeList/0/DeviceList/0/RemoteStationManager/FragmentationThreshold", StringValue ("800"));

  Simulator::Stop (Seconds (55));
  Simulator::Run ();

  Simulator::Destroy ();

  bool result = (m_received > 0);
  NS_TEST_ASSERT_MSG_EQ (result, true, "packet reception unexpectedly stopped after adapting fragmentation threshold!");
}

class SetChannelFrequencyTest: public TestCase
{
public:
  SetChannelFrequencyTest ();

  virtual void DoRun (void);


private:

  Ptr<YansWifiPhy> GetYansWifiPhyPtr (const NetDeviceContainer &nc) const;

};

SetChannelFrequencyTest::SetChannelFrequencyTest ()
  : TestCase ("Test case for setting WifiPhy channel and frequency")
{
}

Ptr<YansWifiPhy>
SetChannelFrequencyTest::GetYansWifiPhyPtr (const NetDeviceContainer &nc) const
{
  Ptr<WifiNetDevice> wnd = nc.Get (0)->GetObject<WifiNetDevice> ();
  Ptr<WifiPhy> wp = wnd->GetPhy ();
  return wp->GetObject<YansWifiPhy> ();
}

void
SetChannelFrequencyTest::DoRun ()
{
  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  // Configure and declare other generic components of this example
  Ssid ssid;
  ssid = Ssid ("wifi-phy-configuration");
  WifiMacHelper macSta;
  macSta.SetType ("ns3::StaWifiMac",
                  "Ssid", SsidValue (ssid),
                  "ActiveProbing", BooleanValue (false));
  NetDeviceContainer staDevice;
  Ptr<YansWifiPhy> phySta;

  // Cases taken from src/wifi/examples/wifi-phy-configuration.cc example
  {
      // case 0
      // Default configuration, without WifiHelper::SetStandard or WifiHelper
      phySta = CreateObject<YansWifiPhy> ();
      // The default results in an invalid configuration of channel 0,
      // width 20, and frequency 0 MHz
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 0, "default configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "default configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 0, "default configuration");
  }
  {
      // case 1
      WifiHelper wifi;
      // By default, WifiHelper will use WIFI_PHY_STANDARD_80211a
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // We expect channel 36, width 20, frequency 5180
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "default configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "default configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "default configuration");
  }
  {
      // case 2
      WifiHelper wifi;
      wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // We expect channel 1, width 22, frequency 2412
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 1, "802.11b configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 22, "802.11b configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 2412, "802.11b configuration");
  }
  {
      // case 3
      WifiHelper wifi;
      wifi.SetStandard (WIFI_PHY_STANDARD_80211g);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // We expect channel 1, width 20, frequency 2412
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 1, "802.11g configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11g configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 2412, "802.11g configuration");
  }
  {
      // case 4
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "802.11n-5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11n-5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "802.11n-5GHz configuration");
  }
  {
      // case 5
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 1, "802.11n-2.4GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11n-2.4GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 2412, "802.11n-2.4GHz configuration");
  }
  {
      // case 6
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 42, "802.11ac configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 80, "802.11ac configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5210, "802.11ac configuration");
  }
  {
      // case 7
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211_10MHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 172, "802.11 10Mhz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 10, "802.11 10Mhz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5860, "802.11 10Mhz configuration");
  }
  {
      // case 8
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211_5MHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 0, "802.11 5Mhz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 5, "802.11 5Mhz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5860, "802.11 5Mhz configuration");
  }
  {
      // case 9
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_holland);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // We expect channel 36, width 20, frequency 5180
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "802.11 5Mhz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5Mhz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "802.11 5Mhz configuration");
  }
  {
      // case 10
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      phy.Set ("ChannelNumber", UintegerValue(44));
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 44, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5220, "802.11 5GHz configuration");
  }
  {
      // case 11
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      phy.Set ("ChannelNumber", UintegerValue(44));
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // Post-install reconfiguration to channel number 40
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/ChannelNumber", UintegerValue(40));
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 40, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5200, "802.11 5GHz configuration");
  }
  {
      // case 12
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      phy.Set ("ChannelNumber", UintegerValue (44));
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // Post-install reconfiguration to channel width 40 MHz
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/ChannelWidth", UintegerValue(40));
      // Although channel 44 is configured originally for 20 MHz, we
      // allow it to be used for 40 MHz here
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 44, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 40, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5220, "802.11 5GHz configuration");
  }
      // modify cases 13 and 14 to avoid Config::SetDefault ()
  {
      // case 13
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      phySta->SetAttribute ("ChannelNumber", UintegerValue (44));
      // Post-install reconfiguration to channel width 40 MHz
      Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/ChannelWidth", UintegerValue(40));
      // Although channel 44 is configured originally for 20 MHz, we
      // allow it to be used for 40 MHz here
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 44, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 40, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5220, "802.11 5GHz configuration");
  }
  {
      // case 14
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      // Test that setting Frequency to a non-standard value will zero the
      // channel number
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      phySta->SetAttribute ("Frequency", UintegerValue (5281));
      // We expect channel number to be zero since frequency doesn't match
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 0, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5281, "802.11 5GHz configuration");
  }
  {
      // case 15:
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // Test that setting Frequency to a standard value will set the
      // channel number correctly
      phySta->SetAttribute ("Frequency", UintegerValue (5500));
      // We expect channel number to be 100 due to frequency 5500
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 100, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5500, "802.11 5GHz configuration");
  }
  {
      // case 16:
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
      phySta = GetYansWifiPhyPtr (staDevice);
      // This case will error exit due to invalid channel number unless
      // we provide the DefineChannelNumber() below
      phySta->DefineChannelNumber (99, WIFI_PHY_STANDARD_80211n_5GHZ, 5185, 40);
      phySta->SetAttribute ("ChannelNumber", UintegerValue (99));
  }
  {
      // case 17:
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      // Test how channel number behaves when frequency is non-standard
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      phySta->SetAttribute ("Frequency", UintegerValue (5181));
      // We expect channel number to be 0 due to unknown center frequency 5181
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 0, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5181, "802.11 5GHz configuration");
      phySta->SetAttribute ("Frequency", UintegerValue (5180));
      // We expect channel number to be 36 due to known center frequency 5180
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "802.11 5GHz configuration");
      phySta->SetAttribute ("Frequency", UintegerValue (5179));
      // We expect channel number to be 0 due to unknown center frequency 5179
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 0, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5179, "802.11 5GHz configuration");
      phySta->SetAttribute ("ChannelNumber", UintegerValue (36));
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "802.11 5GHz configuration");
  }
  {
      // case 18:
      WifiHelper wifi;
      wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
      // Set both channel and frequency to consistent values
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      staDevice = wifi.Install (phy, macSta, wifiStaNode.Get(0));
      phySta = GetYansWifiPhyPtr (staDevice);
      phySta->SetAttribute ("Frequency", UintegerValue (5200));
      phySta->SetAttribute ("ChannelNumber", UintegerValue (40));
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 40, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5200, "802.11 5GHz configuration");
      // Set both channel and frequency to inconsistent values
      phySta->SetAttribute ("Frequency", UintegerValue (5200));
      phySta->SetAttribute ("ChannelNumber", UintegerValue (36));
      // We expect channel number to be 36
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "802.11 5GHz configuration");
      phySta->SetAttribute ("ChannelNumber", UintegerValue (36));
      phySta->SetAttribute ("Frequency", UintegerValue (5200));
      // We expect channel number to be 40
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 40, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5200, "802.11 5GHz configuration");
      phySta->SetAttribute ("Frequency", UintegerValue (5179));
      phySta->SetAttribute ("ChannelNumber", UintegerValue (36));
      // We expect channel number to be 36
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 36, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5180, "802.11 5GHz configuration");
      phySta->SetAttribute ("ChannelNumber", UintegerValue (36));
      phySta->SetAttribute ("Frequency", UintegerValue (5179));
      // We expect channel number to be 0
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 0, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
      NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5179, "802.11 5GHz configuration");
  }

Simulator::Destroy ();

}

//-----------------------------------------------------------------------------
/**
 * Make sure that when virtual collision occurs the wifi remote station manager 
 * is triggered and the retry counter is increased.
 *
 * See \bugid{2222}
 */

class Bug2222TestCase : public TestCase
{
public:
  Bug2222TestCase ();
  virtual ~Bug2222TestCase ();

  virtual void DoRun (void);


private:
  uint32_t m_countInternalCollisions;

  void PopulateArpCache ();
  void TxDataFailedTrace (std::string context, Mac48Address adr);
};

Bug2222TestCase::Bug2222TestCase ()
  : TestCase ("Test case for Bug 2222"),
    m_countInternalCollisions (0)
{
}

Bug2222TestCase::~Bug2222TestCase ()
{
}

void
Bug2222TestCase::TxDataFailedTrace (std::string context, Mac48Address adr)
{
  //Indicate the long retry counter has been increased in the wifi remote station manager
  m_countInternalCollisions++;
}

void
Bug2222TestCase::DoRun (void)
{
  m_countInternalCollisions = 0;
    
  //Generate same backoff for AC_VI and AC_VO
  //The below combination will work
  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (2);
  int64_t streamNumber = 100;

  NodeContainer wifiNodes;
  wifiNodes.Create (2);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::AdhocWifiMac",
               "QosSupported", BooleanValue (true));

  NetDeviceContainer wifiDevices;
  wifiDevices = wifi.Install (phy, mac, wifiNodes);

  // Assign fixed streams to random variables in use
  wifi.AssignStreams (wifiDevices, streamNumber);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiNodes);

  Ptr<WifiNetDevice> device1 = DynamicCast<WifiNetDevice> (wifiDevices.Get (0));
  Ptr<WifiNetDevice> device2 = DynamicCast<WifiNetDevice> (wifiDevices.Get (1));

  PacketSocketAddress socket;
  socket.SetSingleDevice (device1->GetIfIndex ());
  socket.SetPhysicalAddress (device2->GetAddress ());
  socket.SetProtocol (1);

  PacketSocketHelper packetSocket;
  packetSocket.Install (wifiNodes);

  Ptr<PacketSocketClient> clientLowPriority = CreateObject<PacketSocketClient> ();
  clientLowPriority->SetAttribute ("PacketSize", UintegerValue (1460));
  clientLowPriority->SetAttribute ("MaxPackets", UintegerValue (1));
  clientLowPriority->SetAttribute ("Priority", UintegerValue (4)); //AC_VI
  clientLowPriority->SetRemote (socket);
  wifiNodes.Get(0)->AddApplication (clientLowPriority);
  clientLowPriority->SetStartTime (Seconds (0.0));
  clientLowPriority->SetStopTime (Seconds (1.0));
  
  Ptr<PacketSocketClient> clientHighPriority = CreateObject<PacketSocketClient> ();
  clientHighPriority->SetAttribute ("PacketSize", UintegerValue (1460));
  clientHighPriority->SetAttribute ("MaxPackets", UintegerValue (1));
  clientHighPriority->SetAttribute ("Priority", UintegerValue (6)); //AC_VO
  clientHighPriority->SetRemote (socket);
  wifiNodes.Get(0)->AddApplication (clientHighPriority);
  clientHighPriority->SetStartTime (Seconds (0.0));
  clientHighPriority->SetStopTime (Seconds (1.0));

  Ptr<PacketSocketServer> server = CreateObject<PacketSocketServer> ();
  server->SetLocal (socket);
  wifiNodes.Get(1)->AddApplication (server);
  server->SetStartTime (Seconds (0.0));
  server->SetStopTime (Seconds (1.0));

  Config::Connect ("/NodeList/*/DeviceList/*/RemoteStationManager/MacTxDataFailed", MakeCallback (&Bug2222TestCase::TxDataFailedTrace, this));

  Simulator::Stop (Seconds (1.0));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_TEST_ASSERT_MSG_EQ (m_countInternalCollisions, 1, "unexpected number of internal collisions!");
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
  AddTestCase (new WifiTest, TestCase::QUICK);
  AddTestCase (new QosUtilsIsOldPacketTest, TestCase::QUICK);
  AddTestCase (new InterferenceHelperSequenceTest, TestCase::QUICK); //Bug 991
  AddTestCase (new Bug555TestCase, TestCase::QUICK); //Bug 555
  AddTestCase (new Bug730TestCase, TestCase::QUICK); //Bug 730
  AddTestCase (new SetChannelFrequencyTest, TestCase::QUICK);
  AddTestCase (new Bug2222TestCase, TestCase::QUICK); //Bug 2222
}

static WifiTestSuite g_wifiTestSuite;
