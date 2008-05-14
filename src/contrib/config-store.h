#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include "ns3/object-base.h"

namespace ns3 {

/**
 * \brief Store and load simulation attribute configuration
 *
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
