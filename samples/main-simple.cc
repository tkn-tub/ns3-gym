#include <iostream>

#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket.h"
#include "ns3/nstime.h"

using namespace ns3;

static void TestOne (void)
{
  InternetNode *a = new InternetNode ();

  delete a;
}

static void TestTwo (void)
{
  InternetNode a = InternetNode ();

  InternetNode *b = a.Copy ();

  delete b;
}

static void
SmallTests (void)
{
  TestOne ();
  TestTwo ();
}

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

int main (int argc, char *argv[])
{
  SmallTests ();

  InternetNode *a = new InternetNode ();
  
  UdpSocket *sink = new UdpSocket (a);
  sink->Bind (80);

  UdpSocket *source = new UdpSocket (a);
  source->SetDefaultDestination (Ipv4Address::GetLoopback (), 80);

  GenerateTraffic (source, 500);
  PrintTraffic (sink);


  Simulator::Run ();

  Simulator::Destroy ();


  delete a;
  delete source;
  delete sink;

  return 0;
}
