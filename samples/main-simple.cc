#include <iostream>

#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/i-udp.h"
#include "ns3/socket.h"
#include "ns3/nstime.h"

using namespace ns3;

static void
GenerateTraffic (Socket *socket, uint32_t size)
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
SocketPrinter (Socket *socket, uint32_t size, const Ipv4Address &from, uint16_t fromPort)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, rx bytes=" << size << std::endl;
}

static void
PrintTraffic (Socket *socket)
{
  socket->RecvDummy (MakeCallback (&SocketPrinter));
}

void
RunSimulation (void)
{
  Ptr<InternetNode> a = new InternetNode ();

  IUdp *udp;
  udp = a->QueryInterface<IUdp> (IUdp::iid);

  Socket *sink = udp->CreateSocket ();
  sink->Bind (80);

  Socket *source = udp->CreateSocket ();
  source->Connect (Ipv4Address::GetLoopback (), 80);

  udp->Unref ();

  GenerateTraffic (source, 500);
  PrintTraffic (sink);


  Simulator::Run ();

  Simulator::Destroy ();

  sink->Unref ();
  source->Unref ();

  std::cout << "o" << std::endl;
}

int main (int argc, char *argv[])
{
  RunSimulation ();

  return 0;
}
