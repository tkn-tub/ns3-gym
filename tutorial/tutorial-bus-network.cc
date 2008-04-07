
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

#include <fstream>

#include "ns3/core-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/simulator-module.h"
#include "ns3/global-route-manager.h"

NS_LOG_COMPONENT_DEFINE ("BusNetworkSimulation");

using namespace ns3;

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("BusNetworkSimulation", LOG_LEVEL_ALL);

  NS_LOG_INFO ("Bus Network Simulation");

  NodeContainer n;
  n.Create (10);

  InternetStackHelper internet;
  internet.Install (n);

  CsmaHelper csma;
  csma.SetChannelParameter ("BitRate", DataRate(10000000));
  csma.SetChannelParameter ("Delay", MilliSeconds(20));
  NetDeviceContainer nd = csma.Install (n);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.0.0", "255.255.0.0", "0.0.0.3");
  Ipv4InterfaceContainer i = ipv4.Assign (nd);

  uint32_t port = 7;
  UdpEchoClientHelper client;
  client.SetRemote (i.GetAddress (1), port);
  client.SetAppAttribute ("MaxPackets", Uinteger (1));
  client.SetAppAttribute ("Interval", Seconds (1.0));
  client.SetAppAttribute ("PacketSize", Uinteger (1024));
  ApplicationContainer apps = client.Install (n.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  UdpEchoServerHelper server;
  server.SetPort (port);
  apps = server.Install (n.Get (1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  GlobalRouteManager::PopulateRoutingTables ();

  std::ofstream ascii;
  ascii.open ("tutorial.tr");
  CsmaHelper::EnableAscii (ascii);

  Simulator::Run ();
  Simulator::Destroy ();
}
