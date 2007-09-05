/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef TAG_H
#define TAG_H

#include <stdint.h>
#include <string>

/**
 * \relates ns3::Tag
 * \brief this macro should be instantiated exactly once for each
 *        new type of Tag
 *
 * This macro will ensure that your new Tag type is registered
 * within the tag registry. In most cases, this macro
 * is not really needed but, for safety, please, use it all the
 * time.
 *
 * Note: This macro is _absolutely_ needed if you try to run a
 * distributed simulation.
 */
#define NS_TAG_ENSURE_REGISTERED(x)	       \
static class thisisaveryverylongclassname ##x  \
{					       \
 public:				       \
  thisisaveryverylongclassname ##x ()          \
    { uint32_t uid; uid = x::GetUid ();}       \
} g_thisisanotherveryveryverylongname ## x;

namespace ns3 {

/**
 * \brief a tag can be stored in a packet.
 *
 * A tag is a blob of 16 bytes of data which can be stored in
 * a packet: a packet can contain an arbitrary number of tags
 * and these tags are considered as "on-the-side" per-packet
 * data structures which are not taken into account when calculating
 * the size of the payload of a packet. They exist solely as 
 * simulation-specific objects.
 *
 * Tags are typically used to:
 *   - implement per-packet cross-layer communication
 *   - implement packet coloring: you could store a "color" tag
 *     in a packet to mark various types of packet for
 *     simulation analysis
 *
 * To create a new type of tag, you must create a subclass
 * of the Tag base class which defines:
 *  - a public default constructor: needed for implementation
 *    purposes of the Packet code.
 *  - a public copy constructor: needed to copy a tag into
 *    a packet tag buffer when the user invokes Packet::AddTag
 *  - a public destructor: needed to destroy the copy of a tag
 *    stored in a packet buffer when the user invokes Packet::RemoveTag
 *    or when the packet is destroyed and the last reference to 
 *    a tag instance disapears.
 *  - a public static method named GetUid: needed to uniquely
 *    the type of each tag instance.
 *  - a public method named Print: needed to print the content
 *    of a tag when the user calls Packet::PrintTags
 *  - a public method named GetSerializedSize: needed to serialize
 *    the content of a tag to a byte buffer when a packet must
 *    be sent from one computing node to another in a parallel 
 *    simulation. If this method returns 0, it means that the
 *    tag does not need to be transfered from computing node to 
 *     computing node
 *  - a public method named Serialize: perform the serialization
 *    to a byte buffer upon transfer to a new computing node in a 
 *    parallel simulation.
 *  - a public method named Deserialize: invert the serialization
 *    from a byte buffer after being transfered to a new computing
 *    node in a parallel simulation.
 *
 * A detailed example of what these methods should look like
 * and how they should be implemented is described in samples/main-packet-tag.cc
 */
class Tag
{
protected:
  /**
   * \param name the unique name of the new type of tag
   * \returns a newly-allocated uid
   *
   * This method should be used by subclasses to implement
   * their static public GetUid method.
   */
  template <typename T>
  static uint32_t AllocateUid (std::string name);
};

} // namespace ns3

// implementation below.
#include "tag-registry.h"

namespace ns3 {

template <typename T>
uint32_t
Tag::AllocateUid (std::string name)
{
  return TagRegistry::Register<T> (name);
}

} // namespace ns3

#endif /* TAG_H */
