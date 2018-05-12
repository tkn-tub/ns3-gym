/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013
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
 * Author: Ghada Badawy <gbadawy@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "mpdu-aggregator.h"
#include "ampdu-subframe-header.h"

NS_LOG_COMPONENT_DEFINE ("MpduAggregator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MpduAggregator);

TypeId
MpduAggregator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MpduAggregator")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
    .AddConstructor<MpduAggregator> ()
  ;
  return tid;
}

MpduAggregator::MpduAggregator ()
{
}

MpduAggregator::~MpduAggregator ()
{
}

void
MpduAggregator::SetMaxAmpduSize (uint16_t maxSize)
{
  m_maxAmpduLength = maxSize;
}

uint16_t
MpduAggregator::GetMaxAmpduSize (void) const
{
  return m_maxAmpduLength;
}

bool
MpduAggregator::Aggregate (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket) const
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> currentPacket;
  AmpduSubframeHeader currentHdr;

  uint8_t padding = CalculatePadding (aggregatedPacket);
  uint32_t actualSize = aggregatedPacket->GetSize ();

  if ((4 + packet->GetSize () + actualSize + padding) <= GetMaxAmpduSize ())
    {
      if (padding)
        {
          Ptr<Packet> pad = Create<Packet> (padding);
          aggregatedPacket->AddAtEnd (pad);
        }
      currentHdr.SetLength (static_cast<uint16_t> (packet->GetSize ()));
      currentPacket = packet->Copy ();

      currentPacket->AddHeader (currentHdr);
      aggregatedPacket->AddAtEnd (currentPacket);
      return true;
    }
  return false;
}

void
MpduAggregator::AggregateSingleMpdu (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket) const
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> currentPacket;
  AmpduSubframeHeader currentHdr;

  uint8_t padding = CalculatePadding (aggregatedPacket);
  if (padding)
    {
      Ptr<Packet> pad = Create<Packet> (padding);
      aggregatedPacket->AddAtEnd (pad);
    }

  currentHdr.SetEof (1);
  currentHdr.SetLength (static_cast<uint16_t> (packet->GetSize ()));
  currentPacket = packet->Copy ();

  currentPacket->AddHeader (currentHdr);
  aggregatedPacket->AddAtEnd (currentPacket);
}

void
MpduAggregator::AddHeaderAndPad (Ptr<Packet> packet, bool last, bool isSingleMpdu) const
{
  NS_LOG_FUNCTION (this);
  AmpduSubframeHeader currentHdr;

  //This is called to prepare packets from the aggregate queue to be sent so no need to check total size since it has already been
  //done before when deciding how many packets to add to the queue
  currentHdr.SetLength (static_cast<uint16_t> (packet->GetSize ()));
  if (isSingleMpdu)
    {
      currentHdr.SetEof (1);
    }

  packet->AddHeader (currentHdr);
  uint32_t padding = CalculatePadding (packet);

  if (padding && !last)
    {
      Ptr<Packet> pad = Create<Packet> (padding);
      packet->AddAtEnd (pad);
    }
}

bool
MpduAggregator::CanBeAggregated (uint32_t packetSize, Ptr<Packet> aggregatedPacket, uint8_t blockAckSize) const
{
  uint8_t padding = CalculatePadding (aggregatedPacket);
  uint32_t actualSize = aggregatedPacket->GetSize ();
  if (blockAckSize > 0)
    {
      blockAckSize = blockAckSize + 4 + padding;
    }
  if ((4 + packetSize + actualSize + padding + blockAckSize) <= GetMaxAmpduSize ())
    {
      return true;
    }
  else
    {
      return false;
    }
}

uint8_t
MpduAggregator::CalculatePadding (Ptr<const Packet> packet) const
{
  return (4 - (packet->GetSize () % 4 )) % 4;
}

MpduAggregator::DeaggregatedMpdus
MpduAggregator::Deaggregate (Ptr<Packet> aggregatedPacket)
{
  NS_LOG_FUNCTION_NOARGS ();
  DeaggregatedMpdus set;

  AmpduSubframeHeader hdr;
  Ptr<Packet> extractedMpdu = Create<Packet> ();
  uint32_t maxSize = aggregatedPacket->GetSize ();
  uint16_t extractedLength;
  uint32_t padding;
  uint32_t deserialized = 0;

  while (deserialized < maxSize)
    {
      deserialized += aggregatedPacket->RemoveHeader (hdr);
      extractedLength = hdr.GetLength ();
      extractedMpdu = aggregatedPacket->CreateFragment (0, static_cast<uint32_t> (extractedLength));
      aggregatedPacket->RemoveAtStart (extractedLength);
      deserialized += extractedLength;

      padding = (4 - (extractedLength % 4 )) % 4;

      if (padding > 0 && deserialized < maxSize)
        {
          aggregatedPacket->RemoveAtStart (padding);
          deserialized += padding;
        }

      std::pair<Ptr<Packet>, AmpduSubframeHeader> packetHdr (extractedMpdu, hdr);
      set.push_back (packetHdr);
    }
  NS_LOG_INFO ("Deaggreated A-MPDU: extracted " << set.size () << " MPDUs");
  return set;
}

} //namespace ns3
