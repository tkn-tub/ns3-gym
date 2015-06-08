/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <iostream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

static void
GenerateTraffic (Ptr<Socket> socket, uint32_t size)
{
  std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, tx bytes=" << size << std::endl;
  socket->Send (Create<Packet> (size));
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
SocketPrinter (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while ((packet = socket->Recv ()))
    { 
      std::cout << "at=" << Simulator::Now ().GetSeconds () << "s, rx bytes=" << packet->GetSize () << std::endl;
    }
}

static void
PrintTraffic (Ptr<Socket> socket)
{
  socket->SetRecvCallback (MakeCallback (&SocketPrinter));
}

void
RunSimulation (void)
{
  NodeContainer c;
  c.Create (1);

  InternetStackHelper internet;
  internet.Install (c);


  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (c.Get (0), tid);
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
  sink->Bind (local);

  Ptr<Socket> source = Socket::CreateSocket (c.Get (0), tid);
  InetSocketAddress remote = InetSocketAddress (Ipv4Address::GetLoopback (), 80);
  source->Connect (remote);

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
