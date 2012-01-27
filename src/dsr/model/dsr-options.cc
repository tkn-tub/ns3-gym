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

#define NS_LOG_APPEND_CONTEXT                                   \
  if (GetObject<Node> ()) { std::clog << "[node " << GetObject<Node> ()->GetId () << "] "; }

#include <list>
#include <ctime>
#include <map>

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "ns3/node.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/random-variable.h"
#include "ns3/udp-header.h"
#include "ns3/pointer.h"
#include "ns3/node-list.h"
#include "ns3/uinteger.h"
#include "ns3/object-vector.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/ipv4-l4-protocol.h"

#include "dsr-option-header.h"
#include "dsr-options.h"
#include "dsr-rcache.h"

NS_LOG_COMPONENT_DEFINE ("DsrOptions");

namespace ns3 {
namespace dsr {

NS_OBJECT_ENSURE_REGISTERED (DsrOptions);

TypeId DsrOptions::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptions")
    .SetParent<Object> ()
    .AddAttribute ("OptionNumber", "The Dsr option number.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&DsrOptions::GetOptionNumber),
                   MakeUintegerChecker<uint8_t> ())
    .AddTraceSource ("Rx", "Receive DSR packet.",
                     MakeTraceSourceAccessor (&DsrOptions::m_rxPacketTrace))
  ;
  return tid;
}

DsrOptions::DsrOptions ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptions::~DsrOptions ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void DsrOptions::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}

Ptr<Node> DsrOptions::GetNode () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

bool DsrOptions::ContainAddressAfter (Ipv4Address ipv4Address, Ipv4Address destAddress, std::vector<Ipv4Address> &nodeList)
{
  std::vector<Ipv4Address>::iterator it = find (nodeList.begin (), nodeList.end (), destAddress);

  for (std::vector<Ipv4Address>::iterator i = it; i != nodeList.end (); ++i)
    {
      if ((ipv4Address == (*i)) && ((*i) != nodeList.back ()))
        {
          return true;
        }
    }
  return false;
}

std::vector<Ipv4Address>
DsrOptions::CutRoute (Ipv4Address ipv4Address, std::vector<Ipv4Address> &nodeList)
{
  std::vector<Ipv4Address>::iterator it = find (nodeList.begin (), nodeList.end (), ipv4Address);
  std::vector<Ipv4Address> cutRoute;
  for (std::vector<Ipv4Address>::iterator i = it; i != nodeList.end (); ++i)
    {
      cutRoute.push_back (*i);
    }
  return cutRoute;
}

Ptr<Ipv4Route> DsrOptions::SetRoute (Ipv4Address nextHop, Ipv4Address srcAddress)
{
  NS_LOG_FUNCTION (this << nextHop << srcAddress);
  m_ipv4Route = Create<Ipv4Route> ();
  m_ipv4Route->SetDestination (nextHop);
  m_ipv4Route->SetGateway (nextHop);
  m_ipv4Route->SetSource (srcAddress);
  return m_ipv4Route;
}

bool DsrOptions::ReverseRoutes (std::vector<Ipv4Address> & vec)
{
  std::vector<Ipv4Address> vec2 (vec);
  vec.clear ();    // To ensure vec is empty before start
  for (std::vector<Ipv4Address>::reverse_iterator ri = vec2.rbegin (); ri
       != vec2.rend (); ++ri)
    {
      vec.push_back (*ri);
    }

  if ((vec.size () == vec2.size ()) && (vec.front () == vec2.back ()))
    {
      return true;
    }
  return false;
}

Ipv4Address DsrOptions::SearchNextHop (Ipv4Address ipv4Address, std::vector<Ipv4Address>& vec)
{
  Ipv4Address nextHop;
  if (vec.size () == 2)
    {
      NS_LOG_DEBUG ("The two nodes are neighbors");
      nextHop = vec[1];
      return nextHop;
    }
  else
    {
      if (ipv4Address == vec.back ())
        {
          NS_LOG_DEBUG ("We have reached to the final destination " << ipv4Address << " " << vec.back ());
          return ipv4Address;
        }
      for (std::vector<Ipv4Address>::iterator i = vec.begin (); i != vec.end (); ++i)
        {
          if (ipv4Address == (*i))
            {
              nextHop = *(++i);
              return nextHop;
            }
        }
    }
  NS_LOG_DEBUG ("next hop address not found, route corrupted");
  Ipv4Address none = "0.0.0.0";
  return none;
}

Ipv4Address DsrOptions::ReverseSearchNextHop (Ipv4Address ipv4Address, std::vector<Ipv4Address>& vec)
{
  Ipv4Address nextHop;
  if (vec.size () == 2)
    {
      NS_LOG_DEBUG ("The two nodes are neighbors");
      nextHop = vec[0];
      return nextHop;
    }
  else
    {
      for (std::vector<Ipv4Address>::reverse_iterator ri = vec.rbegin (); ri != vec.rend (); ++ri)
        {
          if (ipv4Address == (*ri))
            {
              nextHop = *(++ri);
              return nextHop;
            }
        }
    }
  NS_LOG_DEBUG ("next hop address not found, route corrupted");
  Ipv4Address none = "0.0.0.0";
  return none;
}

void DsrOptions::PrintVector (std::vector<Ipv4Address>& vec)
{
  /*
   * Check elements in a route vector
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

bool DsrOptions::IfDuplicates (std::vector<Ipv4Address>& vec, std::vector<Ipv4Address>& vec2)
{
  for (std::vector<Ipv4Address>::const_iterator i = vec.begin (); i != vec.end (); ++i)
    {
      for (std::vector<Ipv4Address>::const_iterator j = vec2.begin (); j != vec2.end (); ++j)
        {
          if ((*i) == (*j))
            {
              return true;
            }
          else
            {
              continue;
            }
        }
    }
  return false;
}

bool DsrOptions::CheckDuplicates (Ipv4Address ipv4Address, std::vector<Ipv4Address>& vec)
{
  for (std::vector<Ipv4Address>::const_iterator i = vec.begin (); i != vec.end (); ++i)
    {
      if ((*i) == ipv4Address)
        {
          return true;
        }
      else
        {
          continue;
        }
    }
  return false;
}

void DsrOptions::RemoveDuplicates (std::vector<Ipv4Address>& vec)
{
  //Remove duplicate ip address from the route if any, should not happen with normal behavior nodes
  std::vector<Ipv4Address> vec2 (vec); // declare vec2 as a copy of the vec
  PrintVector (vec2); // Print all the ip address in the route
  vec.clear (); // clear vec
  for (std::vector<Ipv4Address>::const_iterator i = vec2.begin (); i != vec2.end (); ++i)
    {
      if (vec.empty ())
        {
          vec.push_back (*i);
          continue;
        }
      else
        {
          for (std::vector<Ipv4Address>::iterator j = vec.begin (); j != vec.end (); ++j)
            {
              if ((*i) == (*j))
                {
                  if ((j + 1) != vec.end ())
                    {
                      vec.erase (j + 1, vec.end ());   // Automatic shorten the route
                      break;
                    }
                  else
                    {
                      break;
                    }
                }
              else if (j == (vec.end () - 1))
                {
                  vec.push_back (*i);
                  break;
                }
              else
                {
                  continue;
                }
            }
        }
    }
}

uint32_t
DsrOptions::GetIDfromIP (Ipv4Address address)
{
  int32_t nNodes = NodeList::GetNNodes ();
  for (int32_t i = 0; i < nNodes; ++i)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4->GetAddress (1, 0).GetLocal () == address)
        {
          return i;
        }
    }
  return 255;
}

Ptr<Node> DsrOptions::GetNodeWithAddress (Ipv4Address ipv4Address)
{
  int32_t nNodes = NodeList::GetNNodes ();
  for (int32_t i = 0; i < nNodes; ++i)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      int32_t ifIndex = ipv4->GetInterfaceForAddress (ipv4Address);
      if (ifIndex != -1)
        {
          return node;
        }
    }
  return 0;
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionPad1);

TypeId DsrOptionPad1::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionPad1")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionPad1> ()
  ;
  return tid;
}

DsrOptionPad1::DsrOptionPad1 ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionPad1::~DsrOptionPad1 ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

uint8_t DsrOptionPad1::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();

  return OPT_NUMBER;
}

uint8_t DsrOptionPad1::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);

  Ptr<Packet> p = packet->Copy ();
  DsrOptionPad1Header pad1Header;
  p->RemoveHeader (pad1Header);

  isPromisc = false;

  return pad1Header.GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionPadn);

TypeId DsrOptionPadn::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionPadn")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionPadn> ()
  ;
  return tid;
}

DsrOptionPadn::DsrOptionPadn ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionPadn::~DsrOptionPadn ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

uint8_t DsrOptionPadn::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return OPT_NUMBER;
}

uint8_t DsrOptionPadn::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);

  Ptr<Packet> p = packet->Copy ();

  DsrOptionPadnHeader padnHeader;
  p->RemoveHeader (padnHeader);

  isPromisc = false;

  return padnHeader.GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionRreq);

TypeId DsrOptionRreq::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionRreq")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionRreq> ()
  ;
  return tid;
}

TypeId DsrOptionRreq::GetInstanceTypeId () const
{
  return GetTypeId ();
}

DsrOptionRreq::DsrOptionRreq ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionRreq::~DsrOptionRreq ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

uint8_t DsrOptionRreq::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();

  return OPT_NUMBER;
}

uint8_t DsrOptionRreq::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);
  /*
   * Get the node associated with the ipv4 address and get several objects from the node and leave for further use
   */
  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();
  Ptr<dsr::RouteCache> m_routeCache = node->GetObject<dsr::RouteCache> ();
  Ptr<dsr::RreqTable>  m_rreqTable = node->GetObject<dsr::RreqTable> ();
  ActiveRouteTimeout = m_routeCache->GetCacheTimeout ();

  // Set the isError boolean value as false
  bool isError = false;
  Ptr<Packet> p = packet->Copy (); // The packet here doesn't contain the fixed size dsr header
  /*
   * \brief Get the number of routers' address field before removing the header
   * \peek the packet and get the value
   */
  uint8_t buf[2];
  p->CopyData (buf, sizeof(buf));
  uint8_t numberAddress = (buf[1] - 6) / 4;

  if (numberAddress >= 255)
    {
      NS_LOG_DEBUG ("Discard the packet, malformed header since two many ip addresses in route");
      m_dropTrace (packet); // call the drop trace to show in the tracing
      return 0;
    }

  NS_LOG_DEBUG ("The number of Ip addresses " << (uint32_t)numberAddress);
  /*
   * Create the dsr rreq header
   */
  DsrOptionRreqHeader rreq;

  // Fields from IP header
  Ipv4Address srcAddress = ipv4Header.GetSource ();
  Ipv4Address destAddress = ipv4Header.GetDestination ();
  /*
   * Set the number of addresses with the value from peek data and remove the rreq header
   */
  rreq.SetNumberAddress (numberAddress);

  /*
   * \ when the ip source address is equal to the address of our own, this is request packet originated
   * \ by the node itself, discard it
   */
  if (srcAddress == ipv4Address)
    {
      NS_LOG_DEBUG ("Discard the packet");
      m_dropTrace (packet); // call the drop trace to show in the tracing
      return 0;
    }

  p->RemoveHeader (rreq);
  Ptr<Packet> errP = p->Copy ();

  // The target address is where we want to send the data packets
  Ipv4Address targetAddress = rreq.GetTarget ();
  uint16_t id = rreq.GetId ();
  // Get the node list and source address from the route request header
  std::vector<Ipv4Address> nodeList = rreq.GetNodesAddresses ();
  PrintVector (nodeList);

  // Get the TTL value
  uint8_t ttl = ipv4Header.GetTtl ();
  NS_LOG_DEBUG ("The TTL " << (uint32_t) ttl);
  /*
   *  Node checks to determine whether it has received a RREQ with the same Originator IP Address and RREQ ID.
   *  If such a RREQ has been received, the node silently discards the newly received RREQ.
   */
  if (ttl)
    {
      // if the TTL value is not 0, save the route request entry
      if (m_rreqTable->FindSrc (source, targetAddress, id))
        {
          NS_LOG_DEBUG ("Received the same route request recently, not processing it further");
          return 0;
        }
    }

  if (CheckDuplicates (ipv4Address, nodeList))
    {
      /*
       * if the route contains the node address already, drop the request packet
       */
      m_dropTrace (packet);    // call drop trace
      NS_LOG_DEBUG ("Our node address is already seen in the route, drop the request");
      return 0;
    }
  else
    {
      // Verify the option length
      uint8_t length = rreq.GetLength ();
      if (length % 2 != 0)
        {
          NS_LOG_LOGIC ("Malformed header. Drop!");
          m_dropTrace (packet); // call drop trace
          return 0;
        }

      // A node ignores all RREQs received from any node in its blacklist
      RouteCacheEntry toPrev;
      /*
       *  When the reverse route is created or updated, the following actions on the route are also carried out:
       *  3. the next hop in the routing table becomes the node from which the  RREQ was received
       *  4. the hop count is copied from the Hop Count in the RREQ message;
       */

      //  A node generates a RREP if either:
      //  (i)  it is itself the destination,
      /*
       * The target address equal to our own ip address
       */
      if (targetAddress == ipv4Address)
        {
          Ipv4Address nextHop; // Declare the next hop address to use
          if (nodeList.size () == 1)
            {
              NS_LOG_DEBUG ("These two nodes are neighbors");
              m_finalRoute.clear ();
              m_finalRoute.push_back (srcAddress);     // push back the request originator's address
              m_finalRoute.push_back (ipv4Address);    // push back our own address
              nextHop = srcAddress;
            }
          else
            {
              nodeList.push_back (ipv4Address);    // push back our own address
              m_finalRoute.clear ();              // get a clear route vector
              for (std::vector<Ipv4Address>::iterator i = nodeList.begin (); i != nodeList.end (); ++i)
                {
                  m_finalRoute.push_back (*i);  // Get the full route from source to destination
                }
              PrintVector (m_finalRoute);
              nextHop = ReverseSearchNextHop (ipv4Address, m_finalRoute); // get the next hop
            }

          DsrOptionRrepHeader rrep;
          rrep.SetNodesAddress (m_finalRoute);     // Set the node addresses in the route reply header
          NS_LOG_DEBUG ("The nextHop address " << nextHop);
          Ipv4Address replyDst = m_finalRoute.front ();
          /*
           * This part add dsr header to the packet and send reply
           */
          DsrRoutingHeader dsrRoutingHeader;
          dsrRoutingHeader.SetNextHeader (protocol);
          dsrRoutingHeader.SetMessageType (1);
          dsrRoutingHeader.SetSourceId (GetIDfromIP (ipv4Address));
          dsrRoutingHeader.SetDestId (GetIDfromIP (replyDst));
          // Set the route for route reply
          SetRoute (nextHop, ipv4Address);

          uint8_t length = rrep.GetLength ();  // Get the length of the rrep header excluding the type header
          dsrRoutingHeader.SetPayloadLength (length + 2);
          dsrRoutingHeader.AddDsrOption (rrep);
          Ptr<Packet> newPacket = Create<Packet> ();
          newPacket->AddHeader (dsrRoutingHeader);
          dsr->ScheduleInitialReply (newPacket, ipv4Address, nextHop, m_ipv4Route);
          /*
           * Create the route entry to the rreq originator and save it to route cache, also need to reverse the route
           */
          PrintVector (m_finalRoute);
          if (ReverseRoutes (m_finalRoute))
            {
              PrintVector (m_finalRoute);
              Ipv4Address dst = m_finalRoute.back ();
              bool addRoute = false;
              if (numberAddress > 0)
                {
                  RouteCacheEntry toSource (/*IP_VECTOR=*/ m_finalRoute, /*dst=*/
                                                           dst, /*expire time=*/ ActiveRouteTimeout);
                  if (m_routeCache->IsLinkCache ())
                    {
                      addRoute = m_routeCache->AddRoute_Link (m_finalRoute, ipv4Address);
                    }
                  else
                    {
                      addRoute = m_routeCache->AddRoute (toSource);
                    }
                }
              else
                {
                  NS_LOG_DEBUG ("Abnormal RouteRequest");
                  return 0;
                }

              if (addRoute)
                {
                  NS_LOG_DEBUG ("The route is failed to add in cache");
                  return 0;
                }
              else
                {
                  /*
                   * Found a route to the dst, construct the source route option header
                   */
                  DsrOptionSRHeader sourceRoute;
                  NS_LOG_DEBUG ("The route length " << m_finalRoute.size ());
                  sourceRoute.SetNodesAddress (m_finalRoute);
                  if (m_routeCache->IsLinkCache ())
                    {
                      m_routeCache->UseExtends (m_finalRoute);
                    }
                  sourceRoute.SetSegmentsLeft ((m_finalRoute.size () - 2));
                  uint8_t salvage = 0;
                  sourceRoute.SetSalvage (salvage);
                  Ipv4Address nextHop = SearchNextHop (ipv4Address, m_finalRoute); // Get the next hop address
                  NS_LOG_DEBUG ("The nextHop address " << nextHop);

                  if (nextHop == "0.0.0.0")
                    {
                      dsr->PacketNewRoute (dsrP, ipv4Address, dst, protocol);
                      return 0;
                    }
                  SetRoute (nextHop, ipv4Address);
                  /*
                   * Schedule the packet retry
                   */
                  dsr->SendPacket (sourceRoute, nextHop, protocol);
                  // Cancel the route request timer for destination
                  dsr->CancelRreqTimer (dst);
                }
            }
          else
            {
              NS_LOG_DEBUG ("Unable to reverse route");
              return 0;
            }
          isPromisc = false;
          return rreq.GetSerializedSize ();
        }

      /*
       * (ii) or it has an active route to the destination, send reply based on request header and route cache,
       *      need to delay based on a random value from d = H * (h - 1 + r), which can avoid possible route
       *      reply storm.
       */
      else if (m_routeCache->LookupRoute (targetAddress, toPrev))
        {

          RouteCacheEntry::IP_VECTOR ip = toPrev.GetVector (); // The route from our own route cache to dst

          PrintVector (ip);
          std::vector<Ipv4Address> nodeList = rreq.GetNodesAddresses (); // Route from rreq header
          std::vector<Ipv4Address> saveRoute (nodeList);
          PrintVector (saveRoute);
          // Verify if the two vector contains duplicates, if so, do not use
          // the route found and forward the route request
          if (!(IfDuplicates (ip, nodeList)))
            {
              m_finalRoute.clear (); // Clear the final route vector
              /**
               * push back the intermediate node address from the source to this node
               */
              for (std::vector<Ipv4Address>::iterator i = nodeList.begin (); i != nodeList.end (); ++i)
                {
                  m_finalRoute.push_back (*i);
                }
              /**
               * push back the route vector we found in our route cache to destination, including this node's address
               */
              for (std::vector<Ipv4Address>::iterator j = ip.begin (); j != ip.end (); ++j)
                {
                  m_finalRoute.push_back (*j);
                }
              /*
               * Create the route entry to the rreq originator and save it to route cache, also need to reverse the route
               */
              bool addRoute = false;
              std::vector<Ipv4Address> reverseRoute (m_finalRoute);

              if (ReverseRoutes (reverseRoute))
                {
                  Ipv4Address dst = reverseRoute.back ();
                  saveRoute.push_back (ipv4Address);
                  ReverseRoutes (saveRoute);
                  PrintVector (saveRoute);
                  RouteCacheEntry toSource (/*IP_VECTOR=*/ saveRoute, /*dst=*/ dst, /*expire time=*/ ActiveRouteTimeout);
                  NS_ASSERT (saveRoute.front () == ipv4Address);
                  // Add the route entry in the route cache
                  if (m_routeCache->IsLinkCache ())
                    {
                      addRoute = m_routeCache->AddRoute_Link (saveRoute, ipv4Address);
                    }
                  else
                    {
                      addRoute = m_routeCache->AddRoute (toSource);
                    }
                  if (addRoute)
                    {
                      NS_LOG_DEBUG ("The route is failed to add in cache");
                      return 0;
                    }
                  else
                    {
                      NS_LOG_DEBUG ("We have added the route and search send buffer for packet with destination " << dst);
                      /*
                       * Found a route the dst, construct the source route option header
                       */
                      DsrOptionSRHeader sourceRoute;
                      NS_LOG_DEBUG ("The route length " << reverseRoute.size ());
                      sourceRoute.SetNodesAddress (reverseRoute);
                      if (m_routeCache->IsLinkCache ())
                        {
                          m_routeCache->UseExtends (reverseRoute);
                        }
                      sourceRoute.SetSegmentsLeft ((saveRoute.size () - 2));
                      uint8_t salvage = 0;
                      sourceRoute.SetSalvage (salvage);
                      Ipv4Address nextHop = SearchNextHop (ipv4Address, reverseRoute); // Get the next hop address
                      NS_LOG_DEBUG ("The nextHop address " << nextHop);

                      if (nextHop == "0.0.0.0")
                        {
                          dsr->PacketNewRoute (dsrP, ipv4Address, dst, protocol);
                          return 0;
                        }
                      SetRoute (nextHop, ipv4Address);
                      /*
                       * Schedule the packet retry
                       */
                      dsr->SendPacket (sourceRoute, nextHop, protocol);
                      // Cancel the route request timer for destination
                      dsr->CancelRreqTimer (dst);
                    }
                }
              else
                {
                  NS_LOG_DEBUG ("Unable to reverse the route");
                }

              /*
               * Need to first pin down the next hop address before removing duplicates
               */
              Ipv4Address nextHop = ReverseSearchNextHop (ipv4Address, m_finalRoute);
              /*
               * First remove the duplicate ip address to automatically shorten the route, and then reversely
               * search the next hop address
               */
              PrintVector (m_finalRoute);
              // Set the route
              SetRoute (nextHop, ipv4Address);

              uint16_t hops = m_finalRoute.size ();
              DsrOptionRrepHeader rrep;
              rrep.SetNodesAddress (m_finalRoute);     // Set the node addresses in the route reply header
              NS_LOG_DEBUG ("The nextHop address " << nextHop);
              // Get the real source of the reply
              Ipv4Address realSource = m_finalRoute.back ();
              /*
               * This part add dsr header to the packet and send reply
               */
              DsrRoutingHeader dsrRoutingHeader;
              dsrRoutingHeader.SetNextHeader (protocol);
              dsrRoutingHeader.SetMessageType (1);
              dsrRoutingHeader.SetSourceId (GetIDfromIP (realSource));
              dsrRoutingHeader.SetDestId (255);

              uint8_t length = rrep.GetLength ();  // Get the length of the rrep header excluding the type header
              dsrRoutingHeader.SetPayloadLength (length + 2);
              dsrRoutingHeader.AddDsrOption (rrep);
              Ptr<Packet> newPacket = Create<Packet> ();
              newPacket->AddHeader (dsrRoutingHeader);
              dsr->ScheduleCachedReply (newPacket, ipv4Address, nextHop, m_ipv4Route, hops);
              isPromisc = false;
            }
        }
      /*
       * (iii) no route in any type has been found
       */
      else
        {
          std::vector<Ipv4Address> nodeList = rreq.GetNodesAddresses (); // route from rreq header
          PrintVector (nodeList);
          /*
           * Otherwise, forward the route request packet
           */
          PrintVector (nodeList);
          nodeList.push_back (ipv4Address);
          PrintVector (nodeList);
          NS_ASSERT (nodeList.front () == source);
          rreq.SetNodesAddress (nodeList);
          /*
          * Decrease the TTL value in the packet tag by one, this tag will go to ip layer 3 send function
          * and drop packet when TTL value equals to 0
          */
          if (ttl)
            {
              Ptr<Packet> interP = Create<Packet> ();
              SocketIpTtlTag tag;
              tag.SetTtl (ttl - 1);
              interP->AddPacketTag (tag);
              /*
               * Construct the route request header to forward the route requests
               */
              DsrRoutingHeader dsrRoutingHeader;
              dsrRoutingHeader.SetNextHeader (protocol);
              dsrRoutingHeader.SetMessageType (1);
              dsrRoutingHeader.SetSourceId (GetIDfromIP (source));
              dsrRoutingHeader.SetDestId (255);
              dsrRoutingHeader.AddDsrOption (rreq);

              // if the errP packet is not 0, then there is an error header after it
              if (errP->GetSize ())
                {
                  NS_LOG_DEBUG ("Error header included");
                  DsrOptionRerrUnreachHeader rerr;
                  p->RemoveHeader (rerr);
                  Ipv4Address errorSrc = rerr.GetErrorSrc ();
                  Ipv4Address unreachNode = rerr.GetUnreachNode ();
                  isError = true;
                  m_routeCache->DeleteAllRoutesIncludeLink (errorSrc, unreachNode, ipv4Address);

                  DsrOptionRerrUnreachHeader newUnreach;
                  newUnreach.SetErrorType (1);
                  newUnreach.SetErrorSrc (rerr.GetErrorSrc ());
                  newUnreach.SetUnreachNode (rerr.GetUnreachNode ());
                  newUnreach.SetErrorDst (rerr.GetErrorDst ());
                  newUnreach.SetSalvage (rerr.GetSalvage ()); // Set the value about whether to salvage a packet or not
                  dsrRoutingHeader.AddDsrOption (newUnreach);
                  uint16_t length = rreq.GetLength () + newUnreach.GetLength ();
                  NS_LOG_DEBUG ("The RREQ and newUnreach header length " <<  length);
                  dsrRoutingHeader.SetPayloadLength (length + 4);
                  interP->AddHeader (dsrRoutingHeader);
                }
              else
                {
                  uint16_t length = rreq.GetLength ();
                  NS_LOG_DEBUG ("The RREQ header length " <<  length);
                  dsrRoutingHeader.SetPayloadLength (length + 2);
                  interP->AddHeader (dsrRoutingHeader);
                }

              dsr->ScheduleInterRequest (interP);
              isPromisc = false;
            }
        }
    }

  return rreq.GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionRrep);

TypeId DsrOptionRrep::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionRrep")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionRrep> ()
  ;
  return tid;
}

DsrOptionRrep::DsrOptionRrep ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionRrep::~DsrOptionRrep ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId DsrOptionRrep::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint8_t DsrOptionRrep::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();

  return OPT_NUMBER;
}

uint8_t DsrOptionRrep::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);

  Ptr<Packet> p = packet->Copy ();

  // Get the number of routers' address field
  uint8_t buf[2];
  p->CopyData (buf, sizeof(buf));
  uint8_t numberAddress = (buf[1] - 2) / 4;

  DsrOptionRrepHeader rrep;
  rrep.SetNumberAddress (numberAddress);  // Set the number of ip address in the header to reserver space for deserialize header
  p->RemoveHeader (rrep);

  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();
  Ptr<dsr::RouteCache> m_routeCache = node->GetObject<dsr::RouteCache> ();
  Ptr<dsr::RreqTable>  m_rreqTable = node->GetObject<dsr::RreqTable> ();
  ActiveRouteTimeout = m_routeCache->GetCacheTimeout ();

  NS_LOG_DEBUG ("The next header value " << (uint32_t)protocol);

  std::vector<Ipv4Address> nodeList = rrep.GetNodesAddress ();
  /*
   * Get the destination address, which is the last element in the nodeList
   */
  Ipv4Address targetAddress = nodeList.front ();
  // If the RREP option has reached to the destination
  if (targetAddress == ipv4Address)
    {
      RemoveDuplicates (nodeList); // This is for the route reply from intermediate node since we didn't remove
                                   // duplicate there
      if (nodeList.size () == 0)
        {
          NS_LOG_DEBUG ("The route we have contains 0 entries");
          return 0;
        }
      /*
       * Get the destination address for the data packet, which is the last element in the nodeList
       */
      Ipv4Address dst = nodeList.back ();
      /*
       * Add the newly found route to the route cache
       * The route looks like:
       * \\ "srcAddress" + "intermediate node address" + "targetAddress"
       */
      RouteCacheEntry toDestination (/*IP_VECTOR=*/ nodeList, /*dst=*/ dst, /*expire time=*/ ActiveRouteTimeout);
      NS_ASSERT (nodeList.front () == ipv4Address);
      bool addRoute = false;
      if (m_routeCache->IsLinkCache ())
        {
          m_routeCache->AddRoute_Link (nodeList, ipv4Address);
        }
      else
        {
          m_routeCache->AddRoute (toDestination);
        }
      if (addRoute)
        {
          NS_LOG_DEBUG ("Failed to add the route");
          return 0;
        }
      else
        {
          NS_LOG_DEBUG ("We have added the route and search send buffer for packet with destination " << dst);
          /*
           * Found a route the dst, construct the source route option header
           */
          DsrOptionSRHeader sourceRoute;
          NS_LOG_DEBUG ("The route length " << nodeList.size ());
          sourceRoute.SetNodesAddress (nodeList);
          sourceRoute.SetSegmentsLeft ((nodeList.size () - 2));
          uint8_t salvage = 0;
          sourceRoute.SetSalvage (salvage);
          Ipv4Address nextHop = SearchNextHop (ipv4Address, nodeList); // Get the next hop address
          NS_LOG_DEBUG ("The nextHop address " << nextHop);
          if (nextHop == "0.0.0.0")
            {
              dsr->PacketNewRoute (dsrP, ipv4Address, dst, protocol);
              return 0;
            }
          PrintVector (nodeList);
          SetRoute (nextHop, ipv4Address);
          /*
           * Schedule the packet retry
           */
          dsr->SendPacket (sourceRoute, nextHop, protocol);
          // Cancel the route request timer for destination
          dsr->CancelRreqTimer (dst);
        }
    }
  else
    {
      uint8_t length = rrep.GetLength () - 2; // The get length - 2 is to get aligned for the malformed header check
      NS_LOG_DEBUG ("The length of rrep option " << (uint32_t)length);

      if (length % 2 != 0)
        {
          NS_LOG_LOGIC ("Malformed header. Drop!");
          m_dropTrace (packet);
          return 0;
        }
      PrintVector (nodeList);
      /*
       * This node is only an intermediate node, but it needs to save the possible route to the destination when cutting the route
       */
      std::vector<Ipv4Address> routeCopy = nodeList;
      std::vector<Ipv4Address> cutRoute = CutRoute (ipv4Address, nodeList);
      PrintVector (cutRoute);
      if (cutRoute.size () >= 2)
        {
          Ipv4Address dst = cutRoute.back ();
          NS_LOG_DEBUG ("The route destination after cut " << dst);
          RouteCacheEntry toDestination (/*IP_VECTOR=*/ cutRoute, /*dst=*/ dst, /*expire time=*/ ActiveRouteTimeout);
          NS_ASSERT (cutRoute.front () == ipv4Address);
          bool addRoute = false;
          if (m_routeCache->IsLinkCache ())
            {
              m_routeCache->AddRoute_Link (nodeList, ipv4Address);
            }
          else
            {
              m_routeCache->AddRoute (toDestination);
            }
          if (addRoute)
            {
              dsr->CancelRreqTimer (dst);
            }
          else
            {
              NS_LOG_DEBUG ("The route not added");
            }
        }
      else
        {
          NS_LOG_DEBUG ("The route is corrupted");
        }
      /*
       * Reverse search the vector for next hop address
       */
      Ipv4Address nextHop = ReverseSearchNextHop (ipv4Address, routeCopy);
      NS_ASSERT (routeCopy.back () == source);
      PrintVector (routeCopy);
      NS_LOG_DEBUG ("The nextHop address " << nextHop << " and the source in the route reply " << source);
      /*
       * Set the route entry we will use to send reply
       */
      SetRoute (nextHop, ipv4Address);
      /*
       * This part add dsr routing header to the packet and send reply
       */
      DsrRoutingHeader dsrRoutingHeader;
      dsrRoutingHeader.SetNextHeader (protocol);

      length = rrep.GetLength ();    // Get the length of the rrep header excluding the type header
      NS_LOG_DEBUG ("The reply header length " << (uint32_t)length);
      dsrRoutingHeader.SetPayloadLength (length + 2);
      dsrRoutingHeader.SetMessageType (1);
      dsrRoutingHeader.SetSourceId (GetIDfromIP (source));
      dsrRoutingHeader.SetDestId (GetIDfromIP (targetAddress));
      dsrRoutingHeader.AddDsrOption (rrep);
      Ptr<Packet> newPacket = Create<Packet> ();
      newPacket->AddHeader (dsrRoutingHeader);
      dsr->SendReply (newPacket, ipv4Address, nextHop, m_ipv4Route);
      isPromisc = false;
    }
  return rrep.GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionSR);

TypeId DsrOptionSR::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionSR")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionSR> ()
  ;
  return tid;
}

DsrOptionSR::DsrOptionSR ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionSR::~DsrOptionSR ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId DsrOptionSR::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint8_t DsrOptionSR::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return OPT_NUMBER;
}

uint8_t DsrOptionSR::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Address << ipv4Header << (uint32_t)protocol << isPromisc);
  Ptr<Packet> p = packet->Copy ();
  // Get the number of routers' address field
  uint8_t buf[2];
  p->CopyData (buf, sizeof(buf));
  uint8_t numberAddress = (buf[1] - 2) / 4;
  DsrOptionSRHeader sourceRoute;
  sourceRoute.SetNumberAddress (numberAddress);
  p->RemoveHeader (sourceRoute);

  // The route size saved in the source route
  std::vector<Ipv4Address> nodeList = sourceRoute.GetNodesAddress ();

  uint8_t segsLeft = sourceRoute.GetSegmentsLeft ();
  uint8_t salvage = sourceRoute.GetSalvage ();

  // Here we remove the ack packet to the previous hop
  DsrOptionAckReqHeader ackReq;
  p->RemoveHeader (ackReq);
  uint16_t ackId = ackReq.GetAckId ();
  /*
   * Get the node from IP address and get the DSR extension object
   */
  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();
  /*
   * Get the source and destination address from ipv4 header
   */
  Ipv4Address srcAddress = ipv4Header.GetSource ();
  Ipv4Address destAddress = ipv4Header.GetDestination ();
  // Get the node list destination
  Ipv4Address destination = nodeList.back ();
  /*
   * If it's a promiscuous receive data packet,
   * 1. see if automatic route shortening possible or not
   * 2. see if it is a passive acknowledgment
   */
  if (isPromisc)
    {
      /*
       * Get the node from Ip address and get the dsr routing object
       */
      Ptr<Node> node = GetNodeWithAddress (ipv4Address);
      Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();

      bool findSame = dsr->FindSamePackets (packet, ipv4Header, source, destination, segsLeft);

      if (findSame)
        {
          NS_LOG_DEBUG ("Received one passive acknowledgment for data packet");
        }
      if (ContainAddressAfter (ipv4Address, destAddress, nodeList))
        {
          NS_LOG_DEBUG ("Send back the gratuitous reply");
          dsr->SendGratuitousReply (source, srcAddress, nodeList, protocol);
        }
      else
        {
          return 0;
        }
    }
  else
    {
      /*
       * Get the number of address from the source route header
       */
      uint8_t length = sourceRoute.GetLength ();
      uint8_t nextAddressIndex;
      Ipv4Address nextAddress;
      /*
       * Send back acknowledgment packet to the earlier hop
       */
      m_ipv4Route = SetRoute (srcAddress, ipv4Address);
      NS_LOG_DEBUG ("Send back ACK to the earlier hop " << srcAddress << " from us " << ipv4Address);
      dsr->SendAck (ackId, srcAddress, source, destination, protocol, m_ipv4Route);
      /*
       * After send back ACK, check if the segments left value has turned to 0 or not, if yes, update the route entry
       * and return header length
       */
      if (segsLeft == 0)
        {
          isPromisc = false;
          return sourceRoute.GetSerializedSize ();
        }

      if (length % 2 != 0)
        {
          NS_LOG_LOGIC ("Malformed header. Drop!");
          m_dropTrace (packet);
          return 0;
        }

      if (segsLeft > numberAddress) // The segmentsLeft field should not be larger than the total number of ip addresses
        {
          NS_LOG_LOGIC ("Malformed header. Drop!");
          m_dropTrace (packet);
          return 0;
        }

      DsrOptionSRHeader newSourceRoute;
      newSourceRoute.SetSegmentsLeft (segsLeft - 1);
      newSourceRoute.SetSalvage (salvage);
      newSourceRoute.SetNodesAddress (nodeList);
      nextAddressIndex = numberAddress - segsLeft;
      nextAddress = newSourceRoute.GetNodeAddress (nextAddressIndex);
      NS_LOG_DEBUG ("The next address of source route option " << nextAddress << " and the nextAddressIndex: " << (uint32_t)nextAddressIndex << " and the segments left : " << (uint32_t)segsLeft);
      /*
       * Get the target Address in the node list
       */
      Ipv4Address targetAddress = nodeList.back ();
      Ipv4Address realSource = nodeList.front ();
      /*
       * Search the vector for next hop address
       */
      Ipv4Address nextHop = SearchNextHop (ipv4Address, nodeList);
      PrintVector (nodeList);

      if (nextHop == "0.0.0.0")
        {
          NS_LOG_DEBUG ("Before new packet " << *dsrP);
          dsr->PacketNewRoute (dsrP, realSource, targetAddress, protocol);
          return 0;
        }

      if (ipv4Address == nextHop)
        {
          NS_LOG_DEBUG ("We have reached the destination");
          newSourceRoute.SetSegmentsLeft (0);
          return newSourceRoute.GetSerializedSize ();
        }
      // Verify the multicast address, leave it here for now
      if (nextAddress.IsMulticast () || destAddress.IsMulticast ())
        {
          m_dropTrace (packet);
          return 0;
        }
      // Set the route and forward the data packet
      SetRoute (nextAddress, ipv4Address);
      dsr->ForwardPacket (dsrP, newSourceRoute, ipv4Header, realSource, nextAddress, targetAddress, protocol, m_ipv4Route);
    }
  return sourceRoute.GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionRerr);

TypeId DsrOptionRerr::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionRerr")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionRerr> ()
  ;
  return tid;
}

DsrOptionRerr::DsrOptionRerr ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionRerr::~DsrOptionRerr ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId DsrOptionRerr::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint8_t DsrOptionRerr::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return OPT_NUMBER;
}

uint8_t DsrOptionRerr::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);

  Ptr<Packet> p = packet->Copy ();
  uint32_t size = p->GetSize ();
  uint8_t *data = new uint8_t[size];
  p->CopyData (data, size);
  uint8_t errorType = *(data + 2);
  /*
   * Get the node from Ip address and get the dsr extension object
   */
  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();

  Ipv4Address srcAddress = ipv4Header.GetSource ();
  Ipv4Address destAddress = ipv4Header.GetDestination ();
  /*
   * The error serialized size
   */
  uint32_t rerrSize;
  if (errorType == 1) // unreachable ip address
    {
      /*
       * Remove the route error header from the packet, and get the error type
       */
      DsrOptionRerrUnreachHeader rerrUnreach;
      p->RemoveHeader (rerrUnreach);
      /*
       * Get the error destination address
       */
      Ipv4Address targetAddress = rerrUnreach.GetErrorDst ();
      Ipv4Address unreachAddress = rerrUnreach.GetUnreachNode ();
      Ipv4Address errorSource = rerrUnreach.GetErrorSrc ();

      NS_LOG_DEBUG ("The destination address and the unreachable node " << targetAddress << " " << unreachAddress);
      /*
       * Get the serialized size of the rerr header
       */
      rerrSize = rerrUnreach.GetSerializedSize ();
      /*
       * Delete all the routes including the unreachable node address from the route cache
       */
      Ptr<Node> node = GetNodeWithAddress (ipv4Address);
      Ptr<dsr::RouteCache> m_routeCache = node->GetObject<dsr::RouteCache> ();
      m_routeCache->DeleteAllRoutesIncludeLink (errorSource, unreachAddress, ipv4Address);

      Ptr<Packet> newP = p->Copy ();
      uint32_t serialized = DoSendError (newP, rerrUnreach, rerrSize, ipv4Address, protocol);
      return serialized;
    }
  else
    {
      /*
       * Two other type of error headers:
       * 1. flow state not supported type-specific information
       * 2. unsupported option with option number
       */
      /*
       * Remove the route error header from the packet, and get the error type
       */
      DsrOptionRerrUnsupportHeader rerrUnsupport;
      p->RemoveHeader (rerrUnsupport);
      rerrSize = rerrUnsupport.GetSerializedSize ();

      // This is for the other two error options TODO
//      uint32_t serialized = DoSendError (p, rerrUnsupport, rerrSize, ipv4Address, protocol);
      uint32_t serialized = 0;
      return serialized;
    }
}

uint8_t DsrOptionRerr::DoSendError (Ptr<Packet> p, DsrOptionRerrUnreachHeader &rerr, uint32_t rerrSize, Ipv4Address ipv4Address, uint8_t protocol)
{
  // Get the number of routers' address field
  uint8_t buf[2];
  p->CopyData (buf, sizeof(buf));
  uint8_t numberAddress = (buf[1] - 2) / 4;

  // Here remove the source route header and schedule next hop error transmission
  NS_LOG_DEBUG ("The number of addresses " << (uint32_t)numberAddress);
  DsrOptionSRHeader sourceRoute;
  sourceRoute.SetNumberAddress (numberAddress);
  p->RemoveHeader (sourceRoute);
  NS_ASSERT (p->GetSize () == 0);
  /*
   * Get the node from ip address and the dsr extension object
   */
  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();
  /*
   * Get the segments left field and the next address
   */
  uint8_t segmentsLeft = sourceRoute.GetSegmentsLeft ();
  NS_LOG_DEBUG ("The segments left " << (uint32_t)segmentsLeft);
  uint8_t length = sourceRoute.GetLength ();
  NS_LOG_DEBUG ("The number of addresses we have " << (uint32_t)numberAddress);
  uint8_t nextAddressIndex;
  Ipv4Address nextAddress;
  /*
   * Get the route size and the error target address
   */
  std::vector<Ipv4Address> nodeList = sourceRoute.GetNodesAddress ();
  Ipv4Address targetAddress = nodeList.back ();
  /*
   * The total serialized size for both the rerr and source route headers
   */
  uint32_t serializedSize = rerrSize + sourceRoute.GetSerializedSize ();

  if (length % 2 != 0)
    {
      NS_LOG_LOGIC ("Malformed header. Drop!");
      m_dropTrace (p);
      return 0;
    }

  if (segmentsLeft > numberAddress)
    {
      NS_LOG_LOGIC ("Malformed header. Drop!");
      m_dropTrace (p);
      return 0;
    }
  /*
   * When the error packet has reached to the destination
   */
  if (segmentsLeft == 0 && targetAddress == ipv4Address)
    {
      NS_LOG_DEBUG ("This is the destination of the error");
      dsr->SendErrorRequest (rerr, protocol);
      return serializedSize;
    }

  DsrOptionRerrUnreachHeader newUnreach;
  newUnreach.SetErrorType (1);
  newUnreach.SetErrorSrc (rerr.GetErrorSrc ());
  newUnreach.SetUnreachNode (rerr.GetUnreachNode ());
  newUnreach.SetErrorDst (rerr.GetErrorDst ());
  newUnreach.SetSalvage (rerr.GetSalvage ()); // Set the value about whether to salvage a packet or not

  // Get the next Router Address
  DsrOptionSRHeader newSourceRoute;
  newSourceRoute.SetSegmentsLeft (segmentsLeft - 1);
  nextAddressIndex = numberAddress - segmentsLeft;
  nextAddress = sourceRoute.GetNodeAddress (nextAddressIndex);
  newSourceRoute.SetSalvage (sourceRoute.GetSalvage ());
  newSourceRoute.SetNodesAddress (nodeList);
  nextAddress = newSourceRoute.GetNodeAddress (nextAddressIndex);

  // / to test if the next address is multicast or not
  if (nextAddress.IsMulticast () || targetAddress.IsMulticast ())
    {
      m_dropTrace (p);
      return serializedSize;
    }

  // Set the route entry
  SetRoute (nextAddress, ipv4Address);
  dsr->ForwardErrPacket (newUnreach, newSourceRoute, nextAddress, protocol, m_ipv4Route);
  return serializedSize;
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionAckReq);

TypeId DsrOptionAckReq::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionAckReq")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionAckReq> ()
  ;
  return tid;
}

DsrOptionAckReq::DsrOptionAckReq ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionAckReq::~DsrOptionAckReq ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId DsrOptionAckReq::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint8_t DsrOptionAckReq::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return OPT_NUMBER;
}

uint8_t DsrOptionAckReq::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);
  /*
   * Current implementation of the ack request header processing is coded in source route header processing
   */
  /*
   * Remove the ack request header
   */
  Ptr<Packet> p = packet->Copy ();
  DsrOptionAckReqHeader ackReq;
  p->RemoveHeader (ackReq);
  /*
   * Get the node with ip address and get the dsr extension and reoute cache objects
   */
  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();
  Ptr<dsr::RouteCache> m_routeCache = node->GetObject<dsr::RouteCache> ();

  NS_LOG_DEBUG ("The next header value " << (uint32_t)protocol);

  Ipv4Address srcAddress = ipv4Header.GetSource ();
  SetRoute (srcAddress, ipv4Address);

  return ackReq.GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (DsrOptionAck);

TypeId DsrOptionAck::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::DsrOptionAck")
    .SetParent<DsrOptions> ()
    .AddConstructor<DsrOptionAck> ()
  ;
  return tid;
}

DsrOptionAck::DsrOptionAck ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

DsrOptionAck::~DsrOptionAck ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId DsrOptionAck::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint8_t DsrOptionAck::GetOptionNumber () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return OPT_NUMBER;
}

uint8_t DsrOptionAck::Process (Ptr<Packet> packet, Ptr<Packet> dsrP, Ipv4Address ipv4Address, Ipv4Address source, Ipv4Header const& ipv4Header, uint8_t protocol, bool& isPromisc)
{
  NS_LOG_FUNCTION (this << packet << dsrP << ipv4Address << source << ipv4Header << (uint32_t)protocol << isPromisc);
  /*
   * Remove the ACK header
   */
  Ptr<Packet> p = packet->Copy ();
  DsrOptionAckHeader ack;
  p->RemoveHeader (ack);
  /*
   * Get the ACK source and destination address
   */
  Ipv4Address realSrc = ack.GetRealSrc ();
  Ipv4Address realDst = ack.GetRealDst ();
  uint16_t ackId = ack.GetAckId ();
  /*
   * Get the node with ip address and get the dsr extension and route cache objects
   */
  Ptr<Node> node = GetNodeWithAddress (ipv4Address);
  Ptr<dsr::DsrRouting> dsr = node->GetObject<dsr::DsrRouting> ();
  Ptr<dsr::RouteCache> srcRouteCache = node->GetObject<dsr::RouteCache> ();
  srcRouteCache->UpdateRouteEntry (realDst);
  /*
   * Cancel the packet retransmit timer when receiving the ack packet
   */
  dsr->CallCancelPacketTimer (ackId, ipv4Header, realSrc, realDst);
  return ack.GetSerializedSize ();
}

} // namespace dsr
} // namespace ns3
