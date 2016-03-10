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
 *         Matías Richart <mrichart@fing.edu.uy>
 */

// Test the operation of IdealWifiManager as the SNR is varied, and create
// a gnuplot output file for plotting
//
// By default, the 802.11b standard is plotted.  Several command line
// arguments can change the following options:
//    --rtsThreshold:     RTS threshold [65535]
//    --BE_MaxAmpduSize:  BE Mac A-MPDU size [65535]
//    --stepSize:         Power between steps (dBm) [1]
//    --stepTime:         Time on each step (seconds) [1]
//    --broadcast:        Send broadcast instead of unicast [0]
//    --channelWidth:     Set channel width (valid only for 802.11n or ac) [20]
//    --standard:         Set standard (802.11a, 802.11b, 802.11g, 802.11n-5GHz, 802.11ac, and others...) [802.11b]

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/stats-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MinstrelHtWifiManagerExample");

// 20 MHz, 290K
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
RateChange (uint64_t newVal, Mac48Address dest)
{
  NS_LOG_DEBUG ("Change to " << newVal);
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
  StandardInfo (std::string name, enum WifiPhyStandard standard, uint32_t width, bool sgi, double snrLow, double snrHigh, double xMin, double xMax, double yMax)
    : m_name (name),
      m_standard (standard),
      m_width (width),
      m_sgi (sgi),
      m_snrLow (snrLow),
      m_snrHigh (snrHigh),
      m_xMin (xMin),
      m_xMax (xMax),
      m_yMax (yMax)
  {
  }
  std::string m_name;
  enum WifiPhyStandard m_standard;
  uint32_t m_width;
  bool m_sgi;
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
  double snr = rss - noiseDbm; //dB
  rateDataset.Add (snr, g_intervalRate / 1000000.0);
  // Calculate received rate since last interval
  double currentRate = ((g_intervalBytes * 8) / step.stepTime) / 1e6; // Mb/s
  actualDataset.Add (snr, currentRate);
  rssModel->SetRss (rss - step.stepSize);
  NS_LOG_INFO ("At time " << Simulator::Now ().As (Time::S) << "; observed rate " << currentRate << "; setting new power to " << rss - step.stepSize);
  g_intervalBytes = 0;
  Simulator::Schedule (Seconds (step.stepTime), &ChangeSignalAndReportRate, rssModel, step, (rss - step.stepSize), rateDataset, actualDataset);
}

int main (int argc, char *argv[])
{
  std::vector <StandardInfo> standards;
  int steps;
  uint32_t rtsThreshold = 65535;
  uint32_t BE_MaxAmpduSize = 65535;
  double stepSize = 1; // dBm
  double stepTime = 1; // seconds
  uint32_t packetSize = 1024;  // bytes
  int broadcast = 0;
  int ap1_x = 0;
  int ap1_y = 0;
  int sta1_x = 5;
  int sta1_y = 0;
  uint32_t channelWidth = 20;
  std::string standard ("802.11b");
  StandardInfo selectedStandard;
  std::string outfileName ("minstrel-ht-");

  CommandLine cmd;
  cmd.AddValue ("rtsThreshold", "RTS threshold", rtsThreshold);
  cmd.AddValue ("BE_MaxAmpduSize", "BE Max A-MPDU size", BE_MaxAmpduSize);
  cmd.AddValue ("stepSize", "Power between steps (dBm)", stepSize);
  cmd.AddValue ("stepTime", "Time on each step (seconds)", stepTime);
  cmd.AddValue ("broadcast", "Send broadcast instead of unicast", broadcast);
  cmd.AddValue ("channelWidth", "Set channel width (valid only for 802.11n or ac)", channelWidth);
  cmd.AddValue ("standard", "Set standard (802.11a, 802.11b, 802.11g, 802.11n-5GHz, 802.11ac, and others...)", standard);
  cmd.Parse (argc, argv);

  outfileName.append (standard);
  std::string tmp = outfileName + ".plt";
  std::ofstream outfile (tmp.c_str ());
  Gnuplot gnuplot = Gnuplot (outfileName + ".eps");

  // The first number is channel width, second is minimum SNR, third is maximum
  // SNR, fourth and fifth provide xrange axis limits, and sixth the yaxis
  // maximum
  standards.push_back (StandardInfo ("802.11a", WIFI_PHY_STANDARD_80211a, 20, false, 3, 27, 0, 30, 60));
  standards.push_back (StandardInfo ("802.11b", WIFI_PHY_STANDARD_80211b, 22, false, -5, 11, -6, 15, 15));
  standards.push_back (StandardInfo ("802.11g", WIFI_PHY_STANDARD_80211g, 20, false, -5, 27, -6, 30, 80));
  standards.push_back (StandardInfo ("802.11n-5GHz", WIFI_PHY_STANDARD_80211n_5GHZ, 20, false, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11n-5GHz-40MHz", WIFI_PHY_STANDARD_80211n_5GHZ, 40, false, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11n-5GHz-SGI", WIFI_PHY_STANDARD_80211n_5GHZ, 20, true, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11n-5GHz-40MHz-SGI", WIFI_PHY_STANDARD_80211n_5GHZ, 40, true, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11n-2.4GHz", WIFI_PHY_STANDARD_80211n_2_4GHZ, 20, false, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11ac", WIFI_PHY_STANDARD_80211ac, 20, false, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11ac-40MHz", WIFI_PHY_STANDARD_80211ac, 40, false, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11ac-80MHz", WIFI_PHY_STANDARD_80211ac, 80, false, 5, 30, 0, 35, 80));
  standards.push_back (StandardInfo ("802.11ac-160MHz", WIFI_PHY_STANDARD_80211ac, 160, false, 5, 30, 0, 35, 80));

  for (std::vector<StandardInfo>::size_type i = 0; i != standards.size (); i++)
    {
      if (standard == standards[i].m_name)
        {
          selectedStandard = standards[i];
        }
    }
  NS_ABORT_IF (selectedStandard.m_name == "none");
  std::cout << "Testing " << selectedStandard.m_name << "..." << std::endl;
  NS_ABORT_MSG_IF (selectedStandard.m_snrLow >= selectedStandard.m_snrHigh, "SNR values in wrong order");
  steps = std::abs ((int) (selectedStandard.m_snrHigh - selectedStandard.m_snrLow ) / stepSize) + 1;
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
  wifiPhy.Set ("ShortGuardEnabled", BooleanValue (selectedStandard.m_sgi));

  wifi.SetRemoteStationManager ("ns3::MinstrelHtWifiManager", "RtsCtsThreshold", UintegerValue (rtsThreshold), "PrintStats", BooleanValue (true));

  // Use Adhoc so we don't get into association issues
  NetDeviceContainer serverDevice;
  NetDeviceContainer clientDevice;

  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac",
                   "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
  serverDevice = wifi.Install (wifiPhy, wifiMac, serverNode);
  clientDevice = wifi.Install (wifiPhy, wifiMac, clientNode);

  Config::ConnectWithoutContext ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$ns3::MinstrelHtWifiManager/RateChange", MakeCallback (&RateChange));

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

  // Set channel width
  // Adjust noise for channel width
  // Obtain pointer to the WifiPhy
  Ptr<NetDevice> nd = clientDevice.Get (0);
  Ptr<WifiNetDevice> wnd = nd->GetObject<WifiNetDevice> ();
  Ptr<WifiPhy> wifiPhyPtr = wnd->GetPhy ();
  wifiPhyPtr->SetChannelWidth (selectedStandard.m_width);
  noiseDbm += 10 * log10 (selectedStandard.m_width * 1000000);
  NS_LOG_DEBUG ("Channel width " << wifiPhyPtr->GetChannelWidth () << " noiseDbm " << noiseDbm);

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
  client->SetStartTime (Seconds (0.5));
  client->SetAttribute ("MaxPackets", UintegerValue (0));
  client->SetAttribute ("PacketSize", UintegerValue (packetSize));
  client->SetAttribute ("Interval", TimeValue (MicroSeconds (20)));
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

  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("SNR (dB)", "Rate (Mb/s)");
  gnuplot.SetExtra  ("set xrange [0:50]");
  gnuplot.SetExtra  ("set yrange [0:150]");
  gnuplot.SetExtra  ("set key reverse left Left");
  gnuplot.GenerateOutput (outfile);
  outfile.close ();
  return 0;
}
