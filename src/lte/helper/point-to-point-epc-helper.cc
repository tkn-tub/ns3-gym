/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 */

#include <ns3/point-to-point-epc-helper.h>
#include <ns3/log.h>
#include <ns3/inet-socket-address.h>
#include <ns3/mac48-address.h>
#include <ns3/string.h>
#include <ns3/boolean.h>
#include <ns3/eps-bearer.h>
#include <ns3/ipv4-address.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/packet-socket-helper.h>
#include <ns3/packet-socket-address.h>
#include <ns3/epc-enb-application.h>
#include <ns3/epc-sgw-pgw-application.h>
#include "ns3/ipv6-static-routing.h"
#include "ns3/ipv6-static-routing-helper.h"
#include <ns3/lte-enb-rrc.h>
#include <ns3/epc-x2.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/epc-mme.h>
#include <ns3/epc-ue-nas.h>
#include <ns3/ipv4-address-generator.h>
#include <ns3/ipv6-address-generator.h>
#include <ns3/icmpv6-l4-protocol.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PointToPointEpcHelper");

NS_OBJECT_ENSURE_REGISTERED (PointToPointEpcHelper);


PointToPointEpcHelper::PointToPointEpcHelper () 
  : m_gtpuUdpPort (2152)  // fixed by the standard
{
  NS_LOG_FUNCTION (this);
  // To access the attribute value within the constructor
  ObjectBase::ConstructSelf (AttributeConstructionList ());

  // since we use point-to-point links for all S1-U links, 
  // we use a /30 subnet which can hold exactly two addresses 
  // (remember that net broadcast and null address are not valid)
  m_s1uIpv4AddressHelper.SetBase ("10.0.0.0", "255.255.255.252");

  m_x2Ipv4AddressHelper.SetBase ("12.0.0.0", "255.255.255.252");

  // we use a /8 net for all UEs
  m_uePgwAddressHelper.SetBase ("7.0.0.0", "255.0.0.0");

  // we use a /64 IPv6 net all UEs
  m_uePgwAddressHelper6.SetBase ("7777:f00d::", Ipv6Prefix (64));

  // create SgwPgwNode
  m_sgwPgw = CreateObject<Node> ();
  InternetStackHelper internet;
  internet.Install (m_sgwPgw);

  // The Tun device resides in different 64 bit subnet.
  // We must create an unique route to tun device for all the packets destined
  // to all 64 bit IPv6 prefixes of UEs, based by the unique 48 bit network prefix of this EPC network
  Ipv6StaticRoutingHelper ipv6RoutingHelper;
  Ptr<Ipv6StaticRouting> pgwStaticRouting = ipv6RoutingHelper.GetStaticRouting (m_sgwPgw->GetObject<Ipv6> ());
  pgwStaticRouting->AddNetworkRouteTo ("7777:f00d::", Ipv6Prefix (64), Ipv6Address ("::"), 1, 0);
  
  // create S1-U socket
  Ptr<Socket> sgwPgwS1uSocket = Socket::CreateSocket (m_sgwPgw, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  int retval = sgwPgwS1uSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_gtpuUdpPort));
  NS_ASSERT (retval == 0);

  // create TUN device implementing tunneling of user data over GTP-U/UDP/IP 
  m_tunDevice = CreateObject<VirtualNetDevice> ();

  // allow jumbo packets
  m_tunDevice->SetAttribute ("Mtu", UintegerValue (30000));

  // yes we need this
  m_tunDevice->SetAddress (Mac48Address::Allocate ());

  m_sgwPgw->AddDevice (m_tunDevice);
  NetDeviceContainer tunDeviceContainer;
  tunDeviceContainer.Add (m_tunDevice);
  
  // the TUN device is on the same subnet as the UEs, so when a packet
  // addressed to an UE arrives at the intenet to the WAN interface of
  // the PGW it will be forwarded to the TUN device. 
  Ipv4InterfaceContainer tunDeviceIpv4IfContainer = AssignUeIpv4Address (tunDeviceContainer);  


  // the TUN device for IPv6 address is on the different subnet as the
  // UEs, it will forward the UE packets as we have inserted the route
  // for all UEs at the time of assigning UE addresses
  Ipv6InterfaceContainer tunDeviceIpv6IfContainer = AssignUeIpv6Address (tunDeviceContainer);


  //Set Forwarding of the IPv6 interface
  tunDeviceIpv6IfContainer.SetForwarding (0,true);
  tunDeviceIpv6IfContainer.SetDefaultRouteInAllNodes (0);

  // create EpcSgwPgwApplication
  m_sgwPgwApp = CreateObject<EpcSgwPgwApplication> (m_tunDevice, sgwPgwS1uSocket);
  m_sgwPgw->AddApplication (m_sgwPgwApp);
  
  // connect SgwPgwApplication and virtual net device for tunneling
  m_tunDevice->SetSendCallback (MakeCallback (&EpcSgwPgwApplication::RecvFromTunDevice, m_sgwPgwApp));


  // Create MME and connect with SGW via S11 interface
  m_mme = CreateObject<EpcMme> ();
  m_mme->SetS11SapSgw (m_sgwPgwApp->GetS11SapSgw ());
  m_sgwPgwApp->SetS11SapMme (m_mme->GetS11SapMme ());
}

PointToPointEpcHelper::~PointToPointEpcHelper ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
PointToPointEpcHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PointToPointEpcHelper")
    .SetParent<EpcHelper> ()
    .SetGroupName("Lte")
    .AddConstructor<PointToPointEpcHelper> ()
    .AddAttribute ("S1uLinkDataRate", 
                   "The data rate to be used for the next S1-U link to be created",
                   DataRateValue (DataRate ("10Gb/s")),
                   MakeDataRateAccessor (&PointToPointEpcHelper::m_s1uLinkDataRate),
                   MakeDataRateChecker ())
    .AddAttribute ("S1uLinkDelay", 
                   "The delay to be used for the next S1-U link to be created",
                   TimeValue (Seconds (0)),
                   MakeTimeAccessor (&PointToPointEpcHelper::m_s1uLinkDelay),
                   MakeTimeChecker ())
    .AddAttribute ("S1uLinkMtu", 
                   "The MTU of the next S1-U link to be created. Note that, because of the additional GTP/UDP/IP tunneling overhead, you need a MTU larger than the end-to-end MTU that you want to support.",
                   UintegerValue (2000),
                   MakeUintegerAccessor (&PointToPointEpcHelper::m_s1uLinkMtu),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("X2LinkDataRate",
                   "The data rate to be used for the next X2 link to be created",
                   DataRateValue (DataRate ("10Gb/s")),
                   MakeDataRateAccessor (&PointToPointEpcHelper::m_x2LinkDataRate),
                   MakeDataRateChecker ())
    .AddAttribute ("X2LinkDelay",
                   "The delay to be used for the next X2 link to be created",
                   TimeValue (Seconds (0)),
                   MakeTimeAccessor (&PointToPointEpcHelper::m_x2LinkDelay),
                   MakeTimeChecker ())
    .AddAttribute ("X2LinkMtu",
                   "The MTU of the next X2 link to be created. Note that, because of some big X2 messages, you need a big MTU.",
                   UintegerValue (3000),
                   MakeUintegerAccessor (&PointToPointEpcHelper::m_x2LinkMtu),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("S1uLinkPcapPrefix",
                   "Prefix for Pcap generated by S1-U link",
                   StringValue ("s1-u"),
                   MakeStringAccessor (&PointToPointEpcHelper::m_s1uLinkPcapPrefix),
                   MakeStringChecker ())
    .AddAttribute ("X2LinkPcapPrefix",
                   "Prefix for Pcap generated by X2 link",
                   StringValue ("x2"),
                   MakeStringAccessor (&PointToPointEpcHelper::m_x2LinkPcapPrefix),
                   MakeStringChecker ())
    .AddAttribute ("X2LinkEnablePcap",
                   "Enable Pcap for X2 link",
                   BooleanValue (false),
                   MakeBooleanAccessor (&PointToPointEpcHelper::m_enablePcapOverX2),
                   MakeBooleanChecker ())
    .AddAttribute ("S1uLinkEnablePcap",
                   "Enable Pcap for X2 link",
                   BooleanValue (false),
                   MakeBooleanAccessor (&PointToPointEpcHelper::m_enablePcapOverS1U),
                   MakeBooleanChecker ())
  ;
  return tid;
}

TypeId
PointToPointEpcHelper::GetInstanceTypeId () const
{
  return GetTypeId ();
}

void
PointToPointEpcHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_tunDevice->SetSendCallback (MakeNullCallback<bool, Ptr<Packet>, const Address&, const Address&, uint16_t> ());
  m_tunDevice = 0;
  m_sgwPgwApp = 0;  
  m_sgwPgw->Dispose ();
}


void
PointToPointEpcHelper::AddEnb (Ptr<Node> enb, Ptr<NetDevice> lteEnbNetDevice, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << enb << lteEnbNetDevice << cellId);

  NS_ASSERT (enb == lteEnbNetDevice->GetNode ());

  // add an IPv4 stack to the previously created eNB
  InternetStackHelper internet;
  internet.Install (enb);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after node creation: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());

  // create a point to point link between the new eNB and the SGW with
  // the corresponding new NetDevices on each side  
  NodeContainer enbSgwNodes;
  enbSgwNodes.Add (m_sgwPgw);
  enbSgwNodes.Add (enb);
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (m_s1uLinkDataRate));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (m_s1uLinkMtu));
  p2ph.SetChannelAttribute ("Delay", TimeValue (m_s1uLinkDelay));
  NetDeviceContainer enbSgwDevices = p2ph.Install (enb, m_sgwPgw);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after installing p2p dev: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());  
  Ptr<NetDevice> enbDev = enbSgwDevices.Get (0);
  Ptr<NetDevice> sgwDev = enbSgwDevices.Get (1);

  if (m_enablePcapOverS1U)
    {
      p2ph.EnablePcapAll(m_s1uLinkPcapPrefix);
    }

  m_s1uIpv4AddressHelper.NewNetwork ();
  Ipv4InterfaceContainer enbSgwIpIfaces = m_s1uIpv4AddressHelper.Assign (enbSgwDevices);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after assigning Ipv4 addr to S1 dev: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());
  
  Ipv4Address enbAddress = enbSgwIpIfaces.GetAddress (0);
  Ipv4Address sgwAddress = enbSgwIpIfaces.GetAddress (1);

  // create S1-U socket for the ENB
  Ptr<Socket> enbS1uSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  int retval = enbS1uSocket->Bind (InetSocketAddress (enbAddress, m_gtpuUdpPort));
  NS_ASSERT (retval == 0);

  // create LTE socket for the ENB 
  Ptr<Socket> enbLteSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::PacketSocketFactory"));
  PacketSocketAddress enbLteSocketBindAddress;
  enbLteSocketBindAddress.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketBindAddress.SetProtocol (Ipv4L3Protocol::PROT_NUMBER);
  retval = enbLteSocket->Bind (enbLteSocketBindAddress);
  NS_ASSERT (retval == 0);  
  PacketSocketAddress enbLteSocketConnectAddress;
  enbLteSocketConnectAddress.SetPhysicalAddress (Mac48Address::GetBroadcast ());
  enbLteSocketConnectAddress.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketConnectAddress.SetProtocol (Ipv4L3Protocol::PROT_NUMBER);
  retval = enbLteSocket->Connect (enbLteSocketConnectAddress);
  NS_ASSERT (retval == 0);  

  // create LTE socket for the ENB 
  Ptr<Socket> enbLteSocket6 = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::PacketSocketFactory"));
  PacketSocketAddress enbLteSocketBindAddress6;
  enbLteSocketBindAddress6.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketBindAddress6.SetProtocol (Ipv6L3Protocol::PROT_NUMBER);
  retval = enbLteSocket6->Bind (enbLteSocketBindAddress6);
  NS_ASSERT (retval == 0);  
  PacketSocketAddress enbLteSocketConnectAddress6;
  enbLteSocketConnectAddress6.SetPhysicalAddress (Mac48Address::GetBroadcast ());
  enbLteSocketConnectAddress6.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketConnectAddress6.SetProtocol (Ipv6L3Protocol::PROT_NUMBER);
  retval = enbLteSocket6->Connect (enbLteSocketConnectAddress6);
  NS_ASSERT (retval == 0);  

  NS_LOG_INFO ("create EpcEnbApplication");
  Ptr<EpcEnbApplication> enbApp = CreateObject<EpcEnbApplication> (enbLteSocket, enbLteSocket6, enbS1uSocket, enbAddress, sgwAddress, cellId);
  enb->AddApplication (enbApp);
  NS_ASSERT (enb->GetNApplications () == 1);
  NS_ASSERT_MSG (enb->GetApplication (0)->GetObject<EpcEnbApplication> () != 0, "cannot retrieve EpcEnbApplication");
  NS_LOG_LOGIC ("enb: " << enb << ", enb->GetApplication (0): " << enb->GetApplication (0));

  
  NS_LOG_INFO ("Create EpcX2 entity");
  Ptr<EpcX2> x2 = CreateObject<EpcX2> ();
  enb->AggregateObject (x2);

  NS_LOG_INFO ("connect S1-AP interface");
  m_mme->AddEnb (cellId, enbAddress, enbApp->GetS1apSapEnb ());
  m_sgwPgwApp->AddEnb (cellId, enbAddress, sgwAddress);
  enbApp->SetS1apSapMme (m_mme->GetS1apSapMme ());
}


void
PointToPointEpcHelper::AddX2Interface (Ptr<Node> enb1, Ptr<Node> enb2)
{
  NS_LOG_FUNCTION (this << enb1 << enb2);

  // Create a point to point link between the two eNBs with
  // the corresponding new NetDevices on each side
  NodeContainer enbNodes;
  enbNodes.Add (enb1);
  enbNodes.Add (enb2);
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (m_x2LinkDataRate));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (m_x2LinkMtu));
  p2ph.SetChannelAttribute ("Delay", TimeValue (m_x2LinkDelay));
  NetDeviceContainer enbDevices = p2ph.Install (enb1, enb2);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #1 after installing p2p dev: " << enb1->GetObject<Ipv4> ()->GetNInterfaces ());
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #2 after installing p2p dev: " << enb2->GetObject<Ipv4> ()->GetNInterfaces ());
  Ptr<NetDevice> enb1Dev = enbDevices.Get (0);
  Ptr<NetDevice> enb2Dev = enbDevices.Get (1);

  if (m_enablePcapOverX2)
    {
      p2ph.EnablePcapAll(m_x2LinkPcapPrefix);
    }

  m_x2Ipv4AddressHelper.NewNetwork ();
  Ipv4InterfaceContainer enbIpIfaces = m_x2Ipv4AddressHelper.Assign (enbDevices);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #1 after assigning Ipv4 addr to X2 dev: " << enb1->GetObject<Ipv4> ()->GetNInterfaces ());
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #2 after assigning Ipv4 addr to X2 dev: " << enb2->GetObject<Ipv4> ()->GetNInterfaces ());

  Ipv4Address enb1X2Address = enbIpIfaces.GetAddress (0);
  Ipv4Address enb2X2Address = enbIpIfaces.GetAddress (1);

  // Add X2 interface to both eNBs' X2 entities
  Ptr<EpcX2> enb1X2 = enb1->GetObject<EpcX2> ();
  Ptr<LteEnbNetDevice> enb1LteDev = enb1->GetDevice (0)->GetObject<LteEnbNetDevice> ();
  uint16_t enb1CellId = enb1LteDev->GetCellId ();
  NS_LOG_LOGIC ("LteEnbNetDevice #1 = " << enb1LteDev << " - CellId = " << enb1CellId);

  Ptr<EpcX2> enb2X2 = enb2->GetObject<EpcX2> ();
  Ptr<LteEnbNetDevice> enb2LteDev = enb2->GetDevice (0)->GetObject<LteEnbNetDevice> ();
  uint16_t enb2CellId = enb2LteDev->GetCellId ();
  NS_LOG_LOGIC ("LteEnbNetDevice #2 = " << enb2LteDev << " - CellId = " << enb2CellId);

  enb1X2->AddX2Interface (enb1CellId, enb1X2Address, enb2CellId, enb2X2Address);
  enb2X2->AddX2Interface (enb2CellId, enb2X2Address, enb1CellId, enb1X2Address);

  enb1LteDev->GetRrc ()->AddX2Neighbour (enb2LteDev->GetCellId ());
  enb2LteDev->GetRrc ()->AddX2Neighbour (enb1LteDev->GetCellId ());
}


void 
PointToPointEpcHelper::AddUe (Ptr<NetDevice> ueDevice, uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi << ueDevice );
  
  m_mme->AddUe (imsi);
  m_sgwPgwApp->AddUe (imsi);
  

}

uint8_t
PointToPointEpcHelper::ActivateEpsBearer (Ptr<NetDevice> ueDevice, uint64_t imsi, Ptr<EpcTft> tft, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this << ueDevice << imsi);

  // we now retrieve the IPv4/IPv6 address of the UE and notify it to the SGW;
  // we couldn't do it before since address assignment is triggered by
  // the user simulation program, rather than done by the EPC   
  Ptr<Node> ueNode = ueDevice->GetNode (); 
  Ptr<Ipv4> ueIpv4 = ueNode->GetObject<Ipv4> ();
  Ptr<Ipv6> ueIpv6 = ueNode->GetObject<Ipv6> ();
  NS_ASSERT_MSG (ueIpv4 != 0 || ueIpv6 != 0, "UEs need to have IPv4/IPv6 installed before EPS bearers can be activated");

  if (ueIpv4)
    {
      int32_t interface =  ueIpv4->GetInterfaceForDevice (ueDevice);
      if (interface >= 0 && ueIpv4->GetNAddresses (interface) == 1)
        {
          Ipv4Address ueAddr = ueIpv4->GetAddress (interface, 0).GetLocal ();
          NS_LOG_LOGIC (" UE IPv4 address: " << ueAddr);
          m_sgwPgwApp->SetUeAddress (imsi, ueAddr);
        }
    }
  if (ueIpv6)
    {
      int32_t interface6 =  ueIpv6->GetInterfaceForDevice (ueDevice);
      if (interface6 >= 0 && ueIpv6->GetNAddresses (interface6) == 2)
        {
          Ipv6Address ueAddr6 = ueIpv6->GetAddress (interface6, 1).GetAddress ();
          NS_LOG_LOGIC (" UE IPv6 address: " << ueAddr6);
          m_sgwPgwApp->SetUeAddress6 (imsi, ueAddr6);
        }
    }
  uint8_t bearerId = m_mme->AddBearer (imsi, tft, bearer);
  Ptr<LteUeNetDevice> ueLteDevice = ueDevice->GetObject<LteUeNetDevice> ();
  if (ueLteDevice)
    {
      Simulator::ScheduleNow (&EpcUeNas::ActivateEpsBearer, ueLteDevice->GetNas (), bearer, tft);
    }
  return bearerId;
}

Ptr<Node>
PointToPointEpcHelper::GetPgwNode ()
{
  return m_sgwPgw;
}

Ipv4InterfaceContainer 
PointToPointEpcHelper::AssignUeIpv4Address (NetDeviceContainer ueDevices)
{
  return m_uePgwAddressHelper.Assign (ueDevices);
}

Ipv6InterfaceContainer 
PointToPointEpcHelper::AssignUeIpv6Address (NetDeviceContainer ueDevices)
{
  for (NetDeviceContainer::Iterator iter = ueDevices.Begin ();
      iter != ueDevices.End ();
      iter ++)
    {
      Ptr<Icmpv6L4Protocol> icmpv6 = (*iter)->GetNode ()->GetObject<Icmpv6L4Protocol> ();
      icmpv6->SetAttribute ("DAD", BooleanValue (false));
    }
  return m_uePgwAddressHelper6.Assign (ueDevices);
}

Ipv4Address
PointToPointEpcHelper::GetUeDefaultGatewayAddress ()
{
  // return the address of the tun device
  return m_sgwPgw->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
}

Ipv6Address
PointToPointEpcHelper::GetUeDefaultGatewayAddress6 ()
{
  // return the address of the tun device
  return m_sgwPgw->GetObject<Ipv6> ()->GetAddress (1, 1).GetAddress ();
}

} // namespace ns3
