/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 *         David Gross <gdavid.devel@gmail.com>
 *         Mehdi Benamor <benamor.mehdi@ensi.rnu.tn>
 *         Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/boolean.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ns3/ipv6-route.h"

#include "ipv6-raw-socket-factory-impl.h"
#include "ipv6-l3-protocol.h"
#include "ipv6-interface.h"
#include "icmpv6-l4-protocol.h"
#include "ndisc-cache.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Icmpv6L4Protocol);

NS_LOG_COMPONENT_DEFINE ("Icmpv6L4Protocol");

const uint8_t Icmpv6L4Protocol::PROT_NUMBER = 58;

const uint8_t Icmpv6L4Protocol::MAX_INITIAL_RTR_ADVERT_INTERVAL = 16;
const uint8_t Icmpv6L4Protocol::MAX_INITIAL_RTR_ADVERTISEMENTS = 3;
const uint8_t Icmpv6L4Protocol::MAX_FINAL_RTR_ADVERTISEMENTS = 3;
const uint8_t Icmpv6L4Protocol::MIN_DELAY_BETWEEN_RAS = 3;
const uint32_t Icmpv6L4Protocol::MAX_RA_DELAY_TIME = 500; /* millisecond */

const uint8_t Icmpv6L4Protocol::MAX_RTR_SOLICITATION_DELAY = 1;
const uint8_t Icmpv6L4Protocol::RTR_SOLICITATION_INTERVAL = 4;
const uint8_t Icmpv6L4Protocol::MAX_RTR_SOLICITATIONS = 3;

const uint8_t Icmpv6L4Protocol::MAX_MULTICAST_SOLICIT = 3;
const uint8_t Icmpv6L4Protocol::MAX_UNICAST_SOLICIT = 3;
const uint8_t Icmpv6L4Protocol::MAX_ANYCAST_DELAY_TIME = 1;
const uint8_t Icmpv6L4Protocol::MAX_NEIGHBOR_ADVERTISEMENT = 3;
const uint32_t Icmpv6L4Protocol::REACHABLE_TIME = 30000;
const uint32_t Icmpv6L4Protocol::RETRANS_TIMER = 1000;
const uint8_t Icmpv6L4Protocol::DELAY_FIRST_PROBE_TIME = 5;
const double Icmpv6L4Protocol::MIN_RANDOM_FACTOR = 0.5;
const double Icmpv6L4Protocol::MAX_RANDOM_FACTOR = 1.5;

TypeId Icmpv6L4Protocol::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Icmpv6L4Protocol")
    .SetParent<IpL4Protocol> ()
    .AddConstructor<Icmpv6L4Protocol> ()
    .AddAttribute ("DAD", "Always do DAD check.",
                   BooleanValue (true),
                   MakeBooleanAccessor (&Icmpv6L4Protocol::m_alwaysDad),
                   MakeBooleanChecker ())
  ;
  return tid;
}

Icmpv6L4Protocol::Icmpv6L4Protocol ()
  : m_node (0)
{
  NS_LOG_FUNCTION (this);
}

Icmpv6L4Protocol::~Icmpv6L4Protocol ()
{
  NS_LOG_FUNCTION (this);
}

void Icmpv6L4Protocol::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  for (CacheList::const_iterator it = m_cacheList.begin (); it != m_cacheList.end (); it++)
    {
      Ptr<NdiscCache> cache = *it;
      cache->Dispose ();
      cache = 0;
    }
  m_cacheList.clear ();
  m_downTarget.Nullify ();

  m_node = 0;
  IpL4Protocol::DoDispose ();
}

void Icmpv6L4Protocol::NotifyNewAggregate ()
{
  NS_LOG_FUNCTION (this);
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      if (node != 0)
        {
          Ptr<Ipv6L3Protocol> ipv6 = this->GetObject<Ipv6L3Protocol> ();
          if (ipv6 != 0 && m_downTarget.IsNull ())
            {
              SetNode (node);
              ipv6->Insert (this);
              Ptr<Ipv6RawSocketFactoryImpl> rawFactory = CreateObject<Ipv6RawSocketFactoryImpl> ();
              ipv6->AggregateObject (rawFactory);
              SetDownTarget6 (MakeCallback (&Ipv6L3Protocol::Send, ipv6));
            }
        }
    }
  Object::NotifyNewAggregate ();
}

void Icmpv6L4Protocol::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}

uint16_t Icmpv6L4Protocol::GetStaticProtocolNumber ()
{
  NS_LOG_FUNCTION_NOARGS ();
  return PROT_NUMBER;
}

int Icmpv6L4Protocol::GetProtocolNumber () const
{
  NS_LOG_FUNCTION (this);
  return PROT_NUMBER;
}

int Icmpv6L4Protocol::GetVersion () const
{
  NS_LOG_FUNCTION (this);
  return 1;
}

bool Icmpv6L4Protocol::IsAlwaysDad () const
{
  NS_LOG_FUNCTION (this);
  return m_alwaysDad;
}

void Icmpv6L4Protocol::DoDAD (Ipv6Address target, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << target << interface);
  Ipv6Address addr;
  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();

  NS_ASSERT (ipv6);

  if (!m_alwaysDad)
    {
      return;
    }

  /** \todo disable multicast loopback to prevent NS probing to be received by the sender */

  Ptr<Packet> p = ForgeNS ("::",Ipv6Address::MakeSolicitedAddress (target), target, interface->GetDevice ()->GetAddress ());

  /* update last packet UID */
  interface->SetNsDadUid (target, p->GetUid ());
  interface->Send (p, Ipv6Address::MakeSolicitedAddress (target));
}

enum IpL4Protocol::RxStatus Icmpv6L4Protocol::Receive (Ptr<Packet> packet, Ipv4Header const &header,  Ptr<Ipv4Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << header);
  return IpL4Protocol::RX_ENDPOINT_UNREACH;
}

enum IpL4Protocol::RxStatus Icmpv6L4Protocol::Receive (Ptr<Packet> packet, Ipv6Header const &header, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << header.GetSourceAddress () << header.GetDestinationAddress () << interface);
  Ptr<Packet> p = packet->Copy ();
  Ptr<Ipv6> ipv6 = m_node->GetObject<Ipv6> ();

  /* very ugly! try to find something better in the future */
  uint8_t type;
  p->CopyData (&type, sizeof(type));

  switch (type)
    {
    case Icmpv6Header::ICMPV6_ND_ROUTER_SOLICITATION:
      if (ipv6->IsForwarding (ipv6->GetInterfaceForDevice (interface->GetDevice ())))
        {
          HandleRS (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
        }
      break;
    case Icmpv6Header::ICMPV6_ND_ROUTER_ADVERTISEMENT:
      if (!ipv6->IsForwarding (ipv6->GetInterfaceForDevice (interface->GetDevice ())))
        {
          HandleRA (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
        }
      break;
    case Icmpv6Header::ICMPV6_ND_NEIGHBOR_SOLICITATION:
      HandleNS (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ND_NEIGHBOR_ADVERTISEMENT:
      HandleNA (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ND_REDIRECTION:
      HandleRedirection (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ECHO_REQUEST:
      HandleEchoRequest (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ECHO_REPLY:
      // EchoReply does not contain any info about L4
      // so we can not forward it up.
      /// \todo implement request / reply consistency check.
      break;
    case Icmpv6Header::ICMPV6_ERROR_DESTINATION_UNREACHABLE:
      HandleDestinationUnreachable (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ERROR_PACKET_TOO_BIG:
      HandlePacketTooBig (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ERROR_TIME_EXCEEDED:
      HandleTimeExceeded (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    case Icmpv6Header::ICMPV6_ERROR_PARAMETER_ERROR:
      HandleParameterError (p, header.GetSourceAddress (), header.GetDestinationAddress (), interface);
      break;
    default:
      NS_LOG_LOGIC ("Unknown ICMPv6 message type=" << type);
      break;
    }

  return IpL4Protocol::RX_OK;
}

void Icmpv6L4Protocol::Forward (Ipv6Address source, Icmpv6Header icmp,
                                uint32_t info, Ipv6Header ipHeader,
                                const uint8_t payload[8])
{
  NS_LOG_FUNCTION (this << source << icmp << info << ipHeader << payload);

  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();

  /// \todo assuming the ICMP is carrying a extensionless IP packet

  uint8_t nextHeader = ipHeader.GetNextHeader ();

  Ptr<IpL4Protocol> l4 = ipv6->GetProtocol (nextHeader);
  if (l4 != 0)
    {
      l4->ReceiveIcmp (source, ipHeader.GetHopLimit (), icmp.GetType (), icmp.GetCode (),
                       info, ipHeader.GetSourceAddress (), ipHeader.GetDestinationAddress (), payload);
    }
}

void Icmpv6L4Protocol::HandleEchoRequest (Ptr<Packet> packet, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << src << dst << interface);
  Icmpv6Echo request;
  uint8_t* buf = new uint8_t[packet->GetSize ()];

  packet->RemoveHeader (request);
  /* XXX IPv6 extension: obtain a fresh copy of data otherwise it crash... */
  packet->CopyData (buf, packet->GetSize ());
  Ptr<Packet> p = Create<Packet> (buf, packet->GetSize ());

  /* if we send message from ff02::* (link-local multicast), we use our link-local address */
  SendEchoReply (dst.IsMulticast () ? interface->GetLinkLocalAddress ().GetAddress () : dst, src, request.GetId (), request.GetSeq (), p);
  delete[] buf;
}

void Icmpv6L4Protocol::HandleRA (Ptr<Packet> packet, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << src << dst << interface);
  Ptr<Packet> p = packet->Copy ();
  Icmpv6RA raHeader;
  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();
  Icmpv6OptionPrefixInformation prefixHdr;
  Icmpv6OptionMtu mtuHdr;
  Icmpv6OptionLinkLayerAddress llaHdr;
  bool next = true;
  bool hasLla = false;
  bool hasMtu = false;
  Ipv6Address defaultRouter = Ipv6Address::GetZero ();

  p->RemoveHeader (raHeader);

  if (raHeader.GetLifeTime())
    {
      defaultRouter = src;
    }

  while (next == true)
    {
      uint8_t type = 0;
      p->CopyData (&type, sizeof(type));

      switch (type)
        {
        case Icmpv6Header::ICMPV6_OPT_PREFIX:
          p->RemoveHeader (prefixHdr);
          ipv6->AddAutoconfiguredAddress (ipv6->GetInterfaceForDevice (interface->GetDevice ()), prefixHdr.GetPrefix (), prefixHdr.GetPrefixLength (),
                                          prefixHdr.GetFlags (), prefixHdr.GetValidTime (), prefixHdr.GetPreferredTime (), defaultRouter);
          break;
        case Icmpv6Header::ICMPV6_OPT_MTU:
          /* take in account the first MTU option */
          if (!hasMtu)
            {
              p->RemoveHeader (mtuHdr);
              hasMtu = true;
              /** \todo case of multiple prefix on single interface */
              /* interface->GetDevice ()->SetMtu (m.GetMtu ()); */
            }
          break;
        case Icmpv6Header::ICMPV6_OPT_LINK_LAYER_SOURCE:
          /* take in account the first LLA option */
          if (!hasLla)
            {
              p->RemoveHeader (llaHdr);
              ReceiveLLA (llaHdr, src, dst, interface);
              hasLla = true;
            }
          break;
        default:
          /* unknow option, quit */
          next = false;
        }
    }
}

void Icmpv6L4Protocol::ReceiveLLA (Icmpv6OptionLinkLayerAddress lla, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << lla << src << dst << interface);
  Address hardwareAddress;
  NdiscCache::Entry* entry = 0;
  Ptr<NdiscCache> cache = FindCache (interface->GetDevice ());

  /* check if we have this address in our cache */
  entry = cache->Lookup (src);

  if (!entry)
    {
      entry = cache->Add (src);
      entry->SetRouter (true);
      entry->SetMacAddress (lla.GetAddress ());
      entry->MarkReachable ();
      entry->StartReachableTimer ();
    }
  else
    {
      std::list<Ptr<Packet> > waiting;
      if (entry->IsIncomplete ())
        {
          entry->StopRetransmitTimer ();
          // mark it to reachable
          waiting = entry->MarkReachable (lla.GetAddress ());
          entry->StopReachableTimer ();
          entry->StartReachableTimer ();
          // send out waiting packet
          for (std::list<Ptr<Packet> >::const_iterator it = waiting.begin (); it != waiting.end (); it++)
            {
              cache->GetInterface ()->Send (*it, src);
            }
          entry->ClearWaitingPacket ();
        }
      else
        {
          if (entry->GetMacAddress () != lla.GetAddress ())
            {
              entry->SetMacAddress (lla.GetAddress ());
              entry->MarkStale ();
              entry->SetRouter (true);
            }
          else
            {
              if (!entry->IsReachable ())
                {
                  entry->StopProbeTimer ();
                  entry->StopDelayTimer ();
                  waiting = entry->MarkReachable (lla.GetAddress ());
                  if (entry->IsProbe ())
                    {
                      for (std::list<Ptr<Packet> >::const_iterator it = waiting.begin (); it != waiting.end (); it++)
                        {
                          cache->GetInterface ()->Send (*it, src);
                        }
                    }
                  entry->StopReachableTimer ();
                  entry->StartReachableTimer ();
                }
            }
        }
    }
}

void Icmpv6L4Protocol::HandleRS (Ptr<Packet> packet, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << src << dst << interface);
  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();
  Icmpv6RS rsHeader;
  packet->RemoveHeader (rsHeader);
  Address hardwareAddress;
  Icmpv6OptionLinkLayerAddress lla (1);
  NdiscCache::Entry* entry = 0;
  Ptr<NdiscCache> cache = FindCache (interface->GetDevice ());

  if (src != Ipv6Address::GetAny ())
    {
      /* XXX search all options following the RS header */
      /* test if the next option is SourceLinkLayerAddress */
      uint8_t type;
      packet->CopyData (&type, sizeof(type));

      if (type != Icmpv6Header::ICMPV6_OPT_LINK_LAYER_SOURCE)
        {
          return;
        }
      packet->RemoveHeader (lla);
      NS_LOG_LOGIC ("Cache updated by RS");

      entry = cache->Lookup (src);
      if (!entry)
        {
          entry = cache->Add (src);
          entry->SetRouter (false);
          entry->MarkStale (lla.GetAddress ());
        }
      else if (entry->GetMacAddress () != lla.GetAddress ())
        {
          entry->MarkStale (lla.GetAddress ());
        }
    }
}

void Icmpv6L4Protocol::HandleNS (Ptr<Packet> packet, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << src << dst << interface);
  Icmpv6NS nsHeader ("::");
  Ipv6InterfaceAddress ifaddr;
  uint32_t nb = interface->GetNAddresses ();
  uint32_t i = 0;
  bool found = false;

  packet->RemoveHeader (nsHeader);

  Ipv6Address target = nsHeader.GetIpv6Target ();

  for (i = 0; i < nb; i++)
    {
      ifaddr = interface->GetAddress (i);

      if (ifaddr.GetAddress () == target)
        {
          found = true;
          break;
        }
    }

  if (!found)
    {
      NS_LOG_LOGIC ("Not a NS for us");
      return;
    }

  if (packet->GetUid () == ifaddr.GetNsDadUid ())
    {
      /* don't process our own DAD probe */
      NS_LOG_LOGIC ("Hey we receive our DAD probe!");
      return;
    }

  Icmpv6OptionLinkLayerAddress lla (1);
  Address hardwareAddress;
  NdiscCache::Entry* entry = 0;
  Ptr<NdiscCache> cache = FindCache (interface->GetDevice ());
  uint8_t flags = 0;

  /* XXX search all options following the NS header */

  if (src != Ipv6Address::GetAny ())
    {
      uint8_t type;
      packet->CopyData (&type, sizeof(type));

      if (type != Icmpv6Header::ICMPV6_OPT_LINK_LAYER_SOURCE)
        {
          return;
        }

      /* Get LLA */
      packet->RemoveHeader (lla);

      entry = cache->Lookup (src);
      if (!entry)
        {
          entry = cache->Add (src);
          entry->SetRouter (false);
          entry->MarkStale (lla.GetAddress ());
        }
      else if (entry->GetMacAddress () != lla.GetAddress ())
        {
          entry->MarkStale (lla.GetAddress ());
        }

      flags = 3; /* S + O flags */
    }
  else
    {
      /* it means someone do a DAD */
      flags = 1; /* O flag */
    }

  /* send a NA to src */
  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();

  if (ipv6->IsForwarding (ipv6->GetInterfaceForDevice (interface->GetDevice ())))
    {
      flags += 4; /* R flag */
    }

  hardwareAddress = interface->GetDevice ()->GetAddress ();
  Ptr<Packet> p = ForgeNA (target.IsLinkLocal () ? interface->GetLinkLocalAddress ().GetAddress () : ifaddr.GetAddress (), src.IsAny () ? Ipv6Address::GetAllNodesMulticast () : src, &hardwareAddress, flags );
  interface->Send (p,  src.IsAny () ? Ipv6Address::GetAllNodesMulticast () : src);

  /* not a NS for us discard it */
}

Ptr<Packet> Icmpv6L4Protocol::ForgeRS (Ipv6Address src, Ipv6Address dst, Address hardwareAddress)
{
  NS_LOG_FUNCTION (this << src << dst << hardwareAddress);
  Ptr<Packet> p = Create<Packet> ();
  Ipv6Header ipHeader;
  Icmpv6RS rs;
  Icmpv6OptionLinkLayerAddress llOption (1, hardwareAddress);  /* we give our mac address in response */

  NS_LOG_LOGIC ("Send RS ( from " << src << " to " << dst << ")");
  p->AddHeader (llOption);

  rs.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + rs.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (rs);

  ipHeader.SetSourceAddress (src);
  ipHeader.SetDestinationAddress (dst);
  ipHeader.SetNextHeader (PROT_NUMBER);
  ipHeader.SetPayloadLength (p->GetSize ());
  ipHeader.SetHopLimit (255);

  p->AddHeader (ipHeader);

  return p;
}

Ptr<Packet> Icmpv6L4Protocol::ForgeEchoRequest (Ipv6Address src, Ipv6Address dst, uint16_t id, uint16_t seq, Ptr<Packet> data)
{
  NS_LOG_FUNCTION (this << src << dst << id << seq << data);
  Ptr<Packet> p = data->Copy ();
  Ipv6Header ipHeader;
  Icmpv6Echo req (1);

  req.SetId (id);
  req.SetSeq (seq);

  p->AddHeader (req);

  return p;
}

void Icmpv6L4Protocol::HandleNA (Ptr<Packet> packet, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << src << dst << interface);
  Icmpv6NA naHeader;
  Icmpv6OptionLinkLayerAddress lla (1);

  packet->RemoveHeader (naHeader);
  Ipv6Address target = naHeader.GetIpv6Target ();

  Address hardwareAddress;
  NdiscCache::Entry* entry = 0;
  Ptr<NdiscCache> cache = FindCache (interface->GetDevice ());
  std::list<Ptr<Packet> > waiting;

  /* check if we have something in our cache */
  entry = cache->Lookup (target);

  if (!entry)
    {
      /* ouch!! we might be victim of a DAD */
      
      Ipv6InterfaceAddress ifaddr;
      bool found = false;
      uint32_t i = 0;
      uint32_t nb = interface->GetNAddresses ();

      for (i = 0; i < nb; i++)
        {
          ifaddr = interface->GetAddress (i);
          if (ifaddr.GetAddress () == target)
            {
              found = true;
              break;
            }
        }

      if (found)
        {
          if (ifaddr.GetState () == Ipv6InterfaceAddress::TENTATIVE || ifaddr.GetState () == Ipv6InterfaceAddress::TENTATIVE_OPTIMISTIC)
            {
              interface->SetState (ifaddr.GetAddress (), Ipv6InterfaceAddress::INVALID);
            }
        }

      /* we have not initiated any communication with the target so... discard the NA */
      return;
    }

  /* XXX search all options following the NA header */
  /* Get LLA */
  uint8_t type;
  packet->CopyData (&type, sizeof(type));

  if (type != Icmpv6Header::ICMPV6_OPT_LINK_LAYER_TARGET)
    {
      return;
    }
  packet->RemoveHeader (lla);

  if (entry->IsIncomplete ())
    {
      /* we receive a NA so stop the retransmission timer */
      entry->StopRetransmitTimer ();

      if (naHeader.GetFlagS ())
        {
          /* mark it to reachable */
          waiting = entry->MarkReachable (lla.GetAddress ());
          entry->StopReachableTimer ();
          entry->StartReachableTimer ();
          /* send out waiting packet */
          for (std::list<Ptr<Packet> >::const_iterator it = waiting.begin (); it != waiting.end (); it++)
            {
              cache->GetInterface ()->Send (*it, src);
            }
          entry->ClearWaitingPacket ();
        }
      else
        {
          entry->MarkStale (lla.GetAddress ());
        }

      if (naHeader.GetFlagR ())
        {
          entry->SetRouter (true);
        }
    }
  else
    {
      /* we receive a NA so stop the probe timer or delay timer if any */
      entry->StopProbeTimer ();
      entry->StopDelayTimer ();

      /* if the Flag O is clear and mac address differs from the cache */
      if (!naHeader.GetFlagO () && lla.GetAddress () != entry->GetMacAddress ())
        {
          if (entry->IsReachable ())
            {
              entry->MarkStale ();
            }
          return;
        }
      else
        {
          if ((!naHeader.GetFlagO () && lla.GetAddress () == entry->GetMacAddress ()) || naHeader.GetFlagO ()) /* XXX lake "no target link-layer address option supplied" */
            {
              entry->SetMacAddress (lla.GetAddress ());

              if (naHeader.GetFlagS ())
                {
                  if (!entry->IsReachable ())
                    {
                      if (entry->IsProbe ())
                        {
                          waiting = entry->MarkReachable (lla.GetAddress ());
                          for (std::list<Ptr<Packet> >::const_iterator it = waiting.begin (); it != waiting.end (); it++)
                            {
                              cache->GetInterface ()->Send (*it, src);
                            }
                          entry->ClearWaitingPacket ();
                        }
                      else
                        {
                          entry->MarkReachable (lla.GetAddress ());
                        }
                    }
                  entry->StopReachableTimer ();
                  entry->StartReachableTimer ();
                }
              else if (lla.GetAddress () != entry->GetMacAddress ())
                {
                  entry->MarkStale ();
                }
              entry->SetRouter (naHeader.GetFlagR ());
            }
        }
    }
}

void Icmpv6L4Protocol::HandleRedirection (Ptr<Packet> packet, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << src << dst << interface);
  bool hasLla = false;
  Ptr<Packet> p = packet->Copy ();
  Icmpv6OptionLinkLayerAddress llOptionHeader (0);

  Icmpv6Redirection redirectionHeader;
  p->RemoveHeader (redirectionHeader);

  /* little ugly try to find a better way */
  uint8_t type;
  p->CopyData (&type, sizeof(type));
  if (type == Icmpv6Header::ICMPV6_OPT_LINK_LAYER_TARGET)
    {
      hasLla = true;
      p->RemoveHeader (llOptionHeader);
    }

  Icmpv6OptionRedirected redirectedOptionHeader;
  p->RemoveHeader (redirectedOptionHeader);

  Ipv6Address redirTarget = redirectionHeader.GetTarget ();
  Ipv6Address redirDestination = redirectionHeader.GetDestination ();

  if (hasLla)
    {
      /* update the cache if needed */
      NdiscCache::Entry* entry = 0;
      Ptr<NdiscCache> cache = FindCache (interface->GetDevice ());

      entry = cache->Lookup (redirTarget);
      if (!entry)
        {
          entry = cache->Add (redirTarget);
          /* destination and target different => necessarily a router */
          entry->SetRouter (!redirTarget.IsEqual (redirDestination) ? true : false);
          entry->SetMacAddress (llOptionHeader.GetAddress ());
          entry->MarkStale ();
        }
      else
        {
          if (entry->IsIncomplete () || entry->GetMacAddress () != llOptionHeader.GetAddress ())
            {
              /* update entry to STALE */
              if (entry->GetMacAddress () != llOptionHeader.GetAddress ())
                {
                  entry->SetMacAddress (llOptionHeader.GetAddress ());
                  entry->MarkStale ();
                }
            }
          else
            {
              /* stay unchanged */
            }
        }
    }

  /* add redirection in routing table */
  Ptr<Ipv6> ipv6 = m_node->GetObject<Ipv6> ();

  if (redirTarget.IsEqual (redirDestination))
    {
      ipv6->GetRoutingProtocol ()->NotifyAddRoute (redirDestination, Ipv6Prefix (128), Ipv6Address ("::"), ipv6->GetInterfaceForAddress (dst));
    }
  else
    {
      uint32_t ifIndex = ipv6->GetInterfaceForAddress (dst);
      ipv6->GetRoutingProtocol ()->NotifyAddRoute (redirDestination, Ipv6Prefix (128), redirTarget, ifIndex);
    }
}

void Icmpv6L4Protocol::HandleDestinationUnreachable (Ptr<Packet> p, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << *p << src << dst << interface);
  Ptr<Packet> pkt = p->Copy ();

  Icmpv6DestinationUnreachable unreach;
  pkt->RemoveHeader (unreach);
  Ptr<Packet> origPkt = unreach.GetPacket ();

  Ipv6Header ipHeader;
  if ( origPkt->GetSerializedSize () > ipHeader.GetSerializedSize () )
    {
      origPkt->RemoveHeader (ipHeader);
      uint8_t payload[8];
      origPkt->CopyData (payload, 8);
      Forward (src, unreach, unreach.GetCode (), ipHeader, payload);
    }
}

void Icmpv6L4Protocol::HandleTimeExceeded (Ptr<Packet> p, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << *p << src << dst << interface);
  Ptr<Packet> pkt = p->Copy ();

  Icmpv6TimeExceeded timeexceeded;
  pkt->RemoveHeader (timeexceeded);
  Ptr<Packet> origPkt = timeexceeded.GetPacket ();
  Ipv6Header ipHeader;
  uint8_t payload[8];
  origPkt->RemoveHeader (ipHeader);
  origPkt->CopyData (payload, 8);

  Forward (src, timeexceeded, timeexceeded.GetCode (), ipHeader, payload);
}

void Icmpv6L4Protocol::HandlePacketTooBig (Ptr<Packet> p, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << *p << src << dst << interface);
  Ptr<Packet> pkt = p->Copy ();

  Icmpv6TooBig tooBig;
  pkt->RemoveHeader (tooBig);
  Ptr<Packet> origPkt = tooBig.GetPacket ();

  Ipv6Header ipHeader;
  origPkt->RemoveHeader (ipHeader);
  uint8_t payload[8];
  origPkt->CopyData (payload, 8);

  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();
  ipv6->SetPmtu(ipHeader.GetDestinationAddress(), tooBig.GetMtu ());

  Forward (src, tooBig, tooBig.GetMtu (), ipHeader, payload);
}

void Icmpv6L4Protocol::HandleParameterError (Ptr<Packet> p, Ipv6Address const &src, Ipv6Address const &dst, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << *p << src << dst << interface);
  Ptr<Packet> pkt = p->Copy ();

  Icmpv6ParameterError paramErr;
  pkt->RemoveHeader (paramErr);
  Ptr<Packet> origPkt = paramErr.GetPacket ();

  Ipv6Header ipHeader;
  origPkt->RemoveHeader (ipHeader);
  uint8_t payload[8];
  origPkt->CopyData (payload, 8);
  Forward (src, paramErr, paramErr.GetCode (), ipHeader, payload);
}

void Icmpv6L4Protocol::SendMessage (Ptr<Packet> packet, Ipv6Address src, Ipv6Address dst, uint8_t ttl)
{
  NS_LOG_FUNCTION (this << packet << src << dst << (uint32_t)ttl);
  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();
  SocketIpTtlTag tag;
  NS_ASSERT (ipv6 != 0);

  tag.SetTtl (ttl);
  packet->AddPacketTag (tag);
  m_downTarget (packet, src, dst, PROT_NUMBER, 0);
}

void Icmpv6L4Protocol::SendMessage (Ptr<Packet> packet, Ipv6Address dst, Icmpv6Header& icmpv6Hdr, uint8_t ttl)
{
  NS_LOG_FUNCTION (this << packet << dst << icmpv6Hdr << (uint32_t)ttl);
  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();
  NS_ASSERT (ipv6 != 0 && ipv6->GetRoutingProtocol () != 0);
  Ipv6Header header;
  SocketIpTtlTag tag;
  Socket::SocketErrno err;
  Ptr<Ipv6Route> route;
  Ptr<NetDevice> oif (0); //specify non-zero if bound to a source address

  header.SetDestinationAddress (dst);
  route = ipv6->GetRoutingProtocol ()->RouteOutput (packet, header, oif, err);

  if (route != 0)
    {
      NS_LOG_LOGIC ("Route exists");
      tag.SetTtl (ttl);
      packet->AddPacketTag (tag);
      Ipv6Address src = route->GetSource ();

      icmpv6Hdr.CalculatePseudoHeaderChecksum (src, dst, packet->GetSize () + icmpv6Hdr.GetSerializedSize (), PROT_NUMBER);
      packet->AddHeader (icmpv6Hdr);
      m_downTarget (packet, src, dst, PROT_NUMBER, route);
    }
  else
    {
      NS_LOG_WARN ("drop icmp message");
    }
}

void Icmpv6L4Protocol::SendNA (Ipv6Address src, Ipv6Address dst, Address* hardwareAddress, uint8_t flags)
{
  NS_LOG_FUNCTION (this << src << dst << hardwareAddress << static_cast<uint32_t> (flags));
  Ptr<Packet> p = Create<Packet> ();
  Icmpv6NA na;
  Icmpv6OptionLinkLayerAddress llOption (0, *hardwareAddress); /* not a source link layer */

  NS_LOG_LOGIC ("Send NA ( from " << src << " to " << dst << " target " << src << ")");
  na.SetIpv6Target (src);

  if ((flags & 1))
    {
      na.SetFlagO (true);
    }
  if ((flags & 2) && src != Ipv6Address::GetAny ())
    {
      na.SetFlagS (true);
    }
  if ((flags & 4))
    {
      na.SetFlagR (true);
    }

  p->AddHeader (llOption);
  na.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + na.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (na);

  SendMessage (p, src, dst, 255);
}

void Icmpv6L4Protocol::SendEchoReply (Ipv6Address src, Ipv6Address dst, uint16_t id, uint16_t seq, Ptr<Packet> data)
{
  NS_LOG_FUNCTION (this << src << dst << id << seq << data);
  Ptr<Packet> p = data->Copy ();
  Icmpv6Echo reply (0); /* echo reply */

  reply.SetId (id);
  reply.SetSeq (seq);

  reply.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + reply.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (reply);
  SendMessage (p, src, dst, 64);
}

void Icmpv6L4Protocol::SendNS (Ipv6Address src, Ipv6Address dst, Ipv6Address target, Address hardwareAddress)
{
  NS_LOG_FUNCTION (this << src << dst << target << hardwareAddress);
  Ptr<Packet> p = Create<Packet> ();
  /* Ipv6Header ipHeader; */
  Icmpv6NS ns (target);
  Icmpv6OptionLinkLayerAddress llOption (1, hardwareAddress);  /* we give our mac address in response */

  /* if the source is unspec, multicast the NA to all-nodes multicast */
  if (src == Ipv6Address::GetAny ())
    {
      dst = Ipv6Address::GetAllNodesMulticast ();
    }

  NS_LOG_LOGIC ("Send NS ( from " << src << " to " << dst << " target " << target << ")");

  p->AddHeader (llOption);
  ns.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + ns.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (ns);
  SendMessage (p, src, dst, 255);
}

void Icmpv6L4Protocol::SendRS (Ipv6Address src, Ipv6Address dst,  Address hardwareAddress)
{
  NS_LOG_FUNCTION (this << src << dst << hardwareAddress);
  Ptr<Packet> p = Create<Packet> ();
  Icmpv6RS rs;
  Icmpv6OptionLinkLayerAddress llOption (1, hardwareAddress);  /* we give our mac address in response */

  /* if the source is unspec, multicast the NA to all-nodes multicast */
  if (src != Ipv6Address::GetAny ())
    {
      p->AddHeader (llOption);
    }

  NS_LOG_LOGIC ("Send RS ( from " << src << " to " << dst << ")");

  rs.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + rs.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (rs);
  SendMessage (p, src, dst, 255);
}

void Icmpv6L4Protocol::SendErrorDestinationUnreachable (Ptr<Packet> malformedPacket, Ipv6Address dst, uint8_t code)
{
  NS_LOG_FUNCTION (this << malformedPacket << dst << (uint32_t)code);
  Ptr<Packet> p = Create<Packet> ();
  uint32_t malformedPacketSize = malformedPacket->GetSize ();
  Icmpv6DestinationUnreachable header;

  NS_LOG_LOGIC ("Send Destination Unreachable ( to " << dst << " code " << (uint32_t)code << " )");

  /* 48 = sizeof IPv6 header + sizeof ICMPv6 error header */
  if (malformedPacketSize <= 1280 - 48)
    {
      header.SetPacket (malformedPacket);
    }
  else
    {
      Ptr<Packet> fragment = malformedPacket->CreateFragment (0, 1280 - 48);
      header.SetPacket (fragment);
    }

  header.SetCode (code);
  SendMessage (p, dst, header, 255);
}

void Icmpv6L4Protocol::SendErrorTooBig (Ptr<Packet> malformedPacket, Ipv6Address dst, uint32_t mtu)
{
  NS_LOG_FUNCTION (this << malformedPacket << dst << mtu);
  Ptr<Packet> p = Create<Packet> ();
  uint32_t malformedPacketSize = malformedPacket->GetSize ();
  Icmpv6TooBig header;

  NS_LOG_LOGIC ("Send Too Big ( to " << dst << " )");

  /* 48 = sizeof IPv6 header + sizeof ICMPv6 error header */
  if (malformedPacketSize <= 1280 - 48)
    {
      header.SetPacket (malformedPacket);
    }
  else
    {
      Ptr<Packet> fragment = malformedPacket->CreateFragment (0, 1280 - 48);
      header.SetPacket (fragment);
    }

  header.SetCode (0);
  header.SetMtu (mtu);
  SendMessage (p, dst, header, 255);
}

void Icmpv6L4Protocol::SendErrorTimeExceeded (Ptr<Packet> malformedPacket, Ipv6Address dst, uint8_t code)
{
  NS_LOG_FUNCTION (this << malformedPacket << dst << static_cast<uint32_t> (code));
  Ptr<Packet> p = Create<Packet> ();
  uint32_t malformedPacketSize = malformedPacket->GetSize ();
  Icmpv6TimeExceeded header;

  NS_LOG_LOGIC ("Send Time Exceeded ( to " << dst << " code " << (uint32_t)code << " )");

  /* 48 = sizeof IPv6 header + sizeof ICMPv6 error header */
  if (malformedPacketSize <= 1280 - 48)
    {
      header.SetPacket (malformedPacket);
    }
  else
    {
      Ptr<Packet> fragment = malformedPacket->CreateFragment (0, 1280 - 48);
      header.SetPacket (fragment);
    }

  header.SetCode (code);
  SendMessage (p, dst, header, 255);
}

void Icmpv6L4Protocol::SendErrorParameterError (Ptr<Packet> malformedPacket, Ipv6Address dst, uint8_t code, uint32_t ptr)
{
  NS_LOG_FUNCTION (this << malformedPacket << dst << static_cast<uint32_t> (code) << ptr);
  Ptr<Packet> p = Create<Packet> ();
  uint32_t malformedPacketSize = malformedPacket->GetSize ();
  Icmpv6ParameterError header;

  NS_LOG_LOGIC ("Send Parameter Error ( to " << dst << " code " << (uint32_t)code << " )");

  /* 48 = sizeof IPv6 header + sizeof ICMPv6 error header */
  if (malformedPacketSize <= 1280 - 48 )
    {
      header.SetPacket (malformedPacket);
    }
  else
    {
      Ptr<Packet> fragment = malformedPacket->CreateFragment (0, 1280 - 48);
      header.SetPacket (fragment);
    }

  header.SetCode (code);
  header.SetPtr (ptr);
  SendMessage (p, dst, header, 255);
}

void Icmpv6L4Protocol::SendRedirection (Ptr<Packet> redirectedPacket, Ipv6Address src, Ipv6Address dst, Ipv6Address redirTarget, Ipv6Address redirDestination, Address redirHardwareTarget)
{
  NS_LOG_FUNCTION (this << redirectedPacket << dst << redirTarget << redirDestination << redirHardwareTarget);
  uint32_t llaSize = 0;
  Ptr<Packet> p = Create<Packet> ();
  uint32_t redirectedPacketSize = redirectedPacket->GetSize ();
  Icmpv6OptionLinkLayerAddress llOption (0);

  NS_LOG_LOGIC ("Send Redirection ( to " << dst << " target " << redirTarget << " destination " << redirDestination << " )");

  Icmpv6OptionRedirected redirectedOptionHeader;

  if ((redirectedPacketSize % 8) != 0)
    {
      Ptr<Packet> pad = Create<Packet> (8 - (redirectedPacketSize % 8));
      redirectedPacket->AddAtEnd (pad);
    }

  if (redirHardwareTarget.GetLength ())
    {
      llOption.SetAddress (redirHardwareTarget);
      llaSize = llOption.GetSerializedSize ();
    }

  /* 56 = sizeof IPv6 header + sizeof ICMPv6 error header + sizeof redirected option */
  if (redirectedPacketSize <= (1280 - 56 - llaSize))
    {
      redirectedOptionHeader.SetPacket (redirectedPacket);
    }
  else
    {
      Ptr<Packet> fragment = redirectedPacket->CreateFragment (0, 1280 - 56 - llaSize);
      redirectedOptionHeader.SetPacket (fragment);
    }

  p->AddHeader (redirectedOptionHeader);

  if (llaSize)
    {
      p->AddHeader (llOption);
    }

  Icmpv6Redirection redirectionHeader;
  redirectionHeader.SetTarget (redirTarget);
  redirectionHeader.SetDestination (redirDestination);
  redirectionHeader.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + redirectionHeader.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (redirectionHeader);

  SendMessage (p, src, dst, 64);
}

Ptr<Packet> Icmpv6L4Protocol::ForgeNA (Ipv6Address src, Ipv6Address dst, Address* hardwareAddress, uint8_t flags)
{
  NS_LOG_FUNCTION (this << src << dst << hardwareAddress << (uint32_t)flags);
  Ptr<Packet> p = Create<Packet> ();
  Ipv6Header ipHeader;
  Icmpv6NA na;
  Icmpv6OptionLinkLayerAddress llOption (0, *hardwareAddress);  /* we give our mac address in response */

  NS_LOG_LOGIC ("Send NA ( from " << src << " to " << dst << ")");

  /* forge the entire NA packet from IPv6 header to ICMPv6 link-layer option, so that the packet does not pass by Icmpv6L4Protocol::Lookup again */

  p->AddHeader (llOption);
  na.SetIpv6Target (src);

  if ((flags & 1))
    {
      na.SetFlagO (true);
    }
  if ((flags & 2) && src != Ipv6Address::GetAny ())
    {
      na.SetFlagS (true);
    }
  if ((flags & 4))
    {
      na.SetFlagR (true);
    }

  na.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + na.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (na);

  ipHeader.SetSourceAddress (src);
  ipHeader.SetDestinationAddress (dst);
  ipHeader.SetNextHeader (PROT_NUMBER);
  ipHeader.SetPayloadLength (p->GetSize ());
  ipHeader.SetHopLimit (255);

  p->AddHeader (ipHeader);

  return p;
}

Ptr<Packet> Icmpv6L4Protocol::ForgeNS (Ipv6Address src, Ipv6Address dst, Ipv6Address target, Address hardwareAddress)
{
  NS_LOG_FUNCTION (this << src << dst << target << hardwareAddress);
  Ptr<Packet> p = Create<Packet> ();
  Ipv6Header ipHeader;
  Icmpv6NS ns (target);
  Icmpv6OptionLinkLayerAddress llOption (1, hardwareAddress);  /* we give our mac address in response */

  /* if the source is unspec, multicast the NA to all-nodes multicast */
  if (src == Ipv6Address::GetAny ())
    {
      dst = Ipv6Address::GetAllNodesMulticast ();
    }

  NS_LOG_LOGIC ("Send NS ( from " << src << " to " << dst << " target " << target << ")");

  p->AddHeader (llOption);
  ns.CalculatePseudoHeaderChecksum (src, dst, p->GetSize () + ns.GetSerializedSize (), PROT_NUMBER);
  p->AddHeader (ns);

  ipHeader.SetSourceAddress (src);
  ipHeader.SetDestinationAddress (dst);
  ipHeader.SetNextHeader (PROT_NUMBER);
  ipHeader.SetPayloadLength (p->GetSize ());
  ipHeader.SetHopLimit (255);

  p->AddHeader (ipHeader);

  return p;
}

Ptr<NdiscCache> Icmpv6L4Protocol::FindCache (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);

  for (CacheList::const_iterator i = m_cacheList.begin (); i != m_cacheList.end (); i++)
    {
      if ((*i)->GetDevice () == device)
        {
          return *i;
        }
    }

  NS_ASSERT (false);
  /* quiet compiler */
  return 0;
}

Ptr<NdiscCache> Icmpv6L4Protocol::CreateCache (Ptr<NetDevice> device, Ptr<Ipv6Interface> interface)
{
  NS_LOG_FUNCTION (this << device << interface);

  Ptr<NdiscCache> cache = CreateObject<NdiscCache> ();

  cache->SetDevice (device, interface);
  device->AddLinkChangeCallback (MakeCallback (&NdiscCache::Flush, cache));
  m_cacheList.push_back (cache);
  return cache;
}

bool Icmpv6L4Protocol::Lookup (Ipv6Address dst, Ptr<NetDevice> device, Ptr<NdiscCache> cache, Address* hardwareDestination)
{
  NS_LOG_FUNCTION (this << dst << device << cache << hardwareDestination);

  if (!cache)
    {
      /* try to find the cache */
      cache = FindCache (device);
    }
  if (cache)
    {
      NdiscCache::Entry* entry = cache->Lookup (dst);
      if (entry)
        {
          if (entry->IsReachable () || entry->IsDelay ())
            {
              *hardwareDestination = entry->GetMacAddress ();
              return true;
            }
          else if (entry->IsStale ())
            {
              entry->StartDelayTimer ();
              entry->MarkDelay ();
              *hardwareDestination = entry->GetMacAddress ();
              return true;
            }
        }
    }
  return false;
}

bool Icmpv6L4Protocol::Lookup (Ptr<Packet> p, Ipv6Address dst, Ptr<NetDevice> device, Ptr<NdiscCache> cache, Address* hardwareDestination)
{
  NS_LOG_FUNCTION (this << p << dst << device << cache << hardwareDestination);

  if (!cache)
    {
      /* try to find the cache */
      cache = FindCache (device);
    }
  if (!cache)
    {
      return false;
    }

  NdiscCache::Entry* entry = cache->Lookup (dst);
  if (entry)
    {
      if (entry->IsReachable () || entry->IsDelay ())
        {
          /* XXX check reachability time */
          /* send packet */
          *hardwareDestination = entry->GetMacAddress ();
          return true;
        }
      else if (entry->IsStale ())
        {
          /* start delay timer */
          entry->StartDelayTimer ();
          entry->MarkDelay ();
          *hardwareDestination = entry->GetMacAddress ();
          return true;
        }
      else /* PROBE */
        {
          /* queue packet */
          entry->AddWaitingPacket (p);
          return false;
        }
    }
  else
    {
      /* we contact this node for the first time
       * add it to the cache and send an NS
       */
      Ipv6Address addr;
      NdiscCache::Entry* entry = cache->Add (dst);
      entry->MarkIncomplete (p);
      entry->SetRouter (false);

      if (dst.IsLinkLocal ())
        {
          addr = cache->GetInterface ()->GetLinkLocalAddress ().GetAddress ();
        }
      else if (cache->GetInterface ()->GetNAddresses () == 1) /* an interface have at least one address (link-local) */
        {
          /* try to resolve global address without having global address so return! */
          cache->Remove (entry);
          return false;
        }
      else
        {
          /* find source address that match destination */
          addr = cache->GetInterface ()->GetAddressMatchingDestination (dst).GetAddress ();
        }

      SendNS (addr, Ipv6Address::MakeSolicitedAddress (dst), dst, cache->GetDevice ()->GetAddress ());

      /* start retransmit timer */
      entry->StartRetransmitTimer ();
      return false;
    }

  return false;
}

void Icmpv6L4Protocol::FunctionDadTimeout (Ptr<Icmpv6L4Protocol> icmpv6, Ipv6Interface* interface, Ipv6Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC (interface << " " << addr);
  Ipv6InterfaceAddress ifaddr;
  bool found = false;
  uint32_t i = 0;
  uint32_t nb = interface->GetNAddresses ();

  for (i = 0; i < nb; i++)
    {
      ifaddr = interface->GetAddress (i);

      if (ifaddr.GetAddress () == addr)
        {
          found = true;
          break;
        }
    }

  /* for the moment, this function is always called, if we was victim of a DAD the address is INVALID
   * and we do not set it to PREFERRED
   */
  if (found && ifaddr.GetState () != Ipv6InterfaceAddress::INVALID)
    {
      interface->SetState (ifaddr.GetAddress (), Ipv6InterfaceAddress::PREFERRED);
      NS_LOG_LOGIC ("DAD OK, interface in state PREFERRED");

      /* send an RS if our interface is not forwarding (router) and if address is a link-local ones
       * (because we will send RS with it)
       */
      Ptr<Ipv6> ipv6 = icmpv6->m_node->GetObject<Ipv6> ();

      if (!ipv6->IsForwarding (ipv6->GetInterfaceForDevice (interface->GetDevice ())) && addr.IsLinkLocal ())
        {
          /* \todo Add random delays before sending RS
           * because all nodes start at the same time, there will be many of RS arround 1 second of simulation time
           */
          Simulator::Schedule (Seconds (0.0), &Icmpv6L4Protocol::SendRS, PeekPointer (icmpv6), ifaddr.GetAddress (), Ipv6Address::GetAllRoutersMulticast (), interface->GetDevice ()->GetAddress ());
        }
    }
}

void
Icmpv6L4Protocol::SetDownTarget (IpL4Protocol::DownTargetCallback callback)
{
  NS_LOG_FUNCTION (this << &callback);
}

void
Icmpv6L4Protocol::SetDownTarget6 (IpL4Protocol::DownTargetCallback6 callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_downTarget = callback;
}

IpL4Protocol::DownTargetCallback
Icmpv6L4Protocol::GetDownTarget (void) const
{
  NS_LOG_FUNCTION (this);
  return (IpL4Protocol::DownTargetCallback)NULL;
}

IpL4Protocol::DownTargetCallback6
Icmpv6L4Protocol::GetDownTarget6 (void) const
{
  NS_LOG_FUNCTION (this);
  return m_downTarget;
}

} /* namespace ns3 */

