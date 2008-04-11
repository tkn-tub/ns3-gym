#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include "ns3/object-base.h"
#include "ns3/object.h"
#include <vector>

namespace ns3 {

class ConfigStore : public ObjectBase
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  ConfigStore ();

  void Configure (void);

private:
  void LoadFrom (std::string filename);
  void StoreTo (std::string filename);
  void Store (std::ostream &os, Ptr<const Object> object);
  bool IsExamined (Ptr<const Object> object);
  std::string GetCurrentPath (std::string attr) const;

  std::string m_loadFilename;
  std::string m_storeFilename;
  std::vector<Ptr<const Object> > m_examined;
  std::vector<std::string> m_currentPath;
};

}  // namespace ns3

#endif /* CONFIG_STORE_H */
