# /*
#  * This program is free software; you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License version 2 as
#  * published by the Free Software Foundation
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

# Network topology
#
#        n0     n1  
#        |      | 
#       ----------
#       | Switch |
#       ----------
#        |      | 
#        n2     n3  
#
#
# - CBR/UDP flows from n0 to n1 and from n3 to n0
# - DropTail queues 
# - Tracing of queues and packet receptions to file "csma-bridge.tr"

import ns3


def main(argv):

    #
    # Allow the user to override any of the defaults and the above Bind() at
    # run-time, via command-line arguments
    #
    cmd = ns3.CommandLine()
    cmd.Parse(argv)

    #
    # Explicitly create the nodes required by the topology(shown above).
    #
    #print "Create nodes."
    terminals = ns3.NodeContainer()
    terminals.Create(4)

    csmaSwitch = ns3.NodeContainer()
    csmaSwitch.Create(1)

    #print "Build Topology"
    csma = ns3.CsmaHelper()
    csma.SetChannelAttribute("DataRate", ns3.DataRateValue(ns3.DataRate(5000000)))
    csma.SetChannelAttribute("Delay", ns3.TimeValue(ns3.MilliSeconds(2)))

    # Create the csma links, from each terminal to the switch

    terminalDevices = ns3.NetDeviceContainer()
    switchDevices = ns3.NetDeviceContainer()

    for i in range(4):
        link = csma.Install(ns3.NodeContainer(ns3.NodeContainer(terminals.Get(i)), csmaSwitch))
        terminalDevices.Add(link.Get(0))
        switchDevices.Add(link.Get(1))

    # Create the bridge netdevice, which will do the packet switching
    switchNode = csmaSwitch.Get(0)
    bridgeDevice = ns3.BridgeNetDevice()
    switchNode.AddDevice(bridgeDevice)

    for portIter in range(switchDevices.GetN()):
        bridgeDevice.AddBridgePort(switchDevices.Get(portIter))

    # Add internet stack to the terminals
    internet = ns3.InternetStackHelper()
    internet.Install(terminals)

    # We've got the "hardware" in place.  Now we need to add IP addresses.
    #
    #print "Assign IP Addresses."
    ipv4 = ns3.Ipv4AddressHelper()
    ipv4.SetBase(ns3.Ipv4Address("10.1.1.0"), ns3.Ipv4Mask("255.255.255.0"))
    ipv4.Assign(terminalDevices)

    #
    # Create an OnOff application to send UDP datagrams from node zero to node 1.
    #
    #print "Create Applications."
    port = 9   # Discard port(RFC 863)

    onoff = ns3.OnOffHelper("ns3::UdpSocketFactory", 
                            ns3.Address(ns3.InetSocketAddress(ns3.Ipv4Address("10.1.1.2"), port)))
    onoff.SetAttribute("OnTime", ns3.RandomVariableValue(ns3.ConstantVariable(1)))
    onoff.SetAttribute("OffTime", ns3.RandomVariableValue(ns3.ConstantVariable(0)))

    app = onoff.Install(ns3.NodeContainer(terminals.Get(0)))
    # Start the application
    app.Start(ns3.Seconds(1.0))
    app.Stop(ns3.Seconds(10.0))

    # Create an optional packet sink to receive these packets
    sink = ns3.PacketSinkHelper("ns3::UdpSocketFactory",
                                ns3.Address(ns3.InetSocketAddress(ns3.Ipv4Address.GetAny(), port)))
    app = sink.Install(ns3.NodeContainer(terminals.Get(1)))
    app.Start (ns3.Seconds (0.0))

    # 
    # Create a similar flow from n3 to n0, starting at time 1.1 seconds
    #
    onoff.SetAttribute("Remote", 
                       ns3.AddressValue(ns3.InetSocketAddress(ns3.Ipv4Address("10.1.1.1"), port)))
    app = onoff.Install(ns3.NodeContainer(terminals.Get(3)))
    app.Start(ns3.Seconds(1.1))
    app.Stop(ns3.Seconds(10.0))

    app = sink.Install(ns3.NodeContainer(terminals.Get(0)))
    app.Start (ns3.Seconds (0.0))

    #
    # Configure tracing of all enqueue, dequeue, and NetDevice receive events.
    # Trace output will be sent to the file "csma-bridge.tr"
    #
    #print "Configure Tracing."
    #std.ofstream ascii
    #ascii.open("csma-bridge.tr")
    #CsmaHelper.EnableAsciiAll(ascii)

    #
    # Also configure some tcpdump traces; each interface will be traced.
    # The output files will be named:
    #     csma-bridge.pcap-<nodeId>-<interfaceId>
    # and can be read by the "tcpdump -r" command(use "-tt" option to
    # display timestamps correctly)
    #
    ns3.CsmaHelper.EnablePcapAll("csma-bridge")

    #
    # Now, do the actual simulation.
    #
    #print "Run Simulation."
    ns3.Simulator.Run()
    ns3.Simulator.Destroy()
    #print "Done."



if __name__ == '__main__':
    import sys
    main(sys.argv)

