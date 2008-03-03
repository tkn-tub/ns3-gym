#include "ns3/internet-node.h"
#include "ns3/ptr.h"
#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/csma-net-device.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/default-value.h"
#include "ns3/string.h"

using namespace ns3;


static void
PrintOneDefaultValue (DefaultValueBase *value, std::ostream &os)
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

static void
PrintDefaultValuesDoxygen (std::ostream &os)
{
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
      PrintOneDefaultValue (*i, os);
    }
  os << "/// </ul>" << std::endl;
}


int main (int argc, char *argv[])
{
  Ptr<Node> node = CreateObject<InternetNode> ();
  node->AggregateObject (CreateObject<MobilityModelNotifier> ());

  Ptr<PointToPointNetDevice> p2p = CreateObjectWith<PointToPointNetDevice> ("Node", node, 
									    "Address", Mac48Address::Allocate ());
  node->AddDevice (p2p);
  p2p->AddQueue (CreateObject<DropTailQueue> ());
  Ptr<CsmaNetDevice> csma = CreateObjectWith<CsmaNetDevice> ("Node", node, 
							     "Address", Mac48Address::Allocate (),
							     "EncapsulationMode", String ("Llc"));
  node->AddDevice (csma);
  csma->AddQueue (CreateObject<DropTailQueue> ());


  PrintDefaultValuesDoxygen (std::cout);

  return 0;
}
