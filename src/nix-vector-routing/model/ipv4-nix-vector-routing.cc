/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Georgia Institute of Technology 
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
 * Authors: Josh Pelkey <jpelkey@gatech.edu>
 */

#include <queue>
#include <iomanip>

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/names.h"
#include "ns3/ipv4-list-routing.h"

#include "ipv4-nix-vector-routing.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv4NixVectorRouting");

NS_OBJECT_ENSURE_REGISTERED (Ipv4NixVectorRouting);

bool Ipv4NixVectorRouting::g_isCacheDirty = false;

TypeId 
Ipv4NixVectorRouting::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4NixVectorRouting")
    .SetParent<Ipv4RoutingProtocol> ()
    .SetGroupName ("NixVectorRouting")
    .AddConstructor<Ipv4NixVectorRouting> ()
  ;
  return tid;
}

Ipv4NixVectorRouting::Ipv4NixVectorRouting ()
  : m_totalNeighbors (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv4NixVectorRouting::~Ipv4NixVectorRouting ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Ipv4NixVectorRouting::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_ASSERT (ipv4 != 0);
  NS_ASSERT (m_ipv4 == 0);
  NS_LOG_DEBUG ("Created Ipv4NixVectorProtocol");

  m_ipv4 = ipv4;
}

void 
Ipv4NixVectorRouting::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_node = 0;
  m_ipv4 = 0;

  Ipv4RoutingProtocol::DoDispose ();
}


void
Ipv4NixVectorRouting::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_node = node;
}

void
Ipv4NixVectorRouting::FlushGlobalNixRoutingCache (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NodeList::Iterator listEnd = NodeList::End ();
  for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4NixVectorRouting> rp = node->GetObject<Ipv4NixVectorRouting> ();
      if (!rp)
        {
          continue;
        }
      NS_LOG_LOGIC ("Flushing Nix caches.");
      rp->FlushNixCache ();
      rp->FlushIpv4RouteCache ();
    }
}

void
Ipv4NixVectorRouting::FlushNixCache (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  m_nixCache.clear ();
}

void
Ipv4NixVectorRouting::FlushIpv4RouteCache (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ipv4RouteCache.clear ();
}

Ptr<NixVector>
Ipv4NixVectorRouting::GetNixVector (Ptr<Node> source, Ipv4Address dest, Ptr<NetDevice> oif)
{
  NS_LOG_FUNCTION_NOARGS ();

  Ptr<NixVector> nixVector = Create<NixVector> ();

  // not in cache, must build the nix vector
  // First, we have to figure out the nodes 
  // associated with these IPs
  Ptr<Node> destNode = GetNodeByIp (dest);
  if (destNode == 0)
    {
      NS_LOG_ERROR ("No routing path exists");
      return 0;
    }

  // if source == dest, then we have a special case
  /// \internal
  /// Do not process packets to self (see \bugid{1308})
  if (source == destNode)
    {
      NS_LOG_DEBUG ("Do not processs packets to self");
      return 0;
    }
  else
    {
      // otherwise proceed as normal 
      // and build the nix vector
      std::vector< Ptr<Node> > parentVector;

      BFS (NodeList::GetNNodes (), source, destNode, parentVector, oif);

      if (BuildNixVector (parentVector, source->GetId (), destNode->GetId (), nixVector))
        {
          return nixVector;
        }
      else
        {
          NS_LOG_ERROR ("No routing path exists");
          return 0;
        }
    }
}

Ptr<NixVector>
Ipv4NixVectorRouting::GetNixVectorInCache (Ipv4Address address)
{
  NS_LOG_FUNCTION_NOARGS ();

  CheckCacheStateAndFlush ();

  NixMap_t::iterator iter = m_nixCache.find (address);
  if (iter != m_nixCache.end ())
    {
      NS_LOG_LOGIC ("Found Nix-vector in cache.");
      return iter->second;
    }

  // not in cache
  return 0;
}

Ptr<Ipv4Route>
Ipv4NixVectorRouting::GetIpv4RouteInCache (Ipv4Address address)
{
  NS_LOG_FUNCTION_NOARGS ();

  CheckCacheStateAndFlush ();

  Ipv4RouteMap_t::iterator iter = m_ipv4RouteCache.find (address);
  if (iter != m_ipv4RouteCache.end ())
    {
      NS_LOG_LOGIC ("Found Ipv4Route in cache.");
      return iter->second;
    }

  // not in cache
  return 0;
}

bool
Ipv4NixVectorRouting::BuildNixVectorLocal (Ptr<NixVector> nixVector)
{
  NS_LOG_FUNCTION_NOARGS ();

  uint32_t numberOfDevices = m_node->GetNDevices ();

  // here we are building a nix vector to 
  // ourself, so we need to find the loopback 
  // interface and add that to the nix vector
  Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < numberOfDevices; i++)
    {
      uint32_t interfaceIndex = (m_ipv4)->GetInterfaceForDevice (m_node->GetDevice (i));
      Ipv4InterfaceAddress ifAddr = m_ipv4->GetAddress (interfaceIndex, 0);
      if (ifAddr.GetLocal () == loopback)
        {
          NS_LOG_LOGIC ("Adding loopback to nix.");
          NS_LOG_LOGIC ("Adding Nix: " << i << " with " << nixVector->BitCount (numberOfDevices) 
                                       << " bits, for node " << m_node->GetId ());
          nixVector->AddNeighborIndex (i, nixVector->BitCount (numberOfDevices));
          return true;
        }
    }
  return false;
}

bool
Ipv4NixVectorRouting::BuildNixVector (const std::vector< Ptr<Node> > & parentVector, uint32_t source, uint32_t dest, Ptr<NixVector> nixVector)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (source == dest)
    {
      return true;
    }

  if (parentVector.at (dest) == 0)
    {
      return false;
    }

  Ptr<Node> parentNode = parentVector.at (dest);

  uint32_t numberOfDevices = parentNode->GetNDevices ();
  uint32_t destId = 0;
  uint32_t totalNeighbors = 0;

  // scan through the net devices on the parent node
  // and then look at the nodes adjacent to them
  for (uint32_t i = 0; i < numberOfDevices; i++)
    {
      // Get a net device from the node
      // as well as the channel, and figure
      // out the adjacent net devices
      Ptr<NetDevice> localNetDevice = parentNode->GetDevice (i);
      if (localNetDevice->IsBridge ())
        {
          continue;
        }
      Ptr<Channel> channel = localNetDevice->GetChannel ();
      if (channel == 0)
        {
          continue;
        }

      // this function takes in the local net dev, and channnel, and
      // writes to the netDeviceContainer the adjacent net devs
      NetDeviceContainer netDeviceContainer;
      GetAdjacentNetDevices (localNetDevice, channel, netDeviceContainer);

      // Finally we can get the adjacent nodes
      // and scan through them.  If we find the 
      // node that matches "dest" then we can add 
      // the index  to the nix vector.
      // the index corresponds to the neighbor index
      uint32_t offset = 0;
      for (NetDeviceContainer::Iterator iter = netDeviceContainer.Begin (); iter != netDeviceContainer.End (); iter++)
        {
          Ptr<Node> remoteNode = (*iter)->GetNode ();

          if (remoteNode->GetId () == dest)
            {
              destId = totalNeighbors + offset;
            }
          offset += 1;
        }

      totalNeighbors += netDeviceContainer.GetN ();
    }
  NS_LOG_LOGIC ("Adding Nix: " << destId << " with " 
                               << nixVector->BitCount (totalNeighbors) << " bits, for node " << parentNode->GetId ());
  nixVector->AddNeighborIndex (destId, nixVector->BitCount (totalNeighbors));

  // recurse through parent vector, grabbing the path 
  // and building the nix vector
  BuildNixVector (parentVector, source, (parentVector.at (dest))->GetId (), nixVector);
  return true;
}

void
Ipv4NixVectorRouting::GetAdjacentNetDevices (Ptr<NetDevice> netDevice, Ptr<Channel> channel, NetDeviceContainer & netDeviceContainer)
{
  NS_LOG_FUNCTION_NOARGS ();

  for (uint32_t i = 0; i < channel->GetNDevices (); i++)
    {
      Ptr<NetDevice> remoteDevice = channel->GetDevice (i);
      if (remoteDevice != netDevice)
        {
          Ptr<BridgeNetDevice> bd = NetDeviceIsBridged (remoteDevice);
          // we have a bridged device, we need to add all 
          // bridged devices
          if (bd)
            {
              NS_LOG_LOGIC ("Looking through bridge ports of bridge net device " << bd);
              for (uint32_t j = 0; j < bd->GetNBridgePorts (); ++j)
                {
                  Ptr<NetDevice> ndBridged = bd->GetBridgePort (j);
                  if (ndBridged == remoteDevice)
                    {
                      NS_LOG_LOGIC ("That bridge port is me, don't walk backward");
                      continue;
                    }
                  Ptr<Channel> chBridged = ndBridged->GetChannel ();
                  if (chBridged == 0)
                    {
                      continue;
                    }
                  GetAdjacentNetDevices (ndBridged, chBridged, netDeviceContainer);
                }
            }
          else
            {
              netDeviceContainer.Add (channel->GetDevice (i));
            }
        }
    }
}

Ptr<Node>
Ipv4NixVectorRouting::GetNodeByIp (Ipv4Address dest)
{ 
  NS_LOG_FUNCTION_NOARGS ();

  NodeContainer allNodes = NodeContainer::GetGlobal ();
  Ptr<Node> destNode;

  for (NodeContainer::Iterator i = allNodes.Begin (); i != allNodes.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4->GetInterfaceForAddress (dest) != -1)
        {
          destNode = node;
          break;
        }
    }

  if (!destNode)
    {
      NS_LOG_ERROR ("Couldn't find dest node given the IP" << dest);
      return 0;
    }

  return destNode;
}

uint32_t
Ipv4NixVectorRouting::FindTotalNeighbors (void)
{
  uint32_t numberOfDevices = m_node->GetNDevices ();
  uint32_t totalNeighbors = 0;

  // scan through the net devices on the parent node
  // and then look at the nodes adjacent to them
  for (uint32_t i = 0; i < numberOfDevices; i++)
    {
      // Get a net device from the node
      // as well as the channel, and figure
      // out the adjacent net devices
      Ptr<NetDevice> localNetDevice = m_node->GetDevice (i);
      Ptr<Channel> channel = localNetDevice->GetChannel ();
      if (channel == 0)
        {
          continue;
        }

      // this function takes in the local net dev, and channnel, and
      // writes to the netDeviceContainer the adjacent net devs
      NetDeviceContainer netDeviceContainer;
      GetAdjacentNetDevices (localNetDevice, channel, netDeviceContainer);

      totalNeighbors += netDeviceContainer.GetN ();
    }

  return totalNeighbors;
}

Ptr<BridgeNetDevice>
Ipv4NixVectorRouting::NetDeviceIsBridged (Ptr<NetDevice> nd) const
{
  NS_LOG_FUNCTION (nd);

  Ptr<Node> node = nd->GetNode ();
  uint32_t nDevices = node->GetNDevices ();

  //
  // There is no bit on a net device that says it is being bridged, so we have
  // to look for bridges on the node to which the device is attached.  If we
  // find a bridge, we need to look through its bridge ports (the devices it
  // bridges) to see if we find the device in question.
  //
  for (uint32_t i = 0; i < nDevices; ++i)
    {
      Ptr<NetDevice> ndTest = node->GetDevice (i);
      NS_LOG_LOGIC ("Examine device " << i << " " << ndTest);

      if (ndTest->IsBridge ())
        {
          NS_LOG_LOGIC ("device " << i << " is a bridge net device");
          Ptr<BridgeNetDevice> bnd = ndTest->GetObject<BridgeNetDevice> ();
          NS_ABORT_MSG_UNLESS (bnd, "Ipv4NixVectorRouting::NetDeviceIsBridged (): GetObject for <BridgeNetDevice> failed");

          for (uint32_t j = 0; j < bnd->GetNBridgePorts (); ++j)
            {
              NS_LOG_LOGIC ("Examine bridge port " << j << " " << bnd->GetBridgePort (j));
              if (bnd->GetBridgePort (j) == nd)
                {
                  NS_LOG_LOGIC ("Net device " << nd << " is bridged by " << bnd);
                  return bnd;
                }
            }
        }
    }
  NS_LOG_LOGIC ("Net device " << nd << " is not bridged");
  return 0;
}

uint32_t
Ipv4NixVectorRouting::FindNetDeviceForNixIndex (uint32_t nodeIndex, Ipv4Address & gatewayIp)
{
  uint32_t numberOfDevices = m_node->GetNDevices ();
  uint32_t index = 0;
  uint32_t totalNeighbors = 0;

  // scan through the net devices on the parent node
  // and then look at the nodes adjacent to them
  for (uint32_t i = 0; i < numberOfDevices; i++)
    {
      // Get a net device from the node
      // as well as the channel, and figure
      // out the adjacent net devices
      Ptr<NetDevice> localNetDevice = m_node->GetDevice (i);
      Ptr<Channel> channel = localNetDevice->GetChannel ();
      if (channel == 0)
        {
          continue;
        }

      // this function takes in the local net dev, and channnel, and
      // writes to the netDeviceContainer the adjacent net devs
      NetDeviceContainer netDeviceContainer;
      GetAdjacentNetDevices (localNetDevice, channel, netDeviceContainer);

      // check how many neighbors we have
      if (nodeIndex < (totalNeighbors + netDeviceContainer.GetN ()))
        {
          // found the proper net device
          index = i;
          Ptr<NetDevice> gatewayDevice = netDeviceContainer.Get (nodeIndex-totalNeighbors);
          Ptr<Node> gatewayNode = gatewayDevice->GetNode ();
          Ptr<Ipv4> ipv4 = gatewayNode->GetObject<Ipv4> ();

          uint32_t interfaceIndex = (ipv4)->GetInterfaceForDevice (gatewayDevice);
          Ipv4InterfaceAddress ifAddr = ipv4->GetAddress (interfaceIndex, 0);
          gatewayIp = ifAddr.GetLocal ();
          break;
        }
      totalNeighbors += netDeviceContainer.GetN ();
    }

  return index;
}

Ptr<Ipv4Route> 
Ipv4NixVectorRouting::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Ipv4Route> rtentry;
  Ptr<NixVector> nixVectorInCache;
  Ptr<NixVector> nixVectorForPacket;

  CheckCacheStateAndFlush ();

  NS_LOG_DEBUG ("Dest IP from header: " << header.GetDestination ());
  // check if cache
  nixVectorInCache = GetNixVectorInCache (header.GetDestination ());

  // not in cache
  if (!nixVectorInCache)
    {
      NS_LOG_LOGIC ("Nix-vector not in cache, build: ");
      // Build the nix-vector, given this node and the
      // dest IP address
      nixVectorInCache = GetNixVector (m_node, header.GetDestination (), oif);

      // cache it
      m_nixCache.insert (NixMap_t::value_type (header.GetDestination (), nixVectorInCache));
    }

  // path exists
  if (nixVectorInCache)
    {
      NS_LOG_LOGIC ("Nix-vector contents: " << *nixVectorInCache);

      // create a new nix vector to be used, 
      // we want to keep the cached version clean
      nixVectorForPacket = Create<NixVector> ();
      nixVectorForPacket = nixVectorInCache->Copy (); 

      // Get the interface number that we go out of, by extracting
      // from the nix-vector
      if (m_totalNeighbors == 0)
        {
          m_totalNeighbors = FindTotalNeighbors ();
        }

      // Get the interface number that we go out of, by extracting
      // from the nix-vector
      uint32_t numberOfBits = nixVectorForPacket->BitCount (m_totalNeighbors);
      uint32_t nodeIndex = nixVectorForPacket->ExtractNeighborIndex (numberOfBits);

      // Search here in a cache for this node index 
      // and look for a Ipv4Route
      rtentry = GetIpv4RouteInCache (header.GetDestination ());

      if (!rtentry || !(rtentry->GetOutputDevice () == oif))
        {
          // not in cache or a different specified output
          // device is to be used

          // first, make sure we erase existing (incorrect)
          // rtentry from the map
          if (rtentry)
            {
              m_ipv4RouteCache.erase (header.GetDestination ());
            }

          NS_LOG_LOGIC ("Ipv4Route not in cache, build: ");
          Ipv4Address gatewayIp;
          uint32_t index = FindNetDeviceForNixIndex (nodeIndex, gatewayIp);
          int32_t interfaceIndex = 0;

          if (!oif)
            {
              interfaceIndex = (m_ipv4)->GetInterfaceForDevice (m_node->GetDevice (index));
            }
          else
            {
              interfaceIndex = (m_ipv4)->GetInterfaceForDevice (oif);
            }

          NS_ASSERT_MSG (interfaceIndex != -1, "Interface index not found for device");

          Ipv4InterfaceAddress ifAddr = m_ipv4->GetAddress (interfaceIndex, 0);

          // start filling in the Ipv4Route info
          rtentry = Create<Ipv4Route> ();
          rtentry->SetSource (ifAddr.GetLocal ());

          rtentry->SetGateway (gatewayIp);
          rtentry->SetDestination (header.GetDestination ());

          if (!oif)
            {
              rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIndex));
            }
          else
            {
              rtentry->SetOutputDevice (oif);
            }

          sockerr = Socket::ERROR_NOTERROR;

          // add rtentry to cache
          m_ipv4RouteCache.insert (Ipv4RouteMap_t::value_type (header.GetDestination (), rtentry));
        }

      NS_LOG_LOGIC ("Nix-vector contents: " << *nixVectorInCache << " : Remaining bits: " << nixVectorForPacket->GetRemainingBits ());

      // Add  nix-vector in the packet class 
      // make sure the packet exists first
      if (p)
        {
          NS_LOG_LOGIC ("Adding Nix-vector to packet: " << *nixVectorForPacket);
          p->SetNixVector (nixVectorForPacket);
        }
    }
  else // path doesn't exist
    {
      NS_LOG_ERROR ("No path to the dest: " << header.GetDestination ());
      sockerr = Socket::ERROR_NOROUTETOHOST;
    }

  return rtentry;
}

bool 
Ipv4NixVectorRouting::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                                  UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                  LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION_NOARGS ();

  CheckCacheStateAndFlush ();

  Ptr<Ipv4Route> rtentry;

  // Get the nix-vector from the packet
  Ptr<NixVector> nixVector = p->GetNixVector ();

  // If nixVector isn't in packet, something went wrong
  NS_ASSERT (nixVector);

  // Get the interface number that we go out of, by extracting
  // from the nix-vector
  if (m_totalNeighbors == 0)
    {
      m_totalNeighbors = FindTotalNeighbors ();
    }
  uint32_t numberOfBits = nixVector->BitCount (m_totalNeighbors);
  uint32_t nodeIndex = nixVector->ExtractNeighborIndex (numberOfBits);

  rtentry = GetIpv4RouteInCache (header.GetDestination ());
  // not in cache
  if (!rtentry)
    {
      NS_LOG_LOGIC ("Ipv4Route not in cache, build: ");
      Ipv4Address gatewayIp;
      uint32_t index = FindNetDeviceForNixIndex (nodeIndex, gatewayIp);
      uint32_t interfaceIndex = (m_ipv4)->GetInterfaceForDevice (m_node->GetDevice (index));
      Ipv4InterfaceAddress ifAddr = m_ipv4->GetAddress (interfaceIndex, 0);

      // start filling in the Ipv4Route info
      rtentry = Create<Ipv4Route> ();
      rtentry->SetSource (ifAddr.GetLocal ());

      rtentry->SetGateway (gatewayIp);
      rtentry->SetDestination (header.GetDestination ());
      rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIndex));

      // add rtentry to cache
      m_ipv4RouteCache.insert (Ipv4RouteMap_t::value_type (header.GetDestination (), rtentry));
    }

  NS_LOG_LOGIC ("At Node " << m_node->GetId () << ", Extracting " << numberOfBits <<
                " bits from Nix-vector: " << nixVector << " : " << *nixVector);

  // call the unicast callback
  // local deliver is handled by Ipv4StaticRoutingImpl
  // so this code is never even called if the packet is
  // destined for this node.
  ucb (rtentry, p, header);

  return true;
}

void
Ipv4NixVectorRouting::PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const
{

  CheckCacheStateAndFlush ();

  std::ostream* os = stream->GetStream ();
  *os << "NixCache:" << std::endl;
  if (m_nixCache.size () > 0)
    {
      *os << "Destination     NixVector" << std::endl;
      for (NixMap_t::const_iterator it = m_nixCache.begin (); it != m_nixCache.end (); it++)
        {
          std::ostringstream dest;
          dest << it->first;
          *os << std::setiosflags (std::ios::left) << std::setw (16) << dest.str ();
          *os << *(it->second) << std::endl;
        }
    }
  *os << "Ipv4RouteCache:" << std::endl;
  if (m_ipv4RouteCache.size () > 0)
    {
      *os << "Destination     Gateway         Source            OutputDevice" << std::endl;
      for (Ipv4RouteMap_t::const_iterator it = m_ipv4RouteCache.begin (); it != m_ipv4RouteCache.end (); it++)
        {
          std::ostringstream dest, gw, src;
          dest << it->second->GetDestination ();
          *os << std::setiosflags (std::ios::left) << std::setw (16) << dest.str ();
          gw << it->second->GetGateway ();
          *os << std::setiosflags (std::ios::left) << std::setw (16) << gw.str ();
          src << it->second->GetSource ();
          *os << std::setiosflags (std::ios::left) << std::setw (16) << src.str ();
          *os << "  ";
          if (Names::FindName (it->second->GetOutputDevice ()) != "")
            {
              *os << Names::FindName (it->second->GetOutputDevice ());
            }
          else
            {
              *os << it->second->GetOutputDevice ()->GetIfIndex ();
            }
          *os << std::endl;
        }
    }
}

// virtual functions from Ipv4RoutingProtocol 
void
Ipv4NixVectorRouting::NotifyInterfaceUp (uint32_t i)
{
  g_isCacheDirty = true;
}
void
Ipv4NixVectorRouting::NotifyInterfaceDown (uint32_t i)
{
  g_isCacheDirty = true;
}
void
Ipv4NixVectorRouting::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  g_isCacheDirty = true;
}
void
Ipv4NixVectorRouting::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  g_isCacheDirty = true;
}

bool
Ipv4NixVectorRouting::BFS (uint32_t numberOfNodes, Ptr<Node> source, 
                           Ptr<Node> dest, std::vector< Ptr<Node> > & parentVector,
                           Ptr<NetDevice> oif)
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_LOG_LOGIC ("Going from Node " << source->GetId () << " to Node " << dest->GetId ());
  std::queue< Ptr<Node> > greyNodeList;  // discovered nodes with unexplored children

  // reset the parent vector
  parentVector.clear ();
  parentVector.reserve (sizeof (Ptr<Node>)*numberOfNodes);
  parentVector.insert (parentVector.begin (), sizeof (Ptr<Node>)*numberOfNodes, 0); // initialize to 0

  // Add the source node to the queue, set its parent to itself 
  greyNodeList.push (source);
  parentVector.at (source->GetId ()) = source;

  // BFS loop
  while (greyNodeList.size () != 0)
    {
      Ptr<Node> currNode = greyNodeList.front ();
      Ptr<Ipv4> ipv4 = currNode->GetObject<Ipv4> ();
 
      if (currNode == dest) 
        {
          NS_LOG_LOGIC ("Made it to Node " << currNode->GetId ());
          return true;
        }

      // if this is the first iteration of the loop and a 
      // specific output interface was given, make sure 
      // we go this way
      if (currNode == source && oif)
        {
          // make sure that we can go this way
          if (ipv4)
            {
              uint32_t interfaceIndex = (ipv4)->GetInterfaceForDevice (oif);
              if (!(ipv4->IsUp (interfaceIndex)))
                {
                  NS_LOG_LOGIC ("Ipv4Interface is down");
                  return false;
                }
            }
          if (!(oif->IsLinkUp ()))
            {
              NS_LOG_LOGIC ("Link is down.");
              return false;
            }
          Ptr<Channel> channel = oif->GetChannel ();
          if (channel == 0)
            { 
              return false;
            }

          // this function takes in the local net dev, and channnel, and
          // writes to the netDeviceContainer the adjacent net devs
          NetDeviceContainer netDeviceContainer;
          GetAdjacentNetDevices (oif, channel, netDeviceContainer);

          // Finally we can get the adjacent nodes
          // and scan through them.  We push them
          // to the greyNode queue, if they aren't 
          // already there.
          for (NetDeviceContainer::Iterator iter = netDeviceContainer.Begin (); iter != netDeviceContainer.End (); iter++)
            {
              Ptr<Node> remoteNode = (*iter)->GetNode ();

              // check to see if this node has been pushed before
              // by checking to see if it has a parent
              // if it doesn't (null or 0), then set its parent and 
              // push to the queue
              if (parentVector.at (remoteNode->GetId ()) == 0)
                {
                  parentVector.at (remoteNode->GetId ()) = currNode;
                  greyNodeList.push (remoteNode);
                }
            }
        }
      else
        {
          // Iterate over the current node's adjacent vertices
          // and push them into the queue
          for (uint32_t i = 0; i < (currNode->GetNDevices ()); i++)
            {
              // Get a net device from the node
              // as well as the channel, and figure
              // out the adjacent net device
              Ptr<NetDevice> localNetDevice = currNode->GetDevice (i);

              // make sure that we can go this way
              if (ipv4)
                {
                  uint32_t interfaceIndex = (ipv4)->GetInterfaceForDevice (currNode->GetDevice (i));
                  if (!(ipv4->IsUp (interfaceIndex)))
                    {
                      NS_LOG_LOGIC ("Ipv4Interface is down");
                      continue;
                    }
                }
              if (!(localNetDevice->IsLinkUp ()))
                {
                  NS_LOG_LOGIC ("Link is down.");
                  continue;
                }
              Ptr<Channel> channel = localNetDevice->GetChannel ();
              if (channel == 0)
                { 
                  continue;
                }

              // this function takes in the local net dev, and channnel, and
              // writes to the netDeviceContainer the adjacent net devs
              NetDeviceContainer netDeviceContainer;
              GetAdjacentNetDevices (localNetDevice, channel, netDeviceContainer);

              // Finally we can get the adjacent nodes
              // and scan through them.  We push them
              // to the greyNode queue, if they aren't 
              // already there.
              for (NetDeviceContainer::Iterator iter = netDeviceContainer.Begin (); iter != netDeviceContainer.End (); iter++)
                {
                  Ptr<Node> remoteNode = (*iter)->GetNode ();

                  // check to see if this node has been pushed before
                  // by checking to see if it has a parent
                  // if it doesn't (null or 0), then set its parent and 
                  // push to the queue
                  if (parentVector.at (remoteNode->GetId ()) == 0)
                    {
                      parentVector.at (remoteNode->GetId ()) = currNode;
                      greyNodeList.push (remoteNode);
                    }
                }
            }
        }

      // Pop off the head grey node.  We have all its children.
      // It is now black.
      greyNodeList.pop ();
    }

  // Didn't find the dest...
  return false;
}

void 
Ipv4NixVectorRouting::CheckCacheStateAndFlush (void) const
{
  if (g_isCacheDirty)
    {
      FlushGlobalNixRoutingCache ();
      g_isCacheDirty = false;
    }
}

} // namespace ns3
