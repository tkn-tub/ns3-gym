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
 * Based on
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 *
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */

#ifndef IDCACHE_H_
#define IDCACHE_H_

#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include <vector>

namespace ns3
{
namespace aodv
{

/**
 * \ingroup aodv
 * \brief packets identification cache
 */
class IdCache
{
public:
  void InsertId (Ipv4Address addr, uint32_t id, Time saveTime);
  bool LookupId (Ipv4Address addr, uint32_t id);
  void Purge ();
private:
  struct UniqueId
  {
    Ipv4Address m_context;
    uint32_t m_id;
    Time m_expire;
  };
  struct IsExpired
  {
    bool
    operator() (const struct UniqueId & u ) const
    {
      return (u.m_expire < Simulator::Now ());
    }
  };
  std::vector<UniqueId> m_idCache;
};

}
}
#endif /* IDCACHE_H_ */
