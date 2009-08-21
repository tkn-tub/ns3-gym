/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* vim: set ts=2 sw=2 sta expandtab ai si cin: */
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
/* These classes implement RFC 5444 - The Generalized Mobile Ad Hoc Network
 * (MANET) Packet/Message Format
 * See: http://tools.ietf.org/html/rfc5444 for details */

#ifndef PACKETBB_H
#define PACKETBB_H

#include <stdexcept>
#include <list>

#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/header.h"
#include "ns3/buffer.h"

namespace ns3 {

namespace pbb {

/* Forward declare objects */
class PacketBB;
class Message;
class AddressBlock;
class TlvBlock;
class AddressTlvBlock;
class Tlv;
class AddressTlv;

enum AddressLength {
  IPV4 = 3,
  IPV6 = 15,
};

class PacketBBError : public std::runtime_error {
public:
  PacketBBError(const std::string &arg) : std::runtime_error(arg) {}
};

class TlvBlock
{
public:
  typedef std::list< Ptr<Tlv> >::iterator Iterator;
  typedef std::list< Ptr<Tlv> >::const_iterator ConstIterator;

  Iterator Begin (void);
  ConstIterator Begin (void) const;
  Iterator End (void);
  ConstIterator End (void) const;

  int Size (void) const;
  bool Empty (void) const;

  Ptr<Tlv> Front (void) const;
  Ptr<Tlv> Back (void) const;

  void PushFront (Ptr<Tlv> tlv);
  void PopFront (void);

  void PushBack (Ptr<Tlv> tlv);
  void PopBack (void);

  Iterator Insert (Iterator position, const Ptr<Tlv> tlv);

  Iterator Erase (Iterator position);
  Iterator Erase (Iterator first, Iterator last);

  void Clear (void);

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const TlvBlock &other) const;
  bool operator!= (const TlvBlock &other) const;

private:
  std::list< Ptr<Tlv> > m_tlvList;
};

class AddressTlvBlock
{
public:
  typedef std::list< Ptr<AddressTlv> >::iterator Iterator;
  typedef std::list< Ptr<AddressTlv> >::const_iterator ConstIterator;

  Iterator Begin (void);
  ConstIterator Begin (void) const;
  Iterator End (void);
  ConstIterator End (void) const;

  int Size (void) const;
  bool Empty (void) const;

  Ptr<AddressTlv> Front (void) const;
  Ptr<AddressTlv> Back (void) const;

  void PushFront (Ptr<AddressTlv> tlv);
  void PopFront (void);

  void PushBack (Ptr<AddressTlv> tlv);
  void PopBack (void);

  Iterator Insert (Iterator position, const Ptr<AddressTlv> tlv);

  Iterator Erase (Iterator position);
  Iterator Erase (Iterator first, Iterator last);

  void Clear (void);

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const AddressTlvBlock &other) const;
  bool operator!= (const AddressTlvBlock &other) const;

private:
  std::list< Ptr<AddressTlv> > m_tlvList;
};

/** Top level PacketBB packet object */
class PacketBB : public Header
{
public:
  typedef std::list< Ptr<Tlv> >::iterator TlvIterator;
  typedef std::list< Ptr<Tlv> >::const_iterator ConstTlvIterator;
  typedef std::list< Ptr<Message> >::iterator MessageIterator;
  typedef std::list< Ptr<Message> >::const_iterator ConstMessageIterator;

  PacketBB (void);

  uint8_t GetVersion (void) const;

  void SetSequenceNumber (uint16_t number);
  /* Calling this while HasSequenceNumber is False is undefined, make sure you
   * check first! */
  uint16_t GetSequenceNumber (void) const;
  bool HasSequenceNumber (void) const;

  /* Manipulating Packet TLVs */

  TlvIterator TlvBegin (void);
  ConstTlvIterator TlvBegin (void) const;
  TlvIterator TlvEnd (void);
  ConstTlvIterator TlvEnd (void) const;

  int TlvSize (void) const;
  bool TlvEmpty (void) const;

  Ptr<Tlv> TlvFront (void);
  const Ptr<Tlv> TlvFront (void) const;
  Ptr<Tlv> TlvBack (void);
  const Ptr<Tlv> TlvBack (void) const;

  void TlvPushFront (Ptr<Tlv>);
  void TlvPopFront (void);
  void TlvPushBack (Ptr<Tlv>);
  void TlvPopBack (void);

  TlvIterator Erase (TlvIterator position);
  TlvIterator Erase (TlvIterator first, TlvIterator last);
  void TlvClear (void);

  /* Manipulating Packet Messages */

  MessageIterator MessageBegin (void);
  ConstMessageIterator MessageBegin (void) const;
  MessageIterator MessageEnd (void);
  ConstMessageIterator MessageEnd (void) const;

  int MessageSize (void) const;
  bool MessageEmpty (void) const;

  Ptr<Message> MessageFront (void);
  const Ptr<Message> MessageFront (void) const;
  Ptr<Message> MessageBack (void);
  const Ptr<Message> MessageBack (void) const;

  void MessagePushFront (Ptr<Message> message);
  void MessagePopFront (void);
  void MessagePushBack (Ptr<Message> message);
  void MessagePopBack (void);

  MessageIterator Erase (MessageIterator position);
  MessageIterator Erase (MessageIterator first, MessageIterator last);
  void MessageClear (void);

  /* Smart pointer methods */
  void Ref (void) const;
  void Unref (void) const;

  /* Methods implemented by all headers */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  /* If this returns a number smaller than the total number of bytes in the
   * buffer, there was an error. */
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  bool operator== (const PacketBB &other) const;
  bool operator!= (const PacketBB &other) const;

protected:
  void SerializePacketTlv (Buffer::Iterator &start) const;

private:
  TlvBlock m_tlvList;
  std::list< Ptr<Message> > m_messageList;

  uint8_t m_version;

  bool m_hasseqnum;
  uint16_t m_seqnum;

  mutable uint32_t m_refCount;
};

class Message
{
public:
  typedef std::list< Ptr<Tlv> >::iterator TlvIterator;
  typedef std::list< Ptr<Tlv> >::const_iterator ConstTlvIterator;
  typedef std::list< Ptr<AddressBlock> >::iterator AddressBlockIterator;
  typedef std::list< Ptr<AddressBlock> >::const_iterator ConstAddressBlockIterator;

  Message (void);

  void SetType (uint8_t type);
  uint8_t GetType (void) const;

  void SetOriginatorAddress (Address address);
  /* Calling this while HasOriginatorAddress is False is undefined, make sure
   * you check first! */
  Address GetOriginatorAddress (void) const;
  bool HasOriginatorAddress (void) const;

  void SetHopLimit (uint8_t hoplimit);
  /* Calling this while HasHopLimit is False is undefined, make sure
   * you check first! */
  uint8_t GetHopLimit (void) const;
  bool HasHopLimit (void) const;

  void SetHopCount (uint8_t hopcount);
  /* Calling this while HasHopCount is False is undefined, make sure
   * you check first! */
  uint8_t GetHopCount (void) const;
  bool HasHopCount (void) const;

  void SetSequenceNumber (uint16_t seqnum);
  /* Calling this while HasSequenceNumber is False is undefined, make sure
   * you check first! */
  uint16_t GetSequenceNumber (void) const;
  bool HasSequenceNumber (void) const;

  /* Manipulating Message TLVs */

  TlvIterator TlvBegin ();
  ConstTlvIterator TlvBegin () const;
  TlvIterator TlvEnd ();
  ConstTlvIterator TlvEnd () const;

  int TlvSize (void) const;
  bool TlvEmpty (void) const;

  Ptr<Tlv> TlvFront (void);
  const Ptr<Tlv> TlvFront (void) const;
  Ptr<Tlv> TlvBack (void);
  const Ptr<Tlv> TlvBack (void) const;

  void TlvPushFront (Ptr<Tlv> tlv);
  void TlvPopFront (void);
  void TlvPushBack (Ptr<Tlv> tlv);
  void TlvPopBack (void);

  TlvIterator TlvErase (TlvIterator position);
  TlvIterator TlvErase (TlvIterator first, TlvIterator last);
  void TlvClear (void);

  /* Manipulating Address Block and Address TLV pairs */

  AddressBlockIterator AddressBlockBegin ();
  ConstAddressBlockIterator AddressBlockBegin () const;
  AddressBlockIterator AddressBlockEnd ();
  ConstAddressBlockIterator AddressBlockEnd () const;

  int AddressBlockSize (void) const;
  bool AddressBlockEmpty (void) const;

  Ptr<AddressBlock> AddressBlockFront (void);
  const Ptr<AddressBlock> AddressBlockFront (void) const;
  Ptr<AddressBlock> AddressBlockBack (void);
  const Ptr<AddressBlock> AddressBlockBack (void) const;

  void AddressBlockPushFront (Ptr<AddressBlock> block);
  void AddressBlockPopFront (void);
  void AddressBlockPushBack (Ptr<AddressBlock> block);
  void AddressBlockPopBack (void);

  AddressBlockIterator AddressBlockErase (AddressBlockIterator position);
  AddressBlockIterator AddressBlockErase (AddressBlockIterator first,
      AddressBlockIterator last);
  void AddressBlockClear (void);

  /* Smart pointer methods */
  void Ref (void) const;
  void Unref (void) const;

  /* Returns 0 on error */
  static Ptr<Message> DeserializeMessage (Buffer::Iterator &start);
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const Message &other) const;
  bool operator!= (const Message &other) const;

protected:
  /* Message size in bytes - 1.
   *
   * IPv4 = 4 - 1 = 3, IPv6 = 16 - 1 = 15
   */
  virtual AddressLength GetAddressLength (void) const = 0;

  virtual void SerializeOriginatorAddress (Buffer::Iterator &start) const = 0;
  virtual Address DeserializeOriginatorAddress (Buffer::Iterator &start) const = 0;
  virtual void PrintOriginatorAddress (std::ostream &os) const = 0;

  virtual Ptr<AddressBlock> AddressBlockDeserialize (Buffer::Iterator &start) const = 0;

private:
  TlvBlock m_tlvList;
  std::list< Ptr<AddressBlock> > m_addressBlockList;

  uint8_t m_type;
  AddressLength m_addrSize;

  bool m_hasOriginatorAddress;
  Address m_originatorAddress;

  bool m_hasHopLimit;
  uint8_t m_hopLimit;

  bool m_hasHopCount;
  uint8_t m_hopCount;

  bool m_hasSequenceNumber;
  uint16_t m_sequenceNumber;

  mutable uint32_t m_refCount;
};

class MessageIpv4 : public Message {
protected:
  virtual AddressLength GetAddressLength (void) const;

  virtual void SerializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual Address DeserializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual void PrintOriginatorAddress (std::ostream &os) const;

  virtual Ptr<AddressBlock> AddressBlockDeserialize (Buffer::Iterator &start) const;
};

class MessageIpv6 : public Message {
protected:
  virtual AddressLength GetAddressLength (void) const;

  virtual void SerializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual Address DeserializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual void PrintOriginatorAddress (std::ostream &os) const;

  virtual Ptr<AddressBlock> AddressBlockDeserialize (Buffer::Iterator &start) const;
};

/** This combines address blocks with their associated TLVs */
class AddressBlock
{
public:
  typedef std::list< Address >::iterator AddressIterator;
  typedef std::list< Address >::const_iterator ConstAddressIterator;

  typedef std::list<uint8_t>::iterator PrefixIterator;
  typedef std::list<uint8_t>::const_iterator ConstPrefixIterator;

  typedef AddressTlvBlock::Iterator TlvIterator;
  typedef AddressTlvBlock::ConstIterator ConstTlvIterator;

  AddressBlock ();

  /* Manipulating the address block */

  AddressIterator AddressBegin (void);
  ConstAddressIterator AddressBegin (void) const;
  AddressIterator AddressEnd (void);
  ConstAddressIterator AddressEnd (void) const;

  int AddressSize (void) const;
  bool AddressEmpty (void) const;

  Address AddressFront (void) const;
  Address AddressBack (void) const;

  void AddressPushFront (Address address);
  void AddressPopFront (void);

  void AddressPushBack (Address address);
  void AddressPopBack (void);

  AddressIterator AddressInsert (AddressIterator position,
      const Address value);

  AddressIterator AddressErase (AddressIterator position);
  AddressIterator AddressErase (AddressIterator first, AddressIterator last);

  void AddressClear (void);

  /* Prefix methods */
  PrefixIterator PrefixBegin (void);
  ConstPrefixIterator PrefixBegin (void) const;
  PrefixIterator PrefixEnd (void);
  ConstPrefixIterator PrefixEnd (void) const;

  int PrefixSize (void) const;
  bool PrefixEmpty (void) const;

  uint8_t PrefixFront (void) const;
  uint8_t PrefixBack (void) const;

  void PrefixPushFront (uint8_t prefix);
  void PrefixPopFront (void);

  void PrefixPushBack (uint8_t prefix);
  void PrefixPopBack (void);

  PrefixIterator PrefixInsert (PrefixIterator position, const uint8_t value);

  PrefixIterator PrefixErase (PrefixIterator position);
  PrefixIterator PrefixErase (PrefixIterator first, PrefixIterator last);

  void PrefixClear (void);

  /* Manipulating the TLV block */
  TlvIterator TlvBegin (void);
  ConstTlvIterator TlvBegin (void) const;
  TlvIterator TlvEnd (void);
  ConstTlvIterator TlvEnd (void) const;

  int TlvSize (void) const;
  bool TlvEmpty (void) const;

  Ptr<AddressTlv> TlvFront (void);
  const Ptr<AddressTlv> TlvFront (void) const;
  Ptr<AddressTlv> TlvBack (void);
  const Ptr<AddressTlv> TlvBack (void) const;

  void TlvPushFront (Ptr<AddressTlv> address);
  void TlvPopFront (void);

  void TlvPushBack (Ptr<AddressTlv> address);
  void TlvPopBack (void);

  TlvIterator TlvInsert (TlvIterator position, const Ptr<Tlv> value);

  TlvIterator TlvErase (TlvIterator position);
  TlvIterator TlvErase (TlvIterator first, TlvIterator last);

  void TlvClear (void);

  /* Smart pointer methods */
  void Ref (void) const;
  void Unref (void) const;

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const AddressBlock &other) const;
  bool operator!= (const AddressBlock &other) const;

protected:
  virtual uint8_t GetAddressLength (void) const = 0;

  virtual void SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const = 0;
  virtual Address DeserializeAddress (uint8_t *buffer) const = 0;
  virtual void PrintAddress (std::ostream &os, ConstAddressIterator iter) const = 0;

private:
  uint8_t GetPrefixFlags (void) const;
  void GetHeadTail (uint8_t *head, uint8_t &headlen,
      uint8_t *tail, uint8_t &taillen) const;
  bool HasZeroTail (const uint8_t *tail, uint8_t taillen) const;

  std::list<Address> m_addressList;
  std::list<uint8_t> m_prefixList;
  AddressTlvBlock m_addressTlvList;

  mutable uint32_t m_refCount;
};

class AddressBlockIpv4 : public AddressBlock
{
protected:
  virtual uint8_t GetAddressLength (void) const;

  virtual void SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const;
  virtual Address DeserializeAddress (uint8_t *buffer) const;
  virtual void PrintAddress (std::ostream &os, ConstAddressIterator iter) const;
};

class AddressBlockIpv6 : public AddressBlock
{
protected:
  virtual uint8_t GetAddressLength (void) const;

  virtual void SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const;
  virtual Address DeserializeAddress (uint8_t *buffer) const;
  virtual void PrintAddress (std::ostream &os, ConstAddressIterator iter) const;
};

/** A packet or message TLV */
class Tlv
{
public:
  Tlv (void);

  void SetType (uint8_t type);
  uint8_t GetType (void) const;

  void SetTypeExt (uint8_t type);
  /* Calling this while HasTypeExt is False is undefined, make sure
   * you check first! */
  uint8_t GetTypeExt (void) const;
  bool HasTypeExt (void) const;

  void SetValue (Buffer start);
  void SetValue (const uint8_t * buffer, uint32_t size);
  /* Calling this while HasValue is False is undefined, make sure
   * you check first! */
  Buffer GetValue (void) const;
  bool HasValue (void) const;

  /* Smart pointer methods */
  void Ref (void) const;
  void Unref (void) const;

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const Tlv &other) const;
  bool operator!= (const Tlv &other) const;

protected:
  void SetIndexStart (uint8_t index);
  uint8_t GetIndexStart (void) const;
  bool HasIndexStart (void) const;

  void SetIndexStop (uint8_t index);
  uint8_t GetIndexStop (void) const;
  bool HasIndexStop (void) const;

  void SetMultivalue (bool isMultivalue);
  bool IsMultivalue (void) const;

private:
  uint8_t m_type;

  bool m_hasTypeExt;
  uint8_t m_typeExt;

  bool m_hasIndexStart;
  uint8_t m_indexStart;

  bool m_hasIndexStop;
  uint8_t m_indexStop;

  bool m_isMultivalue;
  bool m_hasValue;
  Buffer m_value;

  mutable uint32_t m_refCount;
};

class AddressTlv : public Tlv
{
public:
  void SetIndexStart (uint8_t index);
  /* Calling this while HasIndexStart is False is undefined, make sure
   * you check first! */
  uint8_t GetIndexStart (void) const;
  bool HasIndexStart (void) const;

  void SetIndexStop (uint8_t index);
  /* Calling this while HasIndexStop is False is undefined, make sure
   * you check first! */
  uint8_t GetIndexStop (void) const;
  bool HasIndexStop (void) const;

  void SetMultivalue (bool isMultivalue);
  bool IsMultivalue (void) const;
};

} /* namespace pbb */

} /* namespace ns3 */

#endif /* PACKETBB_H */
