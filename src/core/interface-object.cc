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


const MyInterfaceId InterfaceObject::iid = MakeObjectInterfaceId ();


InterfaceObject::InterfaceObject ()
  : m_count (1),
    m_iid (InterfaceObject::iid),
    m_next (this)
{}
InterfaceObject::~InterfaceObject () 
{}
Ptr<InterfaceObject>
InterfaceObject::DoQueryInterface (MyInterfaceId iid)
{
  NS_ASSERT (Check ());
  InterfaceObject *currentObject = this;
  do {
    NS_ASSERT (currentObject != 0);
    MyInterfaceId cur = currentObject->m_iid;
    while (cur != iid && cur != InterfaceObject::iid)
      {
        cur = MyInterfaceId::LookupParent (cur);
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
InterfaceObject::Dispose (void)
{
  NS_ASSERT (Check ());
  InterfaceObject *current = this;
  do {
    NS_ASSERT (current != 0);
    current->DoDispose ();
    current = current->m_next;
  } while (current != this);
}

void 
InterfaceObject::Add (Ptr<InterfaceObject> o)
{
  NS_ASSERT (Check ());
  NS_ASSERT (o->Check ());
  InterfaceObject *other = PeekPointer (o);
  InterfaceObject *next = m_next;
  m_next = other->m_next;
  other->m_next = next;
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
  return (m_count > 0);
}

void
InterfaceObject::MaybeDelete (void)
{
  // First, check if any of the attached
  // Object has a non-zero count.
  InterfaceObject *current = this;
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
  InterfaceObject *end = this;
  do {
    NS_ASSERT (current != 0);
    InterfaceObject *next = current->m_next;
    delete current;
    current = next;
  } while (current != end);
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

  baseA = MakeNewObject<BaseA> ();
  baseB = MakeNewObject<BaseB> ();
  baseA->Add (baseB);
  baseA = 0;
  baseA = baseB->QueryInterface<BaseA> (BaseA::iid);

  return ok;
}

static InterfaceObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


