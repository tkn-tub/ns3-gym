/**
 *
 * Instructions:
 * ./waf --run multi-rate-first
 * gnuplot multi-rate-first-scen*.plt
 *
 * Output: 
 * multi-rate-first-scen1.eps
 * multi-rate-first-scen2.eps
 * multi-rate-first-scen3.eps
 * multi-rate-first-scen4.eps
 *
 * Side Note: It may take some time.
 */

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"

#include <iostream>
#include <fstream>

NS_LOG_COMPONENT_DEFINE ("Main");

using namespace ns3;

class Experiment
{
public:
  Experiment ();
  Experiment (std::string name);
  Gnuplot2dDataset Run (const WifiHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                        const NqosWifiMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel, const MobilityHelper &mobility, int positionStep);
private:
  void ReceivePacket (Ptr<Socket> socket);
  void SetPosition (Ptr<Node> node, Vector position);
  Vector GetPosition (Ptr<Node> node);
  void AdvancePosition (Ptr<Node> node);
  void BackTrackPosition (Ptr<Node> node);
  void StationaryPosition (Ptr<Node> node);
  void MultiPosition (Ptr<Node> node1, Ptr<Node> node2);
  Ptr<Socket> SetupPacketReceive (Ptr<Node> node);

  uint32_t m_bytesTotal;
  Gnuplot2dDataset m_output;
};

Experiment::Experiment ()
{}

Experiment::Experiment (std::string name)
  : m_output (name)
{
  m_output.SetStyle (Gnuplot2dDataset::LINES);
}

void
Experiment::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
Experiment::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void 
Experiment::AdvancePosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add ((Simulator::Now()).GetSeconds(), mbs);
  pos.x += 1.0;
	
  if (pos.x >= 210.0) 
    {
      return;
    }
  SetPosition (node, pos);

  //std::cout << "x="<<pos.x << std::endl;
  Simulator::Schedule (Seconds (1.0), &Experiment::AdvancePosition, this, node);
}
void 
Experiment::BackTrackPosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add ((Simulator::Now()).GetSeconds(), mbs);
  pos.x -= 1.0;
	
  if (pos.x < 0) 
    {
      return;
    }
  SetPosition (node, pos);

  //std::cout << "x="<<pos.x << std::endl;
  Simulator::Schedule (Seconds (1.0), &Experiment::BackTrackPosition, this, node);
}

void
Experiment::StationaryPosition (Ptr<Node> node)
{
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add ((Simulator::Now()).GetSeconds(), mbs);

}
void
Experiment::MultiPosition (Ptr<Node> n1, Ptr<Node> n2)
{
  Vector pos1 = GetPosition(n1);
  Vector pos2 = GetPosition(n2);
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add ((Simulator::Now()).GetSeconds(), mbs);
  
  if( pos1.x < 230)
    {
      pos1.x += 1.0;
      SetPosition (n1, pos1);
    }
  if( pos2.x > 0)
    {
      pos2.x -= 1.0;
      SetPosition (n2, pos2);
    }
}

void
Experiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while (packet = socket->Recv ())
    {
      m_bytesTotal += packet->GetSize ();
    }
}

Ptr<Socket>
Experiment::SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::PacketSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  sink->Bind ();
  sink->SetRecvCallback (MakeCallback (&Experiment::ReceivePacket, this));

  return sink;

}

Gnuplot2dDataset
Experiment::Run (const WifiHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                 const NqosWifiMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel, const MobilityHelper &mobility, int positionStep)
{
  m_bytesTotal = 0;

  NodeContainer c;
  c.Create (2);

  PacketSocketHelper packetSocket;
  packetSocket.Install (c);

  YansWifiPhyHelper phy = wifiPhy;
  phy.SetChannel (wifiChannel.Create ());

  NqosWifiMacHelper mac = wifiMac;
  NetDeviceContainer devices = wifi.Install (phy, mac, c);

  mobility.Install (c);

  PacketSocketAddress socket;
  socket.SetSingleDevice(devices.Get (0)->GetIfIndex ());
  socket.SetPhysicalAddress (devices.Get (1)->GetAddress ());
  socket.SetProtocol (1);

  OnOffHelper onoff ("ns3::PacketSocketFactory", Address (socket));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (250)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  onoff.SetAttribute ("DataRate", DataRateValue (DataRate (60000000)));
  onoff.SetAttribute ("PacketSize", UintegerValue (2000));

  ApplicationContainer apps = onoff.Install (c.Get (0));
  apps.Start (Seconds (0.5));
  apps.Stop (Seconds (250.0));


  Ptr<Socket> recvSink = SetupPacketReceive (c.Get (1));

  if (positionStep == 1)
    {
      Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (1));
    }
  else if (positionStep == -1)
    {
      Simulator::Schedule (Seconds (1.5), &Experiment::BackTrackPosition, this, c.Get (1));
    }
  else if (positionStep == 0)
    {
      for(int i = 1; i <= 210; i++) 
        {
          Simulator::Schedule (Seconds (i), &Experiment::StationaryPosition, this, c.Get (1));
        }
    }
  else if (positionStep == 2)
    {
      for(int i = 1; i <= 210; i++) 
        {
          Simulator::Schedule (Seconds (i), &Experiment::MultiPosition, this, c.Get(0), c.Get (1));
        }
    }

     
  Simulator::Run ();
  Simulator::Destroy ();

  return m_output;
}

int main (int argc, char *argv[])
{
  std::ofstream outfile ("multi-rate-first-scen1.plt");
  std::ofstream outfile2 ("multi-rate-first-scen2.plt");
  std::ofstream outfile3 ("multi-rate-first-scen3.plt");
  std::ofstream outfile4 ("multi-rate-first-scen4.plt");

  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));

  CommandLine cmd;
  cmd.Parse (argc, argv);


  MobilityHelper mobility;
  Experiment experiment;
  Gnuplot gnuplot;
  int myPositionStep; 
  Ptr<ListPositionAllocator> positionAlloc;
  Gnuplot2dDataset dataset;
  WifiHelper wifi = WifiHelper::Default ();
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();

  // Scenario 1: two nodes within transmission range about 5 meters apart
  // Fix a node stationary, move the second node away from it

  // moving forward 
  myPositionStep = 1;

  gnuplot = Gnuplot ("multi-rate-first-scen1.eps");

  positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");
  wifi.SetStandard (WIFI_PHY_STANDARD_holland);

  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("Time (Seconds)", "Throughput(Mbps)");
  gnuplot.SetExtra  ("set xrange [0:250]");
  gnuplot.SetTitle ("Throughput vs Time");
  gnuplot.GenerateOutput (outfile);
  outfile.close ();


  // Scenario 2: two nodes out of transmission range about 230 meters apart
  // Fix a node stationary, move the second node into transmission range

  // moving backward
  myPositionStep = -1;

  gnuplot = Gnuplot ("multi-rate-first-scen2.eps");

  positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (230.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");
  wifi.SetStandard (WIFI_PHY_STANDARD_holland);

  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset= experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset= experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);


  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("Time (Seconds)", "Throughput(Mbps)");
  gnuplot.SetExtra  ("set xrange [0:250]");
  gnuplot.SetTitle ("Throughput vs Time");
  gnuplot.GenerateOutput (outfile2);
  outfile2.close ();


  // Scenario 3: two nodes within transmission range 25 meters part
  // Set both nodes stationary
  // this is more like a sanity check

  // Set position stationary 
  myPositionStep = 0;

  gnuplot = Gnuplot ("multi-rate-first-scen3.eps");

  positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (25.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");
  wifi.SetStandard (WIFI_PHY_STANDARD_holland);

  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset= experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset= experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("Time (Seconds)", "Throughput(Mbps)");
  gnuplot.SetExtra  ("set xrange [0:250]");
  gnuplot.SetTitle ("Throughput vs Time");
  gnuplot.GenerateOutput (outfile3);
  outfile3.close ();

  // Scenario 4: Two nodes in opposite direction about 230 meters apart
  // moving into transmission range and out of transmission range
  myPositionStep = 2;

  gnuplot = Gnuplot ("multi-rate-first-scen4.eps");

  positionAlloc = CreateObject<ListPositionAllocator> ();
  // initial position of node 1
  positionAlloc->Add (Vector (0.0, 25.0, 0.0));
  // initial position of node 2
  positionAlloc->Add (Vector (230.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");
  wifi.SetStandard (WIFI_PHY_STANDARD_holland);

  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset= experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset= experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("Time (Seconds)", "Throughput(Mbps)");
  gnuplot.SetExtra  ("set xrange [0:250]");
  gnuplot.SetTitle ("Throughput vs Time");
  gnuplot.GenerateOutput (outfile4);
  outfile4.close ();
  

  return 0;
}
