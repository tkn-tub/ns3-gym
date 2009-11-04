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
#include "test.h"
#include "object.h"
#include "object-factory.h"
#include "assert.h"
#include "singleton.h"
#include "attribute.h"
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

Object::AggregateIterator::AggregateIterator ()
  : m_first (0),
    m_current (0)
{}

bool 
Object::AggregateIterator::HasNext (void) const
{
  if (m_current != 0 && m_current->m_next != PeekPointer (m_first))
    {
      return true;
    }
  return false;
}
Ptr<const Object> 
Object::AggregateIterator::Next (void)
{
  m_current = m_current->m_next;
  return m_current;
}
Object::AggregateIterator::AggregateIterator (Ptr<const Object> first)
  : m_first (first),
    m_current (first)
{}


TypeId 
Object::GetInstanceTypeId (void) const
{
  return m_tid;
}

TypeId 
Object::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Object")
    .SetParent<ObjectBase> ()
    ;
  return tid;
}


Object::Object ()
  : m_count (1),
    m_tid (Object::GetTypeId ()),
    m_disposed (false),
    m_next (this)
{}
Object::~Object () 
{
  m_next = 0;
}
Object::Object (const Object &o)
  : m_count (1),
    m_tid (o.m_tid),
    m_disposed (false),
    m_next (this)
{}
uint32_t
Object::GetReferenceCount (void) const
{
  return m_count;
}
void
Object::Construct (const AttributeList &attributes)
{
  ConstructSelf (attributes);
}

Ptr<Object>
Object::DoGetObject (TypeId tid) const
{
  NS_ASSERT (CheckLoose ());
  const Object *currentObject = this;
  const Object *prevObject = 0;
  TypeId objectTid = Object::GetTypeId ();
  do {
    NS_ASSERT (currentObject != 0);
    TypeId cur = currentObject->GetInstanceTypeId ();
    while (cur != tid && cur != objectTid)
      {
        cur = cur.GetParent ();
      }
    if (cur == tid)
      {
        if (prevObject != 0)
          {
            // This is an attempt to 'cache' the result of this lookup.
            // the idea is that if we perform a lookup for a TypdId on this object,
            // we are likely to perform the same lookup later so, we re-order
            // the circular linked-list of objects here by putting the object we 
            // just found at the head of the list. This optimization is
            // _extremely_ effective in general.
            const_cast<Object*>(prevObject)->m_next = currentObject->m_next;
            const_cast<Object*>(currentObject)->m_next = m_next;
            const_cast<Object*>(this)->m_next = (Object*)currentObject;
          }
        return const_cast<Object *> (currentObject);
      }
    prevObject = currentObject;
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
Object::AggregateObject (Ptr<Object> o)
{
  NS_ASSERT (!m_disposed);
  NS_ASSERT (!o->m_disposed);
  NS_ASSERT (CheckLoose ());
  NS_ASSERT (o->CheckLoose ());

  if (DoGetObject (o->GetInstanceTypeId ()))
    {
      NS_FATAL_ERROR ("Object::AggregateObject(): "
                      "Multiple aggregation of objects of type " << 
                      o->GetInstanceTypeId ().GetName ());
    }

  Object *other = PeekPointer (o);
  Object *next = m_next;
  m_next = other->m_next;
  other->m_next = next;
  NS_ASSERT (CheckLoose ());
  NS_ASSERT (o->CheckLoose ());
  // call NotifyNewAggregate in the listed chain
  Object *currentObject = this;
  do 
    {
      // the NotifyNewAggregate of the current object implementation
      // should be called on the next object in the linked chain
      currentObject->NotifyNewAggregate ();
      currentObject = currentObject->m_next;
    } while (currentObject != this);
}
/**
 * This function must be implemented in the stack that needs to notify
 * other stacks connected to the node of their presence in the node.
 */
void
Object::NotifyNewAggregate ()
{

}

Object::AggregateIterator 
Object::GetAggregateIterator (void) const
{
  return AggregateIterator (this);
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

// ===========================================================================
// Test case to make sure that we can make Objects using CreateObject.
// ===========================================================================
class CreateObjectTestCase : public TestCase
{
public:
  CreateObjectTestCase ();
  virtual ~CreateObjectTestCase ();

private:
  virtual bool DoRun (void);
};

CreateObjectTestCase::CreateObjectTestCase ()
  : TestCase ("Check CreateObject<Type> template function")
{
}

CreateObjectTestCase::~CreateObjectTestCase ()
{
}

bool
CreateObjectTestCase::DoRun (void)
{
  Ptr<BaseA> baseA = CreateObject<BaseA> ();
  NS_TEST_ASSERT_MSG_NE (baseA, 0, "Unable to CreateObject<BaseA>");

  //
  // Since baseA is a BaseA, we must be able to successfully ask for a BaseA.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<BaseA> (), baseA, "GetObject() of same type returns different Ptr");

  //
  // Since BaseA is a BaseA and not a DerivedA, we must not find a DerivedA if we look.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<DerivedA> (), 0, "GetObject() of unrelated type returns nonzero pointer");

  //
  // Since baseA is not a BaseA, we must not be able to ask for a DerivedA even if we
  // try an implied cast back to a BaseA.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<BaseA> (DerivedA::GetTypeId ()), 0, "GetObject() of unrelated returns nonzero Ptr");

  baseA = CreateObject<DerivedA> ();
  NS_TEST_ASSERT_MSG_NE (baseA, 0, "Unable to CreateObject<DerivedA> with implicit cast to BaseA");

  //
  // If we create a DerivedA and cast it to a BaseA, then if we do a GetObject for
  // that BaseA we should get the same address (same Object).
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<BaseA> (), baseA, "Unable to GetObject<BaseA> on BaseA");

  //
  // Since we created a DerivedA and cast it to a BaseA, we should be able to 
  // get back a DerivedA and it should be the original Ptr.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<DerivedA> (), baseA, "GetObject() of the original type returns different Ptr");

  // If we created a DerivedA and cast it to a BaseA, then we GetObject for the 
  // same DerivedA and cast it back to the same BaseA, we should get the same 
  // object.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<BaseA> (DerivedA::GetTypeId ()), baseA, "GetObject returns different Ptr");

  return GetErrorStatus ();
}

// ===========================================================================
// Test case to make sure that we can aggregate Objects.
// ===========================================================================
class AggregateObjectTestCase : public TestCase
{
public:
  AggregateObjectTestCase ();
  virtual ~AggregateObjectTestCase ();

private:
  virtual bool DoRun (void);
};

AggregateObjectTestCase::AggregateObjectTestCase ()
  : TestCase ("Check Object aggregation functionality")
{
}

AggregateObjectTestCase::~AggregateObjectTestCase ()
{
}

bool
AggregateObjectTestCase::DoRun (void)
{
  Ptr<BaseA> baseA = CreateObject<BaseA> ();
  NS_TEST_ASSERT_MSG_NE (baseA, 0, "Unable to CreateObject<BaseA>");

  Ptr<BaseB> baseB = CreateObject<BaseB> ();
  NS_TEST_ASSERT_MSG_NE (baseB, 0, "Unable to CreateObject<BaseB>");

  Ptr<BaseB> baseBCopy = baseB;
  NS_TEST_ASSERT_MSG_NE (baseBCopy, 0, "Unable to copy BaseB");

  //
  // Make an aggregation of a BaseA object and a BaseB object.
  //
  baseA->AggregateObject (baseB);

  //
  // We should be able to ask the aggregation (through baseA) for the BaseA part
  // of the aggregation.
  //
  NS_TEST_ASSERT_MSG_NE (baseA->GetObject<BaseA> (), 0, "Cannot GetObject (through baseA) for BaseA Object");

  //
  // There is no DerivedA in this picture, so we should not be able to GetObject
  // for that type.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<DerivedA> (), 0, "Unexpectedly found a DerivedA through baseA");

  //
  // We should be able to ask the aggregation (through baseA) for the BaseB part
  //
  NS_TEST_ASSERT_MSG_NE (baseA->GetObject<BaseB> (), 0, "Cannot GetObject (through baseA) for BaseB Object");

  //
  // There is no DerivedB in this picture, so we should not be able to GetObject
  // for that type.
  //
  NS_TEST_ASSERT_MSG_EQ (baseA->GetObject<DerivedB> (), 0, "Unexpectedly found a DerivedB through baseA");

  //
  // We should be able to ask the aggregation (through baseA) for the BaseB part
  //
  NS_TEST_ASSERT_MSG_NE (baseB->GetObject<BaseB> (), 0, "Cannot GetObject (through baseB) for BaseB Object");

  //
  // There is no DerivedB in this picture, so we should not be able to GetObject
  // for that type.
  //
  NS_TEST_ASSERT_MSG_EQ (baseB->GetObject<DerivedB> (), 0, "Unexpectedly found a DerivedB through baseB");

  //
  // We should be able to ask the aggregation (through baseB) for the BaseA part
  // of the aggregation.
  //
  NS_TEST_ASSERT_MSG_NE (baseB->GetObject<BaseA> (), 0, "Cannot GetObject (through baseB) for BaseA Object");

  //
  // There is no DerivedA in this picture, so we should not be able to GetObject
  // for that type.
  //
  NS_TEST_ASSERT_MSG_EQ (baseB->GetObject<DerivedA> (), 0, "Unexpectedly found a DerivedA through baseB");

  //
  // baseBCopy is a copy of the original Ptr to the Object BaseB.  Even though
  // we didn't use baseBCopy directly in the aggregations, the object to which
  // it points was used, therefore, we should be able to use baseBCopy as if
  // it were baseB and get a BaseA out of the aggregation.
  //
  NS_TEST_ASSERT_MSG_NE (baseBCopy->GetObject<BaseA> (), 0, "Cannot GetObject (through baseBCopy) for a BaseA Object");

  //
  // Now, change the underlying type of the objects to be the derived types.
  //
  baseA = CreateObject<DerivedA> ();
  NS_TEST_ASSERT_MSG_NE (baseA, 0, "Unable to CreateObject<DerivedA> with implicit cast to BaseA");

  baseB = CreateObject<DerivedB> ();
  NS_TEST_ASSERT_MSG_NE (baseB, 0, "Unable to CreateObject<DerivedB> with implicit cast to BaseB");

  //
  // Create an aggregation of two objects, both of the derived types; and leave
  // an unaggregated copy of one lying around.
  //
  baseBCopy = baseB;
  baseA->AggregateObject (baseB);

  //
  // We should be able to ask the aggregation (through baseA) for the DerivedB part
  //
  NS_TEST_ASSERT_MSG_NE (baseA->GetObject<DerivedB> (), 0, "Cannot GetObject (through baseA) for DerivedB Object");

  //
  // Since the DerivedB is also a BaseB, we should be able to ask the aggregation 
  // (through baseA) for the BaseB part
  //
  NS_TEST_ASSERT_MSG_NE (baseA->GetObject<BaseB> (), 0, "Cannot GetObject (through baseA) for BaseB Object");

  //
  // We should be able to ask the aggregation (through baseB) for the DerivedA part
  //
  NS_TEST_ASSERT_MSG_NE (baseB->GetObject<DerivedA> (), 0, "Cannot GetObject (through baseB) for DerivedA Object");

  //
  // Since the DerivedA is also a BaseA, we should be able to ask the aggregation 
  // (through baseB) for the BaseA part
  //
  NS_TEST_ASSERT_MSG_NE (baseB->GetObject<BaseA> (), 0, "Cannot GetObject (through baseB) for BaseA Object");

  //
  // baseBCopy is a copy of the original Ptr to the Object BaseB.  Even though
  // we didn't use baseBCopy directly in the aggregations, the object to which
  // it points was used, therefore, we should be able to use baseBCopy as if
  // it were baseB (same underlying Object) and get a BaseA and a DerivedA out 
  // of the aggregation through baseBCopy.
  //
  NS_TEST_ASSERT_MSG_NE (baseBCopy->GetObject<BaseA> (), 0, "Cannot GetObject (through baseBCopy) for a BaseA Object");
  NS_TEST_ASSERT_MSG_NE (baseBCopy->GetObject<DerivedA> (), 0, "Cannot GetObject (through baseBCopy) for a BaseA Object");

  //
  // Since the Ptr<BaseB> is actually a DerivedB, we should be able to ask the 
  // aggregation (through baseB) for the DerivedB part
  //
  NS_TEST_ASSERT_MSG_NE (baseB->GetObject<DerivedB> (), 0, "Cannot GetObject (through baseB) for DerivedB Object");

  //
  // Since the DerivedB was cast to a BaseB, we should be able to ask the 
  // aggregation (through baseB) for the BaseB part
  //
  NS_TEST_ASSERT_MSG_NE (baseB->GetObject<BaseB> (), 0, "Cannot GetObject (through baseB) for BaseB Object");

  //
  // Make sure reference counting works in the aggregate.  Create two Objects
  // and aggregate them, then release one of them.  The aggregation should
  // keep a reference to both and the Object we released should still be there.
  //
  baseA = CreateObject<BaseA> ();
  NS_TEST_ASSERT_MSG_NE (baseA, 0, "Unable to CreateObject<BaseA>");

  baseB = CreateObject<BaseB> ();
  NS_TEST_ASSERT_MSG_NE (baseB, 0, "Unable to CreateObject<BaseA>");

  baseA->AggregateObject (baseB);
  baseA = 0;

  baseA = baseB->GetObject<BaseA> ();
  NS_TEST_ASSERT_MSG_NE (baseA, 0, "Unable to GetObject on released object");

  return GetErrorStatus ();
}

// ===========================================================================
// Test case to make sure that an Object factory can create Objects
// ===========================================================================
class ObjectFactoryTestCase : public TestCase
{
public:
  ObjectFactoryTestCase ();
  virtual ~ObjectFactoryTestCase ();

private:
  virtual bool DoRun (void);
};

ObjectFactoryTestCase::ObjectFactoryTestCase ()
  : TestCase ("Check ObjectFactory functionality")
{
}

ObjectFactoryTestCase::~ObjectFactoryTestCase ()
{
}

bool
ObjectFactoryTestCase::DoRun (void)
{
  ObjectFactory factory;

  //
  // Create an Object of type BaseA through an object factory.
  //
  factory.SetTypeId (BaseA::GetTypeId ());
  Ptr<Object> a = factory.Create ();
  NS_TEST_ASSERT_MSG_NE (a, 0, "Unable to factory.Create() a BaseA");

  //
  // What we made should be a BaseA, not have anything to do with a DerivedA
  //
  NS_TEST_ASSERT_MSG_EQ (a->GetObject<BaseA> (DerivedA::GetTypeId ()), 0, "BaseA is unexpectedly a DerivedA also");

  //
  // The BaseA we got should not respond to a GetObject for DerivedA
  //
  NS_TEST_ASSERT_MSG_EQ (a->GetObject<DerivedA> (), 0, "BaseA unexpectedly responds to GetObject for DerivedA");

  //
  // Now tell the factory to make DerivedA Objects and create one with an 
  // implied cast back to a BaseA
  //
  factory.SetTypeId (DerivedA::GetTypeId ());
  a = factory.Create ();

  //
  // Since the DerivedA has a BaseA part, we should be able to use GetObject to 
  // dynamically cast back to a BaseA.
  //
  NS_TEST_ASSERT_MSG_EQ (a->GetObject<BaseA> (), a, "Unable to use GetObject as dynamic_cast<BaseA>()");

  //
  // Since a is already a BaseA and is really a DerivedA, we should be able to
  // GetObject for the DerivedA and cast it back to a BaseA getting the same
  // value that is there.
  //
  NS_TEST_ASSERT_MSG_EQ (a->GetObject<BaseA> (DerivedA::GetTypeId ()), a, "GetObject with implied cast returns different Ptr");

  //
  // Since a declared a BaseA, even if it is really a DerivedA, we should not
  // be able to GetOBject for a DerivedA since this would break the type
  // declaration.
  //
  NS_TEST_ASSERT_MSG_NE (a->GetObject<DerivedA> (), 0, "Unexpectedly able to work around C++ type system");

  return GetErrorStatus ();
}

// ===========================================================================
// The Test Suite that glues the Test Cases together.
// ===========================================================================
class ObjectTestSuite : public TestSuite
{
public:
  ObjectTestSuite ();
};

ObjectTestSuite::ObjectTestSuite ()
  : TestSuite ("object", BVT)
{
  AddTestCase (new CreateObjectTestCase);
  AddTestCase (new AggregateObjectTestCase);
  AddTestCase (new ObjectFactoryTestCase);
}

ObjectTestSuite objectTestSuite;

} // namespace ns3
