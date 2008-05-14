#include "config-store.h"
#include "attribute-iterator.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/attribute-list.h"
#include "ns3/config.h"
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

NS_LOG_COMPONENT_DEFINE ("ConfigStore");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ConfigStore);

TypeId 
ConfigStore::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConfigStore")
    .SetParent<ObjectBase> ()
    .AddAttribute ("LoadFilename", 
		   "The file where the configuration should be loaded from.",
		   StringValue (""),
		   MakeStringAccessor (&ConfigStore::m_loadFilename),
		   MakeStringChecker ())
    .AddAttribute ("StoreFilename", 
		   "The file where the configuration should be stored to.",
		   StringValue (""),
		   MakeStringAccessor (&ConfigStore::m_storeFilename),
		   MakeStringChecker ())
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
  ObjectBase::ConstructSelf (AttributeList ());
}

void 
ConfigStore::LoadFrom (std::string filename)
{
  std::ifstream is;
  is.open (filename.c_str (), std::ios::in);
  std::string path, value;
  while (is.good())
    {
      is >> path >> value;
      NS_LOG_DEBUG (path << "=" << value);
      Config::Set (path, StringValue (value));
    }
}
void 
ConfigStore::StoreTo (std::string filename)
{

  std::ofstream os;
  os.open (filename.c_str (), std::ios::out);
  TextFileAttributeIterator iter = TextFileAttributeIterator (os);
  iter.Save ();
  os.close ();
  exit (0);
}

void 
ConfigStore::Configure (void)
{
  if (m_loadFilename != "")
    {
      LoadFrom (m_loadFilename);
    }
  if (m_storeFilename != "")
    {
      StoreTo (m_storeFilename);
    }
}

} // namespace ns3
