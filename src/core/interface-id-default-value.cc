#include "interface-id-default-value.h"

namespace ns3 {

TypeIdDefaultValue::TypeIdDefaultValue (std::string name, 
						  std::string help,
						  TypeId iid,
						  std::string defaultValue)
  : DefaultValueBase (name, help),
    m_defaultName (defaultValue),
    m_name (defaultValue),
    m_interfaceId (iid)
{
  DefaultValueList::Add (this);
}
TypeId 
TypeIdDefaultValue::GetValue (void) const
{
  return TypeId::LookupByName (m_name);
}
void 
TypeIdDefaultValue::SetValue (TypeId interfaceId)
{
  m_name = interfaceId.GetName ();
}
void 
TypeIdDefaultValue::SetValue (std::string name)
{
  m_name = name;
}
bool 
TypeIdDefaultValue::DoParseValue (const std::string &value)
{
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      TypeId iid = TypeId::GetRegistered (i);
      do {
	if (iid.GetName () == value &&
	    iid.HasConstructor ())
	  {
	    // check that it really supports the requested interface.
	    TypeId tmp = iid;
	    do {
	      if (tmp == m_interfaceId)
		{
		  m_name = value;
		  return true;
		}
	      tmp = tmp.GetParent ();
	    } while (tmp != Object::GetTypeId ());
	  }
	iid = iid.GetParent ();
      } while (iid != Object::GetTypeId ());
    }
  return false;
}

std::string 
TypeIdDefaultValue::DoGetType (void) const
{
  std::ostringstream oss;
  oss << "(";
  bool first = true;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      TypeId iid = TypeId::GetRegistered (i);
      // can this interface id be used to create objects ?
      if (iid.HasConstructor ())
	{
	  TypeId tmp = iid;
	  // does this interface id supports the requested interface id ?
	  do {
	    if (tmp == m_interfaceId)
	      {
		if (!first)
		  {
		    oss << "|";
		    first = false;
		  }
		oss << iid.GetName ();
	      }
	    tmp = tmp.GetParent ();
	  } while (tmp != Object::GetTypeId ());
	}
    }
  oss << ")";
  return oss.str ();
}

std::string 
TypeIdDefaultValue::DoGetDefaultValue (void) const
{
  return m_name;
}

} // namespace ns3
