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
#ifndef TRACE_RESOLVER_H
#define TRACE_RESOLVER_H

#include <string>
#include <list>
#include "trace-context.h"
#include "trace-doc.h"

namespace ns3 {

class CallbackBase;

/**
 * \brief the base class which is used to recursively perform trace
 *        namespace resolution.
 * \ingroup tracing
 *
 * Although users could conceivably implement their own trace resolver
 * subclasses, doing so is complicated so, it is recommended to use
 * the default implementation ns3::CompositeTraceResolver instead.
 */
class TraceResolver
{
public:

  TraceResolver ();
  virtual ~TraceResolver ();
  void Ref (void);
  void Unref (void);

  /**
   * \param path the namespace path to resolver
   * \param cb the callback to connect to the matching namespace
   * \param context the context in which to store the trace context
   *
   * First, extract the leading path element from the input path, and 
   * match this leading patch element against any terminal trace source
   * contained in this trace resolver.
   * Second, recursively resolve the rest of the path using other 
   * objects if there are any.
   * If there is any TraceContextElement associated to one of the matching
   * elements, it should be added to the input TraceContext.
   */
  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context) = 0;
  /**
   * \param path the namespace path to resolver
   * \param cb the callback to disconnect in the matching namespace
   *
   * This method should behave as Connect.
   */
  virtual void Disconnect (std::string path, CallbackBase const &cb) = 0;

  class SourceCollection
  {
  public:
    void Print (std::ostream &os) const;
    void AddUnique (std::string path, 
                    const TraceContext &context,
                    const TraceDoc &doc);
  private:
    struct Source
    {
      std::string path;
      TraceContext context;
      TraceDoc doc;
    };
    typedef std::vector<struct Source> SourceVector;
    SourceVector m_sources;
  };
  /**
   * \param path the path to the current recursive level.
   * \param context the trace context associated to the current recursive level
   * \param collection the collection in which to gather every trace source found.
   *
   * This method is invoked recursively until all trace sources have been
   * stored in the output SourceCollection argument.
   */
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection) = 0;
protected:
  /**
   * \param path a namespace path
   * \returns the initial element of the path.
   *
   * If the input path is "/foo/...", the return
   * value is "foo".
   */
  std::string GetElement (std::string path);
  /**
   * \param path a namespace path
   * \returns the subpath.
   *
   * If the input path is "/foo/bar/...", the return
   * value is "/bar/...".
   */
  std::string GetSubpath (std::string path);
private:
  uint32_t m_count;
};

}//namespace ns3

#endif /* TRACE_RESOLVER_H */
