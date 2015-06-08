/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "xml-config.h"
#include "attribute-default-iterator.h"
#include "attribute-iterator.h"
#include "ns3/fatal-error.h"
#include "ns3/log.h"
#include "ns3/global-value.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("XmlConfig");

XmlConfigSave::XmlConfigSave ()
  : m_writer (0)
{
  NS_LOG_FUNCTION (this);
}
void
XmlConfigSave::SetFilename (std::string filename)
{
  NS_LOG_FUNCTION (filename);
  if (filename == "")
    {
      return;
    }
  int rc;

  /* Create a new XmlWriter for uri, with no compression. */
  m_writer = xmlNewTextWriterFilename (filename.c_str (), 0);
  if (m_writer == NULL) 
    {
      NS_FATAL_ERROR ("Error creating the xml writer");
    }
  rc = xmlTextWriterSetIndent (m_writer, 1);
  if (rc < 0)
    {
      NS_FATAL_ERROR ("Error at xmlTextWriterSetIndent");
    }
  /* Start the document with the xml default for the version,
   * encoding utf-8 and the default for the standalone
   * declaration. */
  rc = xmlTextWriterStartDocument (m_writer, NULL, "utf-8", NULL);
  if (rc < 0) 
    {
      NS_FATAL_ERROR ("Error at xmlTextWriterStartDocument");
    }

  /* Start an element named "ns3". Since thist is the first
   * element, this will be the root element of the document. */
  rc = xmlTextWriterStartElement (m_writer, BAD_CAST "ns3");
  if (rc < 0) 
    {
      NS_FATAL_ERROR ("Error at xmlTextWriterStartElement\n");
    }
}
XmlConfigSave::~XmlConfigSave ()
{
  NS_LOG_FUNCTION (this);
  if (m_writer == 0)
    {
      return;
    }
  int rc;
  /* Here we could close the remaining elements using the
   * function xmlTextWriterEndElement, but since we do not want to
   * write any other elements, we simply call xmlTextWriterEndDocument,
   * which will do all the work. */
  rc = xmlTextWriterEndDocument (m_writer);
  if (rc < 0) 
    {
      NS_FATAL_ERROR ("Error at xmlTextWriterEndDocument\n");
    }

  xmlFreeTextWriter (m_writer);
  m_writer = 0;
}
void 
XmlConfigSave::Default (void)
{
  class XmlDefaultIterator : public AttributeDefaultIterator
  {
public:
    XmlDefaultIterator (xmlTextWriterPtr writer) {
      m_writer = writer;
    }
private:
    virtual void StartVisitTypeId (std::string name) {
      m_typeid = name;
    }
    virtual void DoVisitAttribute (std::string name, std::string defaultValue) {
      int rc;
      rc = xmlTextWriterStartElement (m_writer, BAD_CAST "default");
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterStartElement");
        }
      std::string fullname = m_typeid + "::" + name;
      rc = xmlTextWriterWriteAttribute (m_writer, BAD_CAST "name",
                                        BAD_CAST fullname.c_str ());
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterWriteAttribute");
        }
      rc = xmlTextWriterWriteAttribute (m_writer, BAD_CAST "value",
                                        BAD_CAST defaultValue.c_str ());
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterWriteAttribute");
        }
      rc = xmlTextWriterEndElement (m_writer);
      if (rc < 0) 
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterEndElement");
        }
    }
    xmlTextWriterPtr m_writer;
    std::string m_typeid;
  };
  XmlDefaultIterator iterator = XmlDefaultIterator (m_writer);
  iterator.Iterate ();
}

void
XmlConfigSave::Attributes (void)
{
  class XmlTextAttributeIterator : public AttributeIterator
  {
public:
    XmlTextAttributeIterator (xmlTextWriterPtr writer)
      : m_writer (writer) {}
private:
    virtual void DoVisitAttribute (Ptr<Object> object, std::string name) {
      StringValue str;
      object->GetAttribute (name, str);
      int rc;
      rc = xmlTextWriterStartElement (m_writer, BAD_CAST "value");
      if (rc < 0) 
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterStartElement");
        }
      rc = xmlTextWriterWriteAttribute (m_writer, BAD_CAST "path",
                                        BAD_CAST GetCurrentPath ().c_str ());
      if (rc < 0) 
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterWriteAttribute");
        }
      rc = xmlTextWriterWriteAttribute (m_writer, BAD_CAST "value",
                                        BAD_CAST str.Get ().c_str ());
      if (rc < 0) 
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterWriteAttribute");
        }
      rc = xmlTextWriterEndElement (m_writer);
      if (rc < 0) 
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterEndElement");
        }
    }
    xmlTextWriterPtr m_writer;
  };

  XmlTextAttributeIterator iter = XmlTextAttributeIterator (m_writer);
  iter.Iterate ();
}

void
XmlConfigSave::Global (void)
{
  int rc;
  for (GlobalValue::Iterator i = GlobalValue::Begin (); i != GlobalValue::End (); ++i)
    {
      StringValue value;
      (*i)->GetValue (value);

      rc = xmlTextWriterStartElement (m_writer, BAD_CAST "global");
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterStartElement");
        }
      rc = xmlTextWriterWriteAttribute (m_writer, BAD_CAST "name",
                                        BAD_CAST (*i)->GetName ().c_str ());
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterWriteAttribute");
        }
      rc = xmlTextWriterWriteAttribute (m_writer, BAD_CAST "value",
                                        BAD_CAST value.Get ().c_str ());
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterWriteAttribute");
        }
      rc = xmlTextWriterEndElement (m_writer);
      if (rc < 0)
        {
          NS_FATAL_ERROR ("Error at xmlTextWriterEndElement");
        }
    }
}

XmlConfigLoad::XmlConfigLoad ()
{
  NS_LOG_FUNCTION (this);
}
XmlConfigLoad::~XmlConfigLoad ()
{
  NS_LOG_FUNCTION (this);
}

void
XmlConfigLoad::SetFilename (std::string filename)
{
  NS_LOG_FUNCTION (filename);
  m_filename = filename;
}
void 
XmlConfigLoad::Default (void)
{
  xmlTextReaderPtr reader = xmlNewTextReaderFilename (m_filename.c_str ());
  if (reader == NULL)
    {
      NS_FATAL_ERROR ("Error at xmlReaderForFile");
    }
  int rc;
  rc = xmlTextReaderRead (reader);
  while (rc > 0)
    {
      const xmlChar *type = xmlTextReaderConstName (reader);
      if (type == 0)
        {
          NS_FATAL_ERROR ("Invalid value");
        }
      if (std::string ((char*)type) == "default")
        {
          xmlChar *name = xmlTextReaderGetAttribute (reader, BAD_CAST "name");
          if (name == 0)
            {
              NS_FATAL_ERROR ("Error getting attribute 'name'");
            }
          xmlChar *value = xmlTextReaderGetAttribute (reader, BAD_CAST "value");
          if (value == 0)
            {
              NS_FATAL_ERROR ("Error getting attribute 'value'");
            }
          NS_LOG_DEBUG ("default="<<(char*)name<<", value=" <<value);
          Config::SetDefault ((char*)name, StringValue ((char*)value));
          xmlFree (name);
          xmlFree (value);
        }
      rc = xmlTextReaderRead (reader);
    }
  xmlFreeTextReader (reader);
}
void 
XmlConfigLoad::Global (void)
{
  xmlTextReaderPtr reader = xmlNewTextReaderFilename (m_filename.c_str ());
  if (reader == NULL)
    {
      NS_FATAL_ERROR ("Error at xmlReaderForFile");
    }
  int rc;
  rc = xmlTextReaderRead (reader);
  while (rc > 0)
    {
      const xmlChar *type = xmlTextReaderConstName (reader);
      if (type == 0)
        {
          NS_FATAL_ERROR ("Invalid value");
        }
      if (std::string ((char*)type) == "global")
        {
          xmlChar *name = xmlTextReaderGetAttribute (reader, BAD_CAST "name");
          if (name == 0)
            {
              NS_FATAL_ERROR ("Error getting attribute 'name'");
            }
          xmlChar *value = xmlTextReaderGetAttribute (reader, BAD_CAST "value");
          if (value == 0)
            {
              NS_FATAL_ERROR ("Error getting attribute 'value'");
            }
          NS_LOG_DEBUG ("global="<<(char*)name<<", value=" <<value);
          Config::SetGlobal ((char*)name, StringValue ((char*)value));
          xmlFree (name);
          xmlFree (value);
        }
      rc = xmlTextReaderRead (reader);
    }
  xmlFreeTextReader (reader);
}
void 
XmlConfigLoad::Attributes (void)
{
  xmlTextReaderPtr reader = xmlNewTextReaderFilename (m_filename.c_str ());
  if (reader == NULL)
    {
      NS_FATAL_ERROR ("Error at xmlReaderForFile");
    }
  int rc;
  rc = xmlTextReaderRead (reader);
  while (rc > 0)
    {
      const xmlChar *type = xmlTextReaderConstName (reader);
      if (type == 0)
        {
          NS_FATAL_ERROR ("Invalid value");
        }
      if (std::string ((char*)type) == "value")
        {
          xmlChar *path = xmlTextReaderGetAttribute (reader, BAD_CAST "path");
          if (path == 0)
            {
              NS_FATAL_ERROR ("Error getting attribute 'path'");
            }
          xmlChar *value = xmlTextReaderGetAttribute (reader, BAD_CAST "value");
          if (value == 0)
            {
              NS_FATAL_ERROR ("Error getting attribute 'value'");
            }
          NS_LOG_DEBUG ("path="<<(char*)path << ", value=" << (char*)value);
          Config::Set ((char*)path, StringValue ((char*)value));
          xmlFree (path);
          xmlFree (value);
        }
      rc = xmlTextReaderRead (reader);
    }
  xmlFreeTextReader (reader);
}



} // namespace ns3
