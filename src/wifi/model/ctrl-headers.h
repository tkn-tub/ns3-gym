/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef CTRL_HEADERS_H
#define CTRL_HEADERS_H

#include "ns3/header.h"

namespace ns3 {

enum BlockAckType
{
  BASIC_BLOCK_ACK,
  COMPRESSED_BLOCK_ACK,
  MULTI_TID_BLOCK_ACK
};

/**
 * \ingroup wifi
 * \brief Headers for Block ack request.
 *
 *  802.11n standard includes three types of block ack:
 *    - Basic block ack (unique type in 802.11e)
 *    - Compressed block ack
 *    - Multi-TID block ack
 *  For now only basic block ack and compressed block ack
 *  are supported.
 *  Basic block ack is also default variant.
 */
class CtrlBAckRequestHeader : public Header
{
public:
  CtrlBAckRequestHeader ();
  ~CtrlBAckRequestHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  void SetHtImmediateAck (bool immediateAck);
  void SetType (enum BlockAckType type);
  void SetTidInfo (uint8_t tid);
  void SetStartingSequence (uint16_t seq);

  bool MustSendHtImmediateAck (void) const;
  uint8_t GetTidInfo (void) const;
  uint16_t GetStartingSequence (void) const;
  bool IsBasic (void) const;
  bool IsCompressed (void) const;
  bool IsMultiTid (void) const;

  uint16_t GetStartingSequenceControl (void) const;

private:
  void SetStartingSequenceControl (uint16_t seqControl);
  uint16_t GetBarControl (void) const;
  void SetBarControl (uint16_t bar);

  /**
   * The lsb bit of the BAR control field is used only for the
   * HT (High Throughput) delayed block ack configuration.
   * For now only non HT immediate block ack is implemented so this field
   * is here only for a future implementation of HT delayed variant.
   */
  bool m_barAckPolicy;
  bool m_multiTid;
  bool m_compressed;
  uint16_t m_tidInfo;
  uint16_t m_startingSeq;
};

/**
 * \ingroup wifi
 * \brief Headers for Block ack response.
 *
 *  802.11n standard includes three types of block ack:
 *    - Basic block ack (unique type in 802.11e)
 *    - Compressed block ack
 *    - Multi-TID block ack
 *  For now only basic block ack and compressed block ack
 *  are supported.
 *  Basic block ack is also default variant.
 */
class CtrlBAckResponseHeader : public Header
{
public:
  CtrlBAckResponseHeader ();
  ~CtrlBAckResponseHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  void SetHtImmediateAck (bool immeadiateAck);
  void SetType (enum BlockAckType type);
  void SetTidInfo (uint8_t tid);
  void SetStartingSequence (uint16_t seq);

  bool MustSendHtImmediateAck (void) const;
  uint8_t GetTidInfo (void) const;
  uint16_t GetStartingSequence (void) const;
  bool IsBasic (void) const;
  bool IsCompressed (void) const;
  bool IsMultiTid (void) const;

  void SetReceivedPacket (uint16_t seq);
  void SetReceivedFragment (uint16_t seq, uint8_t frag);
  bool IsPacketReceived (uint16_t seq) const;
  bool IsFragmentReceived (uint16_t seq, uint8_t frag) const;

  uint16_t GetStartingSequenceControl (void) const;
  void SetStartingSequenceControl (uint16_t seqControl);
  const uint16_t* GetBitmap (void) const;
  uint64_t GetCompressedBitmap (void) const;

  void ResetBitmap (void);

private:
  uint16_t GetBaControl (void) const;
  void SetBaControl (uint16_t bar);

  Buffer::Iterator SerializeBitmap (Buffer::Iterator start) const;
  Buffer::Iterator DeserializeBitmap (Buffer::Iterator start);

  /**
   * This function is used to correctly index in both bitmap
   * and compressed bitmap, one bit or one block of 16 bits respectively.
   *
   * for more details see 7.2.1.8 in IEEE 802.11n/D4.00
   *
   * \param seq the sequence number
   *
   * \return If we are using basic block ack, return value represents index of
   * block of 16 bits for packet having sequence number equals to <i>seq</i>.
   * If we are using compressed block ack, return value represents bit
   * to set to 1 in the compressed bitmap to indicate that packet having
   * sequence number equals to <i>seq</i> was correctly received.
   */
  uint8_t IndexInBitmap (uint16_t seq) const;

  /**
   * Checks if sequence number <i>seq</i> can be acknowledged in the bitmap.
   *
   * \param seq the sequence number
   *
   * \return
   */
  bool IsInBitmap (uint16_t seq) const;

  /**
   * The lsb bit of the BA control field is used only for the
   * HT (High Throughput) delayed block ack configuration.
   * For now only non HT immediate block ack is implemented so this field
   * is here only for a future implementation of HT delayed variant.
   */
  bool m_baAckPolicy;
  bool m_multiTid;
  bool m_compressed;
  uint16_t m_tidInfo;
  uint16_t m_startingSeq;

  union
  {
    uint16_t m_bitmap[64];
    uint64_t m_compressedBitmap;
  } bitmap;
};

} // namespace ns3

#endif /* CTRL_HEADERS_H */
