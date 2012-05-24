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

#ifndef EPC_ENB_APPLICATION_H
#define EPC_ENB_APPLICATION_H

#include <ns3/address.h>
#include <ns3/socket.h>
#include <ns3/virtual-net-device.h>
#include <ns3/traced-callback.h>
#include <ns3/callback.h>
#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/lte-common.h>
#include <ns3/application.h>
#include <map>

namespace ns3 {

/**
 * \ingroup lte
 *
 * This application is installed inside eNBs and provides the bridge functionality for user data plane packets between the radio interface and the S1-U interface.
 */
class EpcEnbApplication : public Application
{

public:

  // inherited from Object
  static TypeId GetTypeId (void);
  

  /** 
   * Constructor
   * 
   * \param lteSocket the socket to be used to send/receive packets to/from the LTE radio interface
   * \param s1uSocket the socket to be used to send/receive packets
   * to/from the S1-U interface connected with the SGW 
   * \param sgwAddress the IPv4 address at which this eNB will be able to reach its SGW
   * 
   */
  EpcEnbApplication (Ptr<Socket> lteSocket, Ptr<Socket> s1uSocket, Ipv4Address sgwAddress);

  /**
   * Destructor
   * 
   */
  virtual ~EpcEnbApplication (void);



  /** 
   * This method is triggered after the eNB received
   * a S1-AP message of type E-RAB Setup Request by the MME and the RadioBearer has already been created
   * 
   * \param teid the Tunnel Endpoint IDentifier of the S1-bearer to be setup.
   * \param rnti the unique ID of the UE on this eNB
   * \param lcid the Logical Channel ID identifying the Radio Bearer
   */
  void ErabSetupRequest (uint32_t teid, uint16_t rnti, uint8_t lcid);

  /** 
   * Method to be assigned to the recv callback of the LTE socket. It is called when the eNB receives a data packet from the radio interface that is to be forwarded to the SGW.
   * 
   * \param socket pointer to the LTE socket
   */
  void RecvFromLteSocket (Ptr<Socket> socket);


  /** 
   * Method to be assigned to the recv callback of the S1-U socket. It is called when the eNB receives a data packet from the SGW that is to be forwarded to the UE.
   * 
   * \param socket pointer to the S1-U socket
   */
  void RecvFromS1uSocket (Ptr<Socket> socket);

  /** 
   * Send a packet to the UE via the LTE radio interface of the eNB
   * 
   * \param packet t
   * \param rbid the Radio Bearer IDentifier
   */
  void SendToLteSocket (Ptr<Packet> packet, uint16_t rnti, uint8_t lcid);


  /** 
   * Send a packet to the SGW via the S1-U interface
   * 
   * \param packet packet to be sent
   * \param teid the Tunnel Enpoint IDentifier
   */
  void SendToS1uSocket (Ptr<Packet> packet, uint32_t teid);


private:


  /**
   * raw packet socket to send and receive the packets to and from the LTE radio interface
   */
  Ptr<Socket> m_lteSocket;

  /**
   * UDP socket to send and receive GTP-U the packets to and from the S1-U interface
   */
  Ptr<Socket> m_s1uSocket;

  /**
   * address of the SGW which terminates all S1-U tunnels
   */
  Ipv4Address m_sgwAddress;

  /**
   * map telling for each RadioBearer (RNTI,LCID) the corresponding  S1-U TEID
   * 
   */
  std::map<LteFlowId_t, uint32_t> m_rbidTeidMap;  

  /**
   * map telling for each S1-U TEID the corresponding RadioBearer (RNTI,LCID) 
   * 
   */
  std::map<uint32_t, LteFlowId_t> m_teidRbidMap;
 
  /**
   * UDP port to be used for GTP
   */
  uint16_t m_gtpuUdpPort;

};

} //namespace ns3

#endif /* EPC_ENB_APPLICATION_H */

