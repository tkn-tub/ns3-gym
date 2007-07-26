/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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
#include "ns3/debug.h"

namespace ns3{

NS_DEBUG_COMPONENT_DEFINE ("Ipv4EndPointDemux");

Ipv4EndPointDemux::Ipv4EndPointDemux ()
  : m_ephemeral (1025)
{}

Ipv4EndPointDemux::~Ipv4EndPointDemux ()
{
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
  NS_DEBUG ("Ipv4EndPointDemux::Allocate ()");
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0) 
    {
      NS_DEBUG ("Ipv4EndPointDemux::Allocate ephemeral port allocation failed.");
      return 0;
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (Ipv4Address::GetAny (), port);
  m_endPoints.push_back (endPoint);
  return endPoint;
}
Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (Ipv4Address address)
{
  NS_DEBUG ("Ipv4EndPointDemux::Allocate (address=" << address << ")");
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0) 
    {
      NS_DEBUG ("Ipv4EndPointDemux::Allocate ephemeral port allocation failed.");
      return 0;
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (address, port);
  m_endPoints.push_back (endPoint);
  return endPoint;
}
Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (uint16_t port)
{
  return Allocate (Ipv4Address::GetAny (), port);
}
Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (Ipv4Address address, uint16_t port)
{
  NS_DEBUG ("Ipv4EndPointDemux::Allocate (address=" << address
            << ", port=" << port << ")");
  if (LookupLocal (address, port)) 
    {
      NS_DEBUG ("Ipv4EndPointDemux::Allocate duplicate address/port; failing.");
      return 0;
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (address, port);
  m_endPoints.push_back (endPoint);
  return endPoint;
}

Ipv4EndPoint *
Ipv4EndPointDemux::Allocate (Ipv4Address localAddress, uint16_t localPort,
			     Ipv4Address peerAddress, uint16_t peerPort)
{
  NS_DEBUG ("Ipv4EndPointDemux::Allocate (localAddress=" << localAddress
            << ", localPort=" << localPort
            << ", peerAddress=" << peerAddress
            << ", peerPort=" << peerPort << ")");
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if ((*i)->GetLocalPort () == localPort &&
          (*i)->GetLocalAddress () == localAddress &&
          (*i)->GetPeerPort () == peerPort &&
          (*i)->GetPeerAddress () == peerAddress) 
        {
          NS_DEBUG ("Ipv4EndPointDemux::Allocate: no way we can allocate this end-point.");
          /* no way we can allocate this end-point. */
          return 0;
        }
    }
  Ipv4EndPoint *endPoint = new Ipv4EndPoint (localAddress, localPort);
  endPoint->SetPeer (peerAddress, peerPort);
  m_endPoints.push_back (endPoint);
  return endPoint;
}

void 
Ipv4EndPointDemux::DeAllocate (Ipv4EndPoint *endPoint)
{
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
Ipv4EndPointDemux::EndPoints
Ipv4EndPointDemux::Lookup (Ipv4Address daddr, uint16_t dport, 
                           Ipv4Address saddr, uint16_t sport)
{
  uint32_t genericity = 3;
  Ipv4EndPoint *generic = 0;
  EndPoints retval;

  NS_DEBUG ("Ipv4EndPointDemux::Lookup (daddr=" << daddr << ", dport=" << dport
            << ", saddr=" << saddr << ", sport=" << sport
            << ")");
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      NS_DEBUG ("Ipv4EndPointDemux::Lookup against " << 
                (*i)->GetLocalAddress ()
                << ":" << 
                (*i)->GetLocalPort () 
                << " " << 
                (*i)->GetPeerAddress () 
                << ":" 
                << (*i)->GetPeerPort ());
      if ((*i)->GetLocalPort () != dport) 
        {
          continue;
        }
      NS_DEBUG ("Ipv4EndPointDemux::Lookup local address matches: "
                << bool ((*i)->GetLocalAddress () == daddr || daddr.IsBroadcast ()));
      NS_DEBUG ("Ipv4EndPointDemux::Lookup peer port matches: "
                << bool ((*i)->GetPeerPort () == sport || sport == 0));
      NS_DEBUG ("Ipv4EndPointDemux::Lookup peer address matches: "
                << bool ((*i)->GetPeerAddress () == saddr ||
                         (*i)->GetPeerAddress () == Ipv4Address::GetAny ()));
      
      if ( ((*i)->GetLocalAddress () == daddr || daddr.IsBroadcast ())
           && ((*i)->GetPeerPort () == sport || (*i)->GetPeerPort () == 0)
           && ((*i)->GetPeerAddress () == saddr || (*i)->GetPeerAddress () == Ipv4Address::GetAny ()))
        {
          NS_DEBUG ("Ipv4EndPointDemux::Lookup MATCH");
          /* this is an exact match. */
          retval.push_back (*i);
        }
      uint32_t tmp = 0;
      if ((*i)->GetLocalAddress () == Ipv4Address::GetAny ()) 
        {
          tmp ++;
        }
      if ((*i)->GetPeerAddress () == Ipv4Address::GetAny ()) 
        {
          tmp ++;
        }
      if (tmp < genericity) 
        {
          generic = (*i);
          genericity = tmp;
        }
    }
  if (retval.size () == 0 && generic != 0)
    {
      retval.push_back (generic);
    }
  return retval;
}

uint16_t
Ipv4EndPointDemux::AllocateEphemeralPort (void)
{
  uint16_t port = m_ephemeral;
  do 
    {
      port++;
      if (port > 5000) 
        {
          port = 1024;
        }
      if (!LookupPortLocal (port)) 
        {
          return port;
        }
  } while (port != m_ephemeral);
  return 0;
}

} //namespace ns3

