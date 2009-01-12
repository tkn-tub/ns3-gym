/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "ns3/assert.h"

#include "mac-tx-middle.h"
#include "wifi-mac-header.h"

namespace ns3 {

MacTxMiddle::MacTxMiddle ()
{
  m_sequence = 0;
  for (uint8_t i = 0; i < 16; i++) 
    {
      m_qosSequences[i] = 0;
    }
}

uint16_t 
MacTxMiddle::GetNextSequenceNumberfor (const WifiMacHeader *hdr)
{
  uint16_t retval;
  if (hdr->IsQosData () &&
      !hdr->GetAddr1 ().IsGroup ()) 
    {
      uint8_t tid = hdr->GetQosTid ();
      NS_ASSERT (tid < 16);
      retval = m_qosSequences[tid];
      m_qosSequences[tid]++;
      m_qosSequences[tid] %= 4096;
    } 
  else 
    {
      retval = m_sequence;
      m_sequence++;
      m_sequence %= 4096;
    }
  return retval;
}

} // namespace ns3
