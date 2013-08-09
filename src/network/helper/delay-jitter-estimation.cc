/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "delay-jitter-estimation.h"
#include "ns3/tag.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

namespace ns3 {

class DelayJitterEstimationTimestampTag : public Tag
{
public:
  DelayJitterEstimationTimestampTag ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  Time GetTxTime (void) const;
private:
  uint64_t m_creationTime;
};

DelayJitterEstimationTimestampTag::DelayJitterEstimationTimestampTag ()
  : m_creationTime (Simulator::Now ().GetTimeStep ())
{
}

TypeId
DelayJitterEstimationTimestampTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("anon::DelayJitterEstimationTimestampTag")
    .SetParent<Tag> ()
    .AddConstructor<DelayJitterEstimationTimestampTag> ()
    .AddAttribute ("CreationTime",
                   "The time at which the timestamp was created",
                   StringValue ("0.0s"),
                   MakeTimeAccessor (&DelayJitterEstimationTimestampTag::GetTxTime),
                   MakeTimeChecker ())
  ;
  return tid;
}
TypeId
DelayJitterEstimationTimestampTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
DelayJitterEstimationTimestampTag::GetSerializedSize (void) const
{
  return 8;
}
void
DelayJitterEstimationTimestampTag::Serialize (TagBuffer i) const
{
  i.WriteU64 (m_creationTime);
}
void
DelayJitterEstimationTimestampTag::Deserialize (TagBuffer i)
{
  m_creationTime = i.ReadU64 ();
}
void
DelayJitterEstimationTimestampTag::Print (std::ostream &os) const
{
  os << "CreationTime=" << m_creationTime;
}
Time
DelayJitterEstimationTimestampTag::GetTxTime (void) const
{
  return TimeStep (m_creationTime);
}

DelayJitterEstimation::DelayJitterEstimation ()
  : m_previousRx (Simulator::Now ()),
    m_previousRxTx (Simulator::Now ()),
    m_jitter (0),
    m_delay (Seconds (0.0))
{
}
void
DelayJitterEstimation::PrepareTx (Ptr<const Packet> packet)
{
  DelayJitterEstimationTimestampTag tag;
  packet->AddByteTag (tag);
}
void
DelayJitterEstimation::RecordRx (Ptr<const Packet> packet)
{
  DelayJitterEstimationTimestampTag tag;
  bool found;
  found = packet->FindFirstMatchingByteTag (tag);
  if (!found)
    {
      return;
    }
  tag.GetTxTime ();

  Time delta = (Simulator::Now () - m_previousRx) - (tag.GetTxTime () - m_previousRxTx);
  m_jitter += (Abs (delta) - m_jitter) / 16;
  m_previousRx = Simulator::Now ();
  m_previousRxTx = tag.GetTxTime ();
  m_delay = Simulator::Now () - tag.GetTxTime ();
}

Time 
DelayJitterEstimation::GetLastDelay (void) const
{
  return m_delay;
}
uint64_t
DelayJitterEstimation::GetLastJitter (void) const
{
  return m_jitter.GetHigh ();
}

} // namespace ns3
