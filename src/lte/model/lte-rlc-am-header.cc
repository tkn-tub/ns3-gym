/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/log.h"

#include "ns3/lte-rlc-am-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LteRlcAmHeader");

NS_OBJECT_ENSURE_REGISTERED (LteRlcAmHeader);

LteRlcAmHeader::LteRlcAmHeader ()
  : m_headerLength (0),
    m_dataControlBit (0xff),
    m_resegmentationFlag (0xff),
    m_pollingBit (0xff),
    m_framingInfo (0xff),
    m_sequenceNumber (0xfffa),
    m_segmentOffset (0xffff),
    m_lastOffset (0xffff),
    m_controlPduType (0xff),
    m_ackSn (0xffff),
    m_nackSn (0xffff)
{
}

LteRlcAmHeader::~LteRlcAmHeader ()
{
    m_headerLength = 0;
    m_dataControlBit = 0xff;
    m_resegmentationFlag = 0xff;
    m_pollingBit = 0xff;
    m_framingInfo = 0xff;
    m_sequenceNumber = 0xfffb;
    m_segmentOffset = 0xffff;
    m_lastOffset = 0xffff;
    m_controlPduType = 0xff;
    m_ackSn = 0xffff;
    m_nackSn = 0xffff;
}

void
LteRlcAmHeader::SetDataPdu (void)
{
  m_headerLength = 4;
  m_dataControlBit = DATA_PDU;
}
void
LteRlcAmHeader::SetControlPdu (uint8_t controlPduType)
{
  m_headerLength = 2;
  m_dataControlBit = CONTROL_PDU;
  m_controlPduType = controlPduType;
}
bool
LteRlcAmHeader::IsDataPdu (void) const
{
  return (m_dataControlBit == DATA_PDU) ? true : false;
}
bool
LteRlcAmHeader::IsControlPdu (void) const
{
  return (m_dataControlBit == CONTROL_PDU) ? true : false;
}

void
LteRlcAmHeader::SetFramingInfo (uint8_t framingInfo)
{
  m_framingInfo = framingInfo & 0x03;
}

void
LteRlcAmHeader::SetSequenceNumber (SequenceNumber10 sequenceNumber)
{
  m_sequenceNumber = sequenceNumber;
}

uint8_t
LteRlcAmHeader::GetFramingInfo () const
{
  return m_framingInfo;
}

SequenceNumber10
LteRlcAmHeader::GetSequenceNumber () const
{
  return m_sequenceNumber;
}


void
LteRlcAmHeader::PushExtensionBit (uint8_t extensionBit)
{
  m_extensionBits.push_back (extensionBit);
  if (m_extensionBits.size() > 1)
    {
      if (m_extensionBits.size() % 2)
        {
          m_headerLength += 1;
        }
      else
        {
          m_headerLength += 2;
        }
    }
}

void
LteRlcAmHeader::PushLengthIndicator (uint16_t lengthIndicator)
{
  m_lengthIndicators.push_back (lengthIndicator);
}


uint8_t
LteRlcAmHeader::PopExtensionBit (void)
{
  uint8_t extensionBit = m_extensionBits.front ();
  m_extensionBits.pop_front ();

  return extensionBit;
}

uint16_t
LteRlcAmHeader::PopLengthIndicator (void)
{
  uint16_t lengthIndicator = m_lengthIndicators.front ();
  m_lengthIndicators.pop_front ();

  return lengthIndicator;
}


void
LteRlcAmHeader::SetResegmentationFlag (uint8_t resegFlag)
{
  m_resegmentationFlag = resegFlag & 0x01;
}

uint8_t
LteRlcAmHeader::GetResegmentationFlag () const
{
  return m_resegmentationFlag;
}


void
LteRlcAmHeader::SetPollingBit (uint8_t pollingBit)
{
  m_pollingBit = pollingBit & 0x01;
}

uint8_t
LteRlcAmHeader::GetPollingBit (void) const
{
  return m_pollingBit;
}


void
LteRlcAmHeader::SetLastSegmentFlag (uint8_t lsf)
{
  m_lastSegmentFlag = lsf & 0x01;
}

uint8_t
LteRlcAmHeader::GetLastSegmentFlag (void) const
{
  return m_lastSegmentFlag;
}


void
LteRlcAmHeader::SetSegmentOffset (uint16_t segmentOffset)
{
  m_segmentOffset = segmentOffset & 0x7FFF;
}

uint16_t
LteRlcAmHeader::GetSegmentOffset (void) const
{
  return m_segmentOffset;
}

uint16_t
LteRlcAmHeader::GetLastOffset (void) const
{
  return m_lastOffset;
}


void
LteRlcAmHeader::SetAckSn (SequenceNumber10 ackSn)
{
  m_ackSn = ackSn;
}

SequenceNumber10
LteRlcAmHeader::GetAckSn (void) const
{
  return m_ackSn;
}


TypeId
LteRlcAmHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteRlcAmHeader")
    .SetParent<Header> ()
    .AddConstructor<LteRlcAmHeader> ()
  ;
  return tid;
}

TypeId
LteRlcAmHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
LteRlcAmHeader::Print (std::ostream &os)  const
{
  std::list <uint8_t>::const_iterator it1 = m_extensionBits.begin ();
  std::list <uint16_t>::const_iterator it2 = m_lengthIndicators.begin ();

  os << "Len=" << m_headerLength;
  os << " D/C=" << (uint16_t)m_dataControlBit;

  if ( m_dataControlBit == DATA_PDU )
    {
      os << " RF=" << (uint16_t)m_resegmentationFlag;
      os << " P=" << (uint16_t)m_pollingBit;
      os << " FI=" << (uint16_t)m_framingInfo;
      os << " E=" << (uint16_t)(*it1);
      os << " SN=" << m_sequenceNumber;
      os << " LSF=" << (uint16_t)(m_lastSegmentFlag);
      os << " SO=" << m_segmentOffset;

      it1++;
      if (it1 != m_extensionBits.end ())
        {
          os << " E=";
        }
      while ( it1 != m_extensionBits.end () )
        {
          os << (uint16_t)(*it1);
          it1++;
        }

      if (it2 != m_lengthIndicators.end ())
        {
          os << " LI=";
        }
      while ( it2 != m_lengthIndicators.end () )
        {
          os << (uint16_t)(*it2) << " ";
          it2++;
        }
    }
  else // if ( m_dataControlBit == CONTROL_PDU )
    {
      os << " ACK_SN=" << m_ackSn;
      os << " NACK_SN=" << m_nackSn;
    }
}

uint32_t LteRlcAmHeader::GetSerializedSize (void) const
{
  return m_headerLength;
}

void LteRlcAmHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  std::list <uint8_t>::const_iterator it1 = m_extensionBits.begin ();
  std::list <uint16_t>::const_iterator it2 = m_lengthIndicators.begin ();

  if ( m_dataControlBit == DATA_PDU )
    {
      i.WriteU8 ( ((DATA_PDU << 7) & 0x80) |
                  ((m_resegmentationFlag << 6) & 0x40) |
                  ((m_pollingBit << 5) & 0x20) |
                  ((m_framingInfo << 3) & 0x18) |
                  (((*it1) << 2) & 0x04) |
                  ((m_sequenceNumber.GetValue () >> 8) & 0x0003) );
      i.WriteU8 ( m_sequenceNumber.GetValue () & 0x00FF );
      i.WriteU8 ( ((m_lastSegmentFlag << 7) & 0x80) |
                  ((m_segmentOffset >> 8) & 0x007F) );
      i.WriteU8 ( m_segmentOffset & 0x00FF );
      it1++;

      while ( it1 != m_extensionBits.end () &&
              it2 != m_lengthIndicators.end () )
        {
          uint16_t oddLi, evenLi;
          uint8_t oddE, evenE;

          oddE = *it1;
          oddLi = *it2;

          it1++;
          it2++;

          if ( it1 != m_extensionBits.end () &&
              it2 != m_lengthIndicators.end () )
            {
              evenE = *it1;
              evenLi = *it2;

              i.WriteU8 ( ((oddE << 7) & 0x80) | ((oddLi >> 4) & 0x007F) );
              i.WriteU8 ( ((oddLi << 4) & 0x00F0) | ((evenE << 3) & 0x08) | ((evenLi >> 8) & 0x0007) );
              i.WriteU8 ( evenLi & 0x00FF );

              it1++;
              it2++;
            }
          else
            {
              i.WriteU8 ( ((oddE << 7) & 0x80) | ((oddLi >> 4) & 0x007F) );
              i.WriteU8 ( ((oddLi << 4) & 0x00F0) ); // Padding is implicit
            }
        }
    }
  else // if ( m_dataControlBit == CONTROL_PDU )
    {
      i.WriteU8 ( ((CONTROL_PDU << 7) & 0x80) |
                  ((m_controlPduType << 4) & 0x70) |
                  ((m_ackSn.GetValue () >> 6) & 0x0F) );
      i.WriteU8 ( ((m_ackSn.GetValue () << 2) & 0xFC) );
    }
}

uint32_t LteRlcAmHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t byte_1;
  uint8_t byte_2;
  uint8_t byte_3;
  uint8_t byte_4;
  uint8_t extensionBit;

  byte_1 = i.ReadU8 ();
  m_headerLength = 1;
  m_dataControlBit = (byte_1 & 0x80) >> 7;

  if ( m_dataControlBit == DATA_PDU )
    {
      byte_2 = i.ReadU8 ();
      byte_3 = i.ReadU8 ();
      byte_4 = i.ReadU8 ();
      m_headerLength += 3;

      m_resegmentationFlag = (byte_1 & 0x40) >> 6;
      m_pollingBit         = (byte_1 & 0x20) >> 5;
      m_framingInfo        = (byte_1 & 0x18) >> 3;
      m_sequenceNumber     = ((byte_1 & 0x03) << 8) | byte_2;

      m_lastSegmentFlag    = (byte_3 & 0x80) >> 7;
      m_segmentOffset      = (byte_3 & 0x7F) | byte_4;

      extensionBit = (byte_1 & 0x04) >> 2;
      m_extensionBits.push_back (extensionBit);

      if (extensionBit == DATA_FIELD_FOLLOWS)
        {
          return GetSerializedSize ();
        }

      uint16_t oddLi, evenLi;
      uint8_t oddE, evenE;
      bool moreLiFields = (extensionBit == E_LI_FIELDS_FOLLOWS);

      while (moreLiFields)
        {
          byte_1 = i.ReadU8 ();
          byte_2 = i.ReadU8 ();

          oddE = (byte_1 & 0x80) >> 7;
          oddLi = ((byte_1 & 0x7F) << 4) | ((byte_2 & 0xF0) >> 4);
          moreLiFields = (oddE == E_LI_FIELDS_FOLLOWS);

          m_extensionBits.push_back (oddE);
          m_lengthIndicators.push_back (oddLi);
          m_headerLength += 2;

          if (moreLiFields)
            {
              byte_3 = i.ReadU8 ();

              evenE = (byte_2 & 0x08) >> 3;
              evenLi = ((byte_2 & 0x07) << 8) | (byte_3 & 0xFF);
              moreLiFields = (evenE == E_LI_FIELDS_FOLLOWS);

              m_extensionBits.push_back (evenE);
              m_lengthIndicators.push_back (evenLi);

              m_headerLength += 1;
            }
        }

      if (m_resegmentationFlag == SEGMENT)
        {
          m_lastOffset = m_segmentOffset + start.GetSize () - m_headerLength; 
        }
    }
  else // if ( m_dataControlBit == CONTROL_PDU )
    {
      byte_2 = i.ReadU8 ();

      m_controlPduType = (byte_1 & 0x70) >> 4;
      m_ackSn = ((byte_1 & 0x0F) << 6 ) | ((byte_2 & 0xFC) >> 2);

      m_headerLength++;
    }

  return GetSerializedSize ();
}

}; // namespace ns3
