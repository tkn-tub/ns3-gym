/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef EPC_HELPER_H
#define EPC_HELPER_H

#include <ns3/object.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/data-rate.h>
#include <ns3/epc-tft.h>
#include <ns3/eps-bearer.h>

namespace ns3 {

class Node;
class NetDevice;
class VirtualNetDevice;
class EpcSgwPgwApplication;
class EpcX2;
class EpcMme;

/**
 * \brief Helper class to handle the creation of the EPC entities and protocols.
 *
 * This Helper will create an EPC network topology comprising of a
 * single node that implements both the SGW and PGW functionality, and
 * is connected to all the eNBs in the simulation by means of the S1-U
 * interface. 
 */
class EpcHelper : public Object
{
public:
  
  /** 
   * Constructor
   */
  EpcHelper ();

  /** 
   * Destructor
   */  
  virtual ~EpcHelper ();
  
  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  
  /** 
   * Add an eNB to the EPC
   * 
   * \param enbNode the previosuly created eNB node which is to be
   * added to the EPC
   * \param lteEnbNetDevice the LteEnbNetDevice of the eNB node
   * \param cellId ID of the eNB
   */
  void AddEnb (Ptr<Node> enbNode, Ptr<NetDevice> lteEnbNetDevice, uint16_t cellId);

  /** 
   * Notify the EPC of the existance of a new UE which might attach at a later time
   * 
   * \param ueLteDevice the UE device to be attached
   * \param imsi the unique identifier of the UE
   */
  void AddUe (Ptr<NetDevice> ueLteDevice, uint64_t imsi);

  /** 
   * Add an X2 interface between two eNB
   * 
   * \param enbNode1 one eNB peer of the X2 interface
   * \param enbNode2 the other eNB peer of the X2 interface
   */
  void AddX2Interface (Ptr<Node> enbNode1, Ptr<Node> enbNode2);

  /** 
   * Activate an EPS bearer, setting up the corresponding S1-U tunnel.
   * 
   * 
   * 
   * \param ueLteDevice the Ipv4-enabled device of the UE, normally
   * connected via the LTE radio interface
   * \param imsi the unique identifier of the UE
   * \param tft the Traffic Flow Template of the new bearer
   * \param bearer struct describing the characteristics of the EPS bearer to be activated
   */
  void ActivateEpsBearer (Ptr<NetDevice> ueLteDevice, uint64_t imsi, Ptr<EpcTft> tft, EpsBearer bearer);


  /** 
   * 
   * \return a pointer to the node implementing PGW
   * functionality. Note that in this particular implementation this
   * node will also hold the SGW functionality. The primary use
   * intended for this method is to allow the user to configure the Gi
   * interface of the PGW, i.e., to connect the PGW to the internet.
   */
  Ptr<Node> GetPgwNode ();

  /** 
   * Assign IPv4 addresses to UE devices
   * 
   * \param ueDevices the set of UE devices
   * 
   * \return the interface container, \see Ipv4AddressHelper::Assign() which has similar semantics
   */
  Ipv4InterfaceContainer AssignUeIpv4Address (NetDeviceContainer ueDevices);


  /** 
   * 
   * \return the address of the Default Gateway to be used by UEs to reach the internet
   */
  Ipv4Address GetUeDefaultGatewayAddress ();



private:

  /**
   * SGW-PGW network element
   */

  /** 
   * helper to assign addresses to UE devices as well as to the TUN device of the SGW/PGW
   */
  Ipv4AddressHelper m_ueAddressHelper; 
  
  Ptr<Node> m_sgwPgw; 
  Ptr<EpcSgwPgwApplication> m_sgwPgwApp;
  Ptr<VirtualNetDevice> m_tunDevice;
  Ptr<EpcMme> m_mme;

  /**
   * S1-U interfaces
   */

  /** 
   * helper to assign addresses to S1-U NetDevices 
   */
  Ipv4AddressHelper m_s1uIpv4AddressHelper; 

  DataRate m_s1uLinkDataRate;
  Time     m_s1uLinkDelay;
  uint16_t m_s1uLinkMtu;

  /**
   * UDP port where the GTP-U Socket is bound, fixed by the standard as 2152
   */
  uint16_t m_gtpuUdpPort;

  /**
   * Map storing for each IMSI the corresponding eNB NetDevice
   * 
   */
  std::map<uint64_t, Ptr<NetDevice> > m_imsiEnbDeviceMap;
  
  /** 
   * helper to assign addresses to X2 NetDevices 
   */
  Ipv4AddressHelper m_x2Ipv4AddressHelper;   

  DataRate m_x2LinkDataRate;
  Time     m_x2LinkDelay;
  uint16_t m_x2LinkMtu;

};




} // namespace ns3

#endif // EPC_HELPER_H
