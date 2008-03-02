/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "mobility-model-notifier.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

TypeId 
MobilityModelNotifier::GetTypeId (void)
{
  static TypeId tid = TypeId ("MobilityModelNotifier")
    .SetParent<Object> ()
    .AddConstructor<MobilityModelNotifier> ()
    .AddTraceSource ("CourseChange", 
                     "The value of the position and/or velocity vector changed",
                     MakeTraceSourceAccessor (&MobilityModelNotifier::m_trace))
    ;
  return tid;
}

MobilityModelNotifier::MobilityModelNotifier ()
{}

void 
MobilityModelNotifier::Notify (Ptr<const MobilityModel> position) const
{
  m_trace (position);
}

} // namespace ns3
