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
 * Author: Jaume Nin <jnin@cttc.es>
 */

#ifndef EPC_HELPER_H
#define EPC_HELPER_H

#include <ns3/object.h>




namespace ns3 {

class Node;
class NetDevice;

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
  TypeId GetTypeId (void);

  
  /** 
   * Add an eNB to the EPC
   * 
   * \param enbNode the previosuly created eNB node which is to be
   * added to the EPC
   * \param lteEnbNetDevice the LteEnbNetDevice of the eNB node
   */
  void AddEnb (Ptr<Node> enbNode, Ptr<NetDevice> lteEnbNetDevice);


  /** 
   * Activate an EPS bearer, setting up the corresponding S1-U tunnel.
   * 
   */
  void ActivateEpsBearer ();


  /** 
   * 
   * \return a pointer to the node implementing PGW
   * functionality. Note that in this particular implementation this
   * node will also hold the SGW functionality. The primary use
   * intended for this method is to allow the user to configure the Gi
   * interface of the PGW, i.e., to connect the PGW to the internet.
   */
  Ptr<Node> GetPgwNode ();

private:
  

  Ipv4AddressHelper m_s1uIpv4AddressHelper; /**< helper to assign
					       addresses to S1-U
					       NetDevices */
  
  Ptr<Node> m_sgwPgw; /**< the SGW/PGW node */ 

  DataRate m_s1uLinkDataRate;
  Time     m_s1uLinkDelay;


  /**
   * UDP port where the GTP-U Socket is bound, fixed by the standard as 2152
   */
  uint16_t m_gtpuUdpPort;

};




} // namespace ns3

#endif // EPC_HELPER_H
