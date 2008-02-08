#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "value.h"
#include "object.h"

namespace ns3 {

class ObjectFactory
{
public:
  ObjectFactory ();

  void SetTypeId (TypeId tid);
  void SetTypeId (std::string tid);
  void Set (std::string name, PValue value);
  void Set (std::string name, std::string value);

  TypeId GetTypeId (void) const;

  Ptr<Object> Create (void) const;

private:
  TypeId m_tid;
  Parameters m_parameters;
};

} // namespace ns3

#endif /* OBJECT_FACTORY_H */
