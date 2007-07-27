/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/***************************************************************************
 *   Copyright (C) 2004 by Francisco J. Ros                                *
 *   fjrm@dif.um.es                                                        *
 *                                                                         *
 *   Modified on 2007 for NS-3 by Gustavo J. A. M. Carneiro, INESC Porto   *
 *                                <gjc@inescporto.pt>                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

///
/// \file	OLSR.cc
/// \brief	Implementation of OLSR agent and related classes.
///
/// This is the main file of this software because %OLSR's behaviour is
/// implemented here.
///

#include <math.h>
// #include <limits.h>
// #include <address.h>
// #include <ip.h>
// #include <cmu-trace.h>
#include <map>
#include "olsr-private.h"
#include "ns3/socket-factory.h"
#include "ns3/udp.h"
#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/debug.h"
#include "ns3/random-variable.h"

#include <iomanip>


/********** Useful macros **********/

///
/// \brief Gets the delay between a given time and the current time.
///
/// If given time is previous to the current one, then this macro returns
/// a number close to 0. This is used for scheduling events at a certain moment.
///
#define DELAY(time) (((time) < (Simulator::Now ())) ? Seconds (0.000001) : \
                     (time - Simulator::Now () + Seconds (0.000001)))



/********** Intervals **********/

/// HELLO messages emission interval.
#define OLSR_HELLO_INTERVAL	Seconds (2)

/// TC messages emission interval.
#define OLSR_TC_INTERVAL	Seconds (5)

/// MID messages emission interval.
#define OLSR_MID_INTERVAL	OLSR_TC_INTERVAL

///
/// \brief Period at which a node must cite every link and every neighbor.
///
/// We only use this value in order to define OLSR_NEIGHB_HOLD_TIME.
///
#define OLSR_REFRESH_INTERVAL	Seconds (2)


/********** Holding times **********/

/// Neighbor holding time.
#define OLSR_NEIGHB_HOLD_TIME	(Scalar (3) * OLSR_REFRESH_INTERVAL)
/// Top holding time.
#define OLSR_TOP_HOLD_TIME	(Scalar (3) * OLSR_TC_INTERVAL)
/// Dup holding time.
#define OLSR_DUP_HOLD_TIME	Seconds (30)
/// MID holding time.
#define OLSR_MID_HOLD_TIME	(Scalar (3) * OLSR_MID_INTERVAL)


/********** Link types **********/

/// Unspecified link type.
#define OLSR_UNSPEC_LINK	0
/// Asymmetric link type.
#define OLSR_ASYM_LINK		1
/// Symmetric link type.
#define OLSR_SYM_LINK		2
/// Lost link type.
#define OLSR_LOST_LINK		3

/********** Neighbor types **********/

/// Not neighbor type.
#define OLSR_NOT_NEIGH		0
/// Symmetric neighbor type.
#define OLSR_SYM_NEIGH		1
/// Asymmetric neighbor type.
#define OLSR_MPR_NEIGH		2


/********** Willingness **********/

/// Willingness for forwarding packets from other nodes: never.
#define OLSR_WILL_NEVER		0
/// Willingness for forwarding packets from other nodes: low.
#define OLSR_WILL_LOW		1
/// Willingness for forwarding packets from other nodes: medium.
#define OLSR_WILL_DEFAULT	3
/// Willingness for forwarding packets from other nodes: high.
#define OLSR_WILL_HIGH		6
/// Willingness for forwarding packets from other nodes: always.
#define OLSR_WILL_ALWAYS	7


/********** Miscellaneous constants **********/

/// Maximum allowed jitter.
#define OLSR_MAXJITTER		(OLSR_HELLO_INTERVAL.GetSeconds () / 4)
/// Maximum allowed sequence number.
#define OLSR_MAX_SEQ_NUM	65535
/// Random number between [0-OLSR_MAXJITTER] used to jitter OLSR packet transmission.
#define JITTER (Seconds (UniformVariable::GetSingleValue (0, OLSR_MAXJITTER)))


#define OLSR_PORT_NUMBER 698
/// Maximum number of messages per packet.
#define OLSR_MAX_MSGS		64

/// Maximum number of hellos per message (4 possible link types * 3 possible nb types).
#define OLSR_MAX_HELLOS		12

/// Maximum number of addresses advertised on a message.
#define OLSR_MAX_ADDRS		64


namespace ns3 {

const InterfaceId Olsr::iid = MakeInterfaceId ("Olsr", Object::iid);
const ClassId Olsr::cid = MakeClassId< olsr::Olsr, Ptr<Node> > ("Olsr", Olsr::iid);

namespace olsr {

NS_DEBUG_COMPONENT_DEFINE ("Olsr");


/********** OLSR class **********/


Olsr::Olsr (Ptr<Node> node)
  :
  m_useL2Notifications (false)
{
  SetInterfaceId (Olsr::iid);

  // Aggregate with the Node, so that OLSR dies when the node is destroyed.
  node->AddInterface (this);

  m_packetSequenceNumber = OLSR_MAX_SEQ_NUM;
  m_messageSequenceNumber = OLSR_MAX_SEQ_NUM;
  m_ansn = OLSR_MAX_SEQ_NUM;

  m_helloInterval = OLSR_HELLO_INTERVAL;
  m_tcInterval = OLSR_TC_INTERVAL;
  m_midInterval = OLSR_MID_INTERVAL;
  m_willingness = OLSR_WILL_ALWAYS;

  m_linkTupleTimerFirstTime = true;

  m_ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
  NS_ASSERT (m_ipv4);

  Ptr<SocketFactory> socketFactory = node->QueryInterface<SocketFactory> (Udp::iid);

  m_receiveSocket = socketFactory->CreateSocket ();
  if (m_receiveSocket->Bind (OLSR_PORT_NUMBER))
    NS_ASSERT_MSG (false, "Failed to bind() OLSR receive socket");

  m_sendSocket = socketFactory->CreateSocket ();
  m_sendSocket->Connect (Ipv4Address (0xffffffff), OLSR_PORT_NUMBER);

}

void Olsr::DoDispose ()
{
  m_ipv4 = 0;
  m_receiveSocket->Dispose ();
  m_receiveSocket = 0;
  m_sendSocket->Dispose ();
  m_sendSocket = 0;  
  m_routingTable->Dispose ();
  m_routingTable = 0;

  Object::DoDispose ();
}

void Olsr::Start ()
{
  if (m_mainAddress == Ipv4Address ())
    {
      Ipv4Address loopback ("127.0.0.1");
      for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
        {
          Ipv4Address addr = m_ipv4->GetAddress (i);
          if (addr != loopback)
            {
              m_mainAddress = addr;
              break;
            }
        }

      NS_ASSERT (m_mainAddress != Ipv4Address ());
    }

  NS_DEBUG ("Starting OLSR on node " << m_mainAddress);

  m_routingTable = Create<RoutingTable> (m_ipv4, m_mainAddress);
  // Add OLSR as routing protocol, with slightly lower priority than
  // static routing.
  m_ipv4->AddRoutingProtocol (m_routingTable, -10);

  if (m_sendSocket->Bind (m_mainAddress, OLSR_PORT_NUMBER))
    NS_ASSERT_MSG (false, "Failed to bind() OLSR send socket");
  m_receiveSocket->Recv (MakeCallback (&Olsr::RecvOlsr,  this));

  HelloTimerExpire ();
  TcTimerExpire ();
  MidTimerExpire ();

  NS_DEBUG ("OLSR on node " << m_mainAddress << " started");
}

void Olsr::SetMainInterface (uint32_t interface)
{
  m_mainAddress = m_ipv4->GetAddress (interface);
}


//
// \brief Processes an incoming %OLSR packet following RFC 3626 specification.
void
Olsr::RecvOlsr (Ptr<Socket> socket,
                const uint8_t *dataPtr, uint32_t dataSize,
                const Ipv4Address &sourceAddress,
                uint16_t sourcePort)
{
  NS_DEBUG ("OLSR node " << m_mainAddress << " received a OLSR packet");

  // All routing messages are sent from and to port RT_PORT,
  // so we check it.
  NS_ASSERT (sourcePort == OLSR_PORT_NUMBER);
  
  Packet packet (dataPtr, dataSize);

  OlsrPacketHeader olsrPacketHeader;
  packet.RemoveHeader (olsrPacketHeader);
  NS_ASSERT (olsrPacketHeader.GetPacketLength () >= olsrPacketHeader.GetSize ());
  uint32_t sizeLeft = olsrPacketHeader.GetPacketLength () - olsrPacketHeader.GetSize ();
  
  while (sizeLeft)
    {
      OlsrMessageHeader messageHeader;
      NS_ASSERT (sizeLeft >= messageHeader.GetSize ());
      if (packet.RemoveHeader (messageHeader) == 0)
        NS_ASSERT (false);
      
      sizeLeft -= messageHeader.GetMessageSize ();

      NS_DEBUG ("Olsr Msg received with type "
                << std::dec << int (messageHeader.GetMessageType ())
                << " TTL=" << int (messageHeader.GetTimeToLive ())
                << " origAddr=" << messageHeader.GetOriginatorAddress ());

      // If ttl is less than or equal to zero, or
      // the receiver is the same as the originator,
      // the message must be silently dropped
      if (messageHeader.GetTimeToLive () == 0
          || messageHeader.GetOriginatorAddress () == m_mainAddress)
        {
          packet.RemoveAtStart (messageHeader.GetMessageSize ()
                                - messageHeader.GetSize ());
          continue;
        }

      // Save the original message payload for forwarding
      Packet messagePayload = packet.CreateFragment
        (0, messageHeader.GetMessageSize () - messageHeader.GetSize ());
      
      // If the message has been processed it must not be processed again
      bool do_forwarding = true;
      DuplicateTuple *duplicated = m_state.FindDuplicateTuple
        (messageHeader.GetOriginatorAddress (),
         messageHeader.GetMessageSequenceNumber ());
      
      if (duplicated == NULL)
        {
          OlsrHelloMessageHeader helloMsg;
          OlsrTcMessageHeader tcMsg;
          OlsrMidMessageHeader midMsg;

          switch (messageHeader.GetMessageType ())
            {
            case OlsrMessageHeader::HELLO_MESSAGE:
              helloMsg.SetMessageSize (messageHeader.GetMessageSize () - messageHeader.GetSize ());
              packet.RemoveHeader (helloMsg);
              NS_DEBUG ("OLSR node received HELLO message of size " << messageHeader.GetMessageSize ());
              ProcessHello(messageHeader, helloMsg, m_mainAddress, sourceAddress);
              break;

            case OlsrMessageHeader::TC_MESSAGE:
              tcMsg.SetMessageSize (messageHeader.GetMessageSize () - messageHeader.GetSize ());
              packet.RemoveHeader (tcMsg);
              NS_DEBUG ("OLSR node received TC message of size " << messageHeader.GetMessageSize ());
              ProcessTc(messageHeader, tcMsg, sourceAddress);
              break;

            case OlsrMessageHeader::MID_MESSAGE:
              midMsg.SetMessageSize (messageHeader.GetMessageSize () - messageHeader.GetSize ());
              packet.RemoveHeader (midMsg);
              NS_DEBUG ("OLSR node received MID message of size " << messageHeader.GetMessageSize ());
              ProcessMid(messageHeader, midMsg, sourceAddress);
              break;

            default:
              packet.RemoveAtStart (messageHeader.GetMessageSize () - messageHeader.GetSize ());
              NS_DEBUG ("OLSR message type " <<
                        int (messageHeader.GetMessageType ()) <<
                        " not implemented");
            }
        }
      else
        {
          NS_DEBUG ("OLSR message is duplicated, not reading it.");
          packet.RemoveAtStart (messageHeader.GetMessageSize () - messageHeader.GetSize ());
      
          // If the message has been considered for forwarding, it should
          // not be retransmitted again
          for (std::vector<Ipv4Address>::const_iterator it = duplicated->ifaceList.begin ();
               it != duplicated->ifaceList.end(); it++)
            {
              if (*it == m_mainAddress)
                {
                  do_forwarding = false;
                  break;
                }
            }
        }
      
      if (do_forwarding)
        {
          // HELLO messages are never forwarded.
          // TC and MID messages are forwarded using the default algorithm.
          // Remaining messages are also forwarded using the default algorithm.
          if (messageHeader.GetMessageType ()  != OlsrMessageHeader::HELLO_MESSAGE)
            ForwardDefault (messageHeader, messagePayload, duplicated,
                            m_mainAddress, sourceAddress);
        }
	
    }

  // After processing all OLSR messages, we must recompute the routing table
  RoutingTableComputation ();
}

///
/// \brief This auxiliary function (defined in RFC 3626) is used for calculating the MPR Set.
///
/// \param tuple the neighbor tuple which has the main address of the node we are going to calculate its degree to.
/// \return the degree of the node.
///
int
Olsr::Degree (NeighborTuple const &tuple)
{
  int degree = 0;
  for (TwoHopNeighborSet::const_iterator it = m_state.GetTwoHopNeighbors ().begin ();
       it != m_state.GetTwoHopNeighbors ().end (); it++)
    {
      TwoHopNeighborTuple const &nb2hop_tuple = *it;
      if (nb2hop_tuple.neighborMainAddr == tuple.neighborMainAddr)
        {
          NeighborTuple *nb_tuple =
            m_state.FindNeighborTuple (nb2hop_tuple.neighborMainAddr);
          if (nb_tuple == NULL)
            degree++;
        }
    }
  return degree;
}

///
/// \brief Computates MPR set of a node following RFC 3626 hints.
///
void
Olsr::MprComputation()
{
  // MPR computation should be done for each interface. See section 8.3.1
  // (RFC 3626) for details.
	
  m_state.ClearMprSet ();
  
  // N is the subset of neighbors of the node, which are
  // neighbor "of the interface I"
  NeighborSet N;
  for (NeighborSet::const_iterator it = m_state.GetNeighbors ().begin();
       it != m_state.GetNeighbors ().end (); it++)
    {
      if ((*it).status == NeighborTuple::STATUS_SYM) // I think that we need this check
        N.push_back (*it);
    }
	
  // N2 is the set of 2-hop neighbors reachable from "the interface
  // I", excluding:
  // (i)   the nodes only reachable by members of N with willingness WILL_NEVER
  // (ii)  the node performing the computation
  // (iii) all the symmetric neighbors: the nodes for which there exists a symmetric
  //       link to this node on some interface.
  TwoHopNeighborSet N2;
  for (TwoHopNeighborSet::const_iterator it = m_state.GetTwoHopNeighbors ().begin ();
       it != m_state.GetTwoHopNeighbors ().end (); it++)
    {
      TwoHopNeighborTuple const &twoHopNeigh = *it;
      bool ok = true;
      NeighborTuple *nb_tuple = m_state.FindSymNeighborTuple (twoHopNeigh.neighborMainAddr);
      if (nb_tuple == NULL)
        {
          ok = false;
        }
      else
        {
          nb_tuple = m_state.FindNeighborTuple (twoHopNeigh.neighborMainAddr, OLSR_WILL_NEVER);
          if (nb_tuple != NULL)
            {
              ok = false;
            }
          else
            {
              nb_tuple = m_state.FindSymNeighborTuple (twoHopNeigh.neighborMainAddr);
              if (nb_tuple != NULL)
                ok = false;
            }
        }

      if (ok)
        N2.push_back (twoHopNeigh);
    }
	
  // 1. Start with an MPR set made of all members of N with
  // N_willingness equal to WILL_ALWAYS
  for (NeighborSet::const_iterator it = N.begin (); it != N.end (); it++)
    {
      NeighborTuple const &nb_tuple = *it;
      if (nb_tuple.willingness == OLSR_WILL_ALWAYS)
        m_state.InsertMprAddress (nb_tuple.neighborMainAddr);
    }
  
  // 2. Calculate D(y), where y is a member of N, for all nodes in N.
  // We will do this later.
  // FIXME
	
  // 3. Add to the MPR set those nodes in N, which are the *only*
  // nodes to provide reachability to a node in N2. Remove the
  // nodes from N2 which are now covered by a node in the MPR set.
  MprSet foundset;
  std::set<Ipv4Address> deleted_addrs;
  for (TwoHopNeighborSet::iterator it = N2.begin (); it != N2.end (); it++)
    {
      TwoHopNeighborTuple const &nb2hop_tuple1 = *it;
		
      MprSet::const_iterator pos = foundset.find (nb2hop_tuple1.twoHopNeighborAddr);
      if (pos != foundset.end ())
        continue;
		
      bool found = false;
      for (NeighborSet::const_iterator it2 = N.begin ();
           it2 != N.end (); it2++)
        {
          if ((*it2).neighborMainAddr == nb2hop_tuple1.neighborMainAddr) {
            found = true;
            break;
          }
        }
      if (!found)
        continue;
		
      found = false;
      for (TwoHopNeighborSet::const_iterator it2 = it + 1;
           it2 != N2.end (); it2++)
        {
          TwoHopNeighborTuple const &nb2hop_tuple2 = *it2;
          if (nb2hop_tuple1.twoHopNeighborAddr == nb2hop_tuple2.twoHopNeighborAddr)
            {
              foundset.insert (nb2hop_tuple1.twoHopNeighborAddr);
              found = true;
              break;
            }
        }
      if (!found)
        {
          m_state.InsertMprAddress (nb2hop_tuple1.neighborMainAddr);
          
          for (TwoHopNeighborSet::iterator it2 = it + 1; it2 != N2.end (); it2++)
            {
              TwoHopNeighborTuple const &nb2hop_tuple2 = *it2;
              if (nb2hop_tuple1.neighborMainAddr == nb2hop_tuple2.neighborMainAddr)
                {
                  deleted_addrs.insert (nb2hop_tuple2.twoHopNeighborAddr);
                  it2 = N2.erase (it2);
                  it2--;
                }
            }
          it = N2.erase (it);
          it--;
        }
		
      for (std::set<Ipv4Address>::iterator it2 = deleted_addrs.begin ();
           it2 != deleted_addrs.end ();
           it2++)
        {
          for (TwoHopNeighborSet::iterator it3 = N2.begin ();
               it3 != N2.end ();
               it3++)
            {
              if ((*it3).twoHopNeighborAddr == *it2)
                {
                  it3 = N2.erase (it3);
                  it3--;
                  // I have to reset the external iterator because it
                  // may have been invalidated by the latter deletion
                  it = N2.begin ();
                  it--;
                }
            }
        }
      deleted_addrs.clear ();
    }
	
  // 4. While there exist nodes in N2 which are not covered by at
  // least one node in the MPR set:
  while (N2.begin () != N2.end ())
    {
      // 4.1. For each node in N, calculate the reachability, i.e., the
      // number of nodes in N2 which are not yet covered by at
      // least one node in the MPR set, and which are reachable
      // through this 1-hop neighbor
      std::map<int, std::vector<const NeighborTuple *> > reachability;
      std::set<int> rs;
      for (NeighborSet::iterator it = N.begin(); it != N.end(); it++)
        {
          NeighborTuple const &nb_tuple = *it;
          int r = 0;
          for (TwoHopNeighborSet::iterator it2 = N2.begin (); it2 != N2.end (); it2++)
            {
              TwoHopNeighborTuple const &nb2hop_tuple = *it2;
              if (nb_tuple.neighborMainAddr == nb2hop_tuple.neighborMainAddr)
                r++;
            }
          rs.insert (r);
          reachability[r].push_back (&nb_tuple);
        }
      
      // 4.2. Select as a MPR the node with highest N_willingness among
      // the nodes in N with non-zero reachability. In case of
      // multiple choice select the node which provides
      // reachability to the maximum number of nodes in N2. In
      // case of multiple nodes providing the same amount of
      // reachability, select the node as MPR whose D(y) is
      // greater. Remove the nodes from N2 which are now covered
      // by a node in the MPR set.
      NeighborTuple const *max = NULL;
      int max_r = 0;
      for (std::set<int>::iterator it = rs.begin (); it != rs.end (); it++)
        {
          int r = *it;
          if (r > 0)
            {
              for (std::vector<const NeighborTuple *>::iterator it2 = reachability[r].begin ();
                   it2 != reachability[r].end ();
                   it2++)
                {
                  const NeighborTuple *nb_tuple = *it2;
                  if (max == NULL || nb_tuple->willingness > max->willingness)
                    {
                      max = nb_tuple;
                      max_r = r;
                    }
                  else if (nb_tuple->willingness == max->willingness)
                    {
                      if (r > max_r)
                        {
                          max = nb_tuple;
                          max_r = r;
                        }
                      else if (r == max_r)
                        {
                          if (Degree (*nb_tuple) > Degree (*max))
                            {
                              max = nb_tuple;
                              max_r = r;
                            }
                        }
                    }
                }
            }
        }

      if (max != NULL)
        {
          m_state.InsertMprAddress (max->neighborMainAddr);
          std::set<Ipv4Address> nb2hop_addrs;
          for (TwoHopNeighborSet::iterator it = N2.begin ();
               it != N2.end (); it++)
            {
              TwoHopNeighborTuple const &nb2hop_tuple = *it;
              if (nb2hop_tuple.neighborMainAddr == max->neighborMainAddr)
                {
                  nb2hop_addrs.insert (nb2hop_tuple.twoHopNeighborAddr);
                  it = N2.erase (it);
                  it--;
                }
            }
          for (TwoHopNeighborSet::iterator it = N2.begin ();
               it != N2.end (); it++)
            {
              TwoHopNeighborTuple const &nb2hop_tuple = *it;
              std::set<Ipv4Address>::iterator it2 =
                nb2hop_addrs.find (nb2hop_tuple.twoHopNeighborAddr);
              if (it2 != nb2hop_addrs.end ())
                {
                  it = N2.erase (it);
                  it--;
                }
            }
        }
    }
}

///
/// \brief Gets the main address associated with a given interface address.
///
/// \param iface_addr the interface address.
/// \return the corresponding main address.
///
Ipv4Address
Olsr::GetMainAddress (Ipv4Address iface_addr)
{
  IfaceAssocTuple *tuple =
    m_state.FindIfaceAssocTuple (iface_addr);
  
  if (tuple != NULL)
    return tuple->mainAddr;
  else
    return iface_addr;
}

///
/// \brief Creates the routing table of the node following RFC 3626 hints.
///
void
Olsr::RoutingTableComputation ()
{
  // 1. All the entries from the routing table are removed.
  m_routingTable->Clear ();
	
  // 2. The new routing entries are added starting with the
  // symmetric neighbors (h=1) as the destination nodes.
  for (NeighborSet::const_iterator it = m_state.GetNeighbors ().begin ();
       it != m_state.GetNeighbors ().end(); it++)
    {
      NeighborTuple const &nb_tuple = *it;
      if (nb_tuple.status == NeighborTuple::STATUS_SYM)
        {
          bool nb_main_addr = false;
          const LinkTuple *lt = NULL;
          for (LinkSet::const_iterator it2 = m_state.GetLinks ().begin();
               it2 != m_state.GetLinks ().end(); it2++)
            {
              LinkTuple const &link_tuple = *it2;
              if ((GetMainAddress (link_tuple.neighborIfaceAddr)
                   == nb_tuple.neighborMainAddr)
                  && link_tuple.time >= Simulator::Now ())
                {
                  lt = &link_tuple;
                  m_routingTable->AddEntry (link_tuple.neighborIfaceAddr,
                                            link_tuple.neighborIfaceAddr,
                                            link_tuple.localIfaceAddr,
                                            1);
                  if (link_tuple.neighborIfaceAddr
                      == nb_tuple.neighborMainAddr)
                    nb_main_addr = true;
                }
            }
          if (!nb_main_addr && lt != NULL)
            {
              m_routingTable->AddEntry(nb_tuple.neighborMainAddr,
                                       lt->neighborIfaceAddr,
                                       lt->localIfaceAddr,
                                       1);
            }
        }
    }
  
  // N2 is the set of 2-hop neighbors reachable from this node, excluding:
  // (i)   the nodes only reachable by members of N with willingness WILL_NEVER
  // (ii)  the node performing the computation
  // (iii) all the symmetric neighbors: the nodes for which there exists a symmetric
  //       link to this node on some interface.
  for (TwoHopNeighborSet::const_iterator it = m_state.GetTwoHopNeighbors ().begin ();
       it != m_state.GetTwoHopNeighbors ().end (); it++)
    {
      TwoHopNeighborTuple const &nb2hop_tuple = *it;
      bool ok = true;
      NeighborTuple *nb_tuple = m_state.FindSymNeighborTuple
        (nb2hop_tuple.neighborMainAddr);
      if (nb_tuple == NULL)
        ok = false;
      else
        {
          nb_tuple = m_state.FindNeighborTuple (nb2hop_tuple.neighborMainAddr,
                                                OLSR_WILL_NEVER);
          if (nb_tuple != NULL)
            ok = false;
          else
            {
              nb_tuple = m_state.FindSymNeighborTuple (nb2hop_tuple.twoHopNeighborAddr);

              if (nb_tuple != NULL)
                ok = false;
            }
        }

      // 3. For each node in N2 create a new entry in the routing table
      if (ok)
        {
          RoutingTableEntry entry;
          bool found_entry = m_routingTable->Lookup (nb2hop_tuple.neighborMainAddr, entry);
          NS_ASSERT (found_entry);
          m_routingTable->AddEntry (nb2hop_tuple.twoHopNeighborAddr,
                                    entry.nextAddr,
                                    entry.interface,
                                    2);
        }
    }
  
  for (uint32_t h = 2; ; h++)
    {
      bool added = false;
		
      // 4.1. For each topology entry in the topology table, if its
      // T_dest_addr does not correspond to R_dest_addr of any
      // route entry in the routing table AND its T_last_addr
      // corresponds to R_dest_addr of a route entry whose R_dist
      // is equal to h, then a new route entry MUST be recorded in
      // the routing table (if it does not already exist)
      for (TopologySet::const_iterator it = m_state.GetTopologySet ().begin ();
           it != m_state.GetTopologySet ().end ();
           it++)
        {
          TopologyTuple const &topology_tuple = *it;
          RoutingTableEntry entry1, entry2;
          bool have_entry1 = m_routingTable->Lookup (topology_tuple.destAddr, entry1);
          bool have_entry2 = m_routingTable->Lookup (topology_tuple.lastAddr, entry2);
          if (!have_entry1 && have_entry2 && entry2.distance == h)
            {
              m_routingTable->AddEntry (topology_tuple.destAddr,
                                        entry2.nextAddr,
                                        entry2.interface,
                                        h + 1);
              added = true;
            }
        }
		
      // 5. For each entry in the multiple interface association base
      // where there exists a routing entry such that:
      //	R_dest_addr  == I_main_addr  (of the multiple interface association entry)
      // AND there is no routing entry such that:
      //	R_dest_addr  == I_iface_addr
      // then a route entry is created in the routing table
      for (IfaceAssocSet::const_iterator it = m_state.GetIfaceAssocSet ().begin ();
           it != m_state.GetIfaceAssocSet ().end ();
           it++)
        {
          IfaceAssocTuple const &tuple = *it;
          RoutingTableEntry entry1, entry2;
          bool have_entry1 = m_routingTable->Lookup (tuple.mainAddr, entry1);
          bool have_entry2 = m_routingTable->Lookup (tuple.ifaceAddr, entry2);
          if (have_entry1 && !have_entry2)
            {
              m_routingTable->AddEntry (tuple.ifaceAddr,
                                        entry1.nextAddr,
                                        entry1.interface,
                                        entry1.distance);
              added = true;
            }
        }

      if (!added)
        break;
    }
}


///
/// \brief Processes a HELLO message following RFC 3626 specification.
///
/// Link sensing and population of the Neighbor Set, 2-hop Neighbor Set and MPR
/// Selector Set are performed.
///
/// \param msg the %OLSR message which contains the HELLO message.
/// \param receiver_iface the address of the interface where the message was received from.
/// \param sender_iface the address of the interface where the message was sent from.
///
void
Olsr::ProcessHello (const OlsrMessageHeader &msg,
                    const OlsrHelloMessageHeader &hello,
                    const Ipv4Address &receiverIface,
                    const Ipv4Address &senderIface)
{
  LinkSensing (msg, hello, receiverIface, senderIface);
  PopulateNeighborSet (msg, hello);
  PopulateTwoHopNeighborSet (msg, hello);
  MprComputation ();
  PopulateMprSelectorSet (msg, hello);
}

///
/// \brief Processes a TC message following RFC 3626 specification.
///
/// The Topology Set is updated (if needed) with the information of
/// the received TC message.
///
/// \param msg the %OLSR message which contains the TC message.
/// \param sender_iface the address of the interface where the message was sent from.
///
void
Olsr::ProcessTc (const OlsrMessageHeader &msg,
                 const OlsrTcMessageHeader &tc,
                 const Ipv4Address &senderIface)
{
  Time now = Simulator::Now ();
	
  // 1. If the sender interface of this message is not in the symmetric
  // 1-hop neighborhood of this node, the message MUST be discarded.
  LinkTuple *link_tuple = m_state.FindSymLinkTuple (senderIface, now);
  if (link_tuple == NULL)
    return;
	
  // 2. If there exist some tuple in the topology set where:
  // 	T_last_addr == originator address AND
  // 	T_seq       >  ANSN,
  // then further processing of this TC message MUST NOT be
  // performed.
  TopologyTuple *topologyTuple =
    m_state.FindNewerTopologyTuple (msg.GetOriginatorAddress (), tc.GetAnsn ());
  if (topologyTuple != NULL)
    return;
	
  // 3. All tuples in the topology set where:
  //	T_last_addr == originator address AND
  //	T_seq       <  ANSN
  // MUST be removed from the topology set.
  m_state.EraseOlderTopologyTuples (msg.GetOriginatorAddress (), tc.GetAnsn ());

  // 4. For each of the advertised neighbor main address received in
  // the TC message:
  for (std::vector<Ipv4Address>::const_iterator i = tc.GetNeighborAddresses ().begin ();
       i != tc.GetNeighborAddresses ().end (); i++)
    {
      const Ipv4Address &addr = *i;
      // 4.1. If there exist some tuple in the topology set where:
      // 	T_dest_addr == advertised neighbor main address, AND
      // 	T_last_addr == originator address,
      // then the holding time of that tuple MUST be set to:
      // 	T_time      =  current time + validity time.
      TopologyTuple *topologyTuple =
        m_state.FindTopologyTuple (addr, msg.GetOriginatorAddress ());

      if (topologyTuple != NULL)
        {
          topologyTuple->expirationTime = now + msg.GetVTime ();
        }
      else
        {
          // 4.2. Otherwise, a new tuple MUST be recorded in the topology
          // set where:
          //	T_dest_addr = advertised neighbor main address,
          //	T_last_addr = originator address,
          //	T_seq       = ANSN,
          //	T_time      = current time + validity time.
          TopologyTuple topologyTuple;;
          topologyTuple.destAddr = addr;
          topologyTuple.lastAddr = msg.GetOriginatorAddress ();
          topologyTuple.sequenceNumber = tc.GetAnsn ();
          topologyTuple.expirationTime = now + msg.GetVTime ();
          AddTopologyTuple (topologyTuple);

          // Schedules topology tuple deletion
          Simulator::Schedule (DELAY (topologyTuple.expirationTime),
                               &Olsr::TopologyTupleTimerExpire,
                               this, topologyTuple);
        }
    }
}

///
/// \brief Processes a MID message following RFC 3626 specification.
///
/// The Interface Association Set is updated (if needed) with the information
/// of the received MID message.
///
/// \param msg the %OLSR message which contains the MID message.
/// \param sender_iface the address of the interface where the message was sent from.
///
void
Olsr::ProcessMid (const OlsrMessageHeader &msg,
                  const OlsrMidMessageHeader &mid,
                  const Ipv4Address &senderIface)
{
  Time now = Simulator::Now ();
	
  // 1. If the sender interface of this message is not in the symmetric
  // 1-hop neighborhood of this node, the message MUST be discarded.
  LinkTuple *linkTuple = m_state.FindSymLinkTuple (senderIface, now);
  if (linkTuple == NULL)
    return;
	
  // 2. For each interface address listed in the MID message
  const std::vector<Ipv4Address> &addrs = mid.GetInterfaceAddresses ();
  for (std::vector<Ipv4Address>::const_iterator i = addrs.begin ();
       i != addrs.end (); i++)
    {
      bool updated = false;
      IfaceAssocSet &ifaceAssoc = m_state.GetIfaceAssocSetMutable ();
      for (IfaceAssocSet::iterator tuple = ifaceAssoc.begin();
           tuple != ifaceAssoc.end(); tuple++)
        {
          if (tuple->ifaceAddr == *i
              && tuple->mainAddr == msg.GetOriginatorAddress ())
            {
              tuple->time = now + msg.GetVTime ();
              updated = true;
            }
        }
      if (!updated)
        {
          IfaceAssocTuple tuple;
          tuple.ifaceAddr = *i;
          tuple.mainAddr = msg.GetOriginatorAddress ();
          tuple.time = now + msg.GetVTime ();
          AddIfaceAssocTuple (tuple);
          // Schedules iface association tuple deletion
          Simulator::Schedule (DELAY (tuple.time),
                               &Olsr::IfaceAssocTupleTimerExpire, this, tuple);
        }
    }
}


///
/// \brief OLSR's default forwarding algorithm.
///
/// See RFC 3626 for details.
///
/// \param p the %OLSR packet which has been received.
/// \param msg the %OLSR message which must be forwarded.
/// \param dup_tuple NULL if the message has never been considered for forwarding,
/// or a duplicate tuple in other case.
/// \param local_iface the address of the interface where the message was received from.
///
void
Olsr::ForwardDefault (OlsrMessageHeader olsrMessage,
                      Packet messagePayload,
                      DuplicateTuple *duplicated,
                      const Ipv4Address &localIface,
                      const Ipv4Address &senderAddress)
{
  Time now = Simulator::Now ();
  
  // If the sender interface address is not in the symmetric
  // 1-hop neighborhood the message must not be forwarded
  LinkTuple *linkTuple = m_state.FindSymLinkTuple (senderAddress, now);
  if (linkTuple == NULL)
    return;

  // If the message has already been considered for forwarding,
  // it must not be retransmitted again
  if (duplicated != NULL && duplicated->retransmitted)
    {
//       debug("%f: Node %d does not forward a message received"
//             " from %d because it is duplicated\n",
//             Simulator::Now (),
//             OLSR::node_id(ra_addr()),
//             OLSR::node_id(dup_tuple->addr()));
      return;
    }
	
  // If the sender interface address is an interface address
  // of a MPR selector of this node and ttl is greater than 1,
  // the message must be retransmitted
  bool retransmitted = false;
  if (olsrMessage.GetTimeToLive () > 1)
    {
      MprSelectorTuple *mprselTuple =
        m_state.FindMprSelectorTuple (GetMainAddress (senderAddress));
      if (mprselTuple != NULL)
        {
          olsrMessage.SetTimeToLive (olsrMessage.GetTimeToLive () - 1);
          olsrMessage.SetHopCount (olsrMessage.GetHopCount () + 1);
          // We have to introduce a random delay to avoid
          // synchronization with neighbors.
          messagePayload.AddHeader (olsrMessage);
          QueueMessage (messagePayload, JITTER);
          retransmitted = true;
        }
    }
	
  // Update duplicate tuple...
  if (duplicated != NULL)
    {
      duplicated->expirationTime = now + OLSR_DUP_HOLD_TIME;
      duplicated->retransmitted = retransmitted;
      duplicated->ifaceList.push_back (localIface);
    }
  // ...or create a new one
  else
    {
      DuplicateTuple newDup;
      newDup.address = olsrMessage.GetOriginatorAddress ();
      newDup.sequenceNumber = olsrMessage.GetMessageSequenceNumber ();
      newDup.expirationTime = now + OLSR_DUP_HOLD_TIME;
      newDup.retransmitted = retransmitted;
      newDup.ifaceList.push_back (localIface);
      AddDuplicateTuple (newDup);
      // Schedule dup tuple deletion
      Simulator::Schedule (OLSR_DUP_HOLD_TIME,
                           &Olsr::DupTupleTimerExpire, this, newDup);
    }
}

///
/// \brief Enques an %OLSR message which will be sent with a delay of (0, delay].
///
/// This buffering system is used in order to piggyback several %OLSR messages in
/// a same %OLSR packet.
///
/// \param msg the %OLSR message which must be sent.
/// \param delay maximum delay the %OLSR message is going to be buffered.
///
void
Olsr::QueueMessage (Packet message, Time delay)
{
  m_queuedMessages.push_back (message);
  if (not m_queuedMessagesTimer.GetEvent ().IsRunning ())
    {
      m_queuedMessagesTimer = Simulator::Schedule (delay, &Olsr::SendQueuedMessages, this);
    }
}

void
Olsr::SendPacket (Packet packet)
{
  NS_DEBUG ("OLSR node " << m_mainAddress << " sending a OLSR packet");
  // Add a header
  OlsrPacketHeader header;
  header.SetPacketLength (header.GetSize () + packet.GetSize ());
  header.SetPacketSequenceNumber (GetPacketSequenceNumber ());
  packet.AddHeader (header);
  // Send it
  m_sendSocket->Send (packet.PeekData (), packet.GetSize ());
}

///
/// \brief Creates as many %OLSR packets as needed in order to send all buffered
/// %OLSR messages.
///
/// Maximum number of messages which can be contained in an %OLSR packet is
/// dictated by OLSR_MAX_MSGS constant.
///
void
Olsr::SendQueuedMessages ()
{
  Packet packet;
  int numMessages = 0;

  NS_DEBUG ("Olsr node " << m_mainAddress << ": SendQueuedMessages");

  for (std::vector<Packet>::const_iterator messagePkt = m_queuedMessages.begin ();
       messagePkt != m_queuedMessages.end ();
       messagePkt++)
    {
      packet.AddAtEnd (*messagePkt);
      if (++numMessages == OLSR_MAX_MSGS)
        {
          SendPacket (packet);
          // Reset variables for next packet
          numMessages = 0;
          packet = Packet ();
        }
    }

  if (packet.GetSize ())
    {
      SendPacket (packet);
    }

  m_queuedMessages.clear ();
}

///
/// \brief Creates a new %OLSR HELLO message which is buffered for being sent later on.
///
void
Olsr::SendHello ()
{
  OlsrMessageHeader msg;
  OlsrHelloMessageHeader hello;
  Time now = Simulator::Now ();

  msg.SetMessageType (OlsrMessageHeader::HELLO_MESSAGE);
  msg.SetVTime (OLSR_NEIGHB_HOLD_TIME);
  msg.SetOriginatorAddress (m_mainAddress);
  msg.SetTimeToLive (1);
  msg.SetHopCount (0);
  msg.SetMessageSequenceNumber (GetMessageSequenceNumber ());

  hello.SetHTime (m_helloInterval);
  hello.SetWillingness (m_willingness);

  std::vector<OlsrHelloMessageHeader::LinkMessage> linkMessages;
	
  for (LinkSet::const_iterator link_tuple = m_state.GetLinks ().begin ();
       link_tuple != m_state.GetLinks ().end (); link_tuple++)
    {
      if (not (link_tuple->localIfaceAddr == m_mainAddress
               && link_tuple->time >= now))
        continue;

      uint8_t link_type, nb_type;
			
      // Establishes link type
      if (m_useL2Notifications && link_tuple->lostTime >= now)
        {
          link_type = OLSR_LOST_LINK;
        }
      else if (link_tuple->symTime >= now)
        {
          link_type = OLSR_SYM_LINK;
        }
      else if (link_tuple->asymTime >= now)
        {
          link_type = OLSR_ASYM_LINK;
        }
      else
        {
          link_type = OLSR_LOST_LINK;
        }
      // Establishes neighbor type.
      if (m_state.FindMprAddress (GetMainAddress (link_tuple->neighborIfaceAddr)))
        {
          nb_type = OLSR_MPR_NEIGH;
        }
      else
        {
          bool ok = false;
          for (NeighborSet::const_iterator nb_tuple = m_state.GetNeighbors ().begin ();
               nb_tuple != m_state.GetNeighbors ().end ();
               nb_tuple++)
            {
              if (nb_tuple->neighborMainAddr == link_tuple->neighborIfaceAddr)
                {
                  if (nb_tuple->status == NeighborTuple::STATUS_SYM)
                    {
                      nb_type = OLSR_SYM_NEIGH;
                    }
                  else if (nb_tuple->status == NeighborTuple::STATUS_NOT_SYM)
                    {
                      nb_type = OLSR_NOT_NEIGH;
                    }
                  else
                    {
                      NS_ASSERT (!"There is a neighbor tuple with an unknown status!\n");
                    }
                  ok = true;
                  break;
                }
            }
          if (!ok)
            {
              NS_ASSERT (!"Link tuple has no corresponding neighbor tuple\n");
            }
        }

      OlsrHelloMessageHeader::LinkMessage linkMessage;
      linkMessage.linkCode = (link_type & 0x03) | ((nb_type << 2) & 0x0f);
      linkMessage.neighborInterfaceAddresses.push_back
        (link_tuple->neighborIfaceAddr);

      std::vector<Ipv4Address> interfaces =
        m_state.FindNeighborInterfaces (link_tuple->neighborIfaceAddr);

      linkMessage.neighborInterfaceAddresses.insert
        (linkMessage.neighborInterfaceAddresses.end (),
         interfaces.begin (), interfaces.end ());

      linkMessages.push_back (linkMessage);
    }
  hello.SetLinkMessages (linkMessages);
  Packet packet;
  packet.AddHeader (hello);
  NS_DEBUG ("OLSR HELLO message size: " << int (packet.GetSize () + msg.GetSize ())
            << " (with " << int (linkMessages.size ()) << " link messages)");
  msg.SetMessageSize (packet.GetSize () + msg.GetSize ());
  packet.AddHeader (msg);
  QueueMessage (packet, JITTER);
}

///
/// \brief Creates a new %OLSR TC message which is buffered for being sent later on.
///
void
Olsr::SendTc ()
{
  OlsrMessageHeader msg;
  OlsrTcMessageHeader tc;

  msg.SetMessageType (OlsrMessageHeader::TC_MESSAGE);
  msg.SetVTime (OLSR_TOP_HOLD_TIME);
  msg.SetOriginatorAddress (m_mainAddress);
  msg.SetTimeToLive (255);
  msg.SetHopCount (0);
  msg.SetMessageSequenceNumber (GetMessageSequenceNumber ());
  
  tc.SetAnsn (m_ansn);
  std::vector<Ipv4Address> neighbors;
  for (MprSelectorSet::const_iterator mprsel_tuple = m_state.GetMprSelectors ().begin();
       mprsel_tuple != m_state.GetMprSelectors ().end(); mprsel_tuple++)
    {
      neighbors.push_back (mprsel_tuple->mainAddr);
    }
  tc.SetNeighborAddresses (neighbors);

  Packet packet;
  packet.AddHeader (tc);
  msg.SetMessageSize (packet.GetSize () + msg.GetSize ());
  packet.AddHeader (msg);
  QueueMessage (packet, JITTER);
}

///
/// \brief Creates a new %OLSR MID message which is buffered for being sent later on.
///
void
Olsr::SendMid ()
{
  OlsrMessageHeader msg;
  OlsrMidMessageHeader mid;

  // A node which has only a single interface address participating in
  // the MANET (i.e., running OLSR), MUST NOT generate any MID
  // message.

  // A node with several interfaces, where only one is participating
  // in the MANET and running OLSR (e.g., a node is connected to a
  // wired network as well as to a MANET) MUST NOT generate any MID
  // messages.

  // A node with several interfaces, where more than one is
  // participating in the MANET and running OLSR MUST generate MID
  // messages as specified.

  // [ Note: assuming here that all interfaces participate in the
  // MANET; later we may want to make this configurable. ]

  Ipv4Address loopback ("127.0.0.1");
  std::vector<Ipv4Address> addresses;
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      Ipv4Address addr = m_ipv4->GetAddress (i);
      if (addr != m_mainAddress && addr != loopback)
        addresses.push_back (addr);
    }
  if (addresses.size () == 0)
    return;
  mid.SetInterfaceAddresses (addresses);
  
  msg.SetMessageType (OlsrMessageHeader::MID_MESSAGE);
  msg.SetVTime (OLSR_MID_HOLD_TIME);
  msg.SetOriginatorAddress (m_mainAddress);
  msg.SetTimeToLive (255);
  msg.SetHopCount (0);
  msg.SetMessageSequenceNumber (GetMessageSequenceNumber ());

  Packet packet;
  packet.AddHeader (mid);
  msg.SetMessageSize (packet.GetSize () + msg.GetSize ());
  packet.AddHeader (msg);
  QueueMessage (packet, JITTER);
}

///
/// \brief	Updates Link Set according to a new received HELLO message (following RFC 3626
///		specification). Neighbor Set is also updated if needed.
///
/// \param msg the OLSR message which contains the HELLO message.
/// \param receiver_iface the address of the interface where the message was received from.
/// \param sender_iface the address of the interface where the message was sent from.
///
void
Olsr::LinkSensing (const OlsrMessageHeader &msg,
                   const OlsrHelloMessageHeader &hello,
                   const Ipv4Address &receiverIface,
                   const Ipv4Address &senderIface)
{
  Time now = Simulator::Now ();
  bool updated = false;
  bool created = false;
	
  LinkTuple *link_tuple = m_state.FindLinkTuple (senderIface);
  if (link_tuple == NULL)
    {
      LinkTuple newLinkTuple;
      // We have to create a new tuple
      newLinkTuple.neighborIfaceAddr = senderIface;
      newLinkTuple.localIfaceAddr = receiverIface;
      newLinkTuple.symTime = now - Seconds (1);
      newLinkTuple.lostTime = Seconds (0);
      newLinkTuple.time = now + msg.GetVTime ();
      link_tuple = &AddLinkTuple (newLinkTuple, hello.GetWillingness ());
      created = true;
    }
  else
    updated = true;
	
  link_tuple->asymTime = now + msg.GetVTime ();
  for (std::vector<OlsrHelloMessageHeader::LinkMessage>::const_iterator linkMessage =
         hello.GetLinkMessages ().begin ();
       linkMessage != hello.GetLinkMessages ().end ();
       linkMessage++)
    {
      int lt = linkMessage->linkCode & 0x03; // Link Type
      int nt = linkMessage->linkCode >> 2; // Neighbor Type
      
      // We must not process invalid advertised links
      if ((lt == OLSR_SYM_LINK && nt == OLSR_NOT_NEIGH) ||
          (nt != OLSR_SYM_NEIGH && nt != OLSR_MPR_NEIGH
           && nt != OLSR_NOT_NEIGH))
        {
          continue;
        }
		
      for (std::vector<Ipv4Address>::const_iterator neighIfaceAddr =
             linkMessage->neighborInterfaceAddresses.begin ();
           neighIfaceAddr != linkMessage->neighborInterfaceAddresses.end ();
           neighIfaceAddr++)
        {
          if (*neighIfaceAddr == receiverIface)
            {
              if (lt == OLSR_LOST_LINK)
                {
                  link_tuple->symTime = now - Seconds (1);
                  updated = true;
                }
              else if (lt == OLSR_SYM_LINK || lt == OLSR_ASYM_LINK)
                {
                  link_tuple->symTime = now + msg.GetVTime ();
                  link_tuple->time = link_tuple->symTime + OLSR_NEIGHB_HOLD_TIME;
                  link_tuple->lostTime = Seconds (0);
                  updated = true;
                }
              break;
            }
        }
    }
  link_tuple->time = std::max(link_tuple->time, link_tuple->asymTime);

  if (updated)
    LinkTupleUpdated (*link_tuple);

  // Schedules link tuple deletion
  if (created && link_tuple != NULL)
    {
      m_events.Track (Simulator::Schedule
                      (DELAY (std::min (link_tuple->time, link_tuple->symTime)),
                       &Olsr::LinkTupleTimerExpire, this, *link_tuple));
    }
}

///
/// \brief	Updates the Neighbor Set according to the information contained in a new received
///		HELLO message (following RFC 3626).
///
/// \param msg the %OLSR message which contains the HELLO message.
///
void
Olsr::PopulateNeighborSet (const OlsrMessageHeader &msg,
                           const OlsrHelloMessageHeader &hello)
{
  NeighborTuple *nb_tuple = m_state.FindNeighborTuple (msg.GetOriginatorAddress ());
  if (nb_tuple != NULL)
    nb_tuple->willingness = hello.GetWillingness ();
}


///
/// \brief	Updates the 2-hop Neighbor Set according to the information contained in a new
///		received HELLO message (following RFC 3626).
///
/// \param msg the %OLSR message which contains the HELLO message.
///
void
Olsr::PopulateTwoHopNeighborSet (const OlsrMessageHeader &msg,
                                 const OlsrHelloMessageHeader &hello)
{
  Time now = Simulator::Now ();
	
  for (LinkSet::const_iterator link_tuple = m_state.GetLinks ().begin ();
       link_tuple != m_state.GetLinks ().end (); link_tuple++)
    {
      if (GetMainAddress (link_tuple->neighborIfaceAddr) == msg.GetOriginatorAddress ())
        {
          if (link_tuple->symTime >= now)
            {
              typedef std::vector<OlsrHelloMessageHeader::LinkMessage> LinkMessageVec;
              for (LinkMessageVec::const_iterator linkMessage =
                     hello.GetLinkMessages ().begin ();
                   linkMessage != hello.GetLinkMessages ().end ();
                   linkMessage++)
                {
                  int nt = linkMessage->linkCode >> 2;

                  for (std::vector<Ipv4Address>::const_iterator nb2hop_addr =
                         linkMessage->neighborInterfaceAddresses.begin ();
                       nb2hop_addr != linkMessage->neighborInterfaceAddresses.end ();
                       nb2hop_addr++)
                    {
                      if (nt == OLSR_SYM_NEIGH || nt == OLSR_MPR_NEIGH)
                        {
                          // if the main address of the 2-hop
                          // neighbor address = main address of
                          // the receiving node: silently
                          // discard the 2-hop neighbor address
                          if (*nb2hop_addr != m_routingAgentAddr)
                            {
                              // Otherwise, a 2-hop tuple is created
                              TwoHopNeighborTuple *nb2hop_tuple =
                                m_state.FindTwoHopNeighborTuple (msg.GetOriginatorAddress (),
                                                                 *nb2hop_addr);
                              if (nb2hop_tuple == NULL)
                                {
                                  TwoHopNeighborTuple new_nb2hop_tuple;
                                  new_nb2hop_tuple.neighborMainAddr = msg.GetOriginatorAddress ();
                                  new_nb2hop_tuple.twoHopNeighborAddr = *nb2hop_addr;
                                  AddTwoHopNeighborTuple (new_nb2hop_tuple);
                                  new_nb2hop_tuple.expirationTime =
                                    now + msg.GetVTime ();
                                  // Schedules nb2hop tuple
                                  // deletion
                                  m_events.Track (Simulator::Schedule
                                                  (DELAY (new_nb2hop_tuple.expirationTime),
                                                   &Olsr::Nb2hopTupleTimerExpire, this,
                                                   new_nb2hop_tuple));
                                }
                              else
                                {
                                  nb2hop_tuple->expirationTime =
                                    now + msg.GetVTime ();
                                }
								
                            }
                        }
                      else if (nt == OLSR_NOT_NEIGH)
                        {
                          // For each 2-hop node listed in the HELLO
                          // message with Neighbor Type equal to
                          // NOT_NEIGH all 2-hop tuples where:
                          // N_neighbor_main_addr == Originator
                          // Address AND N_2hop_addr  == main address
                          // of the 2-hop neighbor are deleted.
                          m_state.EraseTwoHopNeighborTuples
                            (msg.GetOriginatorAddress (), *nb2hop_addr);
                        }
                    }
                }
            }
        }
    }
}



///
/// \brief	Updates the MPR Selector Set according to the information contained in a new
///		received HELLO message (following RFC 3626).
///
/// \param msg the %OLSR message which contains the HELLO message.
///
void
Olsr::PopulateMprSelectorSet (const OlsrMessageHeader &msg,
                              const OlsrHelloMessageHeader &hello)
{
  Time now = Simulator::Now ();
	
  typedef std::vector<OlsrHelloMessageHeader::LinkMessage> LinkMessageVec;
  for (LinkMessageVec::const_iterator linkMessage = hello.GetLinkMessages ().begin ();
       linkMessage != hello.GetLinkMessages ().end ();
       linkMessage++)
    {
      int nt = linkMessage->linkCode >> 2;
      if (nt == OLSR_MPR_NEIGH)
        {
          for (std::vector<Ipv4Address>::const_iterator nb_iface_addr =
                 linkMessage->neighborInterfaceAddresses.begin ();
               nb_iface_addr != linkMessage->neighborInterfaceAddresses.end ();
               nb_iface_addr++)
            {
              if (*nb_iface_addr == m_mainAddress)
                {
                  // We must create a new entry into the mpr selector set
                  MprSelectorTuple *existing_mprsel_tuple =
                    m_state.FindMprSelectorTuple (msg.GetOriginatorAddress ());
                  if (existing_mprsel_tuple == NULL)
                    {
                      MprSelectorTuple mprsel_tuple;

                      mprsel_tuple.mainAddr = msg.GetOriginatorAddress ();
                      mprsel_tuple.expirationTime = now + msg.GetVTime ();
                      AddMprSelectorTuple (mprsel_tuple);

                      // Schedules mpr selector tuple deletion
                      m_events.Track (Simulator::Schedule
                                      (DELAY (mprsel_tuple.expirationTime),
                                       &Olsr::MprSelTupleTimerExpire,
                                       this, mprsel_tuple));
                    }
                  else
                    {
                      existing_mprsel_tuple->expirationTime = now + msg.GetVTime ();
                    }
                }
            }
        }
    }
}


#if 0
///
/// \brief	Drops a given packet because it couldn't be delivered to the corresponding
///		destination by the MAC layer. This may cause a neighbor loss, and appropiate
///		actions are then taken.
///
/// \param p the packet which couldn't be delivered by the MAC layer.
///
void
OLSR::mac_failed(Packet* p) {
	double now		= Simulator::Now ();
	struct hdr_ip* ih	= HDR_IP(p);
	struct hdr_cmn* ch	= HDR_CMN(p);
	
	debug("%f: Node %d MAC Layer detects a breakage on link to %d\n",
		now,
		OLSR::node_id(ra_addr()),
		OLSR::node_id(ch->next_hop()));
	
	if ((u_int32_t)ih->daddr() == IP_BROADCAST) {
		drop(p, DROP_RTR_MAC_CALLBACK);
		return;
	}
	
	OLSR_link_tuple* link_tuple = state_.find_link_tuple(ch->next_hop());
	if (link_tuple != NULL) {
		link_tuple->lost_time()	= now + OLSR_NEIGHB_HOLD_TIME;
		link_tuple->time()	= now + OLSR_NEIGHB_HOLD_TIME;
		nb_loss(link_tuple);
	}
	drop(p, DROP_RTR_MAC_CALLBACK);
}
#endif




///
/// \brief Performs all actions needed when a neighbor loss occurs.
///
/// Neighbor Set, 2-hop Neighbor Set, MPR Set and MPR Selector Set are updated.
///
/// \param tuple link tuple with the information of the link to the neighbor which has been lost.
///
void
Olsr::NeighborLoss (const LinkTuple &tuple)
{
//   debug("%f: Node %d detects neighbor %d loss\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborIfaceAddr));
	
  LinkTupleUpdated (tuple);
  m_state.EraseTwoHopNeighborTuples (GetMainAddress (tuple.neighborIfaceAddr));
  m_state.EraseMprSelectorTuples (GetMainAddress (tuple.neighborIfaceAddr));
  
  MprComputation();
  RoutingTableComputation();
}

///
/// \brief Adds a duplicate tuple to the Duplicate Set.
///
/// \param tuple the duplicate tuple to be added.
///
void
Olsr::AddDuplicateTuple (const DuplicateTuple &tuple)
{
	/*debug("%f: Node %d adds dup tuple: addr = %d seq_num = %d\n",
		Simulator::Now (),
		OLSR::node_id(ra_addr()),
		OLSR::node_id(tuple->addr()),
		tuple->seq_num());*/
  m_state.InsertDuplicateTuple (tuple);
}

///
/// \brief Removes a duplicate tuple from the Duplicate Set.
///
/// \param tuple the duplicate tuple to be removed.
///
void
Olsr::RemoveDuplicateTuple (const DuplicateTuple &tuple)
{
  /*debug("%f: Node %d removes dup tuple: addr = %d seq_num = %d\n",
    Simulator::Now (),
    OLSR::node_id(ra_addr()),
    OLSR::node_id(tuple->addr()),
    tuple->seq_num());*/
  m_state.EraseDuplicateTuple (tuple);
}

///
/// \brief Adds a link tuple to the Link Set (and an associated neighbor tuple to the Neighbor Set).
///
/// \param tuple the link tuple to be added.
/// \param willingness willingness of the node which is going to be inserted in the Neighbor Set.
///
LinkTuple&
Olsr::AddLinkTuple (const LinkTuple &tuple, uint8_t willingness)
{
//   debug("%f: Node %d adds link tuple: nb_addr = %d\n",
//         now,
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborIfaceAddr));
  LinkTuple &addedLinkTuple = m_state.InsertLinkTuple (tuple);
  // Creates associated neighbor tuple
  NeighborTuple nb_tuple;
  nb_tuple.neighborMainAddr = GetMainAddress (tuple.neighborIfaceAddr);
  nb_tuple.willingness = willingness;

  if (tuple.symTime >= Simulator::Now ())
    nb_tuple.status = NeighborTuple::STATUS_SYM;
  else
    nb_tuple.status = NeighborTuple::STATUS_NOT_SYM;

  AddNeighborTuple (nb_tuple);
  return addedLinkTuple;
}

///
/// \brief Removes a link tuple from the Link Set.
///
/// \param tuple the link tuple to be removed.
///
void
Olsr::RemoveLinkTuple (const LinkTuple &tuple)
{
//   nsaddr_t nb_addr	= get_main_addr(tuple->neighborIfaceAddr);
//   double now		= Simulator::Now ();
	
//   debug("%f: Node %d removes link tuple: nb_addr = %d\n",
//         now,
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborIfaceAddr));
	// Prints this here cause we are not actually calling rm_nb_tuple() (efficiency stuff)
  // 	debug("%f: Node %d removes neighbor tuple: nb_addr = %d\n",
  // 		now,
  // 		OLSR::node_id(ra_addr()),
  // 		OLSR::node_id(nb_addr));

  m_state.EraseLinkTuple (tuple);
  m_state.EraseNeighborTuple (GetMainAddress (tuple.neighborIfaceAddr));
}

///
/// \brief	This function is invoked when a link tuple is updated. Its aim is to
///		also update the corresponding neighbor tuple if it is needed.
///
/// \param tuple the link tuple which has been updated.
///
void
Olsr::LinkTupleUpdated (const LinkTuple &tuple)
{
  // Each time a link tuple changes, the associated neighbor tuple must be recomputed
  NeighborTuple *nb_tuple =
    m_state.FindNeighborTuple (GetMainAddress (tuple.neighborIfaceAddr));
  if (nb_tuple != NULL)
    {
      if (m_useL2Notifications && tuple.lostTime >= Simulator::Now ())
        {
          nb_tuple->status = NeighborTuple::STATUS_NOT_SYM;
        }
      else if (tuple.symTime >= Simulator::Now ())
        {
          nb_tuple->status = NeighborTuple::STATUS_SYM;
        }
      else
        {
          nb_tuple->status = NeighborTuple::STATUS_NOT_SYM;
        }
    }
	
//   debug("%f: Node %d has updated link tuple: nb_addr = %d status = %s\n",
//         now,
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborIfaceAddr),
//         ((nb_tuple->status() == OLSR_STATUS_SYM) ? "sym" : "not_sym"));

}

///
/// \brief Adds a neighbor tuple to the Neighbor Set.
///
/// \param tuple the neighbor tuple to be added.
///
void
Olsr::AddNeighborTuple (const NeighborTuple &tuple)
{
//   debug("%f: Node %d adds neighbor tuple: nb_addr = %d status = %s\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborMainAddr),
//         ((tuple->status() == OLSR_STATUS_SYM) ? "sym" : "not_sym"));
  
  m_state.InsertNeighborTuple (tuple);
}

///
/// \brief Removes a neighbor tuple from the Neighbor Set.
///
/// \param tuple the neighbor tuple to be removed.
///
void
Olsr::RemoveNeighborTuple (const NeighborTuple &tuple)
{
//   debug("%f: Node %d removes neighbor tuple: nb_addr = %d status = %s\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborMainAddr),
//         ((tuple->status() == OLSR_STATUS_SYM) ? "sym" : "not_sym"));
	
  m_state.EraseNeighborTuple (tuple);
}

///
/// \brief Adds a 2-hop neighbor tuple to the 2-hop Neighbor Set.
///
/// \param tuple the 2-hop neighbor tuple to be added.
///
void
Olsr::AddTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple)
{
//   debug("%f: Node %d adds 2-hop neighbor tuple: nb_addr = %d nb2hop_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborMainAddr),
//         OLSR::node_id(tuple->twoHopNeighborAddr));
  
  m_state.InsertTwoHopNeighborTuple (tuple);
}

///
/// \brief Removes a 2-hop neighbor tuple from the 2-hop Neighbor Set.
///
/// \param tuple the 2-hop neighbor tuple to be removed.
///
void
Olsr::RemoveTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple)
{
//   debug("%f: Node %d removes 2-hop neighbor tuple: nb_addr = %d nb2hop_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->neighborMainAddr),
//         OLSR::node_id(tuple->twoHopNeighborAddr));

  m_state.EraseTwoHopNeighborTuple (tuple);
}

///
/// \brief Adds an MPR selector tuple to the MPR Selector Set.
///
/// Advertised Neighbor Sequence Number (ANSN) is also updated.
///
/// \param tuple the MPR selector tuple to be added.
///
void
Olsr::AddMprSelectorTuple (const MprSelectorTuple  &tuple)
{
//   debug("%f: Node %d adds MPR selector tuple: nb_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->main_addr()));
  
  m_state.InsertMprSelectorTuple (tuple);
  m_ansn = (m_ansn + 1) % (OLSR_MAX_SEQ_NUM + 1);
}

///
/// \brief Removes an MPR selector tuple from the MPR Selector Set.
///
/// Advertised Neighbor Sequence Number (ANSN) is also updated.
///
/// \param tuple the MPR selector tuple to be removed.
///
void
Olsr::RemoveMprSelectorTuple (const MprSelectorTuple &tuple)
{
//   debug("%f: Node %d removes MPR selector tuple: nb_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->main_addr()));
  
  m_state.EraseMprSelectorTuple (tuple);
  m_ansn = (m_ansn + 1) % (OLSR_MAX_SEQ_NUM + 1);
}

///
/// \brief Adds a topology tuple to the Topology Set.
///
/// \param tuple the topology tuple to be added.
///
void
Olsr::AddTopologyTuple (const TopologyTuple &tuple)
{
//   debug("%f: Node %d adds topology tuple: dest_addr = %d last_addr = %d seq = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->dest_addr()),
//         OLSR::node_id(tuple->last_addr()),
//         tuple->seq());

  m_state.InsertTopologyTuple(tuple);
}

///
/// \brief Removes a topology tuple from the Topology Set.
///
/// \param tuple the topology tuple to be removed.
///
void
Olsr::RemoveTopologyTuple (const TopologyTuple &tuple)
{
//   debug("%f: Node %d removes topology tuple: dest_addr = %d last_addr = %d seq = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->dest_addr()),
//         OLSR::node_id(tuple->last_addr()),
//         tuple->seq());

  m_state.EraseTopologyTuple (tuple);
}

///
/// \brief Adds an interface association tuple to the Interface Association Set.
///
/// \param tuple the interface association tuple to be added.
///
void
Olsr::AddIfaceAssocTuple (const IfaceAssocTuple &tuple)
{
//   debug("%f: Node %d adds iface association tuple: main_addr = %d iface_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->main_addr()),
//         OLSR::node_id(tuple->iface_addr()));

  m_state.InsertIfaceAssocTuple (tuple);
}

///
/// \brief Removes an interface association tuple from the Interface Association Set.
///
/// \param tuple the interface association tuple to be removed.
///
void
Olsr::RemoveIfaceAssocTuple (const IfaceAssocTuple &tuple)
{
//   debug("%f: Node %d removes iface association tuple: main_addr = %d iface_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->main_addr()),
//         OLSR::node_id(tuple->iface_addr()));

  m_state.EraseIfaceAssocTuple (tuple);
}


uint16_t Olsr::GetPacketSequenceNumber ()
{
  m_packetSequenceNumber = (m_packetSequenceNumber + 1) % (OLSR_MAX_SEQ_NUM + 1);
  return m_packetSequenceNumber;
}

/// Increments message sequence number and returns the new value.
uint16_t Olsr::GetMessageSequenceNumber ()
{
  m_messageSequenceNumber = (m_messageSequenceNumber + 1) % (OLSR_MAX_SEQ_NUM + 1);
  return m_messageSequenceNumber;
}


///
/// \brief Sends a HELLO message and reschedules the HELLO timer.
/// \param e The event which has expired.
///
void
Olsr::HelloTimerExpire ()
{
  SendHello ();
  m_helloTimer = Simulator::Schedule (m_helloInterval, &Olsr::HelloTimerExpire, this);
}

///
/// \brief Sends a TC message (if there exists any MPR selector) and reschedules the TC timer.
/// \param e The event which has expired.
///
void
Olsr::TcTimerExpire ()
{
  if (m_state.GetMprSelectors ().size () > 0)
    SendTc ();
  m_tcTimer = Simulator::Schedule (m_tcInterval, &Olsr::TcTimerExpire, this);
}

///
/// \brief Sends a MID message (if the node has more than one interface) and resets the MID timer.
/// \warning Currently it does nothing because there is no support for multiple interfaces.
/// \param e The event which has expired.
///
void
Olsr::MidTimerExpire ()
{
  SendMid ();
  m_midTimer = Simulator::Schedule (m_midInterval, &Olsr::MidTimerExpire, this);
}

///
/// \brief Removes tuple_ if expired. Else timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
Olsr::DupTupleTimerExpire (DuplicateTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveDuplicateTuple (tuple);
    }
  else
    m_events.Track (Simulator::Schedule
                    (DELAY (tuple.expirationTime),
                     &Olsr::DupTupleTimerExpire, this, tuple));
}

///
/// \brief Removes tuple_ if expired. Else if symmetric time
/// has expired then it is assumed a neighbor loss and agent_->nb_loss()
/// is called. In this case the timer is rescheduled to expire at
/// tuple_->time(). Otherwise the timer is rescheduled to expire at
/// the minimum between tuple_->time() and tuple_->sym_time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
Olsr::LinkTupleTimerExpire (LinkTuple tuple)
{
  Time now = Simulator::Now ();
	
  if (tuple.time < now)
    {
      RemoveLinkTuple (tuple);
    }
  else if (tuple.symTime < now)
    {
      if (m_linkTupleTimerFirstTime)
        m_linkTupleTimerFirstTime = false;
      else
        NeighborLoss (tuple);

      m_events.Track (Simulator::Schedule
                      (DELAY(tuple.time),
                       &Olsr::LinkTupleTimerExpire, this, tuple));

    }
  else
    m_events.Track (Simulator::Schedule
                    (DELAY (std::min (tuple.time, tuple.symTime)),
                     &Olsr::LinkTupleTimerExpire, this, tuple));
}

///
/// \brief Removes tuple_ if expired. Else the timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
Olsr::Nb2hopTupleTimerExpire (TwoHopNeighborTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveTwoHopNeighborTuple (tuple);
    }
  else
    m_events.Track (Simulator::Schedule
                    (DELAY (tuple.expirationTime),
                     &Olsr::Nb2hopTupleTimerExpire, this, tuple));
}

///
/// \brief Removes tuple_ if expired. Else the timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
Olsr::MprSelTupleTimerExpire (MprSelectorTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveMprSelectorTuple (tuple);
    }
  else
    m_events.Track (Simulator::Schedule
                    (DELAY (tuple.expirationTime),
                     &Olsr::MprSelTupleTimerExpire, this, tuple));
}

///
/// \brief Removes tuple_ if expired. Else the timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
Olsr::TopologyTupleTimerExpire (TopologyTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveTopologyTuple (tuple);
    }
  else
    m_events.Track (Simulator::Schedule
                    (DELAY (tuple.expirationTime),
                     &Olsr::TopologyTupleTimerExpire, this, tuple));
}

///
/// \brief Removes tuple_ if expired. Else timer is rescheduled to expire at tuple_->time().
/// \warning Actually this is never invoked because there is no support for multiple interfaces.
/// \param e The event which has expired.
///
void
Olsr::IfaceAssocTupleTimerExpire (IfaceAssocTuple tuple)
{
  if (tuple.time < Simulator::Now ())
    {
      RemoveIfaceAssocTuple (tuple);
    }
  else
    m_events.Track (Simulator::Schedule
                    (DELAY (tuple.time),
                     &Olsr::IfaceAssocTupleTimerExpire, this, tuple));
}



}}; // namespace ns3, olsr



#ifdef RUN_SELF_TESTS


#include "ns3/test.h"

namespace ns3 { namespace olsr {

class OlsrTest : public ns3::Test {
private:
public:
  OlsrTest ();
  virtual bool RunTests (void);


};

OlsrTest::OlsrTest ()
  : ns3::Test ("Olsr")
{}


bool 
OlsrTest::RunTests (void)
{
  bool result = true;


  return result;
}

static OlsrTest gOlsrTest;

}}; // namespace


#endif /* RUN_SELF_TESTS */
