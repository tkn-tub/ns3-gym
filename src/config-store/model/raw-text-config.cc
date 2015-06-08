/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "raw-text-config.h"
#include "attribute-iterator.h"
#include "attribute-default-iterator.h"
#include "ns3/global-value.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/config.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RawTextConfig");

RawTextConfigSave::RawTextConfigSave ()
  : m_os (0)
{
}
RawTextConfigSave::~RawTextConfigSave ()
{
  if (m_os != 0)
    {
      m_os->close ();
    }
  delete m_os;
  m_os = 0;
}
void 
RawTextConfigSave::SetFilename (std::string filename)
{
  m_os = new std::ofstream ();
  m_os->open (filename.c_str (), std::ios::out);
}
void 
RawTextConfigSave::Default (void)
{
  class RawTextDefaultIterator : public AttributeDefaultIterator
  {
public:
    RawTextDefaultIterator (std::ostream *os) {
      m_os = os;
    }
private:
    virtual void StartVisitTypeId (std::string name) {
      m_typeId = name;
    }
    virtual void DoVisitAttribute (std::string name, std::string defaultValue) {
      *m_os << "default " << m_typeId << "::" << name << " \"" << defaultValue << "\"" << std::endl;
    }
    std::string m_typeId;
    std::ostream *m_os;
  };

  RawTextDefaultIterator iterator = RawTextDefaultIterator (m_os);
  iterator.Iterate ();
}
void 
RawTextConfigSave::Global (void)
{
  for (GlobalValue::Iterator i = GlobalValue::Begin (); i != GlobalValue::End (); ++i)
    {
      StringValue value;
      (*i)->GetValue (value);
      *m_os << "global " << (*i)->GetName () << " \"" << value.Get () << "\"" << std::endl;
    }
}
void 
RawTextConfigSave::Attributes (void)
{
  class RawTextAttributeIterator : public AttributeIterator
  {
public:
    RawTextAttributeIterator (std::ostream *os)
      : m_os (os) {}
private:
    virtual void DoVisitAttribute (Ptr<Object> object, std::string name) {
      StringValue str;
      object->GetAttribute (name, str);
      *m_os << "value " << GetCurrentPath () << " \"" << str.Get () << "\"" << std::endl;
    }
    std::ostream *m_os;
  };

  RawTextAttributeIterator iter = RawTextAttributeIterator (m_os);
  iter.Iterate ();
}

RawTextConfigLoad::RawTextConfigLoad ()
  : m_is (0)
{
}
RawTextConfigLoad::~RawTextConfigLoad ()
{
  if (m_is != 0)
    {
      m_is->close ();
      delete m_is;
      m_is = 0;
    }
}
void 
RawTextConfigLoad::SetFilename (std::string filename)
{
  m_is = new std::ifstream ();
  m_is->open (filename.c_str (), std::ios::in);
}
std::string
RawTextConfigLoad::Strip (std::string value)
{
  std::string::size_type start = value.find ("\"");
  std::string::size_type end = value.find ("\"", 1);
  NS_ASSERT (start == 0);
  NS_ASSERT (end == value.size () - 1);
  return value.substr (start+1, end-start-1);
}

void 
RawTextConfigLoad::Default (void)
{
  m_is->clear ();
  m_is->seekg (0);
  std::string type, name, value;
  *m_is >> type >> name >> value;
  while (m_is->good ())
    {
      NS_LOG_DEBUG ("type=" << type << ", name=" << name << ", value=" << value);
      value = Strip (value);
      if (type == "default")
        {
          Config::SetDefault (name, StringValue (value));
        }
      *m_is >> type >> name >> value;
    }
}
void 
RawTextConfigLoad::Global (void)
{
  m_is->clear ();
  m_is->seekg (0);
  std::string type, name, value;
  *m_is >> type >> name >> value;
  while (m_is->good ())
    {
      NS_LOG_DEBUG ("type=" << type << ", name=" << name << ", value=" << value);
      value = Strip (value);
      if (type == "global")
        {
          Config::SetGlobal (name, StringValue (value));
        }
      *m_is >> type >> name >> value;
    }
}
void 
RawTextConfigLoad::Attributes (void)
{
  m_is->seekg (0);
  std::string type, path, value;
  *m_is >> type >> path >> value;
  while (m_is->good ())
    {
      NS_LOG_DEBUG ("type=" << type << ", path=" << path << ", value=" << value);
      value = Strip (value);
      if (type == "value")
        {
          Config::Set (path, StringValue (value));
        }
      *m_is >> type >> path >> value;
    }
}


} // namespace ns3
