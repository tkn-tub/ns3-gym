/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/lte-enb-net-device.h"

#include "ns3/epc-x2-header.h"
#include "ns3/epc-x2.h"

NS_LOG_COMPONENT_DEFINE ("EpcX2");

namespace ns3 {


X2IfaceInfo::X2IfaceInfo (Ptr<Socket> localSocket, Ipv4Address remoteIpAddr)
{
  m_localSocket = localSocket;
  m_remoteIpAddr = remoteIpAddr;
}

X2IfaceInfo::~X2IfaceInfo (void)
{
}

X2IfaceInfo& 
X2IfaceInfo::operator= (const X2IfaceInfo& value)
{
  NS_LOG_FUNCTION (this);
  m_localSocket = value.m_localSocket;
  m_remoteIpAddr = value.m_remoteIpAddr;
  return *this;
}

///////////////////////////////////////////

X2CellInfo::X2CellInfo (uint16_t localCellId, uint16_t remoteCellId)
{
  m_localCellId = localCellId;
  m_remoteCellId = remoteCellId;
}

X2CellInfo::~X2CellInfo (void)
{
}

X2CellInfo& 
X2CellInfo::operator= (const X2CellInfo& value)
{
  NS_LOG_FUNCTION (this);
  m_localCellId = value.m_localCellId;
  m_remoteCellId = value.m_remoteCellId;
  return *this;
}

///////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (EpcX2);

EpcX2::EpcX2 ()
  : m_x2cUdpPort (4444)
{
  NS_LOG_FUNCTION (this);
  
  m_x2SapProvider = new EpcX2SpecificEpcX2SapProvider<EpcX2> (this);
}

TypeId
EpcX2::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcX2")
    .SetParent<Object> ();
  return tid;
}

EpcX2::~EpcX2 (void)
{
  NS_LOG_FUNCTION (this);
  delete m_x2SapProvider;
}

void
EpcX2::SetEpcX2SapUser (EpcX2SapUser * s)
{
  NS_LOG_FUNCTION (this << s);
  m_x2SapUser = s;
}

EpcX2SapProvider*
EpcX2::GetEpcX2SapProvider ()
{
  NS_LOG_FUNCTION (this);
  return m_x2SapProvider;
}


void
EpcX2::AddX2Interface (uint16_t enb1CellId, Ptr<Socket> enb1X2cSocket, uint16_t enb2CellId, Ptr<Socket> enb2X2cSocket)
{
  NS_LOG_FUNCTION (this << enb1CellId << enb1X2cSocket << enb2CellId << enb2X2cSocket);

  Address addr;
  int retval;
  
  retval = enb1X2cSocket->GetSockName (addr);
  NS_ASSERT (retval == 0);
  InetSocketAddress localInetAddr = InetSocketAddress::ConvertFrom (addr);
  NS_LOG_LOGIC ("local IP address = " << localInetAddr.GetIpv4 ());

  retval = enb2X2cSocket->GetSockName (addr);
  NS_ASSERT (retval == 0);
  InetSocketAddress remoteInetAddr = InetSocketAddress::ConvertFrom (addr);
  NS_LOG_LOGIC ("remote IP address = " << remoteInetAddr.GetIpv4 ());

  enb1X2cSocket->SetRecvCallback (MakeCallback (&EpcX2::RecvFromX2cSocket, this));

  NS_ASSERT_MSG (m_x2InterfaceSockets.find (enb2CellId) == m_x2InterfaceSockets.end (),
                 "Mapping for remoteCellId = " << enb2CellId << " is already known");
  m_x2InterfaceSockets [enb2CellId] = Create<X2IfaceInfo> (enb1X2cSocket, remoteInetAddr.GetIpv4 ());

  NS_ASSERT_MSG (m_x2InterfaceCellIds.find (enb1X2cSocket) == m_x2InterfaceCellIds.end (),
                 "Mapping for localSocket = " << enb1X2cSocket << " is already known");
  m_x2InterfaceCellIds [enb1X2cSocket] = Create<X2CellInfo> (enb1CellId, enb2CellId);
}


void 
EpcX2::RecvFromX2cSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  NS_LOG_LOGIC ("Recv X2 message: from Socket");
  Ptr<Packet> packet = socket->Recv ();
  NS_LOG_LOGIC ("packetLen = " << packet->GetSize ());

  EpcX2Header x2Header;
  packet->RemoveHeader (x2Header);
  
  uint8_t messageType = x2Header.GetMessageType ();
  uint8_t procedureCode = x2Header.GetProcedureCode ();

  NS_LOG_LOGIC ("messageType = " << (uint32_t)messageType);
  NS_LOG_LOGIC ("procedureCode = " << (uint32_t)procedureCode);

  if (procedureCode == EpcX2Header::HANDOVER_PREPARATION_TYPE)
    {
      if (messageType == EpcX2Header::INITIATING_MESSAGE)
        {
          NS_LOG_LOGIC ("Recv X2 message: HANDOVER REQUEST");

          EpcX2HandoverRequestHeader x2HoReqHeader;
          packet->RemoveHeader (x2HoReqHeader);

          NS_ASSERT_MSG (m_x2InterfaceCellIds.find (socket) != m_x2InterfaceCellIds.end (),
                         "Missing infos of local and remote CellId");
          Ptr<X2CellInfo> cellsInfo = m_x2InterfaceCellIds [socket];

          EpcX2SapUser::HandoverRequestParams params;
          params.cause          = x2HoReqHeader.GetCause ();
          params.sourceCellId   = cellsInfo->m_remoteCellId;
          params.targetCellId   = x2HoReqHeader.GetTargetCellId ();
          NS_ASSERT_MSG (params.targetCellId == cellsInfo->m_localCellId,
                         "TargetCellId mismatches with localCellId");

          m_x2SapUser->RecvHandoverRequest (params);
        }
      else // messageType == SUCCESSFUL_OUTCOME
        {
          NS_LOG_LOGIC ("Recv X2 message: HANDOVER REQUEST ACK");

          EpcX2HandoverRequestAckHeader x2HoReqAckHeader;
          packet->RemoveHeader (x2HoReqAckHeader);

          NS_ASSERT_MSG (m_x2InterfaceCellIds.find (socket) != m_x2InterfaceCellIds.end (),
                         "Missing infos of local and remote CellId");
          Ptr<X2CellInfo> cellsInfo = m_x2InterfaceCellIds [socket];

          EpcX2SapUser::HandoverRequestAckParams params;
          params.cause          = x2HoReqAckHeader.GetCause ();
          params.sourceCellId   = cellsInfo->m_localCellId;
          params.targetCellId   = cellsInfo->m_remoteCellId;

          m_x2SapUser->RecvHandoverRequestAck (params);
        }
    }

}

//
// Implementation of the X2 SAP Provider
//
void
EpcX2::DoSendHandoverRequest (EpcX2SapProvider::HandoverRequestParams params)
{
  NS_LOG_FUNCTION (this);

  NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);

  NS_ASSERT_MSG (m_x2InterfaceSockets.find (params.targetCellId) != m_x2InterfaceSockets.end (),
                 "Missing infos for targetCellId = " << params.targetCellId);
  Ptr<X2IfaceInfo> socketInfo = m_x2InterfaceSockets [params.targetCellId];
  Ptr<Socket> sourceSocket = socketInfo->m_localSocket;
  Ipv4Address targetIpAddr = socketInfo->m_remoteIpAddr;

  NS_LOG_LOGIC ("sourceSocket = " << sourceSocket);
  NS_LOG_LOGIC ("targetIpAddr = " << targetIpAddr);
  
  NS_LOG_INFO ("Send X2 message: HANDOVER REQUEST");

  // Build the X2 message
  EpcX2Header x2Header;
  x2Header.SetMessageType (EpcX2Header::INITIATING_MESSAGE);
  x2Header.SetProcedureCode (EpcX2Header::HANDOVER_PREPARATION_TYPE);

  EpcX2HandoverRequestHeader x2HoReqHeader;
  x2HoReqHeader.SetCause (1111);
  x2HoReqHeader.SetTargetCellId (params.targetCellId);

  // Build the X2 packet
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (x2HoReqHeader);
  packet->AddHeader (x2Header);
  NS_LOG_INFO ("packetLen = " << packet->GetSize ());

  // Send the X2 message through the socket
  sourceSocket->SendTo (packet, 0, InetSocketAddress (targetIpAddr, m_x2cUdpPort));
  
}


void
EpcX2::DoSendHandoverRequestAck (EpcX2SapProvider::HandoverRequestAckParams params)
{
  NS_LOG_FUNCTION (this);

  NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);

  NS_ASSERT_MSG (m_x2InterfaceSockets.find (params.sourceCellId) != m_x2InterfaceSockets.end (),
                 "Socket infos not defined for sourceCellId = " << params.sourceCellId);

  Ptr<Socket> localSocket = m_x2InterfaceSockets [params.sourceCellId]->m_localSocket;
  Ipv4Address remoteIpAddr = m_x2InterfaceSockets [params.sourceCellId]->m_remoteIpAddr;

  NS_LOG_LOGIC ("localSocket = " << localSocket);
  NS_LOG_LOGIC ("remoteIpAddr = " << remoteIpAddr);

  // Build the X2 message
  EpcX2Header x2Header;
  x2Header.SetMessageType (EpcX2Header::SUCCESSFUL_OUTCOME);
  x2Header.SetProcedureCode (EpcX2Header::HANDOVER_PREPARATION_TYPE);

  EpcX2HandoverRequestAckHeader x2HoReqHeader;
  x2HoReqHeader.SetCause (2222);
  x2HoReqHeader.SetTargetCellId (params.targetCellId);

  // Build the X2 packet
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (x2HoReqHeader);
  packet->AddHeader (x2Header);

  NS_LOG_INFO ("Send X2 message: HANDOVER REQUEST ACK");

  localSocket->SendTo (packet, 0, InetSocketAddress (remoteIpAddr, m_x2cUdpPort));
}


} // namespace ns3
