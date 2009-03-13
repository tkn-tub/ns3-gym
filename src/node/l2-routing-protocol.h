/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef L2_ROUTING_PROTOCOL_H
#define L2_ROUTING_PROTOCOL_H

#include <list>
#include <vector>
#include <ostream>
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include "ns3/net-device.h"
#include "ns3/ptr.h"

namespace ns3 {

class Packet;
/**
 * \ingroup mesh
 *
 * \brief Base class for Layer 2 routing protocols
 *
 * \details Yhis class is an internal part of L2RoutingNetDevice class
 * and this class implements interface between L2RoutingNetDevice and
 * any protocol of MAC layer. This interface is similar to ipv4
 * routiong protocol base class.
 */
class L2RoutingProtocol : public Object
{
	public:
		static TypeId GetTypeId();
		virtual ~L2RoutingProtocol();
		/**
		 * \brief Callback to be invoked when route discovery
		 * procedure is completed
		 * \param bool flag indicating whether a route was
		 * actually found and all needed information is added
		 * to the packet succesfully
		 * \param Ptr<Packet> is packet, for which the route
		 * was resolved
		 * \attention All needed information for MAC layer
		 * must be stored in proper tags (like in case of
		 * HWMP, when WifiMacHeader needs address of next
		 * hop), or must be added as a packet header(if MAC
		 * does not need any additional information). So, the
		 * packet is returned back to L2RoutingNetDevice looks
		 * like a pure packet with ethernet header (i.e data
		 * +src +dst + protocol). All information needed to
		 * L2RoutingNetDevice is outgoing port ID.
		 * \param Mac48Address Source address of the packet
		 * \param Mac48Address Destiation address of the
		 * packet
		 * \param uint16_t Ethernet 'Protocol' field - stored
		 * in protocol, needed to form a proper MAC-layer
		 * header.
		 * \param uint32_t Output port
		 */
typedef Callback<
	void,
	bool,
	Ptr<Packet>,
	Mac48Address,
	Mac48Address,
	uint16_t,
	uint32_t
		>
		RouteReplyCallback;
		/**
		 * \brief All packets must go through RequestRoute
		 * \returns Can the route be discovered or not
		 * \param sourceIface The incoming interface of the
		 * packet
		 * \param source Source address of the packet
		 * \param destination Destination address of the
		 * packet
		 * \param packet The packet to be resolved (needed the
		 * whole packet, because we can add tags or headers).
		 * \param protocolType The protocol ID (must be stored
		 * for route discovery time to form a proper MAC-layer
		 * header).
		 * \param routeReply Callback to be invoked after route
		 * discovery procedure.
		 */
		virtual bool RequestRoute(
				uint32_t		sourceIface,
				const Mac48Address	source,
				const Mac48Address	destination,
				Ptr<Packet>		packet,
				uint16_t		protocolType,
				RouteReplyCallback	routeReply
				) = 0;
		/**
		 * \brief Attaches ports to the L2RoutingProtocols,
		 * \returns False if there are not proper devices (like
		 * HWMP, which requires only WifiNetDevice and works
		 * only above MeshWifiMac).
		 * \param ports The array of ports.
		 */
		virtual bool AttachPorts(std::vector<Ptr<NetDevice> > ports) = 0;
		/**
		 * \brief L2Routing protocol must know the
		 * L2RoutingNetDevice ID to determine packets as 'came
		 * from upper layer' and 'received from network' (for
		 * example, to manage TTL).
		 * \param interface The interface ID of
		 * L2RoutingNetDevice
		 */
		virtual void SetIfIndex(uint32_t interface) = 0;
	protected:
		struct QueuedPacket {
			Ptr<Packet>	pkt;
			Mac48Address	dst;
			Mac48Address	src;
			uint16_t	protocol;
			uint32_t	inPort;
			RouteReplyCallback reply;
		};
		/**
		 * \brief Set Max queue size per destinztion
		 * \details Routing Queue is implemented inside the
		 * routing protocol and keeps one queue per
		 * destination (to make it easier to find resolved and
		 * timed out packets).
		 * \param maxPacketsPerDestination Packets per
		 * destination that can be stored inside protocol.
		 */
		virtual void SetMaxQueueSize(int maxPacketsPerDestination) = 0;
		/**
		 * \brief Queue packet with 'Ethernet header'
		 * \returns false if the queue is full.
		 */
		virtual bool QueuePacket(struct QueuedPacket packet) = 0;
		/**
		 * \brief Deques packet with 'Ethernet header'
		 * \returns Ptr<packet> (NULL if queue is empty), src,
		 * dst, protocol ID, incoming port ID, and reply
		 * callback
		 * \param destination The destination address, which
		 * identifyes queue.
		 */
		virtual struct QueuedPacket DequeuePacket(Mac48Address destination) = 0;
};
}//namespace ns3
#endif
