/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/debug.h"
#include "ns3/assert.h"
#include "ns3/channel.h"
#include "ns3/net-device.h"
#include "ns3/internet-node.h"
#include "ns3/ipv4.h"
#include "static-router.h"

NS_DEBUG_COMPONENT_DEFINE ("StaticRouter");

namespace ns3 {

StaticRouterLSA::StaticRouterLSA()
  : 
  m_linkStateId("0.0.0.0"),
  m_advertisingRtr("0.0.0.0"),
  m_linkRecords()
{
  NS_DEBUG("StaticRouterLSA::StaticRouterLSA ()");
}

StaticRouterLSA::~StaticRouterLSA()
{
  NS_DEBUG("StaticRouterLSA::~StaticRouterLSA ()");

  for ( ListOfLinkRecords_t::iterator i = m_linkRecords.begin ();
        i != m_linkRecords.end (); 
        i++)
    {
      NS_DEBUG("StaticRouterLSA::~StaticRouterLSA ():  free link record");

      StaticRouterLinkRecord *p = *i;
      delete p;
      p = 0;

      *i = 0;
    }
  NS_DEBUG("StaticRouterLSA::~StaticRouterLSA ():  clear list");
  m_linkRecords.clear();
}

  uint32_t
StaticRouterLSA::AddLinkRecord (StaticRouterLinkRecord* lr)
{
  m_linkRecords.push_back (lr);
  return m_linkRecords.size ();
}

const InterfaceId StaticRouter::iid = 
  MakeInterfaceId ("StaticRouter", Object::iid);

StaticRouter::StaticRouter (Ptr<Node> node)
  : m_node(node), m_numLSAs(0)
{
  NS_DEBUG("StaticRouter::StaticRouter ()");
  SetInterfaceId (StaticRouter::iid);
}

StaticRouter::~StaticRouter ()
{
  NS_DEBUG("StaticRouter::~StaticRouter ()");
}

//
// Return the number of Link State Advertisements this node has to advertise.
// 
  uint32_t 
StaticRouter::GetNumLSAs (void)
{
  NS_DEBUG("StaticRouter::GetNumLSAs ()");
  NS_ASSERT_MSG(m_node, "<Node> interface not set");
//
// We're aggregated to a node.  We need to ask the node for a pointer to its
// Ipv4 interface.  This is where the information regarding the attached 
// interfaces lives.
//
  Ptr<Ipv4> ipv4Local = m_node->QueryInterface<Ipv4> (Ipv4::iid);
  NS_ASSERT_MSG(ipv4Local, "QI for <Ipv4> interface failed");
//
// Now, we need to ask Ipv4 for the number of interfaces attached to this 
// node. This isn't necessarily equal to the number of links to adjacent 
// nodes (other routers); the number of interfaces may include interfaces
// connected to stub networks (e.g., ethernets, etc.).  So we have to walk
// through the list of net devices and see if they are directly connected
// to another router.
//
// We'll start out at the maximum possible number of LSAs and reduce that
// number if we discover a link that's not actually connected to another
// router.
//
  m_numLSAs = 0;
  uint32_t numDevices = m_node->GetNDevices();
  NS_DEBUG("StaticRouter::GetNumLSAs (): numDevices = " << numDevices);
//
// Loop through the devices looking for those connected to a point-to-point
// channel.  These are the ones that are used to route packets.
//
  for (uint32_t i = 0; i < numDevices; ++i)
    {
      Ptr<NetDevice> nd = m_node->GetDevice(i);

      if (!nd->IsPointToPoint ())
        {
          NS_DEBUG("StaticRouter::GetNumLSAs (): non-point-to-point device");
          continue;
        }

      NS_DEBUG("StaticRouter::GetNumLSAs (): Point-to-point device");
//
// Now, we have to find the Ipv4 interface whose netdevice is the one we 
// just found.  This is the IP on the local side of the channel.  There is 
// a function to do this used down in the guts of the stack, but its not 
// exported so we had to whip up an equivalent.
//
      uint32_t ifIndexLocal = FindIfIndexForDevice(m_node, nd);
//
// Now that we have the Ipv4 interface index, we can get the address and mask
// we need.
//
      Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
      Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
      NS_DEBUG("Working with local address " << addrLocal);
//
// Now, we're going to link to the remote net device on the other end of the
// point-to-point channel we know we have.  This is where our adjacent router
// (to use OSPF lingo) is running.  
//
      Ptr<Channel> ch = nd->GetChannel();
      Ptr<NetDevice> ndRemote = GetAdjacent(nd, ch);
//
// The adjacent net device is aggregated to a node.  We need to ask that net 
// device for its node, then ask that node for its Ipv4 interface.
//
      Ptr<Node> nodeRemote = ndRemote->GetNode();
      Ptr<Ipv4> ipv4Remote = nodeRemote->QueryInterface<Ipv4> (Ipv4::iid);
      NS_ASSERT_MSG(ipv4Remote, "QI for remote <Ipv4> interface failed");
//
// Now, just like we did above, we need to get the IP interface index for the 
// net device on the other end of the point-to-point channel.
//
      uint32_t ifIndexRemote = FindIfIndexForDevice(nodeRemote, ndRemote);
//
// Now that we have the Ipv4 interface, we can get the (remote) address and
// mask we need.
//
      Ipv4Address addrRemote = ipv4Remote->GetAddress(ifIndexRemote);
      Ipv4Mask maskRemote = ipv4Remote->GetNetworkMask(ifIndexRemote);
      NS_DEBUG("Working with remote address " << addrRemote);
      ++m_numLSAs;
    }

  return m_numLSAs;
}

  bool
StaticRouter::GetLSA (uint32_t n, StaticRouterLSA &lsa)
{
  return false;
}

  Ptr<NetDevice>
StaticRouter::GetAdjacent(Ptr<NetDevice> nd, Ptr<Channel> ch)
{
//
// Double-check that channel agrees with device that it's a point-to-point
//
  NS_ASSERT(ch->GetType () == Channel::PointToPoint);

  uint32_t nDevices = ch->GetNDevices();
  NS_ASSERT_MSG(nDevices == 2, 
    "Point to point channel with other than two devices is not expected");
//
// This is a point to point channel with two endpoints.  Get both of them.
//
  Ptr<NetDevice> nd1 = ch->GetDevice(0);
  Ptr<NetDevice> nd2 = ch->GetDevice(1);
//
// One of the endpoints is going to be "us" -- that is the net device attached
// to the node on which we're running -- i.e., "nd".  The other endpoint (the
// one to which we are connected via the channel) is the adjacent router.
//
  if (nd1 == nd)
    {
      return nd2;
    }
  else if (nd2 == nd)
    {
      return nd1;
    }
  else
    {
      NS_ASSERT_MSG(0, 
        "Neither channel endpoint thinks it is connected to this net device");
      return 0;
    }
}

  uint32_t
StaticRouter::FindIfIndexForDevice(Ptr<Node> node, Ptr<NetDevice> nd)
{
  Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
  NS_ASSERT_MSG(ipv4, "QI for <Ipv4> interface failed");
  for (uint32_t i = 0; i < ipv4->GetNInterfaces(); ++i )
    {
      if (ipv4->GetNetDevice(i) == nd) 
        {
          return i;
        }
    }

  NS_ASSERT_MSG(0, "Cannot find interface for device");
  return 0;
}

} // namespace ns3
