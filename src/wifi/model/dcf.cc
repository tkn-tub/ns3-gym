/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "dcf.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Dcf");

NS_OBJECT_ENSURE_REGISTERED (Dcf);

TypeId
Dcf::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Dcf")
    .SetParent<Object> ()
    .AddAttribute ("MinCw", "The minimum value of the contention window.",
                   UintegerValue (15),
                   MakeUintegerAccessor (&Dcf::SetMinCw,
                                         &Dcf::GetMinCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxCw", "The maximum value of the contention window.",
                   UintegerValue (1023),
                   MakeUintegerAccessor (&Dcf::SetMaxCw,
                                         &Dcf::GetMaxCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Aifsn", "The AIFSN: the default value conforms to simple DCA.",
                   UintegerValue (2),
                   MakeUintegerAccessor (&Dcf::SetAifsn,
                                         &Dcf::GetAifsn),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

} // namespace ns3
