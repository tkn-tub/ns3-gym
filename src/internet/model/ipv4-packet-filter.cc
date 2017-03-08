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
#include "ns3/uinteger.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
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

NS_OBJECT_ENSURE_REGISTERED (FqCoDelIpv4PacketFilter);

TypeId
FqCoDelIpv4PacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FqCoDelIpv4PacketFilter")
    .SetParent<Ipv4PacketFilter> ()
    .SetGroupName ("Internet")
    .AddConstructor<FqCoDelIpv4PacketFilter> ()
    .AddAttribute ("Perturbation",
                   "The salt used as an additional input to the hash function of this filter",
                   UintegerValue (0),
                   MakeUintegerAccessor (&FqCoDelIpv4PacketFilter::m_perturbation),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

FqCoDelIpv4PacketFilter::FqCoDelIpv4PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

FqCoDelIpv4PacketFilter::~FqCoDelIpv4PacketFilter ()
{
  NS_LOG_FUNCTION (this);
}

int32_t
FqCoDelIpv4PacketFilter::DoClassify (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION (this << item);
  Ptr<Ipv4QueueDiscItem> ipv4Item = DynamicCast<Ipv4QueueDiscItem> (item);

  NS_ASSERT (ipv4Item != 0);

  Ipv4Header hdr = ipv4Item->GetHeader ();
  Ipv4Address src = hdr.GetSource ();
  Ipv4Address dest = hdr.GetDestination ();
  uint8_t prot = hdr.GetProtocol ();
  uint16_t fragOffset = hdr.GetFragmentOffset ();

  TcpHeader tcpHdr;
  UdpHeader udpHdr;
  uint16_t srcPort = 0;
  uint16_t destPort = 0;

  Ptr<Packet> pkt = ipv4Item->GetPacket ();

  if (prot == 6 && fragOffset == 0) // TCP
    {
      pkt->PeekHeader (tcpHdr);
      srcPort = tcpHdr.GetSourcePort ();
      destPort = tcpHdr.GetDestinationPort ();
    }
  else if (prot == 17 && fragOffset == 0) // UDP
    {
      pkt->PeekHeader (udpHdr);
      srcPort = udpHdr.GetSourcePort ();
      destPort = udpHdr.GetDestinationPort ();
    }

  /* serialize the 5-tuple and the perturbation in buf */
  uint8_t buf[17];
  src.Serialize (buf);
  dest.Serialize (buf + 4);
  buf[8] = prot;
  buf[9] = (srcPort >> 8) & 0xff;
  buf[10] = srcPort & 0xff;
  buf[11] = (destPort >> 8) & 0xff;
  buf[12] = destPort & 0xff;
  buf[13] = (m_perturbation >> 24) & 0xff;
  buf[14] = (m_perturbation >> 16) & 0xff;
  buf[15] = (m_perturbation >> 8) & 0xff;
  buf[16] = m_perturbation & 0xff;

  /* Linux calculates the jhash2 (jenkins hash), we calculate the murmur3 */
  uint32_t hash = Hash32 ((char*) buf, 17);

  NS_LOG_DEBUG ("Found Ipv4 packet; hash value " << hash);

  return hash;
}

} // namespace ns3
