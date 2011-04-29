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

from ns.applications import *
from ns.core import *
from ns.internet import *
from ns.network import *
from ns.point_to_point import *

LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO)
LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO)

nodes = NodeContainer()
nodes.Create(2)

pointToPoint = PointToPointHelper()
pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"))
pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"))

devices = pointToPoint.Install(nodes)

stack = InternetStackHelper()
stack.Install(nodes)

address = Ipv4AddressHelper()
address.SetBase(Ipv4Address("10.1.1.0"), Ipv4Mask("255.255.255.0"))

interfaces = address.Assign (devices);

echoServer = UdpEchoServerHelper(9)

serverApps = echoServer.Install(nodes.Get(1))
serverApps.Start(Seconds(1.0))
serverApps.Stop(Seconds(10.0))

echoClient = UdpEchoClientHelper(interfaces.GetAddress(1), 9)
echoClient.SetAttribute("MaxPackets", UintegerValue(1))
echoClient.SetAttribute("Interval", TimeValue(Seconds (1.0)))
echoClient.SetAttribute("PacketSize", UintegerValue(1024))

clientApps = echoClient.Install(nodes.Get(0))
clientApps.Start(Seconds(2.0))
clientApps.Stop(Seconds(10.0))

Simulator.Run()
Simulator.Destroy()

