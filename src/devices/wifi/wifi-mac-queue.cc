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

#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "wifi-mac-queue.h"

using namespace std;

namespace ns3 {

WifiMacQueue::Item::Item (Packet packet, 
                            WifiMacHeader const &hdr, 
                            Time tstamp)
  : packet (packet), hdr (hdr), tstamp (tstamp)
{}

WifiMacQueue::WifiMacQueue ()
  : m_size (0)
{}

WifiMacQueue::~WifiMacQueue ()
{
  Flush ();
}

void 
WifiMacQueue::SetMaxSize (uint32_t maxSize)
{
  m_maxSize = maxSize;
}
void 
WifiMacQueue::SetMaxDelay (Time delay)
{
  m_maxDelay = delay;
}
void 
WifiMacQueue::Enqueue (Packet packet, WifiMacHeader const &hdr)
{
  Cleanup ();
  if (m_size == m_maxSize) 
    {
      return;
    }
  Time now = Simulator::Now ();
  m_queue.push_back (Item (packet, hdr, now));
  m_size++;
}
void
WifiMacQueue::Cleanup (void)
{
  if (m_queue.empty ()) 
    {
      return;
    }

  Time now = Simulator::Now ();
  uint32_t n = 0;
  PacketQueueI end = m_queue.begin ();
  for (PacketQueueI i = m_queue.begin (); i != m_queue.end (); i++) 
    {
      if (i->tstamp + m_maxDelay > now) 
        {
          end = i;
          break;
        }
      n++;
    }
  m_size -= n;
  m_queue.erase (m_queue.begin (), end);
}

Packet 
WifiMacQueue::Dequeue (WifiMacHeader *hdr, bool *found)
{
  Cleanup ();
  if (!m_queue.empty ()) 
    {
      Item i = m_queue.front ();
      m_queue.pop_front ();
      m_size--;
      *hdr = i.hdr;
      *found = true;
      return i.packet;
    }
  *found = false;
  return Packet ();
}


bool
WifiMacQueue::IsEmpty (void)
{
  Cleanup ();
  return m_queue.empty ();
}


uint32_t
WifiMacQueue::GetSize (void)
{
  return m_size;
}

void
WifiMacQueue::Flush (void)
{
  m_queue.erase (m_queue.begin (), m_queue.end ());
  m_size = 0;
}

} // namespace ns3
