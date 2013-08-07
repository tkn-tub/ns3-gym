/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of Jyvaskyla
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
 * Author: Budiarto Herman <buherman@student.jyu.fi>
 *
 */

#include "handover-algorithm.h"
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("HandoverAlgorithm");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HandoverAlgorithm);


HandoverAlgorithm::HandoverAlgorithm ()
{
}


HandoverAlgorithm::~HandoverAlgorithm ()
{
}


void
HandoverAlgorithm::DoDispose ()
{
}


TypeId
HandoverAlgorithm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HandoverAlgorithm")
    .SetParent<Object> ()
  ;
  return tid;
}


} // end of namespace ns3
