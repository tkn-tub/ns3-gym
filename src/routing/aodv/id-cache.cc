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
#include "id-cache.h"

namespace ns3
{
namespace aodv
{
void
IdCache::InsertId (Ipv4Address addr, uint32_t id, Time saveTime)
{
  if (LookupId (addr, id))
    return;
  struct UniqueId uniqueId = { addr, id, saveTime + Simulator::Now () };
  m_idCache.push_back (uniqueId);
}
bool
IdCache::LookupId (Ipv4Address addr, uint32_t id )
{
  Purge ();
  for (std::vector<UniqueId>::const_iterator i = m_idCache.begin (); i != m_idCache.end (); ++i)
    if (i->m_context == addr && i->m_id == id)
      return true;
  return false;
}
void
IdCache::Purge ()
{
  std::vector<UniqueId>::iterator i = remove_if (m_idCache.begin (), m_idCache.end (), IsExpired ());
  m_idCache.erase (i, m_idCache.end ());
}

}
}
