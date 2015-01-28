/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2009 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 * Author: Ghada Badawy <gbadawy@gmail.com>
 */

#include "ns3/assert.h"

#include "mac-tx-middle.h"
#include "wifi-mac-header.h"

namespace ns3 {

MacTxMiddle::MacTxMiddle ()
  : m_sequence (0)
{
}

MacTxMiddle::~MacTxMiddle ()
{
  for (std::map<Mac48Address,uint16_t*>::iterator i = m_qosSequences.begin (); i != m_qosSequences.end (); i++)
    {
      delete [] i->second;
    }
}

uint16_t
MacTxMiddle::GetNextSequenceNumberfor (const WifiMacHeader *hdr)
{
  uint16_t retval;
  if (hdr->IsQosData ()
      && !hdr->GetAddr1 ().IsGroup ())
    {
      uint8_t tid = hdr->GetQosTid ();
      NS_ASSERT (tid < 16);
      std::map<Mac48Address, uint16_t*>::iterator it = m_qosSequences.find (hdr->GetAddr1 ());
      if (it != m_qosSequences.end ())
        {
          retval = it->second[tid];
          it->second[tid]++;
          it->second[tid] %= 4096;
        }
      else
        {
          retval = 0;
          std::pair <Mac48Address,uint16_t*> newSeq (hdr->GetAddr1 (), new uint16_t[16]);
          std::pair <std::map<Mac48Address,uint16_t*>::iterator,bool> newIns = m_qosSequences.insert (newSeq);
          NS_ASSERT (newIns.second == true);
          for (uint8_t i = 0; i < 16; i++)
            {
              newIns.first->second[i] = 0;
            }
          newIns.first->second[tid]++;
        }
    }
  else
    {
      retval = m_sequence;
      m_sequence++;
      m_sequence %= 4096;
    }
  return retval;
}
uint16_t
MacTxMiddle::PeekNextSequenceNumberfor (const WifiMacHeader *hdr)
{
  uint16_t retval;
  if (hdr->IsQosData ()
      && !hdr->GetAddr1 ().IsGroup ())
    {
      uint8_t tid = hdr->GetQosTid ();
      NS_ASSERT (tid < 16);
      std::map<Mac48Address, uint16_t*>::iterator it = m_qosSequences.find (hdr->GetAddr1 ());
      if (it != m_qosSequences.end ())
        {
          retval = it->second[tid];
        }
      else
        {
          retval = 0;
        }
    }
  else
    {
      retval = m_sequence;
    }
  return retval;
}

uint16_t
MacTxMiddle::GetNextSeqNumberByTidAndAddress (uint8_t tid, Mac48Address addr) const
{
  NS_ASSERT (tid < 16);
  uint16_t seq = 0;
  std::map <Mac48Address,uint16_t*>::const_iterator it = m_qosSequences.find (addr);
  if (it != m_qosSequences.end ())
    {
      return it->second[tid];
    }
  return seq;
}

} // namespace ns3
