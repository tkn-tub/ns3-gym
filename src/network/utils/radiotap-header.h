/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Foundation, Include., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef RADIOTAP_HEADER_H
#define RADIOTAP_HEADER_H


#include <ns3/header.h> 

namespace ns3 {

/**
 * @brief Radiotap header implementation
 *
 * Radiotap is a de facto standard for 802.11 frame injection and reception. 
 * The radiotap header format is a mechanism to supply additional information 
 * about frames, from the driver to userspace applications such as libpcap, and 
 * from a userspace application to the driver for transmission. 
 *
 * @warning the radiotap header specification says that the fields included in
 * the header should be aligned to their natural ize (e.g., 16-bit fields 
 * aligned to 16-bit boundaries, 32-bit fields aligned to 32-bit boundaries, 
 * and so on.  This implementation does not enforce this.  However, the radiotap
 * specification enforces an order in which fields have to appear (if they 
 * appear), and this ordering is such that, provided you don't leave gaps, all
 * fields will end up aligned without the need of inserting padding space.  By 
 * the term "gap" I mean not using a field which would appear between two used 
 * fields.  Moral: don't leave gaps, or if you do be careful about how you
 * do it.
 */
class RadiotapHeader : public Header
{
public:
  RadiotapHeader();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * This method is used by Packet::AddHeader to store the header into the byte
   * buffer of a packet.  This method returns the number of bytes which are 
   * needed to store the header data during a Serialize.
   *
   * @returns The expected size of the header.
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * This method is used by Packet::AddHeader to store the header into the byte
   * buffer of a packet.  The data written is expected to match bit-for-bit the 
   * representation of this header in a real network.
   *
   * @param start An iterator which points to where the header should
   *              be written.
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * This method is used by Packet::RemoveHeader to re-create a header from the 
   * byte buffer of a packet.  The data read is expected to match bit-for-bit 
   * the representation of this header in real networks.
   *
   * @param start An iterator which points to where the header should
   *              written.
   * @returns The number of bytes read.
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * This method is used by Packet::Print to print the content of the header as 
   * ascii data to a C++ output stream.  Although the header is free to format 
   * its output as it wishes, it is recommended to follow a few rules to integrate
   * with the packet pretty printer: start with flags, small field 
   * values located between a pair of parens. Values should be separated 
   * by whitespace. Follow the parens with the important fields, 
   * separated by whitespace.
   *
   * eg: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
   *
   * @param os The output stream
   */
  virtual void Print (std::ostream &os) const;

  /**
   * @brief Set the Time Synchronization Function Timer (TSFT) value.  Valid for
   * received frames only. 
   *
   * @param tsft Value in microseconds of the MAC's 64-bit 802.11 Time 
   *             Synchronization Function timer when the first bit of the MPDU
   *             arrived at the MAC.
   */
  void SetTsft (uint64_t tsft);

  /**
   * @brief Get the Time Synchronization Function Timer (TSFT) value.  Valid for
   * received frames only. 
   *
   * @returns The value in microseconds of the MAC's 64-bit 802.11 Time 
   *          Synchronization Function timer when the first bit of the MPDU
   *          arrived at the MAC.
   */
  uint64_t GetTsft (void) const;

  enum {
    FRAME_FLAG_NONE           = 0x00, /**< No flags set */
    FRAME_FLAG_CFP            = 0x01, /**< Frame sent/received during CFP */
    FRAME_FLAG_SHORT_PREAMBLE = 0x02, /**< Frame sent/received with short preamble */
    FRAME_FLAG_WEP            = 0x04, /**< Frame sent/received with WEP encryption */
    FRAME_FLAG_FRAGMENTED     = 0x08, /**< Frame sent/received with fragmentation */
    FRAME_FLAG_FCS_INCLUDED   = 0x10, /**< Frame includes FCS */
    FRAME_FLAG_DATA_PADDING   = 0x20, /**< Frame has padding between 802.11 header and payload (to 32-bit boundary) */
    FRAME_FLAG_BAD_FCS        = 0x40, /**< Frame failed FCS check */
    FRAME_FLAG_SHORT_GUARD    = 0x80  /**< Frame used short guard interval (HT) */
  };

  /**
   * @brief Set the frame flags of the transmitted or received frame.
   * @param flags flags to set.
   */
  void SetFrameFlags (uint8_t flags);

  /**
   * @brief Get the frame flags of the transmitted or received frame.
   * @returns The frame flags.
   * @see FrameFlags.
   */
  uint8_t GetFrameFlags (void) const;

  /**
   * @brief Set the transmit/receive channel frequency in units of megahertz
   * @param rate the transmit/receive channel frequency in units of megahertz.
   */
  void SetRate (uint8_t rate);

  /**
   * @brief Get the transmit/receive channel frequency in units of megahertz.
   * @returns The transmit/receive channel frequency in units of megahertz.
   */
  uint8_t GetRate (void) const;

  enum {
    CHANNEL_FLAG_NONE          = 0x0000, /**< No flags set */
    CHANNEL_FLAG_TURBO         = 0x0010, /**< Turbo Channel */
    CHANNEL_FLAG_CCK           = 0x0020, /**< CCK channel */
    CHANNEL_FLAG_OFDM          = 0x0040, /**< OFDM channel */
    CHANNEL_FLAG_SPECTRUM_2GHZ = 0x0080, /**< 2 GHz spectrum channel */
    CHANNEL_FLAG_SPECTRUM_5GHZ = 0x0100, /**< 5 GHz spectrum channel */
    CHANNEL_FLAG_PASSIVE       = 0x0200, /**< Only passive scan allowed */
    CHANNEL_FLAG_DYNAMIC       = 0x0400, /**< Dynamic CCK-OFDM channel */
    CHANNEL_FLAG_GFSK          = 0x0800  /**< GFSK channel (FHSS PHY) */
  };

  /**
   * @brief Set the transmit/receive channel frequency and flags
   * @param frequency The transmit/receive data rate in units of 500 kbps.
   * @param flags The flags to set.
   * @see ChannelFlags
   */
  void SetChannelFrequencyAndFlags (uint16_t frequency, uint16_t flags);

  /**
   * @brief Get the transmit/receive data rate in units of 500 kbps.
   * @returns The transmit/receive data rate in units of 500 kbps.
   */
  uint16_t GetChannelFrequency (void) const;

  /**
   * @brief Get the channel flags of the transmitted or received frame.
   * @returns The frame flags.
   * @see ChannelFlags.
   */
  uint16_t GetChannelFlags (void) const;

  /**
   * @brief Set the RF signal power at the antenna as a decibel difference
   * from an arbitrary, fixed reference. 
   *
   * @param signal The RF signal power at the antenna as a decibel difference
   *               from an arbitrary, fixed reference;
   */
  void SetAntennaSignalPower (double signal);

  /**
   * @brief Get the RF signal power at the antenna as a decibel difference
   * from an arbitrary, fixed reference. 
   *
   * @returns The RF signal power at the antenna as a decibel difference
   *          from an arbitrary, fixed reference. 
   */
  uint8_t GetAntennaSignalPower (void) const;

  /**
   * @brief Set the RF noise power at the antenna as a decibel difference
   * from an arbitrary, fixed reference. 
   *
   * @param noise The RF noise power at the antenna as a decibel difference
   *              from an arbitrary, fixed reference. 
   */
  void SetAntennaNoisePower (double noise);

  /**
   * @brief Get the RF noise power at the antenna as a decibel difference
   * from an arbitrary, fixed reference. 
   *
   * @returns The RF noise power at the antenna as a decibel difference
   *          from an arbitrary, fixed reference. 
   */
  uint8_t GetAntennaNoisePower (void) const;

private:
  enum {
    RADIOTAP_TSFT              = 0x00000001,
    RADIOTAP_FLAGS             = 0x00000002,
    RADIOTAP_RATE              = 0x00000004,
    RADIOTAP_CHANNEL           = 0x00000008,
    RADIOTAP_FHSS              = 0x00000010,
    RADIOTAP_DBM_ANTSIGNAL     = 0x00000020,
    RADIOTAP_DBM_ANTNOISE      = 0x00000040,
    RADIOTAP_LOCK_QUALITY      = 0x00000080,
    RADIOTAP_TX_ATTENUATION    = 0x00000100,
    RADIOTAP_DB_TX_ATTENUATION = 0x00000200,
    RADIOTAP_DBM_TX_POWER      = 0x00000200,
    RADIOTAP_ANTENNA           = 0x00000400,
    RADIOTAP_DB_ANTSIGNAL      = 0x00000800,
    RADIOTAP_DB_ANTNOISE       = 0x00001000,
    RADIOTAP_EXT               = 0x10000000
  };
 
  void CheckAddChannelField ();

  uint16_t m_length;
  uint32_t m_present;

  uint64_t m_tsft;
  uint8_t m_flags;
  uint8_t m_rate;
  uint16_t m_channelFreq;
  uint16_t m_channelFlags;
  int8_t m_antennaSignal;
  int8_t m_antennaNoise;
};

} // namespace ns3

#endif /*  RADIOTAP_HEADER_H */
