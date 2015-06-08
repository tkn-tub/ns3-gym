/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef RAW_TEXT_CONFIG_H
#define RAW_TEXT_CONFIG_H

#include <string>
#include <fstream>
#include "file-config.h"

namespace ns3 {

/**
 * \ingroup configstore
 *
 */
class RawTextConfigSave : public FileConfig
{
public:
  RawTextConfigSave ();
  virtual ~RawTextConfigSave ();
  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
private:
  std::ofstream *m_os;
};

/**
 * \ingroup configstore
 *
 */
class RawTextConfigLoad : public FileConfig
{
public:
  RawTextConfigLoad ();
  virtual ~RawTextConfigLoad ();
  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
private:
  std::string Strip (std::string value);
  std::ifstream *m_is;
};

} // namespace ns3

#endif /* RAW_TEXT_CONFIG_H */
