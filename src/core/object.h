/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, Gustavo Carneiro
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
 * Authors: Gustavo Carneiro <gjcarneiro@gmail.com>,
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <string>
#include "ptr.h"
#include "trace-resolver.h"
#include "callback.h"
#include "empty.h"

namespace ns3 {

class TraceContext;
class CallbackBase;
class Object;

/**
 * \brief a unique identifier for an interface.
 *
 */
class InterfaceId
{
public:
  /**
   * \param name the name of the requested interface
   * \returns the unique id associated with the requested
   *          name. 
   *
   * This method cannot fail: it will crash if the input 
   * name is not a valid interface name.
   */
  static InterfaceId LookupByName (std::string name);
  /**
   * \param iid a unique id 
   * \returns the parent of the requested id
   *
   * This method cannot fail: it will crash if the input
   * id is not a valid interface id.
   */
  InterfaceId GetParent (void) const;

  /**
   * \returns the name of this interface.
   */
  std::string GetName (void) const;

  InterfaceId (std::string);

  InterfaceId SetParent (InterfaceId iid);
  template <typename T>
  InterfaceId SetParent (void);

  template <typename T>
  InterfaceId AddConstructor (void);
  template <typename T, typename T1>
  InterfaceId AddConstructor (void);
  template <typename T, typename T1, typename T2>
  InterfaceId AddConstructor (void);
  template <typename T, typename T1, typename T2, typename T3>
  InterfaceId AddConstructor (void);
  template <typename T, typename T1, typename T2, typename T3, typename T4>
  InterfaceId AddConstructor (void);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
  InterfaceId AddConstructor (void);


  Ptr<Object> CreateObject (void);
  template <typename T1>
  Ptr<Object> CreateObject (T1 a1);
  template <typename T1, typename T2>
  Ptr<Object> CreateObject (T1 a1, T2 a2);
  template <typename T1, typename T2, typename T3>
  Ptr<Object> CreateObject (T1 a1, T2 a2, T3 a3);
  template <typename T1, typename T2, typename T3, typename T4>
  Ptr<Object> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4);
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  Ptr<Object> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  ~InterfaceId ();
private:
  friend bool operator == (InterfaceId a, InterfaceId b);
  friend bool operator != (InterfaceId a, InterfaceId b);

  InterfaceId (uint16_t iid);
  void DoAddConstructor (CallbackBase callback, uint32_t nArguments);
  CallbackBase LookupConstructor (uint32_t nArguments);
  
  uint16_t m_iid;
};

/**
 * \brief a base class which provides memory management and object aggregation
 *
 * Note: This base class is quite similar in spirit to IUnknown in COM or
 * BonoboObject in Bonobo: it provides three main methods: Ref, Unref and
 * QueryInterface.
 */
class Object
{
public:
  static InterfaceId iid (void);

  Object ();
  virtual ~Object ();
  /**
   * Increment the reference count. This method should not be called
   * by user code. Object instances are expected to be used in conjunction
   * of the Ptr template which would make calling Ref unecessary and 
   * dangerous.
   */
  inline void Ref (void) const;
  /**
   * Decrement the reference count. This method should not be called
   * by user code. Object instances are expected to be used in conjunction
   * of the Ptr template which would make calling Ref unecessary and 
   * dangerous.
   */
  inline void Unref (void) const;
  /**
   * \returns a pointer to the requested interface or zero if it could not be found.
   */
  template <typename T>
  Ptr<T> QueryInterface (void) const;
  /**
   * \param iid the interface id of the requested interface
   * \returns a pointer to the requested interface or zero if it could not be found.
   */
  template <typename T>
  Ptr<T> QueryInterface (InterfaceId iid) const;
  /**
   * Run the DoDispose methods of this object and all the
   * objects aggregated to it.
   * After calling this method, the object is expected to be
   * totally unusable except for the Ref and Unref methods.
   * It is an error to call Dispose twice on the same object 
   * instance
   */
  void Dispose (void);
  /**
   * \param other another object pointer
   *
   * This method aggregates the two objects together: after this
   * method returns, it becomes possible to call QueryInterface
   * on one to get the other, and vice-versa. 
   */
  void AddInterface (Ptr<Object> other);

  /**
   * \param path the path to match for the callback
   * \param cb callback to connect
   *
   * Connect the input callback to all trace sources which
   * match the input path.
   *
   */
  void TraceConnect (std::string path, const CallbackBase &cb) const;
  /**
   * \param path the path to match for the callback
   * \param cb callback to disconnect
   *
   * Disconnect the input callback from all trace sources which
   * match the input path.
   */
  void TraceDisconnect (std::string path, const CallbackBase &cb) const;
  /**
   * \returns the trace resolver associated to this object.
   *
   * This method should be rarely called by users.
   */
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
protected:
  /**
   * This method is called by Object::Dispose.
   * Subclasses are expected to override this method and chain
   * up to their parent's implementation once they are done.
   */
  virtual void DoDispose (void);
private:
  friend class InterfaceIdTraceResolver;
  template <typename T>
  friend Ptr<T> CreateObject (void);
  template <typename T, typename T1>
  friend Ptr<T> CreateObject (T1 a1);
  template <typename T, typename T1, typename T2>
  friend Ptr<T> CreateObject (T1 a1, T2 a2);
  template <typename T, typename T1, typename T2, typename T3>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3);
  template <typename T, typename T1, typename T2, typename T3, typename T4>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);

  Ptr<Object> DoQueryInterface (InterfaceId iid) const;
  void DoCollectSources (std::string path, const TraceContext &context, 
                         TraceResolver::SourceCollection *collection) const;
  void DoTraceAll (std::ostream &os, const TraceContext &context) const;
  bool Check (void) const;
  bool CheckLoose (void) const;
  void MaybeDelete (void) const;
  /**
   * \param iid an InterfaceId
   *
   * Every subclass which defines a new InterfaceId for itself
   * should register this InterfaceId by calling this method
   * from its constructor.
   */
  void SetInterfaceId (InterfaceId iid);

  mutable uint32_t m_count;
  InterfaceId m_iid;
  bool m_disposed;
  mutable bool m_collecting;
  Object *m_next;
};

template <typename T>
Ptr<T> CreateObject (void);

template <typename T, typename T1>
Ptr<T> CreateObject (T1 a1);

template <typename T, typename T1, typename T2>
Ptr<T> CreateObject (T1 a1, T2 a2);

template <typename T, typename T1, typename T2, typename T3>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3);

template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4);

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);

} // namespace ns3

namespace ns3 {

template <typename T>
InterfaceId 
InterfaceId::SetParent (void)
{
  return SetParent (T::iid ());
}

template <typename T>
InterfaceId 
InterfaceId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (void) {
      return ns3::CreateObject<T> ();
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 0);
  return *this;
}
template <typename T, typename T1>
InterfaceId 
InterfaceId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1) {
      return ns3::CreateObject<T> (a1);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 1);
  return *this;
}
template <typename T, typename T1, typename T2>
InterfaceId 
InterfaceId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1, T2 a2) {
      return ns3::CreateObject<T> (a1, a2);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 2);
  return *this;
}
template <typename T, typename T1, typename T2, typename T3>
InterfaceId 
InterfaceId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1, T2 a2, T3 a3) {
      return ns3::CreateObject<T> (a1, a2, a3);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 3);
  return *this;
}
template <typename T, typename T1, typename T2, typename T3, typename T4>
InterfaceId 
InterfaceId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1, T2 a2, T3 a3, T4 a4) {
      return ns3::CreateObject<T> (a1, a2, a3, a4);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 4);
  return *this;
}
template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
InterfaceId 
InterfaceId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
      return ns3::CreateObject<T> (a1, a2, a3, a4, a5);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 5);
  return *this;
}

template <typename T1>
Ptr<Object> 
InterfaceId::CreateObject (T1 a1)
{
  CallbackBase cb = LookupConstructor (1);
  Callback<Ptr<Object>,T1> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1);
  return object;
}
template <typename T1, typename T2>
Ptr<Object> 
InterfaceId::CreateObject (T1 a1, T2 a2)
{
  CallbackBase cb = LookupConstructor (2);
  Callback<Ptr<Object>,T1,T2> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1,a2);
  return object;
}
template <typename T1, typename T2, typename T3>
Ptr<Object> 
InterfaceId::CreateObject (T1 a1, T2 a2, T3 a3)
{
  CallbackBase cb = LookupConstructor (3);
  Callback<Ptr<Object>,T1,T2,T3> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1,a2,a3);
  return object;
}
template <typename T1, typename T2, typename T3, typename T4>
Ptr<Object> 
InterfaceId::CreateObject (T1 a1, T2 a2, T3 a3, T4 a4)
{
  CallbackBase cb = LookupConstructor (4);
  Callback<Ptr<Object>,T1,T2,T3,T4> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1,a2,a3,a4);
  return object;
}
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<Object> 
InterfaceId::CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  CallbackBase cb = LookupConstructor (5);
  Callback<Ptr<Object>,T1,T2,T3,T4,T5> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1,a2,a3,a4,a5);
  return object;
}



void
Object::Ref (void) const
{
  m_count++;
}
void
Object::Unref (void) const
{
  NS_ASSERT (Check ());
  m_count--;
  if (m_count == 0)
    {
      MaybeDelete ();
    }
}

template <typename T>
Ptr<T> 
Object::QueryInterface () const
{
  Ptr<Object> found = DoQueryInterface (T::iid ());
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

template <typename T>
Ptr<T> 
Object::QueryInterface (InterfaceId iid) const
{
  Ptr<Object> found = DoQueryInterface (iid);
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

template <typename T>
Ptr<T> CreateObject (void)
{
  Ptr<T> p = Ptr<T> (new T (), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1>
Ptr<T> CreateObject (T1 a1)
{
  Ptr<T> p = Ptr<T> (new T (a1), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1, typename T2>
Ptr<T> CreateObject (T1 a1, T2 a2)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4, a5), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4, a5, a6), false);
  p->SetInterfaceId (T::iid ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4, a5, a6, a7), false);
  p->SetInterfaceId (T::iid ());
  return p;
}


} // namespace ns3

#endif /* OBJECT_H */

