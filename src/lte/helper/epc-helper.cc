/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 */

#include <ns3/epc-helper.h>
#include <ns3/log.h>
#include "ns3/inet-socket-address.h"
#include "ns3/mac48-address.h"
#include "ns3/epc-gtpu-tunnel-endpoint.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcHelper");

NS_OBJECT_ENSURE_REGISTERED (EpcHelper);

EpcHelper::EpcHelper () 
{
  NS_LOG_FUNCTION (this);

  // since we use point-to-point links for all S1-U links, 
  // we use a /30 subnet which can hold exactly two addresses 
  // (remember that net broadcast and null address are not valid)
  m_s1uIpv4AddressHelper.SetBase ("10.7.0.0", "255.255.255.252");
  
  // create SgwPgwNode
  m_sgwPgw = CreateObject<Node> ();
  
  Ptr<Socket> sgwPgwS1uSocket = Socket::CreateSocket (m_sgwPgw, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  sgwPgwS1uSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_gtpuUdpPort));

  // create SgwPgwApplication

  // create WAN NetDevice?

  // create virtual net device
  
  // interface SgwPgwApplication and virtual net device for tunneling

  // set up static routes appropriately
  

}

EpcHelper::~EpcHelper ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
EpcHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcHelper")
    .SetParent<Object> ()
    .AddConstructor<EpcHelper> ()
    .AddAttribute ("", "The IP address to assign to the tap device, when in ConfigureLocal mode.  "
    Ipv4AddressValue ("255.255.255.255"),
    MakeIpv4AddressAccessor (&TapBridge::m_tapIp),
    MakeIpv4AddressChecker ())
    .AddAttribute ("S1uLinkDataRate", 
                   "The data rate to be used for the next S1-U link to be created",
                   DataRateValue (DataRate ("10Gb/s")),
                   MakeDataRateAccessor (&EpcHelper::m_s1uLinkDataRate),
                   MakeDataRateChecker ())
    .AddAttribute ("S1uLinkDelay", 
                   "The delay to be used for the next S1-U link to be created",
                   TimeValue (Seconds (0)),
                   MakeTimeAccessor (&EpcHelper::m_m_s1uLinkDelay),
                   MakeTimeChecker ())
     .AddAttribute("GtpuPort",
                   "UDP Port to be used for GTP-U",
                   UintegerValue (2152),
                   MakeUintegerAccessor (&EpcHelper::m_gtpuUdpPort),
                   MakeUintegerChecker<uint16_t> ())
  ;
  return tid;
}

void
EpcHelper::CreateSgwPgw ()
{
 
}


void
EpcHelper::AddEnb (Ptr<Node> enb, Ptr<NetDevice> lteEnbNetDevice)
{

  // add an IPv4 stack to the previously created eNB
  InternetStackHelper internet;
  internet.Install (enb);

  // create a point to point link between the new eNB and the SGW with
  // the corresponding new NetDevices on each side  
  NodeContainer enbSgwNodes;
  enbSgwNodes.Add (m_sgwPgw);
  enbSgwNodes.Add (enb);
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (m_s1uLinkDataRate));
  p2ph.SetChannelAttribute ("Delay", TimeValue (m_s1uLinkDelay));  
  NetDeviceContainer enbSgwDevices = p2ph.Install (enb, m_sgwPgw);
  Ptr<NetDevice> enbDev = enbSgwDevices.Get (0);
  Ptr<NetDevice> sgwDev = enbSgwDevices.Get (1);
  m_s1uIpv4AddressHelper.NewNetwork ();
  Ipv4InterfaceContainer enbSgwIpIfaces  m_s1uIpv4AddressHelper.Assign (enbSgwDevices);
  Ipv4Address enbAddress = enbSgwIpIfaces.GetAddress (0);
  Ipv4Address sgwAddress = enbSgwIpIfaces.GetAddress (1);

  // create S1-U socket for the ENB
  Ptr<Socket> enbS1uSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  enbS1uSocket->Bind (InetSocketAddress (enbAddress, m_gtpuUdpPort));

  // create LTE socket for the ENB 
  PacketSocketAddress enbLteSocketAddr;
  enbLteSocketAddr.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  // do we need the following?
  //enbLteSocketAddr.SetPhysicalAddress (devices.Get (1)->GetAddress ());
  //enbLteSocketAddr.SetProtocol (1);

  Ptr<Socket> enbLteSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::PacketSocketFactory"));
  enbLteSocket->Bind (enbLteSocketAddr);

  // create EpcEnbApplication
  Ptr<EpcEnbApplication> = CreateObject<EpcEnbApplication> (enbLteSocket, enbS1uSocket, sgwAddress);

  
}


void
EpcHelper::ActivateEpsBearer ()
{
  // add tunnel at EpcSgwPgwApplication
  // add tunnel at EpcEnbApplication
}


Ptr<Node>
EpcHelper::GetSgwPgwNode ()
{
  return m_sgwPgw;
}


void
EpcHelper::InstallGtpu (Ptr<Node> n)
{
  InstallGtpu (n, m_ipv4.NewAddress ());
}

void
EpcHelper::InstallGtpu (Ptr<Node> n, Ipv4Address addr)
{
  NS_LOG_FUNCTION (this);
  // UDP socket creation and configuration
  Ptr<Socket> m_socket = Socket::CreateSocket (n, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  m_socket->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_udpPort));

  // tap device creation and configuration
  Ptr<VirtualNetDevice> m_tap = CreateObject<VirtualNetDevice> ();
  m_tap->SetAddress (Mac48Address::Allocate ());

  n->AddDevice (m_tap);
  Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
  uint32_t i = ipv4->AddInterface (m_tap);
  ipv4->AddAddress (i, Ipv4InterfaceAddress (addr, m_mask));
  ipv4->SetUp (i);
  Ptr<GtpuTunnelEndpoint> tunnel = CreateObject<GtpuTunnelEndpoint> (m_tap, m_socket);
  m_gtpuEndpoint[n] = tunnel;
}

void
EpcHelper::CreateGtpuTunnel (Ptr<Node> n, Ipv4Address nAddr, Ptr<Node> m, Ipv4Address mAddr)
{
  uint32_t teid = m_gtpuEndpoint[n]->CreateGtpuTunnel (mAddr);
  m_gtpuEndpoint[m]->CreateGtpuTunnel (nAddr, teid);
}


} // namespace ns3
