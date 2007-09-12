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
#include "trace-doc.h"

namespace ns3 {

/**
 * \brief a helper class to aggregate contained TraceResolver and other trace sources.
 * \ingroup tracing
 */
class CompositeTraceResolver : public TraceResolver
{
public:
  CompositeTraceResolver ();
  virtual ~CompositeTraceResolver ();
  /**
   * \param name name of trace source
   * \param doc the documentation associated to this trace source
   * \param trace a callback trace source
   * \param context the context associated to this trace source
   *
   * Add a callback trace source in this resolver. This trace
   * source will match the name specified during namespace 
   * resolution. The TraceContext of this trace source will also
   * be automatically extended to contain the input context.
   */
  template <typename T>
  void AddSource (std::string name, const TraceDoc &doc,
                  const TraceSource &trace, T const &context);
  /**
   * \param name name of trace source
   * \param doc the documentation associated to this trace source
   * \param trace a callback trace source
   *
   * Add a callback trace source in this resolver. This trace
   * source will match the name specified during namespace 
   * resolution.
   */
  void AddSource (std::string name,
                  const TraceDoc &doc,
                  const TraceSource &trace);

  /**
   * \param name the name of the composite element
   * \param composite the composite object
   *
   * The input composite object will be used to resolve a connection
   * of a disconnection attempt if its name matches the trace path.
   * 
   */
  void AddComposite (std::string name, Ptr<Object> composite);

  /**
   * \param name the name of the composite element
   * \param composite the composite object
   * \param contextElement the context element associated to the composite
   *
   * The input composite object will be used to resolve a connection
   * of a disconnection attempt if its name matches the trace path.
   * The contextElement will be appended to the TraceContext during connection.
   */
  template <typename T>
  void AddComposite (std::string name, Ptr<Object> composite, const T &contextElement);

  /**
   * \param name the name of the array
   * \param begin an iterator which points to the first element of the array
   * \param end an iterator which points to the last element of the array
   * \param index an object which can store the index of an element in the
   *        array. In practice, this object should support a constructor
   *        whose single argument is an array index.
   */
  template <typename ITERATOR, typename INDEX>
  void AddArray (std::string name, 
                 ITERATOR begin, ITERATOR end, INDEX index);

  /**
   * \param parent the parent trace resolver
   *
   * The parent trace resolver is the trace resolver returned by the 
   * GetTraceResolver method of the base class of the caller. It is 
   * used during connection and disconnection to chain up the connect
   * and disconnect calls to the parent.
   */
  void SetParentResolver (Ptr<TraceResolver> parent);


private:
  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection);
  virtual void TraceAll (std::ostream &os, const TraceContext &context);
  friend class CompositeTraceResolverTest;
  class ResolveItem 
  {
  public:
    virtual ~ResolveItem () {}
    virtual void Connect (std::string subpath, const CallbackBase &cb, const TraceContext &context) = 0;
    virtual void Disconnect (std::string subpath, const CallbackBase &cb) = 0;
    virtual void CollectSources (std::string path, const TraceContext &context, 
                                 SourceCollection *collection) = 0;
    virtual void TraceAll (std::ostream &os, const TraceContext &context) = 0;

    std::string name;
    TraceContext context;
  };
  typedef std::vector<ResolveItem *> TraceItems;
  class Operation
  {
  public:
    virtual ~Operation () {}
    virtual void Do (std::string subpath, ResolveItem *item) const = 0;
    virtual void DoParent (std::string path, Ptr<TraceResolver> parent) const = 0;
  };

  void AddItem (ResolveItem *item);
  void DoRecursiveOperation (std::string path, 
                             const Operation &operation);
  void DoRecursiveOperationForParent (std::string path, 
                                      const Operation &operation);
  void DoAddComposite (std::string name, Ptr<Object> composite, const TraceContext &context);
  void DoAddSource (std::string name,
                    const TraceDoc &doc,
                    const TraceSource &trace,
                    const TraceContext &context);
  void Add (std::string name, 
            Callback<Ptr<TraceResolver> > createResolver);


  CompositeTraceResolver::TraceItems m_items;
  Ptr<TraceResolver> m_parent;
};

}//namespace ns3

namespace ns3 {



template <typename T>
void 
CompositeTraceResolver::AddSource (std::string name,
                                   const TraceDoc &doc,
                                   const TraceSource &trace, 
                                   T const &context)
{
  TraceContext ctx;
  ctx.AddElement (context);
  DoAddSource (name, doc, trace, ctx);  
}

template <typename ITERATOR, typename INDEX>
void 
CompositeTraceResolver::AddArray (std::string name, 
                                  ITERATOR begin, ITERATOR end, INDEX index)
{
  class ArrayResolveItem : public ResolveItem
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
    virtual void TraceAll (std::ostream &os, const TraceContext &context)
    {
      TraceContext ctx = context;
      ctx.Union (this->context);
      array->TraceAll (os, ctx);
    }

    Ptr<ArrayTraceResolver<INDEX> > array;
  } *item = new ArrayResolveItem ();
  item->name = name;
  item->context = TraceContext ();
  item->array = Create<ArrayTraceResolver<INDEX> > ();
  item->array->SetIterators (begin, end);
  AddItem (item);
}

template <typename T>
void 
CompositeTraceResolver::AddComposite (std::string name, Ptr<Object> composite, const T &contextElement)
{
  TraceContext context;
  context.AddElement (contextElement);
  DoAddComposite (name, composite, context);
}



}//namespace ns3

#endif /* COMPOSITE_TRACE_RESOLVER_H */
