/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "trace-root.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/trace-context.h"

namespace ns3 {

void 
TraceRoot::Connect (std::string path, CallbackBase const &cb)
{
  TraceResolver *resolver = GetComposite ();
  resolver->Connect (path, cb);
}
void 
TraceRoot::Disconnect (std::string path, CallbackBase const &cb)
{
  TraceResolver *resolver = GetComposite ();
  resolver->Disconnect (path, cb);
}
void 
TraceRoot::Register (std::string name, 
                     Callback<TraceResolver *,TraceContext const &> createResolver)
{
  CompositeTraceResolver *resolver = GetComposite ();
  resolver->Add (name, createResolver, TraceRoot::NOTHING);
}

CompositeTraceResolver *
TraceRoot::GetComposite (void)
{
  static CompositeTraceResolver resolver = CompositeTraceResolver (TraceContext ());
  return &resolver;
}

} // namespace ns3
