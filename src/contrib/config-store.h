#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include "ns3/object-base.h"
#include "file-config.h"

namespace ns3 {

/**
 * \brief Store and load simulation attribute configuration
 *
 * While it is possible to generate a sample config file and lightly
 * edit it to change a couple of values, there are cases where this
 * process will not work because the same value on the same object
 * can appear multiple times in the same automatically-generated 
 * configuration file under different configuration paths.
 *
 * As such, the best way to use this class is to use it to generate
 * an initial configuration file, extract from that configuration
 * file only the strictly necessary elements, and move these minimal
 * elements to a new configuration file which can then safely
 * be edited. Another option is to use the ns3::GtkConfigStore class
 * which will allow you to edit the parameters and will generate 
 * configuration files where all the instances of the same parameter
 * are changed.
 */
class ConfigStore : public ObjectBase
{
public:
  enum Mode {
    LOAD,
    SAVE,
    NONE
  };
  enum FileFormat {
    XML,
    RAW_TEXT
  };
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  ConfigStore ();
  ~ConfigStore ();

  void SetMode (enum Mode mode);
  void SetFileFormat (enum FileFormat format);
  void SetFilename (std::string filename);

  void ConfigureEarly (void);
  void ConfigureLate (void);

private:
  enum Mode m_mode;
  enum FileFormat m_fileFormat;
  std::string m_filename;
  FileConfig *m_file;
};

}  // namespace ns3

#endif /* CONFIG_STORE_H */
