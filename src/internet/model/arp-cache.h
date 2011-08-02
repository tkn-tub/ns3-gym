/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include <list>
#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/net-device.h"
#include "ns3/ipv4-address.h"
#include "ns3/address.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/sgi-hashmap.h"

namespace ns3 {

class NetDevice;
class Ipv4Interface;

/**
 * \ingroup arp
 * \brief An ARP cache
 *
 * A cached lookup table for translating layer 3 addresses to layer 2.
 * This implementation does lookups from IPv4 to a MAC address
 */
class ArpCache : public Object
{
private:
  ArpCache (ArpCache const &);
  ArpCache& operator= (ArpCache const &);

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
   * This callback is set when the ArpCache is set up and allows
   * the cache to generate an Arp request when the WaitReply
   * time expires and a retransmission must be sent
   *
   * \param arpRequestCallback Callback for transmitting an Arp request.
   */
  void SetArpRequestCallback (Callback<void, Ptr<const ArpCache>, 
                                       Ipv4Address> arpRequestCallback);
  /**
   * This method will schedule a timeout at WaitReplyTimeout interval
   * in the future, unless a timer is already running for the cache,
   * in which case this method does nothing.
   */
  void StartWaitReplyTimer (void);
  /**
   * \brief Do lookup in the ARP cache against an IP address
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
     */
    void MarkAlive (Address macAddress);
    /**
     * \param waiting
     */
    void MarkWaitReply (Ptr<Packet> waiting);
    /**
     * \param waiting
     * \return 
     */
    bool UpdateWaitReply (Ptr<Packet> waiting);
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
    Address GetMacAddress (void) const;
    /**
     * \return The Ipv4Address for this entry
     */
    Ipv4Address GetIpv4Address (void) const;
    /**
     * \param destination The Ipv4Address for this entry
     */
    void SetIpv4Address (Ipv4Address destination);
    /**
     * \return True if this entry has timed out; false otherwise.
     *
     * This function returns true if the time elapsed strictly exceeds
     * the timeout value (i.e., is not less than or equal to the timeout).
     */
    bool IsExpired (void) const;
    /**
     * \returns 0 is no packet is pending, the next packet to send if 
     *            packets are pending.
     */
    Ptr<Packet> DequeuePending (void);
    /**
     * \returns number of retries that have been sent for an ArpRequest
     *  in WaitReply state.
     */
    uint32_t GetRetries (void) const;
    /**
     * \brief Increment the counter of number of retries for an entry
     */
    void IncrementRetries (void);
    /**
     * \brief Zero the counter of number of retries for an entry
     */
    void ClearRetries (void);

private:
    enum ArpCacheEntryState_e {
      ALIVE,
      WAIT_REPLY,
      DEAD
    };

    void UpdateSeen (void);
    Time GetTimeout (void) const;
    ArpCache *m_arp;
    ArpCacheEntryState_e m_state;
    Time m_lastSeen;
    Address m_macAddress;
    Ipv4Address m_ipv4Address;
    std::list<Ptr<Packet> > m_pending;
    uint32_t m_retries;
  };

private:
  typedef sgi::hash_map<Ipv4Address, ArpCache::Entry *, Ipv4AddressHash> Cache;
  typedef sgi::hash_map<Ipv4Address, ArpCache::Entry *, Ipv4AddressHash>::iterator CacheI;

  virtual void DoDispose (void);

  Ptr<NetDevice> m_device;
  Ptr<Ipv4Interface> m_interface;
  Time m_aliveTimeout;
  Time m_deadTimeout;
  Time m_waitReplyTimeout;
  EventId m_waitReplyTimer;
  Callback<void, Ptr<const ArpCache>, Ipv4Address> m_arpRequestCallback;
  uint32_t m_maxRetries;
  /**
   * This function is an event handler for the event that the
   * ArpCache wants to check whether it must retry any Arp requests.
   * If there are no Arp requests pending, this event is not scheduled.
   */
  void HandleWaitReplyTimeout (void);
  uint32_t m_pendingQueueSize;
  Cache m_arpCache;
  TracedCallback<Ptr<const Packet> > m_dropTrace;
};


} // namespace ns3

#endif /* ARP_CACHE_H */
