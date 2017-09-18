/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

/**
 * \file
 * \ingroup utils
 * Generate documentation from the TypeId database.
 */

#include <iostream>
#include <algorithm>
#include <map>
#include <climits>    // CHAR_BIT

#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/global-value.h"
#include "ns3/log.h"
#include "ns3/object-vector.h"
#include "ns3/object.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/node-container.h"
#include "ns3/simple-channel.h"
#include "ns3/system-path.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PrintIntrospectedDoxygen");

namespace
{
  std::string anchor;              ///< hyperlink anchor
  std::string argument;            ///< function argument
  std::string boldStart;           ///< start of bold span
  std::string boldStop;            ///< end of bold span
  std::string breakBoth;           ///< linebreak
  std::string breakHtmlOnly;       ///< linebreak for html output only
  std::string breakTextOnly;       ///< linebreak for text output only
  std::string brief;               ///< brief tag
  std::string classStart;          ///< start of a class
  std::string classStop;           ///< end of a class
  std::string codeWord;            ///< format next word as source code
  std::string commentStart;        ///< start of code comment
  std::string commentStop;         ///< end of code comment
  std::string copyDoc;             ///< copy (or refer) to docs elsewhere
  std::string file;                ///< file 
  std::string flagSpanStart;       ///< start of Attribute flag value
  std::string flagSpanStop;        ///< end of Attribute flag value
  std::string functionStart;       ///< start of a method/function
  std::string functionStop;        ///< end of a method/function
  std::string headingStart;        ///< start of section heading (h3)
  std::string headingStop;         ///< end of section heading (h3)
  std::string indentHtmlOnly;      ///< small indent
  std::string listLineStart;       ///< start unordered list item
  std::string listLineStop;        ///< end unordered list item
  std::string listStart;           ///< start unordered list
  std::string listStop;            ///< end unordered list
  std::string note;                ///< start a note section
  std::string page;                ///< start a separate page
  std::string reference;           ///< reference tag
  std::string returns;             ///< the return value
  std::string sectionStart;        ///< start of a section or group
  std::string seeAlso;             ///< Reference to other docs
  std::string subSectionStart;     ///< start a new subsection
  std::string templArgDeduced;     ///< template argument deduced from function
  std::string templArgExplicit;    ///< template argument required
  std::string templateArgument;    ///< template argument
  std::string variable;            ///< variable or class member

}  // unnamed namespace


/**
 * Initialize the markup strings, for either doxygen or text.
 *
 * \param [in] outputText true for text output, false for doxygen output.
 */
void
SetMarkup (bool outputText)
{
  NS_LOG_FUNCTION (outputText);
  if (outputText)
    {
      anchor                       = "";
      argument                     = "  Arg: ";
      boldStart                    = "";
      boldStop                     = "";
      breakBoth                    = "\n";
      breakHtmlOnly                = "";
      breakTextOnly                = "\n";
      brief                        = "";
      classStart                   = "";
      classStop                    = "\n\n";
      codeWord                     = " ";
      commentStart                 = "===============================================================\n";
      commentStop                  = "";
      copyDoc                      = "  See: ";
      file                         = "File: ";
      flagSpanStart                = "";
      flagSpanStop                 = "";
      functionStart                = "";
      functionStop                 = "\n\n";
      headingStart                 = "";
      headingStop                  = "";
      indentHtmlOnly               = "";
      listLineStart                = "    * ";
      listLineStop                 = "";
      listStart                    = "";
      listStop                     = "";
      note                         = "Note: ";
      page                         = "Page ";
      reference                    = " ";
      returns                      = "  Returns: ";
      sectionStart                 = "Section ";
      seeAlso                      = "  See: ";
      subSectionStart              = "Subsection ";
      templArgDeduced              = "[deduced]  ";
      templArgExplicit             = "[explicit] ";
      templateArgument             = "Template Arg: ";
      variable                     = "Variable: ";
    }
  else
    {
      anchor                       = "\\anchor ";
      argument                     = "\\param ";
      boldStart                    = "<b>";
      boldStop                     = "</b>";
      breakBoth                    = "<br>";
      breakHtmlOnly                = "<br>";
      breakTextOnly                = "";
      brief                        = "\\brief ";
      classStart                   = "\\class ";
      classStop                    = "";
      codeWord                     = "\\p ";
      commentStart                 = "/*!\n";
      commentStop                  = "*/\n";
      copyDoc                      = "\\copydoc ";
      file                         = "\\file ";
      flagSpanStart                = "<span class=\"mlabel\">";
      flagSpanStop                 = "</span>";
      functionStart                = "\\fn ";
      functionStop                 = "";
      headingStart                 = "<h3>";
      headingStop                  = "</h3>";
      indentHtmlOnly               = "  ";
      listLineStart                = "<li>";
      listLineStop                 = "</li>";
      listStart                    = "<ul>";
      listStop                     = "</ul>";
      note                         = "\\note ";
      page                         = "\\page ";
      reference                    = " \\ref ";
      returns                      = "\\returns ";
      sectionStart                 = "\\ingroup ";
      seeAlso                      = "\\see ";
      subSectionStart              = "\\addtogroup ";
      templArgDeduced              = "\\deduced ";
      templArgExplicit             = "\\explicit ";
      templateArgument             = "\\tparam ";
      variable                     = "\\var ";
    }
}  // SetMarkup ()


/***************************************************************
 *        Docs for a single TypeId
 ***************************************************************/

/**
 * Print direct Attributes for this TypeId.
 *
 * Only attributes defined directly by this TypeId will be printed.
 *
 * \param [in,out] os The output stream.
 * \param [in] tid The TypeId to print.
 */
void
PrintAttributesTid (std::ostream &os, const TypeId tid)
{
  NS_LOG_FUNCTION (tid);
  os << listStart << std::endl;
  for (uint32_t j = 0; j < tid.GetAttributeN (); j++)
    {
      struct TypeId::AttributeInformation info = tid.GetAttribute(j);
      os << listLineStart
	 <<   boldStart << info.name << boldStop << ": "
	 <<   info.help
	 <<   std::endl;
      os <<   "  "
	 <<   listStart << std::endl;
      os <<     "    "
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
          
          std::string valType = info.checker->GetValueTypeName ();
          std::string underType = info.checker->GetUnderlyingTypeInformation ();
	  if ((valType   != "ns3::EnumValue") && (underType != "std::string"))
	    {
	      // Indirect cases to handle
	      bool handled = false;
              
	      if (valType == "ns3::PointerValue")
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
	      else if (valType == "ns3::ObjectPtrContainerValue")
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
              // Helper to match first part of string
              class StringBeginMatcher
              {
              public:
                StringBeginMatcher (const std::string s)
                  : m_string (s) { };
                bool operator () (const std::string t)
                {
                  std::size_t pos = m_string.find (t);
                  return pos == 0;
                };
              private:
                std::string m_string;
              };
              StringBeginMatcher match (underType);
                  
              if ( match ("bool")     || match ("double")   ||
                   match ("int8_t")   || match ("uint8_t")  ||
                   match ("int16_t")  || match ("uint16_t") ||
                   match ("int32_t")  || match ("uint32_t") ||
                   match ("int64_t")  || match ("uint64_t")
                   )
                {
                  os << underType;
                  handled = true;
                }
	      if (! handled)
		{
		  os << reference << underType;
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
	 << " " << std::endl;
      
    }
  os << listStop << std::endl;
}  // PrintAttributesTid ()


/**
 * Print the Attributes block for tid,
 * including Attributes declared in base classes.
 *
 * All Attributes of this TypeId will be printed,
 * including those defined in parent classes.
 *
 * \param [in,out] os The output stream.
 * \param [in] tid The TypeId to print.
 */
void
PrintAttributes (std::ostream & os, const TypeId tid)
{
  NS_LOG_FUNCTION (tid);
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


/**
 * Print direct Trace sources for this TypeId.
 *
 * Only Trace sources defined directly by this TypeId will be printed.
 *
 * \param [in,out] os The output stream.
 * \param [in] tid The TypeId to print.
 */
void
PrintTraceSourcesTid (std::ostream & os, const TypeId tid)
{
  NS_LOG_FUNCTION (tid);
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
}  // PrintTraceSourcesTid ()


/**
 * Print the Trace sources block for tid,
 * including Trace sources declared in base classes.
 *
 * All Trace sources of this TypeId will be printed,
 * including those defined in parent classes.
 *
 * \param [in,out] os The output stream.
 * \param [in] tid The TypeId to print.
 */
void
PrintTraceSources (std::ostream & os, const TypeId tid)
{
  NS_LOG_FUNCTION (tid);
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

/**
 * Print the size of the type represented by this tid.
 *
 * \param [in,out] os The output stream.
 * \param [in] tid The TypeId to print.
 */
void PrintSize (std::ostream & os, const TypeId tid)
{
  NS_LOG_FUNCTION (tid);
  NS_ASSERT_MSG (CHAR_BIT != 0, "CHAR_BIT is zero");
  
  std::size_t arch = (sizeof (void *) * CHAR_BIT);
  
  os << boldStart << "Size" << boldStop
     << " of this type is " << tid.GetSize ()
     << " bytes (on a " << arch << "-bit architecture)."
     << std::endl;
}  // PrintSize ()


/***************************************************************
 *        Lists of All things
 ***************************************************************/

/**
 * Print the list of all Attributes.
 *
 * \param [in,out] os The output stream.
 */
void
PrintAllAttributes (std::ostream & os)
{
  NS_LOG_FUNCTION_NOARGS ();
  os << commentStart << page << "AttributeList All Attributes\n"
     << std::endl;
  os << "This is a list of all" << reference << "attribute by class.  "
     << "For more information see the" << reference << "attribute "
     << "section of this API documentation and the Attributes sections "
     << "in the Tutorial and Manual.\n"
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


/**
 * Print the list of all global variables.
 *
 * \param [in,out] os The output stream.
 */
void
PrintAllGlobals (std::ostream & os)
{
  NS_LOG_FUNCTION_NOARGS ();
  os << commentStart << page << "GlobalValueList All GlobalValues\n"
     << std::endl;
  os << "This is a list of all" << reference << "ns3::GlobalValue instances.\n"
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
 * Print the list of all LogComponents.
 *
 * \param [in,out] os The output stream.
 */
void
PrintAllLogComponents (std::ostream & os)
{
  NS_LOG_FUNCTION_NOARGS ();
  os << commentStart << page << "LogComponentList All LogComponents\n"
     << std::endl;
  os << "This is a list of all" << reference << "ns3::LogComponent instances.\n"
     << std::endl;

  /**
   * \todo Switch to a border-less table, so the file links align
   * See http://www.stack.nl/~dimitri/doxygen/manual/htmlcmds.html
   */
  os << listStart << std::endl;
  LogComponent::ComponentList * logs = LogComponent::GetComponentList ();
  LogComponent::ComponentList::const_iterator it;
  for (it = logs->begin (); it != logs->end (); ++it)
    {
      std::string file = it->second->File ();
      // Strip leading "../" related to depth in build directory
      // since doxygen only sees the path starting with "src/", etc.
      while (file.find ("../") == 0)
        {
          file = file.substr (3);
        }
      
      os << listLineStart
         <<   boldStart << it->first << boldStop <<   ": " << file
         << listLineStop
         << std::endl;
    }
  os << listStop << std::endl;
  os << commentStop << std::endl;
}  // PrintAllLogComponents ()


/**
 * Print the list of all Trace sources.
 *
 * \param [in,out] os The output stream.
 */
void
PrintAllTraceSources (std::ostream & os)
{
  NS_LOG_FUNCTION_NOARGS ();
  os << commentStart << page << "TraceSourceList All TraceSources\n"
     << std::endl;
  os << "This is a list of all" << reference << "tracing sources.  "
     << "For more information see the " << reference << "tracing "
     << "section of this API documentation and the Tracing sections "
     << "in the Tutorial and Manual.\n"
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


/***************************************************************
 *        Docs for Attribute classes
 ***************************************************************/


/**
 * Print the section definition for an AttributeValue.
 *
 * In doxygen form this will print a comment block with
 * \verbatim
 *   \ingroup attribute
 *   \defgroup attribute_<name>Value <name>Value
 * \endverbatim
 *
 * \param [in,out] os The output stream.
 * \param [in] name The base name of the resulting AttributeValue type.
 * \param [in] seeBase Print a "see also" pointing to the base class.
 */
void
PrintAttributeValueSection (std::ostream & os,
                            const std::string & name,
                            const bool seeBase = true)
{
  NS_LOG_FUNCTION (name);
  std::string section = "attribute_" + name;

  // \ingroup attribute
  // \defgroup attribute_<name>Value <name> Attribute
  os << commentStart << sectionStart << "attribute\n"
     <<   subSectionStart << "attribute_" << name << " "
     <<     name << " Attribute\n"
     <<     "Attribute implementation for " << name << "\n";
  if (seeBase)
    {
      // Some classes don't live in ns3::.  Yuck
      if (name != "IeMeshId")
        {
          os << seeAlso << "ns3::" << name << "\n";
        }
      else
        {
          os << seeAlso << "ns3::dot11s::" << name << "\n";
        }
    }
  os << commentStop;

}  // PrintAttributeValueSection ()


/**
 * Print the AttributeValue documentation for a class.
 *
 * This will print documentation for the \p AttributeValue class and methods.
 *
 * \param [in,out] os The output stream.
 * \param [in] name The token to use in defining the accessor name.
 * \param [in] type The underlying type name.
 * \param [in] header The header file which contains this declaration.
 */
void
PrintAttributeValueWithName (std::ostream & os,
                             const std::string & name,
                             const std::string & type,
                             const std::string & header)
{
  NS_LOG_FUNCTION (name << type << header);
  std::string sectAttr = sectionStart + "attribute_" + name;
  
  // \ingroup attribute_<name>Value
  // \class ns3::<name>Value "header"
  std::string valClass  = name + "Value";
  std::string qualClass = " ns3::" + valClass;
  
  os << commentStart << sectAttr << std::endl;
  os <<   classStart << qualClass << " \"" << header << "\"" << std::endl;
  os <<   "AttributeValue implementation for " << name << "." << std::endl;
  os <<   seeAlso << "AttributeValue" << std::endl;
  os << commentStop;

  // Copy ctor: <name>Value::<name>Value
  os << commentStart
     <<   functionStart << name
     <<     qualClass << "::" << valClass;
  if ( (name == "EmptyAttribute") ||
       (name == "ObjectPtrContainer") )
    {
      // Just default constructors.
      os << "(void)\n";
    }
  else
    {
      // Copy constructors
      os << "(const " << type << " & value)\n"
         << "Copy constructor.\n"
         << argument << "[in] value The " << name << " value to copy.\n";
    }
  os << commentStop;

  // <name>Value::Get (void) const
  os << commentStart
     <<   functionStart << type
     <<     qualClass << "::Get (void) const\n"
     <<   returns << "The " << name << " value.\n"
     << commentStop;

  // <name>Value::GetAccessor (T & value) const
  os << commentStart
     <<   functionStart << "bool"
     <<     qualClass << "::GetAccessor (T & value) const\n"
     <<   "Access the " << name << " value as type " << codeWord << "T.\n"
     <<   templateArgument << "T " << templArgExplicit << "The type to cast to.\n"
     <<   argument << "[out] value The " << name << " value, as type "
     <<     codeWord << "T.\n"
     <<   returns << "true.\n"
     << commentStop;

  // <name>Value::Set (const name & value)
  if (type != "Callback")  // Yuck
    {
      os << commentStart
         <<   functionStart << "void"
         <<     qualClass << "::Set (const " << type << " & value)\n"
         <<   "Set the value.\n"
         <<   argument << "[in] value The value to adopt.\n"
         << commentStop;
    }

  // <name>Value::m_value
  os << commentStart
     <<   variable << type
     <<     qualClass << "::m_value\n" 
     <<   "The stored " << name << " instance.\n"
     << commentStop
     << std::endl;
  
}  // PrintAttributeValueWithName ()


/**
 * Print the AttributeValue MakeAccessor documentation for a class.
 *
 * This will print documentation for the \p Make<name>Accessor functions.
 *
 * \param [in,out] os The output stream.
 * \param [in] name The token to use in defining the accessor name.
 */
void
PrintMakeAccessors (std::ostream & os, const std::string & name)
{
  NS_LOG_FUNCTION (name);
  std::string sectAttr = sectionStart + "attribute_" + name + "\n";
  std::string make = "ns3::Make" + name + "Accessor ";
  
  // \ingroup attribute_<name>Value
  // Make<name>Accessor (T1 a1)
  os << commentStart << sectAttr
     <<   functionStart << "ns3::Ptr<const ns3::AttributeAccessor> "
     <<     make << "(T1 a1)\n"
     <<   copyDoc << "ns3::MakeAccessorHelper(T1)\n"
     <<   seeAlso << "AttributeAccessor\n"
     << commentStop;

  // \ingroup attribute_<name>Value
  // Make<name>Accessor (T1 a1)
  os << commentStart << sectAttr
     <<   functionStart << "ns3::Ptr<const ns3::AttributeAccessor> "
     <<     make << "(T1 a1, T2 a2)\n"
     <<   copyDoc << "ns3::MakeAccessorHelper(T1,T2)\n"
     <<   seeAlso << "AttributeAccessor\n"
     << commentStop;
}  // PrintMakeAccessors ()


/**
 * Print the AttributeValue MakeChecker documentation for a class.
 *
 * This will print documentation for the \p Make<name>Checker function.
 *
 * \param [in,out] os The output stream.
 * \param [in] name The token to use in defining the accessor name.
 * \param [in] header The header file which contains this declaration.
 */
void
PrintMakeChecker (std::ostream & os,
                  const std::string & name,
                  const std::string & header)
{
  NS_LOG_FUNCTION (name << header);
  std::string sectAttr = sectionStart + "attribute_" + name + "\n";
  std::string make = "ns3::Make" + name + "Checker ";

  // \ingroup attribute_<name>Value
  // class <name>Checker
  os << commentStart << sectAttr << std::endl;
  os <<   classStart << " ns3::" << name << "Checker"
     <<   " \"" << header << "\"" << std::endl;
  os <<   "AttributeChecker implementation for " << name << "Value." << std::endl;
  os <<   seeAlso << "AttributeChecker" << std::endl;
  os << commentStop;
    
  // \ingroup attribute_<name>Value
  // Make<name>Checker (void)
  os << commentStart << sectAttr
     <<   functionStart << "ns3::Ptr<const ns3::AttributeChecker> "
     <<     make << "(void)\n"
     <<   returns << "The AttributeChecker.\n"
     <<   seeAlso << "AttributeChecker\n"
     << commentStop;
}  // PrintMakeChecker ()


/**Descriptor for an AttributeValue. */
typedef struct {
  const std::string m_name;   //!< The base name of the resulting AttributeValue type.
  const std::string m_type;   //!< The name of the underlying type.
  const bool m_seeBase;       //!< Print a "see also" pointing to the base class.
  const std::string m_header; //!< The header file name.
} AttributeDescriptor;


/**
 * Print documentation corresponding to use of the
 * ATTRIBUTE_HELPER_HEADER macro or
 * ATTRIBUTE_VALUE_DEFINE_WITH_NAME macro.
 *
 * \param [in,out] os The output stream.
 * \param [in] attr The AttributeDescriptor.
 */
void
PrintAttributeHelper (std::ostream & os,
                      const AttributeDescriptor & attr)
{
  NS_LOG_FUNCTION (attr.m_name << attr.m_type << attr.m_seeBase <<
                   attr.m_header);
  PrintAttributeValueSection  (os, attr.m_name, attr.m_seeBase);
  PrintAttributeValueWithName (os, attr.m_name, attr.m_type, attr.m_header);
  PrintMakeAccessors          (os, attr.m_name);
  PrintMakeChecker            (os, attr.m_name, attr.m_header);
}  // PrintAttributeHelper ()


/**
 * Print documentation for Attribute implementations.
 */
void
PrintAttributeImplementations (std::ostream & os)
{
  NS_LOG_FUNCTION_NOARGS ();

  const AttributeDescriptor attributes [] =
    {
      // Name             Type             see Base  header-file
      // Users of ATTRIBUTE_HELPER_HEADER
      //
      { "Address",        "Address",        true,  "address.h"          },
      { "Box",            "Box",            true,  "box.h"              },
      { "DataRate",       "DataRate",       true,  "data-rate.h"        },
      { "DsssParameterSet",
                          "DsssParameterSet",
                                            true,  "dsss-parameter-set.h"},
      { "EdcaParameterSet",
                          "EdcaParameterSet",
                                            true,  "edca-parameter-set.h"},
      { "ErpInformation", "ErpInformation", true,  "erp-information.h"  },
      { "HeCapabilities", "HeCapabilities", true,  "he-capabilities.h"  },
      { "HtCapabilities", "HtCapabilities", true,  "ht-capabilities.h"  },
      { "IeMeshId",       "IeMeshId",       true,  "ie-dot11s-id.h"     },
      { "Ipv4Address",    "Ipv4Address",    true,  "ipv4-address.h"     },
      { "Ipv4Mask",       "Ipv4Mask",       true,  "ipv4-address.h"     },
      { "Ipv6Address",    "Ipv6Address",    true,  "ipv6-address.h"     },
      { "Ipv6Prefix",     "Ipv6Prefix",     true,  "ipv6-address.h"     },
      { "Mac16Address",   "Mac16Address",   true,  "mac16-address.h"    },
      { "Mac48Address",   "Mac48Address",   true,  "mac48-address.h"    },
      { "Mac64Address",   "Mac64Address",   true,  "mac64-address.h"    },
      { "ObjectFactory",  "ObjectFactory",  true,  "object-factory.h"   },
      { "OrganizationIdentifier",
                          "OrganizationIdentifier",
                                            true,  "vendor-specific-action.h" },
      { "Rectangle",      "Rectangle",      true,  "rectangle.h"        },
      { "Ssid",           "Ssid",           true,  "ssid.h"             },
      { "TypeId",         "TypeId",         true,  "type-id.h"          },
      { "UanModesList",   "UanModesList",   true,  "uan-tx-mode.h"      },
      // { "ValueClassTest", "ValueClassTest", false, "" /* outside ns3 */ },
      { "Vector2D",       "Vector2D",       true,  "vector.h"           },
      { "Vector3D",       "Vector3D",       true,  "vector.h"           },
      { "VhtCapabilities","VhtCapabilities",true,  "vht-capabilities.h" },
      { "VhtOperation",   "VhtOperation",   true,  "vht-operation.h"    },
      { "Waypoint",       "Waypoint",       true,  "waypoint.h"         },
      { "WifiMode",       "WifiMode",       true,  "wifi-mode.h"        },
      
      // All three (Value, Access and Checkers) defined, but custom
      { "Boolean",        "Boolean",        false, "boolean.h"          },
      { "Callback",       "Callback",       true,  "callback.h"         },
      { "Double",         "double",         false, "double.h"           },
      { "Enum",           "int",            false, "enum.h"             },
      { "Integer",        "int64_t",        false, "integer.h"          },
      { "Pointer",        "Pointer",        false, "pointer.h"          },
      { "RandomVariable", "RandomVariable", true,  "random-variable-stream.h"  },
      { "String",         "std::string",    false, "string.h"           },
      { "Time",           "Time",           true,  "nstime.h"           },
      { "Uinteger",       "uint64_t",       false, "uinteger.h"         },
      { "",               "",               false, "last placeholder"   }
    };

  int i = 0;
  while (attributes[i].m_name != "")
    {
      PrintAttributeHelper (os, attributes[i]);
      ++i;
    }

  // Special cases
  PrintAttributeValueSection  (os, "EmptyAttribute", false);
  PrintAttributeValueWithName (os, "EmptyAttribute", "EmptyAttribute",
                                   "attribute.h");

  PrintAttributeValueSection  (os, "ObjectPtrContainer", false);
  PrintAttributeValueWithName (os, "ObjectPtrContainer", "ObjectPtrContainer", "object-ptr-container.h");
  PrintMakeChecker            (os, "ObjectPtrContainer",  "object-ptr-container.h");

  PrintAttributeValueSection  (os, "ObjectVector", false);
  PrintMakeAccessors          (os, "ObjectVector");
  PrintMakeChecker            (os, "ObjectVector", "object-vector.h");

  PrintAttributeValueSection  (os, "ObjectMap", false);
  PrintMakeAccessors          (os, "ObjectMap");
  PrintMakeChecker            (os, "ObjectMap", "object-map.h");
  
}  // PrintAttributeImplementations ()


/***************************************************************
 *        Aggregation and configuration paths
 ***************************************************************/

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

  /**
   * \return the type names we couldn't aggregate.
   */
  std::vector<std::string> GetNoTypeIds (void) const;

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
  /**
   * List of type names without TypeIds, because those modules aren't enabled.
   *
   * This is mutable because GetNoTypeIds sorts and uniquifies this list
   * before returning it.
   */
  mutable std::vector<std::string> m_noTids;
  
};  // class StaticInformation


void 
StaticInformation::RecordAggregationInfo (std::string a, std::string b)
{
  NS_LOG_FUNCTION (this << a << b);
  TypeId aTid;
  bool found = TypeId::LookupByNameFailSafe (a, &aTid);
  if (!found)
    {
      m_noTids.push_back (a);
      return;
    }
  TypeId bTid;
  found = TypeId::LookupByNameFailSafe (b, &bTid);
  if (!found)
    {
      m_noTids.push_back (b);
      return;
    }

  m_aggregates.push_back (std::make_pair (aTid, bTid));
}


void 
StaticInformation::Print (void) const
{
  NS_LOG_FUNCTION (this);
  for (std::vector<std::pair<TypeId,std::string> >::const_iterator i = m_output.begin (); i != m_output.end (); ++i)
    {
      std::pair<TypeId,std::string> item = *i;
      std::cout << item.first.GetName () << " -> " << item.second << std::endl;
    }
}


std::string
StaticInformation::GetCurrentPath (void) const
{
  NS_LOG_FUNCTION (this);
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
  NS_LOG_FUNCTION (this << tid);
  m_output.push_back (std::make_pair (tid, GetCurrentPath ()));
}


bool
StaticInformation::HasAlreadyBeenProcessed (TypeId tid) const
{
  NS_LOG_FUNCTION (this << tid);
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
  NS_LOG_FUNCTION (this << tid);
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

/**
 * Helper to keep only the unique items in a container.
 *
 * The container is modified in place; the elements end up sorted.
 *
 * The container must support \c begin(), \c end() and \c erase(),
 * which, among the STL containers, limits this to
 * \c std::vector, \c std::dequeue and \c std::list.
 *
 * The container elements must support \c operator< (for \c std::sort)
 * and \c operator== (for \c std::unique).
 *
 * \tparam T \deduced The container type.
 * \param t The container.
 */
template <typename T>
void
Uniquefy (T t)
{
  std::sort (t.begin (), t.end ());
  t.erase (std::unique (t.begin (), t.end ()), t.end ());
}

std::vector<std::string>
StaticInformation::GetNoTypeIds (void) const
{
  NS_LOG_FUNCTION (this);
  Uniquefy (m_noTids);
  return m_noTids;
}


void
StaticInformation::Gather (TypeId tid)
{
  NS_LOG_FUNCTION (this << tid);
  DoGather (tid);
  Uniquefy (m_output);
}


void 
StaticInformation::DoGather (TypeId tid)
{
  NS_LOG_FUNCTION (this << tid);
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
          std::string childName = "$" + child.GetName ();
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
          std::string name = "$" + other.GetName ();
          m_currentPath.push_back (name);
          m_alreadyProcessed.push_back (tid);
          DoGather (other);
          m_alreadyProcessed.pop_back ();
          m_currentPath.pop_back ();	  
        }
    }
}  // StaticInformation::DoGather ()


/// Register aggregation relationships that are not automatically
/// detected by this introspection program.  Statements added here
/// result in more configuration paths being added to the doxygen.
/// \return instance of StaticInformation with the registered information
StaticInformation GetTypicalAggregations ()
{
  NS_LOG_FUNCTION_NOARGS ();
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
  info.RecordAggregationInfo ("ns3::Node", "ns3::MobilityModel");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Ipv4L3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Ipv4NixVectorRouting");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Icmpv4L4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::ArpL3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Icmpv4L4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::UdpL4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Ipv6L3Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::Icmpv6L4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::TcpL4Protocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::RipNg");
  info.RecordAggregationInfo ("ns3::Node", "ns3::GlobalRouter");
  info.RecordAggregationInfo ("ns3::Node", "ns3::aodv::RoutingProtocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::dsdv::RoutingProtocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::dsr::DsrRouting");
  info.RecordAggregationInfo ("ns3::Node", "ns3::olsr::RoutingProtocol");
  info.RecordAggregationInfo ("ns3::Node", "ns3::EnergyHarvesterContainer");
  info.RecordAggregationInfo ("ns3::Node", "ns3::EnergySourceContainer");

  // Create a channel object so that channels appear in the namespace
  // paths that will be generated here.
  Ptr<SimpleChannel> simpleChannel;
  simpleChannel = CreateObject<SimpleChannel> ();

  for (uint32_t i = 0; i < Config::GetRootNamespaceObjectN (); ++i)
    {
      Ptr<Object> object = Config::GetRootNamespaceObject (i);
      info.Gather (object->GetInstanceTypeId ());
    }

  return info;

}  // GetTypicalAggregations ()


/// Map from TypeId name to tid
typedef std::map< std::string, int32_t> NameMap;
typedef NameMap::const_iterator         NameMapIterator; ///< NameMap iterator


/**
 * Create a map from the class names to their index in the vector of
 * TypeId's so that the names will end up in alphabetical order.
 *
 * \param info type names withut type ids
 * \returns NameMap
 */
NameMap
GetNameMap (const StaticInformation & info)
{
  NS_LOG_FUNCTION_NOARGS ();
  NameMap nameMap;

  // Registered types
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

  // Type names without TypeIds
  std::vector<std::string> noTids = info.GetNoTypeIds ();
  for (std::vector<std::string>::const_iterator i = noTids.begin ();
       i != noTids.end ();
       ++i)
    {
      nameMap[*i] = -1;
    }
       
  return nameMap;
}  // GetNameMap ()


/**
 * Print config paths
 * \param os the output stream
 * \param info the information
 * \param tid the type ID
 */
void
PrintConfigPaths (std::ostream & os, const StaticInformation & info,
		  const TypeId tid)
{
  NS_LOG_FUNCTION (tid);
  std::vector<std::string> paths = info.Get (tid);

  // Config --------------
  if (paths.empty ())
    {
      os << "Introspection did not find any typical Config paths."
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
	     <<  listLineStop 
	     << breakTextOnly
	     << std::endl;
	}
      os << listStop << std::endl;
    }
}  // PrintConfigPaths ()
      

/***************************************************************
 *        Main
 ***************************************************************/

int main (int argc, char *argv[])
{
  NS_LOG_FUNCTION_NOARGS ();
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

  // mode-line:  helpful when debugging introspected-doxygen.h
  if (!outputText)
    {
      std::cout << "/* -*- Mode:C++; c-file-style:\"gnu\"; "
	           "indent-tabs-mode:nil; -*- */\n"
		<< std::endl;
      std::cout << "#include \"ns3/log.h\""
                << std::endl;
    }

  // Doxygen file header
  std::cout << std::endl;
  std::cout << commentStart
            << file << "\n"
            << sectionStart << "utils\n"
            << "Doxygen docs generated from the TypeId database.\n"
            << note << "This file is automatically generated by "
            << codeWord << "print-introspected-doxygen.cc. Do not edit this file! "
            << "Edit that file instead.\n"
            << commentStop
            << std::endl;
  
  // Get typical aggregation relationships.
  StaticInformation info = GetTypicalAggregations ();
  
  NameMap nameMap = GetNameMap (info);

  // Iterate over the map, which will print the class names in
  // alphabetical order.
  for (NameMapIterator nameMapIterator = nameMap.begin ();
       nameMapIterator != nameMap.end ();
       nameMapIterator++)
    {
      // Get the class's index out of the map;
      std::string name = nameMapIterator->first;
      int32_t i = nameMapIterator->second;
      TypeId tid;

      if (i >= 0)
        {
          tid = TypeId::GetRegistered (i);
          if (tid.MustHideFromDocumentation ())
            {
              continue;
            }
          name = tid.GetName ();
        }
      
      std::cout << commentStart << std::endl;
      
      std::cout << classStart << name << std::endl;
      std::cout << std::endl;

      if (i >= 0)
        {
          PrintConfigPaths (std::cout, info, tid);
          PrintAttributes (std::cout, tid);
          PrintTraceSources (std::cout, tid);
          PrintSize (std::cout, tid);
        }
      else
        {
          std::cout << "Introspection could not find Config paths for " << name
                    << " in this build because the parent module"
                    << " was not included in the waf configuration."
                    << breakBoth
                    << std::endl;
        }
      
      std::cout << commentStop << std::endl;
    }  // class documentation


  PrintAllAttributes (std::cout);
  PrintAllGlobals (std::cout);
  PrintAllLogComponents (std::cout);
  PrintAllTraceSources (std::cout);
  PrintAttributeImplementations (std::cout);

  return 0;
}
