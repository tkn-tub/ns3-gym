/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef FLAME_HEADER_H
#define FLAME_HEADER_H

#include "ns3/header.h"
#include "ns3/mac48-address.h"

namespace ns3 {
namespace flame {
/**
 * \ingroup flame
 *
 * \brief Flame header
 *
 * Header format: | Reserved: 1 | cost: 1 | Sequence number: 2 | OrigDst: 6 | OrigSrc: 6 | Flame port : 2 |
 */

class FlameHeader : public Header
{
public:

  FlameHeader ();
  ~FlameHeader ();
  /**
   * \name Inherited from Header class:
   * \{
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  ///\}
  /**
   * \name Seeters/Getters for fields:
   * \{
   */
  void AddCost (uint8_t cost);
  uint8_t GetCost () const;
  void SetSeqno (uint16_t seqno);
  uint16_t GetSeqno () const;
  void SetOrigDst (Mac48Address dst);
  Mac48Address GetOrigDst () const;
  void SetOrigSrc (Mac48Address OrigSrc);
  Mac48Address GetOrigSrc () const;
  void SetProtocol (uint16_t protocol);
  uint16_t GetProtocol () const;
  ///\}
private:
  uint8_t m_cost;
  uint16_t m_seqno;
  Mac48Address m_origDst;
  Mac48Address m_origSrc;
  uint16_t m_protocol;
  friend bool operator== (const FlameHeader & a, const FlameHeader & b);
};
bool operator== (const FlameHeader & a, const FlameHeader & b);
} //namespace flame
} //namespace ns3
#endif /* FLAME_HEADER_H */
