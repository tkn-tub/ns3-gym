/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <ostream>
#include "buffer.h"

namespace ns3 {

class Chunk {
public:
  Chunk ();
  virtual ~Chunk ();

  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  virtual std::string DoGetName (void) const = 0;
  virtual void PrintTo (std::ostream &os) const = 0;
  virtual uint32_t GetSerializedSize (void) const = 0;
  virtual void SerializeTo (Buffer::Iterator i) const = 0;
  virtual uint32_t DeserializeFrom (Buffer::Iterator i) = 0;
};

std::ostream& operator<< (std::ostream& os, Chunk const& chunk);

}; // namespace ns3

#endif /* CHUNK_H */
