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

#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/adhoc-wifi-mac.h"
#include "ns3/ap-wifi-mac.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/yans-error-rate-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/test.h"
#include "ns3/pointer.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/config.h"
#include "ns3/packet-socket-server.h"
#include "ns3/packet-socket-client.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/spectrum-wifi-helper.h"
#include "ns3/multi-model-spectrum-channel.h"
#include "ns3/wifi-spectrum-signal-parameters.h"
#include "ns3/wifi-phy-tag.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/mgt-headers.h"

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
      rmac->GetAttribute ("Txop", ptr);
      Ptr<Txop> txop = ptr.Get<Txop> ();
      currentStream += txop->AssignStreams (currentStream);

      rmac->GetAttribute ("VO_Txop", ptr);
      Ptr<QosTxop> vo_txop = ptr.Get<QosTxop> ();
      currentStream += vo_txop->AssignStreams (currentStream);

      rmac->GetAttribute ("VI_Txop", ptr);
      Ptr<QosTxop> vi_txop = ptr.Get<QosTxop> ();
      currentStream += vi_txop->AssignStreams (currentStream);

      rmac->GetAttribute ("BE_Txop", ptr);
      Ptr<QosTxop> be_txop = ptr.Get<QosTxop> ();
      currentStream += be_txop->AssignStreams (currentStream);

      rmac->GetAttribute ("BK_Txop", ptr);
      Ptr<QosTxop> bk_txop = ptr.Get<QosTxop> ();
      bk_txop->AssignStreams (currentStream);
    }
}

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Wifi Test
 */
class WifiTest : public TestCase
{
public:
  WifiTest ();

  virtual void DoRun (void);


private:
  /// Run one function
  void RunOne (void);
  /**
   * Create one function
   * \param pos the position
   * \param channel the wifi channel
   */
  void CreateOne (Vector pos, Ptr<YansWifiChannel> channel);
  /**
   * Send one packet function
   * \param dev the device
   */
  void SendOnePacket (Ptr<WifiNetDevice> dev);

  ObjectFactory m_manager; ///< manager
  ObjectFactory m_mac; ///< MAC
  ObjectFactory m_propDelay; ///< propagation delay
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

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Qos Utils Is Old Packet Test
 */
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


/**
 * See \bugid{991}
 */
class InterferenceHelperSequenceTest : public TestCase
{
public:
  InterferenceHelperSequenceTest ();

  virtual void DoRun (void);


private:
  /**
   * Create one function
   * \param pos the position
   * \param channel the wifi channel
   * \returns the node
   */
  Ptr<Node> CreateOne (Vector pos, Ptr<YansWifiChannel> channel);
  /**
   * Send one packet function
   * \param dev the device
   */
  void SendOnePacket (Ptr<WifiNetDevice> dev);
  /**
   * Switch channel function
   * \param dev the device
   */
  void SwitchCh (Ptr<WifiNetDevice> dev);

  ObjectFactory m_manager; ///< manager
  ObjectFactory m_mac; ///< MAC
  ObjectFactory m_propDelay; ///< propagation delay
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
 * device in a short succession, that:
 * 1) no backoff occurs if the frame arrives and the idle time >= DIFS or AIFSn
 *    (this is 'DCF immediate access', Figure 9-3 of IEEE 802.11-2012)
 * 2) a backoff occurs for the second frame that arrives (this is clearly
 *    stated in Sec. 9.3.4.2 of IEEE 802.11-2012, (basic access, which
 *    applies to group-addressed frames) where it states
 *    "If, under these conditions, the medium is determined by the CS
 *    mechanism to be busy when a STA desires to initiate the initial frame
 *    of a frame exchange sequence (described in Annex G), exclusive of the
 *    CF period, the random backoff procedure described in 9.3.4.3
 *    shall be followed."
 *    and from 9.3.4.3
 *    "The result of this procedure is that transmitted
 *    frames from a STA are always separated by at least one backoff interval."
 *
 * The observed behavior is that the first frame will be sent immediately,
 * and the frames are spaced by (backoff + DIFS) time intervals
 * (where backoff is a random number of slot sizes up to maximum CW)
 *
 * The following test case should _not_ generate virtual collision for
 * the second frame.  The seed and run numbers were pick such that the
 * second frame gets backoff = 1 slot.
 *
 *                      frame 1, frame 2
 *                      arrive                DIFS = 2 x slot + SIFS
 *                      |                          = 2 x 9us + 16us for 11a
 *                      |                    <----------->
 *                      V                                 <-backoff->
 * time  |--------------|-------------------|-------------|----------->
 *       0              1s                  1.001408s     1.001442s  |1.001451s
 *                      ^                   ^                        ^
 *                      start TX            finish TX                start TX
 *                      frame 1             frame 1                  frame 2
 *                      ^
 *                      frame 2
 *                      backoff = 1 slot
 *
 * The buggy behavior observed in prior versions was shown by picking
 * RngSeedManager::SetRun (17);
 * which generated a 0 slot backoff for frame 2.  Then, frame 2
 * experiences a virtual collision and re-selects the backoff again.
 * As a result, the _actual_ backoff experience by frame 2 is less likely
 * to be 0 since that would require two successions of 0 backoff (one that
 * generates the virtual collision and one after the virtual collision).
 *
 * See \bugid{555} for past behavior.
 */

class DcfImmediateAccessBroadcastTestCase : public TestCase
{
public:
  DcfImmediateAccessBroadcastTestCase ();

  virtual void DoRun (void);


private:
  /**
   * Send one packet function
   * \param dev the device
   */
  void SendOnePacket (Ptr<WifiNetDevice> dev);

  ObjectFactory m_manager; ///< manager
  ObjectFactory m_mac; ///< MAC
  ObjectFactory m_propDelay; ///< propagation delay

  Time m_firstTransmissionTime; ///< first transmission time
  Time m_secondTransmissionTime; ///< second transmission time
  unsigned int m_numSentPackets; ///< number of sent packets

  /**
   * Notify Phy transmit begin
   * \param p the packet
   */
  void NotifyPhyTxBegin (Ptr<const Packet> p);
};

DcfImmediateAccessBroadcastTestCase::DcfImmediateAccessBroadcastTestCase ()
  : TestCase ("Test case for DCF immediate access with broadcast frames")
{
}

void
DcfImmediateAccessBroadcastTestCase::NotifyPhyTxBegin (Ptr<const Packet> p)
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
DcfImmediateAccessBroadcastTestCase::SendOnePacket (Ptr<WifiNetDevice> dev)
{
  Ptr<Packet> p = Create<Packet> (1000);
  dev->Send (p, dev->GetBroadcast (), 1);
}

void
DcfImmediateAccessBroadcastTestCase::DoRun (void)
{
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
  m_propDelay.SetTypeId ("ns3::ConstantSpeedPropagationDelayModel");
  m_manager.SetTypeId ("ns3::ConstantRateWifiManager");

  //Assign a seed and run number, and later fix the assignment of streams to
  //WiFi random variables, so that the first backoff used is one slot
  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (40);  // a value of 17 will result in zero slots

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
  //The below stream assignment will result in the Txop object
  //using a backoff value of zero for this test when the
  //Txop::EndTxNoAck() calls to StartBackoffNow()
  AssignWifiRandomStreams (txMac, 23);

  Ptr<ConstantPositionMobilityModel> txMobility = CreateObject<ConstantPositionMobilityModel> ();
  Ptr<YansWifiPhy> txPhy = CreateObject<YansWifiPhy> ();
  Ptr<ErrorRateModel> txError = CreateObject<YansErrorRateModel> ();
  txPhy->SetErrorRateModel (txError);
  txPhy->SetChannel (channel);
  txPhy->SetDevice (txDev);
  txPhy->SetMobility (txMobility);
  txPhy->ConfigureStandard (WIFI_PHY_STANDARD_80211a);

  txPhy->TraceConnectWithoutContext ("PhyTxBegin", MakeCallback (&DcfImmediateAccessBroadcastTestCase::NotifyPhyTxBegin, this));

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

  Simulator::Schedule (Seconds (1.0), &DcfImmediateAccessBroadcastTestCase::SendOnePacket, this, txDev);
  Simulator::Schedule (Seconds (1.0) + MicroSeconds (1), &DcfImmediateAccessBroadcastTestCase::SendOnePacket, this, txDev);

  Simulator::Stop (Seconds (2.0));
  Simulator::Run ();
  Simulator::Destroy ();

  //First packet has 1408 us of transmit time.   Slot time is 9 us.
  //Backoff is 1 slots.  SIFS is 16 us.  DIFS is 2 slots = 18 us.
  //Should send next packet at 1408 us + (1 * 9 us) + 16 us + (2 * 9) us
  //1451 us after the first one.
  uint32_t expectedWait1 = 1408 + (1 * 9) + 16 + (2 * 9);
  Time expectedSecondTransmissionTime = MicroSeconds (expectedWait1) + MilliSeconds (1000);
  NS_TEST_ASSERT_MSG_EQ (m_firstTransmissionTime, MilliSeconds (1000), "The first transmission time not correct!");

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
  uint32_t m_received; ///< received

  /**
   * Receive function
   * \param context the context
   * \param p the packet
   * \param adr the address
   */
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
  wifiStaNode.Get (0)->AddApplication (client);
  client->SetStartTime (Seconds (1));
  client->SetStopTime (Seconds (51.0));

  Ptr<PacketSocketServer> server = CreateObject<PacketSocketServer> ();
  server->SetLocal (socket);
  wifiApNode.Get (0)->AddApplication (server);
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

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Set Channel Frequency Test
 */
class SetChannelFrequencyTest : public TestCase
{
public:
  SetChannelFrequencyTest ();

  virtual void DoRun (void);


private:
  /**
   * Get yans wifi phy function
   * \param nc the device collection
   * \returns the wifi phy
   */
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    phy.Set ("ChannelNumber", UintegerValue (44));
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
    phySta = GetYansWifiPhyPtr (staDevice);
    NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 44, "802.11 5GHz configuration");
    NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
    NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5220, "802.11 5GHz configuration");
  }
  {
    // case 11
    WifiHelper wifi;
    wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
    phy.Set ("ChannelNumber", UintegerValue (44));
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
    phySta = GetYansWifiPhyPtr (staDevice);
    // Post-install reconfiguration to channel number 40
    Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/ChannelNumber", UintegerValue (40));
    NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelNumber (), 40, "802.11 5GHz configuration");
    NS_TEST_ASSERT_MSG_EQ (phySta->GetChannelWidth (), 20, "802.11 5GHz configuration");
    NS_TEST_ASSERT_MSG_EQ (phySta->GetFrequency (), 5200, "802.11 5GHz configuration");
  }
  {
    // case 12
    WifiHelper wifi;
    wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
    phy.Set ("ChannelNumber", UintegerValue (44));
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
    phySta = GetYansWifiPhyPtr (staDevice);
    // Post-install reconfiguration to channel width 40 MHz
    Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/ChannelWidth", UintegerValue (40));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
    phySta = GetYansWifiPhyPtr (staDevice);
    phySta->SetAttribute ("ChannelNumber", UintegerValue (44));
    // Post-install reconfiguration to channel width 40 MHz
    Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/ChannelWidth", UintegerValue (40));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
    staDevice = wifi.Install (phy, macSta, wifiStaNode.Get (0));
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
  uint32_t m_countInternalCollisions; ///< count internal collisions

  /// Populate ARP cache function
  void PopulateArpCache ();
  /**
   * Transmit data failed function
   * \param context the context
   * \param adr the MAC address
   */
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
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate54Mbps"),
                                "ControlMode", StringValue ("OfdmRate24Mbps"));
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
  positionAlloc->Add (Vector (10.0, 0.0, 0.0));
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
  wifiNodes.Get (0)->AddApplication (clientLowPriority);
  clientLowPriority->SetStartTime (Seconds (1.0));
  clientLowPriority->SetStopTime (Seconds (2.0));

  Ptr<PacketSocketClient> clientHighPriority = CreateObject<PacketSocketClient> ();
  clientHighPriority->SetAttribute ("PacketSize", UintegerValue (1460));
  clientHighPriority->SetAttribute ("MaxPackets", UintegerValue (1));
  clientHighPriority->SetAttribute ("Priority", UintegerValue (6)); //AC_VO
  clientHighPriority->SetRemote (socket);
  wifiNodes.Get (0)->AddApplication (clientHighPriority);
  clientHighPriority->SetStartTime (Seconds (1.0));
  clientHighPriority->SetStopTime (Seconds (2.0));

  Ptr<PacketSocketServer> server = CreateObject<PacketSocketServer> ();
  server->SetLocal (socket);
  wifiNodes.Get (1)->AddApplication (server);
  server->SetStartTime (Seconds (1.0));
  server->SetStopTime (Seconds (2.0));

  Config::Connect ("/NodeList/*/DeviceList/*/RemoteStationManager/MacTxDataFailed", MakeCallback (&Bug2222TestCase::TxDataFailedTrace, this));

  Simulator::Stop (Seconds (2.0));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_TEST_ASSERT_MSG_EQ (m_countInternalCollisions, 1, "unexpected number of internal collisions!");
}

//-----------------------------------------------------------------------------
/**
 * Make sure that the correct channel width and center frequency have been set
 * for OFDM basic rate transmissions and BSS channel widths larger than 20 MHz.
 *
 * The scenario considers a UDP transmission between a 40 MHz 802.11ac station and a
 * 40 MHz 802.11ac access point. All transmission parameters are checked so as
 * to ensure that only 2 {starting frequency, channelWidth, Number of subbands
 * in SpectrumModel, modulation type} tuples are used.
 *
 * See \bugid{2483}
 */

class Bug2483TestCase : public TestCase
{
public:
  Bug2483TestCase ();
  virtual ~Bug2483TestCase ();
  virtual void DoRun (void);

private:
  /**
   * A tuple of {starting frequency, channelWidth, Number of subbands in SpectrumModel, modulation type}
   */
  typedef std::tuple<double, uint16_t, uint32_t, WifiModulationClass> FreqWidthSubbandModulationTuple;
  std::vector<FreqWidthSubbandModulationTuple> m_distinctTuples; ///< vector of distinct {starting frequency, channelWidth, Number of subbands in SpectrumModel, modulation type} tuples

  /**
   * Stores the distinct {starting frequency, channelWidth, Number of subbands in SpectrumModel, modulation type} tuples
   * that have been used during the testcase run.
   * \param context the context
   * \param txParams spectrum signal parameters set by transmitter
   */
  void StoreDistinctTuple (std::string context, Ptr<SpectrumSignalParameters> txParams);
  /**
   * Triggers the arrival of a burst of 1000 Byte-long packets in the source device
   * \param numPackets number of packets in burst (maximum: 255)
   * \param sourceDevice pointer to the source NetDevice
   * \param destination address of the destination device
   */
  void SendPacketBurst (uint8_t numPackets, Ptr<NetDevice> sourceDevice, Address& destination) const;
};

Bug2483TestCase::Bug2483TestCase ()
  : TestCase ("Test case for Bug 2483")
{
}

Bug2483TestCase::~Bug2483TestCase ()
{
}

void
Bug2483TestCase::StoreDistinctTuple (std::string context,  Ptr<SpectrumSignalParameters> txParams)
{
  // Extract starting frequency and number of subbands
  Ptr<const SpectrumModel> c = txParams->psd->GetSpectrumModel ();
  std::size_t numBands = c->GetNumBands ();
  double startingFreq = c->Begin ()->fl;

  // Get channel bandwidth and modulation class
  Ptr<const WifiSpectrumSignalParameters> wifiTxParams = DynamicCast<WifiSpectrumSignalParameters> (txParams);
  Ptr<Packet> packet = wifiTxParams->packet->Copy ();
  WifiPhyTag tag;
  if (!packet->RemovePacketTag (tag))
    {
      NS_FATAL_ERROR ("Received Wi-Fi Signal with no WifiPhyTag");
      return;
    }
  WifiTxVector txVector = tag.GetWifiTxVector ();
  uint16_t channelWidth = txVector.GetChannelWidth ();
  WifiModulationClass modulationClass = txVector.GetMode ().GetModulationClass ();

  // Build a tuple and check if seen before (if so store it)
  FreqWidthSubbandModulationTuple tupleForCurrentTx = std::make_tuple (startingFreq, channelWidth,
                                                                       numBands, modulationClass);
  bool found = false;
  for (std::vector<FreqWidthSubbandModulationTuple>::const_iterator it = m_distinctTuples.begin (); it != m_distinctTuples.end (); it++)
    {
      if (*it == tupleForCurrentTx)
        {
          found = true;
        }
    }
  if (!found)
    {
      m_distinctTuples.push_back (tupleForCurrentTx);
    }
}

void
Bug2483TestCase::SendPacketBurst (uint8_t numPackets, Ptr<NetDevice> sourceDevice,
                                  Address& destination) const
{
  for (uint8_t i = 0; i < numPackets; i++)
    {
      Ptr<Packet> pkt = Create<Packet> (1000);  // 1000 dummy bytes of data
      sourceDevice->Send (pkt, destination, 0);
    }
}

void
Bug2483TestCase::DoRun (void)
{
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("500")); // so as to force RTS/CTS for data frames
  Config::SetDefault ("ns3::WifiPhy::CcaMode1Threshold", DoubleValue (-62.0));

  uint16_t channelWidth = 40; // at least 40 MHz expected here

  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);

  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  SpectrumWifiPhyHelper spectrumPhy = SpectrumWifiPhyHelper::Default ();
  Ptr<MultiModelSpectrumChannel> spectrumChannel = CreateObject<MultiModelSpectrumChannel> ();
  Ptr<FriisPropagationLossModel> lossModel = CreateObject<FriisPropagationLossModel> ();
  lossModel->SetFrequency (5.180e9);
  spectrumChannel->AddPropagationLossModel (lossModel);

  Ptr<ConstantSpeedPropagationDelayModel> delayModel
    = CreateObject<ConstantSpeedPropagationDelayModel> ();
  spectrumChannel->SetPropagationDelayModel (delayModel);

  spectrumPhy.SetChannel (spectrumChannel);
  spectrumPhy.SetErrorRateModel ("ns3::NistErrorRateModel");
  spectrumPhy.Set ("Frequency", UintegerValue (5180));
  spectrumPhy.Set ("ChannelWidth", UintegerValue (channelWidth));
  spectrumPhy.Set ("TxPowerStart", DoubleValue (10));
  spectrumPhy.Set ("TxPowerEnd", DoubleValue (10));

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("VhtMcs8"),
                                "ControlMode", StringValue ("VhtMcs8"));

  WifiMacHelper mac;
  mac.SetType ("ns3::StaWifiMac");
  NetDeviceContainer staDevice;
  staDevice = wifi.Install (spectrumPhy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac");
  NetDeviceContainer apDevice;
  apDevice = wifi.Install (spectrumPhy, mac, wifiApNode);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0)); // put close enough in order to use MCS
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  // Send two 5 packet-bursts
  Simulator::Schedule (Seconds (0.5), &Bug2483TestCase::SendPacketBurst, this, 5, apDevice.Get (0), staDevice.Get (0)->GetAddress ());
  Simulator::Schedule (Seconds (0.6), &Bug2483TestCase::SendPacketBurst, this, 5, apDevice.Get (0), staDevice.Get (0)->GetAddress ());

  Config::Connect ("/ChannelList/*/$ns3::MultiModelSpectrumChannel/TxSigParams", MakeCallback (&Bug2483TestCase::StoreDistinctTuple, this));

  Simulator::Stop (Seconds (0.8));
  Simulator::Run ();

  Simulator::Destroy ();

  // {starting frequency, channelWidth, Number of subbands in SpectrumModel, modulation type} tuples
  std::size_t numberTuples = m_distinctTuples.size ();
  NS_TEST_ASSERT_MSG_EQ (numberTuples, 2, "Only two distinct tuples expected");
  NS_TEST_ASSERT_MSG_EQ (std::get<0> (m_distinctTuples[0]) - 20e6, std::get<0> (m_distinctTuples[1]), "The starting frequency of the first tuple should be shifted 20 MHz to the right wrt second tuple");
  // Note that the first tuple should the one initiated by the beacon, i.e. legacy OFDM (20 MHz)
  NS_TEST_ASSERT_MSG_EQ (std::get<1> (m_distinctTuples[0]), 20, "First tuple's channel width should be 20 MHz");
  NS_TEST_ASSERT_MSG_EQ (std::get<2> (m_distinctTuples[0]), 193, "First tuple should have 193 subbands (64+DC, 20MHz+DC, inband and 64*2 out-of-band, 20MHz on each side)");
  NS_TEST_ASSERT_MSG_EQ (std::get<3> (m_distinctTuples[0]), WifiModulationClass::WIFI_MOD_CLASS_OFDM, "First tuple should be OFDM");
  // Second tuple
  NS_TEST_ASSERT_MSG_EQ (std::get<1> (m_distinctTuples[1]), channelWidth, "Second tuple's channel width should be 40 MHz");
  NS_TEST_ASSERT_MSG_EQ (std::get<2> (m_distinctTuples[1]), 385, "Second tuple should have 385 subbands (128+DC, 40MHz+DC, inband and 128*2 out-of-band, 40MHz on each side)");
  NS_TEST_ASSERT_MSG_EQ (std::get<3> (m_distinctTuples[1]), WifiModulationClass::WIFI_MOD_CLASS_VHT, "Second tuple should be VHT_OFDM");
}

//-----------------------------------------------------------------------------
/**
 * Make sure that the channel width and the channel number can be changed at runtime.
 *
 * The scenario considers an access point and a station using a 20 MHz channel width.
 * After 1s, we change the channel width and the channel number to use a 40 MHz channel.
 * The tests checks the operational channel width sent in Beacon frames
 * and verify that a reassociation procedure is executed.
 *
 * See \bugid{2831}
 */

class Bug2831TestCase : public TestCase
{
public:
  Bug2831TestCase ();
  virtual ~Bug2831TestCase ();
  virtual void DoRun (void);

private:
  /**
   * Function called to change the supported channel width at runtime
   */
  void ChangeSupportedChannelWidth (void);
  /**
   * Callback triggered when a packet is received by the PHYs
   * \param context the context
   * \param p the received packet
   */
  void RxCallback (std::string context, Ptr<const Packet> p);

  Ptr<YansWifiPhy> m_apPhy; ///< AP PHY
  Ptr<YansWifiPhy> m_staPhy; ///< STA PHY

  uint8_t m_reassocReqCount; ///< count number of reassociation requests
  uint8_t m_reassocRespCount; ///< count number of reassociation responses
  uint8_t m_countOperationalChannelWidth20; ///< count number of beacon frames announcing a 20 MHz operating channel width
  uint8_t m_countOperationalChannelWidth40; ///< count number of beacon frames announcing a 40 MHz operating channel width
};

Bug2831TestCase::Bug2831TestCase ()
  : TestCase ("Test case for Bug 2831"),
    m_reassocReqCount (0),
    m_reassocRespCount (0),
    m_countOperationalChannelWidth20 (0),
    m_countOperationalChannelWidth40 (0)
{
}

Bug2831TestCase::~Bug2831TestCase ()
{
}

void
Bug2831TestCase::ChangeSupportedChannelWidth ()
{
  m_apPhy->SetChannelNumber (38);
  m_apPhy->SetChannelWidth (40);
  m_staPhy->SetChannelNumber (38);
  m_staPhy->SetChannelWidth (40);
}

void
Bug2831TestCase::RxCallback (std::string context, Ptr<const Packet> p)
{
  Ptr<Packet> packet = p->Copy ();
  WifiMacHeader hdr;
  packet->RemoveHeader (hdr);
  if (hdr.IsReassocReq ())
    {
      m_reassocReqCount++;
    }
  else if (hdr.IsReassocResp ())
    {
      m_reassocRespCount++;
    }
  else if (hdr.IsBeacon ())
    {
      MgtBeaconHeader beacon;
      packet->RemoveHeader (beacon);
      HtOperation htOperation = beacon.GetHtOperation ();
      if (htOperation.GetStaChannelWidth () > 0)
        {
          m_countOperationalChannelWidth40++;
        }
      else
        {
          m_countOperationalChannelWidth20++;
        }
    }
}

void
Bug2831TestCase::DoRun (void)
{
  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  ObjectFactory propDelay;
  propDelay.SetTypeId ("ns3::ConstantSpeedPropagationDelayModel");
  Ptr<PropagationDelayModel> propagationDelay = propDelay.Create<PropagationDelayModel> ();
  Ptr<PropagationLossModel> propagationLoss = CreateObject<FriisPropagationLossModel> ();
  channel->SetPropagationDelayModel (propagationDelay);
  channel->SetPropagationLossModel (propagationLoss);

  Ptr<Node> apNode = CreateObject<Node> ();
  Ptr<WifiNetDevice> apDev = CreateObject<WifiNetDevice> ();
  ObjectFactory mac;
  mac.SetTypeId ("ns3::ApWifiMac");
  mac.Set ("EnableBeaconJitter", BooleanValue (false));
  Ptr<WifiMac> apMac = mac.Create<WifiMac> ();
  apMac->ConfigureStandard (WIFI_PHY_STANDARD_80211ax_5GHZ);

  Ptr<Node> staNode = CreateObject<Node> ();
  Ptr<WifiNetDevice> staDev = CreateObject<WifiNetDevice> ();
  mac.SetTypeId ("ns3::StaWifiMac");
  Ptr<WifiMac> staMac = mac.Create<WifiMac> ();
  staMac->ConfigureStandard (WIFI_PHY_STANDARD_80211ax_5GHZ);

  Ptr<ConstantPositionMobilityModel> apMobility = CreateObject<ConstantPositionMobilityModel> ();
  apMobility->SetPosition (Vector (0.0, 0.0, 0.0));
  apNode->AggregateObject (apMobility);

  Ptr<ErrorRateModel> error = CreateObject<YansErrorRateModel> ();
  m_apPhy = CreateObject<YansWifiPhy> ();
  m_apPhy->SetErrorRateModel (error);
  m_apPhy->SetChannel (channel);
  m_apPhy->SetMobility (apMobility);
  m_apPhy->SetDevice (apDev);
  m_apPhy->ConfigureStandard (WIFI_PHY_STANDARD_80211ax_5GHZ);
  m_apPhy->SetChannelNumber (36);
  m_apPhy->SetChannelWidth (20);

  Ptr<ConstantPositionMobilityModel> staMobility = CreateObject<ConstantPositionMobilityModel> ();
  staMobility->SetPosition (Vector (1.0, 0.0, 0.0));
  staNode->AggregateObject (staMobility);

  m_staPhy = CreateObject<YansWifiPhy> ();
  m_staPhy->SetErrorRateModel (error);
  m_staPhy->SetChannel (channel);
  m_staPhy->SetMobility (staMobility);
  m_staPhy->SetDevice (apDev);
  m_staPhy->ConfigureStandard (WIFI_PHY_STANDARD_80211ax_5GHZ);
  m_staPhy->SetChannelNumber (36);
  m_staPhy->SetChannelWidth (20);

  apMac->SetAddress (Mac48Address::Allocate ());
  apDev->SetMac (apMac);
  apDev->SetPhy (m_apPhy);
  ObjectFactory manager;
  manager.SetTypeId ("ns3::ConstantRateWifiManager");
  apDev->SetRemoteStationManager (manager.Create<WifiRemoteStationManager> ());
  apNode->AddDevice (apDev);

  staMac->SetAddress (Mac48Address::Allocate ());
  staDev->SetMac (staMac);
  staDev->SetPhy (m_staPhy);
  staDev->SetRemoteStationManager (manager.Create<WifiRemoteStationManager> ());
  staNode->AddDevice (staDev);

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyRxBegin", MakeCallback (&Bug2831TestCase::RxCallback, this));

  Simulator::Schedule (Seconds (1.0), &Bug2831TestCase::ChangeSupportedChannelWidth, this);

  Simulator::Stop (Seconds (3.0));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_TEST_ASSERT_MSG_EQ (m_reassocReqCount, 1, "Reassociation request not received");
  NS_TEST_ASSERT_MSG_EQ (m_reassocRespCount, 1, "Reassociation response not received");
  NS_TEST_ASSERT_MSG_EQ (m_countOperationalChannelWidth20, 10, "Incorrect operational channel width before channel change");
  NS_TEST_ASSERT_MSG_EQ (m_countOperationalChannelWidth40, 20, "Incorrect operational channel width after channel change");
}

//-----------------------------------------------------------------------------
/**
 * Make sure that Wifi STA is correctly associating to the best AP (i.e.,
 * nearest from STA). We consider 3 AP and 1 STA. This test case consisted of
 * three sub tests:
 *   - The best AP sends its beacon later than the other APs. STA is expected
 *     to associate to the best AP.
 *   - The STA is using active scanning instead of passive, the rest of the
 *     APs works normally. STA is expected to associate to the best AP
 *   - The nearest AP is turned off after sending beacon and while STA is
 *     still scanning. STA is expected to associate to the second best AP.
 *
 * See \bugid{2399}
 * \todo Add explicit association refusal test if ns-3 implemented it.
 */

class StaWifiMacScanningTestCase : public TestCase
{
public:
  StaWifiMacScanningTestCase ();
  virtual ~StaWifiMacScanningTestCase ();
  virtual void DoRun (void);

private:
  /**
   * Callback function on STA assoc event
   * \param context context string
   * \param bssid the associated AP's bssid
   */
  void AssocCallback (std::string context, Mac48Address bssid);
  /**
   * Turn beacon generation on the AP node
   * \param apNode the AP node
   */
  void TurnBeaconGenerationOn (Ptr<Node> apNode);
  /**
   * Turn the AP node off
   * \param apNode the AP node
   */
  void TurnApOff (Ptr<Node> apNode);
  /**
   * Setup test
   * \param nearestApBeaconGeneration set BeaconGeneration attribute of the nearest AP
   * \param staActiveProbe set ActiveProbing attribute of the STA
   * \return node container containing all nodes
   */
  NodeContainer Setup (bool nearestApBeaconGeneration, bool staActiveProbe);

  Mac48Address m_associatedApBssid; ///< Associated AP's bssid
};

StaWifiMacScanningTestCase::StaWifiMacScanningTestCase ()
  : TestCase ("Test case for StaWifiMac scanning capability")
{
}

StaWifiMacScanningTestCase::~StaWifiMacScanningTestCase ()
{
}

void
StaWifiMacScanningTestCase::AssocCallback (std::string context, Mac48Address bssid)
{
  m_associatedApBssid = bssid;
}

void
StaWifiMacScanningTestCase::TurnBeaconGenerationOn (Ptr<Node> apNode)
{
  Ptr<WifiNetDevice> netDevice = DynamicCast<WifiNetDevice> (apNode->GetDevice (0));
  Ptr<ApWifiMac> mac = DynamicCast<ApWifiMac> (netDevice->GetMac ());
  mac->SetAttribute ("BeaconGeneration", BooleanValue (true));
}

void
StaWifiMacScanningTestCase::TurnApOff (Ptr<Node> apNode)
{
  Ptr<WifiNetDevice> netDevice = DynamicCast<WifiNetDevice> (apNode->GetDevice (0));
  Ptr<WifiPhy> phy = netDevice->GetPhy ();
  phy->SetOffMode();
}

NodeContainer
StaWifiMacScanningTestCase::Setup (bool nearestApBeaconGeneration, bool staActiveProbe)
{
  NodeContainer apNodes;
  apNodes.Create (2);

  Ptr<Node> apNodeNearest = CreateObject<Node> ();
  Ptr<Node> staNode = CreateObject<Node> ();

  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager");

  WifiMacHelper mac;
  NetDeviceContainer apDevice, apDeviceNearest;
  mac.SetType ("ns3::ApWifiMac",
               "BeaconGeneration", BooleanValue (true));
  apDevice = wifi.Install (phy, mac, apNodes);
  mac.SetType ("ns3::ApWifiMac",
               "BeaconGeneration", BooleanValue (nearestApBeaconGeneration));
  apDeviceNearest = wifi.Install (phy, mac, apNodeNearest);

  NetDeviceContainer staDevice;
  mac.SetType ("ns3::StaWifiMac",
               "ActiveProbing", BooleanValue (staActiveProbe));
  staDevice = wifi.Install (phy, mac, staNode);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));  // Furthest AP
  positionAlloc->Add (Vector (10.0, 0.0, 0.0)); // Second nearest AP
  positionAlloc->Add (Vector (5.0, 5.0, 0.0));  // Nearest AP
  positionAlloc->Add (Vector (6.0, 5.0, 0.0));  // STA
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (apNodes);
  mobility.Install (apNodeNearest);
  mobility.Install (staNode);

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::StaWifiMac/Assoc", MakeCallback (&StaWifiMacScanningTestCase::AssocCallback, this));

  NodeContainer allNodes = NodeContainer (apNodes, apNodeNearest, staNode);
  return allNodes;
}

void
StaWifiMacScanningTestCase::DoRun (void)
{
  {
    NodeContainer nodes = Setup (false, false);
    Ptr<Node> nearestAp = nodes.Get (2);
    Mac48Address nearestApAddr = DynamicCast<WifiNetDevice> (nearestAp->GetDevice (0))->GetMac ()->GetAddress ();

    Simulator::Schedule (Seconds (0.05), &StaWifiMacScanningTestCase::TurnBeaconGenerationOn, this, nearestAp);

    Simulator::Stop (Seconds (0.2));
    Simulator::Run ();
    Simulator::Destroy ();

    NS_TEST_ASSERT_MSG_EQ (m_associatedApBssid, nearestApAddr, "STA is associated to the wrong AP");
  }
  m_associatedApBssid = Mac48Address ();
  {
    NodeContainer nodes = Setup (true, true);
    Ptr<Node> nearestAp = nodes.Get (2);
    Mac48Address nearestApAddr = DynamicCast<WifiNetDevice> (nearestAp->GetDevice (0))->GetMac ()->GetAddress ();

    Simulator::Stop (Seconds (0.2));
    Simulator::Run ();
    Simulator::Destroy ();

    NS_TEST_ASSERT_MSG_EQ (m_associatedApBssid, nearestApAddr, "STA is associated to the wrong AP");
  }
  m_associatedApBssid = Mac48Address ();
  {
    NodeContainer nodes = Setup (true, false);
    Ptr<Node> nearestAp = nodes.Get (2);
    Mac48Address secondNearestApAddr = DynamicCast<WifiNetDevice> (nodes.Get (1)->GetDevice (0))->GetMac ()->GetAddress ();

    Simulator::Schedule (Seconds (0.1), &StaWifiMacScanningTestCase::TurnApOff, this, nearestAp);

    Simulator::Stop (Seconds (1.5));
    Simulator::Run ();
    Simulator::Destroy ();

    NS_TEST_ASSERT_MSG_EQ (m_associatedApBssid, secondNearestApAddr, "STA is associated to the wrong AP");
  }
}

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Wifi Test Suite
 */
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
  AddTestCase (new DcfImmediateAccessBroadcastTestCase, TestCase::QUICK);
  AddTestCase (new Bug730TestCase, TestCase::QUICK); //Bug 730
  AddTestCase (new SetChannelFrequencyTest, TestCase::QUICK);
  AddTestCase (new Bug2222TestCase, TestCase::QUICK); //Bug 2222
  AddTestCase (new Bug2483TestCase, TestCase::QUICK); //Bug 2483
  AddTestCase (new Bug2831TestCase, TestCase::QUICK); //Bug 2831
  AddTestCase (new StaWifiMacScanningTestCase, TestCase::QUICK); //Bug 2399
}

static WifiTestSuite g_wifiTestSuite; ///< the test suite
