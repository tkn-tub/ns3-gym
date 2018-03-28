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
#include "attribute-construction-list.h"
#include "string.h"
#include "ns3/core-config.h"
#ifdef HAVE_STDLIB_H
#include <cstdlib>
#endif

/**
 * \file
 * \ingroup object
 * ns3::ObjectBase class implementation.
 */

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ObjectBase");

NS_OBJECT_ENSURE_REGISTERED (ObjectBase);

/**
 * Ensure the TypeId for ObjectBase gets fully configured
 * to anchor the inheritance tree properly.
 *
 * \relates ns3::ObjectBase
 *
 * \return The TypeId for ObjectBase.
 */
static TypeId
GetObjectIid (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  TypeId tid = TypeId ("ns3::ObjectBase");
  tid.SetParent (tid);
  tid.SetGroupName ("Core");
  return tid;
}

TypeId 
ObjectBase::GetTypeId (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static TypeId tid = GetObjectIid ();
  return tid;
}

ObjectBase::~ObjectBase () 
{
  NS_LOG_FUNCTION (this);
}

void
ObjectBase::NotifyConstructionCompleted (void)
{
  NS_LOG_FUNCTION (this);
}

void
ObjectBase::ConstructSelf (const AttributeConstructionList &attributes)
{
  // loop over the inheritance tree back to the Object base class.
  NS_LOG_FUNCTION (this << &attributes);
  TypeId tid = GetInstanceTypeId ();
  do {
      // loop over all attributes in object type
      NS_LOG_DEBUG ("construct tid="<<tid.GetName ()<<", params="<<tid.GetAttributeN ());
      for (uint32_t i = 0; i < tid.GetAttributeN (); i++)
        {
          struct TypeId::AttributeInformation info = tid.GetAttribute(i);
          NS_LOG_DEBUG ("try to construct \""<< tid.GetName ()<<"::"<<
                        info.name <<"\"");
          // is this attribute stored in this AttributeConstructionList instance ?
          Ptr<AttributeValue> value = attributes.Find(info.checker);
          // See if this attribute should not be set here in the
          // constructor.
          if (!(info.flags & TypeId::ATTR_CONSTRUCT))
            {
              // Handle this attribute if it should not be 
              // set here.
              if (value == 0)
                {
                  // Skip this attribute if it's not in the
                  // AttributeConstructionList.
                  continue;
                }              
              else
                {
                  // This is an error because this attribute is not
                  // settable in its constructor but is present in
                  // the AttributeConstructionList.
                  NS_FATAL_ERROR ("Attribute name="<<info.name<<" tid="<<tid.GetName () << ": initial value cannot be set using attributes");
                }
            }

          if (value != 0)
            {
              // We have a matching attribute value.
              if (DoSet (info.accessor, info.checker, *value))
                {
                  NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                                info.name<<"\"");
                  continue;
                }
            }

#ifdef HAVE_GETENV
          // No matching attribute value so we try to look at the env var.
          char *envVar = getenv ("NS_ATTRIBUTE_DEFAULT");
          if (envVar != 0)
            {
              std::string env = std::string (envVar);
              std::string::size_type cur = 0;
              std::string::size_type next = 0;
              while (next != std::string::npos)
                {
                  next = env.find (";", cur);
                  std::string tmp = std::string (env, cur, next-cur);
                  std::string::size_type equal = tmp.find ("=");
                  if (equal != std::string::npos)
                    {
                      std::string name = tmp.substr (0, equal);
                      std::string envval = tmp.substr (equal+1, tmp.size () - equal - 1);
                      if (name == tid.GetAttributeFullName (i))
                        {
                          if (DoSet (info.accessor, info.checker, StringValue (envval)))
                            {
                              NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                                            info.name <<"\" from env var");
                              break;
                            }
                        }
                    }
                  cur = next + 1;
                }
            }
#endif /* HAVE_GETENV */

          // No matching attribute value so we try to set the default value.
          DoSet (info.accessor, info.checker, *info.initialValue);
          NS_LOG_DEBUG ("construct \""<< tid.GetName ()<<"::"<<
                        info.name <<"\" from initial value.");
        }
      tid = tid.GetParent ();
    } while (tid != ObjectBase::GetTypeId ());
  NotifyConstructionCompleted ();
}

bool
ObjectBase::DoSet (Ptr<const AttributeAccessor> accessor, 
                   Ptr<const AttributeChecker> checker,
                   const AttributeValue &value)
{
  NS_LOG_FUNCTION (this << accessor << checker << &value);
  Ptr<AttributeValue> v = checker->CreateValidValue (value);
  if (v == 0)
    {
      return false;
    }
  bool ok = accessor->Set (this, *v);
  return ok;
}

void
ObjectBase::SetAttribute (std::string name, const AttributeValue &value)
{
  NS_LOG_FUNCTION (this << name << &value);
  struct TypeId::AttributeInformation info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" does not exist for this object: tid="<<tid.GetName ());
    }
  if (!(info.flags & TypeId::ATTR_SET) ||
      !info.accessor->HasSetter ())
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" is not settable for this object: tid="<<tid.GetName ());
    }
  if (!DoSet (info.accessor, info.checker, value))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" could not be set for this object: tid="<<tid.GetName ());
    }
}
bool 
ObjectBase::SetAttributeFailSafe (std::string name, const AttributeValue &value)
{
  NS_LOG_FUNCTION (this << name << &value);
  struct TypeId::AttributeInformation info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_SET) ||
      !info.accessor->HasSetter ())
    {
      return false;
    }
  return DoSet (info.accessor, info.checker, value);
}

void
ObjectBase::GetAttribute (std::string name, AttributeValue &value) const
{
  NS_LOG_FUNCTION (this << name << &value);
  struct TypeId::AttributeInformation info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" does not exist for this object: tid="<<tid.GetName ());
    }
  if (!(info.flags & TypeId::ATTR_GET) || 
      !info.accessor->HasGetter ())
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" is not gettable for this object: tid="<<tid.GetName ());
    }
  bool ok = info.accessor->Get (this, value);
  if (ok)
    {
      return;
    }
  StringValue *str = dynamic_cast<StringValue *> (&value);
  if (str == 0)
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" tid="<<tid.GetName () << ": input value is not a string");
    }
  Ptr<AttributeValue> v = info.checker->Create ();
  ok = info.accessor->Get (this, *PeekPointer (v));
  if (!ok)
    {
      NS_FATAL_ERROR ("Attribute name="<<name<<" tid="<<tid.GetName () << ": could not get value");
    }
  str->Set (v->SerializeToString (info.checker));
}


bool
ObjectBase::GetAttributeFailSafe (std::string name, AttributeValue &value) const
{
  NS_LOG_FUNCTION (this << name << &value);
  struct TypeId::AttributeInformation info;
  TypeId tid = GetInstanceTypeId ();
  if (!tid.LookupAttributeByName (name, &info))
    {
      return false;
    }
  if (!(info.flags & TypeId::ATTR_GET) ||
      !info.accessor->HasGetter ())
    {
      return false;
    }
  bool ok = info.accessor->Get (this, value);
  if (ok)
    {
      return true;
    }
  StringValue *str = dynamic_cast<StringValue *> (&value);
  if (str == 0)
    {
      return false;
    }
  Ptr<AttributeValue> v = info.checker->Create ();
  ok = info.accessor->Get (this, *PeekPointer (v));
  if (!ok)
    {
      return false;
    }
  str->Set (v->SerializeToString (info.checker));
  return true;
}

bool 
ObjectBase::TraceConnectWithoutContext (std::string name, const CallbackBase &cb)
{
  NS_LOG_FUNCTION (this << name << &cb);
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
  NS_LOG_FUNCTION (this << name << context << &cb);
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
  NS_LOG_FUNCTION (this << name << &cb);
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
  NS_LOG_FUNCTION (this << name << context << &cb);
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
