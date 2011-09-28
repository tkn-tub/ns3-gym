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


#include "epc-enb-application.h"
#include "ns3/log.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcEnbApplication");

uint16_t EpcEnbApplication::m_teidCounter = 0;
uint32_t EpcEnbApplication::m_indexCounter = 0;


TypeId
EpcEnbApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcEnbApplication")
    .SetParent<Object> ()
     .AddAttribute("GtpuPort",
                   "UDP Port to be used for GTP-U",
                   UintegerValue (2152),
                   MakeUintegerAccessor (&EpcEnbApplication::m_updPort),
                   MakeUintegerChecker<uint16_t> ());
  return tid;
}

EpcEnbApplication::EpcEnbApplication (Address sgwAddress)
{
  NS_LOG_FUNCTION (this);
}


EpcEnbApplication::~EpcEnbApplication (void)
{
}
 

void 
EpcEnbApplication::RecvFromLteSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);  
  NS_ASSERT (socket == m_lteSocket);
  Ptr<Packet> packet = socket->Recv ();
  GtpuHeader gtpu;
  // TODO: should determine the TEID based on the Radio Bearer ID
  // which should be conveyed by means of a Packet Tag 
  uint32_t teid = 0;
  SendToS1uSocket (packet, teid);
}

void 
EpcEnbApplication::RecvFromS1uSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);  
  NS_ASSERT (socket == m_s1uSocket);
  Ptr<Packet> packet = socket->Recv ();
  GtpuHeader gtpu;
  packet->RemoveHeader (gtpu);
  uint32_t teid = gtpu.GetTeid ();
  uint32_t rbid = GetRbid (teid);
  SendToLteSocket (packet, rbid);
}

void 
EpcEnbApplication::SendToS1uSocket (Ptr<Packet> packet, uint32_t teid)
{
  gtpu.SetTeid (teid);
  // From 3GPP TS 29.281 v10.0.0 Section 5.1
  // Length of the payload + the non obligatory GTP-U header
  gtpu.SetLength (p->GetSize () + h.GetSerializedSize () - 8);  
  packet->AddHeader (gtpu);
  uint32_t flags = 0;
  m_s1uSocket->SendTo (packet, flags, m_sgwAddress);
}


uint32_t
EpcEnbApplication::CreateGtpuTunnel (Ipv4Address destination)
{
  NS_LOG_FUNCTION (this);
  if (m_teidCounter == 0xffffffff)
    {
      NS_FATAL_ERROR ("TEID space exhausted, please implement some TEID reuse mechanism"); 
    }
  CreateGtpuTunnel (destination, ++m_teidCounter);
  return m_teidCounter;
}

void
EpcEnbApplication::CreateGtpuTunnel (Ipv4Address destination, uint32_t teid)
{
  NS_LOG_FUNCTION (this);
  m_gtpuMap[m_indexCounter] = CreateObject<GtpuL5Protocol> (teid);
  m_dstAddrMap[m_indexCounter] = destination;
}

uint32_t 
EpcEnbApplication::GetRbid (uint32_t teid)
{
  // since we don't have SRBs for now, we can just use the same identifiers
  return teid;
}

uint32_t 
EpcEnbApplication::GetTeid (uint32_t rbid)
{
  // since we don't have SRBs for now, we can just use the same identifiers
  return rbid;
}

void
EpcEnbApplication::GtpuRecv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);  
  Ptr<Packet> packet = socket->Recv ();
  uint32_t index = 0;
  m_gtpuMap[index]->RemoveHeader (packet);
  m_tap->Receive (packet, 0x0800, m_tap->GetAddress (), m_tap->GetAddress (), NetDevice::PACKET_HOST);
}

bool
EpcEnbApplication::GtpuSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
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

EpcEnbApplication::~EpcEnbApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

}; // namespace ns3
