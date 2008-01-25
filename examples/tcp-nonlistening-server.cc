/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

//
// Network topology
//
//           100Kb/s, 10ms       1Mb/s, 10ms
//       n0-----------------n1-----------------n2
//
//
// - Tracing of queues and packet receptions to file 
//   "tcp-nonlistening-server.tr"
// - pcap traces also generated in the following files
//   "tcp-nonlistening-server.pcap-$n-$i" where n and i represent node and interface numbers respectively
//  Usage (e.g.): ./waf --run tcp-nonlistening-server

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/command-line.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "ns3/log.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/internet-node.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/point-to-point-topology.h"
#include "ns3/onoff-application.h"
#include "ns3/packet-sink.h"
#include "ns3/error-model.h"

#include "ns3/tcp.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpNonListeningServer");

void ConnectionSucceededCallback (Ptr<Socket> localSocket) 
{
  uint32_t nBytes = 2000;
  uint8_t data[nBytes];
  for(uint32_t i = 0; i < nBytes; ++i)
  {
    char m = 'A';
    data[i] = m;
  } //put something interesting in the packets ABCDEF...
  localSocket->Send (data, nBytes);
}

void ConnectionFailedCallback (Ptr<Socket> localSocket) 
{
  NS_LOG_ERROR("Connection failed at time " << Simulator::Now ().GetSeconds ());
}

void StartFlow(Ptr<Socket> localSocket, uint16_t servPort)
{
  NS_LOG_LOGIC(std::endl << "Connection attempt at time " <<  
    Simulator::Now ().GetSeconds () << std::endl);
  localSocket->Connect (InetSocketAddress ("10.1.2.2", servPort));
  localSocket->SetConnectCallback (
    MakeCallback (&ConnectionSucceededCallback),
    MakeCallback (&ConnectionFailedCallback), 
    MakeNullCallback<void, Ptr<Socket> > () );
}

int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //LogComponentEnable("TcpL4Protocol", LOG_LEVEL_ALL);
  //LogComponentEnable("TcpSocket", LOG_LEVEL_ALL);
  LogComponentEnable("TcpNonListeningServer", LOG_LEVEL_ALL);

  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine::Parse (argc, argv);

  // Here, we will explicitly create three nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> (); 
  Ptr<Node> n2 = Create<InternetNode> ();

  // We create the channels first without any IP addressing information
  Ptr<PointToPointChannel> channel0 = 
    PointToPointTopology::AddPointToPointLink (
    n0, n1, DataRate(1000000), MilliSeconds(10));
  
  // Later, we add IP addresses.  
  PointToPointTopology::AddIpv4Addresses (
      channel0, n0, Ipv4Address("10.1.3.1"),
      n1, Ipv4Address("10.1.3.2"));

  Ptr<PointToPointChannel> channel1 = 
      PointToPointTopology::AddPointToPointLink (
      n1, n2, DataRate(100000), MilliSeconds(10));
  
  PointToPointTopology::AddIpv4Addresses (
      channel1, n1, Ipv4Address("10.1.2.1"),
      n2, Ipv4Address("10.1.2.2"));
  
  // Finally, we add static routes.  These three steps (Channel and
  // NetDevice creation, IP Address assignment, and routing) are 
  // separated because there may be a need to postpone IP Address
  // assignment (emulation) or modify to use dynamic routing
  PointToPointTopology::AddIpv4Routes(n0, n1, channel0);
  PointToPointTopology::AddIpv4Routes(n1, n2, channel1);
  Ptr<Ipv4> ipv4;
  ipv4 = n0->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->SetDefaultRoute (Ipv4Address ("10.1.3.2"), 1);
  ipv4 = n2->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->SetDefaultRoute (Ipv4Address ("10.1.2.1"), 1);


  ///////////////////////////////////////////////////////////////////////////
  // Simulation 1
  // 
  // Send 2000 bytes over a connection to server port 500 at time 0
  // Should observe SYN exchange, two data segments, and FIN exchange
  //
  ///////////////////////////////////////////////////////////////////////////

  uint16_t servPort = 500;

  Ptr<SocketFactory> socketFactory = 
    n0->QueryInterface<SocketFactory> (Tcp::iid);
  Ptr<Socket> localSocket = socketFactory->CreateSocket ();
  localSocket->Bind ();

#ifdef NOTFORTHISSCRIPT
  // Create an optional packet sink to receive these packets
  Ptr<PacketSink> sink = Create<PacketSink> (
    n2,
    InetSocketAddress (Ipv4Address::GetAny (), servPort),
    "Tcp");
  // Start the sink
  sink->Start (Seconds (0.0));
  sink->Stop (Seconds (10.0));
#endif

  Simulator::Schedule(Seconds(0), &StartFlow, localSocket, servPort);

  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the simple-examples.tr file
  AsciiTrace asciitrace ("tcp-nonlistening-server.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named 
  // simple-examples.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("tcp-nonlistening-server.pcap");
  pcaptrace.TraceAllIp ();


  Simulator::StopAt (Seconds(1000));
  Simulator::Run ();
  Simulator::Destroy ();
}
