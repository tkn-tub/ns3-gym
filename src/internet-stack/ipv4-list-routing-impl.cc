/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "ns3/log.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/node.h"
#include "ns3/ipv4-static-routing.h"
#include "ipv4-list-routing-impl.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4ListRoutingImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4ListRoutingImpl);

TypeId
Ipv4ListRoutingImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4ListRoutingImpl")
    .SetParent<Ipv4ListRouting> ()
    .AddConstructor<Ipv4ListRoutingImpl> ()
    ;
  return tid;
}


Ipv4ListRoutingImpl::Ipv4ListRoutingImpl () 
 : m_ipv4 (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv4ListRoutingImpl::~Ipv4ListRoutingImpl () 
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Ipv4ListRoutingImpl::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (Ipv4RoutingProtocolList::iterator rprotoIter = m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); rprotoIter++)
    {
      // Note:  Calling dispose on these protocols causes memory leak
      //        The routing protocols should not maintain a pointer to
      //        this object, so Dispose() shouldn't be necessary.
      (*rprotoIter).second = 0;
    }
  m_routingProtocols.clear ();
  m_ipv4 = 0;
}

Ptr<Ipv4Route>
Ipv4ListRoutingImpl::RouteOutput (const Ipv4Header &header, uint32_t oif, enum Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << header.GetDestination () << " " << header.GetSource () << " " << oif);
  Ptr<Ipv4Route> route;

  for (Ipv4RoutingProtocolList::const_iterator i = m_routingProtocols.begin ();
       i != m_routingProtocols.end ();
       i++)
    {
      NS_LOG_LOGIC ("Checking protocol " << (*i).second->GetInstanceTypeId () << " with priority " << (*i).first);
      NS_LOG_LOGIC ("Requesting source address for destination " << header.GetDestination ());
      route = (*i).second->RouteOutput (header, oif, sockerr);
      if (route)
        {
          NS_LOG_LOGIC ("Found route " << route);
          sockerr = Socket::ERROR_NOTERROR;
          return route;
        }
    }
  NS_LOG_LOGIC ("Done checking " << GetTypeId ());
  NS_LOG_LOGIC ("");
  sockerr = Socket::ERROR_NOROUTETOHOST;
  return 0;
}

// Patterned after Linux ip_route_input and ip_route_input_slow
bool 
Ipv4ListRoutingImpl::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, 
                             UnicastForwardCallback ucb, MulticastForwardCallback mcb, 
                             LocalDeliverCallback lcb, ErrorCallback ecb)
{
  bool retVal = false;
  NS_LOG_FUNCTION (p << header << idev);
  NS_LOG_LOGIC ("RouteInput logic for node: " << m_ipv4->GetObject<Node> ()->GetId ());

  NS_ASSERT (m_ipv4 != 0);
  uint32_t iif = m_ipv4->GetInterfaceForDevice (idev); 

  // Multicast recognition; handle local delivery here
  //
  if (header.GetDestination().IsMulticast ())
    {
#ifdef NOTYET
      if (m_ipv4->MulticastCheckGroup (iif, header.GetDestination ()))
#endif
      if (true)
        {
          NS_LOG_LOGIC ("Multicast packet for me-- local deliver");
          Ptr<Packet> packetCopy = p->Copy();
          // Here may want to disable lcb callback in recursive RouteInput
          // call below
          lcb (packetCopy, header, iif);
          // Fall through-- we may also need to forward this
          retVal = true;
        }
      for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
           rprotoIter != m_routingProtocols.end ();
           rprotoIter++)
        {
          NS_LOG_LOGIC ("Multicast packet for me-- trying to forward");
          if ((*rprotoIter).second->RouteInput (p, header, idev, ucb, mcb, lcb, ecb))
            {
              retVal = true;
            }
        }
      return retVal;
    }

  if (header.GetDestination ().IsBroadcast ())
    {
      NS_LOG_LOGIC ("For me (Ipv4Addr broadcast address)");
      // TODO:  Local Deliver for broadcast
      // TODO:  Forward broadcast
    }

 // TODO:  Configurable option to enable RFC 1222 Strong End System Model
 // Right now, we will be permissive and allow a source to send us
 // a packet to one of our other interface addresses; that is, the
 // destination unicast address does not match one of the iif addresses,
 // but we check our other interfaces.  This could be an option
 // (to remove the outer loop immediately below and just check iif).
  for (uint32_t j = 0; j < m_ipv4->GetNInterfaces (); j++)
    {
      for (uint32_t i = 0; i < m_ipv4->GetNAddresses (j); i++)
        {
          Ipv4InterfaceAddress iaddr = m_ipv4->GetAddress (j, i);
          Ipv4Address addr = iaddr.GetLocal ();
          if (addr.IsEqual (header.GetDestination ()))
            {
              if (j == iif)
                {
                  NS_LOG_LOGIC ("For me (destination " << addr << " match)");
                }
              else
                {
                  NS_LOG_LOGIC ("For me (destination " << addr << " match) on another interface " << header.GetDestination ());
                }
              lcb (p, header, iif);
              return true;
            }
          if (header.GetDestination ().IsEqual (iaddr.GetBroadcast ()))
            {
              NS_LOG_LOGIC ("For me (interface broadcast address)");
              lcb (p, header, iif);
              return true;
            }
          NS_LOG_LOGIC ("Address "<< addr << " not a match");
        }
    }
 // Next, try to find a route
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      if ((*rprotoIter).second->RouteInput (p, header, idev, ucb, mcb, lcb, ecb))
        {
          return true;
        }
    }
  // No routing protocol has found a route.  
  return retVal;
}

void 
Ipv4ListRoutingImpl::NotifyInterfaceUp (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyInterfaceUp (interface);
    }  
}
void 
Ipv4ListRoutingImpl::NotifyInterfaceDown (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyInterfaceDown (interface);
    }  
}
void 
Ipv4ListRoutingImpl::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION(this << interface << address);
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyAddAddress (interface, address);
    }  
}
void 
Ipv4ListRoutingImpl::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION(this << interface << address);
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyRemoveAddress (interface, address);
    }  
}
void 
Ipv4ListRoutingImpl::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION(this << ipv4);
  NS_ASSERT (m_ipv4 == 0);
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter =
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->SetIpv4 (ipv4);
    }  
  m_ipv4 = ipv4;
}

void
Ipv4ListRoutingImpl::AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol, int16_t priority)
{
  NS_LOG_FUNCTION (this << routingProtocol->GetInstanceTypeId () << priority);
  m_routingProtocols.push_back (std::make_pair (-priority, routingProtocol));
  m_routingProtocols.sort ();
  if (m_ipv4 != 0)
    {
      routingProtocol->SetIpv4 (m_ipv4);
    }
}

uint32_t 
Ipv4ListRoutingImpl::GetNRoutingProtocols (void) const
{
  NS_LOG_FUNCTION (this);
  return m_routingProtocols.size (); 
}

Ptr<Ipv4RoutingProtocol> 
Ipv4ListRoutingImpl::GetRoutingProtocol (uint32_t index, int16_t& priority) const
{
  NS_LOG_FUNCTION (index);
  if (index > m_routingProtocols.size ())
    {
      NS_FATAL_ERROR ("Ipv4ListRoutingImpl::GetRoutingProtocol():  index " << index << " out of range");
    }
  uint32_t i = 0;
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter = m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); rprotoIter++, i++)
    {
      if (i == index)
        {
          priority = (*rprotoIter).first;
          return (*rprotoIter).second;
        }
    }
  return 0;
}

Ptr<Ipv4StaticRouting>
Ipv4ListRoutingImpl::GetStaticRouting (void) const
{
  NS_LOG_FUNCTION (this);
  Ipv4StaticRouting* srp;
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter = m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); rprotoIter++)
    {
      NS_LOG_LOGIC ("Searching for static routing");
      srp = dynamic_cast<Ipv4StaticRouting*> (PeekPointer((*rprotoIter).second));
      if (srp)
        {
          NS_LOG_LOGIC ("Found static routing");
          return Ptr<Ipv4StaticRouting> (srp);
        }
    }
  NS_LOG_LOGIC ("Static routing not found");
  return 0;

}

}//namespace ns3
