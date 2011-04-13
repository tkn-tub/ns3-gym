/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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

#ifndef ICMPV4_H
#define ICMPV4_H

#include "ns3/header.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-header.h"
#include <stdint.h>

namespace ns3 {

class Packet;

class Icmpv4Header : public Header
{
public:
  enum {
    ECHO_REPLY = 0,
    DEST_UNREACH = 3,
    ECHO = 8,
    TIME_EXCEEDED = 11
  };

  void EnableChecksum (void);
  void SetType (uint8_t type);
  void SetCode (uint8_t code);

  uint8_t GetType (void) const;
  uint8_t GetCode (void) const;

  static TypeId GetTypeId (void);
  Icmpv4Header ();
  virtual ~Icmpv4Header ();
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  uint8_t m_type;
  uint8_t m_code;
  bool m_calcChecksum;
};

class Icmpv4Echo : public Header
{
public:
  void SetIdentifier (uint16_t id);
  void SetSequenceNumber (uint16_t seq);
  void SetData (Ptr<const Packet> data);
  uint16_t GetIdentifier (void) const;
  uint16_t GetSequenceNumber (void) const;
  uint32_t GetDataSize (void) const;
  uint32_t GetData (uint8_t payload[]) const;


  static TypeId GetTypeId (void);
  Icmpv4Echo ();
  virtual ~Icmpv4Echo ();
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
private:
  uint16_t m_identifier;
  uint16_t m_sequence;
  uint8_t *m_data;
  uint32_t m_dataSize;
};

class Icmpv4DestinationUnreachable : public Header
{
public:
  enum {
    NET_UNREACHABLE = 0,
    HOST_UNREACHABLE = 1,
    PROTOCOL_UNREACHABLE = 2,
    PORT_UNREACHABLE = 3,
    FRAG_NEEDED = 4,
    SOURCE_ROUTE_FAILED = 5
  };
  static TypeId GetTypeId (void);
  Icmpv4DestinationUnreachable ();
  virtual ~Icmpv4DestinationUnreachable ();

  void SetNextHopMtu (uint16_t mtu);
  uint16_t GetNextHopMtu (void) const;

  void SetData (Ptr<const Packet> data);
  void SetHeader (Ipv4Header header);

  void GetData (uint8_t payload[8]) const;
  Ipv4Header GetHeader (void) const;

private:
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
private:
  uint16_t m_nextHopMtu;
  Ipv4Header m_header;
  uint8_t m_data[8];
};


class Icmpv4TimeExceeded : public Header
{
public:
  enum {
    TIME_TO_LIVE = 0,
    FRAGMENT_REASSEMBLY = 1
  };

  void SetData (Ptr<const Packet> data);
  void SetHeader (Ipv4Header header);

  void GetData (uint8_t payload[8]) const;
  Ipv4Header GetHeader (void) const;

  static TypeId GetTypeId (void);
  Icmpv4TimeExceeded ();
  virtual ~Icmpv4TimeExceeded ();
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  Ipv4Header m_header;
  uint8_t m_data[8];
};

} // namespace ns3

#endif /* ICMPV4_H */
