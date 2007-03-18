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
#ifndef EMPTY_TRACE_RESOLVER_H
#define EMPTY_TRACE_RESOLVER_H

#include "trace-resolver.h"

namespace ns3 {

class TraceContext;

/**
 * \brief a TraceResolver instance which does not resolve anything.
 * \ingroup tracing
 *
 * Trying to resolve against this class will yield no matches and no
 * connections. Returning an instance of this class from a 
 * CreateTraceResolver method is a hand way of not implementing
 * any Tracing code.
 */
class EmptyTraceResolver : public TraceResolver
{
public:
  /**
   * \param o necessary context for this class.
   *
   * The only constructor exported by this class.
   */
  EmptyTraceResolver (TraceContext const &o);
};

}//namespace ns3

#endif /* EMPTY_TRACE_RESOLVER_H */
