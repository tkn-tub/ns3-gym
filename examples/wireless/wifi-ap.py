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

from ns.applications import *
from ns.core import *
from ns.internet import *
from ns.mobility import *
from ns.network import *
from ns.point_to_point import *
from ns.wifi import *


# void
# DevTxTrace (std::string context, Ptr<const Packet> p, Mac48Address address)
# {
#   std::cout << " TX to=" << address << " p: " << *p << std::endl;
# }
# void
# DevRxTrace(std::string context, Ptr<const Packet> p, Mac48Address address)
# {
#   std::cout << " RX from=" << address << " p: " << *p << std::endl;
# }
# void
# PhyRxOkTrace(std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble)
# {
#   std::cout << "PHYRXOK mode=" << mode << " snr=" << snr << " " << *packet << std::endl;
# }
# void
# PhyRxErrorTrace(std::string context, Ptr<const Packet> packet, double snr)
# {
#   std::cout << "PHYRXERROR snr=" << snr << " " << *packet << std::endl;
# }
# void
# PhyTxTrace(std::string context, Ptr<const Packet> packet, WifiMode mode, WifiPreamble preamble, uint8_t txPower)
# {
#   std::cout << "PHYTX mode=" << mode << " " << *packet << std::endl;
# }
# void
# PhyStateTrace(std::string context, Time start, Time duration, enum WifiPhy::State state)
# {
#   std::cout << " state=";
#   switch(state) {
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
    mobility = node.GetObject(MobilityModel.GetTypeId())
    mobility.SetPosition(position)


def GetPosition(node):
    mobility = node.GetObject(MobilityModel.GetTypeId())
    return mobility.GetPosition()

def AdvancePosition(node):
    pos = GetPosition(node);
    pos.x += 5.0
    if pos.x >= 210.0:
      return
    SetPosition(node, pos)
    Simulator.Schedule(Seconds(1.0), AdvancePosition, node)


def main(argv):
    Packet.EnablePrinting();

    # enable rts cts all the time.
    Config.SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("0"))
    # disable fragmentation
    Config.SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("2200"))

    wifi = WifiHelper.Default()
    mobility = MobilityHelper()
    stas = NodeContainer()
    ap = NodeContainer()
    #NetDeviceContainer staDevs;
    packetSocket = PacketSocketHelper()

    stas.Create(2)
    ap.Create(1)

    # give packet socket powers to nodes.
    packetSocket.Install(stas)
    packetSocket.Install(ap)

    wifiPhy = YansWifiPhyHelper.Default()
    wifiChannel = YansWifiChannelHelper.Default()
    wifiPhy.SetChannel(wifiChannel.Create())

    ssid = Ssid("wifi-default")
    wifi.SetRemoteStationManager("ns3::ArfWifiManager")
    wifiMac = NqosWifiMacHelper.Default()

    # setup stas.
    wifiMac.SetType("ns3::StaWifiMac",
                    "Ssid", SsidValue(ssid),
                    "ActiveProbing", BooleanValue(False))
    staDevs = wifi.Install(wifiPhy, wifiMac, stas)
    # setup ap.
    wifiMac.SetType("ns3::ApWifiMac",
                    "Ssid", SsidValue(ssid),
                    "BeaconGeneration", BooleanValue(True),
                    "BeaconInterval", TimeValue(Seconds(2.5)))
    wifi.Install(wifiPhy, wifiMac, ap)

    # mobility.
    mobility.Install(stas)
    mobility.Install(ap)

    Simulator.Schedule(Seconds(1.0), AdvancePosition, ap.Get(0))

    socket = PacketSocketAddress()
    socket.SetSingleDevice(staDevs.Get(0).GetIfIndex())
    socket.SetPhysicalAddress(staDevs.Get(1).GetAddress())
    socket.SetProtocol(1)

    onoff = OnOffHelper("ns3::PacketSocketFactory", Address(socket))
    onoff.SetAttribute("OnTime", RandomVariableValue(ConstantVariable(42)))
    onoff.SetAttribute("OffTime", RandomVariableValue(ConstantVariable(0)))

    apps = onoff.Install(NodeContainer(stas.Get(0)))
    apps.Start(Seconds(0.5))
    apps.Stop(Seconds(43.0))

    Simulator.Stop(Seconds(44.0))

  #   Config::Connect("/NodeList/*/DeviceList/*/Tx", MakeCallback(&DevTxTrace));
  #   Config::Connect("/NodeList/*/DeviceList/*/Rx", MakeCallback(&DevRxTrace));
  #   Config::Connect("/NodeList/*/DeviceList/*/Phy/RxOk", MakeCallback(&PhyRxOkTrace));
  #   Config::Connect("/NodeList/*/DeviceList/*/Phy/RxError", MakeCallback(&PhyRxErrorTrace));
  #   Config::Connect("/NodeList/*/DeviceList/*/Phy/Tx", MakeCallback(&PhyTxTrace));
  #   Config::Connect("/NodeList/*/DeviceList/*/Phy/State", MakeCallback(&PhyStateTrace));


    Simulator.Run()
    Simulator.Destroy()

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))

