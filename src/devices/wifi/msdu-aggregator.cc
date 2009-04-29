/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "msdu-aggregator.h"
#include "wifi-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("MsduAggregator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MsduAggregator);

TypeId
MsduAggregator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MsduAggregator")
    .SetParent<Object> ()
    ;
  return tid;
}

MsduAggregator::DeaggregatedMsdus
MsduAggregator::Deaggregate (Ptr<Packet> aggregatedPacket)
{
  NS_LOG_FUNCTION_NOARGS ();
  DeaggregatedMsdus set;
  
  AmsduSubframeHeader hdr;
  uint32_t maxSize = aggregatedPacket->GetSize ();
  // The worst condition is: two aggregated packets with no padding.
  // 28 bytes is the size of two Amsdu subframe headers.
  uint8_t *buffer = new uint8_t[maxSize-28];
  uint32_t padding;
  uint32_t deserialized = 0;

  while (deserialized < maxSize)
   {
     deserialized += aggregatedPacket->RemoveHeader (hdr);
     deserialized += aggregatedPacket->CopyData (buffer, hdr.GetLength ());
     aggregatedPacket->RemoveAtStart (hdr.GetLength ());
     
     padding = (4 - ((hdr.GetLength () + 14) %4 )) % 4;
  
     if (padding > 0)
       {
         aggregatedPacket->RemoveAtStart (padding);
         deserialized += padding;
       }
     //a new packet is created with the content of extracted msdu
     Ptr<Packet> p = Create<Packet> (buffer, hdr.GetLength ());
     
     std::pair<Ptr<Packet>, AmsduSubframeHeader> packetHdr (p,hdr);
     set.push_back (packetHdr);
   }
  delete [] buffer;
  NS_LOG_INFO ("Deaggreated A-MSDU: extracted "<< set.size () << " MSDUs");
  return set;
}

} //namespace ns3
