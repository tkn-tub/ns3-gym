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

#ifndef EPS_GTPU_L5_PROTOCOL_H
#define EPS_GTPU_L5_PROTOCOL_H

#include <stdint.h>
#include <ns3/object.h>
#include <ns3/packet.h>

namespace ns3 {

/**
 * \ingroup lte
 *
 * Implementation of the GTP-U v1 protocol. Basically, given a TEID, it adds and
 * removes the GTPU header of the packets going in and out of the tunnel.
 */
class GtpuL5Protocol : public Object
{
public:
  static TypeId GetTypeId (void);

  GtpuL5Protocol ();
  GtpuL5Protocol (uint32_t teid);
  virtual  ~GtpuL5Protocol ();

  /**
   * \brief Adds the GTPv1-U header to a packet
   * \param p packet to add the header
   * \return  packet with the header added
   */
  Ptr<Packet> AddHeader (Ptr<Packet> p);
  /**
   * \brief Strips the GTPv1-U header of a packet and
   * checks its coherence
   * \param p packet to strip the header
   */
  void  RemoveHeader (Ptr<Packet> p);

  /**
   * \brief Returns the Tunnel Endpoint IDentifier of the the GTPv1-U instance
   * \return the TEID
   */
  uint32_t GetTeid (void);
  /**
   * \brief Sets the Tunnel Endpoint IDentifier of the the GTPv1-U instance
   * \param teid the TEID
   */
  void  SetTeid (uint32_t teid);

private:
  /**
   * Tunnel Endpoint IDentifier
   */
  uint32_t m_teid;

};

}
; // namespace ns3

#endif /* EPS_GTPU_L5_PROTOCOL_H */
