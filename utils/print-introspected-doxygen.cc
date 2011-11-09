#include <iostream>
#include <algorithm>
#include <map>
#include "ns3/object.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/global-value.h"
#include "ns3/string.h"
#include "ns3/node-container.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PrintIntrospectedDoxygen");

namespace
{
  std::string anchor;
  std::string boldStart;
  std::string boldStop;
  std::string breakBoth;
  std::string breakHtmlOnly;
  std::string breakTextOnly;
  std::string brief;
  std::string commentStart;
  std::string commentStop;
  std::string defgroupAttributeListStart;
  std::string defgroupAttributeListStop;
  std::string defgroupGlobalValueListStart;
  std::string defgroupGlobalValueListStop;
  std::string defgroupTraceSourceListStart;
  std::string defgroupTraceSourceListStop;
  std::string functionStart;
  std::string functionStop;
  std::string indentHtmlOnly;
  std::string ingroupConstructs;
  std::string listStart;
  std::string listStop;
  std::string listLineStart;
  std::string listLineStop;
  std::string reference;
  std::string temporaryCharacter;

} // anonymous namespace

void
PrintAttributes (TypeId tid, std::ostream &os)
{
  os << listStart << std::endl;
  for (uint32_t j = 0; j < tid.GetAttributeN (); j++)
    {
      struct TypeId::AttributeInformation info = tid.GetAttribute(j);
      os << listLineStart << boldStart << info.name << boldStop << ": "
		<< info.help << std::endl;
      os << "  " << listStart << std::endl 
	 << "    " << listLineStart << "Set with class: " << reference << info.checker->GetValueTypeName () << listLineStop << std::endl;
      if (info.checker->HasUnderlyingTypeInformation ())
	{
	  os << "    " << listLineStart << "Underlying type: " << reference << info.checker->GetUnderlyingTypeInformation () << listLineStop << std::endl;
	}
      if (info.flags & TypeId::ATTR_CONSTRUCT && info.accessor->HasSetter ())
	{
	  os << "    " << listLineStart << "Initial value: " << info.initialValue->SerializeToString (info.checker) << listLineStop << std::endl;
	}
      os << "    " << listLineStart << "Flags: ";
      if (info.flags & TypeId::ATTR_CONSTRUCT && info.accessor->HasSetter ())
	{
	  os << "construct ";
	}
      if (info.flags & TypeId::ATTR_SET && info.accessor->HasSetter ())
	{
	  os << "write ";
	}
      if (info.flags & TypeId::ATTR_GET && info.accessor->HasGetter ())
	{
	  os << "read ";
	}
      os << listLineStop << std::endl;
      os << "  " << listStop << " " << std::endl;
      
    }
  os << listStop << std::endl;
}

void
PrintTraceSources (TypeId tid, std::ostream &os)
{
  os << listStart << std::endl;
  for (uint32_t i = 0; i < tid.GetTraceSourceN (); ++i)
    {
      struct TypeId::TraceSourceInformation info = tid.GetTraceSource (i);
      os << listLineStart << boldStart << info.name << boldStop << ": "
	 << info.help
	 << std::endl;
      os << listLineStop << std::endl;
    }
  os << listStop << std::endl;
}


class StaticInformation
{
public:
  void RecordAggregationInfo (std::string a, std::string b);
  void Gather (TypeId tid);
  void Print (void) const;

  std::vector<std::string> Get (TypeId tid);

private:
  std::string GetCurrentPath (void) const;
  void DoGather (TypeId tid);
  void RecordOutput (TypeId tid);
  bool HasAlreadyBeenProcessed (TypeId tid) const;
  void find_and_replace (std::string &source, const std::string find, std::string replace );
  std::vector<std::pair<TypeId,std::string> > m_output;
  std::vector<std::string> m_currentPath;
  std::vector<TypeId> m_alreadyProcessed;
  std::vector<std::pair<TypeId,TypeId> > m_aggregates;
};

void 
StaticInformation::RecordAggregationInfo (std::string a, std::string b)
{
  m_aggregates.push_back (std::make_pair (TypeId::LookupByName (a), TypeId::LookupByName (b)));
}

void 
StaticInformation::Print (void) const
{
  for (std::vector<std::pair<TypeId,std::string> >::const_iterator i = m_output.begin (); i != m_output.end (); ++i)
    {
      std::pair<TypeId,std::string> item = *i;
      std::cout << item.first.GetName () << " -> " << item.second << std::endl;
    }
}

std::string
StaticInformation::GetCurrentPath (void) const
{
  std::ostringstream oss;
  for (std::vector<std::string>::const_iterator i = m_currentPath.begin (); i != m_currentPath.end (); ++i)
    {
      std::string item = *i;
      oss << "/" << item;
    }
  return oss.str ();
}

void
StaticInformation::RecordOutput (TypeId tid)
{
  m_output.push_back (std::make_pair (tid, GetCurrentPath ()));
}

bool
StaticInformation::HasAlreadyBeenProcessed (TypeId tid) const
{
  for (uint32_t i = 0; i < m_alreadyProcessed.size (); ++i)
    {
      if (m_alreadyProcessed[i] == tid)
	{
	  return true;
	}
    }
  return false;
}

std::vector<std::string> 
StaticInformation::Get (TypeId tid)
{
  std::vector<std::string> paths;
  for (uint32_t i = 0; i < m_output.size (); ++i)
    {
      std::pair<TypeId,std::string> tmp = m_output[i];
      if (tmp.first == tid)
	{
	  paths.push_back (tmp.second);
	}
    }
  return paths;
}

void
StaticInformation::Gather (TypeId tid)
{
  DoGather (tid);

  std::sort (m_output.begin (), m_output.end ());
  m_output.erase (std::unique (m_output.begin (), m_output.end ()), m_output.end ());
}

void 
StaticInformation::DoGather (TypeId tid)
{
  NS_LOG_FUNCTION (this);
  if (HasAlreadyBeenProcessed (tid))
    {
      return;
    }
  RecordOutput (tid);
  for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
    {
      struct TypeId::AttributeInformation info = tid.GetAttribute(i);
      const PointerChecker *ptrChecker = dynamic_cast<const PointerChecker *> (PeekPointer (info.checker));
      if (ptrChecker != 0)
        {
          TypeId pointee = ptrChecker->GetPointeeTypeId ();
          m_currentPath.push_back (info.name);
          m_alreadyProcessed.push_back (tid);
          DoGather (pointee);
          m_alreadyProcessed.pop_back ();
          m_currentPath.pop_back ();
          continue;
        }
      // attempt to cast to an object vector.
      const ObjectPtrContainerChecker *vectorChecker = dynamic_cast<const ObjectPtrContainerChecker *> (PeekPointer (info.checker));
      if (vectorChecker != 0)
        {
          TypeId item = vectorChecker->GetItemTypeId ();
          m_currentPath.push_back (info.name + "/[i]");
          m_alreadyProcessed.push_back (tid);
          DoGather (item);
          m_alreadyProcessed.pop_back ();
          m_currentPath.pop_back ();
          continue;
        }
    }
  for (uint32_t j = 0; j < TypeId::GetRegisteredN (); j++)
    {
      TypeId child = TypeId::GetRegistered (j);
      if (child.IsChildOf (tid))
        {
          //please take a look at the following note for an explanation 
          std::string childName = "$" + temporaryCharacter + child.GetName ();
          std::string replaceWith = "::" + temporaryCharacter;
          find_and_replace(childName,"::",replaceWith);
          m_currentPath.push_back (childName);
          m_alreadyProcessed.push_back (tid);
          DoGather (child);
          m_alreadyProcessed.pop_back ();
          m_currentPath.pop_back ();
        }
    }
  for (uint32_t k = 0; k < m_aggregates.size (); ++k)
    {
      std::pair<TypeId,TypeId> tmp = m_aggregates[k];
      if (tmp.first == tid || tmp.second == tid)
        {
          TypeId other;
          if (tmp.first == tid)
            {
              other = tmp.second;
            }
          if (tmp.second == tid)
            {
              other = tmp.first;
            }
          /**
           * Note: for the Doxygen version only, we insert a % in the
           * path below to ensure that doxygen does not attempt to
           * resolve the typeid names included in the string.  if the
           * name contains ::, using the % sign will remove that sign
           * resulting for instance in $ns3MobilityModel instead of
           * $ns3::MobilityModel hence the output must be in the form
           * $%ns3::%MobilityModel in order to show correctly
           * $ns3::MobilityModel We add at the beginning of the name
           * $% and we replace all the :: in the string by ::%.
           */  
          std::string name = "$" + temporaryCharacter + other.GetName ();
          //finding and replacing :: by ::% (for Doxygen version only).
          std::string replaceWith = "::" + temporaryCharacter;
          find_and_replace(name,"::",replaceWith);
          m_currentPath.push_back (name);
          m_alreadyProcessed.push_back (tid);
          DoGather (other);
          m_alreadyProcessed.pop_back ();
          m_currentPath.pop_back ();	  
        }
    }
}

void 
StaticInformation::find_and_replace( std::string &source, const std::string find, std::string replace )
{
  size_t j; 
  j = source.find (find);
  while (j != std::string::npos ) 
    {
      source.replace (j, find.length (),replace);
      j = source.find (find,j+1);
    }
}

void
PrintHelp (const char *program_name)
{
  std::cout << "Usage: " << program_name << " [options]" << std::endl
            << std::endl
            << "Options:" << std::endl
            << "  --help        : print these options" << std::endl
            << "  --output-text : format output as plain text" << std::endl;  
}

int main (int argc, char *argv[])
{
  bool outputText = false;
  char *programName = argv[0];

  argv++;

  while (*argv != 0)
    {
      char *arg = *argv;

      if (strcmp (arg, "--help") == 0)
        {
          PrintHelp (programName);
          return 0;
        }
      else if (strcmp(arg, "--output-text") == 0)
        {
          outputText = true;
        }
      else
        {
          // un-recognized command-line argument
          PrintHelp (programName);
          return 0;
        }
      argv++;
    }

  if (outputText)
    {
      anchor                       = "";
      boldStart                    = "";
      boldStop                     = "";
      breakBoth                    = "\n";
      breakHtmlOnly                = "";
      breakTextOnly                = "\n";
      brief                        = "";
      commentStart                 = "===============================================================\n";
      commentStop                  = "";
      defgroupAttributeListStart   = "";
      defgroupAttributeListStop    = "\n";
      defgroupGlobalValueListStart = "";
      defgroupGlobalValueListStop  = "";
      defgroupTraceSourceListStart = "";
      defgroupTraceSourceListStop  = "\n";
      functionStart                = "";
      functionStop                 = "\n\n";
      indentHtmlOnly               = "";
      ingroupConstructs            = "";
      listStart                    = "";
      listStop                     = "";
      listLineStart                = "    * ";
      listLineStop                 = "";
      reference                    = "";
      temporaryCharacter           = "";
    }
  else
    {
      anchor                       = "\\anchor ";
      boldStart                    = "<b>";
      boldStop                     = "</b>";
      breakBoth                    = "<br>";
      breakHtmlOnly                = "<br>";
      breakTextOnly                = "";
      brief                        = "\\brief ";
      commentStart                 = "/*!";
      commentStop                  = "*/";
      defgroupAttributeListStart   = "\\defgroup AttributeList ";
      defgroupAttributeListStop    = "";
      defgroupGlobalValueListStart = "\\defgroup GlobalValueList ";
      defgroupGlobalValueListStop  = "";
      defgroupTraceSourceListStart = "\\defgroup TraceSourceList ";
      defgroupTraceSourceListStop  = "";
      functionStart                = "\\fn ";
      functionStop                 = "";
      indentHtmlOnly               = "  ";
      ingroupConstructs            = "\\ingroup constructs\n";
      listStart                    = "<ul>";
      listStop                     = "</ul>";
      listLineStart                = "<li>";
      listLineStop                 = "</li>";
      reference                    = "\\ref ";
      temporaryCharacter           = "%";
    }

  NodeContainer c; c.Create (1);

  StaticInformation info;
  info.RecordAggregationInfo ("ns3::Node", "ns3::TcpSocketFactory");
  info.RecordAggregationInfo ("ns3::Node", "ns3::UdpSocketFactory");
  info.RecordAggregationInfo ("ns3::Node", "ns3::PacketSocketFactory");
  info.RecordAggregationInfo ("ns3::Node", "ns3::olsr::RoutingProtocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::MobilityModel");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Ipv4L3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::ArpL3Protocol");

  for (uint32_t i = 0; i < Config::GetRootNamespaceObjectN (); ++i)
    {
      Ptr<Object> object = Config::GetRootNamespaceObject (i);
      info.Gather (object->GetInstanceTypeId ());
    }

  std::map< std::string, uint32_t> nameMap;
  std::map< std::string, uint32_t>::const_iterator nameMapIterator;

  // Create a map from the class names to their index in the vector of
  // TypeId's so that the names will end up in alphabetical order.
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.MustHideFromDocumentation ())
	{
	  continue;
	}

      // Capitalize all of letters in the name so that it sorts
      // correctly in the map.
      std::string name = tid.GetName ();
      for (uint32_t j = 0; j < name.length (); j++)
	{
	  name[j] = toupper (name[j]);
	}

      // Save this name's index.
      nameMap[name] = i;
    }

  // Iterate over the map, which will print the class names in
  // alphabetical order.
  for (nameMapIterator = nameMap.begin (); nameMapIterator != nameMap.end (); nameMapIterator++)
    {
      // Get the class's index out of the map;
      uint32_t i = nameMapIterator->second;

      std::cout << commentStart << std::endl;
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << functionStart << "static TypeId " << tid.GetName () << "::GetTypeId (void)" << functionStop << std::endl;
      std::cout << brief << "This method returns the TypeId associated to " << reference << tid.GetName () << "."
		<< std::endl << std::endl;
      std::vector<std::string> paths = info.Get (tid);
      if (!paths.empty ())
	{
	  std::cout << "This object is accessible through the following paths with Config::Set and Config::Connect:" 
		    << std::endl;
	  std::cout << listStart << std::endl;
	  for (uint32_t k = 0; k < paths.size (); ++k)
	    {
	      std::string path = paths[k];
	      std::cout << listLineStart << path << listLineStop << breakTextOnly << std::endl;
	    }
	  std::cout << listStop << std::endl;
	}
      if (tid.GetAttributeN () == 0)
	{
	  std::cout << "No Attributes defined for this type." << breakBoth << std::endl;
	}
      else
	{
	  std::cout << "Attributes defined for this type:" << breakHtmlOnly << std::endl;
	  PrintAttributes (tid, std::cout);
	}
      {
	TypeId tmp = tid.GetParent ();
	while (tmp.GetParent () != tmp)
	  {
	    if (tmp.GetAttributeN () != 0)
	      {
		std::cout << "Attributes defined in parent class " << tmp.GetName () << ":" << breakHtmlOnly << std::endl;
		PrintAttributes (tmp, std::cout);
	      }
	    tmp = tmp.GetParent ();
	  }
      }
      if (tid.GetTraceSourceN () == 0)
	{
	  std::cout << "No TraceSources defined for this type." << breakBoth << std::endl;
	}
      else
	{
	  std::cout << "TraceSources defined for this type:" << breakHtmlOnly << std::endl;
	  PrintTraceSources (tid, std::cout);
	}
      {
	TypeId tmp = tid.GetParent ();
	while (tmp.GetParent () != tmp)
	  {
	    if (tmp.GetTraceSourceN () != 0)
	      {
		std::cout << "TraceSources defined in parent class " << tmp.GetName () << ":" << breakHtmlOnly << std::endl;
		PrintTraceSources (tmp, std::cout);
	      }
	    tmp = tmp.GetParent ();
	  }
      }
      std::cout << commentStop << std::endl;
    }


  std::cout << commentStart << std::endl
            << ingroupConstructs
            << defgroupTraceSourceListStart << "The list of all trace sources." << defgroupTraceSourceListStop << std::endl;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetTraceSourceN () == 0 ||
	  tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << boldStart << tid.GetName () << boldStop << breakHtmlOnly << std::endl
		<< listStart << std::endl;
      for (uint32_t j = 0; j < tid.GetTraceSourceN (); ++j)
	{
	  struct TypeId::TraceSourceInformation info = tid.GetTraceSource(j);
	  std::cout << listLineStart << info.name << ": " << info.help << listLineStop << std::endl;
	}
      std::cout << listStop << std::endl;
    }
  std::cout << commentStop << std::endl;

  std::cout << commentStart << std::endl
            << ingroupConstructs
            << defgroupAttributeListStart << "The list of all attributes." << defgroupAttributeListStop << std::endl;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetAttributeN () == 0 ||
	  tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << boldStart << tid.GetName () << boldStop << breakHtmlOnly << std::endl
		<< listStart << std::endl;
      for (uint32_t j = 0; j < tid.GetAttributeN (); ++j)
	{
	  struct TypeId::AttributeInformation info = tid.GetAttribute(j);
	  std::cout << listLineStart << info.name << ": " << info.help << listLineStop << std::endl;
	}
      std::cout << listStop << std::endl;
    }
  std::cout << commentStop << std::endl;



  std::cout << commentStart << std::endl
            << ingroupConstructs
            << defgroupGlobalValueListStart << "The list of all global values." << defgroupGlobalValueListStop << std::endl
            << listStart << std::endl;
  for (GlobalValue::Iterator i = GlobalValue::Begin (); i != GlobalValue::End (); ++i)
    {
      StringValue val;
      (*i)->GetValue (val);
      std::cout << indentHtmlOnly << listLineStart << boldStart << anchor << "GlobalValue" << (*i)->GetName () << " " << (*i)->GetName () << boldStop << ": " << (*i)->GetHelp () << "(" << val.Get () << ")" << listLineStop << std::endl;
    }
  std::cout << listStop << std::endl
	    << commentStop << std::endl;


  return 0;
}
