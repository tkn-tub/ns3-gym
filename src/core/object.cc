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
#include "trace-resolver.h"
#include "value.h"
#include "log.h"
#include <vector>
#include <sstream>

NS_LOG_COMPONENT_DEFINE ("Object");

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
  void SetTypeName (uint16_t uid, std::string typeName);
  void SetGroupName (uint16_t uid, std::string groupName);
  void AddConstructor (uint16_t uid, ns3::CallbackBase callback, uint32_t nArguments);
  uint16_t GetUid (std::string name) const;
  std::string GetName (uint16_t uid) const;
  uint16_t GetParent (uint16_t uid) const;
  std::string GetTypeName (uint16_t uid) const;
  std::string GetGroupName (uint16_t uid) const;
  ns3::CallbackBase GetConstructor (uint16_t uid, uint32_t nArguments);
  bool HasConstructor (uint16_t uid);
  uint32_t GetRegisteredN (void);
  uint16_t GetRegistered (uint32_t i);
  void AddParameter (uint16_t uid, 
                     std::string name,
                     std::string help, 
                     uint32_t flags,
                     ns3::Ptr<const ns3::ParamSpec> spec);
  uint32_t GetParametersN (uint16_t uid) const;
  std::string GetParameterName (uint16_t uid, uint32_t i) const;
  uint32_t GetParameterFlags (uint16_t uid, uint32_t i) const;
  uint32_t GetParameterUid (uint16_t uid, uint32_t i) const;
  ns3::Ptr<const ns3::ParamSpec> GetParameterParamSpec (uint16_t uid, uint32_t i) const;
private:
  struct ConstructorInformation {
    ns3::CallbackBase cb;
    uint32_t nArguments;
  };
  struct ParameterInformation {
    std::string name;
    std::string help;
    uint32_t flags;
    uint32_t uid;
    ns3::Ptr<const ns3::ParamSpec> param;
  };
  struct IidInformation {
    std::string name;
    uint16_t parent;
    std::string typeName;
    std::string groupName;
    std::vector<struct ConstructorInformation> constructors;
    std::vector<struct ParameterInformation> parameters;
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
  information.typeName = "";
  information.groupName = "";
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
IidManager::SetTypeName (uint16_t uid, std::string typeName)
{
  struct IidInformation *information = LookupInformation (uid);
  information->typeName = typeName;
}
void 
IidManager::SetGroupName (uint16_t uid, std::string groupName)
{
  struct IidInformation *information = LookupInformation (uid);
  information->groupName = groupName;
}

void 
IidManager::AddConstructor (uint16_t uid, ns3::CallbackBase callback, uint32_t nArguments)
{
  struct IidInformation *information = LookupInformation (uid);
  struct ConstructorInformation constructor;
  constructor.cb = callback;
  constructor.nArguments = nArguments;
  for (std::vector<struct ConstructorInformation>::const_iterator i = information->constructors.begin ();
       i != information->constructors.end (); i++)
    {
      if (i->nArguments == nArguments)
        {
          NS_FATAL_ERROR ("registered two constructors on the same type with the same number of arguments.");
          break;
        }
    }
  information->constructors.push_back (constructor);
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
IidManager::GetTypeName (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->typeName;
}
std::string 
IidManager::GetGroupName (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->groupName;
}

ns3::CallbackBase 
IidManager::GetConstructor (uint16_t uid, uint32_t nArguments)
{
  struct IidInformation *information = LookupInformation (uid);
  for (std::vector<struct ConstructorInformation>::const_iterator i = information->constructors.begin ();
       i != information->constructors.end (); i++)
    {
      if (i->nArguments == nArguments)
        {
          return i->cb;
        } 
    }
  NS_FATAL_ERROR ("Requested constructor with "<<nArguments<<" arguments not found");
  return ns3::CallbackBase ();
}

bool 
IidManager::HasConstructor (uint16_t uid)
{
  struct IidInformation *information = LookupInformation (uid);
  return !information->constructors.empty ();
}

uint32_t 
IidManager::GetRegisteredN (void)
{
  return m_information.size ();
}
uint16_t 
IidManager::GetRegistered (uint32_t i)
{
  return i + 1;
}

void 
IidManager::AddParameter (uint16_t uid, 
                          std::string name,
                          std::string help, 
                          uint32_t flags,
                          ns3::Ptr<const ns3::ParamSpec> spec)
{
  struct IidInformation *information = LookupInformation (uid);
  for (std::vector<struct ParameterInformation>::const_iterator j = information->parameters.begin ();
       j != information->parameters.end (); j++)
    {
      if (j->name == name)
        {
          NS_FATAL_ERROR ("Registered the same parameter twice name=\""<<name<<"\" in TypeId=\""<<information->name<<"\"");
          return;
        }
    }
  struct ParameterInformation param;
  param.name = name;
  param.help = help;
  param.flags = flags;
  param.param = spec;
  information->parameters.push_back (param);
}


uint32_t 
IidManager::GetParametersN (uint16_t uid) const
{
  struct IidInformation *information = LookupInformation (uid);
  return information->parameters.size ();
}
std::string 
IidManager::GetParameterName (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->parameters.size ());
  return information->parameters[i].name;
}
uint32_t
IidManager::GetParameterFlags (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->parameters.size ());
  return information->parameters[i].flags;
}
ns3::Ptr<const ns3::ParamSpec>
IidManager::GetParameterParamSpec (uint16_t uid, uint32_t i) const
{
  struct IidInformation *information = LookupInformation (uid);
  NS_ASSERT (i < information->parameters.size ());
  return information->parameters[i].param;
}


} // anonymous namespace

/*********************************************************************
 *         The TypeId TraceResolver
 *********************************************************************/

namespace ns3 {

class TypeIdTraceResolver : public TraceResolver
{
public:
  TypeIdTraceResolver (Ptr<const Object> aggregate);
  virtual void Connect (std::string path, CallbackBase const &cb, const TraceContext &context);
  virtual void Disconnect (std::string path, CallbackBase const &cb);
  virtual void CollectSources (std::string path, const TraceContext &context, 
                               SourceCollection *collection);
  virtual void TraceAll (std::ostream &os, const TraceContext &context);
private:
  Ptr<const Object> ParseForInterface (std::string path);
  Ptr<const Object> m_aggregate;
};

TypeIdTraceResolver::TypeIdTraceResolver (Ptr<const Object> aggregate)
  : m_aggregate (aggregate)
{}
Ptr<const Object>
TypeIdTraceResolver::ParseForInterface (std::string path)
{
  std::string element = GetElement (path);
  std::string::size_type dollar_pos = element.find ("$");
  if (dollar_pos != 0)
    {
      return 0;
    }
  std::string interfaceName = element.substr (1, std::string::npos);
  TypeId interfaceId = TypeId::LookupByName (interfaceName);
  Ptr<Object> interface = m_aggregate->QueryInterface<Object> (interfaceId);
  return interface;
}
void  
TypeIdTraceResolver::Connect (std::string path, CallbackBase const &cb, const TraceContext &context)
{
  Ptr<const Object> interface = ParseForInterface (path);
  if (interface != 0)
    {
      interface->GetTraceResolver ()->Connect (GetSubpath (path), cb, context);
    }
}
void 
TypeIdTraceResolver::Disconnect (std::string path, CallbackBase const &cb)
{
  Ptr<const Object> interface = ParseForInterface (path);
  if (interface != 0)
    {
      interface->TraceDisconnect (GetSubpath (path), cb);
    }
}
void 
TypeIdTraceResolver::CollectSources (std::string path, const TraceContext &context, 
                                          SourceCollection *collection)
{
  m_aggregate->DoCollectSources (path, context, collection);
}
void 
TypeIdTraceResolver::TraceAll (std::ostream &os, const TraceContext &context)
{
  m_aggregate->DoTraceAll (os, context);
}

/*********************************************************************
 *         The TypeId class
 *********************************************************************/

TypeId::TypeId (std::string name)
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
Ptr<const ParamSpec>
TypeId::LookupParamSpecByFullName (std::string fullName)
{
  std::string::size_type pos = fullName.find ("::");
  if (pos == std::string::npos)
    {
      return 0;
    }
  std::string tidName = fullName.substr (0, pos);
  std::string paramName = fullName.substr (pos+2, fullName.size () - (pos+2));
  TypeId tid = LookupByName (tidName);
  return tid.LookupParamSpecByName (paramName);
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

Ptr<const ParamSpec>
TypeId::LookupParamSpecByName (std::string name) const
{
  for (uint32_t i = 0; i < GetParametersN (); i++)
    {
      std::string paramName = GetParameterName (i);
      if (paramName == name)
        {
          return GetParameterParamSpec (i);
        }
    }
  return 0;
}
Ptr<const ParamSpec>
TypeId::LookupParamSpecByPosition (uint32_t i) const
{
  uint32_t cur = 0;
  TypeId tid = TypeId (0);
  TypeId nextTid = *this;
  do {
    tid = nextTid;
    for (uint32_t j = 0; j < tid.GetParametersN (); j++)
      {
        if (cur == i)
          {
            return tid.GetParameterParamSpec (j);
          }
        cur++;
      }
    nextTid = tid.GetParent ();
  } while (nextTid != tid);
  return 0;
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
TypeId::SetTypeName (std::string typeName)
{
  Singleton<IidManager>::Get ()->SetTypeName (m_tid, typeName);
  return *this;
}
TypeId 
TypeId::GetParent (void) const
{
  uint16_t parent = Singleton<IidManager>::Get ()->GetParent (m_tid);
  return TypeId (parent);
}
std::string 
TypeId::GetGroupName (void) const
{
  std::string groupName = Singleton<IidManager>::Get ()->GetGroupName (m_tid);
  return groupName;
}
std::string 
TypeId::GetTypeName (void) const
{
  std::string typeName = Singleton<IidManager>::Get ()->GetTypeName (m_tid);
  return typeName;
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
TypeId::DoAddConstructor (CallbackBase cb, uint32_t nArguments)
{
  Singleton<IidManager>::Get ()->AddConstructor (m_tid, cb, nArguments);
}

TypeId 
TypeId::AddParameter (std::string name,
                      std::string help, 
                      Ptr<const ParamSpec> param)
{
  Singleton<IidManager>::Get ()->AddParameter (m_tid, name, help, 0, param);
  return *this;
}

TypeId 
TypeId::AddParameter (std::string name,
                      std::string help, 
                      uint32_t flags,
                      Ptr<const ParamSpec> param)
{
  Singleton<IidManager>::Get ()->AddParameter (m_tid, name, help, flags, param);
  return *this;
}


CallbackBase
TypeId::LookupConstructor (uint32_t nArguments) const
{
  CallbackBase constructor = Singleton<IidManager>::Get ()->GetConstructor (m_tid, nArguments);
  return constructor;
}

Ptr<Object> 
TypeId::CreateObject (void) const
{
  return CreateObject (Parameters ());
}
Ptr<Object> 
TypeId::CreateObject (const Parameters &parameters) const
{
  CallbackBase cb = LookupConstructor (0);
  Callback<Ptr<Object>,const Parameters &> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (parameters);
  return object;  
}

uint32_t 
TypeId::GetParametersN (void) const
{
  uint32_t n = Singleton<IidManager>::Get ()->GetParametersN (m_tid);
  return n;
}
std::string 
TypeId::GetParameterName (uint32_t i) const
{
  std::string name = Singleton<IidManager>::Get ()->GetParameterName (m_tid, i);
  return name;
}
std::string 
TypeId::GetParameterFullName (uint32_t i) const
{
  return GetName () + "::" + GetParameterName (i);
}
Ptr<const ParamSpec>
TypeId::GetParameterParamSpec (uint32_t i) const
{
  // Used exclusively by the Object class.
  Ptr<const ParamSpec> param = Singleton<IidManager>::Get ()->GetParameterParamSpec (m_tid, i);
  return param;
}


bool operator == (TypeId a, TypeId b)
{
  return a.m_tid == b.m_tid;
}

bool operator != (TypeId a, TypeId b)
{
  return a.m_tid != b.m_tid;
}

/*********************************************************************
 *         The Parameters container implementation
 *********************************************************************/

Parameters::Parameters ()
{}

Parameters::Parameters (const Parameters &o)
{
  for (Params::const_iterator i = o.m_parameters.begin (); i != o.m_parameters.end (); i++)
    {
      struct Param param;
      param.spec = i->spec;
      param.value = i->value.Copy ();
      m_parameters.push_back (param);
    }
}
Parameters &
Parameters::operator = (const Parameters &o)
{
  Reset ();
  for (Params::const_iterator i = o.m_parameters.begin (); i != o.m_parameters.end (); i++)
    {
      struct Param param;
      param.spec = i->spec;
      param.value = i->value.Copy ();
      m_parameters.push_back (param);
    }
  return *this;
}
Parameters::~Parameters ()
{
  Reset ();
}

bool 
Parameters::Set (std::string name, PValue value)
{
  Ptr<const ParamSpec> spec = TypeId::LookupParamSpecByFullName (name);
  bool ok = DoSet (spec, value);
  return ok;
}
bool 
Parameters::Set (std::string name, std::string value)
{
  Ptr<const ParamSpec> spec = TypeId::LookupParamSpecByFullName (name);
  bool ok = DoSet (spec,value);
  return ok;
}
void 
Parameters::SetWithTid (TypeId tid, std::string name, std::string value)
{
  Ptr<const ParamSpec> spec = tid.LookupParamSpecByName (name);
  DoSet (spec, value);
}
void 
Parameters::SetWithTid (TypeId tid, std::string name, PValue value)
{
  Ptr<const ParamSpec> spec = tid.LookupParamSpecByName (name);
  DoSet (spec, value);
}
void 
Parameters::SetWithTid (TypeId tid, uint32_t position, std::string value)
{
  Ptr<const ParamSpec> spec = tid.LookupParamSpecByPosition (position);
  DoSet (spec, value);
}
void 
Parameters::SetWithTid (TypeId tid, uint32_t position, PValue value)
{
  Ptr<const ParamSpec> spec = tid.LookupParamSpecByPosition (position);
  DoSet (spec, value);
}

void
Parameters::DoSetOne (Ptr<const ParamSpec> spec, PValue value)
{
  // get rid of any previous value stored in this
  // vector of values.
  for (Params::iterator k = m_parameters.begin (); k != m_parameters.end (); k++)
    {
      if (k->spec == spec)
        {
          m_parameters.erase (k);
          break;
        }
    }
  // store the new value.
  struct Param p;
  p.spec = spec;
  p.value = value.Copy ();
  m_parameters.push_back (p);
}
bool
Parameters::DoSet (Ptr<const ParamSpec> spec, PValue value)
{
  if (spec == 0)
    {
      return false;
    }
  bool ok = spec->Check (value);
  if (!ok)
    {
      return false;
    }
  DoSetOne (spec, value);
  return true;
}
bool
Parameters::DoSet (Ptr<const ParamSpec> spec, std::string value)
{
  if (spec == 0)
    {
      return false;
    }
  PValue v = spec->CreateValue ();
  bool ok = v.DeserializeFromString (value, spec);
  if (!ok)
    {
      return false;
    }
  ok = spec->Check (v);
  if (!ok)
    {
      return false;
    }
  DoSetOne (spec, v);
  return true;
}
void 
Parameters::Reset (void)
{
  m_parameters.clear ();
}
Parameters *
Parameters::GetGlobal (void)
{
  return Singleton<Parameters>::Get ();
}

std::string
Parameters::LookupParameterFullNameByParamSpec (Ptr<const ParamSpec> spec) const
{
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      TypeId tid = TypeId::GetRegistered (i);
      for (uint32_t j = 0; j < tid.GetParametersN (); j++)
        {
          if (spec == tid.GetParameterParamSpec (j))
            {
              return tid.GetParameterFullName (j);
            }
        }
    }
  NS_FATAL_ERROR ("Could not find requested ParamSpec.");
  // quiet compiler.
  return "";
}

std::string 
Parameters::SerializeToString (void) const
{
  std::ostringstream oss;
  for (Params::const_iterator i = m_parameters.begin (); i != m_parameters.end (); i++)
    {
      std::string name = LookupParameterFullNameByParamSpec (i->spec);
      oss << name << "=" << i->value.SerializeToString (PeekPointer (i->spec));
      if (i != m_parameters.end ())
        {
          oss << "|";
        }
    }  
  return oss.str ();
}
bool 
Parameters::DeserializeFromString (std::string str)
{
  Reset ();

  std::string::size_type cur;
  cur = 0;
  do {
    std::string::size_type equal = str.find ("=", cur);
    if (equal == std::string::npos)
      {
        // XXX: invalid parameter.
        break;
      }
    else
      {
        std::string name = str.substr (cur, equal-cur);
        Ptr<const ParamSpec> spec = TypeId::LookupParamSpecByFullName (name);
        if (spec == 0)
          {
            // XXX invalid name.
            break;
          }
        else
          {
            std::string::size_type next = str.find ("|", cur);
            std::string value;
            if (next == std::string::npos)
              {
                value = str.substr (equal+1, str.size () - (equal+1));
                cur = str.size ();
              }
            else
              {
                value = str.substr (equal+1, next - (equal+1));
                cur++;
              }
            PValue val = spec->CreateValue ();
            bool ok = val.DeserializeFromString (value, spec);
            if (!ok)
              {
                // XXX invalid value
                break;
              }
            else
              {
                DoSetOne (spec, val);
              }
          }
      }
  } while (cur != str.size ());

  return true;
}


/*********************************************************************
 *         The Object implementation
 *********************************************************************/

NS_OBJECT_ENSURE_REGISTERED (Object);

static TypeId
GetObjectIid (void)
{
  TypeId tid = TypeId ("Object");
  tid.SetParent (tid);
  return tid;
}

TypeId 
Object::GetTypeId (void)
{
  static TypeId tid = GetObjectIid ();
  return tid;
}


Object::Object ()
  : m_count (1),
    m_tid (Object::GetTypeId ()),
    m_disposed (false),
    m_collecting (false),
    m_next (this)
{}
Object::~Object () 
{
  m_next = 0;
}
void
Object::Construct (const Parameters &parameters)
{
  // loop over the inheritance tree back to the Object base class.
  TypeId tid = m_tid;
  do {
    // loop over all parameters in object type
    NS_LOG_DEBUG ("construct tid="<<tid.GetName ()<<", params="<<tid.GetParametersN ());
    for (uint32_t i = 0; i < tid.GetParametersN (); i++)
      {
        Ptr<const ParamSpec> paramSpec = tid.GetParameterParamSpec (i);
        NS_LOG_DEBUG ("try to construct \""<< tid.GetName ()<<"::"<<
                      tid.GetParameterName (i)<<"\"");
        bool found = false;
        // is this parameter stored in this Parameters instance ?
        for (Parameters::Params::const_iterator j = parameters.m_parameters.begin ();
             j != parameters.m_parameters.end (); j++)
          {
            if (j->spec == paramSpec)
              {
                // We have a matching parameter value.
                paramSpec->Set (this, j->value);
                NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                              tid.GetParameterName (i)<<"\"");
                found = true;
                break;
              }
          }
        if (!found)
          {
            // is this parameter stored in the global instance instance ?
            for (Parameters::Params::const_iterator j = Parameters::GetGlobal ()->m_parameters.begin ();
                 j != Parameters::GetGlobal ()->m_parameters.end (); j++)
              {
                if (j->spec == paramSpec)
                  {
                    // We have a matching parameter value.
                    paramSpec->Set (this, j->value);
                    NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                                  tid.GetParameterName (i)<<"\" from global");
                    found = true;
                    break;
                  }
              }
          }
        if (!found)
          {
            // No matching parameter value so we set the default value.
            PValue initial = paramSpec->GetInitialValue ();
            paramSpec->Set (this, initial);
            NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                          tid.GetParameterName (i)<<"\" from local");
          }
      }
    tid = tid.GetParent ();
  } while (tid != Object::GetTypeId ());
  NotifyConstructionCompleted ();
}
bool
Object::DoSet (std::string name, PValue value)
{
  Ptr<const ParamSpec> spec = m_tid.LookupParamSpecByName (name);
  if (spec == 0)
    {
      return false;
    }
  bool ok = spec->Check (value);
  if (!ok)
    {
      return false;
    }
  ok = spec->Set (this, value);
  return ok;
}
bool
Object::Set (std::string name, PValue value)
{
  return DoSet (name, value);
}
bool
Object::Set (std::string name, std::string value)
{
  Ptr<const ParamSpec> spec = m_tid.LookupParamSpecByName (name);
  if (spec == 0)
    {
      return false;
    }
  PValue v = spec->CreateValue ();
  bool ok = v.DeserializeFromString (value, spec);
  if (!ok)
    {
      return false;
    }
  ok = spec->Check (v);
  if (!ok)
    {
      return false;
    }
  ok = spec->Set (this, v);
  return ok;
}
bool 
Object::Get (std::string name, std::string &value) const
{
  Ptr<const ParamSpec> paramSpec = m_tid.LookupParamSpecByName (name);
  if (paramSpec == 0)
    {
      return false;
    }
  PValue v = paramSpec->CreateValue ();
  bool ok = paramSpec->Get (this, v);
  if (ok)
    {
      value = v.SerializeToString (paramSpec);
    }
  return ok;
}

PValue
Object::Get (std::string name) const
{
  Ptr<const ParamSpec> paramSpec = m_tid.LookupParamSpecByName (name);
  if (paramSpec == 0)
    {
      return PValue ();
    }
  PValue value = paramSpec->CreateValue ();
  bool ok = paramSpec->Get (this, value);
  if (!ok)
    {
      return PValue ();
    }
  return value;
}

bool 
Object::DoGet (std::string name, PValue parameter) const
{
  Ptr<const ParamSpec> paramSpec = m_tid.LookupParamSpecByName (name);
  if (paramSpec == 0)
    {
      return false;
    }
  bool ok = paramSpec->Get (this, parameter);
  return ok;
}


Ptr<Object>
Object::DoQueryInterface (TypeId tid) const
{
  NS_ASSERT (CheckLoose ());
  const Object *currentObject = this;
  do {
    NS_ASSERT (currentObject != 0);
    TypeId cur = currentObject->m_tid;
    while (cur != tid && cur != Object::GetTypeId ())
      {
        cur = cur.GetParent ();
      }
    if (cur == tid)
      {
        return const_cast<Object *> (currentObject);
      }
    currentObject = currentObject->m_next;
  } while (currentObject != this);
  return 0;
}
void 
Object::Dispose (void)
{
  Object *current = this;
  do {
    NS_ASSERT (current != 0);
    NS_ASSERT (!current->m_disposed);
    current->DoDispose ();
    current->m_disposed = true;
    current = current->m_next;
  } while (current != this);
}
void
Object::NotifyConstructionCompleted (void)
{}
void 
Object::AddInterface (Ptr<Object> o)
{
  NS_ASSERT (!m_disposed);
  NS_ASSERT (!o->m_disposed);
  NS_ASSERT (CheckLoose ());
  NS_ASSERT (o->CheckLoose ());
  Object *other = PeekPointer (o);
  Object *next = m_next;
  m_next = other->m_next;
  other->m_next = next;
  NS_ASSERT (CheckLoose ());
  NS_ASSERT (o->CheckLoose ());
}

void 
Object::TraceConnect (std::string path, const CallbackBase &cb) const
{
  NS_ASSERT (CheckLoose ());
  GetTraceResolver ()->Connect (path, cb, TraceContext ());
}
void 
Object::TraceDisconnect (std::string path, const CallbackBase &cb) const
{
  NS_ASSERT (CheckLoose ());
  GetTraceResolver ()->Disconnect (path, cb);
}

void 
Object::SetTypeId (TypeId tid)
{
  NS_ASSERT (Check ());
  m_tid = tid;
}

void
Object::DoDispose (void)
{
  NS_ASSERT (!m_disposed);
}

Ptr<TraceResolver>
Object::GetTraceResolver (void) const
{
  NS_ASSERT (CheckLoose ());
  Ptr<TypeIdTraceResolver> resolver =
    Create<TypeIdTraceResolver> (this);
  return resolver;
}

bool 
Object::Check (void) const
{
  return (m_count > 0);
}

/* In some cases, when an event is scheduled against a subclass of
 * Object, and if no one owns a reference directly to this object, the
 * object is alive, has a refcount of zero and the method ran when the
 * event expires runs against the raw pointer which means that we are
 * manipulating an object with a refcount of zero.  So, instead we
 * check the aggregate reference count.
 */
bool 
Object::CheckLoose (void) const
{
  uint32_t refcount = 0;
  const Object *current = this;
  do
    {
      refcount += current->m_count;
      current = current->m_next;
    }
  while (current != this);

  return (refcount > 0);
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

void 
Object::DoCollectSources (std::string path, const TraceContext &context, 
                          TraceResolver::SourceCollection *collection) const
{
  const Object *current;
  current = this;
  do {
    if (current->m_collecting)
      {
        return;
      }
    current = current->m_next;
  } while (current != this);

  m_collecting = true;

  current = this->m_next;
  while (current != this)
    {
      NS_ASSERT (current != 0);
      NS_LOG_LOGIC ("collect current=" << current);
      TypeId cur = current->m_tid;
      while (cur != Object::GetTypeId ())
        {
          std::string name = cur.GetName ();
          std::string fullpath = path;
          fullpath.append ("/$");
          fullpath.append (name);
          NS_LOG_LOGIC("collect: " << fullpath);
          current->GetTraceResolver ()->CollectSources (fullpath, context, collection);
          cur = cur.GetParent ();
        }
      current = current->m_next;
    }

  m_collecting = false;
}
void 
Object::DoTraceAll (std::ostream &os, const TraceContext &context) const
{
  const Object *current;
  current = this;
  do {
    if (current->m_collecting)
      {
        return;
      }
    current = current->m_next;
  } while (current != this);

  m_collecting = true;

  current = this->m_next;
  while (current != this)
    {
      NS_ASSERT (current != 0);
      current->GetTraceResolver ()->TraceAll (os, context);
      current = current->m_next;
    }

  m_collecting = false;
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "test.h"
#include "sv-trace-source.h"
#include "composite-trace-resolver.h"

namespace {

class BaseA : public ns3::Object
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("BaseA")
      .SetParent (Object::GetTypeId ())
      .AddConstructor<BaseA> ();
    return tid;
  }
  BaseA ()
  {}
  void BaseGenerateTrace (int16_t v)
  { m_source = v; }
  virtual void Dispose (void) {}
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("basea-x", ns3::TraceDoc ("test source"), m_source);
    resolver->SetParentResolver (Object::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_source;
};

class DerivedA : public BaseA
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("DerivedA")
      .SetParent (BaseA::GetTypeId ())
      .AddConstructor<DerivedA> ();
    return tid;
  }
  DerivedA ()
  {}
  void DerivedGenerateTrace (int16_t v)
  { m_sourceDerived = v; }
  virtual void Dispose (void) {
    BaseA::Dispose ();
  }
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("deriveda-x", ns3::TraceDoc ("test source"), m_sourceDerived);
    resolver->SetParentResolver (BaseA::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_sourceDerived;
};

class BaseB : public ns3::Object
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("BaseB")
      .SetParent (Object::GetTypeId ())
      .AddConstructor<BaseB> ();
    return tid;
  }
  BaseB ()
  {}
  void BaseGenerateTrace (int16_t v)
  { m_source = v; }
  virtual void Dispose (void) {}
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("baseb-x", ns3::TraceDoc ("test source"), m_source);
    resolver->SetParentResolver (Object::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_source;
};

class DerivedB : public BaseB
{
public:
  static ns3::TypeId GetTypeId (void) {
    static ns3::TypeId tid = ns3::TypeId ("DerivedB")
      .SetParent (BaseB::GetTypeId ())
      .AddConstructor<DerivedB> ();
    return tid;
  }
  DerivedB ()
  {}
  void DerivedGenerateTrace (int16_t v)
  { m_sourceDerived = v; }
  virtual void Dispose (void) {
    BaseB::Dispose ();
  }
  virtual ns3::Ptr<ns3::TraceResolver> GetTraceResolver (void) const
  {
    ns3::Ptr<ns3::CompositeTraceResolver> resolver = 
      ns3::Create<ns3::CompositeTraceResolver> ();
    resolver->AddSource ("derivedb-x", ns3::TraceDoc ("test source"), m_sourceDerived);
    resolver->SetParentResolver (BaseB::GetTraceResolver ());
    return resolver;
  }
  ns3::SVTraceSource<int16_t> m_sourceDerived;
};

NS_OBJECT_ENSURE_REGISTERED (BaseA);
NS_OBJECT_ENSURE_REGISTERED (DerivedA);
NS_OBJECT_ENSURE_REGISTERED (BaseB);
NS_OBJECT_ENSURE_REGISTERED (DerivedB);

} // namespace anonymous

namespace ns3 {

class ObjectTest : public Test
{
public:
  ObjectTest ();
  virtual bool RunTests (void);
private:
  void BaseATrace (const TraceContext &context, int64_t oldValue, int64_t newValue);
  void DerivedATrace (const TraceContext &context, int64_t oldValue, int64_t newValue);
  void BaseBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue);
  void DerivedBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue);

  bool m_baseATrace;
  bool m_derivedATrace;
  bool m_baseBTrace;
  bool m_derivedBTrace;
};

ObjectTest::ObjectTest ()
  : Test ("Object")
{}
void 
ObjectTest::BaseATrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_baseATrace = true;
}
void 
ObjectTest::DerivedATrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_derivedATrace = true;
}
void 
ObjectTest::BaseBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_baseBTrace = true;
}
void 
ObjectTest::DerivedBTrace (const TraceContext &context, int64_t oldValue, int64_t newValue)
{
  m_derivedBTrace = true;
}

bool 
ObjectTest::RunTests (void)
{
  bool result = true;

  Ptr<BaseA> baseA = CreateObject<BaseA> ();
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (DerivedA::GetTypeId ()), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedA> (), 0);
  baseA = CreateObject<DerivedA> ();
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (), baseA);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<BaseA> (DerivedA::GetTypeId ()), baseA);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<DerivedA> (), 0);

  baseA = CreateObject<BaseA> ();
  Ptr<BaseB> baseB = CreateObject<BaseB> ();
  Ptr<BaseB> baseBCopy = baseB;
  baseA->AddInterface (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseB> (), 0);
  NS_TEST_ASSERT_EQUAL (baseA->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseB> (), 0);
  NS_TEST_ASSERT_EQUAL (baseB->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_EQUAL (baseB->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<BaseA> (), 0);

  baseA = CreateObject<DerivedA> ();
  baseB = CreateObject<DerivedB> ();
  baseBCopy = baseB;
  baseA->AddInterface (baseB);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseA->QueryInterface<BaseB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<DerivedA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseBCopy->QueryInterface<BaseA> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<DerivedB> (), 0);
  NS_TEST_ASSERT_UNEQUAL (baseB->QueryInterface<BaseB> (), 0)

  baseA = CreateObject<BaseA> ();
  baseB = CreateObject<BaseB> ();
  baseA->AddInterface (baseB);
  baseA = 0;
  baseA = baseB->QueryInterface<BaseA> ();

  baseA = CreateObject<BaseA> ();
  baseA->TraceConnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (1);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseB = CreateObject<BaseB> ();
  baseB->TraceConnect ("/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));
  m_baseBTrace = false;
  baseB->BaseGenerateTrace (2);
  NS_TEST_ASSERT (m_baseBTrace);
  baseB->TraceDisconnect ("/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));

  baseA->AddInterface (baseB);

  baseA->TraceConnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (3);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseA->TraceConnect ("/$BaseB/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));
  m_baseBTrace = false;
  baseB->BaseGenerateTrace (4);
  NS_TEST_ASSERT (m_baseBTrace);
  baseA->TraceDisconnect ("/$BaseB/baseb-x",  MakeCallback (&ObjectTest::BaseBTrace, this));
  m_baseBTrace = false;
  baseB->BaseGenerateTrace (5);
  NS_TEST_ASSERT (!m_baseBTrace);

  baseB->TraceConnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (6);
  NS_TEST_ASSERT (m_baseATrace);
  baseB->TraceDisconnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseA->TraceConnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_baseATrace = false;
  baseA->BaseGenerateTrace (7);
  NS_TEST_ASSERT (m_baseATrace);
  baseA->TraceDisconnect ("/$BaseA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  Ptr<DerivedA> derivedA;
  derivedA = CreateObject<DerivedA> ();
  baseB = CreateObject<BaseB> ();
  derivedA->AddInterface (baseB);
  baseB->TraceConnect ("/$DerivedA/deriveda-x", MakeCallback (&ObjectTest::DerivedATrace, this));
  baseB->TraceConnect ("/$DerivedA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));
  m_derivedATrace = false;
  m_baseATrace = false;
  derivedA->DerivedGenerateTrace (8);
  derivedA->BaseGenerateTrace (9);
  NS_TEST_ASSERT (m_derivedATrace);
  NS_TEST_ASSERT (m_baseATrace);
  baseB->TraceDisconnect ("/$DerivedA/deriveda-x", MakeCallback (&ObjectTest::BaseATrace, this));
  baseB->TraceDisconnect ("/$DerivedA/basea-x", MakeCallback (&ObjectTest::BaseATrace, this));

  baseB->TraceConnect ("/$DerivedA/*", MakeCallback (&ObjectTest::DerivedATrace, this));
  m_derivedATrace = false;
  derivedA->DerivedGenerateTrace (10);
  NS_TEST_ASSERT (m_derivedATrace);
  // here, we have connected the derived trace sink to all 
  // trace sources, including the base trace source.
  m_derivedATrace = false;
  derivedA->BaseGenerateTrace (11);
  NS_TEST_ASSERT (m_derivedATrace);
  baseB->TraceDisconnect ("/$DerivedA/*", MakeCallback (&ObjectTest::BaseATrace, this));

  // Test the object creation code of TypeId
  Ptr<Object> a = BaseA::GetTypeId ().CreateObject ();
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (), a);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (DerivedA::GetTypeId ()), 0);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<DerivedA> (), 0);
  a = DerivedA::GetTypeId ().CreateObject ();
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (), a);
  NS_TEST_ASSERT_EQUAL (a->QueryInterface<BaseA> (DerivedA::GetTypeId ()), a);
  NS_TEST_ASSERT_UNEQUAL (a->QueryInterface<DerivedA> (), 0);


  return result;
}

static ObjectTest g_interfaceObjectTests;


} // namespace ns3

#endif /* RUN_SELF_TESTS */


