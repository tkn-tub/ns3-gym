/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */


#ifndef HWMP_TAG_H
#define HWMP_TAG_H

#include "ns3/tag.h"
#include "ns3/object.h"
#include "ns3/mac48-address.h"
namespace ns3 {
namespace dot11s {
/**
 * \ingroup mesh
 *
 * \brief Hwmp tag implements interaction between HWMP
 * protocol and MeshWifiMac
 *
 * \details Hwmp tag keeps the following:
 * 1. When packet is passed from Hwmp to 11sMAC:
 *  - retransmitter address,
 *  - TTL value,
 * 2. When packet is passed to Hwmp from 11sMAC:
 *  - lasthop address,
 *  - TTL value,
 *  - metric value (metric of link is recalculated
 *  at each packet, but routing table stores metric
 *  obtained during path discovery procedure)
 */
class HwmpTag : public Tag
{
public:
  HwmpTag ();
  ~HwmpTag ();
  void  SetAddress (Mac48Address retransmitter);
  Mac48Address GetAddress ();
  void  SetTtl (uint8_t ttl);
  uint8_t GetTtl ();
  void  SetMetric (uint32_t metric);
  uint32_t GetMetric ();
  void  SetSeqno (uint32_t seqno);
  uint32_t GetSeqno ();
  void  DecrementTtl ();

  static  TypeId  GetTypeId ();
  virtual TypeId  GetInstanceTypeId () const;
  virtual uint32_t GetSerializedSize () const;
  virtual void  Serialize (TagBuffer i) const;
  virtual void  Deserialize (TagBuffer i);
  virtual void  Print (std::ostream &os) const;
private:
  Mac48Address m_address;
  uint8_t  m_ttl;
  uint32_t m_metric;
  uint32_t m_seqno;
};
} //namespace dot11s
} //namespace ns3
#endif
