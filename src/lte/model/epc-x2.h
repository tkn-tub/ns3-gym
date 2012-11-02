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

#ifndef EPC_X2_H
#define EPC_X2_H

#include "ns3/socket.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"

#include "ns3/epc-x2-sap.h"

namespace ns3 {

  
class X2IfaceInfo : public SimpleRefCount<X2IfaceInfo>
{
public:
  X2IfaceInfo (Ptr<Socket> localSocket, Ipv4Address remoteIpAddr);
  virtual ~X2IfaceInfo (void);
  
  X2IfaceInfo& operator= (const X2IfaceInfo &);

public:
  Ptr<Socket>   m_localSocket;
  Ipv4Address   m_remoteIpAddr;
};


class X2CellInfo : public SimpleRefCount<X2CellInfo>
{
public:
  X2CellInfo (uint16_t localCellId, uint16_t remoteCellId);
  virtual ~X2CellInfo (void);
  
  X2CellInfo& operator= (const X2CellInfo &);

public:
  uint16_t m_localCellId;
  uint16_t m_remoteCellId;
};


/**
 * \ingroup lte
 *
 * This entity is installed inside an eNB and provides the functionality for the X2 interface
 */
class EpcX2 : public Object
{
  friend class EpcX2SpecificEpcX2SapProvider<EpcX2>;

public:
  /** 
   * Constructor
   */
  EpcX2 ();

  /**
   * Destructor
   */
  virtual ~EpcX2 (void);
  
  static TypeId GetTypeId (void);


  /**
   * \param s the X2 SAP User to be used by this EPC X2 entity
   */
  void SetEpcX2SapUser (EpcX2SapUser * s);

  /**
   * \param s the X2 SAP Provider interface offered by this EPC X2 entity
   */
  EpcX2SapProvider* GetEpcX2SapProvider ();


  /**
   * \param s the X2 SAP Provider interface offered by this EPC X2 entity
   */
  void AddX2Interface (uint16_t enb1CellId, Ptr<Socket> enb1X2cSocket, uint16_t enb2CellId, Ptr<Socket> enb2X2cSocket);


  /** 
   * Method to be assigned to the recv callback of the X2 socket.
   * It is called when the eNB receives a packet from the peer eNB of the X2 interface
   * 
   * \param socket socket of the X2 interface
   */
  void RecvFromX2cSocket (Ptr<Socket> socket);


protected:
  // Interface provided by LteRlcSapProvider
  virtual void DoSendHandoverRequest (EpcX2SapProvider::HandoverRequestParams params);
  virtual void DoSendHandoverRequestAck (EpcX2SapProvider::HandoverRequestAckParams params);
  virtual void DoSendUeContextRelease (EpcX2SapProvider::UeContextReleaseParams params);

  EpcX2SapUser* m_x2SapUser;
  EpcX2SapProvider* m_x2SapProvider;


private:

  /**
   * Map the enbUeX2apId to the corresponding cellId where the UE is camping on
   */
  std::map < uint16_t, uint16_t > m_x2Ues;

  /**
   * Map the targetCellId to the corresponding (sourceSocket, remoteIpAddr) to be used
   * to send the X2 message
   */
  std::map < uint16_t, Ptr<X2IfaceInfo> > m_x2InterfaceSockets;

  /**
   * Map the localSocket (the one receiving the X2 message) 
   * to the corresponding (sourceCellId, targetCellId) associated with the X2 interface
   */
  std::map < Ptr<Socket>, Ptr<X2CellInfo> > m_x2InterfaceCellIds;

  /**
   * UDP port to be used for the X2 interface
   */
   uint16_t m_x2cUdpPort;

};

} //namespace ns3

#endif // EPC_X2_H
