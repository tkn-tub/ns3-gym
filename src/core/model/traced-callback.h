/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#ifndef TRACED_CALLBACK_H
#define TRACED_CALLBACK_H

#include <list>
#include "callback.h"

namespace ns3 {

/**
 * \brief forward calls to a chain of Callback
 * \ingroup tracing
 *
 * An ns3::TracedCallback has almost exactly the same API as a normal ns3::Callback but
 * instead of forwarding calls to a single function (as an ns3::Callback normally does),
 * it forwards calls to a chain of ns3::Callback. TracedCallback::Connect adds a ns3::Callback
 * at the end of the chain of callbacks. TracedCallback::Disconnect removes a ns3::Callback from
 * the chain of callbacks.
 */
template<typename T1 = empty, typename T2 = empty, 
         typename T3 = empty, typename T4 = empty,
         typename T5 = empty, typename T6 = empty,
         typename T7 = empty, typename T8 = empty>
class TracedCallback 
{
public:
  TracedCallback ();
  /**
   * \param callback callback to add to chain of callbacks
   *
   * Append the input callback to the end of the internal list 
   * of ns3::Callback.
   */
  void ConnectWithoutContext (const CallbackBase & callback);
  /**
   * \param callback callback to add to chain of callbacks
   * \param path the path to send back to the user callback.
   *
   * Append the input callback to the end of the internal list 
   * of ns3::Callback. This method also will make sure that the
   * input path specified by the user will be give back to the
   * user's callback as its first argument. 
   */
  void Connect (const CallbackBase & callback, std::string path);
  /**
   * \param callback callback to remove from the chain of callbacks.
   *
   * Remove the input callback from the internal list 
   * of ns3::Callback. This method is really the symmetric
   * of the TracedCallback::ConnectWithoutContext method.
   */
  void DisconnectWithoutContext (const CallbackBase & callback);
  /**
   * \param callback callback to remove from the chain of callbacks.
   * \param path the path which is sent back to the user callback.
   *
   * Remove the input callback which has a matching path as first argument 
   * from the internal list of ns3::Callback. This method is really the symmetric
   * of the TracedCallback::Connect method.
   */
  void Disconnect (const CallbackBase & callback, std::string path);
  void operator() (void) const;
  void operator() (T1 a1) const;
  void operator() (T1 a1, T2 a2) const;
  void operator() (T1 a1, T2 a2, T3 a3) const;
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4) const;
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) const;
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) const;
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) const;
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) const;

private:  
  typedef std::list<Callback<void,T1,T2,T3,T4,T5,T6,T7,T8> > CallbackList;
  CallbackList m_callbackList;
};

} // namespace ns3

// implementation below.

namespace ns3 {

template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::TracedCallback ()
  : m_callbackList () 
{}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::ConnectWithoutContext (const CallbackBase & callback)
{
  Callback<void,T1,T2,T3,T4,T5,T6,T7,T8> cb;
  cb.Assign (callback);
  m_callbackList.push_back (cb);
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::Connect (const CallbackBase & callback, std::string path)
{
  Callback<void,std::string,T1,T2,T3,T4,T5,T6,T7,T8> cb;
  cb.Assign (callback);
  Callback<void,T1,T2,T3,T4,T5,T6,T7,T8> realCb = cb.Bind (path);
  m_callbackList.push_back (realCb);
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::DisconnectWithoutContext (const CallbackBase & callback)
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
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::Disconnect (const CallbackBase & callback, std::string path)
{
  Callback<void,std::string,T1,T2,T3,T4,T5,T6,T7,T8> cb;
  cb.Assign (callback);
  Callback<void,T1,T2,T3,T4,T5,T6,T7,T8> realCb = cb.Bind (path);
  DisconnectWithoutContext (realCb);
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (void) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) ();
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2, T3 a3) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2, a3);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2, T3 a3, T4 a4) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2, a3, a4);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2, a3, a4, a5);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2, a3, a4, a5, a6);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2, a3, a4, a5, a6, a7);
    }
}
template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
void 
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) const
{
  for (typename CallbackList::const_iterator i = m_callbackList.begin ();
       i != m_callbackList.end (); i++)
    {
      (*i) (a1, a2, a3, a4, a5, a6, a7, a8);
    }
}

}//namespace ns3

#endif /* TRACED_CALLBACK_H */
