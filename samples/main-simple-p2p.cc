#include <iostream>
#include <string>

#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket.h"
#include "ns3/nstime.h"
#include "ns3/p2p-channel.h"
#include "ns3/p2p-net-device.h"
#include "ns3/ipv4.h"
#include "ns3/arp-ipv4-interface.h"
#include "ns3/ipv4-route.h"

using namespace ns3;


static void
GenerateTraffic (UdpSocket *socket, uint32_t size)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, tx bytes=" << size << std::endl;
  socket->SendDummy (size);
  if (size > 0)
    {
      Simulator::Schedule (Seconds (0.5), &GenerateTraffic, socket, size - 50);
    }
}

static void
UdpSocketPrinter (UdpSocket *socket, uint32_t size, Ipv4Address from, uint16_t fromPort)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, rx bytes=" << size << std::endl;
}

static void
PrintTraffic (UdpSocket *socket)
{
  socket->SetDummyRxCallback (MakeCallback (&UdpSocketPrinter));
}

static void
DestroyP2PNetDevice (P2PNetDevice *dev)
{
  delete dev;
}

static void
PrintRoutingTable (InternetNode *a, std::string name)
{
  Ipv4 *ipv4 = a->GetIpv4 ();
  std::cout << "routing table start node=" << name << std::endl;
  for (uint32_t i = 0; i < ipv4->GetNRoutes (); i++)
    {
      Ipv4Route *route = ipv4->GetRoute (i);
      std::cout << (*route) << std::endl;
    }
  std::cout << "routing table end" << std::endl;
}

static void
AddP2PLink (InternetNode *a, InternetNode *b)
{
  P2PChannel * channel = new P2PChannel (MilliSeconds (20), 1000);
  P2PNetDevice *devA = channel->CreateNetDevice (a, MacAddress ("00:00:00:00:00:01"));
  P2PNetDevice *devB = channel->CreateNetDevice (b, MacAddress ("00:00:00:00:00:02"));
  Simulator::ScheduleDestroy (&DestroyP2PNetDevice, devA);
  Simulator::ScheduleDestroy (&DestroyP2PNetDevice, devB);
  uint32_t indexA = a->GetIpv4 ()->AddInterface (new ArpIpv4Interface (a, devA));
  uint32_t indexB = b->GetIpv4 ()->AddInterface (new ArpIpv4Interface (b, devB));
  Ipv4Address ipa = Ipv4Address ("192.168.0.2");
  Ipv4Address ipb = Ipv4Address ("192.168.0.3");
  a->GetIpv4 ()->AddHostRouteTo (ipb, indexA);
  b->GetIpv4 ()->AddHostRouteTo (ipa, indexB);
  PrintRoutingTable (a, "a");
  PrintRoutingTable (b, "b");
}

int main (int argc, char *argv[])
{

  InternetNode *a = new InternetNode ();
  InternetNode *b = new InternetNode ();

  AddP2PLink (a, b);
  
  UdpSocket *sink = new UdpSocket (a);
  sink->Bind (80);
  UdpSocket *source = new UdpSocket (b);
  source->SetDefaultDestination (Ipv4Address ("192.168.0.2"), 80);

  GenerateTraffic (source, 500);
  PrintTraffic (sink);


  Simulator::Run ();

  Simulator::Destroy ();


  delete a;
  delete source;
  delete sink;

  return 0;
}
