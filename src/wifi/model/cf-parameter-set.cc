/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015
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

#include "cf-parameter-set.h"

namespace ns3 {

CfParameterSet::CfParameterSet ()
  : m_CFPCount (0),
    m_CFPPeriod (0),
    m_CFPMaxDuration (0),
    m_CFPDurRemaining (0),
    m_pcfSupported (0)
{
}

WifiInformationElementId
CfParameterSet::ElementId () const
{
  return IE_CF_PARAMETER_SET;
}

void
CfParameterSet::SetPcfSupported (uint8_t pcfsupported)
{
  m_pcfSupported = pcfsupported;
}

uint8_t
CfParameterSet::GetInformationFieldSize () const
{
  //we should not be here if pcf is not supported
  NS_ASSERT (m_pcfSupported > 0);
  return 6;
}

Buffer::Iterator
CfParameterSet::Serialize (Buffer::Iterator i) const
{
  if (m_pcfSupported < 1)
    {
      return i;
    }
  return WifiInformationElement::Serialize (i);
}

uint16_t
CfParameterSet::GetSerializedSize () const
{
  if (m_pcfSupported < 1)
    {
      return 0;
    }
  return WifiInformationElement::GetSerializedSize ();
}

void
CfParameterSet::SerializeInformationField (Buffer::Iterator start) const
{
  if (m_pcfSupported == 1)
    {
      start.WriteU8 (m_CFPCount);
      start.WriteU8 (m_CFPPeriod);
      start.WriteHtolsbU16 (m_CFPMaxDuration / 1024);
      start.WriteHtolsbU16 (m_CFPDurRemaining / 1024);
    }
}

uint8_t
CfParameterSet::DeserializeInformationField (Buffer::Iterator start,
                                             uint8_t length)
{
  NS_ASSERT (length == 6);
  Buffer::Iterator i = start;
  m_CFPCount = i.ReadU8 ();
  m_CFPPeriod = i.ReadU8 ();
  m_CFPMaxDuration = i.ReadLsbtohU16 ();
  m_CFPMaxDuration *= 1024;
  m_CFPDurRemaining = i.ReadLsbtohU16 ();
  m_CFPDurRemaining *= 1024;
  return length;
}

uint8_t
CfParameterSet::GetCFPCount (void) const
{
  return m_CFPCount;
}

uint8_t
CfParameterSet::GetCFPPeriod (void) const
{
  return m_CFPPeriod;
}

uint64_t
CfParameterSet::GetCFPMaxDurationUs (void) const
{
  return m_CFPMaxDuration;
}

uint64_t
CfParameterSet::GetCFPDurRemainingUs (void) const
{
  return m_CFPDurRemaining;
}

void
CfParameterSet::SetCFPCount (uint8_t count)
{
  m_CFPCount = count;
}

void
CfParameterSet::SetCFPPeriod (uint8_t period)
{
  m_CFPPeriod = period;
}

void
CfParameterSet::SetCFPMaxDurationUs (uint64_t maxDuration)
{
  m_CFPMaxDuration = maxDuration;
}

void
CfParameterSet::SetCFPDurRemainingUs (uint64_t durRemaining)
{
  m_CFPDurRemaining = durRemaining;
}

/**
 * output operator
 *
 * \param os output stream
 * \param cfParameterSet
 *
 * \return output stream
 */
std::ostream &
operator << (std::ostream &os, const CfParameterSet &cfParameterSet)
{
  os <<  (uint16_t)cfParameterSet.GetCFPCount ()
     << "|" << (uint16_t)cfParameterSet.GetCFPPeriod ()
     << "|" << cfParameterSet.GetCFPMaxDurationUs ()
     << "|" << cfParameterSet.GetCFPDurRemainingUs ();

  return os;
}

} //namespace ns3
