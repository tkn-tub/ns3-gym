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
#include "object-container.h"

namespace ns3 {

ObjectContainer::DeleterList ObjectContainer::m_deleterList;

ObjectContainer::~ObjectContainer ()
{
  Cleanup ();
}

void
ObjectContainer::Cleanup (void)
{
  for (List::iterator i = m_list.begin (); i != m_list.end (); i++)
    {
      uint32_t uid = i->first;
      std::vector<void *> *vec = i->second;
      ObjectDeleter deleter = LookupObjectDeleter (uid);
      for (std::vector<void *>::iterator j = vec->begin (); 
	   j != vec->end (); j++) 
	{
	  (deleter) (*j); 
	}
      delete vec;
    }
  m_list.erase (m_list.begin (), m_list.end ());
}

uint32_t 
ObjectContainer::GetGlobalUid (void) const
{
  static uint32_t globalUid = 0;
  globalUid ++;
  return globalUid;
}

uint32_t 
ObjectContainer::RegisterUid (uint32_t uid, ObjectDeleter deleter) const
{
  for (DeleterList::iterator i = m_deleterList.begin ();
       i != m_deleterList.end (); i++)
    {
      NS_ASSERT (i->first != uid);
    }
  m_deleterList.push_back (std::make_pair (uid, deleter));
  return uid;
}

ObjectContainer::ObjectDeleter 
ObjectContainer::LookupObjectDeleter (uint32_t uid) const
{
  for (DeleterList::iterator i = m_deleterList.begin ();
       i != m_deleterList.end (); i++)
    {
      if (i->first == uid)
	{
	  return i->second;
	}
    }
  NS_FATAL_ERROR ("unknown deleter requested.");
  return 0;
}

}//namespace ns3


#ifdef RUN_SELF_TESTS

#include "test.h"
namespace ns3 {
  
class A 
{
public:
  A () {}
  ~A () {}
};

class WithCopy
{
public:
  WithCopy () {}
  ~WithCopy () {}
  WithCopy *Copy (void) const {return new WithCopy ();}
};

class B
{
public:
  B () {}
  ~B () {}
};

class Base
{
public:
  Base () {}
  virtual ~Base () {}
};

class DerivedA
{
public:
  DerivedA () {}
  virtual ~DerivedA () {}
};

class DerivedB
{
public:
  DerivedB () {}
  virtual ~DerivedB () {}
};


class ObjectContainerTest : public Test
{
public:
  ObjectContainerTest ();
  virtual ~ObjectContainerTest ();

  virtual bool RunTests (void);
};

ObjectContainerTest::ObjectContainerTest ()
  : Test ("ObjectContainer")
{}

ObjectContainerTest::~ObjectContainerTest ()
{}

bool
ObjectContainerTest::RunTests (void)
{
  bool ok = true;

  ObjectContainer container;
  A *a = new A ();
  A *firstA = a;
  container.Acquire (a);
  a = new A ();
  container.Acquire (a);
  a = new A ();
  container.Acquire (a);
  B *b = new B ();
  container.Acquire (b);
  a = new A ();
  container.Acquire (a);
  b = new B ();
  container.Acquire (b);

  container.Remove (firstA);
  delete firstA;

  Base *base = new Base ();
  container.Acquire (base);
  DerivedA *derivedA = new DerivedA ();
  container.Acquire (derivedA);
  DerivedB *derivedB = new DerivedB ();
  container.Acquire (derivedB);
  base = new Base ();
  container.Acquire (base);
  derivedB = new DerivedB ();
  container.Acquire (derivedB);


  // the following cannot work because no copy method defined.
  //container.Add (A ());
  container.Add (WithCopy ());

  container.Cleanup ();
  
  return ok;
}

static ObjectContainerTest g_objectContainerTest;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
