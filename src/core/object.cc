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

InterfaceId::InterfaceId (uint32_t iid)
  : m_iid (iid)
{}
InterfaceId::~InterfaceId ()
{}
InterfaceId 
InterfaceId::LookupByName (std::string name)
{
  return InterfaceId (Singleton<IidManager>::Get ()->LookupByName (name));
}
InterfaceId 
InterfaceId::LookupParent (InterfaceId iid)
{
  return Singleton<IidTree>::Get ()->LookupParent (iid.m_iid);
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
  InterfaceId iid = Singleton<IidManager>::Get ()->Allocate (name);
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
    m_next (this)
{}
Object::~Object () 
{
  m_next = 0;
}
Ptr<Object>
Object::DoQueryInterface (InterfaceId iid)
{
  NS_ASSERT (Check ());
  Object *currentObject = this;
  do {
    NS_ASSERT (currentObject != 0);
    InterfaceId cur = currentObject->m_iid;
    while (cur != iid && cur != Object::iid)
      {
        cur = InterfaceId::LookupParent (cur);
      }
    if (cur == iid)
      {
        return currentObject;
      }
    currentObject = currentObject->m_next;
  } while (currentObject != this);
  return 0;
}
void 
Object::Dispose (void)
{
  NS_ASSERT (Check ());
  Object *current = this;
  do {
    NS_ASSERT (current != 0);
    current->DoDispose ();
    current = current->m_next;
  } while (current != this);
}

void 
Object::AddInterface (Ptr<Object> o)
{
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
Object::SetInterfaceId (InterfaceId iid)
{
  NS_ASSERT (Check ());
  m_iid = iid;
}

void
Object::DoDispose (void)
{
  NS_ASSERT (Check ());
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

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "test.h"

namespace {

class BaseA : public ns3::Object
{
public:
  static const ns3::InterfaceId iid;
  BaseA ()
  {
    SetInterfaceId (BaseA::iid);
  }
  virtual void Dispose (void) {}
};

class DerivedA : public BaseA
{
public:
  static const ns3::InterfaceId iid;
  DerivedA (int v)
  {
    SetInterfaceId (DerivedA::iid);
  }
  virtual void Dispose (void) {
    BaseA::Dispose ();
  }
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
  virtual void Dispose (void) {}
};

class DerivedB : public BaseB
{
public:
  static const ns3::InterfaceId iid;
  DerivedB (int v)
  {
    SetInterfaceId (DerivedB::iid);
  }
  virtual void Dispose (void) {
    BaseB::Dispose ();
  }
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
};

ObjectTest::ObjectTest ()
  : Test ("Object")
{}
bool 
ObjectTest::RunTests (void)
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
  baseA->AddInterface (baseB);
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
  baseA->AddInterface (baseB);
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

  baseA = MakeNewObject<BaseA> ();
  baseB = MakeNewObject<BaseB> ();
  baseA->AddInterface (baseB);
  baseA = 0;
  baseA = baseB->QueryInterface<BaseA> (BaseA::iid);

  return ok;
}

static ObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


