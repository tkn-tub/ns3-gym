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
                     ns3::Attribute initialValue,
                     ns3::Ptr<const ns3::AttributeAccessor> spec,
                     ns3::Ptr<const ns3::AttributeChecker> checker);
  uint32_t GetAttributeN (uint16_t uid) const;
  std::string GetAttributeName (uint16_t uid, uint32_t i) const;
  std::string GetAttributeHelp (uint16_t uid, uint32_t i) const;
  uint32_t GetAttributeFlags (uint16_t uid, uint32_t i) const;
  ns3::Attribute GetAttributeInitialValue (uint16_t uid, uint32_t i) const;
  ns3::Ptr<const ns3::AttributeAccessor> GetAttributeAccessor (uint16_t uid, uint32_t i) const;
  ns3::Ptr<const ns3::AttributeChecker> GetAttributeChecker (uint16_t uid, uint32_t i) const;
  void AddTraceSource (uint16_t uid,
                       std::string name, 
                       std::string help,
                       ns3::Ptr<const ns3::TraceSourceAccessor> accessor);
  uint32_t GetTraceSourceN (uint16_t uid) const;
  std::string GetTraceSourceName (uint16_t uid, uint32_t i) const;
  std::string GetTraceSourceHelp (uint16_t uid, uint32_t i) const;
  ns3::Ptr<const ns3::TraceSourceAccessor> GetTraceSourceAccessor (uint16_t uid, uint32_t i) const;
  bool MustHideFromDocumentation (uint16_t uid) const;

private:
  struct AttributeInformation {
    std::string name;
    std::string help;
    uint32_t flags;
    ns3::Attribute initialValue;
    ns3::Ptr<const ns3::AttributeAccessor> param;
    ns3::Ptr<const ns3::AttributeChecker> checker;
  };
  struct TraceSourceInformation {
    std::string name;
    std::string help;
    ns3::Ptr<const ns3::TraceSourceAccessor> accessor;
  };
  struct IidInformation {
    std::string name;
    uint16_t parent;
    std::string groupName;
    bool hasConstructor;
    ns3::Callback<ns3::ObjectBase *> constructor;
    bool mustHideFromDocumentation;
    std::vector<struct AttributeInformation> attributes;
    std::vector<struct TraceSourceInformation> traceSources;
  };
  typedef std::vector<struct IidInformation>::const_iterator Iterator;

  struct IidManager::IidInformation *LookupInformation (uint16_t uid) const;

  std::vector<struct IidInformation> m_information;
};

IidManager::IidManager ()
{}

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

void 
IidManager::AddAttribute (uint16_t uid, 
                          std::string name,
                          std::string help, 
                          uint32_t flags,
                          ns3::Attribute initialValue,
                          ns3::Ptr<const ns3::AttributeAccessor> spec,
                          ns3::Ptr<const ns3::AttributeChecker> checker)
{
  struct IidInformation *information = LookupInformation (uid);
  for (std::vector<struct AttributeInformation>::const_iterator j = information->attributes.begin ();
       j != information->attributes.end (); j++)
    {
      if (j->name == name)
        {
          NS_FATAL_ERROR ("Registered the same attribute twice name=\""<<name<<"\" in TypeId=\""<<information->name<<"\"");
          return;
        }
    }
  struct AttributeInformation param;
  param.name = name;
  param.help = help;
  param.flags = flags;
  param.initialValue = initialValue;
  param.param = spec;
  param.checker = checker;
  information->attributes.push_back (param);
}


uint32_t 
IidManager::GetAttributeN (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->attributes.size ();
}
std::string 
IidManager::GetAttributeName (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i].name;
}
std::string 
IidManager::GetAttributeHelp (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i].help;
}
uint32_t
IidManager::GetAttributeFlags (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i].flags;
}
ns3::Attribute 
IidManager::GetAttributeInitialValue (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i].initialValue;
}
ns3::Ptr<const ns3::AttributeAccessor>
IidManager::GetAttributeAccessor (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i].param;
}
ns3::Ptr<const ns3::AttributeChecker>
IidManager::GetAttributeChecker (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i].checker;
}

void 
IidManager::AddTraceSource (uint16_t uid,
                            std::string name, 
                            std::string help,
                            ns3::Ptr<const ns3::TraceSourceAccessor> accessor)
{
  struct IidInformation *information  = LookupInformation (uid);
  struct TraceSourceInformation source;
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
std::string 
IidManager::GetTraceSourceName (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->traceSources.size ());
  return information->traceSources[i].name;
}
std::string 
IidManager::GetTraceSourceHelp (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->traceSources.size ());
  return information->traceSources[i].help;
}
ns3::Ptr<const ns3::TraceSourceAccessor> 
IidManager::GetTraceSourceAccessor (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->traceSources.size ());
  return information->traceSources[i].accessor;
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

TypeId::TypeId ()
  : m_tid (0)
{}

TypeId::TypeId (const char *name)
{
  uint16_t uid = Singleton<IidManager>::Get ()->AllocateUid (name);
  NS_ASSERT (uid != 0);
  m_tid = uid;
}


TypeId::TypeId (uint16_t tid)
  : m_tid (tid)
{}
TypeId::~TypeId ()
{}
TypeId 
TypeId::LookupByName (std::string name)
{
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (name);
  NS_ASSERT (uid != 0);
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

bool
TypeId::LookupAttributeByFullName (std::string fullName, struct TypeId::AttributeInfo *info)
{
  std::string::size_type pos = fullName.rfind ("::");
  if (pos == std::string::npos)
    {
      return 0;
    }
  std::string tidName = fullName.substr (0, pos);
  std::string paramName = fullName.substr (pos+2, fullName.size () - (pos+2));
  TypeId tid;
  bool ok = LookupByNameFailSafe (tidName, &tid);
  if (!ok)
    {
      return false;
    }
  return tid.LookupAttributeByName (paramName, info);
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
TypeId::LookupAttributeByName (std::string name, struct TypeId::AttributeInfo *info) const
{
  TypeId tid;
  TypeId nextTid = *this;
  do {
    tid = nextTid;
    for (uint32_t i = 0; i < tid.GetAttributeN (); i++)
      {
        std::string paramName = tid.GetAttributeName (i);
        if (paramName == name)
          {
            info->accessor = tid.GetAttributeAccessor (i);
            info->flags = tid.GetAttributeFlags (i);
            info->initialValue = tid.GetAttributeInitialValue (i);
            info->checker = tid.GetAttributeChecker (i);
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
                      Attribute initialValue,
                      Ptr<const AttributeAccessor> param,
                      Ptr<const AttributeChecker> checker)
{
  Singleton<IidManager>::Get ()->AddAttribute (m_tid, name, help, ATTR_SGC, initialValue, param, checker);
  return *this;
}

TypeId 
TypeId::AddAttribute (std::string name,
                      std::string help, 
                      uint32_t flags,
                      Attribute initialValue,
                      Ptr<const AttributeAccessor> param,
                      Ptr<const AttributeChecker> checker)
{
  Singleton<IidManager>::Get ()->AddAttribute (m_tid, name, help, flags, initialValue, param, checker);
  return *this;
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
std::string 
TypeId::GetAttributeName (uint32_t i) const
{
  std::string name = Singleton<IidManager>::Get ()->GetAttributeName (m_tid, i);
  return name;
}
std::string 
TypeId::GetAttributeHelp (uint32_t i) const
{
  std::string help = Singleton<IidManager>::Get ()->GetAttributeHelp (m_tid, i);
  return help;
}
std::string 
TypeId::GetAttributeFullName (uint32_t i) const
{
  return GetName () + "::" + GetAttributeName (i);
}
Attribute 
TypeId::GetAttributeInitialValue (uint32_t i) const
{
  Attribute value = Singleton<IidManager>::Get ()->GetAttributeInitialValue (m_tid, i);
  return value;
}
Ptr<const AttributeAccessor>
TypeId::GetAttributeAccessor (uint32_t i) const
{
  // Used exclusively by the Object class.
  Ptr<const AttributeAccessor> param = Singleton<IidManager>::Get ()->GetAttributeAccessor (m_tid, i);
  return param;
}
uint32_t 
TypeId::GetAttributeFlags (uint32_t i) const
{
  // Used exclusively by the Object class.
  uint32_t flags = Singleton<IidManager>::Get ()->GetAttributeFlags (m_tid, i);
  return flags;
}
Ptr<const AttributeChecker>
TypeId::GetAttributeChecker (uint32_t i) const
{
  // Used exclusively by the Object class.
  Ptr<const AttributeChecker> checker = Singleton<IidManager>::Get ()->GetAttributeChecker (m_tid, i);
  return checker;
}

uint32_t 
TypeId::GetTraceSourceN (void) const
{
  return Singleton<IidManager>::Get ()->GetTraceSourceN (m_tid);
}
std::string 
TypeId::GetTraceSourceName (uint32_t i) const
{
  return Singleton<IidManager>::Get ()->GetTraceSourceName (m_tid, i);
}
std::string 
TypeId::GetTraceSourceHelp (uint32_t i) const
{
  return Singleton<IidManager>::Get ()->GetTraceSourceHelp (m_tid, i);
}
Ptr<const TraceSourceAccessor> 
TypeId::GetTraceSourceAccessor (uint32_t i) const
{
  return Singleton<IidManager>::Get ()->GetTraceSourceAccessor (m_tid, i);
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
        std::string srcName = tid.GetTraceSourceName (i);
        if (srcName == name)
          {
            return tid.GetTraceSourceAccessor (i);
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

bool operator == (TypeId a, TypeId b)
{
  return a.m_tid == b.m_tid;
}

bool operator != (TypeId a, TypeId b)
{
  return a.m_tid != b.m_tid;
}

bool operator < (TypeId a, TypeId b)
{
  return a.m_tid < b.m_tid;
}

} // namespace ns3
