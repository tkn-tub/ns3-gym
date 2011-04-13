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
 * Based on 
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 * 
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Buchatskaia <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */

#ifndef AODV_H
#define AODV_H

/**
 * \ingroup routing
 * \defgroup aodv AODV
 * 
 * This model implements the base specification of the Ad hoc on demand distance vector (AODV)
 * protocol. Implementation is based on RFC3561.
 *
 * Class aodv::RoutingProtocol implements all functionality of service packet exchange and inherits Ipv4RoutingProtocol. 
 * Base class defines two virtual functions for packet routing and forwarding.  The first one,
 * RouteOutput(), is used for locally originated packets, and the second one, RouteInput(), is used for forwarding
 * and/or delivering received packets.
 *
 * Protocol operation depends on the many adjustable parameters. Parameters for this functionality are attributes of
 * aodv::RoutingProtocol. We support parameters, with their default values, from RFC and parameters that enable/disable
 * protocol features, such as broadcasting HELLO messages, broadcasting data packets and so on.
 *
 * AODV discovers routes on demand. Therefore, our AODV model  buffer all packets, while a route request packet (RREQ)
 * is disseminated. We implement a packet queue in aodv-rqueue.cc. Smart pointer to packet, Ipv4RoutingProtocol::ErrorCallback,
 * Ipv4RoutingProtocol::UnicastForwardCallback and IP header are stored in this queue. The packet queue implements garbage collection of old
 * packets and a queue size limit.
 *
 * Routing table implementation support garbage collection of old entries and state machine, defined in standard.
 * It implements as a STL map container. The key is a destination IP address.
 *
 * Some moments of protocol operation aren't described in RFC. This moments generally concern cooperation of different OSI model layers.
 * We use following heuristics:
 * 
 * 1) This AODV implementation can detect the presence of unidirectional links and avoid them if necessary.
 *    If the node we received a RREQ for is a neighbor we are probably facing a unidirectional link... 
 *    This heuristic is taken from AODV-UU implementation and can be disabled.
 *    
 * 2) Protocol operation strongly depends on broken link detection mechanism. We implements two such heuristics.
 *    First, this implementation support  HELLO messages. However HELLO messages are not a good way to do neighbor sensing
 *    in a wireless environment (at least not over 802.11). Therefore, you may experience bad performance when running over wireless.
 *    There are several reasons for this:
 *      -# HELLO messages are broadcasted. In 802.11, broadcasting is done at a lower bit rate than unicasting,
 *         thus HELLO messages travel further than data.
 *      -# HELLO messages are small, thus less prone to bit errors than data transmissions.
 *      -# Broadcast transmissions are not guaranteed to be bidirectional, unlike unicast transmissions.
 *    Second, we use layer 2 feedback when possible. Link considered to be broken, if frame transmission results in a transmission 
 *    failure for all retries. This mechanism meant for active links and work much more faster, than first method.
 *    Layer 2 feedback implementation relies on TxErrHeader trace source, currently it is supported in AdhocWifiMac only.
 *    
 * 3) Duplicate packet detection. We use special class DuplicatePacketDetection for this purpose.
 *
 * Following optional protocol optimizations aren't implemented:
 *  - Expanding ring search.
 *  - Local link repair.
 *  - RREP, RREQ and HELLO message extensions.
 * This techniques require direct access to IP header, which contradict assertion from AODV RFC that AODV works over UDP. Our model use UDP
 * for simplicity, but this disable us to implement protocol optimizations. We don't use low layer raw socket, because they are not portable.
 *
 */

#endif /* AODV_H */
