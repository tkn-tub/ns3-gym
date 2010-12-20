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
//           10Mb/s, 0.1ms      10Mb/s, 0.1ms
//       n0-----------------n1-----------------n2
//
// Testcases for the correctness of TCP operations.
//
//  Usage (e.g.): ./waf --run="tcp-testcases --case=1"
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

NS_LOG_COMPONENT_DEFINE ("TcpTestCases");

// The number of bytes to send in this simulation.
static uint32_t totalTxBytes = 200000;
static uint32_t currentTxBytes = 0;
// Perform series of 26*40=1040 byte writes (this is a multiple of 26 since
// we want to detect data splicing in the output stream)
static const uint32_t writeSize = 1040;
uint8_t data[writeSize];
// Need to close the socket on node 0 upon finishing sending data
static bool needToClose = true;

////////////////////////////////////////////////////////////////////
// Implementing an "application" to send bytes over a TCP connection
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

// cwnd tracer
static void
CwndTracer (uint32_t oldval, uint32_t newval)
{
  NS_LOG_INFO ("Moving cwnd from " << oldval << " to " << newval << " at time " << Simulator::Now ().GetSeconds () << " seconds");
}

int main (int argc, char *argv[])
{
  std::string tcpModel ("ns3::TcpNewReno");
  uint32_t testcase = 1;
  uint32_t verbose = 0;
  bool tracing = true;

  // Configuration and command line parameter parsing
  CommandLine cmd;
  cmd.AddValue ("tcpModel", "TCP congestion control model", tcpModel);
  cmd.AddValue ("testcase", "test case", testcase);
  cmd.AddValue ("verbose", "turn on selected log components", verbose);
  cmd.Parse (argc, argv);

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpModel));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000)); // 1000-byte packet for easier reading
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", UintegerValue (20));

  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("TcpTestCases", LOG_LEVEL_ALL);
  if (verbose)
    {
      //LogComponentEnableAll (LOG_PREFIX_TIME);
      LogComponentEnable ("ErrorModel", LOG_LEVEL_DEBUG);
      LogComponentEnable ("TcpTestCases", LOG_LEVEL_ALL);
      LogComponentEnable ("TcpNewReno", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpReno", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpTahoe", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpSocketBase", (verbose>1)?LOG_LEVEL_ALL:LOG_LEVEL_INFO);
      //LogComponentEnable ("TcpTxBuffer", LOG_LEVEL_ALL);
      //LogComponentEnable ("TcpRxBuffer", LOG_LEVEL_ALL);
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
  
  // Create three nodes
  NodeContainer n0n1;
  n0n1.Create (2);
  Names::Add ("n0", n0n1.Get (0));
  Names::Add ("n1", n0n1.Get (1));

  NodeContainer n1n2;
  n1n2.Add (n0n1.Get (1));
  n1n2.Create (1);
  Names::Add ("n2", n1n2.Get (1));

  // Set up TCP/IP stack to all nodes (and create loopback device at device 0)
  InternetStackHelper internet;
  internet.InstallAll ();

  // Connect the nodes
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (1000000)));
  p2p.SetChannelAttribute ("Delay", TimeValue (Seconds (0.0001)));
  NetDeviceContainer dev0 = p2p.Install (n0n1);
  NetDeviceContainer dev1 = p2p.Install (n1n2);

  // Add IP addresses to each network interfaces
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4.Assign (dev0);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer ipInterfs = ipv4.Assign (dev1);

  // Set up routes to all nodes
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  ////////////////////////////////////////////////////////
  // A flow from node n0 to node n2
  //

  // Create a packet sink to receive packets on node n2
  uint16_t servPort = 50000; // Destination port number
  PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), servPort));
  ApplicationContainer apps = sink.Install (n1n2.Get (1));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (100.0));

  // Create a data source to send packets on node n0
  // Instead of full application, here use the socket directly by
  // registering callbacks in function StarFlow().
  Ptr<Socket> localSocket = Socket::CreateSocket (n0n1.Get (0), TcpSocketFactory::GetTypeId ());
  localSocket->Bind ();
  Simulator::ScheduleNow (&StartFlow, localSocket, ipInterfs.GetAddress (1), servPort);

  // Trace changes to the congestion window (available in Tahoe and descendents)
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndTracer));

  ////////////////////////////////////////////////////////
  // Set up different test cases: Lost model at node n1, different file size
  //

  std::list<uint32_t> dropListN0;
  std::list<uint32_t> dropListN1;
  switch (testcase)
    {
    case 0: // Verify connection establishment
      totalTxBytes = 1000;
      break;
    case 1: // Verify a bigger (100 pkts) transfer: Sliding window operation, etc.
      totalTxBytes = 100*1000;
      break;
    case 2: // Survive a SYN lost
      totalTxBytes = 1000;
      dropListN0.push_back (0);
      break;
    case 3: // Survive a SYN+ACK lost
      totalTxBytes = 2000;
      dropListN1.push_back (0);
      break;
    case 4: // Survive a ACK (last packet in 3-way handshake) lost
      totalTxBytes = 2000;
      dropListN0.push_back (1);
      break;
    case 5: // Immediate FIN upon SYN_RCVD
      totalTxBytes = 0;
      needToClose = false;
      dropListN0.push_back (1); // Hide the ACK in 3WHS
      Simulator::Schedule (Seconds(0.002), &Socket::Close, localSocket);
      break;
    case 6: // Simulated simultaneous close
      totalTxBytes = 5000;
      dropListN1.push_back (5); // Hide the ACK-to-FIN from n2
      break;
    case 7: // FIN check 1: Lost of initiator's FIN. Shall wait until application close.
      needToClose = false;
      totalTxBytes = 5000;
      dropListN0.push_back (7); // Hide the FIN from n0
      Simulator::Schedule (Seconds(0.04), &Socket::Close, localSocket);
      break;
    case 8: // FIN check 2: Lost of responder's FIN. The FIN will resent after last ack timeout
      totalTxBytes = 5000;
      dropListN1.push_back (6); // Hide the FIN from n2
      break;
    default:
      NS_FATAL_ERROR ("Program fatal error: specified test case not supported: " << testcase);
      break;
    }

  Ptr<ReceiveListErrorModel> errN0 = CreateObject<ReceiveListErrorModel> ();
  errN0->SetList (dropListN0);
  dev0.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (errN0));

  Ptr<ReceiveListErrorModel> errN1 = CreateObject<ReceiveListErrorModel> ();
  errN1->SetList (dropListN1);
  dev1.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (errN1));

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
    }

  // Finally, set up the simulator to run.  The 1000 second hard limit is a
  // failsafe in case some change above causes the simulation to never end
  Simulator::Stop (Seconds (1000));
  Simulator::Run ();
  Simulator::Destroy ();
}
