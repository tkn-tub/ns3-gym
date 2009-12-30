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
#include "ns3/ipv6.h"
#include "ns3/ipv6-route.h"
#include "ns3/node.h"
#include "ns3/ipv6-static-routing.h"
#include "ipv6-list-routing.h"

NS_LOG_COMPONENT_DEFINE ("Ipv6ListRouting");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv6ListRouting);

TypeId
Ipv6ListRouting::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv6ListRouting")
    .SetParent<Ipv6RoutingProtocol> ()
    .AddConstructor<Ipv6ListRouting> ()
    ;
  return tid;
}


Ipv6ListRouting::Ipv6ListRouting () 
  : m_ipv6 (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv6ListRouting::~Ipv6ListRouting () 
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Ipv6ListRouting::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (Ipv6RoutingProtocolList::iterator rprotoIter = m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); rprotoIter++)
    {
      // Note:  Calling dispose on these protocols causes memory leak
      //        The routing protocols should not maintain a pointer to
      //        this object, so Dispose () shouldn't be necessary.
      (*rprotoIter).second = 0;
    }
  m_routingProtocols.clear ();
  m_ipv6 = 0;
}

Ptr<Ipv6Route>
Ipv6ListRouting::RouteOutput (Ptr<Packet> p, const Ipv6Header &header, Ptr<NetDevice> oif, enum Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << header.GetDestinationAddress () << header.GetSourceAddress () << oif);
  Ptr<Ipv6Route> route;

  for (Ipv6RoutingProtocolList::const_iterator i = m_routingProtocols.begin ();
       i != m_routingProtocols.end (); i++)
    {
      NS_LOG_LOGIC ("Checking protocol " << (*i).second->GetInstanceTypeId () << " with priority " << (*i).first);
      NS_LOG_LOGIC ("Requesting source address for destination " << header.GetDestinationAddress ());
      route = (*i).second->RouteOutput (p, header, oif, sockerr);
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
Ipv6ListRouting::RouteInput (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev, 
                             UnicastForwardCallback ucb, MulticastForwardCallback mcb, 
                             LocalDeliverCallback lcb, ErrorCallback ecb)
{
  bool retVal = false;
  NS_LOG_FUNCTION (p << header << idev);
  NS_LOG_LOGIC ("RouteInput logic for node: " << m_ipv6->GetObject<Node> ()->GetId ());

  NS_ASSERT (m_ipv6 != 0);
  // Check if input device supports IP 
  NS_ASSERT (m_ipv6->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv6->GetInterfaceForDevice (idev); 
  Ipv6Address dst = header.GetDestinationAddress ();

  // Multicast recognition; handle local delivery here
  //
  if (dst.IsMulticast ())
    {
#ifdef NOTYET
      if (m_ipv6->MulticastCheckGroup (iif, dst))
#endif
        if (true)
          {
            NS_LOG_LOGIC ("Multicast packet for me-- local deliver");
            Ptr<Packet> packetCopy = p->Copy ();
            // Here may want to disable lcb callback in recursive RouteInput
            // call below
            lcb (packetCopy, header, iif);
            // Fall through-- we may also need to forward this
            retVal = true;
          }

      /* do not forward link-local multicast address */
      if (dst == Ipv6Address::GetAllNodesMulticast () || dst == Ipv6Address::GetAllRoutersMulticast () || dst == Ipv6Address::GetAllHostsMulticast ())
        {
          return retVal;
        }

      for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
           m_routingProtocols.begin (); rprotoIter != m_routingProtocols.end ();
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

  // TODO:  Configurable option to enable RFC 1222 Strong End System Model
  // Right now, we will be permissive and allow a source to send us
  // a packet to one of our other interface addresses; that is, the
  // destination unicast address does not match one of the iif addresses,
  // but we check our other interfaces.  This could be an option
  // (to remove the outer loop immediately below and just check iif).
  for (uint32_t j = 0; j < m_ipv6->GetNInterfaces (); j++)
    {
      for (uint32_t i = 0; i < m_ipv6->GetNAddresses (j); i++)
        {
          Ipv6InterfaceAddress iaddr = m_ipv6->GetAddress (j, i);
          Ipv6Address addr = iaddr.GetAddress ();
          if (addr.IsEqual (header.GetDestinationAddress ()))
            {
              if (j == iif)
                {
                  NS_LOG_LOGIC ("For me (destination " << addr << " match)");
                }
              else
                {
                  NS_LOG_LOGIC ("For me (destination " << addr << " match) on another interface " << header.GetDestinationAddress ());
                }
              lcb (p, header, iif);
              return true;
            }
          NS_LOG_LOGIC ("Address "<< addr << " not a match");
        }
    }
  // Check if input device supports IP forwarding
  if (m_ipv6->IsForwarding (iif) == false)
    {
      NS_LOG_LOGIC ("Forwarding disabled for this interface");
      ecb (p, header, Socket::ERROR_NOROUTETOHOST);
      return false;
    }
  // Next, try to find a route
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
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
Ipv6ListRouting::NotifyInterfaceUp (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyInterfaceUp (interface);
    }  
}
void 
Ipv6ListRouting::NotifyInterfaceDown (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyInterfaceDown (interface);
    }  
}
void 
Ipv6ListRouting::NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyAddAddress (interface, address);
    }  
}
void 
Ipv6ListRouting::NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyRemoveAddress (interface, address);
    }  
}

void Ipv6ListRouting::NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{
  NS_LOG_FUNCTION (this << dst << mask << nextHop << interface);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyAddRoute (dst, mask, nextHop, interface, prefixToUse);
    }
}

void Ipv6ListRouting::NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{
  NS_LOG_FUNCTION (this << dst << mask << nextHop << interface);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->NotifyRemoveRoute (dst, mask, nextHop, interface, prefixToUse);
    }
}

void 
Ipv6ListRouting::SetIpv6 (Ptr<Ipv6> ipv6)
{
  NS_LOG_FUNCTION (this << ipv6);
  NS_ASSERT (m_ipv6 == 0);
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter =
       m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end ();
       rprotoIter++)
    {
      (*rprotoIter).second->SetIpv6 (ipv6);
    }  
  m_ipv6 = ipv6;
}

void
Ipv6ListRouting::AddRoutingProtocol (Ptr<Ipv6RoutingProtocol> routingProtocol, int16_t priority)
{
  NS_LOG_FUNCTION (this << routingProtocol->GetInstanceTypeId () << priority);
  m_routingProtocols.push_back (std::make_pair (priority, routingProtocol));
  m_routingProtocols.sort ( Compare );
  if (m_ipv6 != 0)
    {
      routingProtocol->SetIpv6 (m_ipv6);
    }
}

uint32_t 
Ipv6ListRouting::GetNRoutingProtocols (void) const
{
  NS_LOG_FUNCTION (this);
  return m_routingProtocols.size (); 
}

Ptr<Ipv6RoutingProtocol> 
Ipv6ListRouting::GetRoutingProtocol (uint32_t index, int16_t& priority) const
{
  NS_LOG_FUNCTION (index);
  if (index > m_routingProtocols.size ())
    {
      NS_FATAL_ERROR ("Ipv6ListRouting::GetRoutingProtocol ():  index " << index << " out of range");
    }
  uint32_t i = 0;
  for (Ipv6RoutingProtocolList::const_iterator rprotoIter = m_routingProtocols.begin ();
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

bool 
Ipv6ListRouting::Compare (const Ipv6RoutingProtocolEntry& a, const Ipv6RoutingProtocolEntry& b)
{
  return a.first > b.first;
}


} // namespace ns3


#include "ns3/test.h"
#include "ipv6-list-routing.h"
#include "ns3/ipv6-routing-protocol.h"

namespace ns3 {

    class Ipv6ARouting : public Ipv6RoutingProtocol {
    public:
      Ptr<Ipv6Route> RouteOutput (Ptr<Packet> p, const Ipv6Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)  { return 0;}
      bool RouteInput  (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev,
                        UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                        LocalDeliverCallback lcb, ErrorCallback ecb) {return false;}
      void NotifyInterfaceUp (uint32_t interface) {}
      void NotifyInterfaceDown (uint32_t interface) {}
      void NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address) {}
      void NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address) {}
      void NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::
                           GetZero ()) {}
      void NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse) {}
      void SetIpv6 (Ptr<Ipv6> ipv6) {}
    };

    class Ipv6BRouting : public Ipv6RoutingProtocol {
    public:
      Ptr<Ipv6Route> RouteOutput (Ptr<Packet> p, const Ipv6Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)  { return 0;}
      bool RouteInput  (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev,
                        UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                        LocalDeliverCallback lcb, ErrorCallback ecb) {return false;}
      void NotifyInterfaceUp (uint32_t interface) {}
      void NotifyInterfaceDown (uint32_t interface) {}
      void NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address) {}
      void NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address) {}
      void NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::
                           GetZero ()) {}
      void NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse) {}
      void SetIpv6 (Ptr<Ipv6> ipv6) {}
    };

    class Ipv6ListRoutingNegativeTestCase : public TestCase
    {
  public:
    Ipv6ListRoutingNegativeTestCase();
    virtual bool DoRun (void);
    };

    Ipv6ListRoutingNegativeTestCase::Ipv6ListRoutingNegativeTestCase()
      : TestCase("Check negative priorities")
      {}
    bool 
      Ipv6ListRoutingNegativeTestCase::DoRun (void)
        {
          Ptr<Ipv6ListRouting> lr = CreateObject<Ipv6ListRouting> ();
          Ptr<Ipv6RoutingProtocol> aRouting = CreateObject<Ipv6ARouting> ();
          Ptr<Ipv6RoutingProtocol> bRouting = CreateObject<Ipv6BRouting> ();
          // The Ipv6BRouting should be added with higher priority (larger integer value)
          lr->AddRoutingProtocol (aRouting, -10);
          lr->AddRoutingProtocol (bRouting, -5);
          int16_t first = 3;
          uint32_t num = lr->GetNRoutingProtocols ();
          NS_TEST_ASSERT_MSG_EQ (num, 2, "XXX");
          Ptr<Ipv6RoutingProtocol> firstRp = lr->GetRoutingProtocol (0, first);
          NS_TEST_ASSERT_MSG_EQ (-5, first, "XXX");
          NS_TEST_ASSERT_MSG_EQ (firstRp, bRouting, "XXX");

          // XXX
          return false;
        }

    class Ipv6ListRoutingPositiveTestCase : public TestCase
    {
  public:
    Ipv6ListRoutingPositiveTestCase();
    virtual bool DoRun (void);
    };

    Ipv6ListRoutingPositiveTestCase::Ipv6ListRoutingPositiveTestCase()
      : TestCase("Check positive priorities")
      {}
    bool 
      Ipv6ListRoutingPositiveTestCase::DoRun (void)
        {
          Ptr<Ipv6ListRouting> lr = CreateObject<Ipv6ListRouting> ();
          Ptr<Ipv6RoutingProtocol> aRouting = CreateObject<Ipv6ARouting> ();
          Ptr<Ipv6RoutingProtocol> bRouting = CreateObject<Ipv6BRouting> ();
          // The Ipv6ARouting should be added with higher priority (larger integer 
          // value) and will be fetched first below
          lr->AddRoutingProtocol (aRouting, 10);
          lr->AddRoutingProtocol (bRouting, 5);
          int16_t first = 3;
          int16_t second = 3;
          uint32_t num = lr->GetNRoutingProtocols ();
          NS_TEST_ASSERT_MSG_EQ (num, 2, "XXX");
          Ptr<Ipv6RoutingProtocol> firstRp = lr->GetRoutingProtocol (0, first);
          NS_TEST_ASSERT_MSG_EQ (10, first, "XXX");
          NS_TEST_ASSERT_MSG_EQ (firstRp, aRouting, "XXX");
          Ptr<Ipv6RoutingProtocol> secondRp = lr->GetRoutingProtocol (1, second);
          NS_TEST_ASSERT_MSG_EQ (5, second, "XXX");
          NS_TEST_ASSERT_MSG_EQ (secondRp, bRouting, "XXX");

          // XXX
          return false;
        }

    static class Ipv6ListRoutingTestSuite : public TestSuite
    {
  public:
    Ipv6ListRoutingTestSuite()
      : TestSuite("ipv6-list-routing", UNIT)
      {
        AddTestCase(new Ipv6ListRoutingPositiveTestCase());
        AddTestCase(new Ipv6ListRoutingNegativeTestCase());
      }

    } g_ipv6ListRoutingTestSuite;

} // namespace ns3
