#ifndef MTAG_H
#define MTAG_H

#include "ns3/object-base.h"
#include "tag-buffer.h"
#include <stdint.h>

namespace ns3 {

/**
 * \brief tag a set of bytes in a packet
 *
 * New kinds of tags can be created by subclassing this base class.
 */
class Tag : public ObjectBase
{
public:
  static TypeId GetTypeId (void);

  /**
   * \returns the number of bytes required to serialize the data of the tag.
   *
   * This method is typically invoked by Packet::AddTag just prior to calling
   * Tag::Serialize.
   */
  virtual uint32_t GetSerializedSize (void) const = 0;
  /**
   * \param i the buffer to write data into.
   *
   * Write the content of the tag in the provided tag buffer.
   */
  virtual void Serialize (TagBuffer i) const = 0;
  /**
   * \param i the buffer to read data from.
   *
   * Read the content of the tag from the provided tag buffer.
   */
  virtual void Deserialize (TagBuffer i) = 0;
};

} // namespace ns3

#endif /* MTAG_H */
