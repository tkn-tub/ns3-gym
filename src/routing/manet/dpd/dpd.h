/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */

#ifndef DUPLICATEPACKETDETECTION_H_
#define DUPLICATEPACKETDETECTION_H_

#include "id-cache.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"

namespace ns3
{
namespace dpd
{
/**
 * \ingroup dpd
 * 
 * \brief Helper class used to remember already seen packets and detect duplicates.
 */
class DuplicatePacketDetection
{
public:
  /// C-tor
  DuplicatePacketDetection (Time lifetime) : m_idCache(lifetime) {}
  /// Check that the packet is duplicated. If not, save information about this packet.
  bool IsDuplicate (Ptr<const Packet> p, const Ipv4Header & header);
  /// Set duplicate records lifetimes
  void SetLifetime (Time lifetime);
  /// Get duplicate records lifetimes
  Time GetLifetime () const;
private:
  /// Impl
  IdCache m_idCache;
};

}
}

#endif
