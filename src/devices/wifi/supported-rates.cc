/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "supported-rates.h"
#include "ns3/assert.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("SupportedRates");

#define ELEMENT_ID (1)

namespace ns3 {

SupportedRates::SupportedRates ()
  : m_nRates (0)
{}

void 
SupportedRates::AddSupportedRate (uint32_t bs)
{
  NS_ASSERT (m_nRates < 8);
  if (IsSupportedRate (bs))
    {
      return;
    }
  m_rates[m_nRates] = bs/500000;
  m_nRates++;
  NS_LOG_DEBUG ("add rate="<<bs<<", n rates="<<(uint32_t)m_nRates);
}
void 
SupportedRates::SetBasicRate (uint32_t bs)
{
  uint8_t rate = bs / 500000;
  for (uint8_t i = 0; i < m_nRates; i++)
    {
      if ((rate | 0x80) == m_rates[i])
        {
          return;
        }
      if (rate == m_rates[i])
        {
          NS_LOG_DEBUG ("set basic rate="<<bs<<", n rates="<<(uint32_t)m_nRates);
          m_rates[i] |= 0x80;
          return;
        }
    }
  AddSupportedRate (bs);
  SetBasicRate (bs);
}
bool 
SupportedRates::IsBasicRate (uint32_t bs) const
{
  uint8_t rate = (bs / 500000) | 0x80;
  for (uint8_t i = 0; i < m_nRates; i++)
    {
      if (rate == m_rates[i])
        {
          return true;
        }
    }
  return false;
}
bool 
SupportedRates::IsSupportedRate (uint32_t bs) const
{
  uint8_t rate = bs / 500000;
  for (uint8_t i = 0; i < m_nRates; i++) 
    {
      if (rate == m_rates[i] ||
          (rate|0x80) == m_rates[i]) 
        {
          return true;
        }
    }
  return false;
}
uint8_t 
SupportedRates::GetNRates (void) const
{
  return m_nRates;
}
uint32_t 
SupportedRates::GetRate (uint8_t i) const
{
  return (m_rates[i]&0x7f) * 500000;
}
uint32_t 
SupportedRates::GetSerializedSize (void) const
{
  return m_nRates + 1 + 1;
}
Buffer::Iterator 
SupportedRates::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (ELEMENT_ID);
  start.WriteU8 (m_nRates);
  start.Write (m_rates, m_nRates);
  return start;
}
Buffer::Iterator 
SupportedRates::Deserialize (Buffer::Iterator start)
{
  uint8_t elementId;
  elementId = start.ReadU8 ();
  NS_ASSERT (elementId == ELEMENT_ID);
  m_nRates = start.ReadU8 ();
  NS_ASSERT (m_nRates <= 8);
  start.Read (m_rates, m_nRates);
  return start;
}

std::ostream &operator << (std::ostream &os, const SupportedRates &rates)
{
  os << "[";
  for (uint8_t i = 0; i < rates.GetNRates (); i++)
    {
      uint32_t rate = rates.GetRate (i);
      if (rates.IsBasicRate (rate))
        {
          os << "*";
        }
      os << rate << "mbs";
      if (i < rates.GetNRates () - 1)
        {
          os << " ";
        }
    }
  os << "]";
  return os;
}

} // namespace ns3
