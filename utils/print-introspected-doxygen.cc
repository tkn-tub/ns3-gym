#include <iostream>
#include <algorithm>
#include <map>
#include <limits.h>

#include "ns3/object.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/global-value.h"
#include "ns3/string.h"
#include "ns3/node-container.h"
#include "ns3/csma-channel.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PrintIntrospectedDoxygen");

namespace
{
  std::string anchor;                        ///< hyperlink anchor
  std::string boldStart;                     ///< start of bold span
  std::string boldStop;                      ///< end of bold span
  std::string breakBoth;                     ///< linebreak
  std::string breakHtmlOnly;                 ///< linebreak for html output only
  std::string breakTextOnly;                 ///< linebreak for text output only
  std::string brief;                         ///< brief tag
  std::string commentStart;                  ///< start of code comment
  std::string commentStop;                   ///< end of code comment
  std::string defgroupAttributeListStart;    ///< start of AttributeList group
  std::string defgroupAttributeListStop;     ///< end of AttributeList group
  std::string defgroupGlobalValueListStart;  ///< start of GlobalValueList group
  std::string defgroupGlobalValueListStop;   ///< end of GlobalValueList group
  std::string defgroupTraceSourceListStart;  ///< start of TraceSourceList group
  std::string defgroupTraceSourceListStop;   ///< end of TraceSourceList group
  std::string flagSpanStart;                 ///< start of Attribute flag value
  std::string flagSpanStop;                  ///< end of Attribute flag value
  std::string functionStart;                 ///< start of a class/function
  std::string functionStop;                  ///< end of a class/function
  std::string headingStart;                  ///< start of section heading (h3)
  std::string headingStop;                   ///< end of section heading (h3)
  std::string indentHtmlOnly;                ///< small indent
  std::string ingroupConstructs;             ///< add to constructs group
  std::string listStart;                     ///< start unordered list
  std::string listStop;                      ///< end unordered list
  std::string listLineStart;                 ///< start unordered list item
  std::string listLineStop;                  ///< end unordered list item
  std::string reference;                     ///< reference tag
  std::string temporaryCharacter;            ///< "%" placeholder

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
	  os << "    " << listLineStart << "Underlying type: ";
	  if (    (info.checker->GetValueTypeName () != "ns3::EnumValue")
	       && (info.checker->GetUnderlyingTypeInformation () != "std::string")
	      )
	    {
	      os << reference;
	    }
	  os << info.checker->GetUnderlyingTypeInformation () << listLineStop << std::endl;
	}
      if (info.flags & TypeId::ATTR_CONSTRUCT && info.accessor->HasSetter ())
	{
	  os << "    " << listLineStart << "Initial value: " << info.initialValue->SerializeToString (info.checker) << listLineStop << std::endl;
	}
      os << "    " << listLineStart << "Flags: ";
      if (info.flags & TypeId::ATTR_CONSTRUCT && info.accessor->HasSetter ())
	{
	  os << flagSpanStart << "construct " << flagSpanStop;
	}
      if (info.flags & TypeId::ATTR_SET && info.accessor->HasSetter ())
	{
	  os << flagSpanStart << "write " << flagSpanStop;
	}
      if (info.flags & TypeId::ATTR_GET && info.accessor->HasGetter ())
	{
	  os << flagSpanStart << "read " << flagSpanStop;
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


/**
 * Gather aggregation and configuration path information from registered types.
 */
class StaticInformation
{
public:
  /**
   * Record the a -> b aggregation relation.
   *
   * \param a [in] the source(?) TypeId name
   * \param b [in] the destination(?) TypeId name
   */
  void RecordAggregationInfo (std::string a, std::string b);
  /**
   * Gather aggregation and configuration path information for tid
   *
   * \param tid [in] the TypeId to gather information from
   */
  void Gather (TypeId tid);
  /**
   * Print output in "a -> b" form on std::cout
   */
  void Print (void) const;

  /**
   * \return the configuration paths for tid
   *
   * \param tid [in] the TypeId to return information for
   */
  std::vector<std::string> Get (TypeId tid);

private:
  /**
   * \return the current configuration path
   */
  std::string GetCurrentPath (void) const;
  /**
   * Gather attribute, configuration path information for tid
   *
   * \param tid [in] the TypeId to gather information from
   */
  void DoGather (TypeId tid);
  /**
   *  Record the current config path for tid.
   *
   * \param tid [in] the TypeId to record.
   */
  void RecordOutput (TypeId tid);
  /**
   * \return whether the tid has already been processed
   *
   * \param tid [in] the TypeId to check.
   */
  bool HasAlreadyBeenProcessed (TypeId tid) const;
  /**
   * (Inplace) find and replace all instances of string
   *
   * \param source [inout] string to search and replace in
   * \param find [in] string to search for
   * \param replace [in] string to insert in place of find
   */
  void find_and_replace (std::string &source, const std::string find, std::string replace );
  /**
   * Configuration path for each TypeId
   */
  std::vector<std::pair<TypeId,std::string> > m_output;
  /**
   * Current configuration path
   */
  std::vector<std::string> m_currentPath;
  /**
   * List of TypeIds we've already processed
   */
  std::vector<TypeId> m_alreadyProcessed;
  /**
   * List of aggregation relationships.
   */
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

	  // See if this is a pointer to an Object.
	  Ptr<Object> object = CreateObject<Object> ();
	  TypeId objectTypeId = object->GetTypeId ();
	  if (objectTypeId == pointee)
	    {
	      // Stop the recursion at this attribute if it is a
	      // pointer to an Object, which create too many spurious
	      // paths in the list of attribute paths because any
	      // Object can be in that part of the path.
	      continue;
	    }

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
      flagSpanStart                = "";
      flagSpanStop                 = "";
      functionStart                = "";
      functionStop                 = "\n\n";
      headingStart                 = "";
      headingStop                  = "";
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
      flagSpanStart                = "<span class=\"mlabel\">";
      flagSpanStop                 = "</span>";
      functionStart                = "\\class ";
      functionStop                 = "";
      headingStart                 = "<h3>";
      headingStop                  = "</h3>";
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

  // The below statements register typical aggregation relationships
  // in ns-3 programs, that otherwise aren't picked up automatically
  // by the creation of the above node.  To manually list other common
  // aggregation relationships that you would like to see show up in
  // the list of configuration paths in the doxygen, add additional
  // statements below.
  StaticInformation info;
  info.RecordAggregationInfo ("ns3::Node", "ns3::TcpSocketFactory");
  info.RecordAggregationInfo ("ns3::Node", "ns3::UdpSocketFactory");
  info.RecordAggregationInfo ("ns3::Node", "ns3::PacketSocketFactory");
  info.RecordAggregationInfo ("ns3::Node", "ns3::olsr::RoutingProtocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::MobilityModel");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Ipv4L3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::ArpL3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Icmpv4L4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::UdpL4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Ipv6L3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Icmpv6L4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::TcpL4Protocol");

  // Create a channel object so that channels appear in the namespace
  // paths that will be generated here.
  Ptr<CsmaChannel> csma;
  csma = CreateObject<CsmaChannel> ();

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
  for (nameMapIterator = nameMap.begin ();
       nameMapIterator != nameMap.end ();
       nameMapIterator++)
    {
      // Get the class's index out of the map;
      uint32_t i = nameMapIterator->second;

      std::cout << commentStart << std::endl;
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << functionStart << tid.GetName () << std::endl;
      std::cout << std::endl;
      std::vector<std::string> paths = info.Get (tid);

      // Config --------------
      if (paths.empty ())
	{
	  std::cout << "Doxygen introspection did not find any typical Config paths."
		    << breakBoth << std::endl;
	}
      else
	{
	  std::cout << headingStart
		    << "Config Paths"
		    << headingStop << std::endl;
	  std::cout << std::endl;
	  std::cout << tid.GetName ()
		    << " is accessible through the following paths"
		    << " with Config::Set and Config::Connect:"
		    << std::endl;
	  std::cout << listStart << std::endl;
	  for (uint32_t k = 0; k < paths.size (); ++k)
	    {
	      std::string path = paths[k];
	      std::cout << listLineStart << path
			<< listLineStop  << breakTextOnly << std::endl;
	    }
	  std::cout << listStop << std::endl;
	}  // Config

      // Attributes ----------
      if (tid.GetAttributeN () == 0)
	{
	  std::cout << "No Attributes are defined for this type."
		    << breakBoth << std::endl;
	}
      else
	{
	  std::cout << headingStart << "Attributes"
		    << headingStop  << std::endl;
	  PrintAttributes (tid, std::cout);

	  TypeId tmp = tid.GetParent ();
	  while (tmp.GetParent () != tmp)
	    {
	      if (tmp.GetAttributeN () != 0)
		{
		  std::cout << headingStart
			    << "Attributes defined in parent class "
			    << tmp.GetName ()
			    << headingStop << std::endl;
		  PrintAttributes (tmp, std::cout);
		}
	      tmp = tmp.GetParent ();
	    }
	}  // Attributes

      // Tracing -------------
      if (tid.GetTraceSourceN () == 0)
	{
	  std::cout << "No TraceSources are defined for this type."
		    << breakBoth << std::endl;
	}
      else
	{
	  std::cout << headingStart << "TraceSources"
		    << headingStop  << std::endl;
	  PrintTraceSources (tid, std::cout);
	}
      {
	TypeId tmp = tid.GetParent ();
	while (tmp.GetParent () != tmp)
	  {
	    if (tmp.GetTraceSourceN () != 0)
	      {
		std::cout << headingStart
			  << "TraceSources defined in parent class "
			  << tmp.GetName ()
			  << headingStop << std::endl;
		PrintTraceSources (tmp, std::cout);
	      }
	    tmp = tmp.GetParent ();
	  }
      }
      std::cout << commentStop << std::endl;
    }  // class documentation


  std::cout << commentStart << std::endl
            << ingroupConstructs
            << defgroupTraceSourceListStart << "The list of all trace sources."
	    << defgroupTraceSourceListStop << std::endl;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetTraceSourceN () == 0 ||
	  tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << boldStart << tid.GetName ()
		<< boldStop  << breakHtmlOnly << std::endl
		<< listStart << std::endl;
      for (uint32_t j = 0; j < tid.GetTraceSourceN (); ++j)
	{
	  struct TypeId::TraceSourceInformation info = tid.GetTraceSource(j);
	  std::cout << listLineStart << info.name << ": " << info.help
		    << listLineStop  << std::endl;
	}
      std::cout << listStop << std::endl;
    }
  std::cout << commentStop << std::endl;

  std::cout << commentStart << std::endl
            << ingroupConstructs
            << defgroupAttributeListStart << "The list of all attributes."
	    << defgroupAttributeListStop  << std::endl;
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetAttributeN () == 0 ||
	  tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      std::cout << boldStart << tid.GetName ()
		<< boldStop  << breakHtmlOnly << std::endl
		<< listStart << std::endl;
      for (uint32_t j = 0; j < tid.GetAttributeN (); ++j)
	{
	  struct TypeId::AttributeInformation info = tid.GetAttribute(j);
	  std::cout << listLineStart << info.name << ": " << info.help
		    << listLineStop  << std::endl;
	}
      std::cout << listStop << std::endl;
    }
  std::cout << commentStop << std::endl;



  std::cout << commentStart << std::endl
            << ingroupConstructs
            << defgroupGlobalValueListStart << "The list of all global values."
	    << defgroupGlobalValueListStop  << std::endl
            << listStart << std::endl;
  for (GlobalValue::Iterator i = GlobalValue::Begin ();
       i != GlobalValue::End ();
       ++i)
    {
      StringValue val;
      (*i)->GetValue (val);
      std::cout << indentHtmlOnly
		<<   listLineStart
		<<     boldStart
		<<       anchor
		<< "GlobalValue" << (*i)->GetName () << " " << (*i)->GetName ()
		<<     boldStop
		<< ": " << (*i)->GetHelp () << ".  Default value: " << val.Get () << "."
		<<   listLineStop << std::endl;
    }
  std::cout << listStop    << std::endl
	    << commentStop << std::endl;


  return 0;
}
