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
#include "ns3/uinteger.h"
#include "ns3/epc-gtpu-header.h"
#include "ns3/lte-radio-bearer-tag.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcEnbApplication");


TypeId
EpcEnbApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcEnbApplication")
    .SetParent<Object> ();
  return tid;
}

EpcEnbApplication::EpcEnbApplication (Ptr<Socket> lteSocket, Ptr<Socket> s1uSocket, Ipv4Address sgwAddress)
  : m_lteSocket (lteSocket),
    m_s1uSocket (s1uSocket),    
    m_sgwAddress (sgwAddress),
    m_gtpuUdpPort (2152) // fixed by the standard
{
  NS_LOG_FUNCTION (this << lteSocket << s1uSocket << sgwAddress);
  m_s1uSocket->SetRecvCallback (MakeCallback (&EpcEnbApplication::RecvFromS1uSocket, this));
  m_lteSocket->SetRecvCallback (MakeCallback (&EpcEnbApplication::RecvFromLteSocket, this));
}


EpcEnbApplication::~EpcEnbApplication (void)
{
  NS_LOG_FUNCTION (this);
}
 
void 
EpcEnbApplication::ErabSetupRequest (uint32_t teid, uint16_t rnti, uint8_t lcid)
{
  NS_LOG_FUNCTION (this << teid << rnti << (uint16_t) lcid);
  LteFlowId_t rbid (rnti, lcid);
  // side effect: create entries if not exist
  m_rbidTeidMap[rbid] = teid;
  m_teidRbidMap[teid] = rbid;
}

void 
EpcEnbApplication::RecvFromLteSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);  
  NS_ASSERT (socket == m_lteSocket);
  Ptr<Packet> packet = socket->Recv ();

  // workaround for bug 231 https://www.nsnam.org/bugzilla/show_bug.cgi?id=231
  SocketAddressTag satag;
  packet->RemovePacketTag (satag);

  LteRadioBearerTag tag;
  bool found = packet->RemovePacketTag (tag);
  NS_ASSERT (found);
  LteFlowId_t flowId;
  flowId.m_rnti = tag.GetRnti ();
  flowId.m_lcId = tag.GetLcid ();
  NS_LOG_LOGIC ("received packet with RNTI=" << flowId.m_rnti << ", LCID=" << (uint16_t)  flowId.m_lcId);
  std::map<LteFlowId_t, uint32_t>::iterator it = m_rbidTeidMap.find (flowId);
  NS_ASSERT (it != m_rbidTeidMap.end ());
  uint32_t teid = it->second;
  SendToS1uSocket (packet, teid);
}

void 
EpcEnbApplication::RecvFromS1uSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);  
  NS_ASSERT (socket == m_s1uSocket);
  Ptr<Packet> packet = socket->Recv ();
  GtpuHeader gtpu;
  packet->RemoveHeader (gtpu);
  uint32_t teid = gtpu.GetTeid ();
  std::map<uint32_t, LteFlowId_t>::iterator it = m_teidRbidMap.find (teid);
  NS_ASSERT (it != m_teidRbidMap.end ());

  // workaround for bug 231 https://www.nsnam.org/bugzilla/show_bug.cgi?id=231
  SocketAddressTag tag;
  packet->RemovePacketTag (tag);
  
  SendToLteSocket (packet, it->second.m_rnti, it->second.m_lcId);
}

void 
EpcEnbApplication::SendToLteSocket (Ptr<Packet> packet, uint16_t rnti, uint8_t lcid)
{
  NS_LOG_FUNCTION (this << packet << rnti << (uint16_t) lcid);  
  LteRadioBearerTag tag (rnti, lcid);
  packet->AddPacketTag (tag);
  int sentBytes = m_lteSocket->Send (packet);
  NS_ASSERT (sentBytes > 0);
}


void 
EpcEnbApplication::SendToS1uSocket (Ptr<Packet> packet, uint32_t teid)
{
  NS_LOG_FUNCTION (this << packet << teid);  
  GtpuHeader gtpu;
  gtpu.SetTeid (teid);
  // From 3GPP TS 29.281 v10.0.0 Section 5.1
  // Length of the payload + the non obligatory GTP-U header
  gtpu.SetLength (packet->GetSize () + gtpu.GetSerializedSize () - 8);  
  packet->AddHeader (gtpu);
  uint32_t flags = 0;
  m_s1uSocket->SendTo (packet, flags, InetSocketAddress(m_sgwAddress, m_gtpuUdpPort));
}


}; // namespace ns3
