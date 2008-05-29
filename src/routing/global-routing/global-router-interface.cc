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
#include "ns3/channel.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/ipv4.h"
#include "global-router-interface.h"

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
  NS_ASSERT_MSG(false, 
    "GlobalRoutingLSA::GetAttachedRouter (): invalid index");
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
// Go out and discover any adjacent routers and build the Link State 
// Advertisements that reflect them and their associated networks.
// 
  uint32_t 
GlobalRouter::DiscoverLSAs (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Node> node = GetObject<Node> ();
  NS_LOG_LOGIC("For node " << node->GetId () );
  NS_ASSERT_MSG(node, 
    "GlobalRouter::DiscoverLSAs (): <Node> interface not set");

  ClearLSAs ();

// While building the router-LSA, keep a list of those NetDevices for
// which I am the designated router and need to later build a NetworkLSA
  std::list<Ptr<NetDevice> > listOfDRInterfaces;

//
// We're aggregated to a node.  We need to ask the node for a pointer to its
// Ipv4 interface.  This is where the information regarding the attached 
// interfaces lives.
//
  Ptr<Ipv4> ipv4Local = node->GetObject<Ipv4> ();
  NS_ASSERT_MSG(ipv4Local, 
    "GlobalRouter::DiscoverLSAs (): QI for <Ipv4> interface failed");
//
// Each node originates a Router-LSA
//
  GlobalRoutingLSA *pLSA = new GlobalRoutingLSA;
  pLSA->SetLSType (GlobalRoutingLSA::RouterLSA);
  pLSA->SetLinkStateId (m_routerId);
  pLSA->SetAdvertisingRouter (m_routerId);
  pLSA->SetStatus (GlobalRoutingLSA::LSA_SPF_NOT_EXPLORED);
//
// We need to ask the node for the number of net devices attached. This isn't
// necessarily equal to the number of links to adjacent nodes (other routers)
// as the number of devices may include those for stub networks (e.g., 
// ethernets, etc.).  
//
  uint32_t numDevices = node->GetNDevices();
  NS_LOG_LOGIC ("numDevices = " << numDevices);
  for (uint32_t i = 0; i < numDevices; ++i)
    {
      Ptr<NetDevice> ndLocal = node->GetDevice(i);

      // Check if it is an IP interface (could be a pure L2 NetDevice)
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
          continue;
        }

      if (ndLocal->IsBroadcast () && !ndLocal->IsPointToPoint () )
        {
          NS_LOG_LOGIC ("Broadcast link");
          GlobalRoutingLinkRecord *plr = new GlobalRoutingLinkRecord;
//
// We need to determine whether we are on a transit or stub network
// If we find at least one more router on this channel, we are a transit
//
//
// Now, we have to find the Ipv4 interface whose netdevice is the one we 
// just found.  This is still the IP on the local side of the channel.  There 
// is a function to do this used down in the guts of the stack, but it's not 
// exported so we had to whip up an equivalent.
//
          uint32_t ifIndexLocal = FindIfIndexForDevice(node, ndLocal);
          Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
          Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
          NS_LOG_LOGIC ("Working with local address " << addrLocal);
          uint16_t metricLocal = ipv4Local->GetMetric (ifIndexLocal);
//
// Now, we're going to walk over to the remote net device on the other end of 
// the point-to-point channel we now know we have.  This is where our adjacent 
// router (to use OSPF lingo) is running.  
//
          Ptr<Channel> ch = ndLocal->GetChannel();
          uint32_t nDevices = ch->GetNDevices();
          if (nDevices == 1)
            {
              // This is a stub broadcast interface
              NS_LOG_LOGIC("Router-LSA stub broadcast link");
              // XXX in future, need to consider if >1 includes other routers
              plr->SetLinkType (GlobalRoutingLinkRecord::StubNetwork);
              // Link ID is IP network number of attached network
              plr->SetLinkId (addrLocal.CombineMask(maskLocal));
              // Link Data is network mask; convert to Ipv4Address
              Ipv4Address maskLocalAddr;
              maskLocalAddr.Set(maskLocal.Get ());
              plr->SetLinkData (maskLocalAddr);
              plr->SetMetric (metricLocal);
              pLSA->AddLinkRecord(plr);
              plr = 0;
              continue;
            }
          else
            {
              NS_LOG_LOGIC ("Router-LSA Broadcast link");
              // multiple routers on a broadcast interface
              // lowest IP address is designated router
              plr->SetLinkType (GlobalRoutingLinkRecord::TransitNetwork);
              // Link ID is IP interface address of designated router
              Ipv4Address desigRtr = 
                FindDesignatedRouterForLink (node, ndLocal);
              if (desigRtr == addrLocal) 
                {
                  listOfDRInterfaces.push_back (ndLocal);
                  NS_LOG_LOGIC (node->GetId () << " is a DR");
                }
              plr->SetLinkId (desigRtr);
              // Link Data is router's own IP address
              plr->SetLinkData (addrLocal);
              plr->SetMetric (metricLocal);
              pLSA->AddLinkRecord (plr);
              plr = 0;
              continue;
            }
        }
      else if (ndLocal->IsPointToPoint () )
        {
          NS_LOG_LOGIC ("Router-LSA Point-to-point device");
//
// Now, we have to find the Ipv4 interface whose netdevice is the one we 
// just found.  This is still the IP on the local side of the channel.  There 
// is a function to do this used down in the guts of the stack, but it's not 
// exported so we had to whip up an equivalent.
//
          uint32_t ifIndexLocal = FindIfIndexForDevice(node, ndLocal);
//
// Now that we have the Ipv4 interface index, we can get the address and mask
// we need.
//
          Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
          Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);
          NS_LOG_LOGIC ("Working with local address " << addrLocal);
          uint16_t metricLocal = ipv4Local->GetMetric (ifIndexLocal);
//
// Now, we're going to walk over to the remote net device on the other end of 
// the point-to-point channel we now know we have.  This is where our adjacent 
// router (to use OSPF lingo) is running.  
//
          Ptr<Channel> ch = ndLocal->GetChannel();
          if (ch == NULL)
            {
              continue;
            }
          Ptr<NetDevice> ndRemote = GetAdjacent(ndLocal, ch);
//
// The adjacent net device is aggregated to a node.  We need to ask that net 
// device for its node, then ask that node for its Ipv4 interface.
//
          Ptr<Node> nodeRemote = ndRemote->GetNode();
          Ptr<Ipv4> ipv4Remote = nodeRemote->GetObject<Ipv4> ();
          NS_ASSERT_MSG(ipv4Remote, 
            "GlobalRouter::DiscoverLSAs (): QI for remote <Ipv4> failed");
//
// Per the OSPF spec, we're going to need the remote router ID, so we might as
// well get it now.
//
          Ptr<GlobalRouter> srRemote = 
            nodeRemote->GetObject<GlobalRouter> ();
          NS_ASSERT_MSG(srRemote, 
            "GlobalRouter::DiscoverLSAs():QI for remote <GlobalRouter> failed");
          Ipv4Address rtrIdRemote = srRemote->GetRouterId();
          NS_LOG_LOGIC ("Working with remote router " << rtrIdRemote);
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
          NS_LOG_LOGIC ("Working with remote address " << addrRemote);
//
// Now we can fill out the link records for this link.  There are always two
// link records; the first is a point-to-point record describing the link and
// the second is a stub network record with the network number.
//
          GlobalRoutingLinkRecord *plr = new GlobalRoutingLinkRecord;
          plr->SetLinkType (GlobalRoutingLinkRecord::PointToPoint);
          plr->SetLinkId (rtrIdRemote);
          plr->SetLinkData (addrLocal);
          plr->SetMetric (metricLocal);
          pLSA->AddLinkRecord (plr);
          plr = 0;

          plr = new GlobalRoutingLinkRecord;
          plr->SetLinkType (GlobalRoutingLinkRecord::StubNetwork);
          plr->SetLinkId (addrRemote);
          plr->SetLinkData (Ipv4Address(maskRemote.Get()));  // Frown
          plr->SetMetric (metricLocal);
          pLSA->AddLinkRecord (plr);
          plr = 0;
        }
      else
        {
          NS_ASSERT_MSG(0, "GlobalRouter::DiscoverLSAs (): unknown link type");
        }

    }
//
// The LSA goes on a list of LSAs in case we want to begin exporting other
// kinds of advertisements (than Router LSAs).
  m_LSAs.push_back (pLSA);
  NS_LOG_LOGIC (*pLSA);

// Now, determine whether we need to build a NetworkLSA
  if (listOfDRInterfaces.size () > 0)
    {
      for (std::list<Ptr<NetDevice> >::iterator i = listOfDRInterfaces.begin ();
        i != listOfDRInterfaces.end (); i++)
        {
// Build one NetworkLSA for each interface that is a DR
          Ptr<NetDevice> ndLocal = *i;
          uint32_t ifIndexLocal = FindIfIndexForDevice(node, ndLocal);
          Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
          Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);

          GlobalRoutingLSA *pLSA = new GlobalRoutingLSA;
          pLSA->SetLSType (GlobalRoutingLSA::NetworkLSA);
          pLSA->SetLinkStateId (addrLocal);
          pLSA->SetAdvertisingRouter (m_routerId);
          pLSA->SetNetworkLSANetworkMask (maskLocal);
          pLSA->SetStatus (GlobalRoutingLSA::LSA_SPF_NOT_EXPLORED);
// Build list of AttachedRouters
          Ptr<Channel> ch = ndLocal->GetChannel();
          uint32_t nDevices = ch->GetNDevices();
          NS_ASSERT (nDevices);
          for (uint32_t i = 0; i < nDevices; i++)
            {
              Ptr<NetDevice> tempNd = ch->GetDevice (i);
              NS_ASSERT (tempNd);
              Ptr<Node> tempNode = tempNd->GetNode ();
              uint32_t tempIfIndex = FindIfIndexForDevice (tempNode, tempNd);
              Ptr<Ipv4> tempIpv4 = tempNode->GetObject<Ipv4> ();
              NS_ASSERT (tempIpv4);
              Ipv4Address tempAddr = tempIpv4->GetAddress(tempIfIndex);
              pLSA->AddAttachedRouter (tempAddr);
            }
          m_LSAs.push_back (pLSA);
          NS_LOG_LOGIC (*pLSA);
        }
    }

  return m_LSAs.size ();
}

  Ipv4Address
GlobalRouter::FindDesignatedRouterForLink (Ptr<Node> node, 
  Ptr<NetDevice> ndLocal) const
{
  uint32_t ifIndexLocal = FindIfIndexForDevice(node, ndLocal);
  Ptr<Ipv4> ipv4Local = GetObject<Ipv4> ();
  NS_ASSERT (ipv4Local);
  Ipv4Address addrLocal = ipv4Local->GetAddress(ifIndexLocal);
  Ipv4Mask maskLocal = ipv4Local->GetNetworkMask(ifIndexLocal);

  Ptr<Channel> ch = ndLocal->GetChannel();
  uint32_t nDevices = ch->GetNDevices();
  NS_ASSERT (nDevices);
  Ipv4Address lowest = addrLocal;
  // iterate all NetDevices and return the lowest numbered IP address 
  for (uint32_t i = 0; i < nDevices; i++)
    {
      Ptr<NetDevice> tempNd = ch->GetDevice (i);
      NS_ASSERT (tempNd);
      Ptr<Node> tempNode = tempNd->GetNode ();
      uint32_t tempIfIndex = FindIfIndexForDevice (tempNode, tempNd);
      Ptr<Ipv4> tempIpv4 = tempNode->GetObject<Ipv4> ();
      NS_ASSERT (tempIpv4);
      Ipv4Address tempAddr = tempIpv4->GetAddress(tempIfIndex);
      if (tempAddr < addrLocal)
        {
          addrLocal = tempAddr;
        }
    }
  return addrLocal;
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
GlobalRouter::GetAdjacent(Ptr<NetDevice> nd, Ptr<Channel> ch) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG(ch->GetNDevices() == 2, 
    "GlobalRouter::GetAdjacent (): Channel with other than two devices");
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
// Given a node and a net device, find the IPV4 interface index that 
// corresponds to that net device.
//
  uint32_t
GlobalRouter::FindIfIndexForDevice(Ptr<Node> node, Ptr<NetDevice> nd) const
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
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
