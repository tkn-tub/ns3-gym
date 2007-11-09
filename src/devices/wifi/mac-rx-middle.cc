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

#include "mac-rx-middle.h"
#include "wifi-mac-header.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include <list>

NS_LOG_COMPONENT_DEFINE ("MacRxMiddle");

#define TRACE(x) NS_LOG_DEBUG(Simulator::Now () << " " << x)

namespace ns3 {


class OriginatorRxStatus {
private:
  typedef std::list<Packet> Fragments;
  typedef std::list<Packet>::const_iterator FragmentsCI;

  bool m_defragmenting;
  uint16_t m_lastSequenceControl;
  Fragments m_fragments;
public:
  OriginatorRxStatus () {
    /* this is a magic value necessary. */
    m_lastSequenceControl = 0xffff;
    m_defragmenting = false;
  }
  ~OriginatorRxStatus () {
    for (FragmentsCI i = m_fragments.begin (); i != m_fragments.end (); i++) 
      {
        // XXX ?
      }
    m_fragments.erase (m_fragments.begin (), m_fragments.end ());
  }
  bool IsDeFragmenting (void) {
    return m_defragmenting;
  }
  void AccumulateFirstFragment (Packet const packet) {
    NS_ASSERT (!m_defragmenting);
    m_defragmenting = true;
    m_fragments.push_back (packet);
  }
  Packet AccumulateLastFragment (Packet const packet) {
    NS_ASSERT (m_defragmenting);
    m_fragments.push_back (packet);
    m_defragmenting = false;
    Packet full;
    for (FragmentsCI i = m_fragments.begin (); i != m_fragments.end (); i++) 
      {
        full.AddAtEnd (*i);
      }
    m_fragments.erase (m_fragments.begin (), m_fragments.end ());
    return full;
  }
  void AccumulateFragment (Packet const packet) {
    NS_ASSERT (m_defragmenting);
    m_fragments.push_back (packet);
  }
  bool IsNextFragment (uint16_t sequenceControl) {
    if ((sequenceControl >> 4) == (m_lastSequenceControl >> 4) &&
        (sequenceControl & 0x0f) == ((m_lastSequenceControl & 0x0f)+1)) 
      {
        return true;
      } 
    else 
      {
        return false;
      }
  }
  uint16_t GetLastSequenceControl (void) 
  {
    return m_lastSequenceControl;
  }
  void SetSequenceControl (uint16_t sequenceControl) 
  {
    m_lastSequenceControl = sequenceControl;
  }
  
};


MacRxMiddle::MacRxMiddle ()
{}

MacRxMiddle::~MacRxMiddle ()
{
  for (OriginatorsI i = m_originatorStatus.begin ();
       i != m_originatorStatus.end (); i++) 
    {
      delete (*i).second;
    }
  m_originatorStatus.erase (m_originatorStatus.begin (),
                            m_originatorStatus.end ());
  for (QosOriginatorsI i = m_qosOriginatorStatus.begin ();
       i != m_qosOriginatorStatus.end (); i++) 
    {
      delete (*i).second;
    }
  m_qosOriginatorStatus.erase (m_qosOriginatorStatus.begin (),
                               m_qosOriginatorStatus.end ());
}

void 
MacRxMiddle::SetForwardCallback (ForwardUpCallback callback)
{
  m_callback = callback;
}

bool
MacRxMiddle::SequenceControlSmaller (int seqca, int seqcb)
{
  int seqa = seqca >> 4;
  int seqb = seqcb >> 4;
  int delta = seqb - seqa;
  TRACE ("seqb="<<seqb<<", seqa="<<seqa<<", delta="<<delta);
  if (delta <= 0 && delta < -2048) 
    {
      return true;
    } 
  else if (delta >= 0 && delta < 2048) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}


OriginatorRxStatus *
MacRxMiddle::Lookup (WifiMacHeader const *hdr)
{
  OriginatorRxStatus *originator;
  Mac48Address source = hdr->GetAddr2 ();
  if (hdr->IsQosData () &&
      !hdr->GetAddr2 ().IsBroadcast ()) 
    {
      /* only for qos data non-broadcast frames */
      originator = m_qosOriginatorStatus[std::make_pair(source, hdr->GetQosTid ())];
      if (originator == 0) 
        {
          originator = new OriginatorRxStatus ();
          m_qosOriginatorStatus[std::make_pair(source, hdr->GetQosTid ())] = originator;
        }
    } 
  else 
    {
      /* - management frames
       * - qos data broadcast frames
       * - nqos data frames
       * see section 7.1.3.4.1
       */
      originator = m_originatorStatus[source];
      if (originator == 0) 
        {
          originator = new OriginatorRxStatus ();
          m_originatorStatus[source] = originator;
        }
    }
  return originator;
}

bool
MacRxMiddle::IsDuplicate (WifiMacHeader const*hdr, 
                          OriginatorRxStatus *originator) const
{
  if (hdr->IsRetry () &&
      originator->GetLastSequenceControl () == hdr->GetSequenceControl ()) 
    {
      return true;
    }
  return false;
}

Packet 
MacRxMiddle::HandleFragments (Packet packet, WifiMacHeader const*hdr,
             OriginatorRxStatus *originator, bool *complete)
{
  if (originator->IsDeFragmenting ()) 
    {
      if (hdr->IsMoreFragments ()) 
        {
          if (originator->IsNextFragment (hdr->GetSequenceControl ())) 
            {
              TRACE ("accumulate fragment seq="<<hdr->GetSequenceNumber ()<<
                     ", frag="<<hdr->GetFragmentNumber ()<<
                     ", size="<<packet.GetSize ());
              originator->AccumulateFragment (packet);
              originator->SetSequenceControl (hdr->GetSequenceControl ());
            } 
          else 
            {
              TRACE ("non-ordered fragment");
            }
          *complete = false;
          return Packet ();
        } 
      else 
        {
          if (originator->IsNextFragment (hdr->GetSequenceControl ())) 
            {
              TRACE ("accumulate last fragment seq="<<hdr->GetSequenceNumber ()<<
                     ", frag="<<hdr->GetFragmentNumber ()<<
                     ", size="<<hdr->GetSize ());
              packet = originator->AccumulateLastFragment (packet);
              originator->SetSequenceControl (hdr->GetSequenceControl ());
              *complete = true;
              return packet;
            } 
          else 
            {
              TRACE ("non-ordered fragment");
              *complete = false;
              return Packet ();
            }
        }
    } 
  else 
    {
      if (hdr->IsMoreFragments ()) 
        {
          TRACE ("accumulate first fragment seq="<<hdr->GetSequenceNumber ()<<
                 ", frag="<<hdr->GetFragmentNumber ()<<
                 ", size="<<packet.GetSize ());
          originator->AccumulateFirstFragment (packet);
          originator->SetSequenceControl (hdr->GetSequenceControl ());
          *complete = false;
          return Packet ();
        } 
      else 
        {
          *complete = true;
          return packet;
        }
    }
}

void
MacRxMiddle::Receive (Packet packet, WifiMacHeader const *hdr)
{
  OriginatorRxStatus *originator = Lookup (hdr);
  if (hdr->IsData ()) 
    {
      NS_ASSERT (SequenceControlSmaller (originator->GetLastSequenceControl (), 
                                         hdr->GetSequenceControl ()));
      // filter duplicates.
      if (IsDuplicate (hdr, originator)) 
        {
          TRACE ("duplicate from="<<hdr->GetAddr2 ()<<
                 ", seq="<<hdr->GetSequenceNumber ()<<
                 ", frag="<<hdr->GetFragmentNumber ());
          return;
        }
      bool complete;
      Packet agregate = HandleFragments (packet, hdr, originator, &complete);
      if (!complete) 
        {
          return;
        }
      TRACE ("forwarding data from="<<hdr->GetAddr2 ()<<
             ", seq="<<hdr->GetSequenceNumber ()<<
             ", frag="<<hdr->GetFragmentNumber ());
      originator->SetSequenceControl (hdr->GetSequenceControl ());
      m_callback (agregate, hdr);
    } 
  else 
    {
      TRACE ("forwarding "<<hdr->GetTypeString ()<<
             ", seq="<<hdr->GetSequenceNumber ()<<
             ", frag="<<hdr->GetFragmentNumber ());
      originator->SetSequenceControl (hdr->GetSequenceControl ());
      m_callback (packet, hdr);
    }
}

} // namespace ns3
