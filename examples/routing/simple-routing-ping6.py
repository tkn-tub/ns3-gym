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

import ns3;

def main(argv):

    cmd = ns3.CommandLine();

    cmd.Parse(argv);

    # Create nodes
    print "Create nodes"
    n0 = ns3.Node();
    r = ns3.Node();
    n1 = ns3.Node();

    net1 = ns3.NodeContainer();
    net1.Add(n0);
    net1.Add(r);
    net2 = ns3.NodeContainer();
    net2.Add(r);
    net2.Add(n1);
    all = ns3.NodeContainer();
    all.Add(n0);
    all.Add(r);
    all.Add(n1);

    # Create IPv6 Internet Stack
    internetv6 = ns3.InternetStackHelper();
    internetv6.Install(all);

    # Create channels
    csma = ns3.CsmaHelper();
    csma.SetChannelAttribute("DataRate", ns3.DataRateValue(ns3.DataRate(5000000)));
    csma.SetChannelAttribute("Delay", ns3.TimeValue(ns3.MilliSeconds(2)));
    d1 = csma.Install(net1);
    d2 = csma.Install(net2);

    # Create networks and assign IPv6 Addresses
    print "Addressing"
    ipv6 = ns3.Ipv6AddressHelper();
    ipv6.NewNetwork(ns3.Ipv6Address("2001:1::"), ns3.Ipv6Prefix(64));
    i1 = ipv6.Assign(d1);
    i1.SetRouter(1, True);
    ipv6.NewNetwork(ns3.Ipv6Address("2001:2::"), ns3.Ipv6Prefix(64));
    i2 = ipv6.Assign(d2);
    i2.SetRouter(0, True);

    # Create a Ping6 application to send ICMPv6 echo request from n0 to n1 via r 
    print "Application"
    packetSize = 1024;
    maxPacketCount = 5;
    interPacketInterval = ns3.Seconds(1.);
    ping6 = ns3.Ping6Helper();

    ping6.SetLocal(i1.GetAddress(0, 1));
    ping6.SetRemote(i2.GetAddress(1, 1)); 

    ping6.SetAttribute("MaxPackets", ns3.UintegerValue(maxPacketCount));
    ping6.SetAttribute("Interval", ns3.TimeValue(interPacketInterval));
    ping6.SetAttribute("PacketSize", ns3.UintegerValue(packetSize));

    apps = ping6.Install(ns3.NodeContainer(net1.Get(0)));
    apps.Start(ns3.Seconds(2.0));
    apps.Stop(ns3.Seconds(20.0));

    print "Tracing"
    ascii = ns3.AsciiTraceHelper()
    csma.EnableAsciiAll(ascii.CreateFileStream("simple-routing-ping6.tr"))
    csma.EnablePcapAll("simple-routing-ping6", True)

    # Run Simulation     
    ns3.Simulator.Run()
    ns3.Simulator.Destroy()

if __name__ == '__main__':
    import sys
    main(sys.argv)

