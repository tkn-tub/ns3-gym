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
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/inet-socket-address.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"

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

NS_LOG_COMPONENT_DEFINE ("OlsrAgent");


/********** OLSR class **********/

NS_OBJECT_ENSURE_REGISTERED (AgentImpl);

TypeId 
AgentImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::olsr::AgentImpl")
    .SetParent<Agent> ()
    .AddConstructor<AgentImpl> ()
    .AddAttribute ("HelloInterval", "XXX",
                   TimeValue (OLSR_HELLO_INTERVAL),
                   MakeTimeAccessor (&AgentImpl::m_helloInterval),
                   MakeTimeChecker ())
    .AddAttribute ("TcInterval", "XXX",
                   TimeValue (OLSR_TC_INTERVAL),
                   MakeTimeAccessor (&AgentImpl::m_tcInterval),
                   MakeTimeChecker ())
    .AddAttribute ("MidInterval", "XXX",
                   TimeValue (OLSR_MID_INTERVAL),
                   MakeTimeAccessor (&AgentImpl::m_midInterval),
                   MakeTimeChecker ())
    .AddAttribute ("Willingness", "XXX",
                   UintegerValue (OLSR_WILL_DEFAULT),
                   MakeUintegerAccessor (&AgentImpl::m_willingness),
                   MakeUintegerChecker<uint8_t> ())
    .AddTraceSource ("Rx", "Receive OLSR packet.",
                     MakeTraceSourceAccessor (&AgentImpl::m_rxPacketTrace))
    .AddTraceSource ("Tx", "Send OLSR packet.",
                     MakeTraceSourceAccessor (&AgentImpl::m_txPacketTrace))
    .AddTraceSource ("RoutingTableChanged", "The OLSR routing table has changed.",
		     MakeTraceSourceAccessor (&AgentImpl::m_routingTableChanged))
    ;
  return tid;
}


AgentImpl::AgentImpl ()
  :
  m_helloTimer (Timer::CANCEL_ON_DESTROY),
  m_tcTimer (Timer::CANCEL_ON_DESTROY),
  m_midTimer (Timer::CANCEL_ON_DESTROY)
{}

AgentImpl::~AgentImpl ()
{}

void
AgentImpl::SetNode (Ptr<Node> node)
{
  NS_LOG_DEBUG ("Created olsr::AgentImpl");
  m_helloTimer.SetFunction (&AgentImpl::HelloTimerExpire, this);
  m_tcTimer.SetFunction (&AgentImpl::TcTimerExpire, this);
  m_midTimer.SetFunction (&AgentImpl::MidTimerExpire, this);
  m_queuedMessagesTimer.SetFunction (&AgentImpl::SendQueuedMessages, this);

  m_packetSequenceNumber = OLSR_MAX_SEQ_NUM;
  m_messageSequenceNumber = OLSR_MAX_SEQ_NUM;
  m_ansn = OLSR_MAX_SEQ_NUM;

  m_linkTupleTimerFirstTime = true;

  m_ipv4 = node->GetObject<Ipv4> ();
  NS_ASSERT (m_ipv4);
}

void AgentImpl::DoDispose ()
{
  m_ipv4 = 0;

  for (std::map< Ptr<Socket>, Ipv4Address >::iterator iter = m_socketAddresses.begin ();
       iter != m_socketAddresses.end (); iter++)
    {
      iter->first->Dispose ();
    }
  m_socketAddresses.clear ();

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

  NS_LOG_DEBUG ("Starting OLSR on node " << m_mainAddress);

  m_routingTable = CreateObject<RoutingTable> ();
  m_routingTable->SetIpv4 (m_ipv4);
  m_routingTable->SetMainAddress (m_mainAddress);
  // Add OLSR as routing protocol, with slightly higher priority than
  // static routing.
  m_ipv4->AddRoutingProtocol (m_routingTable, 10);
  
  Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      Ipv4Address addr = m_ipv4->GetAddress (i);
      if (addr == loopback)
        continue;

      if (addr != m_mainAddress)
        {
          // Create never expiring interface association tuple entries for our
          // own network interfaces, so that GetMainAddress () works to
          // translate the node's own interface addresses into the main address.
          IfaceAssocTuple tuple;
          tuple.ifaceAddr = addr;
          tuple.mainAddr = m_mainAddress;
          AddIfaceAssocTuple (tuple);
          NS_ASSERT (GetMainAddress (addr) == m_mainAddress);
        }

      // Create a socket to listen only on this interface
      Ptr<Socket> socket = Socket::CreateSocket (GetObject<Node> (), 
        UdpSocketFactory::GetTypeId()); 
      socket->SetRecvCallback (MakeCallback (&AgentImpl::RecvOlsr,  this));
      if (socket->Bind (InetSocketAddress (addr, OLSR_PORT_NUMBER)))
        {
          NS_FATAL_ERROR ("Failed to bind() OLSR receive socket");
        }
      socket->Connect (InetSocketAddress (Ipv4Address (0xffffffff), OLSR_PORT_NUMBER));
      m_socketAddresses[socket] = addr;
    }

  HelloTimerExpire ();
  TcTimerExpire ();
  MidTimerExpire ();

  NS_LOG_DEBUG ("OLSR on node " << m_mainAddress << " started");
}

void AgentImpl::SetMainInterface (uint32_t interface)
{
  m_mainAddress = m_ipv4->GetAddress (interface);
}


//
// \brief Processes an incoming %OLSR packet following RFC 3626 specification.
void
AgentImpl::RecvOlsr (Ptr<Socket> socket)
{
  Ptr<Packet> receivedPacket;
  receivedPacket = socket->Recv ();

  SocketRxAddressTag tag;
  bool found = receivedPacket->PeekTag (tag);
  NS_ASSERT (found);
  Address sourceAddress = tag.GetAddress ();

  InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom (sourceAddress);
  Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4 ();
  Ipv4Address receiverIfaceAddr = m_socketAddresses[socket];
  NS_ASSERT (receiverIfaceAddr != Ipv4Address ());
  NS_LOG_DEBUG ("OLSR node " << m_mainAddress << " received a OLSR packet from "
                << senderIfaceAddr << " to " << receiverIfaceAddr);
  
  // All routing messages are sent from and to port RT_PORT,
  // so we check it.
  NS_ASSERT (inetSourceAddr.GetPort () == OLSR_PORT_NUMBER);
  
  Ptr<Packet> packet = receivedPacket;

  olsr::PacketHeader olsrPacketHeader;
  packet->RemoveHeader (olsrPacketHeader);
  NS_ASSERT (olsrPacketHeader.GetPacketLength () >= olsrPacketHeader.GetSerializedSize ());
  uint32_t sizeLeft = olsrPacketHeader.GetPacketLength () - olsrPacketHeader.GetSerializedSize ();

  MessageList messages;
  
  while (sizeLeft)
    {
      MessageHeader messageHeader;
      if (packet->RemoveHeader (messageHeader) == 0)
        NS_ASSERT (false);
      
      sizeLeft -= messageHeader.GetSerializedSize ();

      NS_LOG_DEBUG ("Olsr Msg received with type "
                << std::dec << int (messageHeader.GetMessageType ())
                << " TTL=" << int (messageHeader.GetTimeToLive ())
                << " origAddr=" << messageHeader.GetOriginatorAddress ());
      messages.push_back (messageHeader);
    }

  m_rxPacketTrace (olsrPacketHeader, messages);

  m_state.SetModified (false);

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
          packet->RemoveAtStart (messageHeader.GetSerializedSize ()
                                 - messageHeader.GetSerializedSize ());
          continue;
        }

      // If the message has been processed it must not be processed again
      bool do_forwarding = true;
      DuplicateTuple *duplicated = m_state.FindDuplicateTuple
        (messageHeader.GetOriginatorAddress (),
         messageHeader.GetMessageSequenceNumber ());

      // Get main address of the peer, which may be different from the packet source address
//       const IfaceAssocTuple *ifaceAssoc = m_state.FindIfaceAssocTuple (inetSourceAddr.GetIpv4 ());
//       Ipv4Address peerMainAddress;
//       if (ifaceAssoc != NULL)
//         {
//           peerMainAddress = ifaceAssoc->mainAddr;
//         }
//       else
//         {
//           peerMainAddress = inetSourceAddr.GetIpv4 () ;
//         }
      
      if (duplicated == NULL)
        {
          switch (messageHeader.GetMessageType ())
            {
            case olsr::MessageHeader::HELLO_MESSAGE:
              NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                            << "s OLSR node " << m_mainAddress
                            << " received HELLO message of size " << messageHeader.GetSerializedSize ());
              ProcessHello (messageHeader, receiverIfaceAddr, senderIfaceAddr);
              break;

            case olsr::MessageHeader::TC_MESSAGE:
              NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                            << "s OLSR node " << m_mainAddress
                            << " received TC message of size " << messageHeader.GetSerializedSize ());
              ProcessTc (messageHeader, senderIfaceAddr);
              break;

            case olsr::MessageHeader::MID_MESSAGE:
              NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                            << "s OLSR node " << m_mainAddress
                            <<  " received MID message of size " << messageHeader.GetSerializedSize ());
              ProcessMid (messageHeader, senderIfaceAddr);
              break;

            default:
              NS_LOG_DEBUG ("OLSR message type " <<
                        int (messageHeader.GetMessageType ()) <<
                        " not implemented");
            }
        }
      else
        {
          NS_LOG_DEBUG ("OLSR message is duplicated, not reading it.");
      
          // If the message has been considered for forwarding, it should
          // not be retransmitted again
          for (std::vector<Ipv4Address>::const_iterator it = duplicated->ifaceList.begin ();
               it != duplicated->ifaceList.end(); it++)
            {
              if (*it == receiverIfaceAddr)
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
            {
              ForwardDefault (messageHeader, duplicated,
                              receiverIfaceAddr, inetSourceAddr.GetIpv4 ());
            }
        }
	
    }

  // After processing all OLSR messages, we must recompute the routing table
  if (m_state.GetModified ())
    {
      RoutingTableComputation ();
      m_state.SetModified (false);
    }
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
  MprSet mprSet;
	
  
  // N is the subset of neighbors of the node, which are
  // neighbor "of the interface I"
  NeighborSet N;
  for (NeighborSet::const_iterator neighbor = m_state.GetNeighbors ().begin();
       neighbor != m_state.GetNeighbors ().end (); neighbor++)
    {
      if (neighbor->status == NeighborTuple::STATUS_SYM) // I think that we need this check
        {
          N.push_back (*neighbor);
        }
    }
	
  // N2 is the set of 2-hop neighbors reachable from "the interface
  // I", excluding:
  // (i)   the nodes only reachable by members of N with willingness WILL_NEVER
  // (ii)  the node performing the computation
  // (iii) all the symmetric neighbors: the nodes for which there exists a symmetric
  //       link to this node on some interface.
  TwoHopNeighborSet N2;
  for (TwoHopNeighborSet::const_iterator twoHopNeigh = m_state.GetTwoHopNeighbors ().begin ();
       twoHopNeigh != m_state.GetTwoHopNeighbors ().end (); twoHopNeigh++)
    {
      // excluding:
      // (ii)  the node performing the computation
      if (twoHopNeigh->twoHopNeighborAddr == m_mainAddress)
        {
          continue;
        }

      //  excluding:
      // (i)   the nodes only reachable by members of N with willingness WILL_NEVER      
      bool ok = false;
      for (NeighborSet::const_iterator neigh = N.begin ();
           neigh != N.end (); neigh++)
        {
          if (neigh->neighborMainAddr == twoHopNeigh->neighborMainAddr)
            {
              if (neigh->willingness == OLSR_WILL_NEVER)
                {
                  ok = false;
                  break;
                }
              else
                {
                  ok = true;
                  break;
                }
            }
        }
      if (!ok)
        {
          continue;
        }
      
      // excluding:
      // (iii) all the symmetric neighbors: the nodes for which there exists a symmetric
      //       link to this node on some interface.
      for (NeighborSet::const_iterator neigh = N.begin ();
           neigh != N.end (); neigh++)
        {
          if (neigh->neighborMainAddr == twoHopNeigh->twoHopNeighborAddr)
            {
              ok = false;
              break;
            }
        }

      if (ok)
        {
          N2.push_back (*twoHopNeigh);
        }
    }

  // 1. Start with an MPR set made of all members of N with
  // N_willingness equal to WILL_ALWAYS
  for (NeighborSet::const_iterator neighbor = N.begin (); neighbor != N.end (); neighbor++)
    {
      if (neighbor->willingness == OLSR_WILL_ALWAYS)
        {
          mprSet.insert (neighbor->neighborMainAddr);
          // (not in RFC but I think is needed: remove the 2-hop
          // neighbors reachable by the MPR from N2)
          for (TwoHopNeighborSet::iterator twoHopNeigh = N2.begin ();
               twoHopNeigh != N2.end (); )
            {
              if (twoHopNeigh->neighborMainAddr == neighbor->neighborMainAddr)
                {
                  twoHopNeigh = N2.erase (twoHopNeigh);
                }
              else
                {
                  twoHopNeigh++;
                }
            }
        }
    }
  
  // 2. Calculate D(y), where y is a member of N, for all nodes in N.
  // (we do this later)
	
  // 3. Add to the MPR set those nodes in N, which are the *only*
  // nodes to provide reachability to a node in N2.
  std::set<Ipv4Address> coveredTwoHopNeighbors;
  for (TwoHopNeighborSet::iterator twoHopNeigh = N2.begin (); twoHopNeigh != N2.end (); twoHopNeigh++)
    {
      NeighborSet::const_iterator onlyNeighbor = N.end ();
      
      for (NeighborSet::const_iterator neighbor = N.begin ();
           neighbor != N.end (); neighbor++)
        {
          if (neighbor->neighborMainAddr == twoHopNeigh->neighborMainAddr)
            {
              if (onlyNeighbor == N.end ())
                {
                  onlyNeighbor = neighbor;
                }
              else
                {
                  onlyNeighbor = N.end ();
                  break;
                }
            }
        }
      if (onlyNeighbor != N.end ())
        {
          mprSet.insert (onlyNeighbor->neighborMainAddr);
          coveredTwoHopNeighbors.insert (twoHopNeigh->twoHopNeighborAddr);
        }
    }
  // Remove the nodes from N2 which are now covered by a node in the MPR set.
  for (TwoHopNeighborSet::iterator twoHopNeigh = N2.begin ();
       twoHopNeigh != N2.end (); )
    {
      if (coveredTwoHopNeighbors.find (twoHopNeigh->twoHopNeighborAddr) != coveredTwoHopNeighbors.end ())
        {
          twoHopNeigh = N2.erase (twoHopNeigh);
        }
      else
        {
          twoHopNeigh++;
        }
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
          if (r == 0)
            {
              continue;
            }
          for (std::vector<const NeighborTuple *>::iterator it2 = reachability[r].begin ();
               it2 != reachability[r].end (); it2++)
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

      if (max != NULL)
        {
          mprSet.insert (max->neighborMainAddr);
          for (TwoHopNeighborSet::iterator twoHopNeigh = N2.begin ();
               twoHopNeigh != N2.end (); )
            {
              if (twoHopNeigh->neighborMainAddr == max->neighborMainAddr)
                {
                  twoHopNeigh = N2.erase (twoHopNeigh);
                }
              else
                {
                  twoHopNeigh++;
                }
            }
        }
    }

  m_state.SetMprSet (mprSet);

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
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " s: Node " << m_mainAddress
                << ": RoutingTableComputation begin...");

  // 1. All the entries from the routing table are removed.
  m_routingTable->Clear ();
	
  // 2. The new routing entries are added starting with the
  // symmetric neighbors (h=1) as the destination nodes.
  const NeighborSet &neighborSet = m_state.GetNeighbors ();
  for (NeighborSet::const_iterator it = neighborSet.begin ();
       it != neighborSet.end(); it++)
    {
      NeighborTuple const &nb_tuple = *it;
      NS_LOG_DEBUG ("Looking at neighbor tuple: " << nb_tuple);
      if (nb_tuple.status == NeighborTuple::STATUS_SYM)
        {
          bool nb_main_addr = false;
          const LinkTuple *lt = NULL;
          const LinkSet &linkSet = m_state.GetLinks ();
          for (LinkSet::const_iterator it2 = linkSet.begin();
               it2 != linkSet.end(); it2++)
            {
              LinkTuple const &link_tuple = *it2;
              NS_LOG_DEBUG ("Looking at link tuple: " << link_tuple
                            << (link_tuple.time >= Simulator::Now ()? "" : " (expired)"));
              if ((GetMainAddress (link_tuple.neighborIfaceAddr) == nb_tuple.neighborMainAddr)
                  && link_tuple.time >= Simulator::Now ())
                {
                  NS_LOG_LOGIC ("Link tuple matches neighbor " << nb_tuple.neighborMainAddr
                                << " => adding routing table entry to neighbor");
                  lt = &link_tuple;
                  m_routingTable->AddEntry (link_tuple.neighborIfaceAddr,
                                            link_tuple.neighborIfaceAddr,
                                            link_tuple.localIfaceAddr,
                                            1);
                  if (link_tuple.neighborIfaceAddr == nb_tuple.neighborMainAddr)
                    {
                      nb_main_addr = true;
                    }
                }
              else
                {
                  NS_LOG_LOGIC ("Link tuple: linkMainAddress= " << GetMainAddress (link_tuple.neighborIfaceAddr)
                                << "; neighborMainAddr =  " << nb_tuple.neighborMainAddr
                                << "; expired=" << int (link_tuple.time < Simulator::Now ())
                                << " => IGNORE");
                }
            }

          // If, in the above, no R_dest_addr is equal to the main
          // address of the neighbor, then another new routing entry
          // with MUST be added, with:
          //      R_dest_addr  = main address of the neighbor;
          //      R_next_addr  = L_neighbor_iface_addr of one of the
          //                     associated link tuple with L_time >= current time;
          //      R_dist       = 1;
          //      R_iface_addr = L_local_iface_addr of the
          //                     associated link tuple.
          if (!nb_main_addr && lt != NULL)
            {
              NS_LOG_LOGIC ("no R_dest_addr is equal to the main address of the neighbor "
                            "=> adding additional routing entry");
              m_routingTable->AddEntry(nb_tuple.neighborMainAddr,
                                       lt->neighborIfaceAddr,
                                       lt->localIfaceAddr,
                                       1);
            }
        }
    }
  
  //  3. for each node in N2, i.e., a 2-hop neighbor which is not a
  //  neighbor node or the node itself, and such that there exist at
  //  least one entry in the 2-hop neighbor set where
  //  N_neighbor_main_addr correspond to a neighbor node with
  //  willingness different of WILL_NEVER,
  const TwoHopNeighborSet &twoHopNeighbors = m_state.GetTwoHopNeighbors ();
  for (TwoHopNeighborSet::const_iterator it = twoHopNeighbors.begin ();
       it != twoHopNeighbors.end (); it++)
    {
      TwoHopNeighborTuple const &nb2hop_tuple = *it;

      NS_LOG_LOGIC ("Looking at two-hop neighbor tuple: " << nb2hop_tuple);

      // a 2-hop neighbor which is not a neighbor node or the node itself
      if (m_state.FindNeighborTuple (nb2hop_tuple.twoHopNeighborAddr))
        {
          NS_LOG_LOGIC ("Two-hop neighbor tuple is also neighbor; skipped.");
          continue;
        }

      if (nb2hop_tuple.twoHopNeighborAddr == m_mainAddress)
        {
          NS_LOG_LOGIC ("Two-hop neighbor is self; skipped.");
          continue;
        }

      // ...and such that there exist at least one entry in the 2-hop
      // neighbor set where N_neighbor_main_addr correspond to a
      // neighbor node with willingness different of WILL_NEVER...
      bool nb2hopOk = false;
      for (NeighborSet::const_iterator neighbor = neighborSet.begin ();
           neighbor != neighborSet.end(); neighbor++)
        {
          if (neighbor->neighborMainAddr == nb2hop_tuple.neighborMainAddr
              && neighbor->willingness != OLSR_WILL_NEVER)
            {
              nb2hopOk = true;
              break;
            }
        }
      if (!nb2hopOk)
        {
          NS_LOG_LOGIC ("Two-hop neighbor tuple skipped: 2-hop neighbor "
                        << nb2hop_tuple.twoHopNeighborAddr
                        << " is attached to neighbor " << nb2hop_tuple.neighborMainAddr
                        << ", which was not found in the Neighbor Set.");
          continue;
        }
      
      // one selects one 2-hop tuple and creates one entry in the routing table with:
      //                R_dest_addr  =  the main address of the 2-hop neighbor;
      //                R_next_addr  = the R_next_addr of the entry in the
      //                               routing table with:
      //                                   R_dest_addr == N_neighbor_main_addr
      //                                                  of the 2-hop tuple;
      //                R_dist       = 2;
      //                R_iface_addr = the R_iface_addr of the entry in the
      //                               routing table with:
      //                                   R_dest_addr == N_neighbor_main_addr
      //                                                  of the 2-hop tuple;
      RoutingTableEntry entry;
      bool foundEntry = m_routingTable->Lookup (nb2hop_tuple.neighborMainAddr, entry);
      if (foundEntry)
        {
          NS_LOG_LOGIC ("Adding routing entry for two-hop neighbor.");
          m_routingTable->AddEntry (nb2hop_tuple.twoHopNeighborAddr,
                                    entry.nextAddr,
                                    entry.interface,
                                    2);
        }
      else
        {
          NS_LOG_LOGIC ("NOT adding routing entry for two-hop neighbor ("
                        << nb2hop_tuple.twoHopNeighborAddr
                        << " not found in the routing table)");
        }
    }
  
  for (uint32_t h = 2; ; h++)
    {
      bool added = false;
		
      // 3.1. For each topology entry in the topology table, if its
      // T_dest_addr does not correspond to R_dest_addr of any
      // route entry in the routing table AND its T_last_addr
      // corresponds to R_dest_addr of a route entry whose R_dist
      // is equal to h, then a new route entry MUST be recorded in
      // the routing table (if it does not already exist)
      const TopologySet &topology = m_state.GetTopologySet ();
      for (TopologySet::const_iterator it = topology.begin ();
           it != topology.end (); it++)
        {
          const TopologyTuple &topology_tuple = *it;
          NS_LOG_LOGIC ("Looking at topology tuple: " << topology_tuple);

          RoutingTableEntry destAddrEntry, lastAddrEntry;
          bool have_destAddrEntry = m_routingTable->Lookup (topology_tuple.destAddr, destAddrEntry);
          bool have_lastAddrEntry = m_routingTable->Lookup (topology_tuple.lastAddr, lastAddrEntry);
          if (!have_destAddrEntry && have_lastAddrEntry && lastAddrEntry.distance == h)
            {
              NS_LOG_LOGIC ("Adding routing table entry based on the topology tuple.");
              // then a new route entry MUST be recorded in
              //                the routing table (if it does not already exist) where:
              //                     R_dest_addr  = T_dest_addr;
              //                     R_next_addr  = R_next_addr of the recorded
              //                                    route entry where:
              //                                    R_dest_addr == T_last_addr
              //                     R_dist       = h+1; and
              //                     R_iface_addr = R_iface_addr of the recorded
              //                                    route entry where:
              //                                       R_dest_addr == T_last_addr.
              m_routingTable->AddEntry (topology_tuple.destAddr,
                                        lastAddrEntry.nextAddr,
                                        lastAddrEntry.interface,
                                        h + 1);
              added = true;
            }
          else
            {
              NS_LOG_LOGIC ("NOT adding routing table entry based on the topology tuple: "
                            "have_destAddrEntry=" << have_destAddrEntry
                            << " have_lastAddrEntry=" << have_lastAddrEntry
                            << " lastAddrEntry.distance=" << (int) lastAddrEntry.distance
                            << " (h=" << h << ")");
            }
        }
      
      if (!added)
        break;
    }

  // 4. For each entry in the multiple interface association base
  // where there exists a routing entry such that:
  //	R_dest_addr  == I_main_addr  (of the multiple interface association entry)
  // AND there is no routing entry such that:
  //	R_dest_addr  == I_iface_addr
  const IfaceAssocSet &ifaceAssocSet = m_state.GetIfaceAssocSet ();
  for (IfaceAssocSet::const_iterator it = ifaceAssocSet.begin ();
       it != ifaceAssocSet.end (); it++)
    {
      IfaceAssocTuple const &tuple = *it;
      RoutingTableEntry entry1, entry2;
      bool have_entry1 = m_routingTable->Lookup (tuple.mainAddr, entry1);
      bool have_entry2 = m_routingTable->Lookup (tuple.ifaceAddr, entry2);
      if (have_entry1 && !have_entry2)
        {
          // then a route entry is created in the routing table with:
          //       R_dest_addr  =  I_iface_addr (of the multiple interface
          //                                     association entry)
          //       R_next_addr  =  R_next_addr  (of the recorded route entry)
          //       R_dist       =  R_dist       (of the recorded route entry)
          //       R_iface_addr =  R_iface_addr (of the recorded route entry).
          m_routingTable->AddEntry (tuple.ifaceAddr,
                                    entry1.nextAddr,
                                    entry1.interface,
                                    entry1.distance);
        }
    }

  NS_LOG_DEBUG ("Node " << m_mainAddress << ": RoutingTableComputation end.");
  m_routingTableChanged (m_routingTable->GetSize ());
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

#ifdef NS3_LOG_ENABLE
  {
    const LinkSet &links = m_state.GetLinks ();
    NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                  << "s ** BEGIN dump Link Set for OLSR Node " << m_mainAddress);
    for (LinkSet::const_iterator link = links.begin (); link != links.end (); link++)
      {
        NS_LOG_DEBUG(*link);
      }
    NS_LOG_DEBUG ("** END dump Link Set for OLSR Node " << m_mainAddress);

    const NeighborSet &neighbors = m_state.GetNeighbors ();
    NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                  << "s ** BEGIN dump Neighbor Set for OLSR Node " << m_mainAddress);
    for (NeighborSet::const_iterator neighbor = neighbors.begin (); neighbor != neighbors.end (); neighbor++)
      {
        NS_LOG_DEBUG(*neighbor);
      }
    NS_LOG_DEBUG ("** END dump Neighbor Set for OLSR Node " << m_mainAddress);
  }
#endif

  PopulateNeighborSet (msg, hello);
  PopulateTwoHopNeighborSet (msg, hello);

#ifdef NS3_LOG_ENABLE
  {
    const TwoHopNeighborSet &twoHopNeighbors = m_state.GetTwoHopNeighbors ();
    NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                  << "s ** BEGIN dump TwoHopNeighbor Set for OLSR Node " << m_mainAddress);
    for (TwoHopNeighborSet::const_iterator tuple = twoHopNeighbors.begin ();
         tuple != twoHopNeighbors.end (); tuple++)
      {
        NS_LOG_DEBUG(*tuple);
      }
    NS_LOG_DEBUG ("** END dump TwoHopNeighbor Set for OLSR Node " << m_mainAddress);
  }
#endif

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
                                               this,
                                               topologyTuple.destAddr,
                                               topologyTuple.lastAddr));
        }
    }

#ifdef NS3_LOG_ENABLE
  {
    const TopologySet &topology = m_state.GetTopologySet ();
    NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                  << "s ** BEGIN dump TopologySet for OLSR Node " << m_mainAddress);
    for (TopologySet::const_iterator tuple = topology.begin ();
         tuple != topology.end (); tuple++)
      {
        NS_LOG_DEBUG (*tuple);
      }
    NS_LOG_DEBUG ("** END dump TopologySet Set for OLSR Node " << m_mainAddress);
  }
#endif
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
  
  NS_LOG_DEBUG ("Node " << m_mainAddress << " ProcessMid from " << senderIface);
  // 1. If the sender interface of this message is not in the symmetric
  // 1-hop neighborhood of this node, the message MUST be discarded.
  LinkTuple *linkTuple = m_state.FindSymLinkTuple (senderIface, now);
  if (linkTuple == NULL)
    {
      NS_LOG_LOGIC ("Node " << m_mainAddress <<
                    ": the sender interface of this message is not in the "
                    "symmetric 1-hop neighborhood of this node,"
                    " the message MUST be discarded.");
      return;
    }
	
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
              NS_LOG_LOGIC ("IfaceAssoc updated: " << *tuple);
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
          NS_LOG_LOGIC ("New IfaceAssoc added: " << tuple);
          // Schedules iface association tuple deletion
          Simulator::Schedule (DELAY (tuple.time),
                               &AgentImpl::IfaceAssocTupleTimerExpire, this, tuple.ifaceAddr);
        }
    }

  // 3. (not part of the RFC) iterate over all NeighborTuple's and
  // TwoHopNeighborTuples, update the neighbor addresses taking into account
  // the new MID information.
  NeighborSet &neighbors = m_state.GetNeighbors ();
  for (NeighborSet::iterator neighbor = neighbors.begin (); neighbor != neighbors.end(); neighbor++)
    {
      neighbor->neighborMainAddr = GetMainAddress (neighbor->neighborMainAddr);
    }

  TwoHopNeighborSet &twoHopNeighbors = m_state.GetTwoHopNeighbors ();
  for (TwoHopNeighborSet::iterator twoHopNeighbor = twoHopNeighbors.begin ();
       twoHopNeighbor != twoHopNeighbors.end(); twoHopNeighbor++)
    {
      twoHopNeighbor->neighborMainAddr = GetMainAddress (twoHopNeighbor->neighborMainAddr);
      twoHopNeighbor->twoHopNeighborAddr = GetMainAddress (twoHopNeighbor->twoHopNeighborAddr);
    }
  NS_LOG_DEBUG ("Node " << m_mainAddress << " ProcessMid from " << senderIface << " -> END.");
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
      NS_LOG_LOGIC (Simulator::Now () << "Node " << m_mainAddress << " does not forward a message received"
                    " from " << olsrMessage.GetOriginatorAddress () << " because it is duplicated");
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
                           &AgentImpl::DupTupleTimerExpire, this,
                           newDup.address, newDup.sequenceNumber);
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
AgentImpl::SendPacket (Ptr<Packet> packet, 
                       const MessageList &containedMessages)
{
  NS_LOG_DEBUG ("OLSR node " << m_mainAddress << " sending a OLSR packet");

  // Add a header
  olsr::PacketHeader header;
  header.SetPacketLength (header.GetSerializedSize () + packet->GetSize ());
  header.SetPacketSequenceNumber (GetPacketSequenceNumber ());
  packet->AddHeader (header);

  // Trace it
  m_txPacketTrace (header, containedMessages);

  // Send it
  m_socketAddresses.begin ()->first->Send (packet);
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
  Ptr<Packet> packet = Create<Packet> ();
  int numMessages = 0;

  NS_LOG_DEBUG ("Olsr node " << m_mainAddress << ": SendQueuedMessages");

  MessageList msglist;

  for (std::vector<olsr::MessageHeader>::const_iterator message = m_queuedMessages.begin ();
       message != m_queuedMessages.end ();
       message++)
    {
      Ptr<Packet> p = Create<Packet> ();
      p->AddHeader (*message);
      packet->AddAtEnd (p);
      msglist.push_back (*message);
      if (++numMessages == OLSR_MAX_MSGS)
        {
          SendPacket (packet, msglist);
          msglist.clear ();
          // Reset variables for next packet
          numMessages = 0;
          packet = Create<Packet> ();
        }
    }

  if (packet->GetSize ())
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
	
  const LinkSet &links = m_state.GetLinks ();
  for (LinkSet::const_iterator link_tuple = links.begin ();
       link_tuple != links.end (); link_tuple++)
    {
      if (!(GetMainAddress (link_tuple->localIfaceAddr) == m_mainAddress
            && link_tuple->time >= now))
        {
          continue;
        }

      uint8_t link_type, nb_type = 0xff;
			
      // Establishes link type
      if (link_tuple->symTime >= now)
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
              if (nb_tuple->neighborMainAddr == GetMainAddress (link_tuple->neighborIfaceAddr))
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
                      NS_FATAL_ERROR ("There is a neighbor tuple with an unknown status!\n");
                    }
                  ok = true;
                  break;
                }
            }
          if (!ok)
            {
              continue;
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
  NS_LOG_DEBUG ("OLSR HELLO message size: " << int (msg.GetSerializedSize ())
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
  NS_LOG_DEBUG ("@" << now.GetSeconds () << ": Olsr node " << m_mainAddress
                << ": LinkSensing(receiverIface=" << receiverIface
                << ", senderIface=" << senderIface << ") BEGIN");
	
  NS_ASSERT (msg.GetVTime () > Seconds (0));
  LinkTuple *link_tuple = m_state.FindLinkTuple (senderIface);
  if (link_tuple == NULL)
    {
      LinkTuple newLinkTuple;
      // We have to create a new tuple
      newLinkTuple.neighborIfaceAddr = senderIface;
      newLinkTuple.localIfaceAddr = receiverIface;
      newLinkTuple.symTime = now - Seconds (1);
      newLinkTuple.time = now + msg.GetVTime ();
      link_tuple = &m_state.InsertLinkTuple (newLinkTuple);
      created = true;
      NS_LOG_LOGIC ("Existing link tuple did not exist => creating new one");
    }
  else
    {
      NS_LOG_LOGIC ("Existing link tuple already exists => will update it");
      updated = true;
    }
	
  link_tuple->asymTime = now + msg.GetVTime ();
  for (std::vector<olsr::MessageHeader::Hello::LinkMessage>::const_iterator linkMessage =
         hello.linkMessages.begin ();
       linkMessage != hello.linkMessages.end ();
       linkMessage++)
    {
      int lt = linkMessage->linkCode & 0x03; // Link Type
      int nt = (linkMessage->linkCode >> 2) & 0x03; // Neighbor Type

#ifdef NS3_LOG_ENABLE
      const char *linkTypeName;
      switch (lt)
        {
        case OLSR_UNSPEC_LINK: linkTypeName = "UNSPEC_LINK"; break;
        case OLSR_ASYM_LINK: linkTypeName = "ASYM_LINK"; break;
        case OLSR_SYM_LINK: linkTypeName = "SYM_LINK"; break;
        case OLSR_LOST_LINK: linkTypeName = "LOST_LINK"; break;
        default: linkTypeName = "(invalid value!)";
        }

      const char *neighborTypeName;
      switch (nt)
        {
        case OLSR_NOT_NEIGH: neighborTypeName = "NOT_NEIGH"; break;
        case OLSR_SYM_NEIGH: neighborTypeName = "SYM_NEIGH"; break;
        case OLSR_MPR_NEIGH: neighborTypeName = "MPR_NEIGH"; break;
        default: neighborTypeName = "(invalid value!)";
        }

      NS_LOG_DEBUG ("Looking at HELLO link messages with Link Type "
                    << lt << " (" << linkTypeName
                    << ") and Neighbor Type " << nt
                    << " (" << neighborTypeName << ")");
#endif

      // We must not process invalid advertised links
      if ((lt == OLSR_SYM_LINK && nt == OLSR_NOT_NEIGH) ||
          (nt != OLSR_SYM_NEIGH && nt != OLSR_MPR_NEIGH
           && nt != OLSR_NOT_NEIGH))
        {
          NS_LOG_LOGIC ("HELLO link code is invalid => IGNORING");
          continue;
        }
      
      for (std::vector<Ipv4Address>::const_iterator neighIfaceAddr =
             linkMessage->neighborInterfaceAddresses.begin ();
           neighIfaceAddr != linkMessage->neighborInterfaceAddresses.end ();
           neighIfaceAddr++)
        {
          NS_LOG_DEBUG ("   -> Neighbor: " << *neighIfaceAddr);
          if (*neighIfaceAddr == receiverIface)
            {
              if (lt == OLSR_LOST_LINK)
                {
                  NS_LOG_LOGIC ("link is LOST => expiring it");
                  link_tuple->symTime = now - Seconds (1);
                  updated = true;
                }
              else if (lt == OLSR_SYM_LINK || lt == OLSR_ASYM_LINK)
                {
                  NS_LOG_DEBUG (*link_tuple << ": link is SYM or ASYM => should become SYM now"
                                " (symTime being increased to " << now + msg.GetVTime ());
                  link_tuple->symTime = now + msg.GetVTime ();
                  link_tuple->time = link_tuple->symTime + OLSR_NEIGHB_HOLD_TIME;
                  updated = true;
                }
              else
                {
                  NS_FATAL_ERROR ("bad link type");
                }
              break;
            }
          else
            {
              NS_LOG_DEBUG ("     \\-> *neighIfaceAddr (" << *neighIfaceAddr
                            << " != receiverIface (" << receiverIface << ") => IGNORING!");
            }
        }
      NS_LOG_DEBUG ("Link tuple updated: " << int (updated));
    }
  link_tuple->time = std::max(link_tuple->time, link_tuple->asymTime);

  if (updated)
    {
      LinkTupleUpdated (*link_tuple);
    }

  // Schedules link tuple deletion
  if (created && link_tuple != NULL)
    {
      LinkTupleAdded (*link_tuple, hello.willingness);
      m_events.Track (Simulator::Schedule (DELAY (std::min (link_tuple->time, link_tuple->symTime)),
                                           &AgentImpl::LinkTupleTimerExpire, this,
                                           link_tuple->neighborIfaceAddr));
    }
  NS_LOG_DEBUG ("@" << now.GetSeconds () << ": Olsr node " << m_mainAddress
                << ": LinkSensing END");
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

  NS_LOG_DEBUG ("Olsr node " << m_mainAddress << ": PopulateTwoHopNeighborSet BEGIN");
	
  for (LinkSet::const_iterator link_tuple = m_state.GetLinks ().begin ();
       link_tuple != m_state.GetLinks ().end (); link_tuple++)
    {
      NS_LOG_LOGIC ("Looking at link tuple: " << *link_tuple);
      if (GetMainAddress (link_tuple->neighborIfaceAddr) != msg.GetOriginatorAddress ())
        {
          NS_LOG_LOGIC ("Link tuple ignored: "
                        "GetMainAddress (link_tuple->neighborIfaceAddr) != msg.GetOriginatorAddress ()");
          NS_LOG_LOGIC ("(GetMainAddress(" << link_tuple->neighborIfaceAddr << "): "
                        << GetMainAddress (link_tuple->neighborIfaceAddr)
                        << "; msg.GetOriginatorAddress (): " << msg.GetOriginatorAddress ());
          continue;
        }

      if (link_tuple->symTime < now)
        {
          NS_LOG_LOGIC ("Link tuple ignored: expired.");
          continue;
        }

      typedef std::vector<olsr::MessageHeader::Hello::LinkMessage> LinkMessageVec;
      for (LinkMessageVec::const_iterator linkMessage = hello.linkMessages.begin ();
           linkMessage != hello.linkMessages.end (); linkMessage++)
        {
          int neighborType = (linkMessage->linkCode >> 2) & 0x3;
#ifdef NS3_LOG_ENABLE
          const char *neighborTypeNames[3] = { "NOT_NEIGH", "SYM_NEIGH", "MPR_NEIGH" };
          const char *neighborTypeName = ((neighborType < 3)?
                                          neighborTypeNames[neighborType]
                                          : "(invalid value)");
          NS_LOG_DEBUG ("Looking at Link Message from HELLO message: neighborType="
                        << neighborType << " (" << neighborTypeName << ")");
#endif

          for (std::vector<Ipv4Address>::const_iterator nb2hop_addr_iter =
                 linkMessage->neighborInterfaceAddresses.begin ();
               nb2hop_addr_iter != linkMessage->neighborInterfaceAddresses.end ();
               nb2hop_addr_iter++)
            {
              Ipv4Address nb2hop_addr = GetMainAddress (*nb2hop_addr_iter);
              NS_LOG_DEBUG ("Looking at 2-hop neighbor address from HELLO message: "
                            << *nb2hop_addr_iter
                            << " (main address is " << nb2hop_addr << ")");
              if (neighborType == OLSR_SYM_NEIGH || neighborType == OLSR_MPR_NEIGH)
                {
                  // If the main address of the 2-hop neighbor address == main address
                  // of the receiving node, silently discard the 2-hop
                  // neighbor address.
                  if (nb2hop_addr == m_routingAgentAddr)
                    {
                      NS_LOG_LOGIC ("Ignoring 2-hop neighbor (it is the node itself)");
                      continue;
                    }

                  // Otherwise, a 2-hop tuple is created
                  TwoHopNeighborTuple *nb2hop_tuple =
                    m_state.FindTwoHopNeighborTuple (msg.GetOriginatorAddress (), nb2hop_addr);
                  NS_LOG_LOGIC ("Adding the 2-hop neighbor"
                                << (nb2hop_tuple? " (refreshing existing entry)" : ""));
                  if (nb2hop_tuple == NULL)
                    {
                      TwoHopNeighborTuple new_nb2hop_tuple;
                      new_nb2hop_tuple.neighborMainAddr = msg.GetOriginatorAddress ();
                      new_nb2hop_tuple.twoHopNeighborAddr = nb2hop_addr;
                      new_nb2hop_tuple.expirationTime = now + msg.GetVTime ();
                      AddTwoHopNeighborTuple (new_nb2hop_tuple);
                      // Schedules nb2hop tuple deletion
                      m_events.Track (Simulator::Schedule (DELAY (new_nb2hop_tuple.expirationTime),
                                                           &AgentImpl::Nb2hopTupleTimerExpire, this,
                                                           new_nb2hop_tuple.neighborMainAddr,
                                                           new_nb2hop_tuple.twoHopNeighborAddr));
                    }
                  else
                    {
                      nb2hop_tuple->expirationTime = now + msg.GetVTime ();
                    }
                }
              else if (neighborType == OLSR_NOT_NEIGH)
                {
                  // For each 2-hop node listed in the HELLO message
                  // with Neighbor Type equal to NOT_NEIGH all 2-hop
                  // tuples where: N_neighbor_main_addr == Originator
                  // Address AND N_2hop_addr == main address of the
                  // 2-hop neighbor are deleted.
                  NS_LOG_LOGIC ("2-hop neighbor is NOT_NEIGH => deleting matching 2-hop neighbor state");
                  m_state.EraseTwoHopNeighborTuples (msg.GetOriginatorAddress (), nb2hop_addr);
                }
              else
                {
                  NS_LOG_LOGIC ("*** WARNING *** Ignoring link message (inside HELLO) with bad"
                                " neighbor type value: " << neighborType);
                }
            }
        }
    }

  NS_LOG_DEBUG ("Olsr node " << m_mainAddress << ": PopulateTwoHopNeighborSet END");
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
              if (GetMainAddress (*nb_iface_addr) == m_mainAddress)
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
                                       mprsel_tuple.mainAddr));
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
OLSR::mac_failed(Ptr<Packet> p) {
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
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                << "s: OLSR Node " << m_mainAddress
                << " LinkTuple " << tuple.neighborIfaceAddr << " -> neighbor loss.");
  LinkTupleUpdated (tuple);
  m_state.EraseTwoHopNeighborTuples (GetMainAddress (tuple.neighborIfaceAddr));
  m_state.EraseMprSelectorTuples (GetMainAddress (tuple.neighborIfaceAddr));
  
  MprComputation ();
  RoutingTableComputation ();
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

void
AgentImpl::LinkTupleAdded (const LinkTuple &tuple, uint8_t willingness)
{
  // Creates associated neighbor tuple
  NeighborTuple nb_tuple;
  nb_tuple.neighborMainAddr = GetMainAddress (tuple.neighborIfaceAddr);
  nb_tuple.willingness = willingness;

  if (tuple.symTime >= Simulator::Now ())
    {
      nb_tuple.status = NeighborTuple::STATUS_SYM;
    }
  else
    {
      nb_tuple.status = NeighborTuple::STATUS_NOT_SYM;
    }

  AddNeighborTuple (nb_tuple);
}

///
/// \brief Removes a link tuple from the Link Set.
///
/// \param tuple the link tuple to be removed.
///
void
AgentImpl::RemoveLinkTuple (const LinkTuple &tuple)
{
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                << "s: OLSR Node " << m_mainAddress
                << " LinkTuple " << tuple << " REMOVED.");

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

  NS_LOG_DEBUG (Simulator::Now ().GetSeconds ()
                << "s: OLSR Node " << m_mainAddress
                << " LinkTuple " << tuple << " UPDATED.");

  NeighborTuple *nb_tuple =
    m_state.FindNeighborTuple (GetMainAddress (tuple.neighborIfaceAddr));

  if (nb_tuple != NULL)
    {
#ifdef NS3_LOG_ENABLE
      int statusBefore = nb_tuple->status;
#endif
      if (tuple.symTime >= Simulator::Now ())
        {
          nb_tuple->status = NeighborTuple::STATUS_SYM;
          NS_LOG_DEBUG (*nb_tuple << "->status = STATUS_SYM; changed:"
                        << int (statusBefore != nb_tuple->status));
        }
      else
        {
          nb_tuple->status = NeighborTuple::STATUS_NOT_SYM;
          NS_LOG_DEBUG (*nb_tuple << "->status = STATUS_NOT_SYM; changed:"
                        << int (statusBefore != nb_tuple->status));
        }
    }
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
  IncrementAnsn ();
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
  IncrementAnsn ();
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

void
AgentImpl::IncrementAnsn ()
{
  m_ansn = (m_ansn + 1) % (OLSR_MAX_SEQ_NUM + 1);
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
  IncrementAnsn ();
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
  IncrementAnsn ();
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
AgentImpl::DupTupleTimerExpire (Ipv4Address address, uint16_t sequenceNumber)
{
  DuplicateTuple *tuple =
    m_state.FindDuplicateTuple (address, sequenceNumber);
  if (tuple == NULL)
    {
      return;
    }
  if (tuple->expirationTime < Simulator::Now ())
    {
      RemoveDuplicateTuple (*tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple->expirationTime),
                                           &AgentImpl::DupTupleTimerExpire, this,
                                           address, sequenceNumber));
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
AgentImpl::LinkTupleTimerExpire (Ipv4Address neighborIfaceAddr)
{
  Time now = Simulator::Now ();

  // the tuple parameter may be a stale copy; get a newer version from m_state
  LinkTuple *tuple = m_state.FindLinkTuple (neighborIfaceAddr);
  if (tuple == NULL)
    {
      return;
    }
  if (tuple->time < now)
    {
      RemoveLinkTuple (*tuple);
    }
  else if (tuple->symTime < now)
    {
      if (m_linkTupleTimerFirstTime)
        m_linkTupleTimerFirstTime = false;
      else
        NeighborLoss (*tuple);

      m_events.Track (Simulator::Schedule (DELAY (tuple->time),
                                           &AgentImpl::LinkTupleTimerExpire, this,
                                           neighborIfaceAddr));
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (std::min (tuple->time, tuple->symTime)),
                                           &AgentImpl::LinkTupleTimerExpire, this,
                                           neighborIfaceAddr));
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
AgentImpl::Nb2hopTupleTimerExpire (Ipv4Address neighborMainAddr, Ipv4Address twoHopNeighborAddr)
{
  TwoHopNeighborTuple *tuple;
  tuple = m_state.FindTwoHopNeighborTuple (neighborMainAddr, twoHopNeighborAddr);
  if (tuple == NULL)
    {
      return;
    }
  if (tuple->expirationTime < Simulator::Now ())
    {
      RemoveTwoHopNeighborTuple (*tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple->expirationTime),
                                           &AgentImpl::Nb2hopTupleTimerExpire,
                                           this, neighborMainAddr, twoHopNeighborAddr));
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
AgentImpl::MprSelTupleTimerExpire (Ipv4Address mainAddr)
{
  MprSelectorTuple *tuple = m_state.FindMprSelectorTuple (mainAddr);
  if (tuple == NULL)
    {
      return;
    }
  if (tuple->expirationTime < Simulator::Now ())
    {
      RemoveMprSelectorTuple (*tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple->expirationTime),
                                           &AgentImpl::MprSelTupleTimerExpire,
                                           this, mainAddr));
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
AgentImpl::TopologyTupleTimerExpire (Ipv4Address destAddr, Ipv4Address lastAddr)
{
  TopologyTuple *tuple = m_state.FindTopologyTuple (destAddr, lastAddr);
  if (tuple == NULL)
    {
      return;
    }
  if (tuple->expirationTime < Simulator::Now ())
    {
      RemoveTopologyTuple (*tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple->expirationTime),
                                           &AgentImpl::TopologyTupleTimerExpire,
                                           this, tuple->destAddr, tuple->lastAddr));
    }
}

///
/// \brief Removes tuple_ if expired. Else timer is rescheduled to expire at tuple_->time().
/// \warning Actually this is never invoked because there is no support for multiple interfaces.
/// \param e The event which has expired.
///
void
AgentImpl::IfaceAssocTupleTimerExpire (Ipv4Address ifaceAddr)
{
  IfaceAssocTuple *tuple = m_state.FindIfaceAssocTuple (ifaceAddr);
  if (tuple == NULL)
    {
      return;
    }
  if (tuple->time < Simulator::Now ())
    {
      RemoveIfaceAssocTuple (*tuple);
    }
  else
    {
      m_events.Track (Simulator::Schedule (DELAY (tuple->time),
                                           &AgentImpl::IfaceAssocTupleTimerExpire,
                                           this, ifaceAddr));
    }
}



}} // namespace olsr, ns3


