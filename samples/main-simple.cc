#include <iostream>

#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/datagram-socket.h"
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
GenerateTraffic (DatagramSocket *socket, uint32_t size)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, tx bytes=" << size << std::endl;
  socket->SendDummy (size);
  if (size > 0)
    {
      Simulator::Schedule (Seconds (0.5), &GenerateTraffic, socket, size - 50);
    }
}

static void
DatagramSocketPrinter (DatagramSocket *socket, uint32_t size, Ipv4Address from, uint16_t fromPort)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, rx bytes=" << size << std::endl;
}

static void
PrintTraffic (DatagramSocket *socket)
{
  socket->SetDummyRxCallback (MakeCallback (&DatagramSocketPrinter));
}

int main (int argc, char *argv[])
{
  SmallTests ();

  InternetNode *a = new InternetNode ();
  
  DatagramSocket *sink = new DatagramSocket (a);
  sink->Bind (80);

  DatagramSocket *source = new DatagramSocket (a);
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
