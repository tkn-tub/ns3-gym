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
 *              Song Luan <lsuper@mail.ustc.edu.cn> (Implemented Link Cache using dijsktra algorithm to get the best route)
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

#ifndef DSR_RCACHE_H
#define DSR_RCACHE_H

#include <map>
#include <stdint.h>
#include <cassert>
#include <sys/types.h>
#include <iostream>
#include <vector>

#include "ns3/simulator.h"
#include "ns3/timer.h"
#include "ns3/simple-ref-count.h"
#include "ns3/header.h"
#include "ns3/enum.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/net-device.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/callback.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/arp-cache.h"
#include "dsr-option-header.h"

namespace ns3 {
class Time;
namespace dsr {

/*
 * The route cache structure
  \verbatim
  +-+-+-+-+-+-+-+-+-+-+-+-         +-+-+-+-+-+-+-+-+-+-+-            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
  |  Destination Address |---------| Route Cache Entry  | ---------- | IP_VECTOR | dst | exp time |
  +-+-+-+-+-+-+-+-+-+-+-+-   Map   +-+-+-+-+-+-+-+-+-+-+-  Contains  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
                                   +-+-+-+-+-+-+-+-+-+-+-            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
                                   | Route Cache Entry  | ---------- | IP_VECTOR | dst | exp time |
                                   +-+-+-+-+-+-+-+-+-+-+-  Contains  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
                                             .                                       .
                                             .                                       .
                                             .                                       .
                                             .                                       .
                                   +-+-+-+-+-+-+-+-+-+-+-            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
                                   | Route Cache Entry  | ---------- | IP_VECTOR | dst | exp time |
                                   +-+-+-+-+-+-+-+-+-+-+-  Contains  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

  \endverbatim
 */
/**
 * \ingroup dsr
 * \brief DSR Route Cache Entry
 */
struct Link
{
  Ipv4Address m_low;
  Ipv4Address m_high;
  Link (Ipv4Address ip1, Ipv4Address ip2)
  {
    if (ip1 < ip2)
      {
        m_low = ip1;
        m_high = ip2;
      }
    else
      {
        m_low = ip2;
        m_high = ip1;
      }
  }
  bool operator < (Link const& L) const
  {
    if (m_low < L.m_low)
      {
        return true;
      }
    else if (m_low == L.m_low)
      {
        return (m_high < L.m_high);
      }
    else
      {
        return false;
      }
  }
  void Print () const;
};

class LinkStab
{
public:
  /**
   * \brief Constructor
   */
  LinkStab (Time linkStab = Simulator::Now ());
  /**
   * \brief Destructor
   */
  virtual ~LinkStab ();

  void SetLinkStability (Time linkStab)
  {
    m_linkStability = linkStab + Simulator::Now ();
  }
  Time GetLinkStability () const
  {
    return m_linkStability - Simulator::Now ();
  }

  void Print () const;

private:
  /*
   * The link stability lifetime expected, when the time is due, the link expires the expiration happens
   * when purge the node and link cache before update them when receiving new information
   */
  Time m_linkStability;
};

class NodeStab
{
public:
  /**
   * \brief Constructor
   */
//  NodeStab ();
  NodeStab (Time nodeStab = Simulator::Now ());
  /**
   * \brief Destructor
   */
  virtual ~NodeStab ();

  void SetNodeStability (Time nodeStab)
  {
    m_nodeStability = nodeStab + Simulator::Now ();
  }
  Time GetNodeStability () const
  {
    return m_nodeStability - Simulator::Now ();
  }
private:
  Time m_nodeStability;
};

class RouteCacheEntry
{
public:
  typedef std::vector<Ipv4Address> IP_VECTOR;                // Define the vector to hold Ip address
  typedef std::vector<Ipv4Address>::iterator Iterator;       // Define the iterator
  // / c-tor
  /**
   * \brief Constructor
   */
  RouteCacheEntry (IP_VECTOR const  & ip = IP_VECTOR (), Ipv4Address dst = Ipv4Address (), Time exp = Simulator::Now ());
  /**
   * \brief Destructor
   */
  virtual ~RouteCacheEntry ();
  // / Mark entry as "down" (i.e. disable it)
  void Invalidate (Time badLinkLifetime);
  // /\name Fields
  // \{
  void SetUnidirectional (bool u)
  {
    m_blackListState = u;
  }
  bool IsUnidirectional () const
  {
    return m_blackListState;
  }
  void SetBlacklistTimeout (Time t)
  {
    m_blackListTimeout = t;
  }
  Time GetBlacklistTimeout () const
  {
    return m_blackListTimeout;
  }
  Ipv4Address GetDestination () const
  {
    return m_dst;
  }
  void SetDestination (Ipv4Address d)
  {
    m_dst = d;
  }
  IP_VECTOR GetVector () const
  {
    return m_path;
  }
  void SetVector (IP_VECTOR v)
  {
    m_path = v;
  }
  void SetExpireTime (Time exp)
  {
    m_expire = exp + Simulator::Now ();
  }
  Time GetExpireTime () const
  {
    return m_expire - Simulator::Now ();
  }
  // \}
  /**
   * \brief Print necessary fields
   */
  void Print (std::ostream & os) const;
  /**
   * \brief Compare the route cache entry
   * \return true if equal
   */
  bool operator== (RouteCacheEntry const & o) const
  {
    if (m_path.size () != o.m_path.size ())
      {
        NS_ASSERT (false);
        return false;
      }
    IP_VECTOR::const_iterator j = o.m_path.begin ();
    for (IP_VECTOR::const_iterator i = m_path.begin (); i
         != m_path.end (); i++, j++)
      {
        /*
         * Verify if neither the entry are not 0 and they equal to each other
         */
        if (((*i) == 0) || ((*j) == 0))
          {
            return false;
          }
        else if (!((*i) == (*j)) )
          {
            return false;
          }
        else
          {
            return true;
          }
      }
    return false;
  }
  // \}

private:
  // / RREP_ACK timer
  Timer m_ackTimer;
  // / The destination Ip address
  Ipv4Address m_dst;
  // / brief The IP address constructed route
  IP_VECTOR m_path;
  // / Expire time for queue entry
  Time m_expire;
  // / Output interface address
  Ipv4InterfaceAddress m_iface;
  // / Number of route requests
  uint8_t m_reqCount;
  // / Indicate if this entry is in "blacklist"
  bool m_blackListState;
  // / Time for which the node is put into the blacklist
  Time m_blackListTimeout;
  // / The Ipv4 route
  Ptr<Ipv4Route> m_ipv4Route;
  // / The Ipv4 layer 3
  Ptr<Ipv4> m_ipv4;
};
/**
 * \ingroup dsr
 * \brief DSR route request queue
 * Since DSR is an on demand routing we queue requests while looking for route.
 */
class RouteCache : public Object
{
public:
  // / Default c-tor
/**
 * \ingroup dsr
 * \brief The Route Cache used by DSR
 */
  static TypeId GetTypeId ();
  /**
   * \brief Constructor.
   */
  RouteCache ();
  /**
   * \brief Destructor.
   */
  virtual ~RouteCache ();
  // / Remove the aged route cache entries when the route cache is full
  void RemoveLastEntry (std::list<RouteCacheEntry> & rtVector);
  // / Define the vector of route entries.
  typedef std::list<RouteCacheEntry::IP_VECTOR> routeVector;
  // / Get the destination address of the route.
  Ipv4Address GetDestination (void) const;
  // / Remove all packets with destination IP address dst
  void DropPathWithDst (Ipv4Address dst);
  // / To know if the two entries are the same
  bool IsEqual (RouteCacheEntry ca);
  // /\name Fields
  // \{
  bool GetSubRoute () const
  {
    return m_subRoute;
  }
  void SetSubRoute (bool subRoute)
  {
    m_subRoute = subRoute;
  }
  uint32_t GetMaxCacheLen () const
  {
    return m_maxCacheLen;
  }
  void SetMaxCacheLen (uint32_t len)
  {
    m_maxCacheLen = len;
  }
  Time GetCacheTimeout () const
  {
    return RouteCacheTimeout;
  }
  void SetCacheTimeout (Time t)
  {
    RouteCacheTimeout = t;
  }
  uint32_t GetMaxEntriesEachDst () const
  {
    return m_maxEntriesEachDst;
  }
  void SetMaxEntriesEachDst (uint32_t entries)
  {
    m_maxEntriesEachDst = entries;
  }
  Time GetBadLinkLifetime () const
  {
    return m_badLinkLifetime;
  }
  void SetBadLinkLifetime (Time t)
  {
    m_badLinkLifetime = t;
  }
  uint64_t GetStabilityDecrFactor () const
  {
    return m_stabilityDecrFactor;
  }
  void SetStabilityDecrFactor (uint64_t decrFactor)
  {
    m_stabilityDecrFactor = decrFactor;
  }
  uint64_t GetStabilityIncrFactor () const
  {
    return m_stabilityIncrFactor;
  }
  void SetStabilityIncrFactor (uint64_t incrFactor)
  {
    m_stabilityIncrFactor = incrFactor;
  }
  Time GetInitStability () const
  {
    return m_initStability;
  }
  void SetInitStability (Time initStability)
  {
    m_initStability = initStability;
  }
  Time GetMinLifeTime () const
  {
    return m_minLifeTime;
  }
  void SetMinLifeTime (Time minLifeTime)
  {
    m_minLifeTime = minLifeTime;
  }
  Time GetUseExtends () const
  {
    return m_useExtends;
  }
  void SetUseExtends (Time useExtends)
  {
    m_useExtends = useExtends;
  }
  // \}
  /**
   * Update route cache entry if it has been recently used and successfully delivered the data packet
   * \param dst destination address of the route
   * \param vec the route vector
   * \return true in success
   */
  bool UpdateRouteEntry (Ipv4Address dst);
  /**
   * Add route cache entry if it doesn't yet exist in route cache
   * \param rt route cache entry
   * \return true in success
   */
  bool AddRoute (RouteCacheEntry & rt);
  /**
   * Lookup route cache entry with destination address dst
   * \param dst destination address
   * \param rt entry with destination address dst, if exists
   * \return true on success
   */
  bool LookupRoute (Ipv4Address id, RouteCacheEntry & rt);
  /**
   * Print the route vector elements
   * \param vec the route vector
   */
  void PrintVector (std::vector<Ipv4Address>& vec);
  /**
   * Print all the route vector elements from the route list
   * \param route the route list
   */
  void PrintRouteVector (std::list<RouteCacheEntry> route);
  /**
   * Find the same route in the route cache
   * \param rt entry with destination address dst, if exists
   * \param rtVector the route vector
   */
  bool FindSameRoute (RouteCacheEntry & rt, std::list<RouteCacheEntry> & rtVector);
  /**
   * Delete the route with certain destination address
   * \param dst the destination address of the routes that should be deleted
   */
  bool DeleteRoute (Ipv4Address dst);
  /*
   * Delete all the routes which includes the link from next hop address that has just been notified
   * as unreachable
   */
  void DeleteAllRoutesIncludeLink (Ipv4Address errorSrc, Ipv4Address unreachNode, Ipv4Address node);
  // / Delete all entries from routing table
  void Clear ()
  {
    m_routeEntryVector.erase (m_routeEntryVector.begin (), m_routeEntryVector.end ());
  }
  // / Delete all outdated entries and invalidate valid entry if Lifetime is expired
  void Purge ();
  // / Print route cache
  void Print (std::ostream &os);

  //------------------------------------------------------------------------------------------
  /*
   * The following code deals with duplicate ack ids
   */
  // / Check for duplicate ids and save new entries if the id is not present in the table
  uint16_t CheckUniqueAckId (Ipv4Address nextHop);
  // / Get the ack table size
  uint16_t GetAckSize ();

  // --------------------------------------------------------------------------------------------
  /*
   * The following code handles link-layer acks
   */
  // / Neighbor description
  struct Neighbor
  {
    Ipv4Address m_neighborAddress;
    Mac48Address m_hardwareAddress;
    Time m_expireTime;
    bool close;

    Neighbor (Ipv4Address ip, Mac48Address mac, Time t)
      : m_neighborAddress (ip),
        m_hardwareAddress (mac),
        m_expireTime (t),
        close (false)
    {
    }

    Neighbor () {} // For Python bindings
  };
  // / Return expire time for neighbor node with address addr, if exists, else return 0.
  Time GetExpireTime (Ipv4Address addr);
  // / Check that node with address addr  is neighbor
  bool IsNeighbor (Ipv4Address addr);
  // / Update expire time for entry with address addr, if it exists, else add new entry
  void UpdateNeighbor (std::vector<Ipv4Address> nodeList, Time expire);
  // / Add to the neighbor list
  void AddNeighbor (std::vector<Ipv4Address> nodeList, Ipv4Address ownAddress, Time expire);
  // / Remove all expired mac entries
  void PurgeMac ();
  // / Schedule m_ntimer.
  void ScheduleTimer ();
  // / Remove all entries
  void ClearMac ()
  {
    m_nb.clear ();
  }
  // / Add ARP cache to be used to allow layer 2 notifications processing
  void AddArpCache (Ptr<ArpCache>);
  // / Don't use given ARP cache any more (interface is down)
  void DelArpCache (Ptr<ArpCache>);
  // / Get callback to ProcessTxError
  /*
   * This callback is trying to use the wifi mac tx error header to notify a link layer drop event, however,
   * it is not fully supported yet
   */
  Callback<void, WifiMacHeader const &> GetTxErrorCallback () const
  {
    return m_txErrorCallback;
  }
  // /\name Handle link failure callback
  // \{
  void SetCallback (Callback<void, Ipv4Address, uint8_t > cb)
  {
    m_handleLinkFailure = cb;
  }
  Callback<void, Ipv4Address, uint8_t > GetCallback () const
  {
    return m_handleLinkFailure;
  }
  // \}

private:
  RouteCache & operator= (RouteCache const &);
  RouteCacheEntry::IP_VECTOR m_vector; // /< The route vector to save the ip addresses for intermediate nodes.
  uint32_t m_maxCacheLen;              // /< The maximum number of packets that we allow a routing protocol to buffer.
  Time     RouteCacheTimeout;          // /< The maximum period of time that dsr is allowed to for an unused route.
  Time     m_badLinkLifetime;          // /< The time for which the neighboring node is put into the blacklist.
  /*
   * Define the parameters for link cache type
   */
  uint64_t m_stabilityDecrFactor;
  uint64_t m_stabilityIncrFactor;
  Time m_initStability;
  Time m_minLifeTime;
  Time m_useExtends;
  /*
   * Define the route cache data structure
   */
  typedef std::list<RouteCacheEntry> routeEntryVector;
  // / Map the ipv4Address to route entry vector
  std::map<Ipv4Address, routeEntryVector> m_sortedRoutes;
  // Define the route vector
  routeEntryVector m_routeEntryVector;
  // / number of entries for each destination
  uint32_t m_maxEntriesEachDst;
  // / The id cache to ensure all the ids are unique
  std::map<Ipv4Address, uint16_t> m_ackIdCache;
  // / Check if the route is using path cache or link cache
  bool m_isLinkCache;
  // / Check if save the sub route entries or not
  bool m_subRoute;
  /**
   * The link cache to update all the link status, bi-link is two link for link is a struct
   * when the weight is calculated we normalized them: 100*weight/max of Weight
   */
  #define MAXWEIGHT 0xFFFF;
  /**
   * Current network graph state for this node, double is weight, which is calculated by the node information
   * and link information, any time some changes of link cache and node cache
   * change the weight and then recompute the best choice for each node
   */
  std::map<Ipv4Address, std::map<Ipv4Address, uint32_t> > m_netGraph;
  // for link route cache
  std::map<Ipv4Address, RouteCacheEntry::IP_VECTOR> m_bestRoutesTable_link;
  std::map<Link, LinkStab> m_linkCache;
  std::map<Ipv4Address, NodeStab> m_nodeCache;
  // used by LookupRoute when LinkCache
  bool LookupRoute_Link (Ipv4Address id, RouteCacheEntry & rt);

  bool IncStability (Ipv4Address node);

  bool DecStability (Ipv4Address node);

public:
  /**
   * \brief dijsktra algorithm to get the best route from m_netGraph and update the m_bestRoutesTable_link
   * \when current graph information has changed
   */
  void SetCacheType (std::string type);
  bool IsLinkCache ();
  bool AddRoute_Link (RouteCacheEntry::IP_VECTOR nodelist, Ipv4Address node);
  /**
   *  \brief USE MAXWEIGHT TO REPRESENT MAX; USE BROADCAST ADDRESS TO REPRESENT NULL PRECEEDING ADDRESS
   */
  void RebuildBestRouteTable (Ipv4Address source);
  void PurgeLinkNode ();
  /**
   * When a link from the Route Cache is used in routing a packet originated or salvaged
   * by that node, the stability metric for each of the two endpoint nodes of that link is incremented by the
   * amount of time since that link was last used. When a link is used in a route chosen for a packet originated or
   * salvaged by this node, the link's lifetime is set to be at least UseExtends into the future
   */
  void UseExtends (RouteCacheEntry::IP_VECTOR rt);
  /**
   *  \brief Update the Net Graph for the link and node cache has changed
   */
  void UpdateNetGraph ();
  //---------------------------------------------------------------------------------------
  /**
   * The following code handles link-layer acks
   */
  // / link failure callback
  Callback<void, Ipv4Address, uint8_t > m_handleLinkFailure;
  // / TX error callback
  Callback<void, WifiMacHeader const &> m_txErrorCallback;
  // / Timer for neighbor's list. Schedule Purge().
  Timer m_ntimer;
  // / vector of entries
  std::vector<Neighbor> m_nb;
  // / list of ARP cached to be used for layer 2 notifications processing
  std::vector<Ptr<ArpCache> > m_arp;
  // This timeout deals with the passive ack
  Time m_delay;
  // / Find MAC address by IP using list of ARP caches
  Mac48Address LookupMacAddress (Ipv4Address);
  // / Process layer 2 TX error notification
  void ProcessTxError (WifiMacHeader const &);
};
} // namespace dsr
} // namespace ns3
#endif /* DSR_RCACHE_H */
