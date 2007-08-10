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
#ifndef TERMINAL_TRACE_RESOLVER_H
#define TERMINAL_TRACE_RESOLVER_H

#include "trace-resolver.h"
#include "ns3/assert.h"

namespace ns3 {

class TraceContext;

template <typename T>
class TerminalTraceResolver : public TraceResolver
{
 public:
  TerminalTraceResolver (T *traceSource);

  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
 private:
  T *m_traceSource;
};

}//namespace ns3

namespace ns3 {

template <typename T>
TerminalTraceResolver<T>::TerminalTraceResolver (T *traceSource)
  : m_traceSource (traceSource)
{}
template <typename T>
void 
TerminalTraceResolver<T>::Connect (std::string path, CallbackBase const &cb, const TraceContext &context)
{
  if (path == "")
    {
      m_traceSource->AddCallback (cb, context);
    }
}
template <typename T>
void 
TerminalTraceResolver<T>::Disconnect (std::string path, CallbackBase const &cb)
{
  if (path == "")
    {
      m_traceSource->RemoveCallback (cb);
    }
}

}//namespace ns3

#endif /* TERMINAL_TRACE_RESOLVER_H */
