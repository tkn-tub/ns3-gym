/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 *
 * This is toplevel mesh module description
 */

/**
 * \ingroup devices
 * \defgroup mesh Mesh
 *
 * \brief MAC-layer mobile mesh networking.
 * \section MeshOverview Overview of Layer-2 Mesh networking protocols.
 * The main goal of this module is to provide an NS-3 model of MAC-layer routing
 * protocols, which can be interfaced as plain network device by upper layer protocols.
 * Also our model supports stations with multiple network devices handled by a single
 * MAC-layer routing protocol, and this devices (called 'interfaces') are hidden from upper-layer protocols.
 * 
 * Current implementation provides 2 layers of MAC-layer routing stack.
 * The upper layer is Mesh point device layer - it is an interface to upper-layer protocols and it implements
 * routing itself. The main task of this layer is to aggregate multiple network devices working under the same MAC-layer
 * routing protocol, to send, receive, and forward packets.
 * The lower layer implements interaction with MAC-layer itself, because some routing protocols (like HWMP in IEEE802.11s)
 * require their own management frames and some of this management frames may be MAC-specific. So, the main task
 * of this layer is to handle MAC-specific management frames needed by routing protocols.
 * 
 * At the moment two routing protocols are implemented in this module:
 *      - HWMP (default routing protocol for IEEE802.11s standard) + Peer management protocol (also described in 802.11s
 *      draft standard), which is required by HWMP to manage peer links (works like association in IEEE802.11).
 *      - FLAME (Forwarding LAyer for MEshing).
 * The former protocol works only with 802.11-MAC, the latter protocol works with all network devices, which support
 * 48-bit MAC_addressing scheme.
 * \subsection Architecture Architecture of MAC-layer routing stack.
 * According to said above, MAC-layer routing stack consists of two layers:
 * The upper is a ns3::MeshPointDevice, which forwards packets using ns3::MeshL2RoutingProtocol, attached to it.
 * Interface between MeshPointDevice and upper layers inherited from ns3::NetDevice class. Interface between MeshPointDevice
 * and MeshL2RoutingProtocol is the following: MeshPointDevice gives a packet with source and destination addresses,
 * number of network device, from which it was received and callback to be executed when route is found.
 * All routing queues are implemented inside a protocol. After the routing discovery procedure is done, protocol returns a
 * packet back to mesh point device and gives an index of network device, where packet shall be sent to. All additional
 * routing information is stored inside a packet using a tags. After all this routine packet goes to the lower layer.
 * The lower layer is responsible for filling MAC-specific headers. At the moment we have implemented only lower layer
 * for WifiNetDevice - ns::MeshWifiInterfaceMac, and ns3::MeshWifiInterfaceMacPlugin. The former is a new kind of WifiMac, which
 * implements IEEE802.11S mesh interface, when beaconing is turned on, and a simple ad hoc, when the beaconing is turned off.
 * Also MeshWfifiInterfaceMac supports beacon shifting, yo avoid collisions in mesh network.
 * The latter is a lower part of L2Routing protocol. It handles all outgoing and all incoming frames, fills needed headers and
 * decides, whether to drop a frame or not and it can add needed information elements to a beacon.
 * See http://www.nsnam.org/wiki/index.php/Mesh for module architectural description.
 * \subsection NewProtocol Adding a new protocol.
 * This model requires, that all network devices shall operate with ns3::Mac48Address type of addresses.
 * To add a new protocol you need to define the following things:
 *      - Write a upper part of the protocol.
 *      - If Your protocol works only with 802.11 MAC - write a lower part, inherited from ns3::MeshWifiInterfaceMacPlugin
 *      - If Your protocol works with other type of Network devices - write your own lower part of protocol (interface of
 *      communication with other types of network devices is not implemented) if needed.
 * 
 * When you implement a protocol, you must remember, that when you are at MeshPointDevice level, you work with a packet
 * without LLC header, and when you are at plugin-level using ns3::MeshWifiInterfaceMacPlugun, the LLC header is already attached (by WifiNetDevice)
 * \attention Note, when you work with ns3::MeshWifiInterfaceMac, you have an ability to install multiple number of plugins. 
 * \subsection Dot11s Overview of 802.11s
 * Implementation of 802.11s draft standard consists of two main parts: Peer management protocol and HWMP - Hybrid Wireless Mesh Protocol.
 * The task of peer management protocol is to open links, detecting beacons, and close peer links due to transmission failures or beacon loss.
 * If peer link between sender and receiver does not exist, the packet shall be dropped.
 * \subsubsection IE Implementation of Information elements
 * To make a model of 802.11 stack, we need to implement a big quantity of information elements and have an ability to add them in a beacon.
 * Also we have to parse beacon and extract a proper information elements from it. So, two classes ns3::WifiInformationElement and ns3::WifiInformationElementVector
 * implement this functionality. The former keeps all information of information element and its elementId and has methods SerializeInformation and Deserialize information
 * ns3::WifiInformationElementVector works as a header and adds all stored information elements to a packet.
 * \subsubsection PMP Peer management protocol
 * Peer management protocol consists of three main parts:
 *      - Protocol itself ns3::dot11s::PeerManagementProtocol, which keeps all active peer links and handles all changes of their states
 *      and notifies a routing protocol about link failures.
 *      - its MAC plugin ns3::dot11s::PeerManagementProtocolMac which drops packet, if there is no peer link,
 *      and peek all needed information from management frames and information lements from beacons.
 *      - peer link instance ns3::dot11s::PeerLink which keeps finite state machine for each peer link, keeps beacon loss counter and counter of failed packets
 *      to be transmitted.
 *      
 * Procedure of closing peer link is not described detailed in 802.11s draft standard, so the link may be closed by:
 *      - Beacon loss (see a proper attribute of ns3::dot11s::PeerLink class)
 *      - by transmission failure - when a proper number of successive packets have failed to transmit, the link shall be closed.
 * Also Peer management protocol is responsible for beacon collision avoidance, because it keeps beacon timing elements from all neighbours.
 * Note, that PeerManagementProtocol is not attached to MeshPointDevice as a routing protocol, but the structure is the similar: the upper layer of protocol, and its plugins.
 * 
 * \subsubsection HWMP Hybrid Wireless Mesh Protocol
 * HWMP is implemented in both modes - reactive and proactive. Also we have implemented an ability to transmit broadcast data and management frames as unicasts (see proper attributes).
 * This feature turns on when you need to transmit a broadcast and the number of neighbours of the transmitting station is is more than threshold.
 * \subsection overview of FLAME
 * 
 * \subsection Statistics
 * Each routing protocol has a statistic structure, Report and ResetStats methods. This gives an opportunity to write statistic to an *.xml file periodically. As soon as you can reset statistics,
 * the period of report you can ajust as you need. 
 */
