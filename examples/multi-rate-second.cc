/* 
 * Instructions:
 * ./waf --run multi-rate-second
 * gnuplot multi-rate-second.plt 
 *
 * Output: multi-rate-second.eps 
 *
 */

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

NS_LOG_COMPONENT_DEFINE ("Main");

using namespace ns3;

class Experiment
{
public:
  Experiment ();
  Experiment (std::string name);
  Gnuplot2dDataset Run (const WifiHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                        const NqosWifiMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel);
private:
  void ReceivePacket (Ptr<Socket> socket);
  void SetPosition (Ptr<Node> node, Vector position);
  Vector GetPosition (Ptr<Node> node);
  Ptr<Socket> SetupPacketReceive (Ptr<Node> node);
  void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, 
                        uint32_t pktCount, Time pktInterval , Ptr<Node> node);

  uint32_t m_pktsTotal;
  Gnuplot2dDataset m_output;
  bool advanceStep; 
};

Experiment::Experiment ()
{
  advanceStep= true;
}

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
Experiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while (packet = socket->Recv ())
    {
      m_pktsTotal ++;
    }
}

Ptr<Socket>
Experiment::SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
  sink->Bind (local);
  sink->SetRecvCallback (MakeCallback (&Experiment::ReceivePacket, this));
  return sink;
}

void
Experiment::GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, 
                             uint32_t pktCount, Time pktInterval, Ptr<Node> node )
{
  Vector pos = GetPosition(node);

  if (pktCount > 0)
    {
      ///To simulate nodes moving in and out of transmission constantly
      if(pos.x <= 230 && advanceStep)
        {
          ///keep moving away
          pos.x += .1;
          SetPosition(node, pos);
        }
      else
        {
          if(pos.x < 150)
            {
              advanceStep=true;
            }
          else
            {
              advanceStep = false;
            }			

          ///moving back in
          pos.x -= .1;
          SetPosition(node, pos);
        }
      socket->Send (Create<Packet> (pktSize));
      Simulator::Schedule (pktInterval, &Experiment::GenerateTraffic, this, 
                           socket, pktSize,pktCount-1, pktInterval, node);
    }
  else
    {
      m_output.Add((Simulator::Now()).GetSeconds(), m_pktsTotal); 
    }
}

Gnuplot2dDataset
Experiment::Run (const WifiHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                 const NqosWifiMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel)
{
  m_pktsTotal = 0;

  NodeContainer c;
  c.Create (2);

  YansWifiPhyHelper phy = wifiPhy;
  phy.SetChannel (wifiChannel.Create ());

  NqosWifiMacHelper mac = wifiMac;
  NetDeviceContainer devices = wifi.Install (phy, mac, c);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  InternetStackHelper internet;
  internet.Install (c);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiNodesInterface = ipv4.Assign (devices);


  Ptr<Socket> recvSink = SetupPacketReceive (c.Get (0));

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> source = Socket::CreateSocket (c.Get (1), tid);
  InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);
  source->Connect (remote);
  uint32_t packetSize = 1014;
  uint32_t maxPacketCount = 1000;
  Time interPacketInterval = Seconds (1.);

  Ptr<Node> n1 = c.Get(0);
  Ptr<Ipv4> ipv41 = n1->GetObject<Ipv4> ();
  // parameters for Ipv4::SetDown and SetUp
  // The first ifIndex is 0 for loopback, then the first p2p is numbered 1,
  // then the next p2p is numbered 2

  double downTime = 0.0;
  for (int i= 1; i <= 100; i++) 
    {

      Simulator::Schedule (Seconds (i), &Experiment::GenerateTraffic,
                           this, source, packetSize, maxPacketCount,interPacketInterval, c.Get(1));
      if ( i % 10 == 0 )
        {
          ///bring a network interface down 
          Simulator::Schedule (Seconds (i+.1), &Ipv4::SetDown, ipv41, 1); 
	
          //duration of the down time
          downTime += .1;

          ///bring a network interface up
          Simulator::Schedule (Seconds (i + downTime), &Ipv4::SetUp, ipv41, 1); 
        }
    }

  Simulator::Run ();
  Simulator::Destroy ();

  return m_output;
}

int main (int argc, char *argv[])
{
  std::ofstream outfile ("multi-rate-second.plt");

  std::vector <std::string> ratesControl;
  ratesControl.push_back ("Minstrel");
  ratesControl.push_back ("Ideal");

  std::vector <std::string> wifiManager;
  wifiManager.push_back("ns3::MinstrelWifiManager");
  wifiManager.push_back("ns3::IdealWifiManager");

  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));

  CommandLine cmd;
  cmd.Parse (argc, argv);

  Gnuplot gnuplot = Gnuplot ("multi-rate-second.eps");
  
  for (uint32_t i = 0; i < ratesControl.size(); i++)
  {
    std::cout << ratesControl[i] << std::endl;
    std::cout << wifiManager[i] << std::endl;
    Gnuplot2dDataset dataset (ratesControl[i]);
    dataset.SetStyle (Gnuplot2dDataset::LINES);
    Experiment experiment;

    WifiHelper wifi;
    NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
    YansWifiChannelHelper wifiChannel  = YansWifiChannelHelper::Default ();

    wifiMac.SetType ("ns3::AdhocWifiMac");

    NS_LOG_DEBUG (ratesControl[i]);

    experiment = Experiment (ratesControl[i]);
    wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
    wifi.SetRemoteStationManager (wifiManager[i]);
    dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
    gnuplot.AddDataset (dataset);

  }
  gnuplot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  gnuplot.SetLegend ("Time ", "Number of packets received");
  gnuplot.SetExtra  ("set xrange [1000:1100]");
  gnuplot.SetTitle ("Number of Packets Received vs Time");
  gnuplot.GenerateOutput (outfile);
  outfile.close ();

  return 0;
}
