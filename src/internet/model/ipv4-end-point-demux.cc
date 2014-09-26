/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "ipv4-end-point-demux.h"
#include "ipv4-end-point.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4EndPointDemux");

namespace ns3 {

Ipv4EndPointDemux::Ipv4EndPointDemux ()
  : m_ephemeral (49152), m_portLast (65535), m_portFirst (49152)
{
  NS_LOG_FUNCTION (this);
}

Ipv4EndPointDemux::~Ipv4EndPointDemux ()
{
  NS_LOG_FUNCTION (this);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      Ipv4EndPoint *endPoint = *i;
      delete endPoint;
    }
  m_endPoints.clear ();
}

bool
Ipv4EndPointDemux::LookupPortLocal (uint16_t port)
{
  NS_LOG_FUNCTION (this << port);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if ((*i)->GetLocalPort  () == port) 
        {
          return true;
        }
    }
  return false;
}

bool
Ipv4EndPointDemux::LookupLocal (Ipv4Address addr, uint16_t port)
{
  NS_LOG_FUNCTION (this << addr << port);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if ((*i)->GetLocalPort () == port &&
          (*i)->GetLocalAddress () == addr) 
        {
          return true;
        }
    }
  return false;
}

Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (void)
{
  NS_LOG_FUNCTION (this);
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0) 
    {
      NS_LOG_WARN ("Ephemeral port allocation failed.");
      return 0;
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (Ipv4Address::GetAny (), port);
  m_endPoints.push_back (endPoint);
  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");
  return endPoint;
}

Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (Ipv4Address address)
{
  NS_LOG_FUNCTION (this << address);
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0) 
    {
      NS_LOG_WARN ("Ephemeral port allocation failed.");
      return 0;
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (address, port);
  m_endPoints.push_back (endPoint);
  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");
  return endPoint;
}

Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (uint16_t port)
{
  NS_LOG_FUNCTION (this <<  port);

  return Allocate (Ipv4Address::GetAny (), port);
}

Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (Ipv4Address address, uint16_t port)
{
  NS_LOG_FUNCTION (this << address << port);
  if (LookupLocal (address, port)) 
    {
      NS_LOG_WARN ("Duplicate address/port; failing.");
      return 0;
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (address, port);
  m_endPoints.push_back (endPoint);
  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");
  return endPoint;
}

Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (Ipv4Address localAddress, uint16_t localPort,
                             Ipv4Address peerAddress, uint16_t peerPort)
{
  NS_LOG_FUNCTION (this << localAddress << localPort << peerAddress << peerPort);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if ((*i)->GetLocalPort () == localPort &&
          (*i)->GetLocalAddress () == localAddress &&
          (*i)->GetPeerPort () == peerPort &&
          (*i)->GetPeerAddress () == peerAddress) 
        {
          NS_LOG_WARN ("No way we can allocate this end-point.");
          /* no way we can allocate this end-point. */
          return 0;
        }
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (localAddress, localPort);
  endPoint->SetPeer (peerAddress, peerPort);
  m_endPoints.push_back (endPoint);

  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");

  return endPoint;
}

void 
Ipv4EndPointDemux::DeAllocate (Ipv4EndPoint *endPoint)
{
  NS_LOG_FUNCTION (this << endPoint);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if (*i == endPoint)
        {
          delete endPoint;
          m_endPoints.erase (i);
          break;
        }
    }
}

/*
 * return list of all available Endpoints
 */
Ipv4EndPointDemux::EndPoints
Ipv4EndPointDemux::GetAllEndPoints (void)
{
  NS_LOG_FUNCTION (this);
  EndPoints ret;

  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++)
    {
      Ipv4EndPoint* endP = *i;
      ret.push_back (endP);
    }
  return ret;
}


/*
 * If we have an exact match, we return it.
 * Otherwise, if we find a generic match, we return it.
 * Otherwise, we return 0.
 */
Ipv4EndPointDemux::EndPoints
Ipv4EndPointDemux::Lookup (Ipv4Address daddr, uint16_t dport, 
                           Ipv4Address saddr, uint16_t sport,
                           Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << daddr << dport << saddr << sport << incomingInterface);
  
  EndPoints retval1; // Matches exact on local port, wildcards on others
  EndPoints retval2; // Matches exact on local port/adder, wildcards on others
  EndPoints retval3; // Matches all but local address
  EndPoints retval4; // Exact match on all 4

  NS_LOG_DEBUG ("Looking up endpoint for destination address " << daddr);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      Ipv4EndPoint* endP = *i;
      NS_LOG_DEBUG ("Looking at endpoint dport=" << endP->GetLocalPort ()
                                                 << " daddr=" << endP->GetLocalAddress ()
                                                 << " sport=" << endP->GetPeerPort ()
                                                 << " saddr=" << endP->GetPeerAddress ());
      if (endP->GetLocalPort () != dport) 
        {
          NS_LOG_LOGIC ("Skipping endpoint " << &endP
                                             << " because endpoint dport "
                                             << endP->GetLocalPort ()
                                             << " does not match packet dport " << dport);
          continue;
        }
      if (endP->GetBoundNetDevice ())
        {
          if (endP->GetBoundNetDevice () != incomingInterface->GetDevice ())
            {
              NS_LOG_LOGIC ("Skipping endpoint " << &endP
                                                 << " because endpoint is bound to specific device and"
                                                 << endP->GetBoundNetDevice ()
                                                 << " does not match packet device " << incomingInterface->GetDevice ());
              continue;
            }
        }
      bool subnetDirected = false;
      Ipv4Address incomingInterfaceAddr = daddr;  // may be a broadcast
      for (uint32_t i = 0; i < incomingInterface->GetNAddresses (); i++)
        {
          Ipv4InterfaceAddress addr = incomingInterface->GetAddress (i);
          if (addr.GetLocal ().CombineMask (addr.GetMask ()) == daddr.CombineMask (addr.GetMask ()) &&
              daddr.IsSubnetDirectedBroadcast (addr.GetMask ()))
            {
              subnetDirected = true;
              incomingInterfaceAddr = addr.GetLocal ();
            }
        }
      bool isBroadcast = (daddr.IsBroadcast () || subnetDirected == true);
      NS_LOG_DEBUG ("dest addr " << daddr << " broadcast? " << isBroadcast);
      bool localAddressMatchesWildCard = 
        endP->GetLocalAddress () == Ipv4Address::GetAny ();
      bool localAddressMatchesExact = endP->GetLocalAddress () == daddr;

      if (isBroadcast)
        {
          NS_LOG_DEBUG ("Found bcast, localaddr " << endP->GetLocalAddress ());
        }

      if (isBroadcast && (endP->GetLocalAddress () != Ipv4Address::GetAny ()))
        {
          localAddressMatchesExact = (endP->GetLocalAddress () ==
                                      incomingInterfaceAddr);
        }
      // if no match here, keep looking
      if (!(localAddressMatchesExact || localAddressMatchesWildCard))
        continue; 
      bool remotePeerMatchesExact = endP->GetPeerPort () == sport;
      bool remotePeerMatchesWildCard = endP->GetPeerPort () == 0;
      bool remoteAddressMatchesExact = endP->GetPeerAddress () == saddr;
      bool remoteAddressMatchesWildCard = endP->GetPeerAddress () ==
        Ipv4Address::GetAny ();
      // If remote does not match either with exact or wildcard,
      // skip this one
      if (!(remotePeerMatchesExact || remotePeerMatchesWildCard))
        continue;
      if (!(remoteAddressMatchesExact || remoteAddressMatchesWildCard))
        continue;

      // Now figure out which return list to add this one to
      if (localAddressMatchesWildCard &&
          remotePeerMatchesWildCard &&
          remoteAddressMatchesWildCard)
        { // Only local port matches exactly
          retval1.push_back (endP);
        }
      if ((localAddressMatchesExact || (isBroadcast && localAddressMatchesWildCard))&&
          remotePeerMatchesWildCard &&
          remoteAddressMatchesWildCard)
        { // Only local port and local address matches exactly
          retval2.push_back (endP);
        }
      if (localAddressMatchesWildCard &&
          remotePeerMatchesExact &&
          remoteAddressMatchesExact)
        { // All but local address
          retval3.push_back (endP);
        }
      if (localAddressMatchesExact &&
          remotePeerMatchesExact &&
          remoteAddressMatchesExact)
        { // All 4 match
          retval4.push_back (endP);
        }
    }

  // Here we find the most exact match
  if (!retval4.empty ()) return retval4;
  if (!retval3.empty ()) return retval3;
  if (!retval2.empty ()) return retval2;
  return retval1;  // might be empty if no matches
}

Ipv4EndPoint *
Ipv4EndPointDemux::SimpleLookup (Ipv4Address daddr, 
                                 uint16_t dport, 
                                 Ipv4Address saddr, 
                                 uint16_t sport)
{
  NS_LOG_FUNCTION (this << daddr << dport << saddr << sport);

  // this code is a copy/paste version of an old BSD ip stack lookup
  // function.
  uint32_t genericity = 3;
  Ipv4EndPoint *generic = 0;
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if ((*i)->GetLocalPort () != dport) 
        {
          continue;
        }
      if ((*i)->GetLocalAddress () == daddr &&
          (*i)->GetPeerPort () == sport &&
          (*i)->GetPeerAddress () == saddr) 
        {
          /* this is an exact match. */
          return *i;
        }
      uint32_t tmp = 0;
      if ((*i)->GetLocalAddress () == Ipv4Address::GetAny ()) 
        {
          tmp++;
        }
      if ((*i)->GetPeerAddress () == Ipv4Address::GetAny ()) 
        {
          tmp++;
        }
      if (tmp < genericity) 
        {
          generic = (*i);
          genericity = tmp;
        }
    }
  return generic;
}
uint16_t
Ipv4EndPointDemux::AllocateEphemeralPort (void)
{
  // Similar to counting up logic in netinet/in_pcb.c
  NS_LOG_FUNCTION (this);
  uint16_t port = m_ephemeral;
  int count = m_portLast - m_portFirst;
  do 
    {
      if (count-- < 0)
        {
          return 0;
        }
      ++port;
      if (port < m_portFirst || port > m_portLast)
        {
          port = m_portFirst;
        }
    } while (LookupPortLocal (port));
  m_ephemeral = port;
  return port;
}

} // namespace ns3

