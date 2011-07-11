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

namespace ns3
{

/**
 * Implementation of the GTP-U v1 protocol
 */
class GtpuL5Protocol : public Object
{
public:
  static TypeId GetTypeId (void);

  GtpuL5Protocol ();
  GtpuL5Protocol (uint32_t teid);
  virtual  ~GtpuL5Protocol ();

  Ptr<Packet> AddHeader (Ptr<Packet> p);
  void  RemoveHeader (Ptr<Packet> p);

  uint32_t GetTeid (void);
  void  SetTeid (uint32_t teid);

private:

  uint32_t m_teid;

};

}
; // namespace ns3

#endif /* EPS_GTPU_L5_PROTOCOL_H */
