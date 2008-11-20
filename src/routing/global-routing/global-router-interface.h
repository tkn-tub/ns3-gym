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
 * Authors:  Craig Dowell (craigdo@ee.washington.edu)
 *           Tom Henderson (tomhend@u.washington.edu)
 */

#ifndef GLOBAL_ROUTER_INTERFACE_H
#define GLOBAL_ROUTER_INTERFACE_H

#include <stdint.h>
#include <list>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/ipv4-address.h"
#include "ns3/net-device-container.h"
#include "ns3/bridge-net-device.h"
#include "ns3/global-route-manager.h"

namespace ns3 {

/**
 * @brief A single link record for a link state advertisement.
 *
 * The GlobalRoutingLinkRecord is modeled after the OSPF link record field of
 * a Link State Advertisement.  Right now we will only see two types of link
 * records corresponding to a stub network and a point-to-point link (channel).
 */
class GlobalRoutingLinkRecord
{
public:
/**
 * @enum LinkType
 * @brief Enumeration of the possible types of Global Routing Link Records.
 *
 * These values are defined in the OSPF spec.  We currently only use 
 * PointToPoint and StubNetwork types.
 */
  enum LinkType {
    Unknown = 0,        /**< Uninitialized Link Record */
    PointToPoint,       /**< Record representing a point to point channel */
    TransitNetwork,     /**< Unused -- for future OSPF compatibility  */
    StubNetwork,        /**< Record represents a leaf node network */
    VirtualLink         /**< Unused -- for future OSPF compatibility  */
  };

/**
 * @brief Construct an empty ("uninitialized") Global Routing Link Record.
 *
 * The Link ID and Link Data Ipv4 addresses are set to "0.0.0.0";
 * The Link Type is set to Unknown;
 * The metric is set to 0.
 */
  GlobalRoutingLinkRecord ();

/**
 * Construct an initialized Global Routing Link Record.
 *
 * @param linkType The type of link record to construct.
 * @param linkId The link ID for the record.
 * @param linkData The link data field for the record.
 * @param metric The metric field for the record.
 * @see LinkType
 * @see SetLinkId
 * @see SetLinkData
 */
  GlobalRoutingLinkRecord (
    LinkType    linkType, 
    Ipv4Address linkId, 
    Ipv4Address linkData, 
    uint16_t    metric);

/**
 * @brief Destroy a Global Routing Link Record.
 *
 * Currently does nothing.  Here as a placeholder only.
 */
  ~GlobalRoutingLinkRecord ();

/**
 * Get the Link ID field of the Global Routing Link Record.
 *
 * For an OSPF type 1 link (PointToPoint) the Link ID will be the Router ID
 * of the neighboring router.
 *
 * For an OSPF type 3 link (StubNetwork), the Link ID will be the adjacent
 * neighbor's IP address
 *
 * @returns The Ipv4Address corresponding to the Link ID field of the record.
 */
  Ipv4Address GetLinkId(void) const;

/**
 * @brief Set the Link ID field of the Global Routing Link Record.
 *
 * For an OSPF type 1 link (PointToPoint) the Link ID must be the Router ID
 * of the neighboring router.
 *
 * For an OSPF type 3 link (StubNetwork), the Link ID must be the adjacent
 * neighbor's IP address
 *
 * @param addr An Ipv4Address to store in the Link ID field of the record.
 */
  void SetLinkId(Ipv4Address addr);

/**
 * @brief Get the Link Data field of the Global Routing Link Record.
 *
 * For an OSPF type 1 link (PointToPoint) the Link Data will be the IP
 * address of the node of the local side of the link.
 *
 * For an OSPF type 3 link (StubNetwork), the Link Data will be the
 * network mask
 *
 * @returns The Ipv4Address corresponding to the Link Data field of the record.
 */
  Ipv4Address GetLinkData(void) const;

/**
 * @brief Set the Link Data field of the Global Routing Link Record.
 *
 * For an OSPF type 1 link (PointToPoint) the Link Data must be the IP
 * address of the node of the local side of the link.
 *
 * For an OSPF type 3 link (StubNetwork), the Link Data must be set to the
 * network mask
 *
 * @param addr An Ipv4Address to store in the Link Data field of the record.
 */
  void SetLinkData(Ipv4Address addr);

/**
 * @brief Get the Link Type field of the Global Routing Link Record.
 *
 * The Link Type describes the kind of link a given record represents.  The
 * values are defined by OSPF.
 *
 * @see LinkType
 * @returns The LinkType of the current Global Routing Link Record.
 */
  LinkType GetLinkType(void) const;

/**
 * @brief Set the Link Type field of the Global Routing Link Record.
 *
 * The Link Type describes the kind of link a given record represents.  The
 * values are defined by OSPF.
 *
 * @see LinkType
 * @param linkType The new LinkType for the current Global Routing Link Record.
 */
  void SetLinkType(LinkType linkType);

/**
 * @brief Get the Metric Data field of the Global Routing Link Record.
 *
 * The metric is an abstract cost associated with forwarding a packet across
 * a link.  A sum of metrics must have a well-defined meaning.  That is, you
 * shouldn't use bandwidth as a metric (how does the sum of the bandwidth of
 * two hops relate to the cost of sending a packet); rather you should use
 * something like delay.
 *
 * @returns The metric field of the Global Routing Link Record.
 */
  uint16_t GetMetric(void) const;

/**
 * @brief Set the Metric Data field of the Global Routing Link Record.
 *
 * The metric is an abstract cost associated with forwarding a packet across
 * a link.  A sum of metrics must have a well-defined meaning.  That is, you
 * shouldn't use bandwidth as a metric (how does the sum of the bandwidth of
 * two hops relate to the cost of sending a packet); rather you should use
 * something like delay.
 *
 * @param metric The new metric for the current Global Routing Link Record.
 */
  void SetMetric(uint16_t metric);

private:
/**
 * m_linkId and m_linkData are defined by OSPF to have different meanings 
 * depending on the type of link a given link records represents.  They work
 * together.
 *
 * For Type 1 link (PointToPoint), set m_linkId to Router ID of 
 * neighboring router.
 *
 * For Type 3 link (Stub), set m_linkId to neighbor's IP address
 */
  Ipv4Address m_linkId;         

/**
 * m_linkId and m_linkData are defined by OSPF to have different meanings 
 * depending on the type of link a given link records represents.  They work
 * together.
 *
 * For Type 1 link (PointToPoint), set m_linkData to local IP address  
 *
 * For Type 3 link (Stub), set m_linkData to mask
 */
  Ipv4Address m_linkData;    // for links to RouterLSA, 

/**
 * The type of the Global Routing Link Record.  Defined in the OSPF spec.  
 * We currently only use PointToPoint and StubNetwork types.
 */
  LinkType m_linkType;

/**
 * The metric for a given link.
 *
 * A metric is abstract cost associated with forwarding a packet across a 
 * link.  A sum of metrics must have a well-defined meaning.  That is, you 
 * shouldn't use bandwidth as a metric (how does the sum of the bandwidth 
 * of two hops relate to the cost of sending a packet); rather you should
 * use something like delay.
 */
  uint16_t m_metric;  
};

/**  
 * @brief a Link State Advertisement (LSA) for a router, used in global 
 * routing.
 * 
 * Roughly equivalent to a global incarnation of the OSPF link state header
 * combined with a list of Link Records.  Since it's global, there's
 * no need for age or sequence number.  See RFC 2328, Appendix A.
 */
class GlobalRoutingLSA
{
public:
/**
 * @enum LSType
 * @brief corresponds to LS type field of RFC 2328 OSPF LSA header
 */
  enum LSType {
    Unknown = 0,        /**< Uninitialized Type */
    RouterLSA,
    NetworkLSA,
    SummaryLSA,
    SummaryLSA_ASBR,
    ASExternalLSAs
  };
/**
 * @enum SPFStatus
 * @brief Enumeration of the possible values of the status flag in the Routing 
 * Link State Advertisements.
 */
  enum SPFStatus {
    LSA_SPF_NOT_EXPLORED = 0,	/**< New vertex not yet considered */
    LSA_SPF_CANDIDATE,		/**< Vertex is in the SPF candidate queue */
    LSA_SPF_IN_SPFTREE		/**< Vertex is in the SPF tree */
  };
/**
 * @brief Create a blank Global Routing Link State Advertisement.  
 *
 * On completion Ipv4Address variables initialized to 0.0.0.0 and the 
 * list of Link State Records is empty.
 */
  GlobalRoutingLSA();

/**
 * @brief Create an initialized Global Routing Link State Advertisement.  
 *
 * On completion the list of Link State Records is empty.
 *
 * @param status The status to of the new LSA.
 * @param linkStateId The Ipv4Address for the link state ID field.
 * @param advertisingRtr The Ipv4Address for the advertising router field.
 */
  GlobalRoutingLSA(SPFStatus status, Ipv4Address linkStateId, 
    Ipv4Address advertisingRtr);

/**
 * @brief Copy constructor for a Global Routing Link State Advertisement.
 *
 * Takes a piece of memory and constructs a semantically identical copy of
 * the given LSA.
 *
 * @param lsa The existing LSA to be used as the source.
 */
  GlobalRoutingLSA (GlobalRoutingLSA& lsa);

/**
 * @brief Destroy an existing Global Routing Link State Advertisement.
 *
 * Any Global Routing Link Records present in the list are freed.
 */
  ~GlobalRoutingLSA();

/**
 * @brief Assignment operator for a Global Routing Link State Advertisement.
 *
 * Takes an existing Global Routing Link State Advertisement and overwrites
 * it to make a semantically identical copy of a given prototype LSA.
 *
 * If there are any Global Routing Link Records present in the existing 
 * LSA, they are freed before the assignment happens.
 *
 * @param lsa The existing LSA to be used as the source.
 * @returns Reference to the overwritten LSA.
 */
  GlobalRoutingLSA& operator= (const GlobalRoutingLSA& lsa);

/**
 * @brief Copy any Global Routing Link Records in a given Global Routing Link
 * State Advertisement to the current LSA.  
 * 
 * Existing Link Records are not deleted -- this is a concatenation of Link 
 * Records.
 *
 * @see ClearLinkRecords ()
 * @param lsa The LSA to copy the Link Records from.
 */
  void CopyLinkRecords (const GlobalRoutingLSA& lsa);

/**
 * @brief Add a given Global Routing Link Record to the LSA.
 *
 * @param lr The Global Routing Link Record to be added.
 * @returns The number of link records in the list.
 */
  uint32_t AddLinkRecord (GlobalRoutingLinkRecord* lr);

/**
 * @brief Return the number of Global Routing Link Records in the LSA.
 *
 * @returns The number of link records in the list.
 */
  uint32_t GetNLinkRecords (void) const;

/**
 * @brief Return a pointer to the specified Global Routing Link Record.
 *
 * @param n The LSA number desired.
 * @returns The number of link records in the list.
 */
  GlobalRoutingLinkRecord* GetLinkRecord (uint32_t n) const;

/**
 * @brief Release all of the Global Routing Link Records present in the Global
 * Routing Link State Advertisement and make the list of link records empty.
 */
  void ClearLinkRecords(void);

/**
 * @brief Check to see if the list of Global Routing Link Records present in the
 * Global Routing Link State Advertisement is empty.
 *
 * @returns True if the list is empty, false otherwise.
 */
  bool IsEmpty(void) const;

/**
 * @brief Print the contents of the Global Routing Link State Advertisement and
 * any Global Routing Link Records present in the list.  Quite verbose.
 */
  void Print (std::ostream &os) const;

/**
 * @brief Return the LSType field of the LSA 
 */
  LSType GetLSType (void) const;
/**
 * @brief Set the LS type field of the LSA
 */
  void SetLSType (LSType typ);

/**
 * @brief Get the Link State ID as defined by the OSPF spec.  We always set it
 * to the router ID of the router making the advertisement.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @see GlobalRouting::GetRouterId ()
 * @returns The Ipv4Address stored as the link state ID.
 */
  Ipv4Address GetLinkStateId (void) const;

/**
 * @brief Set the Link State ID is defined by the OSPF spec.  We always set it
 * to the router ID of the router making the advertisement.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @see GlobalRouting::GetRouterId ()
 */
  void SetLinkStateId (Ipv4Address addr);

/**
 * @brief Get the Advertising Router as defined by the OSPF spec.  We always
 * set it to the router ID of the router making the advertisement.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @see GlobalRouting::GetRouterId ()
 * @returns The Ipv4Address stored as the advetising router.
 */
  Ipv4Address GetAdvertisingRouter (void) const;

/**
 * @brief Set the Advertising Router as defined by the OSPF spec.  We always
 * set it to the router ID of the router making the advertisement.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @see GlobalRouting::GetRouterId ()
 */
  void SetAdvertisingRouter (Ipv4Address  rtr);

/**
 * @brief For a Network LSA, set the Network Mask field that precedes
 * the list of attached routers.
 */
  void SetNetworkLSANetworkMask (Ipv4Mask mask);

/**
 * @brief For a Network LSA, get the Network Mask field that precedes
 * the list of attached routers.
 * 
 * @returns the NetworkLSANetworkMask 
 */
  Ipv4Mask GetNetworkLSANetworkMask (void) const;

/**
 * @brief Add an attached router to the list in the NetworkLSA
 *
 * @param addr The Ipv4Address of the interface on the network link
 * @returns The number of addresses in the list.
 */
  uint32_t AddAttachedRouter (Ipv4Address addr);

/**
 * @brief Return the number of attached routers listed in the NetworkLSA
 *
 * @returns The number of attached routers.
 */
  uint32_t GetNAttachedRouters (void) const;

/**
 * @brief Return an Ipv4Address corresponding to the specified attached router
 *
 * @param n The attached router number desired (number in the list).
 * @returns The Ipv4Address of the requested router
 */
  Ipv4Address GetAttachedRouter (uint32_t n) const;

/**
 * @brief Get the SPF status of the advertisement.
 *
 * @see SPFStatus
 * @returns The SPFStatus of the LSA.
 */
  SPFStatus GetStatus (void) const;

/**
 * @brief Set the SPF status of the advertisement
 *
 * @see SPFStatus
 */
  void SetStatus (SPFStatus status);

private:
/**
 * The type of the LSA.  Each LSA type has a separate advertisement
 * format.
 */
  LSType m_lsType;
/**
 * The Link State ID is defined by the OSPF spec.  We always set it to the
 * router ID of the router making the advertisement.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @see GlobalRouting::GetRouterId ()
 */
  Ipv4Address  m_linkStateId;

/**
 * The Advertising Router is defined by the OSPF spec.  We always set it to 
 * the router ID of the router making the advertisement.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @see GlobalRouting::GetRouterId ()
 */
  Ipv4Address  m_advertisingRtr;

/**
 * A convenience typedef to avoid too much writers cramp.
 */
  typedef std::list<GlobalRoutingLinkRecord*> ListOfLinkRecords_t;

/**
 * Each Link State Advertisement contains a number of Link Records that
 * describe the kinds of links that are attached to a given node.  We 
 * consider PointToPoint and StubNetwork links.
 *
 * m_linkRecords is an STL list container to hold the Link Records that have
 * been discovered and prepared for the advertisement.
 *
 * @see GlobalRouting::DiscoverLSAs ()
 */
  ListOfLinkRecords_t m_linkRecords;

/**
 * Each Network LSA contains the network mask of the attached network
 */
  Ipv4Mask m_networkLSANetworkMask;

/**
 * A convenience typedef to avoid too much writers cramp.
 */
  typedef std::list<Ipv4Address> ListOfAttachedRouters_t;

/**
 * Each Network LSA contains a list of attached routers
 *
 * m_attachedRouters is an STL list container to hold the addresses that have
 * been discovered and prepared for the advertisement.
 *
 * @see GlobalRouting::DiscoverLSAs ()
 */
  ListOfAttachedRouters_t m_attachedRouters;

/**
 * This is a tristate flag used internally in the SPF computation to mark
 * if an SPFVertex (a data structure representing a vertex in the SPF tree
 * -- a router) is new, is a candidate for a shortest path, or is in its
 * proper position in the tree.
 */
  SPFStatus m_status;
};

std::ostream& operator<< (std::ostream& os, GlobalRoutingLSA& lsa);

/**
 * @brief An interface aggregated to a node to provide global routing info
 *
 * An interface aggregated to a node that provides global routing information
 * to a global route manager.  The presence of the interface indicates that
 * the node is a router.  The interface is the mechanism by which the router
 * advertises its connections to neighboring routers.  We're basically 
 * allowing the route manager to query for link state advertisements.
 */
class GlobalRouter : public Object
{
public:
/**
 * @brief The Interface ID of the Global Router interface.
 *
 * @see Object::GetObject ()
 */
  static TypeId GetTypeId (void);

/**
 * @brief Create a Global Router class 
 */
  GlobalRouter ();

/**
 * @brief Get the Router ID associated with this Global Router.
 *
 * The Router IDs are allocated in the RoutingEnvironment -- one per Router, 
 * starting at 0.0.0.1 and incrementing with each instantiation of a router.
 *
 * @see RoutingEnvironment::AllocateRouterId ()
 * @returns The Router ID associated with the Global Router.
 */
  Ipv4Address GetRouterId (void) const;

/**
 * @brief Walk the connected channels, discover the adjacent routers and build
 * the associated number of Global Routing Link State Advertisements that 
 * this router can export.
 *
 * This is a fairly expensive operation in that every time it is called
 * the current list of LSAs is built by walking connected point-to-point
 * channels and peeking into adjacent IPV4 stacks to get address information.
 * This is done to allow for limited dymanics of the Global Routing 
 * environment.  By that we mean that you can discover new link state 
 * advertisements after a network topology change by calling DiscoverLSAs 
 * and then by reading those advertisements.
 *
 * @see GlobalRoutingLSA
 * @see GlobalRouter::GetLSA ()
 * @returns The number of Global Routing Link State Advertisements.
 */
  uint32_t DiscoverLSAs (void);

/**
 * @brief Get the Number of Global Routing Link State Advertisements that this
 * router can export.
 *
 * To get meaningful information you must have previously called DiscoverLSAs.
 * After you know how many LSAs are present in the router, you may call 
 * GetLSA () to retrieve the actual advertisement.
 *
 * @see GlobalRouterLSA
 * @see GlobalRouting::DiscoverLSAs ()
 * @see GlobalRouting::GetLSA ()
 * @returns The number of Global Routing Link State Advertisements.
 */
  uint32_t GetNumLSAs (void) const;

/**
 * @brief Get a Global Routing Link State Advertisements that this router has 
 * said that it can export.
 *
 * This is a fairly inexpensive expensive operation in that the hard work
 * was done in GetNumLSAs.  We just copy the indicated Global Routing Link
 * State Advertisement into the requested GlobalRoutingLSA object.
 *
 * You must call GlobalRouter::GetNumLSAs before calling this method in 
 * order to discover the adjacent routers and build the advertisements.
 * GetNumLSAs will return the number of LSAs this router advertises.  
 * The parameter n (requested LSA number) must be in the range 0 to 
 * GetNumLSAs() - 1.
 *
 * @see GlobalRoutingLSA
 * @see GlobalRouting::GetNumLSAs ()
 * @param n The index number of the LSA you want to read.
 * @param lsa The GlobalRoutingLSA class to receive the LSA information.
 * @returns The number of Global Router Link State Advertisements.
 */
  bool GetLSA (uint32_t n, GlobalRoutingLSA &lsa) const;

private:
  virtual ~GlobalRouter ();
  void ClearLSAs (void);

  Ptr<NetDevice> GetAdjacent(Ptr<NetDevice> nd, Ptr<Channel> ch) const;
  bool FindIfIndexForDevice(Ptr<Node> node, Ptr<NetDevice> nd, uint32_t &index) const;
  Ipv4Address FindDesignatedRouterForLink (Ptr<NetDevice> ndLocal) const;
  bool AnotherRouterOnLink (Ptr<NetDevice> nd, bool allowRecursion) const;
  void ProcessBroadcastLink (Ptr<NetDevice> nd, GlobalRoutingLSA *pLSA, NetDeviceContainer &c);
  void ProcessSingleBroadcastLink (Ptr<NetDevice> nd, GlobalRoutingLSA *pLSA, NetDeviceContainer &c);
  void ProcessBridgedBroadcastLink (Ptr<NetDevice> nd, GlobalRoutingLSA *pLSA, NetDeviceContainer &c);

  void ProcessPointToPointLink (Ptr<NetDevice> ndLocal, GlobalRoutingLSA *pLSA);
  void BuildNetworkLSAs (NetDeviceContainer c);
  Ptr<BridgeNetDevice> NetDeviceIsBridged (Ptr<NetDevice> nd) const;


  typedef std::list<GlobalRoutingLSA*> ListOfLSAs_t;
  ListOfLSAs_t m_LSAs;

  Ipv4Address m_routerId;

  // inherited from Object
  virtual void DoDispose (void);

/**
 * @brief Global Router copy construction is disallowed.
 */
  GlobalRouter (GlobalRouter& sr);

/**
 * @brief Global Router assignment operator is disallowed.
 */
  GlobalRouter& operator= (GlobalRouter& sr);
};

} // namespace ns3

#endif /* GLOBAL_ROUTER_INTERFACE_H */
