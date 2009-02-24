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
#  *
#  */

# 
#  This ns-3 example demonstrates the use of helper functions to ease 
#  the construction of simulation scenarios.  
#  
#  The simulation topology consists of a mixed wired and wireless
#  scenario in which a hierarchical mobility model is used.
# 
#  The simulation layout consists of N backbone routers interconnected
#  by an ad hoc wifi network.
#  Each backbone router also has a local 802.11 network and is connected
#  to a local LAN.  An additional set of(K-1) nodes are connected to
#  this backbone.  Finally, a local LAN is connected to each router
#  on the backbone, with L-1 additional hosts.  
# 
#  The nodes are populated with TCP/IP stacks, and OLSR unicast routing
#  on the backbone.  An example UDP transfer is shown.  The simulator
#  be configured to output tcpdumps or traces from different nodes.
# 
# 
#           +--------------------------------------------------------+
#           |                                                        |
#           |              802.11 ad hoc, ns-2 mobility              | 
#           |                                                        |
#           +--------------------------------------------------------+
#                    |       o o o(N backbone routers)       |
#                +--------+                               +--------+
#      wired LAN | mobile |                     wired LAN | mobile |
#     -----------| router |                    -----------| router |
#                ---------                                ---------
#                    |                                        |
#           +----------------+                       +----------------+
#           |     802.11     |                       |     802.11     |
#           |      net       |                       |       net      |
#           |   K-1 hosts    |                       |   K-1 hosts    |
#           +----------------+                       +----------------+
# 

import ns3

# # 
# #  This function will be used below as a trace sink
# #  
# static void
# CourseChangeCallback(std.string path, Ptr<const MobilityModel> model)
# {
#   Vector position = model.GetPosition();
#   std.cout << "CourseChange " << path << " x=" << position.x << ", y=" << position.y << ", z=" << position.z << std.endl;
# }

def main(argv): 
    # 
    #  First, we declare and initialize a few local variables that control some 
    #  simulation parameters.
    # 
    backboneNodes = 10
    infraNodes = 5
    lanNodes = 5
    stopTime = 10

    # 
    #  Simulation defaults are typically set next, before command line
    #  arguments are parsed.
    # 
    ns3.Config.SetDefault("ns3::OnOffApplication::PacketSize", ns3.StringValue("210"))
    ns3.Config.SetDefault("ns3::OnOffApplication::DataRate", ns3.StringValue("448kb/s"))

    # 
    #  For convenience, we add the local variables to the command line argument
    #  system so that they can be overridden with flags such as 
    #  "--backboneNodes=20"
    # 
    cmd = ns3.CommandLine()
    #cmd.AddValue("backboneNodes", "number of backbone nodes", backboneNodes)
    #cmd.AddValue("infraNodes", "number of leaf nodes", infraNodes)
    #cmd.AddValue("lanNodes", "number of LAN nodes", lanNodes)
    #cmd.AddValue("stopTime", "simulation stop time(seconds)", stopTime)

    # 
    #  The system global variables and the local values added to the argument
    #  system can be overridden by command line arguments by using this call.
    # 
    cmd.Parse(argv)

    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 
    #                                                                        # 
    #  Construct the backbone                                                # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 

    # 
    #  Create a container to manage the nodes of the adhoc(backbone) network.
    #  Later we'll create the rest of the nodes we'll need.
    # 
    backbone = ns3.NodeContainer()
    backbone.Create(backboneNodes)
    # 
    #  Create the backbone wifi net devices and install them into the nodes in 
    #  our container
    # 
    wifi = ns3.WifiHelper()
    wifi.SetMac("ns3::AdhocWifiMac")
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode", ns3.StringValue ("wifia-54mbs"))
    wifiPhy = ns3.YansWifiPhyHelper.Default ()
    wifiChannel = ns3.YansWifiChannelHelper.Default ()
    wifiPhy.SetChannel (wifiChannel.Create ())
    backboneDevices = wifi.Install(wifiPhy, backbone)
    # 
    #  Add the IPv4 protocol stack to the nodes in our container
    # 
    internet = ns3.InternetStackHelper()
    internet.Install(backbone)
    # 
    #  Assign IPv4 addresses to the device drivers(actually to the associated
    #  IPv4 interfaces) we just created.
    # 
    ipAddrs = ns3.Ipv4AddressHelper()
    ipAddrs.SetBase(ns3.Ipv4Address("192.168.0.0"), ns3.Ipv4Mask("255.255.255.0"))
    ipAddrs.Assign(backboneDevices)

    # 
    #  The ad-hoc network nodes need a mobility model so we aggregate one to 
    #  each of the nodes we just finished building.  
    # 
    mobility = ns3.MobilityHelper()
    positionAlloc = ns3.ListPositionAllocator()
    x = 0.0
    for i in range(backboneNodes):
        positionAlloc.Add(ns3.Vector(x, 0.0, 0.0))
        x += 5.0
    mobility.SetPositionAllocator(positionAlloc)
    mobility.SetMobilityModel("ns3::RandomDirection2dMobilityModel",
                               "Bounds", ns3.RectangleValue(ns3.Rectangle(0, 1000, 0, 1000)),
                               "Speed", ns3.RandomVariableValue(ns3.ConstantVariable(2000)),
                               "Pause", ns3.RandomVariableValue(ns3.ConstantVariable(0.2)))
    mobility.Install(backbone)

    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 
    #                                                                        # 
    #  Construct the LANs                                                    # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 

    #  Reset the address base-- all of the CSMA networks will be in
    #  the "172.16 address space
    ipAddrs.SetBase(ns3.Ipv4Address("172.16.0.0"), ns3.Ipv4Mask("255.255.255.0"))

    for i in range(backboneNodes):
        print "Configuring local area network for backbone node ", i
        # 
        #  Create a container to manage the nodes of the LAN.  We need
        #  two containers here; one with all of the new nodes, and one
        #  with all of the nodes including new and existing nodes
        # 
        newLanNodes = ns3.NodeContainer()
        newLanNodes.Create(lanNodes - 1)
        #  Now, create the container with all nodes on this link
        lan = ns3.NodeContainer(ns3.NodeContainer(backbone.Get(i)), newLanNodes)
        # 
        #  Create the CSMA net devices and install them into the nodes in our 
        #  collection.
        # 
        csma = ns3.CsmaHelper()
        csma.SetChannelAttribute("DataRate", ns3.DataRateValue(ns3.DataRate(5000000)))
        csma.SetChannelAttribute("Delay", ns3.TimeValue(ns3.MilliSeconds(2)))
        lanDevices = csma.Install(lan)
        # 
        #  Add the IPv4 protocol stack to the new LAN nodes
        # 
        internet.Install(newLanNodes)
        # 
        #  Assign IPv4 addresses to the device drivers(actually to the 
        #  associated IPv4 interfaces) we just created.
        # 
        ipAddrs.Assign(lanDevices)
        # 
        #  Assign a new network prefix for the next LAN, according to the
        #  network mask initialized above
        # 
        ipAddrs.NewNetwork()

    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 
    #                                                                        # 
    #  Construct the mobile networks                                         # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 

    #  Reset the address base-- all of the 802.11 networks will be in
    #  the "10.0" address space
    ipAddrs.SetBase(ns3.Ipv4Address("10.0.0.0"), ns3.Ipv4Mask("255.255.255.0"))

    for i in range(backboneNodes):
        print "Configuring wireless network for backbone node ", i
        # 
        #  Create a container to manage the nodes of the LAN.  We need
        #  two containers here; one with all of the new nodes, and one
        #  with all of the nodes including new and existing nodes
        # 
        stas = ns3.NodeContainer()
        stas.Create(infraNodes - 1)
        #  Now, create the container with all nodes on this link
        infra = ns3.NodeContainer(ns3.NodeContainer(backbone.Get(i)), stas)
        # 
        #  Create another ad hoc network and devices
        # 
        ssid = ns3.Ssid ('wifi-infra' + str(i))
        wifiInfra = ns3.WifiHelper.Default ()
        wifiPhy.SetChannel (wifiChannel.Create ())
        wifiInfra.SetRemoteStationManager ('ns3::ArfWifiManager')
        # setup stas
        wifiInfra.SetMac ("ns3::NqstaWifiMac",
                          "Ssid", ns3.SsidValue (ssid),
                          "ActiveProbing", ns3.BooleanValue (False))
        staDevices = wifiInfra.Install (wifiPhy, stas)
        # setup ap.
        wifiInfra.SetMac ("ns3::NqapWifiMac", "Ssid", ns3.SsidValue (ssid),
                          "BeaconGeneration", ns3.BooleanValue (True),
                          "BeaconInterval", ns3.TimeValue (ns3.Seconds (2.5)))
        apDevices = wifiInfra.Install (wifiPhy, backbone.Get (i))
        # Collect all of these new devices
        infraDevices = ns3.NetDeviceContainer (apDevices, staDevices)

        #  Add the IPv4 protocol stack to the nodes in our container
        # 
        internet.Install(stas)
        # 
        #  Assign IPv4 addresses to the device drivers(actually to the associated
        #  IPv4 interfaces) we just created.
        # 
        ipAddrs.Assign(infraDevices)
        # 
        #  Assign a new network prefix for each mobile network, according to 
        #  the network mask initialized above
        # 
        ipAddrs.NewNetwork()
        # 
        #  The new wireless nodes need a mobility model so we aggregate one 
        #  to each of the nodes we just finished building.
        # 
        subnetAlloc = ns3.ListPositionAllocator()
        for j in range(infra.GetN()):
            subnetAlloc.Add(ns3.Vector(0.0, j, 0.0))

        mobility.PushReferenceMobilityModel(backbone.Get(i))
        mobility.SetPositionAllocator(subnetAlloc)
        mobility.SetMobilityModel("ns3::RandomDirection2dMobilityModel",
                                  "Bounds", ns3.RectangleValue(ns3.Rectangle(-25, 25, -25, 25)),
                                  "Speed", ns3.RandomVariableValue(ns3.ConstantVariable(30)),
                                  "Pause", ns3.RandomVariableValue(ns3.ConstantVariable(0.4)))
        mobility.Install(infra)

    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 
    #                                                                        # 
    #  Routing configuration                                                 # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 

    print "Enabling OLSR routing on all backbone nodes"
    olsr = ns3.OlsrHelper()
    olsr.Install(backbone)

    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 
    #                                                                        # 
    #  Application configuration                                             # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 

    #  Create the OnOff application to send UDP datagrams of size
    #  210 bytes at a rate of 448 Kb/s, between two nodes
    print "Create Applications."
    port = 9   #  Discard port(RFC 863)

    #  Let's make sure that the user does not define too few LAN nodes
    #  to make this example work.  We need lanNodes >= 5
    assert (lanNodes >= 5)
    appSource = ns3.NodeList.GetNode(11)
    appSink = ns3.NodeList.GetNode(13)
    remoteAddr = ns3.Ipv4Address("172.16.0.5")

    onoff = ns3.OnOffHelper("ns3::UdpSocketFactory", 
                            ns3.Address(ns3.InetSocketAddress(remoteAddr, port)))
    onoff.SetAttribute("OnTime", ns3.RandomVariableValue(ns3.ConstantVariable(1)))
    onoff.SetAttribute("OffTime", ns3.RandomVariableValue(ns3.ConstantVariable(0)))
    apps = onoff.Install(ns3.NodeContainer(appSource))
    apps.Start(ns3.Seconds(3.0))
    apps.Stop(ns3.Seconds(20.0))

    #  Create a packet sink to receive these packets
    sink = ns3.PacketSinkHelper("ns3::UdpSocketFactory", 
                                ns3.InetSocketAddress(ns3.Ipv4Address.GetAny(), port))
    apps = sink.Install(ns3.NodeContainer(appSink))
    apps.Start(ns3.Seconds(3.0))

    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 
    #                                                                        # 
    #  Tracing configuration                                                 # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # / 

    print "Configure Tracing."
    # 
    #  Let's set up some ns-2-like ascii traces, using another helper class
    # 
    #std.ofstream ascii
    #ascii.open("mixed-wireless.tr")
    #WifiHelper.EnableAsciiAll(ascii)
    #CsmaHelper.EnableAsciiAll(ascii)
    print "(tracing not done for Python)"
    #  Look at nodes 11, 13 only
    # WifiHelper.EnableAscii(ascii, 11, 0); 
    # WifiHelper.EnableAscii(ascii, 13, 0); 

    #  Let's do a pcap trace on the backbone devices
    ns3.YansWifiPhyHelper.EnablePcap("mixed-wireless", backboneDevices)
    #  Let's additionally trace the application Sink, ifIndex 0
    ns3.CsmaHelper.EnablePcap("mixed-wireless", appSink.GetId(), 0)

#   #ifdef ENABLE_FOR_TRACING_EXAMPLE
#     Config.Connect("/NodeList/*/$MobilityModel/CourseChange",
#       MakeCallback(&CourseChangeCallback))
#   #endif


    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #  
    #                                                                        # 
    #  Run simulation                                                        # 
    #                                                                        # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #  

    print "Run Simulation."
    ns3.Simulator.Stop(ns3.Seconds(stopTime))
    ns3.Simulator.Run()
    ns3.Simulator.Destroy()


if __name__ == '__main__':
    import sys
    main(sys.argv)
