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
#ifndef COMPOSITE_TRACE_RESOLVER_H
#define COMPOSITE_TRACE_RESOLVER_H

#include "ns3/callback.h"
#include "trace-resolver.h"
#include "callback-trace-source.h"
#include "uv-trace-source.h"
#include "sv-trace-source.h"
#include "fv-trace-source.h"
#include "terminal-trace-resolver.h"

namespace ns3 {

/**
 * \brief a helper class to aggregate contained TraceResolver and other trace sources.
 * \ingroup lowleveltracing
 */
class CompositeTraceResolver : public TraceResolver
{
public:
  CompositeTraceResolver ();
  virtual ~CompositeTraceResolver ();
  /**
   * \param name name of trace source
   * \param trace a callback trace source
   * \param context the context associated to this trace source
   *
   * Add a callback trace source in this resolver. This trace
   * source will match the name specified during namespace 
   * resolution. The TraceContext of this trace source will also
   * be automatically extended to contain the input context.
   */
  template <typename T1, typename T2,
            typename T3, typename T4,
            typename T>
  void Add (std::string name,
            CallbackTraceSource<T1,T2,T3,T4> &trace, T const &context);
  /**
   * \param name name of trace source
   * \param trace a signed variable trace source
   * \param context the context associated to this trace source
   *
   * Add a signed variable trace source in this resolver. 
   * This trace source will match the name specified during namespace 
   * resolution. The TraceContext of this trace source will also
   * be automatically extended to contain the input context.
   */
  template <typename T>
  void Add (std::string name,
            SVTraceSource<T> &trace, T const &context);
  /**
   * \param name name of trace source
   * \param trace an unsigned variable trace source
   * \param context the context associated to this trace source
   *
   * Add an unsigned variable trace source in this resolver. 
   * This trace source will match the name specified during namespace 
   * resolution. The TraceContext of this trace source will also
   * be automatically extended to contain the input context.
   */
  template <typename T>
  void Add (std::string name,
            UVTraceSource<T> &trace, T const &context);
  /**
   * \param name name of trace source
   * \param trace a floating-point variable trace source
   * \param context the context associated to this trace source
   *
   * Add a floating-point variable trace source in this resolver. 
   * This trace source will match the name specified during namespace 
   * resolution. The TraceContext of this trace source will also
   * be automatically extended to contain the input context.
   */
  template <typename T>
  void Add (std::string name,
            FVTraceSource<T> &trace, T const &context);

  /**
   * \param name name of child trace resolver
   * \param createResolver a trace resolver constructor
   * \param context the context associated to this entry
   *
   * Add a child trace resolver to this resolver. This child
   * trace resolver will match the name specified during
   * namespace resolution. When this happens, the constructor
   * will be invoked to create the child trace resolver and
   * the associated TraceContext will be automatically extended
   * to contain the input context.
   */
  template <typename T>
  void Add (std::string name, 
            Callback<TraceResolver *> createResolver,
            T const &context);

  /**
   * \param name name of child trace resolver
   * \param createResolver a trace resolver constructor
   *
   * Add a child trace resolver to this resolver. This child
   * trace resolver will match the name specified during
   * namespace resolution. When this happens, the constructor
   * will be invoked to create the child trace resolver.
   */
  void Add (std::string name, 
            Callback<TraceResolver *> createResolver);
private:
  template <typename SOURCE, typename CONTEXT>
  void DoAddTraceSource (std::string name,
                         SOURCE &traceSource, CONTEXT const &context);
  template <typename SOURCE>
  static TraceResolver *CreateTerminalTraceResolver (SOURCE *trace);
  void DoAdd (std::string name, 
              Callback<TraceResolver *> createResolver,
              TraceContext const &context);
  virtual TraceResolverList DoLookup (std::string id) const;

  struct CallbackTraceSourceItem
  {
    std::string name;
    Callback<TraceResolver *> createResolver;
    TraceContext context;
  };

  typedef std::list<struct CallbackTraceSourceItem> TraceItems;
  TraceItems m_items;
};

}//namespace ns3

namespace ns3 {

template <typename SOURCE, typename CONTEXT>
void 
CompositeTraceResolver::DoAddTraceSource (std::string name,
                                          SOURCE &traceSource, CONTEXT const &context)
{
  TraceResolver *(*create) (SOURCE *trace);
  create = &CompositeTraceResolver::CreateTerminalTraceResolver<SOURCE>;
  Callback<TraceResolver *> createResolver = 
    MakeBoundCallback (create, &traceSource);

  TraceContext ctx;
  ctx.Add (context);
  DoAdd (name, createResolver, ctx);
}

template <typename SOURCE>
TraceResolver *
CompositeTraceResolver::CreateTerminalTraceResolver (SOURCE *traceSource)
{
  return new TerminalTraceResolver<SOURCE> (*traceSource);
}




template <typename T1, typename T2,
          typename T3, typename T4,
          typename T>
void 
CompositeTraceResolver::Add (std::string name,
                             CallbackTraceSource<T1,T2,T3,T4> &trace, 
                             T const &context)
{
  DoAddTraceSource (name, trace, context);
}
template <typename T>
void 
CompositeTraceResolver::Add (std::string name,
                             SVTraceSource<T> &trace, T const &context)
{
  DoAddTraceSource (name, trace, context);
}
template <typename T>
void 
CompositeTraceResolver::Add (std::string name,
                             UVTraceSource<T> &trace, T const &context)
{
  DoAddTraceSource (name, trace, context);
}
template <typename T>
void 
CompositeTraceResolver::Add (std::string name,
                             FVTraceSource<T> &trace, T const &context)
{
  DoAddTraceSource (name, trace, context);
}
template <typename T>
void 
CompositeTraceResolver::Add (std::string name, 
                             Callback<TraceResolver *> createResolver,
                             T const &context)
{
  TraceContext ctx;
  ctx.Add (context);
  DoAdd (name, createResolver, ctx);
}

}//namespace ns3

#endif /* COMPOSITE_TRACE_RESOLVER_H */
