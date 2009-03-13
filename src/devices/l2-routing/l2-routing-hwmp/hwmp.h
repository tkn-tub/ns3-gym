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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#ifndef HWMP_H
#define HWMP_H
#include <map>
#include <queue>
#include "ns3/tag.h"
#include "ns3/object.h"
#include "ns3/mac48-address.h"
#include "ns3/mac48-address-comparator.h"
#include "ns3/l2-routing-protocol.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "hwmp-state.h"
namespace ns3 {
	class HwmpState;
	/**
	 * \brief Hwmp tag implements interaction between HWMP
	 * protocol and MeshWifiMac
	 * \details Hwmp tag keeps the following:
	 * 1. When packet is passed from Hwmp to 11sMAC:
	 * 	- retransmitter address,
	 * 	- TTL value,
	 * 2. When packet is passed to Hwmp from 11sMAC:
	 * 	- lasthop address,
	 * 	- TTL value,
	 * 	- metric value (metric of link is recalculated
	 * 	at each packet, but routing table stores metric
	 * 	obtained during path discovery procedure)
	 */
	class HwmpTag : public Tag
	{
		public:
			HwmpTag();
			~HwmpTag();
			void		SetAddress(Mac48Address retransmitter);
			Mac48Address	GetAddress();
			void		SetTtl(uint8_t ttl);
			uint8_t	GetTtl();
			void		SetMetric(uint32_t metric);
			uint32_t	GetMetric();
			void		SetSeqno(uint32_t seqno);
			uint32_t	GetSeqno();
			void		DecrementTtl();

			static TypeId		GetTypeId (void);
			virtual TypeId		GetInstanceTypeId (void) const;
			virtual uint32_t	GetSerializedSize (void) const;
			virtual void		Serialize (TagBuffer i) const;
			virtual void		Deserialize (TagBuffer i);
			virtual void		Print (std::ostream &os) const;
		private:
			Mac48Address	m_address;
			uint8_t		m_ttl;
			uint32_t	m_metric;
			uint32_t	m_seqno;
	};
	class Hwmp : public L2RoutingProtocol
	{
		public:
			static TypeId GetTypeId();
			Hwmp();
			~Hwmp();
			void	DoDispose();
			//intheritedfrom L2RoutingProtocol
			/**
			 * \brief L2Routing protocol base class metod
			 *
			 * \details Route resolving procedure consits
			 * of the following steps:
			 * 1. Find reactive route and send a packet.
			 * 2. Find all ports which are operate in
			 * 'Proactive' mode and find a proper default
			 * routes. and send packet to all proactive
			 * 'ports'.
			 * 3. If there are ports which are operate in
			 * reactive mode - queue packet and start
			 * route reactive path discovery
			 *
			 * \bug Now packet is sent to the 'best root'
			 * rather than to the best root at each port
			 */
			bool	RequestRoute(
					uint32_t		sourceIface,
					const Mac48Address	source,
					const Mac48Address	destination,
					Ptr<Packet>		packet,
					uint16_t		protocolType,
					L2RoutingProtocol::RouteReplyCallback
					routeReply
					);
			bool	AttachPorts(std::vector<Ptr<NetDevice> >);
			void	SetIfIndex(uint32_t interface);
			/**
			 * \brief Disables port by index.
			 * \details Needed for external modules like
			 * clusterization.
			 */
			void	DisablePort(uint32_t port);
			/**
			 * \brief enables port
			 */
			void	EnablePort(uint32_t port);
			/**
			 * \brief Setting proative mode.
			 * \details To enable proactive mode you must
			 * set a root node. Choosing the route is made
			 * in accordance with the following: 
			 * 	1. Find 'reactive' route. If route is
			 * 	found - send a packet using this
			 * 	information
			 * 	2. If there is no reactive route -
			 * 	find proactive or 'default' route.
			 * \attention Comparsion between proactive and
			 * reactive routes is incorrect, because we
			 * have metric to root MP in te first case and
			 * metric to the destination in the second
			 * case.
			 * \param port is the port where proactive
			 * mode should be activated
			 */
			void	SetRoot(uint32_t port);
			/**
			 * \brief Disable root functionality at a
			 * given port
			 */
			void	UnSetRoot(uint32_t port);
			/**
			 * \brief HwmpState retrns to Hwmp class all
			 * routing information obtained from all HWMP
			 * action frames
			 */
			void	ObtainRoutingInformation(
					HwmpState::INFO info
					);
			/**
			 * \brief Hwmp state noyifyes that neighbour
			 * is dissapeared. Hwmp state knows about peer
			 * failure from MAC
			 */
			void	PeerFailure(Mac48Address peerAddress);
			void	SetMaxTtl(uint8_t ttl);
			uint8_t
				GetMaxTtl();
		private:
			static const uint32_t MAX_SEQNO = 0xffffffff;
			//candidate queue is implemented inside the
			//protocol:
			void		SetMaxQueueSize(int maxPacketsPerDestination);
			int		m_maxQueueSize;
			bool		QueuePacket(L2RoutingProtocol::QueuedPacket packet);
			L2RoutingProtocol::QueuedPacket
					DequeuePacket(Mac48Address dst);
			void		SendAllPossiblePackets(Mac48Address dst);
			std::map<Mac48Address, std::queue<QueuedPacket> > 
					m_rqueue;
			//devices and HWMP states:
			enum DeviceState {
				ENABLED,
				DISABLED
			};
			enum DeviceMode {
				REACTIVE,
				PROACTIVE,
				ROOT
			};
			std::vector<enum DeviceState>
					m_states;
			std::vector<enum DeviceMode>
					m_modes;
			std::vector<Ptr<HwmpState> >
					m_hwmpStates;
			//Routing table:
			Ptr<HwmpRtable>
					m_rtable;
			//Proactive routines:
			/**
			 * \brief Set port state as proactive.
			 * \details mode is supposed to be proactive
			 * when proatcive PREQ with a valid
			 * information was received.
			 */
			void		SetProactive(uint32_t port);
			/**
			 * \brief Checks if the port is root, if true
			 * - no default routes must be used at this
			 * port
			 */
			bool		IsRoot(uint32_t port);
			/**
			 * \brief Interaction with HwmpState class -
			 * request for starting routing discover
			 * procedure (reactive route discovery!)
			 * \param Mac48Address is destination to be
			 * resolved
			 */
			std::vector< Callback<void, Mac48Address> >
					m_requestCallback;
			/**
			 * \brief Callback that shall be executed when
			 * need to send Path error
			 * \param std::vector<Mac48Address> is the
			 * list of unreachable destinations
			 * \param std::vector<Mac48Address> is
			 * receivers of PERR
			 */
			std::vector<Callback<void,std::vector<HwmpRtable::FailedDestination> > >
					m_pathErrorCallback;
			void	StartPathErrorProcedure(
					std::vector<HwmpRtable::FailedDestination> destinations,
					uint32_t port);
			/**
			 * \brief HwmpState need to know where to
			 * retransmit PERR, only HWMP knows how to
			 * retransmit it (broadcast/unicast) and only
			 * HWMP has accessto routing table
			 */
			std::vector<Mac48Address>
					GetRetransmittersForFailedDestinations(
							std::vector<HwmpRtable::FailedDestination> failedDest,
							uint32_t port);
			/**
			 * \brief Needed by HwmpState to find routes in case
			 * of intermediate reply and choosing the
			 * better route
			 * 
			 */
			HwmpRtable::LookupResult
					RequestRouteForAddress(const Mac48Address& destination);
			HwmpRtable::LookupResult
					RequestRootPathForPort(uint32_t port);

			/**
			 * \brief interface which is the HWMP attached to
			 * (Virtual netdevice)
			 */
			uint32_t	m_interface;
			/**
			 * \attention mesh seqno is processed at HWMP
			 */
			uint32_t	m_seqno;
			std::map<Mac48Address, uint32_t, mac48addrComparator>
					m_seqnoDatabase;
			//Timers:
			/**
			 * /brief checks when last preq was initiated, returns
			 * false when retry has not expired
			 */
			bool		ShouldSendPreq(Mac48Address dst);
			/**
			 * \brief Generates PREQ retry when route is
			 * still unresolved after first PREQ
			 * If number of retries greater than
			 * dot11sParameters::dot11MeshHWMPmaxPREQretries,
			 * we return all packets to upper layers
			 */
			void		RetryPathDiscovery(Mac48Address dst, uint8_t numOfRetry);
			/**
			 * Keeps PREQ retry timers for every
			 * destination
			 */
			std::map<Mac48Address, EventId, mac48addrComparator>
					m_timeoutDatabase;
			/**
			 * Configurable parameters:
			 */
			uint8_t		m_maxTtl;
			bool		m_broadcastPerr;
	};
} //namespace ns3
#endif
