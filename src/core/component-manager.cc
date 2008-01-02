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
#include "component-manager.h"
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
ClassId::GetName (void) const
{
  return Singleton<CidManager>::Get ()->LookupByUid (m_classId);
}

bool operator == (const ClassId &a, const ClassId &b)
{
  return a.m_classId == b.m_classId;
}

ComponentManager::ClassIdEntry::ClassIdEntry (ClassId classId)
  : m_classId (classId)
{}

Ptr<Object>
ComponentManager::Create (ClassId classId)
{
  Callback<Ptr<Object> > callback = DoGetCallback<empty,empty,empty,empty,empty> (classId);
  return callback ();
}

CallbackBase *
ComponentManager::Lookup (ClassId classId)
{
  List *list = Singleton<List>::Get ();
  for (List::const_iterator i = list->begin (); i != list->end (); i++)
    {
      if (i->m_classId == classId)
	{
	  return i->m_callback;
	}
    }
  return 0;
}

ClassId 
ComponentManager::LookupByName (std::string name)
{
  return ClassId (Singleton<CidManager>::Get ()->LookupByName (name));
}
ClassId 
ComponentManager::LookupByName (std::string name, bool *ok)
{
  uint32_t cid = Singleton<CidManager>::Get ()->LookupByName (name);
  if (cid == 0)
    {
      *ok = false;
    }
  else
    {
      *ok = true;
    }
  return ClassId (cid);
}
std::vector<ClassId> 
ComponentManager::LookupByInterfaceId (InterfaceId iid)
{
  std::vector<ClassId> classIdList;
  List *list = Singleton<List>::Get ();
  for (List::const_iterator i = list->begin (); i != list->end (); i++)
    {
      for (std::vector<const InterfaceId *>::const_iterator j = i->m_supportedInterfaces.begin ();
           j != i->m_supportedInterfaces.end (); j++)
        {
          if (*(*j) == iid)
            {
              classIdList.push_back (i->m_classId);
              break;
            }
        }
    }
  unique (classIdList.begin (), classIdList.end ());
  return classIdList;
}

void
ComponentManager::Register (ClassId classId, CallbackBase *callback, 
                            std::vector<const InterfaceId *> supportedInterfaces)
{
  List *list = Singleton<List>::Get ();
  struct ClassIdEntry entry = ClassIdEntry (classId);
  entry.m_callback = callback;
  bool foundObject = false;
  for (std::vector<const InterfaceId *>::iterator i = supportedInterfaces.begin ();
       i != supportedInterfaces.end (); i++)
    {
      if (*i == &Object::iid)
        {
          foundObject = true;
        }
    }
  if (!foundObject)
    {
      supportedInterfaces.push_back (&Object::iid);
    }
  entry.m_supportedInterfaces = supportedInterfaces;
  list->push_back (entry);
}

void
RegisterCallback (ClassId classId, CallbackBase *callback, std::vector<const InterfaceId *> supportedInterfaces)
{
  return ComponentManager::Register (classId, callback, supportedInterfaces);
}


ClassIdDefaultValue::ClassIdDefaultValue (std::string name, 
                                          std::string help,
                                          const InterfaceId &iid,
                                          std::string defaultValue)
  : DefaultValueBase (name, help),
    m_defaultName (defaultValue),
    m_name (defaultValue),
    m_interfaceId (&iid)
{
  DefaultValueList::Add (this);
}
ClassId 
ClassIdDefaultValue::GetValue (void) const
{
  return ComponentManager::LookupByName (m_name);
}
void 
ClassIdDefaultValue::SetValue (ClassId classId)
{
  m_name = classId.GetName ();
}
void 
ClassIdDefaultValue::SetValue (std::string name)
{
  m_name = name;
}
bool 
ClassIdDefaultValue::DoParseValue (const std::string &value)
{
  bool ok;
  ClassId classId = ComponentManager::LookupByName (value, &ok);
  if (!ok)
    {
      return false;
    }
  std::vector<ClassId> classIdList = ComponentManager::LookupByInterfaceId (*m_interfaceId);
  for (std::vector<ClassId>::const_iterator i = classIdList.begin ();
       i != classIdList.end (); i++)
    {
      if (*i == classId)
        {
          m_name = value;
          return true;
        }
    }
  return false;
}

std::string 
ClassIdDefaultValue::DoGetType (void) const
{
  std::vector<ClassId> classIdList = ComponentManager::LookupByInterfaceId (*m_interfaceId);
  std::ostringstream oss;
  oss << "(";
  for (std::vector<ClassId>::const_iterator i = classIdList.begin ();
       i != classIdList.end (); i++)
    {
      if (i != classIdList.begin ())
        {
          oss << "|";
        }
      oss << i->GetName ();
    }
  oss << ")";
  return oss.str ();
}

std::string 
ClassIdDefaultValue::DoGetDefaultValue (void) const
{
  return m_name;
}




} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "object.h"

namespace {


class B : public ns3::Object
{
public:
  static const ns3::InterfaceId iid;
  B ();
};

const ns3::InterfaceId B::iid = MakeInterfaceId ("B", Object::iid);

B::B ()
{}


class A : public ns3::Object
{
public:
  static const ns3::ClassId cidZero;
  static const ns3::ClassId cidOneBool;
  static const ns3::ClassId cidOneUi32;
  static const ns3::ClassId cidOther;
  static const ns3::InterfaceId iid;

  A ();
  A (bool);
  A (uint32_t);

  bool m_zeroInvoked;
  bool m_oneBoolInvoked;
  bool m_oneUi32Invoked;

  bool m_bool;
  int m_ui32;
};

const ns3::ClassId A::cidZero = ns3::MakeClassId<A> ("A", A::iid);
const ns3::ClassId A::cidOneBool = ns3::MakeClassId <A,bool> ("ABool", A::iid);
const ns3::ClassId A::cidOneUi32 = ns3::MakeClassId <A,uint32_t> ("AUi32", A::iid);
const ns3::InterfaceId A::iid = ns3::MakeInterfaceId ("A", Object::iid);

A::A ()
  : m_zeroInvoked (true),
    m_oneBoolInvoked (false),
    m_oneUi32Invoked (false)
{
  ns3::Ptr<B> b = ns3::CreateObject<B> ();
  AddInterface (b);
}

A::A (bool bo)
  : m_zeroInvoked (false),
    m_oneBoolInvoked (true),
    m_oneUi32Invoked (false),
    m_bool (bo)
{
  ns3::Ptr<B> b = ns3::CreateObject<B> ();
  AddInterface (b);
}

A::A (uint32_t i)
  : m_zeroInvoked (false),
    m_oneBoolInvoked (false),
    m_oneUi32Invoked (true),
    m_ui32 (i)
{
  ns3::Ptr<B> b = ns3::CreateObject<B> ();
  AddInterface (b);
}

class X : public A
{
public:
  static const ns3::InterfaceId iid;
};
class C : public X
{
public:
  static const ns3::InterfaceId iid;
};
class D : public C
{
public:
  static const ns3::InterfaceId iid;
  static const ns3::ClassId cid;
};

const ns3::InterfaceId X::iid = ns3::MakeInterfaceId ("X", A::iid);
const ns3::InterfaceId C::iid = ns3::MakeInterfaceId ("C", X::iid);
const ns3::InterfaceId D::iid = ns3::MakeInterfaceId ("D", C::iid);
const ns3::ClassId D::cid = ns3::MakeClassId<D> ("D", A::iid, X::iid, C::iid, D::iid);

}

namespace ns3 {

class ComponentManagerTest : public Test
{
public:
  ComponentManagerTest ();
  virtual bool RunTests (void);
};

ComponentManagerTest::ComponentManagerTest ()
  : Test ("ComponentManager")
{}
bool 
ComponentManagerTest::RunTests (void)
{
  bool ok = true;

  Ptr<A> a = 0;
  a = ComponentManager::Create<A> (A::cidZero, A::iid);
  if (a == 0 ||
      !a->m_zeroInvoked)
    {
      ok = false;
    }

  a = ComponentManager::Create<A,bool> (A::cidOneBool, A::iid, true);
  if (a == 0 ||
      !a->m_oneBoolInvoked ||
      !a->m_bool)
    {
      ok = false;
    }

  a = ComponentManager::Create<A,bool> (A::cidOneBool, A::iid, false);
  if (a == 0 ||
      !a->m_oneBoolInvoked ||
      a->m_bool)
    {
      ok = false;
    }

  a = ComponentManager::Create<A,uint32_t> (A::cidOneUi32, A::iid, 10);
  if (a == 0 ||
      !a->m_oneUi32Invoked ||
      a->m_ui32 != 10)
    {
      ok = false;
    }

  a = ComponentManager::Create<A> (A::cidOneUi32, A::iid, (uint32_t)10);
  if (a == 0 ||
      !a->m_oneUi32Invoked ||
      a->m_ui32 != 10)
    {
      ok = false;
    }

  Ptr<B> b = ComponentManager::Create<B,uint32_t> (A::cidOneUi32, B::iid, 10);
  if (b == 0)
    {
      ok = false;
    }

  return ok;
}


static ComponentManagerTest g_unknownManagerTest;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
