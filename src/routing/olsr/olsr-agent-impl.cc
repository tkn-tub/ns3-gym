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


///
/// \file	OLSR.cc
/// \brief	Implementation of OLSR agent and related classes.
///
/// This is the main file of this software because %OLSR's behaviour is
/// implemented here.
///

#include "olsr-agent-impl.h"
#include "ns3/socket-factory.h"
#include "ns3/udp.h"
#include "ns3/internet-node.h"
#include "ns3/simulator.h"
#include "ns3/debug.h"
#include "ns3/random-variable.h"
#include "ns3/inet-socket-address.h"
#include "ns3/composite-trace-resolver.h"


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
namespace olsr {

NS_DEBUG_COMPONENT_DEFINE ("OlsrAgent");


/********** OLSR class **********/


AgentImpl::AgentImpl (Ptr<Node> node)
  :
  m_useL2Notifications (false),
  m_helloTimer (Timer::CANCEL_ON_DESTROY),
  m_tcTimer (Timer::CANCEL_ON_DESTROY),
  m_midTimer (Timer::CANCEL_ON_DESTROY)
{
  m_helloTimer.SetFunction (&AgentImpl::HelloTimerExpire, this);
  m_tcTimer.SetFunction (&AgentImpl::TcTimerExpire, this);
  m_midTimer.SetFunction (&AgentImpl::MidTimerExpire, this);
  m_queuedMessagesTimer.SetFunction (&AgentImpl::SendQueuedMessages, this);


  SetInterfaceId (AgentImpl::iid);

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
  if (m_receiveSocket->Bind (InetSocketAddress (OLSR_PORT_NUMBER)))
    NS_ASSERT_MSG (false, "Failed to bind() OLSR receive socket");

  m_sendSocket = socketFactory->CreateSocket ();
  m_sendSocket->Connect (InetSocketAddress (Ipv4Address (0xffffffff), OLSR_PORT_NUMBER));

}

void AgentImpl::DoDispose ()
{
  m_ipv4 = 0;
  if (m_receiveSocket)
    {
      m_receiveSocket->Dispose ();
      m_receiveSocket = 0;
    }
  if (m_sendSocket)
    {
      m_sendSocket->Dispose ();
      m_sendSocket = 0;  
    }
  if (m_routingTable)
    {
      m_routingTable->Dispose ();
      m_routingTable = 0;
    }

  Object::DoDispose ();
}

void AgentImpl::Start ()
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

  if (m_sendSocket->Bind (InetSocketAddress (m_mainAddress, OLSR_PORT_NUMBER)))
    NS_ASSERT_MSG (false, "Failed to bind() OLSR send socket");
  m_receiveSocket->SetRecvCallback (MakeCallback (&AgentImpl::RecvOlsr,  this));

  HelloTimerExpire ();
  TcTimerExpire ();
  MidTimerExpire ();

  NS_DEBUG ("OLSR on node " << m_mainAddress << " started");
}

void AgentImpl::SetMainInterface (uint32_t interface)
{
  m_mainAddress = m_ipv4->GetAddress (interface);
}


Ptr<TraceResolver> 
AgentImpl::GetTraceResolver (void) const
{
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddSource ("rx",
                       TraceDoc ("receive OLSR packet",
                                 "const olsr::PacketHeader &", "header of OLSR packet received",
                                 "const olsr::MessageList &", "list of OLSR messages contained in the packet"
                                 ),
                       m_rxPacketTrace);
  resolver->AddSource ("tx",
                       TraceDoc ("send OLSR packet",
                                 "const olsr::PacketHeader &", "header of OLSR packet sent",
                                 "const olsr::MessageList &", "list of OLSR messages contained in the packet"
                                 ),
                       m_txPacketTrace);
  resolver->SetParentResolver (Object::GetTraceResolver ());
  return resolver;
}


//
// \brief Processes an incoming %OLSR packet following RFC 3626 specification.
void
AgentImpl::RecvOlsr (Ptr<Socket> socket,
                         const Packet &receivedPacket,
                         const Address &sourceAddress)
{
  NS_DEBUG ("OLSR node " << m_mainAddress << " received a OLSR packet");
  InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom (sourceAddress);
  
  // All routing messages are sent from and to port RT_PORT,
  // so we check it.
  NS_ASSERT (inetSourceAddr.GetPort () == OLSR_PORT_NUMBER);
  
  Packet packet = receivedPacket;

  olsr::PacketHeader olsrPacketHeader;
  packet.RemoveHeader (olsrPacketHeader);
  NS_ASSERT (olsrPacketHeader.GetPacketLength () >= olsrPacketHeader.GetSerializedSize ());
  uint32_t sizeLeft = olsrPacketHeader.GetPacketLength () - olsrPacketHeader.GetSerializedSize ();

  MessageList messages;
  
  while (sizeLeft)
    {
      MessageHeader messageHeader;
      if (packet.RemoveHeader (messageHeader) == 0)
        NS_ASSERT (false);
      
      sizeLeft -= messageHeader.GetSerializedSize ();

      NS_DEBUG ("Olsr Msg received with type "
                << std::dec << int (messageHeader.GetMessageType ())
                << " TTL=" << int (messageHeader.GetTimeToLive ())
                << " origAddr=" << messageHeader.GetOriginatorAddress ());
      messages.push_back (messageHeader);
    }

  m_rxPacketTrace (olsrPacketHeader, messages);

  for (MessageList::const_iterator messageIter = messages.begin ();
       messageIter != messages.end (); messageIter++)
    {
      const MessageHeader &messageHeader = *messageIter;
      // If ttl is less than or equal to zero, or
      // the receiver is the same as the originator,
      // the message must be silently dropped
      if (messageHeader.GetTimeToLive () == 0
          || messageHeader.GetOriginatorAddress () == m_mainAddress)
        {
          packet.RemoveAtStart (messageHeader.GetSerializedSize ()
                                - messageHeader.GetSerializedSize ());
          continue;
        }

      // If the message has been processed it must not be processed again
      bool do_forwarding = true;
      DuplicateTuple *duplicated = m_state.FindDuplicateTuple
        (messageHeader.GetOriginatorAddress (),
         messageHeader.GetMessageSequenceNumber ());
      
      if (duplicated == NULL)
        {
          switch (messageHeader.GetMessageType ())
            {
            case olsr::MessageHeader::HELLO_MESSAGE:
              NS_DEBUG ("OLSR node received HELLO message of size " << messageHeader.GetSerializedSize ());
              ProcessHello (messageHeader, m_mainAddress, inetSourceAddr.GetIpv4 ());
              break;

            case olsr::MessageHeader::TC_MESSAGE:
              NS_DEBUG ("OLSR node received TC message of size " << messageHeader.GetSerializedSize ());
              ProcessTc (messageHeader, inetSourceAddr.GetIpv4 ());
              break;

            case olsr::MessageHeader::MID_MESSAGE:
              NS_DEBUG ("OLSR node received MID message of size " << messageHeader.GetSerializedSize ());
              ProcessMid (messageHeader, inetSourceAddr.GetIpv4 ());
              break;

            default:
              NS_DEBUG ("OLSR message type " <<
                        int (messageHeader.GetMessageType ()) <<
                        " not implemented");
            }
        }
      else
        {
          NS_DEBUG ("OLSR message is duplicated, not reading it.");
      
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
          if (messageHeader.GetMessageType ()  != olsr::MessageHeader::HELLO_MESSAGE)
            ForwardDefault (messageHeader, duplicated,
                            m_mainAddress, inetSourceAddr.GetIpv4 ());
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
AgentImpl::Degree (NeighborTuple const &tuple)
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
AgentImpl::MprComputation()
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
      const NeighborTuple *nb_tuple = m_state.FindSymNeighborTuple (twoHopNeigh.neighborMainAddr);
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
AgentImpl::GetMainAddress (Ipv4Address iface_addr) const
{
  const IfaceAssocTuple *tuple =
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
AgentImpl::RoutingTableComputation ()
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
      const NeighborTuple *nb_tuple = m_state.FindSymNeighborTuple
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
          bool foundEntry = m_routingTable->Lookup (nb2hop_tuple.neighborMainAddr, entry);
          if (!foundEntry)
            NS_FATAL_ERROR ("m_routingTable->Lookup failure");

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
AgentImpl::ProcessHello (const olsr::MessageHeader &msg,
                             const Ipv4Address &receiverIface,
                             const Ipv4Address &senderIface)
{
  const olsr::MessageHeader::Hello &hello = msg.GetHello ();
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
AgentImpl::ProcessTc (const olsr::MessageHeader &msg,
                          const Ipv4Address &senderIface)
{
  const olsr::MessageHeader::Tc &tc = msg.GetTc ();
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
    m_state.FindNewerTopologyTuple (msg.GetOriginatorAddress (), tc.ansn);
  if (topologyTuple != NULL)
    return;
	
  // 3. All tuples in the topology set where:
  //	T_last_addr == originator address AND
  //	T_seq       <  ANSN
  // MUST be removed from the topology set.
  m_state.EraseOlderTopologyTuples (msg.GetOriginatorAddress (), tc.ansn);

  // 4. For each of the advertised neighbor main address received in
  // the TC message:
  for (std::vector<Ipv4Address>::const_iterator i = tc.neighborAddresses.begin ();
       i != tc.neighborAddresses.end (); i++)
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
          topologyTuple.sequenceNumber = tc.ansn;
          topologyTuple.expirationTime = now + msg.GetVTime ();
          AddTopologyTuple (topologyTuple);

          // Schedules topology tuple deletion
          m_events.Track (Simulator::Schedule (DELAY (topologyTuple.expirationTime),
                                               &AgentImpl::TopologyTupleTimerExpire,
                                               this, topologyTuple));
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
AgentImpl::ProcessMid (const olsr::MessageHeader &msg,
                           const Ipv4Address &senderIface)
{
  const olsr::MessageHeader::Mid &mid = msg.GetMid ();
  Time now = Simulator::Now ();
	
  // 1. If the sender interface of this message is not in the symmetric
  // 1-hop neighborhood of this node, the message MUST be discarded.
  LinkTuple *linkTuple = m_state.FindSymLinkTuple (senderIface, now);
  if (linkTuple == NULL)
    return;
	
  // 2. For each interface address listed in the MID message
  for (std::vector<Ipv4Address>::const_iterator i = mid.interfaceAddresses.begin ();
       i != mid.interfaceAddresses.end (); i++)
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
                               &AgentImpl::IfaceAssocTupleTimerExpire, this, tuple);
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
AgentImpl::ForwardDefault (olsr::MessageHeader olsrMessage,
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
          QueueMessage (olsrMessage, JITTER);
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
                           &AgentImpl::DupTupleTimerExpire, this, newDup);
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
AgentImpl::QueueMessage (const olsr::MessageHeader &message, Time delay)
{
  m_queuedMessages.push_back (message);
  if (not m_queuedMessagesTimer.IsRunning ())
    {
      m_queuedMessagesTimer.SetDelay (delay);
      m_queuedMessagesTimer.Schedule ();
    }
}

void
AgentImpl::SendPacket (Packet packet, const MessageList &containedMessages)
{
  NS_DEBUG ("OLSR node " << m_mainAddress << " sending a OLSR packet");

  // Add a header
  olsr::PacketHeader header;
  header.SetPacketLength (header.GetSerializedSize () + packet.GetSize ());
  header.SetPacketSequenceNumber (GetPacketSequenceNumber ());
  packet.AddHeader (header);

  // Trace it
  m_txPacketTrace (header, containedMessages);

  // Send it
  m_sendSocket->Send (packet);
}

///
/// \brief Creates as many %OLSR packets as needed in order to send all buffered
/// %OLSR messages.
///
/// Maximum number of messages which can be contained in an %OLSR packet is
/// dictated by OLSR_MAX_MSGS constant.
///
void
AgentImpl::SendQueuedMessages ()
{
  Packet packet;
  int numMessages = 0;

  NS_DEBUG ("Olsr node " << m_mainAddress << ": SendQueuedMessages");

  MessageList msglist;

  for (std::vector<olsr::MessageHeader>::const_iterator message = m_queuedMessages.begin ();
       message != m_queuedMessages.end ();
       message++)
    {
      Packet p;
      p.AddHeader (*message);
      packet.AddAtEnd (p);
      msglist.push_back (*message);
      if (++numMessages == OLSR_MAX_MSGS)
        {
          SendPacket (packet, msglist);
          msglist.clear ();
          // Reset variables for next packet
          numMessages = 0;
          packet = Packet ();
        }
    }

  if (packet.GetSize ())
    {
      SendPacket (packet, msglist);
    }

  m_queuedMessages.clear ();
}

///
/// \brief Creates a new %OLSR HELLO message which is buffered for being sent later on.
///
void
AgentImpl::SendHello ()
{
  olsr::MessageHeader msg;
  Time now = Simulator::Now ();

  msg.SetVTime (OLSR_NEIGHB_HOLD_TIME);
  msg.SetOriginatorAddress (m_mainAddress);
  msg.SetTimeToLive (1);
  msg.SetHopCount (0);
  msg.SetMessageSequenceNumber (GetMessageSequenceNumber ());
  olsr::MessageHeader::Hello &hello = msg.GetHello ();

  hello.SetHTime (m_helloInterval);
  hello.willingness = m_willingness;

  std::vector<olsr::MessageHeader::Hello::LinkMessage>
    &linkMessages = hello.linkMessages;
	
  for (LinkSet::const_iterator link_tuple = m_state.GetLinks ().begin ();
       link_tuple != m_state.GetLinks ().end (); link_tuple++)
    {
      if (not (link_tuple->localIfaceAddr == m_mainAddress
               && link_tuple->time >= now))
        continue;

      uint8_t link_type, nb_type = 0xff;
			
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

      olsr::MessageHeader::Hello::LinkMessage linkMessage;
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
  NS_DEBUG ("OLSR HELLO message size: " << int (msg.GetSerializedSize ())
            << " (with " << int (linkMessages.size ()) << " link messages)");
  QueueMessage (msg, JITTER);
}

///
/// \brief Creates a new %OLSR TC message which is buffered for being sent later on.
///
void
AgentImpl::SendTc ()
{
  olsr::MessageHeader msg;

  msg.SetVTime (OLSR_TOP_HOLD_TIME);
  msg.SetOriginatorAddress (m_mainAddress);
  msg.SetTimeToLive (255);
  msg.SetHopCount (0);
  msg.SetMessageSequenceNumber (GetMessageSequenceNumber ());
  
  olsr::MessageHeader::Tc &tc = msg.GetTc ();
  tc.ansn = m_ansn;
  for (MprSelectorSet::const_iterator mprsel_tuple = m_state.GetMprSelectors ().begin();
       mprsel_tuple != m_state.GetMprSelectors ().end(); mprsel_tuple++)
    {
      tc.neighborAddresses.push_back (mprsel_tuple->mainAddr);
    }
  QueueMessage (msg, JITTER);
}

///
/// \brief Creates a new %OLSR MID message which is buffered for being sent later on.
///
void
AgentImpl::SendMid ()
{
  olsr::MessageHeader msg;
  olsr::MessageHeader::Mid &mid = msg.GetMid ();

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
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      Ipv4Address addr = m_ipv4->GetAddress (i);
      if (addr != m_mainAddress && addr != loopback)
        mid.interfaceAddresses.push_back (addr);
    }
  if (mid.interfaceAddresses.size () == 0)
    return;
  
  msg.SetVTime (OLSR_MID_HOLD_TIME);
  msg.SetOriginatorAddress (m_mainAddress);
  msg.SetTimeToLive (255);
  msg.SetHopCount (0);
  msg.SetMessageSequenceNumber (GetMessageSequenceNumber ());

  QueueMessage (msg, JITTER);
}

///
/// \brief	Updates Link Set according to a new received HELLO message (following RFC 3626
///		specification). Neighbor Set is also updated if needed.
void
AgentImpl::LinkSensing (const olsr::MessageHeader &msg,
                            const olsr::MessageHeader::Hello &hello,
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
      link_tuple = &AddLinkTuple (newLinkTuple, hello.willingness);
      created = true;
    }
  else
    updated = true;
	
  link_tuple->asymTime = now + msg.GetVTime ();
  for (std::vector<olsr::MessageHeader::Hello::LinkMessage>::const_iterator linkMessage =
         hello.linkMessages.begin ();
       linkMessage != hello.linkMessages.end ();
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
      m_events.Track (Simulator::Schedule (DELAY (std::min (link_tuple->time, link_tuple->symTime)),
                                           &AgentImpl::LinkTupleTimerExpire, this, *link_tuple));
    }
}

///
/// \brief	Updates the Neighbor Set according to the information contained in a new received
///		HELLO message (following RFC 3626).
void
AgentImpl::PopulateNeighborSet (const olsr::MessageHeader &msg,
                                    const olsr::MessageHeader::Hello &hello)
{
  NeighborTuple *nb_tuple = m_state.FindNeighborTuple (msg.GetOriginatorAddress ());
  if (nb_tuple != NULL)
    nb_tuple->willingness = hello.willingness;
}


///
/// \brief	Updates the 2-hop Neighbor Set according to the information contained in a new
///		received HELLO message (following RFC 3626).
void
AgentImpl::PopulateTwoHopNeighborSet (const olsr::MessageHeader &msg,
                                          const olsr::MessageHeader::Hello &hello)
{
  Time now = Simulator::Now ();
	
  for (LinkSet::const_iterator link_tuple = m_state.GetLinks ().begin ();
       link_tuple != m_state.GetLinks ().end (); link_tuple++)
    {
      if (GetMainAddress (link_tuple->neighborIfaceAddr) == msg.GetOriginatorAddress ())
        {
          if (link_tuple->symTime >= now)
            {
              typedef std::vector<olsr::MessageHeader::Hello::LinkMessage> LinkMessageVec;
              for (LinkMessageVec::const_iterator linkMessage =
                     hello.linkMessages.begin ();
                   linkMessage != hello.linkMessages.end ();
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
                                  m_events.Track (Simulator::Schedule (DELAY (new_nb2hop_tuple.expirationTime),
                                                                       &AgentImpl::Nb2hopTupleTimerExpire, this,
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
void
AgentImpl::PopulateMprSelectorSet (const olsr::MessageHeader &msg,
                                       const olsr::MessageHeader::Hello &hello)
{
  Time now = Simulator::Now ();
	
  typedef std::vector<olsr::MessageHeader::Hello::LinkMessage> LinkMessageVec;
  for (LinkMessageVec::const_iterator linkMessage = hello.linkMessages.begin ();
       linkMessage != hello.linkMessages.end ();
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
                                       &AgentImpl::MprSelTupleTimerExpire, this,
                                       mprsel_tuple));
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
AgentImpl::NeighborLoss (const LinkTuple &tuple)
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
AgentImpl::AddDuplicateTuple (const DuplicateTuple &tuple)
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
AgentImpl::RemoveDuplicateTuple (const DuplicateTuple &tuple)
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
AgentImpl::AddLinkTuple (const LinkTuple &tuple, uint8_t willingness)
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
AgentImpl::RemoveLinkTuple (const LinkTuple &tuple)
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
AgentImpl::LinkTupleUpdated (const LinkTuple &tuple)
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
AgentImpl::AddNeighborTuple (const NeighborTuple &tuple)
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
AgentImpl::RemoveNeighborTuple (const NeighborTuple &tuple)
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
AgentImpl::AddTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple)
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
AgentImpl::RemoveTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple)
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
AgentImpl::AddMprSelectorTuple (const MprSelectorTuple  &tuple)
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
AgentImpl::RemoveMprSelectorTuple (const MprSelectorTuple &tuple)
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
AgentImpl::AddTopologyTuple (const TopologyTuple &tuple)
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
AgentImpl::RemoveTopologyTuple (const TopologyTuple &tuple)
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
AgentImpl::AddIfaceAssocTuple (const IfaceAssocTuple &tuple)
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
AgentImpl::RemoveIfaceAssocTuple (const IfaceAssocTuple &tuple)
{
//   debug("%f: Node %d removes iface association tuple: main_addr = %d iface_addr = %d\n",
//         Simulator::Now (),
//         OLSR::node_id(ra_addr()),
//         OLSR::node_id(tuple->main_addr()),
//         OLSR::node_id(tuple->iface_addr()));

  m_state.EraseIfaceAssocTuple (tuple);
}


uint16_t AgentImpl::GetPacketSequenceNumber ()
{
  m_packetSequenceNumber = (m_packetSequenceNumber + 1) % (OLSR_MAX_SEQ_NUM + 1);
  return m_packetSequenceNumber;
}

/// Increments message sequence number and returns the new value.
uint16_t AgentImpl::GetMessageSequenceNumber ()
{
  m_messageSequenceNumber = (m_messageSequenceNumber + 1) % (OLSR_MAX_SEQ_NUM + 1);
  return m_messageSequenceNumber;
}


///
/// \brief Sends a HELLO message and reschedules the HELLO timer.
/// \param e The event which has expired.
///
void
AgentImpl::HelloTimerExpire ()
{
  SendHello ();
  m_helloTimer.Schedule (m_helloInterval);
}

///
/// \brief Sends a TC message (if there exists any MPR selector) and reschedules the TC timer.
/// \param e The event which has expired.
///
void
AgentImpl::TcTimerExpire ()
{
  if (m_state.GetMprSelectors ().size () > 0)
    {
      SendTc ();
    }
  m_tcTimer.Schedule (m_tcInterval);
}

///
/// \brief Sends a MID message (if the node has more than one interface) and resets the MID timer.
/// \warning Currently it does nothing because there is no support for multiple interfaces.
/// \param e The event which has expired.
///
void
AgentImpl::MidTimerExpire ()
{
  SendMid ();
  m_midTimer.Schedule (m_midInterval);
}

///
/// \brief Removes tuple if expired. Else timer is rescheduled to expire at tuple.expirationTime.
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param tuple The tuple which has expired.
///
void
AgentImpl::DupTupleTimerExpire (DuplicateTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveDuplicateTuple (tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple.expirationTime),
                                           &AgentImpl::DupTupleTimerExpire, this,
                                           tuple));
    }
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
AgentImpl::LinkTupleTimerExpire (LinkTuple tuple)
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

      m_events.Track (Simulator::Schedule (DELAY (tuple.time),
                                           &AgentImpl::LinkTupleTimerExpire, this,
                                           tuple));
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (std::min (tuple.time, tuple.symTime)),
                                           &AgentImpl::LinkTupleTimerExpire, this,
                                           tuple));
    }
}

///
/// \brief Removes tuple_ if expired. Else the timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
AgentImpl::Nb2hopTupleTimerExpire (TwoHopNeighborTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveTwoHopNeighborTuple (tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple.expirationTime),
                                           &AgentImpl::Nb2hopTupleTimerExpire,
                                           this, tuple));
    }
}

///
/// \brief Removes tuple_ if expired. Else the timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
AgentImpl::MprSelTupleTimerExpire (MprSelectorTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveMprSelectorTuple (tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple.expirationTime),
                                           &AgentImpl::MprSelTupleTimerExpire,
                                           this, tuple));
    }
}

///
/// \brief Removes tuple_ if expired. Else the timer is rescheduled to expire at tuple_->time().
///
/// The task of actually removing the tuple is left to the OLSR agent.
///
/// \param e The event which has expired.
///
void
AgentImpl::TopologyTupleTimerExpire (TopologyTuple tuple)
{
  if (tuple.expirationTime < Simulator::Now ())
    {
      RemoveTopologyTuple (tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple.expirationTime),
                                           &AgentImpl::TopologyTupleTimerExpire,
                                           this, tuple));
    }
}

///
/// \brief Removes tuple_ if expired. Else timer is rescheduled to expire at tuple_->time().
/// \warning Actually this is never invoked because there is no support for multiple interfaces.
/// \param e The event which has expired.
///
void
AgentImpl::IfaceAssocTupleTimerExpire (IfaceAssocTuple tuple)
{
  if (tuple.time < Simulator::Now ())
    {
      RemoveIfaceAssocTuple (tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple.time),
                                           &AgentImpl::IfaceAssocTupleTimerExpire,
                                           this, tuple));
    }
}



}} // namespace olsr, ns3



#ifdef RUN_SELF_TESTS


#include "ns3/test.h"

namespace ns3 {

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

}


#endif /* RUN_SELF_TESTS */
