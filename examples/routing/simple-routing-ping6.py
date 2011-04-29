#
# Copyright (c) 2008-2009 Strasbourg University
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Author: David Gross <gdavid.devel@gmail.com>
#         Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
#

#
# Network topology:
# 
#             n0   r    n1
#             |    _    |
#             ====|_|====
#                router
#

from ns.applications import *
from ns.core import *
from ns.csma import *
from ns.internet import *
from ns.network import *

def main(argv):

    cmd = CommandLine();

    cmd.Parse(argv);

    # Create nodes
    print "Create nodes"
    n0 = Node();
    r = Node();
    n1 = Node();

    net1 = NodeContainer();
    net1.Add(n0);
    net1.Add(r);
    net2 = NodeContainer();
    net2.Add(r);
    net2.Add(n1);
    all = NodeContainer();
    all.Add(n0);
    all.Add(r);
    all.Add(n1);

    # Create IPv6 Internet Stack
    internetv6 = InternetStackHelper();
    internetv6.Install(all);

    # Create channels
    csma = CsmaHelper();
    csma.SetChannelAttribute("DataRate", DataRateValue(DataRate(5000000)));
    csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));
    d1 = csma.Install(net1);
    d2 = csma.Install(net2);

    # Create networks and assign IPv6 Addresses
    print "Addressing"
    ipv6 = Ipv6AddressHelper();
    ipv6.NewNetwork(Ipv6Address("2001:1::"), Ipv6Prefix(64));
    i1 = ipv6.Assign(d1);
    i1.SetRouter(1, True);
    ipv6.NewNetwork(Ipv6Address("2001:2::"), Ipv6Prefix(64));
    i2 = ipv6.Assign(d2);
    i2.SetRouter(0, True);

    # Create a Ping6 application to send ICMPv6 echo request from n0 to n1 via r 
    print "Application"
    packetSize = 1024;
    maxPacketCount = 5;
    interPacketInterval = Seconds(1.);
    ping6 = Ping6Helper();

    ping6.SetLocal(i1.GetAddress(0, 1));
    ping6.SetRemote(i2.GetAddress(1, 1)); 

    ping6.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    ping6.SetAttribute("Interval", TimeValue(interPacketInterval));
    ping6.SetAttribute("PacketSize", UintegerValue(packetSize));

    apps = ping6.Install(NodeContainer(net1.Get(0)));
    apps.Start(Seconds(2.0));
    apps.Stop(Seconds(20.0));

    print "Tracing"
    ascii = AsciiTraceHelper()
    csma.EnableAsciiAll(ascii.CreateFileStream("simple-routing-ping6.tr"))
    csma.EnablePcapAll("simple-routing-ping6", True)

    # Run Simulation     
    Simulator.Run()
    Simulator.Destroy()

if __name__ == '__main__':
    import sys
    main(sys.argv)

