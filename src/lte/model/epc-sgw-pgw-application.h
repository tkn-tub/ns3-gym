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

#ifndef EPC_SGW_PGW_APPLICATION_H
#define EPC_SGW_PGW_APPLICATION_H

#include <ns3/address.h>
#include <ns3/socket.h>
#include <ns3/virtual-net-device.h>
#include <ns3/traced-callback.h>
#include <ns3/callback.h>
#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/eps-bearer.h>
#include <ns3/epc-tft.h>
#include <ns3/epc-tft-classifier.h>
#include <ns3/lte-common.h>
#include <ns3/application.h>
#include <ns3/epc-s1ap-sap.h>
#include <ns3/epc-s11-sap.h>
#include <map>

namespace ns3 {

/**
 * \ingroup lte
 *
 * This application implements the SGW/PGW functionality.
 */
class EpcSgwPgwApplication : public Application
{
  /// allow MemberEpcS11SapSgw<EpcSgwPgwApplication> class friend access
  friend class MemberEpcS11SapSgw<EpcSgwPgwApplication>;

public:

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * Constructor that binds the tap device to the callback methods.
   *
   * \param tunDevice TUN VirtualNetDevice used to tunnel IP packets from
   * the Gi interface of the PGW/SGW over the
   * internet over GTP-U/UDP/IP on the S1-U interface
   * \param s1uSocket socket used to send GTP-U packets to the eNBs
   */

  EpcSgwPgwApplication (const Ptr<VirtualNetDevice> tunDevice, const Ptr<Socket> s1uSocket);

  /** 
   * Destructor
   */
  virtual ~EpcSgwPgwApplication (void);
  
  /** 
   * Method to be assigned to the callback of the Gi TUN VirtualNetDevice. It
   * is called when the SGW/PGW receives a data packet from the
   * internet (including IP headers) that is to be sent to the UE via
   * its associated eNB, tunneling IP over GTP-U/UDP/IP.
   * 
   * \param packet 
   * \param source 
   * \param dest 
   * \param protocolNumber 
   * \return true always 
   */
  bool RecvFromTunDevice (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);


  /** 
   * Method to be assigned to the recv callback of the S1-U socket. It
   * is called when the SGW/PGW receives a data packet from the eNB
   * that is to be forwarded to the internet. 
   * 
   * \param socket pointer to the S1-U socket
   */
  void RecvFromS1uSocket (Ptr<Socket> socket);

  /** 
   * Send a packet to the internet via the Gi interface of the SGW/PGW
   * 
   * \param packet 
   * \param teid the Tunnel Enpoint Identifier
   */
  void SendToTunDevice (Ptr<Packet> packet, uint32_t teid);


  /** 
   * Send a packet to the SGW via the S1-U interface
   * 
   * \param packet packet to be sent
   * \param enbS1uAddress the address of the eNB
   * \param teid the Tunnel Enpoint IDentifier
   */
  void SendToS1uSocket (Ptr<Packet> packet, Ipv4Address enbS1uAddress, uint32_t teid);
  

  /** 
   * Set the MME side of the S11 SAP 
   * 
   * \param s the MME side of the S11 SAP 
   */
  void SetS11SapMme (EpcS11SapMme * s);

  /** 
   * 
   * \return the SGW side of the S11 SAP 
   */
  EpcS11SapSgw* GetS11SapSgw ();


  /** 
   * Let the SGW be aware of a new eNB 
   * 
   * \param cellId the cell identifier
   * \param enbAddr the address of the eNB
   * \param sgwAddr the address of the SGW
   */
  void AddEnb (uint16_t cellId, Ipv4Address enbAddr, Ipv4Address sgwAddr);

  /** 
   * Let the SGW be aware of a new UE
   * 
   * \param imsi the unique identifier of the UE
   */
  void AddUe (uint64_t imsi);

  /** 
   * set the address of a previously added UE
   * 
   * \param imsi the unique identifier of the UE
   * \param ueAddr the IPv4 address of the UE
   */
  void SetUeAddress (uint64_t imsi, Ipv4Address ueAddr);

  /** 
   * set the address of a previously added UE
   * 
   * \param imsi the unique identifier of the UE
   * \param ueAddr the IPv6 address of the UE
   */
  void SetUeAddress6 (uint64_t imsi, Ipv6Address ueAddr);

  /**
   * TracedCallback signature for data Packet reception event.
   *
   * \param [in] packet The data packet sent from the internet.
   */
  typedef void (* RxTracedCallback)
    (Ptr<Packet> packet);

private:

  // S11 SAP SGW methods
  /**
   * Create session request function
   * \param msg EpcS11SapSgw::CreateSessionRequestMessage
   */
  void DoCreateSessionRequest (EpcS11SapSgw::CreateSessionRequestMessage msg);
  /**
   * Modify bearer request function
   * \param msg EpcS11SapSgw::ModifyBearerRequestMessage
   */
  void DoModifyBearerRequest (EpcS11SapSgw::ModifyBearerRequestMessage msg);  

  /**
   * Delete bearer command function
   * \param req EpcS11SapSgw::DeleteBearerCommandMessage
   */
  void DoDeleteBearerCommand (EpcS11SapSgw::DeleteBearerCommandMessage req);
  /**
   * Delete bearer response function
   * \param req EpcS11SapSgw::DeleteBearerResponseMessage
   */
  void DoDeleteBearerResponse (EpcS11SapSgw::DeleteBearerResponseMessage req);


  /**
   * store info for each UE connected to this SGW
   */
  class UeInfo : public SimpleRefCount<UeInfo>
  {
public:
    UeInfo ();  

    /** 
     * 
     * \param tft the Traffic Flow Template of the new bearer to be added
     * \param epsBearerId the ID of the EPS Bearer to be activated
     * \param teid  the TEID of the new bearer
     */
    void AddBearer (Ptr<EpcTft> tft, uint8_t epsBearerId, uint32_t teid);

    /** 
     * \brief Function, deletes contexts of bearer on SGW and PGW side
     * \param bearerId the Bearer Id whose contexts to be removed
     */
    void RemoveBearer (uint8_t bearerId);

    /**
     * 
     * 
     * \param p the IP packet from the internet to be classified
     * \param protocolNumber the protocol number of the IP packet
     * 
     * \return the corresponding bearer ID > 0 identifying the bearer
     * among all the bearers of this UE;  returns 0 if no bearers
     * matches with the previously declared TFTs
     */
    uint32_t Classify (Ptr<Packet> p, uint16_t protocolNumber);

    /** 
     * \return the address of the eNB to which the UE is connected
     */
    Ipv4Address GetEnbAddr ();

    /** 
     * set the address of the eNB to which the UE is connected
     * 
     * \param addr the address of the eNB
     */
    void SetEnbAddr (Ipv4Address addr);

    /** 
     * \return the IPv4 address of the UE
     */
    Ipv4Address GetUeAddr ();

    /** 
     * set the IPv4 address of the UE
     * 
     * \param addr the IPv4 address of the UE
     */
    void SetUeAddr (Ipv4Address addr);

    /**
     * \return the IPv6 address of the UE
     */
    Ipv6Address GetUeAddr6 ();

    /**
     * set the IPv6 address of the UE
     *
     * \param addr the IPv6 address of the UE
     */
    void SetUeAddr6 (Ipv6Address addr);

  private:
    EpcTftClassifier m_tftClassifier; ///< TFT classifier
    Ipv4Address m_enbAddr; ///< ENB IPv4 address
    Ipv4Address m_ueAddr; ///< UE IPv4 address
    Ipv6Address m_ueAddr6; ///< UE IPv6 address
    std::map<uint8_t, uint32_t> m_teidByBearerIdMap; ///< TEID By bearer ID Map
  };


 /**
  * UDP socket to send and receive GTP-U packets to and from the S1-U interface
  */
  Ptr<Socket> m_s1uSocket;
  
  /**
   * TUN VirtualNetDevice used for tunneling/detunneling IP packets
   * from/to the internet over GTP-U/UDP/IP on the S1 interface 
   */
  Ptr<VirtualNetDevice> m_tunDevice;

  /**
   * Map telling for each UE IPv4 address the corresponding UE info 
   */
  std::map<Ipv4Address, Ptr<UeInfo> > m_ueInfoByAddrMap;

  /**
   * Map telling for each UE IPv6 address the corresponding UE info 
   */
  std::map<Ipv6Address, Ptr<UeInfo> > m_ueInfoByAddrMap6;

  /**
   * Map telling for each IMSI the corresponding UE info 
   */
  std::map<uint64_t, Ptr<UeInfo> > m_ueInfoByImsiMap;

  /**
   * UDP port to be used for GTP
   */
  uint16_t m_gtpuUdpPort;

  /**
   * TEID count
   */
  uint32_t m_teidCount;

  /**
   * MME side of the S11 SAP
   * 
   */
  EpcS11SapMme* m_s11SapMme;

  /**
   * SGW side of the S11 SAP
   * 
   */
  EpcS11SapSgw* m_s11SapSgw;

  /// EnbInfo structure
  struct EnbInfo
  {
    Ipv4Address enbAddr; ///< eNB IPv4 address
    Ipv4Address sgwAddr; ///< SGW IPV4 address
  };

  std::map<uint16_t, EnbInfo> m_enbInfoByCellId; ///< eNB info by cell ID

  /**
   * \brief Callback to trace RX (reception) data packets at Tun Net Device from internet.
   */
  TracedCallback<Ptr<Packet> > m_rxTunPktTrace;

  /**
   * \brief Callback to trace RX (reception) data packets from S1-U socket.
   */
  TracedCallback<Ptr<Packet> > m_rxS1uPktTrace;
};

} //namespace ns3

#endif /* EPC_SGW_PGW_APPLICATION_H */

