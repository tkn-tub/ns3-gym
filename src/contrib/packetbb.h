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
 * (MANET) Packet/PbbMessage Format
 * See: http://tools.ietf.org/html/rfc5444 for details */

#ifndef PACKETBB_H
#define PACKETBB_H

#include <list>

#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/header.h"
#include "ns3/buffer.h"

namespace ns3 {

/* Forward declare objects */
class PbbPacket;
class PbbMessage;
class PbbAddressBlock;
class PbbTlvBlock;
class PbbAddressTlvBlock;
class PbbTlv;
class PbbAddressTlv;

enum AddressLength {
  IPV4 = 3,
  IPV6 = 15,
};

/**
 * \brief A block of Packet or PbbMessage TLVs.
 *
 * Acts similar to a C++ STL container.  Should not be used for Address TLVs.
 */
class PbbTlvBlock
{
public:
  typedef std::list< Ptr<PbbTlv> >::iterator Iterator;
  typedef std::list< Ptr<PbbTlv> >::const_iterator ConstIterator;

  Iterator Begin (void);
  ConstIterator Begin (void) const;
  Iterator End (void);
  ConstIterator End (void) const;

  int Size (void) const;
  bool Empty (void) const;

  Ptr<PbbTlv> Front (void) const;
  Ptr<PbbTlv> Back (void) const;

  void PushFront (Ptr<PbbTlv> tlv);
  void PopFront (void);

  void PushBack (Ptr<PbbTlv> tlv);
  void PopBack (void);

  Iterator Insert (Iterator position, const Ptr<PbbTlv> tlv);

  Iterator Erase (Iterator position);
  Iterator Erase (Iterator first, Iterator last);

  void Clear (void);

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const PbbTlvBlock &other) const;
  bool operator!= (const PbbTlvBlock &other) const;

private:
  std::list< Ptr<PbbTlv> > m_tlvList;
};

/**
 * \brief A block of Address TLVs.
 *
 * Acts similar to a C++ STL container.
 */
class PbbAddressTlvBlock
{
public:
  typedef std::list< Ptr<PbbAddressTlv> >::iterator Iterator;
  typedef std::list< Ptr<PbbAddressTlv> >::const_iterator ConstIterator;

  Iterator Begin (void);
  ConstIterator Begin (void) const;
  Iterator End (void);
  ConstIterator End (void) const;

  int Size (void) const;
  bool Empty (void) const;

  Ptr<PbbAddressTlv> Front (void) const;
  Ptr<PbbAddressTlv> Back (void) const;

  void PushFront (Ptr<PbbAddressTlv> tlv);
  void PopFront (void);

  void PushBack (Ptr<PbbAddressTlv> tlv);
  void PopBack (void);

  Iterator Insert (Iterator position, const Ptr<PbbAddressTlv> tlv);

  Iterator Erase (Iterator position);
  Iterator Erase (Iterator first, Iterator last);

  void Clear (void);

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const PbbAddressTlvBlock &other) const;
  bool operator!= (const PbbAddressTlvBlock &other) const;

private:
  std::list< Ptr<PbbAddressTlv> > m_tlvList;
};

/**
 * \brief Main PbbPacket Packet object.
 */
class PbbPacket : public Header
{
public:
  typedef std::list< Ptr<PbbTlv> >::iterator TlvIterator;
  typedef std::list< Ptr<PbbTlv> >::const_iterator ConstTlvIterator;
  typedef std::list< Ptr<PbbMessage> >::iterator MessageIterator;
  typedef std::list< Ptr<PbbMessage> >::const_iterator ConstMessageIterator;

  PbbPacket (void);

  uint8_t GetVersion (void) const;

  void SetSequenceNumber (uint16_t number);
  /**
   * \returns the sequence number of this packet.
   *
   * Calling this while HasSequenceNumber is False is undefined.  Make sure you
   * check it first.  This will be checked by an assert in debug builds.
   */
  uint16_t GetSequenceNumber (void) const;
  bool HasSequenceNumber (void) const;

  /* Manipulating Packet TLVs */

  TlvIterator TlvBegin (void);
  ConstTlvIterator TlvBegin (void) const;
  TlvIterator TlvEnd (void);
  ConstTlvIterator TlvEnd (void) const;

  int TlvSize (void) const;
  bool TlvEmpty (void) const;

  Ptr<PbbTlv> TlvFront (void);
  const Ptr<PbbTlv> TlvFront (void) const;
  Ptr<PbbTlv> TlvBack (void);
  const Ptr<PbbTlv> TlvBack (void) const;

  void TlvPushFront (Ptr<PbbTlv>);
  void TlvPopFront (void);
  void TlvPushBack (Ptr<PbbTlv>);
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

  Ptr<PbbMessage> MessageFront (void);
  const Ptr<PbbMessage> MessageFront (void) const;
  Ptr<PbbMessage> MessageBack (void);
  const Ptr<PbbMessage> MessageBack (void) const;

  void MessagePushFront (Ptr<PbbMessage> message);
  void MessagePopFront (void);
  void MessagePushBack (Ptr<PbbMessage> message);
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
  /**
   * \returns the number of bytes deserialized
   *
   * If this returns a number smaller than the total number of bytes in the
   * buffer, there was an error.
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  bool operator== (const PbbPacket &other) const;
  bool operator!= (const PbbPacket &other) const;

protected:
  void SerializePacketTlv (Buffer::Iterator &start) const;

private:
  PbbTlvBlock m_tlvList;
  std::list< Ptr<PbbMessage> > m_messageList;

  uint8_t m_version;

  bool m_hasseqnum;
  uint16_t m_seqnum;

  mutable uint32_t m_refCount;
};

/**
 * \brief A message within a PbbPacket packet.
 *
 * There may be any number of messages in one PbbPacket packet.
 * This is a pure virutal base class, you should instantiate either PbbMessageIpv4
 * or PbbMessageIpv6.
 */
class PbbMessage
{
public:
  typedef std::list< Ptr<PbbTlv> >::iterator TlvIterator;
  typedef std::list< Ptr<PbbTlv> >::const_iterator ConstTlvIterator;
  typedef std::list< Ptr<PbbAddressBlock> >::iterator AddressBlockIterator;
  typedef std::list< Ptr<PbbAddressBlock> >::const_iterator ConstAddressBlockIterator;

  PbbMessage (void);

  void SetType (uint8_t type);
  uint8_t GetType (void) const;

  void SetOriginatorAddress (Address address);
  /**
   * \returns the address of the node that created this packet.
   *
   * Calling this while HasOriginatorAddress is False is undefined.  Make sure
   * you check it first.  This will be checked by an assert in debug builds.
   */
  Address GetOriginatorAddress (void) const;
  bool HasOriginatorAddress (void) const;

  void SetHopLimit (uint8_t hoplimit);
  /**
   * \returns the maximum number of hops this message should travel.
   *
   * Calling this while HasHopLimit is False is undefined.  Make sure you check
   * it first.  This will be checked by an assert in debug builds.
   */
  uint8_t GetHopLimit (void) const;
  bool HasHopLimit (void) const;

  void SetHopCount (uint8_t hopcount);
  /**
   * \returns the current number of hops this message has traveled.
   *
   * Calling this while HasHopCount is False is undefined.  Make sure you check
   * it first.  This will be checked by an assert in debug builds.
   */
  uint8_t GetHopCount (void) const;
  bool HasHopCount (void) const;

  void SetSequenceNumber (uint16_t seqnum);
  /**
   * \returns the sequence number of this message.
   *
   * Calling this while HasSequenceNumber is False is undefined.  Make sure you
   * check it first.  This will be checked by an assert in debug builds.
   */
  uint16_t GetSequenceNumber (void) const;
  bool HasSequenceNumber (void) const;

  /* Manipulating PbbMessage TLVs */

  TlvIterator TlvBegin ();
  ConstTlvIterator TlvBegin () const;
  TlvIterator TlvEnd ();
  ConstTlvIterator TlvEnd () const;

  int TlvSize (void) const;
  bool TlvEmpty (void) const;

  Ptr<PbbTlv> TlvFront (void);
  const Ptr<PbbTlv> TlvFront (void) const;
  Ptr<PbbTlv> TlvBack (void);
  const Ptr<PbbTlv> TlvBack (void) const;

  void TlvPushFront (Ptr<PbbTlv> tlv);
  void TlvPopFront (void);
  void TlvPushBack (Ptr<PbbTlv> tlv);
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

  Ptr<PbbAddressBlock> AddressBlockFront (void);
  const Ptr<PbbAddressBlock> AddressBlockFront (void) const;
  Ptr<PbbAddressBlock> AddressBlockBack (void);
  const Ptr<PbbAddressBlock> AddressBlockBack (void) const;

  void AddressBlockPushFront (Ptr<PbbAddressBlock> block);
  void AddressBlockPopFront (void);
  void AddressBlockPushBack (Ptr<PbbAddressBlock> block);
  void AddressBlockPopBack (void);

  AddressBlockIterator AddressBlockErase (AddressBlockIterator position);
  AddressBlockIterator AddressBlockErase (AddressBlockIterator first,
      AddressBlockIterator last);
  void AddressBlockClear (void);

  /* Smart pointer methods */
  void Ref (void) const;
  void Unref (void) const;

  /* Returns 0 on error */
  static Ptr<PbbMessage> DeserializeMessage (Buffer::Iterator &start);
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator &start) const;
  void Deserialize (Buffer::Iterator &start);
  void Print (std::ostream &os) const;
  void Print (std::ostream &os, int level) const;

  bool operator== (const PbbMessage &other) const;
  bool operator!= (const PbbMessage &other) const;

protected:
  /* PbbMessage size in bytes - 1.
   *
   * IPv4 = 4 - 1 = 3, IPv6 = 16 - 1 = 15
   */
  virtual AddressLength GetAddressLength (void) const = 0;

  virtual void SerializeOriginatorAddress (Buffer::Iterator &start) const = 0;
  virtual Address DeserializeOriginatorAddress (Buffer::Iterator &start) const = 0;
  virtual void PrintOriginatorAddress (std::ostream &os) const = 0;

  virtual Ptr<PbbAddressBlock> AddressBlockDeserialize (Buffer::Iterator &start) const = 0;

private:
  PbbTlvBlock m_tlvList;
  std::list< Ptr<PbbAddressBlock> > m_addressBlockList;

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

/**
 * \brief Concrete IPv4 specific PbbMessage.
 *
 * This message will only contain IPv4 addresses.
 */
class PbbMessageIpv4 : public PbbMessage {
protected:
  virtual AddressLength GetAddressLength (void) const;

  virtual void SerializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual Address DeserializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual void PrintOriginatorAddress (std::ostream &os) const;

  virtual Ptr<PbbAddressBlock> AddressBlockDeserialize (Buffer::Iterator &start) const;
};

/**
 * \brief Concrete IPv6 specific PbbMessage class.
 *
 * This message will only contain IPv6 addresses.
 */
class PbbMessageIpv6 : public PbbMessage {
protected:
  virtual AddressLength GetAddressLength (void) const;

  virtual void SerializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual Address DeserializeOriginatorAddress (Buffer::Iterator &start) const;
  virtual void PrintOriginatorAddress (std::ostream &os) const;

  virtual Ptr<PbbAddressBlock> AddressBlockDeserialize (Buffer::Iterator &start) const;
};

/**
 * \brief An Address Block and its associated Address TLV Blocks.
 *
 * This is a pure virtual base class, you should instantiate either
 * PbbAddressBlockIpv4 or PbbAddressBlockIpv6.
 */
class PbbAddressBlock
{
public:
  typedef std::list< Address >::iterator AddressIterator;
  typedef std::list< Address >::const_iterator ConstAddressIterator;

  typedef std::list<uint8_t>::iterator PrefixIterator;
  typedef std::list<uint8_t>::const_iterator ConstPrefixIterator;

  typedef PbbAddressTlvBlock::Iterator TlvIterator;
  typedef PbbAddressTlvBlock::ConstIterator ConstTlvIterator;

  PbbAddressBlock ();

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

  Ptr<PbbAddressTlv> TlvFront (void);
  const Ptr<PbbAddressTlv> TlvFront (void) const;
  Ptr<PbbAddressTlv> TlvBack (void);
  const Ptr<PbbAddressTlv> TlvBack (void) const;

  void TlvPushFront (Ptr<PbbAddressTlv> address);
  void TlvPopFront (void);

  void TlvPushBack (Ptr<PbbAddressTlv> address);
  void TlvPopBack (void);

  TlvIterator TlvInsert (TlvIterator position, const Ptr<PbbTlv> value);

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

  bool operator== (const PbbAddressBlock &other) const;
  bool operator!= (const PbbAddressBlock &other) const;

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
  PbbAddressTlvBlock m_addressTlvList;

  mutable uint32_t m_refCount;
};

/**
 * \brief Concrete IPv4 specific PbbAddressBlock.
 *
 * This address block will only contain IPv4 addresses.
 */
class PbbAddressBlockIpv4 : public PbbAddressBlock
{
protected:
  virtual uint8_t GetAddressLength (void) const;

  virtual void SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const;
  virtual Address DeserializeAddress (uint8_t *buffer) const;
  virtual void PrintAddress (std::ostream &os, ConstAddressIterator iter) const;
};

/**
 * \brief Concrete IPv6 specific PbbAddressBlock.
 *
 * This address block will only contain IPv6 addresses.
 */
class PbbAddressBlockIpv6 : public PbbAddressBlock
{
protected:
  virtual uint8_t GetAddressLength (void) const;

  virtual void SerializeAddress (uint8_t *buffer, ConstAddressIterator iter) const;
  virtual Address DeserializeAddress (uint8_t *buffer) const;
  virtual void PrintAddress (std::ostream &os, ConstAddressIterator iter) const;
};

/**
 * \brief A packet or message TLV
 */
class PbbTlv
{
public:
  PbbTlv (void);

  void SetType (uint8_t type);
  uint8_t GetType (void) const;

  void SetTypeExt (uint8_t type);
  /**
   * \returns the type extension for this TLV.
   *
   * Calling this while HasTypeExt is False is undefined.  Make sure you check
   * it first.  This will be checked by an assert in debug builds.
   */
  uint8_t GetTypeExt (void) const;
  bool HasTypeExt (void) const;

  void SetValue (Buffer start);
  void SetValue (const uint8_t * buffer, uint32_t size);
  /**
   * \returns a Buffer pointing to the value of this TLV.
   *
   * Calling this while HasValue is False is undefined.  Make sure you check it
   * first.  This will be checked by an assert in debug builds.
   */
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

  bool operator== (const PbbTlv &other) const;
  bool operator!= (const PbbTlv &other) const;

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

/**
 * \brief An Address TLV
 */
class PbbAddressTlv : public PbbTlv
{
public:
  void SetIndexStart (uint8_t index);
  /**
   * \returns the first (inclusive) index of the address in the corresponding
   * PbbAddressBlock that this TLV applies to.
   *
   * Calling this while HasIndexStart is False is undefined.  Make sure you
   * check it first.  This will be checked by an assert in debug builds.
   */
  uint8_t GetIndexStart (void) const;
  bool HasIndexStart (void) const;

  void SetIndexStop (uint8_t index);
  /**
   * \returns the last (inclusive) index of the address in the corresponding
   * PbbAddressBlock that this TLV applies to.
   *
   * Calling this while HasIndexStop is False is undefined.  Make sure you
   * check it first.  This will be checked by an assert in debug builds.
   */
  uint8_t GetIndexStop (void) const;
  bool HasIndexStop (void) const;

  void SetMultivalue (bool isMultivalue);
  bool IsMultivalue (void) const;
};

} /* namespace ns3 */

#endif /* PACKETBB_H */
