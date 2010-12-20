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
//           8Mb/s, 0.1ms       0.8Mb/s, 100ms
//       s1-----------------r1-----------------k1
//
// Example corresponding to simulations in the paper "Simulation-based
// Comparisons of Tahoe, Reno, and SACK TCP"
//
// To look at the time-sequence plots corresponding to the packet traces,
// the tcptrace tool (www.tcptrace.org) with the -S option can produce
// xplot plots (a2b.xpl, b2a.xpl) which can be viewed using xplot
//
// - Tracing of queues and packet receptions to file
//   "tcp-loss-response.tr
// - pcap traces also generated in the following files
//   "tcp-loss-response-s1-0.pcap" and "tcp-loss-response-k1-0.pcap"
//
//  Usage (e.g.): ./waf --run tcp-loss-response
//


#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/helper-module.h"
#include "ns3/node-module.h"
#include "ns3/simulator-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpLossResponse");

// The number of bytes to send in this simulation.
static const uint32_t totalTxBytes = 200000;
static uint32_t currentTxBytes = 0;
// Perform series of 26*40=1040 byte writes (this is a multiple of 26 since
// we want to detect data splicing in the output stream)
static const uint32_t writeSize = 1040;
uint8_t data[writeSize];
// Need to invoke Socket::Close() on sender?
bool needToClose = true;

// These are for starting the writing process, and handling the sending
// socket's notification upcalls (events).  These two together more or less
// implement a sending "Application", although not a proper ns3::Application
// subclass.

void StartFlow (Ptr<Socket>, Ipv4Address, uint16_t);
void WriteUntilBufferFull (Ptr<Socket>, uint32_t);

static void
CwndTracer (uint32_t oldval, uint32_t newval)
{
  NS_LOG_INFO ("Moving cwnd from " << oldval << " to " << newval << " at time " << Simulator::Now ().GetSeconds () << " seconds");
}

int main (int argc, char *argv[])
{
  std::string tcpModel ("ns3::TcpNewReno");
  uint32_t losses = 1;
  bool verbose = false;
  bool tracing = true;

  // Configuration and command line parameter parsing
  CommandLine cmd;
  cmd.AddValue ("tcpModel", "Tcp congestion control model", tcpModel);
  cmd.AddValue ("losses", "number of packet losses", losses);
  cmd.AddValue ("verbose", "turn on selected log components", verbose);
  cmd.AddValue ("tracing", "turn on ascii and pcap tracing", tracing);
  cmd.Parse (argc, argv);

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpModel));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000)); // 1000-byte packet for easier reading
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));

  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("TcpLossResponse", LOG_LEVEL_ALL);
  if (verbose)
    {
      //LogComponentEnableAll (LOG_PREFIX_TIME);
      LogComponentEnable ("ErrorModel", LOG_LEVEL_DEBUG);
      LogComponentEnable ("TcpLossResponse", LOG_LEVEL_ALL);
      LogComponentEnable ("TcpNewReno", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpReno", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpTahoe", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpSocketBase", LOG_LEVEL_INFO);
    }

  // initialize the tx buffer (fill with lowercase a to z)
  for (uint32_t i = 0; i < writeSize; ++i)
    {
      char m = toascii (97 + i % 26);
      data[i] = m;
    }

  ////////////////////////////////////////////////////////
  // Topology construction
  //
  
  // Create three nodes: s1, r1, and k1
  NodeContainer s1r1;
  s1r1.Create (2);
  Names::Add ("s1", s1r1.Get (0));
  Names::Add ("r1", s1r1.Get (1));

  NodeContainer r1k1;
  r1k1.Add (s1r1.Get (1));
  r1k1.Create (1);
  Names::Add ("k1", r1k1.Get (1));

  // Set up TCP/IP stack to all nodes (and create loopback device at device 0)
  InternetStackHelper internet;
  internet.InstallAll ();

  // Connect the nodes
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (8000000)));
  p2p.SetChannelAttribute ("Delay", TimeValue (Seconds (0.0001)));
  NetDeviceContainer dev0 = p2p.Install (s1r1);
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (800000)));
  p2p.SetChannelAttribute ("Delay", TimeValue (Seconds (0.1)));
  NetDeviceContainer dev1 = p2p.Install (r1k1);

  // Add IP addresses to each network interfaces
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4.Assign (dev0);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer ipInterfs = ipv4.Assign (dev1);

  // Set up routes to all nodes
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  ////////////////////////////////////////////////////////
  // Send 20000 (totalTxBytes) bytes from node s1 to node k1
  //

  // Create a packet sink to receive packets on node k1
  uint16_t servPort = 50000; // Destination port number
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), servPort));
  ApplicationContainer apps = sink.Install (r1k1.Get (1));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (100.0));

  // Create a data source to send packets on node s0.
  // Instead of full application, here use the socket directly by
  // registering callbacks in function StarFlow().
  Ptr<Socket> localSocket = Socket::CreateSocket (s1r1.Get (0), TcpSocketFactory::GetTypeId ());
  localSocket->Bind ();
  Simulator::ScheduleNow (&StartFlow, localSocket, ipInterfs.GetAddress (1), servPort);

  // Trace changes to the congestion window (available in Tahoe and descendents)
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndTracer));

  ////////////////////////////////////////////////////////
  // Set up loss model at node k1
  //

  std::list<uint32_t> sampleList;
  switch (losses)
    {
    case 0:
      break;
    case 1:
      // Force a loss for 15th data packet. TCP cwnd will be at 14 segments
      // (14000 bytes) when duplicate acknowledgments start to come.
      sampleList.push_back (16);
      break;
    case 2:
      sampleList.push_back (16);
      sampleList.push_back (17);
      break;
    case 3:
      sampleList.push_back (16);
      sampleList.push_back (17);
      sampleList.push_back (18);
      break;
    case 4:
      sampleList.push_back (16);
      sampleList.push_back (17);
      sampleList.push_back (18);
      sampleList.push_back (19);
      break;
    default:
      NS_FATAL_ERROR ("Program fatal error: loss value " << losses << " not supported.");
      break;
    }

  Ptr<ReceiveListErrorModel> pem = CreateObject<ReceiveListErrorModel> ();
  pem->SetList (sampleList);
  dev1.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (pem));

  // One can toggle the comment for the following line on or off to see the
  // effects of finite send buffer modelling.  One can also change the size of
  // that buffer.
  // localSocket->SetAttribute("SndBufSize", UintegerValue(4096));

  /////////////////////////////////////////////////////////
  // Set up trace and run the simulation
  //

  if (tracing)
    {
      // Ask for ASCII and pcap traces of network traffic
      AsciiTraceHelper ascii;
      //Ptr<OutputStreamWrapper> osw = ascii.CreateFileStream ("tcp-loss-response.tr");
      Ptr<OutputStreamWrapper> osw = Create<OutputStreamWrapper> (&std::clog);
      *(osw->GetStream ()) << std::setprecision(9) << std::fixed;
      p2p.EnableAsciiAll (osw);
//      p2p.EnablePcap ("tcp-loss-response", 0, 0);
//      p2p.EnablePcap ("tcp-loss-response", 2, 0);
    }

  // Finally, set up the simulator to run.  The 1000 second hard limit is a
  // failsafe in case some change above causes the simulation to never end
  Simulator::Stop (Seconds (1000));
  Simulator::Run ();
  Simulator::Destroy ();
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// begin implementation of sending "Application"
void StartFlow (Ptr<Socket> localSocket,
                Ipv4Address servAddress,
                uint16_t servPort)
{
  NS_LOG_LOGIC ("Starting flow at time " <<  Simulator::Now ().GetSeconds ());
  localSocket->Connect (InetSocketAddress (servAddress, servPort)); // connect

  // tell the tcp implementation to call WriteUntilBufferFull again
  // if we blocked and new tx buffer space becomes available
  localSocket->SetSendCallback (MakeCallback (&WriteUntilBufferFull));
  WriteUntilBufferFull (localSocket, localSocket->GetTxAvailable ());
}

void WriteUntilBufferFull (Ptr<Socket> localSocket, uint32_t txSpace)
{
  while (currentTxBytes < totalTxBytes)
    {
      uint32_t left = totalTxBytes - currentTxBytes;
      uint32_t dataOffset = currentTxBytes % writeSize;
      uint32_t toWrite = writeSize - dataOffset;
      uint32_t txAvail = localSocket->GetTxAvailable ();
      toWrite = std::min (toWrite, left);
      toWrite = std::min (toWrite, txAvail);
      if (txAvail == 0)
        {
          NS_LOG_LOGIC ("TCP socket buffer full at " <<  Simulator::Now ().GetSeconds ());
          return;
        };
      NS_LOG_LOGIC ("Submitting " << toWrite << " bytes to TCP socket");
      int amountSent = localSocket->Send (&data[dataOffset], toWrite, 0);
      NS_ASSERT (amountSent > 0);  // Given GetTxAvailable() non-zero, amountSent should not be zero
      currentTxBytes += amountSent;
    }
  if (needToClose)
    {
      NS_LOG_LOGIC ("Close socket at " <<  Simulator::Now ().GetSeconds ());
      localSocket->Close ();
      needToClose = false;
    }
}

