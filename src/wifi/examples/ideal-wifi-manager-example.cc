/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 University of Washington
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
 * Author: Tom Henderson <tomhend@u.washington.edu>
 */

// Test the operation of IdealWifiManager as the SNR is varied, and create
// a gnuplot output file for plotting
//
// By default, the 802.11b standard is plotted.  Several command line 
// arguments can change the following options:
// --standard (802.11a, 802.11b, 802.11g, 802.11n-5GHz, 802.11n-2.4GHz, 802.11ac, 802.11-holland, 802.11-10MHz, 802.11-5MHz)
// --shortGuard (for 802.11n/ac)
// --nss (for 802.11n/ac)
// --channelWidth (for 802.11n/ac)
// --broadcast instead of unicast (default is unicast)
// --rtsThreshold (by default, value of 99999 disables it)

#include <cmath>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/stats-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("IdealWifiManagerExample");

// 290K @ 20 MHz
const double NOISE_DBM_Hz = -174.0;
double noiseDbm = NOISE_DBM_Hz;

double g_intervalBytes = 0;
uint64_t g_intervalRate = 0;

void
PacketRx (Ptr<const Packet> pkt, const Address &addr)
{
  NS_LOG_DEBUG ("Received size " << pkt->GetSize ());
  g_intervalBytes += pkt->GetSize ();
}

void
RateChange (uint64_t oldVal, uint64_t newVal)
{
  NS_LOG_DEBUG ("Change from " << oldVal << " to " << newVal);
  g_intervalRate = newVal;
}

struct Step
{
  double stepSize;
  double stepTime;
};

struct StandardInfo
{
  StandardInfo ()
  {
    m_name = "none";
  }
  StandardInfo (std::string name, enum WifiPhyStandard standard, uint32_t width, double snrLow, double snrHigh, double xMin, double xMax, double yMax) 
   : m_name (name),
     m_standard (standard),
     m_width (width),
     m_snrLow (snrLow),
     m_snrHigh (snrHigh),
     m_xMin (xMin),
     m_xMax (xMax),
     m_yMax (yMax)
    {}
  std::string m_name;
  enum WifiPhyStandard m_standard;
  uint32_t m_width;
  double m_snrLow;
  double m_snrHigh;
  double m_xMin; 
  double m_xMax; 
  double m_yMax; 
};

void
ChangeSignalAndReportRate (Ptr<FixedRssLossModel> rssModel, struct Step step, double rss, Gnuplot2dDataset& rateDataset, Gnuplot2dDataset& actualDataset)
{
  NS_LOG_FUNCTION (rssModel << step.stepSize << step.stepTime << rss);
  double snr = rss - noiseDbm;
  rateDataset.Add (snr, g_intervalRate / 1000000.0);
  // Calculate received rate since last interval
  double currentRate = ((g_intervalBytes * 8)/step.stepTime) / 1e6; // Mb/s
  actualDataset.Add (snr, currentRate);
  rssModel->SetRss (rss - step.stepSize);
  NS_LOG_INFO ("At time " << Simulator::Now ().As (Time::S) << "; observed rate " << currentRate << "; setting new power to " << rss - step.stepSize);
  g_intervalBytes = 0;
  Simulator::Schedule (Seconds (step.stepTime), &ChangeSignalAndReportRate, rssModel, step, (rss - step.stepSize), rateDataset, actualDataset);
}

int main (int argc, char *argv[])
{
  std::vector <StandardInfo> standards;
  uint32_t steps;

  uint32_t rtsThreshold = 999999;  // disabled even for large A-MPDU
  double stepSize = 1; // dBm
  double stepTime = 0.5; // seconds
  uint32_t packetSize = 1024;  // bytes
  int broadcast = 0;
  int ap1_x = 0;
  int ap1_y = 0;
  int sta1_x = 5;
  int sta1_y = 0;
  uint16_t nss = 1;
  bool shortGuardInterval = false;
  uint32_t channelWidth = 20;
  std::string standard ("802.11b");
  StandardInfo selectedStandard;

  CommandLine cmd;
  cmd.AddValue ("rtsThreshold", "RTS threshold", rtsThreshold);
  cmd.AddValue ("stepSize", "Power between steps (dBm)", stepSize);
  cmd.AddValue ("stepTime", "Time on each step (seconds)", stepTime);
  cmd.AddValue ("broadcast", "Send broadcast instead of unicast", broadcast);
  cmd.AddValue ("channelWidth", "Set channel width (valid only for 802.11n or ac)", channelWidth);
  cmd.AddValue ("nss", "Set nss (valid only for 802.11n or ac)", nss);
  cmd.AddValue ("shortGuard", "Set short guard interval (802.11n/ac)", shortGuardInterval);
  cmd.AddValue ("standard", "Set standard (802.11a, 802.11b, 802.11g, 802.11n-5GHz, 802.11n-2.4GHz, 802.11ac, 802.11-holland, 802.11-10MHz, 802.11-5MHz)", standard);
  cmd.Parse (argc, argv);

  if (standard == "802.11b")
    {
      NS_ABORT_MSG_IF (channelWidth != 20 && channelWidth != 22, "Invalid channel width for standard " << standard);
      NS_ABORT_MSG_IF (nss != 1, "Invalid nss for standard " << standard);
    }
  else if (standard == "802.11a" || standard == "802.11g")
    {
      NS_ABORT_MSG_IF (channelWidth != 20, "Invalid channel width for standard " << standard);
      NS_ABORT_MSG_IF (nss != 1, "Invalid nss for standard " << standard);
    }
  else if (standard == "802.11n-5GHz" || standard == "802.11n-2.4GHz")
    {
      NS_ABORT_MSG_IF (channelWidth != 20 && channelWidth != 40, "Invalid channel width for standard " << standard);
      NS_ABORT_MSG_IF (nss == 0 || nss > 4, "Invalid nss " << nss << " for standard " << standard);
    }
  else if (standard == "802.11ac")
    {
      NS_ABORT_MSG_IF (channelWidth != 20 && channelWidth != 40 && channelWidth != 80 && channelWidth != 160, "Invalid channel width for standard " << standard);
      NS_ABORT_MSG_IF (nss == 0 || nss > 4, "Invalid nss " << nss << " for standard " << standard);
    }

  std::string plotName = "ideal-wifi-manager-example-";
  std::string dataName = "ideal-wifi-manager-example-";
  plotName += standard;
  dataName += standard;
  plotName += ".eps";
  dataName += ".plt";
  std::ofstream outfile (dataName.c_str ());
  Gnuplot gnuplot = Gnuplot (plotName);

  // As channel width increases, scale up plot's yRange value
  uint32_t channelRateFactor = channelWidth / 20;
  channelRateFactor = channelRateFactor * nss;

  // The first number is channel width, second is minimum SNR, third is maximum
  // SNR, fourth and fifth provide xrange axis limits, and sixth the yaxis
  // maximum
  standards.push_back (StandardInfo ("802.11a", WIFI_PHY_STANDARD_80211a, 20, 3, 27, 0, 30, 60));
  standards.push_back (StandardInfo ("802.11b", WIFI_PHY_STANDARD_80211b, 22, -5, 11, -6, 15, 15));
  standards.push_back (StandardInfo ("802.11g", WIFI_PHY_STANDARD_80211g, 20, -5, 27, -6, 30, 60));
  standards.push_back (StandardInfo ("802.11n-5GHz", WIFI_PHY_STANDARD_80211n_5GHZ, channelWidth, 3, 30, 0, 35, 80 * channelRateFactor));
  standards.push_back (StandardInfo ("802.11n-2.4GHz", WIFI_PHY_STANDARD_80211n_2_4GHZ, channelWidth, 3, 30, 0, 35, 80 * channelRateFactor));
  standards.push_back (StandardInfo ("802.11ac", WIFI_PHY_STANDARD_80211ac, channelWidth, 5, 35, 0, 40, 120 * channelRateFactor));
  standards.push_back (StandardInfo ("802.11-holland", WIFI_PHY_STANDARD_holland, 20, 3, 27, 0, 30, 60));
  standards.push_back (StandardInfo ("802.11-10MHz", WIFI_PHY_STANDARD_80211_10MHZ, 10, 3, 27, 0, 30, 60));
  standards.push_back (StandardInfo ("802.11-5MHz", WIFI_PHY_STANDARD_80211_5MHZ, 10, 3, 27, 0, 30, 60));
 
  for (std::vector<StandardInfo>::size_type i = 0; i != standards.size(); i++)
    {
      if (standard == standards[i].m_name)
        {
          selectedStandard = standards[i];
        }
    }
  NS_ABORT_IF (selectedStandard.m_name == "none");
  std::cout << "Testing " << selectedStandard.m_name << "..." << std::endl;
  NS_ABORT_MSG_IF (selectedStandard.m_snrLow >= selectedStandard.m_snrHigh, "SNR values in wrong order"); 
  steps = static_cast<uint32_t> (std::floor ((selectedStandard.m_snrHigh - selectedStandard.m_snrLow )/stepSize)) + 1;
  NS_LOG_DEBUG ("Using " << steps << " steps for SNR range " << selectedStandard.m_snrLow << ":" << selectedStandard.m_snrHigh);
  Ptr<Node> clientNode = CreateObject<Node> ();
  Ptr<Node> serverNode = CreateObject<Node> ();

  WifiHelper wifi;
  wifi.SetStandard (selectedStandard.m_standard);
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (0.0));
  wifiPhy.Set ("RxNoiseFigure", DoubleValue (0.0));
  wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue (-110.0));
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (-110.0));

  Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel> ();
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  wifiChannel->SetPropagationDelayModel (delayModel);
  Ptr<FixedRssLossModel> rssLossModel = CreateObject<FixedRssLossModel> ();
  wifiChannel->SetPropagationLossModel (rssLossModel);
  wifiPhy.SetChannel (wifiChannel);

  wifi.SetRemoteStationManager ("ns3::IdealWifiManager", "RtsCtsThreshold", UintegerValue (rtsThreshold));

  // Use Adhoc so we don't get into association issues
  NetDeviceContainer serverDevice;
  NetDeviceContainer clientDevice;

  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  serverDevice = wifi.Install (wifiPhy, wifiMac, serverNode);
  clientDevice = wifi.Install (wifiPhy, wifiMac, clientNode);

  Config::ConnectWithoutContext ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$ns3::IdealWifiManager/Rate", MakeCallback(&RateChange)); 
  // Configure the mobility.
  MobilityHelper mobility;
 Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  //Initial position of AP and STA
  positionAlloc->Add (Vector (ap1_x, ap1_y, 0.0));
  NS_LOG_INFO ("Setting initial AP position to " << Vector (ap1_x, ap1_y, 0.0));
  positionAlloc->Add (Vector (sta1_x, sta1_y, 0.0));
  NS_LOG_INFO ("Setting initial STA position to " << Vector (sta1_x, sta1_y, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (clientNode);
  mobility.Install (serverNode);

  Gnuplot2dDataset rateDataset (selectedStandard.m_name + std::string ("-rate selected"));
  Gnuplot2dDataset actualDataset (selectedStandard.m_name + std::string ("-observed"));
  struct Step step;
  step.stepSize = stepSize;
  step.stepTime = stepTime;
  
  // Perform post-install configuration from defaults for channel width,
  // guard interval, and nss, if necessary
  // Obtain pointer to the WifiPhy
  Ptr<NetDevice> ndClient = clientDevice.Get (0);
  Ptr<NetDevice> ndServer = serverDevice.Get (0);
  Ptr<WifiNetDevice> wndClient = ndClient->GetObject<WifiNetDevice> ();
  Ptr<WifiNetDevice> wndServer = ndServer->GetObject<WifiNetDevice> ();
  Ptr<WifiPhy> wifiPhyPtrClient = wndClient->GetPhy ();
  Ptr<WifiPhy> wifiPhyPtrServer = wndServer->GetPhy ();
  wifiPhyPtrClient->SetNumberOfTransmitAntennas (nss);
  wifiPhyPtrClient->SetNumberOfReceiveAntennas (nss);
  wifiPhyPtrServer->SetNumberOfTransmitAntennas (nss);
  wifiPhyPtrServer->SetNumberOfReceiveAntennas (nss);
  // Only set the channel width and guard interval for HT and VHT modes
  if (selectedStandard.m_name == "802.11n-5GHz" || 
      selectedStandard.m_name == "802.11n-2.4GHz" ||
      selectedStandard.m_name == "802.11ac")
    {
      wifiPhyPtrClient->SetChannelWidth (selectedStandard.m_width);
      wifiPhyPtrServer->SetChannelWidth (selectedStandard.m_width);
      wifiPhyPtrClient->SetGuardInterval (shortGuardInterval);
      wifiPhyPtrServer->SetGuardInterval (shortGuardInterval);
    }
  NS_LOG_DEBUG ("Channel width " << wifiPhyPtrClient->GetChannelWidth () << " noiseDbm " << noiseDbm);
  NS_LOG_DEBUG ("NSS " << wifiPhyPtrClient->GetSupportedTxSpatialStreams ());

  // Configure signal and noise, and schedule first iteration
  noiseDbm += 10 * log10 (selectedStandard.m_width * 1000000);
  double rssCurrent = (selectedStandard.m_snrHigh + noiseDbm);
  rssLossModel->SetRss (rssCurrent);
  NS_LOG_INFO ("Setting initial Rss to " << rssCurrent);
  //Move the STA by stepsSize meters every stepTime seconds
  Simulator::Schedule (Seconds (0.5 + stepTime), &ChangeSignalAndReportRate, rssLossModel, step, rssCurrent, rateDataset, actualDataset);

  PacketSocketHelper packetSocketHelper;
  packetSocketHelper.Install (serverNode);
  packetSocketHelper.Install (clientNode);
  
  PacketSocketAddress socketAddr;
  socketAddr.SetSingleDevice (serverDevice.Get (0)->GetIfIndex ());
  if (broadcast)
    {
      socketAddr.SetPhysicalAddress (serverDevice.Get (0)->GetBroadcast ());
    }
  else
    {
      socketAddr.SetPhysicalAddress (serverDevice.Get (0)->GetAddress ());
    }
  // Arbitrary protocol type.
  // Note: PacketSocket doesn't have any L4 multiplexing or demultiplexing
  //       The only mux/demux is based on the protocol field
  socketAddr.SetProtocol (1);

  Ptr<PacketSocketClient> client = CreateObject<PacketSocketClient> ();
  client->SetRemote (socketAddr);
  client->SetStartTime (Seconds (0.5));  // allow simulation warmup
  client->SetAttribute ("MaxPackets", UintegerValue (0));  // unlimited
  client->SetAttribute ("PacketSize", UintegerValue (packetSize));

  // Set a maximum rate 10% above the yMax specified for the selected standard 
  double rate = selectedStandard.m_yMax * 1e6 * 1.10;
  double clientInterval = static_cast<double> (packetSize) * 8 / rate;
  NS_LOG_DEBUG ("Setting interval to " << clientInterval << " sec for rate of " << rate << " bits/sec");

  client->SetAttribute ("Interval", TimeValue (Seconds (clientInterval)));
  clientNode->AddApplication (client);

  Ptr<PacketSocketServer> server = CreateObject<PacketSocketServer> ();
  server->SetLocal (socketAddr);
  server->TraceConnectWithoutContext ("Rx", MakeCallback (&PacketRx));
  serverNode->AddApplication (server);

  Simulator::Stop (Seconds ((steps + 1) * stepTime));
  Simulator::Run ();
  Simulator::Destroy ();

  gnuplot.AddDataset (rateDataset);
  gnuplot.AddDataset (actualDataset);

  std::ostringstream xMinStr, xMaxStr, yMaxStr;
  std::string xRangeStr ("set xrange [");
  xMinStr << selectedStandard.m_xMin;
  xRangeStr.append (xMinStr.str ());
  xRangeStr.append (":");
  xMaxStr << selectedStandard.m_xMax;
  xRangeStr.append (xMaxStr.str ());
  xRangeStr.append ("]");
  std::string yRangeStr ("set yrange [0:");
  yMaxStr << selectedStandard.m_yMax;
  yRangeStr.append (yMaxStr.str ());
  yRangeStr.append ("]");

  std::ostringstream widthStrStr;
  std::ostringstream nssStrStr;
  std::string title ("Wi-Fi ideal rate control: ");
  title.append (standard);
  title.append (" channel width: ");
  widthStrStr << selectedStandard.m_width;
  title.append (widthStrStr.str ());
  title.append (" MHz nss: ");
  nssStrStr << nss;
  title.append (nssStrStr.str ());
  if (shortGuardInterval == true)
    {
      title.append (" shortGuard: true");
    }

  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("SNR (dB)", "Rate (Mb/s)");
  gnuplot.SetTitle (title);
  gnuplot.SetExtra (xRangeStr);
  gnuplot.AppendExtra (yRangeStr);
  gnuplot.AppendExtra ("set key top left");
  gnuplot.GenerateOutput (outfile);
  outfile.close ();

  return 0;
}

