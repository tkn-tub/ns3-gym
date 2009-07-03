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
#include "attribute-list.h"
#include "string.h"
#include "singleton.h"

namespace ns3 {

/*********************************************************************
 *         The AttributeList container implementation
 *********************************************************************/

AttributeList::AttributeList ()
{}

AttributeList::AttributeList (const AttributeList &o)
{
  for (Attrs::const_iterator i = o.m_attributes.begin (); i != o.m_attributes.end (); i++)
    {
      struct Attr attr;
      attr.checker = i->checker;
      attr.value = i->value->Copy ();
      m_attributes.push_back (attr);
    }
}
AttributeList &
AttributeList::operator = (const AttributeList &o)
{
  Reset ();
  for (Attrs::const_iterator i = o.m_attributes.begin (); i != o.m_attributes.end (); i++)
    {
      struct Attr attr;
      attr.checker = i->checker;
      attr.value = i->value->Copy ();
      m_attributes.push_back (attr);
    }
  return *this;
}
AttributeList::~AttributeList ()
{
  Reset ();
}

void
AttributeList::Set (std::string name, const AttributeValue &value)
{
  struct TypeId::AttributeInfo info;
  bool ok = TypeId::LookupAttributeByFullName (name, &info);
  if (!ok)
    {
      NS_FATAL_ERROR ("Could not find attribute "<<name);
    }
  ok = DoSet (&info, value);
  if (!ok)
    {
      NS_FATAL_ERROR ("Could not set value for attribute "<<name);
    }
}
bool 
AttributeList::SetFailSafe (std::string name, const AttributeValue &value)
{
  struct TypeId::AttributeInfo info;
  bool ok = TypeId::LookupAttributeByFullName (name, &info);
  if (!ok)
    {
      return false;
    }
  ok = DoSet (&info, value);
  return ok;
}
void
AttributeList::SetWithTid (TypeId tid, std::string name, const AttributeValue & value)
{
  struct TypeId::AttributeInfo info;
  bool ok = tid.LookupAttributeByName (name, &info);
  if (!ok)
    {
      NS_FATAL_ERROR ("Could not find attribute "<<tid.GetName ()<<"::"<<name);
    }
  ok = DoSet (&info, value);
  if (!ok)
    {
      NS_FATAL_ERROR ("Could not set value for attribute "<<tid.GetName ()<<"::"<<name);
    }
}

void
AttributeList::DoSetOne (Ptr<const AttributeChecker> checker, const AttributeValue &value)
{
  // get rid of any previous value stored in this
  // vector of values.
  for (Attrs::iterator k = m_attributes.begin (); k != m_attributes.end (); k++)
    {
      if (k->checker == checker)
        {
          m_attributes.erase (k);
          break;
        }
    }
  // store the new value.
  struct Attr attr;
  attr.checker = checker;
  attr.value = value.Copy ();
  m_attributes.push_back (attr);
}
bool
AttributeList::DoSet (struct TypeId::AttributeInfo *info, const AttributeValue &value)
{
  if (info->checker == 0)
    {
      return false;
    }
  bool ok = info->checker->Check (value);
  if (ok)
    {
      DoSetOne (info->checker, value);
      return true;
    }

  // attempt to convert to string.
  const StringValue *str = dynamic_cast<const StringValue *> (&value);
  if (str == 0)
    {
      return false;
    }
  // attempt to convert back to value.
  Ptr<AttributeValue> v = info->checker->Create ();
  ok = v->DeserializeFromString (str->Get (), info->checker);
  if (!ok)
    {
      return false;
    }
  ok = info->checker->Check (*v);
  if (!ok)
    {
      return false;
    }
  DoSetOne (info->checker, *v);
  return true;
}
void 
AttributeList::Reset (void)
{
  m_attributes.clear ();
}
AttributeList *
AttributeList::GetGlobal (void)
{
  return Singleton<AttributeList>::Get ();
}

std::string
AttributeList::LookupAttributeFullNameByChecker (Ptr<const AttributeChecker> checker) const
{
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      TypeId tid = TypeId::GetRegistered (i);
      for (uint32_t j = 0; j < tid.GetAttributeN (); j++)
        {
          if (checker == tid.GetAttributeChecker (j))
            {
              return tid.GetAttributeFullName (j);
            }
        }
    }
  NS_FATAL_ERROR ("Could not find requested Accessor.");
  // quiet compiler.
  return "";
}

std::string 
AttributeList::SerializeToString (void) const
{
  std::ostringstream oss;
  for (Attrs::const_iterator i = m_attributes.begin (); i != m_attributes.end ();)
    {
      std::string name = LookupAttributeFullNameByChecker (i->checker);
      oss << name << "=" << i->value->SerializeToString (i->checker);
      i++;
      if (i != m_attributes.end ())
        {
          oss << "|";
        }
    }  
  return oss.str ();
}
bool 
AttributeList::DeserializeFromString (std::string str)
{
  Reset ();

  std::string::size_type cur;
  cur = 0;
  while (cur != str.size ())
    {
      std::string::size_type equal = str.find ("=", cur);
      if (equal == std::string::npos)
        {
          NS_FATAL_ERROR ("Error while parsing serialized attribute: \"" << str << "\"");
          break;
        }
      else
        {
          std::string name = str.substr (cur, equal-cur);
          struct TypeId::AttributeInfo info;
          if (!TypeId::LookupAttributeByFullName (name, &info))
            {
              NS_FATAL_ERROR ("Error while parsing serialized attribute: name does not exist: \"" << name << "\"");
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
                  cur = next + 1;
                }
              Ptr<AttributeValue> val = info.checker->Create ();
              bool ok = val->DeserializeFromString (value, info.checker);
              if (!ok)
                {
                  NS_FATAL_ERROR ("Error while parsing serialized attribute: value invalid: \"" << value << "\"");
                  break;
                }
              else
                {
                  DoSetOne (info.checker, *val);
                }
            }
        }
    }

  return true;
}

UnsafeAttributeList::UnsafeAttributeList ()
{}
UnsafeAttributeList::UnsafeAttributeList (const UnsafeAttributeList &o)
{
  for (uint32_t i = 0; i < o.m_attributes.size (); ++i)
    {
      Set (o.m_attributes[i].first, *(o.m_attributes[i].second));
    }
}
UnsafeAttributeList &
UnsafeAttributeList::operator = (const UnsafeAttributeList &o)
{
  m_attributes.clear ();
  for (uint32_t i = 0; i < o.m_attributes.size (); ++i)
    {
      Set (o.m_attributes[i].first, *(o.m_attributes[i].second));
    }
  return *this;
}

UnsafeAttributeList::~UnsafeAttributeList ()
{
  m_attributes.clear ();
}
void 
UnsafeAttributeList::Set (std::string name, const AttributeValue &param)
{
  if (name == "")
    {
      return;
    }
  for (uint32_t i = 0; i < m_attributes.size (); ++i)
    {
      if (m_attributes[i].first == name)
        {
          m_attributes[i].second = param.Copy ();
          return;
        }
    }
  m_attributes.push_back (std::make_pair (name, param.Copy ()));
}

AttributeList 
UnsafeAttributeList::GetSafe (std::string tidName) const
{
  AttributeList list;
  for (uint32_t i = 0; i < m_attributes.size (); ++i)
    {
      TypeId tid = TypeId::LookupByName (tidName);
      list.SetWithTid (tid, m_attributes[i].first, *m_attributes[i].second);
    }
  return list;
}


} // namespace ns3
