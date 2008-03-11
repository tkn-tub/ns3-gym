/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef TRACE_SOURCE_ACCESSOR_H
#define TRACE_SOURCE_ACCESSOR_H

#include <stdint.h>
#include "object-base.h"
#include "callback.h"
#include "ptr.h"

namespace ns3 {

/**
 * \brief control access to objects' trace sources
 *
 * This class abstracts the kind of trace source to which we want to connect.
 */
class TraceSourceAccessor : public ObjectBase
{
public:
  TraceSourceAccessor ();
  virtual ~TraceSourceAccessor ();
  void Ref (void) const;
  void Unref (void) const;

  /**
   * \param obj the object instance which contains the target trace source.
   * \param cb the callback to connect to the target trace source.
   */
  virtual bool ConnectWithoutContext (ObjectBase *obj, const CallbackBase &cb) const = 0;
  /**
   * \param obj the object instance which contains the target trace source.
   * \param context the context to bind to the user callback.
   * \param cb the callback to connect to the target trace source.
   */
  virtual bool Connect (ObjectBase *obj, std::string context, const CallbackBase &cb) const = 0;
  /**
   * \param obj the object instance which contains the target trace source.
   * \param cb the callback to disconnect from the target trace source.
   */
  virtual bool DisconnectWithoutContext (ObjectBase *obj, const CallbackBase &cb) const = 0;
  /**
   * \param obj the object instance which contains the target trace source.
   * \param context the context which was bound to the user callback.
   * \param cb the callback to disconnect from the target trace source.
   */
  virtual bool Disconnect (ObjectBase *obj, std::string context, const CallbackBase &cb) const = 0;
private:
  mutable uint32_t m_count;
};

/**
 * \param a the trace source
 *
 * Create a TraceSourceAccessor which will control access to the underlying
 * trace source.
 */
template <typename T>
Ptr<const TraceSourceAccessor> MakeTraceSourceAccessor (T a);

} // namespace ns3

namespace ns3 {

template <typename T, typename SOURCE>
Ptr<const TraceSourceAccessor> 
DoMakeTraceSourceAccessor (SOURCE T::*a)
{
  struct Accessor : public TraceSourceAccessor
  {
    virtual bool ConnectWithoutContext (ObjectBase *obj, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).ConnectWithoutContext (cb);
      return true;
    }
    virtual bool Connect (ObjectBase *obj, std::string context, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).Connect (cb, context);
      return true;
    }
    virtual bool DisconnectWithoutContext (ObjectBase *obj, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).DisconnectWithoutContext (cb);
      return true;      
    }
    virtual bool Disconnect (ObjectBase *obj, std::string context, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).Disconnect (cb, context);
      return true;      
    }
    SOURCE T::*m_source;
  } *accessor = new Accessor ();
  accessor->m_source = a;
  return Ptr<const TraceSourceAccessor> (accessor, false);
}

template <typename T>
Ptr<const TraceSourceAccessor> MakeTraceSourceAccessor (T a)
{
  return DoMakeTraceSourceAccessor (a);
}

} // namespace ns3


#endif /* TRACE_SOURCE_ACCESSOR_H */
