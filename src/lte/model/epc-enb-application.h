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

#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/virtual-net-device.h"
#include "ns3/epc-gtpu-l5-protocol.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include <map>

namespace ns3 {

/**
 * \ingroup lte
 *
 * GTPv1-U endpoint for all the tunnels of a given node. It encapsulates all the tunnel logic for creating tunnels and encapsulating data through it
 */
class EpcEnbApplication : public Object
{

public:
  static TypeId GetTypeId (void);
  

  /** 
   * 
   * 
   * \param radioSocket the socket to be used to send/receive packets to/from the LTE radio interface
   * \param s1uSocket the socket to be used to send/receive packets
   * to/from the S1-U interface connected with the SGW 
   * \param sgwAddress the IPv4 address at which this eNB will be able to reach its SGW
   * 
   */
  EpcEnbApplication (Ptr<Socket> radioSocket, Ptr<Socket> s1uSocket, Address sgwAddress);

  /**
   * destructor
   * 
   */
  virtual ~EpcEnbApplication (void);



  void SetupS1Bearer ()

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
  void SendToLteSocket (Ptr<Packet> packet, uint32_t rbid);


  /** 
   * Send a packet to the SGW via the S1-U interface
   * 
   * \param packet packet to be sent
   * \param teid the Tunnel Enpoint IDentifier
   */
  void SendToS1uSocket (Ptr<Packet> packet, uint32_t teid);


private:


  /**
   * Creates a GTPv1-U tunnel between the given destination and the enpoint
   * using the specified TEID.
   * \param destination IP address of the other end of the tunnel
   * \param teid Tunnel Endpoint IDentifier to be assigned to the tunnel
   */
  void CreateGtpuTunnel (Ipv4Address destination, uint32_t teid);

  /**
   * Creates a GTPv1-U tunnel between the given destination and the enpoint. The
   * TEID is automatically sellected.
   * \param destination IP address of the other end of the tunnel
   * \return the Tunnel Endpoint IDentifier (TEID) assigned to the tunnel
   */
  uint32_t CreateGtpuTunnel (Ipv4Address destination);


  /** 
   * this function implements the 1-to-1 mapping between S1 Bearers and Radio Bearers
   * 
   * \param teid the Tunnel Endpoint IDentifier (TEID) that identifies an S1-bearer on this eNB
   * 
   * \return the corresponding Radio Bearer Identifier
   */
  uint32_t GetRbid (uint32_t teid);


  /** 
   * this function implements the 1-to-1 mapping between Radio Bearers and S1 Bearers 
   * 
   * \param rbid the Radio Bearer Identifier
   * 
   * \return the corresponding the Tunnel Endpoint IDentifier (TEID) that identifies an S1-bearer on this eNB
   */
  uint32_t GetTeid (uint32_t rbid);

  /**
   * raw packet socket to send and receive the packets to and from the LTE radio interface
   */
  Ptr<Socket> m_lteSocket;

  /**
   * UDP socket to send and receive GTP-U the packets to and from the S1-U interface
   */
  Ptr<Socket> m_epcSocket;

  /**
   * UDP port where the GTP-U Socket is bound, fixed by the standard as 2152
   */
  uint16_t m_udpPort;

  /**
   * Map holding the GTP instances of the active tunnels on this endpoint
   */
  std::map<uint32_t, Ptr<GtpuL5Protocol> > m_gtpuMap;

  /**
   * address of the SGW which terminates all tunnels
   */
  Ipv4Address m_sgwAddress;

  static uint16_t m_teidCounter;
  static uint32_t m_indexCounter;
};

} //namespace ns3

#endif /* EPC_ENB_APPLICATION_H */

