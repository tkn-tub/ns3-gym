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
#include "ns3/callback.h"
#include "trace-resolver.h"

namespace ns3 {

/**
 * \brief a helper class to offer trace resolution for an array of objects.
 * \ingroup lowleveltracing
 */
template <typename T>
class ArrayTraceResolver : public TraceResolver
{
public:
  /**
   * \brief array index trace context
   *
   * During namespace parsing, ns3::ArrayTraceResolver will
   * embed an instance of this class in the TraceContext 
   * associated to every child object of the object stored
   * at the index.
   *
   * The reason why this class exists is to ensure that we 
   * need to ensure that we can store a unique type as context 
   * into the TraceContext associated to this trace resolver.
   */
  class Index
  {
  public:
    Index ();
    Index (uint32_t index);
    /**
     * The Index is automatically convertible to the 
     * uin32_t type such that it really behaves like a uint32_t
     * array index for the user. 
     *
     * \returns the index itself
     */
    operator uint32_t ();
  private:
    uint32_t m_index;
  };
  /**
   * \param context trace context associated to this trace resolver
   * \param getSize callback which returns dynamically the size of underlying array
   * \param get callback which returns any element in the underlying array
   *
   * Construct a trace resolver which can match any input integer
   * against an element in an array. The array is accessed using a 
   * pair of callbacks. It is the responsability of the user to
   * provide two such callbacks whose job is to adapt the array
   * API to the resolver needs. Each element of the array is expected
   * to provide a method named CreateTraceResolver which takes as
   * only argument a reference to a const TraceContext and returns
   * a pointer to a TraceResolver. i.e. the signature is:
   * TraceResolver * (*) (TraceContext const &)
   */
  ArrayTraceResolver (TraceContext const &context,
                      Callback<uint32_t> getSize, 
                      Callback<T *, uint32_t> get);
private:
  virtual TraceResolverList DoLookup (std::string id) const;
  Callback<uint32_t> m_getSize;
  Callback<T *, uint32_t> m_get;
};
}//namespace ns3

namespace ns3 {

template <typename T>
ArrayTraceResolver<T>::Index::Index ()
  : m_index ()
{}
template <typename T>
ArrayTraceResolver<T>::Index::Index (uint32_t index)
  : m_index (index)
{}
template <typename T>
ArrayTraceResolver<T>::Index::operator uint32_t ()
{
  return m_index;
}

template <typename T>
ArrayTraceResolver<T>::ArrayTraceResolver (TraceContext const &context,
                                           Callback<uint32_t> getSize, 
                                           Callback<T *, uint32_t> get)
  : TraceResolver (context),
    m_getSize (getSize),
    m_get (get)
{}
template <typename T>
TraceResolver::TraceResolverList 
ArrayTraceResolver<T>::DoLookup (std::string id) const
{
  TraceResolverList list;
  if (id == "*")
  {
    for (uint32_t i = 0; i < m_getSize (); i++)
    {
	  TraceContext context = GetContext ();
      typename ArrayTraceResolver<T>::Index index = typename ArrayTraceResolver<T>::Index (i);
	  context.Add (index);
	  list.push_back (m_get (i)->CreateTraceResolver (context));
    }
  }
  return list;
}


}//namespace ns3

#endif /* ARRAY_TRACE_RESOLVER_H */
