/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "ns3/log.h"
#include "ns3/test.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/mobility-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiInterferenceTestSuite");

class WifiInterferenceTestCase : public TestCase
{
public:
  WifiInterferenceTestCase ();
  virtual ~WifiInterferenceTestCase ();

private:
  virtual void DoRun (void);
  void ReceivePacket (Ptr<Socket> socket);
  static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval);
  void PrintEndSync (std::string context, uint32_t dataRate, double snr, double per);
  double WifiSimpleInterference (std::string phyMode, double Prss, double Irss, double delta, uint32_t PpacketSize, 
                                 uint32_t IpacketSize, bool verbose, InternetStackHelper internet, WifiPhyStandard wifiStandard);
  double m_PER;
  double m_SNR;
  uint32_t m_DataRate;
};

// Add some help text to this case to describe what it is intended to test
WifiInterferenceTestCase::WifiInterferenceTestCase  ()
  : TestCase ("Test interference calculation when interfering frame exactly overlaps intended frame")
{
}

WifiInterferenceTestCase::~WifiInterferenceTestCase ()
{
}

static inline std::string
PrintReceivedPacket (Ptr<Socket> socket)
{
  Address addr;
  socket->GetSockName (addr);
  InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (addr);

  std::ostringstream oss;
  oss << "Received one packet!  Socket: " << iaddr.GetIpv4 () << " port: " << iaddr.GetPort ();

  return oss.str ();
}

void
WifiInterferenceTestCase::ReceivePacket (Ptr<Socket> socket)
{
  NS_LOG_UNCOND (PrintReceivedPacket (socket));
}

void 
WifiInterferenceTestCase::GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval)
{
  if (pktCount > 0)
    {
      socket->Send (Create<Packet> (pktSize));
      Simulator::Schedule (pktInterval, &WifiInterferenceTestCase::GenerateTraffic, socket, pktSize, pktCount-1, pktInterval);
    }
  else
    {
      socket->Close ();
    }
}

void 
WifiInterferenceTestCase::PrintEndSync (std::string context, uint32_t dataRate, double snr, double per)
{
  NS_LOG_UNCOND ("EndSync:  Received frame with dataRate=" << dataRate << ", SNR=" << snr << ", PER =" << per);
  m_PER = per;
  m_SNR = snr;
  m_DataRate = dataRate;
}

double 
WifiInterferenceTestCase::WifiSimpleInterference (std::string phyMode,double Prss, double Irss, double delta, uint32_t PpacketSize, uint32_t IpacketSize, bool verbose, InternetStackHelper internet, WifiPhyStandard wifiStandard)
{

  uint32_t numPackets = 1;
  double interval = 1.0; // seconds
  double startTime = 10.0; // seconds
  double distanceToRx = 100.0; // meters

  double offset = 91;  // This is a magic number used to set the 
  // transmit power, based on other configuration

  m_PER = 0;
  m_SNR = 0;
  m_DataRate = 0;

  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", 
                      StringValue (phyMode));

  NodeContainer c;
  c.Create (3);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  wifi.SetStandard (wifiStandard);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // This is one parameter that matters when using FixedRssLossModel
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (0) ); 
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (0.0) );

  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO); 

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a mac and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));

  // Set it to adhoc mode (and enable QOS for HT/VHT stations)
  if (wifiStandard == WIFI_PHY_STANDARD_80211n_2_4GHZ
      || wifiStandard == WIFI_PHY_STANDARD_80211n_5GHZ
      || wifiStandard == WIFI_PHY_STANDARD_80211ac)
    {
      wifiMac.SetType ("ns3::AdhocWifiMac",
                       "QosSupported", BooleanValue (true));
    }
  else
    {
      wifiMac.SetType ("ns3::AdhocWifiMac");
    }
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c.Get (0));

  // This will disable these sending devices from detecting a signal 
  // so that they do not backoff
  wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue (0.0) );
  wifiPhy.Set ("TxGain", DoubleValue (offset + Prss) ); 
  devices.Add (wifi.Install (wifiPhy, wifiMac, c.Get (1)));
  wifiPhy.Set ("TxGain", DoubleValue (offset + Irss) ); 
  devices.Add (wifi.Install (wifiPhy, wifiMac, c.Get (2)));

  // Note that with FixedRssLossModel, the positions below are not 
  // used for received signal strength. 
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (distanceToRx, 0.0, 0.0));
  positionAlloc->Add (Vector (-1*distanceToRx, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  // InternetStackHelper internet;
  internet.Install (c);
 
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (c.Get (0), tid);
  InetSocketAddress local = InetSocketAddress (Ipv4Address ("10.1.1.1"), 80);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&WifiInterferenceTestCase::ReceivePacket, this));

  Ptr<Socket> source = Socket::CreateSocket (c.Get (1), tid);
  InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);
  source->Connect (remote);

  // Interferer will send to a different port; we will not see a
  // "Received packet" message
  Ptr<Socket> interferer = Socket::CreateSocket (c.Get (2), tid);
  InetSocketAddress interferingAddr = InetSocketAddress (Ipv4Address ("255.255.255.255"), 49000);
  interferer->Connect (interferingAddr);

  Config::Connect ("/NodeList/0/DeviceList/0/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/EndSync", MakeCallback (&WifiInterferenceTestCase::PrintEndSync, this)); 
  // Tracing
//  wifiPhy.EnablePcap ("wifi-simple-interference", devices.Get (0));

  Simulator::ScheduleWithContext (source->GetNode ()->GetId (),
                                  Seconds (startTime), &GenerateTraffic,
                                  source, PpacketSize, numPackets, interPacketInterval);

  Simulator::ScheduleWithContext (interferer->GetNode ()->GetId (),
                                  Seconds (startTime + delta/1000000.0), &GenerateTraffic,
                                  interferer, IpacketSize, numPackets, interPacketInterval);

  Simulator::Run ();
  Simulator::Destroy ();

  return m_PER;
}

void
WifiInterferenceTestCase::DoRun (void)
{

  std::string phyMode ("DsssRate1Mbps");
  WifiPhyStandard wifiStandard=WIFI_PHY_STANDARD_80211b;
  double Prss = -90;  // -dBm
  double Irss = -90;  // -dBm
  double delta = 0;  // microseconds
  uint32_t PpacketSize = 1000; // bytes
  uint32_t IpacketSize = 1000; // bytes
  bool verbose = false;
  double PER, PER1, PER2;
  InternetStackHelper internet;

  // Compute the packet error rate (PER) when delta=0 microseconds.  This
  // means that the interferer arrives at exactly the same time as the
  // intended packet
  PER = WifiSimpleInterference (phyMode,Prss,Irss,delta,PpacketSize,IpacketSize,verbose,internet,wifiStandard);

  // Now rerun this test case and compute the PER when the delta time between
  // arrival of the intended frame and interferer is 1 microsecond.
  delta = 1;
  PER1 = WifiSimpleInterference (phyMode,Prss,Irss,delta,PpacketSize,IpacketSize,verbose,internet,wifiStandard);

  // Now rerun this test case and compute the PER when the delta time between
  // arrival of the intended frame and interferer is 2 microseconds.
  delta = 2;
  PER2 = WifiSimpleInterference (phyMode,Prss,Irss,delta,PpacketSize,IpacketSize,verbose,internet,wifiStandard);

  double PERDiff1 = PER - PER1;

  double PERDiff2 = PER1 - PER2;

  NS_TEST_ASSERT_MSG_EQ (PERDiff1, PERDiff2, 
                         "The PER difference due to 1 microsecond difference in arrival shouldn't depend on absolute arrival");
  //Now rerun for 11n
  wifiStandard = WIFI_PHY_STANDARD_80211n_2_4GHZ;
  // Compute the packet error rate (PER) when delta=0 microseconds.  This
  // means that the interferer arrives at exactly the same time as the
  // intended packet
  PER = WifiSimpleInterference (phyMode,Prss,Irss,delta,PpacketSize,IpacketSize,verbose,internet,wifiStandard);

  // Now rerun this test case and compute the PER when the delta time between
  // arrival of the intended frame and interferer is 1 microsecond.
  delta = 1;
  PER1 = WifiSimpleInterference (phyMode,Prss,Irss,delta,PpacketSize,IpacketSize,verbose,internet,wifiStandard);

  // Now rerun this test case and compute the PER when the delta time between
  // arrival of the intended frame and interferer is 2 microseconds.
  delta = 2;
  PER2 = WifiSimpleInterference (phyMode,Prss,Irss,delta,PpacketSize,IpacketSize,verbose,internet,wifiStandard);

  PERDiff1 = PER - PER1;

  PERDiff2 = PER1 - PER2;

  NS_TEST_ASSERT_MSG_EQ (PERDiff1, PERDiff2, 
                         "The PER difference due to 1 microsecond difference in arrival shouldn't depend on absolute arrival");

}

class WifiInterferenceTestSuite : public TestSuite
{
public:
  WifiInterferenceTestSuite ();
};

WifiInterferenceTestSuite::WifiInterferenceTestSuite ()
  : TestSuite ("ns3-wifi-interference", UNIT)
{
  AddTestCase (new WifiInterferenceTestCase, TestCase::QUICK);
}

static WifiInterferenceTestSuite wifiInterferenceTestSuite;

