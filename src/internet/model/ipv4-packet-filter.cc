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
#include "ipv4-queue-disc-item.h"
#include "ipv4-packet-filter.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv4PacketFilter");

NS_OBJECT_ENSURE_REGISTERED (Ipv4PacketFilter);

TypeId 
Ipv4PacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4PacketFilter")
    .SetParent<PacketFilter> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

Ipv4PacketFilter::Ipv4PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

Ipv4PacketFilter::~Ipv4PacketFilter()
{
  NS_LOG_FUNCTION (this);
}

bool
Ipv4PacketFilter::CheckProtocol (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION (this << item);
  return (DynamicCast<Ipv4QueueDiscItem> (item) != 0);
}

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED (PfifoFastIpv4PacketFilter);

TypeId 
PfifoFastIpv4PacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PfifoFastIpv4PacketFilter")
    .SetParent<Ipv4PacketFilter> ()
    .SetGroupName ("Internet")
    .AddConstructor<PfifoFastIpv4PacketFilter> ()
    .AddAttribute ("Mode",
                   "Whether to interpret the TOS byte as legacy TOS or DSCP",
                   EnumValue (PF_MODE_DSCP),
                   MakeEnumAccessor (&PfifoFastIpv4PacketFilter::m_trafficClassMode),
                   MakeEnumChecker (PF_MODE_TOS, "TOS semantics",
                                    PF_MODE_DSCP, "DSCP semantics"))
  ;
  return tid;
}

PfifoFastIpv4PacketFilter::PfifoFastIpv4PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

PfifoFastIpv4PacketFilter::~PfifoFastIpv4PacketFilter()
{
  NS_LOG_FUNCTION (this);
}

int32_t
PfifoFastIpv4PacketFilter::DoClassify (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION (this << item);
  uint32_t band;
  Ptr<Ipv4QueueDiscItem> ipv4Item = DynamicCast<Ipv4QueueDiscItem> (item);

  NS_ASSERT (ipv4Item != 0);

  if (m_trafficClassMode == PF_MODE_TOS)
    {
      uint8_t tos = ipv4Item->GetHeader ().GetTos ();
      band = TosToBand (tos);
      NS_LOG_DEBUG ("Found Ipv4 packet; TOS " << (uint8_t) tos << " band " << band);
    }
  else
    {
      Ipv4Header::DscpType dscp = ipv4Item->GetHeader ().GetDscp ();
      band = DscpToBand (dscp);
      NS_LOG_DEBUG ("Found Ipv4 packet; DSCP " << ipv4Item->GetHeader ().DscpTypeToString (dscp) << " band " << band);
    }

  return band;
}

uint32_t
PfifoFastIpv4PacketFilter::TosToBand (uint8_t tos) const
{
  NS_LOG_FUNCTION (this << (uint16_t) tos);

  uint32_t band = 1;
  switch (tos) {
    case 0x10 :
    case 0x12 :
    case 0x14 :
    case 0x16 :
      band = 0;
      break;
    case 0x0 :
    case 0x4 :
    case 0x6 :
    case 0x18 :
    case 0x1a :
    case 0x1c :
    case 0x1e :
      band = 1;
      break;
    case 0x2 :
    case 0x8 :
    case 0xa :
    case 0xc :
    case 0xe :
      band = 2;
      break;
    default :
      NS_LOG_ERROR ("Invalid TOS " << (uint16_t) tos);
  }
  return band;
}

uint32_t
PfifoFastIpv4PacketFilter::DscpToBand (Ipv4Header::DscpType dscpType) const
{
  NS_LOG_FUNCTION (this);

  uint32_t band = 1;
  switch (dscpType) {
    case Ipv4Header::DSCP_EF :
    case Ipv4Header::DSCP_AF13 :
    case Ipv4Header::DSCP_AF23 :
    case Ipv4Header::DSCP_AF33 :
    case Ipv4Header::DSCP_AF43 :
    case Ipv4Header::DscpDefault :
    case Ipv4Header::DSCP_CS2 :
    case Ipv4Header::DSCP_CS3 :
      band = 1;
      break;
    case Ipv4Header::DSCP_AF11 :
    case Ipv4Header::DSCP_AF21 :
    case Ipv4Header::DSCP_AF31 :
    case Ipv4Header::DSCP_AF41 :
    case Ipv4Header::DSCP_CS1 :
      band = 2;
      break;
    case Ipv4Header::DSCP_AF12 :
    case Ipv4Header::DSCP_AF22 :
    case Ipv4Header::DSCP_AF32 :
    case Ipv4Header::DSCP_AF42 :
    case Ipv4Header::DSCP_CS4 :
    case Ipv4Header::DSCP_CS5 :
    case Ipv4Header::DSCP_CS6 :
    case Ipv4Header::DSCP_CS7 :
      band = 0;
      break;
    default :
      band = 1;
  }
  NS_LOG_DEBUG ("Band returned:  " << band);
  return band;
}

} // namespace ns3
