/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Strasbourg University
 *
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
 *
 * Author: David Gross <gdavid.devel@gmail.com>
 *         Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

// Network topology
// //
// //             n0   R    n1
// //             |    _    |
// //             ====|_|====
// //                router
// // - R sends RA to n0's subnet (2001:1::/64 and 2001:ABCD::/64);
// // - R interface to n0 has two addresses with following prefixes 2001:1::/64 and 2001:ABCD::/64;
// // - R sends RA to n1's subnet (2001:2::/64);
// // - n0 ping6 n1.
// //
// // - Tracing of queues and packet receptions to file "radvd-two-prefix.tr"

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"

#include "ns3/ipv6-routing-table-entry.h"
#include "ns3/radvd.h"
#include "ns3/radvd-interface.h"
#include "ns3/radvd-prefix.h"
#include "ns3/ipv6-static-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RadvdTwoPrefixExample");

/**
 * \class StackHelper
 * \brief Helper to set or get some IPv6 information about nodes.
 */
class StackHelper
{
public:
  /**
   * \brief Add an address to a IPv6 node.
   * \param n node
   * \param interface interface index
   * \param address IPv6 address to add
   */
  inline void AddAddress (Ptr<Node>& n, uint32_t interface, Ipv6Address address)
  {
    Ptr<Ipv6> ipv6 = n->GetObject<Ipv6> ();
    ipv6->AddAddress (interface, address);
  }

  /**
   * \brief Print the routing table.
   * \param n the node
   */
  inline void PrintRoutingTable (Ptr<Node>& n)
  {
    Ptr<Ipv6StaticRouting> routing = 0;
    Ipv6StaticRoutingHelper routingHelper;
    Ptr<Ipv6> ipv6 = n->GetObject<Ipv6> ();
    uint32_t nbRoutes = 0;
    Ipv6RoutingTableEntry route;

    routing = routingHelper.GetStaticRouting (ipv6);

    std::cout << "Routing table of " << n << " : " << std::endl;
    std::cout << "Destination\t\t\t\t" << "Gateway\t\t\t\t\t" << "Interface\t" << "Prefix to use" << std::endl;

    nbRoutes = routing->GetNRoutes ();
    for (uint32_t i = 0; i < nbRoutes; i++)
      {
        route = routing->GetRoute (i);
        std::cout << route.GetDest () << "\t"
                  << route.GetGateway () << "\t"
                  << route.GetInterface () << "\t"
                  << route.GetPrefixToUse () << "\t"
                  << std::endl;
      }
  }
};

int main (int argc, char** argv)
{
#if 0 
  LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_ALL);
  LogComponentEnable ("Ipv6RawSocketImpl", LOG_LEVEL_ALL);
  LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_ALL);
  LogComponentEnable ("Ipv6StaticRouting", LOG_LEVEL_ALL);
  LogComponentEnable ("Ipv6Interface", LOG_LEVEL_ALL);
  LogComponentEnable ("RadvdApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("Ping6Application", LOG_LEVEL_ALL);
#endif

  CommandLine cmd;
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> n0 = CreateObject<Node> ();
  Ptr<Node> r = CreateObject<Node> ();
  Ptr<Node> n1 = CreateObject<Node> ();

  NodeContainer net1 (n0, r);
  NodeContainer net2 (r, n1);
  NodeContainer all (n0, r, n1);
  StackHelper stackHelper;

  NS_LOG_INFO ("Create IPv6 Internet Stack");
  InternetStackHelper internetv6;
  internetv6.Install (all);

  NS_LOG_INFO ("Create channels.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  NetDeviceContainer d1 = csma.Install (net1); /* n0 - R */
  NetDeviceContainer d2 = csma.Install (net2); /* R - n1 */

  NS_LOG_INFO ("Create networks and assign IPv6 Addresses.");
  Ipv6AddressHelper ipv6;

  /* first subnet */
  ipv6.NewNetwork (Ipv6Address ("2001:1::"), 64);
  NetDeviceContainer tmp;
  tmp.Add (d1.Get (0)); /* n0 */
  Ipv6InterfaceContainer iic1 = ipv6.AssignWithoutAddress (tmp); /* n0 interface */

  NetDeviceContainer tmp2;
  tmp2.Add (d1.Get (1)); /* R */
  Ipv6InterfaceContainer iicr1 = ipv6.Assign (tmp2); /* R interface to the first subnet is just statically assigned */
  iicr1.SetRouter (0, true);
  iic1.Add (iicr1);

  /* add another IPv6 address for second prefix advertised on first subnet */
  stackHelper.AddAddress (r, iic1.GetInterfaceIndex (1), Ipv6Address ("2001:ABCD::2"));

  /* second subnet R - n1 */
  ipv6.NewNetwork (Ipv6Address ("2001:2::"), 64);
  NetDeviceContainer tmp3;
  tmp3.Add (d2.Get (0)); /* R */
  Ipv6InterfaceContainer iicr2 = ipv6.Assign (tmp3); /* R interface */
  iicr2.SetRouter (0, true);

  NetDeviceContainer tmp4;
  tmp4.Add (d2.Get (1)); /* n1 */
  Ipv6InterfaceContainer iic2 = ipv6.AssignWithoutAddress (tmp4); 
  iic2.Add (iicr2);

  /* radvd configuration */
  Ipv6Address prefix ("2001:ABCD::0"); /* create the prefix */
  Ipv6Address prefixBis ("2001:1::0"); /* create the prefix */
  Ipv6Address prefix2 ("2001:2::0"); /* create the prefix */
  uint32_t indexRouter = iic1.GetInterfaceIndex (1); /* R interface (n0 - R) */
  uint32_t indexRouter2 = iic2.GetInterfaceIndex (1); /* R interface (R - n1) */
  Ptr<Radvd> radvd = CreateObject<Radvd> ();
  Ptr<RadvdInterface> routerInterface = Create<RadvdInterface> (indexRouter, 2000, 1000);
  Ptr<RadvdPrefix> routerPrefix = Create<RadvdPrefix> (prefix, 64, 3, 5);
  Ptr<RadvdPrefix> routerPrefixBis = Create<RadvdPrefix> (prefixBis, 64, 3, 5);
  Ptr<RadvdInterface> routerInterface2 = Create<RadvdInterface> (indexRouter2, 2000, 1000);
  Ptr<RadvdPrefix> routerPrefix2 = Create<RadvdPrefix> (prefix2, 64, 3, 5);

  /* first interface advertise two prefixes (2001:1::/64 and 2001:ABCD::/64) */
  /* prefix is added in the inverse order in packet */
  routerInterface->AddPrefix (routerPrefix);
  routerInterface->AddPrefix (routerPrefixBis);
  routerInterface2->AddPrefix (routerPrefix2);
  radvd->AddConfiguration (routerInterface);
  radvd->AddConfiguration (routerInterface2);

  r->AddApplication (radvd);
  radvd->SetStartTime (Seconds (1.0));
  radvd->SetStopTime (Seconds (2.0));

  /* Create a Ping6 application to send ICMPv6 echo request from n0 to n1 via R */
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 8;
  Time interPacketInterval = Seconds (1.);
  Ping6Helper ping6;

  /* ping6.SetLocal (iic1.GetAddress (0, 1)); */
  ping6.SetRemote (Ipv6Address ("2001:2::200:ff:fe00:4")); /* should be n1 address after autoconfiguration */
  ping6.SetIfIndex (iic1.GetInterfaceIndex (0));

  ping6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  ping6.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping6.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = ping6.Install (net1.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (9.0));

  /* RA should be received, two prefixes + routes + default route should be present */
  Simulator::Schedule (Seconds (2.0), &StackHelper::PrintRoutingTable, &stackHelper, n0); 
  /* at the end, RA addresses and routes should be cleared */
  Simulator::Schedule (Seconds (10.0), &StackHelper::PrintRoutingTable, &stackHelper, n0); 

  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream ("radvd-two-prefix.tr"));
  csma.EnablePcapAll (std::string ("radvd-two-prefix"), true);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}

