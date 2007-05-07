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
#include "ns-unknown-manager.h"
#include "uid-manager.h"
#include "singleton.h"

namespace ns3 {

// we redefine a UidManager type for the class id singleton below.
// otherwise, we would have to share the same id singleton instance
// with the Iids.
class CidManager : public UidManager
{};

ClassId::ClassId (std::string name)
  : m_classId (Singleton<CidManager>::Get ()->Allocate (name))
{}

ClassId::ClassId (uint32_t classId)
  : m_classId (classId)
{}

std::string 
ClassId::GetName (void)
{
  return Singleton<CidManager>::Get ()->LookupByUid (m_classId);
}

bool operator == (const ClassId &a, const ClassId &b)
{
  return a.m_classId == b.m_classId;
}

NsUnknown *
NsUnknownManager::Create (ClassId classId)
{
  Callback<NsUnknown *> callback = DoGetCallback<empty,empty,empty,empty,empty> (classId);
  return callback ();
}

CallbackBase *
NsUnknownManager::Lookup (ClassId classId)
{
  List *list = Singleton<List>::Get ();
  for (List::const_iterator i = list->begin (); i != list->end (); i++)
    {
      if (i->first == classId)
	{
	  return i->second;
	}
    }
  return 0;
}

ClassId 
NsUnknownManager::LookupByName (std::string name)
{
  return ClassId (Singleton<CidManager>::Get ()->LookupByName (name));
}

void
NsUnknownManager::Register (ClassId classId, CallbackBase *callback)
{
  List *list = Singleton<List>::Get ();
  list->push_back (std::make_pair (classId, callback));
}


} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "ns-unknown.h"

namespace {

class A : public ns3::NsUnknown
{
public:
  static const ns3::ClassId cidZero;
  static const ns3::ClassId cidOneBool;
  static const ns3::ClassId cidOneInt;
  static const ns3::Iid iid;

  A ();
  A (bool);
  A (int);

  bool m_zeroInvoked;
  bool m_oneBoolInvoked;
  bool m_oneIntInvoked;

  bool m_bool;
  int m_int;
};

const ns3::ClassId A::cidZero = ns3::NsUnknownManager::RegisterConstructor <A> ("A");
const ns3::ClassId A::cidOneBool = ns3::NsUnknownManager::RegisterConstructor <A,bool> ("ABool");
const ns3::ClassId A::cidOneInt = ns3::NsUnknownManager::RegisterConstructor <A,int> ("AInt");
const ns3::Iid A::iid ("IA");

A::A ()
  : NsUnknown (A::iid),
    m_zeroInvoked (true),
    m_oneBoolInvoked (false),
    m_oneIntInvoked (false)
{}

A::A (bool b)
  : NsUnknown (A::iid),
    m_zeroInvoked (false),
    m_oneBoolInvoked (true),
    m_oneIntInvoked (false),
    m_bool (b)
{}

A::A (int i)
  : NsUnknown (A::iid),
    m_zeroInvoked (false),
    m_oneBoolInvoked (false),
    m_oneIntInvoked (true),
    m_int (i)
{}

}

namespace ns3 {

class NsUnknownManagerTest : public Test
{
public:
  NsUnknownManagerTest ();
  virtual bool RunTests (void);
};

NsUnknownManagerTest::NsUnknownManagerTest ()
  : Test ("NsUnknownManager")
{}
bool 
NsUnknownManagerTest::RunTests (void)
{
  bool ok = true;

  A *a = 0;
  a = NsUnknownManager::Create<A> (A::cidZero, A::iid);
  if (a == 0 ||
      !a->m_zeroInvoked)
    {
      ok = false;
    }
  a->Unref ();

  a = NsUnknownManager::Create<A> (A::cidOneBool, A::iid, true);
  if (a == 0 ||
      !a->m_oneBoolInvoked ||
      !a->m_bool)
    {
      ok = false;
    }
  a->Unref ();

  a = NsUnknownManager::Create<A> (A::cidOneBool, A::iid, false);
  if (a == 0 ||
      !a->m_oneBoolInvoked ||
      a->m_bool)
    {
      ok = false;
    }
  a->Unref ();

  return ok;
}


static NsUnknownManagerTest g_unknownManagerTest;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
