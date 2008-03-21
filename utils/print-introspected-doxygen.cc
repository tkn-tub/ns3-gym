#include <iostream>
#include "ns3/object.h"

using namespace ns3;

void
PrintAttributes (TypeId tid, std::ostream &os)
{
  os << "<ul>"<<std::endl;
  for (uint32_t j = 0; j < tid.GetAttributeN (); j++)
    {
      os << "<li><b>" << tid.GetAttributeName (j) << "</b>: "
		<< tid.GetAttributeHelp (j) << std::endl;
      Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (j);
      os << "  <ul>" << std::endl << "    <li>Type: " <<  checker->GetType ();
      if (checker->HasTypeConstraints ())
	{
	  os << " -> " << checker->GetTypeConstraints ();
	}
      os << "</li>" << std::endl;
      uint32_t flags = tid.GetAttributeFlags (j);
      os << "<li>Flags: ";
      if (flags & TypeId::ATTR_SET)
	{
	  os << "write ";
	}
      if (flags & TypeId::ATTR_GET)
	{
	  os << "read ";
	}
      if (flags & TypeId::ATTR_CONSTRUCT)
	{
	  os << "construct ";
	}
      os << "  </ul> " << std::endl;
      
    }
  os << "</ul>" << std::endl;
}


int main (int argc, char *argv[])
{

  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      std::cout << "/*!" << std::endl;
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << "\\fn static TypeId " << tid.GetName () << "::GetTypeId (void)" << std::endl;
      std::cout << "\\brief This method returns the TypeId associated to \\ref " << tid.GetName () << std::endl << std::endl;
      if (tid.GetAttributeN () == 0)
	{
	  std::cout << "No Attributes defined for this type." << std::endl;
	}
      else
	{
	  std::cout << "Attributes defined for this type:" << std::endl;
	  PrintAttributes (tid, std::cout);
	}
      bool hasAttributesInParent = false;
      TypeId tmp = tid.GetParent ();
      while (tmp.GetParent () != tmp)
	{
	  if (tmp.GetAttributeN () != 0)
	    {
	      hasAttributesInParent = true;
	    }
	  tmp = tmp.GetParent ();
	}
      if (hasAttributesInParent)
	{
	  std::cout << "Attributes defined in parent classes:<br>" << std::endl;
	  tmp = tid.GetParent ();
	  while (tmp.GetParent () != tmp)
	    {
	      if (tmp.GetAttributeN () != 0)
		{
		  std::cout << tmp.GetName () << std::endl;
		  PrintAttributes (tmp, std::cout);
		}
	      tmp = tmp.GetParent ();
	    }
	}
      std::cout << "*/" << std::endl;
    }


  return 0;
}
