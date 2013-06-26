/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Yufei Cheng
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
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *              Song Luan <lsuper@mail.ustc.edu.cn> (Implemented Link Cache using Dijsktra algorithm)
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#include "dsr-rcache.h"
#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <iomanip>

#include "ns3/simulator.h"
#include "ns3/ipv4-route.h"
#include "ns3/socket.h"
#include "ns3/log.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("RouteCache");

namespace ns3 {
namespace dsr {

bool CompareRoutesBoth (const RouteCacheEntry &a, const RouteCacheEntry &b)
{
  // compare based on both with hop count considered priority
  return (a.GetVector ().size () < b.GetVector ().size ())
         || ((a.GetVector ().size () == b.GetVector ().size ()) && (a.GetExpireTime () > b.GetExpireTime ()))
  ;
}

bool CompareRoutesHops (const RouteCacheEntry &a, const RouteCacheEntry &b)
{
  // compare based on hops
  return a.GetVector ().size () < b.GetVector ().size ();
}

bool CompareRoutesExpire (const RouteCacheEntry &a, const RouteCacheEntry &b)
{
  // compare based on expire time
  return a.GetExpireTime () > b.GetExpireTime ();
}

void Link::Print () const
{
  NS_LOG_DEBUG (m_low << "----" << m_high);
}

NodeStab::NodeStab (Time nodeStab)
  : m_nodeStability (nodeStab + Simulator::Now ())
{
}

NodeStab::~NodeStab ()
{
}

LinkStab::LinkStab (Time linkStab)
  : m_linkStability (linkStab + Simulator::Now ())
{
}

LinkStab::~LinkStab ()
{
}

void LinkStab::Print ( ) const
{
  NS_LOG_LOGIC ("LifeTime: " << GetLinkStability ().GetSeconds ());
}

typedef std::list<RouteCacheEntry>::value_type route_pair;

RouteCacheEntry::RouteCacheEntry (IP_VECTOR const  & ip, Ipv4Address dst, Time exp)
  : m_ackTimer (Timer::CANCEL_ON_DESTROY),
    m_dst (dst),
    m_path (ip),
    m_expire (exp + Simulator::Now ()),
    m_reqCount (0),
    m_blackListState (false),
    m_blackListTimeout (Simulator::Now ())
{
}

RouteCacheEntry::~RouteCacheEntry ()
{
}

void
RouteCacheEntry::Invalidate (Time badLinkLifetime)
{
  m_reqCount = 0;
  m_expire = badLinkLifetime + Simulator::Now ();
}

void
RouteCacheEntry::Print (std::ostream & os) const
{
  os << m_dst << "\t" << (m_expire - Simulator::Now ()).GetSeconds ()
     << "\t";
}

NS_OBJECT_ENSURE_REGISTERED (RouteCache);

TypeId RouteCache::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::RouteCache")
    .SetParent<Object> ()
    .AddConstructor<RouteCache> ()
  ;
  return tid;
}

RouteCache::RouteCache ()
  : m_vector (0),
    m_maxEntriesEachDst (3),
    m_isLinkCache (false),
    m_ntimer (Timer::CANCEL_ON_DESTROY),
    m_delay (MilliSeconds (100))
{
  /*
   * The timer to set layer 2 notification, not fully supported by ns3 yet
   */
  m_ntimer.SetDelay (m_delay);
  m_ntimer.SetFunction (&RouteCache::PurgeMac, this);
  m_txErrorCallback = MakeCallback (&RouteCache::ProcessTxError, this);
}

RouteCache::~RouteCache ()
{
  NS_LOG_FUNCTION_NOARGS ();
  // clear the route cache when done
  m_sortedRoutes.clear ();
}

void
RouteCache::RemoveLastEntry (std::list<RouteCacheEntry> & rtVector)
{
  NS_LOG_FUNCTION (this);
  // Release the last entry of route list
  rtVector.pop_back ();
}

bool
RouteCache::UpdateRouteEntry (Ipv4Address dst)
{
  NS_LOG_FUNCTION (this << dst);
  std::map<Ipv4Address, std::list<RouteCacheEntry> >::const_iterator i =
    m_sortedRoutes.find (dst);
  if (i == m_sortedRoutes.end ())
    {
      NS_LOG_LOGIC ("Failed to find the route entry for the destination " << dst);
      return false;
    }
  else
    {
      std::list<RouteCacheEntry> rtVector = i->second;
      RouteCacheEntry successEntry = rtVector.front ();
      successEntry.SetExpireTime (RouteCacheTimeout);
      rtVector.pop_front ();
      rtVector.push_back (successEntry);
      rtVector.sort (CompareRoutesExpire);      // sort the route vector first
      m_sortedRoutes.erase (dst);               // erase the entry first
      /*
       * Save the new route cache along with the destination address in map
       */
      std::pair<std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator, bool> result =
        m_sortedRoutes.insert (std::make_pair (dst, rtVector));
      return result.second;
    }
  return false;
}

bool
RouteCache::LookupRoute (Ipv4Address id, RouteCacheEntry & rt)
{
  NS_LOG_FUNCTION (this << id);
  if (IsLinkCache ())
    {
      return LookupRoute_Link (id, rt);
    }
  else
    {
      Purge ();  // Purge first to remove expired entries
      if (m_sortedRoutes.empty ())
        {
          NS_LOG_LOGIC ("Route to " << id << " not found; m_sortedRoutes is empty");
          return false;
        }
      std::map<Ipv4Address, std::list<RouteCacheEntry> >::const_iterator i = m_sortedRoutes.find (id);
      if (i == m_sortedRoutes.end ())
        {
          NS_LOG_LOGIC ("No Direct Route to " << id << " found");
          for (std::map<Ipv4Address, std::list<RouteCacheEntry> >::const_iterator j =
                 m_sortedRoutes.begin (); j != m_sortedRoutes.end (); ++j)
            {
              std::list<RouteCacheEntry> rtVector = j->second; // The route cache vector linked with destination address
              /*
               * Loop through the possibly multiple routes within the route vector
               */
              for (std::list<RouteCacheEntry>::const_iterator k = rtVector.begin (); k != rtVector.end (); ++k)
                {
                  // return the first route in the route vector
                  RouteCacheEntry::IP_VECTOR routeVector = k->GetVector ();
                  RouteCacheEntry::IP_VECTOR changeVector;

                  for (RouteCacheEntry::IP_VECTOR::iterator l = routeVector.begin (); l != routeVector.end (); ++l)
                    {
                      if (*l != id)
                        {
                          changeVector.push_back (*l);
                        }
                      else
                        {
                          changeVector.push_back (*l);
                          break;
                        }
                    }
                  /*
                   * When the changed vector is smaller in size and larger than 1, which means we have found a route with the destination
                   * address we are looking for
                   */
                  if ((changeVector.size () < routeVector.size ())  && (changeVector.size () > 1))
                    {
                      RouteCacheEntry changeEntry; // Create the route entry
                      changeEntry.SetVector (changeVector);
                      changeEntry.SetDestination (id);
                      // Use the expire time from original route entry
                      changeEntry.SetExpireTime (k->GetExpireTime ());
                      // We need to add new route entry here
                      std::list<RouteCacheEntry> newVector;
                      newVector.push_back (changeEntry);
                      newVector.sort (CompareRoutesExpire);  // sort the route vector first
                      m_sortedRoutes[id] = newVector;   // Only get the first sub route and add it in route cache
                      NS_LOG_INFO ("We have a sub-route to " << id << " add it in route cache");
                    }
                }
            }
        }
      NS_LOG_INFO ("Here we check the route cache again after updated the sub routes");
      std::map<Ipv4Address, std::list<RouteCacheEntry> >::const_iterator m = m_sortedRoutes.find (id);
      if (m == m_sortedRoutes.end ())
        {
          NS_LOG_LOGIC ("No updated route till last time");
          return false;
        }
      /*
       * We have a direct route to the destination address
       */
      std::list<RouteCacheEntry> rtVector = m->second;
      rt = rtVector.front ();  // use the first entry in the route vector
      NS_LOG_LOGIC ("Route to " << id << " with route size " << rtVector.size ());
      return true;
    }
}

void
RouteCache::SetCacheType (std::string type)
{
  NS_LOG_FUNCTION (this << type);
  if (type == std::string ("LinkCache"))
    {
      m_isLinkCache = true;
    }
  else if (type == std::string ("PathCache"))
    {
      m_isLinkCache = false;
    }
  else
    {
      m_isLinkCache = true;             // use link cache as default
      NS_LOG_INFO ("Error Cache Type");
    }
}

bool
RouteCache::IsLinkCache ()
{
  NS_LOG_FUNCTION (this);
  return m_isLinkCache;
}

void
RouteCache::RebuildBestRouteTable (Ipv4Address source)
{
  NS_LOG_FUNCTION (this << source);
  /**
   * \brief The followings are initialize-single-source
   */
  // @d shortest-path estimate
  std::map<Ipv4Address, uint32_t> d;
  // @pre preceeding node
  std::map<Ipv4Address, Ipv4Address> pre;
  for (std::map<Ipv4Address, std::map<Ipv4Address, uint32_t> >::iterator i = m_netGraph.begin (); i != m_netGraph.end (); ++i)
    {
      if (i->second.find (source) != i->second.end ())
        {
          d[i->first] = i->second[source];
          pre[i->first] = source;
        }
      else
        {
          d[i->first] = MAXWEIGHT;
          pre[i->first] = Ipv4Address ("255.255.255.255");
        }
    }
  d[source] = 0;
  /**
   * \brief The followings are core of dijskra algorithm
   */
  // the node set which shortest distance has been calculated, if true calculated
  std::map<Ipv4Address, bool> s;
  double temp = MAXWEIGHT;
  Ipv4Address tempip = Ipv4Address ("255.255.255.255");
  for (uint32_t i = 0; i < m_netGraph.size (); i++)
    {
      temp = MAXWEIGHT;
      for (std::map<Ipv4Address,uint32_t>::const_iterator j = d.begin (); j != d.end (); ++j)
        {
          Ipv4Address ip = j->first;
          if (s.find (ip) == s.end ())
            {
              /*
               * \brief The followings are for comparison
               */
              if (j->second <= temp)
                {
                  temp = j->second;
                  tempip = ip;
                }
            }
        }
      if (!tempip.IsBroadcast ())
        {
          s[tempip] = true;
          for (std::map<Ipv4Address, uint32_t>::const_iterator k = m_netGraph[tempip].begin (); k != m_netGraph[tempip].end (); ++k)
            {
              if (s.find (k->first) == s.end () && d[k->first] > d[tempip] + k->second)
                {
                  d[k->first] = d[tempip] + k->second;
                  pre[k->first] = tempip;
                }
              /*
               *  Selects the shortest-length route that has the longest expected lifetime
               *  (highest minimum timeout of any link in the route)
               *  For the computation overhead and complexity
               *  Here I just implement kind of greedy strategy to select link with the longest expected lifetime when there is two options
               */
              else if (d[k->first] == d[tempip] + k->second)
                {
                  std::map<Link, LinkStab>::iterator oldlink = m_linkCache.find (Link (k->first, pre[k->first]));
                  std::map<Link, LinkStab>::iterator newlink = m_linkCache.find (Link (k->first, tempip));
                  if (oldlink != m_linkCache.end () && newlink != m_linkCache.end ())
                    {
                      if (oldlink->second.GetLinkStability () < newlink->second.GetLinkStability ())
                        {
                          NS_LOG_INFO ("Select the link with longest expected lifetime");
                          d[k->first] = d[tempip] + k->second;
                          pre[k->first] = tempip;
                        }
                    }
                  else
                    {
                      NS_LOG_INFO ("Link Stability Info Corrupt");
                    }
                }
            }
        }
    }
  // clean the best route table
  m_bestRoutesTable_link.clear ();
  for (std::map<Ipv4Address, Ipv4Address>::iterator i = pre.begin (); i != pre.end (); ++i)
    {
      // loop for all vertexes
      RouteCacheEntry::IP_VECTOR route;
      Ipv4Address iptemp = i->first;

      if (!i->second.IsBroadcast () && iptemp != source)
        {
          while (iptemp != source)
            {
              route.push_back (iptemp);
              iptemp = pre[iptemp];
            }
          route.push_back (source);
          // Reverse the route
          RouteCacheEntry::IP_VECTOR reverseroute;
          for (RouteCacheEntry::IP_VECTOR::reverse_iterator j = route.rbegin (); j != route.rend (); ++j)
            {
              reverseroute.push_back (*j);
            }
          NS_LOG_LOGIC ("Add newly calculated best routes");
          PrintVector (reverseroute);
          m_bestRoutesTable_link[i->first] = reverseroute;
        }
    }
}

bool
RouteCache::LookupRoute_Link (Ipv4Address id, RouteCacheEntry & rt)
{
  NS_LOG_FUNCTION (this << id);
  /// We need to purge the link node cache
  PurgeLinkNode ();
  std::map<Ipv4Address, RouteCacheEntry::IP_VECTOR>::const_iterator i = m_bestRoutesTable_link.find (id);
  if (i == m_bestRoutesTable_link.end ())
    {
      NS_LOG_INFO ("No route find to " << id);
      return false;
    }
  else
    {
      if (i->second.size () < 2)
        {
          NS_LOG_LOGIC ("Route to " << id << " error");
          return false;
        }

      RouteCacheEntry newEntry; // Create the route entry
      newEntry.SetVector (i->second);
      newEntry.SetDestination (id);
      newEntry.SetExpireTime (RouteCacheTimeout);
      NS_LOG_INFO ("Route to " << id << " found with the length " << i->second.size ());
      rt = newEntry;
      std::vector<Ipv4Address> path = rt.GetVector ();
      PrintVector (path);
      return true;
    }
}

void
RouteCache::PurgeLinkNode ()
{
  NS_LOG_FUNCTION (this);
  for (std::map<Link, LinkStab>::iterator i = m_linkCache.begin (); i != m_linkCache.end (); )
    {
      NS_LOG_DEBUG ("The link stability " << i->second.GetLinkStability ().GetSeconds ());
      std::map<Link, LinkStab>::iterator itmp = i;
      if (i->second.GetLinkStability () <= Seconds (0))
        {
          ++i;
          m_linkCache.erase (itmp);
        }
      else
        {
          ++i;
        }
    }
  /// may need to remove them after verify
  for (std::map<Ipv4Address, NodeStab>::iterator i = m_nodeCache.begin (); i != m_nodeCache.end (); )
    {
      NS_LOG_DEBUG ("The node stability " << i->second.GetNodeStability ().GetSeconds ());
      std::map<Ipv4Address, NodeStab>::iterator itmp = i;
      if (i->second.GetNodeStability () <= Seconds (0))
        {
          ++i;
          m_nodeCache.erase (itmp);
        }
      else
        {
          ++i;
        }
    }
}

void
RouteCache::UpdateNetGraph ()
{
  NS_LOG_FUNCTION (this);
  m_netGraph.clear ();
  for (std::map<Link, LinkStab>::iterator i = m_linkCache.begin (); i != m_linkCache.end (); ++i)
    {
      // Here the weight is set as 1
      /// \todo May need to set different weight for different link here later
      uint32_t weight = 1;
      m_netGraph[i->first.m_low][i->first.m_high] = weight;
      m_netGraph[i->first.m_high][i->first.m_low] = weight;
    }
}

bool
RouteCache::IncStability (Ipv4Address node)
{
  NS_LOG_FUNCTION (this << node);
  std::map<Ipv4Address, NodeStab>::const_iterator i = m_nodeCache.find (node);
  if (i == m_nodeCache.end ())
    {
      NS_LOG_INFO ("The initial stability " << m_initStability.GetSeconds ());
      NodeStab ns (m_initStability);
      m_nodeCache[node] = ns;
      return false;
    }
  else
    {
      /// \todo get rid of the debug here
      NS_LOG_INFO ("The node stability " << i->second.GetNodeStability ().GetSeconds ());
      NS_LOG_INFO ("The stability here " << Time (i->second.GetNodeStability () * m_stabilityIncrFactor).GetSeconds ());
      NodeStab ns (Time (i->second.GetNodeStability () * m_stabilityIncrFactor));
      m_nodeCache[node] = ns;
      return true;
    }
  return false;
}

bool
RouteCache::DecStability (Ipv4Address node)
{
  NS_LOG_FUNCTION (this << node);
  std::map<Ipv4Address, NodeStab>::const_iterator i = m_nodeCache.find (node);
  if (i == m_nodeCache.end ())
    {
      NodeStab ns (m_initStability);
      m_nodeCache[node] = ns;
      return false;
    }
  else
    {
      /// \todo remove it here
      NS_LOG_INFO ("The stability here " << i->second.GetNodeStability ().GetSeconds ());
      NS_LOG_INFO ("The stability here " << Time (i->second.GetNodeStability () / m_stabilityDecrFactor).GetSeconds ());
      NodeStab ns (Time (i->second.GetNodeStability () / m_stabilityDecrFactor));
      m_nodeCache[node] = ns;
      return true;
    }
  return false;
}

bool
RouteCache::AddRoute_Link (RouteCacheEntry::IP_VECTOR nodelist, Ipv4Address source)
{
  NS_LOG_FUNCTION (this << source);
  NS_LOG_LOGIC ("Use Link Cache");
  /// Purge the link node cache first
  PurgeLinkNode ();
  for (uint32_t i = 0; i < nodelist.size () - 1; i++)
    {
      NodeStab ns;                /// This is the node stability
      ns.SetNodeStability (m_initStability);

      if (m_nodeCache.find (nodelist[i]) == m_nodeCache.end ())
        {
          m_nodeCache[nodelist[i]] = ns;
        }
      if (m_nodeCache.find (nodelist[i + 1]) == m_nodeCache.end ())
        {
          m_nodeCache[nodelist[i + 1]] = ns;
        }
      Link link (nodelist[i], nodelist[i + 1]);         /// Link represent the one link for the route
      LinkStab stab;                /// Link stability
      stab.SetLinkStability (m_initStability);
      /// Set the link stability as the smallest node stability
      if (m_nodeCache[nodelist[i]].GetNodeStability () < m_nodeCache[nodelist[i + 1]].GetNodeStability ())
        {
          stab.SetLinkStability (m_nodeCache[nodelist[i]].GetNodeStability ());
        }
      else
        {
          stab.SetLinkStability (m_nodeCache[nodelist[i + 1]].GetNodeStability ());
        }
      if (stab.GetLinkStability () < m_minLifeTime)
        {
          NS_LOG_LOGIC ("Stability: " << stab.GetLinkStability ().GetSeconds ());
          /// Set the link stability as the m)minLifeTime, default is 1 second
          stab.SetLinkStability (m_minLifeTime);
        }
      m_linkCache[link] = stab;
      NS_LOG_DEBUG ("Add a new link");
      link.Print ();
      NS_LOG_DEBUG ("Link Info");
      stab.Print ();
    }
  UpdateNetGraph ();
  RebuildBestRouteTable (source);
  return true;
}

void
RouteCache::UseExtends (RouteCacheEntry::IP_VECTOR rt)
{
  NS_LOG_FUNCTION (this);
  /// Purge the link node cache first
  PurgeLinkNode ();
  if (rt.size () < 2)
    {
      NS_LOG_INFO ("The route is too short");
      return;
    }
  for (RouteCacheEntry::IP_VECTOR::iterator i = rt.begin (); i != rt.end () - 1; ++i)
    {
      Link link (*i, *(i + 1));
      if (m_linkCache.find (link) != m_linkCache.end ())
        {
          if (m_linkCache[link].GetLinkStability () < m_useExtends)
            {
              m_linkCache[link].SetLinkStability (m_useExtends);
              /// \todo remove after debug
              NS_LOG_INFO ("The time of the link " << m_linkCache[link].GetLinkStability ().GetSeconds ());
            }
        }
      else
        {
          NS_LOG_INFO ("We cannot find a link in cache");
        }
    }
  /// Increase the stability of the node cache
  for (RouteCacheEntry::IP_VECTOR::iterator i = rt.begin (); i != rt.end (); ++i)
    {
      if (m_nodeCache.find (*i) != m_nodeCache.end ())
        {
          NS_LOG_LOGIC ("Increase the stability");
          if (m_nodeCache[*i].GetNodeStability () <= m_initStability)
            {
              IncStability (*i);
            }
          else
            {
              NS_LOG_INFO ("The node stability has already been increased");
            }
        }
    }
}

bool
RouteCache::AddRoute (RouteCacheEntry & rt)
{
  NS_LOG_FUNCTION (this);
  Purge ();
  std::list<RouteCacheEntry> rtVector;   // Declare the route cache entry vector
  Ipv4Address dst = rt.GetDestination ();
  std::vector<Ipv4Address> route = rt.GetVector ();

  NS_LOG_DEBUG ("The route destination we have " << dst);
  std::map<Ipv4Address, std::list<RouteCacheEntry> >::const_iterator i =
    m_sortedRoutes.find (dst);

  if (i == m_sortedRoutes.end ())
    {
      rtVector.push_back (rt);
      m_sortedRoutes.erase (dst);   // Erase the route entries for dst first
      /**
       * Save the new route cache along with the destination address in map
       */
      std::pair<std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator, bool> result =
        m_sortedRoutes.insert (std::make_pair (dst, rtVector));
      return result.second;
    }
  else
    {
      rtVector = i->second;
      NS_LOG_DEBUG ("The existing route size " << rtVector.size () << " for destination address " << dst);
      /**
       * \brief Drop the most aged packet when buffer reaches to max
       */
      if (rtVector.size () >= m_maxEntriesEachDst)
        {
          RemoveLastEntry (rtVector);         // Drop the last entry for the sorted route cache, the route has already been sorted
        }

      if (FindSameRoute (rt, rtVector))
        {
          NS_LOG_DEBUG ("Find same vector, the FindSameRoute function will update the route expire time");
          return true;
        }
      else
        {
          // Check if the expire time for the new route has expired or not
          if (rt.GetExpireTime () > 0)
            {
              rtVector.push_back (rt);
              // This sort function will sort the route cache entries based on the size of route in each of the
              // route entries
              rtVector.sort (CompareRoutesExpire);
              NS_LOG_DEBUG ("The first time" << rtVector.front ().GetExpireTime ().GetSeconds () << " The second time "
                                             << rtVector.back ().GetExpireTime ().GetSeconds ());
              NS_LOG_DEBUG ("The first hop" << rtVector.front ().GetVector ().size () << " The second hop "
                                            << rtVector.back ().GetVector ().size ());
              m_sortedRoutes.erase (dst);               // erase the route entries for dst first
              /**
               * Save the new route cache along with the destination address in map
               */
              std::pair<std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator, bool> result =
                m_sortedRoutes.insert (std::make_pair (dst, rtVector));
              return result.second;
            }
          else
            {
              NS_LOG_INFO ("The newly found route is already expired");
            }
        }
    }
  return false;
}

bool RouteCache::FindSameRoute (RouteCacheEntry & rt, std::list<RouteCacheEntry> & rtVector)
{
  NS_LOG_FUNCTION (this);
  for (std::list<RouteCacheEntry>::iterator i = rtVector.begin (); i != rtVector.end (); ++i)
    {
      // return the first route in the route vector
      RouteCacheEntry::IP_VECTOR routeVector = i->GetVector ();
      RouteCacheEntry::IP_VECTOR newVector = rt.GetVector ();

      if (routeVector == newVector)
        {
          NS_LOG_DEBUG ("Found same routes in the route cache with the vector size "
                        << rt.GetDestination () << " " << rtVector.size ());
          NS_LOG_DEBUG ("The new route expire time " << rt.GetExpireTime ().GetSeconds ()
                                                     << " the original expire time " << i->GetExpireTime ().GetSeconds ());
          if (rt.GetExpireTime () > i->GetExpireTime ())
            {
              i->SetExpireTime (rt.GetExpireTime ());
            }
          m_sortedRoutes.erase (rt.GetDestination ()); // erase the entry first
          rtVector.sort (CompareRoutesExpire);  // sort the route vector first
          /*
           * Save the new route cache along with the destination address in map
           */
          std::pair<std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator, bool> result =
            m_sortedRoutes.insert (std::make_pair (rt.GetDestination (), rtVector));
          return result.second;
        }
    }
  return false;
}

bool
RouteCache::DeleteRoute (Ipv4Address dst)
{
  NS_LOG_FUNCTION (this << dst);
  Purge (); // purge the route cache first to remove timeout entries
  if (m_sortedRoutes.erase (dst) != 0)
    {
      NS_LOG_LOGIC ("Route deletion to " << dst << " successful");
      return true;
    }
  NS_LOG_LOGIC ("Route deletion to " << dst << " not successful");
  return false;
}

void
RouteCache::DeleteAllRoutesIncludeLink (Ipv4Address errorSrc, Ipv4Address unreachNode, Ipv4Address node)
{
  NS_LOG_FUNCTION (this << errorSrc << unreachNode << node);
  if (IsLinkCache ())
    {
      // Purge the link node cache first
      PurgeLinkNode ();
      /*
       * The followings are for cleaning the broken link in link cache
       * We basically remove the link between errorSrc and unreachNode
       */
      Link link1 (errorSrc, unreachNode);
      Link link2 (unreachNode, errorSrc);
      // erase the two kind of links to make sure the link is removed from the link cache
      NS_LOG_DEBUG ("Erase the route");
      m_linkCache.erase (link1);
      /// \todo get rid of this one
      NS_LOG_DEBUG ("The link cache size " << m_linkCache.size());
      m_linkCache.erase (link2);
      NS_LOG_DEBUG ("The link cache size " << m_linkCache.size());

      std::map<Ipv4Address, NodeStab>::iterator i = m_nodeCache.find (errorSrc);
      if (i == m_nodeCache.end ())
        {
          NS_LOG_LOGIC ("Update the node stability unsuccessfully");
        }
      else
        {
          DecStability (i->first);
        }
      i = m_nodeCache.find (unreachNode);
      if (i == m_nodeCache.end ())
        {
          NS_LOG_LOGIC ("Update the node stability unsuccessfully");
        }
      else
        {
          DecStability (i->first);
        }
      UpdateNetGraph ();
      RebuildBestRouteTable (node);
    }
  else
    {
      /*
       * the followings are for cleaning the broken link in pathcache
       *
       */
      Purge ();
      if (m_sortedRoutes.empty ())
        {
          return;
        }
      /*
       * Loop all the routes saved in the route cache
       */
      for (std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator j =
             m_sortedRoutes.begin (); j != m_sortedRoutes.end (); )
        {
          std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator jtmp = j;
          Ipv4Address address = j->first;
          std::list<RouteCacheEntry> rtVector = j->second;
          /*
           * Loop all the routes for a single destination
           */
          for (std::list<RouteCacheEntry>::iterator k = rtVector.begin (); k != rtVector.end (); )
            {
              // return the first route in the route vector
              RouteCacheEntry::IP_VECTOR routeVector = k->GetVector ();
              RouteCacheEntry::IP_VECTOR changeVector;
              /*
               * Loop the ip addresses within a single route entry
               */
              for (RouteCacheEntry::IP_VECTOR::iterator i = routeVector.begin (); i != routeVector.end (); ++i)
                {
                  if (*i != errorSrc)
                    {
                      changeVector.push_back (*i);
                    }
                  else
                    {
                      if (*(i + 1) == unreachNode)
                        {
                          changeVector.push_back (*i);
                          break;
                        }
                      else
                        {
                          changeVector.push_back (*i);
                        }
                    }
                }
              /*
               * Verify if need to remove some affected links
               */
              if (changeVector.size () == routeVector.size ())
                {
                  NS_LOG_DEBUG ("The route does not contain the broken link");
                  ++k;
                }
              else if ((changeVector.size () < routeVector.size ()) && (changeVector.size () > 1))
                {
                  NS_LOG_DEBUG ("sub route " << m_subRoute);
                  if (m_subRoute)
                    {
                      Time expire = k->GetExpireTime ();
                      /*
                       * Remove the route first
                       */
                      k = rtVector.erase (k);
                      RouteCacheEntry changeEntry;
                      changeEntry.SetVector (changeVector);
                      Ipv4Address destination = changeVector.back ();
                      NS_LOG_DEBUG ("The destination of the newly formed route " << destination << " and the size of the route " << changeVector.size ());
                      changeEntry.SetDestination (destination);
                      changeEntry.SetExpireTime (expire); // Initialize the timeout value to the one it has
                      rtVector.push_back (changeEntry);  // Add the route entry to the route list
                      NS_LOG_DEBUG ("We have a sub-route to " << destination);
                    }
                  else
                    {
                      /*
                       * Remove the route
                       */
                      k = rtVector.erase (k);
                    }
                }
              else
                {
                  NS_LOG_LOGIC ("Cut route unsuccessful and erase the route");
                  /*
                   * Remove the route
                   */
                  k = rtVector.erase (k);
                }
            }
          ++j;
          if (!IsLinkCache ())
            {
              m_sortedRoutes.erase (jtmp);
            }
          if (rtVector.size ())
            {
              /*
               * Save the new route cache along with the destination address in map
               */
              rtVector.sort (CompareRoutesExpire);
              m_sortedRoutes[address] = rtVector;
            }
          else
            {
              NS_LOG_DEBUG ("There is no route left for that destination " << address);
            }
        }
    }
}

void
RouteCache::PrintVector (std::vector<Ipv4Address>& vec)
{
  NS_LOG_FUNCTION (this);
  /*
   * Check elements in a route vector, used when one wants to check the IP addresses saved in
   */
  if (!vec.size ())
    {
      NS_LOG_DEBUG ("The vector is empty");
    }
  else
    {
      NS_LOG_DEBUG ("Print all the elements in a vector");
      for (std::vector<Ipv4Address>::const_iterator i = vec.begin (); i != vec.end (); ++i)
        {
          NS_LOG_DEBUG ("The ip address " << *i);
        }
    }
}

void
RouteCache::PrintRouteVector (std::list<RouteCacheEntry> route)
{
  NS_LOG_FUNCTION (this);
  for (std::list<RouteCacheEntry>::iterator i = route.begin (); i != route.end (); i++)
    {
      std::vector<Ipv4Address> path = i->GetVector ();
      NS_LOG_INFO ("Route NO. ");
      PrintVector (path);
    }
}

void
RouteCache::Purge ()
{
  NS_LOG_FUNCTION (this);
  //Trying to purge the route cache
  if (m_sortedRoutes.empty ())
    {
      NS_LOG_DEBUG ("The route cache is empty");
      return;
    }
  for (std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator i =
         m_sortedRoutes.begin (); i != m_sortedRoutes.end (); )
    {
      // Loop of route cache entry with the route size
      std::map<Ipv4Address, std::list<RouteCacheEntry> >::iterator itmp = i;
      /*
       * The route cache entry vector
       */
      Ipv4Address dst = i->first;
      std::list<RouteCacheEntry> rtVector = i->second;
      NS_LOG_DEBUG ("The route vector size of 1 " << dst << " " << rtVector.size ());
      if (rtVector.size ())
        {
          for (std::list<RouteCacheEntry>::iterator j = rtVector.begin (); j != rtVector.end (); )
            {
              NS_LOG_DEBUG ("The expire time of every entry with expire time " << j->GetExpireTime ());
              /*
               * First verify if the route has expired or not
               */
              if (j->GetExpireTime () <= Seconds (0))
                {
                  /*
                   * When the expire time has passed, erase the certain route
                   */
                  NS_LOG_DEBUG ("Erase the expired route for " << dst << " with expire time " << j->GetExpireTime ());
                  j = rtVector.erase (j);
                }
              else
                {
                  ++j;
                }
            }
          NS_LOG_DEBUG ("The route vector size of 2 " << dst << " " << rtVector.size ());
          if (rtVector.size ())
            {
              ++i;
              m_sortedRoutes.erase (itmp); // erase the entry first
              /*
               * Save the new route cache along with the destination address in map
               */
              m_sortedRoutes.insert (std::make_pair (dst, rtVector));
            }
          else
            {
              ++i;
              m_sortedRoutes.erase (itmp);
            }
        }
      else
        {
          ++i;
          m_sortedRoutes.erase (itmp);
        }
    }
  return;
}

void
RouteCache::Print (std::ostream &os)
{
  NS_LOG_FUNCTION (this);
  Purge ();
  os << "\nDSR Route Cache\n"
     << "Destination\tGateway\t\tInterface\tFlag\tExpire\tHops\n";
  for (std::list<RouteCacheEntry>::const_iterator i =
         m_routeEntryVector.begin (); i != m_routeEntryVector.end (); ++i)
    {
      i->Print (os);
    }
  os << "\n";
}

// ----------------------------------------------------------------------------------------------------------
/**
 * This part of code maintains an Acknowledgment id cache for next hop and remove duplicate ids
 */
uint16_t
RouteCache::CheckUniqueAckId (Ipv4Address nextHop)
{
  NS_LOG_FUNCTION (this);
  std::map<Ipv4Address, uint16_t>::const_iterator i =
    m_ackIdCache.find (nextHop);
  if (i == m_ackIdCache.end ())
    {
      NS_LOG_LOGIC ("No Ack id for " << nextHop << " found and use id 1 for the first network ack id");
      m_ackIdCache[nextHop] = 1;
      return 1;
    }
  else
    {
      uint16_t ackId = m_ackIdCache[nextHop];
      NS_LOG_LOGIC ("Ack id for " << nextHop << " found in the cache has value " << ackId);
      ackId++;
      m_ackIdCache[nextHop] = ackId;
      return ackId;
    }
}

uint16_t
RouteCache::GetAckSize ()
{
  return m_ackIdCache.size ();
}

// ----------------------------------------------------------------------------------------------------------
/**
 * This part maintains a neighbor list to handle unidirectional links and link-layer acks
 */
bool
RouteCache::IsNeighbor (Ipv4Address addr)
{
  NS_LOG_FUNCTION (this);
  PurgeMac ();  // purge the mac cache
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin ();
       i != m_nb.end (); ++i)
    {
      if (i->m_neighborAddress == addr)
        {
          return true;
        }
    }
  return false;
}

Time
RouteCache::GetExpireTime (Ipv4Address addr)
{
  NS_LOG_FUNCTION (this);
  PurgeMac ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i
       != m_nb.end (); ++i)
    {
      if (i->m_neighborAddress == addr)
        {
          return (i->m_expireTime - Simulator::Now ());
        }
    }
  return Seconds (0);
}

void
RouteCache::UpdateNeighbor (std::vector<Ipv4Address> nodeList, Time expire)
{
  NS_LOG_FUNCTION (this);
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    {
      for (std::vector<Ipv4Address>::iterator j = nodeList.begin (); j != nodeList.end (); ++j)
        {
          if (i->m_neighborAddress == (*j))
            {
              i->m_expireTime
                = std::max (expire + Simulator::Now (), i->m_expireTime);
              if (i->m_hardwareAddress == Mac48Address ())
                {
                  i->m_hardwareAddress = LookupMacAddress (i->m_neighborAddress);
                }
              return;
            }
        }
    }

  Ipv4Address addr;
  NS_LOG_LOGIC ("Open link to " << addr);
  Neighbor neighbor (addr, LookupMacAddress (addr), expire + Simulator::Now ());
  m_nb.push_back (neighbor);
  PurgeMac ();
}

void
RouteCache::AddNeighbor (std::vector<Ipv4Address> nodeList, Ipv4Address ownAddress, Time expire)
{
  NS_LOG_LOGIC ("Add neighbor number " << nodeList.size ());
  for (std::vector<Ipv4Address>::iterator j = nodeList.begin (); j != nodeList.end (); ++j)
    {
      Ipv4Address addr = *j;
      if (addr == ownAddress)
        {
          nodeList.erase (j);
          NS_LOG_DEBUG ("The node list size " << nodeList.size ());
        }
      Neighbor neighbor (addr, LookupMacAddress (addr), expire + Simulator::Now ());
      m_nb.push_back (neighbor);
      PurgeMac ();
    }
}

struct CloseNeighbor
{
  bool operator() (const RouteCache::Neighbor & nb) const
  {
    return ((nb.m_expireTime < Simulator::Now ()) || nb.close);
  }
};

void
RouteCache::PurgeMac ()
{
  if (m_nb.empty ())
    {
      return;
    }

  CloseNeighbor pred;
  if (!m_handleLinkFailure.IsNull ())
    {
      for (std::vector<Neighbor>::iterator j = m_nb.begin (); j != m_nb.end (); ++j)
        {
          if (pred (*j))
            {
              NS_LOG_LOGIC ("Close link to " << j->m_neighborAddress);
              /// \todo disable temporarily
//              m_handleLinkFailure (j->m_neighborAddress);
            }
        }
    }
  m_nb.erase (std::remove_if (m_nb.begin (), m_nb.end (), pred), m_nb.end ());
  m_ntimer.Cancel ();
  m_ntimer.Schedule ();
}

void
RouteCache::ScheduleTimer ()
{
  m_ntimer.Cancel ();
  m_ntimer.Schedule ();
}

void
RouteCache::AddArpCache (Ptr<ArpCache> a)
{
  m_arp.push_back (a);
}

void
RouteCache::DelArpCache (Ptr<ArpCache> a)
{
  m_arp.erase (std::remove (m_arp.begin (), m_arp.end (), a), m_arp.end ());
}

Mac48Address
RouteCache::LookupMacAddress (Ipv4Address addr)
{
  Mac48Address hwaddr;
  for (std::vector<Ptr<ArpCache> >::const_iterator i = m_arp.begin ();
       i != m_arp.end (); ++i)
    {
      ArpCache::Entry * entry = (*i)->Lookup (addr);
      if (entry != 0 && entry->IsAlive () && !entry->IsExpired ())
        {
          hwaddr = Mac48Address::ConvertFrom (entry->GetMacAddress ());
          break;
        }
    }
  return hwaddr;
}

void
RouteCache::ProcessTxError (WifiMacHeader const & hdr)
{
  Mac48Address addr = hdr.GetAddr1 ();

  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    {
      if (i->m_hardwareAddress == addr)
        {
          i->close = true;
        }
    }
  PurgeMac ();
}
} // namespace dsr
} // namespace ns3
