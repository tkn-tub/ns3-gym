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
#ifndef ARP_CACHE_H
#define ARP_CACHE_H

#include <stdint.h>
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/net-device.h"
#include "ns3/ipv4-address.h"
#include "ns3/address.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "sgi-hashmap.h"

namespace ns3 {

class NetDevice;
class Ipv4Interface;

/**
 * \brief An ARP cache
 *
 * A cached lookup table for translating layer 3 addresses to layer 2.
 * This implementation does lookups from IPv4 to a MAC address
 */
class ArpCache : public Object
{
public:
  static TypeId GetTypeId (void);
  class Entry;
  ArpCache ();
  ~ArpCache ();

  /**
   * \param device The hardware NetDevice associated with this ARP chache
   * \param interface the Ipv4Interface associated with this ARP chache
   */
  void SetDevice (Ptr<NetDevice> device, Ptr<Ipv4Interface> interface);
  /**
   * \return The NetDevice that this ARP cache is associated with
   */
  Ptr<NetDevice> GetDevice (void) const;
  /**
   * \return the Ipv4Interface that this ARP cache is associated with
   */
  Ptr<Ipv4Interface> GetInterface (void) const;
  
  void SetAliveTimeout (Time aliveTimeout);
  void SetDeadTimeout (Time deadTimeout);
  void SetWaitReplyTimeout (Time waitReplyTimeout);
  Time GetAliveTimeout (void) const;
  Time GetDeadTimeout (void) const;
  Time GetWaitReplyTimeout (void) const;

  /**
   * \brief Do lookup in the ARP chache against an IP address
   * \param destination The destination IPv4 address to lookup the MAC address
   * of
   * \return An ArpCache::Entry with info about layer 2
   */
  ArpCache::Entry *Lookup (Ipv4Address destination);
  /**
   * \brief Add an Ipv4Address to this ARP cache
   */
  ArpCache::Entry *Add (Ipv4Address to);
  /**
   * \brief Clear the ArpCache of all entries
   */
  void Flush (void);

  /**
   * \brief A record that that holds information about an ArpCache entry
   */
  class Entry {
  public:
    /**
     * \brief Constructor
     * \param arp The ArpCache this entry belongs to
     */
    Entry (ArpCache *arp);
    
    /**
     * \brief Changes the state of this entry to dead
     */
    void MarkDead (void);
    /**
     * \param macAddress
     * \return 
     */
    Ptr<Packet> MarkAlive (Address macAddress);
    /**
     * \param waiting
     */
    void MarkWaitReply (Ptr<Packet> waiting);
    /**
     * \param waiting
     * \return 
     */
    Ptr<Packet> UpdateWaitReply (Ptr<Packet> waiting);
    /**
     * \return True if the state of this entry is dead; false otherwise.
     */
    bool IsDead (void);
    /**
     * \return True if the state of this entry is alive; false otherwise.
     */
    bool IsAlive (void);
    /**
     * \return True if the state of this entry is wait_reply; false otherwise.
     */
    bool IsWaitReply (void);
    
    /**
     * \return The MacAddress of this entry
     */
    Address GetMacAddress (void);
    /**
     * \return True if this entry has timedout; false otherwise.
     */
    bool IsExpired (void);
  private:
    enum ArpCacheEntryState_e {
      ALIVE,
      WAIT_REPLY,
      DEAD
    };

    void UpdateSeen (void);
    ArpCache *m_arp;
    ArpCacheEntryState_e m_state;
    Time m_lastSeen;
    Address m_macAddress;
    Ptr<Packet> m_waiting;
  };

private:
  typedef sgi::hash_map<Ipv4Address, ArpCache::Entry *, Ipv4AddressHash> Cache;
  typedef sgi::hash_map<Ipv4Address, ArpCache::Entry *, Ipv4AddressHash>::iterator CacheI;

  Ptr<NetDevice> m_device;
  Ptr<Ipv4Interface> m_interface;
  Time m_aliveTimeout;
  Time m_deadTimeout;
  Time m_waitReplyTimeout;
  Cache m_arpCache;
};


}; // namespace ns3

#endif /* ARP_CACHE_H */
