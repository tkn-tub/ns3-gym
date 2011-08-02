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
 */

#ifndef GTPU_TUNNEL_ENDPOINT_H
#define GTPU_TUNNEL_ENDPOINT_H

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
class GtpuTunnelEndpoint : public Object
{

public:
  static TypeId GetTypeId (void);

  /**
   * Method assigned to the send callback of the upper end of the tunnel. It adds
   * the GTP header and sends it through the tunnel
   */
  bool GtpuSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  /**
   * Method assigned to the receive callback of the upper end of the tunnel. It strips
   * the GTP header and sends it up to the application
   */
  void GtpuRecv (Ptr<Socket> socket);

  /**
   * Constructor that binds the tap device to the callback methods.
   * \param tap VirtualNetDevice used to tunnel the packets
   * \param s Socket used to send the tunneled packets
   */
  GtpuTunnelEndpoint (const Ptr<VirtualNetDevice> tap, const Ptr<Socket> s);
  virtual ~GtpuTunnelEndpoint (void);

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
   * \return teid Tunnel Endpoint IDentifier assigned to the tunnel
   */
  uint32_t CreateGtpuTunnel (Ipv4Address destination);

private:
  /**
   * UDP socket to send and receive the packets to and from the tunnel
   */
  Ptr<Socket> m_socket;
  /**
   * UDP port where the socket is bind, fixed by the standard as 2152
   */
  uint16_t m_udpPort;
  /**
   * VirtualNetDevice to create the tunnel
   */
  Ptr<VirtualNetDevice> m_tap;
  /**
   * Map holding the GTP instances of the active tunnels on this endpoint
   */
  std::map<uint32_t, Ptr<GtpuL5Protocol> > m_gtpuMap;
  /**
   * Map holding the destination address of the active tunnels on this endpoint
   */
  std::map<uint32_t, Ipv4Address> m_dstAddrMap;

  static uint16_t m_teidCounter;
  static uint32_t m_indexCounter;
};

} //namespace ns3

#endif /* GTPU_TUNNEL_H */

