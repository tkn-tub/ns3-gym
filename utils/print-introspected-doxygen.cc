#include "ns3/internet-node.h"
#include "ns3/ptr.h"
#include "ns3/trace-resolver.h"
#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/csma-net-device.h"
#include "ns3/queue.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/default-value.h"
#include "ns3/string.h"

using namespace ns3;

void
PrintSimpleText (const TraceResolver::SourceCollection *sources, std::ostream &os)
{
  for (TraceResolver::SourceCollection::Iterator i = sources->Begin (); i != sources->End (); i++)
    {
      os << "source=" << i->path << std::endl;
      os << "TraceContext=[";
      i->context.PrintAvailable (os, ",");
      os << "]" << std::endl;
      os << "help=\"" << i->doc.GetHelp () << "\"" << std::endl;
      os << "void TraceSinkCallback (const TraceContext &";
      for (TraceDoc::Iterator k = i->doc.ArgsBegin (); k != i->doc.ArgsEnd (); k++)
        {
          os << ", " << k->first;
        }
      os << ")" << std::endl;
      os << "argument 1  --  the trace context associated to the connected trace source." << std::endl;
      uint32_t k = 2;
      for (TraceDoc::Iterator j = i->doc.ArgsBegin (); j != i->doc.ArgsEnd (); j++)
        {
          os << "argument " << k << "  --  " << j->second << "." << std::endl;
          k++;
        }
      os << std::endl;
    }
}
static void
PrintDoxygenText (const TraceResolver::SourceCollection *sources, std::ostream &os)
{
  uint32_t z = 0;
  for (TraceResolver::SourceCollection::Iterator i = sources->Begin (); i != sources->End (); i++)
    {
      os << "///" << std::endl;
      os << "/// \\ingroup TraceSourceList" << std::endl; 
      os << "/// \\brief " << i->doc.GetHelp () << std::endl;
      os << "/// \\param arg1 the trace context associated to the connected trace source." << std::endl;
      uint32_t j = 2;
      for (TraceDoc::Iterator l = i->doc.ArgsBegin (); l != i->doc.ArgsEnd (); l++)
        {
          os << "/// \\param arg" << j << " " << l->second << "." << std::endl;
          j++;
        }
      os << "///" << std::endl;
      os << "///" << std::endl;
      os << "/// The path to this trace source is: <b><tt>\"" << i->path << "\"</tt></b>." << std::endl;
      os << "///" << std::endl;
      if (i->context.Begin ().IsLast ())
        {
          os << "/// No data can be extracted from \\p arg1 with ns3::TraceContext::GetElement." << std::endl;
        }
      else
        {
          os << "/// The following classes can be extracted from \\p arg1 with " << std::endl;
          os << "/// ns3::TraceContext::GetElement:" << std::endl;
          for (TraceContext::Iterator m = i->context.Begin (); !m.IsLast (); m.Next ())
            {
              os << "///  - " << m.Get () << std::endl;
            }
        }
      os << "void TraceSinkCallback" << z << " (const TraceContext & arg1" ;
      j = 2;
      for (TraceDoc::Iterator k = i->doc.ArgsBegin (); k != i->doc.ArgsEnd (); k++)
        {
          os << ", " << k->first << " arg" << j;
          j++;
        }
      os << ");" << std::endl;
      os << std::endl;
      z++;
    }
}

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
  p2p->AddQueue (Queue::CreateDefault ());
  Ptr<CsmaNetDevice> csma = CreateObjectWith<CsmaNetDevice> ("Node", node, 
							     "Address", Mac48Address::Allocate (),
							     "EncapsulationMode", String ("Llc"));
  node->AddDevice (csma);
  csma->AddQueue (Queue::CreateDefault ());

  TraceResolver::SourceCollection collection;
  NodeList::GetTraceResolver ()->CollectSources ("", TraceContext (), &collection);
  PrintDoxygenText (&collection, std::cout);


  PrintDefaultValuesDoxygen (std::cout);

  return 0;
}
