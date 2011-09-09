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

#ifndef GTPU_TUNNEL_HELPER_H_
#define GTPU_TUNNEL_HELPER_H_

#include "ns3/object.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-address-helper.h"
#include <map>



namespace ns3 {

class Node;
class GtpuTunnelEndpoint;
/**
 * Helper class to handle the creation of the EPC entities and protocols
 */
class GtpuTunnerHelper : public Object
{
public:
  /**
   * Constructor
   */
  GtpuTunnerHelper ();

  /**
   * Destructor
   */
  virtual ~GtpuTunnerHelper ();

  /**
   * Inherited from ns3::Object
   */
  static TypeId GetTypeId (void);

  /**
   * Creates and configure the necessary instances for the node to act as a
   * GTP endpoint. The IP address of the new interfaces are within 100.0.0./24
   * \param n node to install GTPv1-U
   */
  void InstallGtpu (Ptr<Node> n);
  /**
   * Creates and configure the necessary instances for the node to act as a
   * GTP endpoint.
   */
  void InstallGtpu (Ptr<Node> n, Ipv4Address addr);

  /**
   * Creates a GTPv1-U tunnel between two nodes, both of them need to have GTPv1-U installed.
   * \param n First tunnel endpoint node
   * \param nAddr First tunnel endpoing address
   * \param m Second tunnel endpoint node
   * \param mAddr Second tunnel endpoing address
   */
  void CreateGtpuTunnel (Ptr<Node> n, Ipv4Address nAddr, Ptr<Node> m, Ipv4Address mAddr);

private:
  uint16_t m_udpPort;
  Ipv4AddressHelper m_ipv4;
  Ipv4Mask m_mask;
  std::map <Ptr<Node>, Ptr<GtpuTunnelEndpoint> > m_gtpuEndpoint;

};

} // namespace ns3

#endif /* GTPU_TUNNEL_HELPER_H_ */
