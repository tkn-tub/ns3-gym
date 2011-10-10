/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 *         Mehdi Benamor <benamor.mehdi@ensi.rnu.tn> 
 *         David Gross <gdavid.devel@gmail.com>
 */

#include "ns3/assert.h"
#include "ns3/address-utils.h"
#include "ns3/log.h"

#include "icmpv6-header.h"

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (Icmpv6Header);

NS_LOG_COMPONENT_DEFINE ("Icmpv6Header");

TypeId Icmpv6Header::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6Header")
    .SetParent<Header> ()
    .AddConstructor<Icmpv6Header> ()
  ;
  return tid;
}

TypeId Icmpv6Header::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6Header::Icmpv6Header ()
  : m_calcChecksum (true),
    m_checksum (0),
    m_type (0),
    m_code (0)
{
}

Icmpv6Header::~Icmpv6Header ()
{
}

uint8_t Icmpv6Header::GetType () const
{
  return m_type;
}

void Icmpv6Header::SetType (uint8_t type)
{
  m_type = type;
}

uint8_t Icmpv6Header::GetCode () const
{
  return m_code;
}

void Icmpv6Header::SetCode (uint8_t code)
{
  m_code = code;
}

uint16_t Icmpv6Header::GetChecksum () const
{
  return m_checksum;
}

void Icmpv6Header::SetChecksum (uint16_t checksum)
{
  m_checksum = checksum;
}

void Icmpv6Header::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)m_type << " code = " << (uint32_t)m_code << " checksum = " << (uint32_t)m_checksum << ")";
}

uint32_t Icmpv6Header::GetSerializedSize () const
{
  return 4;
}

uint32_t Icmpv6Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_type = i.ReadU8 ();
  m_code = i.ReadU8 ();
  m_checksum = i.ReadNtohU16 ();
#if 0
  i.ReadU32 (); /* padding */
#endif
  return GetSerializedSize ();
}

void Icmpv6Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_type);
  i.WriteU8 (m_code);
  i.WriteU16 (0);
#if 0
  i.WriteU32 (0); /* padding */
#endif

  if (m_calcChecksum)
    {
      i = start;
      uint16_t checksum = i.CalculateIpChecksum (i.GetSize (), m_checksum);
      i = start;
      i.Next (2);
      i.WriteU16 (checksum);
    }
}

void Icmpv6Header::CalculatePseudoHeaderChecksum (Ipv6Address src, Ipv6Address dst, uint16_t length, uint8_t protocol)
{
  Buffer buf = Buffer (40);
  uint8_t tmp[16];
  Buffer::Iterator it;

  buf.AddAtStart (40);
  it = buf.Begin ();

  src.Serialize (tmp);
  it.Write (tmp, 16); /* source IPv6 address */
  dst.Serialize (tmp);
  it.Write (tmp, 16); /* destination IPv6 address */
  it.WriteU16 (0); /* length */
  it.WriteU8 (length >> 8); /* length */
  it.WriteU8 (length & 0xff); /* length */
  it.WriteU16 (0); /* zero */
  it.WriteU8 (0); /* zero */
  it.WriteU8 (protocol); /* next header */

  it = buf.Begin ();
  m_checksum = ~(it.CalculateIpChecksum (40));
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6NS);

Icmpv6NS::Icmpv6NS ()
{
  SetType (ICMPV6_ND_NEIGHBOR_SOLICITATION);
  SetCode (0);
  SetReserved (0);
  m_checksum = 0;
}

TypeId Icmpv6NS::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6NS")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6NS> ()
  ;
  return tid;
}

TypeId Icmpv6NS::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6NS::Icmpv6NS (Ipv6Address target)
{
  SetType (ICMPV6_ND_NEIGHBOR_SOLICITATION);
  SetCode (0);
  SetReserved (0);
  SetIpv6Target (target);
  m_checksum = 0;

  /* test */
  /*
     m_reserved = 0xdeadbeef;
     */
}

Icmpv6NS::~Icmpv6NS ()
{
}

uint32_t Icmpv6NS::GetReserved () const
{
  return m_reserved;
}

void Icmpv6NS::SetReserved (uint32_t reserved)
{
  m_reserved = reserved;
}

Ipv6Address Icmpv6NS::GetIpv6Target () const
{
  return m_target;
}

void Icmpv6NS::SetIpv6Target (Ipv6Address target)
{
  m_target = target;
}

void Icmpv6NS::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (NS) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum ()  << ")";
}

uint32_t Icmpv6NS::GetSerializedSize () const
{
  return 24;
}

void Icmpv6NS::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION_NOARGS ();
  uint8_t buff_target[16];
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteU16 (0);
  i.WriteHtonU32 (m_reserved);
  m_target.Serialize (buff_target);
  i.Write (buff_target, 16);

  if (m_calcChecksum)
    {
      i = start;
      checksum = i.CalculateIpChecksum (i.GetSize (), m_checksum);
      i = start;
      i.Next (2);
      i.WriteU16 (checksum);
    }
}

uint32_t Icmpv6NS::Deserialize (Buffer::Iterator start)
{
  uint8_t buf[16];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  m_reserved = i.ReadNtohU32 ();
  i.Read (buf, 16);
  m_target.Set (buf);

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6NA);

TypeId Icmpv6NA::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6NA")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6NA> ()
  ;
  return tid;
}

TypeId Icmpv6NA::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6NA::Icmpv6NA ()
{
  SetType (ICMPV6_ND_NEIGHBOR_ADVERTISEMENT);
  SetCode (0);
  SetReserved (0);
  SetFlagR (0);
  SetFlagS (0);
  SetFlagO (0);
  m_checksum = 0;
}

Icmpv6NA::~Icmpv6NA ()
{
}

uint32_t Icmpv6NA::GetReserved () const
{
  return m_reserved;
}

void Icmpv6NA::SetReserved (uint32_t reserved)
{
  m_reserved = reserved;
}

Ipv6Address Icmpv6NA::GetIpv6Target () const
{
  return m_target;
}

bool Icmpv6NA::GetFlagR () const
{
  return m_flagR;
}

void Icmpv6NA::SetFlagR (bool r)
{
  m_flagR = r;
}

bool Icmpv6NA::GetFlagS () const
{
  return m_flagS;
}

void Icmpv6NA::SetFlagS (bool s)
{
  m_flagS = s;
}

bool Icmpv6NA::GetFlagO () const
{
  return m_flagO;
}

void Icmpv6NA::SetFlagO (bool o)
{
  m_flagO = o;
}

void Icmpv6NA::SetIpv6Target (Ipv6Address target)
{
  m_target = target;
}

void Icmpv6NA::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (NA) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum () << ")";
}

uint32_t Icmpv6NA::GetSerializedSize () const
{
  return 24;
}

void Icmpv6NA::Serialize (Buffer::Iterator start) const
{
  uint8_t buff_target[16];
  uint16_t checksum = 0;
  Buffer::Iterator i = start;
  uint32_t reserved = m_reserved;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteU16 (0);

  if (m_flagR)
    {
      reserved |= (uint32_t)(1 << 31);
    }

  if (m_flagS)
    {
      reserved |= (uint32_t)(1<< 30);
    }

  if (m_flagO)
    {
      reserved |= (uint32_t)(1<< 29);
    }

  i.WriteHtonU32 (reserved);
  m_target.Serialize (buff_target);
  i.Write (buff_target, 16);

  if (m_calcChecksum)
    {
      i = start;
      checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());
      i = start;
      i.Next (2);
      i.WriteU16 (checksum);
    }
}

uint32_t Icmpv6NA::Deserialize (Buffer::Iterator start)
{
  uint8_t buf[16];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  m_reserved = i.ReadNtohU32 ();

  m_flagR = false;
  m_flagS = false;
  m_flagO = false;

  if (m_reserved & (1 << 31))
    {
      m_flagR = true;
    }

  if (m_reserved & (1 << 30))
    {
      m_flagS = true;
    }

  if (m_reserved & (1 << 29))
    {
      m_flagO = true;
    }

  i.Read (buf, 16);
  m_target.Set (buf);

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6RA);

TypeId Icmpv6RA::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6RA")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6RA> ()
  ;
  return tid;
}

TypeId Icmpv6RA::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6RA::Icmpv6RA ()
{
  SetType (ICMPV6_ND_ROUTER_ADVERTISEMENT);
  SetCode (0);
  SetFlags (0);
  SetFlagM (0);
  SetFlagO (0);
  SetFlagH (0);
  SetCurHopLimit (0);
  SetLifeTime (0);
  SetRetransmissionTime (0);
  SetReachableTime (0);
}

Icmpv6RA::~Icmpv6RA ()
{
}

void Icmpv6RA::SetCurHopLimit (uint8_t m)
{
  m_curHopLimit = m;
}

uint8_t Icmpv6RA::GetCurHopLimit () const
{
  return m_curHopLimit;
}

uint16_t Icmpv6RA::GetLifeTime () const
{
  return m_LifeTime;
}

uint32_t Icmpv6RA::GetReachableTime () const
{
  return m_ReachableTime;
}

uint32_t Icmpv6RA::GetRetransmissionTime () const
{
  return m_RetransmissionTimer;
}

void Icmpv6RA::SetLifeTime (uint16_t l)
{
  m_LifeTime = l;
}

void Icmpv6RA::SetReachableTime (uint32_t r)
{
  m_ReachableTime = r;
}

void Icmpv6RA::SetRetransmissionTime (uint32_t r)
{
  m_RetransmissionTimer = r;
}

bool Icmpv6RA::GetFlagM () const
{
  return m_flagM;
}

void Icmpv6RA::SetFlagM (bool m)
{
  m_flagM = m;
}

bool Icmpv6RA::GetFlagO () const
{
  return m_flagO;
}

void Icmpv6RA::SetFlagO (bool o)
{
  m_flagO = o;
}

bool Icmpv6RA::GetFlagH () const
{
  return m_flagH;
}

void Icmpv6RA::SetFlagH (bool h)
{
  m_flagH = h;
}

uint8_t Icmpv6RA::GetFlags () const
{
  return m_flags;
}

void Icmpv6RA::SetFlags (uint8_t f)
{
  m_flags = f;
}

void Icmpv6RA::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (RA) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum () << ")";
}

uint32_t Icmpv6RA::GetSerializedSize () const
{
  return 16;
}

void Icmpv6RA::Serialize (Buffer::Iterator start) const
{
  uint16_t checksum = 0;
  Buffer::Iterator i = start;
  uint8_t flags = 0;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteHtonU16 (0);
  i.WriteU8 (m_curHopLimit);

  if (m_flagM)
    {
      flags |= (uint8_t)(1<< 7);
    }

  if (m_flagO)
    {
      flags |= (uint8_t)(1<< 6);
    }

  if (m_flagH)
    {
      flags |= (uint8_t)(1<< 5);
    }
  i.WriteU8 (flags);
  i.WriteHtonU16 (GetLifeTime ());
  i.WriteHtonU32 (GetReachableTime ());
  i.WriteHtonU32 (GetRetransmissionTime ());

  i = start;
  checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

  i = start;
  i.Next (2);
  i.WriteU16 (checksum);
}

uint32_t Icmpv6RA::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  SetCurHopLimit (i.ReadU8 ());
  m_flags = i.ReadU8 ();
  m_flagM = false;
  m_flagO = false;
  m_flagH = false;

  if (m_flags & (1 << 7))
    {
      m_flagM = true;
    }

  if (m_flags & (1 << 6))
    {
      m_flagO = true;
    }

  if (m_flags & (1 << 5))
    {
      m_flagH = true;
    }
  SetLifeTime (i.ReadNtohU16 ());
  SetReachableTime (i.ReadNtohU32 ());
  SetRetransmissionTime (i.ReadNtohU32 ());

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6RS);

TypeId Icmpv6RS::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6RS")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6RS> ()
  ;
  return tid;
}

TypeId Icmpv6RS::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6RS::Icmpv6RS ()
{
  SetType (ICMPV6_ND_ROUTER_SOLICITATION);
  SetCode (0);
  SetReserved (0);
}

Icmpv6RS::~Icmpv6RS ()
{
}

uint32_t Icmpv6RS::GetReserved () const
{
  return m_reserved;
}

void Icmpv6RS::SetReserved (uint32_t reserved)
{
  m_reserved = reserved;
}

void Icmpv6RS::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (RS) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum ()  << ")";
}

uint32_t Icmpv6RS::GetSerializedSize () const
{
  return 8;
}

void Icmpv6RS::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION_NOARGS ();
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteU16 (0);
  i.WriteHtonU32 (m_reserved);

  if (m_calcChecksum)
    {
      i = start;
      checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

      i = start;
      i.Next (2);
      i.WriteU16 (checksum);
    }
}

uint32_t Icmpv6RS::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION_NOARGS ();
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  m_reserved = i.ReadNtohU32 ();

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6Redirection);

TypeId Icmpv6Redirection::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6Redirection")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6Redirection> ()
  ;
  return tid;
}

TypeId Icmpv6Redirection::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6Redirection::Icmpv6Redirection ()
  : m_target (Ipv6Address ("")),
    m_destination (Ipv6Address ("")),
    m_reserved (0)
{
  SetType (ICMPV6_ND_REDIRECTION);
  SetCode (0);
  m_checksum = 0;
}

Icmpv6Redirection::~Icmpv6Redirection ()
{
}

void Icmpv6Redirection::SetReserved (uint32_t reserved)
{
  m_reserved = reserved;
}

uint32_t Icmpv6Redirection::GetReserved () const
{
  return m_reserved;
}

Ipv6Address Icmpv6Redirection::GetTarget () const
{
  return m_target;
}

void Icmpv6Redirection::SetTarget (Ipv6Address target)
{
  m_target = target;
}

Ipv6Address Icmpv6Redirection::GetDestination () const
{
  return m_destination;
}

void Icmpv6Redirection::SetDestination (Ipv6Address destination)
{
  m_destination = destination;
}

void Icmpv6Redirection::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (Redirection) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum ()  << " target = " << m_target << " destination = " << m_destination << ")";
}

uint32_t Icmpv6Redirection::GetSerializedSize () const 
{
  return 40;
}

void Icmpv6Redirection::Serialize (Buffer::Iterator start) const 
{
  uint8_t buff[16];
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteU16 (checksum);
  i.WriteU32 (m_reserved);

  m_target.Serialize (buff);
  i.Write (buff, 16);

  m_destination.Serialize (buff);
  i.Write (buff, 16);

  if (m_calcChecksum)
    {
      i = start;
      checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

      i = start;
      i.Next (2);
      i.WriteU16 (checksum);
    }
}

uint32_t Icmpv6Redirection::Deserialize (Buffer::Iterator start) 
{
  uint8_t buff[16];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  SetReserved (i.ReadU32 ());

  i.Read (buff, 16);
  m_target.Set (buff);

  i.Read (buff, 16);
  m_destination.Set (buff);

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6Echo);

TypeId Icmpv6Echo::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6Echo")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6Echo> ()
  ;
  return tid;
}

TypeId Icmpv6Echo::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6Echo::Icmpv6Echo ()
{
  SetType (Icmpv6Header::ICMPV6_ECHO_REQUEST);
  SetCode (0);
  m_checksum = 0;
  SetId (0);
  SetSeq (0);
}

Icmpv6Echo::Icmpv6Echo (bool request)
{
  SetType (request ? Icmpv6Header::ICMPV6_ECHO_REQUEST : Icmpv6Header::ICMPV6_ECHO_REPLY);
  SetCode (0);
  m_checksum = 0;
  SetId (0);
  SetSeq (0);
}

Icmpv6Echo::~Icmpv6Echo ()
{
}

uint16_t Icmpv6Echo::GetId () const
{
  return m_id;
}

void Icmpv6Echo::SetId (uint16_t id)
{
  m_id = id;
}

uint16_t Icmpv6Echo::GetSeq () const
{
  return m_seq;
}

void Icmpv6Echo::SetSeq (uint16_t seq)
{
  m_seq = seq;
}

void Icmpv6Echo::Print (std::ostream& os) const
{
  os << "( type = " << (GetType () == 128 ? "128 (Request)" : "129 (Reply)") << 
  " code = " << (uint32_t)GetCode () <<
  " checksum = "  << (uint32_t)GetChecksum () << ")";
}

uint32_t Icmpv6Echo::GetSerializedSize () const
{
  return 8; 
}

void Icmpv6Echo::Serialize (Buffer::Iterator start) const
{
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteHtonU16 (0);

  i.WriteHtonU16 (m_id);
  i.WriteHtonU16 (m_seq);

  if (m_calcChecksum)
    {
      i = start;
      checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());
      i = start;
      i.Next (2);
      i.WriteU16 (checksum);
    }
}

uint32_t Icmpv6Echo::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();

  m_id = i.ReadNtohU16 ();
  m_seq = i.ReadNtohU16 ();
  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6DestinationUnreachable);

TypeId Icmpv6DestinationUnreachable::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6DestinationUnreachable")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6DestinationUnreachable> ()
  ;
  return tid;
}

TypeId Icmpv6DestinationUnreachable::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6DestinationUnreachable::Icmpv6DestinationUnreachable ()
  : m_packet (0)
{
  SetType (ICMPV6_ERROR_DESTINATION_UNREACHABLE);
}

Icmpv6DestinationUnreachable::~Icmpv6DestinationUnreachable ()
{
}

Ptr<Packet> Icmpv6DestinationUnreachable::GetPacket () const
{
  return m_packet;
}

void Icmpv6DestinationUnreachable::SetPacket (Ptr<Packet> p)
{
  NS_ASSERT (p->GetSize () <= 1280);
  m_packet = p;
}

void Icmpv6DestinationUnreachable::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (Destination Unreachable) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum ()  << ")";
}

uint32_t Icmpv6DestinationUnreachable::GetSerializedSize () const
{
  return 8 + m_packet->GetSize ();
}

void Icmpv6DestinationUnreachable::Serialize (Buffer::Iterator start) const
{
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteHtonU16 (0);
  i.WriteHtonU32 (0);

  uint32_t size = m_packet->GetSize ();
  uint8_t *buf = new uint8_t[size];
  m_packet->CopyData (buf, size);
  i.Write (buf, size);
  delete[] buf;

  i = start;
  checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

  i = start;
  i.Next (2);
  i.WriteU16 (checksum);
}

uint32_t Icmpv6DestinationUnreachable::Deserialize (Buffer::Iterator start)
{
  uint16_t length = start.GetSize () - 8;
  uint8_t* data = new uint8_t[length];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  i.ReadNtohU32 ();
  i.Read (data, length);
  m_packet = Create<Packet> (data, length);

  delete[] data;
  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6TooBig);

TypeId Icmpv6TooBig::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6TooBig")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6TooBig> ()
  ;
  return tid;
}

TypeId Icmpv6TooBig::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6TooBig::Icmpv6TooBig ()
  : m_packet (0),
    m_mtu (0)
{
  SetType (ICMPV6_ERROR_PACKET_TOO_BIG);
}

Icmpv6TooBig::~Icmpv6TooBig ()
{
}

Ptr<Packet> Icmpv6TooBig::GetPacket () const
{
  return m_packet;
}

void Icmpv6TooBig::SetPacket (Ptr<Packet> p)
{
  NS_ASSERT (p->GetSize () <= 1280);
  m_packet = p;
}

uint32_t Icmpv6TooBig::GetMtu () const
{
  return m_mtu;
}

void Icmpv6TooBig::SetMtu (uint32_t mtu) 
{
  m_mtu = mtu;
}

void Icmpv6TooBig::Print (std::ostream& os)  const
{
  os << "( type = " << (uint32_t)GetType () << " (Too Big) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum () << " mtu = " << (uint32_t)GetMtu () << ")";
}

uint32_t Icmpv6TooBig::GetSerializedSize () const 
{
  return 8 + m_packet->GetSize ();
}

void Icmpv6TooBig::Serialize (Buffer::Iterator start) const
{
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteHtonU16 (0);
  i.WriteHtonU32 (GetMtu ());

  uint32_t size = m_packet->GetSize ();
  uint8_t *buf = new uint8_t[size];
  m_packet->CopyData (buf, size);
  i.Write (buf, size);
  delete[] buf;

  i = start;
  checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

  i = start;
  i.Next (2);
  i.WriteU16 (checksum);
}

uint32_t Icmpv6TooBig::Deserialize (Buffer::Iterator start) 
{
  uint16_t length = start.GetSize () - 8;
  uint8_t* data = new uint8_t[length];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  SetMtu (i.ReadNtohU32 ());
  i.Read (data, length);
  m_packet = Create<Packet> (data, length);

  delete[] data;
  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6TimeExceeded);

TypeId Icmpv6TimeExceeded::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6TimeExceeded")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6TimeExceeded> ()
  ;
  return tid;
}

TypeId Icmpv6TimeExceeded::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6TimeExceeded::Icmpv6TimeExceeded ()
  : m_packet (0)
{
  SetType (ICMPV6_ERROR_TIME_EXCEEDED);
}

Icmpv6TimeExceeded::~Icmpv6TimeExceeded ()
{
}

Ptr<Packet> Icmpv6TimeExceeded::GetPacket () const 
{
  return m_packet;
}

void Icmpv6TimeExceeded::SetPacket (Ptr<Packet> p) 
{
  NS_ASSERT (p->GetSize () <= 1280);
  m_packet = p;
}

void Icmpv6TimeExceeded::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (Destination Unreachable) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum ()  << ")";
}

uint32_t Icmpv6TimeExceeded::GetSerializedSize () const
{
  return 8 + m_packet->GetSize ();
}

void Icmpv6TimeExceeded::Serialize (Buffer::Iterator start) const
{
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteHtonU16 (0);
  i.WriteHtonU32 (0);

  uint32_t size = m_packet->GetSize ();
  uint8_t *buf = new uint8_t[size];
  m_packet->CopyData (buf, size);
  i.Write (buf, size);
  delete[] buf;

  i = start;
  checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

  i = start;
  i.Next (2);
  i.WriteU16 (checksum);
}

uint32_t Icmpv6TimeExceeded::Deserialize (Buffer::Iterator start)
{
  uint16_t length = start.GetSize () - 8;
  uint8_t* data = new uint8_t[length];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  i.ReadNtohU32 ();
  i.Read (data, length);
  m_packet = Create<Packet> (data, length);

  delete[] data;
  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6ParameterError);

TypeId Icmpv6ParameterError::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6ParameterError")
    .SetParent<Icmpv6Header> ()
    .AddConstructor<Icmpv6ParameterError> ()
  ;
  return tid;
}

TypeId Icmpv6ParameterError::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6ParameterError::Icmpv6ParameterError ()
  : m_packet (0),
    m_ptr (0)
{
  SetType (ICMPV6_ERROR_PARAMETER_ERROR);
}

Icmpv6ParameterError::~Icmpv6ParameterError ()
{
}

Ptr<Packet> Icmpv6ParameterError::GetPacket () const
{
  return m_packet;
}

void Icmpv6ParameterError::SetPacket (Ptr<Packet> p)
{
  NS_ASSERT (p->GetSize () <= 1280);
  m_packet = p;
}

uint32_t Icmpv6ParameterError::GetPtr () const
{
  return m_ptr;
}

void Icmpv6ParameterError::SetPtr (uint32_t ptr) 
{
  m_ptr = ptr;
}

void Icmpv6ParameterError::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " (Destination Unreachable) code = " << (uint32_t)GetCode () << " checksum = " << (uint32_t)GetChecksum ()  << " ptr = " << (uint32_t)GetPtr () << ")";
}

uint32_t Icmpv6ParameterError::GetSerializedSize () const 
{
  return 8 + m_packet->GetSize ();
}

void Icmpv6ParameterError::Serialize (Buffer::Iterator start) const 
{
  uint16_t checksum = 0;
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetCode ());
  i.WriteHtonU16 (0);
  i.WriteHtonU32 (GetPtr ());

  uint32_t size = m_packet->GetSize ();
  uint8_t *buf = new uint8_t[size];
  m_packet->CopyData (buf, size);
  i.Write (buf, size);
  delete[] buf;

  i = start;
  checksum = i.CalculateIpChecksum (i.GetSize (), GetChecksum ());

  i = start;
  i.Next (2);
  i.WriteU16 (checksum);
}

uint32_t Icmpv6ParameterError::Deserialize (Buffer::Iterator start) 
{
  uint16_t length = start.GetSize () - 8;
  uint8_t* data = new uint8_t[length];
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetCode (i.ReadU8 ());
  m_checksum = i.ReadU16 ();
  SetPtr (i.ReadNtohU32 ());
  i.Read (data, length);
  m_packet = Create<Packet> (data, length);
  delete[] data;

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6OptionHeader);

TypeId Icmpv6OptionHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6OptionHeader")
    .SetParent<Header> ()
    .AddConstructor<Icmpv6OptionHeader> ()
  ;
  return tid;
}

TypeId Icmpv6OptionHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}


Icmpv6OptionHeader::Icmpv6OptionHeader ()
{
  /* TODO */
  m_type = 0;
  m_len = 0;
}

Icmpv6OptionHeader::~Icmpv6OptionHeader ()
{
}

uint8_t Icmpv6OptionHeader::GetType () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_type;
}

void Icmpv6OptionHeader::SetType (uint8_t type)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_type = type;
}

uint8_t Icmpv6OptionHeader::GetLength () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_len;
}

void Icmpv6OptionHeader::SetLength (uint8_t len)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_len = len;
}

void Icmpv6OptionHeader::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << ")";
}

uint32_t Icmpv6OptionHeader::GetSerializedSize () const
{
  return m_len*8;
}

uint32_t Icmpv6OptionHeader::Deserialize (Buffer::Iterator start)
{
  return GetSerializedSize ();
}

void Icmpv6OptionHeader::Serialize (Buffer::Iterator start) const
{
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6OptionMtu);

TypeId Icmpv6OptionMtu::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6OptionMtu")
    .SetParent<Icmpv6OptionHeader> ()
    .AddConstructor<Icmpv6OptionMtu> ()
  ;
  return tid;
}

TypeId Icmpv6OptionMtu::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6OptionMtu::Icmpv6OptionMtu ()
{
  SetType (Icmpv6Header::ICMPV6_OPT_MTU);
  SetLength (1);
  SetReserved (0);
}

Icmpv6OptionMtu::Icmpv6OptionMtu (uint32_t mtu)
  : m_mtu (mtu)
{
  SetType (Icmpv6Header::ICMPV6_OPT_MTU);
  SetLength (1);
  SetReserved (0);
}

Icmpv6OptionMtu::~Icmpv6OptionMtu ()
{
}

uint16_t Icmpv6OptionMtu::GetReserved () const
{
  return m_reserved;
}

void Icmpv6OptionMtu::SetReserved (uint16_t reserved)
{
  m_reserved = reserved;
}

uint32_t Icmpv6OptionMtu::GetMtu () const
{
  return m_mtu;
}

void Icmpv6OptionMtu::SetMtu (uint32_t mtu)
{
  m_mtu = mtu;
}

void Icmpv6OptionMtu::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << " MTU = " << m_mtu << ")";
}

uint32_t Icmpv6OptionMtu::GetSerializedSize () const
{
  return 8; /* m_len = 1 so the real size is multiple by 8 */
}

void Icmpv6OptionMtu::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteHtonU16 (GetReserved ());
  i.WriteHtonU32 (GetMtu ());
}

uint32_t Icmpv6OptionMtu::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  SetReserved (i.ReadNtohU16 ());
  SetMtu (i.ReadNtohU32 ());
  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6OptionPrefixInformation);

TypeId Icmpv6OptionPrefixInformation::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6OptionPrefixInformation")
    .SetParent<Icmpv6OptionHeader> ()
    .AddConstructor<Icmpv6OptionPrefixInformation> ()
  ;
  return tid;
}

TypeId Icmpv6OptionPrefixInformation::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6OptionPrefixInformation::Icmpv6OptionPrefixInformation ()
{
  SetType (Icmpv6Header::ICMPV6_OPT_PREFIX);
  SetLength (4);
  SetPrefix (Ipv6Address ("::"));
  SetPrefixLength (0);
  SetValidTime (0);
  SetPreferredTime (0);
  SetFlags (0);
  SetReserved (0);
}

Icmpv6OptionPrefixInformation::Icmpv6OptionPrefixInformation (Ipv6Address prefix, uint8_t prefixlen)
{
  SetType (Icmpv6Header::ICMPV6_OPT_PREFIX);
  SetLength (4);
  SetPrefix (prefix);
  SetPrefixLength (prefixlen);
  SetValidTime (0);
  SetPreferredTime (0);
  SetFlags (0);
  SetReserved (0);
}

Icmpv6OptionPrefixInformation::~Icmpv6OptionPrefixInformation ()
{
}

uint8_t Icmpv6OptionPrefixInformation::GetPrefixLength () const
{
  return m_prefixLength;
}

void Icmpv6OptionPrefixInformation::SetPrefixLength (uint8_t prefixLength)
{
  NS_ASSERT (prefixLength <= 128);
  m_prefixLength = prefixLength;
}

uint8_t Icmpv6OptionPrefixInformation::GetFlags () const
{
  return m_flags;
}

void Icmpv6OptionPrefixInformation::SetFlags (uint8_t flags)
{
  m_flags = flags;
}

uint32_t Icmpv6OptionPrefixInformation::GetValidTime () const
{
  return m_validTime;
}

void Icmpv6OptionPrefixInformation::SetValidTime (uint32_t validTime)
{
  m_validTime = validTime;
}

uint32_t Icmpv6OptionPrefixInformation::GetPreferredTime () const
{
  return m_preferredTime;
}

void Icmpv6OptionPrefixInformation::SetPreferredTime (uint32_t preferredTime)
{
  m_preferredTime = preferredTime;
}

uint32_t Icmpv6OptionPrefixInformation::GetReserved () const
{
  return m_preferredTime;
}

void Icmpv6OptionPrefixInformation::SetReserved (uint32_t reserved)
{
  m_reserved = reserved;
}

Ipv6Address Icmpv6OptionPrefixInformation::GetPrefix () const
{
  return m_prefix;
}

void Icmpv6OptionPrefixInformation::SetPrefix (Ipv6Address prefix)
{
  m_prefix = prefix;
}

void Icmpv6OptionPrefixInformation::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << " prefix " << m_prefix << ")";
}

uint32_t Icmpv6OptionPrefixInformation::GetSerializedSize () const
{
  return 32;
}

void Icmpv6OptionPrefixInformation::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  memset (buf, 0x00, sizeof (buf));

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 (m_prefixLength);
  i.WriteU8 (m_flags);
  i.WriteHtonU32 (m_validTime);
  i.WriteHtonU32 (m_preferredTime);
  i.WriteHtonU32 (m_reserved);
  m_prefix.GetBytes (buf);
  i.Write (buf, 16);
}

uint32_t Icmpv6OptionPrefixInformation::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  SetPrefixLength (i.ReadU8 ());
  SetFlags (i.ReadU8 ());
  SetValidTime (i.ReadNtohU32 ());
  SetPreferredTime (i.ReadNtohU32 ());
  SetReserved (i.ReadNtohU32 ());
  i.Read (buf, 16);

  Ipv6Address prefix (buf);
  SetPrefix (prefix);

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6OptionLinkLayerAddress);

TypeId Icmpv6OptionLinkLayerAddress::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6OptionLinkLayerAddress")
    .SetParent<Icmpv6OptionHeader> ()
    .AddConstructor<Icmpv6OptionLinkLayerAddress> ()
  ;
  return tid;
}

TypeId Icmpv6OptionLinkLayerAddress::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress (bool source)
{
  SetType (source ? Icmpv6Header::ICMPV6_OPT_LINK_LAYER_SOURCE : Icmpv6Header::ICMPV6_OPT_LINK_LAYER_TARGET);
}

Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress ()
{
  SetType (Icmpv6Header::ICMPV6_OPT_LINK_LAYER_SOURCE);
}

Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress (bool source, Address addr)
{
  SetType (source ? Icmpv6Header::ICMPV6_OPT_LINK_LAYER_SOURCE : Icmpv6Header::ICMPV6_OPT_LINK_LAYER_TARGET);
  SetAddress (addr);
  SetLength (GetSerializedSize () / 8);
}

Icmpv6OptionLinkLayerAddress::~Icmpv6OptionLinkLayerAddress ()
{
}

Address Icmpv6OptionLinkLayerAddress::GetAddress () const
{
  return m_addr;
}

void Icmpv6OptionLinkLayerAddress::SetAddress (Address addr)
{
  m_addr = addr;
}

void Icmpv6OptionLinkLayerAddress::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << " L2 Address = " << m_addr << ")";
}

uint32_t Icmpv6OptionLinkLayerAddress::GetSerializedSize () const
{
  /* TODO add padding */
  uint8_t nb = 2 + m_addr.GetLength ();
  return nb;
}

void Icmpv6OptionLinkLayerAddress::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION_NOARGS ();
  Buffer::Iterator i = start;
  uint8_t mac[32];

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  m_addr.CopyTo (mac);
  i.Write (mac, m_addr.GetLength ());

  /* XXX if size of the option is not a multiple of 8, add padding */
}

uint32_t Icmpv6OptionLinkLayerAddress::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t mac[32];

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  i.Read (mac, (GetLength () * 8) - 2);

  m_addr.CopyFrom (mac, (GetLength () * 8)-2);

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (Icmpv6OptionRedirected);

TypeId Icmpv6OptionRedirected::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6OptionRedirected")
    .SetParent<Icmpv6OptionHeader> ()
    .AddConstructor<Icmpv6OptionRedirected> ()
  ;
  return tid;
}

TypeId Icmpv6OptionRedirected::GetInstanceTypeId () const
{
  return GetTypeId ();
}

Icmpv6OptionRedirected::Icmpv6OptionRedirected ()
  : m_packet (0)
{
  SetType (Icmpv6Header::ICMPV6_OPT_REDIRECTED);
}

Icmpv6OptionRedirected::~Icmpv6OptionRedirected ()
{
  m_packet = 0;
}

Ptr<Packet> Icmpv6OptionRedirected::GetPacket () const
{
  return m_packet;
}

void Icmpv6OptionRedirected::SetPacket (Ptr<Packet> packet)
{
  NS_ASSERT (packet->GetSize () <= 1280);
  m_packet = packet;
  SetLength (1 + (m_packet->GetSize () / 8));
}

void Icmpv6OptionRedirected::Print (std::ostream& os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << ")";
}

uint32_t Icmpv6OptionRedirected::GetSerializedSize () const
{
  return 8 + m_packet->GetSize ();
}

void Icmpv6OptionRedirected::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION_NOARGS ();
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  // Reserved
  i.WriteU16 (0);
  i.WriteU32 (0);

  uint32_t size = m_packet->GetSize ();
  uint8_t *buf = new uint8_t[size];
  m_packet->CopyData (buf, size);
  i.Write (buf, size);
  delete[] buf;
}

uint32_t Icmpv6OptionRedirected::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  uint8_t length = i.ReadU8 ();
  SetLength (length);
  i.ReadU16 ();
  i.ReadU32 ();

  uint32_t len2 = (GetLength () - 1) * 8;
  uint8_t* buff = new uint8_t[len2];
  i.Read (buff, len2);
  m_packet = Create<Packet> (buff, len2);
  delete[] buff;

  return GetSerializedSize ();
}

} /* namespace ns3 */

