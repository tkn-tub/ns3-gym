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


#ifndef L2ROUTING_NET_DEVICE_H
#define L2ROUTING_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"
#include "ns3/bridge-net-device.h"
#include "ns3/bridge-channel.h"
#include "ns3/l2-routing-protocol.h"

namespace ns3 {
	class Node;
	/**
	 * \ingroup devices
	 * \defgroup L2RoutingNetDevice L2routingNetDevice
	 */
	/**
	 * \ingroup L2RoutingNetDevice
	 * \brief a virtual net device that may forward packets
	 * between real network devices using routing protocols of
	 * MAC-layer
	 * \details This is a virtual netdevice, which aggreagates
	 * real netdevices and uses interface of L2RoutingProtocol to
	 * forward packets
	 * \attention The idea of L2RoutingNetDevice is similar to
	 * BridgeNetDevice, but the packets, which going through
	 * L2RoutingNetDevice may be changed (because routing protocol
	 * may require its own headers or tags). 
	 */
	class L2RoutingNetDevice : public NetDevice
	{
		public:
			static TypeId GetTypeId ();
			L2RoutingNetDevice ();
			virtual ~L2RoutingNetDevice ();
			/**
			 * \brief Attaches a 'port' to a virtual
			 * L2RoutingNetDevice, and this port is handled
			 * by L2RoutingProtocol.
			 * \attention Like in a bridge,
			 * L2RoutingNetDevice's ports must not have IP
			 * addresses, and only L2RoutingNetDevice
			 * itself may have an IP address.
			 *
			 * \attention L2RoutingNetDevice may be a port
			 * of BridgeNetDevice.
			 */
			void AddPort (Ptr<NetDevice> port);
			/**
			 * \returns number of ports attached to
			 * L2RoutingNetDevice
			 */
			uint32_t GetNPorts () const;
			/**
			 * \returns a pointer to netdevice
			 * \param n is device ID to be returned
			 */
			Ptr<NetDevice> GetPort (uint32_t n) const;
			//inherited from netdevice:
			virtual void SetName(const std::string name);
			virtual std::string GetName() const;
			virtual void SetIfIndex(const uint32_t index);
			virtual uint32_t GetIfIndex() const;
			virtual Ptr<Channel> GetChannel () const;
			virtual Address GetAddress () const;
			virtual bool SetMtu (const uint16_t mtu);
			virtual uint16_t GetMtu () const;
			virtual bool IsLinkUp () const;
			virtual void SetLinkChangeCallback (Callback<void> callback);
			virtual bool IsBroadcast () const;
			virtual Address GetBroadcast () const;
			virtual bool IsMulticast () const;
			virtual Address GetMulticast (Ipv4Address multicastGroup) const;
			virtual bool IsPointToPoint () const;
			virtual bool IsBridge () const;
			virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
			virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
			virtual Ptr<Node> GetNode () const;
			virtual void SetNode (Ptr<Node> node);
			virtual bool NeedsArp () const;
			virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
			virtual void SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb);
			virtual bool SupportsSendFrom () const;
			virtual Address GetMulticast (Ipv6Address addr) const;
			virtual void DoDispose ();
			/**
			 * \brief Attaches protocol to a given virtual
			 * device
			 * \returns true if success
			 */
			virtual bool AttachProtocol(Ptr<L2RoutingProtocol> protocol);
		protected:
			/**
			 * \brief This is similar to BridgeNetDevice
			 * method
			 */
			void ReceiveFromDevice (Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
					Address const &source, Address const &destination, PacketType packetType);
			/**
			 * \brief This is similar to BridgeNetDevice
			 * method
			 */
			void Forward (Ptr<NetDevice> incomingPort, Ptr<Packet> packet,
					uint16_t protocol, const Mac48Address src, const Mac48Address dst);
			/**
			 * \brief This is a function, which should be
			 * passed to L2RoutingProtocol as response
			 * callback (see L2RoutingProtocol).
			 */
			virtual void ProtocolResponse(
					bool success,
					Ptr<Packet>,
					Mac48Address src,
					Mac48Address dst,
					uint16_t protocol,
					uint32_t outPort
					);
		private:
			NetDevice::ReceiveCallback
					m_rxCallback;
			NetDevice::PromiscReceiveCallback
					m_promiscRxCallback;
			Mac48Address	m_address;
			Ptr<Node>	m_node;
			std::string	m_name;
			std::vector< Ptr<NetDevice> >
					m_ports;
			uint32_t	m_ifIndex;
			uint16_t	m_mtu;
			Ptr<BridgeChannel>
					m_channel;
			Callback<
				bool,
				uint32_t,
				Mac48Address,
				Mac48Address,
				Ptr<Packet>,
				uint16_t,
				L2RoutingProtocol::RouteReplyCallback>
					m_requestRoute;
			//What we give to L2Routing
			L2RoutingProtocol::RouteReplyCallback
					m_myResponse;
	};
} //namespace ns3
#endif
