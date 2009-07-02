/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997, 1998 Carnegie Mellon University.
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
 * Authors: The AODV code developed by the CMU/MONARCH group was optimized and 
 * tuned by Samir Das and Mahesh Marina, University of Cincinnati. 
 * The work was partially done in Sun Microsystems.
 * 
 * Ported to ns-3 by Elena Borovkova <borovkovaes@iitp.ru>
 */

#include "aodv-rtable.h"
#include <algorithm>

namespace ns3 {
namespace aodv {

/*
  The Routing Table
 */

aodv_rt_entry::aodv_rt_entry()
{
  rt_req_timeout = 0.0;
  rt_req_cnt = 0;

  rt_dst = 0;
  rt_seqno = 0;
  rt_hops = rt_last_hop_count = INFINITY2;
  rt_nexthop = 0;
  rt_expire = 0.0;
  rt_flags = RTF_DOWN;

  /*
 rt_errors = 0;
 rt_error_time = 0.0;
   */
  
  for (int i=0; i < MAX_HISTORY; i++) 
    {
      rt_disc_latency[i] = 0.0;
    }
  hist_indx = 0;
  rt_req_last_ttl = 0;
}


aodv_rt_entry::~aodv_rt_entry()
{
}


void
aodv_rt_entry::nb_insert(Ipv4Address id)
{
  rt_nblist.push_back(AODV_Neighbor(id));
}

bool
aodv_rt_entry::nb_lookup(Ipv4Address id, AODV_Neighbor & n)
{
  for(std::vector<AODV_Neighbor>::const_iterator i = rt_nblist.begin(); i != rt_nblist.end(); ++i)
    if (i->nb_addr == id)
      {
        n = *i;
        return true;
      }
  return false;
}

void
aodv_rt_entry::pc_insert(Ipv4Address id)
{
  AODV_Precursor p (id);
 
  if (! pc_lookup(id, p)) rt_pclist.push_back(p);
}

bool
aodv_rt_entry::pc_lookup(Ipv4Address id, AODV_Precursor & p)
{
  for(std::vector<AODV_Precursor>::const_iterator i = rt_pclist.begin(); i != rt_pclist.end(); ++i)
    if (i->pc_addr == id)
      {
        p = *i;
        return true;
      }
  return false;
}

void
aodv_rt_entry::pc_delete(Ipv4Address id) 
{
  AODV_Precursor p(id);
  rt_pclist.erase (std::remove(rt_pclist.begin(), rt_pclist.end(), p), rt_pclist.end());
}

void
aodv_rt_entry::pc_delete() 
{
  rt_pclist.clear();
}	

bool
aodv_rt_entry::pc_empty() const
{
  return rt_pclist.empty();
}

/*
  The Routing Table
 */

bool
aodv_rtable::rt_lookup(Ipv4Address id, aodv_rt_entry & rt) const
{
  std::vector<aodv_rt_entry>::const_iterator i = std::find (rthead.begin(), rthead.end(), id);  
  if (i == rthead.end()) return false;
  rt = *i;
  return true;
}

void
aodv_rtable::rt_delete(Ipv4Address id)
{
  rthead.erase (std::remove(rthead.begin(), rthead.end(), id), rthead.end());
}

void
aodv_rtable::rt_add(aodv_rt_entry const & rt)
{
  aodv_rt_entry dummy;
  NS_ASSERT(! rt_lookup(rt.rt_dst, dummy));
  rthead.push_back(rt);
}

}}
