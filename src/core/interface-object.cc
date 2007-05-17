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
#include "interface-object.h"
#include "assert.h"
#include "singleton.h"
#include "uid-manager.h"
#include <vector>

namespace {

class IidManager : public ns3::UidManager
{};

class IidTree
{
public:
  void SetParent (uint32_t child, const uint32_t *parent);
  uint32_t LookupParent (uint32_t child);
private:
  std::vector<const uint32_t *> m_parents;
};

void 
IidTree::SetParent (uint32_t child, const uint32_t *parent)
{
  m_parents.resize (child+1);
  m_parents[child] = parent;
}
uint32_t 
IidTree::LookupParent (uint32_t child)
{
  return *(m_parents[child]);
}

} // anonymous namespace

namespace ns3 {

MyInterfaceId::MyInterfaceId (uint32_t iid)
  : m_iid (iid)
{}
MyInterfaceId 
MyInterfaceId::LookupByName (std::string name)
{
  return MyInterfaceId (Singleton<IidManager>::Get ()->LookupByName (name));
}
MyInterfaceId 
MyInterfaceId::LookupParent (MyInterfaceId iid)
{
  return Singleton<IidTree>::Get ()->LookupParent (iid.m_iid);
}

bool operator == (const MyInterfaceId &a, const MyInterfaceId &b)
{
  return a.m_iid == b.m_iid;
}

bool operator != (const MyInterfaceId &a, const MyInterfaceId &b)
{
  return a.m_iid != b.m_iid;
}

MyInterfaceId
MakeInterfaceId (std::string name, const MyInterfaceId &parent)
{
  MyInterfaceId iid = Singleton<IidManager>::Get ()->Allocate (name);
  Singleton<IidTree>::Get ()->SetParent (iid.m_iid, &parent.m_iid);
  return iid;
}

MyInterfaceId
MakeObjectInterfaceId (void)
{
  MyInterfaceId iid = Singleton<IidManager>::Get ()->Allocate ("Object");
  Singleton<IidTree>::Get ()->SetParent (iid.m_iid, &iid.m_iid);
  return iid;
}


class AggregateObject
{
public:
  AggregateObject ();
  ~AggregateObject ();
  void Ref (void);
  void Ref (AggregateObject *other);
  void RefAll (uint32_t count);
  void Unref (void);
  void UnrefAll (void);
  InterfaceObject *PeekQueryInterface (MyInterfaceId iid, InterfaceObject *caller);
  void Dispose (void);
  void AddObject (InterfaceObject *object);
  void Swallow (AggregateObject *object);
private:
  typedef std::vector<InterfaceObject *> ObjectList;
  uint32_t m_count;
  ObjectList m_objectList;
};

AggregateObject::AggregateObject ()
  : m_count (1)
{}
AggregateObject::~AggregateObject ()
{
  NS_ASSERT (m_count == 0);
}
void 
AggregateObject::Ref (void)
{
  m_count++;
}
void 
AggregateObject::Ref (AggregateObject *other)
{
  m_count+= other->m_count;
}
void 
AggregateObject::RefAll (uint32_t count)
{
  m_count += count;
}
void
AggregateObject::UnrefAll (void)
{
  m_count = 0;
  delete this;
}
void 
AggregateObject::Unref (void)
{
  m_count--;
  if (m_count == 0)
    {
      for (ObjectList::iterator i = m_objectList.begin (); i != m_objectList.end (); i++)
	{
	  delete *i;
	}
      delete this;
    }
}
InterfaceObject *
AggregateObject::PeekQueryInterface (MyInterfaceId iid, InterfaceObject *caller)
{
  for (ObjectList::iterator i = m_objectList.begin (); i != m_objectList.end (); i++)
    {
      if (*i != caller)
	{
	  MyInterfaceId cur = (*i)->m_iid;
	  while (cur != iid && cur != InterfaceObject::iid)
	    {
	      cur = MyInterfaceId::LookupParent (cur);
	    }
	  if (cur == iid)
	    {
	      return *i;
	    }
	}
    }
  return 0;
}
void 
AggregateObject::Dispose (void)
{
  for (ObjectList::iterator i = m_objectList.begin (); i != m_objectList.end (); i++)
    {
      (*i)->DoDispose ();
    }
}
void 
AggregateObject::AddObject (InterfaceObject *object)
{
  // XXX should check for interface uniqueness.
  m_objectList.push_back (object);
}

void
AggregateObject::Swallow (AggregateObject *other)
{
  for (ObjectList::iterator i = other->m_objectList.begin (); i != other->m_objectList.end (); i++)
    {
      m_objectList.push_back (*i);
    }
  other->m_objectList.clear ();
}

const MyInterfaceId InterfaceObject::iid = MakeObjectInterfaceId ();


InterfaceObject::InterfaceObject ()
  : m_count (1),
    m_iid (InterfaceObject::iid),
    m_aggregate (0)
{}
InterfaceObject::~InterfaceObject () 
{}
void 
InterfaceObject::Ref (void)
{
  NS_ASSERT (Check ());
  if (m_aggregate != 0)
    {
      NS_ASSERT (m_count == 0);
      m_aggregate->Ref ();
    }
  else
    {
      m_count++;
    }
}
void 
InterfaceObject::Unref (void)
{
  NS_ASSERT (Check ());
  if (m_aggregate != 0)
    {
      NS_ASSERT (m_count == 0);
      m_aggregate->Unref ();
    }
  else
    {
      m_count--;
      if (m_count == 0)
	{
	  delete this;
	}
    }
}
Ptr<InterfaceObject>
InterfaceObject::DoQueryInterface (MyInterfaceId iid)
{
  NS_ASSERT (Check ());
  MyInterfaceId cur = m_iid;
  while (cur != iid && cur != InterfaceObject::iid)
    {
      cur = MyInterfaceId::LookupParent (cur);
    }
  if (cur == iid)
    {
      return this;
    }
  if (m_aggregate != 0)
    {
      NS_ASSERT (m_count == 0);
      return m_aggregate->PeekQueryInterface (iid, this);
    }
  return 0;
}
void 
InterfaceObject::Dispose (void)
{
  NS_ASSERT (Check ());
  if (m_aggregate != 0)
    {
      NS_ASSERT (m_count == 0);
      m_aggregate->Dispose ();
    }
  else
    {
      DoDispose ();
    }
}

void 
InterfaceObject::Add (Ptr<InterfaceObject> o)
{
  NS_ASSERT (Check ());
  NS_ASSERT (o->Check ());
  InterfaceObject *other = PeekPointer (o);
  if (m_aggregate != 0 && other->m_aggregate != 0)
    {
      NS_ASSERT (m_count == 0);
      NS_ASSERT (other->m_count == 0);
      m_aggregate->Swallow (other->m_aggregate);
      m_aggregate->Ref (other->m_aggregate);
      other->m_aggregate->UnrefAll ();
      other->m_aggregate = m_aggregate;
    }
  else if (m_aggregate != 0)
    {
      NS_ASSERT (other->m_aggregate == 0);
      NS_ASSERT (m_count == 0);
      m_aggregate->AddObject (other);
      other->m_aggregate = m_aggregate;
      other->m_aggregate->RefAll (other->m_count);
      other->m_count = 0;
    }
  else if (other->m_aggregate != 0)
    {
      NS_ASSERT (m_aggregate == 0);
      other->m_aggregate->AddObject (this);
      m_aggregate = other->m_aggregate;
      m_aggregate->RefAll (m_count);
      m_count = 0;
    }
  else 
    {
      NS_ASSERT (m_aggregate == 0);
      NS_ASSERT (other->m_aggregate == 0);
      m_aggregate = new AggregateObject ();
      other->m_aggregate = m_aggregate;
      m_aggregate->AddObject (this);
      m_aggregate->AddObject (other);
      m_aggregate->RefAll (m_count);
      m_aggregate->RefAll (other->m_count);
      m_aggregate->Unref ();
      other->m_count = 0;
      m_count = 0;
    }
  NS_ASSERT (Check ());
  NS_ASSERT (o->Check ());
}

void 
InterfaceObject::SetInterfaceId (MyInterfaceId iid)
{
  NS_ASSERT (Check ());
  m_iid = iid;
}

void
InterfaceObject::DoDispose (void)
{
  NS_ASSERT (Check ());
}

bool 
InterfaceObject::Check (void)
{
  return (m_count == 0 && m_aggregate != 0) || (m_count != 0 && m_aggregate == 0);
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "test.h"

namespace {

class BaseA : public ns3::InterfaceObject
{
public:
  static const ns3::MyInterfaceId iid;
  BaseA ()
  {
    SetInterfaceId (BaseA::iid);
  }
  virtual void Dispose (void) {}
};

class DerivedA : public BaseA
{
public:
  static const ns3::MyInterfaceId iid;
  DerivedA (int v)
  {
    SetInterfaceId (DerivedA::iid);
  }
  virtual void Dispose (void) {
    BaseA::Dispose ();
  }
};

const ns3::MyInterfaceId BaseA::iid = 
  ns3::MakeInterfaceId ("BaseA", InterfaceObject::iid);
const ns3::MyInterfaceId DerivedA::iid = 
  ns3::MakeInterfaceId ("DerivedA", BaseA::iid);;

class BaseB : public ns3::InterfaceObject
{
public:
  static const ns3::MyInterfaceId iid;
  BaseB ()
  {
    SetInterfaceId (BaseB::iid);
  }
  virtual void Dispose (void) {}
};

class DerivedB : public BaseB
{
public:
  static const ns3::MyInterfaceId iid;
  DerivedB (int v)
  {
    SetInterfaceId (DerivedB::iid);
  }
  virtual void Dispose (void) {
    BaseB::Dispose ();
  }
};

const ns3::MyInterfaceId BaseB::iid = 
  ns3::MakeInterfaceId ("BaseB", InterfaceObject::iid);
const ns3::MyInterfaceId DerivedB::iid = 
  ns3::MakeInterfaceId ("DerivedB", BaseB::iid);;

} // namespace anonymous

namespace ns3 {

class InterfaceObjectTest : public Test
{
public:
  InterfaceObjectTest ();
  virtual bool RunTests (void);
};

InterfaceObjectTest::InterfaceObjectTest ()
  : Test ("InterfaceObject")
{}
bool 
InterfaceObjectTest::RunTests (void)
{
  bool ok = true;

  Ptr<BaseA> baseA = MakeNewObject<BaseA> ();
  if (baseA->QueryInterface<BaseA> (BaseA::iid) != baseA)
    {
      ok = false;
    }
  if (baseA->QueryInterface<BaseA> (DerivedA::iid) != 0)
    {
      ok = false;
    }
  if (baseA->QueryInterface<DerivedA> (DerivedA::iid) != 0)
    {
      ok = false;
    }
  baseA = MakeNewObject<DerivedA> (10);
  if (baseA->QueryInterface<BaseA> (BaseA::iid) != baseA)
    {
      ok = false;
    }
  if (baseA->QueryInterface<BaseA> (DerivedA::iid) != baseA)
    {
      ok = false;
    }
  if (baseA->QueryInterface<DerivedA> (DerivedA::iid) == 0)
    {
      ok = false;
    }

  baseA = MakeNewObject<BaseA> ();
  Ptr<BaseB> baseB = MakeNewObject<BaseB> ();
  Ptr<BaseB> baseBCopy = baseB;
  baseA->Add (baseB);
  if (baseA->QueryInterface<BaseA> (BaseA::iid) == 0)
    {
      ok = false;
    }
  if (baseA->QueryInterface<DerivedA> (DerivedA::iid) != 0)
    {
      ok = false;
    }
  if (baseA->QueryInterface<BaseB> (BaseB::iid) == 0)
    {
      ok = false;
    }
  if (baseA->QueryInterface<DerivedB> (DerivedB::iid) != 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<BaseB> (BaseB::iid) == 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<DerivedB> (DerivedB::iid) != 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<BaseA> (BaseA::iid) == 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<DerivedA> (DerivedA::iid) != 0)
    {
      ok = false;
    }
  if (baseBCopy->QueryInterface<BaseA> (BaseA::iid) == 0)
    {
      ok = false;
    }

  baseA = MakeNewObject<DerivedA> (1);
  baseB = MakeNewObject<DerivedB> (1);
  baseBCopy = baseB;
  baseA->Add (baseB);
  if (baseA->QueryInterface<DerivedB> (DerivedB::iid) == 0)
    {
      ok = false;
    }
  if (baseA->QueryInterface<BaseB> (BaseB::iid) == 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<DerivedA> (DerivedA::iid) == 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<BaseA> (BaseA::iid) == 0)
    {
      ok = false;
    }
  if (baseBCopy->QueryInterface<DerivedA> (DerivedA::iid) == 0)
    {
      ok = false;
    }
  if (baseBCopy->QueryInterface<BaseA> (BaseA::iid) == 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<DerivedB> (DerivedB::iid) == 0)
    {
      ok = false;
    }
  if (baseB->QueryInterface<BaseB> (BaseB::iid) == 0)
    {
      ok = false;
    }


  return ok;
}

static InterfaceObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


