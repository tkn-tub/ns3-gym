/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef ARP_CACHE_H
#define ARP_CACHE_H

#include <stdint.h>
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/net-device.h"
#include "ns3/ipv4-address.h"
#include "ns3/mac-address.h"
#include "sgi-hashmap.h"
#include "ns3/ptr.h"
#include "ipv4-interface.h"

namespace ns3 {

class NetDevice;

class ArpCache {
public:
  class Entry;

  ArpCache (NetDevice *device, Ptr<Ipv4Interface> interface);
  ~ArpCache ();

  NetDevice *PeekDevice (void) const;
  Ptr<Ipv4Interface> GetInterface (void) const;

  void SetAliveTimeout (Time aliveTimeout);
  void SetDeadTimeout (Time deadTimeout);
  void SetWaitReplyTimeout (Time waitReplyTimeout);
  Time GetAliveTimeout (void) const;
  Time GetDeadTimeout (void) const;
  Time GetWaitReplyTimeout (void) const;

  
  ArpCache::Entry *Lookup (Ipv4Address destination);
  ArpCache::Entry *Add (Ipv4Address to);
  void Flush (void);


  class Entry {
  public:
    Entry (ArpCache *arp);

    void MarkDead (void);
    Packet MarkAlive (MacAddress macAddress);
    void MarkWaitReply (Packet waiting);
    Packet UpdateWaitReply (Packet waiting);
    
    bool IsDead (void);
    bool IsAlive (void);
    bool IsWaitReply (void);
    
    MacAddress GetMacAddress (void);
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
    MacAddress m_macAddress;
    Packet m_waiting;
  };

private:
  typedef sgi::hash_map<Ipv4Address, ArpCache::Entry *, Ipv4AddressHash> Cache;
  typedef sgi::hash_map<Ipv4Address, ArpCache::Entry *, Ipv4AddressHash>::iterator CacheI;

  NetDevice *m_device;
  Ptr<Ipv4Interface> m_interface;
  Time m_aliveTimeout;
  Time m_deadTimeout;
  Time m_waitReplyTimeout;
  Cache m_arpCache;
};


}; // namespace ns3

#endif /* ARP_CACHE_H */
