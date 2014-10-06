#include <iostream>
#include <algorithm>
#include <map>

#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/global-value.h"
#include "ns3/log.h"
#include "ns3/object-vector.h"
#include "ns3/object.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/node-container.h"
#include "ns3/csma-channel.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PrintIntrospectedDoxygen");

namespace
{
  std::string anchor;              ///< hyperlink anchor
  std::string boldStart;           ///< start of bold span
  std::string boldStop;            ///< end of bold span
  std::string breakBoth;           ///< linebreak
  std::string breakHtmlOnly;       ///< linebreak for html output only
  std::string breakTextOnly;       ///< linebreak for text output only
  std::string brief;               ///< brief tag
  std::string commentStart;        ///< start of code comment
  std::string commentStop;         ///< end of code comment
  std::string flagSpanStart;       ///< start of Attribute flag value
  std::string flagSpanStop;        ///< end of Attribute flag value
  std::string functionStart;       ///< start of a class/function
  std::string functionStop;        ///< end of a class/function
  std::string headingStart;        ///< start of section heading (h3)
  std::string headingStop;         ///< end of section heading (h3)
  std::string indentHtmlOnly;      ///< small indent
  std::string listLineStart;       ///< start unordered list item
  std::string listLineStop;        ///< end unordered list item
  std::string listStart;           ///< start unordered list
  std::string listStop;            ///< end unordered list
  std::string pageAttributeList;   ///< start Attributes list
  std::string pageGlobalValueList; ///< start GlobalValue page
  std::string pageTraceSourceList; ///< start Trace sources page
  std::string reference;           ///< reference tag
  std::string temporaryCharacter;  ///< "%" placeholder

} // anonymous namespace


void
SetMarkup (bool outputText)
{
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
      flagSpanStart                = "";
      flagSpanStop                 = "";
      functionStart                = "";
      functionStop                 = "\n\n";
      headingStart                 = "";
      headingStop                  = "";
      indentHtmlOnly               = "";
      pageAttributeList            = "";
      pageGlobalValueList          = "";
      pageTraceSourceList          = "";
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
      commentStart                 = "/*!\n";
      commentStop                  = "*/\n";
      flagSpanStart                = "<span class=\"mlabel\">";
      flagSpanStop                 = "</span>";
      functionStart                = "\\class ";
      functionStop                 = "";
      headingStart                 = "<h3>";
      headingStop                  = "</h3>";
      indentHtmlOnly               = "  ";
      pageAttributeList            = "\\page AttributesList ";
      pageGlobalValueList          = "\\page GlobalValueList ";
      pageTraceSourceList          = "\\page TraceSourceList ";
      listStart                    = "<ul>";
      listStop                     = "</ul>";
      listLineStart                = "<li>";
      listLineStop                 = "</li>";
      reference                    = "\\ref ";
      temporaryCharacter           = "%";
    }
}  // SetMarkup ()


// Print direct Attributes for this tid
void
PrintAttributesTid (std::ostream &os, const TypeId tid)
{
  os << listStart << std::endl;
  for (uint32_t j = 0; j < tid.GetAttributeN (); j++)
    {
      struct TypeId::AttributeInformation info = tid.GetAttribute(j);
      os << listLineStart
	 <<   boldStart << info.name << boldStop << ": "
	 <<   info.help
	 << std::endl;
      os <<   "  "
	 <<   listStart
	 << std::endl 
	 <<     "    "
	 <<     listLineStart
	 <<       "Set with class: " << reference
	 <<       info.checker->GetValueTypeName ()
	 <<     listLineStop
	 << std::endl;
      if (info.checker->HasUnderlyingTypeInformation ())
	{
	  os << "    "
	     << listLineStart
	     <<   "Underlying type: ";
	  if (    (info.checker->GetValueTypeName () != "ns3::EnumValue")
	       && (info.checker->GetUnderlyingTypeInformation () != "std::string")
	      )
	    {
	      // Two indirect cases to handle
	      bool handled = false;

	      if (info.checker->GetValueTypeName () == "ns3::PointerValue")
		{
		  const PointerChecker *ptrChecker =
		    dynamic_cast<const PointerChecker *> (PeekPointer (info.checker));
		  if (ptrChecker != 0)
		    {
		      os << reference << "ns3::Ptr" << "< "
			 << reference << ptrChecker->GetPointeeTypeId ().GetName ()
			 << ">";
		      handled = true;
		    }
		}
	      else if (info.checker->GetValueTypeName () == "ns3::ObjectPtrContainerValue")
		{
		  const ObjectPtrContainerChecker * ptrChecker =
		    dynamic_cast<const ObjectPtrContainerChecker *> (PeekPointer (info.checker));
		  if (ptrChecker != 0)
		    {
		      os << reference << "ns3::Ptr" << "< "
			 << reference << ptrChecker->GetItemTypeId ().GetName ()
			 << ">";
		      handled = true;
		    }
		}
	      if (! handled)
		{
		  os << reference << info.checker->GetUnderlyingTypeInformation ();
		}
	    }
	  os << listLineStop << std::endl;
	}
      if (info.flags & TypeId::ATTR_CONSTRUCT && info.accessor->HasSetter ())
	{
	  os << "    "
	     << listLineStart
	     <<   "Initial value: "
	     <<   info.initialValue->SerializeToString (info.checker)
	     << listLineStop
	     << std::endl;
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
      os << "  "
	 << listStop
	 << " "
	 << std::endl;
      
    }
  os << listStop << std::endl;
}


// Print the Attributes block for tid,
// including Attributes declared in base classes.
void
PrintAttributes (std::ostream & os, const TypeId tid)
{
  if (tid.GetAttributeN () == 0)
    {
      os << "No Attributes are defined for this type."
	 << breakBoth
	 << std::endl;
    }
  else
    {
      os << headingStart
	 <<   "Attributes"
	 << headingStop
	 << std::endl;
      PrintAttributesTid (os, tid);
    }

  // Attributes from base classes
  TypeId tmp = tid.GetParent ();
  while (tmp.GetParent () != tmp)
    {
      if (tmp.GetAttributeN () != 0)
	{
	  os << headingStart
	     <<   "Attributes defined in parent class "
	     <<   tmp.GetName ()
	     << headingStop
	     << std::endl;
	  PrintAttributesTid (os, tmp);
	}
      tmp = tmp.GetParent ();

    }  // Attributes
} // PrintAttributes ()


// Print direct Trace sources for this tid
void
PrintTraceSourcesTid (std::ostream & os, const TypeId tid)
{
  os << listStart << std::endl;
  for (uint32_t i = 0; i < tid.GetTraceSourceN (); ++i)
    {
      struct TypeId::TraceSourceInformation info = tid.GetTraceSource (i);
      os << listLineStart
	 <<   boldStart << info.name << boldStop << ": "
	 <<   info.help << breakBoth
	//    '%' prevents doxygen from linking to the Callback class...
	 <<   "%Callback signature: " 
	 <<   info.callback
	 <<   std::endl;
      os << listLineStop << std::endl;
    }
  os << listStop << std::endl;
}


// Print the Trace Sources block for this tid,
// including Trace sources declared in base classes.
void
PrintTraceSources (std::ostream & os, const TypeId tid)
{
  if (tid.GetTraceSourceN () == 0)
    {
      os << "No TraceSources are defined for this type."
	 << breakBoth
	 << std::endl;
    }
  else
    {
      os << headingStart
	 <<   "TraceSources"
	 << headingStop  << std::endl;
      PrintTraceSourcesTid (os, tid);
    }

  // Trace sources from base classes
  TypeId tmp = tid.GetParent ();
  while (tmp.GetParent () != tmp)
    {
      if (tmp.GetTraceSourceN () != 0)
	{
	  os << headingStart
	     << "TraceSources defined in parent class "
	     << tmp.GetName ()
	     << headingStop << std::endl;
	  PrintTraceSourcesTid (os, tmp);
	}
      tmp = tmp.GetParent ();
    }

}  // PrintTraceSources ()


void
PrintAllTraceSources (std::ostream & os)
{
   os << commentStart << pageTraceSourceList << "All TraceSources\n"
      << std::endl;

  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetTraceSourceN () == 0 ||
	  tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      os << boldStart << tid.GetName () << boldStop  << breakHtmlOnly
	 << std::endl;
      
      os << listStart << std::endl;
      for (uint32_t j = 0; j < tid.GetTraceSourceN (); ++j)
	{
	  struct TypeId::TraceSourceInformation info = tid.GetTraceSource(j);
	  os << listLineStart 
	     <<   boldStart << info.name << boldStop
	     <<   ": "      << info.help
	     << listLineStop
	     << std::endl;
	}
      os << listStop << std::endl;
    }
  os << commentStop << std::endl;

}  // PrintAllTraceSources ()


void
PrintAllAttributes (std::ostream & os)
{
  
  os << commentStart << pageAttributeList << "All Attributes\n"
     << std::endl;

  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); ++i)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.GetAttributeN () == 0 ||
	  tid.MustHideFromDocumentation ())
	{
	  continue;
	}
      os << boldStart << tid.GetName () << boldStop << breakHtmlOnly
	 << std::endl;
      
      os << listStart << std::endl;
      for (uint32_t j = 0; j < tid.GetAttributeN (); ++j)
	{
	  struct TypeId::AttributeInformation info = tid.GetAttribute(j);
	  os << listLineStart
	     <<   boldStart << info.name << boldStop
	     <<   ": "      << info.help
	     << listLineStop
	     << std::endl;
	}
      os << listStop << std::endl;
    }
  os << commentStop << std::endl;

}  // PrintAllAttributes ()


void
PrintAllGlobals (std::ostream & os)
{
  
  os << commentStart << pageGlobalValueList << "All GlobalValues\n"
     << std::endl;
  
  os << listStart << std::endl;
  for (GlobalValue::Iterator i = GlobalValue::Begin ();
       i != GlobalValue::End ();
       ++i)
    {
      StringValue val;
      (*i)->GetValue (val);
      os << indentHtmlOnly
	 <<   listLineStart
	 <<     boldStart
	 <<       anchor
	 <<       "GlobalValue" << (*i)->GetName () << " " << (*i)->GetName ()
	 <<     boldStop
	 <<     ": "            << (*i)->GetHelp ()
	 <<     ".  Default value: " << val.Get () << "."
	 <<   listLineStop
	 << std::endl;
    }
  os << listStop << std::endl;
  os << commentStop << std::endl;

}  // PrintAllGlobals ()


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
  std::vector<std::string> Get (TypeId tid) const;

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
StaticInformation::Get (TypeId tid) const
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


StaticInformation
GetTypicalAggregations ()
{
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

  return info;

}  // GetTypicalAggregations ()


// Map from TypeId name to tid
typedef std::map< std::string, uint32_t> NameMap;
typedef NameMap::const_iterator          NameMapIterator;


// Create a map from the class names to their index in the vector of
// TypeId's so that the names will end up in alphabetical order.
NameMap
GetNameMap (void)
{
  NameMap nameMap;
  
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
  return nameMap;
}  // GetNameMap ()


void
PrintConfigPaths (std::ostream & os, const StaticInformation & info,
		  const TypeId tid)
{
  std::vector<std::string> paths = info.Get (tid);

  // Config --------------
  if (paths.empty ())
    {
      os << "Doxygen introspection did not find any typical Config paths."
	 << breakBoth
	 << std::endl;
    }
  else
    {
      os << headingStart
	 <<   "Config Paths"
	 << headingStop
	 << std::endl;
      os << std::endl;
      os << tid.GetName ()
	 << " is accessible through the following paths"
	 << " with Config::Set and Config::Connect:"
	 << std::endl;
      os << listStart << std::endl;
      for (uint32_t k = 0; k < paths.size (); ++k)
	{
	  std::string path = paths[k];
	  os << listLineStart
	     <<   "\"" << path << "\""
	     << listLineStop
	     << breakTextOnly
	     << std::endl;
	}
      os << listStop << std::endl;
    }
}  // PrintConfigPaths
      

int main (int argc, char *argv[])
{
  bool outputText = false;

  CommandLine cmd;
  cmd.Usage ("Generate documentation for all ns-3 registered types, "
	     "trace sources, attributes and global variables.");
  cmd.AddValue ("output-text", "format output as plain text", outputText);
  cmd.Parse (argc, argv);
    
  SetMarkup (outputText);


  // Create a Node, to force linking and instantiation of our TypeIds
  NodeContainer c;
  c.Create (1);

  // Get typical aggregation relationships.
  StaticInformation info = GetTypicalAggregations ();
  
  NameMap nameMap = GetNameMap ();

  // Iterate over the map, which will print the class names in
  // alphabetical order.
  for (NameMapIterator nameMapIterator = nameMap.begin ();
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

      PrintConfigPaths (std::cout, info, tid);
      PrintAttributes (std::cout, tid);
      PrintTraceSources (std::cout, tid);
      
      std::cout << commentStop << std::endl;
    }  // class documentation


  PrintAllTraceSources (std::cout);
  PrintAllAttributes (std::cout);
  PrintAllGlobals (std::cout);

  return 0;
}
