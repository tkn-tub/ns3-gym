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
#include "ns-unknown.h"
#include "singleton.h"
#include "uid-manager.h"
#include <string>
#include <list>
#include <stdint.h>
#include "assert.h"
#include "debug.h"

NS_DEBUG_COMPONENT_DEFINE ("NsUnknown");

namespace ns3 {

class IidManager : public UidManager
{};

Iid::Iid (std::string name)
  : m_iid (Singleton<IidManager>::Get ()->Allocate (name))
{}

bool operator == (const Iid &a, const Iid &b)
{
  return a.m_iid == b.m_iid;
}


class NsUnknownImpl
{
public:
  NsUnknownImpl (Iid iid, NsUnknown *interface);
  ~NsUnknownImpl ();
  void Ref (void);
  void RefAll (NsUnknownImpl *other);
  void Unref (void);
  void UnrefAll (void);
  NsUnknown *DoQueryInterface (Iid iid) const;
  void DoDisposeAll (void);
  void AddInterface (NsUnknown *interface);
  void AddSelfInterface (Iid iid, NsUnknown *interface);
private:
  typedef std::list<std::pair<Iid,NsUnknown *> > List;
  uint32_t m_ref;
  List m_list;
  bool m_disposed;
};

NsUnknownImpl::NsUnknownImpl (Iid iid, NsUnknown *interface)
  : m_ref (1),
    m_disposed (false)
{
  m_list.push_back (std::make_pair (iid, interface));
}
NsUnknownImpl::~NsUnknownImpl ()
{
  for (List::const_iterator i = m_list.begin ();
       i != m_list.end (); i++)
    {
      i->second->UnrefInternal ();
    }
  m_list.clear ();
}
void 
NsUnknownImpl::Ref (void)
{
  m_ref++;
  NS_DEBUG ("inc " << this << " ref=" << m_ref);
}
void 
NsUnknownImpl::RefAll (NsUnknownImpl *other)
{
  m_ref += other->m_ref;
}
void 
NsUnknownImpl::Unref (void)
{
  m_ref--;
  NS_DEBUG ("dec " << this << " ref=" << m_ref);
  if (m_ref == 0)
    {
      delete this;
    }
}
void
NsUnknownImpl::UnrefAll (void)
{
  m_ref = 0;
  delete this;
}
void
NsUnknownImpl::DoDisposeAll (void)
{
  NS_ASSERT (!m_disposed);
  for (List::const_iterator i = m_list.begin ();
       i != m_list.end (); i++)
    {
      NsUnknown *interface = i->second;
      interface->DoDispose ();
    }
  m_disposed = true;
}
NsUnknown *
NsUnknownImpl::DoQueryInterface (Iid iid) const
{
  for (List::const_iterator i = m_list.begin ();
       i != m_list.end (); i++)
    {
      if (i->first == iid)
	{
	  i->second->Ref ();
	  return i->second;
	}
    }
  return 0;
}
void 
NsUnknownImpl::AddInterface (NsUnknown *interface)
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
NsUnknownImpl::AddSelfInterface (Iid iid, NsUnknown *interface)
{
  interface->RefInternal ();
  m_list.push_back (std::make_pair (iid, interface));
}


NsUnknown::NsUnknown (Iid iid)
  : m_impl (new NsUnknownImpl (iid, this)),
    m_ref (1)
{}
NsUnknown::~NsUnknown ()
{
  m_impl = 0;
}
void 
NsUnknown::Ref (void)
{
  m_impl->Ref ();
}
void 
NsUnknown::Unref (void)
{
  m_impl->Unref ();
}

void 
NsUnknown::Dispose (void)
{
  m_impl->DoDisposeAll ();
}

void 
NsUnknown::DoDispose (void)
{
  // we do not do anything by default.
}

void
NsUnknown::RefInternal (void)
{
  m_ref++;
}

void
NsUnknown::UnrefInternal (void)
{
  NS_ASSERT (m_ref != 0);
  m_ref--;
  if (m_ref == 0)
    {
      delete this;
    }
}

NsUnknown *
NsUnknown::DoQueryInterface (Iid iid) const
{
  return m_impl->DoQueryInterface (iid);
}

void 
NsUnknown::AddInterface (NsUnknown *interface)
{
  m_impl->AddInterface (interface);
  m_impl->RefAll (interface->m_impl);
  interface->m_impl->UnrefAll ();
  interface->m_impl = m_impl;
}

void
NsUnknown::AddSelfInterface (Iid iid, NsUnknown *interface)
{
  m_impl->AddSelfInterface (iid, interface);
}


}//namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"

namespace {

class A : public ns3::NsUnknown
{
public:
  static const ns3::Iid iid;
  A ()
    : NsUnknown (A::iid)
  {}
};
class B : public ns3::NsUnknown
{
public:
  static const ns3::Iid iid;
  B ()
    : NsUnknown (B::iid)
  {}
};
class BaseA : public ns3::NsUnknown
{
public:
  static const ns3::Iid iid;
  BaseA ()
    : NsUnknown (BaseA::iid)
  {}
};
class BaseB : public ns3::NsUnknown
{
public:
  static const ns3::Iid iid;
  BaseB ()
    : NsUnknown (BaseB::iid)
  {}
};
class Base : public ns3::NsUnknown
{
public:
  static const ns3::Iid iid;
  Base ()
    : NsUnknown (Base::iid)
  {}
};
class Derived : public Base
{
public:
  static const ns3::Iid iid;
  Derived ()
  {
    AddSelfInterface (Derived::iid, this);
  }
};

const ns3::Iid A::iid ("A");
const ns3::Iid B::iid ("B");
const ns3::Iid BaseA::iid ("BaseA");
const ns3::Iid BaseB::iid ("BaseB");
const ns3::Iid Base::iid ("Base");
const ns3::Iid Derived::iid ("Derived");

}//namespace


namespace ns3 {

class InterfaceTest : public Test
{
public:
  InterfaceTest ();
  virtual bool RunTests (void);
};

InterfaceTest::InterfaceTest ()
  : Test ("NsUnknown")
{}
bool 
InterfaceTest::RunTests (void)
{
  bool ok = true;

  //DerivedAB *derivedAB;


  A *a = new A ();
  a->Unref ();

  a = new A ();
  A *a1 = a->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  a1->Unref ();
  a1 = a->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  a1->Unref ();
  a->Unref ();

  B *b = new B ();
  B *b1 = b->QueryInterface<B> (B::iid);
  if (b1 == 0 || b1 != b)
    {
      ok = false;
    }
  b1->Unref ();
  
  a = new A ();
  a->AddInterface (b);
  b1 = b->QueryInterface<B> (B::iid);
  if (b1 == 0 || b1 != b)
    {
      ok = false;
    }
  b1->Unref ();
  a1 = b->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  a1->Unref ();
  a1 = a->QueryInterface<A> (A::iid);
  if (a1 == 0 || a1 != a)
    {
      ok = false;
    }
  a1->Unref ();
  b1 = a->QueryInterface<B> (B::iid);
  if (b1 == 0 || b1 != b)
    {
      ok = false;
    }
  b1->Unref ();
  
  a->Unref ();
  b->Unref ();

  Derived *derived = new Derived ();
  Base *base = derived->QueryInterface<Base> (Base::iid);
  if (base == 0)
    {
      ok = false;
    }
  Derived *derived1 = base->QueryInterface<Derived> (Derived::iid);
  if (derived1 == 0 || derived1 != derived)
    {
      ok = false;
    }
  derived1->Unref ();
  base->Unref ();
  derived->Unref ();

  return ok;
}


static InterfaceTest g_interface_test;

}// namespace ns3

#endif /* RUN_SELF_TESTS */
