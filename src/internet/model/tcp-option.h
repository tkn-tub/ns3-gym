/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#ifndef TCP_OPTION_H
#define TCP_OPTION_H

#include <stdint.h>
#include "ns3/object.h"
#include "ns3/buffer.h"
#include "ns3/object-factory.h"

namespace ns3 {

/**
 * Base class for all kinds of TCP options
 */
class TcpOption : public Object
{
public:
  TcpOption ();
  virtual ~TcpOption ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  enum Kind
  {
    END = 0,
    NOP = 1,
    MSS = 2,
    WINSCALE = 3,
    SACK_PERM = 4,
    SACK = 5,
    TS = 8
  };

  virtual void Print (std::ostream &os) const = 0;
  virtual void Serialize (Buffer::Iterator start) const = 0;
  virtual uint32_t Deserialize (Buffer::Iterator start) = 0;

  virtual uint8_t GetKind (void) const = 0; // Get the `kind' (as in RFC793) of this option
  virtual uint32_t GetSerializedSize (void) const = 0; // Get the total length of this option, >= 1

  static Ptr<TcpOption> CreateOption (uint8_t kind); // Factory method for all options
  static bool IsKindKnown (uint8_t kind);
};

} // namespace ns3

#endif /* TCP_OPTION */
