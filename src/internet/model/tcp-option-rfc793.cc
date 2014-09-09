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

// TCP options that are specified in RFC 793 (kinds 0, 1, and 2)

#include "tcp-option-rfc793.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("TcpOptionRfc793");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpOptionEnd);

TcpOptionEnd::TcpOptionEnd () : TcpOption ()
{
}

TcpOptionEnd::~TcpOptionEnd ()
{
}

TypeId
TcpOptionEnd::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpOptionEnd")
    .SetParent<TcpOption> ()
    .AddConstructor<TcpOptionEnd> ()
  ;
  return tid;
}

TypeId
TcpOptionEnd::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
TcpOptionEnd::Print (std::ostream &os) const
{
  os << "EOL";
}

uint32_t
TcpOptionEnd::GetSerializedSize (void) const
{
  return 1;
}

void
TcpOptionEnd::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (GetKind ());
}

uint32_t
TcpOptionEnd::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  uint8_t readKind = i.ReadU8 ();

  if (readKind != GetKind ())
    {
      NS_LOG_WARN ("Malformed END option");
      return 0;
    }

  return GetSerializedSize ();
}

uint8_t
TcpOptionEnd::GetKind (void) const
{
  return TcpOption::END;
}


// Tcp Option NOP

NS_OBJECT_ENSURE_REGISTERED (TcpOptionNOP);

TcpOptionNOP::TcpOptionNOP ()
  : TcpOption ()
{
}

TcpOptionNOP::~TcpOptionNOP ()
{
}

TypeId
TcpOptionNOP::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpOptionNOP")
    .SetParent<TcpOption> ()
    .AddConstructor<TcpOptionNOP> ()
  ;
  return tid;
}

TypeId
TcpOptionNOP::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
TcpOptionNOP::Print (std::ostream &os) const
{
  os << "NOP";
}

uint32_t
TcpOptionNOP::GetSerializedSize (void) const
{
  return 1;
}

void
TcpOptionNOP::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (GetKind ());
}

uint32_t
TcpOptionNOP::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  uint8_t readKind = i.ReadU8 ();
  if (readKind != GetKind ())
    {
      NS_LOG_WARN ("Malformed NOP option");
      return 0;
    }

  return GetSerializedSize ();
}

uint8_t
TcpOptionNOP::GetKind (void) const
{
  return TcpOption::NOP;
}

// Tcp Option MSS

NS_OBJECT_ENSURE_REGISTERED (TcpOptionMSS);

TcpOptionMSS::TcpOptionMSS ()
  : TcpOption (),
    m_mss (1460)
{
}

TcpOptionMSS::~TcpOptionMSS ()
{
}

TypeId
TcpOptionMSS::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpOptionMSS")
    .SetParent<TcpOption> ()
    .AddConstructor<TcpOptionMSS> ()
  ;
  return tid;
}

TypeId
TcpOptionMSS::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
TcpOptionMSS::Print (std::ostream &os) const
{
  os << "MSS:" << m_mss;
}

uint32_t
TcpOptionMSS::GetSerializedSize (void) const
{
  return 4;
}

void
TcpOptionMSS::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (GetKind ()); // Kind
  i.WriteU8 (4); // Length
  i.WriteHtonU16 (m_mss); // Max segment size
}

uint32_t
TcpOptionMSS::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  uint8_t readKind = i.ReadU8 ();
  if (readKind != GetKind ())
    {
      NS_LOG_WARN ("Malformed MSS option");
      return 0;
    }

  uint8_t size = i.ReadU8 ();

  NS_ASSERT (size == 4);
  m_mss = i.ReadNtohU16 ();

  return GetSerializedSize ();
}

uint8_t
TcpOptionMSS::GetKind (void) const
{
  return TcpOption::MSS;
}

uint16_t
TcpOptionMSS::GetMSS (void) const
{
  return m_mss;
}

void
TcpOptionMSS::SetMSS (uint16_t mss)
{
  m_mss = mss;
}

// Tcp Option Unknown

NS_OBJECT_ENSURE_REGISTERED (TcpOptionUnknown);

TcpOptionUnknown::TcpOptionUnknown ()
  : TcpOption ()
{
  m_kind = 0xFF;
  m_size = 0;
}

TcpOptionUnknown::~TcpOptionUnknown ()
{
}

TypeId
TcpOptionUnknown::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpOptionUnknown")
    .SetParent<TcpOption> ()
    .AddConstructor<TcpOptionUnknown> ()
  ;
  return tid;
}

TypeId
TcpOptionUnknown::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
TcpOptionUnknown::Print (std::ostream &os) const
{
  os << "Unknown option";
}

uint32_t
TcpOptionUnknown::GetSerializedSize (void) const
{
  return m_size;
}

void
TcpOptionUnknown::Serialize (Buffer::Iterator i) const
{
  if (m_size == 0)
    {
      NS_LOG_WARN ("Can't Serialize an Unknown Tcp Option");
      return;
    }

  i.WriteU8 (GetKind ());
  i.WriteU8 (GetSerializedSize ());
  i.Write (m_content, m_size-2);
}

uint32_t
TcpOptionUnknown::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_kind = i.ReadU8 ();
  NS_LOG_WARN ("Trying to Deserialize an Unknown Option of Kind " << int (m_kind));

  m_size = i.ReadU8 ();
  NS_ASSERT_MSG ((m_size >= 2) && (m_size < 40), "Unable to parse an Unknown Option of Kind " << int (m_kind) << " with apparent size " << int (m_size));

  i.Read (m_content, m_size-2);

  return m_size;
}

uint8_t
TcpOptionUnknown::GetKind (void) const
{
  return m_kind;
}

} // namespace ns3
