/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/object-vector.h"
#include "ns3/trace-source-accessor.h"

#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "arp-header.h"
#include "arp-cache.h"
#include "ipv4-interface.h"

NS_LOG_COMPONENT_DEFINE ("ArpL3Protocol");

namespace ns3 {

const uint16_t ArpL3Protocol::PROT_NUMBER = 0x0806;

NS_OBJECT_ENSURE_REGISTERED (ArpL3Protocol);

TypeId 
ArpL3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ArpL3Protocol")
    .SetParent<Object> ()
    .AddAttribute ("CacheList",
                   "The list of ARP caches",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&ArpL3Protocol::m_cacheList),
                   MakeObjectVectorChecker<ArpCache> ())
    .AddTraceSource ("Drop",
                     "Packet dropped because not enough room in pending queue for a specific cache entry.",
                     MakeTraceSourceAccessor (&ArpL3Protocol::m_dropTrace))
    ;
  return tid;
}

ArpL3Protocol::ArpL3Protocol ()
{
  NS_LOG_FUNCTION (this);
}

ArpL3Protocol::~ArpL3Protocol ()
{
  NS_LOG_FUNCTION (this);
}

void 
ArpL3Protocol::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this);
  m_node = node;
}

void 
ArpL3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (CacheList::iterator i = m_cacheList.begin (); i != m_cacheList.end (); ++i)
    {
      Ptr<ArpCache> cache = *i;
      cache->Dispose ();
    }
  m_cacheList.clear ();
  m_node = 0;
  Object::DoDispose ();
}

Ptr<ArpCache> 
ArpL3Protocol::CreateCache (Ptr<NetDevice> device, Ptr<Ipv4Interface> interface)
{
  NS_LOG_FUNCTION (this << device << interface);
  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  Ptr<ArpCache> cache = CreateObject<ArpCache> ();
  cache->SetDevice (device, interface);
  NS_ASSERT (device->IsBroadcast ());
  device->SetLinkChangeCallback (MakeCallback (&ArpCache::Flush, cache));
  cache->SetArpRequestCallback (MakeCallback (&ArpL3Protocol::SendArpRequest, this));
  m_cacheList.push_back (cache);
  return cache;
}

Ptr<ArpCache>
ArpL3Protocol::FindCache (Ptr<NetDevice> device)
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
  // quiet compiler
  return 0;
}

void 
ArpL3Protocol::Receive(Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                       const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << device << p->GetSize () << protocol << from << to << packetType);

  Ptr<Packet> packet = p->Copy ();

  Ptr<ArpCache> cache = FindCache (device);
  ArpHeader arp;
  packet->RemoveHeader (arp);
  
  NS_LOG_LOGIC ("ARP: received "<< (arp.IsRequest ()? "request" : "reply") <<
            " node="<<m_node->GetId ()<<", got request from " <<
            arp.GetSourceIpv4Address () << " for address " <<
            arp.GetDestinationIpv4Address () << "; we have address " <<
            cache->GetInterface ()->GetAddress ());

  /**
   * Note: we do not update the ARP cache when we receive an ARP request
   * from an unknown node. See bug #107
   */

  if (arp.IsRequest () && 
      arp.GetDestinationIpv4Address () == cache->GetInterface ()->GetAddress ()) 
    {
      NS_LOG_LOGIC ("node="<<m_node->GetId () <<", got request from " << 
                arp.GetSourceIpv4Address () << " -- send reply");
      SendArpReply (cache, arp.GetSourceIpv4Address (),
                    arp.GetSourceHardwareAddress ());
    } 
  else if (arp.IsReply () &&
           arp.GetDestinationIpv4Address ().IsEqual (cache->GetInterface ()->GetAddress ()) &&
           arp.GetDestinationHardwareAddress () == device->GetAddress ()) 
    {
      Ipv4Address from = arp.GetSourceIpv4Address ();
      ArpCache::Entry *entry = cache->Lookup (from);
      if (entry != 0)
        {
          if (entry->IsWaitReply ()) 
            {
              NS_LOG_LOGIC ("node="<<m_node->GetId ()<<", got reply from " << 
                        arp.GetSourceIpv4Address ()
                     << " for waiting entry -- flush");
              Address from_mac = arp.GetSourceHardwareAddress ();
              entry->MarkAlive (from_mac);
              Ptr<Packet> pending = entry->DequeuePending();
              while (pending != 0)
                {
                  cache->GetInterface ()->Send (pending,
                                                arp.GetSourceIpv4Address ());
                  pending = entry->DequeuePending();
                }
            } 
          else 
            {
              // ignore this reply which might well be an attempt 
              // at poisening my arp cache.
              NS_LOG_LOGIC("node="<<m_node->GetId ()<<", got reply from " << 
                        arp.GetSourceIpv4Address () << 
                        " for non-waiting entry -- drop");
              m_dropTrace (packet);
            }
        } 
      else 
        {
          NS_LOG_LOGIC ("node="<<m_node->GetId ()<<", got reply for unknown entry -- drop");
          m_dropTrace (packet);
        }
    }
  else
    {
      NS_LOG_LOGIC ("node="<<m_node->GetId ()<<", got request from " <<
                arp.GetSourceIpv4Address () << " for unknown address " <<
                arp.GetDestinationIpv4Address () << " -- drop");
    }
}
bool 
ArpL3Protocol::Lookup (Ptr<Packet> packet, Ipv4Address destination, 
                       Ptr<NetDevice> device,
                       Ptr<ArpCache> cache,
                       Address *hardwareDestination)
{
  NS_LOG_FUNCTION (this << packet << destination << device << cache);
  ArpCache::Entry *entry = cache->Lookup (destination);
  if (entry != 0)
    {
      if (entry->IsExpired ()) 
        {
          if (entry->IsDead ()) 
            {
              NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                        ", dead entry for " << destination << " expired -- send arp request");
              entry->MarkWaitReply (packet);
              SendArpRequest (cache, destination);
            } 
          else if (entry->IsAlive ()) 
            {
              NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                        ", alive entry for " << destination << " expired -- send arp request");
              entry->MarkWaitReply (packet);
              SendArpRequest (cache, destination);
            } 
          else if (entry->IsWaitReply ()) 
            {
              NS_FATAL_ERROR ("Test for possibly unreachable code-- please file a bug report, with a test case, if this is ever hit");
            }
        } 
      else 
        {
          if (entry->IsDead ()) 
            {
              NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                            ", dead entry for " << destination << " valid -- drop");
              m_dropTrace (packet);
            } 
          else if (entry->IsAlive ()) 
            {
              NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                            ", alive entry for " << destination << " valid -- send");
	      *hardwareDestination = entry->GetMacAddress ();
              return true;
            } 
          else if (entry->IsWaitReply ()) 
            {
              NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                            ", wait reply for " << destination << " valid -- drop previous");
              if (!entry->UpdateWaitReply (packet))
                {
                  m_dropTrace (packet);
                }
            }
        }
    }
  else
    {
      // This is our first attempt to transmit data to this destination.
      NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                ", no entry for " << destination << " -- send arp request");
      entry = cache->Add (destination);
      entry->MarkWaitReply (packet);
      SendArpRequest (cache, destination);
    }
  return false;
}

void
ArpL3Protocol::SendArpRequest (Ptr<const ArpCache> cache, Ipv4Address to)
{
  NS_LOG_FUNCTION (this << cache << to);
  ArpHeader arp;
  NS_LOG_LOGIC ("ARP: sending request from node "<<m_node->GetId ()<<
            " || src: " << cache->GetDevice ()->GetAddress () <<
            " / " << cache->GetInterface ()->GetAddress () <<
            " || dst: " << cache->GetDevice ()->GetBroadcast () <<
            " / " << to);
  arp.SetRequest (cache->GetDevice ()->GetAddress (),
		  cache->GetInterface ()->GetAddress (), 
                  cache->GetDevice ()->GetBroadcast (),
                  to);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (arp);
  cache->GetDevice ()->Send (packet, cache->GetDevice ()->GetBroadcast (), PROT_NUMBER);
}

void
ArpL3Protocol::SendArpReply (Ptr<const ArpCache> cache, Ipv4Address toIp, Address toMac)
{
  NS_LOG_FUNCTION (this << cache << toIp << toMac);
  ArpHeader arp;
  NS_LOG_LOGIC ("ARP: sending reply from node "<<m_node->GetId ()<<
            "|| src: " << cache->GetDevice ()->GetAddress () << 
            " / " << cache->GetInterface ()->GetAddress () <<
            " || dst: " << toMac << " / " << toIp);
  arp.SetReply (cache->GetDevice ()->GetAddress (),
                cache->GetInterface ()->GetAddress (),
                toMac, toIp);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (arp);
  cache->GetDevice ()->Send (packet, toMac, PROT_NUMBER);
}

}//namespace ns3
