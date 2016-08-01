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
 *           Pasquale Imputato <p.imputato@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
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

NS_OBJECT_ENSURE_REGISTERED (FqCoDelIpv6PacketFilter);

TypeId
FqCoDelIpv6PacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FqCoDelIpv6PacketFilter")
    .SetParent<Ipv6PacketFilter> ()
    .SetGroupName ("Internet")
    .AddConstructor<FqCoDelIpv6PacketFilter> ()
    .AddAttribute ("Perturbation",
                   "The salt used as an additional input to the hash function of this filter",
                   UintegerValue (0),
                   MakeUintegerAccessor (&FqCoDelIpv6PacketFilter::m_perturbation),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

FqCoDelIpv6PacketFilter::FqCoDelIpv6PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

FqCoDelIpv6PacketFilter::~FqCoDelIpv6PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

int32_t
FqCoDelIpv6PacketFilter::DoClassify (Ptr< QueueDiscItem > item) const
{
  NS_LOG_FUNCTION (this << item);
  Ptr<Ipv6QueueDiscItem> ipv6Item = DynamicCast<Ipv6QueueDiscItem> (item);

  NS_ASSERT (ipv6Item != 0);

  Ipv6Header hdr = ipv6Item->GetHeader ();
  Ipv6Address src = hdr.GetSourceAddress ();
  Ipv6Address dest = hdr.GetDestinationAddress ();
  uint8_t prot = hdr.GetNextHeader ();

  TcpHeader tcpHdr;
  UdpHeader udpHdr;
  uint16_t srcPort = 0;
  uint16_t destPort = 0;

  Ptr<Packet> pkt = ipv6Item->GetPacket ();

  if (prot == 6) // TCP
    {
      pkt->PeekHeader (tcpHdr);
      srcPort = tcpHdr.GetSourcePort ();
      destPort = tcpHdr.GetDestinationPort ();
    }
  else if (prot == 17) // UDP
    {
      pkt->PeekHeader (udpHdr);
      srcPort = udpHdr.GetSourcePort ();
      destPort = udpHdr.GetDestinationPort ();
    }

  /* serialize the 5-tuple and the perturbation in buf */
  uint8_t buf[41];
  src.Serialize (buf);
  dest.Serialize (buf + 16);
  buf[32] = prot;
  buf[33] = (srcPort >> 8) & 0xff;
  buf[34] = srcPort & 0xff;
  buf[35] = (destPort >> 8) & 0xff;
  buf[36] = destPort & 0xff;
  buf[37] = (m_perturbation >> 24) & 0xff;
  buf[38] = (m_perturbation >> 16) & 0xff;
  buf[39] = (m_perturbation >> 8) & 0xff;
  buf[40] = m_perturbation & 0xff;

  /* Linux calculates the jhash2 (jenkins hash), we calculate the murmur3 */
  uint32_t hash = Hash32 ((char*) buf, 41);

  NS_LOG_DEBUG ("Found Ipv6 packet; hash of the five tuple " << hash);

  return hash;
}

} // namespace ns3
