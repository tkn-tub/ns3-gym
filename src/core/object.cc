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
#include "object.h"
#include "assert.h"
#include "singleton.h"
#include "attribute.h"
#include "trace-source-accessor.h"
#include "log.h"
#include "string.h"
#include <vector>
#include <sstream>

NS_LOG_COMPONENT_DEFINE ("Object");

namespace ns3 {

/*********************************************************************
 *         The Object implementation
 *********************************************************************/

NS_OBJECT_ENSURE_REGISTERED (Object);

static TypeId
GetObjectIid (void)
{
  TypeId tid = TypeId ("ns3::Object");
  tid.SetParent (tid);
  return tid;
}

TypeId 
Object::GetTypeId (void)
{
  static TypeId tid = GetObjectIid ();
  return tid;
}


Object::Object ()
  : m_count (1),
    m_tid (Object::GetTypeId ()),
    m_disposed (false),
    m_collecting (false),
    m_next (this)
{}
Object::~Object () 
{
  m_next = 0;
}
void
Object::Construct (const AttributeList &attributes)
{
  // loop over the inheritance tree back to the Object base class.
  TypeId tid = m_tid;
  do {
    // loop over all attributes in object type
    NS_LOG_DEBUG ("construct tid="<<tid.GetName ()<<", params="<<tid.GetAttributeListN ());
    for (uint32_t i = 0; i < tid.GetAttributeListN (); i++)
      {
        Ptr<const AttributeAccessor> paramSpec = tid.GetAttributeAccessor (i);
        Attribute initial = tid.GetAttributeInitialValue (i);
        Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (i);
        NS_LOG_DEBUG ("try to construct \""<< tid.GetName ()<<"::"<<
                      tid.GetAttributeName (i)<<"\"");
        if (!(tid.GetAttributeFlags (i) & TypeId::ATTR_CONSTRUCT))
          {
            continue;
          }
        bool found = false;
        // is this attribute stored in this AttributeList instance ?
        for (AttributeList::Attrs::const_iterator j = attributes.m_attributes.begin ();
             j != attributes.m_attributes.end (); j++)
          {
            if (j->checker == checker)
              {
                // We have a matching attribute value.
                DoSet (paramSpec, initial, checker, j->value);
                NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                              tid.GetAttributeName (i)<<"\"");
                found = true;
                break;
              }
          }
        if (!found)
          {
            // is this attribute stored in the global instance instance ?
            for (AttributeList::Attrs::const_iterator j = AttributeList::GetGlobal ()->m_attributes.begin ();
                 j != AttributeList::GetGlobal ()->m_attributes.end (); j++)
              {
                if (j->checker == checker)
                  {
                    // We have a matching attribute value.
                    DoSet (paramSpec, initial, checker, j->value);
                    NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                                  tid.GetAttributeName (i)<<"\" from global");
                    found = true;
                    break;
                  }
              }
          }
        if (!found)
          {
            // No matching attribute value so we set the default value.
            paramSpec->Set (this, initial);
            NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                          tid.GetAttributeName (i)<<"\" from initial value.");
          }
      }
    tid = tid.GetParent ();
  } while (tid != Object::GetTypeId ());
  NotifyConstructionCompleted ();
}
bool
Object::DoSet (Ptr<const AttributeAccessor> spec, Attribute initialValue, 
               Ptr<const AttributeChecker> checker, Attribute value)
{
  bool ok = checker->Check (value);
  if (!ok)
    {
      // attempt to convert to string
      const StringValue *str = value.DynCast<const StringValue *> ();
      if (str == 0)
        {
          return false;
        }
      // attempt to convert back from string.
      Attribute v = checker->Create ();
      ok = v.DeserializeFromString (str->Get ().Get (), checker);
      if (!ok)
        {
          return false;
        }
      ok = checker->Check (v);
      if (!ok)
        {
          return false;
        }
      value = v;
    }
  ok = spec->Set (this, value);
  return ok;
}
void
Object::SetAttribute (std::string name, Attribute value)
{
  struct TypeId::AttributeInfo info;
  if (!m_tid.LookupAttributeByName (name, &info))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" does not exist for this object: tid="<<m_tid.GetName ());
    }
  if (!(info.flags & TypeId::ATTR_SET))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" is not settable for this object: tid="<<m_tid.GetName ());
    }
  if (!DoSet (info.accessor, info.initialValue, info.checker, value))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" could not be set for this object: tid="<<m_tid.GetName ());
    }
}
bool 
Object::SetAttributeFailSafe (std::string name, Attribute value)
{
  struct TypeId::AttributeInfo info;
  if (!m_tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_SET))
    {
      return false;
    }
  return DoSet (info.accessor, info.initialValue, info.checker, value);
}
bool 
Object::GetAttribute (std::string name, std::string &value) const
{
  struct TypeId::AttributeInfo info;
  if (!m_tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_GET))
    {
      return false;
    }
  Attribute v = info.checker->Create ();
  bool ok = info.accessor->Get (this, v);
  if (ok)
    {
      value = v.SerializeToString (info.checker);
    }
  return ok;
}

Attribute
Object::GetAttribute (std::string name) const
{
  struct TypeId::AttributeInfo info;
  if (!m_tid.LookupAttributeByName (name, &info))
    {
      return Attribute ();
    }
  if (!(info.flags & TypeId::ATTR_GET))
    {
      return Attribute ();
    }
  Attribute value = info.checker->Create ();
  bool ok = info.accessor->Get (this, value);
  if (!ok)
    {
      return Attribute ();
    }
  return value;
}

bool 
Object::TraceConnectWithoutContext (std::string name, const CallbackBase &cb)
{
  Ptr<const TraceSourceAccessor> accessor = m_tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->ConnectWithoutContext (this, cb);
  return ok;
}
bool 
Object::TraceConnectWithoutContext (std::string name, std::string context, const CallbackBase &cb)
{
  Ptr<const TraceSourceAccessor> accessor = m_tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->Connect (this, context, cb);
  return ok;
}
bool 
Object::TraceDisconnectWithoutContext (std::string name, const CallbackBase &cb)
{
  Ptr<const TraceSourceAccessor> accessor = m_tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->DisconnectWithoutContext (this, cb);
  return ok;
}
bool 
Object::TraceDisconnectWithoutContext (std::string name, std::string context, const CallbackBase &cb)
{
  Ptr<const TraceSourceAccessor> accessor = m_tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->Disconnect (this, context, cb);
  return ok;
}

TypeId 
Object::GetRealTypeId (void) const
{
  return m_tid;
}


Ptr<Object>
Object::DoGetObject (TypeId tid) const
{
  NS_ASSERT (CheckLoose ());
  const Object *currentObject = this;
  do {
    NS_ASSERT (currentObject != 0);
    TypeId cur = currentObject->m_tid;
    while (cur != tid && cur != Object::GetTypeId ())
      {
        cur = cur.GetParent ();
      }
    if (cur == tid)
      {
        return const_cast<Object *> (currentObject);
      }
    currentObject = currentObject->m_next;
  } while (currentObject != this);
  return 0;
}
void 
Object::Dispose (void)
{
  Object *current = this;
  do {
    NS_ASSERT (current != 0);
    NS_ASSERT (!current->m_disposed);
    current->DoDispose ();
    current->m_disposed = true;
    current = current->m_next;
  } while (current != this);
}
void
Object::NotifyConstructionCompleted (void)
{}
void 
Object::AggregateObject (Ptr<Object> o)
{
  NS_ASSERT (!m_disposed);
  NS_ASSERT (!o->m_disposed);
  NS_ASSERT (CheckLoose ());
  NS_ASSERT (o->CheckLoose ());
  Object *other = PeekPointer (o);
  Object *next = m_next;
  m_next = other->m_next;
  other->m_next = next;
  NS_ASSERT (CheckLoose ());
  NS_ASSERT (o->CheckLoose ());
}

void 
Object::SetTypeId (TypeId tid)
{
  NS_ASSERT (Check ());
  m_tid = tid;
}

void
Object::DoDispose (void)
{
  NS_ASSERT (!m_disposed);
}


bool 
Object::Check (void) const
{
  return (m_count > 0);
}

/* In some cases, when an event is scheduled against a subclass of
 * Object, and if no one owns a reference directly to this object, the
 * object is alive, has a refcount of zero and the method ran when the
 * event expires runs against the raw pointer which means that we are
 * manipulating an object with a refcount of zero.  So, instead we
 * check the aggregate reference count.
 */
bool 
Object::CheckLoose (void) const
{
  uint32_t refcount = 0;
  const Object *current = this;
  do
    {
      refcount += current->m_count;
      current = current->m_next;
    }
  while (current != this);

  return (refcount > 0);
}

void
Object::MaybeDelete (void) const
{
  // First, check if any of the attached
  // Object has a non-zero count.
  const Object *current = this;
  do {
    NS_ASSERT (current != 0);
    if (current->m_count != 0)
      {
        return;
      }
    current = current->m_next;
  } while (current != this);

  // Ensure we are disposed.
  Object *tmp = const_cast<Object *> (this);
  const Object *end = this;
  do {
    NS_ASSERT (current != 0);
    Object *next = tmp->m_next;
    if (!tmp->m_disposed)
      {
        tmp->DoDispose ();
      }
    tmp = next;
  } while (tmp != end);

  // all attached objects have a zero count so, 
  // we can delete all attached objects.
  current = this;
  do {
    NS_ASSERT (current != 0);
    Object *next = current->m_next;
    delete current;
    current = next;
  } while (current != end);
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "test.h"
#include "object-factory.h"

namespace {

class BaseA : public ns3::Object
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("BaseA")
      .SetParent (Object::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<BaseA> ();
    return tid;
  }
  BaseA ()
  {}
  virtual void Dispose (void) {}
};

class DerivedA : public BaseA
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("DerivedA")
      .SetParent (BaseA::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<DerivedA> ();
    return tid;
  }
  DerivedA ()
  {}
  virtual void Dispose (void) {
    BaseA::Dispose ();
  }
};

class BaseB : public ns3::Object
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("BaseB")
      .SetParent (Object::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<BaseB> ();
    return tid;
  }
  BaseB ()
  {}
  virtual void Dispose (void) {}
};

class DerivedB : public BaseB
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("DerivedB")
      .SetParent (BaseB::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<DerivedB> ();
    return tid;
  }
  DerivedB ()
  {}
  virtual void Dispose (void) {
    BaseB::Dispose ();
  }
};

NS_OBJECT_ENSURE_REGISTERED (BaseA);
NS_OBJECT_ENSURE_REGISTERED (DerivedA);
NS_OBJECT_ENSURE_REGISTERED (BaseB);
NS_OBJECT_ENSURE_REGISTERED (DerivedB);

} // namespace anonymous

namespace ns3 {

class ObjectTest : public Test
{
public:
  ObjectTest ();
  virtual bool RunTests (void);
};

ObjectTest::ObjectTest ()
  : Test ("Object")
{}

bool 
ObjectTest::RunTests (void)
{
  bool result = true;

  Ptr<BaseA> baseA = CreateObject<BaseA> ();
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<BaseA> (), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<BaseA> (DerivedA::GetTypeId ()), 0);
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<DerivedA> (), 0);
  baseA = CreateObject<DerivedA> ();
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<BaseA> (), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<BaseA> (DerivedA::GetTypeId ()), baseA);
  NS_TEST_ASSERT_UNEQUAL (baseA->GetObject<DerivedA> (), 0);

  baseA = CreateObject<BaseA> ();
  Ptr<BaseB> baseB = CreateObject<BaseB> ();
  Ptr<BaseB> baseBCopy = baseB;
  baseA->AggregateObject (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->GetObject<BaseA> (), 0);
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->GetObject<BaseB> (), 0);
  NS_TEST_ASSERT_EQUAL (baseA->GetObject<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->GetObject<BaseB> (), 0);
  NS_TEST_ASSERT_EQUAL (baseB->GetObject<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->GetObject<BaseA> (), 0);
  NS_TEST_ASSERT_EQUAL (baseB->GetObject<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->GetObject<BaseA> (), 0);

  baseA = CreateObject<DerivedA> ();
  baseB = CreateObject<DerivedB> ();
  baseBCopy = baseB;
  baseA->AggregateObject (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->GetObject<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->GetObject<BaseB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->GetObject<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->GetObject<BaseA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->GetObject<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->GetObject<BaseA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->GetObject<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->GetObject<BaseB> (), 0)

  baseA = CreateObject<BaseA> ();
  baseB = CreateObject<BaseB> ();
  baseA->AggregateObject (baseB);
  baseA = 0;
  baseA = baseB->GetObject<BaseA> ();


  // Test the object creation code of TypeId
  ObjectFactory factory;
  factory.SetTypeId (BaseA::GetTypeId ());
  Ptr<Object> a = factory.Create ();
  NS_TEST_ASSERT_EQUAL (a->GetObject<BaseA> (), a);
  NS_TEST_ASSERT_EQUAL (a->GetObject<BaseA> (DerivedA::GetTypeId ()), 0);
  NS_TEST_ASSERT_EQUAL (a->GetObject<DerivedA> (), 0);
  factory.SetTypeId (DerivedA::GetTypeId ());
  a = factory.Create ();
  NS_TEST_ASSERT_EQUAL (a->GetObject<BaseA> (), a);
  NS_TEST_ASSERT_EQUAL (a->GetObject<BaseA> (DerivedA::GetTypeId ()), a);
  NS_TEST_ASSERT_UNEQUAL (a->GetObject<DerivedA> (), 0);


  return result;
}

static ObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


