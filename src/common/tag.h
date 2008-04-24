#ifndef MTAG_H
#define MTAG_H

#include "ns3/object-base.h"
#include "tag-buffer.h"
#include <stdint.h>

namespace ns3 {

class Tag : public ObjectBase
{
public:
  static TypeId GetTypeId (void);

  virtual uint32_t GetSerializedSize (void) const = 0;
  virtual void Serialize (TagBuffer i) const = 0;
  virtual void Deserialize (TagBuffer i) = 0;
};

} // namespace ns3

#endif /* MTAG_H */
