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

#include "aodv-neighbor.h"

namespace ns3
{
namespace aodv
{

bool
Neighbors::Lookup (Ipv4Address addr, Neighbor & n )
{
  Purge ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      {
        n = *i;
        return true;
      }
  return false;
}
bool
Neighbors::IsNeighbor (Ipv4Address addr )
{
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      return true;
  return false;
}

void
Neighbors::Update (Ipv4Address addr, Time expire )
{
  Purge ();
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      {
        i->m_expireTime = expire + Simulator::Now ();
        return;
      }
  struct Neighbor neighbor =
  { addr, expire + Simulator::Now () };
  m_nb.push_back (neighbor);
}
void
Neighbors::Purge ()
{
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_expireTime < Simulator::Now ())
      {
        m_handleLinleFailure (i->m_neighborAddress);
      }
  std::vector<Neighbor>::iterator i = remove_if (m_nb.begin (), m_nb.end (), IsExpired ());
  m_nb.erase (i, m_nb.end ());

}


}
}

