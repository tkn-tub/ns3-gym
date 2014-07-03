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
#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "type-id.h"
#include "callback.h"
#include <string>
#include <list>

/**
 * \ingroup object
 * \brief Register the class in the ns-3 factory.
 *
 * This macro should be invoked once for every class which
 * defines a new GetTypeId method.
 *
 * If the class is in a namespace, then the macro call should also be
 * in the namespace.
 */
#define NS_OBJECT_ENSURE_REGISTERED(type)       \
  static struct X ## type ## RegistrationClass      \
  {                                             \
    X ## type ## RegistrationClass () {             \
      ns3::TypeId tid = type::GetTypeId ();     \
      tid.GetParent ();                         \
    }                                           \
  } x_ ## type ## RegistrationVariable

namespace ns3 {

class AttributeConstructionList;

/**
 * \ingroup object
 *
 * \brief implement the ns-3 type and attribute system
 *
 * Every class which wants to integrate in the ns-3 type and attribute
 * system should derive from this base class. This base class provides:
 * - a way to associate an ns3::TypeId to each object instance
 * - a way to set and get the attributes registered in the ns3::TypeId.
 */
class ObjectBase
{
public:
  /**
   * Get the type ID.
   */
  static TypeId GetTypeId (void);

  /**
   * Virtual destructor
   */
  virtual ~ObjectBase ();

  /**
   * \return the TypeId associated to the most-derived type
   *          of this instance.
   *
   * This method is typically implemented by ns3::Object::GetInstanceTypeId
   * but some classes which derive from ns3::ObjectBase directly
   * have to implement it themselves.
   */
  virtual TypeId GetInstanceTypeId (void) const = 0;

  /**
   * \param name the name of the attribute to set
   * \param value the name of the attribute to set
   *
   * Set a single attribute. This cannot fail: if the input is invalid,
   * it will crash immediately.
   */
  void SetAttribute (std::string name, const AttributeValue &value);
  /**
   * \param name the name of the attribute to set
   * \param value the name of the attribute to set
   * \return true if the requested attribute exists and could be set, 
   * false otherwise.
   */
  bool SetAttributeFailSafe (std::string name, const AttributeValue &value);
  /**
   * \param name the name of the attribute to read
   * \param value a reference to the value where the result should be stored.
   * \return the attribute read.
   *
   * If the input attribute name does not exist, this method crashes.
   */
  void GetAttribute (std::string name, AttributeValue &value) const;
  /**
   * \param name the name of the attribute to read
   * \param attribute the attribute where the result value should be stored
   * \return true if the requested attribute was found, false otherwise.
   */
  bool GetAttributeFailSafe (std::string name, AttributeValue &attribute) const;

  /**
   * \param name the name of the targetted trace source
   * \param context the trace context associated to the callback
   * \param cb the callback to connect to the trace source.
   *
   * The targetted trace source should be registered with TypeId::AddTraceSource.
   */
  bool TraceConnect (std::string name, std::string context, const CallbackBase &cb);
  /**
   * \param name the name of the targetted trace source
   * \param cb the callback to connect to the trace source.
   *
   * The targetted trace source should be registered with TypeId::AddTraceSource.
   */
  bool TraceConnectWithoutContext (std::string name, const CallbackBase &cb);
  /**
   * \param name the name of the targetted trace source
   * \param context the trace context associated to the callback
   * \param cb the callback to disconnect from the trace source.
   *
   * The targetted trace source should be registered with TypeId::AddTraceSource.
   */
  bool TraceDisconnect (std::string name, std::string context, const CallbackBase &cb);
  /**
   * \param name the name of the targetted trace source
   * \param cb the callback to disconnect from the trace source.
   *
   * The targetted trace source should be registered with TypeId::AddTraceSource.
   */
  bool TraceDisconnectWithoutContext (std::string name, const CallbackBase &cb);

protected:
  /**
   * This method is invoked once all member attributes have been 
   * initialized. Subclasses can override this method to be notified
   * of this event but if they do this, they must chain up to their
   * parent's NotifyConstructionCompleted method.
   */
  virtual void NotifyConstructionCompleted (void);
  /**
   * \param attributes the attribute values used to initialize 
   *        the member variables of this object's instance.
   *
   * Invoked from subclasses to initialize all of their 
   * attribute members. This method will typically be invoked
   * automatically from ns3::CreateObject if your class derives
   * from ns3::Object. If you derive from ns3::ObjectBase directly,
   * you should make sure that you invoke this method from
   * your most-derived constructor.
   */
  void ConstructSelf (const AttributeConstructionList &attributes);

private:
  bool DoSet (Ptr<const AttributeAccessor> spec,
              Ptr<const AttributeChecker> checker, 
              const AttributeValue &value);

};

} // namespace ns3

#endif /* OBJECT_BASE_H */
