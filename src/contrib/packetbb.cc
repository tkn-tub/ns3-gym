/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* vim: set noai ts=2 sw=2 expandtab: */
/* 
 * Copyright (c) 2009 Drexel University
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
 * Author: Tom Wambold <tom5760@gmail.com>
 */
/* TODO:
 * - Check style
 * - Check copy constructors
 */

#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"

#include "packetbb.h"

static const uint8_t VERSION = 0;
/* Packet flags */
static const uint8_t PHAS_SEQ_NUM = 0x8;
static const uint8_t PHAS_TLV = 0x4;

/* Message flags */
static const uint8_t MHAS_ORIG = 0x80;
static const uint8_t MHAS_HOP_LIMIT = 0x40;
static const uint8_t MHAS_HOP_COUNT = 0x20;
static const uint8_t MHAS_SEQ_NUM = 0x10;

/* Address block flags */
static const uint8_t AHAS_HEAD = 0x80;
static const uint8_t AHAS_FULL_TAIL = 0x40;
static const uint8_t AHAS_ZERO_TAIL = 0x20;
static const uint8_t AHAS_SINGLE_PRE_LEN = 0x10;
static const uint8_t AHAS_MULTI_PRE_LEN = 0x08;

/* TLV Flags */
static const uint8_t THAS_TYPE_EXT = 0x80;
static const uint8_t THAS_SINGLE_INDEX = 0x40;
static const uint8_t THAS_MULTI_INDEX = 0x20;
static const uint8_t THAS_VALUE = 0x10;
static const uint8_t THAS_EXT_LEN = 0x08;
static const uint8_t TIS_MULTIVALUE = 0x04;

namespace ns3 {

namespace pbb {

NS_OBJECT_ENSURE_REGISTERED (PacketBB);

TlvBlock::Iterator
TlvBlock::Begin (void)
{
  return m_tlvList.begin ();
}

TlvBlock::ConstIterator
TlvBlock::Begin (void) const
{
  return m_tlvList.begin ();
}

TlvBlock::Iterator
TlvBlock::End (void)
{
  return m_tlvList.end ();
}

TlvBlock::ConstIterator
TlvBlock::End (void) const
{
  return m_tlvList.end ();
}

int
TlvBlock::Size (void) const
{
  return m_tlvList.size ();
}

bool
TlvBlock::Empty (void) const
{
  return m_tlvList.empty ();
}

Ptr<Tlv>
TlvBlock::Front (void) const
{
  return m_tlvList.front ();
}

Ptr<Tlv>
TlvBlock::Back (void) const
{
  return m_tlvList.back ();
}

void
TlvBlock::PushFront (Ptr<Tlv> tlv)
{
  //Ptr<Tlv> * newptr = new Ptr<Tlv> (GetPointer<Tlv> (tlv));
  m_tlvList.push_front (tlv);
}

void
TlvBlock::PopFront (void)
{
  m_tlvList.pop_front ();
}

void
TlvBlock::PushBack (Ptr<Tlv> tlv)
{
  //Ptr<Tlv> * newptr = new Ptr<Tlv> (GetPointer<Tlv> (tlv));
  m_tlvList.push_back (tlv);
}

void
TlvBlock::PopBack (void)
{
  m_tlvList.pop_back ();
}

TlvBlock::Iterator
TlvBlock::Insert (TlvBlock::Iterator position, const Ptr<Tlv> tlv)
{
  return m_tlvList.insert (position, tlv);
}

TlvBlock::Iterator
TlvBlock::Erase (TlvBlock::Iterator position)
{
  return m_tlvList.erase (position);
}

TlvBlock::Iterator
TlvBlock::Erase (TlvBlock::Iterator first, TlvBlock::Iterator last)
{
  return m_tlvList.erase (first, last);
}

void
TlvBlock::Clear (void)
{
  m_tlvList.clear ();
}

uint32_t
TlvBlock::GetSerializedSize (void) const
{
  /* tlv size */
  uint32_t size = 2;
  for (ConstIterator iter = Begin (); iter != End (); iter++)
  {
    size += (*iter)->GetSerializedSize ();
  }
  return size;
}

void
TlvBlock::Serialize (Buffer::Iterator &start) const
{
  if (Empty ())
  {
    start.WriteHtonU16 (0);
    return;
  }

  /* We need to write the size of the TLV block in front, so save its
   * position. */
  Buffer::Iterator tlvsize = start;
  start.Next (2);
  for (ConstIterator iter = Begin (); iter != End (); iter++)
  {
    (*iter)->Serialize (start);
  }
  /* - 2 to not include the size field */
  uint16_t size = start.GetDistanceFrom (tlvsize) - 2;
  tlvsize.WriteHtonU16 (size);
}

void
TlvBlock::Deserialize (Buffer::Iterator &start)
{
  uint16_t size = start.ReadNtohU16 ();

  Buffer::Iterator tlvstart = start;
  if (size > 0)
  {
    while (start.GetDistanceFrom (tlvstart) < size)
    {
      Ptr<Tlv> newtlv = Create<Tlv> ();
      //Tlv * newtlv = new Tlv ();
      newtlv->Deserialize (start);

      //Ptr<Tlv> * newptr = new Ptr<Tlv> (newtlv);
      PushBack (newtlv);
    }
  }
}

void
TlvBlock::Print (std::ostream &os) const
{
  Print (os, 0);
}

void
TlvBlock::Print (std::ostream &os, int level) const
{
  std::string prefix = "";
  for (int i = 0; i < level; i++)
    prefix.append("\t");

  os << prefix << "TLV Block {" << std::endl;
  os << prefix << "\tsize = " << Size () << std::endl;
  os << prefix << "\tmembers [" << std::endl;
  for (ConstIterator iter = Begin (); iter != End (); iter++)
  {
    (*iter)->Print (os, level+2);
  }
  os << prefix << "\t]" << std::endl;
  os << prefix << "}" << std::endl;
}

bool
TlvBlock::operator== (const TlvBlock &other) const
{
  if (Size () != other.Size ())
    return false;

  ConstIterator ti, oi;
  for (ti = Begin (), oi = other.Begin ();
      ti != End () && oi != other.End ();
      ti++, oi++)
  {
    if (**ti != **oi)
      return false;
  }
  return true;
}

bool
TlvBlock::operator!= (const TlvBlock &other) const
{
  return !(*this == other);
}

/* End TlvBlock class */

AddressTlvBlock::Iterator
AddressTlvBlock::Begin (void)
{
  return m_tlvList.begin ();
}

AddressTlvBlock::ConstIterator
AddressTlvBlock::Begin (void) const
{
  return m_tlvList.begin ();
}

AddressTlvBlock::Iterator
AddressTlvBlock::End (void)
{
  return m_tlvList.end ();
}

AddressTlvBlock::ConstIterator
AddressTlvBlock::End (void) const
{
  return m_tlvList.end ();
}

int
AddressTlvBlock::Size (void) const
{
  return m_tlvList.size ();
}

bool
AddressTlvBlock::Empty (void) const
{
  return m_tlvList.empty ();
}

Ptr<AddressTlv>
AddressTlvBlock::Front (void) const
{
  return m_tlvList.front ();
}

Ptr<AddressTlv>
AddressTlvBlock::Back (void) const
{
  return m_tlvList.back ();
}

void
AddressTlvBlock::PushFront (Ptr<AddressTlv> tlv)
{
  m_tlvList.push_front (tlv);
}

void
AddressTlvBlock::PopFront (void)
{
  m_tlvList.pop_front ();
}

void
AddressTlvBlock::PushBack (Ptr<AddressTlv> tlv)
{
  m_tlvList.push_back (tlv);
}

void
AddressTlvBlock::PopBack (void)
{
  m_tlvList.pop_back ();
}

AddressTlvBlock::Iterator
AddressTlvBlock::Insert (AddressTlvBlock::Iterator position, const Ptr<AddressTlv> tlv)
{
  return m_tlvList.insert (position, tlv);
}

AddressTlvBlock::Iterator
AddressTlvBlock::Erase (AddressTlvBlock::Iterator position)
{
  return m_tlvList.erase (position);
}

AddressTlvBlock::Iterator
AddressTlvBlock::Erase (AddressTlvBlock::Iterator first, AddressTlvBlock::Iterator last)
{
  return m_tlvList.erase (first, last);
}

void
AddressTlvBlock::Clear (void)
{
  m_tlvList.clear ();
}

uint32_t
AddressTlvBlock::GetSerializedSize (void) const
{
  /* tlv size */
  uint32_t size = 2;
  for (ConstIterator iter = Begin (); iter != End (); iter++)
  {
    size += (*iter)->GetSerializedSize ();
  }
  return size;
}

void
AddressTlvBlock::Serialize (Buffer::Iterator &start) const
{
  if (Empty ())
  {
    start.WriteHtonU16 (0);
    return;
  }

  /* We need to write the size of the TLV block in front, so save its
   * position. */
  Buffer::Iterator tlvsize = start;
  start.Next (2);
  for (ConstIterator iter = Begin (); iter != End (); iter++)
  {
    (*iter)->Serialize (start);
  }
  /* - 2 to not include the size field */
  uint16_t size = start.GetDistanceFrom (tlvsize) - 2;
  tlvsize.WriteHtonU16 (size);
}

void
AddressTlvBlock::Deserialize (Buffer::Iterator &start)
{
  uint16_t size = start.ReadNtohU16 ();

  Buffer::Iterator tlvstart = start;
  if (size > 0)
  {
    while (start.GetDistanceFrom (tlvstart) < size)
    {
      Ptr<AddressTlv> newtlv = Create<AddressTlv> ();
      //AddressTlv * newtlv = new AddressTlv ();
      newtlv->Deserialize (start);

      //Ptr<AddressTlv> * newptr = new Ptr<AddressTlv> (newtlv);
      PushBack (newtlv);
    }
  }
}

void
AddressTlvBlock::Print (std::ostream &os) const
{
  Print (os, 0);
}

void
AddressTlvBlock::Print (std::ostream &os, int level) const
{
  std::string prefix = "";
  for (int i = 0; i < level; i++)
    prefix.append("\t");

  os << prefix << "TLV Block {" << std::endl;
  os << prefix << "\tsize = " << Size () << std::endl;
  os << prefix << "\tmembers [" << std::endl;
  for (ConstIterator iter = Begin (); iter != End (); iter++)
  {
    (*iter)->Print (os, level+2);
  }
  os << prefix << "\t]" << std::endl;
  os << prefix << "}" << std::endl;
}

bool
AddressTlvBlock::operator== (const AddressTlvBlock &other) const
{
  if (Size () != other.Size ())
    return false;

  ConstIterator it, ot;
  for (it = Begin (), ot = other.Begin ();
      it != End () && ot != other.End ();
      it++, ot++)
  {
    if (**it != **ot)
      return false;
  }
  return true;
}

bool
AddressTlvBlock::operator!= (const AddressTlvBlock &other) const
{
  return !(*this == other);
}


/* End AddressTlvBlock Class */

PacketBB::PacketBB (void)
{
  m_refCount = 1;
  m_version = VERSION;
  m_hasseqnum = false;
}

uint8_t
PacketBB::GetVersion (void) const
{
  return m_version;
}

void
PacketBB::SetSequenceNumber (uint16_t number)
{
  m_seqnum = number;
  m_hasseqnum = true;
}

uint16_t
PacketBB::GetSequenceNumber (void) const throw (PacketBBError)
{
  if (!HasSequenceNumber ())
  {
    throw PacketBBError ("Packet has no sequence number.");
  }
  return m_seqnum;
}

bool
PacketBB::HasSequenceNumber (void) const
{
  return m_hasseqnum;
}

/* Manipulating Packet TLVs */

PacketBB::TlvIterator
PacketBB::TlvBegin (void)
{
  return m_tlvList.Begin ();
}

PacketBB::ConstTlvIterator
PacketBB::TlvBegin (void) const
{
  return m_tlvList.Begin ();
}

PacketBB::TlvIterator
PacketBB::TlvEnd (void)
{
  return m_tlvList.End ();
}

PacketBB::ConstTlvIterator
PacketBB::TlvEnd (void) const
{
  return m_tlvList.End ();
}

int
PacketBB::TlvSize (void) const
{
  return m_tlvList.Size ();
}

bool
PacketBB::TlvEmpty (void) const
{
  return m_tlvList.Empty ();
}

Ptr<Tlv>
PacketBB::TlvFront (void)
{
  return m_tlvList.Front ();
}

const Ptr<Tlv>
PacketBB::TlvFront (void) const
{
  return m_tlvList.Front ();
}

Ptr<Tlv>
PacketBB::TlvBack (void)
{
  return m_tlvList.Back ();
}

const Ptr<Tlv>
PacketBB::TlvBack (void) const
{
  return m_tlvList.Back ();
}

void
PacketBB::TlvPushFront (Ptr<Tlv> tlv)
{
  m_tlvList.PushFront (tlv);
}

void
PacketBB::TlvPopFront (void)
{
  m_tlvList.PopFront ();
}

void
PacketBB::TlvPushBack (Ptr<Tlv> tlv)
{
  m_tlvList.PushBack (tlv);
}

void
PacketBB::TlvPopBack (void)
{
  m_tlvList.PopBack ();
}

PacketBB::TlvIterator
PacketBB::Erase (PacketBB::TlvIterator position)
{
  return m_tlvList.Erase (position);
}

PacketBB::TlvIterator
PacketBB::Erase (PacketBB::TlvIterator first, PacketBB::TlvIterator last)
{
  return m_tlvList.Erase (first, last);
}

void
PacketBB::TlvClear (void)
{
  m_tlvList.Clear ();
}

/* Manipulating Packet Messages */

PacketBB::MessageIterator
PacketBB::MessageBegin (void)
{
  return m_messageList.begin ();
}

PacketBB::ConstMessageIterator
PacketBB::MessageBegin (void) const
{
  return m_messageList.begin ();
}

PacketBB::MessageIterator
PacketBB::MessageEnd (void)
{
  return m_messageList.end ();
}

PacketBB::ConstMessageIterator
PacketBB::MessageEnd (void) const
{
  return m_messageList.end ();
}

int
PacketBB::MessageSize (void) const
{
  return m_messageList.size ();
}

bool
PacketBB::MessageEmpty (void) const
{
  return m_messageList.empty ();
}

Ptr<Message>
PacketBB::MessageFront (void)
{
  return m_messageList.front ();
}

const Ptr<Message>
PacketBB::MessageFront (void) const
{
  return m_messageList.front ();
}

Ptr<Message>
PacketBB::MessageBack (void)
{
  return m_messageList.back ();
}

const Ptr<Message>
PacketBB::MessageBack (void) const
{
  return m_messageList.back ();
}

void
PacketBB::MessagePushFront (Ptr<Message> tlv)
{
  m_messageList.push_front (tlv);
}

void
PacketBB::MessagePopFront (void)
{
  m_messageList.pop_front ();
}

void
PacketBB::MessagePushBack (Ptr<Message> tlv)
{
  m_messageList.push_back (tlv);
}

void
PacketBB::MessagePopBack (void)
{
  m_messageList.pop_back ();
}

PacketBB::MessageIterator
PacketBB::Erase (PacketBB::MessageIterator position)
{
  return m_messageList.erase (position);
}

PacketBB::MessageIterator
PacketBB::Erase (PacketBB::MessageIterator first,
    PacketBB::MessageIterator last)
{
  return m_messageList.erase (first, last);
}

void
PacketBB::MessageClear (void)
{
  m_messageList.clear ();
}

void
PacketBB::Ref (void) const
{
  m_refCount++;
}

void
PacketBB::Unref (void) const
{
  m_refCount--;
  if (m_refCount == 0)
  {
    delete this;
  }
}

TypeId
PacketBB::GetTypeId (void)
{
  static TypeId tid = TypeId ("PacketBB")
    .SetParent<Header> ()
    .AddConstructor<PacketBB> ()
  ;
  return tid;
}

TypeId
PacketBB::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
PacketBB::GetSerializedSize (void) const
{
  /* Version number + flags */
  uint32_t size = 1;

  if (HasSequenceNumber())
  {
    size += 2;
  }

  if (!TlvEmpty ())
    size += m_tlvList.GetSerializedSize ();

  for (ConstMessageIterator iter = MessageBegin (); iter != MessageEnd ();
      iter++)
  {
    size += (*iter)->GetSerializedSize ();
  }

  return size;
}

void
PacketBB::Serialize (Buffer::Iterator start) const
{
  /* We remember the start, so we can write the flags after we check for a
   * sequence number and TLV. */
  Buffer::Iterator bufref = start;
  start.Next ();

  uint8_t flags = VERSION;
  /* Make room for 4 bit flags */
  flags <<= 4;

  if (HasSequenceNumber ())
  {
    flags |= PHAS_SEQ_NUM;
    start.WriteHtonU16 (GetSequenceNumber ());
  }

  if (!TlvEmpty ())
  {
    flags |= PHAS_TLV;
    m_tlvList.Serialize (start);
  }

  bufref.WriteU8(flags);

  for (ConstMessageIterator iter = MessageBegin ();
      iter != MessageEnd ();
      iter++)
  {
    (*iter)->Serialize (start);
  }
}

uint32_t
PacketBB::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator begin = start;

  uint8_t flags = start.ReadU8 ();

  if (flags & PHAS_SEQ_NUM)
    SetSequenceNumber (start.ReadNtohU16 ());

  if (flags & PHAS_TLV)
    m_tlvList.Deserialize (start);

  while (!start.IsEnd())
  {
    Ptr<Message> newmsg = Message::DeserializeMessage (start);
    //Message * newmsg = Message::DeserializeMessage (start);
    //Ptr<Message> * newptr = new Ptr<Message> (newmsg);
    MessagePushBack (newmsg);
  }

  flags >>= 4;
  m_version = flags;

  return start.GetDistanceFrom (begin);
}

void
PacketBB::Print (std::ostream &os) const
{
  os << "PacketBB {" << std::endl;

  if (HasSequenceNumber ())
    os << "\tsequence number = " << GetSequenceNumber ();

  os << std::endl;

  m_tlvList.Print (os, 1);

  for (ConstMessageIterator iter = MessageBegin ();
      iter != MessageEnd ();
      iter++)
  {
    (*iter)->Print (os, 1);
  }

  os << "}" << std::endl;
}

bool
PacketBB::operator== (const PacketBB &other) const
{
  if (GetVersion () != other.GetVersion ())
    return false;

  if (HasSequenceNumber () != other.HasSequenceNumber ())
    return false;

  if (HasSequenceNumber ())
  {
    if (GetSequenceNumber () != other.GetSequenceNumber ())
      return false;
  }

  if (m_tlvList != other.m_tlvList)
    return false;

  if (MessageSize () != other.MessageSize ())
      return false;

  ConstMessageIterator tmi, omi;
  for (tmi = MessageBegin (), omi = other.MessageBegin ();
    tmi != MessageEnd () && omi != other.MessageEnd ();
    tmi++, omi++)
  {
    if (**tmi != **omi)
      return false;
  }
  return true;
}

bool
PacketBB::operator!= (const PacketBB &other) const
{
  return !(*this == other);
}

/* End PacketBB class */

Message::Message (void)
{
  m_refCount = 1;
  /* Default to IPv4 */
  m_addrSize = IPV4;
  m_hasOriginatorAddress = false;
  m_hasHopLimit = false;
  m_hasHopCount = false;
  m_hasSequenceNumber = false;
}

void
Message::SetType (uint8_t type)
{
  m_type = type;
}

uint8_t
Message::GetType (void) const
{
  return m_type;
}

AddressLength
Message::GetAddressLength (void) const
{
  return m_addrSize;
}

void
Message::SetOriginatorAddress (Address address)
{
  m_originatorAddress = address;
  m_hasOriginatorAddress = true;
}

Address
Message::GetOriginatorAddress (void) const throw (PacketBBError)
{
  if (!HasOriginatorAddress()) 
  {
    throw PacketBBError ("Message has no originator address.");
  }
  return m_originatorAddress;
}

bool
Message::HasOriginatorAddress (void) const
{
  return m_hasOriginatorAddress;
}

void
Message::SetHopLimit (uint8_t hopLimit)
{
  m_hopLimit = hopLimit;
  m_hasHopLimit = true;
}

uint8_t
Message::GetHopLimit (void) const throw (PacketBBError)
{
  if (!HasHopLimit())
  {
    throw PacketBBError ("Message has no hop limit.");
  }
  return m_hopLimit;
}

bool
Message::HasHopLimit (void) const
{
  return m_hasHopLimit;
}

void
Message::SetHopCount (uint8_t hopCount)
{
  m_hopCount = hopCount;
  m_hasHopCount = true;
}

uint8_t
Message::GetHopCount (void) const throw (PacketBBError)
{
  if (!HasHopCount())
  {
    throw PacketBBError ("Message has no hop count.");
  }
  return m_hopCount;
}

bool
Message::HasHopCount (void) const
{
  return m_hasHopCount;
}

void
Message::SetSequenceNumber (uint16_t sequenceNumber)
{
  m_sequenceNumber = sequenceNumber;
  m_hasSequenceNumber = true;
}

uint16_t
Message::GetSequenceNumber (void) const throw (PacketBBError)
{
  if (!HasSequenceNumber())
  {
    throw PacketBBError ("Message has no sequence number.");
  }
  return m_sequenceNumber;
}

bool
Message::HasSequenceNumber (void) const
{
  return m_hasSequenceNumber;
}

/* Manipulating Message TLVs */

Message::TlvIterator
Message::TlvBegin (void)
{
  return m_tlvList.Begin();
}

Message::ConstTlvIterator
Message::TlvBegin (void) const
{
  return m_tlvList.Begin();
}

Message::TlvIterator
Message::TlvEnd (void)
{
  return m_tlvList.End();
}

Message::ConstTlvIterator
Message::TlvEnd (void) const
{
  return m_tlvList.End();
}

int
Message::TlvSize (void) const
{
  return m_tlvList.Size();
}

bool
Message::TlvEmpty (void) const
{
  return m_tlvList.Empty();
}

Ptr<Tlv>
Message::TlvFront (void)
{
  return m_tlvList.Front();
}

const Ptr<Tlv>
Message::TlvFront (void) const
{
  return m_tlvList.Front();
}

Ptr<Tlv>
Message::TlvBack (void)
{
  return m_tlvList.Back();
}

const Ptr<Tlv>
Message::TlvBack (void) const
{
  return m_tlvList.Back();
}

void
Message::TlvPushFront (Ptr<Tlv> tlv)
{
  m_tlvList.PushFront(tlv);
}

void
Message::TlvPopFront (void)
{
  m_tlvList.PopFront();
}

void
Message::TlvPushBack (Ptr<Tlv> tlv)
{
  m_tlvList.PushBack(tlv);
}

void
Message::TlvPopBack (void)
{
  m_tlvList.PopBack();
}

Message::TlvIterator
Message::TlvErase (Message::TlvIterator position)
{
  return m_tlvList.Erase(position);
}

Message::TlvIterator
Message::TlvErase (Message::TlvIterator first, Message::TlvIterator last)
{
  return m_tlvList.Erase(first, last);
}

void
Message::TlvClear (void)
{
  return m_tlvList.Clear();
}

/* Manipulating Address Block and Address TLV pairs */

Message::AddressBlockIterator
Message::AddressBlockBegin (void)
{
  return m_addressBlockList.begin();
}

Message::ConstAddressBlockIterator
Message::AddressBlockBegin (void) const
{
  return m_addressBlockList.begin();
}

Message::AddressBlockIterator
Message::AddressBlockEnd (void)
{
  return m_addressBlockList.end();
}

Message::ConstAddressBlockIterator
Message::AddressBlockEnd (void) const
{
  return m_addressBlockList.end();
}

int
Message::AddressBlockSize (void) const
{
  return m_addressBlockList.size();
}

bool
Message::AddressBlockEmpty (void) const
{
  return m_addressBlockList.empty();
}

Ptr<AddressBlock>
Message::AddressBlockFront (void)
{
  return m_addressBlockList.front();
}

const Ptr<AddressBlock>
Message::AddressBlockFront (void) const
{
  return m_addressBlockList.front();
}

Ptr<AddressBlock>
Message::AddressBlockBack (void)
{
  return m_addressBlockList.back();
}

const Ptr<AddressBlock>
Message::AddressBlockBack (void) const
{
  return m_addressBlockList.back();
}

void
Message::AddressBlockPushFront (Ptr<AddressBlock> tlv)
{
  m_addressBlockList.push_front(tlv);
}

void
Message::AddressBlockPopFront (void)
{
  m_addressBlockList.pop_front();
}

void
Message::AddressBlockPushBack (Ptr<AddressBlock> tlv)
{
  m_addressBlockList.push_back(tlv);
}

void
Message::AddressBlockPopBack (void)
{
  m_addressBlockList.pop_back();
}

Message::AddressBlockIterator
Message::AddressBlockErase (Message::AddressBlockIterator position)
{
  return m_addressBlockList.erase(position);
}

Message::AddressBlockIterator
Message::AddressBlockErase (Message::AddressBlockIterator first,
    Message::AddressBlockIterator last)
{
  return m_addressBlockList.erase(first, last);
}

void
Message::AddressBlockClear (void)
{
  return m_addressBlockList.clear();
}

void
Message::Ref (void) const
{
  m_refCount++;
}

void
Message::Unref (void) const
{
  m_refCount--;
  if (m_refCount == 0)
  {
    delete this;
  }
}

uint32_t
Message::GetSerializedSize (void) const
{
  /* msg-type + (msg-flags + msg-addr-length) + 2msg-size */
  uint32_t size = 4;

  if (HasOriginatorAddress())
    size += GetAddressLength() + 1;

  if (HasHopLimit())
    size++;

  if (HasHopCount())
    size++;

  if (HasSequenceNumber())
    size += 2;

  size += m_tlvList.GetSerializedSize ();

  for (ConstAddressBlockIterator iter = AddressBlockBegin ();
      iter != AddressBlockEnd ();
      iter++)
  {
    size += (*iter)->GetSerializedSize ();
  }

  return size;
}

void
Message::Serialize (Buffer::Iterator &start) const
{
  Buffer::Iterator front = start;

  start.WriteU8 (GetType());

  /* Save a reference to the spot where we will later write the flags */
  Buffer::Iterator bufref = start;
  start.Next (1);

  uint8_t flags = 0;

  flags = GetAddressLength ();

  Buffer::Iterator sizeref = start;
  start.Next (2);

  if (HasOriginatorAddress ())
  {
    flags |= MHAS_ORIG;
    SerializeOriginatorAddress (start);
  }

  if (HasHopLimit ())
  {
    flags |= MHAS_HOP_LIMIT;
    start.WriteU8 (GetHopLimit ());
  }

  if (HasHopCount ())
  {
    flags |= MHAS_HOP_COUNT;
    start.WriteU8 (GetHopCount ());
  }

  if (HasSequenceNumber ())
  {
    flags |= MHAS_SEQ_NUM;
    start.WriteHtonU16 (GetSequenceNumber ());
  }

  bufref.WriteU8(flags);

  m_tlvList.Serialize (start);

  for (ConstAddressBlockIterator iter = AddressBlockBegin ();
      iter != AddressBlockEnd ();
      iter++)
  {
    (*iter)->Serialize (start);
  }

  sizeref.WriteHtonU16 (front.GetDistanceFrom (start));
}

Ptr<Message>
Message::DeserializeMessage (Buffer::Iterator &start) throw (PacketBBError)
{
  /* We need to read the msg-addr-len field to determine what kind of object to
   * construct. */
  start.Next ();
  uint8_t addrlen = start.ReadU8 ();
  start.Prev (2); /* Go back to the start */

  /* The first four bytes of the flag is the address length.  Set the last four
   * bytes to 0 to read it. */
  addrlen = (addrlen & 0xf);

  Ptr<Message> newmsg;

  switch (addrlen)
  {
    case 0:
    case IPV4:
      newmsg = Create<MessageIpv4> ();
      break;
    case IPV6:
      newmsg = Create<MessageIpv6> ();
      break;
    default:
      throw PacketBBError ("Message deserialization has invalid address size.");
      break;
  }
  newmsg->Deserialize (start);
  return newmsg;
}

void
Message::Deserialize (Buffer::Iterator &start)
{
  Buffer::Iterator front = start;
  SetType (start.ReadU8 ());
  uint8_t flags = start.ReadU8 ();

  uint16_t size = start.ReadNtohU16 ();

  if (flags & MHAS_ORIG)
    SetOriginatorAddress (DeserializeOriginatorAddress (start));

  if (flags & MHAS_HOP_LIMIT)
    SetHopLimit (start.ReadU8 ());

  if (flags & MHAS_HOP_COUNT)
    SetHopCount (start.ReadU8 ());

  if (flags & MHAS_SEQ_NUM)
    SetSequenceNumber (start.ReadNtohU16 ());

  m_tlvList.Deserialize (start);

  if (size > 0)
  {
    while (start.GetDistanceFrom(front) < size)
    {
      Ptr<AddressBlock> newab = AddressBlockDeserialize (start);
      //AddressBlock * newab = AddressBlockDeserialize (start);
      //Ptr<AddressBlock> * newptr = new Ptr<AddressBlock> (newab);
      AddressBlockPushBack (newab);
    }
  }
}

void
Message::Print (std::ostream &os) const
{
  Print (os, 0);
}

void
Message::Print (std::ostream &os, int level) const
{
  std::string prefix = "";
  for (int i = 0; i < level; i++)
    prefix.append ("\t");

  os << prefix << "Message {" << std::endl;

  os << prefix << "\tmessage type = " << (int)GetType () << std::endl;
  os << prefix << "\taddress size = " << GetAddressLength () << std::endl;

  if (HasOriginatorAddress ())
  {
    os << prefix << "\toriginator address = ";
    PrintOriginatorAddress (os);
    os << std::endl;
  }

  if (HasHopLimit ())
    os << prefix << "\thop limit = " << (int)GetHopLimit () << std::endl;

  if (HasHopCount ())
    os << prefix << "\thop count = " << (int)GetHopCount () << std::endl;

  if (HasSequenceNumber ())
    os << prefix << "\tseqnum = " << GetSequenceNumber () << std::endl;

  m_tlvList.Print (os, level+1);

  for (ConstAddressBlockIterator iter = AddressBlockBegin ();
      iter != AddressBlockEnd ();
      iter++)
  {
    (*iter)->Print (os, level+1);
  }
  os << prefix << "}" << std::endl;
}

bool
Message::operator== (const Message &other) const
{
  if (GetAddressLength () != other.GetAddressLength ())
    return false;

  if (GetType () != other.GetType ())
    return false;

  if (HasOriginatorAddress () != other.HasOriginatorAddress ())
    return false;

  if (HasOriginatorAddress ())
  {
    if (GetOriginatorAddress () != other.GetOriginatorAddress ())
      return false;
  }

  if (HasHopLimit () != other.HasHopLimit ())
    return false;

  if (HasHopLimit ())
  {
    if (GetHopLimit () != other.GetHopLimit ())
      return false;
  }

  if (HasHopCount () != other.HasHopCount ())
    return false;

  if (HasHopCount ())
  {
    if (GetHopCount () != other.GetHopCount ())
      return false;
  }

  if (HasSequenceNumber () != other.HasSequenceNumber ())
    return false;

  if (HasSequenceNumber ())
  {
    if (GetSequenceNumber () != other.GetSequenceNumber ())
      return false;
  }

  if (m_tlvList != other.m_tlvList)
    return false;

  if (AddressBlockSize () != other.AddressBlockSize ())
    return false;

  ConstAddressBlockIterator tai, oai;
  for (tai = AddressBlockBegin (), oai = other.AddressBlockBegin ();
      tai != AddressBlockEnd () && oai != other.AddressBlockEnd ();
      tai++, oai++)
  {
    if (**tai != **oai)
      return false;
  }
  return true;
}

bool
Message::operator!= (const Message &other) const
{
  return !(*this == other);
}

/* End Message Class */

AddressLength
MessageIpv4::GetAddressLength (void) const
{
  return IPV4;
}

void
MessageIpv4::SerializeOriginatorAddress (Buffer::Iterator &start) const
{
  uint8_t buffer[GetAddressLength () + 1];
  Ipv4Address::ConvertFrom (GetOriginatorAddress ()).Serialize(buffer);
  start.Write (buffer, GetAddressLength () + 1);
}

Address
MessageIpv4::DeserializeOriginatorAddress (Buffer::Iterator &start) const
{
  uint8_t buffer[GetAddressLength () + 1];
  start.Read(buffer, GetAddressLength () + 1);
  return Ipv4Address::Deserialize (buffer);
}

void
MessageIpv4::PrintOriginatorAddress (std::ostream &os) const
{
  Ipv4Address::ConvertFrom (GetOriginatorAddress ()).Print (os);
}

Ptr<AddressBlock>
MessageIpv4::AddressBlockDeserialize (Buffer::Iterator &start) const
{
  Ptr<AddressBlock> newab = Create<AddressBlockIpv4> ();
  newab->Deserialize (start);
  return newab;
}

/* End MessageIpv4 Class */

AddressLength
MessageIpv6::GetAddressLength (void) const
{
  return IPV6;
}

void
MessageIpv6::SerializeOriginatorAddress (Buffer::Iterator &start) const
{
  uint8_t buffer[GetAddressLength () + 1];
  Ipv6Address::ConvertFrom (GetOriginatorAddress ()).Serialize(buffer);
  start.Write (buffer, GetAddressLength () + 1);
}

Address
MessageIpv6::DeserializeOriginatorAddress (Buffer::Iterator &start) const
{
  uint8_t buffer[GetAddressLength () + 1];
  start.Read(buffer, GetAddressLength () + 1);
  return Ipv6Address::Deserialize (buffer);
}

void
MessageIpv6::PrintOriginatorAddress (std::ostream &os) const
{
  Ipv6Address::ConvertFrom (GetOriginatorAddress ()).Print (os);
}

Ptr<AddressBlock>
MessageIpv6::AddressBlockDeserialize (Buffer::Iterator &start) const
{
  Ptr<AddressBlock> newab = Create<AddressBlockIpv6> ();
  newab->Deserialize (start);
  return newab;
}

/* End MessageIpv6 Class */

AddressBlock::AddressBlock ()
{
  m_refCount = 1;
}

/* Manipulating the address block */

AddressBlock::AddressIterator
AddressBlock::AddressBegin (void)
{
  return m_addressList.begin();
}

AddressBlock::ConstAddressIterator
AddressBlock::AddressBegin (void) const
{
  return m_addressList.begin();
}

AddressBlock::AddressIterator
AddressBlock::AddressEnd (void)
{
  return m_addressList.end();
}

AddressBlock::ConstAddressIterator
AddressBlock::AddressEnd (void) const
{
  return m_addressList.end();
}

int
AddressBlock::AddressSize (void) const
{
  return m_addressList.size();
}

bool
AddressBlock::AddressEmpty (void) const
{
  return m_addressList.empty();
}

Address
AddressBlock::AddressFront (void) const
{
  return m_addressList.front();
}

Address
AddressBlock::AddressBack (void) const
{
  return m_addressList.back();
}

void
AddressBlock::AddressPushFront (Address tlv)
{
  m_addressList.push_front(tlv);
}

void
AddressBlock::AddressPopFront (void)
{
  m_addressList.pop_front();
}

void
AddressBlock::AddressPushBack (Address tlv)
{
  m_addressList.push_back(tlv);
}

void
AddressBlock::AddressPopBack (void)
{
  m_addressList.pop_back();
}

AddressBlock::AddressIterator
AddressBlock::AddressErase (AddressBlock::AddressIterator position)
{
  return m_addressList.erase(position);
}

AddressBlock::AddressIterator
AddressBlock::AddressErase (AddressBlock::AddressIterator first,
    AddressBlock::AddressIterator last)
{
  return m_addressList.erase(first, last);
}

  void
AddressBlock::AddressClear (void)
{
  return m_addressList.clear();
}

/* Manipulating the prefix list */

AddressBlock::PrefixIterator
AddressBlock::PrefixBegin (void)
{
  return m_prefixList.begin ();
}

AddressBlock::ConstPrefixIterator
AddressBlock::PrefixBegin (void) const
{
  return m_prefixList.begin ();
}

AddressBlock::PrefixIterator
AddressBlock::PrefixEnd (void)
{
  return m_prefixList.end ();
}

AddressBlock::ConstPrefixIterator
AddressBlock::PrefixEnd (void) const
{
  return m_prefixList.end ();
}

int
AddressBlock::PrefixSize (void) const
{
  return m_prefixList.size ();
}

bool
AddressBlock::PrefixEmpty (void) const
{
  return m_prefixList.empty ();
}

uint8_t
AddressBlock::PrefixFront (void) const
{
  return m_prefixList.front ();
}

uint8_t
AddressBlock::PrefixBack (void) const
{
  return m_prefixList.back ();
}

void
AddressBlock::PrefixPushFront (uint8_t prefix)
{
  m_prefixList.push_front (prefix);
}

void
AddressBlock::PrefixPopFront (void)
{
  m_prefixList.pop_front ();
}

void
AddressBlock::PrefixPushBack (uint8_t prefix)
{
  m_prefixList.push_back (prefix);
}

void
AddressBlock::PrefixPopBack (void)
{
  m_prefixList.pop_back ();
}

AddressBlock::PrefixIterator
AddressBlock::PrefixInsert (AddressBlock::PrefixIterator position, const uint8_t value)
{
  return m_prefixList.insert (position, value);
}

AddressBlock::PrefixIterator
AddressBlock::PrefixErase (AddressBlock::PrefixIterator position)
{
  return m_prefixList.erase (position);
}

AddressBlock::PrefixIterator
AddressBlock::PrefixErase (AddressBlock::PrefixIterator first, AddressBlock::PrefixIterator last)
{
  return m_prefixList.erase (first, last);
}

void
AddressBlock::PrefixClear (void)
{
  m_prefixList.clear ();
}

/* Manipulating the TLV block */

AddressBlock::TlvIterator
AddressBlock::TlvBegin (void)
{
  return m_addressTlvList.Begin();
}

AddressBlock::ConstTlvIterator
AddressBlock::TlvBegin (void) const
{
  return m_addressTlvList.Begin();
}

AddressBlock::TlvIterator
AddressBlock::TlvEnd (void)
{
  return m_addressTlvList.End();
}

AddressBlock::ConstTlvIterator
AddressBlock::TlvEnd (void) const
{
  return m_addressTlvList.End();
}

int
AddressBlock::TlvSize (void) const
{
  return m_addressTlvList.Size();
}

bool
AddressBlock::TlvEmpty (void) const
{
  return m_addressTlvList.Empty();
}

Ptr<AddressTlv>
AddressBlock::TlvFront (void)
{
  return m_addressTlvList.Front();
}

const Ptr<AddressTlv>
AddressBlock::TlvFront (void) const
{
  return m_addressTlvList.Front();
}

Ptr<AddressTlv>
AddressBlock::TlvBack (void)
{
  return m_addressTlvList.Back();
}

const Ptr<AddressTlv>
AddressBlock::TlvBack (void) const
{
  return m_addressTlvList.Back();
}

void
AddressBlock::TlvPushFront (Ptr<AddressTlv> tlv)
{
  m_addressTlvList.PushFront(tlv);
}

void
AddressBlock::TlvPopFront (void)
{
  m_addressTlvList.PopFront();
}

void
AddressBlock::TlvPushBack (Ptr<AddressTlv> tlv)
{
  m_addressTlvList.PushBack(tlv);
}

void
AddressBlock::TlvPopBack (void)
{
  m_addressTlvList.PopBack();
}

AddressBlock::TlvIterator
AddressBlock::TlvErase (AddressBlock::TlvIterator position)
{
  return m_addressTlvList.Erase(position);
}

AddressBlock::TlvIterator
AddressBlock::TlvErase (AddressBlock::TlvIterator first,
    AddressBlock::TlvIterator last)
{
  return m_addressTlvList.Erase(first, last);
}

void
AddressBlock::TlvClear (void)
{
  return m_addressTlvList.Clear();
}

void
AddressBlock::Ref (void) const
{
  m_refCount++;
}

void
AddressBlock::Unref (void) const
{
  m_refCount--;
  if (m_refCount == 0)
  {
    delete this;
  }
}

uint32_t
AddressBlock::GetSerializedSize (void) const
{
  /* num-addr + flags */
  uint32_t size = 2;

  if (AddressSize () == 1)
  {
    size += GetAddressLength () + PrefixSize();
  }
  else if (AddressSize () > 0)
  {
    uint8_t head[GetAddressLength ()];
    uint8_t headlen = 0;
    uint8_t tail[GetAddressLength ()];
    uint8_t taillen = 0;

    GetHeadTail (head, headlen, tail, taillen);

    if (headlen > 0)
      size += 1 + headlen;

    if (taillen > 0)
    {
      size++;
      if (!HasZeroTail (tail, taillen))
        size += taillen;
    }

    /* mid size */
    size += (GetAddressLength () - headlen - taillen) * AddressSize ();

    size += PrefixSize ();
  }

  size += m_addressTlvList.GetSerializedSize ();

  return size;
}

void
AddressBlock::Serialize (Buffer::Iterator &start) const
{
  start.WriteU8 (AddressSize ());

  if (AddressSize () == 1) {
    start.WriteU8 (0);

    uint8_t buf[GetAddressLength ()];
    SerializeAddress (buf, AddressBegin ());
    start.Write (buf, GetAddressLength ());

    if (PrefixSize () == 1)
      start.WriteU8 (PrefixFront ());
  }
  else if (AddressSize () > 0)
  {
    Buffer::Iterator bufref = start;
    uint8_t flags = 0;
    start.Next ();

    uint8_t head[GetAddressLength ()];
    uint8_t tail[GetAddressLength ()];
    uint8_t headlen = 0;
    uint8_t taillen = 0;

    GetHeadTail (head, headlen, tail, taillen);

    if (headlen > 0)
    {
      flags |= AHAS_HEAD;
      start.WriteU8 (headlen);
      start.Write (head, headlen);
    }

    if (taillen > 0)
    {
      start.WriteU8 (taillen);

      if (HasZeroTail (tail, taillen))
      {
        flags |= AHAS_ZERO_TAIL;
      }
      else
      {
        flags |= AHAS_FULL_TAIL;
        start.Write (tail, taillen);
      }
    }

    if (headlen + taillen < GetAddressLength ())
    {
      uint8_t mid[GetAddressLength ()];
      for (AddressBlock::ConstAddressIterator iter = AddressBegin ();
          iter != AddressEnd ();
          iter++)
      {
        SerializeAddress (mid, iter);
        start.Write (mid + headlen, GetAddressLength () - headlen - taillen);
      }
    }

    flags |= GetPrefixFlags ();
    bufref.WriteU8 (flags);

    for (ConstPrefixIterator iter = PrefixBegin ();
        iter != PrefixEnd ();
        iter++)
    {
      start.WriteU8 (*iter);
    }
  }
  
  m_addressTlvList.Serialize (start);
}

void
AddressBlock::Deserialize (Buffer::Iterator &start)
{
  uint8_t numaddr = start.ReadU8 ();
  uint8_t flags = start.ReadU8 ();

  if (numaddr > 0)
  {
    uint8_t headlen = 0;
    uint8_t taillen = 0;
    uint8_t addrtmp[GetAddressLength ()];
    memset(addrtmp, 0, GetAddressLength ());

    if (flags & AHAS_HEAD)
    {
      headlen = start.ReadU8 ();
      start.Read (addrtmp, headlen);
    }

    if ((flags & AHAS_FULL_TAIL) ^ (flags & AHAS_ZERO_TAIL))
    {
      taillen = start.ReadU8 ();
      
      if (flags & AHAS_FULL_TAIL)
      {
        start.Read (addrtmp + GetAddressLength () - taillen, taillen);
      }
    }

    for (int i = 0; i < numaddr; i++)
    {
      start.Read (addrtmp + headlen, GetAddressLength () - headlen - taillen);
      AddressPushBack (DeserializeAddress (addrtmp));
    }

    if (flags & AHAS_SINGLE_PRE_LEN)
    {
      PrefixPushBack (start.ReadU8 ());
    }
    else if (flags & AHAS_MULTI_PRE_LEN)
    {
      for (int i = 0; i < numaddr; i++)
      {
        PrefixPushBack (start.ReadU8 ());
      }
    }
  }

  m_addressTlvList.Deserialize (start);
}

void
AddressBlock::Print (std::ostream &os) const
{
  Print (os, 0);
}

void
AddressBlock::Print (std::ostream &os, int level) const
{
  std::string prefix = "";
  for (int i = 0; i < level; i++)
    prefix.append ("\t");

  os << prefix << "AddressBlock {" << std::endl;
  os << prefix << "\taddresses = " << std::endl;
  for (ConstAddressIterator iter = AddressBegin ();
      iter != AddressEnd ();
      iter++)
  {
    os << prefix << "\t\t";
    PrintAddress(os, iter);
    os << std::endl;
  }

  os << prefix << "\tprefixes = " << std::endl;
  for (ConstPrefixIterator iter = PrefixBegin ();
      iter != PrefixEnd ();
      iter++)
  {
    os << prefix << "\t\t" << (int)(*iter) << std::endl;
  }

  m_addressTlvList.Print (os, level+1);
}

bool
AddressBlock::operator== (const AddressBlock &other) const
{
  if (AddressSize () != other.AddressSize ())
    return false;

  ConstAddressIterator tai, oai;
  for (tai = AddressBegin (), oai = other.AddressBegin ();
      tai != AddressEnd () && oai != other.AddressEnd ();
      tai++, oai++)
  {
    if (*tai != *oai)
      return false;
  }

  if (PrefixSize () != other.PrefixSize ())
    return false;

  ConstPrefixIterator tpi, opi;
  for (tpi = PrefixBegin (), opi = other.PrefixBegin ();
      tpi != PrefixEnd () && opi != other.PrefixEnd ();
      tpi++, opi++)
  {
    if (*tpi != *opi)
      return false;
  }

  if (m_addressTlvList != other.m_addressTlvList)
    return false;

  return true;
}

bool
AddressBlock::operator!= (const AddressBlock &other) const
{
  return !(*this == other);
}

uint8_t
AddressBlock::GetPrefixFlags (void) const
{
  switch (PrefixSize ())
  {
    case 0:
      return 0;
      break;
    case 1:
      return AHAS_SINGLE_PRE_LEN;
      break;
    default:
      return AHAS_MULTI_PRE_LEN;
      break;
  }
}

void
AddressBlock::GetHeadTail (uint8_t *head, uint8_t &headlen,
    uint8_t *tail, uint8_t &taillen) const
{
  headlen = GetAddressLength ();
  taillen = headlen;

  /* Temporary automatic buffers to store serialized addresses */
  uint8_t * buflast = new uint8_t[GetAddressLength ()];
  uint8_t * bufcur = new uint8_t[GetAddressLength ()];
  uint8_t * tmp;

  SerializeAddress (buflast, AddressBegin ());

  /* Skip the first item */
  for (AddressBlock::ConstAddressIterator iter = AddressBegin ()++;
      iter != AddressEnd ();
      iter++)
  {
    SerializeAddress (bufcur, iter);

    int i;
    for (i = 0; i < headlen; i++)
    {
      if (buflast[i] != bufcur[i])
      {
        headlen = i;
        break;
      }
    }

    /* If headlen == fulllen - 1, then tail is 0 */
    if (headlen <= GetAddressLength () - 1)
    {
      for (i = GetAddressLength () - 1;
          GetAddressLength () - 1 - i <= taillen && i > headlen;
          i--)
      {
        if (buflast[i] != bufcur[i])
          break;
      }
      taillen = GetAddressLength () - 1 - i;
    }
    else if (headlen == 0)
    {
      taillen = 0;
      break;
    }

    tmp = buflast;
    buflast = bufcur;
    bufcur = tmp;
  }

  memcpy(head, bufcur, headlen);
  memcpy(tail, bufcur + (GetAddressLength () - taillen), taillen);

  delete[] buflast;
  delete[] bufcur;
}

bool
AddressBlock::HasZeroTail (const uint8_t *tail, uint8_t taillen) const
{
  int i;
  for (i = 0; i < taillen; i++)
  {
    if (tail[i] != 0)
      break;
  }
  return i == taillen;
}

/* End AddressBlock Class */

uint8_t
AddressBlockIpv4::GetAddressLength (void) const
{
  return 4;
}

void
AddressBlockIpv4::SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const
{
  Ipv4Address::ConvertFrom (*iter).Serialize (buffer);
}

Address
AddressBlockIpv4::DeserializeAddress (uint8_t *buffer) const
{
  return Ipv4Address::Deserialize (buffer);
}

void
AddressBlockIpv4::PrintAddress (std::ostream &os, ConstAddressIterator iter) const
{
  Ipv4Address::ConvertFrom (*iter).Print (os);
}

/* End AddressBlockIpv4 Class */

uint8_t
AddressBlockIpv6::GetAddressLength (void) const
{
  return 16;
}

void
AddressBlockIpv6::SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const
{
  Ipv6Address::ConvertFrom (*iter).Serialize (buffer);
}

Address
AddressBlockIpv6::DeserializeAddress (uint8_t *buffer) const
{
  return Ipv6Address::Deserialize (buffer);
}

void
AddressBlockIpv6::PrintAddress (std::ostream &os, ConstAddressIterator iter) const
{
  Ipv6Address::ConvertFrom (*iter).Print (os);
}

/* End AddressBlockIpv6 Class */

Tlv::Tlv (void)
{
  m_refCount = 1;
  m_hasTypeExt = false;
  m_hasIndexStart = false;
  m_hasIndexStop = false;
  m_isMultivalue = false;
  m_hasValue = false;
}

void
Tlv::SetType (uint8_t type)
{
  m_type = type;
}

uint8_t
Tlv::GetType (void) const
{
  return m_type;
}

void
Tlv::SetTypeExt (uint8_t typeExt)
{
  m_typeExt = typeExt;
  m_hasTypeExt = true;
}

uint8_t
Tlv::GetTypeExt (void) const throw (PacketBBError)
{
  if (!HasTypeExt())
  {
    throw PacketBBError ("TLV has no type extension.");
  }
  return m_typeExt;
}

bool
Tlv::HasTypeExt (void) const
{
  return m_hasTypeExt;
}

void
Tlv::SetIndexStart (uint8_t index)
{
  m_indexStart = index;
  m_hasIndexStart = true;
}

uint8_t
Tlv::GetIndexStart (void) const throw (PacketBBError)
{
  if (!HasIndexStart())
  {
    throw PacketBBError ("TLV has no start index.");
  }
  return m_indexStart;
}

bool
Tlv::HasIndexStart (void) const
{
  return m_hasIndexStart;
}

void
Tlv::SetIndexStop (uint8_t index)
{
  m_indexStop = index;
  m_hasIndexStop = true;
}

uint8_t
Tlv::GetIndexStop (void) const throw (PacketBBError)
{
  if (!HasIndexStop())
  {
    throw PacketBBError ("TLV has no stop index.");
  }
  return m_indexStop;
}

bool
Tlv::HasIndexStop (void) const
{
  return m_hasIndexStop;
}

void
Tlv::SetMultivalue (bool isMultivalue)
{
  m_isMultivalue = isMultivalue;
}

bool
Tlv::IsMultivalue (void) const
{
  return m_isMultivalue;
}

void
Tlv::SetValue (Buffer start)
{
  m_hasValue = true;
  m_value = start;
}

void
Tlv::SetValue (const uint8_t * buffer, uint32_t size)
{
  Buffer value;
  value.AddAtStart (size);
  value.Begin ().Write (buffer, size);
  SetValue (value);
}

Buffer
Tlv::GetValue (void) const throw (PacketBBError)
{
  if (!HasValue ())
  {
    throw PacketBBError ("TLV has no value.");
  }
  return m_value;
}

bool
Tlv::HasValue (void) const
{
  return m_hasValue;
}

void
Tlv::Ref (void) const
{
  m_refCount++;
}

void
Tlv::Unref (void) const
{
  m_refCount--;
  if (m_refCount == 0)
  {
    delete this;
  }
}

uint32_t
Tlv::GetSerializedSize (void) const
{
  /* type + flags */
  uint32_t size = 2;

  if (HasTypeExt ()) {
    size++;
  }

  if (HasIndexStart ()) {
    size++;
  }

  if (HasIndexStop ()) {
    size++;
  }

  if (HasValue ())
  {
    if (GetValue ().GetSize () > 255) {
      size += 2;
    } else {
      size++;
    }
    size += GetValue ().GetSize ();
  }

  return size;
}

void
Tlv::Serialize (Buffer::Iterator &start) const
{
  start.WriteU8 (GetType ());

  Buffer::Iterator bufref = start;
  uint8_t flags = 0;
  start.Next();

  if (HasTypeExt())
  {
    flags |= THAS_TYPE_EXT;
    start.WriteU8 (GetTypeExt ());
  }

  if (HasIndexStart ())
  {
    start.WriteU8 (GetIndexStart ());

    if (HasIndexStop ())
    {
      flags |= THAS_MULTI_INDEX;
      start.WriteU8 (GetIndexStop ());
    } 
    else
    {
      flags |= THAS_SINGLE_INDEX;
    }
  }

  if (HasValue ()) {
    flags |= THAS_VALUE;

    uint32_t size = GetValue ().GetSize ();
    if (size > 255)
    {
      flags |= THAS_EXT_LEN;
      start.WriteHtonU16 (size);
    }
    else
    {
      start.WriteU8 (size);
    }

    if (IsMultivalue ())
      flags |= TIS_MULTIVALUE;

    start.Write(GetValue ().Begin (), GetValue ().End ());
  }

  bufref.WriteU8 (flags);
}

void
Tlv::Deserialize (Buffer::Iterator &start)
{
  SetType (start.ReadU8 ());

  uint8_t flags = start.ReadU8 ();

  if (flags & THAS_TYPE_EXT)
    SetTypeExt (start.ReadU8 ());

  if (flags & THAS_MULTI_INDEX)
  {
    SetIndexStart (start.ReadU8 ());
    SetIndexStop (start.ReadU8 ());
  }
  else if (flags & THAS_SINGLE_INDEX)
  {
    SetIndexStart (start.ReadU8 ());
  }

  if (flags & THAS_VALUE)
  {
    uint16_t len = 0;

    if (flags & THAS_EXT_LEN)
      len = start.ReadNtohU16 ();
    else
      len = start.ReadU8 ();

    m_value.AddAtStart (len);

    Buffer::Iterator valueStart = start;
    start.Next (len);
    m_value.Begin ().Write (valueStart, start);
    m_hasValue = true;
  }
}

void
Tlv::Print (std::ostream &os) const
{
  Print (os, 0);
}

void
Tlv::Print (std::ostream &os, int level) const
{
  std::string prefix = "";
  for (int i = 0; i < level; i++)
    prefix.append ("\t");

  os << prefix << "Tlv {" << std::endl;
  os << prefix << "\ttype = " << (int)GetType () << std::endl;

  if (HasTypeExt ())
    os << prefix << "\ttypeext = " << (int)GetTypeExt () << std::endl;

  if (HasIndexStart ())
    os << prefix << "\tindexStart = " << (int)GetIndexStart () << std::endl;

  if (HasIndexStop ())
    os << prefix << "\tindexStop = " << (int)GetIndexStop () << std::endl;

  os << prefix << "\tisMultivalue = " << IsMultivalue () << std::endl;

  if (HasValue ())
    os << prefix << "\thas value; size = " << GetValue (). GetSize () << std::endl;

  os << prefix << "}" << std::endl;
}

bool
Tlv::operator== (const Tlv &other) const
{
  if (GetType () != other.GetType ())
    return false;

  if (HasTypeExt () != other.HasTypeExt ())
    return false;

  if (HasTypeExt ())
  {
    if (GetTypeExt () != other.GetTypeExt ())
      return false;
  }

  if (HasValue () != other.HasValue ())
    return false;

  if (HasValue ())
  {
    Buffer tv = GetValue ();
    Buffer ov = other.GetValue ();
    if (tv.GetSize () != ov.GetSize ())
      return false;

    /* The docs say I probably shouldn't use Buffer::PeekData, but I think it
     * is justified in this case. */
    if (memcmp (tv.PeekData (), ov.PeekData (), tv.GetSize ()) != 0)
      return false;
  }
  return true;
}

bool
Tlv::operator!= (const Tlv &other) const
{
  return !(*this == other);
}

/* End Tlv Class */

void 
AddressTlv::SetIndexStart (uint8_t index)
{
  Tlv::SetIndexStart (index);
}

uint8_t
AddressTlv::GetIndexStart (void) const throw (PacketBBError)
{
  return Tlv::GetIndexStart ();
}

bool
AddressTlv::HasIndexStart (void) const
{
  return Tlv::HasIndexStart ();
}

void 
AddressTlv::SetIndexStop (uint8_t index)
{
  Tlv::SetIndexStop (index);
}

uint8_t
AddressTlv::GetIndexStop (void) const throw (PacketBBError)
{
  return Tlv::GetIndexStop ();
}

bool
AddressTlv::HasIndexStop (void) const
{
  return Tlv::HasIndexStop ();
}

void
AddressTlv::SetMultivalue (bool isMultivalue)
{
  Tlv::SetMultivalue (isMultivalue);
}

bool
AddressTlv::IsMultivalue (void) const
{
  return Tlv::IsMultivalue ();
}

} /* namespace pbb */

} /* namespace ns3 */
