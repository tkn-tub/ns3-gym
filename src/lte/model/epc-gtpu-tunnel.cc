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
 * Author: Jaume Nin <jnin@cttc.cat>
 */


#include "epc-gtpu-tunnel.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/boolean.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h"
#include "ns3/mac48-address.h"
#include "ns3/ppp-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("GtpuTunnel");

//m_gtpuPort = 2152;

TypeId
GtpuTunnel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GtpuTunnel")
    .SetParent<Object> ();
  ;
  return tid;
}

GtpuTunnel::GtpuTunnel (Ptr<Node> nA, Ptr<Node> nB, Ipv4Address addrA, Ipv4Address addrB, Ipv4Address tAddrA, Ipv4Address tAddrB, Ipv4Mask m, uint32_t teid):
      m_addressA(addrA), m_addressB(addrB), m_udpPort (2152)
{
  NS_LOG_FUNCTION (this);
  m_gtpu = CreateObject<GtpuL5Protocol> (teid);

  // UDP socket creation and configuration
  m_socketA = Socket::CreateSocket (nA, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  m_socketA->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_udpPort));
  m_socketA->SetRecvCallback (MakeCallback (&GtpuTunnel::GtpuNaRecv, this));

  m_socketB = Socket::CreateSocket (nB, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  m_socketB->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_udpPort));
  m_socketB->SetRecvCallback (MakeCallback (&GtpuTunnel::GtpuNbRecv, this));

  // tap device creation and configuration
  // n0 tap device
  m_tapA = CreateObject<VirtualNetDevice> ();
  m_tapA->SetAddress (Mac48Address::Allocate ());
  m_tapA->SetSendCallback (MakeCallback (&GtpuTunnel::GtpuNaSend, this));
  nA->AddDevice (m_tapA);
  Ptr<Ipv4> ipv4 = nA->GetObject<Ipv4> ();
  uint32_t i = ipv4->AddInterface (m_tapA);
  ipv4->AddAddress (i, Ipv4InterfaceAddress (tAddrA, m));
  ipv4->SetUp (i);

  m_tapB = CreateObject<VirtualNetDevice> ();
  m_tapB->SetAddress (Mac48Address::Allocate ());
  m_tapB->SetSendCallback (MakeCallback (&GtpuTunnel::GtpuNbSend, this));
  nB->AddDevice (m_tapB);
  ipv4 = nB->GetObject<Ipv4> ();
  i = ipv4->AddInterface (m_tapA);
  ipv4->AddAddress (i, Ipv4InterfaceAddress (tAddrB, m));
  ipv4->SetUp (i);

}

void
GtpuTunnel::GtpuNaRecv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> packet = socket->Recv (65535, 0);
  m_gtpu->RemoveHeader (packet);
  m_tapA->Receive (packet, 0x0800, m_tapA->GetAddress (), m_tapA->GetAddress (), NetDevice::PACKET_HOST);
}

void
GtpuTunnel::GtpuNbRecv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> packet = socket->Recv (65535, 0);
  m_gtpu->RemoveHeader (packet);
  m_tapB->Receive (packet, 0x0800, m_tapB->GetAddress (), m_tapB->GetAddress (), NetDevice::PACKET_HOST);
}

bool
GtpuTunnel::GtpuNaSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << source << dest << packet << packet->GetSize ());
  packet = m_gtpu->AddHeader (packet);
  m_socketA->SendTo (packet, 0, InetSocketAddress (m_addressB, m_udpPort));
  return true;
}

bool
GtpuTunnel::GtpuNbSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << source << dest << packet << packet->GetSize ());
  packet = m_gtpu->AddHeader (packet);
  m_socketB->SendTo (packet, 0, InetSocketAddress (m_addressA, m_udpPort));
  return true;
}

GtpuTunnel::~GtpuTunnel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

}; // namespace ns3
