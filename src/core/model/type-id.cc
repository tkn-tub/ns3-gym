/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "type-id.h"
#include "singleton.h"
#include "trace-source-accessor.h"
#include <vector>
#include <sstream>

/*********************************************************************
 *         Helper code
 *********************************************************************/

namespace {

class IidManager
{
public:
  IidManager ();
  uint16_t AllocateUid (std::string name);
  void SetParent (uint16_t uid, uint16_t parent);
  void SetGroupName (uint16_t uid, std::string groupName);
  void AddConstructor (uint16_t uid, ns3::Callback<ns3::ObjectBase *> callback);
  void HideFromDocumentation (uint16_t uid);
  uint16_t GetUid (std::string name) const;
  std::string GetName (uint16_t uid) const;
  uint16_t GetParent (uint16_t uid) const;
  std::string GetGroupName (uint16_t uid) const;
  ns3::Callback<ns3::ObjectBase *> GetConstructor (uint16_t uid) const;
  bool HasConstructor (uint16_t uid) const;
  uint32_t GetRegisteredN (void) const;
  uint16_t GetRegistered (uint32_t i) const;
  void AddAttribute (uint16_t uid, 
                     std::string name,
                     std::string help, 
                     uint32_t flags,
                     ns3::Ptr<const ns3::AttributeValue> initialValue,
                     ns3::Ptr<const ns3::AttributeAccessor> spec,
                     ns3::Ptr<const ns3::AttributeChecker> checker);
  void SetAttributeInitialValue(uint16_t uid,
                                uint32_t i,
                                ns3::Ptr<const ns3::AttributeValue> initialValue);
  uint32_t GetAttributeN (uint16_t uid) const;
  struct ns3::TypeId::AttributeInformation GetAttribute(uint16_t uid, uint32_t i) const;
  void AddTraceSource (uint16_t uid,
                       std::string name, 
                       std::string help,
                       ns3::Ptr<const ns3::TraceSourceAccessor> accessor);
  uint32_t GetTraceSourceN (uint16_t uid) const;
  struct ns3::TypeId::TraceSourceInformation GetTraceSource(uint16_t uid, uint32_t i) const;
  bool MustHideFromDocumentation (uint16_t uid) const;

private:
  bool HasTraceSource (uint16_t uid, std::string name);
  bool HasAttribute (uint16_t uid, std::string name);

  struct IidInformation {
    std::string name;
    uint16_t parent;
    std::string groupName;
    bool hasConstructor;
    ns3::Callback<ns3::ObjectBase *> constructor;
    bool mustHideFromDocumentation;
    std::vector<struct ns3::TypeId::AttributeInformation> attributes;
    std::vector<struct ns3::TypeId::TraceSourceInformation> traceSources;
  };
  typedef std::vector<struct IidInformation>::const_iterator Iterator;

  struct IidManager::IidInformation *LookupInformation (uint16_t uid) const;

  std::vector<struct IidInformation> m_information;
};

IidManager::IidManager ()
{
}

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
  information.groupName = "";
  information.hasConstructor = false;
  information.mustHideFromDocumentation = false;
  m_information.push_back (information);
  uint32_t uid = m_information.size ();
  NS_ASSERT (uid <= 0xffff);
  return uid;
}

struct IidManager::IidInformation *
IidManager::LookupInformation (uint16_t uid) const
{
  NS_ASSERT (uid <= m_information.size () && uid != 0);
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
IidManager::SetGroupName (uint16_t uid, std::string groupName)
{
  struct IidInformation *information = LookupInformation (uid);
  information->groupName = groupName;
}
void
IidManager::HideFromDocumentation (uint16_t uid)
{
  struct IidInformation *information = LookupInformation (uid);
  information->mustHideFromDocumentation = true;
}

void 
IidManager::AddConstructor (uint16_t uid, ns3::Callback<ns3::ObjectBase *> callback)
{
  struct IidInformation *information = LookupInformation (uid);
  if (information->hasConstructor)
    {
      NS_FATAL_ERROR (information->name<<" already has a constructor.");
    }
  information->hasConstructor = true;
  information->constructor = callback;
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
std::string 
IidManager::GetGroupName (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->groupName;
}

ns3::Callback<ns3::ObjectBase *> 
IidManager::GetConstructor (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  if (!information->hasConstructor)
    {
      NS_FATAL_ERROR ("Requested constructor for "<<information->name<<" but it does not have one.");
    }
  return information->constructor;
}

bool 
IidManager::HasConstructor (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->hasConstructor;
}

uint32_t 
IidManager::GetRegisteredN (void) const
{
  return m_information.size ();
}
uint16_t 
IidManager::GetRegistered (uint32_t i) const
{
  return i + 1;
}

bool
IidManager::HasAttribute (uint16_t uid,
                          std::string name)
{
  struct IidInformation *information  = LookupInformation (uid);
  while (true)
    {
      for (std::vector<struct ns3::TypeId::AttributeInformation>::const_iterator i = information->attributes.begin ();
           i != information->attributes.end (); ++i)
        {
          if (i->name == name)
            {
              return true;
            }
        }
      struct IidInformation *parent = LookupInformation (information->parent);
      if (parent == information)
        {
          // top of inheritance tree
          return false;
        }
      // check parent
      information = parent;
    }
  return false;
}

void 
IidManager::AddAttribute (uint16_t uid, 
                          std::string name,
                          std::string help, 
                          uint32_t flags,
                          ns3::Ptr<const ns3::AttributeValue> initialValue,
                          ns3::Ptr<const ns3::AttributeAccessor> accessor,
                          ns3::Ptr<const ns3::AttributeChecker> checker)
{
  struct IidInformation *information = LookupInformation (uid);
  if (HasAttribute (uid, name))
    {
      NS_FATAL_ERROR ("Attribute \"" << name << "\" already registered on tid=\"" << 
                      information->name << "\"");
    }
  struct ns3::TypeId::AttributeInformation info;
  info.name = name;
  info.help = help;
  info.flags = flags;
  info.initialValue = initialValue;
  info.originalInitialValue = initialValue;
  info.accessor = accessor;
  info.checker = checker;
  information->attributes.push_back (info);
}
void 
IidManager::SetAttributeInitialValue(uint16_t uid,
                                     uint32_t i,
                                     ns3::Ptr<const ns3::AttributeValue> initialValue)
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  information->attributes[i].initialValue = initialValue;
}



uint32_t 
IidManager::GetAttributeN (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->attributes.size ();
}
struct ns3::TypeId::AttributeInformation 
IidManager::GetAttribute(uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i];
}

bool
IidManager::HasTraceSource (uint16_t uid,
                            std::string name)
{
  struct IidInformation *information  = LookupInformation (uid);
  while (true)
    {
      for (std::vector<struct ns3::TypeId::TraceSourceInformation>::const_iterator i = information->traceSources.begin ();
           i != information->traceSources.end (); ++i)
        {
          if (i->name == name)
            {
              return true;
            }
        }
      struct IidInformation *parent = LookupInformation (information->parent);
      if (parent == information)
        {
          // top of inheritance tree
          return false;
        }
      // check parent
      information = parent;
    }
  return false;
}

void 
IidManager::AddTraceSource (uint16_t uid,
                            std::string name, 
                            std::string help,
                            ns3::Ptr<const ns3::TraceSourceAccessor> accessor)
{
  struct IidInformation *information  = LookupInformation (uid);
  if (HasTraceSource (uid, name))
    {
      NS_FATAL_ERROR ("Trace source \"" << name << "\" already registered on tid=\"" << 
                      information->name << "\"");
    }
  struct ns3::TypeId::TraceSourceInformation source;
  source.name = name;
  source.help = help;
  source.accessor = accessor;
  information->traceSources.push_back (source);
}
uint32_t 
IidManager::GetTraceSourceN (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->traceSources.size ();
}
struct ns3::TypeId::TraceSourceInformation 
IidManager::GetTraceSource(uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->traceSources.size ());
  return information->traceSources[i];
}
bool 
IidManager::MustHideFromDocumentation (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->mustHideFromDocumentation;
}

} // anonymous namespace

namespace ns3 {

/*********************************************************************
 *         The TypeId class
 *********************************************************************/

TypeId::TypeId (const char *name)
{
  uint16_t uid = Singleton<IidManager>::Get ()->AllocateUid (name);
  NS_ASSERT (uid != 0);
  m_tid = uid;
}


TypeId::TypeId (uint16_t tid)
  : m_tid (tid)
{
}
TypeId
TypeId::LookupByName (std::string name)
{
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (name);
  NS_ASSERT_MSG (uid != 0, "Assert in TypeId::LookupByName: " << name << " not found");
  return TypeId (uid);
}
bool
TypeId::LookupByNameFailSafe (std::string name, TypeId *tid)
{
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (name);
  if (uid == 0)
    {
      return false;
    }
  *tid = TypeId (uid);
  return true;
}

uint32_t 
TypeId::GetRegisteredN (void)
{
  return Singleton<IidManager>::Get ()->GetRegisteredN ();
}
TypeId 
TypeId::GetRegistered (uint32_t i)
{
  return TypeId (Singleton<IidManager>::Get ()->GetRegistered (i));
}

bool
TypeId::LookupAttributeByName (std::string name, struct TypeId::AttributeInformation *info) const
{
  TypeId tid;
  TypeId nextTid = *this;
  do {
      tid = nextTid;
      for (uint32_t i = 0; i < tid.GetAttributeN (); i++)
        {
          struct TypeId::AttributeInformation tmp = tid.GetAttribute(i);
          if (tmp.name == name)
            {
              *info = tmp;
              return true;
            }
        }
      nextTid = tid.GetParent ();
    } while (nextTid != tid);
  return false;
}

TypeId 
TypeId::SetParent (TypeId tid)
{
  Singleton<IidManager>::Get ()->SetParent (m_tid, tid.m_tid);
  return *this;
}
TypeId 
TypeId::SetGroupName (std::string groupName)
{
  Singleton<IidManager>::Get ()->SetGroupName (m_tid, groupName);
  return *this;
}
TypeId 
TypeId::GetParent (void) const
{
  uint16_t parent = Singleton<IidManager>::Get ()->GetParent (m_tid);
  return TypeId (parent);
}
bool 
TypeId::HasParent (void) const
{
  uint16_t parent = Singleton<IidManager>::Get ()->GetParent (m_tid);
  return parent != m_tid;
}
bool 
TypeId::IsChildOf (TypeId other) const
{
  TypeId tmp = *this;
  while (tmp != other && tmp != tmp.GetParent ())
    {
      tmp = tmp.GetParent ();
    }
  return tmp == other && *this != other;
}
std::string 
TypeId::GetGroupName (void) const
{
  std::string groupName = Singleton<IidManager>::Get ()->GetGroupName (m_tid);
  return groupName;
}

std::string 
TypeId::GetName (void) const
{
  std::string name = Singleton<IidManager>::Get ()->GetName (m_tid);
  return name;
}

bool 
TypeId::HasConstructor (void) const
{
  bool hasConstructor = Singleton<IidManager>::Get ()->HasConstructor (m_tid);
  return hasConstructor;
}

void
TypeId::DoAddConstructor (Callback<ObjectBase *> cb)
{
  Singleton<IidManager>::Get ()->AddConstructor (m_tid, cb);
}

TypeId 
TypeId::AddAttribute (std::string name,
                      std::string help, 
                      const AttributeValue &initialValue,
                      Ptr<const AttributeAccessor> accessor,
                      Ptr<const AttributeChecker> checker)
{
  Singleton<IidManager>::Get ()->AddAttribute (m_tid, name, help, ATTR_SGC, initialValue.Copy (), accessor, checker);
  return *this;
}

TypeId 
TypeId::AddAttribute (std::string name,
                      std::string help, 
                      uint32_t flags,
                      const AttributeValue &initialValue,
                      Ptr<const AttributeAccessor> accessor,
                      Ptr<const AttributeChecker> checker)
{
  Singleton<IidManager>::Get ()->AddAttribute (m_tid, name, help, flags, initialValue.Copy (), accessor, checker);
  return *this;
}

bool 
TypeId::SetAttributeInitialValue(uint32_t i, 
                                 Ptr<const AttributeValue> initialValue)
{
  Singleton<IidManager>::Get ()->SetAttributeInitialValue (m_tid, i, initialValue);
  return true;
}


Callback<ObjectBase *> 
TypeId::GetConstructor (void) const
{
  Callback<ObjectBase *>  cb = Singleton<IidManager>::Get ()->GetConstructor (m_tid);
  return cb;
}

bool 
TypeId::MustHideFromDocumentation (void) const
{
  bool mustHide = Singleton<IidManager>::Get ()->MustHideFromDocumentation (m_tid);
  return mustHide;
}

uint32_t 
TypeId::GetAttributeN (void) const
{
  uint32_t n = Singleton<IidManager>::Get ()->GetAttributeN (m_tid);
  return n;
}
struct TypeId::AttributeInformation 
TypeId::GetAttribute(uint32_t i) const
{
  return Singleton<IidManager>::Get ()->GetAttribute(m_tid, i);
}
std::string 
TypeId::GetAttributeFullName (uint32_t i) const
{
  struct TypeId::AttributeInformation info = GetAttribute(i);
  return GetName () + "::" + info.name;
}

uint32_t 
TypeId::GetTraceSourceN (void) const
{
  return Singleton<IidManager>::Get ()->GetTraceSourceN (m_tid);
}
struct TypeId::TraceSourceInformation 
TypeId::GetTraceSource(uint32_t i) const
{
  return Singleton<IidManager>::Get ()->GetTraceSource(m_tid, i);
}

TypeId 
TypeId::AddTraceSource (std::string name,
                        std::string help,
                        Ptr<const TraceSourceAccessor> accessor)
{
  Singleton<IidManager>::Get ()->AddTraceSource (m_tid, name, help, accessor);
  return *this;
}

TypeId 
TypeId::HideFromDocumentation (void)
{
  Singleton<IidManager>::Get ()->HideFromDocumentation (m_tid);
  return *this;
}


Ptr<const TraceSourceAccessor> 
TypeId::LookupTraceSourceByName (std::string name) const
{
  TypeId tid;
  TypeId nextTid = *this;
  do {
      tid = nextTid;
      for (uint32_t i = 0; i < tid.GetTraceSourceN (); i++)
        {
          struct TypeId::TraceSourceInformation info = tid.GetTraceSource (i);
          if (info.name == name)
            {
              return info.accessor;
            }
        }
      nextTid = tid.GetParent ();
    } while (nextTid != tid);
  return 0;
}

uint16_t 
TypeId::GetUid (void) const
{
  return m_tid;
}
void 
TypeId::SetUid (uint16_t tid)
{
  m_tid = tid;
}

std::ostream & operator << (std::ostream &os, TypeId tid)
{
  os << tid.GetName ();
  return os;
}
std::istream & operator >> (std::istream &is, TypeId &tid)
{
  std::string tidString;
  is >> tidString;
  bool ok = TypeId::LookupByNameFailSafe (tidString, &tid);
  if (!ok)
    {
      is.setstate (std::ios_base::badbit);
    }
  return is;
}


ATTRIBUTE_HELPER_CPP (TypeId);

bool operator < (TypeId a, TypeId b)
{
  return a.m_tid < b.m_tid;
}

} // namespace ns3
