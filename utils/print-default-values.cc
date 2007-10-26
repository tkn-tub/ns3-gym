#include "ns3/default-value.h"

using namespace ns3;

static void
PrintOne (DefaultValueBase *value, std::ostream &os)
{
  os << "///  <li> \\anchor DefaultValue" << value->GetName ()
     << " " << value->GetName () << std::endl;
  os << "///    <ul>" << std::endl;
  os << "///         <li>Type: " << value->GetType () << "</td></tr>" << std::endl;
  os << "///         <li>Default value: " << value->GetDefaultValue () << "</td></tr>" << std::endl;
  os << "///         <li>Description: " << value->GetHelp () << "</td></tr>" << std::endl;
  os << "///    </ul>" << std::endl;
  os << "///  </li>" << std::endl;
}

int main (int argc, char *argv[])
{
  std::ostream &os = std::cout;
  os << "/// \\page ListOfDefaultValues The list of default values" << std::endl;
  os << "/// \\defgroup ListOfDefaultValuesGroup The list of default values" << std::endl;
  os << "/// <ul>" << std::endl;
  for (DefaultValueList::Iterator i = DefaultValueList::Begin ();
       i != DefaultValueList::End (); i++)
    {
      if ((*i)->GetName () == "help")
	{
	  continue;
	}
      PrintOne (*i, os);
    }
  os << "/// </ul>" << std::endl;
  return 0;
}
