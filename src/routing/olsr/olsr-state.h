/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004 Francisco J. Ros 
 * Copyright (c) 2007 INESC Porto
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
 * Authors: Francisco J. Ros  <fjrm@dif.um.es>
 *          Gustavo J. A. M. Carneiro <gjc@inescporto.pt>
 */

/// \brief	This header file declares and defines internal state of an OLSR node.

#ifndef __OLSR_STATE_H__
#define __OLSR_STATE_H__

#include "repositories.h"

namespace ns3 {

using namespace olsr;

/// This class encapsulates all data structures needed for maintaining internal state of an OLSR node.
class OlsrState
{
  //  friend class Olsr;
  
protected:
  LinkSet m_linkSet;	///< Link Set (RFC 3626, section 4.2.1).
  NeighborSet m_neighborSet;		///< Neighbor Set (RFC 3626, section 4.3.1).
  TwoHopNeighborSet m_twoHopNeighborSet;	///< 2-hop Neighbor Set (RFC 3626, section 4.3.2).
  TopologySet m_topologySet;	///< Topology Set (RFC 3626, section 4.4).
  MprSet m_mprSet;	///< MPR Set (RFC 3626, section 4.3.3).
  MprSelectorSet m_mprSelectorSet;	///< MPR Selector Set (RFC 3626, section 4.3.4).
  DuplicateSet m_duplicateSet;	///< Duplicate Set (RFC 3626, section 3.4).
  IfaceAssocSet m_ifaceAssocSet;	///< Interface Association Set (RFC 3626, section 4.1).

// m_modified is set to true when any of the following databases is modified:
//      -    the link set,
//      -    the neighbor set,
//      -    the 2-hop neighbor set,
//      -    the topology set,
//      -    the Multiple Interface Association Information Base,
  bool m_modified;

public:

  OlsrState ()
    : m_modified (false)
  {}
  
  bool GetModified () const
  {
    return m_modified;
  }
  void SetModified (bool modified)
  {
    m_modified = modified;
  }

  // MPR selector
  const MprSelectorSet & GetMprSelectors () const
  {
    return m_mprSelectorSet;
  }
  MprSelectorTuple* FindMprSelectorTuple (const Ipv4Address &mainAddr);
  void EraseMprSelectorTuple (const MprSelectorTuple &tuple);
  void EraseMprSelectorTuples (const Ipv4Address &mainAddr);
  void InsertMprSelectorTuple (const MprSelectorTuple &tuple);

  // Neighbor
  const NeighborSet & GetNeighbors () const
  {
    return m_neighborSet;
  }
  NeighborSet & GetNeighbors ()
  {
    return m_neighborSet;
  }
  NeighborTuple* FindNeighborTuple (const Ipv4Address &mainAddr);
  const NeighborTuple* FindSymNeighborTuple (const Ipv4Address &mainAddr) const;
  NeighborTuple* FindNeighborTuple (const Ipv4Address &mainAddr,
                                    uint8_t willingness);
  void EraseNeighborTuple (const NeighborTuple &neighborTuple);
  void EraseNeighborTuple (const Ipv4Address &mainAddr);
  void InsertNeighborTuple (const NeighborTuple &tuple);

  // Two-hop neighbor
  const TwoHopNeighborSet & GetTwoHopNeighbors () const
  {
    return m_twoHopNeighborSet;
  }
  TwoHopNeighborSet & GetTwoHopNeighbors ()
  {
    return m_twoHopNeighborSet;
  }
  TwoHopNeighborTuple* FindTwoHopNeighborTuple (const Ipv4Address &neighbor,
                                                const Ipv4Address &twoHopNeighbor);
  void EraseTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
  void EraseTwoHopNeighborTuples (const Ipv4Address &neighbor);
  void EraseTwoHopNeighborTuples (const Ipv4Address &neighbor,
                                  const Ipv4Address &twoHopNeighbor);
  void InsertTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);

  // MPR
  bool FindMprAddress (const Ipv4Address &address);
  void InsertMprAddress (const Ipv4Address &address);
  void ClearMprSet ();

  // Duplicate
  DuplicateTuple* FindDuplicateTuple (const Ipv4Address &address,
                                      uint16_t sequenceNumber);
  void EraseDuplicateTuple (const DuplicateTuple &tuple);
  void InsertDuplicateTuple (const DuplicateTuple &tuple);

  // Link
  const LinkSet & GetLinks () const
  {
    return m_linkSet;
  }
  LinkTuple* FindLinkTuple (const Ipv4Address &ifaceAddr);
  LinkTuple* FindSymLinkTuple (const Ipv4Address &ifaceAddr, Time time);
  void EraseLinkTuple (const LinkTuple &tuple);
  LinkTuple& InsertLinkTuple (const LinkTuple &tuple);

  // Topology
  const TopologySet & GetTopologySet () const
  {
    return m_topologySet;
  }
  TopologyTuple* FindTopologyTuple (const Ipv4Address &destAddr,
                                    const Ipv4Address &lastAddr);
  TopologyTuple* FindNewerTopologyTuple (const Ipv4Address &lastAddr,
                                         uint16_t ansn);
  void EraseTopologyTuple (const TopologyTuple &tuple);
  void EraseOlderTopologyTuples (const Ipv4Address &lastAddr,
                                 uint16_t ansn);
  void InsertTopologyTuple (const TopologyTuple &tuple);

  // Interface association
  const IfaceAssocSet & GetIfaceAssocSet () const
  {
    return m_ifaceAssocSet;
  }
  IfaceAssocSet & GetIfaceAssocSetMutable ()
  {
    return m_ifaceAssocSet;
  }
  IfaceAssocTuple* FindIfaceAssocTuple (const Ipv4Address &ifaceAddr);
  const IfaceAssocTuple* FindIfaceAssocTuple (const Ipv4Address &ifaceAddr) const;
  void EraseIfaceAssocTuple (const IfaceAssocTuple &tuple);
  void InsertIfaceAssocTuple (const IfaceAssocTuple &tuple);

  // Returns a vector of all interfaces of a given neighbor, with the
  // exception of the "main" one.
  std::vector<Ipv4Address>
  FindNeighborInterfaces (const Ipv4Address &neighborMainAddr) const;

};

} // namespace ns3

#endif
