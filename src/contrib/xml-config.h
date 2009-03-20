#ifndef XML_CONFIG_STORE_H
#define XML_CONFIG_STORE_H

#include <string>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include "file-config.h"

namespace ns3 {

class XmlConfigSave : public FileConfig
{
public:
  XmlConfigSave ();
  virtual ~XmlConfigSave ();

  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
private:
  xmlTextWriterPtr m_writer;
};

class XmlConfigLoad : public FileConfig
{
public:
  XmlConfigLoad ();
  virtual ~XmlConfigLoad ();

  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
private:
  std::string m_filename;
};

} // namespace ns3

#endif /* XML_CONFIG_STORE_H */
