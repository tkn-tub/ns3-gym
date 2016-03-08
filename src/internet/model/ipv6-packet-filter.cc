/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
 *               2016 University of Washington
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
 * Authors:  Stefano Avallone <stavallo@unina.it>
 *           Tom Henderson <tomhend@u.washington.edu>
 */

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ipv6-queue-disc-item.h"
#include "ipv6-packet-filter.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv6PacketFilter");

NS_OBJECT_ENSURE_REGISTERED (Ipv6PacketFilter);

TypeId 
Ipv6PacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv6PacketFilter")
    .SetParent<PacketFilter> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

Ipv6PacketFilter::Ipv6PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

Ipv6PacketFilter::~Ipv6PacketFilter()
{
  NS_LOG_FUNCTION (this);
}

bool
Ipv6PacketFilter::CheckProtocol (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION (this << item);
  return (DynamicCast<Ipv6QueueDiscItem> (item) != 0);
}

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED (PfifoFastIpv6PacketFilter);

TypeId 
PfifoFastIpv6PacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PfifoFastIpv6PacketFilter")
    .SetParent<Ipv6PacketFilter> ()
    .SetGroupName ("Internet")
    .AddConstructor<PfifoFastIpv6PacketFilter> ()
  ;
  return tid;
}

PfifoFastIpv6PacketFilter::PfifoFastIpv6PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

PfifoFastIpv6PacketFilter::~PfifoFastIpv6PacketFilter()
{
  NS_LOG_FUNCTION (this);
}

int32_t
PfifoFastIpv6PacketFilter::DoClassify (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION (this << item);
  uint32_t band;
  Ptr<Ipv6QueueDiscItem> ipv6Item = DynamicCast<Ipv6QueueDiscItem> (item);

  NS_ASSERT (ipv6Item != 0);

  Ipv6Header::DscpType dscp = ipv6Item->GetHeader ().GetDscp ();
  band = DscpToBand (dscp);
  NS_LOG_DEBUG ("Found Ipv6 packet; DSCP " << ipv6Item->GetHeader ().DscpTypeToString (dscp) << " band " << band);

  return band;
}

uint32_t
PfifoFastIpv6PacketFilter::DscpToBand (Ipv6Header::DscpType dscpType) const
{
  NS_LOG_FUNCTION (this);

  uint32_t band = 1;
  switch (dscpType) {
    case Ipv6Header::DSCP_EF :
    case Ipv6Header::DSCP_AF13 :
    case Ipv6Header::DSCP_AF23 :
    case Ipv6Header::DSCP_AF33 :
    case Ipv6Header::DSCP_AF43 :
    case Ipv6Header::DscpDefault :
    case Ipv6Header::DSCP_CS2 :
    case Ipv6Header::DSCP_CS3 :
      band = 1;
      break;
    case Ipv6Header::DSCP_AF11 :
    case Ipv6Header::DSCP_AF21 :
    case Ipv6Header::DSCP_AF31 :
    case Ipv6Header::DSCP_AF41 :
    case Ipv6Header::DSCP_CS1 :
      band = 2;
      break;
    case Ipv6Header::DSCP_AF12 :
    case Ipv6Header::DSCP_AF22 :
    case Ipv6Header::DSCP_AF32 :
    case Ipv6Header::DSCP_AF42 :
    case Ipv6Header::DSCP_CS4 :
    case Ipv6Header::DSCP_CS5 :
    case Ipv6Header::DSCP_CS6 :
    case Ipv6Header::DSCP_CS7 :
      band = 0;
      break;
    default :
      band = 1;
  }
  NS_LOG_DEBUG ("Band returned:  " << band);
  return band;
}

} // namespace ns3
