/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#ifndef CALLBACK_TRACE_H
#define CALLBACK_TRACE_H

#include <list>
#include "callback.h"
#include "fatal-error.h"
#include "trace-context.h"

namespace ns3 {

class CallbackTraceSourceBase 
{
public:
  virtual ~CallbackTraceSourceBase () {}
  virtual void AddCallback (CallbackBase const & callback, TraceContext const & context) = 0;
  virtual void RemoveCallback (CallbackBase const & callback) = 0;
};


/**
 * \brief log arbitrary number of parameters to a matching ns3::Callback
 * \ingroup lowleveltracing
 *
 * Whenever operator () is invoked on this class, the call and its arguments
 * are forwarded to the internal matching ns3::Callback.
 */
template<typename T1 = empty, typename T2 = empty, 
         typename T3 = empty, typename T4 = empty>
class CallbackTraceSource : public CallbackTraceSourceBase {
public:
  CallbackTraceSource ();
  virtual void AddCallback (CallbackBase const & callback, TraceContext const & context);
  virtual void RemoveCallback (CallbackBase const & callback);
  void operator() (void) const;
  void operator() (T1 a1) const;
  void operator() (T1 a1, T2 a2) const;
  void operator() (T1 a1, T2 a2, T3 a3) const;
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4) const;

private:
  typedef std::list<Callback<void,TraceContext const &,T1,T2,T3,T4> > CallbackList;
  TraceContext m_context;
  CallbackList m_callbackList;
};

}; // namespace ns3

// implementation below.

namespace ns3 {

template<typename T1, typename T2, 
         typename T3, typename T4>
CallbackTraceSource<T1,T2,T3,T4>::CallbackTraceSource ()
  : m_callbackList () 
{}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::AddCallback (CallbackBase const & callback,
                                               TraceContext const &context)
{
  Callback<void,TraceContext const &,T1,T2,T3,T4> cb;
  cb.Assign (callback);
  m_context.Add (context);
  m_callbackList.push_back (cb);
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::RemoveCallback (CallbackBase const & callback)
{
  for (typename CallbackList::iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); /* empty */)
    {
      if ((*i).IsEqual (callback))
	{
	  i = m_callbackList.erase (i);
	}
      else
	{
	  i++;
	}
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (void) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1, T2 a2) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1, a2);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1, T2 a2, T3 a3) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1, a2, a3);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1, T2 a2, T3 a3, T4 a4) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1, a2, a3, a4);
    }
}

}//namespace ns3

#endif /* CALLBACK_TRACE_H */
