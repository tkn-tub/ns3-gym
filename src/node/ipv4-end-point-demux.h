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

#ifndef IPV4_END_POINT_DEMUX_H
#define IPV4_END_POINT_DEMUX_H

#include <stdint.h>
#include <list>
#include "ipv4-address.h"

namespace ns3 {

class Ipv4EndPoint;

template <typename T>
class Ipv4EndPointDemux {
public:
  Ipv4EndPointDemux ();
  ~Ipv4EndPointDemux ();

  bool LookupPortLocal (uint16_t port);
  bool LookupLocal (Ipv4Address addr, uint16_t port);
  T *Lookup (Ipv4Address daddr, 
             uint16_t dport, 
             Ipv4Address saddr, 
             uint16_t sport);

  T *Allocate (void);
  T *Allocate (Ipv4Address address);
  T *Allocate (uint16_t port);
  T *Allocate (Ipv4Address address, uint16_t port);
  T *Allocate (Ipv4Address localAddress, 
               uint16_t localPort,
               Ipv4Address peerAddress, 
               uint16_t peerPort);

 private:
  uint16_t AllocateEphemeralPort (void);
  typedef std::list<T *> EndPoints;
  typedef typename std::list<T *>::iterator EndPointsI;

  uint16_t m_ephemeral;
  EndPoints m_endPoints;
};

}; // namespace ns3

namespace ns3{

template <typename T>
Ipv4EndPointDemux<T>::Ipv4EndPointDemux ()
  : m_ephemeral (1025)
{}

template <typename T>
Ipv4EndPointDemux<T>::~Ipv4EndPointDemux ()
{}

template <typename T>
bool
Ipv4EndPointDemux<T>::LookupPortLocal (uint16_t port)
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

template <typename T>
bool
Ipv4EndPointDemux<T>::LookupLocal (Ipv4Address addr, uint16_t port)
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

template <typename T>
T *
Ipv4EndPointDemux<T>::Allocate (void)
{
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0) 
    {
      return 0;
    }
  T *endPoint = new T (Ipv4Address::GetAny (), port);
  m_endPoints.push_back (endPoint);
  return endPoint;
}
template <typename T>
T *
Ipv4EndPointDemux<T>::Allocate (Ipv4Address address)
{
  uint16_t port = AllocateEphemeralPort ();
  if (port == 0) 
    {
      return 0;
    }
  T *endPoint = new T (address, port);
  m_endPoints.push_back (endPoint);
  return endPoint;
}
template <typename T>
T *
Ipv4EndPointDemux<T>::Allocate (uint16_t port)
{
  return Allocate (Ipv4Address::GetAny (), port);
}
template <typename T>
T *
Ipv4EndPointDemux<T>::Allocate (Ipv4Address address, uint16_t port)
{
  if (LookupLocal (address, port)) 
    {
      return 0;
    }
  T *endPoint = new T (address, port);
  m_endPoints.push_back (endPoint);
  return endPoint;
}

template <typename T>
T *
Ipv4EndPointDemux<T>::Allocate (Ipv4Address localAddress, uint16_t localPort,
                                Ipv4Address peerAddress, uint16_t peerPort)
{
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
      if ((*i)->GetLocalPort () == localPort &&
          (*i)->GetLocalAddress () == localAddress &&
          (*i)->GetPeerPort () == peerPort &&
          (*i)->GetPeerAddress () == peerAddress) 
        {
          /* no way we can allocate this end-point. */
          return 0;
        }
    }
  T *endPoint = new T (localAddress, localPort);
  endPoint->SetPeer (peerAddress, peerPort);
  m_endPoints.push_back (endPoint);
  return endPoint;
}


/*
 * If we have an exact match, we return it.
 * Otherwise, if we find a generic match, we return it.
 * Otherwise, we return 0.
 */
template <typename T>
T *
Ipv4EndPointDemux<T>::Lookup (Ipv4Address daddr, uint16_t dport, 
                              Ipv4Address saddr, uint16_t sport)
{
  uint32_t genericity = 3;
  T *generic = 0;
  //TRACE ("lookup " << daddr << ":" << dport << " " << saddr << ":" << sport);
  for (EndPointsI i = m_endPoints.begin (); i != m_endPoints.end (); i++) 
    {
#if 0
      TRACE ("against " << 
             (*i)->GetLocalAddress ()
             << ":" << 
             (*i)->GetLocalPort () 
             << " " << 
             (*i)->GetPeerAddress () 
             << ":" 
             << (*i)->GetPeerPort ());
#endif
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
  return generic;
}

template <typename T>
uint16_t
Ipv4EndPointDemux<T>::AllocateEphemeralPort (void)
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



}//namespace ns3


#endif /* IPV4_END_POINTS_H */
