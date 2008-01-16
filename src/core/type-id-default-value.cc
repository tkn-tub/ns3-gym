#include "type-id-default-value.h"

namespace ns3 {

TypeIdDefaultValue::TypeIdDefaultValue (std::string name, 
						  std::string help,
						  TypeId tid,
						  std::string defaultValue)
  : DefaultValueBase (name, help),
    m_defaultName (defaultValue),
    m_name (defaultValue),
    m_interfaceId (tid)
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
      TypeId tid = TypeId::GetRegistered (i);
      do {
	if (tid.GetName () == value &&
	    tid.HasConstructor ())
	  {
	    // check that it really supports the requested interface.
	    TypeId tmp = tid;
	    do {
	      if (tmp == m_interfaceId)
		{
		  m_name = value;
		  return true;
		}
	      tmp = tmp.GetParent ();
	    } while (tmp != Object::GetTypeId ());
	  }
	tid = tid.GetParent ();
      } while (tid != Object::GetTypeId ());
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
      TypeId tid = TypeId::GetRegistered (i);
      // can this interface id be used to create objects ?
      if (tid.HasConstructor ())
	{
	  TypeId tmp = tid;
	  // does this interface id supports the requested interface id ?
	  do {
	    if (tmp == m_interfaceId)
	      {
		if (!first)
		  {
		    oss << "|";
		    first = false;
		  }
		oss << tid.GetName ();
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
