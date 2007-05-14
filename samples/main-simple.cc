#include <iostream>

#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/i-socket-factory.h"
#include "ns3/socket.h"
#include "ns3/nstime.h"

using namespace ns3;

static void
GenerateTraffic (Ptr<Socket> socket, uint32_t size)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, tx bytes=" << size << std::endl;
  socket->Send (0, size);
  if (size > 0)
    {
      Simulator::Schedule (Seconds (0.5), &GenerateTraffic, socket, size - 50);
    }
  else
    {
      socket->Close ();
    }
}

static void
SocketPrinter (Ptr<Socket> socket, uint32_t size, const Ipv4Address &from, uint16_t fromPort)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, rx bytes=" << size << std::endl;
}

static void
PrintTraffic (Ptr<Socket> socket)
{
  socket->RecvDummy (MakeCallback (&SocketPrinter));
}

void
RunSimulation (void)
{
  Ptr<INode> a = MakeInternetNode ();

  InterfaceId iid = InterfaceId::LookupByName ("IUdp");
  Ptr<ISocketFactory> socketFactory = a->QueryInterface<ISocketFactory> (iid);

  Ptr<Socket> sink = socketFactory->CreateSocket ();
  sink->Bind (80);

  Ptr<Socket> source = socketFactory->CreateSocket ();
  source->Connect (Ipv4Address::GetLoopback (), 80);

  GenerateTraffic (source, 500);
  PrintTraffic (sink);


  Simulator::Run ();

  Simulator::Destroy ();
}

int main (int argc, char *argv[])
{
  RunSimulation ();

  return 0;
}
