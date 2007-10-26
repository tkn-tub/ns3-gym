#include "ns3/default-value.h"

using namespace ns3;

static void
PrintOne (DefaultValueBase *value, std::ostream &os)
{
  os << "/// <tr>" << std::endl;
  os << "///   <td> \\anchor DefaultValue" << value->GetName ()
     << " " << value->GetName () << " </td>" << std::endl;
  os << "///    <td>" << std::endl;
  os << "///       <table>" << std::endl;
  os << "///         <tr><td>" << value->GetType () << "</td></tr>" << std::endl;
  os << "///         <tr><td>" << value->GetDefaultValue () << "</td></tr>" << std::endl;
  os << "///         <tr><td>" << value->GetHelp () << "</td></tr>" << std::endl;
  os << "///       </table>"<<std::endl;
  os << "///    </td>" << std::endl;
  os << "/// </tr>" << std::endl;
}

int main (int argc, char *argv[])
{
  std::ostream &os = std::cout;
  os << "/// \\page ListOfDefaultValues The list of default values" << std::endl;
  os << "/// \\defgroup ListOfDefaultValuesGroup The list of default values" << std::endl;
  os << "/// <table>" << std::endl;
  for (DefaultValueList::Iterator i = DefaultValueList::Begin ();
       i != DefaultValueList::End (); i++)
    {
      if ((*i)->GetName () == "help")
	{
	  continue;
	}
      PrintOne (*i, os);
    }

  return 0;
}
