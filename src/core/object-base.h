#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "type-id.h"

/**
 * This macro should be invoked once for every class which
 * defines a new GetTypeId method.
 */
#define NS_OBJECT_ENSURE_REGISTERED(type)       \
  static struct X##type##RegistrationClass      \
  {                                             \
    X##type##RegistrationClass () {             \
      ns3::TypeId tid = type::GetTypeId ();     \
      tid.GetParent ();                         \
    }                                           \
} x_##type##RegistrationVariable

namespace ns3 {

/**
 * This base class is really used only to make sure that 
 * every subclass has RTTI information and that they all
 * share a single base class to allow us to make type 
 * checks across all these types.
 */
class ObjectBase
{
public:
  virtual ~ObjectBase ();
};

} // namespace ns3

#endif /* OBJECT_BASE_H */
