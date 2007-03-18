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
#include "ns3/callback.h"
#include "ns3/fatal-error.h"
#include "trace-context.h"

namespace ns3 {


/**
 * \brief log arbitrary number of parameters to a matching ns3::Callback
 * \ingroup tracing
 *
 * Whenever operator () is invoked on this class, the call and its arguments
 * are forwarded to the internal matching ns3::Callback.
 */
template<typename T1 = empty, typename T2 = empty, 
         typename T3 = empty, typename T4 = empty>
class CallbackTraceSource {
public:
  CallbackTraceSource ();
  void AddCallback (CallbackBase const & callback, TraceContext const & context);
  void RemoveCallback (CallbackBase const & callback);
  void operator() (void);
  void operator() (T1 a1);
  void operator() (T1 a1, T2 a2);
  void operator() (T1 a1, T2 a2, T3 a3);
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4);

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
  if (!cb.CheckType (callback))
    {
      NS_FATAL_ERROR ("Incompatible callbacks. (feed to \"c++filt -t\"): got=\"" << 
		      typeid (callback).name () << "\" expected=\"" <<
		      typeid (cb).name () << "\"");
    }
  m_context.Add (context);
  cb = *static_cast<Callback<void,TraceContext const &,T1,T2,T3,T4> const *> (&callback);
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
CallbackTraceSource<T1,T2,T3,T4>::operator() (void) 
{
  for (typename CallbackList::iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1) 
{
  for (typename CallbackList::iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1, T2 a2) 
{
  for (typename CallbackList::iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1, a2);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1, T2 a2, T3 a3) 
{
  for (typename CallbackList::iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1, a2, a3);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4>
void 
CallbackTraceSource<T1,T2,T3,T4>::operator() (T1 a1, T2 a2, T3 a3, T4 a4) 
{
  for (typename CallbackList::iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (m_context, a1, a2, a3, a4);
    }
}

}//namespace ns3

#endif /* CALLBACK_TRACE_H */
