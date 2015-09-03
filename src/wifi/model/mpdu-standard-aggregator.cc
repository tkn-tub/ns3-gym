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
#include "ns3/uinteger.h"
#include "ampdu-subframe-header.h"
#include "mpdu-standard-aggregator.h"

NS_LOG_COMPONENT_DEFINE ("MpduStandardAggregator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MpduStandardAggregator);

TypeId
MpduStandardAggregator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MpduStandardAggregator")
    .SetParent<MpduAggregator> ()
    .SetGroupName ("Wifi")
    .AddConstructor<MpduStandardAggregator> ()
    .AddAttribute ("MaxAmpduSize", "Max length in bytes of an A-MPDU",
                   UintegerValue (65535),
                   MakeUintegerAccessor (&MpduStandardAggregator::m_maxAmpduLength),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

MpduStandardAggregator::MpduStandardAggregator ()
{
}

MpduStandardAggregator::~MpduStandardAggregator ()
{
}

bool
MpduStandardAggregator::Aggregate (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> currentPacket;
  AmpduSubframeHeader currentHdr;

  uint32_t padding = CalculatePadding (aggregatedPacket);
  uint32_t actualSize = aggregatedPacket->GetSize ();

  if ((4 + packet->GetSize () + actualSize + padding) <= m_maxAmpduLength)
    {
      if (padding)
        {
          Ptr<Packet> pad = Create<Packet> (padding);
          aggregatedPacket->AddAtEnd (pad);
        }
      currentHdr.SetCrc (1);
      currentHdr.SetSig ();
      currentHdr.SetLength (packet->GetSize ());
      currentPacket = packet->Copy ();

      currentPacket->AddHeader (currentHdr);
      aggregatedPacket->AddAtEnd (currentPacket);
      return true;
    }
  return false;
}

void
MpduStandardAggregator::AggregateVhtSingleMpdu (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> currentPacket;
  AmpduSubframeHeader currentHdr;

  uint32_t padding = CalculatePadding (aggregatedPacket);
  if (padding)
    {
      Ptr<Packet> pad = Create<Packet> (padding);
      aggregatedPacket->AddAtEnd (pad);
    }

  currentHdr.SetEof (1);
  currentHdr.SetCrc (1);
  currentHdr.SetSig ();
  currentHdr.SetLength (packet->GetSize ());
  currentPacket = packet->Copy ();

  currentPacket->AddHeader (currentHdr);
  aggregatedPacket->AddAtEnd (currentPacket);
}

void
MpduStandardAggregator::AddHeaderAndPad (Ptr<Packet> packet, bool last, bool vhtSingleMpdu)
{
  NS_LOG_FUNCTION (this);
  AmpduSubframeHeader currentHdr;

  //This is called to prepare packets from the aggregate queue to be sent so no need to check total size since it has already been
  //done before when deciding how many packets to add to the queue
  currentHdr.SetCrc (1);
  currentHdr.SetSig ();
  currentHdr.SetLength (packet->GetSize ());
  if (vhtSingleMpdu)
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
MpduStandardAggregator::CanBeAggregated (uint32_t packetSize, Ptr<Packet> aggregatedPacket, uint8_t blockAckSize)
{
  uint32_t padding = CalculatePadding (aggregatedPacket);
  uint32_t actualSize = aggregatedPacket->GetSize ();
  if (blockAckSize > 0)
    {
      blockAckSize = blockAckSize + 4 + padding;
    }
  if ((4 + packetSize + actualSize + padding + blockAckSize) <= m_maxAmpduLength)
    {
      return true;
    }
  else
    {
      return false;
    }
}

uint32_t
MpduStandardAggregator::CalculatePadding (Ptr<const Packet> packet)
{
  return (4 - (packet->GetSize () % 4 )) % 4;
}

} //namespace ns3
