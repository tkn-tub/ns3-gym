#ifndef CHUNK_H
#define CHUNK_H

#include "ns3/object-base.h"
#include "buffer.h"

namespace ns3 {

/**
 * \ingroup packet
 *
 * \brief abstract base class for ns3::Header and ns3::Trailer
 */
class Chunk : public ObjectBase
{
 public:
  static TypeId GetTypeId (void); 

  virtual uint32_t Deserialize (Buffer::Iterator start) = 0;
  virtual void Print (std::ostream &os) const = 0;
};

} // namespace ns3

#endif /* CHUNK_H */
