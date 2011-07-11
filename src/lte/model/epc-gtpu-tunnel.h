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

#ifndef GTPU_TUNNEL_H
#define GTPU_TUNNEL_H

#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/virtual-net-device.h"
#include "ns3/epc-gtpu-l5-protocol.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3
{

class GtpuTunnel : public Object
{

public:
  static TypeId GetTypeId (void);
  bool GtpuNaSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  bool GtpuNbSend (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  void GtpuNaRecv (Ptr<Socket> socket);
  void GtpuNbRecv (Ptr<Socket> socket);

  GtpuTunnel (Ptr<Node> nA, Ptr<Node> nB, Ipv4Address addrA, Ipv4Address addrB, Ipv4Address tAddrA, Ipv4Address tAddrB, Ipv4Mask m, uint32_t teid);
  virtual ~GtpuTunnel (void);

private:

  Ptr<Socket> m_socketA;
  Ptr<Socket> m_socketB;
  Ipv4Address m_addressA;
  Ipv4Address m_addressB;
  uint16_t m_udpPort;
  //static const uint16_t m_gtpuPort;
  Ptr<VirtualNetDevice> m_tapA;
  Ptr<VirtualNetDevice> m_tapB;
  Ptr<GtpuL5Protocol> m_gtpu;
};

} //namespace ns3

#endif /* GTPU_TUNNEL_H */

