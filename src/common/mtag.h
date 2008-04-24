#ifndef MTAG_H
#define MTAG_H

#include "ns3/object-base.h"
#include "mtag-buffer.h"
#include <stdint.h>

namespace ns3 {

class Mtag : public ObjectBase
{
public:
  static TypeId GetTypeId (void);

  virtual uint32_t GetSerializedSize (void) const = 0;
  virtual void Serialize (MtagBuffer i) const = 0;
  virtual void Deserialize (MtagBuffer i) = 0;
};

} // namespace ns3

#endif /* MTAG_H */
