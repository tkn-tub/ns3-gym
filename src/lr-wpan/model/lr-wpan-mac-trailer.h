/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Author:
 *  kwong yin <kwong-sang.yin@boeing.com>
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 *  Erwan Livolant <erwan.livolant@inria.fr>
 */

#ifndef LR_WPAN_MAC_TRAILER_H
#define LR_WPAN_MAC_TRAILER_H

#include <ns3/trailer.h>

namespace ns3 {

class Packet;

/**
 * The length in octets of the IEEE 802.15.4 MAC FCS field
 */

static const uint16_t LRWPAN_MAC_FCS_LENGTH = 2;

class LrWpanMacTrailer : public Trailer
{
public:
  LrWpanMacTrailer ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  uint16_t GetFcs (void) const;
  void SetFcs (Ptr<const Packet> p);
  bool CheckFcs (Ptr<const Packet> p);
  void EnableFcs (bool enable);

private:
  uint16_t m_fcs;
  bool m_calcFcs;
  uint16_t GenerateCrc16 (uint8_t *data, int length);
};
} // namespace ns3
#endif /* LR_WPAN_MAC_TRAILER_H */
