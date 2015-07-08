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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
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
 */
class RadiotapHeader : public Header
{
public:
  RadiotapHeader ();
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
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

  enum
  {
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

  enum
  {
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

  enum
  {
    MCS_KNOWN_NONE           = 0x00, /**< No flags set */
    MCS_KNOWN_BANDWIDTH      = 0x01, /**< Bandwidth */
    MCS_KNOWN_INDEX          = 0x02, /**< MCS index known */
    MCS_KNOWN_GUARD_INTERVAL = 0x04, /**< Guard interval */
    MCS_KNOWN_HT_FORMAT      = 0x08, /**< HT format */
    MCS_KNOWN_FEC_TYPE       = 0x10, /**< FEC type */
    MCS_KNOWN_STBC           = 0x20, /**< STBC known */
    MCS_KNOWN_NESS           = 0x40, /**< Ness known (Number of extension spatial streams) */
    MCS_KNOWN_NESS_BIT_1     = 0x80, /**< Ness data - bit 1 (MSB) of Number of extension spatial streams */
  };

  enum
  {
    MCS_FLAGS_NONE           = 0x00, /**< Default: 20 MHz, long guard interval, mixed HT format and BCC FEC type */
    MCS_FLAGS_BANDWIDTH_40   = 0x01, /**< 40 MHz */
    MCS_FLAGS_BANDWIDTH_20L  = 0x02, /**< 20L (20 MHz in lower half of 40 MHz channel) */
    MCS_FLAGS_BANDWIDTH_20U  = 0x03, /**< 20U (20 MHz in upper half of 40 MHz channel) */
    MCS_FLAGS_GUARD_INTERVAL = 0x04, /**< Short guard interval */
    MCS_FLAGS_HT_GREENFIELD  = 0x08, /**< Greenfield HT format */
    MCS_FLAGS_FEC_TYPE       = 0x10, /**< LDPC FEC type */
    MCS_FLAGS_STBC_STREAMS   = 0x60, /**< STBC enabled */
    MCS_FLAGS_NESS_BIT_0     = 0x80, /**< Ness - bit 0 (LSB) of Number of extension spatial streams */
  };

  /**
   * @brief Set the MCS fields
   *
   * @param known The kwown flags.
   * @param flags The flags to set.
   * @param mcs The MCS index value.
   */
  void SetMcsFields (uint8_t known, uint8_t flags, uint8_t mcs);

  /**
   * @brief Get the MCS known bitmap.
   *
   * @returns The MCS known bitmap.
   */
  uint8_t GetMcsKnown (void) const;
  /**
   * @brief Get the MCS flags.
   *
   * @returns The MCS flags.
   */
  uint8_t GetMcsFlags (void) const;
  /**
   * @brief Get the MCS index value.
   *
   * @returns The MCS index value.
   */
  uint8_t GetMcsRate (void) const;

  enum
  {
    A_MPDU_STATUS_NONE                = 0x00, /**< No flags set */
    A_MPDU_STATUS_REPORT_ZERO_LENGTH  = 0x01, /**< Driver reports 0-length subframes */
    A_MPDU_STATUS_IS_ZERO_LENGTH      = 0x02, /**< Frame is 0-length subframe (valid only if 0x0001 is set) */
    A_MPDU_STATUS_LAST_KNOWN          = 0x04, /**< Last subframe is known (should be set for all subframes in an A-MPDU) */
    A_MPDU_STATUS_LAST                = 0x08, /**< This frame is the last subframe */
    A_MPDU_STATUS_DELIMITER_CRC_ERROR = 0x10, /**< Delimiter CRC error */
    A_MPDU_STATUS_DELIMITER_CRC_KNOWN = 0x20  /**< Delimiter CRC value known: the delimiter CRC value field is valid */
  };

  /**
   * @brief Set the A-MPDU status fields
   *
   * @param referenceNumber The A-MPDU reference number to identify all subframes belonging to the same A-MPDU.
   * @param flags The flags to set.
   * @param crc The CRC value value.
   */
  void SetAmpduStatus (uint32_t referenceNumber, uint16_t flags, uint8_t crc);

  /**
   * @brief Get the A-MPDU reference number.
   *
   * @returns The A-MPDU reference number.
   */
  uint32_t GetAmpduStatusRef (void) const;
  /**
   * @brief Get the A-MPDU status flags.
   *
   * @returns The A-MPDU status flags.
   */
  uint16_t GetAmpduStatusFlags (void) const;


private:
  enum
  {
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
    RADIOTAP_DBM_TX_POWER      = 0x00000400,
    RADIOTAP_ANTENNA           = 0x00000800,
    RADIOTAP_DB_ANTSIGNAL      = 0x00001000,
    RADIOTAP_DB_ANTNOISE       = 0x00002000,
    RADIOTAP_RX_FLAGS          = 0x00004000,
    RADIOTAP_MCS               = 0x00080000,
    RADIOTAP_AMPDU_STATUS      = 0x00100000,
    RADIOTAP_VHT               = 0x00200000,
    RADIOTAP_EXT               = 0x10000000
  };

  uint16_t m_length;        //!< entire length of radiotap data + header
  uint32_t m_present;       //!< bits describing which fields follow header

  uint64_t m_tsft;          //!< Time Synchronization Function Timer (when the first bit of the MPDU arrived at the MAC)
  uint8_t m_flags;          //!< Properties of transmitted and received frames.
  uint8_t m_rate;           //!< TX/RX data rate in units of 500 kbps
  uint8_t m_channelPad;     //!< Tx/Rx channel padding.
  uint16_t m_channelFreq;   //!< Tx/Rx frequency in MHz.
  uint16_t m_channelFlags;  //!< Tx/Rx channel flags.
  int8_t m_antennaSignal;   //!< RF signal power at the antenna, dB difference from an arbitrary, fixed reference.
  int8_t m_antennaNoise;    //!< RF noise power at the antenna, dB difference from an arbitrary, fixed reference.

  uint8_t m_mcsKnown; //!< MCS Flags, known information field.
  uint8_t m_mcsFlags; //!< MCS Flags, flags field.
  uint8_t m_mcsRate;  //!< MCS Flags, mcs rate index.

  uint8_t m_ampduStatusPad;       //!< A-MPDU Status Flags, padding before A-MPDU Status Field.
  uint32_t m_ampduStatusRef;      //!< A-MPDU Status Flags, reference number.
  uint16_t m_ampduStatusFlags;    //!< A-MPDU Status Flags, information about the received A-MPDU.
  uint8_t m_ampduStatusCRC;       //!< A-MPDU Status Flags, delimiter CRC value.
};

} // namespace ns3

#endif /*  RADIOTAP_HEADER_H */
