#include "config-store.h"
#include "raw-text-config.h"
#include "ns3/abort.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/attribute-construction-list.h"
#include "ns3/enum.h"
#include "ns3/config-store-config.h"
#ifdef HAVE_LIBXML2
#include "xml-config.h"
#endif

#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>


NS_LOG_COMPONENT_DEFINE ("ConfigStore");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (ConfigStore);

TypeId 
ConfigStore::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConfigStore")
    .SetParent<ObjectBase> ()
    .AddAttribute ("Mode", 
                   "Configuration mode",
                   EnumValue (ConfigStore::NONE),
                   MakeEnumAccessor (&ConfigStore::SetMode),
                   MakeEnumChecker (ConfigStore::NONE, "None",
                                    ConfigStore::LOAD, "Load",
                                    ConfigStore::SAVE, "Save"))
    .AddAttribute ("Filename", 
                   "The file where the configuration should be saved to or loaded from.",
                   StringValue (""),
                   MakeStringAccessor (&ConfigStore::SetFilename),
                   MakeStringChecker ())
    .AddAttribute ("FileFormat",
                   "Type of file format",
                   EnumValue (ConfigStore::RAW_TEXT),
                   MakeEnumAccessor (&ConfigStore::SetFileFormat),
                   MakeEnumChecker (ConfigStore::RAW_TEXT, "RawText",
                                    ConfigStore::XML, "Xml"))
  ;
  return tid;
}
TypeId 
ConfigStore::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}


ConfigStore::ConfigStore ()
{
  ObjectBase::ConstructSelf (AttributeConstructionList ());

#ifdef HAVE_LIBXML2
  if (m_fileFormat == ConfigStore::XML)
    {
      if (m_mode == ConfigStore::SAVE)
        {
          m_file = new XmlConfigSave ();
        }
      else if (m_mode == ConfigStore::LOAD)
        {
          m_file = new XmlConfigLoad ();
        }
      else 
        {
          m_file = new NoneFileConfig ();
        }
    }
#else
  if (m_fileFormat == ConfigStore::XML)
    {
      if (m_mode == ConfigStore::SAVE || m_mode == ConfigStore::LOAD)
        {
	  NS_ABORT_MSG ("ConfigStore tried to read or write an XML file but XML is not supported.");
        }
      else
        {
          m_file = new NoneFileConfig ();
        }
    }
#endif /* HAVE_LIBXML2 */

  if (m_fileFormat == ConfigStore::RAW_TEXT)
    {
      if (m_mode == ConfigStore::SAVE)
        {
          m_file = new RawTextConfigSave ();
        }
      else if (m_mode == ConfigStore::LOAD)
        {
          m_file = new RawTextConfigLoad ();
        }
      else
        {
          m_file = new NoneFileConfig ();
        }
    }
  m_file->SetFilename (m_filename);
}

ConfigStore::~ConfigStore ()
{
  delete m_file;
  m_file = 0;
}

void 
ConfigStore::SetMode (enum Mode mode)
{
  m_mode = mode;
}
void 
ConfigStore::SetFileFormat (enum FileFormat format)
{
  m_fileFormat = format;
}
void 
ConfigStore::SetFilename (std::string filename)
{
  m_filename = filename;
}

void 
ConfigStore::ConfigureAttributes (void)
{
  m_file->Attributes ();
}

void 
ConfigStore::ConfigureDefaults (void)
{
  m_file->Default ();
  m_file->Global ();
}

} // namespace ns3
