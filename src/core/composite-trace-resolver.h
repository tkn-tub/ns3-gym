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

#include <vector>
#include "callback.h"
#include "ptr.h"
#include "trace-resolver.h"
#include "callback-trace-source.h"
#include "uv-trace-source.h"
#include "sv-trace-source.h"
#include "fv-trace-source.h"
#include "array-trace-resolver.h"

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
  template <typename T>
  void AddSource (std::string name,
                  TraceSource &trace, T const &context);
  /**
   * \param name name of trace source
   * \param trace a callback trace source
   *
   * Add a callback trace source in this resolver. This trace
   * source will match the name specified during namespace 
   * resolution.
   */
  void AddSource (std::string name,
                  TraceSource &trace);
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
            Callback<Ptr<TraceResolver> > createResolver);

  void AddChild (std::string name, Ptr<Object> child);

  template <typename T>
  void AddChild (std::string name, Ptr<Object> child, const T &contextElement);

  template <typename ITERATOR, typename INDEX>
  void AddArray (std::string name, 
                 ITERATOR begin, ITERATOR end, INDEX index);


  void SetParent (Ptr<TraceResolver> parent);

  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection);

private:
  class CompositeItem 
  {
  public:
    virtual ~CompositeItem () {}
    virtual void Connect (std::string subpath, const CallbackBase &cb, const TraceContext &context) = 0;
    virtual void Disconnect (std::string subpath, const CallbackBase &cb) = 0;
    virtual void CollectSources (std::string path, const TraceContext &context, 
                                 SourceCollection *collection) = 0;

    std::string name;
    TraceContext context;
  };
  typedef std::vector<CompositeItem *> TraceItems;
  class Operation
  {
  public:
    virtual ~Operation () {}
    virtual void Do (std::string subpath, CompositeItem *item) const = 0;
    virtual void DoParent (std::string path, Ptr<TraceResolver> parent) const = 0;
  };

  void AddItem (CompositeItem *item);
  void DoRecursiveOperation (std::string path, 
                             const Operation &operation);
  void DoRecursiveOperationForParent (std::string path, 
                                      const Operation &operation);
  void DoAddChild (std::string name, Ptr<Object> child, const TraceContext &context);
  void DoAddSource (std::string name,
                    TraceSource &trace,
                    const TraceContext &context);

  CompositeTraceResolver::TraceItems m_items;
  Ptr<TraceResolver> m_parent;
};

}//namespace ns3

namespace ns3 {



template <typename T>
void 
CompositeTraceResolver::AddSource (std::string name,
                                   TraceSource &trace, 
                                   T const &context)
{
  TraceContext ctx;
  ctx.AddElement (context);
  DoAddSource (name, trace, ctx);  
}

template <typename ITERATOR, typename INDEX>
void 
CompositeTraceResolver::AddArray (std::string name, 
                                  ITERATOR begin, ITERATOR end, INDEX index)
{
  class ArrayCompositeItem : public CompositeItem
  {
  public:
    virtual void Connect (std::string subpath, const CallbackBase &cb, const TraceContext &context)
    {array->Connect (subpath, cb, context);}
    virtual void Disconnect (std::string subpath, const CallbackBase &cb)
    {array->Disconnect (subpath, cb);}
    virtual void CollectSources (std::string path, const TraceContext &context, 
                                 SourceCollection *collection)
    {
      path.append ("/");
      path.append (this->name);
      TraceContext ctx = context;
      ctx.Union (this->context);
      array->CollectSources (path, ctx, collection);
    }

    Ptr<ArrayTraceResolver<INDEX> > array;
  } *item = new ArrayCompositeItem ();
  item->name = name;
  item->context = TraceContext ();
  item->array = Create<ArrayTraceResolver<INDEX> > ();
  item->array->SetIterators (begin, end);
  AddItem (item);
}

template <typename T>
void 
CompositeTraceResolver::AddChild (std::string name, Ptr<Object> child, const T &contextElement)
{
  TraceContext context;
  context.AddElement (contextElement);
  DoAddChild (name, child, context);
}



}//namespace ns3

#endif /* COMPOSITE_TRACE_RESOLVER_H */
