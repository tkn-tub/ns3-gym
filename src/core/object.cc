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
#include "trace-resolver.h"
#include "log.h"
#include <vector>

NS_LOG_COMPONENT_DEFINE ("Object");

/*********************************************************************
 *         Helper code
 *********************************************************************/

namespace {

class IidManager
{
public:
  uint16_t AllocateUid (std::string name);
  void SetParent (uint16_t uid, uint16_t parent);
  void AddConstructor (uint16_t uid, ns3::CallbackBase callback, uint32_t nArguments);
  uint16_t GetUid (std::string name) const;
  std::string GetName (uint16_t uid) const;
  uint16_t GetParent (uint16_t uid) const;
  ns3::CallbackBase GetConstructor (uint16_t uid, uint32_t nArguments);
  bool HasConstructor (uint16_t uid);
  uint32_t GetRegisteredN (void);
  uint16_t GetRegistered (uint32_t i);
private:
  struct ConstructorInformation {
    ns3::CallbackBase cb;
    uint32_t nArguments;
  };
  struct IidInformation {
    std::string name;
    uint16_t parent;
    std::vector<struct ConstructorInformation> constructors;
  };
  typedef std::vector<struct IidInformation>::const_iterator Iterator;

  struct IidManager::IidInformation *LookupInformation (uint16_t uid) const;

  std::vector<struct IidInformation> m_information;
};

uint16_t 
IidManager::AllocateUid (std::string name)
{
  uint16_t j = 1;
  for (Iterator i = m_information.begin (); i != m_information.end (); i++)
    {
      if (i->name == name)
        {
          NS_FATAL_ERROR ("Trying to allocate twice the same uid: " << name);
          return 0;
        }
      j++;
    }
  struct IidInformation information;
  information.name = name;
  information.parent = 0;
  m_information.push_back (information);
  uint32_t uid = m_information.size ();
  NS_ASSERT (uid <= 0xffff);
  return uid;
}

struct IidManager::IidInformation *
IidManager::LookupInformation (uint16_t uid) const
{
  NS_ASSERT (uid <= m_information.size ());
  return const_cast<struct IidInformation *> (&m_information[uid-1]);
}

void 
IidManager::SetParent (uint16_t uid, uint16_t parent)
{
  NS_ASSERT (parent <= m_information.size ());
  struct IidInformation *information = LookupInformation (uid);
  information->parent = parent;
}
void 
IidManager::AddConstructor (uint16_t uid, ns3::CallbackBase callback, uint32_t nArguments)
{
  struct IidInformation *information = LookupInformation (uid);
  struct ConstructorInformation constructor;
  constructor.cb = callback;
  constructor.nArguments = nArguments;
  for (std::vector<struct ConstructorInformation>::const_iterator i = information->constructors.begin ();
       i != information->constructors.end (); i++)
    {
      if (i->nArguments == nArguments)
        {
          NS_FATAL_ERROR ("registered two constructors on the same type with the same number of arguments.");
          break;
        }
    }
  information->constructors.push_back (constructor);
}

uint16_t 
IidManager::GetUid (std::string name) const
{
  uint32_t j = 1;
  for (Iterator i = m_information.begin (); i != m_information.end (); i++)
    {
      if (i->name == name)
        {
          NS_ASSERT (j <= 0xffff);
          return j;
        }
      j++;
    }
  return 0;
}
std::string 
IidManager::GetName (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->name;
}
uint16_t 
IidManager::GetParent (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->parent;
}
ns3::CallbackBase 
IidManager::GetConstructor (uint16_t uid, uint32_t nArguments)
{
  struct IidInformation *information = LookupInformation (uid);
  for (std::vector<struct ConstructorInformation>::const_iterator i = information->constructors.begin ();
       i != information->constructors.end (); i++)
    {
      if (i->nArguments == nArguments)
        {
          return i->cb;
        } 
    }
  NS_FATAL_ERROR ("Requested constructor with "<<nArguments<<" arguments not found");
  return ns3::CallbackBase ();
}

bool 
IidManager::HasConstructor (uint16_t uid)
{
  struct IidInformation *information = LookupInformation (uid);
  return !information->constructors.empty ();
}

uint32_t 
IidManager::GetRegisteredN (void)
{
  return m_information.size ();
}
uint16_t 
IidManager::GetRegistered (uint32_t i)
{
  return i;
}


} // anonymous namespace

/*********************************************************************
 *         The InterfaceId TraceResolver
 *********************************************************************/

namespace ns3 {

class InterfaceIdTraceResolver : public TraceResolver
{
public:
  InterfaceIdTraceResolver (Ptr<const Object> aggregate);
  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection);
  virtual void TraceAll (std::ostream &os, const TraceContext &context);
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
void 
InterfaceIdTraceResolver::TraceAll (std::ostream &os, const TraceContext &context)
{
  m_aggregate->DoTraceAll (os, context);
}

/*********************************************************************
 *         The InterfaceId class
 *********************************************************************/

InterfaceId::InterfaceId (std::string name)
{
  uint16_t uid = Singleton<IidManager>::Get ()->AllocateUid (name);
  NS_ASSERT (uid != 0);
  m_iid = uid;
}


InterfaceId::InterfaceId (uint16_t iid)
  : m_iid (iid)
{}
InterfaceId::~InterfaceId ()
{}
InterfaceId 
InterfaceId::LookupByName (std::string name)
{
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (name);
  NS_ASSERT (uid != 0);
  return InterfaceId (uid);
}
uint32_t 
InterfaceId::GetRegisteredN (void)
{
  return Singleton<IidManager>::Get ()->GetRegisteredN ();
}
InterfaceId 
InterfaceId::GetRegistered (uint32_t i)
{
  return InterfaceId (Singleton<IidManager>::Get ()->GetRegistered (i));
}

InterfaceId 
InterfaceId::SetParent (InterfaceId iid)
{
  Singleton<IidManager>::Get ()->SetParent (m_iid, iid.m_iid);
  return *this;
}
InterfaceId 
InterfaceId::GetParent (void) const
{
  uint16_t parent = Singleton<IidManager>::Get ()->GetParent (m_iid);
  return InterfaceId (parent);
}
std::string 
InterfaceId::GetName (void) const
{
  std::string name = Singleton<IidManager>::Get ()->GetName (m_iid);
  return name;
}

bool 
InterfaceId::HasConstructor (void) const
{
  bool hasConstructor = Singleton<IidManager>::Get ()->HasConstructor (m_iid);
  return hasConstructor;
}

void
InterfaceId::DoAddConstructor (CallbackBase cb, uint32_t nArguments)
{
  Singleton<IidManager>::Get ()->AddConstructor (m_iid, cb, nArguments);
}

CallbackBase
InterfaceId::LookupConstructor (uint32_t nArguments)
{
  CallbackBase constructor = Singleton<IidManager>::Get ()->GetConstructor (m_iid, nArguments);
  return constructor;
}

Ptr<Object> 
InterfaceId::CreateObject (void)
{
  CallbackBase cb = LookupConstructor (0);
  Callback<Ptr<Object> > realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb ();
  return object;
}

bool operator == (InterfaceId a, InterfaceId b)
{
  return a.m_iid == b.m_iid;
}

bool operator != (InterfaceId a, InterfaceId b)
{
  return a.m_iid != b.m_iid;
}

/*********************************************************************
 *         The Object implementation
 *********************************************************************/

static InterfaceId
GetObjectIid (void)
{
  InterfaceId iid = InterfaceId ("Object");
  iid.SetParent (iid);
  return iid;
}

InterfaceId 
Object::iid (void)
{
  static InterfaceId iid = GetObjectIid ();
  return iid;
}


Object::Object ()
  : m_count (1),
    m_iid (Object::iid ()),
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
  NS_ASSERT (CheckLoose ());
  const Object *currentObject = this;
  do {
    NS_ASSERT (currentObject != 0);
    InterfaceId cur = currentObject->m_iid;
    while (cur != iid && cur != Object::iid ())
      {
        cur = cur.GetParent ();
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
Object::TraceConnect (std::string path, const CallbackBase &cb) const
{
  NS_ASSERT (CheckLoose ());
  GetTraceResolver ()->Connect (path, cb, TraceContext ());
}
void 
Object::TraceDisconnect (std::string path, const CallbackBase &cb) const
{
  NS_ASSERT (CheckLoose ());
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
  NS_ASSERT (CheckLoose ());
  Ptr<InterfaceIdTraceResolver> resolver =
    Create<InterfaceIdTraceResolver> (this);
  return resolver;
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
      NS_LOG_LOGIC ("collect current=" << current);
      InterfaceId cur = current->m_iid;
      while (cur != Object::iid ())
        {
          std::string name = cur.GetName ();
          std::string fullpath = path;
          fullpath.append ("/$");
          fullpath.append (name);
          NS_LOG_LOGIC("collect: " << fullpath);
          current->GetTraceResolver ()->CollectSources (fullpath, context, collection);
          cur = cur.GetParent ();
        }
      current = current->m_next;
    }

  m_collecting = false;
}
void 
Object::DoTraceAll (std::ostream &os, const TraceContext &context) const
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
      current->GetTraceResolver ()->TraceAll (os, context);
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
  static ns3::InterfaceId iid (void) {
    static ns3::InterfaceId iid = ns3::InterfaceId ("BaseA")
      .SetParent (Object::iid ())
      .AddConstructor<BaseA> ();
    return iid;
  }
  BaseA ()
  {}
  void BaseGenerateTrace (int16_t v)
  { m_source = v; }
  virtual void Dispose (void) {}
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("basea-x", ns3::TraceDoc ("test source"), m_source);
    resolver->SetParentResolver (Object::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_source;
};

class DerivedA : public BaseA
{
public:
  static ns3::InterfaceId iid (void) {
    static ns3::InterfaceId iid = ns3::InterfaceId ("DerivedA")
      .SetParent (BaseA::iid ())
      .AddConstructor<DerivedA,int> ();
    return iid;
  }
  DerivedA (int v)
  {}
  void DerivedGenerateTrace (int16_t v)
  { m_sourceDerived = v; }
  virtual void Dispose (void) {
    BaseA::Dispose ();
  }
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("deriveda-x", ns3::TraceDoc ("test source"), m_sourceDerived);
    resolver->SetParentResolver (BaseA::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_sourceDerived;
};

class BaseB : public ns3::Object
{
public:
  static ns3::InterfaceId iid (void) {
    static ns3::InterfaceId iid = ns3::InterfaceId ("BaseB")
      .SetParent (Object::iid ())
      .AddConstructor<BaseB> ();
    return iid;
  }
  BaseB ()
  {}
  void BaseGenerateTrace (int16_t v)
  { m_source = v; }
  virtual void Dispose (void) {}
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("baseb-x", ns3::TraceDoc ("test source"), m_source);
    resolver->SetParentResolver (Object::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_source;
};

class DerivedB : public BaseB
{
public:
  static ns3::InterfaceId iid (void) {
    static ns3::InterfaceId iid = ns3::InterfaceId ("DerivedB")
      .SetParent (BaseB::iid ())
      .AddConstructor<DerivedB,int> ()
      .AddConstructor<DerivedB,int,int &> ();
    return iid;
  }
  DerivedB (int v)
  {}
  DerivedB (int v1, int &v2)
  {}
  void DerivedGenerateTrace (int16_t v)
  { m_sourceDerived = v; }
  virtual void Dispose (void) {
    BaseB::Dispose ();
  }
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("derivedb-x", ns3::TraceDoc ("test source"), m_sourceDerived);
    resolver->SetParentResolver (BaseB::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_sourceDerived;
};

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

  Ptr<BaseA> baseA = CreateObject<BaseA> ();
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (DerivedA::iid ()), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedA> (), 0);
  baseA = CreateObject<DerivedA> (10);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (DerivedA::iid ()), baseA);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<DerivedA> (), 0);

  baseA = CreateObject<BaseA> ();
  Ptr<BaseB> baseB = CreateObject<BaseB> ();
  Ptr<BaseB> baseBCopy = baseB;
  baseA->AddInterface (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseB> (), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseB> (), 0);
  NS_TEST_ASSERT_EQUAL (baseB->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_EQUAL (baseB->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<BaseA> (), 0);

  baseA = CreateObject<DerivedA> (1);
  baseB = CreateObject<DerivedB> (1);
  baseBCopy = baseB;
  baseA->AddInterface (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseB> (), 0)

  baseA = CreateObject<BaseA> ();
  baseB = CreateObject<BaseB> ();
  baseA->AddInterface (baseB);
  baseA = 0;
  baseA = baseB->QueryInterface<BaseA> ();

  baseA = CreateObject<BaseA> ();
  baseA->TraceConnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (1);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseB = CreateObject<BaseB> ();
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
  derivedA = CreateObject<DerivedA> (1);
  baseB = CreateObject<BaseB> ();
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

  // Test the object creation code of InterfaceId
  Ptr<Object> a = BaseA::iid ().CreateObject ();
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (), a);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (DerivedA::iid ()), 0);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<DerivedA> (), 0);
  a = DerivedA::iid ().CreateObject (10);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (), a);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (DerivedA::iid ()), a);
  NS_TEST_ASSERT_UNEQUAL (a->QueryInterface<DerivedA> (), 0);


  return result;
}

static ObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


