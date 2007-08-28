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
#include "uid-manager.h"
#include "trace-resolver.h"
#include "debug.h"
#include <vector>

NS_DEBUG_COMPONENT_DEFINE ("Object");

namespace {

class IidManager : public ns3::UidManager
{};

class IidTree
{
public:
  void SetParent (uint16_t child, const uint16_t *parent);
  uint16_t LookupParent (uint16_t child);

private:
  std::vector<const uint16_t *> m_parents;
};

void 
IidTree::SetParent (uint16_t child, const uint16_t *parent)
{
  m_parents.resize (child+1);
  m_parents[child] = parent;
}
uint16_t 
IidTree::LookupParent (uint16_t child)
{
  NS_ASSERT (child < m_parents.size ());
  return *(m_parents[child]);
}

} // anonymous namespace

namespace ns3 {

class InterfaceIdTraceResolver : public TraceResolver
{
public:
  InterfaceIdTraceResolver (Ptr<const Object> aggregate);
  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection);
private:
  Ptr<const Object> ParseForInterface (std::string path);
  Ptr<const Object> m_aggregate;
};

InterfaceIdTraceResolver::InterfaceIdTraceResolver (Ptr<const Object> aggregate)
  : m_aggregate (aggregate)
{}
Ptr<const Object>
InterfaceIdTraceResolver::ParseForInterface (std::string path)
{
  std::string element = GetElement (path);
  std::string::size_type dollar_pos = element.find ("$");
  if (dollar_pos != 0)
    {
      return 0;
    }
  std::string interfaceName = element.substr (1, std::string::npos);
  InterfaceId interfaceId = InterfaceId::LookupByName (interfaceName);
  Ptr<Object> interface = m_aggregate->QueryInterface<Object> (interfaceId);
  return interface;
}
void 
InterfaceIdTraceResolver::Connect (std::string path, CallbackBase const &cb, const TraceContext &context)
{
  Ptr<const Object> interface = ParseForInterface (path);
  if (interface != 0)
    {
      interface->GetTraceResolver ()->Connect (GetSubpath (path), cb, context);
    }
}
void 
InterfaceIdTraceResolver::Disconnect (std::string path, CallbackBase const &cb)
{
  Ptr<const Object> interface = ParseForInterface (path);
  if (interface != 0)
    {
      interface->TraceDisconnect (GetSubpath (path), cb);
    }
}
void 
InterfaceIdTraceResolver::CollectSources (std::string path, const TraceContext &context, 
                                          SourceCollection *collection)
{
  m_aggregate->DoCollectSources (path, context, collection);
}


InterfaceId::InterfaceId (uint16_t iid)
  : m_iid (iid)
{}
InterfaceId::~InterfaceId ()
{}
InterfaceId 
InterfaceId::LookupByName (std::string name)
{
  uint32_t uid = Singleton<IidManager>::Get ()->LookupByName (name);
  NS_ASSERT (uid != 0 && uid <= 0xffff);
  return InterfaceId (uid);
}
InterfaceId 
InterfaceId::LookupParent (InterfaceId iid)
{
  return Singleton<IidTree>::Get ()->LookupParent (iid.m_iid);
}
std::string 
InterfaceId::GetName (void) const
{
  std::string name = Singleton<IidManager>::Get ()->LookupByUid (m_iid);
  return name;
}

bool operator == (const InterfaceId &a, const InterfaceId &b)
{
  return a.m_iid == b.m_iid;
}

bool operator != (const InterfaceId &a, const InterfaceId &b)
{
  return a.m_iid != b.m_iid;
}

InterfaceId
MakeInterfaceId (std::string name, const InterfaceId &parent)
{
  uint32_t uid = Singleton<IidManager>::Get ()->Allocate (name);
  NS_ASSERT (uid <= 0xffff);
  InterfaceId iid = uid;
  Singleton<IidTree>::Get ()->SetParent (iid.m_iid, &parent.m_iid);
  return iid;
}

InterfaceId
MakeObjectInterfaceId (void)
{
  InterfaceId iid = Singleton<IidManager>::Get ()->Allocate ("Object");
  Singleton<IidTree>::Get ()->SetParent (iid.m_iid, &iid.m_iid);
  return iid;
}


const InterfaceId Object::iid = MakeObjectInterfaceId ();


Object::Object ()
  : m_count (1),
    m_iid (Object::iid),
    m_disposed (false),
    m_collecting (false),
    m_next (this)
{}
Object::~Object () 
{
  m_next = 0;
}
Ptr<Object>
Object::DoQueryInterface (InterfaceId iid) const
{
  NS_ASSERT (Check ());
  const Object *currentObject = this;
  do {
    NS_ASSERT (currentObject != 0);
    InterfaceId cur = currentObject->m_iid;
    while (cur != iid && cur != Object::iid)
      {
        cur = InterfaceId::LookupParent (cur);
      }
    if (cur == iid)
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
Object::AddInterface (Ptr<Object> o)
{
  NS_ASSERT (!m_disposed);
  NS_ASSERT (!o->m_disposed);
  NS_ASSERT (Check ());
  NS_ASSERT (o->Check ());
  Object *other = PeekPointer (o);
  Object *next = m_next;
  m_next = other->m_next;
  other->m_next = next;
  NS_ASSERT (Check ());
  NS_ASSERT (o->Check ());
}

void 
Object::TraceConnect (std::string path, const CallbackBase &cb) const
{
  GetTraceResolver ()->Connect (path, cb, TraceContext ());
}
void 
Object::TraceDisconnect (std::string path, const CallbackBase &cb) const
{
  GetTraceResolver ()->Disconnect (path, cb);
}

void 
Object::SetInterfaceId (InterfaceId iid)
{
  NS_ASSERT (Check ());
  m_iid = iid;
}

void
Object::DoDispose (void)
{
  NS_ASSERT (!m_disposed);
}

Ptr<TraceResolver>
Object::GetTraceResolver (void) const
{
  Ptr<InterfaceIdTraceResolver> resolver =
    Create<InterfaceIdTraceResolver> (this);
  return resolver;
}

bool 
Object::Check (void) const
{
  return (m_count > 0);
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

  // all attached objects have a zero count so, 
  // we can delete all attached objects.
  current = this;
  const Object *end = this;
  do {
    NS_ASSERT (current != 0);
    Object *next = current->m_next;
    delete current;
    current = next;
  } while (current != end);
}

void 
Object::DoCollectSources (std::string path, const TraceContext &context, 
                          TraceResolver::SourceCollection *collection) const
{
  const Object *current;
  current = this;
  do {
    if (current->m_collecting)
      {
        return;
      }
    current = current->m_next;
  } while (current != this);

  m_collecting = true;

  current = this->m_next;
  while (current != this)
    {
      NS_ASSERT (current != 0);
      NS_DEBUG ("collect current=" << current);
      InterfaceId cur = current->m_iid;
      while (cur != Object::iid)
        {
          std::string name = cur.GetName ();
          std::string fullpath = path;
          fullpath.append ("/$");
          fullpath.append (name);
          NS_DEBUG ("collect: " << fullpath);
          current->GetTraceResolver ()->CollectSources (fullpath, context, collection);
          cur = InterfaceId::LookupParent (cur);
        }
      current = current->m_next;
    }

  m_collecting = false;
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "test.h"
#include "sv-trace-source.h"
#include "composite-trace-resolver.h"

namespace {

class BaseA : public ns3::Object
{
public:
  static const ns3::InterfaceId iid;
  BaseA ()
  {
    SetInterfaceId (BaseA::iid);
  }
  void BaseGenerateTrace (int16_t v)
  { m_source = v; }
  virtual void Dispose (void) {}
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void)
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("basea-x", ns3::TraceDoc ("test source"), m_source);
    resolver->SetParent (Object::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_source;
};

class DerivedA : public BaseA
{
public:
  static const ns3::InterfaceId iid;
  DerivedA (int v)
  {
    SetInterfaceId (DerivedA::iid);
  }
  void DerivedGenerateTrace (int16_t v)
  { m_sourceDerived = v; }
  virtual void Dispose (void) {
    BaseA::Dispose ();
  }
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void)
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("deriveda-x", ns3::TraceDoc ("test source"), m_sourceDerived);
    resolver->SetParent (BaseA::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_sourceDerived;
};

const ns3::InterfaceId BaseA::iid = 
  ns3::MakeInterfaceId ("BaseA", Object::iid);
const ns3::InterfaceId DerivedA::iid = 
  ns3::MakeInterfaceId ("DerivedA", BaseA::iid);;

class BaseB : public ns3::Object
{
public:
  static const ns3::InterfaceId iid;
  BaseB ()
  {
    SetInterfaceId (BaseB::iid);
  }
  void BaseGenerateTrace (int16_t v)
  { m_source = v; }
  virtual void Dispose (void) {}
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void)
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("baseb-x", ns3::TraceDoc ("test source"), m_source);
    resolver->SetParent (Object::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_source;
};

class DerivedB : public BaseB
{
public:
  static const ns3::InterfaceId iid;
  DerivedB (int v)
  {
    SetInterfaceId (DerivedB::iid);
  }
  void DerivedGenerateTrace (int16_t v)
  { m_sourceDerived = v; }
  virtual void Dispose (void) {
    BaseB::Dispose ();
  }
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void)
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("derivedb-x", ns3::TraceDoc ("test source"), m_sourceDerived);
    resolver->SetParent (BaseB::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_sourceDerived;
};

const ns3::InterfaceId BaseB::iid = 
  ns3::MakeInterfaceId ("BaseB", Object::iid);
const ns3::InterfaceId DerivedB::iid = 
  ns3::MakeInterfaceId ("DerivedB", BaseB::iid);;

} // namespace anonymous

namespace ns3 {

class ObjectTest : public Test
{
public:
  ObjectTest ();
  virtual bool RunTests (void);
private:
  void BaseATrace (const TraceContext &context, int64_t oldValue, int64_t newValue);
  void DerivedATrace (const TraceContext &context, int64_t oldValue, int64_t newValue);
  void BaseBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue);
  void DerivedBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue);

  bool m_baseATrace;
  bool m_derivedATrace;
  bool m_baseBTrace;
  bool m_derivedBTrace;
};

ObjectTest::ObjectTest ()
  : Test ("Object")
{}
void 
ObjectTest::BaseATrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_baseATrace = true;
}
void 
ObjectTest::DerivedATrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_derivedATrace = true;
}
void 
ObjectTest::BaseBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_baseBTrace = true;
}
void 
ObjectTest::DerivedBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_derivedBTrace = true;
}

bool 
ObjectTest::RunTests (void)
{
  bool result = true;

  Ptr<BaseA> baseA = Create<BaseA> ();
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (BaseA::iid), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (DerivedA::iid), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedA> (DerivedA::iid), 0);
  baseA = Create<DerivedA> (10);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (BaseA::iid), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (DerivedA::iid), baseA);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<DerivedA> (DerivedA::iid), 0);

  baseA = Create<BaseA> ();
  Ptr<BaseB> baseB = Create<BaseB> ();
  Ptr<BaseB> baseBCopy = baseB;
  baseA->AddInterface (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseA> (BaseA::iid), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedA> (DerivedA::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseB> (BaseB::iid), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedB> (DerivedB::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseB> (BaseB::iid), 0);
  NS_TEST_ASSERT_EQUAL (baseB->QueryInterface<DerivedB> (DerivedB::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseA> (BaseA::iid), 0);
  NS_TEST_ASSERT_EQUAL (baseB->QueryInterface<DerivedA> (DerivedA::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<BaseA> (BaseA::iid), 0);

  baseA = Create<DerivedA> (1);
  baseB = Create<DerivedB> (1);
  baseBCopy = baseB;
  baseA->AddInterface (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<DerivedB> (DerivedB::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseB> (BaseB::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<DerivedA> (DerivedA::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseA> (BaseA::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<DerivedA> (DerivedA::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<BaseA> (BaseA::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<DerivedB> (DerivedB::iid), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseB> (BaseB::iid), 0)

  baseA = Create<BaseA> ();
  baseB = Create<BaseB> ();
  baseA->AddInterface (baseB);
  baseA = 0;
  baseA = baseB->QueryInterface<BaseA> (BaseA::iid);

  baseA = Create<BaseA> ();
  baseA->TraceConnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (1);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseB = Create<BaseB> ();
  baseB->TraceConnect ("/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));
  m_baseBTrace = false;
  baseB->BaseGenerateTrace (2);
  NS_TEST_ASSERT (m_baseBTrace);
  baseB->TraceDisconnect ("/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));

  baseA->AddInterface (baseB);

  baseA->TraceConnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (3);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseA->TraceConnect ("/$BaseB/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));
  m_baseBTrace = false;
  baseB->BaseGenerateTrace (4);
  NS_TEST_ASSERT (m_baseBTrace);
  baseA->TraceDisconnect ("/$BaseB/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));
  m_baseBTrace = false;
  baseB->BaseGenerateTrace (5);
  NS_TEST_ASSERT (!m_baseBTrace);

  baseB->TraceConnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (6);
  NS_TEST_ASSERT (m_baseATrace);
  baseB->TraceDisconnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseA->TraceConnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (7);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  Ptr<DerivedA> derivedA;
  derivedA = Create<DerivedA> (1);
  baseB = Create<BaseB> ();
  derivedA->AddInterface (baseB);
  baseB->TraceConnect ("/$DerivedA/deriveda-x", MakeCallback (&ObjectTest::DerivedATrace, this));
  baseB->TraceConnect ("/$DerivedA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_derivedATrace = false;
  m_baseATrace = false;
  derivedA->DerivedGenerateTrace (8);
  derivedA->BaseGenerateTrace (9);
  NS_TEST_ASSERT (m_derivedATrace);
  NS_TEST_ASSERT (m_baseATrace);
  baseB->TraceDisconnect ("/$DerivedA/deriveda-x", MakeCallback (&ObjectTest::BaseATrace, this));
  baseB->TraceDisconnect ("/$DerivedA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseB->TraceConnect ("/$DerivedA/*", MakeCallback (&ObjectTest::DerivedATrace, this));
  m_derivedATrace = false;
  derivedA->DerivedGenerateTrace (10);
  NS_TEST_ASSERT (m_derivedATrace);
  // here, we have connected the derived trace sink to all 
  // trace sources, including the base trace source.
  m_derivedATrace = false;
  derivedA->BaseGenerateTrace (11);
  NS_TEST_ASSERT (m_derivedATrace);
  baseB->TraceDisconnect ("/$DerivedA/*", MakeCallback (&ObjectTest::BaseATrace, this));

  return result;
}

static ObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


