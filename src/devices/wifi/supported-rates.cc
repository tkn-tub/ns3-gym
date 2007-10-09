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

namespace ns3 {

SupportedRates::SupportedRates ()
  : m_nRates (0)
{}

void 
SupportedRates::AddSupportedRate (uint32_t bs)
{
  NS_ASSERT (m_nRates < 8);
  m_rates[m_nRates] = bs/500000;
  m_nRates++;
}
void 
SupportedRates::SetBasicRate (uint32_t bs)
{
  uint8_t rate = bs / 500000;
  for (uint8_t i = 0; i < m_nRates; i++)
    {
      if (rate == m_rates[i])
        {
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
  uint8_t rate = bs / 500000;
  for (uint8_t i = 0; i < m_nRates; i++)
    {
      if (rate == m_rates[i] && 
          m_rates[i] & 0x80)
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
  for (uint8_t i = 0; i < m_nRates; i++) {
    if (rate == m_rates[i]) {
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
SupportedRates::GetSerializedSize (void) const
{
  return m_nRates + 1;
}
Buffer::Iterator 
SupportedRates::Write (Buffer::Iterator start) const
{
  start.WriteU8 (m_nRates);
  start.Write (m_rates, m_nRates);
  return start;
}
Buffer::Iterator 
SupportedRates::Read (Buffer::Iterator start)
{
  m_nRates = start.ReadU8 ();
  NS_ASSERT (m_nRates <= 8);
  start.Read (m_rates, m_nRates);
  return start;
}

} // namespace ns3
