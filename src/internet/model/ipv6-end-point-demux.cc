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
 */

#include "ipv6-end-point-demux.h"
#include "ipv6-end-point.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("Ipv6EndPointDemux");

namespace ns3 {

Ipv6EndPointDemux::Ipv6EndPointDemux ()
  : m_ephemeral (49152),
    m_portFirst (49152),
    m_portLast (65535)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv6EndPointDemux::~Ipv6EndPointDemux ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++)
    {
      Ipv6EndPoint *endPoint = *i;
      delete endPoint;
    }
  m_endPoints.clear ();
}

bool Ipv6EndPointDemux::LookupPortLocal (uint16_t port)
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

bool Ipv6EndPointDemux::LookupLocal (Ipv6Address addr, uint16_t port)
{
  NS_LOG_FUNCTION (this << addr << port);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++)
    {
      if ((*i)->GetLocalPort () == port
          && (*i)->GetLocalAddress () == addr)
        {
          return true;
        }
    }
  return false;
}

Ipv6EndPoint* Ipv6EndPointDemux::Allocate ()
{
  NS_LOG_FUNCTION_NOARGS ();
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0)
    {
      NS_LOG_WARN ("Ephemeral port allocation failed.");
      return 0;
    }
  Ipv6EndPoint *endPoint = new Ipv6EndPoint (Ipv6Address::GetAny (), port);
  m_endPoints.push_back (endPoint);
  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");
  return endPoint;
}

Ipv6EndPoint* Ipv6EndPointDemux::Allocate (Ipv6Address address)
{
  NS_LOG_FUNCTION (this << address);
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0)
    {
      NS_LOG_WARN ("Ephemeral port allocation failed.");
      return 0;
    }
  Ipv6EndPoint *endPoint = new Ipv6EndPoint (address, port);
  m_endPoints.push_back (endPoint);
  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");
  return endPoint;
}

Ipv6EndPoint* Ipv6EndPointDemux::Allocate (uint16_t port)
{
  NS_LOG_FUNCTION (this <<  port);

  return Allocate (Ipv6Address::GetAny (), port);
}

Ipv6EndPoint* Ipv6EndPointDemux::Allocate (Ipv6Address address, uint16_t port)
{
  NS_LOG_FUNCTION (this << address << port);
  if (LookupLocal (address, port))
    {
      NS_LOG_WARN ("Duplicate address/port; failing.");
      return 0;
    }
  Ipv6EndPoint *endPoint = new Ipv6EndPoint (address, port);
  m_endPoints.push_back (endPoint);
  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");
  return endPoint;
}

Ipv6EndPoint* Ipv6EndPointDemux::Allocate (Ipv6Address localAddress, uint16_t localPort,
                                           Ipv6Address peerAddress, uint16_t peerPort)
{
  NS_LOG_FUNCTION (this << localAddress << localPort << peerAddress << peerPort);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++)
    {
      if ((*i)->GetLocalPort () == localPort
          && (*i)->GetLocalAddress () == localAddress
          && (*i)->GetPeerPort () == peerPort
          && (*i)->GetPeerAddress () == peerAddress)
        {
          NS_LOG_WARN ("No way we can allocate this end-point.");
          /* no way we can allocate this end-point. */
          return 0;
        }
    }
  Ipv6EndPoint *endPoint = new Ipv6EndPoint (localAddress, localPort);
  endPoint->SetPeer (peerAddress, peerPort);
  m_endPoints.push_back (endPoint);

  NS_LOG_DEBUG ("Now have >>" << m_endPoints.size () << "<< endpoints.");

  return endPoint;
}

void Ipv6EndPointDemux::DeAllocate (Ipv6EndPoint *endPoint)
{
  NS_LOG_FUNCTION_NOARGS ();
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
 * If we have an exact match, we return it.
 * Otherwise, if we find a generic match, we return it.
 * Otherwise, we return 0.
 */
Ipv6EndPointDemux::EndPoints Ipv6EndPointDemux::Lookup (Ipv6Address daddr, uint16_t dport,
                                                        Ipv6Address saddr, uint16_t sport,
                                                        Ptr<Ipv6Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << daddr << dport << saddr << sport << incomingInterface);

  EndPoints retval1; /* Matches exact on local port, wildcards on others */
  EndPoints retval2; /* Matches exact on local port/adder, wildcards on others */
  EndPoints retval3; /* Matches all but local address */
  EndPoints retval4; /* Exact match on all 4 */

  NS_LOG_DEBUG ("Looking up endpoint for destination address " << daddr);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++)
    {
      Ipv6EndPoint* endP = *i;
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

      /*    Ipv6Address incomingInterfaceAddr = incomingInterface->GetAddress (); */
      NS_LOG_DEBUG ("dest addr " << daddr);

      bool localAddressMatchesWildCard = endP->GetLocalAddress () == Ipv6Address::GetAny ();
      bool localAddressMatchesExact = endP->GetLocalAddress () == daddr;
      bool localAddressMatchesAllRouters = endP->GetLocalAddress () == Ipv6Address::GetAllRoutersMulticast ();

      /* if no match here, keep looking */
      if (!(localAddressMatchesExact || localAddressMatchesWildCard))
        {
          continue;
        }
      bool remotePeerMatchesExact = endP->GetPeerPort () == sport;
      bool remotePeerMatchesWildCard = endP->GetPeerPort () == 0;
      bool remoteAddressMatchesExact = endP->GetPeerAddress () == saddr;
      bool remoteAddressMatchesWildCard = endP->GetPeerAddress () == Ipv6Address::GetAny ();

      /* If remote does not match either with exact or wildcard,i
         skip this one */
      if (!(remotePeerMatchesExact || remotePeerMatchesWildCard))
        {
          continue;
        }
      if (!(remoteAddressMatchesExact || remoteAddressMatchesWildCard))
        {
          continue;
        }

      /* Now figure out which return list to add this one to */
      if (localAddressMatchesWildCard
          && remotePeerMatchesWildCard
          && remoteAddressMatchesWildCard)
        { /* Only local port matches exactly */
          retval1.push_back (endP);
        }
      if ((localAddressMatchesExact || (localAddressMatchesAllRouters))
          && remotePeerMatchesWildCard
          && remoteAddressMatchesWildCard)
        { /* Only local port and local address matches exactly */
          retval2.push_back (endP);
        }
      if (localAddressMatchesWildCard
          && remotePeerMatchesExact
          && remoteAddressMatchesExact)
        { /* All but local address */
          retval3.push_back (endP);
        }
      if (localAddressMatchesExact
          && remotePeerMatchesExact
          && remoteAddressMatchesExact)
        { /* All 4 match */
          retval4.push_back (endP);
        }
    }

  /* Here we find the most exact match */
  if (!retval4.empty ())
    {
      return retval4;
    }
  if (!retval3.empty ())
    {
      return retval3;
    }
  if (!retval2.empty ())
    {
      return retval2;
    }
  return retval1;  /* might be empty if no matches */
}

Ipv6EndPoint* Ipv6EndPointDemux::SimpleLookup (Ipv6Address dst, uint16_t dport, Ipv6Address src, uint16_t sport)
{
  uint32_t genericity = 3;
  Ipv6EndPoint *generic = 0;

  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++)
    {
      uint32_t tmp = 0;

      if ((*i)->GetLocalPort () != dport)
        {
          continue;
        }

      if ((*i)->GetLocalAddress () == dst && (*i)->GetPeerPort () == sport
          && (*i)->GetPeerAddress () == src)
        {
          /* this is an exact match. */
          return *i;
        }

      if ((*i)->GetLocalAddress () == Ipv6Address::GetAny ())
        {
          tmp++;
        }

      if ((*i)->GetPeerAddress () == Ipv6Address::GetAny ())
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

uint16_t Ipv6EndPointDemux::AllocateEphemeralPort ()
{
  NS_LOG_FUNCTION_NOARGS ();
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
    }
  while (LookupPortLocal (port));
  m_ephemeral = port;
  return port;
}

Ipv6EndPointDemux::EndPoints Ipv6EndPointDemux::GetEndPoints () const
{
  return m_endPoints;
}

} /* namespace ns3 */

