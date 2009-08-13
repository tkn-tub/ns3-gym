/**
 *
 * Instructions:
 * ./waf --run multi-rate-first > m.data
 * gnuplot m.data
 * eog *.png
 *
 */

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"

#include <iostream>

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

  if(positionStep == 1)
    {
      Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (1));
    }
  else if(positionStep == -1)
    {
      Simulator::Schedule (Seconds (1.5), &Experiment::BackTrackPosition, this, c.Get (1));
    }
  else if(positionStep == 0)
    {
      for(int i = 1; i <= 210; i++) 
        {
          Simulator::Schedule (Seconds (i), &Experiment::StationaryPosition, this, c.Get (1));
        }
    }
  Simulator::Run ();
  Simulator::Destroy ();

  return m_output;
}

int main (int argc, char *argv[])
{
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));

  CommandLine cmd;
  cmd.Parse (argc, argv);

  Gnuplot gnuplot = Gnuplot ("multi-rate-first.png");
  Experiment experiment;
  WifiHelper wifi = WifiHelper::Default ();
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  Gnuplot2dDataset dataset;
  int myPositionStep = 0; 

/*

  // Scenario 1: moving away from one another
  // Initially set them 5 meters apart 
  // Set  positionStep parameter of Experiment::Run to 1  
  // Set RateErrorModel of Experiment::Run to 0
  myPositionStep = 1;

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");

  gnuplot = Gnuplot ("multi-rate-first.png");
  Config::SetDefault ("ns3::YansWifiPhy::Standard", StringValue ("holland"));

  NS_LOG_DEBUG ("minstrel");
  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("ideal");
  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);
  */


  // Scenario 2: two nodes out of range, moving into transmission range range
  // Initially set them 230 meters apart 
  // Set positionStep parameter of Experiment::Rung to -1
  // set RateErrorModel of Experiment::Run to 0

  myPositionStep = -1;

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (230.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");

  gnuplot = Gnuplot ("multi-rate-first.png");
  Config::SetDefault ("ns3::YansWifiPhy::Standard", StringValue ("holland"));

  NS_LOG_DEBUG ("minstrel");
  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("ideal");
  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);



/*
  // Scenario 3:
  // Initially set them 25 meters apart, stationary
  // Set positionStep parameter of Experiment::Rung to 0 
  // This is a sanity check

  myPositionStep = 0;
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (25.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  wifiMac.SetType ("ns3::AdhocWifiMac");

  gnuplot = Gnuplot ("multi-rate-first.png");
  Config::SetDefault ("ns3::YansWifiPhy::Standard", StringValue ("holland"));

  NS_LOG_DEBUG ("minstrel");
  experiment = Experiment ("minstrel");
  wifi.SetRemoteStationManager ("ns3::MinstrelWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("ideal");
  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility, myPositionStep);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);
  */

  return 0;
}
