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
 * Author: Matias Richart <mrichart@fing.edu.uy>
 */

/**
 * This example program is designed to illustrate the behavior of three
 * power/rate-adaptive WiFi rate controls; namely, ns3::ParfWifiManager,
 * ns3::AparfWifiManager and ns3::RrpaaWifiManager.
 *
 * The output of this is typically two plot files, named throughput-parf.plt
 * (or throughput-aparf.plt, if Aparf is used) and power-parf.plt. If
 * Gnuplot program is available, one can use it to convert the plt file
 * into an eps file, by running:
 * \code{.sh}
 *   gnuplot throughput-parf.plt
 * \endcode
 * Also, to enable logging of rate and power changes to the terminal, set this
 * environment variable:
 * \code{.sh}
 *   export NS_LOG=PowerAdaptationDistance=level_info
 * \endcode
 *
 * This simulation consist of 2 nodes, one AP and one STA.
 * The AP generates UDP traffic with a CBR of 54 Mbps to the STA.
 * The AP can use any power and rate control mechanism and the STA uses
 * only Minstrel rate control.
 * The STA can be configured to move away from (or towards to) the AP.
 * By default, the AP is at coordinate (0,0,0) and the STA starts at
 * coordinate (5,0,0) (meters) and moves away on the x axis by 1 meter every
 * second.
 *
 * The output consists of:
 * - A plot of average throughput vs. distance.
 * - A plot of average transmit power vs. distance.
 * - (if logging is enabled) the changes of power and rate to standard output.
 *
 * The Average Transmit Power is defined as an average of the power
 * consumed per measurement interval, expressed in milliwatts.  The
 * power level for each frame transmission is reported by the simulator,
 * and the energy consumed is obtained by multiplying the power by the
 * frame duration.  At every 'stepTime' (defaulting to 1 second), the
 * total energy for the collection period is divided by the step time
 * and converted from dbm to milliwatt units, and this average is
 * plotted against time.
 *
 * When neither Parf, Aparf or Rrpaa is selected as the rate control, the
 * generation of the plot of average transmit power vs distance is suppressed
 * since the other Wifi rate controls do not support the necessary callbacks
 * for computing the average power.
 *
 * To display all the possible arguments and their defaults:
 * \code{.sh}
 *   ./waf --run "power-adaptation-distance --help"
 * \endcode
 *
 * Example usage (selecting Aparf rather than Parf):
 * \code{.sh}
 *   ./waf --run "power-adaptation-distance --manager=ns3::AparfWifiManager --outputFileName=aparf"
 * \endcode
 *
 * Another example (moving towards the AP):
 * \code{.sh}
 *   ./waf --run "power-adaptation-distance --manager=ns3::AparfWifiManager --outputFileName=aparf --stepsSize=-1 --STA1_x=200"
 * \endcode
 *
 * To enable the log of rate and power changes:
 * \code{.sh}
 *   export NS_LOG=PowerAdaptationDistance=level_info
 * \endcode
 */

#include "ns3/gnuplot.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/mobility-model.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("PowerAdaptationDistance");

//packet size generated at the AP
static const uint32_t packetSize = 1420;

class NodeStatistics
{
public:
  NodeStatistics (NetDeviceContainer aps, NetDeviceContainer stas);

  void PhyCallback (std::string path, Ptr<const Packet> packet);
  void RxCallback (std::string path, Ptr<const Packet> packet, const Address &from);
  void PowerCallback (std::string path, double oldPower, double newPower, Mac48Address dest);
  void RateCallback (std::string path, DataRate oldRate, DataRate newRate, Mac48Address dest);
  void SetPosition (Ptr<Node> node, Vector position);
  void AdvancePosition (Ptr<Node> node, int stepsSize, int stepsTime);
  Vector GetPosition (Ptr<Node> node);

  Gnuplot2dDataset GetDatafile ();
  Gnuplot2dDataset GetPowerDatafile ();


private:
  typedef std::vector<std::pair<Time, DataRate> > TxTime;
  void SetupPhy (Ptr<WifiPhy> phy);
  Time GetCalcTxTime (DataRate rate);

  std::map<Mac48Address, double> currentPower;
  std::map<Mac48Address, DataRate> currentRate;
  uint32_t m_bytesTotal;
  double totalEnergy;
  double totalTime;
  Ptr<WifiPhy> myPhy;
  TxTime timeTable;
  Gnuplot2dDataset m_output;
  Gnuplot2dDataset m_output_power;
};

NodeStatistics::NodeStatistics (NetDeviceContainer aps, NetDeviceContainer stas)
{
  Ptr<NetDevice> device = aps.Get (0);
  Ptr<WifiNetDevice> wifiDevice = DynamicCast<WifiNetDevice> (device);
  Ptr<WifiPhy> phy = wifiDevice->GetPhy ();
  myPhy = phy;
  SetupPhy (phy);
  DataRate dataRate = DataRate (phy->GetMode (0).GetDataRate (phy->GetChannelWidth ()));
  double power = phy->GetTxPowerEnd ();
  for (uint32_t j = 0; j < stas.GetN (); j++)
    {
      Ptr<NetDevice> staDevice = stas.Get (j);
      Ptr<WifiNetDevice> wifiStaDevice = DynamicCast<WifiNetDevice> (staDevice);
      Mac48Address addr = wifiStaDevice->GetMac ()->GetAddress ();
      currentPower[addr] = power;
      currentRate[addr] = dataRate;
    }
  currentRate[Mac48Address ("ff:ff:ff:ff:ff:ff")] = dataRate;
  totalEnergy = 0;
  totalTime = 0;
  m_bytesTotal = 0;
  m_output.SetTitle ("Throughput Mbits/s");
  m_output_power.SetTitle ("Average Transmit Power");
}

void
NodeStatistics::SetupPhy (Ptr<WifiPhy> phy)
{
  uint32_t nModes = phy->GetNModes ();
  for (uint32_t i = 0; i < nModes; i++)
    {
      WifiMode mode = phy->GetMode (i);
      WifiTxVector txVector;
      txVector.SetMode (mode);
      txVector.SetPreambleType (WIFI_PREAMBLE_LONG);
      txVector.SetChannelWidth (phy->GetChannelWidth ());
      DataRate dataRate = DataRate (mode.GetDataRate (phy->GetChannelWidth ()));
      Time time = phy->CalculateTxDuration (packetSize, txVector, phy->GetFrequency ());
      NS_LOG_DEBUG (i << " " << time.GetSeconds () << " " << dataRate);
      timeTable.push_back (std::make_pair (time, dataRate));
    }
}

Time
NodeStatistics::GetCalcTxTime (DataRate rate)
{
  for (TxTime::const_iterator i = timeTable.begin (); i != timeTable.end (); i++)
    {
      if (rate == i->second)
        {
          return i->first;
        }
    }
  NS_ASSERT (false);
  return Seconds (0);
}

void
NodeStatistics::PhyCallback (std::string path, Ptr<const Packet> packet)
{
  WifiMacHeader head;
  packet->PeekHeader (head);
  Mac48Address dest = head.GetAddr1 ();

  if (head.GetType () == WIFI_MAC_DATA)
    {
      totalEnergy += pow (10.0, currentPower[dest] / 10.0) * GetCalcTxTime (currentRate[dest]).GetSeconds ();
      totalTime += GetCalcTxTime (currentRate[dest]).GetSeconds ();
    }
}

void
NodeStatistics::PowerCallback (std::string path, double oldPower, double newPower, Mac48Address dest)
{
  currentPower[dest] = newPower;
}

void
NodeStatistics::RateCallback (std::string path, DataRate oldRate, DataRate newRate, Mac48Address dest)
{
  currentRate[dest] = newRate;
}

void
NodeStatistics::RxCallback (std::string path, Ptr<const Packet> packet, const Address &from)
{
  m_bytesTotal += packet->GetSize ();
}

void
NodeStatistics::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
NodeStatistics::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void
NodeStatistics::AdvancePosition (Ptr<Node> node, int stepsSize, int stepsTime)
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / (1000000 * stepsTime));
  m_bytesTotal = 0;
  double atp = totalEnergy / stepsTime;
  totalEnergy = 0;
  totalTime = 0;
  m_output_power.Add (pos.x, atp);
  m_output.Add (pos.x, mbs);
  pos.x += stepsSize;
  SetPosition (node, pos);
  NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << " sec; setting new position to " << pos);
  Simulator::Schedule (Seconds (stepsTime), &NodeStatistics::AdvancePosition, this, node, stepsSize, stepsTime);
}

Gnuplot2dDataset
NodeStatistics::GetDatafile ()
{
  return m_output;
}

Gnuplot2dDataset
NodeStatistics::GetPowerDatafile ()
{
  return m_output_power;
}

void PowerCallback (std::string path, double oldPower, double newPower, Mac48Address dest)
{
  NS_LOG_INFO ((Simulator::Now ()).GetSeconds () << " " << dest << " Old power=" << oldPower << " New power=" << newPower);
}

void RateCallback (std::string path, DataRate oldRate, DataRate newRate, Mac48Address dest)
{
  NS_LOG_INFO ((Simulator::Now ()).GetSeconds () << " " << dest << " Old rate=" << oldRate << " New rate=" <<  newRate);
}

int main (int argc, char *argv[])
{
  double maxPower = 17;
  double minPower = 0;
  uint32_t powerLevels = 18;

  uint32_t rtsThreshold = 2346;
  std::string manager = "ns3::ParfWifiManager";
  std::string outputFileName = "parf";
  int ap1_x = 0;
  int ap1_y = 0;
  int sta1_x = 5;
  int sta1_y = 0;
  uint32_t steps = 200;
  uint32_t stepsSize = 1;
  uint32_t stepsTime = 1;

  CommandLine cmd;
  cmd.AddValue ("manager", "PRC Manager", manager);
  cmd.AddValue ("rtsThreshold", "RTS threshold", rtsThreshold);
  cmd.AddValue ("outputFileName", "Output filename", outputFileName);
  cmd.AddValue ("steps", "How many different distances to try", steps);
  cmd.AddValue ("stepsTime", "Time on each step", stepsTime);
  cmd.AddValue ("stepsSize", "Distance between steps", stepsSize);
  cmd.AddValue ("maxPower", "Maximum available transmission level (dbm).", maxPower);
  cmd.AddValue ("minPower", "Minimum available transmission level (dbm).", minPower);
  cmd.AddValue ("powerLevels", "Number of transmission power levels available between "
                "TxPowerStart and TxPowerEnd included.", powerLevels);
  cmd.AddValue ("AP1_x", "Position of AP1 in x coordinate", ap1_x);
  cmd.AddValue ("AP1_y", "Position of AP1 in y coordinate", ap1_y);
  cmd.AddValue ("STA1_x", "Position of STA1 in x coordinate", sta1_x);
  cmd.AddValue ("STA1_y", "Position of STA1 in y coordinate", sta1_y);
  cmd.Parse (argc, argv);

  if (steps == 0)
    {
      std::cout << "Exiting without running simulation; steps value of 0" << std::endl;
    }

  uint32_t simuTime = (steps + 1) * stepsTime;

  //Define the APs
  NodeContainer wifiApNodes;
  wifiApNodes.Create (1);

  //Define the STAs
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (1);

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  WifiMacHelper wifiMac;
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();

  wifiPhy.SetChannel (wifiChannel.Create ());

  NetDeviceContainer wifiApDevices;
  NetDeviceContainer wifiStaDevices;
  NetDeviceContainer wifiDevices;

  //Configure the STA node
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager", "RtsCtsThreshold", UintegerValue (rtsThreshold));
  wifiPhy.Set ("TxPowerStart", DoubleValue (maxPower));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (maxPower));

  Ssid ssid = Ssid ("AP");
  wifiMac.SetType ("ns3::StaWifiMac",
                   "Ssid", SsidValue (ssid));
  wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

  //Configure the AP node
  wifi.SetRemoteStationManager (manager, "DefaultTxPowerLevel", UintegerValue (powerLevels - 1), "RtsCtsThreshold", UintegerValue (rtsThreshold));
  wifiPhy.Set ("TxPowerStart", DoubleValue (minPower));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (maxPower));
  wifiPhy.Set ("TxPowerLevels", UintegerValue (powerLevels));

  ssid = Ssid ("AP");
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid));
  wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));

  wifiDevices.Add (wifiStaDevices);
  wifiDevices.Add (wifiApDevices);

  //Configure the mobility.
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  //Initial position of AP and STA
  positionAlloc->Add (Vector (ap1_x, ap1_y, 0.0));
  NS_LOG_INFO ("Setting initial AP position to " << Vector (ap1_x, ap1_y, 0.0));
  positionAlloc->Add (Vector (sta1_x, sta1_y, 0.0));
  NS_LOG_INFO ("Setting initial STA position to " << Vector (sta1_x, sta1_y, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNodes.Get (0));
  mobility.Install (wifiStaNodes.Get (0));

  //Statistics counter
  NodeStatistics statistics = NodeStatistics (wifiApDevices, wifiStaDevices);

  //Move the STA by stepsSize meters every stepsTime seconds
  Simulator::Schedule (Seconds (0.5 + stepsTime), &NodeStatistics::AdvancePosition, &statistics, wifiStaNodes.Get (0), stepsSize, stepsTime);

  //Configure the IP stack
  InternetStackHelper stack;
  stack.Install (wifiApNodes);
  stack.Install (wifiStaNodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = address.Assign (wifiDevices);
  Ipv4Address sinkAddress = i.GetAddress (0);
  uint16_t port = 9;

  //Configure the CBR generator
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  ApplicationContainer apps_sink = sink.Install (wifiStaNodes.Get (0));

  OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  onoff.SetConstantRate (DataRate ("54Mb/s"), packetSize);
  onoff.SetAttribute ("StartTime", TimeValue (Seconds (0.5)));
  onoff.SetAttribute ("StopTime", TimeValue (Seconds (simuTime)));
  ApplicationContainer apps_source = onoff.Install (wifiApNodes.Get (0));

  apps_sink.Start (Seconds (0.5));
  apps_sink.Stop (Seconds (simuTime));

  //------------------------------------------------------------
  //-- Setup stats and data collection
  //--------------------------------------------

  //Register packet receptions to calculate throughput
  Config::Connect ("/NodeList/1/ApplicationList/*/$ns3::PacketSink/Rx",
                   MakeCallback (&NodeStatistics::RxCallback, &statistics));

  //Register power and rate changes to calculate the Average Transmit Power
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$" + manager + "/PowerChange",
                   MakeCallback (&NodeStatistics::PowerCallback, &statistics));
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$" + manager + "/RateChange",
                   MakeCallback (&NodeStatistics::RateCallback, &statistics));

  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin",
                   MakeCallback (&NodeStatistics::PhyCallback, &statistics));

  //Callbacks to print every change of power and rate
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$" + manager + "/PowerChange",
                   MakeCallback (PowerCallback));
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$" + manager + "/RateChange",
                   MakeCallback (RateCallback));

  Simulator::Stop (Seconds (simuTime));
  Simulator::Run ();

  std::ofstream outfile (("throughput-" + outputFileName + ".plt").c_str ());
  Gnuplot gnuplot = Gnuplot (("throughput-" + outputFileName + ".eps").c_str (), "Throughput");
  gnuplot.SetTerminal ("post eps color enhanced");
  gnuplot.SetLegend ("Time (seconds)", "Throughput (Mb/s)");
  gnuplot.SetTitle ("Throughput (AP to STA) vs time");
  gnuplot.AddDataset (statistics.GetDatafile ());
  gnuplot.GenerateOutput (outfile);

  if (manager.compare ("ns3::ParfWifiManager") == 0
      || manager.compare ("ns3::AparfWifiManager") == 0
      || manager.compare ("ns3::RrpaaWifiManager") == 0)
    {
      std::ofstream outfile2 (("power-" + outputFileName + ".plt").c_str ());
      gnuplot = Gnuplot (("power-" + outputFileName + ".eps").c_str (), "Average Transmit Power");
      gnuplot.SetTerminal ("post eps color enhanced");
      gnuplot.SetLegend ("Time (seconds)", "Power (mW)");
      gnuplot.SetTitle ("Average transmit power (AP to STA) vs time");
      gnuplot.AddDataset (statistics.GetPowerDatafile ());
      gnuplot.GenerateOutput (outfile2);
    }

  Simulator::Destroy ();

  return 0;
}
