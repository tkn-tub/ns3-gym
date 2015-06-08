/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void); 

  /**
   * \brief Deserialize the object from a buffer iterator
   * \param start the buffer iterator
   * \returns the number of deserialized bytes
   */
  virtual uint32_t Deserialize (Buffer::Iterator start) = 0;

  /**
   * \brief Print the object contents
   * \param os the output stream
   */
  virtual void Print (std::ostream &os) const = 0;
};

} // namespace ns3

#endif /* CHUNK_H */
