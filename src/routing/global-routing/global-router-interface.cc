/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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
 * Authors:  Tom Henderson (tomhend@u.washington.edu)
 */

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/abort.h"
#include "ns3/channel.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/ipv4.h"
#include "ns3/bridge-net-device.h"
#include "ns3/net-device-container.h"
#include "global-router-interface.h"
#include <vector>

NS_LOG_COMPONENT_DEFINE ("GlobalRouter");

namespace ns3 {

// ---------------------------------------------------------------------------
//
// GlobalRoutingLinkRecord Implementation
//
// ---------------------------------------------------------------------------

GlobalRoutingLinkRecord::GlobalRoutingLinkRecord ()
:
  m_linkId ("0.0.0.0"),
  m_linkData ("0.0.0.0"),
  m_linkType (Unknown),
  m_metric (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

GlobalRoutingLinkRecord::GlobalRoutingLinkRecord (
  LinkType    linkType, 
  Ipv4Address linkId, 
  Ipv4Address linkData, 
  uint16_t    metric)
:
  m_linkId (linkId),
  m_linkData (linkData),
  m_linkType (linkType),
  m_metric (metric)
{
  NS_LOG_FUNCTION (this << linkType << linkId << linkData << metric);
}

GlobalRoutingLinkRecord::~GlobalRoutingLinkRecord ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

  Ipv4Address
GlobalRoutingLinkRecord::GetLinkId (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkId;
}

  void
GlobalRoutingLinkRecord::SetLinkId (Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_linkId = addr;
}

  Ipv4Address
GlobalRoutingLinkRecord::GetLinkData (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkData;
}

  void
GlobalRoutingLinkRecord::SetLinkData (Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_linkData = addr;
}

  GlobalRoutingLinkRecord::LinkType
GlobalRoutingLinkRecord::GetLinkType (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkType;
}

  void
GlobalRoutingLinkRecord::SetLinkType (
  GlobalRoutingLinkRecord::LinkType linkType)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_linkType = linkType;
}

  uint16_t
GlobalRoutingLinkRecord::GetMetric (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_metric;
}

  void
GlobalRoutingLinkRecord::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_metric = metric;
}

// ---------------------------------------------------------------------------
//
// GlobalRoutingLSA Implementation
//
// ---------------------------------------------------------------------------

GlobalRoutingLSA::GlobalRoutingLSA()
  : 
  m_lsType (GlobalRoutingLSA::Unknown),
  m_linkStateId("0.0.0.0"),
  m_advertisingRtr("0.0.0.0"),
  m_linkRecords(),
  m_networkLSANetworkMask("0.0.0.0"),
  m_attachedRouters(),
  m_status(GlobalRoutingLSA::LSA_SPF_NOT_EXPLORED)
{
  NS_LOG_FUNCTION_NOARGS ();
}

GlobalRoutingLSA::GlobalRoutingLSA (
  GlobalRoutingLSA::SPFStatus status,
  Ipv4Address linkStateId, 
  Ipv4Address advertisingRtr)
:
  m_lsType (GlobalRoutingLSA::Unknown),
  m_linkStateId(linkStateId),
  m_advertisingRtr(advertisingRtr),
  m_linkRecords(),
  m_networkLSANetworkMask("0.0.0.0"),
  m_attachedRouters(),
  m_status(status)
{
  NS_LOG_FUNCTION (this << status << linkStateId << advertisingRtr);
}

GlobalRoutingLSA::GlobalRoutingLSA (GlobalRoutingLSA& lsa)
  : m_lsType(lsa.m_lsType), m_linkStateId(lsa.m_linkStateId), 
    m_advertisingRtr(lsa.m_advertisingRtr), 
    m_networkLSANetworkMask(lsa.m_networkLSANetworkMask), 
    m_status(lsa.m_status)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG(IsEmpty(), 
    "GlobalRoutingLSA::GlobalRoutingLSA (): Non-empty LSA in constructor");
  CopyLinkRecords (lsa);
}

  GlobalRoutingLSA&
GlobalRoutingLSA::operator= (const GlobalRoutingLSA& lsa)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_lsType = lsa.m_lsType;
  m_linkStateId = lsa.m_linkStateId;
  m_advertisingRtr = lsa.m_advertisingRtr;
  m_networkLSANetworkMask = lsa.m_networkLSANetworkMask, 
  m_status = lsa.m_status;

  ClearLinkRecords ();
  CopyLinkRecords (lsa);
  return *this;
}

  void
GlobalRoutingLSA::CopyLinkRecords (const GlobalRoutingLSA& lsa)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (ListOfLinkRecords_t::const_iterator i = lsa.m_linkRecords.begin ();
       i != lsa.m_linkRecords.end (); 
       i++)
    {
      GlobalRoutingLinkRecord *pSrc = *i;
      GlobalRoutingLinkRecord *pDst = new GlobalRoutingLinkRecord;

      pDst->SetLinkType (pSrc->GetLinkType ());
      pDst->SetLinkId (pSrc->GetLinkId ());
      pDst->SetLinkData (pSrc->GetLinkData ());
      pDst->SetMetric (pSrc->GetMetric ());

      m_linkRecords.push_back(pDst);
      pDst = 0;
    }

   m_attachedRouters = lsa.m_attachedRouters;
}

GlobalRoutingLSA::~GlobalRoutingLSA()
{
  NS_LOG_FUNCTION_NOARGS ();
  ClearLinkRecords ();
}

  void
GlobalRoutingLSA::ClearLinkRecords(void)
{
  NS_LOG_FUNCTION_NOARGS ();
  for ( ListOfLinkRecords_t::iterator i = m_linkRecords.begin ();
        i != m_linkRecords.end (); 
        i++)
    {
      NS_LOG_LOGIC ("Free link record");

      GlobalRoutingLinkRecord *p = *i;
      delete p;
      p = 0;

      *i = 0;
    }
  NS_LOG_LOGIC ("Clear list");
  m_linkRecords.clear();
}

  uint32_t
GlobalRoutingLSA::AddLinkRecord (GlobalRoutingLinkRecord* lr)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_linkRecords.push_back (lr);
  return m_linkRecords.size ();
}

  uint32_t
GlobalRoutingLSA::GetNLinkRecords (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkRecords.size ();
}

  GlobalRoutingLinkRecord *
GlobalRoutingLSA::GetLinkRecord (uint32_t n) const
{
  NS_LOG_FUNCTION_NOARGS ();
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
  NS_ASSERT_MSG(false, "GlobalRoutingLSA::GetLinkRecord (): invalid index");
  return 0;
}

  bool
GlobalRoutingLSA::IsEmpty (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkRecords.size () == 0;
}

  GlobalRoutingLSA::LSType
GlobalRoutingLSA::GetLSType (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_lsType;
}

  void 
GlobalRoutingLSA::SetLSType (GlobalRoutingLSA::LSType typ) 
{
  NS_LOG_FUNCTION_NOARGS ();
  m_lsType = typ;
}

  Ipv4Address
GlobalRoutingLSA::GetLinkStateId (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkStateId;
}

  void
GlobalRoutingLSA::SetLinkStateId (Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_linkStateId = addr;
}

  Ipv4Address
GlobalRoutingLSA::GetAdvertisingRouter (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_advertisingRtr;
}

  void
GlobalRoutingLSA::SetAdvertisingRouter (Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_advertisingRtr = addr;
}

  void 
GlobalRoutingLSA::SetNetworkLSANetworkMask (Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_networkLSANetworkMask = mask;
}

  Ipv4Mask 
GlobalRoutingLSA::GetNetworkLSANetworkMask (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_networkLSANetworkMask;
}

  GlobalRoutingLSA::SPFStatus
GlobalRoutingLSA::GetStatus (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_status;
}

  uint32_t 
GlobalRoutingLSA::AddAttachedRouter (Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_attachedRouters.push_back (addr);
  return m_attachedRouters.size ();
}

  uint32_t 
GlobalRoutingLSA::GetNAttachedRouters (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_attachedRouters.size (); 
}

  Ipv4Address 
GlobalRoutingLSA::GetAttachedRouter (uint32_t n) const
{
  NS_LOG_FUNCTION_NOARGS ();
  uint32_t j = 0;
  for ( ListOfAttachedRouters_t::const_iterator i = m_attachedRouters.begin ();
        i != m_attachedRouters.end (); 
        i++, j++)
    {
      if (j == n) 
        {
          return *i;
        }
    }
  NS_ASSERT_MSG(false, "GlobalRoutingLSA::GetAttachedRouter (): invalid index");
  return Ipv4Address("0.0.0.0");
}

  void
GlobalRoutingLSA::SetStatus (GlobalRoutingLSA::SPFStatus status)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_status = status;
}

  void 
GlobalRoutingLSA::Print (std::ostream &os) const
{
  os << "m_lsType = " << m_lsType << std::endl <<
        "m_linkStateId = " << m_linkStateId << std::endl <<
        "m_advertisingRtr = " << m_advertisingRtr << std::endl;

  if (m_lsType == GlobalRoutingLSA::RouterLSA) 
    {
      for ( ListOfLinkRecords_t::const_iterator i = m_linkRecords.begin ();
            i != m_linkRecords.end (); 
            i++)
        {
          GlobalRoutingLinkRecord *p = *i;
          os << "----------" << std::endl;
          os << "m_linkId = " << p->GetLinkId () << std::endl;
          os << "m_linkData = " << p->GetLinkData () << std::endl;
          os << "m_metric = " << p->GetMetric () << std::endl;
        }
    }
  else if (m_lsType == GlobalRoutingLSA::NetworkLSA) 
    {
      os << "----------" << std::endl;
      os << "m_networkLSANetworkMask = " << m_networkLSANetworkMask 
         << std::endl;
      for ( ListOfAttachedRouters_t::const_iterator i = 
            m_attachedRouters.begin ();
            i != m_attachedRouters.end (); 
            i++)
        {
          Ipv4Address p = *i;
          os << "attachedRouter = " << p << std::endl;
        }
    }
  else 
    {
      NS_ASSERT_MSG(0, "Illegal LSA LSType: " << m_lsType);
    }
}

std::ostream& operator<< (std::ostream& os, GlobalRoutingLSA& lsa)
{
  lsa.Print (os);
  return os;
}

// ---------------------------------------------------------------------------
//
// GlobalRouter Implementation
//
// ---------------------------------------------------------------------------

NS_OBJECT_ENSURE_REGISTERED (GlobalRouter);

TypeId 
GlobalRouter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GlobalRouter")
    .SetParent<Object> ();
  return tid;
}

GlobalRouter::GlobalRouter ()
  : m_LSAs()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_routerId.Set(GlobalRouteManager::AllocateRouterId ());
}

GlobalRouter::~GlobalRouter ()
{
  NS_LOG_FUNCTION_NOARGS ();
  ClearLSAs();
}

void
GlobalRouter::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  Object::DoDispose ();
}

  void
GlobalRouter::ClearLSAs ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for ( ListOfLSAs_t::iterator i = m_LSAs.begin ();
        i != m_LSAs.end (); 
        i++)
    {
      NS_LOG_LOGIC ("Free LSA");

      GlobalRoutingLSA *p = *i;
      delete p;
      p = 0;

      *i = 0;
    }
  NS_LOG_LOGIC ("Clear list");
  m_LSAs.clear();
}

  Ipv4Address
GlobalRouter::GetRouterId (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_routerId;
}

//
// DiscoverLSAs is called on all nodes in the system that have a GlobalRouter
// interface aggregated.  We need to go out and discover any adjacent routers 
// and build the Link State Advertisements that reflect them and their associated
// networks.
// 
  uint32_t 
GlobalRouter::DiscoverLSAs (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Node> node = GetObject<Node> ();
  NS_ABORT_MSG_UNLESS (node, "GlobalRouter::DiscoverLSAs (): GetObject for <Node> interface failed");
  NS_LOG_LOGIC ("For node " << node->GetId () );

  ClearLSAs ();

  //
  // While building the Router-LSA, keep a list of those NetDevices for
  // which the current node is the designated router and we will later build 
  // a NetworkLSA for.
  //
  NetDeviceContainer c;

  //
  // We're aggregated to a node.  We need to ask the node for a pointer to its
  // Ipv4 interface.  This is where the information regarding the attached 
  // interfaces lives.  If we're a router, we had better have an Ipv4 interface.
  //
  Ptr<Ipv4> ipv4Local = node->GetObject<Ipv4> ();
  NS_ABORT_MSG_UNLESS (ipv4Local, "GlobalRouter::DiscoverLSAs (): GetObject for <Ipv4> interface failed");

  //
  // Every router node originates a Router-LSA
  //
  GlobalRoutingLSA *pLSA = new GlobalRoutingLSA;
  pLSA->SetLSType (GlobalRoutingLSA::RouterLSA);
  pLSA->SetLinkStateId (m_routerId);
  pLSA->SetAdvertisingRouter (m_routerId);
  pLSA->SetStatus (GlobalRoutingLSA::LSA_SPF_NOT_EXPLORED);

  //
  // Ask the node for the number of net devices attached. This isn't necessarily 
  // equal to the number of links to adjacent nodes (other routers) as the number
  // of devices may include those for stub networks (e.g., ethernets, etc.) and 
  // bridge devices also take up an "extra" net device.
  //
  uint32_t numDevices = node->GetNDevices();

  //
  // Iterate through the devices on the node and walk the channel to see what's
  // on the other side of the standalone devices..
  //
  for (uint32_t i = 0; i < numDevices; ++i)
    {
      Ptr<NetDevice> ndLocal = node->GetDevice(i);

      //
      // Check to see if the net device we just got has a corresponding IP 
      // interface (could be a pure L2 NetDevice).
      //
      bool isIp = false;
      for (uint32_t i = 0; i < ipv4Local->GetNInterfaces (); ++i )
        {
          if (ipv4Local->GetNetDevice (i) == ndLocal) 
            {
              isIp = true;
              break;
            }
        }

      if (!isIp)
        {
          NS_LOG_LOGIC ("**** Net device " << ndLocal << "has no IP interface, skipping");
          continue;
        }

      //
      // We have a net device that we need to check out.  If it suports 
      // broadcast and is not a point-point link, then it will be either a stub 
      // network or a transit network depending on the number of routers on
      // the segment.  We add the appropriate link record to the LSA.
      //
      // If the device is a point to point link, we treat it separately.  In
      // that case, there always two link records added.
      //
      if (ndLocal->IsBroadcast () && !ndLocal->IsPointToPoint () )
        {
          NS_LOG_LOGIC ("**** Broadcast link");
          ProcessBroadcastLink (ndLocal, pLSA, c);
        }
      else if (ndLocal->IsPointToPoint () )
        {
          NS_LOG_LOGIC ("**** Point=to-point link");
          ProcessPointToPointLink (ndLocal, pLSA);
        }
      else
        {
          NS_ASSERT_MSG(0, "GlobalRouter::DiscoverLSAs (): unknown link type");
        }
    }

  NS_LOG_LOGIC ("========== LSA for node " << node->GetId () << " ==========");
  NS_LOG_LOGIC (*pLSA);
  m_LSAs.push_back (pLSA);
  pLSA = 0;

  // 
  // Now, determine whether we need to build a NetworkLSA.  This is the case if
  // we found at least one designated router.
  //
  uint32_t nDesignatedRouters = c.GetN ();
  if (nDesignatedRouters > 0)
    {
      NS_LOG_LOGIC ("Build Network LSAs");
      BuildNetworkLSAs (c);
    }

  return m_LSAs.size ();
}

  void
GlobalRouter::ProcessBroadcastLink (Ptr<NetDevice> nd, GlobalRoutingLSA *pLSA, NetDeviceContainer &c)
{
  NS_LOG_FUNCTION (nd << pLSA << &c);

  if (nd->IsBridge ())
    {
      ProcessBridgedBroadcastLink (nd, pLSA, c);
    }
  else
    {
      ProcessSingleBroadcastLink (nd, pLSA, c);
    }
}

  void
GlobalRouter::ProcessSingleBroadcastLink (Ptr<NetDevice> nd, GlobalRoutingLSA *pLSA, NetDeviceContainer &c)
{
  NS_LOG_FUNCTION (nd << pLSA << &c);

  GlobalRoutingLinkRecord *plr = new GlobalRoutingLinkRecord;
  NS_ABORT_MSG_IF (plr == 0, "GlobalRouter::ProcessSingleBroadcastLink(): Can't alloc link record");

  //
  // We have some preliminaries to do to get enough information to proceed.
  // This information we need comes from the internet stack, so notice that
  // there is an implied assumption that global routing is only going to 
  // work with devices attached to the internet stack (have an ipv4 interface
  // associated to them.
  //
  Ptr<Node> node = nd->GetNode ();

  uint32_t ifIndexLocal;
  bool rc = FindIfIndexForDevice(node, nd, ifIndexLocal);
  NS_ABORT_MSG_IF (rc == false, "GlobalRouter::ProcessSingleBroadcastLink(): No interface index associated with device");

  Ptr<Ipv4> ipv4Local = node->GetObject<Ipv4> ();
  NS_ABORT_MSG_UNLESS (ipv4Local, "GlobalRouter::ProcessSingleBroadcastLink (): GetObject for <Ipv4> interface failed");

  Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
  Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
  NS_LOG_LOGIC ("Working with local address " << addrLocal);
  uint16_t metricLocal = ipv4Local->GetMetric (ifIndexLocal);

  //
  // Check to see if the net device is connected to a channel/network that has
  // another router on it.  If there is no other router on the link (but us) then
  // this is a stub network.  If we find another router, then what we have here
  // is a transit network.
  //
  if (AnotherRouterOnLink (nd) == false)
    {
      //
      // This is a net device connected to a stub network
      //
      NS_LOG_LOGIC("**** Router-LSA Stub Network");
      plr->SetLinkType (GlobalRoutingLinkRecord::StubNetwork);

      // 
      // According to OSPF, the Link ID is the IP network number of 
      // the attached network.
      //
      plr->SetLinkId (addrLocal.CombineMask(maskLocal));

      //
      // and the Link Data is the network mask; converted to Ipv4Address
      //
      Ipv4Address maskLocalAddr;
      maskLocalAddr.Set(maskLocal.Get ());
      plr->SetLinkData (maskLocalAddr);
      plr->SetMetric (metricLocal);
      pLSA->AddLinkRecord(plr);
      plr = 0;
    }
  else
    {
      //
      // We have multiple routers on a broadcast interface, so this is
      // a transit network.
      //
      NS_LOG_LOGIC ("**** Router-LSA Transit Network");
      plr->SetLinkType (GlobalRoutingLinkRecord::TransitNetwork);

      // 
      // By definition, the router with the lowest IP address is the
      // designated router for the network.  OSPF says that the Link ID
      // gets the IP interface address of the designated router in this 
      // case.
      //
      Ipv4Address desigRtr = FindDesignatedRouterForLink (nd);
      if (desigRtr == addrLocal) 
        {
          c.Add (nd);
          NS_LOG_LOGIC ("Node " << node->GetId () << " elected a designated router");
        }
      plr->SetLinkId (desigRtr);
      
      //
      // OSPF says that the Link Data is this router's own IP address.
      //
      plr->SetLinkData (addrLocal);
      plr->SetMetric (metricLocal);
      pLSA->AddLinkRecord (plr);
      plr = 0;
    }
}

  void
GlobalRouter::ProcessBridgedBroadcastLink (Ptr<NetDevice> nd, GlobalRoutingLSA *pLSA, NetDeviceContainer &c)
{
  NS_LOG_FUNCTION (nd << pLSA << &c);

  NS_ASSERT_MSG (nd->IsBridge (), "GlobalRouter::ProcessBridgedBroadcastLink(): Called with non-bridge net device");

  Ptr<BridgeNetDevice> bnd = nd->GetObject<BridgeNetDevice> ();
  NS_ABORT_MSG_UNLESS (bnd, "GlobalRouter::DiscoverLSAs (): GetObject for <BridgeNetDevice> failed");

  //
  // We need to handle a bridge on the router.  This means that we have been 
  // given a net device that is a BridgeNetDevice.  It has an associated Ipv4
  // interface index and address.  Some number of other net devices live "under"
  // the bridge device as so-called bridge ports.  In a nutshell, what we have
  // to do is to repeat what is done for a single broadcast link on all of 
  // those net devices living under the bridge (trolls?)
  //
  bool areTransitNetwork = false;
  Ipv4Address desigRtr ("255.255.255.255");

  for (uint32_t i = 0; i < bnd->GetNBridgePorts (); ++i)
    {
      Ptr<NetDevice> ndTemp = bnd->GetBridgePort (i);
      GlobalRoutingLSA *pLsaTest = new GlobalRoutingLSA;
      NetDeviceContainer cTest;
      ProcessSingleBroadcastLink (ndTemp, pLsaTest, cTest);

      //
      // The GlobalRoutingLSA pLsaTest will now have a link record attached to
      // it indicating what was found.  If another router is found on any one
      // of the bridged networks, we need to treat the whole bridge as a transit 
      // network.
      //
      // If the link type is a transit network, then we have got to do some work
      // to figure out what to do about the other routers on the bridge.
      //
      if (pLsaTest->GetLinkRecord (0)->GetLinkType () == GlobalRoutingLinkRecord::TransitNetwork)
        {
          areTransitNetwork = true;

          //
          // If we're going to be a transit network, then we have got to elect
          // a designated router for the whole bridge.  This means finding the
          // router with the lowest IP address on the whole bridge.  We ask 
          // for the lowest address on each segment and pick the lowest of them
          // all.
          //
          Ipv4Address desigRtrTemp = FindDesignatedRouterForLink (ndTemp);
          if (desigRtrTemp < desigRtr)
            {
              desigRtr = desigRtrTemp;
            }
        }
    }

  //
  // That's all the information we need to put it all together, just like we did
  // in the case of a single broadcast link.
  //

  GlobalRoutingLinkRecord *plr = new GlobalRoutingLinkRecord;
  NS_ABORT_MSG_IF (plr == 0, "GlobalRouter::ProcessBridgedBroadcastLink(): Can't alloc link record");

  //
  // We have some preliminaries to do to get enough information to proceed.
  // This information we need comes from the internet stack, so notice that
  // there is an implied assumption that global routing is only going to 
  // work with devices attached to the internet stack (have an ipv4 interface
  // associated to them.
  //
  Ptr<Node> node = nd->GetNode ();

  uint32_t ifIndexLocal;
  bool rc = FindIfIndexForDevice(node, nd, ifIndexLocal);
  NS_ABORT_MSG_IF (rc == false, "GlobalRouter::ProcessBridgedBroadcastLink(): No interface index associated with device");

  Ptr<Ipv4> ipv4Local = node->GetObject<Ipv4> ();
  NS_ABORT_MSG_UNLESS (ipv4Local, "GlobalRouter::ProcessBridgedBroadcastLink (): GetObject for <Ipv4> interface failed");

  Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
  Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
  NS_LOG_LOGIC ("Working with local address " << addrLocal);
  uint16_t metricLocal = ipv4Local->GetMetric (ifIndexLocal);

  if (areTransitNetwork == false)
    {
      //
      // This is a net device connected to a bridge of stub networks
      //
      NS_LOG_LOGIC("**** Router-LSA Stub Network");
      plr->SetLinkType (GlobalRoutingLinkRecord::StubNetwork);

      // 
      // According to OSPF, the Link ID is the IP network number of 
      // the attached network.
      //
      plr->SetLinkId (addrLocal.CombineMask(maskLocal));

      //
      // and the Link Data is the network mask; converted to Ipv4Address
      //
      Ipv4Address maskLocalAddr;
      maskLocalAddr.Set(maskLocal.Get ());
      plr->SetLinkData (maskLocalAddr);
      plr->SetMetric (metricLocal);
      pLSA->AddLinkRecord(plr);
      plr = 0;
    }
  else
    {
      //
      // We have multiple routers on a bridged broadcast interface, so this is
      // a transit network.
      //
      NS_LOG_LOGIC ("**** Router-LSA Transit Network");
      plr->SetLinkType (GlobalRoutingLinkRecord::TransitNetwork);

      // 
      // By definition, the router with the lowest IP address is the
      // designated router for the network.  OSPF says that the Link ID
      // gets the IP interface address of the designated router in this 
      // case.
      //
      if (desigRtr == addrLocal) 
        {
          c.Add (nd);
          NS_LOG_LOGIC ("Node " << node->GetId () << " elected a designated router");
        }
      plr->SetLinkId (desigRtr);
      
      //
      // OSPF says that the Link Data is this router's own IP address.
      //
      plr->SetLinkData (addrLocal);
      plr->SetMetric (metricLocal);
      pLSA->AddLinkRecord (plr);
      plr = 0;
    }
}

  void
GlobalRouter::ProcessPointToPointLink (Ptr<NetDevice> ndLocal, GlobalRoutingLSA *pLSA)
{
  NS_LOG_FUNCTION (ndLocal << pLSA);

  //
  // We have some preliminaries to do to get enough information to proceed.
  // This information we need comes from the internet stack, so notice that
  // there is an implied assumption that global routing is only going to 
  // work with devices attached to the internet stack (have an ipv4 interface
  // associated to them.
  //
  Ptr<Node> nodeLocal = ndLocal->GetNode ();

  uint32_t ifIndexLocal;
  bool rc = FindIfIndexForDevice(nodeLocal, ndLocal, ifIndexLocal);
  NS_ABORT_MSG_IF (rc == false, "GlobalRouter::ProcessPointToPointLink (): No interface index associated with device");

  Ptr<Ipv4> ipv4Local = nodeLocal->GetObject<Ipv4> ();
  NS_ABORT_MSG_UNLESS (ipv4Local, "GlobalRouter::ProcessPointToPointLink (): GetObject for <Ipv4> interface failed");

  Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
  Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
  NS_LOG_LOGIC ("Working with local address " << addrLocal);
  uint16_t metricLocal = ipv4Local->GetMetric (ifIndexLocal);

  //
  // Now, we're going to walk over to the remote net device on the other end of 
  // the point-to-point channel we know we have.  This is where our adjacent 
  // router (to use OSPF lingo) is running.  
  //
  Ptr<Channel> ch = ndLocal->GetChannel();

  //
  // Get the net device on the other side of the point-to-point channel.
  //
  Ptr<NetDevice> ndRemote = GetAdjacent(ndLocal, ch);

  //
  // The adjacent net device is aggregated to a node.  We need to ask that net 
  // device for its node, then ask that node for its Ipv4 interface.  Note a
  // requirement that nodes on either side of a point-to-point link must have 
  // internet stacks; and an assumption that point-to-point links are incompatible 
  // with bridging.
  //
  Ptr<Node> nodeRemote = ndRemote->GetNode();
  Ptr<Ipv4> ipv4Remote = nodeRemote->GetObject<Ipv4> ();
  NS_ABORT_MSG_UNLESS (ipv4Remote, 
                       "GlobalRouter::ProcessPointToPointLink(): GetObject for remote <Ipv4> failed");

  //
  // Further note the requirement that nodes on either side of a point-to-point 
  // link must participate in global routing and therefore have a GlobalRouter
  // interface aggregated.
  //
  Ptr<GlobalRouter> rtrRemote = nodeRemote->GetObject<GlobalRouter> ();
  NS_ABORT_MSG_UNLESS(rtrRemote, 
                      "GlobalRouter::ProcessPointToPointLinks(): GetObject for remote <GlobalRouter> failed");

  //
  // We're going to need the remote router ID, so we might as well get it now.
  //
  Ipv4Address rtrIdRemote = rtrRemote->GetRouterId();
  NS_LOG_LOGIC ("Working with remote router " << rtrIdRemote);

  //
  // Now, just like we did above, we need to get the IP interface index for the 
  // net device on the other end of the point-to-point channel.
  //
  uint32_t ifIndexRemote;
  rc = FindIfIndexForDevice(nodeRemote, ndRemote, ifIndexRemote);
  NS_ABORT_MSG_IF (rc == false, "GlobalRouter::ProcessPointToPointLinks(): No interface index associated with remote device");

  //
  // Now that we have the Ipv4 interface, we can get the (remote) address and
  // mask we need.
  //
  Ipv4Address addrRemote = ipv4Remote->GetAddress(ifIndexRemote);
  Ipv4Mask maskRemote = ipv4Remote->GetNetworkMask(ifIndexRemote);
  NS_LOG_LOGIC ("Working with remote address " << addrRemote);

  //
  // Now we can fill out the link records for this link.  There are always two
  // link records; the first is a point-to-point record describing the link and
  // the second is a stub network record with the network number.
  //
  GlobalRoutingLinkRecord *plr = new GlobalRoutingLinkRecord;
  NS_ABORT_MSG_IF (plr == 0, "GlobalRouter::ProcessPointToPointLink(): Can't alloc link record");
  plr->SetLinkType (GlobalRoutingLinkRecord::PointToPoint);
  plr->SetLinkId (rtrIdRemote);
  plr->SetLinkData (addrLocal);
  plr->SetMetric (metricLocal);
  pLSA->AddLinkRecord (plr);
  plr = 0;

  plr = new GlobalRoutingLinkRecord;
  NS_ABORT_MSG_IF (plr == 0, "GlobalRouter::ProcessPointToPointLink(): Can't alloc link record");
  plr->SetLinkType (GlobalRoutingLinkRecord::StubNetwork);
  plr->SetLinkId (addrRemote);
  plr->SetLinkData (Ipv4Address(maskRemote.Get()));  // Frown
  plr->SetMetric (metricLocal);
  pLSA->AddLinkRecord (plr);
  plr = 0;
}

  void
GlobalRouter::BuildNetworkLSAs (NetDeviceContainer c)
{
  NS_LOG_FUNCTION (&c);

  uint32_t nDesignatedRouters = c.GetN ();
      
  for (uint32_t i = 0; i < nDesignatedRouters; ++i)
    {
      //
      // Build one NetworkLSA for each net device talking to a network that we are the 
      // designated router for.  These devices are in the provided container.
      //
      Ptr<NetDevice> ndLocal = c.Get (i);
      Ptr<Node> node = ndLocal->GetNode ();

      uint32_t ifIndexLocal;
      bool rc = FindIfIndexForDevice(node, ndLocal, ifIndexLocal);
      NS_ABORT_MSG_IF (rc == false, "GlobalRouter::BuildNetworkLSAs (): No interface index associated with device");

      Ptr<Ipv4> ipv4Local = node->GetObject<Ipv4> ();
      NS_ABORT_MSG_UNLESS (ipv4Local, "GlobalRouter::ProcessPointToPointLink (): GetObject for <Ipv4> interface failed");

      Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
      Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);

      GlobalRoutingLSA *pLSA = new GlobalRoutingLSA;
      NS_ABORT_MSG_IF (pLSA == 0, "GlobalRouter::BuildNetworkLSAs(): Can't alloc link record");

      pLSA->SetLSType (GlobalRoutingLSA::NetworkLSA);
      pLSA->SetLinkStateId (addrLocal);
      pLSA->SetAdvertisingRouter (m_routerId);
      pLSA->SetNetworkLSANetworkMask (maskLocal);
      pLSA->SetStatus (GlobalRoutingLSA::LSA_SPF_NOT_EXPLORED);

      //
      // Build a list of AttachedRouters by walking the devices in the channel
      // and, if we find a node with a GlobalRouter interface and an IPv4 
      // interface associated with that device, we call it an attached router.  
      //
      Ptr<Channel> ch = ndLocal->GetChannel();
      uint32_t nDevices = ch->GetNDevices();
      NS_ASSERT (nDevices);
      
      for (uint32_t i = 0; i < nDevices; i++)
        {
          Ptr<NetDevice> tempNd = ch->GetDevice (i);
          NS_ASSERT (tempNd);
          Ptr<Node> tempNode = tempNd->GetNode ();

          //
          // Does the node in question have a GlobalRouter interface?  If not it can
          // hardly be considered an attached router.
          //
          Ptr<GlobalRouter> rtr = tempNode->GetObject<GlobalRouter> ();
          if (rtr == 0)
            { 
              continue;
            }

          //
          // Does the attached node have an ipv4 interface for the device we're probing?
          // If not, it can't play router.
          //
          uint32_t tempIfIndex;
          if (FindIfIndexForDevice (tempNode, tempNd, tempIfIndex))
            {
              Ptr<Ipv4> tempIpv4 = tempNode->GetObject<Ipv4> ();
              NS_ASSERT (tempIpv4);
              Ipv4Address tempAddr = tempIpv4->GetAddress(tempIfIndex);
              pLSA->AddAttachedRouter (tempAddr);
            }
        }
      m_LSAs.push_back (pLSA);
      pLSA = 0;
    }
}

//
// Given a local net device, we need to walk the channel to which the net device is
// attached and look for nodes with GlobalRouter interfaces on them (one of them 
// will be us).  Of these, the router with the lowest IP address on the net device 
// connecting to the channel becomes the designated router for the link.
//
  Ipv4Address
GlobalRouter::FindDesignatedRouterForLink (Ptr<NetDevice> ndLocal) const
{
  NS_LOG_FUNCTION_NOARGS ();

  Ptr<Channel> ch = ndLocal->GetChannel();
  uint32_t nDevices = ch->GetNDevices();
  NS_ASSERT (nDevices);

  Ipv4Address addr ("255.255.255.255");

  for (uint32_t i = 0; i < nDevices; i++)
    {
      Ptr<NetDevice> currentNd = ch->GetDevice (i);
      NS_ASSERT (currentNd);

      Ptr<Node> currentNode = currentNd->GetNode ();
      NS_ASSERT (currentNode);
      //
      // We require a designated router to have a GlobalRouter interface and
      // an internet stack that includes the Ipv4 interface.
      //
      Ptr<GlobalRouter> rtr = currentNode->GetObject<GlobalRouter> ();
      Ptr<Ipv4> ipv4 = currentNode->GetObject<Ipv4> ();
      if (rtr == 0 || ipv4 == 0 )
        {
          continue;
        }

      //
      // This node could be a designated router, so we can check and see if
      // it has a lower IP address than the one we have.  In order to have
      // an IP address, it needs to have an interface index.  If it doesen't
      // have an interface index directly, it's probably part of a bridge
      // net device XXX which is not yet handled.
      //
      uint32_t currentIfIndex;
      bool rc = FindIfIndexForDevice(currentNode, currentNd, currentIfIndex);
      if (rc == false)
        {
          continue;
        }

      //
      // Okay, get the IP address corresponding to the interface we're 
      // examining and if it's the lowest so far, remember it.
      //
      Ipv4Address currentAddr = ipv4->GetAddress(currentIfIndex);

      if (currentAddr < addr)
        {
          addr = currentAddr;
        }
    }

  //
  // Return the lowest IP address found, which will become the designated router
  // for the link.
  //
  NS_ASSERT_MSG (addr.IsBroadcast() == false, "GlobalRouter::FindDesignatedRouterForLink(): Bogus address");
  return addr;
}

//
// Given a node and an attached net device, take a look off in the channel to 
// which the net device is attached and look for a node on the other side
// that has a GlobalRouter interface aggregated.
//
  bool
GlobalRouter::AnotherRouterOnLink (Ptr<NetDevice> nd) const
{
  NS_LOG_FUNCTION_NOARGS ();

  Ptr<Channel> ch = nd->GetChannel();
  uint32_t nDevices = ch->GetNDevices();
  NS_ASSERT (nDevices);

  for (uint32_t i = 0; i < nDevices; i++)
    {
      Ptr<NetDevice> ndTemp = ch->GetDevice (i);
      NS_ASSERT (ndTemp);

      if (ndTemp == nd)
        {
          continue;
        }

      Ptr<Node> nodeTemp = ndTemp->GetNode ();
      NS_ASSERT (nodeTemp);

      Ptr<GlobalRouter> rtr = nodeTemp->GetObject<GlobalRouter> ();
      if (rtr)
        {
          return true;
        }
    }
  return false;
}

  uint32_t 
GlobalRouter::GetNumLSAs (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_LSAs.size ();
}

//
// Get the nth link state advertisement from this router.
//
  bool
GlobalRouter::GetLSA (uint32_t n, GlobalRoutingLSA &lsa) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG(lsa.IsEmpty(), "GlobalRouter::GetLSA (): Must pass empty LSA");
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
          GlobalRoutingLSA *p = *i;
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
GlobalRouter::GetAdjacent (Ptr<NetDevice> nd, Ptr<Channel> ch) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG(ch->GetNDevices() == 2, "GlobalRouter::GetAdjacent (): Channel with other than two devices");
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
        "GlobalRouter::GetAdjacent (): Wrong or confused channel?");
      return 0;
    }
}

//
// Given a node and a net device, find an IPV4 interface index that corresponds
// to that net device.  This function may fail for various reasons.  If a node
// does not have an internet stack (for example if it is a bridge) we won't have
// an IPv4 at all.  If the node does have a stack, but the net device in question
// is bridged, there will not be an interface associated directly with the device.
//
  bool
GlobalRouter::FindIfIndexForDevice (Ptr<Node> node, Ptr<NetDevice> nd, uint32_t &index) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC("For node " << node->GetId () << " for net device " << nd );

  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  if (ipv4 == 0)
    {
      NS_LOG_LOGIC ("**** No Ipv4 interface on node " << node->GetId ());
      return false;
    }

  for (uint32_t i = 0; i < ipv4->GetNInterfaces(); ++i )
    {
      if (ipv4->GetNetDevice(i) == nd) 
        {
          NS_LOG_LOGIC ("**** Device " << nd << " has associated ipv4 index " << i);
          index = i;
          return true;
        }
    }

  NS_LOG_LOGIC ("**** Device " << nd << " has no associated ipv4 index");
  return false;
}

//
// Decide whether or not a given net device is being bridged by a BridgeNetDevice.
//
  bool
GlobalRouter::IsNetDeviceBridged (Ptr<NetDevice> nd) const
{
  Ptr<Node> node = nd->GetNode ();
  uint32_t nDevices = node->GetNDevices();

  //
  // There is no bit on a net device that says it is being bridged, so we have
  // to look for bridges on the node to which the device is attached.  If we
  // find a bridge, we need to look through its bridge ports (the devices it
  // bridges) to see if we find the device in question.
  //
  for (uint32_t i = 0; i < nDevices; ++i)
    {
      Ptr<NetDevice> nd = node->GetDevice(i);

      if (nd->IsBridge ())
        {
          Ptr<BridgeNetDevice> bnd = nd->GetObject<BridgeNetDevice> ();
          NS_ABORT_MSG_UNLESS (bnd, "GlobalRouter::DiscoverLSAs (): GetObject for <BridgeNetDevice> failed");

          for (uint32_t j = 0; j < bnd->GetNBridgePorts (); ++j)
            {
              if (bnd->GetBridgePort (j) == nd)
                {
                  return true;
                }
            }
        }
    }
  return false;
}

} // namespace ns3
