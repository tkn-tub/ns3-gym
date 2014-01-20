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

/**
 * Enumeration for different block ACK policies.
 */
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

  /**
   * Enable or disable HT immediate ACK.
   *
   * \param immediateAck enable or disable HT immediate ACK
   */
  void SetHtImmediateAck (bool immediateAck);
  /**
   * Set the block ACK type.
   *
   * \param type
   */
  void SetType (enum BlockAckType type);
  /**
   * Set Traffic ID (TID).
   *
   * \param tid
   */
  void SetTidInfo (uint8_t tid);
  /**
   * Set the starting sequence number from the given 
   * raw sequence control field.
   *
   * \param seq the raw sequence control 
   */
  void SetStartingSequence (uint16_t seq);

  /**
   * Check if the current ACK policy is immediate.
   *
   * \return true if the current ACK policy is immediate,
   *         false otherwise
   */
  bool MustSendHtImmediateAck (void) const;
  /**
   * Return the Traffic ID (TID).
   *
   * \return TID
   */
  uint8_t GetTidInfo (void) const;
  /**
   * Return the starting sequence number.
   *
   * \return the starting sequence number
   */
  uint16_t GetStartingSequence (void) const;
  /**
   * Check if the current ACK policy is basic
   * (i.e. not multiple TID and not compressed ACK).
   *
   * \return true if the current ACK policy is basic,
   *         false otherwise
   */
  bool IsBasic (void) const;
  /**
   * Check if the current ACK policy is compressed ACK
   * and not multiple TID.
   *
   * \return true if the current ACK policy is compressed ACK,
   *         false otherwise
   */
  bool IsCompressed (void) const;
  /**
   * Check if the current ACK policy has multiple TID.
   *
   * \return true if the current ACK policy has multiple TID,
   *         false otherwise
   */
  bool IsMultiTid (void) const;

  /**
   * Return the starting sequence control.
   *
   * \return the starting sequence control
   */
  uint16_t GetStartingSequenceControl (void) const;

private:
  /**
   * Set the starting sequence control with the given
   * sequence control value
   *
   * \param seqControl
   */
  void SetStartingSequenceControl (uint16_t seqControl);
  /**
   * Return the Block ACK control.
   *
   * \return the Block ACK control
   */
  uint16_t GetBarControl (void) const;
  /**
   * Set the Block ACK control.
   *
   * \param bar
   */
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

  /**
   * Enable or disable HT immediate ACK.
   *
   * \param immediateAck enable or disable HT immediate ACK
   */
  void SetHtImmediateAck (bool immediateAck);
  /**
   * Set the block ACK type.
   *
   * \param type
   */
  void SetType (enum BlockAckType type);
  /**
   * Set Traffic ID (TID).
   *
   * \param tid
   */
  void SetTidInfo (uint8_t tid);
  /**
   * Set the starting sequence number from the given 
   * raw sequence control field.
   *
   * \param seq the raw sequence control 
   */
  void SetStartingSequence (uint16_t seq);

  /**
   * Check if the current ACK policy is immediate.
   *
   * \return true if the current ACK policy is immediate,
   *         false otherwise
   */
  bool MustSendHtImmediateAck (void) const;
  /**
   * Return the Traffic ID (TID).
   *
   * \return TID
   */
  uint8_t GetTidInfo (void) const;
  /**
   * Return the starting sequence number.
   *
   * \return the starting sequence number
   */
  uint16_t GetStartingSequence (void) const;
  /**
   * Check if the current ACK policy is basic
   * (i.e. not multiple TID and not compressed ACK).
   *
   * \return true if the current ACK policy is basic,
   *         false otherwise
   */
  bool IsBasic (void) const;
  /**
   * Check if the current ACK policy is compressed ACK
   * and not multiple TID.
   *
   * \return true if the current ACK policy is compressed ACK,
   *         false otherwise
   */
  bool IsCompressed (void) const;
  /**
   * Check if the current ACK policy has multiple TID.
   *
   * \return true if the current ACK policy has multiple TID,
   *         false otherwise
   */
  bool IsMultiTid (void) const;

  /**
   * Set the bitmap that the packet with the given sequence
   * number was received.
   *
   * \param seq
   */
  void SetReceivedPacket (uint16_t seq);
  /**
   * Set the bitmap that the packet with the given sequence
   * number and fragment number was received.
   *
   * \param seq
   * \param frag
   */
  void SetReceivedFragment (uint16_t seq, uint8_t frag);
  /**
   * Check if the packet with the given sequence number
   * was ACKed in this Block ACK response.
   *
   * \param seq
   * \return true if the packet with the given sequence number
   *         was ACKed in this Block ACK response, false otherwise
   */
  bool IsPacketReceived (uint16_t seq) const;
  /**
   * Check if the packet with the given sequence number
   * and fragment number was ACKed in this Block ACK response.
   *
   * \param seq
   * \param frag
   * \return true if the packet with the given sequence number
   *         and sequence number was ACKed in this Block ACK response,
   *         false otherwise
   */
  bool IsFragmentReceived (uint16_t seq, uint8_t frag) const;

  /**
   * Return the starting sequence control.
   *
   * \return the starting sequence control
   */
  uint16_t GetStartingSequenceControl (void) const;
  /**
   * Set the starting sequence control with the given
   * sequence control value
   *
   * \param seqControl
   */
  void SetStartingSequenceControl (uint16_t seqControl);
  /**
   * Return the bitmap from the block ACK response header.
   *
   * \return the bitmap from the block ACK response header
   */
  const uint16_t* GetBitmap (void) const;
  /**
   * Return the compressed bitmap from the block ACK response header.
   *
   * \return the compressed bitmap from the block ACK response header
   */
  uint64_t GetCompressedBitmap (void) const;

  /**
   * Reset the bitmap to 0.
   */
  void ResetBitmap (void);

private:
  /**
   * Return the block ACK control.
   *
   * \return the block ACK control
   */
  uint16_t GetBaControl (void) const;
  /**
   * Set the block ACK control.
   *
   * \param bar
   */
  void SetBaControl (uint16_t bar);

  /**
   * Serialize bitmap to the given buffer.
   *
   * \param start
   * \return Buffer::Iterator to the next available buffer
   */
  Buffer::Iterator SerializeBitmap (Buffer::Iterator start) const;
  /**
   * Deserialize bitmap from the given buffer.
   *
   * \param start
   * \return Buffer::Iterator to the next available buffer
   */
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
