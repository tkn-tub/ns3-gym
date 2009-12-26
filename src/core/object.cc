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
#include <stdlib.h>
#include <string.h>

NS_LOG_COMPONENT_DEFINE ("Object");

namespace ns3 {

/*********************************************************************
 *         The Object implementation
 *********************************************************************/

NS_OBJECT_ENSURE_REGISTERED (Object);

Object::AggregateIterator::AggregateIterator ()
  : m_object (0),
    m_current (0)
{}

bool 
Object::AggregateIterator::HasNext (void) const
{
  return m_current < m_object->m_aggregates->n;
}
Ptr<const Object> 
Object::AggregateIterator::Next (void)
{
  Object *object = m_object->m_aggregates->buffer[m_current];
  m_current++;
  return object;
}
Object::AggregateIterator::AggregateIterator (Ptr<const Object> object)
  : m_object (object),
    m_current (0)
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
  : m_tid (Object::GetTypeId ()),
    m_disposed (false),
    m_started (false),
    m_aggregates ((struct Aggregates *)malloc (sizeof (struct Aggregates))),
    m_getObjectCount (0)
{
  m_aggregates->n = 1;
  m_aggregates->buffer[0] = this;
}
Object::~Object () 
{
  // remove this object from the aggregate list
  uint32_t n = m_aggregates->n;
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      if (current == this)
        {
          memmove (&m_aggregates->buffer[i], 
                   &m_aggregates->buffer[i+1],
                   sizeof (Object *)*(m_aggregates->n - (i+1)));
          m_aggregates->n--;
        }
    }
  // finally, if all objects have been removed from the list,
  // delete the aggregate list
  if (m_aggregates->n == 0)
    {
      free (m_aggregates);
    }
  m_aggregates = 0;
}
Object::Object (const Object &o)
  : m_tid (o.m_tid),
    m_disposed (false),
    m_started (false),
    m_aggregates ((struct Aggregates *)malloc (sizeof (struct Aggregates))),
    m_getObjectCount (0)
{
  m_aggregates->n = 1;
  m_aggregates->buffer[0] = this;
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

  uint32_t n = m_aggregates->n;
  TypeId objectTid = Object::GetTypeId ();
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      TypeId cur = current->GetInstanceTypeId ();
      while (cur != tid && cur != objectTid)
        {
          cur = cur.GetParent ();
        }
      if (cur == tid)
        {
          // This is an attempt to 'cache' the result of this lookup.
          // the idea is that if we perform a lookup for a TypeId on this object,
          // we are likely to perform the same lookup later so, we make sure
          // that the aggregate array is sorted by the number of accesses
          // to each object.

          // first, increment the access count
          current->m_getObjectCount++;
          // then, update the sort
          UpdateSortedArray (m_aggregates, i);
          // finally, return the match
          return const_cast<Object *> (current);
        }
    }
  return 0;
}
void
Object::Start (void)
{
  /**
   * Note: the code here is a bit tricky because we need to protect ourselves from
   * modifications in the aggregate array while DoStart is called. The user's
   * implementation of the DoStart method could call GetObject (which could
   * reorder the array) and it could call AggregateObject which would add an 
   * object at the end of the array. To be safe, we restart iteration over the 
   * array whenever we call some user code, just in case.
   */
 restart:
  uint32_t n = m_aggregates->n;
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      if (!current->m_started)
        {
          current->DoStart ();
          current->m_started = true;
          goto restart;
        }
    }
}
void 
Object::Dispose (void)
{
  /**
   * Note: the code here is a bit tricky because we need to protect ourselves from
   * modifications in the aggregate array while DoDispose is called. The user's
   * DoDispose implementation could call GetObject (which could reorder the array) 
   * and it could call AggregateObject which would add an object at the end of the array.
   * So, to be safe, we restart the iteration over the array whenever we call some
   * user code.
   */
 restart:
  uint32_t n = m_aggregates->n;
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      if (!current->m_disposed)
        {
          current->DoDispose ();
          current->m_disposed = true;
          goto restart;
        }
    }
}
void
Object::UpdateSortedArray (struct Aggregates *aggregates, uint32_t j) const
{
  while (j > 0 && 
         aggregates->buffer[j]->m_getObjectCount > aggregates->buffer[j-1]->m_getObjectCount)
    {
      Object *tmp = aggregates->buffer[j-1];
      aggregates->buffer[j-1] = aggregates->buffer[j];
      aggregates->buffer[j] = tmp;
      j--;
    }
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
  // first create the new aggregate buffer.
  uint32_t total = m_aggregates->n + other->m_aggregates->n;
  struct Aggregates *aggregates = 
    (struct Aggregates *)malloc (sizeof(struct Aggregates)+(total-1)*sizeof(Object*));
  aggregates->n = total;

  // copy our buffer to the new buffer
  memcpy (&aggregates->buffer[0], 
          &m_aggregates->buffer[0], 
          m_aggregates->n*sizeof(Object*));

  // append the other buffer into the new buffer too
  for (uint32_t i = 0; i < other->m_aggregates->n; i++)
    {
      aggregates->buffer[m_aggregates->n+i] = other->m_aggregates->buffer[i];
      UpdateSortedArray (aggregates, m_aggregates->n + i);
    }

  // keep track of the old aggregate buffers for the iteration
  // of NotifyNewAggregates
  struct Aggregates *a = m_aggregates;
  struct Aggregates *b = other->m_aggregates;

  // Then, assign the new aggregation buffer to every object
  uint32_t n = aggregates->n;
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = aggregates->buffer[i];
      current->m_aggregates = aggregates;
    }

  // Finally, call NotifyNewAggregate on all the objects aggregates together.
  // We purposedly use the old aggregate buffers to iterate over the objects
  // because this allows us to assume that they will not change from under 
  // our feet, even if our users call AggregateObject from within their
  // NotifyNewAggregate method.
  for (uint32_t i = 0; i < a->n; i++)
    {
      Object *current = a->buffer[i];
      current->NotifyNewAggregate ();
    }
  for (uint32_t i = 0; i < b->n; i++)
    {
      Object *current = b->buffer[i];
      current->NotifyNewAggregate ();
    }

  // Now that we are done with them, we can free our old aggregate buffers
  free (a);
  free (b);
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

void
Object::DoStart (void)
{
  NS_ASSERT (!m_started);
}

bool 
Object::Check (void) const
{
  return (GetReferenceCount () > 0);
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
  uint32_t n = m_aggregates->n;
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      refcount += current->GetReferenceCount ();
    }
  return (refcount > 0);
}
void
Object::DoDelete (void)
{
  // check if we really need to die
  for (uint32_t i = 0; i < m_aggregates->n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      if (current->GetReferenceCount () > 0)
        {
          return;
        }
    }

  // Now, we know that we are alone to use this aggregate so, 
  // we can dispose and delete everything safely.

  uint32_t n = m_aggregates->n;
  // Ensure we are disposed.
  for (uint32_t i = 0; i < n; i++)
    {
      Object *current = m_aggregates->buffer[i];
      if (!current->m_disposed)
        {
          current->DoDispose ();
        }
    }

  // Now, actually delete all objects
  struct Aggregates *aggregates = m_aggregates;
  for (uint32_t i = 0; i < n; i++)
    {
      // There is a trick here: each time we call delete below,
      // the deleted object is removed from the aggregate buffer
      // in the destructor so, the index of the next element to 
      // lookup is always zero
      Object *current = aggregates->buffer[0];
      delete current;
    }
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
