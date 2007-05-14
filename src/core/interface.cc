/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "interface.h"
#include "singleton.h"
#include "uid-manager.h"
#include <string>
#include <list>
#include <stdint.h>
#include "assert.h"
#include "debug.h"

NS_DEBUG_COMPONENT_DEFINE ("Interface");

namespace ns3 {

class IidManager : public UidManager
{};

InterfaceId::InterfaceId (std::string name)
  : m_iid (Singleton<IidManager>::Get ()->Allocate (name))
{}

InterfaceId::InterfaceId (uint32_t iid)
  : m_iid (iid)
{}

InterfaceId 
InterfaceId::LookupByName (std::string name)
{
  return InterfaceId (Singleton<IidManager>::Get ()->LookupByName (name));
}

bool operator == (const InterfaceId &a, const InterfaceId &b)
{
  return a.m_iid == b.m_iid;
}


class InterfaceImpl
{
public:
  InterfaceImpl (InterfaceId iid, Interface *interface);
  ~InterfaceImpl ();
  void Ref (void);
  void RefAll (InterfaceImpl *other);
  void Unref (void);
  void UnrefAll (void);
  Interface *PeekQueryInterface (InterfaceId iid) const;
  void DoDisposeAll (void);
  void AddInterface (Interface * interface);
  void AddSelfInterface (InterfaceId iid, Interface *interface);
private:
  typedef std::list<std::pair<InterfaceId,Interface *> > List;
  uint32_t m_ref;
  List m_list;
  bool m_disposed;
};

InterfaceImpl::InterfaceImpl (InterfaceId iid, Interface * interface)
  : m_ref (1),
    m_disposed (false)
{
  NS_DEBUG ("new " << this << " ref=" << m_ref);
  m_list.push_back (std::make_pair (iid, interface));
}
InterfaceImpl::~InterfaceImpl ()
{
  for (List::const_iterator i = m_list.begin ();
       i != m_list.end (); i++)
    {
      i->second->UnrefInternal ();
    }
  m_list.clear ();
}
void 
InterfaceImpl::Ref (void)
{
  m_ref++;
  NS_DEBUG ("inc " << this << " ref=" << m_ref);
}
void 
InterfaceImpl::RefAll (InterfaceImpl *other)
{
  m_ref += other->m_ref;
  NS_DEBUG ("inc all " << this << " o=" << other->m_ref << " ref=" << m_ref);
}
void 
InterfaceImpl::Unref (void)
{
  NS_ASSERT (m_ref > 0);
  m_ref--;
  NS_DEBUG ("dec " << this << " ref=" << m_ref);
  if (m_ref == 0)
    {
      delete this;
    }
}
void
InterfaceImpl::UnrefAll (void)
{
  NS_ASSERT (m_ref > 0);
  m_ref = 0;
  delete this;
  NS_DEBUG ("dec all " << this);
}
void
InterfaceImpl::DoDisposeAll (void)
{
  NS_ASSERT (!m_disposed);
  for (List::const_iterator i = m_list.begin ();
       i != m_list.end (); i++)
    {
      Interface *interface = i->second;
      interface->DoDispose ();
    }
  m_disposed = true;
}
Interface *
InterfaceImpl::PeekQueryInterface (InterfaceId iid) const
{
  for (List::const_iterator i = m_list.begin ();
       i != m_list.end (); i++)
    {
      if (i->first == iid)
	{
	  return i->second;
	}
    }
  return 0;
}
void 
InterfaceImpl::AddInterface (Interface *interface)
{
  for (List::const_iterator i = interface->m_impl->m_list.begin ();
       i != interface->m_impl->m_list.end (); i++)
    {
      // XXX here, we should check that we have only one 
      // instance of each interface
      i->second->RefInternal ();
      m_list.push_back (std::make_pair (i->first, i->second));
    }
}
void 
InterfaceImpl::AddSelfInterface (InterfaceId iid, Interface *interface)
{
  interface->RefInternal ();
  m_list.push_back (std::make_pair (iid, interface));
}


Interface::Interface (InterfaceId iid)
  : m_impl (new InterfaceImpl (iid, this)),
    m_ref (1)
{}
Interface::~Interface ()
{
  m_impl = 0;
  m_ref = -1;
}
void 
Interface::Ref (void) const
{
  m_impl->Ref ();
}
void 
Interface::Unref (void) const
{
  m_impl->Unref ();
}

void 
Interface::Dispose (void)
{
  m_impl->DoDisposeAll ();
}

void 
Interface::DoDispose (void)
{
  // we do not do anything by default.
}

void
Interface::RefInternal (void)
{
  m_ref++;
}

void
Interface::UnrefInternal (void)
{
  NS_ASSERT (m_ref != 0);
  m_ref--;
  if (m_ref == 0)
    {
      delete this;
    }
}

Ptr<Interface>
Interface::DoQueryInterface (InterfaceId iid) const
{
  return m_impl->PeekQueryInterface (iid);
}

void 
Interface::AddInterface (Ptr<Interface> interface)
{
  Interface *p = PeekPointer (interface);
  m_impl->AddInterface (p);
  m_impl->RefAll (p->m_impl);
  p->m_impl->UnrefAll ();
  p->m_impl = m_impl;
}

void
Interface::AddSelfInterface (InterfaceId iid, Ptr<Interface> interface)
{
  m_impl->AddSelfInterface (iid, PeekPointer (interface));
}


}//namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"

namespace {

class A : public ns3::Interface
{
public:
  static const ns3::InterfaceId iid;
  A ()
    : Interface (A::iid)
  {}
};
class B : public ns3::Interface
{
public:
  static const ns3::InterfaceId iid;
  B ()
    : Interface (B::iid)
  {}
};
class BaseA : public ns3::Interface
{
public:
  static const ns3::InterfaceId iid;
  BaseA ()
    : Interface (BaseA::iid)
  {}
};
class BaseB : public ns3::Interface
{
public:
  static const ns3::InterfaceId iid;
  BaseB ()
    : Interface (BaseB::iid)
  {}
};
class Base : public ns3::Interface
{
public:
  static const ns3::InterfaceId iid;
  Base ()
    : Interface (Base::iid)
  {}
};
class Derived : public Base
{
public:
  static const ns3::InterfaceId iid;
  Derived ()
  {
    AddSelfInterface (Derived::iid, this);
  }
};

const ns3::InterfaceId A::iid ("A");
const ns3::InterfaceId B::iid ("B");
const ns3::InterfaceId BaseA::iid ("BaseA");
const ns3::InterfaceId BaseB::iid ("BaseB");
const ns3::InterfaceId Base::iid ("Base");
const ns3::InterfaceId Derived::iid ("Derived");

}//namespace


namespace ns3 {

class InterfaceTest : public Test
{
public:
  InterfaceTest ();
  virtual bool RunTests (void);
};

InterfaceTest::InterfaceTest ()
  : Test ("Interface")
{}
bool 
InterfaceTest::RunTests (void)
{
  bool ok = true;

  //DerivedAB *derivedAB;


  Ptr<A> a = MakeNewObject<A> ();

  a = MakeNewObject<A> ();
  Ptr<A> a1 = a->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  a1 = a->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }

  Ptr<B> b = MakeNewObject<B> ();
  Ptr<B> b1 = b->QueryInterface<B> (B::iid);
  if (b1 == 0 || b1 != b)
    {
      ok = false;
    }
  
  a = MakeNewObject<A> ();
  a->AddInterface (b);
  b1 = b->QueryInterface<B> (B::iid);
  if (b1 == 0 || b1 != b)
    {
      ok = false;
    }
  a1 = b->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  a1 = a->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  b1 = a->QueryInterface<B> (B::iid);
  if (b1 == 0 || b1 != b)
    {
      ok = false;
    }

  Ptr<Derived> derived = MakeNewObject<Derived> ();
  Ptr<Base> base = derived->QueryInterface<Base> (Base::iid);
  if (base == 0)
    {
      ok = false;
    }
  Ptr<Derived> derived1 = base->QueryInterface<Derived> (Derived::iid);
  if (derived1 == 0 || derived1 != derived)
    {
      ok = false;
    }

  // the following cannot work and it is on purpose
  // delete derived;

  return ok;
}


static InterfaceTest g_interface_test;

}// namespace ns3

#endif /* RUN_SELF_TESTS */
