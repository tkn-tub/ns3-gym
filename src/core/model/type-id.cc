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
#include "log.h"  // NS_ASSERT and NS_LOG
#include "hash.h"
#include "type-id.h"
#include "singleton.h"
#include "trace-source-accessor.h"

#include <map>
#include <vector>
#include <sstream>
#include <iomanip>

/*********************************************************************
 *         Helper code
 *********************************************************************/

NS_LOG_COMPONENT_DEFINE ("TypeId");

namespace ns3 {

// IidManager needs to be in ns3 namespace for NS_ASSERT and NS_LOG
// to find g_log

/**
 * \brief TypeId information manager
 *
 * Information records are stored in a vector.  Name and hash lookup
 * are performed by maps to the vector index.
 *
 * \internal
 * <b>Hash Chaining</b>
 *
 * We require all types to produce distinct hashes. What if we encounter
 * two types that produce the same hash value?  As we move to a
 * federated distribution model (the App store), it becomes increasingly
 * likely that the core ns3 team *won't* discover this in test builds.
 * Therefore, we need to handle this case explicitly.
 *
 * Note, we expect this to be *extremely* rare.  As of this writing we
 * have ~400 < 2^9 types, so the probability of getting a collision
 * when we introduce a new type is ~2^9/2^31 = 2^-22, assuming we
 * reserve 31 bits for the hash, and one bit for chaining.  Even with
 * double the number of types the probability of having a collision
 * is only 2 x 10^-4.  The probability for a three-fold collision is
 * 1 x 10^-10.
 *
 * Therefore, we'll handle one collision explicitly by reserving
 * the high order bit of the hash value, and assert on higher level
 * collisions.  The three-fold collision probability should be an
 * acceptablly small error rate.
 */
class IidManager
{
public:
  IidManager ();
  uint16_t AllocateUid (std::string name);
  void SetParent (uint16_t uid, uint16_t parent);
  void SetGroupName (uint16_t uid, std::string groupName);
  void AddConstructor (uint16_t uid, Callback<ObjectBase *> callback);
  void HideFromDocumentation (uint16_t uid);
  uint16_t GetUid (std::string name) const;
  uint16_t GetUid (TypeId::hash_t hash) const;
  std::string GetName (uint16_t uid) const;
  TypeId::hash_t GetHash (uint16_t uid) const;
  uint16_t GetParent (uint16_t uid) const;
  std::string GetGroupName (uint16_t uid) const;
  Callback<ObjectBase *> GetConstructor (uint16_t uid) const;
  bool HasConstructor (uint16_t uid) const;
  uint32_t GetRegisteredN (void) const;
  uint16_t GetRegistered (uint32_t i) const;
  void AddAttribute (uint16_t uid, 
                     std::string name,
                     std::string help, 
                     uint32_t flags,
                     Ptr<const AttributeValue> initialValue,
                     Ptr<const AttributeAccessor> spec,
                     Ptr<const AttributeChecker> checker);
  void SetAttributeInitialValue(uint16_t uid,
                                uint32_t i,
                                Ptr<const AttributeValue> initialValue);
  uint32_t GetAttributeN (uint16_t uid) const;
  struct TypeId::AttributeInformation GetAttribute(uint16_t uid, uint32_t i) const;
  void AddTraceSource (uint16_t uid,
                       std::string name, 
                       std::string help,
                       Ptr<const TraceSourceAccessor> accessor);
  uint32_t GetTraceSourceN (uint16_t uid) const;
  struct TypeId::TraceSourceInformation GetTraceSource(uint16_t uid, uint32_t i) const;
  bool MustHideFromDocumentation (uint16_t uid) const;

private:
  bool HasTraceSource (uint16_t uid, std::string name);
  bool HasAttribute (uint16_t uid, std::string name);
  static TypeId::hash_t Hasher (const std::string name);

  struct IidInformation {
    std::string name;
    TypeId::hash_t hash;
    uint16_t parent;
    std::string groupName;
    bool hasConstructor;
    Callback<ObjectBase *> constructor;
    bool mustHideFromDocumentation;
    std::vector<struct TypeId::AttributeInformation> attributes;
    std::vector<struct TypeId::TraceSourceInformation> traceSources;
  };
  typedef std::vector<struct IidInformation>::const_iterator Iterator;

  struct IidManager::IidInformation *LookupInformation (uint16_t uid) const;

  std::vector<struct IidInformation> m_information;

  typedef std::map<std::string, uint16_t> namemap_t;
  namemap_t m_namemap;

  typedef std::map<TypeId::hash_t, uint16_t> hashmap_t;
  hashmap_t m_hashmap;

  
  // To handle the first collision, we reserve the high bit as a
  // chain flag:
  enum { HashChainFlag = 0x80000000};
};

IidManager::IidManager ()
{
  NS_LOG_FUNCTION (this);
}

  //static
TypeId::hash_t
IidManager::Hasher (const std::string name)
{
  static ns3::Hasher hasher ( Create<Hash::Function::Murmur3> () );
  return hasher.clear ().GetHash32 (name);
}
  
uint16_t
IidManager::AllocateUid (std::string name)
{
  NS_LOG_FUNCTION (this << name);
  // Type names are definitive: equal names are equal types
  NS_ASSERT_MSG (m_namemap.count (name) == 0,
                 "Trying to allocate twice the same uid: " << name);
  
  TypeId::hash_t hash = Hasher (name) & (~HashChainFlag);
  if (m_hashmap.count (hash) == 1) {
    NS_LOG_ERROR ("Hash chaining TypeId for '" << name << "'.  "
                 << "This is not a bug, but is extremely unlikely.  "
                 << "Please contact the ns3 developers.");
    // ns3 developer contacted about this message:
    // You have four options (in order of difficulty):
    //   1. Let it ride, and play the odds that a third collision
    //        never appears.
    //   2. Change the name of the new (or old) tag, even trivially, to
    //        remove the collision.
    //   3. Switch to 64-bit hashes.
    //   4. Implement 2-bit (or higher) chaining.
    //
    //  Oh, by the way, I owe you a beer, since I bet Mathieu that
    //  this would never happen..  -- Peter Barnes, LLNL

    NS_ASSERT_MSG (m_hashmap.count (hash | HashChainFlag) == 0,
                   "Triplicate hash detected while chaining TypeId for '"
                   << name
                   << "'. Please contact the ns3 developers for assistance.");
    // ns3 developer contacted about this message:
    // You have three options: #2-4 above.
    //
    // Oh, by the way, I have no idea how this crazy hashing idea got
    // into ns3.  -- Peter Barnes, LLNL
    
    // Alphabetize the two types, so it's deterministic
    struct IidInformation * hinfo = LookupInformation (GetUid (hash));
    if (name > hinfo->name)
      { // new type gets chained
        NS_LOG_LOGIC ("New TypeId '" << name << "' getting chained.");
        hash = hash | HashChainFlag;
      }
    else
      { // chain old type
        NS_LOG_LOGIC ("Old TypeId '" << hinfo->name << "' getting chained.");
        uint32_t oldUid = GetUid (hinfo->hash);
        m_hashmap.erase (m_hashmap.find (hinfo->hash));
        hinfo->hash = hash | HashChainFlag;
        m_hashmap.insert (std::make_pair (hinfo->hash, oldUid));
        // leave new hash unchained
      }
  }

 struct IidInformation information;
  information.name = name;
  information.hash = hash;
  information.parent = 0;
  information.groupName = "";
  information.hasConstructor = false;
  information.mustHideFromDocumentation = false;
  m_information.push_back (information);
  uint32_t uid = m_information.size ();
  NS_ASSERT (uid <= 0xffff);

  // Add to both maps:
  m_namemap.insert (std::make_pair (name, uid));
  m_hashmap.insert (std::make_pair (hash, uid));
  return uid;
}

struct IidManager::IidInformation *
IidManager::LookupInformation (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  NS_ASSERT (uid <= m_information.size () && uid != 0);
  return const_cast<struct IidInformation *> (&m_information[uid-1]);
}

void 
IidManager::SetParent (uint16_t uid, uint16_t parent)
{
  NS_LOG_FUNCTION (this << uid << parent);
  NS_ASSERT (parent <= m_information.size ());
  struct IidInformation *information = LookupInformation (uid);
  information->parent = parent;
}
void 
IidManager::SetGroupName (uint16_t uid, std::string groupName)
{
  NS_LOG_FUNCTION (this << uid << groupName);
  struct IidInformation *information = LookupInformation (uid);
  information->groupName = groupName;
}
void
IidManager::HideFromDocumentation (uint16_t uid)
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  information->mustHideFromDocumentation = true;
}

void 
IidManager::AddConstructor (uint16_t uid, Callback<ObjectBase *> callback)
{
  NS_LOG_FUNCTION (this << uid << &callback);
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
  NS_LOG_FUNCTION (this << name);
  namemap_t::const_iterator it = m_namemap.find (name);
  if (it != m_namemap.end ())
    {
      return it->second;
    }
  else
    {
      return 0;
    }
}
uint16_t 
IidManager::GetUid (TypeId::hash_t hash) const
{
  hashmap_t::const_iterator it = m_hashmap.find (hash);
  if (it != m_hashmap.end ())
    {
    return it->second;
    }
  else
    { // hash not found 
      return 0;
    }
}
std::string 
IidManager::GetName (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->name;
}
TypeId::hash_t
IidManager::GetHash (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->hash;
}
uint16_t 
IidManager::GetParent (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->parent;
}
std::string 
IidManager::GetGroupName (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->groupName;
}

Callback<ObjectBase *> 
IidManager::GetConstructor (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
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
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->hasConstructor;
}

uint32_t 
IidManager::GetRegisteredN (void) const
{
  NS_LOG_FUNCTION (this);
  return m_information.size ();
}
uint16_t 
IidManager::GetRegistered (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  return i + 1;
}

bool
IidManager::HasAttribute (uint16_t uid,
                          std::string name)
{
  NS_LOG_FUNCTION (this << uid << name);
  struct IidInformation *information  = LookupInformation (uid);
  while (true)
    {
      for (std::vector<struct TypeId::AttributeInformation>::const_iterator i = information->attributes.begin ();
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
                          Ptr<const AttributeValue> initialValue,
                          Ptr<const AttributeAccessor> accessor,
                          Ptr<const AttributeChecker> checker)
{
  NS_LOG_FUNCTION (this << uid << name << help << flags << initialValue << accessor << checker);
  struct IidInformation *information = LookupInformation (uid);
  if (HasAttribute (uid, name))
    {
      NS_FATAL_ERROR ("Attribute \"" << name << "\" already registered on tid=\"" << 
                      information->name << "\"");
    }
  struct TypeId::AttributeInformation info;
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
                                     Ptr<const AttributeValue> initialValue)
{
  NS_LOG_FUNCTION (this << uid << i << initialValue);
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  information->attributes[i].initialValue = initialValue;
}



uint32_t 
IidManager::GetAttributeN (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->attributes.size ();
}
struct TypeId::AttributeInformation 
IidManager::GetAttribute(uint16_t uid, uint32_t i) const
{
  NS_LOG_FUNCTION (this << uid << i);
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->attributes.size ());
  return information->attributes[i];
}

bool
IidManager::HasTraceSource (uint16_t uid,
                            std::string name)
{
  NS_LOG_FUNCTION (this << uid << name);
  struct IidInformation *information  = LookupInformation (uid);
  while (true)
    {
      for (std::vector<struct TypeId::TraceSourceInformation>::const_iterator i = information->traceSources.begin ();
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
                            Ptr<const TraceSourceAccessor> accessor)
{
  NS_LOG_FUNCTION (this << uid << name << help << accessor);
  struct IidInformation *information  = LookupInformation (uid);
  if (HasTraceSource (uid, name))
    {
      NS_FATAL_ERROR ("Trace source \"" << name << "\" already registered on tid=\"" << 
                      information->name << "\"");
    }
  struct TypeId::TraceSourceInformation source;
  source.name = name;
  source.help = help;
  source.accessor = accessor;
  information->traceSources.push_back (source);
}
uint32_t 
IidManager::GetTraceSourceN (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->traceSources.size ();
}
struct TypeId::TraceSourceInformation 
IidManager::GetTraceSource(uint16_t uid, uint32_t i) const
{
  NS_LOG_FUNCTION (this << uid << i);
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->traceSources.size ());
  return information->traceSources[i];
}
bool 
IidManager::MustHideFromDocumentation (uint16_t uid) const
{
  NS_LOG_FUNCTION (this << uid);
  struct IidInformation *information = LookupInformation (uid);
  return information->mustHideFromDocumentation;
}

} // namespace ns3

namespace ns3 {

/*********************************************************************
 *         The TypeId class
 *********************************************************************/

TypeId::TypeId (const char *name)
{
  NS_LOG_FUNCTION (this << name);
  uint16_t uid = Singleton<IidManager>::Get ()->AllocateUid (name);
  NS_ASSERT (uid != 0);
  m_tid = uid;
}


TypeId::TypeId (uint16_t tid)
  : m_tid (tid)
{
  NS_LOG_FUNCTION (this << tid);
}
TypeId
TypeId::LookupByName (std::string name)
{
  NS_LOG_FUNCTION (name);
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (name);
  NS_ASSERT_MSG (uid != 0, "Assert in TypeId::LookupByName: " << name << " not found");
  return TypeId (uid);
}
bool
TypeId::LookupByNameFailSafe (std::string name, TypeId *tid)
{
  NS_LOG_FUNCTION (name << tid);
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (name);
  if (uid == 0)
    {
      return false;
    }
  *tid = TypeId (uid);
  return true;
}
TypeId
TypeId::LookupByHash (hash_t hash)
{
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (hash);
  NS_ASSERT_MSG (uid != 0, "Assert in TypeId::LookupByHash: 0x"
                 << std::hex << hash << std::dec << " not found");
  return TypeId (uid);
}
bool
TypeId::LookupByHashFailSafe (hash_t hash, TypeId *tid)
{
  uint16_t uid = Singleton<IidManager>::Get ()->GetUid (hash);
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
  NS_LOG_FUNCTION_NOARGS ();
  return Singleton<IidManager>::Get ()->GetRegisteredN ();
}
TypeId 
TypeId::GetRegistered (uint32_t i)
{
  NS_LOG_FUNCTION (i);
  return TypeId (Singleton<IidManager>::Get ()->GetRegistered (i));
}

bool
TypeId::LookupAttributeByName (std::string name, struct TypeId::AttributeInformation *info) const
{
  NS_LOG_FUNCTION (this << name << info);
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
  NS_LOG_FUNCTION (this << tid);
  Singleton<IidManager>::Get ()->SetParent (m_tid, tid.m_tid);
  return *this;
}
TypeId 
TypeId::SetGroupName (std::string groupName)
{
  NS_LOG_FUNCTION (this << groupName);
  Singleton<IidManager>::Get ()->SetGroupName (m_tid, groupName);
  return *this;
}
TypeId 
TypeId::GetParent (void) const
{
  NS_LOG_FUNCTION (this);
  uint16_t parent = Singleton<IidManager>::Get ()->GetParent (m_tid);
  return TypeId (parent);
}
bool 
TypeId::HasParent (void) const
{
  NS_LOG_FUNCTION (this);
  uint16_t parent = Singleton<IidManager>::Get ()->GetParent (m_tid);
  return parent != m_tid;
}
bool 
TypeId::IsChildOf (TypeId other) const
{
  NS_LOG_FUNCTION (this << other);
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
  NS_LOG_FUNCTION (this);
  std::string groupName = Singleton<IidManager>::Get ()->GetGroupName (m_tid);
  return groupName;
}

std::string 
TypeId::GetName (void) const
{
  NS_LOG_FUNCTION (this);
  std::string name = Singleton<IidManager>::Get ()->GetName (m_tid);
  return name;
}

TypeId::hash_t
TypeId::GetHash (void) const
{
  hash_t hash = Singleton<IidManager>::Get ()->GetHash (m_tid);
  return hash;
}

bool 
TypeId::HasConstructor (void) const
{
  NS_LOG_FUNCTION (this);
  bool hasConstructor = Singleton<IidManager>::Get ()->HasConstructor (m_tid);
  return hasConstructor;
}

void
TypeId::DoAddConstructor (Callback<ObjectBase *> cb)
{
  NS_LOG_FUNCTION (this << &cb);
  Singleton<IidManager>::Get ()->AddConstructor (m_tid, cb);
}

TypeId 
TypeId::AddAttribute (std::string name,
                      std::string help, 
                      const AttributeValue &initialValue,
                      Ptr<const AttributeAccessor> accessor,
                      Ptr<const AttributeChecker> checker)
{
  NS_LOG_FUNCTION (this << name << help << &initialValue << accessor << checker);
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
  NS_LOG_FUNCTION (this << name << help << flags << &initialValue << accessor << checker);
  Singleton<IidManager>::Get ()->AddAttribute (m_tid, name, help, flags, initialValue.Copy (), accessor, checker);
  return *this;
}

bool 
TypeId::SetAttributeInitialValue(uint32_t i, 
                                 Ptr<const AttributeValue> initialValue)
{
  NS_LOG_FUNCTION (this << i << initialValue);
  Singleton<IidManager>::Get ()->SetAttributeInitialValue (m_tid, i, initialValue);
  return true;
}


Callback<ObjectBase *> 
TypeId::GetConstructor (void) const
{
  NS_LOG_FUNCTION (this);
  Callback<ObjectBase *>  cb = Singleton<IidManager>::Get ()->GetConstructor (m_tid);
  return cb;
}

bool 
TypeId::MustHideFromDocumentation (void) const
{
  NS_LOG_FUNCTION (this);
  bool mustHide = Singleton<IidManager>::Get ()->MustHideFromDocumentation (m_tid);
  return mustHide;
}

uint32_t 
TypeId::GetAttributeN (void) const
{
  NS_LOG_FUNCTION (this);
  uint32_t n = Singleton<IidManager>::Get ()->GetAttributeN (m_tid);
  return n;
}
struct TypeId::AttributeInformation 
TypeId::GetAttribute(uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  return Singleton<IidManager>::Get ()->GetAttribute(m_tid, i);
}
std::string 
TypeId::GetAttributeFullName (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  struct TypeId::AttributeInformation info = GetAttribute(i);
  return GetName () + "::" + info.name;
}

uint32_t 
TypeId::GetTraceSourceN (void) const
{
  NS_LOG_FUNCTION (this);
  return Singleton<IidManager>::Get ()->GetTraceSourceN (m_tid);
}
struct TypeId::TraceSourceInformation 
TypeId::GetTraceSource(uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  return Singleton<IidManager>::Get ()->GetTraceSource(m_tid, i);
}

TypeId 
TypeId::AddTraceSource (std::string name,
                        std::string help,
                        Ptr<const TraceSourceAccessor> accessor)
{
  NS_LOG_FUNCTION (this << name << help << accessor);
  Singleton<IidManager>::Get ()->AddTraceSource (m_tid, name, help, accessor);
  return *this;
}

TypeId 
TypeId::HideFromDocumentation (void)
{
  NS_LOG_FUNCTION (this);
  Singleton<IidManager>::Get ()->HideFromDocumentation (m_tid);
  return *this;
}


Ptr<const TraceSourceAccessor> 
TypeId::LookupTraceSourceByName (std::string name) const
{
  NS_LOG_FUNCTION (this << name);
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
  NS_LOG_FUNCTION (this);
  return m_tid;
}
void 
TypeId::SetUid (uint16_t tid)
{
  NS_LOG_FUNCTION (this << tid);
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
