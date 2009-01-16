# -*-  Mode: Python; -*-
# /*
#  * Copyright (c) 2005,2006,2007 INRIA
#  * Copyright (c) 2009 INESC Porto
#  *
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
#  * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
#  *          Gustavo Carneiro <gjc@inescporto.pt>
#  */

import sys
import ns3

# void
# DevTxTrace (std::string context, Ptr<const Packet> p, Mac48Address address)
# {
#   std::cout << " TX to=" << address << " p: " << *p << std::endl;
# }
# void
# DevRxTrace (std::string context, Ptr<const Packet> p, Mac48Address address)
# {
#   std::cout << " RX from=" << address << " p: " << *p << std::endl;
# }
# void
# PhyRxOkTrace (std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble)
# {
#   std::cout << "PHYRXOK mode=" << mode << " snr=" << snr << " " << *packet << std::endl;
# }
# void
# PhyRxErrorTrace (std::string context, Ptr<const Packet> packet, double snr)
# {
#   std::cout << "PHYRXERROR snr=" << snr << " " << *packet << std::endl;
# }
# void
# PhyTxTrace (std::string context, Ptr<const Packet> packet, WifiMode mode, WifiPreamble preamble, uint8_t txPower)
# {
#   std::cout << "PHYTX mode=" << mode << " " << *packet << std::endl;
# }
# void
# PhyStateTrace (std::string context, Time start, Time duration, enum WifiPhy::State state)
# {
#   std::cout << " state=";
#   switch (state) {
#   case WifiPhy::TX:
#     std::cout << "tx      ";
#     break;
#   case WifiPhy::SYNC:
#     std::cout << "sync    ";
#     break;
#   case WifiPhy::CCA_BUSY:
#     std::cout << "cca-busy";
#     break;
#   case WifiPhy::IDLE:
#     std::cout << "idle    ";
#     break;
#   }
#   std::cout << " start="<<start<<" duration="<<duration<<std::endl;
# }

def SetPosition(node, position):
    mobility = node.GetObject(ns3.MobilityModel.GetTypeId())
    mobility.SetPosition(position)


def GetPosition(node):
    mobility = node.GetObject(ns3.MobilityModel.GetTypeId())
    return mobility.GetPosition()

def AdvancePosition(node):
    pos = GetPosition (node);
    pos.x += 5.0
    if pos.x >= 210.0:
      return
    SetPosition(node, pos)
    ns3.Simulator.Schedule(ns3.Seconds(1.0), AdvancePosition, node)


def main(argv):
    ns3.Packet.EnablePrinting ();

    # enable rts cts all the time.
    ns3.Config.SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ns3.StringValue("0"))
    # disable fragmentation
    ns3.Config.SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", ns3.StringValue ("2200"))

    wifi = ns3.WifiHelper.Default()
    mobility = ns3.MobilityHelper()
    stas = ns3.NodeContainer()
    ap = ns3.NodeContainer()
    #NetDeviceContainer staDevs;
    packetSocket = ns3.PacketSocketHelper()

    stas.Create(2)
    ap.Create(1)

    # give packet socket powers to nodes.
    packetSocket.Install(stas)
    packetSocket.Install(ap)

    wifiPhy = ns3.YansWifiPhyHelper.Default()
    wifiChannel = ns3.YansWifiChannelHelper.Default()
    wifiPhy.SetChannel(wifiChannel.Create())

    ssid = ns3.Ssid("wifi-default")
    wifi.SetRemoteStationManager("ns3::ArfWifiManager")
    # setup stas.
    wifi.SetMac("ns3::NqstaWifiMac", 
               "Ssid", ns3.SsidValue(ssid),
               "ActiveProbing", ns3.BooleanValue(False))
    staDevs = wifi.Install(wifiPhy, stas)
    # setup ap.
    wifi.SetMac("ns3::NqapWifiMac", "Ssid", ns3.SsidValue(ssid),
                "BeaconGeneration", ns3.BooleanValue(True),
                "BeaconInterval", ns3.TimeValue(ns3.Seconds(2.5)))
    wifi.Install(wifiPhy, ap)

    # mobility.
    mobility.Install(stas)
    mobility.Install(ap)

    ns3.Simulator.Schedule(ns3.Seconds(1.0), AdvancePosition, ap.Get(0))

    socket = ns3.PacketSocketAddress()
    socket.SetSingleDevice(staDevs.Get(0).GetIfIndex())
    socket.SetPhysicalAddress(staDevs.Get(1).GetAddress())
    socket.SetProtocol(1)

    onoff = ns3.OnOffHelper("ns3::PacketSocketFactory", ns3.Address(socket))
    onoff.SetAttribute("OnTime", ns3.RandomVariableValue(ns3.ConstantVariable(42)))
    onoff.SetAttribute("OffTime", ns3.RandomVariableValue(ns3.ConstantVariable(0)))

    apps = onoff.Install(ns3.NodeContainer(stas.Get(0)))
    apps.Start(ns3.Seconds(0.5))
    apps.Stop(ns3.Seconds(43.0))

    ns3.Simulator.Stop(ns3.Seconds(44.0))

  #   Config::Connect ("/NodeList/*/DeviceList/*/Tx", MakeCallback (&DevTxTrace));
  #   Config::Connect ("/NodeList/*/DeviceList/*/Rx", MakeCallback (&DevRxTrace));
  #   Config::Connect ("/NodeList/*/DeviceList/*/Phy/RxOk", MakeCallback (&PhyRxOkTrace));
  #   Config::Connect ("/NodeList/*/DeviceList/*/Phy/RxError", MakeCallback (&PhyRxErrorTrace));
  #   Config::Connect ("/NodeList/*/DeviceList/*/Phy/Tx", MakeCallback (&PhyTxTrace));
  #   Config::Connect ("/NodeList/*/DeviceList/*/Phy/State", MakeCallback (&PhyStateTrace));

    ns3.Simulator.Run()

    ns3.Simulator.Destroy()

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))

