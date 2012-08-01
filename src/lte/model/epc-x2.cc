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
  
  NS_LOG_LOGIC ("X2 header: " << x2Header);

  uint8_t messageType = x2Header.GetMessageType ();
  uint8_t procedureCode = x2Header.GetProcedureCode ();

  NS_LOG_LOGIC ("messageType = " << (uint32_t)messageType);
  NS_LOG_LOGIC ("procedureCode = " << (uint32_t)procedureCode);

  if (procedureCode == EpcX2Header::HandoverPreparation)
    {
      if (messageType == EpcX2Header::InitiatingMessage)
        {
          NS_LOG_LOGIC ("Recv X2 message: HANDOVER REQUEST");

          EpcX2HandoverRequestHeader x2HoReqHeader;
          packet->RemoveHeader (x2HoReqHeader);

          NS_LOG_INFO ("X2 HandoverRequest header: " << x2HoReqHeader);

          NS_ASSERT_MSG (m_x2InterfaceCellIds.find (socket) != m_x2InterfaceCellIds.end (),
                         "Missing infos of local and remote CellId");
          Ptr<X2CellInfo> cellsInfo = m_x2InterfaceCellIds [socket];

          EpcX2SapUser::HandoverRequestParams params;
          params.oldEnbUeX2apId = x2HoReqHeader.GetOldEnbUeX2apId ();
          params.cause          = x2HoReqHeader.GetCause ();
          params.sourceCellId   = cellsInfo->m_remoteCellId;
          params.targetCellId   = x2HoReqHeader.GetTargetCellId ();
          params.ueAggregateMaxBitRateDownlink = x2HoReqHeader.GetUeAggregateMaxBitRateDownlink ();
          params.ueAggregateMaxBitRateUplink   = x2HoReqHeader.GetUeAggregateMaxBitRateUplink ();
          params.bearers        = x2HoReqHeader.GetBearers ();
          params.rrcContext     = packet;

          NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
          NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);
          NS_LOG_LOGIC ("cellsInfo->m_localCellId = " << cellsInfo->m_localCellId);
          NS_ASSERT_MSG (params.targetCellId == cellsInfo->m_localCellId,
                         "TargetCellId mismatches with localCellId");

          m_x2SapUser->RecvHandoverRequest (params);
        }
      else // messageType == SuccessfulOutcome
        {
          NS_LOG_LOGIC ("Recv X2 message: HANDOVER REQUEST ACK");

          EpcX2HandoverRequestAckHeader x2HoReqAckHeader;
          packet->RemoveHeader (x2HoReqAckHeader);

          NS_ASSERT_MSG (m_x2InterfaceCellIds.find (socket) != m_x2InterfaceCellIds.end (),
                         "Missing infos of local and remote CellId");
          Ptr<X2CellInfo> cellsInfo = m_x2InterfaceCellIds [socket];

          EpcX2SapUser::HandoverRequestAckParams params;
          params.oldEnbUeX2apId = x2HoReqAckHeader.GetOldEnbUeX2apId ();
          params.newEnbUeX2apId = x2HoReqAckHeader.GetNewEnbUeX2apId ();
          params.sourceCellId   = cellsInfo->m_localCellId;
          params.targetCellId   = cellsInfo->m_remoteCellId;
          params.admittedBearers = x2HoReqAckHeader.GetAdmittedBearers ();
          params.notAdmittedBearers = x2HoReqAckHeader.GetNotAdmittedBearers ();
          params.rrcContext     = packet;

          NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
          NS_LOG_LOGIC ("newEnbUeX2apId = " << params.newEnbUeX2apId);
          NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
          NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);

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

  NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
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
  EpcX2HandoverRequestHeader x2HoReqHeader;
  x2HoReqHeader.SetOldEnbUeX2apId (params.oldEnbUeX2apId);
  x2HoReqHeader.SetCause (params.cause);
  x2HoReqHeader.SetTargetCellId (params.targetCellId);
  x2HoReqHeader.SetUeAggregateMaxBitRateDownlink (params.ueAggregateMaxBitRateDownlink);
  x2HoReqHeader.SetUeAggregateMaxBitRateUplink (params.ueAggregateMaxBitRateUplink);
  x2HoReqHeader.SetBearers (params.bearers);

  EpcX2Header x2Header;
  x2Header.SetMessageType (EpcX2Header::InitiatingMessage);
  x2Header.SetProcedureCode (EpcX2Header::HandoverPreparation);
  x2Header.SetLengthOfIes (x2HoReqHeader.GetLengthOfIes ());
  x2Header.SetNumberOfIes (x2HoReqHeader.GetNumberOfIes ());

  NS_LOG_INFO ("X2 header: " << x2Header);
  NS_LOG_INFO ("X2 HandoverRequest header: " << x2HoReqHeader);

  // Build the X2 packet
  Ptr<Packet> packet = params.rrcContext;
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

  NS_LOG_LOGIC ("oldEnbUeX2apId = " << params.oldEnbUeX2apId);
  NS_LOG_LOGIC ("newEnbUeX2apId = " << params.newEnbUeX2apId);
  NS_LOG_LOGIC ("sourceCellId = " << params.sourceCellId);
  NS_LOG_LOGIC ("targetCellId = " << params.targetCellId);

  NS_ASSERT_MSG (m_x2InterfaceSockets.find (params.sourceCellId) != m_x2InterfaceSockets.end (),
                 "Socket infos not defined for sourceCellId = " << params.sourceCellId);

  Ptr<Socket> localSocket = m_x2InterfaceSockets [params.sourceCellId]->m_localSocket;
  Ipv4Address remoteIpAddr = m_x2InterfaceSockets [params.sourceCellId]->m_remoteIpAddr;

  NS_LOG_LOGIC ("localSocket = " << localSocket);
  NS_LOG_LOGIC ("remoteIpAddr = " << remoteIpAddr);

  NS_LOG_INFO ("Send X2 message: HANDOVER REQUEST ACK");

  // Build the X2 message
  EpcX2Header x2Header;
  x2Header.SetMessageType (EpcX2Header::SuccessfulOutcome);
  x2Header.SetProcedureCode (EpcX2Header::HandoverPreparation);

  EpcX2HandoverRequestAckHeader x2HoAckHeader;
  x2HoAckHeader.SetOldEnbUeX2apId (params.oldEnbUeX2apId);
  x2HoAckHeader.SetNewEnbUeX2apId (params.newEnbUeX2apId);
  x2HoAckHeader.SetAdmittedBearers (params.admittedBearers);
  x2HoAckHeader.SetNotAdmittedBearers (params.notAdmittedBearers);

  NS_LOG_INFO ("X2 header: " << x2Header);
  NS_LOG_INFO ("X2 HandoverAck header: " << x2HoAckHeader);
  NS_LOG_INFO ("RRC context: " << params.rrcContext);

  // Build the X2 packet
  Ptr<Packet> packet = (params.rrcContext != 0) ? (params.rrcContext) : (Create <Packet> ());
  packet->AddHeader (x2HoAckHeader);
  packet->AddHeader (x2Header);
  NS_LOG_INFO ("packetLen = " << packet->GetSize ());

  // Send the X2 message through the socket
  localSocket->SendTo (packet, 0, InetSocketAddress (remoteIpAddr, m_x2cUdpPort));
}


} // namespace ns3
