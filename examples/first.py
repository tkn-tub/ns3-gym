# /*
#  * This program is free software; you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License version 2 as
#  * published by the Free Software Foundation;
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program; if not, write to the Free Software
#  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#  */

import ns3

ns3.LogComponentEnable("UdpEchoClientApplication", ns3.LOG_LEVEL_INFO)
ns3.LogComponentEnable("UdpEchoServerApplication", ns3.LOG_LEVEL_INFO)

ns3.RandomVariable.UseGlobalSeed(1, 1, 2, 3, 5, 8)

nodes = ns3.NodeContainer()
nodes.Create(2)

pointToPoint = ns3.PointToPointHelper()
pointToPoint.SetDeviceAttribute("DataRate", ns3.StringValue("5Mbps"))
pointToPoint.SetChannelAttribute("Delay", ns3.StringValue("2ms"))

devices = pointToPoint.Install(nodes)

stack = ns3.InternetStackHelper()
stack.Install(nodes)

address = ns3.Ipv4AddressHelper()
address.SetBase(ns3.Ipv4Address("10.1.1.0"), ns3.Ipv4Mask("255.255.255.0"))

interfaces = address.Assign (devices);

echoServer = ns3.UdpEchoServerHelper(9)

serverApps = echoServer.Install(nodes.Get(1))
serverApps.Start(ns3.Seconds(1.0))
serverApps.Stop(ns3.Seconds(10.0))

echoClient = ns3.UdpEchoClientHelper(interfaces.GetAddress(1), 9)
echoClient.SetAttribute("MaxPackets", ns3.UintegerValue(1))
echoClient.SetAttribute("Interval", ns3.TimeValue(ns3.Seconds (1.0)))
echoClient.SetAttribute("PacketSize", ns3.UintegerValue(1024))

clientApps = echoClient.Install(nodes.Get(0))
clientApps.Start(ns3.Seconds(2.0))
clientApps.Stop(ns3.Seconds(10.0))

ns3.Simulator.Run()
ns3.Simulator.Destroy()

