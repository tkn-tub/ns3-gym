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
 *         Nicola Baldo <nbaldo@cttc.cat>
 */


#include "epc-sgw-pgw-application.h"
#include "ns3/log.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcSgwPgwApplication");

uint16_t EpcSgwPgwAplication::m_teidCounter = 0;
uint32_t EpcSgwPgwAplication::m_indexCounter = 0;


TypeId
EpcSgwPgwAplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcSgwPgwApplication")
    .SetParent<Object> ();
  return tid;
}

EpcSgwPgwAplication::EpcSgwPgwAplication (const Ptr<VirtualNetDevice> tap, const Ptr<Socket> s)
  : m_udpPort (2152)
{
  NS_LOG_FUNCTION (this);
  m_tap = tap;
  m_tap->SetSendCallback (MakeCallback (&EpcSgwPgwAplication::GtpuSend, this));
  m_socket = s;
  m_socket->SetRecvCallback (MakeCallback (&EpcSgwPgwAplication::GtpuRecv, this));
}

uint32_t
EpcSgwPgwAplication::CreateGtpuTunnel (Ipv4Address destination)
{
  NS_LOG_FUNCTION (this);
  CreateGtpuTunnel (destination, ++m_teidCounter);
  return m_teidCounter;
}

void
EpcSgwPgwAplication::CreateGtpuTunnel (Ipv4Address destination, uint32_t teid)
{
  NS_LOG_FUNCTION (this);
  m_gtpuMap[m_indexCounter] = CreateObject<GtpuL5Protocol> (teid);
  m_dstAddrMap[m_indexCounter] = destination;
}


void
EpcSgwPgwAplication::GtpuRecv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> packet = socket->Recv (65535, 0);
  uint32_t index = 0;
  m_gtpuMap[index]->RemoveHeader (packet);
  m_tap->Receive (packet, 0x0800, m_tap->GetAddress (), m_tap->GetAddress (), NetDevice::PACKET_HOST);
}

bool
EpcSgwPgwAplication::GtpuSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << source << dest << packet << packet->GetSize ());
  // TODO: Instead of use always index 0 make use of the TFT classifier class, probably
  // a mapping between the tunnel index (assigned during the tunnel creation) and the classifier
  // index (assigned when the first packet is send through) should be maintained.
  uint32_t index = 0;
  packet = m_gtpuMap[index]->AddHeader (packet);
  m_socket->SendTo (packet, 0, InetSocketAddress (m_dstAddrMap[index], m_udpPort));
  return true;
}

EpcSgwPgwAplication::~EpcSgwPgwAplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

}; // namespace ns3
