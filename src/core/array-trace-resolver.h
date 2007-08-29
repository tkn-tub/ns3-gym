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
#ifndef ARRAY_TRACE_RESOLVER_H
#define ARRAY_TRACE_RESOLVER_H

#include <stdint.h>
#include <string>
#include "callback.h"
#include "trace-resolver.h"
#include "object.h"

namespace ns3 {

/**
 * \brief a helper class to offer trace resolution for an array of objects.
 * \ingroup tracing
 *
 * \class ArrayTraceResolver
 *
 * An ArrayTraceResolver is a resolver which can match any input integer
 * against an element in an array. The array is accessed using a 
 * pair iterators. Each element of the array is expected
 * to be a subclass of the Object base class.
 * 
 * When the Connect method is called, this trace resolver will 
 * automatically store in the TraceContext of each resolved object
 * its index through an object of type INDEX specified during the
 * instanciation of the ArrayTraceResolver template.
 */
template <typename INDEX>
class ArrayTraceResolver : public TraceResolver
{
public:
  ArrayTraceResolver ();
  ~ArrayTraceResolver ();

  /**
   * \param begin an iterator which points to the start of the array.
   * \param end an iterator which points to the end of the array.
   */
  template <typename T>
  void SetIterators (T begin, T end);

  // inherited from TraceResolver
  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection);

private:
  class IteratorBase
  {
  public:
    virtual ~IteratorBase () {}
    virtual void Next (void) = 0;
    virtual bool HasNext (void) = 0;
    virtual Ptr<Object> Get (void) = 0;
    virtual void Rewind (void) = 0;
  };
  IteratorBase *m_iter;
};
}//namespace ns3


// implementation
namespace ns3 {

template <typename INDEX>
ArrayTraceResolver<INDEX>::ArrayTraceResolver ()
  : m_iter (0)
{}

template <typename INDEX>
ArrayTraceResolver<INDEX>::~ArrayTraceResolver ()
{
  delete m_iter;
}

template <typename INDEX>
template <typename T>
void
ArrayTraceResolver<INDEX>::SetIterators (T begin, T end)
{
  class Iterator : public IteratorBase 
  {
  public:
    Iterator (T begin, T end)
      : m_begin (begin), m_end (end), m_cur (begin)
    {}
    virtual void Next (void)
    {m_cur++;}
    virtual bool HasNext (void)
    {return m_cur != m_end;}
    virtual Ptr<Object> Get (void)
    {return *m_cur;}
    virtual void Rewind (void)
    {m_cur = m_begin;}
  private:
    T m_begin;
    T m_end;
    T m_cur;
  };
  delete m_iter;
  m_iter = new Iterator (begin, end);
}

template <typename INDEX>
void 
ArrayTraceResolver<INDEX>::Connect (std::string path, CallbackBase const &cb, const TraceContext &context)
{
  if (path == "")
    {
      return;
    }
  std::string id = GetElement (path);
  std::string subpath = GetSubpath (path);
  if (id == "*")
  {
    uint32_t j = 0;
    for (m_iter->Rewind (); m_iter->HasNext (); m_iter->Next ())
      {
        TraceContext tmp = context;
        INDEX index = j;
        tmp.AddElement (index);
        Ptr<Object> obj = m_iter->Get ();
        obj->GetTraceResolver ()->Connect (subpath, cb, tmp);
        j++;
      }
  }
}
template <typename INDEX>
void 
ArrayTraceResolver<INDEX>::Disconnect (std::string path, CallbackBase const &cb)
{
  if (path == "")
    {
      return;
    }
  std::string id = GetElement (path);
  std::string subpath = GetSubpath (path);
  if (id == "*")
  {
    for (m_iter->Rewind (); m_iter->HasNext (); m_iter->Next ())
      {
        Ptr<Object> obj = m_iter->Get ();
        obj->TraceDisconnect (subpath, cb);
      }
  }
}
template <typename INDEX>
void 
ArrayTraceResolver<INDEX>::CollectSources (std::string path, const TraceContext &context, 
                                           SourceCollection *collection)
{
  path.append ("/[0-n]");
  uint32_t j = 0;
  for (m_iter->Rewind (); m_iter->HasNext (); m_iter->Next ())
    {
        TraceContext tmp = context;
        INDEX index = j;
        tmp.AddElement (index);
        Ptr<Object> obj = m_iter->Get ();
        obj->GetTraceResolver ()->CollectSources (path, tmp, collection);
        j++;
    }
}


}//namespace ns3

#endif /* ARRAY_TRACE_RESOLVER_H */
