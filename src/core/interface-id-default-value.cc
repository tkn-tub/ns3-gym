#include "interface-id-default-value.h"

namespace ns3 {

InterfaceIdDefaultValue::InterfaceIdDefaultValue (std::string name, 
						  std::string help,
						  InterfaceId iid,
						  std::string defaultValue)
  : DefaultValueBase (name, help),
    m_defaultName (defaultValue),
    m_name (defaultValue),
    m_interfaceId (iid)
{
  DefaultValueList::Add (this);
}
InterfaceId 
InterfaceIdDefaultValue::GetValue (void) const
{
  return InterfaceId::LookupByName (m_name);
}
void 
InterfaceIdDefaultValue::SetValue (InterfaceId interfaceId)
{
  m_name = interfaceId.GetName ();
}
void 
InterfaceIdDefaultValue::SetValue (std::string name)
{
  m_name = name;
}
bool 
InterfaceIdDefaultValue::DoParseValue (const std::string &value)
{
  for (uint32_t i = 0; i < InterfaceId::GetRegisteredN (); i++)
    {
      InterfaceId iid = InterfaceId::GetRegistered (i);
      do {
	if (iid.GetName () == value &&
	    iid.HasConstructor ())
	  {
	    // check that it really supports the requested interface.
	    InterfaceId tmp = iid;
	    do {
	      if (tmp == m_interfaceId)
		{
		  m_name = value;
		  return true;
		}
	      tmp = tmp.GetParent ();
	    } while (tmp != Object::iid ());
	  }
	iid = iid.GetParent ();
      } while (iid != Object::iid ());
    }
  return false;
}

std::string 
InterfaceIdDefaultValue::DoGetType (void) const
{
  std::ostringstream oss;
  oss << "(";
  bool first = true;
  for (uint32_t i = 0; i < InterfaceId::GetRegisteredN (); i++)
    {
      InterfaceId iid = InterfaceId::GetRegistered (i);
      // can this interface id be used to create objects ?
      if (iid.HasConstructor ())
	{
	  InterfaceId tmp = iid;
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
	  } while (tmp != Object::iid ());
	}
    }
  oss << ")";
  return oss.str ();
}

std::string 
InterfaceIdDefaultValue::DoGetDefaultValue (void) const
{
  return m_name;
}

} // namespace ns3
