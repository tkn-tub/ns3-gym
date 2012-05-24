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

#include <iomanip>
#include <math.h>
#include "ns3/log.h"
#include "radiotap-header.h"

NS_LOG_COMPONENT_DEFINE ("RadiotapHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RadiotapHeader);
 
RadiotapHeader::RadiotapHeader()
  : m_length (8),
    m_present (0),
    m_tsft (0),
    m_flags (FRAME_FLAG_NONE),
    m_rate (0),
    m_channelFreq (0),
    m_channelFlags (CHANNEL_FLAG_NONE),
    m_antennaSignal (0),
    m_antennaNoise (0)
{
  NS_LOG_FUNCTION (this);
}
 
TypeId RadiotapHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RadiotapHeader")
    .SetParent<Header> ()
    .AddConstructor<RadiotapHeader> ()
  ;
  return tid;
}

TypeId 
RadiotapHeader::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}

uint32_t
RadiotapHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_length;
}

void
RadiotapHeader::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this);

  start.WriteU8 (0); // major version of radiotap header
  start.WriteU8 (0); // pad field
  start.WriteU16 (m_length); // entire length of radiotap data + header
  start.WriteU32 (m_present); // bits describing which fields follow header

  //
  // Time Synchronization Function Timer (when the first bit of the MPDU 
  // arrived at the MAC)
  //
  if (m_present & RADIOTAP_TSFT) // bit 0
    {
      start.WriteU64 (m_tsft);
    }

  //
  // Properties of transmitted and received frames.
  //
  if (m_present & RADIOTAP_FLAGS) // bit 1
    {
      start.WriteU8 (m_flags);
    }

  //
  // TX/RX data rate in units of 500 kbps
  //
  if (m_present & RADIOTAP_RATE) // bit 2
    {
      start.WriteU8 (m_rate);
    }

  //
  // Tx/Rx frequency in MHz, followed by flags.
  //
  if (m_present & RADIOTAP_CHANNEL) // bit 3
    {
      start.WriteU16 (m_channelFreq);
      start.WriteU16 (m_channelFlags);
    }

  //
  // RF signal power at the antenna, decibel difference from an arbitrary, fixed
  // reference.
  //
  if (m_present & RADIOTAP_DBM_ANTSIGNAL) // bit 5
    {
      start.WriteU8 (m_antennaSignal);
    }

  //
  // RF noise power at the antenna, decibel difference from an arbitrary, fixed 
  // reference.
  //
  if (m_present & RADIOTAP_DBM_ANTNOISE) // bit 6
    {
      start.WriteU8 (m_antennaNoise);
    }
}

uint32_t
RadiotapHeader::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this);

  uint8_t tmp = start.ReadU8 (); // major version of radiotap header
  NS_ASSERT_MSG (tmp == 0x00, "RadiotapHeader::Deserialize(): Unexpected major version");
  start.ReadU8 (); // pad field

  m_length = start.ReadU16 (); // entire length of radiotap data + header
  m_present = start.ReadU32 (); // bits describing which fields follow header

  uint32_t bytesRead = 8;

  //
  // Time Synchronization Function Timer (when the first bit of the MPDU arrived at the MAC)
  //
  if (m_present & RADIOTAP_TSFT) // bit 0
    {
      m_tsft = start.ReadU64 ();
      bytesRead += 8;
    }

  //
  // Properties of transmitted and received frames.
  //
  if (m_present & RADIOTAP_FLAGS) // bit 1
    {
      m_flags = start.ReadU8 ();
      ++bytesRead;
    }

  //
  // TX/RX data rate in units of 500 kbps
  //
  if (m_present & RADIOTAP_RATE) // bit 2
    {
      m_rate = start.ReadU8 ();
      ++bytesRead;
    }

  //
  // Tx/Rx frequency in MHz, followed by flags.
  //
  if (m_present & RADIOTAP_CHANNEL) // bit 3
    {
      m_channelFreq = start.ReadU16 ();
      m_channelFlags = start.ReadU16 ();
      bytesRead += 4;
    }

  //
  // The hop set and pattern for frequency-hopping radios.  We don't need it but
  // still need to account for it.
  //
  if (m_present & RADIOTAP_FHSS) // bit 4
    {
      start.ReadU8 ();
      ++bytesRead;
    }

  //
  // RF signal power at the antenna, decibel difference from an arbitrary, fixed
  // reference.
  //
  if (m_present & RADIOTAP_DBM_ANTSIGNAL) // bit 5
    {
      m_antennaSignal = start.ReadU8 ();
      ++bytesRead;
    }

  //
  // RF noise power at the antenna, decibel difference from an arbitrary, fixed 
  // reference.
  //
  if (m_present & RADIOTAP_DBM_ANTNOISE) // bit 6
    {
      m_antennaNoise = start.ReadU8 ();
      ++bytesRead;
    }

  NS_ASSERT_MSG (m_length == bytesRead, "RadiotapHeader::Deserialize(): expected and actual lengths inconsistent");
  return bytesRead;
}

void
RadiotapHeader::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);
  os << " tsft=" << m_tsft
     << " flags=" << std::hex << m_flags << std::dec
     << " rate=" << (uint16_t) m_rate
     << " freq=" << m_channelFreq
     << " chflags=" << std::hex << (uint32_t)m_channelFlags << std::dec
     << " signal=" << (int16_t) m_antennaSignal
     << " noise=" << (int16_t) m_antennaNoise;
}

void
RadiotapHeader::SetTsft (uint64_t value)
{
  NS_LOG_FUNCTION (this << value);
  m_tsft = value;

  if (!(m_present & RADIOTAP_TSFT))
    {
      m_present |= RADIOTAP_TSFT;
      m_length += 8;
    }

  NS_LOG_LOGIC (this << " m_length=" << m_length << " m_present=0x" << std::hex << m_present << std::dec);
}

uint64_t
RadiotapHeader::GetTsft () const
{
  NS_LOG_FUNCTION (this);
  return m_tsft;
}

void 
RadiotapHeader::SetFrameFlags (uint8_t flags)
{
  NS_LOG_FUNCTION (this << flags);
  m_flags = flags;

  if (!(m_present & RADIOTAP_FLAGS))
    {
      m_present |= RADIOTAP_FLAGS;
      m_length += 1;
    }

  NS_LOG_LOGIC (this << " m_length=" << m_length << " m_present=0x" << std::hex << m_present << std::dec);
}

uint8_t
RadiotapHeader::GetFrameFlags (void) const
{
  NS_LOG_FUNCTION (this);
  return m_flags;
}

void 
RadiotapHeader::SetRate (uint8_t rate)
{
  NS_LOG_FUNCTION (this << rate);
  m_rate = rate;

  if (!(m_present & RADIOTAP_RATE))
    {
      m_present |= RADIOTAP_RATE;
      m_length += 1;
    }

  NS_LOG_LOGIC (this << " m_length=" << m_length << " m_present=0x" << std::hex << m_present << std::dec);
}

uint8_t
RadiotapHeader::GetRate (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rate;
}

void 
RadiotapHeader::SetChannelFrequencyAndFlags (uint16_t frequency, uint16_t flags)
{
  NS_LOG_FUNCTION (this << frequency << flags);
  m_channelFreq = frequency;
  m_channelFlags = flags;

  if (!(m_present & RADIOTAP_CHANNEL))
    {
      m_present |= RADIOTAP_CHANNEL;
      m_length += 4;
    }

  NS_LOG_LOGIC (this << " m_length=" << m_length << " m_present=0x" << std::hex << m_present << std::dec);
}

uint16_t 
RadiotapHeader::GetChannelFrequency (void) const
{
  NS_LOG_FUNCTION (this);
  return m_channelFreq;
}

uint16_t 
RadiotapHeader::GetChannelFlags (void) const
{
  NS_LOG_FUNCTION (this);
  return m_channelFlags;
}

void 
RadiotapHeader::SetAntennaSignalPower (double signal)
{
  NS_LOG_FUNCTION (this << signal);

  if (!(m_present & RADIOTAP_DBM_ANTSIGNAL))
    {
      m_present |= RADIOTAP_DBM_ANTSIGNAL;
      m_length += 1;
    }
  if (signal > 127)
    {
      m_antennaSignal = 127;
    }
  else if (signal < -128)
    {
      m_antennaSignal = -128;
    }
  else
    {
      m_antennaSignal = static_cast<int8_t> (floor (signal + 0.5));
    }

  NS_LOG_LOGIC (this << " m_length=" << m_length << " m_present=0x" << std::hex << m_present << std::dec);
}

uint8_t
RadiotapHeader::GetAntennaSignalPower (void) const
{
  NS_LOG_FUNCTION (this);
  return m_antennaSignal;
}

void
RadiotapHeader::SetAntennaNoisePower (double noise)
{
  NS_LOG_FUNCTION (this << noise);

  if (!(m_present & RADIOTAP_DBM_ANTNOISE))
    {
      m_present |= RADIOTAP_DBM_ANTNOISE;
      m_length += 1;
    }
  if (noise > 127.0)
    {
      m_antennaNoise = 127;
    }
  else if (noise < -128.0)
    {
      m_antennaNoise = -128;
    }
  else
    {
      m_antennaNoise = static_cast<int8_t> (floor (noise + 0.5));
    }

  NS_LOG_LOGIC (this << " m_length=" << m_length << " m_present=0x" << std::hex << m_present << std::dec);
}

uint8_t 
RadiotapHeader::GetAntennaNoisePower (void) const
{
  NS_LOG_FUNCTION (this);
  return m_antennaNoise;
}

} // namespace ns3
