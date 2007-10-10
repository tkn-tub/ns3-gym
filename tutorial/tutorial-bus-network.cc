
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
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/udp-echo-client.h"
#include "ns3/udp-echo-server.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/ascii-trace.h"

#include "bus-network.h"

NS_LOG_COMPONENT_DEFINE ("BusNetworkSimulation");

using namespace ns3;

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("BusNetworkSimulation", LOG_LEVEL_ALL);
  LogComponentEnable ("BusNetwork", LOG_LEVEL_ALL);

  NS_LOG_INFO ("Bus Network Simulation");

  BusNetwork busNetwork (Ipv4Mask ("255.255.0.0"), Ipv4Address ("10.1.0.0"),
    Ipv4Address ("0.0.0.0"), DataRate(10000000), MilliSeconds(20), 10);

  uint32_t port = 7;

  Ptr<Node> n0 = busNetwork.GetNode (0);
  Ptr<UdpEchoClient> client = Create<UdpEchoClient> (n0, "10.1.0.1", port, 
    1, Seconds(1.), 1024);

  Ptr<Node> n1 = busNetwork.GetNode (1);
  Ptr<UdpEchoServer> server = Create<UdpEchoServer> (n1, port);

  server->Start(Seconds(1.));
  client->Start(Seconds(2.));

  server->Stop (Seconds(10.));
  client->Stop (Seconds(10.));

  AsciiTrace asciitrace ("tutorial.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  Simulator::Run ();
  Simulator::Destroy ();
}
