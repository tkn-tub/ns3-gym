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
 */

#include <ns3/epc-helper.h>
#include <ns3/log.h>
#include "ns3/inet-socket-address.h"
#include "ns3/mac48-address.h"
#include "ns3/epc-gtpu-tunnel-endpoint.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcHelper");

NS_OBJECT_ENSURE_REGISTERED (EpcHelper);

EpcHelper::EpcHelper () : m_udpPort (2152)
{
  NS_LOG_FUNCTION (this);
  m_mask = "255.255.255.0";
  m_ipv4.SetBase ( "100.0.0.0", m_mask);
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
  ;
  return tid;
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
