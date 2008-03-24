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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "object-base.h"
#include "log.h"
#include "trace-source-accessor.h"
#include "string.h"

NS_LOG_COMPONENT_DEFINE ("ObjectBase");

namespace ns3 {

static TypeId
GetObjectIid (void)
{
  TypeId tid = TypeId ("ns3::ObjectBase");
  tid.SetParent (tid);
  return tid;
}

TypeId 
ObjectBase::GetTypeId (void)
{
  static TypeId tid = GetObjectIid ();
  return tid;
}

ObjectBase::~ObjectBase () 
{}

void
ObjectBase::NotifyConstructionCompleted (void)
{}

void
ObjectBase::ConstructSelf (const AttributeList &attributes)
{
  // loop over the inheritance tree back to the Object base class.
  TypeId tid = GetInstanceTypeId ();
  do {
    // loop over all attributes in object type
    NS_LOG_DEBUG ("construct tid="<<tid.GetName ()<<", params="<<tid.GetAttributeN ());
    for (uint32_t i = 0; i < tid.GetAttributeN (); i++)
      {
        Ptr<const AttributeAccessor> paramSpec = tid.GetAttributeAccessor (i);
        Attribute initial = tid.GetAttributeInitialValue (i);
        Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (i);
        NS_LOG_DEBUG ("try to construct \""<< tid.GetName ()<<"::"<<
                      tid.GetAttributeName (i)<<"\"");
        if (!(tid.GetAttributeFlags (i) & TypeId::ATTR_CONSTRUCT))
          {
            continue;
          }
        bool found = false;
        // is this attribute stored in this AttributeList instance ?
        for (AttributeList::Attrs::const_iterator j = attributes.m_attributes.begin ();
             j != attributes.m_attributes.end (); j++)
          {
            if (j->checker == checker)
              {
                // We have a matching attribute value.
                DoSet (paramSpec, initial, checker, j->value);
                NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                              tid.GetAttributeName (i)<<"\"");
                found = true;
                break;
              }
          }
        if (!found)
          {
            // is this attribute stored in the global instance instance ?
            for (AttributeList::Attrs::const_iterator j = AttributeList::GetGlobal ()->m_attributes.begin ();
                 j != AttributeList::GetGlobal ()->m_attributes.end (); j++)
              {
                if (j->checker == checker)
                  {
                    // We have a matching attribute value.
                    DoSet (paramSpec, initial, checker, j->value);
                    NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                                  tid.GetAttributeName (i)<<"\" from global");
                    found = true;
                    break;
                  }
              }
          }
        if (!found)
          {
            // No matching attribute value so we set the default value.
            paramSpec->Set (this, initial);
            NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                          tid.GetAttributeName (i)<<"\" from initial value.");
          }
      }
    tid = tid.GetParent ();
  } while (tid != ObjectBase::GetTypeId ());
  NotifyConstructionCompleted ();
}

bool
ObjectBase::DoSet (Ptr<const AttributeAccessor> spec, Attribute initialValue, 
		   Ptr<const AttributeChecker> checker, Attribute value)
{
  bool ok = checker->Check (value);
  if (!ok)
    {
      // attempt to convert to string
      const StringValue *str = value.DynCast<const StringValue *> ();
      if (str == 0)
        {
          return false;
        }
      // attempt to convert back from string.
      Attribute v = checker->Create ();
      ok = v.DeserializeFromString (str->Get ().Get (), checker);
      if (!ok)
        {
          return false;
        }
      ok = checker->Check (v);
      if (!ok)
        {
          return false;
        }
      value = v;
    }
  ok = spec->Set (this, value);
  return ok;
}
void
ObjectBase::SetAttribute (std::string name, Attribute value)
{
  struct TypeId::AttributeInfo info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" does not exist for this object: tid="<<tid.GetName ());
    }
  if (!(info.flags & TypeId::ATTR_SET))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" is not settable for this object: tid="<<tid.GetName ());
    }
  if (!DoSet (info.accessor, info.initialValue, info.checker, value))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" could not be set for this object: tid="<<tid.GetName ());
    }
}
bool 
ObjectBase::SetAttributeFailSafe (std::string name, Attribute value)
{
  struct TypeId::AttributeInfo info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_SET))
    {
      return false;
    }
  return DoSet (info.accessor, info.initialValue, info.checker, value);
}
std::string
ObjectBase::GetAttributeAsString (std::string name) const
{
  struct TypeId::AttributeInfo info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" does not exist for this object: tid="<<tid.GetName ());
    }
  if (!(info.flags & TypeId::ATTR_GET))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" is not gettable for this object: tid="<<tid.GetName ());
    }
  Attribute v = info.checker->Create ();
  bool ok = info.accessor->Get (this, v);
  if (!ok)
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" is not gettable for this object: tid="<<tid.GetName ());
    }
  std::string value = v.SerializeToString (info.checker);
  return value;
}

Attribute
ObjectBase::GetAttribute (std::string name) const
{
  struct TypeId::AttributeInfo info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      return Attribute ();
    }
  if (!(info.flags & TypeId::ATTR_GET))
    {
      return Attribute ();
    }
  Attribute value = info.checker->Create ();
  bool ok = info.accessor->Get (this, value);
  if (!ok)
    {
      return Attribute ();
    }
  return value;
}

bool
ObjectBase::GetAttributeAsStringFailSafe (std::string name, std::string &value) const
{
  struct TypeId::AttributeInfo info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_GET))
    {
      return false;
    }
  Attribute v = info.checker->Create ();
  bool ok = info.accessor->Get (this, v);
  if (ok)
    {
      value = v.SerializeToString (info.checker);
    }
  return ok;
}

bool
ObjectBase::GetAttributeFailSafe (std::string name, Attribute &value) const
{
  struct TypeId::AttributeInfo info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_GET))
    {
      return false;
    }
  value = info.checker->Create ();
  bool ok = info.accessor->Get (this, value);
  return ok;
}

bool 
ObjectBase::TraceConnectWithoutContext (std::string name, const CallbackBase &cb)
{
  TypeId tid = GetInstanceTypeId ();
  Ptr<const TraceSourceAccessor> accessor = tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->ConnectWithoutContext (this, cb);
  return ok;
}
bool 
ObjectBase::TraceConnect (std::string name, std::string context, const CallbackBase &cb)
{
  TypeId tid = GetInstanceTypeId ();
  Ptr<const TraceSourceAccessor> accessor = tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->Connect (this, context, cb);
  return ok;
}
bool 
ObjectBase::TraceDisconnectWithoutContext (std::string name, const CallbackBase &cb)
{
  TypeId tid = GetInstanceTypeId ();
  Ptr<const TraceSourceAccessor> accessor = tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->DisconnectWithoutContext (this, cb);
  return ok;
}
bool 
ObjectBase::TraceDisconnect (std::string name, std::string context, const CallbackBase &cb)
{
  TypeId tid = GetInstanceTypeId ();
  Ptr<const TraceSourceAccessor> accessor = tid.LookupTraceSourceByName (name);
  if (accessor == 0)
    {
      return false;
    }
  bool ok = accessor->Disconnect (this, context, cb);
  return ok;
}



} // namespace ns3
