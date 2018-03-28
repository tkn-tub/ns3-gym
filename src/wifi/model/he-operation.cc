/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Sébastien Deronne
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "he-operation.h"

namespace ns3 {

HeOperation::HeOperation ()
  : m_bssColor (0),
    m_defaultPEDuration (0),
    m_twtRequired (0),
    m_heDurationBasedRtsThreshold (0),
    m_partialBssColor (0),
    m_maxBssidIndicator (0),
    m_txBssidIndicator (0),
    m_bssColorDisabled (0),
    m_dualBeacon (0),
    m_basicHeMcsAndNssSet (0),
    m_heSupported (0)
{
}

WifiInformationElementId
HeOperation::ElementId () const
{
  return IE_HE_OPERATION;
}

void
HeOperation::SetHeSupported (uint8_t hesupported)
{
  m_heSupported = hesupported;
}

uint8_t
HeOperation::GetInformationFieldSize () const
{
  //we should not be here if he is not supported
  NS_ASSERT (m_heSupported > 0);
  return 10;
}

void
HeOperation::SetHeOperationParameters (uint32_t ctrl)
{
  m_bssColor = ctrl & 0x3f;
  m_defaultPEDuration = (ctrl >> 6) & 0x07;
  m_twtRequired = (ctrl >> 9) & 0x01;
  m_heDurationBasedRtsThreshold = (ctrl >> 10) & 0x03ff;
  m_partialBssColor = (ctrl >> 20) & 0x01;
  m_maxBssidIndicator = (ctrl >> 21) & 0xff;
  m_txBssidIndicator = (ctrl >> 29) & 0x01;
  m_bssColorDisabled = (ctrl >> 30) & 0x01;
  m_dualBeacon = (ctrl >> 31) & 0x01;
}

uint32_t
HeOperation::GetHeOperationParameters () const
{
  uint32_t val = 0;
  val |= m_bssColor & 0x3f;
  val |= (m_defaultPEDuration & 0x07) << 6;
  val |= (m_twtRequired & 0x01) << 9;
  val |= (m_heDurationBasedRtsThreshold & 0x03ff) << 10;
  val |= (m_partialBssColor & 0x01) << 20;
  val |= (m_maxBssidIndicator & 0xff) << 21;
  val |= (m_txBssidIndicator & 0x01) << 29;
  val |= (m_bssColorDisabled & 0x01) << 30;
  val |= (m_dualBeacon & 0x01) << 31;
  return val;
}

void
HeOperation::SetMaxHeMcsPerNss (uint8_t nss, uint8_t maxHeMcs)
{
  NS_ASSERT ((maxHeMcs >= 7 && maxHeMcs <= 11) && (nss >= 1 && nss <= 8));
  m_basicHeMcsAndNssSet |= (((maxHeMcs - 7) & 0x07) << ((nss - 1) * 3));
}

uint32_t
HeOperation::GetBasicHeMcsAndNssSet (void) const
{
  return m_basicHeMcsAndNssSet;
}

Buffer::Iterator
HeOperation::Serialize (Buffer::Iterator i) const
{
  if (m_heSupported < 1)
    {
      return i;
    }
  return WifiInformationElement::Serialize (i);
}

uint16_t
HeOperation::GetSerializedSize () const
{
  if (m_heSupported < 1)
    {
      return 0;
    }
  return WifiInformationElement::GetSerializedSize ();
}

void
HeOperation::SerializeInformationField (Buffer::Iterator start) const
{
  if (m_heSupported == 1)
    {
      //write the corresponding value for each bit
      start.WriteHtolsbU32 (GetHeOperationParameters ());
      uint32_t mcsset = GetBasicHeMcsAndNssSet ();
      start.WriteU16 (mcsset & 0xffff);
      start.WriteU8 ((mcsset >> 16) & 0xff);
      start.WriteU16 (0); //todo: VHT Operation Information
      start.WriteU8 (0); //todo: VHT Operation Information
    }
}

uint8_t
HeOperation::DeserializeInformationField (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  uint32_t heOperationParameters = i.ReadLsbtohU32 ();
  uint16_t mcsset_1 = i.ReadU16 ();
  uint8_t mcsset_2 = i.ReadU8 ();
  i.ReadU16 (); //todo: VHT Operation Information
  i.ReadU8 (); //todo: VHT Operation Information
  SetHeOperationParameters (heOperationParameters);
  m_basicHeMcsAndNssSet |= mcsset_1 & 0xffff;
  m_basicHeMcsAndNssSet |= (mcsset_2 & 0xff) << 16;
  return length;
}

/**
 * output stream output operator
 *
 * \param os output stream
 * \param HeOperation the HE operation
 *
 * \returns output stream
 */
std::ostream &
operator << (std::ostream &os, const HeOperation &HeOperation)
{
  os << HeOperation.GetHeOperationParameters () << "|"
     << HeOperation.GetBasicHeMcsAndNssSet ();
  return os;
}

} //namespace ns3
