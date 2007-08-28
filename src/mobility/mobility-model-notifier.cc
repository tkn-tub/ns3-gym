/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#include "ns3/composite-trace-resolver.h"
#include "ns3/trace-doc.h"

namespace ns3 {

const InterfaceId MobilityModelNotifier::iid = MakeInterfaceId ("MobilityModelNotifier", Object::iid);
const ClassId MobilityModelNotifier::cid = 
  MakeClassId<MobilityModelNotifier> ("MobilityModelNotifier", 
				    MobilityModelNotifier::iid);

MobilityModelNotifier::MobilityModelNotifier ()
{
  SetInterfaceId (MobilityModelNotifier::iid);
}

void 
MobilityModelNotifier::Notify (Ptr<const MobilityModel> position) const
{
  m_trace (position);
}

Ptr<TraceResolver> 
MobilityModelNotifier::GetTraceResolver (void)
{
  Ptr<CompositeTraceResolver> resolver = 
    Create<CompositeTraceResolver> ();
  resolver->AddSource ("course-change", 
                       TraceDoc ("The value of the speed vector changed",
                                 "Ptr<const MobilityModel>", 
                                 "the mobility model whose course changed"),
                       m_trace);
  resolver->SetParent (Object::GetTraceResolver ());
  return resolver;
}

} // namespace ns3
