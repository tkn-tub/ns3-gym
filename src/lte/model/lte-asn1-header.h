/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Lluis Parcerisa <lparcerisa@cttc.cat>
 */

#ifndef ASN1_HEADER_H
#define ASN1_HEADER_H

#include "ns3/header.h"

#include <bitset>
#include <string>

#include "ns3/lte-rrc-sap.h"

namespace ns3 {

/**
 * This class has the purpose to encode Information Elements according
 * to ASN.1 syntax, as defined in ITU-T  X-691.
 * IMPORTANT: The encoding is done following the UNALIGNED variant.
 */
class Asn1Header : public Header
{
public:
  Asn1Header ();
  virtual ~Asn1Header ();

  // Inherited from ns3::Header base class
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator bIterator) const;

  // Inherited from ns3::Header base class
  // Pure virtual methods, to be implemented in child classes
  virtual uint32_t Deserialize (Buffer::Iterator bIterator) = 0;
  virtual void Print (std::ostream &os) const = 0;
    
  /**
   * This function serializes class attributes to m_serializationResult local Buffer.
   * As ASN1 encoding produces a bitstream that does not have a fixed length,
   * this function is needed to store the result, so its length can be retrieved
   * with Header::GetSerializedSize() function.
   * This method is pure virtual in this class (needs to be implemented in child classes)
   * as the meningful information elements are in the subclasses.
   */
  virtual void PreSerialize (void) const = 0;

protected:
  mutable uint8_t m_serializationPendingBits;
  mutable uint8_t m_numSerializationPendingBits;
  mutable bool m_isDataSerialized;
  mutable Buffer m_serializationResult;

  // Function to write in m_serializationResult, after resizing its size
  void WriteOctet (uint8_t octet) const;

  // Serialization functions
  void SerializeBoolean (bool value) const;
  void SerializeInteger (int n, int nmin, int nmax) const;
  void SerializeOctetstring (std::string s) const;
  void SerializeSequenceOf (int numElems, int nMax, int nMin) const;
  void SerializeChoice (int numOptions, int selectedOption, bool isExtensionMarkerPresent) const;
  void SerializeEnum (int numElems, int selectedElem) const;
  void SerializeNull () const;
  void FinalizeSerialization () const;

  template <int N>
  void SerializeBitset (std::bitset<N> data) const;

  template <int N>
  void SerializeSequence (std::bitset<N> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<0> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<1> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<2> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<3> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<4> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<5> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<6> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<9> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<10> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;
  void SerializeSequence (std::bitset<11> optionalOrDefaultMask, bool isExtensionMarkerPresent) const;

  template <int N>
  void SerializeBitstring (std::bitset<N> bitstring) const;
  void SerializeBitstring (std::bitset<1> bitstring) const;
  void SerializeBitstring (std::bitset<2> bitstring) const;
  void SerializeBitstring (std::bitset<8> bitstring) const;
  void SerializeBitstring (std::bitset<10> bitstring) const;
  void SerializeBitstring (std::bitset<16> bitstring) const;
  void SerializeBitstring (std::bitset<27> bitstring) const;
  void SerializeBitstring (std::bitset<28> bitstring) const;
  void SerializeBitstring (std::bitset<32> bitstring) const;

  // Deserialization functions
  template <int N>
  Buffer::Iterator DeserializeBitset (std::bitset<N> *data, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitset (std::bitset<8> *data, Buffer::Iterator bIterator);

  Buffer::Iterator DeserializeBoolean (bool *value, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeInteger (int *n, int nmin, int nmax, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeChoice (int numOptions, bool isExtensionMarkerPresent, int *selectedOption, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeEnum (int numElems, int *selectedElem, Buffer::Iterator bIterator);

  template <int N>
  Buffer::Iterator DeserializeSequence (std::bitset<N> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<0> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<1> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<2> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<3> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<4> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<5> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<6> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<9> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<10> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequence (std::bitset<11> *optionalOrDefaultMask, bool isExtensionMarkerPresent, Buffer::Iterator bIterator);

  template <int N>
  Buffer::Iterator DeserializeBitstring (std::bitset<N> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<1> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<2> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<8> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<10> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<16> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<27> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<28> *bitstring, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeBitstring (std::bitset<32> *bitstring, Buffer::Iterator bIterator);

  Buffer::Iterator DeserializeNull (Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSequenceOf (int *numElems, int nMax, int nMin, Buffer::Iterator bIterator);
};

} // namespace ns3

#endif // ASN1_HEADER_H

