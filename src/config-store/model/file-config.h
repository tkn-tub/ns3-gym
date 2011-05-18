#ifndef FILE_CONFIG_H
#define FILE_CONFIG_H

#include <string>

namespace ns3 {

/**
 * \ingroup configstore
 *
 */
class FileConfig
{
public:
  virtual ~FileConfig ();
  virtual void SetFilename (std::string filename) = 0;
  virtual void Default (void) = 0;
  virtual void Global (void) = 0;
  virtual void Attributes (void) = 0;
};

/**
 * \ingroup configstore
 *
 */
class NoneFileConfig : public FileConfig
{
public:
  NoneFileConfig ();
  virtual ~NoneFileConfig ();
  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
};

} // namespace ns3

#endif /* FILE_CONFIG_H */
