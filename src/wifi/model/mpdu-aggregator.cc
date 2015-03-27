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

#include "mpdu-aggregator.h"
#include "wifi-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("MpduAggregator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MpduAggregator);

TypeId
MpduAggregator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MpduAggregator")
    .SetGroupName ("Wifi")
    .SetParent<Object> ()
  ;
  return tid;
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


} // namespace ns3
