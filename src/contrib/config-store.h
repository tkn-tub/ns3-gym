#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include "ns3/object-base.h"

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
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  ConfigStore ();

  /**
   * Depending on which attribute was set:
   *  - Store simulation configuration in file and exit
   *  - Load simulation configuration from file and proceed.
   */
  void Configure (void);

private:
  void LoadFrom (std::string filename);
  void StoreTo (std::string filename);

  std::string m_loadFilename;
  std::string m_storeFilename;
};

}  // namespace ns3

#endif /* CONFIG_STORE_H */
