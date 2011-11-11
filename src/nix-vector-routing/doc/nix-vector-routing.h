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

/**
 * \defgroup nixvectorrouting Nix-Vector Routing
 *
 * \section model Model
 *
 * Nix-vector routing is a simulation specific routing protocol and is 
 * intended for large network topologies.  The on-demand nature of this 
 * protocol as well as the low-memory footprint of the nix-vector provides 
 * improved performance in terms of memory usage and simulation run time 
 * when dealing with a large number of nodes.
 *
 * Currently, the ns-3 model of nix-vector routing supports IPv4 p2p links 
 * as well as CSMA links.  It does not (yet) provide support for 
 * efficient adaptation to link failures.  It simply flushes all nix-vector 
 * routing caches. Finally, IPv6 is not supported.
 *
 * \section api API and Usage
 *
 * The Nix-vector routing protocol must be added to a list of routing 
 * protocols.  It is important that list routing is utilized.
 *
 *    Example:
 *
 *    Ipv4NixVectorHelper nixRouting;
 *    Ipv4StaticRoutingHelper staticRouting;
 *
 *    Ipv4ListRoutingHelper list;
 *    list.Add (staticRouting, 0);
 *    list.Add (nixRouting, 10);
 *
 *    InternetStackHelper stack;
 *    stack.SetRoutingHelper (list);
 *    stack.Install (allNodes);
 *
 * \section impl Implementation
 *
 * ns-3 nix-vector-routing performs on-demand route computation using 
 * a breadth-first search and an efficient route-storage data structure 
 * known as a nix-vector.  When a packet is generated at a node for 
 * transmission, the route is calculated, and the nix-vector is built.  
 * The nix-vector stores an index for each hop along the path, which 
 * corresponds to the neighbor-index.  This index is used to determine 
 * which net-device and gateway should be used.  To route a packet, the 
 * nix-vector must be transmitted with the packet. At each hop, the 
 * current node extracts the appropriate neighbor-index from the 
 * nix-vector and transmits the packet through the corresponding 
 * net-device.  This continues until the packet reaches the destination.
 * */
