/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#include "ns3/log.h"
#include "ns3/uinteger.h"

#include "amsdu-subframe-header.h"
#include "msdu-standard-aggregator.h"

NS_LOG_COMPONENT_DEFINE ("MsduStandardAggregator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MsduStandardAggregator);

TypeId
MsduStandardAggregator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MsduStandardAggregator")
    .SetParent<MsduAggregator> ()
    .AddConstructor<MsduStandardAggregator> ()
    .AddAttribute ("MaxAmsduSize", "Max length in byte of an A-MSDU",
                   UintegerValue (7935),
                   MakeUintegerAccessor (&MsduStandardAggregator::m_maxAmsduLength),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

MsduStandardAggregator::MsduStandardAggregator ()
{
}

MsduStandardAggregator::~MsduStandardAggregator ()
{
}

bool
MsduStandardAggregator::Aggregate (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket,
                                   Mac48Address src, Mac48Address dest)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> currentPacket;
  AmsduSubframeHeader currentHdr;

  uint32_t padding = CalculatePadding (aggregatedPacket);
  uint32_t actualSize = aggregatedPacket->GetSize ();

  if ((14 + packet->GetSize () + actualSize + padding) <= m_maxAmsduLength)
    {
      if (padding)
        {
          Ptr<Packet> pad = Create<Packet> (padding);
          aggregatedPacket->AddAtEnd (pad);
        }
      currentHdr.SetDestinationAddr (dest);
      currentHdr.SetSourceAddr (src);
      currentHdr.SetLength (packet->GetSize ());
      currentPacket = packet->Copy ();

      currentPacket->AddHeader (currentHdr);
      aggregatedPacket->AddAtEnd (currentPacket);
      return true;
    }
  return false;
}

uint32_t
MsduStandardAggregator::CalculatePadding (Ptr<const Packet> packet)
{
  return (4 - (packet->GetSize () % 4 )) % 4;
}

}  // namespace ns3
