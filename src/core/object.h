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

namespace ns3 {

class TraceContext;
class CallbackBase;

/**
 * \brief a unique identifier for an interface.
 *
 * Instances of this class can be created only through
 * calls to ns3::MakeInterfaceId.
 *
 * Note: This class is quite similar to COM's UUIDs.
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
   * \returns the parent of the requested id, as registered
   *          by ns3::MakeInterfaceId.
   *
   * This method cannot fail: it will crash if the input
   * id is not a valid interface id.
   */
  static InterfaceId LookupParent (InterfaceId iid);

  /**
   * \returns the name of this interface.
   */
  std::string GetName (void) const;
  ~InterfaceId ();
private:
  InterfaceId (uint16_t iid);
  friend InterfaceId MakeInterfaceId (std::string name, const InterfaceId &parent);
  friend InterfaceId MakeObjectInterfaceId (void);
  friend bool operator == (const InterfaceId &a, const InterfaceId &b);
  friend bool operator != (const InterfaceId &a, const InterfaceId &b);
  uint16_t m_iid;
};

/**
 * \param name of the new InterfaceId to create.
 * \param parent the "parent" of the InterfaceId to create.
 * \returns a new InterfaceId
 * \relates InterfaceId
 *
 * Every InterfaceId is a child of another InterfaceId. The
 * top-most InterfaceId is Object::iid and its parent is 
 * itself.
 */
InterfaceId
MakeInterfaceId (std::string name, const InterfaceId &parent);

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
  static const InterfaceId iid;

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
   * \param iid the interface requested
   * \returns a pointer to the requested interface or zero if it could not be found.
   * 
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


  void TraceConnect (std::string path, const CallbackBase &cb);
  void TraceDisconnect (std::string path, const CallbackBase &cb);
  virtual Ptr<TraceResolver> GetTraceResolver (void);
protected:
  /**
   * \param iid an InterfaceId
   *
   * Every subclass which defines a new InterfaceId for itself
   * should register this InterfaceId by calling this method
   * from its constructor.
   */
  void SetInterfaceId (InterfaceId iid);
  /**
   * This method is called by Object::Dispose.
   * Subclasses are expected to override this method and chain
   * up to their parent's implementation once they are done.
   */
  virtual void DoDispose (void);
private:
  friend class InterfaceIdTraceResolver;
  Ptr<Object> DoQueryInterface (InterfaceId iid) const;
  void DoCollectSources (std::string path, const TraceContext &context, 
                         TraceResolver::SourceCollection *collection);
  bool Check (void) const;
  void MaybeDelete (void) const;
  mutable uint32_t m_count;
  InterfaceId m_iid;
  bool m_disposed;
  bool m_collecting;
  Object *m_next;
};

} // namespace ns3

namespace ns3 {

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
Object::QueryInterface (InterfaceId iid) const
{
  Ptr<Object> found = DoQueryInterface (iid);
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

} // namespace ns3

#endif /* OBJECT_H */

