#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License version 2 as
# * published by the Free Software Foundation;
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program; if not, write to the Free Software
# * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Network topology
#
#       n0    n1   n2   n3
#       |     |    |    |
#       =================
#              LAN
#
# - UDP flows from n0 to n1 and back
# - DropTail queues 
# - Tracing of queues and packet receptions to file "udp-echo.tr"

import ns3


def main(argv):
  #
  # Allow the user to override any of the defaults and the above Bind() at
  # run-time, via command-line arguments
  #
  cmd = ns3.CommandLine()
  cmd.Parse(argv)

  #
  # But since this is a realtime script, don't allow the user to mess with
  # that.
  #
  ns3.GlobalValue.Bind("SimulatorImplementationType", ns3.StringValue("ns3::RealtimeSimulatorImpl"))

  #
  # Explicitly create the nodes required by the topology (shown above).
  #
  print "Create nodes."
  n = ns3.NodeContainer()
  n.Create(4)

  internet = ns3.InternetStackHelper()
  internet.Install(n)

  #
  # Explicitly create the channels required by the topology (shown above).
  #
  print ("Create channels.")
  csma = ns3.CsmaHelper()
  csma.SetChannelAttribute("DataRate", ns3.DataRateValue(ns3.DataRate(5000000)))
  csma.SetChannelAttribute("Delay", ns3.TimeValue(ns3.MilliSeconds(2)));
  csma.SetDeviceAttribute("Mtu", ns3.UintegerValue(1400))
  d = csma.Install(n)

  #
  # We've got the "hardware" in place.  Now we need to add IP addresses.
  #
  print ("Assign IP Addresses.")
  ipv4 = ns3.Ipv4AddressHelper()
  ipv4.SetBase(ns3.Ipv4Address("10.1.1.0"), ns3.Ipv4Mask("255.255.255.0"))
  i = ipv4.Assign(d)

  print ("Create Applications.")

  #
  # Create a UdpEchoServer application on node one.
  #
  port = 9  # well-known echo port number
  server = ns3.UdpEchoServerHelper(port)
  apps = server.Install(n.Get(1))
  apps.Start(ns3.Seconds(1.0))
  apps.Stop(ns3.Seconds(10.0))

  #
  # Create a UdpEchoClient application to send UDP datagrams from node zero to
  # node one.
  #
  packetSize = 1024
  maxPacketCount = 500
  interPacketInterval = ns3.Seconds(0.01)
  client = ns3.UdpEchoClientHelper(i.GetAddress (1), port)
  client.SetAttribute("MaxPackets", ns3.UintegerValue(maxPacketCount))
  client.SetAttribute("Interval", ns3.TimeValue(interPacketInterval))
  client.SetAttribute("PacketSize", ns3.UintegerValue(packetSize))
  apps = client.Install(n.Get(0))
  apps.Start(ns3.Seconds(2.0))
  apps.Stop(ns3.Seconds(10.0))

  ascii = ns3.AsciiTraceHelper()
  csma.EnableAsciiAll(ascii.CreateFileStream("realtime-udp-echo.tr"))
  csma.EnablePcapAll("realtime-udp-echo", False)

  #
  # Now, do the actual simulation.
  #
  print ("Run Simulation.")
  ns3.Simulator.Run()
  ns3.Simulator.Destroy()
  print ("Done.")

if __name__ == '__main__':
  import sys
  main(sys.argv)
