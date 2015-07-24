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

/**
 * \file
 * \ingroup tracing
 * ns3::TracedCallback declaration and template implementation.
 */

namespace ns3 {

/**
 * \ingroup tracing
 * \brief Forward calls to a chain of Callback
 *
 * An TracedCallback has almost exactly the same API as a normal
 * Callback but instead of forwarding calls to a single function
 * (as a Callback normally does), it forwards calls to a chain
 * of Callback.  Connect adds a Callback at the end of the chain
 * of callbacks.  Disconnect removes a Callback from the chain of callbacks.
 *
 * This is a functor: the chain of Callbacks is invoked by
 * calling one of the \c operator() forms with the appropriate
 * number of arguments.
 *
 * \tparam T1 \explicit Type of the first argument to the functor.
 * \tparam T2 \explicit Type of the second argument to the functor.
 * \tparam T3 \explicit Type of the third argument to the functor.
 * \tparam T4 \explicit Type of the fourth argument to the functor.
 * \tparam T5 \explicit Type of the fifth argument to the functor.
 * \tparam T6 \explicit Type of the sixth argument to the functor.
 * \tparam T7 \explicit Type of the seventh argument to the functor.
 * \tparam T8 \explicit Type of the eighth argument to the functor.
 */
template<typename T1 = empty, typename T2 = empty, 
         typename T3 = empty, typename T4 = empty,
         typename T5 = empty, typename T6 = empty,
         typename T7 = empty, typename T8 = empty>
class TracedCallback 
{
public:
  /** Constructor. */
  TracedCallback ();
  /**
   * Append a Callback to the chain (without a context).
   *
   * \param [in] callback Callback to add to chain.
   */
  void ConnectWithoutContext (const CallbackBase & callback);
  /**
   * Append a Callback to the chain with a context.
   *
   * The context string will be provided as the first argument
   * to the Callback.
   *
   * \param [in] callback Callback to add to chain.
   * \param [in] path Context string to provide when invoking the Callback.
   */
  void Connect (const CallbackBase & callback, std::string path);
  /**
   * Remove from the chain a Callback which was connected without a context.
   *
   * \param [in] callback Callback to remove from the chain.
   */
  void DisconnectWithoutContext (const CallbackBase & callback);
  /**
   * Remove from the chain a Callback which was connected with a context.
   *
   * \param [in] callback Callback to remove from the chain.
   * \param [in] path Context path which was used to connect the Callback.
   */
  void Disconnect (const CallbackBase & callback, std::string path);
  /**
   * \name Functors taking various numbers of arguments.
   *
   * The version selected is determined by the number of arguments
   * at the point where the Callback is invoked in the class
   * which fires the Callback.
   */
  /**@{*/
  /** Functor which invokes the chain of Callbacks. */
  void operator() (void) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \param [in] a1 The first argument to the functor.
   */
  void operator() (T1 a1) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   */
  void operator() (T1 a1, T2 a2) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   * \param [in] a3 The third argument to the functor.
   */
  void operator() (T1 a1, T2 a2, T3 a3) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \tparam T4 \deduced Type of the fourth argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   * \param [in] a3 The third argument to the functor.
   * \param [in] a4 The fourth argument to the functor.
   */
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \tparam T4 \deduced Type of the fourth argument to the functor.
   * \tparam T5 \deduced Type of the fifth argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   * \param [in] a3 The third argument to the functor.
   * \param [in] a4 The fourth argument to the functor.
   * \param [in] a5 The fifth argument to the functor.
   */
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \tparam T4 \deduced Type of the fourth argument to the functor.
   * \tparam T5 \deduced Type of the fifth argument to the functor.
   * \tparam T6 \deduced Type of the sixth argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   * \param [in] a3 The third argument to the functor.
   * \param [in] a4 The fourth argument to the functor.
   * \param [in] a5 The fifth argument to the functor.
   * \param [in] a6 The sixth argument to the functor.
   */
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \tparam T4 \deduced Type of the fourth argument to the functor.
   * \tparam T5 \deduced Type of the fifth argument to the functor.
   * \tparam T6 \deduced Type of the sixth argument to the functor.
   * \tparam T7 \deduced Type of the seventh argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   * \param [in] a3 The third argument to the functor.
   * \param [in] a4 The fourth argument to the functor.
   * \param [in] a5 The fifth argument to the functor.
   * \param [in] a6 The sixth argument to the functor.
   * \param [in] a7 The seventh argument to the functor.
   */
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) const;
  /**
   * \copybrief operator()()
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \tparam T4 \deduced Type of the fourth argument to the functor.
   * \tparam T5 \deduced Type of the fifth argument to the functor.
   * \tparam T6 \deduced Type of the sixth argument to the functor.
   * \tparam T7 \deduced Type of the seventh argument to the functor.
   * \tparam T8 \deduced Type of the eighth argument to the functor.
   * \param [in] a1 The first argument to the functor.
   * \param [in] a2 The second argument to the functor.
   * \param [in] a3 The third argument to the functor.
   * \param [in] a4 The fourth argument to the functor.
   * \param [in] a5 The fifth argument to the functor.
   * \param [in] a6 The sixth argument to the functor.
   * \param [in] a7 The seventh argument to the functor.
   * \param [in] a8 The eighth argument to the functor.
   */
  void operator() (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) const;
  /**@}*/

  /**
   *  TracedCallback signature for POD.
   *
   * \param [in] value Value of the traced variable.
   * @{
   */
  // Uint32Callback appears to be the only one used at the moment.
  // Feel free to add typedef's for any other POD you need.
  typedef void (* Uint32Callback)(const uint32_t value);
  /**@}*/

  
private:
  /**
   * Container type for holding the chain of Callbacks.
   *
   * \tparam T1 \deduced Type of the first argument to the functor.
   * \tparam T2 \deduced Type of the second argument to the functor.
   * \tparam T3 \deduced Type of the third argument to the functor.
   * \tparam T4 \deduced Type of the fourth argument to the functor.
   * \tparam T5 \deduced Type of the fifth argument to the functor.
   * \tparam T6 \deduced Type of the sixth argument to the functor.
   * \tparam T7 \deduced Type of the seventh argument to the functor.
   * \tparam T8 \deduced Type of the eighth argument to the functor.
   */
  typedef std::list<Callback<void,T1,T2,T3,T4,T5,T6,T7,T8> > CallbackList;
  /** The chain of Callbacks. */
  CallbackList m_callbackList;
};

} // namespace ns3


/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/

namespace ns3 {

template<typename T1, typename T2, 
         typename T3, typename T4,
         typename T5, typename T6,
         typename T7, typename T8>
TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::TracedCallback ()
  : m_callbackList () 
{
}
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
      (*i)();
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
      (*i)(a1);
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
      (*i)(a1, a2);
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
      (*i)(a1, a2, a3);
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
      (*i)(a1, a2, a3, a4);
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
      (*i)(a1, a2, a3, a4, a5);
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
      (*i)(a1, a2, a3, a4, a5, a6);
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
      (*i)(a1, a2, a3, a4, a5, a6, a7);
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
      (*i)(a1, a2, a3, a4, a5, a6, a7, a8);
    }
}

} // namespace ns3

#endif /* TRACED_CALLBACK_H */
