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
//           10Mb/s, 10ms       10Mb/s, 10ms
//       n0-----------------n1-----------------n2
//
//
// - Tracing of queues and packet receptions to file 
//   "tcp-large-transfer.tr"
// - pcap traces also generated in the following files
//   "tcp-large-transfer-$n-$i.pcap" where n and i represent node and interface
// numbers respectively
//  Usage (e.g.): ./waf --run tcp-large-transfer


#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/helper-module.h"
#include "ns3/node-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/simulator-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpLargeTransfer");

// The number of bytes to send in this simulation.
static uint32_t txBytes = 2000000;

// These are for starting the writing process, and handling the sending 
// socket's notification upcalls (events).  These two together more or less
// implement a sending "Application", although not a proper ns3::Application
// subclass.

void StartFlow(Ptr<Socket>, Ipv4Address, uint16_t);
void WriteUntilBufferFull (Ptr<Socket>, uint32_t);

int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //  LogComponentEnable("TcpL4Protocol", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpSocketImpl", LOG_LEVEL_ALL);
  //  LogComponentEnable("PacketSink", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpLargeTransfer", LOG_LEVEL_ALL);


  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Here, we will explicitly create three nodes.  The first container contains
  // nodes 0 and 1 from the diagram above, and the second one contains nodes
  // 1 and 2.  This reflects the channel connectivity, and will be used to
  // install the network interfaces and connect them with a channel.
  NodeContainer n0n1;
  n0n1.Create (2);

  NodeContainer n1n2;
  n1n2.Add (n0n1.Get (1));
  n1n2.Create (1);

  // We create the channels first without any IP addressing information
  // First make and configure the helper, so that it will put the appropriate
  // attributes on the network interfaces and channels we are about to install.
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate(10000000)));
  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds(10)));

  // And then install devices and channels connecting our topology.
  NetDeviceContainer dev0 = p2p.Install (n0n1);
  NetDeviceContainer dev1 = p2p.Install (n1n2);

  // Now add ip/tcp stack to all nodes.
  NodeContainer allNodes = NodeContainer (n0n1, n1n2.Get (1));
  InternetStackHelper internet;
  internet.Install (allNodes);

  // Later, we add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4.Assign (dev0);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer ipInterfs = ipv4.Assign (dev1);

  // and setup ip routing tables to get total ip-level connectivity.
  GlobalRouteManager::PopulateRoutingTables ();

  ///////////////////////////////////////////////////////////////////////////
  // Simulation 1
  //
  // Send 2000000 bytes over a connection to server port 50000 at time 0
  // Should observe SYN exchange, a lot of data segments and ACKS, and FIN 
  // exchange.  FIN exchange isn't quite compliant with TCP spec (see release
  // notes for more info)
  //
  ///////////////////////////////////////////////////////////////////////////

  uint16_t servPort = 50000;

  // Create a packet sink to receive these packets on n2...
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), servPort));

  ApplicationContainer apps = sink.Install (n1n2.Get (1));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (3.0));

  // Create a source to send packets from n0.  Instead of a full Application
  // and the helper APIs you might see in other example files, this example
  // will use sockets directly and register some socket callbacks as a sending
  // "Application".

  // Create and bind the socket...
  Ptr<Socket> localSocket =
      Socket::CreateSocket (n0n1.Get (0), TcpSocketFactory::GetTypeId ());
  localSocket->Bind ();

  // ...and schedule the sending "Application"; This is similar to what an 
  // ns3::Application subclass would do internally.
  Simulator::ScheduleNow (&StartFlow, localSocket,
                          ipInterfs.GetAddress (1), servPort);

  // One can toggle the comment for the following line on or off to see the
  // effects of finite send buffer modelling.  One can also change the size of
  // said buffer.

  //localSocket->SetAttribute("SndBufSize", UintegerValue(4096));

  //Ask for ASCII and pcap traces of network traffic
  std::ofstream ascii;
  ascii.open ("tcp-large-transfer.tr");
  PointToPointHelper::EnableAsciiAll (ascii);

  PointToPointHelper::EnablePcapAll ("tcp-large-transfer");

  // Finally, set up the simulator to run.  The 1000 second hard limit is a
  // failsafe in case some change above causes the simulation to never end
  Simulator::Stop (Seconds(1000));
  Simulator::Run ();
  Simulator::Destroy ();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//begin implementation of sending "Application"
void StartFlow(Ptr<Socket> localSocket,
               Ipv4Address servAddress,
               uint16_t servPort)
{
  NS_LOG_LOGIC("Starting flow at time " <<  Simulator::Now ().GetSeconds ());
  localSocket->Connect (InetSocketAddress (servAddress, servPort));//connect

  // tell the tcp implementation to call WriteUntilBufferFull again
  // if we blocked and new tx buffer space becomes available
  localSocket->SetSendCallback (MakeCallback (&WriteUntilBufferFull));
  WriteUntilBufferFull (localSocket, txBytes);
}

void WriteUntilBufferFull (Ptr<Socket> localSocket, uint32_t txSpace)
{
  // Perform series of 1040 byte writes (this is a multiple of 26 since
  // we want to detect data splicing in the output stream)
  uint32_t writeSize = 1040;
  uint8_t data[writeSize];

  while (txBytes > 0) {
    uint32_t curSize= txBytes > writeSize ? writeSize : txBytes;
    if (curSize > txSpace)
      curSize = txSpace;
    for(uint32_t i = 0; i < curSize; ++i)
    {
      char m = toascii (97 + i % 26);
      data[i] = m;
    }
    int amountSent = localSocket->Send (data, curSize, 0);
    if(amountSent < 0)
      {
        // we will be called again when new tx space becomes available.
        std::cout << "Socket blocking, " << txBytes << " left to write, returning" << std::endl;
        return;
      }
    txBytes -= curSize;
    if (amountSent != (int)curSize)
      {
        std::cout << "Short Write, returning" << std::endl;
        return;
      }
  }
  localSocket->Close ();
}
