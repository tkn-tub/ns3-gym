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
#ifndef TRACE_CONTEXT_H
#define TRACE_CONTEXT_H

#include <stdint.h>
#include <vector>
#include "ns3/fatal-error.h"

namespace ns3 {

/**
 * \brief Provide context to trace sources
 * \ingroup lowleveltracing
 *
 * Instances of this class are used to hold context
 * for each trace source. Each instance holds a list of
 * 'contexts'. Trace sinks can lookup these contexts
 * from this list with the ns3::TraceContext::Get method.
 *
 * This class is implemented
 * using Copy On Write which means that copying unmodified
 * versions of this class is very cheap. However, modifying
 * the content of this class through a call 
 * to ns3::TraceContext::Add will trigger a costly memory
 * reallocation if needed.
 */
class TraceContext
{
public:
  TraceContext ();
  TraceContext (TraceContext const &o);
  TraceContext const & operator = (TraceContext const &o);
  ~TraceContext ();

  /**
   * \param context add context to list of trace contexts.
   */
  template <typename T>
  void Add (T const &context);

  /**
   * \param o the other context
   *
   * Perform the Union operation (in the sense of set theory) on the
   * two input lists of elements. This method is used in the
   * ns3::CallbackTraceSourceSource class when multiple sinks are connected
   * to a single source to ensure that the source does not need
   * to store a single TraceContext instance per connected sink.
   * Instead, all sinks share the same TraceContext.
   */
  void Add (TraceContext const &o);

  /**
   * \param context context to get from this list of trace contexts.
   *
   * This method cannot fail. If the requested trace context is not
   * stored in this TraceContext, then, the program will halt.
   */
  template <typename T>
  void Get (T &context) const;
private:
  friend class TraceContextTest;
  // used exclusively for testing code.
  template <typename T>
  bool SafeGet (T &context) const;
  template <typename T>
  bool SafeAdd (T &context);

  template <typename T>
  static uint8_t GetUid (void);
  template <typename T>
  static uint8_t GetNextUid (void);
  static uint8_t DoGetNextUid (void);
  static uint8_t GetSize (uint8_t uid);
  uint8_t *CheckPresent (uint8_t uid) const;
  bool DoAdd (uint8_t uid, uint8_t const *buffer);
  bool DoGet (uint8_t uid, uint8_t *buffer) const;

  typedef std::vector<uint8_t> Sizes; 
  static Sizes *GetSizes (void);
  struct Data {
    uint16_t count;
    uint16_t size;
    uint8_t data[4];
  } * m_data;
};

}//namespace ns3

namespace ns3 {

template <typename T>
void 
TraceContext::Add (T const &context)
{
  uint8_t *data = (uint8_t *) &context;
  bool ok = DoAdd (TraceContext::GetUid<T> (), data);
  if (!ok)
    {
      NS_FATAL_ERROR ("Trying to add twice the same type with different values is invalid.");
    }
}
template <typename T>
void
TraceContext::Get (T &context) const
{
  uint8_t *data = (uint8_t *) &context;
  bool found = DoGet (TraceContext::GetUid<T> (), data);
  if (!found)
    {
      NS_FATAL_ERROR ("Type not stored in TraceContext");
    }
}
template <typename T>
bool
TraceContext::SafeGet (T &context) const
{
  uint8_t *data = (uint8_t *) &context;
  bool found = DoGet (TraceContext::GetUid<T> (), data);
  return found;
}
template <typename T>
bool
TraceContext::SafeAdd (T &context)
{
  uint8_t *data = (uint8_t *) &context;
  bool ok = DoAdd (TraceContext::GetUid<T> (), data);
  return ok;
}
template <typename T>
uint8_t
TraceContext::GetUid (void)
{
  static uint8_t uid = GetNextUid<T> ();
  return uid;
}

template <typename T>
uint8_t
TraceContext::GetNextUid (void)
{
  uint8_t uid = DoGetNextUid ();
  GetSizes ()->push_back (sizeof (T));
  return uid;
}

}//namespace ns3

#endif /* TRACE_CONTEXT_H */
