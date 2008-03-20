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
      attr.value = i->value.Copy ();
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
      attr.value = i->value.Copy ();
      m_attributes.push_back (attr);
    }
  return *this;
}
AttributeList::~AttributeList ()
{
  Reset ();
}

void
AttributeList::Set (std::string name, Attribute value)
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
AttributeList::SetFailSafe (std::string name, Attribute value)
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
AttributeList::SetWithTid (TypeId tid, std::string name, Attribute value)
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
AttributeList::DoSetOne (Ptr<const AttributeChecker> checker, Attribute value)
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
AttributeList::DoSet (struct TypeId::AttributeInfo *info, Attribute value)
{
  if (info->checker == 0)
    {
      return false;
    }
  bool ok = info->checker->Check (value);
  if (!ok)
    {
      // attempt to convert to string.
      const StringValue *str = value.DynCast<const StringValue *> ();
      if (str == 0)
        {
          return false;
        }
      // attempt to convert back to value.
      Attribute v = info->checker->Create ();
      ok = v.DeserializeFromString (str->Get ().Get (), info->checker);
      if (!ok)
        {
          return false;
        }
      ok = info->checker->Check (v);
      if (!ok)
        {
          return false;
        }
      value = v;
    }
  DoSetOne (info->checker, value);
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
      for (uint32_t j = 0; j < tid.GetAttributeListN (); j++)
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
  for (Attrs::const_iterator i = m_attributes.begin (); i != m_attributes.end (); i++)
    {
      std::string name = LookupAttributeFullNameByChecker (i->checker);
      oss << name << "=" << i->value.SerializeToString (i->checker);
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
  do {
    std::string::size_type equal = str.find ("=", cur);
    if (equal == std::string::npos)
      {
        // XXX: invalid attribute.
        break;
      }
    else
      {
        std::string name = str.substr (cur, equal-cur);
        struct TypeId::AttributeInfo info;
        if (!TypeId::LookupAttributeByFullName (name, &info))
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
            Attribute val = info.checker->Create ();
            bool ok = val.DeserializeFromString (value, info.checker);
            if (!ok)
              {
                // XXX invalid value
                break;
              }
            else
              {
                DoSetOne (info.checker, val);
              }
          }
      }
  } while (cur != str.size ());

  return true;
}

} // namespace ns3
