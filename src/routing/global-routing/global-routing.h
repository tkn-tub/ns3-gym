/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2008 University of Washington
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
 */

/**
 * \ingroup routing
 * \defgroup globalrouting Global Routing
 *
 * \section model Model
 *
 * ns-3 global routing performs pre-simulation static route computation
 * on a layer-3 Ipv4 topology.  The user API from the script level is
 * fairly minimal; once a topology has been constructed and addresses
 * assigned, the user may call ns3::GlobalRouteManager::PopulateRoutingTables()
 * and the simulator will initialize the routing database and set up
 * static unicast forwarding tables for each node.
 *
 * The model assumes that all nodes on an ns-3 channel are reachable to
 * one another, regardless of whether the nodes can use the channel
 * successfully (in the case of wireless).  Therefore, this model
 * should typically be used only on wired topologies.  API does not
 * yet exist to control the subset of a topology to which this global
 * static routing is applied.
 * 
 * This model also does not yet deal with the possible presence of
 * layer-2 relays such as switches, bridges, and hubs, although ns-3 does
 * not have such devices yet.
 * 
 * \section api API and Usage
 * 
 * Users must include ns3/global-route-manager.h header file.  After the
 * IPv4 topology has been built and addresses assigned, users call
 * ns3::GlobalRouteManager::PopulateRoutingTables (), prior to the 
 * ns3::Simulator::Run() call.  There are no other attributes or
 * public methods that are typically called, or ways to parameterize
 * the behavior.
 *
 * \section impl Implementation
 *
 * A singleton object, ns3::GlobalRouteManager, builds a global routing
 * database of information about the topology, and executes a Dijkstra
 * Shortest Path First (SPF) algorithm on the topology for each node, and
 * stores the computed routes in each node's IPv4 forwarding table by
 * making use of the routing API in class ns3::Ipv4.
 *
 * The nodes that export data are those that have had an ns3::GlobalRouter
 * object aggregated to them.  The ns3::GlobalRouter can be thought of
 * as a per-node agent that exports topology information to the 
 * ns3::GlobalRouteManager.  When it comes time to build the global
 * routing database, the list of nodes is iterated and each node with
 * an ns3::GlobalRouter object is asked to export routing information
 * concerning the links to which it is attached.
 *
 * The format of the data exported conforms to the OSPFv2 standard
 * (http://www.ietf.org/rfc/rfc2328.txt).  In particular, the
 * information is exported in the form of ns3::GlobalLSA objects that
 * semantically match the Link State Advertisements of OSPF.  
 *
 * By using a standard data format for reporting topology, existing 
 * OSPF route computation code can be reused, and that is what is done
 * by the ns3::GlobalRouteManager.  The main computation functions are
 * ported from the quagga routing suite (http://www.quagga.net).
 * 
 */
