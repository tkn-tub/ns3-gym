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

// ---------------------------------------------------------------------------
//
// StaticRouterLinkRecord Implementation
//
// ---------------------------------------------------------------------------

StaticRouterLinkRecord::StaticRouterLinkRecord ()
:
  m_linkId ("0.0.0.0"),
  m_linkData ("0.0.0.0"),
  m_linkType (Unknown),
  m_metric (0)
{
  NS_DEBUG("StaticRouterLinkRecord::StaticRouterLinkRecord ()");
}

StaticRouterLinkRecord::StaticRouterLinkRecord (
  LinkType    linkType, 
  Ipv4Address linkId, 
  Ipv4Address linkData, 
  uint32_t    metric)
:
  m_linkId (linkId),
  m_linkData (linkData),
  m_linkType (linkType),
  m_metric (metric)
{
  NS_DEBUG("StaticRouterLinkRecord::StaticRouterLinkRecord (" <<
    linkType << ", " << linkId << ", " << linkData << ", " << metric << ")");
}

StaticRouterLinkRecord::~StaticRouterLinkRecord ()
{
  NS_DEBUG("StaticRouterLinkRecord::~StaticRouterLinkRecord ()");
}

  Ipv4Address
StaticRouterLinkRecord::GetLinkId (void) const
{
  NS_DEBUG("StaticRouterLinkRecord::GetLinkId ()");
  return m_linkId;
}

  void
StaticRouterLinkRecord::SetLinkId (Ipv4Address addr)
{
  NS_DEBUG("StaticRouterLinkRecord::SetLinkId ()");
  m_linkId = addr;
}

  Ipv4Address
StaticRouterLinkRecord::GetLinkData (void) const
{
  NS_DEBUG("StaticRouterLinkRecord::GetLinkData ()");
  return m_linkData;
}

  void
StaticRouterLinkRecord::SetLinkData (Ipv4Address addr)
{
  NS_DEBUG("StaticRouterLinkRecord::SetLinkData ()");
  m_linkData = addr;
}

  StaticRouterLinkRecord::LinkType
StaticRouterLinkRecord::GetLinkType (void) const
{
  NS_DEBUG("StaticRouterLinkRecord::GetLinkType ()");
  return m_linkType;
}

  void
StaticRouterLinkRecord::SetLinkType (
  StaticRouterLinkRecord::LinkType linkType)
{
  NS_DEBUG("StaticRouterLinkRecord::SetLinkType ()");
  m_linkType = linkType;
}

  uint32_t
StaticRouterLinkRecord::GetMetric (void) const
{
  NS_DEBUG("StaticRouterLinkRecord::GetMetric ()");
  return m_metric;
}

  void
StaticRouterLinkRecord::SetMetric (uint32_t metric)
{
  NS_DEBUG("StaticRouterLinkRecord::SetMetric ()");
  m_metric = metric;
}

// ---------------------------------------------------------------------------
//
// StaticRouterLSA Implementation
//
// ---------------------------------------------------------------------------

StaticRouterLSA::StaticRouterLSA()
  : 
  m_linkStateId("0.0.0.0"),
  m_advertisingRtr("0.0.0.0"),
  m_linkRecords(),
  m_status(StaticRouterLSA::LSA_SPF_NOT_EXPLORED)
{
  NS_DEBUG("StaticRouterLSA::StaticRouterLSA ()");
}

StaticRouterLSA::StaticRouterLSA (
  StaticRouterLSA::SPFStatus status,
  Ipv4Address linkStateId, 
  Ipv4Address advertisingRtr)
:
  m_linkStateId(linkStateId),
  m_advertisingRtr(advertisingRtr),
  m_linkRecords(),
  m_status(status)
{
  NS_DEBUG("StaticRouterLSA::StaticRouterLSA (" << status << ", " <<
    linkStateId << ", " << advertisingRtr << ")");
}

StaticRouterLSA::StaticRouterLSA (StaticRouterLSA& lsa)
  : m_linkStateId(lsa.m_linkStateId), m_advertisingRtr(lsa.m_advertisingRtr),
    m_status(lsa.m_status)
{
  NS_ASSERT_MSG(IsEmpty(), 
    "StaticRouterLSA::StaticRouterLSA (): Non-empty LSA in constructor");
  CopyLinkRecords (lsa);
}

  StaticRouterLSA&
StaticRouterLSA::operator= (const StaticRouterLSA& lsa)
{
  m_linkStateId = lsa.m_linkStateId;
  m_advertisingRtr = lsa.m_advertisingRtr;
  m_status = lsa.m_status;

  ClearLinkRecords ();
  CopyLinkRecords (lsa);
  return *this;
}

  void
StaticRouterLSA::CopyLinkRecords (const StaticRouterLSA& lsa)
{
  for (ListOfLinkRecords_t::const_iterator i = lsa.m_linkRecords.begin ();
       i != lsa.m_linkRecords.end (); 
       i++)
    {
      StaticRouterLinkRecord *pSrc = *i;
      StaticRouterLinkRecord *pDst = new StaticRouterLinkRecord;

      pDst->SetLinkType (pSrc->GetLinkType ());
      pDst->SetLinkId (pSrc->GetLinkId ());
      pDst->SetLinkData (pSrc->GetLinkData ());

      m_linkRecords.push_back(pDst);
      pDst = 0;
    }
}

StaticRouterLSA::~StaticRouterLSA()
{
  NS_DEBUG("StaticRouterLSA::~StaticRouterLSA ()");
  ClearLinkRecords ();
}

  void
StaticRouterLSA::ClearLinkRecords(void)
{
  for ( ListOfLinkRecords_t::iterator i = m_linkRecords.begin ();
        i != m_linkRecords.end (); 
        i++)
    {
      NS_DEBUG("StaticRouterLSA::ClearLinkRecords ():  free link record");

      StaticRouterLinkRecord *p = *i;
      delete p;
      p = 0;

      *i = 0;
    }
  NS_DEBUG("StaticRouterLSA::ClearLinkRecords():  clear list");
  m_linkRecords.clear();
}

  uint32_t
StaticRouterLSA::AddLinkRecord (StaticRouterLinkRecord* lr)
{
  m_linkRecords.push_back (lr);
  return m_linkRecords.size ();
}

  uint32_t
StaticRouterLSA::GetNLinkRecords (void) const
{
  return m_linkRecords.size ();
}

  StaticRouterLinkRecord *
StaticRouterLSA::GetLinkRecord (uint32_t n) const
{
  uint32_t j = 0;
  for ( ListOfLinkRecords_t::const_iterator i = m_linkRecords.begin ();
        i != m_linkRecords.end (); 
        i++, j++)
    {
      if (j == n) 
        {
          return *i;
        }
    }
  NS_ASSERT_MSG(false, "StaticRouterLSA::GetLinkRecord (): invalid index");
  return 0;
}

  bool
StaticRouterLSA::IsEmpty (void) const
{
  return m_linkRecords.size () == 0;
}

  Ipv4Address
StaticRouterLSA::GetLinkStateId (void) const
{
  return m_linkStateId;
}

  void
StaticRouterLSA::SetLinkStateId (Ipv4Address addr)
{
  m_linkStateId = addr;
}

  Ipv4Address
StaticRouterLSA::GetAdvertisingRouter (void) const
{
  return m_advertisingRtr;
}

  void
StaticRouterLSA::SetAdvertisingRouter (Ipv4Address addr)
{
  m_advertisingRtr = addr;
}

  StaticRouterLSA::SPFStatus
StaticRouterLSA::GetStatus (void) const
{
  return m_status;
}

  void
StaticRouterLSA::SetStatus (StaticRouterLSA::SPFStatus status)
{
  m_status = status;
}

  void 
StaticRouterLSA::Print (std::ostream &os) const
{
  os << "m_linkStateId = " << m_linkStateId << std::endl <<
        "m_advertisingRtr = " << m_advertisingRtr << std::endl;

  for ( ListOfLinkRecords_t::const_iterator i = m_linkRecords.begin ();
        i != m_linkRecords.end (); 
        i++)
    {
      StaticRouterLinkRecord *p = *i;
      os << "----------" << std::endl;
      os << "m_linkId = " << p->GetLinkId () << std::endl;
      os << "m_linkData = " << p->GetLinkData () << std::endl;
    }
}

std::ostream& operator<< (std::ostream& os, StaticRouterLSA& lsa)
{
  lsa.Print (os);
  return os;
}

// ---------------------------------------------------------------------------
//
// StaticRouter Implementation
//
// ---------------------------------------------------------------------------

const InterfaceId StaticRouter::iid = 
  MakeInterfaceId ("StaticRouter", Object::iid);

StaticRouter::StaticRouter (Ptr<Node> node)
  : m_node(node), m_LSAs()
{
  NS_DEBUG("StaticRouter::StaticRouter ()");
  SetInterfaceId (StaticRouter::iid);
  m_routerId.Set(RoutingEnvironment::AllocateRouterId());
}

StaticRouter::~StaticRouter ()
{
  NS_DEBUG("StaticRouter::~StaticRouter ()");
  ClearLSAs();
}

  void
StaticRouter::ClearLSAs ()
{
  NS_DEBUG("StaticRouter::ClearLSAs ()");

  for ( ListOfLSAs_t::iterator i = m_LSAs.begin ();
        i != m_LSAs.end (); 
        i++)
    {
      NS_DEBUG("StaticRouter::ClearLSAs ():  free LSA");

      StaticRouterLSA *p = *i;
      delete p;
      p = 0;

      *i = 0;
    }
  NS_DEBUG("StaticRouter::ClearLSAs ():  clear list");
  m_LSAs.clear();
}

  Ipv4Address
StaticRouter::GetRouterId (void) const
{
  return m_routerId;
}

//
// Go out and discover any adjacent routers and build the Link State 
// Advertisements that reflect them and their associated networks.
// 
  uint32_t 
StaticRouter::DiscoverLSAs (void)
{
  NS_DEBUG("StaticRouter::DiscoverLSAs ()");
  NS_ASSERT_MSG(m_node, 
    "StaticRouter::DiscoverLSAs (): <Node> interface not set");

  ClearLSAs ();
//
// We're aggregated to a node.  We need to ask the node for a pointer to its
// Ipv4 interface.  This is where the information regarding the attached 
// interfaces lives.
//
  Ptr<Ipv4> ipv4Local = m_node->QueryInterface<Ipv4> (Ipv4::iid);
  NS_ASSERT_MSG(ipv4Local, 
    "StaticRouter::DiscoverLSAs (): QI for <Ipv4> interface failed");
//
// We are, for now at least, only going to report RouterLSAs in this method.
// What this means is that there is going to be one advertisement with some
// number of link records.  This means that GetNumLSAs will actually always
// return exactly one.
//
  StaticRouterLSA *pLSA = new StaticRouterLSA;
  pLSA->SetLinkStateId (m_routerId);
  pLSA->SetAdvertisingRouter (m_routerId);
  pLSA->SetStatus (StaticRouterLSA::LSA_SPF_NOT_EXPLORED);
//
// We need to ask the node for the number of net devices attached. This isn't
// necessarily equal to the number of links to adjacent nodes (other routers)
// as the number of devices may include those for stub networks (e.g., 
// ethernets, etc.).  So we have to walk through the list of net devices and
// pay attention to those that are directly connected to another router through
// a point-to-point channel.
//
  uint32_t numDevices = m_node->GetNDevices();
  NS_DEBUG("StaticRouter::DiscoverLSAs (): numDevices = " << numDevices);
//
// Loop through the devices looking for those connected to a point-to-point
// channel.
//
  for (uint32_t i = 0; i < numDevices; ++i)
    {
      Ptr<NetDevice> ndLocal = m_node->GetDevice(i);

      if (!ndLocal->IsPointToPoint ())
        {
          NS_DEBUG("StaticRouter::DiscoverLSAs (): non-point-to-point device");
          continue;
        }

      NS_DEBUG("StaticRouter::DiscoverLSAs (): Point-to-point device");
//
// Now, we have to find the Ipv4 interface whose netdevice is the one we 
// just found.  This is still the IP on the local side of the channel.  There 
// is a function to do this used down in the guts of the stack, but it's not 
// exported so we had to whip up an equivalent.
//
      uint32_t ifIndexLocal = FindIfIndexForDevice(m_node, ndLocal);
//
// Now that we have the Ipv4 interface index, we can get the address and mask
// we need.
//
      Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
      Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
      NS_DEBUG("Working with local address " << addrLocal);
//
// Now, we're going to walk over to the remote net device on the other end of 
// the point-to-point channel we now know we have.  This is where our adjacent 
// router (to use OSPF lingo) is running.  
//
      Ptr<Channel> ch = ndLocal->GetChannel();
      Ptr<NetDevice> ndRemote = GetAdjacent(ndLocal, ch);
//
// The adjacent net device is aggregated to a node.  We need to ask that net 
// device for its node, then ask that node for its Ipv4 interface.
//
      Ptr<Node> nodeRemote = ndRemote->GetNode();
      Ptr<Ipv4> ipv4Remote = nodeRemote->QueryInterface<Ipv4> (Ipv4::iid);
      NS_ASSERT_MSG(ipv4Remote, 
        "StaticRouter::DiscoverLSAs (): QI for remote <Ipv4> failed");
//
// Per the OSPF spec, we're going to need the remote router ID, so we might as
// well get it now.
//
      Ptr<StaticRouter> srRemote = 
        nodeRemote->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(srRemote, 
        "StaticRouter::DiscoverLSAs (): QI for remote <StaticRouter> failed");
      Ipv4Address rtrIdRemote = srRemote->GetRouterId();
      NS_DEBUG("Working with remote router " << rtrIdRemote);
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
//
// Now we can fill out the link records for this link.  There are always two
// link records; the first is a point-to-point record describing the link and
// the second is a stub network record with the network number.
//
      StaticRouterLinkRecord *plr = new StaticRouterLinkRecord;
      plr->SetLinkType (StaticRouterLinkRecord::PointToPoint);
      plr->SetLinkId (rtrIdRemote);
      plr->SetLinkData (addrLocal);
      pLSA->AddLinkRecord(plr);
      plr = 0;

      plr = new StaticRouterLinkRecord;
      plr->SetLinkType (StaticRouterLinkRecord::StubNetwork);
      plr->SetLinkId (addrRemote);
      plr->SetLinkData (Ipv4Address(maskRemote.GetHostOrder()));  // Frown
      pLSA->AddLinkRecord(plr);
      plr = 0;
    }
//
// The LSA goes on a list of LSAs in case we want to begin exporting other
// kinds of advertisements (than Router LSAs).
  m_LSAs.push_back (pLSA);
  NS_DEBUG(*pLSA);
  return m_LSAs.size ();
}

  uint32_t 
StaticRouter::GetNumLSAs (void) const
{
  NS_DEBUG("StaticRouter::GetNumLSAs ()");
  return m_LSAs.size ();
}

//
// Get the nth link state advertisement from this router.
//
  bool
StaticRouter::GetLSA (uint32_t n, StaticRouterLSA &lsa) const
{
  NS_ASSERT_MSG(lsa.IsEmpty(), "StaticRouter::GetLSA (): Must pass empty LSA");
//
// All of the work was done in GetNumLSAs.  All we have to do here is to
// walk the list of link state advertisements created there and return the 
// one the client is interested in.
//
  ListOfLSAs_t::const_iterator i = m_LSAs.begin ();
  uint32_t j = 0;

  for (; i != m_LSAs.end (); i++, j++)
    {
      if (j == n)
        {
          StaticRouterLSA *p = *i;
          lsa = *p;
          return true;
        }
    }

  return false;
}

//
// Link through the given channel and find the net device that's on the
// other end.  This only makes sense with a point-to-point channel.
//
  Ptr<NetDevice>
StaticRouter::GetAdjacent(Ptr<NetDevice> nd, Ptr<Channel> ch) const
{
//
// Double-check that channel agrees with device that it's a point-to-point
//
  NS_ASSERT(ch->GetType () == Channel::PointToPoint);

  uint32_t nDevices = ch->GetNDevices();
  NS_ASSERT_MSG(nDevices == 2, 
    "StaticRouter::GetAdjacent (): Channel with other than two devices");
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
      NS_ASSERT_MSG(false,
        "StaticRouter::GetAdjacent (): Wrong or confused channel?");
      return 0;
    }
}

//
// Given a node and a net device, find the IPV4 interface index that 
// corresponds to that net device.
//
  uint32_t
StaticRouter::FindIfIndexForDevice(Ptr<Node> node, Ptr<NetDevice> nd) const
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
